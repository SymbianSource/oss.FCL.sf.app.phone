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
* Description: Implementation of CPhoneUnattendedTransferCmdHandler class.
*
*/

#include <featmgr.h>
#include <mpeengineinfo.h>
#include <StringLoader.h>

#include "cphoneunattendedtransfercmdhandler.h"

#include "cphonelogger.h"
#include "mphonestatemachine.h"
#include "cphonetransferdialercontroller.h"
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
// CPhoneUnattendedTransferCmdHandler::CPhoneUnattendedTransferCmdHandler
// C++ constructor.
// ---------------------------------------------------------------------------
//
CPhoneUnattendedTransferCmdHandler::CPhoneUnattendedTransferCmdHandler( 
        MPhoneStateMachine& aStateMachine,
        MPhoneViewCommandHandle& aViewCommandHandle )
    : iStateMachine( aStateMachine ), iViewCommandHandle( aViewCommandHandle )
    {

    }


// ---------------------------------------------------------------------------
// CPhoneUnattendedTransferCmdHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CPhoneUnattendedTransferCmdHandler::ConstructL()
    {
    iTransferDialerController = CPhoneTransferDialerController::NewL();
    }


// ---------------------------------------------------------------------------
// CPhoneUnattendedTransferCmdHandler::NewL
// ---------------------------------------------------------------------------
//
CPhoneUnattendedTransferCmdHandler* CPhoneUnattendedTransferCmdHandler::NewL(
        MPhoneStateMachine& aStateMachine,
        MPhoneViewCommandHandle& aViewCommandHandle )
    {
    CPhoneUnattendedTransferCmdHandler* self = 
        CPhoneUnattendedTransferCmdHandler::NewLC( 
            aStateMachine, aViewCommandHandle );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CPhoneUnattendedTransferCmdHandler::NewLC
// ---------------------------------------------------------------------------
//
CPhoneUnattendedTransferCmdHandler* CPhoneUnattendedTransferCmdHandler::NewLC(
        MPhoneStateMachine& aStateMachine,
        MPhoneViewCommandHandle& aViewCommandHandle )
    {
    CPhoneUnattendedTransferCmdHandler* self = 
        new( ELeave ) CPhoneUnattendedTransferCmdHandler( 
            aStateMachine, aViewCommandHandle );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPhoneUnattendedTransferCmdHandler::~CPhoneUnattendedTransferCmdHandler
// Destructor.
// ---------------------------------------------------------------------------
//
CPhoneUnattendedTransferCmdHandler::~CPhoneUnattendedTransferCmdHandler()
    {
    delete iTransferDialerController;
    }


// ---------------------------------------------------------------------------
// CPhoneUnattendedTransferCmdHandler::HandleCommandL
// ---------------------------------------------------------------------------
//
void CPhoneUnattendedTransferCmdHandler::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EPhoneInCallCmdUnattendedTransfer:
            ShowTransferDialerL();               
            break;
        
        case EPhoneCmdTransferDialerOk:
            DoUnattendedTransferL();     
            break;
        
        case EPhoneCmdTransferDialerExit:
            CloseTransferDialerL();
            break;
        
        case EPhoneCmdTransferDialerSearch:
            OpenSingleItemContactFetchL( 
                EPhoneCmdTransferDialerContactFetch ); 
            break;
        
        case EPhoneCmdTransferDialerContactFetch:
            {
            TBuf<KPhoneNumberEntryBufferSize> fetchContent;    
            fetchContent = iViewCommandHandle.FetchContent();
            
            // Set transfer to address to the phoneengine
            iStateMachine.PhoneEngineInfo()->
                SetTransferTargetCommand( fetchContent );
            iStateMachine.SendPhoneEngineMessage( 
                MPEPhoneModel::EPEMessageDoUnattendedTransfer );
            
            if ( KErrNone == iStateMachine.PhoneEngineInfo()->
                    ErrorInfo().iErrorCode )
                {
                ShowWaitingAcceptanceNoteL();
                }
            }
            break;
        case EPhoneCmdUnattendedTransferCallBackOk:
            {
            MPEEngineInfo* info = iStateMachine.PhoneEngineInfo();
            const TDesC& callBackToAddr = info->CallBackAddress();
            // Service id used for last call is used to call back
            TUint32 serviceId = info->ServiceIdCommand();
            static_cast<CPhoneStateMachineVoIP&>( 
                iStateMachine ).StateUtils().SelectServiceAndDialL( 
                    callBackToAddr, serviceId );
            }
            break;
        case EPhoneCmdUnattendedTransferCallBackCancel:
            {
            MPEEngineInfo* info = iStateMachine.PhoneEngineInfo();
            info->SetCallBackAddress( KNullDesC );
            }
            break;
        default:
            __PHONELOG( EOnlyFatal, PhoneUIVoIPExtension, 
                "CPhoneUnattendedTransferCmdHandler::HandleCommandL, DEFAULT" );    
            ASSERT( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// CPhoneUnattendedTransferCmdHandler::ShowTransferDialerL
// ---------------------------------------------------------------------------
//
void CPhoneUnattendedTransferCmdHandler::ShowTransferDialerL()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneUnattendedTransferCmdHandler::ShowTransferDialerL" )
    
    CPhoneState* phoneState = static_cast<CPhoneState*>( iStateMachine.State() );
    if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) )
        {
        TPhoneCmdParamCustomDialer customDialerParam;
        customDialerParam.SetDialerController( iTransferDialerController );
           
        // Set dialer to transfer mode.
        iViewCommandHandle.ExecuteCommandL( 
            EPhoneViewShowCustomizedDialer,
            &customDialerParam );
        
        // Open transfer dialer
        iViewCommandHandle.ExecuteCommandL( EPhoneViewCreateNumberEntry );
        
        // Update CBA to transfer dialer CBA
        TPhoneCmdParamInteger resourceId;
        resourceId.SetInteger( CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( iTransferDialerController->CbaResourceId() ) );
        iViewCommandHandle.ExecuteCommandL( EPhoneViewUpdateCba,
            &resourceId );
        }
    else
        {
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
        queryDialogParam.SetQueryType( EPhoneGenericTextQuery );
        queryDialogParam.SetQueryResourceId( 
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneVoIPTransferAddressQuery ) );
        
        queryDialogParam.SetDefaultCba( CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneVoIPTransferAddressQueryEmptySoftkeys ) );
        
        queryDialogParam.SetContentCba( CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneVoIPTransferAddressQueryNotEmptySoftkeys ) );
        
        queryDialogParam.SetDataText( &ptr );
        queryDialogParam.SetSendKeyEnabled( ETrue );
        
        // Display dialog        
        iViewCommandHandle.ExecuteCommandL( 
            EPhoneViewShowQuery, &queryDialogParam );
        
        CleanupStack::PopAndDestroy( text );
        }
    }


// ---------------------------------------------------------------------------
// CPhoneUnattendedTransferCmdHandler::DoUnattendedTransferL
// ---------------------------------------------------------------------------
//
void CPhoneUnattendedTransferCmdHandler::DoUnattendedTransferL()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneUnattendedTransferCmdHandler::DoUnattendedTransferL" )
    
    // First get the transfer to address from dialog
    TPhoneCmdParamString transferTarget;
    HBufC* content = HBufC::NewLC( KPEPhoneNumberMaxLength );
    TPtr ptr( content->Des() );
    transferTarget.SetString( &ptr );
    CPhoneState* phoneState = 
        static_cast< CPhoneState* >( iStateMachine.State() );    
    if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) && 
         phoneState->IsCustomizedDialerVisibleL() )
        {
        iViewCommandHandle.ExecuteCommand(
            EPhoneViewGetNumberFromEntry,
            &transferTarget );  
        
        phoneState->CloseCustomizedDialerL();
        }
    else
        {
        iViewCommandHandle.ExecuteCommandL( 
            EPhoneViewGetTextQueryContent, &transferTarget );
        }
    
    // Set transfer to address to the phoneengine
    iStateMachine.PhoneEngineInfo()->SetTransferTargetCommand( ptr );
    iStateMachine.SendPhoneEngineMessage( 
        MPEPhoneModel::EPEMessageDoUnattendedTransfer );
    
    if ( KErrNone == iStateMachine.PhoneEngineInfo()->ErrorInfo().iErrorCode )
        {
        ShowWaitingAcceptanceNoteL();
        }
    CleanupStack::PopAndDestroy( content );
    }


// ---------------------------------------------------------------------------
// CPhoneUnattendedTransferCmdHandler::CloseTransferDialerL
// ---------------------------------------------------------------------------
//
void CPhoneUnattendedTransferCmdHandler::CloseTransferDialerL()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneUnattendedTransferCmdHandler::CloseTransferDialerL" )
    
    CPhoneState* phoneState = 
        static_cast< CPhoneState* >( iStateMachine.State() );
    phoneState->CloseCustomizedDialerL();
    }


// ---------------------------------------------------------------------------
// CPhoneUnattendedTransferCmdHandler::OpenSingleItemContactFetchL
// ---------------------------------------------------------------------------
//
void CPhoneUnattendedTransferCmdHandler::OpenSingleItemContactFetchL( 
        TInt aFetchType )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
       "CPhoneUnattendedTransferCmdHandler::OpenSingleItemContactFetchL" )
    
    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( aFetchType );
    
    iViewCommandHandle.HandleCommandL( 
        EPhoneViewOpenSingleItemFetchDialog, 
        &integerParam );
    }

// ---------------------------------------------------------------------------
// CPhoneUnattendedTransferCmdHandler::ShowWaitingAcceptanceNoteL
// ---------------------------------------------------------------------------
//
void CPhoneUnattendedTransferCmdHandler::ShowWaitingAcceptanceNoteL()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneUnattendedTransferCmdHandler::ShowWaitingAcceptanceNoteL" )
    // Show "Waiting acceptance..." note
    TPhoneCmdParamGlobalNote globalNoteParam;
    globalNoteParam.SetType( EAknGlobalWaitNote );
    globalNoteParam.SetTone( EAvkonSIDNoSound );
    globalNoteParam.SetSoftkeys( R_AVKON_SOFTKEYS_EMPTY );
    globalNoteParam.SetTextResourceId( CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneVoIPWaitingAcceptanceNote ) );
    iViewCommandHandle.ExecuteCommandL( 
        EPhoneViewShowGlobalNote, &globalNoteParam );
    }

// ---------------------------------------------------------------------------
// CPhoneUnattendedTransferCmdHandler::LaunchCallBackQuery
// ---------------------------------------------------------------------------
//
void CPhoneUnattendedTransferCmdHandler::LaunchCallBackQueryL()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
          "CPhoneUnattendedTransferCmdHandler::LaunchCallBackQueryL() ")
    MPEEngineInfo* info = iStateMachine.PhoneEngineInfo();
    const TDesC& callBackToAddr = info->CallBackAddress();
    // We cannot do call back if original call had private address
    if ( callBackToAddr.Compare( KNullDesC ) == 0 )
        {
        return;
        }
          
    TPhoneCmdParamQuery queryParam;
    queryParam.SetQueryType( EPhoneGlobalMsgQuery );
    
    HBufC* msgHeader = 
      StringLoader::LoadLC( CPhoneMainResourceResolver::Instance()->
          ResolveResourceID( EPhoneVoIPTransferCallBackHeader ) );
    queryParam.SetQueryPrompt( *msgHeader );
         
    HBufC* queryText = 
      StringLoader::LoadLC( CPhoneMainResourceResolver::Instance()->
          ResolveResourceID( 
                  EPhoneVoIPTransferCallBackText ), callBackToAddr );
    
    queryParam.SetDataText( &queryText->Des() );
    
    queryParam.SetDefaultCba( R_AVKON_SOFTKEYS_OK_CANCEL );
    
    // configure custom command mappings for user responses
    queryParam.SetCbaCommandMapping( 
      EAknSoftkeyOk, EPhoneCmdUnattendedTransferCallBackOk );
    queryParam.SetCbaCommandMapping( 
      EAknSoftkeyCancel, EPhoneCmdUnattendedTransferCallBackCancel );
    
    iViewCommandHandle.ExecuteCommandL( EPhoneViewShowQuery, &queryParam );
    
    CleanupStack::PopAndDestroy( queryText );
    CleanupStack::PopAndDestroy( msgHeader );
    }
