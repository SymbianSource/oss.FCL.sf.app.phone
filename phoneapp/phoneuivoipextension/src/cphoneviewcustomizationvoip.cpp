/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: VoIP-specific view customization implementation.
*
*/

#include <cconvergedserviceselector.h>
#include "cphoneviewcustomizationvoip.h"
#include "mphoneviewblockingdialogobserver.h"
#include "phonestatedefinitionsvoip.h"
#include "tphonecmdparamselector.h"
#include "tphonecommandparam.h"
#include "cphonelogger.h"
#include "mphonecustomization.h"
#include "phoneappvoipcommands.hrh"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPhoneViewCustomizationVoip::CPhoneViewCustomizationVoip
// C++ constructor.
// ---------------------------------------------------------------------------
//
CPhoneViewCustomizationVoip::CPhoneViewCustomizationVoip(
        MPhoneCustomization& aCustomization ) :
    iPhoneCustomization( aCustomization )
    {

    }


// ---------------------------------------------------------------------------
// CPhoneViewCustomizationVoip::ConstructL
// ---------------------------------------------------------------------------
//
void CPhoneViewCustomizationVoip::ConstructL()
    {
    iServiceSelector = CConvergedServiceSelector::NewL();
    }


// ---------------------------------------------------------------------------
// CPhoneViewCustomizationVoip::NewL
// ---------------------------------------------------------------------------
//
CPhoneViewCustomizationVoip* CPhoneViewCustomizationVoip::NewL(
        MPhoneCustomization& aCustomization )
    {
    CPhoneViewCustomizationVoip* self = CPhoneViewCustomizationVoip::NewLC(
        aCustomization );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CPhoneViewCustomizationVoip::NewLC
// ---------------------------------------------------------------------------
//
CPhoneViewCustomizationVoip* CPhoneViewCustomizationVoip::NewLC(
        MPhoneCustomization& aCustomization )
    {
    CPhoneViewCustomizationVoip* self = 
        new( ELeave ) CPhoneViewCustomizationVoip( aCustomization );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPhoneViewCustomizationVoip::~CPhoneViewCustomizationVoip
// Destructor.
// ---------------------------------------------------------------------------
//
CPhoneViewCustomizationVoip::~CPhoneViewCustomizationVoip()
    {
    delete iServiceSelector;
    }


// ---------------------------------------------------------------------------
// From class MPhoneViewCustomization.
// CPhoneViewCustomizationVoip::HandleCommandL
// ---------------------------------------------------------------------------
//
TBool CPhoneViewCustomizationVoip::HandleCommandL(
        MPhoneViewBlockingDialogObserver& aBlockingObserver,
        TPhoneViewCommandId aCmdId,
        TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneViewCustomizationVoip::HandleCommandL" );
    
    TBool isCommandHandled( EFalse );
    
    switch ( aCmdId )
        {
        case EPhoneViewServiceSelection:
            {
            isCommandHandled = SelectVoipService( 
                aBlockingObserver, aCommandParam );
            }
            break;
        case EPhoneViewOpenCallHandling:
            {
            // Close transfer dialer
            iPhoneCustomization.HandleCommandL( 
                EPhoneCmdTransferDialerExit );
            isCommandHandled = ETrue;
            }
            break;
        default:
            break;
        }
    
    return isCommandHandled;
    }


// ---------------------------------------------------------------------------
// From class MPhoneViewCustomization.
// CPhoneViewCustomizationVoip::RemoveDialog
// ---------------------------------------------------------------------------
//
void CPhoneViewCustomizationVoip::RemoveDialog()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneViewCustomizationVoip::RemoveDialog" );
    
    // Dismiss service selector queries, if necessary
    iServiceSelector->CancelSelection();
    }


// ---------------------------------------------------------------------------
// CPhoneViewCustomizationVoip::SelectVoipService
// ---------------------------------------------------------------------------
//
TBool CPhoneViewCustomizationVoip::SelectVoipService(
        MPhoneViewBlockingDialogObserver& aBlockingObserver,
        TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneViewCustomizationVoip::SelectVoipService" );
    
    // Indicate that the service selection is blocking key events 
    // from the Phone.
    aBlockingObserver.SetBlockingDialogIsDisplayed( ETrue );
    
    CConvergedServiceSelector::TSsResult results;
    TInt selectionResult = iServiceSelector->GetCallingServiceByCallType( 
        results, CConvergedServiceSelector::ESsVoipCall, 0, EFalse );
    
    if ( KErrNone == selectionResult )
        {
        TPhoneCmdParamSelector* selectorContent =
            static_cast<TPhoneCmdParamSelector*>( aCommandParam );
        selectorContent->SetRegStatus( results.iServiceEnabled );
        selectorContent->SetServiceId( results.iServiceId );
        }
    
    // Reset the flag
    aBlockingObserver.SetBlockingDialogIsDisplayed( EFalse );
    
    return ( KErrNone == selectionResult );
    }
