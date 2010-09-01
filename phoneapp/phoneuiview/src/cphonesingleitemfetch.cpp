/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Fetch items from Contacts
*
*/


#include <eikenv.h>
#include <cphcntsingleitemfetch.h>
#include <cphcntcontactid.h>
#include <PhCltUtils.h>
#include <eikcmobs.h>
#include <AknQueryDialog.h>

#include "cphoneviewcontroller.h"
#include "cphonecontactcontroller.h"
#include "cphonesingleitemfetch.h"
#include "tphonecommandparam.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamsingleitemfetch.h"
#include "cphonestatuspane.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "phonelogger.h"
#include "cphoneview.h"


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave
// ---------------------------------------------------------------------------
//
CPhoneSingleItemFetch::CPhoneSingleItemFetch(
    CEikonEnv& aEikEnv,
    CPhoneViewController& aViewController,
    CPhoneContactController& aContactController,
    CPhoneStatusPane& aStatusPane )
    : CActive ( EPriorityStandard ),
    iEikEnv( aEikEnv ),
    iViewController( aViewController ),
    iContactController( aContactController ),
    iStatusPane( aStatusPane )
    {
    CActiveScheduler::Add( this );
    iViewController.PhoneView()->SetPhoneViewObserver( *this );
    }


// ---------------------------------------------------------------------------
// CPhoneSingleItemFetch::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CPhoneSingleItemFetch* CPhoneSingleItemFetch::NewL(
    CEikonEnv& aEikEnv,
    CPhoneViewController& aViewController,
    CPhoneContactController& aContactController,
    CPhoneStatusPane& aStatusPane )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneSingleItemFetch::NewL() ");
    CPhoneSingleItemFetch* self =
        new( ELeave ) CPhoneSingleItemFetch( aEikEnv,
                                             aViewController,
                                             aContactController,
                                             aStatusPane );
    return self;
    }


// ---------------------------------------------------------
// CPhoneSingleItemFetch::RunL
// ---------------------------------------------------------
//
void CPhoneSingleItemFetch::RunL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneSingleItemFetch::RunL()" );

    iButtonId = EPhoneViewYesSingleItemFetch;

    // First open the single item fetch dialog
    HBufC* fetchContent = HBufC::NewLC( KPhoneNumberEntryBufferSize );
    fetchContent->Des().Zero();
    TPtr ptr( fetchContent->Des() );
    TPhoneCmdParamSingleItemFetch singleItemFetchParam;

    if ( iResourceId == EPhoneNewCallFetchTitle )
        {
        singleItemFetchParam.SetType( CPhCntSingleItemFetch::EFetchNewCall );
        singleItemFetchParam.SetTitlePaneResourceId(
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneNewCallFetchTitle ) );
        singleItemFetchParam.SetCbaResourceId(
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneNewCallFetchCBA ) );
        singleItemFetchParam.SetString( &ptr );
        }
    else if ( iResourceId > EPhoneRssBaseLast )
        {
        singleItemFetchParam.SetType( CPhCntSingleItemFetch::EFetchNewPSCall );
        singleItemFetchParam.SetTitlePaneResourceId(
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneNewCallFetchTitle ) );
        singleItemFetchParam.SetCbaResourceId(
        CPhoneMainResourceResolver::Instance()->
           ResolveResourceID( EPhoneNewCallFetchCBA ) );
        singleItemFetchParam.SetString( &ptr );
        }
    else // EPhoneDtmfFetchTitle & EPhoneDtmfNumberQuery
        {
        singleItemFetchParam.SetType( CPhCntSingleItemFetch::EFetchDtmf );
        singleItemFetchParam.SetTitlePaneResourceId(
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneDtmfFetchTitle ) );
        singleItemFetchParam.SetCbaResourceId(
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( KPhoneDefaultFetchCBA ) );
        singleItemFetchParam.SetString( &ptr );
        }

    // Change title pane contents
    CBase* textTitlePane = iStatusPane.CreateTextTitlePaneLC(
    singleItemFetchParam.TitlePaneResourceId() );

    CPhCntSingleItemFetch::TFetchParams result;
    result.iType = singleItemFetchParam.Type();
    result.iString = singleItemFetchParam.String();
    result.iCbaResource = singleItemFetchParam.CbaResourceId();
    result.iNumberType = MPhCntMatch::ENone;
    result.iContactId = NULL;

    TInt retval = KErrNotFound;
    if( iSingleItemFetch )
        {
        retval = iSingleItemFetch->FetchLD( result );
        iFetchPerformed = ETrue;
        iSingleItemFetch = NULL; // destructed in FetchLD
        delete result.iContactId; // pointer which will leak otherwise
        result.iContactId = NULL;
        }

    // The contact operation may be cancelled if the END key
    // is pressed (res == KErrCancel).
    // If so, the blocking dialog will be removed.
    if ( !iViewController.BlockingDialogIsDisplayed() || ptr == KNullDesC )
        {
        // Indicate that the operation has been cancelled
        iButtonId = EPhoneViewNoSingleItemFetch;
        static_cast<MEikCommandObserver*>( iEikEnv.EikAppUi() )
            ->ProcessCommandL( iButtonId );
        iFetchPerformed = EFalse;
        }
    else if ( retval == KErrNone )
        {
        if ( CPhCntSingleItemFetch::EFetchNewPSCall !=
                singleItemFetchParam.Type() )
            {
            // Remove garbage from buffer.
            PhCltUtils::RemoveInvalidChars(
                *( singleItemFetchParam.String() ) );
            }
        iViewController.SetFetchContent( ptr );
        }

    // restore title pane
    CleanupStack::PopAndDestroy( textTitlePane );
    CleanupStack::PopAndDestroy( fetchContent );
    // Reset the flag
    // ETrue if a blocking dialog is currently being displayed.
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( EFalse );
    iViewController.ExecuteCommandL(
        EPhoneViewSetBlockingDialogStatus,
        &booleanParam );
  }

// ---------------------------------------------------------
// CPhoneSingleItemFetch::RunError
// ---------------------------------------------------------
//
TInt CPhoneSingleItemFetch::RunError( TInt /*aError */ )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneSingleItemFetch::RunError( ) ");
    Delete();
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( EFalse );
    TRAP_IGNORE( iViewController.ExecuteCommandL(
        EPhoneViewSetBlockingDialogStatus, &booleanParam ))
    return KErrNone;
    }

// ---------------------------------------------------------
// CPhoneSingleItemFetch::DoCancel
// ---------------------------------------------------------
//
void CPhoneSingleItemFetch::DoCancel()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneSingleItemFetch::DoCancel()" );
    Delete();
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhoneSingleItemFetch::~CPhoneSingleItemFetch()
    {
    __LOGMETHODSTARTEND( EPhoneUIView,
        "CPhoneSingleItemFetch::~CPhoneSingleItemFetch()" );
    Delete();
    }


// ---------------------------------------------------------
// CPhoneSingleItemFetch::OpenSingleItemFetchDialogL
// ---------------------------------------------------------
//
void CPhoneSingleItemFetch::OpenSingleItemFetchDialogL(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView,
        "CPhoneSingleItemFetch::OpenSingleItemFetchDialogL()" );
   if( !IsActive() )
        {
        TPhoneCmdParamInteger* resourceId 
            = static_cast<TPhoneCmdParamInteger*>( aCommandParam );
        iResourceId = resourceId->Integer();
    
        // Create the contact dialog
        iSingleItemFetch = iContactController.CreateSingleItemFetchL();
        
        iViewController.SetSingleItemFetchType( iResourceId );
        
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }

// ---------------------------------------------------------
// CPhoneSingleItemFetch::Delete
// ---------------------------------------------------------
//
void CPhoneSingleItemFetch::Delete()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneSingleItemFetch::Delete()" );
    if ( iSingleItemFetch )
        {
        iViewController.SetSingleItemFetchType( NULL );
        
        // Cancel request (FetchLD) by destroying the instance.
        delete iSingleItemFetch;
        iSingleItemFetch = NULL;
        }
    }

// ---------------------------------------------------------
// CPhoneSingleItemFetch::PhoneViewActivatedL
// ---------------------------------------------------------
//
void CPhoneSingleItemFetch::PhoneViewActivatedL()
    {
    // Phone view is fully activated, now provide selection to states.
    if ( iFetchPerformed )
        {
        // reset flag
        iFetchPerformed = EFalse;
        static_cast<MEikCommandObserver*>( iEikEnv.EikAppUi() )
            ->ProcessCommandL( iButtonId );
        }
    }
