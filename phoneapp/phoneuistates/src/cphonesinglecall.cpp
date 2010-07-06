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
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C CPhoneSingleCall::~CPhoneSingleCall()
    {
    }

// -----------------------------------------------------------
// CPhoneSingleCall::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C void CPhoneSingleCall::ConstructL()
    {
    CPhoneGsmInCall::ConstructL();
    
    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );

    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    // Fetch active call's id from view    
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnected );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
            
    if( callStateData.CallId() == KErrNotFound )
        {
        holdFlag.SetBoolean( ETrue );
        // No connected call, find the hold call
        callStateData.SetCallState( EPEStateHeld );                    
        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );
            
        if ( callStateData.CallId() == KErrNotFound ) 
            {
            // No hold call, find the disconnected call
            callStateData.SetCallState( EPEStateDisconnecting );                    
            iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );    
            }
        }

    iCallId = callStateData.CallId();
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    
    // Update phone number availability for menu use
    PhoneNumberAvailableInPhoneEngineL( callStateData.CallId() );
      
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
        // send-key
        case EKeyYes:
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
                        // Send a manual control sequence by providing number 
                        // information with dial command
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
                    // Provide number information with dial command
                    CallFromNumberEntryL();
                    }
                }
            else
                {
                ToggleHoldL();                    
                }
            break;
            
        default:
            // do base operation
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
            
        case EPhoneInCallCmdNewCall:
            LaunchNewCallQueryL();
            break;
            
        case EPhoneInCallCmdNewCallCall:
            CallFromNewCallQueryL();
            break;

        case EPhoneInCallCmdSwitchToVideo:
            SwitchToVideoL();
            break;
            
        case EPhoneInCallCmdSwitchToVoice:
            SwitchToVoiceL();
            break;
            
        case EPhoneCmdYesSwitchToVideo:
            // Set current call id
            iStateMachine->SetCallId( iCallId );
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessageSwitchToVideoOrVoice );
            break;
        
        /* Flow through */ 
        case EPhoneCmdNoSwitchToVideo:
        case EPhoneCmdNoSwitchToVoice:
            // Sets touch buttons to correct status if 'No' command has occured.    
            iViewCommandHandle->ExecuteCommand( EPhoneResetTouchButtons );    
            break;        
                
        case EPhoneCmdYesSwitchToVoice:
            // Set current call id
            iStateMachine->SetCallId( iCallId );
                        if( IsVideoCall( iCallId ) )
                        {
                           // Video call can be released only after we get response to VT Shutdown Command
                           CPhoneMediatorFactory::Instance()->Sender()->IssueCommand( KMediatorVideoTelephonyDomain,
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
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSendAiwCommand, 
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
// CPhoneSingleCall::OpenMenuBarL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneSingleCall::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleCall::OpenMenuBarL()");

    if ( IsVideoCall ( iCallId ))
        {
        OpenVideoCallMenuBarL();
        }
    else
        {
        CPhoneGsmInCall::OpenMenuBarL();            
        }
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
// CPhoneSingleCall::OpenVideoCallMenuBarL
// -----------------------------------------------------------
//
void CPhoneSingleCall::OpenVideoCallMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleCall::OpenVideoCallMenuBarL()");
    if( FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
        {
        TInt resourceId;

        if ( IsNumberEntryUsedL() )
            {
            resourceId = EPhoneVideoCallMenubarWithNumberEntry;
            }
        else
            {
            resourceId = EPhoneVideoCallMenubar;
            }
        
        TPhoneCmdParamInteger integerParam;
        integerParam.SetInteger( 
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( resourceId ) );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
            &integerParam );
        }                    
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
       
    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
        
    if ( !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        { 
        HandleHoldNoteL( aCallId , ETrue );
        }

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
    
    // Show bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
    
    // Get initial hold flag from view. 
    TPhoneCmdParamBoolean holdFlag;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetHoldFlag, &holdFlag );
        
    if ( holdFlag.Boolean() )
        {
        // Update hold flag to view
        holdFlag.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
        
        if ( !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
            {
            HandleHoldNoteL( aCallId , EFalse );
            }
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
    
    TPhoneCmdParamBoolean dialerParam;
    dialerParam.SetBoolean( ETrue );
    
    // Get allow waiting call header param value.
    AllowShowingOfWaitingCallHeaderL( dialerParam );    

    // Display incoming call
    DisplayIncomingCallL( aCallId, dialerParam );
    
    // Show incoming call buttons
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
    
    // Go to incoming state
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
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    // Cannot delete active note, e.g. New call query, 
    // but show waiting note with or without caller name
    if ( IsAnyQueryActiveL() || 
        ( !aCommandParam.Boolean() && iOnScreenDialer ) )
        {
        CallWaitingNoteL( aCallId );        
        }
    else
        {
        // Remove any phone dialogs if they are displayed
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
        }
    
    // Indicate that the Phone needs to be sent to the background if
    // an application other than the top application is in the foreground
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( !TopAppIsDisplayedL() );
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewSetNeedToSendToBackgroundStatus,
        &booleanParam );

    // Bring Phone app in the foreground
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );

    // Set Phone as the top application
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
        &uidParam );

    DisplayHeaderForCallComingInL( aCallId, ETrue ); //waiting call 
    }    
    
// ---------------------------------------------------------
// CPhoneSingleCall::CallFromNewCallQueryL
// ---------------------------------------------------------
//
void CPhoneSingleCall::CallFromNewCallQueryL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::CallFromNewCallQueryL()");
    // First get the phone number from the dialog
    TPhoneCmdParamString phoneNumberParam;
    HBufC *content = HBufC::NewLC( KPhoneNumberEntryBufferSize );
    TPtr ptr( content->Des() );
    phoneNumberParam.SetString( &ptr );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetTextQueryContent,
        &phoneNumberParam );

    // Store the phone number
    iStateMachine->PhoneEngineInfo()->SetPhoneNumber( ptr );

    // clean up stack
    CleanupStack::PopAndDestroy( content );
    
    DialVoiceCallL();
    }

// -----------------------------------------------------------
// CPhoneSingleCall::HandleDiallingL
// -----------------------------------------------------------
//
void CPhoneSingleCall::HandleDiallingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::HandleDiallingL()");
    
    BeginTransEffectLC( ENumberEntryClose );
    BeginUiUpdateLC();
    
    SetNumberEntryVisibilityL(EFalse);

    // Display call setup 
    DisplayCallSetupL( aCallId );
    
    // Show incoming call buttons
    SetTouchPaneButtons( EPhoneCallSetupAndSingleButtons );

    EndUiUpdate();
    
    EndTransEffect();

    // Go to call setup state
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
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    // Remove dialogs if necessary
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );

    // Capture keys when the phone is dialling
    CaptureKeysDuringCallNotificationL( ETrue );

    // Force telephony to the foreground
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );

    // Display call setup header
    DisplayHeaderForOutgoingCallL( aCallId );
    }

// -----------------------------------------------------------
// CPhoneSingleCall::SetCallHoldL
// -----------------------------------------------------------
//
void CPhoneSingleCall::SetCallHoldL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::SetCallHold()");
    
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallId( iCallId );
    iViewCommandHandle->HandleCommandL( EPhoneViewGetCallState,
        &callStateData );
    
    if( callStateData.CallState() == EPEStateConnected && !IsVideoCallActiveL() )
        {
        iStateMachine->SetCallId( iCallId );         
        iStateMachine->SendPhoneEngineMessage( 
            CPEPhoneModelIF::EPEMessageHold );    
        }
    else
        {
        // Show not allowed note
        SendGlobalInfoNoteL( EPhoneNoteTextNotAllowed, ETrue );    
        }
    }
    
// -----------------------------------------------------------
// CPhoneSingleCall::SetCallResumeL
// -----------------------------------------------------------
//
void CPhoneSingleCall::SetCallResumeL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::SetCallHold()");

    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallId( iCallId );
    iViewCommandHandle->HandleCommandL( EPhoneViewGetCallState,
        &callStateData );
    
    if( callStateData.CallState() == EPEStateHeld )
        {
        iStateMachine->SetCallId( iCallId );
        iStateMachine->SendPhoneEngineMessage(
            CPEPhoneModelIF::EPEMessageResume );
        }
    else
        {
        // Show not allowed note
        SendGlobalInfoNoteL( EPhoneNoteTextNotAllowed, ETrue );    
        }                
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
    
        TBool resetTouchButtons = EFalse;
        
        if ( networkMode != ENWNetworkModeWcdma )
            {
            // We aren't in 3G, video call not possible
            SendGlobalInfoNoteL( EPhoneInformationNoNetworkSupportForVideoCallNote );    
            resetTouchButtons = ETrue;
            }
        else if ( restoreOngoing & ( conn::EBURRestoreFull | conn::EBURRestorePartial ))
            {
            // MC restore ongoing
            SendGlobalInfoNoteL( EPhoneInformationVideoCallNotAllowedDuringRestoreNote );
            resetTouchButtons = ETrue;    
            }
        else
            {
        iSwitchToVideoQuery = ETrue;
        ShowQueryL( EPhoneSwitchToVideoQuery );
        iSwitchToVideoQuery = EFalse;
        }    
    
        if( resetTouchButtons )
            {
            // Sets touch buttons to correct status if error has occured.    
            iViewCommandHandle->ExecuteCommand( EPhoneResetTouchButtons ); 
            }
        }
    }

// -----------------------------------------------------------
// CPhoneSingleCall::SwitchToVoiceL
// -----------------------------------------------------------
//
void CPhoneSingleCall::SwitchToVoiceL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleCall::SwitchToVoiceL()");
    
    ShowQueryL( EPhoneSwitchToVoiceQuery );
    }

// End of File
