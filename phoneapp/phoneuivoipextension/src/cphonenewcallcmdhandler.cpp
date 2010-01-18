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
* Description: Implementation of CPhoneNewCallCmdHandler class.
*
*/

#include <featmgr.h>

#include "cphonenewcallcmdhandler.h"
#include "cphonelogger.h"
#include "mphonestatemachine.h"
#include "phonerssvoip.h"
#include "phoneappvoipcommands.hrh"
#include "cphonestate.h"
#include "cphonestateincall.h"
#include "tphonecmdparamcustomdialer.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamquery.h"
#include "tphonecmdparamglobalnote.h"
#include "cphonemainresourceresolver.h"
#include "cphonestatemachinevoip.h"
#include "cphonestateutilsvoip.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPhoneNewCallCmdHandler::CPhoneNewCallCmdHandler
// C++ constructor.
// ---------------------------------------------------------------------------
//
CPhoneNewCallCmdHandler::CPhoneNewCallCmdHandler( 
        MPhoneStateMachine& aStateMachine,
        MPhoneViewCommandHandle& aViewCommandHandle )
    : iStateMachine( aStateMachine ), iViewCommandHandle( aViewCommandHandle )
    {

    }


// ---------------------------------------------------------------------------
// CPhoneNewCallCmdHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CPhoneNewCallCmdHandler::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CPhoneNewCallCmdHandler::NewL
// ---------------------------------------------------------------------------
//
CPhoneNewCallCmdHandler* CPhoneNewCallCmdHandler::NewL(
        MPhoneStateMachine& aStateMachine,
        MPhoneViewCommandHandle& aViewCommandHandle )
    {
    CPhoneNewCallCmdHandler* self = 
        CPhoneNewCallCmdHandler::NewLC( 
            aStateMachine, aViewCommandHandle );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CPhoneNewCallCmdHandler::NewLC
// ---------------------------------------------------------------------------
//
CPhoneNewCallCmdHandler* CPhoneNewCallCmdHandler::NewLC(
        MPhoneStateMachine& aStateMachine,
        MPhoneViewCommandHandle& aViewCommandHandle )
    {
    CPhoneNewCallCmdHandler* self = 
        new( ELeave ) CPhoneNewCallCmdHandler( 
            aStateMachine, aViewCommandHandle );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPhoneNewCallCmdHandler::~CPhoneNewCallCmdHandler
// Destructor.
// ---------------------------------------------------------------------------
//
CPhoneNewCallCmdHandler::~CPhoneNewCallCmdHandler()
    {
    }


// ---------------------------------------------------------------------------
// CPhoneNewCallCmdHandler::HandleCommandL
// ---------------------------------------------------------------------------
//
void CPhoneNewCallCmdHandler::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
            "CPhoneNewCallCmdHandler::HandleCommandL" )
    
    switch ( aCommand )
        {
        case EPhoneInCallCmdNewInternetCall:
            ShowNewCallQueryL();
            break;
        
        case EPhoneCmdNewInternetCallOk:
            DoNewCallL();     
            break;
        
        case EPhoneCmdNewInternetCallSearch:
            OpenSingleItemContactFetchL( 
                EPhoneCmdNewInternetCallContactFetch ); 
            break;
        
        case EPhoneCmdNewInternetCallContactFetch:
            {
            TBuf<KPhoneNumberEntryBufferSize> fetchContent;    
            fetchContent = iViewCommandHandle.FetchContent();
               
            StateUtils().SelectServiceAndDialL( fetchContent );
            break;
            }
        
        default:
            __PHONELOG( EOnlyFatal, PhoneUIVoIPExtension, 
                "CPhoneNewCallCmdHandler::HandleCommandL, DEFAULT" );
            ASSERT( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// CPhoneNewCallCmdHandler::ShowNewCallQueryL
// ---------------------------------------------------------------------------
//
void CPhoneNewCallCmdHandler::ShowNewCallQueryL()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneNewCallCmdHandler::ShowNewCallQueryL" )
    
    if ( !FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) )
        {
        CPhoneState* phoneState = 
            static_cast<CPhoneState*>( iStateMachine.State() );
        
        HBufC *text = HBufC::NewLC( KPhoneNumberEntryBufferSize );
        TPtr ptr( text->Des() );
        // Pre-populate the query with the number entry contents, if it exists
        if ( phoneState->IsNumberEntryUsedL() )
            {
            // get the number entry contents
            TPhoneCmdParamString stringParam;
            stringParam.SetString( &ptr );
            iViewCommandHandle.ExecuteCommandL(
                EPhoneViewGetLocalizedNumberFromEntry,
                &stringParam );
            }
        
        TPhoneCmdParamQuery queryDialogParam;
        queryDialogParam.SetQueryType( EPhoneTextQueryDialog );
        queryDialogParam.SetQueryResourceId( 
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneVoIPNewCallQuery ) );
        
        queryDialogParam.SetDefaultCba( CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneVoIPNewCallQueryEmptySoftkeys ) );
        
        queryDialogParam.SetContentCba( CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneVoIPNewCallQueryNotEmptySoftkeys ) );
        
        queryDialogParam.SetDataText( &ptr );
        queryDialogParam.SetSendKeyEnabled( ETrue );
        
        // Display dialog        
        iViewCommandHandle.ExecuteCommandL( 
            EPhoneViewShowQuery, &queryDialogParam );
        
        CleanupStack::PopAndDestroy( text );
        }
    }


// ---------------------------------------------------------------------------
// CPhoneNewCallCmdHandler::DoNewCallL
// ---------------------------------------------------------------------------
//
void CPhoneNewCallCmdHandler::DoNewCallL()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneNewCallCmdHandler::DoNewCallL" )
    
    // First get the string from dialog
    TPhoneCmdParamString string;
    HBufC* content = HBufC::NewLC( KPEPhoneNumberMaxLength );
    TPtr ptr( content->Des() );
    string.SetString( &ptr );   

    iViewCommandHandle.ExecuteCommandL( 
        EPhoneViewGetTextQueryContent, &string );

    // Dial new voip call
    StateUtils().SelectServiceAndDialL( ptr );
    
    CleanupStack::PopAndDestroy( content );
    }


// ---------------------------------------------------------------------------
// CPhoneNewCallCmdHandler::OpenSingleItemContactFetchL
// ---------------------------------------------------------------------------
//
void CPhoneNewCallCmdHandler::OpenSingleItemContactFetchL( 
        TInt aFetchType )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
       "CPhoneNewCallCmdHandler::OpenSingleItemContactFetchL" )
    
    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( aFetchType );
    
    iViewCommandHandle.HandleCommandL( 
        EPhoneViewOpenSingleItemFetchDialog, 
        &integerParam );
    }


// -----------------------------------------------------------
// CPhoneNewCallCmdHandler::StateUtils
// -----------------------------------------------------------
//
CPhoneStateUtilsVoip& CPhoneNewCallCmdHandler::StateUtils()
    {
    return static_cast<CPhoneStateMachineVoIP&>( 
        iStateMachine ).StateUtils();
    }
     
// End of File
