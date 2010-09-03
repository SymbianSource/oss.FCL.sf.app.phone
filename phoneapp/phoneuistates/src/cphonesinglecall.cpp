/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneSingleCall class.
*
*/


// INCLUDES
#include <eikmenub.h> 
#include <eikenv.h>
#include <StringLoader.h>
#include <featmgr.h>
#include <pevirtualengine.h>
#include <mpeengineinfo.h>
#include <cpephonemodelif.h>
#include <connect/sbdefs.h>
#include <videotelcontrolmediatorapi.h>
#include <MediatorDomainUIDs.h>
#include <telephonyvariant.hrh>

#include "cphonesinglecall.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamcallheaderdata.h"
#include "mphonestatemachine.h"
#include "tphonecmdparaminteger.h"
#include "phoneviewcommanddefinitions.h"
#include "phoneui.hrh"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "phonestatedefinitionsgsm.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamcallstatedata.h"
#include "phonelogger.h"
#include "cphonepubsubproxy.h"
#include "cphonemediatorfactory.h"
#include "cphonemediatorsender.h"
#include "cphoneswitchtovideoorvoicecommand.h"
#include "mphonestorage.h"
#include "cphonecenrepproxy.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C CPhoneSingleCall::CPhoneSingleCall( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneGsmInCall( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneSingleCall::~CPhoneSingleCall()
// -----------------------------------------------------------
//
EXPORT_C CPhoneSingleCall::~CPhoneSingleCall()
    {
    }

// -----------------------------------------------------------
// CPhoneSingleCall::ConstructL()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneSingleCall::ConstructL()
    {
    CPhoneGsmInCall::ConstructL();
    
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );
    
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnected );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    // No connected call, 
    if( callStateData.CallId() == KErrNotFound )
        {
        holdFlag.SetBoolean( ETrue );
        //find the held call.
        callStateData.SetCallState( EPEStateHeld );
        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );
        // No held call.
        if ( callStateData.CallId() == KErrNotFound ) 
            {
            // find the disconnected call.
            callStateData.SetCallState( EPEStateDisconnecting );
            iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );    
            }
        }
    iCallId = callStateData.CallId();
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
      
    }

// -----------------------------------------------------------
// CPhoneSingleCall::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneSingleCall* CPhoneSingleCall::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneSingleCall* self = new( ELeave ) CPhoneSingleCall( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// --------------------------------------------------------------
// CPhoneSingleCall::HandleKeyMessageL
// --------------------------------------------------------------
//
EXPORT_C void CPhoneSingleCall::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleCall::HandleKeyMessageL()");
    switch ( aCode )
        {
        case EKeyYes: // send-key
            if( IsNumberEntryUsedL() )
                {
                if ( IsVideoCallActiveL() )
                    {
                    HBufC *phoneNumber = HBufC::NewLC( KPhoneNumberEntryBufferSize );
                    TPtr ptr( phoneNumber->Des() );
                    TPhoneCmdParamString stringParam;
                    stringParam.SetString( &ptr );
                    iViewCommandHandle->ExecuteCommand(
                        EPhoneViewGetNumberFromEntry,
                        &stringParam );
                    iStateMachine->PhoneEngineInfo()->SetPhoneNumber( ptr ) ;
                                            
                    if (  iStateMachine->PhoneEngineInfo()->PhoneNumberIsServiceCode() ||
                          phoneNumber->Des().Length() < KPhoneValidPhoneNumberLength )
                        {
                        CallFromNumberEntryL();
                        }
                    else
                        {
                        iStateMachine->SendPhoneEngineMessage( 
                            MPEPhoneModel::EPEMessageCheckEmergencyNumber );
                        }
                    CleanupStack::PopAndDestroy( phoneNumber ); 
                    }
                else
                    {
                    CallFromNumberEntryL();
                    }
                }
            else
                {
                ToggleHoldL();
                }
            break;
            
        default:
            CPhoneGsmInCall::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneSingleCall::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneSingleCall::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleCall::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageHeld:
            HandleHeldL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;

        case MEngineMonitor::EPEMessageIncoming:
            HandleIncomingL( aCallId );
            break;
                
        case MEngineMonitor::EPEMessageDialing:
            HandleDiallingL( aCallId );
            break;
        
        /* Flow through */ 
        case MEngineMonitor::EPEMessageRemoteTerminated:  
        case MEngineMonitor::EPEMessageDisconnecting:
            iViewCommandHandle->ExecuteCommandL( EPhoneViewHideNaviPaneAudioVolume );
            CPhoneGsmInCall::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        
        case MEngineMonitor::EPEMessageInValidEmergencyNumber:
            SendGlobalErrorNoteL( EPhoneNoteTextNotAllowed, ETrue );
            break;
            
        case MEngineMonitor::EPEMessageValidEmergencyNumber:
            DialVoiceCallL();
            break;
            
        default:
            CPhoneGsmInCall::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneSingleCall::HandleCommandL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneSingleCall::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleCall::HandleCommandL()");
    TBool commandStatus = ETrue;
    switch( aCommand )
        {
        /* Flow through */           
        case EPhoneInCallCmdHold:
        case EPhoneInCallCmdUnhold:
            ToggleHoldL();
            break;
            
        case EPhoneInCallCmdSwitchToVideo:
            SwitchToVideoL();
            break;
            
        case EPhoneCmdYesSwitchToVideo:
            // Set current call id
            iStateMachine->SetCallId( iCallId );
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessageSwitchToVideoOrVoice );
            break;
        
        /* Flow through */ 
        case EPhoneInCallCmdNewCall:
        case EPhoneInCallCmdNewCallCall:
        case EPhoneInCallCmdSwitchToVoice:
        case EPhoneCmdNoSwitchToVideo:
        case EPhoneCmdNoSwitchToVoice:
            break;        
                
        case EPhoneCmdYesSwitchToVoice:
            // Set current call id.
            iStateMachine->SetCallId( iCallId );
            if( IsVideoCall( iCallId ) )
                {
                // Video call can be released only after we get response to VT Shutdown Command
                CPhoneMediatorFactory::Instance()->Sender()->IssueCommand( 
                       KMediatorVideoTelephonyDomain,
                       KCatPhoneToVideotelCommands, 
                       EVtCmdReleaseDataport,
                       TVersion( KPhoneToVideotelCmdVersionMajor,
                             KPhoneToVideotelCmdVersionMinor, 
                             KPhoneToVideotelCmdVersionBuild ),
                       KNullDesC8,
                       CPhoneSwitchToVideoOrVoiceCommand::NewL( *iStateMachine ) );
                }
            else
                {
                iStateMachine->SendPhoneEngineMessage(
                  CPEPhoneModelIF::EPEMessageSwitchToVideoOrVoice );
                }
            break;

        case EPhoneVideoSharing:
        case EPhoneVideoSharingOpt2:
        case EPhoneVideoSharingOpt3:
            {
            TPhoneCmdParamInteger command;
            command.SetInteger( aCommand );
            iViewCommandHandle->ExecuteCommandL( 
                    EPhoneViewSendAiwCommand, 
                    &command );
            }
            break;
            
        default:
            commandStatus = CPhoneGsmInCall::HandleCommandL( aCommand );
            break;
        }
    return commandStatus;
    }

// -----------------------------------------------------------
// CPhoneSingleCall::CallId
// -----------------------------------------------------------
//
EXPORT_C TInt CPhoneSingleCall::CallId() const
    {
    return iCallId;
    }
    
// -----------------------------------------------------------
// CPhoneSingleCall::HandleHeldL
// -----------------------------------------------------------
//
void CPhoneSingleCall::HandleHeldL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::HandleHeldL()");
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateHeld );
    
    TInt callLabelId;
    TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
    callLabelId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCallOnHold );

    StringLoader::Load( 
        labelText, 
        callLabelId, 
        CCoeEnv::Static() );        
    callHeaderParam.SetLabelText( labelText );
 
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewUpdateBubble, aCallId, &callHeaderParam );
    
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    UpdateInCallCbaL();
    SetTouchPaneButtons( EPhoneIncallHeldButtons );
    }
    
// -----------------------------------------------------------
// CPhoneSingleCall::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneSingleCall::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::HandleConnectedL()");
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
    
    // Get initial hold flag from view. 
    TPhoneCmdParamBoolean holdFlag;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetHoldFlag, &holdFlag );
        
    if ( holdFlag.Boolean() )
        {
        holdFlag.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
        }

    UpdateInCallCbaL();  
    SetTouchPaneButtons( EPhoneIncallButtons );
    }
    
// -----------------------------------------------------------
// CPhoneSingleCall::ToggleHold
// -----------------------------------------------------------
//
void CPhoneSingleCall::ToggleHoldL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::ToggleHoldL()");

    TPECallControlCaps callControlCaps =
        iStateMachine->PhoneEngineInfo()->CallControlCaps( iCallId );
    
    if ( callControlCaps & MCCECallObserver::ECCECapsHold  )
        {
        iStateMachine->SetCallId( iCallId );         
        iStateMachine->SendPhoneEngineMessage( 
            CPEPhoneModelIF::EPEMessageHold );    
        }
    else if ( callControlCaps & MCCECallObserver::ECCECapsResume )
        {
        iStateMachine->SetCallId( iCallId );         
        iStateMachine->SendPhoneEngineMessage( 
            CPEPhoneModelIF::EPEMessageResume );    
        }
    else
        {
        SendGlobalInfoNoteL( EPhoneNoteTextNotAllowed, ETrue );    
        }
    }
        
// -----------------------------------------------------------
// CPhoneSingleCall::HandleIncomingL
// -----------------------------------------------------------
//
void CPhoneSingleCall::HandleIncomingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::HandleIncomingL()");
    BeginUiUpdateLC();
    
    // Get allow waiting call header param value.
    TPhoneCmdParamBoolean dialerParam;
    dialerParam.SetBoolean( ETrue );
    AllowShowingOfWaitingCallHeaderL( dialerParam );
    
    DisplayIncomingCallL( aCallId, dialerParam );
    SetTouchPaneButtons( EPhoneWaitingCallButtons ); 
    
    if( FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke ) 
            && !CPhoneCenRepProxy::Instance()->
            IsTelephonyFeatureSupported( KTelephonyLVFlagAllowUnlockOnIncoming ) 
            && ( IsKeyLockOn() || IsAutoLockOn() ) )
        {
        DisableCallUIL();
        }
    else
        {
        // if keys have been locked, disable keylock without information note
        if ( IsKeyLockOn() )
            {
            iViewCommandHandle->ExecuteCommandL( EPhoneViewDisableKeyLockWithoutNote );
            }
        }
    
    if( CPhoneCenRepProxy::Instance()->
            IsTelephonyFeatureSupported( KTelephonyLVFlagDisableCallControlHardKeysWhileLocked ) 
            && ( IsKeyLockOn() || IsAutoLockOn() ) )
        {
        DisableHWKeysL();
        }
        
    EndUiUpdate();

    // This query is required to dismiss
    // Operation cannot be completed in waiting and single state
    if ( iSwitchToVideoQuery )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveQuery );
        }
    
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
    iStateMachine->ChangeState( EPhoneStateWaitingInSingle );        
    }
    
// -----------------------------------------------------------
// CPhoneSingleCall::DisplayIncomingCallL
// -----------------------------------------------------------
//
void CPhoneSingleCall::DisplayIncomingCallL( 
    TInt aCallId, 
    const TPhoneCmdParamBoolean aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::DisplayIncomingCallL()");
    // Cannot delete active note, e.g. New call query, 
    // but show waiting note with or without caller name
    if ( IsAnyQueryActiveL() || !aCommandParam.Boolean() )
        {
        CallWaitingNoteL( aCallId );
        }
    else
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
        }
    
    // Indicate that the Phone needs to be sent to the background if
    // an application other than the top application is in the foreground
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( !TopAppIsDisplayedL() );
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewSetNeedToSendToBackgroundStatus,
        &booleanParam );

    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
        &uidParam );
    DisplayHeaderForCallComingInL( aCallId, ETrue ); //waiting call 
    }    
// -----------------------------------------------------------
// CPhoneSingleCall::HandleDiallingL
// -----------------------------------------------------------
//
void CPhoneSingleCall::HandleDiallingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::HandleDiallingL()");
    BeginUiUpdateLC();
    SetNumberEntryVisibilityL(EFalse);
    DisplayCallSetupL( aCallId );
    SetTouchPaneButtons( EPhoneCallSetupAndSingleButtons );
    EndUiUpdate();
    
    UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
    iStateMachine->ChangeState( EPhoneStateCallSetupInSingle );
    }
    
// -----------------------------------------------------------
// CPhoneSingleCall::DisplayCallSetupL
// -----------------------------------------------------------
//
void CPhoneSingleCall::DisplayCallSetupL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::DisplayCallSetupL()");
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );

    DisplayHeaderForOutgoingCallL( aCallId );
    }

// -----------------------------------------------------------
// CPhoneSingleCall::SwitchToVideoL
// -----------------------------------------------------------
//
void CPhoneSingleCall::SwitchToVideoL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::SwitchToVideoL()");
    if( FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
        {        
        TInt networkMode( CPhonePubSubProxy::Instance()->Value( KPSUidNetworkInfo,
            KNWTelephonyNetworkMode ) );
        TBool restoreOngoing( CPhonePubSubProxy::Instance()->Value( KUidSystemCategory,
            conn::KUidBackupRestoreKey ));
        if ( networkMode != ENWNetworkModeWcdma )
            {
            // We aren't in 3G, video call not possible
            SendGlobalInfoNoteL( EPhoneInformationNoNetworkSupportForVideoCallNote );    
            }
        else if ( restoreOngoing & ( conn::EBURRestoreFull | conn::EBURRestorePartial ))
            {
            SendGlobalInfoNoteL( EPhoneInformationVideoCallNotAllowedDuringRestoreNote );
            }
        else
            {
            iSwitchToVideoQuery = ETrue;
            iSwitchToVideoQuery = EFalse;
            }    
        }
    }


// End of File
