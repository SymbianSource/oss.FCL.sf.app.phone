/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneGsmInCall class.
*
*/


// INCLUDES
#include <mpeengineinfo.h>
#include <pevirtualengine.h>
#include <cpephonemodelif.h>
#include <StringLoader.h>

#include "cphonegsmincall.h"
#include "phonerssbase.h"
#include "phonelogger.h"
#include "cphonegeneralgsmmessageshandler.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "mphonestatemachine.h"
#include "cphonemainresourceresolver.h"
#include "phoneui.hrh"

#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamcallheaderdata.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneGsmInCall::CPhoneGsmInCall( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneStateInCall( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneGsmInCall::~CPhoneGsmInCall()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------

CPhoneGsmInCall::~CPhoneGsmInCall()
    {
    }

// -----------------------------------------------------------
// CPhoneGsmInCall::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneGsmInCall::ConstructL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneGsmInCall::ConstructL()");
    CPhoneStateInCall::ConstructL();
    }

// -----------------------------------------------------------
// CPhoneGsmInCall::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneGsmInCall* CPhoneGsmInCall::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneGsmInCall* self = new (ELeave) 
        CPhoneGsmInCall( aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneGsmInCall::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneGsmInCall::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneGsmInCall::HandlePhoneEngineMessageL() ");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageRemoteHeld:
            CPhoneState::SendGlobalInfoNoteL( 
                EPhoneInformationRemotePutOnHoldNote, ETrue );
            break;
        
        case MEngineMonitor::EPEMessageRemoteResumed:
            CPhoneState::SendGlobalInfoNoteL( 
                EPhoneInformationConnectedNote, ETrue );
            break;
            
        case MEngineMonitor::EPEMessageRemoteCreatedConference:
            CPhoneState::SendGlobalInfoNoteL( 
                EPhoneInformationRemoteCreateConferenceNote, ETrue );
            break;        
        case MEngineMonitor::EPEMessageIncCallIsForw:  // fall through
        case MEngineMonitor::EPEMessageIssuingSSRequest: // fall through
        case MEngineMonitor::EPEMessageCallBarred: // fall through
        case MEngineMonitor::EPEMessageIssuedSSRequest: // fall through
        case MEngineMonitor::EPEMessageTempClirActivationUnsuccessful:
        case MEngineMonitor::EPEMessageIncCallForwToC: // fall through
        case MEngineMonitor::EPEMessageOutCallForwToC: // fall through
        case MEngineMonitor::EPEMessageForwardUnconditionalModeActive: // fall through
        case MEngineMonitor::EPEMessageForwardConditionallyModeActive:
            {
            CPhoneGeneralGsmMessagesHandler* gsmMsgHandler =
                CPhoneGeneralGsmMessagesHandler::NewL( *iStateMachine,
                                                       *iViewCommandHandle,
                                                       *this );
            CleanupStack::PushL( gsmMsgHandler );
            gsmMsgHandler->HandlePhoneEngineMessageL( aMessage, aCallId );
            CleanupStack::PopAndDestroy( gsmMsgHandler );
            
            // Needed also in non-touch, if call waiting request (*43#) 
            // is sent during active call at least.
            UpdateCbaL( EPhoneCallHandlingInCallCBA );
			}
			break;
			
        case MEngineMonitor::EPEMessageColpNumberAvailable:
            {
            HandleColpNoteL( aCallId );                    
            }
            break; 

        default:
            CPhoneStateInCall::HandlePhoneEngineMessageL( 
                aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneGsmInCall:HandleCommandL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneGsmInCall::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneGsmInCall::HandleCommandL() ");
    TBool commandStatus = ETrue;

    switch( aCommand )
        {                        
        case EPhoneCallComingCmdAnswer:
        case EPhoneInCallCmdAnswer:
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessageAnswer );
            break;
        
        // 'Replace' from menu            
        case EPhoneInCallCmdReplace:
            ReplaceCallL();
            break;
        
        case EPhoneInCallCmdSwap:
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessageSwap ); 
            break;  
            
        case EPhoneInCallCmdEndAllCalls:
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessageReleaseAll );        
            break;
            
        case EPhoneInCallCmdCreateConference:
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessageCreateConference );
            break;
            
        case EPhoneInCallCmdTransfer:
            iStateMachine->SendPhoneEngineMessage( 
                CPEPhoneModelIF::EPEMessageTransfer );                        
            break;   
            
       default:
            CPhoneStateInCall::HandleCommandL( aCommand );
            break;
        }
    return commandStatus;
    }


// -----------------------------------------------------------
// CPhoneGsmInCall::ReplaceCallL
// -----------------------------------------------------------
//
void CPhoneGsmInCall::ReplaceCallL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneGsmInCall::ReplaceCallL() ");
          
    iStateMachine->SendPhoneEngineMessage( 
        MPEPhoneModel::EPEMessageReplaceActive );
    }

// -----------------------------------------------------------
// CPhoneGsmInCall::HandleColpNoteL
// -----------------------------------------------------------
//
void CPhoneGsmInCall::HandleColpNoteL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneGsmInCall::HandleColpNoteL() ");

    MPEEngineInfo* EngineInfo = CPhoneState::iStateMachine->PhoneEngineInfo();
    
    TPhoneCmdParamGlobalNote globalNoteParam;
    globalNoteParam.SetText(  EngineInfo->RemoteColpNumber( aCallId ) ); 
    globalNoteParam.SetType( EPhoneNotificationDialog );
    globalNoteParam.SetTextResourceId( 
            CPhoneMainResourceResolver::Instance()->
	        ResolveResourceID( EPhoneColpConnected ) );
    globalNoteParam.SetNotificationDialog( ETrue );
	        
    iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewShowGlobalNote, &globalNoteParam );
    
    }

// -----------------------------------------------------------
// CPhoneGsmInCall::BringIncomingToForegroundL()
// -----------------------------------------------------------
//
void CPhoneGsmInCall::BringIncomingToForegroundL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneGsmInCall::BringIncomingToForegroundL( ) ");

    // Remove any phone dialogs if they are displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );

    // Bring Phone app in the foreground
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );

    // Set Phone as the top application
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
        &uidParam );
    
    // Disable global notes when there is an incoming call
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetEikonNotifiersDisabled,
        &globalNotifierParam );
    
    }

// -----------------------------------------------------------
// CPhoneGsmInCall::AllowShowingOfWaitingCallHeaderL
// -----------------------------------------------------------
//
void CPhoneGsmInCall::AllowShowingOfWaitingCallHeaderL( 
    TPhoneCmdParamBoolean& aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneGsmInCall::AllowShowingOfWaitingCallHeaderL() ");

    if ( aCommandParam.Boolean() && IsNumberEntryUsedL() )
        {
        SetNumberEntryVisibilityL(EFalse);
        }
    }

// -----------------------------------------------------------
// CPhoneGsmInCall::HandlePhoneForegroundEventL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneGsmInCall::HandlePhoneForegroundEventL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneGsmInCall::HandlePhoneForegroundEventL( ) ");
     }

// -----------------------------------------------------------
// CPhoneGsmInCall::HandleHoldNoteL
// -----------------------------------------------------------
//
void CPhoneGsmInCall::HandleHoldNoteL( 
    TInt aCallId, TBool aHold )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneGsmInCall::HandleHoldNoteL( ) ");
    TBuf< KPhoneContactNameMaxLength > holdText( KNullDesC );
    TPhoneCmdParamGlobalNote globalNoteParam;
    TInt resourceId(0);
    
    if( iStateMachine->PhoneEngineInfo()->RemoteName( aCallId ).Length() ||
        iStateMachine->PhoneEngineInfo()->RemoteCompanyName( aCallId ).Length())
        {
        TInt resource( EPhoneInformationCallActivetedCli );
        if ( aHold )
            {
            resource = EPhoneInformationCallOnHoldCli;    
            }
        GetRemoteInfoDataL ( aCallId, holdText );
        resourceId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( resource );
        globalNoteParam.SetTextResourceId( resourceId );    
        }
    else
        {
        TInt resource( EPhoneInformationCallActiveted );
        if ( aHold )
            {
            resource = EPhoneInformationCallOnHoldNoCli;    
            }
        resourceId = CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( resource );
                
        HBufC* tmp = StringLoader::LoadL( resourceId, 
            aCallId + 1 , CCoeEnv::Static() );

        holdText = *tmp;
        delete tmp;
        tmp = NULL;  
        }
         
    globalNoteParam.SetText( holdText ); 
    globalNoteParam.SetType( EPhoneMessageBoxInformation );
        
    iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewShowGlobalNote, &globalNoteParam ); 
    }


// ---------------------------------------------------------
// CPhoneGsmInCall::SetDivertIndication
// ---------------------------------------------------------
//
EXPORT_C void CPhoneGsmInCall::SetDivertIndication( const TBool aDivertIndication )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneGsmInCall::SetDivertIndication()");
           
    CPhoneState::SetDivertIndication( aDivertIndication );    
    
    TRAP_IGNORE( HandeDivertIndicationL() );
    }

// ---------------------------------------------------------
// CPhoneGsmInCall::HandeDivertIndicationL
// ---------------------------------------------------------
//
void CPhoneGsmInCall::HandeDivertIndicationL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneGsmInCall::HandeDivertIndicationL()");
       
    TBuf< KPhoneContactNameMaxLength > remoteInfoText( KNullDesC );
    TInt ringingCallId ( KErrNotFound );
        
    ringingCallId = GetRingingCallL();
    
    if( ringingCallId > KErrNotFound )
       {
       TPhoneCmdParamCallHeaderData divertData;
    
        divertData.SetDiverted( ETrue );
       
       GetRemoteInfoDataL( ringingCallId, remoteInfoText );
       divertData.SetCLIText( 
                  remoteInfoText,
                  TPhoneCmdParamCallHeaderData::ERight );
       
       divertData.SetCiphering(
           iStateMachine->PhoneEngineInfo()->IsSecureCall( ringingCallId ) );
       divertData.SetCipheringIndicatorAllowed(
           iStateMachine->PhoneEngineInfo()->SecureSpecified() );
           
       iViewCommandHandle->ExecuteCommandL( 
           EPhoneViewUpdateCallHeaderRemoteInfoData, ringingCallId,
           &divertData );
        }
    }

// End of File
