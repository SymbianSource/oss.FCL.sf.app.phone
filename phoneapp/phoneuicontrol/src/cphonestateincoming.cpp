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
* Description: Implementation of CPhoneStateIncoming class.
*
*/


// INCLUDES
#include <cntdef.h>
#include <e32base.h>
#include <eikenv.h>
#include <featmgr.h>
#include <StringLoader.h>
#include <dundomainpskeys.h>
#include <telephonyvariant.hrh>
#include <ScreensaverInternalPSKeys.h>
#include <mpeengineinfo.h>
#include <MediatorDomainUIDs.h>
// <-- QT PHONE START -->
#include <videotelcontrolmediatorapi.h>
// <-- QT PHONE END -->

#include "cphonepubsubproxy.h"
#include "phoneui.pan"
#include "cphonestateincoming.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamringtone.h"
#include "tphonecmdparamsfidata.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamdynmenu.h"
#include "mphonestatemachine.h"
#include "phonestatedefinitions.h"
#include "phoneui.hrh"
#include "phonerssbase.h"
#include "cphonemainresourceresolver.h"
#include "phonelogger.h"
#include "cphonecenrepproxy.h"
#include "cphonepubsubproxy.h"
#include "tphonecmdparamquery.h"
#include "cphonemediatorfactory.h"
#include "cphonemediatorsender.h"
#include "cphonereleasecommand.h"
#include "mphonecustomization.h"
#include "mphonestorage.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C CPhoneStateIncoming::CPhoneStateIncoming(
    MPhoneStateMachine* aStateMachine,
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) :
    CPhoneState( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::~CPhoneStateIncoming()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C CPhoneStateIncoming::~CPhoneStateIncoming()
    {
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIncoming::ConstructL()
    {
    CPhoneState::BaseConstructL();
    
    // Fetch incoming call's id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );

    // Do nothing if negative incoming call id
    if( callStateData.CallId() > KErrNotFound  )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveQuery );
        
        iRingingCallId = callStateData.CallId();
        }
    else
        {
        __PHONELOG( EOnlyFatal, EPhoneControl,
            "CPhoneStateIncoming::ConstructL - incoming call already in disconnecting state!");
        }
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneStateIncoming* CPhoneStateIncoming::NewL(
    MPhoneStateMachine* aStateMachine,
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneStateIncoming* self = new (ELeave) CPhoneStateIncoming(
        aStateMachine, aViewCommandHandle, aPhoneCustomization );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::HandleKeyEventL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIncoming::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::HandleKeyEventL ()" );
    // Handle numeric keys when key events are received in incoming state
    CPhoneState::HandleKeyEventL( aKeyEvent, aEventCode );
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::HandleNumberEntryClearedL()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIncoming::HandleNumberEntryClearedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::HandleNumberEntryClearedL ()" );
    // Set incoming call CBA when number entry is cleared
    iCbaManager->UpdateIncomingCbaL( iRingingCallId );
    UpdateSilenceButtonDimming();
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::HandleKeyMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIncoming::HandleKeyMessageL(
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneStateIncoming::HandleKeyMessageL ()" );
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );

    switch ( aCode )
        {
        // end-key
        case EKeyNo:
            {
            // handle long press
            if ( aMessage == EPhoneKeyLongPress )
                {
                // Close all connections
                iStateMachine->SendPhoneEngineMessage(
                    MPEPhoneModel::EPEMessageTerminateAllConnections );
                iViewCommandHandle->ExecuteCommandL( EPhoneViewCloseFSW );

                if ( CPhoneState::IsNumberEntryUsedL() )
                    {
                    // Remove number entry from screen
                    iViewCommandHandle->ExecuteCommandL(
                        EPhoneViewRemoveNumberEntry );
                    // Do state-specific operation when number entry is cleared
                    HandleNumberEntryClearedL();
                    }
                }
            else
                {
                // handle end key
                DisconnectWaitingCallL();
                }
            }
            break;

        // send-key
        case EKeyYes:
            if( CPhoneState::IsNumberEntryVisibleL() )
                {
                HandleSendL();
                }
            else
                {
                // Answer the call
                AnswerCallL();
                }
            break;

        // OK key
        case EKeyDevice3:
            if ( aMessage == EPhoneKeyLongPress )
                {
                // Answer the call if long press of selection key
                AnswerCallL();
                }
            else if ( CPhoneCenRepProxy::Instance()->IsTelephonyFeatureSupported(
                        KTelephonyLVFlagCoverHideSendEndKey ))
                {
                // Open number entry OK menubar
                OpenMenuBarL();
                }
            break;

        case EKeyDeviceF:
                {
                __PHONELOG( EBasic, EPhoneUIStates,
                    "CPhoneConferenceAndSingleAndWaiting::HandleKeyMessageL-deviceF" );
                HandleHoldSwitchL();
                }
            break;
            
        default:
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::HandleSendL
// -----------------------------------------------------------
//
void CPhoneStateIncoming::HandleSendL()
    {
    // Get the number entry contents
    HBufC* phoneNumber = PhoneNumberFromEntryLC();
    TPtr ptr( phoneNumber->Des() );

    iStateMachine->PhoneEngineInfo()->SetPhoneNumber( ptr );
    iStateMachine->SendPhoneEngineMessage(
            MPEPhoneModel::EPEMessagePhoneNumberEdited );
    
    if ( phoneNumber->Des().Length() < KPhoneValidPhoneNumberLength 
            && iStateMachine->PhoneEngineInfo()->PhoneNumberIsServiceCode() ) 
        {
        // Send a manual control sequence by providing number
        // information with dial command
        CPhoneState::CallFromNumberEntryL();
        }
    else
        {
        iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageCheckEmergencyNumber );
        }
    CleanupStack::PopAndDestroy( phoneNumber );
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::HandleKeyPressDurationL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIncoming::HandleKeyPressDurationL(
    TKeyCode /*aCode*/,
    TTimeIntervalMicroSeconds /*aKeyPressDuration*/ )
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::HandleKeyPressDurationL ()" );
 
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIncoming::HandlePhoneEngineMessageL(
    const TInt aMessage,
    TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::HandlePhoneEngineMessageL ()" );
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;

        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;

        case MEngineMonitor::EPEMessageInValidEmergencyNumber:
            // Answer the call
            AnswerCallL();
            break;

        case MEngineMonitor::EPEMessageValidEmergencyNumber:
            DialVoiceCallL();
            break;

        case MEngineMonitor::EPEMessageAnswering:
            iViewCommandHandle->ExecuteCommandL( EPhoneViewStopRingTone );
            break;

        case MEngineMonitor::EPEMessageAudioOutputChanged:
            HandleAudioOutputChangedL();
            break;

        case MEngineMonitor::EPEMessageMuteRingingTone:
            // Silence the ringer. And stop vibrating, if it is active.
            iViewCommandHandle->ExecuteCommandL( EPhoneViewStopRingTone );
            HandleAudioPlayStoppedL();
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageStopTonePlay );
            break;
         
        case MEngineMonitor::EPEMessageAvailableAudioOutputsChanged:
            HandleAudioAvailableOutputChangedL();
            // Need to avoid CBA update
            break;
        
        case MEngineMonitor::EPEMessageRemotePartyInfoChanged:
            // Update remote info data
            UpdateRemoteInfoDataAndLabelL( aCallId, UpdateCallHeaderInfoL( aCallId ) );
            break;
            
        default:
            CPhoneState::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::UpdateRemoteInfoDataAndLabelL
// -----------------------------------------------------------
//
void CPhoneStateIncoming::UpdateRemoteInfoDataAndLabelL( 
        TInt aCallId, 
        TPhoneCmdParamCallHeaderData aCallHeaderParam )
    {
    __LOGMETHODSTARTEND(EPhoneControl,
            "CPhoneStateIncoming::UpdateRemoteInfoDataAndLabelL ()" );
    // Update the remote info data in the call header
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewUpdateCallHeaderRemoteInfoDataAndLabel, 
        aCallId,
        &aCallHeaderParam );
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneStateIncoming::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::HandleConnectedL ()" );

    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetEikonNotifiersDisabled,
        &globalNotifierParam );
    // Stop tone playing, if necessary
    iViewCommandHandle->ExecuteCommandL( EPhoneViewStopRingTone );
    
    if( IsVideoCall( aCallId ) && !IsAutoLockOn() )
        {
        // For keeping video call on top
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewSetNeedToSendToBackgroundStatus, &booleanParam );
        }
    
    BeginTransEffectLC( ENumberEntryOpen );

    if( FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke ) 
         && iStateMachine->PhoneStorage()->IsScreenLocked() )
        {
        EnableCallUIL();
        }
    
    // Reset blocked keys list
    iStateMachine->PhoneStorage()->ResetBlockedKeysList();

    BeginUiUpdateLC();
    
    // Update single call
    UpdateSingleActiveCallL( aCallId );

    SetTouchPaneButtons( EPhoneIncallButtons );
    SetToolbarDimming( EFalse );
            
    EndUiUpdate();
    EndTransEffect();

    // Go to single state
    iCbaManager->UpdateCbaL( EPhoneCallHandlingInCallCBA );

    iStateMachine->ChangeState( EPhoneStateSingle );
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::HandleAudioPlayStoppedL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIncoming::HandleAudioPlayStoppedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::HandleAudioPlayStoppedL ()" );
    // Update the CBA
    
    // Set the ringtone silenced status
    iCbaManager->SetRingtoneSilencedStatus( ETrue );
    TInt resourceId = EPhoneCallHandlingIncomingRejectCBA;
    // Get the soft reject flag status
    TPhoneCmdParamBoolean softRejectParam;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetSoftRejectFlag,
        &softRejectParam );

    if ( IsNumberEntryVisibleL() && !iOnScreenDialer )
        {
        resourceId = EPhoneNumberAcqCBA;
        }
    else if ( softRejectParam.Boolean() )
        {
        resourceId = EPhoneCallHandlingIncomingSoftRejectCBA;
        }
    iCbaManager->SetCbaL( resourceId );
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneStateIncoming::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::HandleIdleL ()" );

    BeginTransEffectLC( ENumberEntryOpen );
    BeginUiUpdateLC();

    // Enable call UI
    if( FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke ) 
        && iStateMachine->PhoneStorage()->IsScreenLocked() )
        {
        EnableCallUIL();
        }

    // Reset blocked keys list
    iStateMachine->PhoneStorage()->ResetBlockedKeysList();
    
    // Remove call
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
    // Stop tone playing, if necessary
    iViewCommandHandle->ExecuteCommandL( EPhoneViewStopRingTone );
    
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetEikonNotifiersDisabled,
        &globalNotifierParam );
    

    SetDefaultFlagsL();
    
     if ( IsNumberEntryUsedL() )
        {
        if ( NeedToSendToBackgroundL() )
            {
            // Return phone to the background if send to background is needed.
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );

            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetControlAndVisibility );

            // Set Number Entry CBA
            iCbaManager->SetCbaL( EPhoneNumberAcqCBA );
            }
        else
            {
            // Show the number entry if it exists.
            SetNumberEntryVisibilityL(ETrue);
            }
        }
    else if ( NeedToSendToBackgroundL() ||
        SoftRejectMessageEditorIsDisplayedL() )
        {
        // Continue displaying current app but set up the
        // idle screen in the background
        SetupIdleScreenInBackgroundL();
        }
    else
        {
        // Display idle screen
        DisplayIdleScreenL();
        }
 
    DeleteTouchPaneButtons();
    EndUiUpdate();
    EndTransEffect();
    // Go to idle state   
    iCbaManager->UpdateCbaL( EPhoneEmptyCBA );
    iStateMachine->ChangeState( EPhoneStateIdle );
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::HandleCommandL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneStateIncoming::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::HandleCommandL ()" );
    TBool commandStatus = ETrue;

    switch( aCommand )
        {
        case EPhoneCmdOptions:
            // Stop tone playing, if necessary.
            // And stop vibrating, if it is active.
            iViewCommandHandle->ExecuteCommandL( EPhoneViewStopRingTone );
            // Open the menu bar
            OpenMenuBarL();
            break;

        case EPhoneCallComingCmdAnswer:
            AnswerCallL();
            break;

        case EPhoneCallComingCmdAnswerHandPortableVideo:
        case EPhoneCallComingCmdAnswerHandPortable:
            iStateMachine->PhoneEngineInfo()->SetAudioOutputCommand(
                EPEHandset, ETrue );
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageSetAudioOutput );
            AnswerCallL();
            break;

        case EPhoneCallComingCmdReject:
            DisconnectWaitingCallL();
            break;

        case EPhoneCallComingCmdSilent:
            // Silence the ringer. And stop vibrating, if it is active.
            iViewCommandHandle->ExecuteCommandL( EPhoneViewMuteRingTone );
            // Dim silence button
            SetTouchPaneButtonDisabled( EPhoneCallComingCmdSilent );
            HandleAudioPlayStoppedL();
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageStopTonePlay );
            break;

        case EPhoneCallComingCmdSoftReject:
            // Open Soft reject message editor
            OpenSoftRejectMessageEditorL();
            break;

        case EPhoneNumberAcqCmdSendCommand:
            HandleSendL();
            break;

        case EPhoneInCallCmdHelp:
            {
            TPtrC contextName;
            if( IsVideoCall( iRingingCallId ) )
                {
                contextName.Set( KINCAL_HLP_VIDEOCALL() );
                }
            else
                {
                contextName.Set( KINCAL_HLP_CALL_HANDLING() );
                }
            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewLaunchHelpApplication, 0, contextName );
            }
            break;

        default:
            commandStatus = CPhoneState::HandleCommandL( aCommand );
            break;
        }

    return commandStatus;
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::AnswerCallL
// -----------------------------------------------------------
//
void CPhoneStateIncoming::AnswerCallL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::AnswerCallL ()" );
    // Mute the ring tone
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMuteRingToneOnAnswer );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewCloseFSW );
    // Answer the call
    iStateMachine->SendPhoneEngineMessage(
        MPEPhoneModel::EPEMessageAnswer );
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::DisconnectWaitingCallL
// -----------------------------------------------------------
//
void CPhoneStateIncoming::DisconnectWaitingCallL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::DisconnectWaitingCallL ()" );

    // Stop tone playing, if necessary
    iViewCommandHandle->ExecuteCommandL( EPhoneViewStopRingTone );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewCloseFSW );

    // The ringing call might have changed
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    
    if ( callStateData.CallId() > KErrNotFound )
        {
        iRingingCallId = callStateData.CallId();
        }
    
    iStateMachine->SetCallId( iRingingCallId );

    if( IsVideoCall( iRingingCallId ) )
        {
        // Video call can be released only after we get response to VT Shutdown Command
// <-- QT PHONE START -->
 
        CPhoneMediatorFactory::Instance()->Sender()->IssueCommand( KMediatorVideoTelephonyDomain,
                                                                             KCatPhoneToVideotelCommands, 
                                                                             EVtCmdReleaseDataport,
                                                                   TVersion( KPhoneToVideotelCmdVersionMajor,
                                                                             KPhoneToVideotelCmdVersionMinor, 
                                                                             KPhoneToVideotelCmdVersionBuild ),
                                                                   KNullDesC8,
                                                                   CPhoneReleaseCommand::NewL( *iStateMachine ) );

// <-- QT PHONE END --> 
        }
    else
        {
        // Release the call
        iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageRelease );
        }
    
    ShowDisconnectingL( iRingingCallId );
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::OpenSoftRejectMessageEditorL
// -----------------------------------------------------------
//
void CPhoneStateIncoming::OpenSoftRejectMessageEditorL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::OpenSoftRejectMessageEditorL ()" );
    // Clear the soft reject flag
    TPhoneCmdParamBoolean softRejectParam;
    softRejectParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetSoftRejectFlag,
        &softRejectParam );

    // Dim silence button
    SetTouchPaneButtonDisabled( EPhoneCallComingCmdSilent );
    
    // Silence the vibrating
    iViewCommandHandle->ExecuteCommandL( EPhoneViewStopRingTone );
    iStateMachine->SendPhoneEngineMessage(
        MPEPhoneModel::EPEMessageStopTonePlay );
    
    // Re-enable global notes. Otherwise message editor is not opened.
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetEikonNotifiersDisabled,
        &globalNotifierParam );

    // Change the CBA to Options..Reject
    iCbaManager->SetCbaL( EPhoneCallHandlingIncomingRejectCBA );

    TPhoneCmdParamSfiData sfiDataParam;

    if ( iStateMachine->PhoneEngineInfo()->RemoteName( iRingingCallId ).Length() )
        {
        // store both the name and the number
        sfiDataParam.SetNumber( iStateMachine->PhoneEngineInfo()->RemotePhoneNumber( iRingingCallId ) );
        sfiDataParam.SetName( iStateMachine->PhoneEngineInfo()->RemoteName( iRingingCallId ) );
        }
    else
        {
        // store the number
        sfiDataParam.SetNumber( iStateMachine->PhoneEngineInfo()->RemotePhoneNumber( iRingingCallId ) );
        }

    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewOpenSoftRejectEditor, &sfiDataParam );
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::SoftRejectMessageEditorIsDisplayedL
// -----------------------------------------------------------
//
TBool CPhoneStateIncoming::SoftRejectMessageEditorIsDisplayedL() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIncoming::SoftRejectMessageEditorIsDisplayedL () ");
    // Get the foreground application window group id
    TPhoneCmdParamInteger foregroundAppParam;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetForegroundApplication,
        &foregroundAppParam );

    // Get the soft reject message editor window group id
    TPhoneCmdParamInteger softRejectMessageEditorWgId;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetSoftRejectWindowGroupId,
        &softRejectMessageEditorWgId );

    __PHONELOG1(
        EBasic,
        EPhoneControl,
        "CPhoneStateIncoming::SoftRejectMessageEditorIsDisplayedL() SoftRejectGroupId %d",
        softRejectMessageEditorWgId.Integer() );
    __PHONELOG1(
        EBasic,
        EPhoneControl,
        "CPhoneStateIncoming::SoftRejectMessageEditorIsDisplayedL() ForegroundAppGroupId %d",
        foregroundAppParam.Integer() );
    // Return ETrue if soft reject message editor is displayed
    return softRejectMessageEditorWgId.Integer() == foregroundAppParam.Integer();
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneStateIncoming::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneStateIncoming::OpenMenuBarL ()" );
    TInt resourceId;

    // Determine the correct menu bar to display
    if ( CPhoneState::IsNumberEntryVisibleL() )
        {
        resourceId = GetNumberEntryVisibleMenuBar();
        }
    else
        {
        resourceId = GetNumberEntryNotVisibleMenuBar();
        }

    // Silence the ringer. And stop vibrating, if it is active.
    iViewCommandHandle->ExecuteCommandL( EPhoneViewStopRingTone );
    iStateMachine->SendPhoneEngineMessage(
        MPEPhoneModel::EPEMessageStopTonePlay );

    //Set correct cba
    HandleAudioPlayStoppedL();
    
    // Dim button
    SetTouchPaneButtonDisabled( EPhoneCallComingCmdSilent );

    // Open the menu bar
    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger(
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen,
        &integerParam );
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::GetNumberEntryVisibleMenuBar
// -----------------------------------------------------------
//
TInt CPhoneStateIncoming::GetNumberEntryVisibleMenuBar()
    {
    if( CPhoneState::IsVideoCall ( iRingingCallId ) )
        {
        return EPhoneIncomingVideoCallMenubarWithNumberEntry;
        }
    else
        {
        return EPhoneIncomingCallMenubarWithNumberEntry;
        }
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::GetNumberEntryNotVisibleMenuBar
// -----------------------------------------------------------
//
TInt CPhoneStateIncoming::GetNumberEntryNotVisibleMenuBar()
    {
    if( CPhoneState::IsVideoCall ( iRingingCallId ) )
        {
        return EPhoneIncomingVideoCallMenubar;
        }
    else
        {
        return EPhoneIncomingCallMenubar;
        }
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::DynInitMenuPaneL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIncoming::DynInitMenuPaneL(
    TInt aResourceId,
    CEikMenuPane* aMenuPane )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIncoming::DynInitMenuPaneL() ");
    __ASSERT_DEBUG( aMenuPane && aResourceId,
        Panic( EPhoneCtrlParameterNotInitialized ) );

    // Save the number of digits in the number entry before processing
    // the menu pane
    if ( IsNumberEntryUsedL() )
        {
        TPhoneCmdParamBoolean serviceCodeParam;
        serviceCodeParam.SetBoolean( ETrue );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetServiceCodeFlag,
            &serviceCodeParam );
        }

    if ( iCustomization )
        {
        iCustomization->CustomizeMenuPaneL(aResourceId, aMenuPane);
        }
    // Process the menu pane
    TPhoneCmdParamDynMenu dynMenuPane;
    dynMenuPane.SetResourceId( aResourceId );
    dynMenuPane.SetDynMenu( aMenuPane );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuPane, &dynMenuPane );
    }

// -----------------------------------------------------------
// CPhoneStateIncoming::ShowDisconnectingL
// -----------------------------------------------------------
//
void CPhoneStateIncoming::ShowDisconnectingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIncoming::ShowDisconnectingL( ) ");
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateDisconnecting );

    TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
    TInt callLabelId = CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneInCallDisconnected );

    StringLoader::Load( labelText, callLabelId, CCoeEnv::Static() );
    callHeaderParam.SetLabelText( labelText );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );
    }

// ---------------------------------------------------------
// CPhoneStateIncoming::HandleKeyLockEnabledL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneStateIncoming::HandleKeyLockEnabled( TBool aKeylockEnabled )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIncoming::HandleKeyLockEnabledL( ) ");
    if( !FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke )
        && CPhoneCenRepProxy::Instance()->
            IsTelephonyFeatureSupported( KTelephonyLVFlagDisableCallControlHardKeysWhileLocked ) )
        {
        if( aKeylockEnabled )
            {
            // Keylock enabled
            if( iStateMachine->PhoneStorage()->IsBlockedKeysListEmpty() )
                {
                // Disable HW Keys if needed
                DisableHWKeysL();
                }
            }
        else
            {
            // Keylock disabled
            // Reset blocked keys list
            iStateMachine->PhoneStorage()->ResetBlockedKeysList();
            }
        }
    }

// End of File
