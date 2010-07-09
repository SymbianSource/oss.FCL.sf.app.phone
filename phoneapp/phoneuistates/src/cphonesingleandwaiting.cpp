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
* Description: Implementation of CPhoneSingleAndWaiting class.
*
*/


// INCLUDES
#include <eikmenub.h>
#include <eikenv.h>
#include <StringLoader.h>
#include <featmgr.h>
#include <pevirtualengine.h>
#include <mpeengineinfo.h>
#include <telephonyvariant.hrh>
#include <cpephonemodelif.h>
#include "cphonesingleandwaiting.h"
#include "mphonestatemachine.h"
#include "phoneviewcommanddefinitions.h"
#include "phoneui.hrh"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "cphonegsmincall.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamcallstatedata.h"
#include "phonestatedefinitionsgsm.h"
#include "phonelogger.h"
#include "cphonecenrepproxy.h"
#include "cphonepubsubproxy.h"
#include "mphonestorage.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneSingleAndWaiting::CPhoneSingleAndWaiting(
    MPhoneStateMachine* aStateMachine,
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) :
    CPhoneGsmInCall( aStateMachine, aViewCommandHandle, aPhoneCustomization ),
    iSingleCallId ( KErrNotFound )
    {
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::~CPhoneSingleAndWaiting()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneSingleAndWaiting::~CPhoneSingleAndWaiting()
    {

    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::ConstructL()
    {
    CPhoneGsmInCall::ConstructL();

    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnected );

    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );

    if ( callStateData.CallId() == KErrNotFound )
        {
        callStateData.SetCallState( EPEStateHeld );
        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );
        }
    iSingleCallId = callStateData.CallId();

    __PHONELOG1( EBasic, EPhoneUIStates,
        "CPhoneSingleAndWaiting::ConstructL()  (iSingleCallId=%d)", iSingleCallId);
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneSingleAndWaiting* CPhoneSingleAndWaiting::NewL(
    MPhoneStateMachine* aStateMachine,
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneSingleAndWaiting* self = new( ELeave ) CPhoneSingleAndWaiting(
        aStateMachine, aViewCommandHandle, aPhoneCustomization );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::HandlePhoneEngineMessageL(
    const TInt aMessage,
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::HandlePhoneEngineMessageL() ");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;

        case MEngineMonitor::EPEMessageHeld:
            HandleHeldL( aCallId );
            break;

        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;

        case MEngineMonitor::EPEMessageDialing:
            HandleDiallingL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageDisconnecting:
            HandleDisconnectingL( aCallId );
            /* Flow through */

        default:
            CPhoneGsmInCall::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting:HandleCommandL
// -----------------------------------------------------------
//
TBool CPhoneSingleAndWaiting::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::HandleCommandL() ");
    TBool commandStatus = ETrue;

    switch( aCommand )
        {
        case EPhoneInCallCmdHold:
            {
            iStateMachine->SetCallId( iSingleCallId );
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessageHold );
            break;
            }

        case EPhoneInCallCmdUnhold:
            {
            iStateMachine->SetCallId( iSingleCallId );
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessageResume );
            break;
            }

        case EPhoneInCallCmdNewCall:
            break;

        case EPhoneInCallCmdNewCallCall:
            break;

        case EPhoneCallComingCmdReject: // fall through
            {
            // Get waiting callid
            TPhoneCmdParamCallStateData callStateData;
            callStateData.SetCallState( EPEStateRinging );
            iViewCommandHandle->HandleCommandL( EPhoneViewGetCallIdByState,
                &callStateData );
            iStateMachine->SetCallId( callStateData.CallId() );
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageReject );
            break;
            }


        case EPhoneInCallCmdReject:
            iStateMachine->SetCallId( iSingleCallId );
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageReject );
            break;

        case EPhoneCmdUpdateUiControls:
            UpdateUiControlsL();
            break;

        default:
            commandStatus = CPhoneGsmInCall::HandleCommandL( aCommand );
            break;
        }

    return commandStatus;
    }

// --------------------------------------------------------------
// CPhoneSingleAndWaiting::HandleKeyMessageL
// --------------------------------------------------------------
//
void CPhoneSingleAndWaiting::HandleKeyMessageL(
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::HandleKeyMessageL() ");
    switch ( aCode )
        {
        // send-key
        case EKeyYes:
            {
            if( IsNumberEntryVisibleL() )
                {
                // Send a manual control sequence by providing number
                // information with Dial command
                CallFromNumberEntryL();
                }
             else
                {
                // Close DTMF sending note if it exists
                iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );

                // Number entry is below incoming call bubble so
                // answer the call
                iStateMachine->SendPhoneEngineMessage(
                    CPEPhoneModelIF::EPEMessageAnswer );
                }
            break;
            }

        default:
            // do base operation
            CPhoneGsmInCall::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::HandleKeyEventL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode /*aEventCode*/ )
    {
    if( EKeyDeviceF == aKeyEvent.iCode )
        {
        __PHONELOG( EBasic, EPhoneUIStates,
            "CPhoneSingleAndWaiting::HandleKeyMessageL-deviceF" );
        HandleHoldSwitchL();
        }
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::HandleIdleL() ");

    if ( iSingleCallId == KErrNotFound )
        {
        TPhoneCmdParamCallStateData callStateData;
        callStateData.SetCallState( EPEStateDisconnecting );

        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );

        if ( callStateData.CallId() == KErrNotFound )
            {
            callStateData.SetCallState( EPEStateConnected );
            iViewCommandHandle->HandleCommandL(
                EPhoneViewGetCallIdByState, &callStateData );

            if ( callStateData.CallId() == KErrNotFound )
                {
                callStateData.SetCallState( EPEStateHeld );
                iViewCommandHandle->HandleCommandL(
                    EPhoneViewGetCallIdByState, &callStateData );
                }
            }

        iSingleCallId = callStateData.CallId();
        }

    __PHONELOG1( EBasic, EPhoneControl, 
        "CPhoneSingleAndWaiting::HandleIdleL iSingleCallId(%d)",
        iSingleCallId );
    __PHONELOG1( EBasic, EPhoneControl, 
        "CPhoneSingleAndWaiting::HandleIdleL aCallId(%d)",
        aCallId );
    
    if( iSingleCallId == aCallId )
        {
        // Idle message came for active call
        BeginUiUpdateLC();

        // Remove call
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

        // Get waiting callId
        TPhoneCmdParamCallStateData callStateData;
        callStateData.SetCallState( EPEStateRinging );
        iViewCommandHandle->HandleCommandL( EPhoneViewGetCallIdByState,
            &callStateData );
        
        __PHONELOG1( EBasic, EPhoneControl, 
            "CPhoneSingleAndWaiting::HandleIdleL EPEStateRinging CallId(%d)",
            callStateData.CallId() );
        
        if ( callStateData.CallId() > KErrNotFound )
            {
            // Display ringing bubble
            TPhoneCmdParamCallHeaderData callHeaderParam;
            callHeaderParam.SetCallState( EPEStateRinging );

            // Set Hold flag to view
            TPhoneCmdParamBoolean holdFlag;
            holdFlag.SetBoolean( EFalse );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
            
            SetCallHeaderTextsForCallComingInL( callStateData.CallId(), EFalse, &callHeaderParam );
            
            iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble,
                callStateData.CallId(),
                &callHeaderParam );

            // Hide number entry if visible
            if ( IsNumberEntryVisibleL() )
                {
                SetNumberEntryVisibilityL(EFalse);
                }

            // Check if HW Keys or Call UI should be disabled
            CheckDisableHWKeysAndCallUIL();
            
            SetTouchPaneButtons( EPhoneIncomingCallButtons );
            
            // Bring up callhandling view
            BringIncomingToForegroundL();

            //Set incoming cba
            iCbaManager->UpdateIncomingCbaL( callStateData.CallId() );
            
            // Play ringtone
            SetRingingTonePlaybackL( callStateData.CallId() );
            }

        SetBackButtonActive(EFalse);
        EndUiUpdate();

        iStateMachine->ChangeState( EPhoneStateIncoming );
        }
    else
        {

        BeginUiUpdateLC();

        // Remove call
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

        // Idle message came for waiting call
        if ( IsNumberEntryUsedL() )
            {
            if ( NeedToSendToBackgroundL() )
                {
                // Return phone to the background if send to background is needed.
                iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );

                UpdateCbaL( EPhoneCallHandlingInCallCBA );
                }
            else
                {
                // Show the number entry if it exists
                SetNumberEntryVisibilityL(ETrue);
                }
            }
        else
            {
            UpdateCbaL( EPhoneCallHandlingInCallCBA );
            // If numberentry is not open just check NeedToSendToBackgroundL and
            // sendbackround if needed.
            if ( NeedToSendToBackgroundL() )
                {
                // Return phone to the background if send to background is needed.
                iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
                }
            }
        
        // Enable call UI
        if( FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke ) 
            && iStateMachine->PhoneStorage()->IsScreenLocked() )
            {
            EnableCallUIL();
            }

        // Reset blocked keys list
        iStateMachine->PhoneStorage()->ResetBlockedKeysList();

        SetTouchPaneButtons( EPhoneIncallButtons );
        EndUiUpdate();

        // CBA updates in above if-else conditions
        iStateMachine->ChangeState( EPhoneStateSingle );
        }

    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::HandleHeldL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::HandleHeldL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::HandleHeldL() ");

     // Display hold bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateHeld );

    TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
    TInt callLabelId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCallOnHold );

    StringLoader::Load( labelText, callLabelId, CCoeEnv::Static() );        
    callHeaderParam.SetLabelText( labelText );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );

    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    if ( !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        HandleHoldNoteL( aCallId, ETrue );
        }

    UpdateInCallCbaL();
    
    SetTouchPaneButtons( EPhoneWaitingCallButtons ); 
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::HandleConnectedL() ");

    __PHONELOG1( EBasic, EPhoneUIStates,
        "CPhoneSingleAndWaiting::HandleConnectedL  (CallId=%d)", aCallId);

    if ( iSingleCallId == aCallId )
        {
        // Connected message came for the hold call, we still
        // have the waiting call also
        HandleUnholdL( aCallId );
        }
    else
        {
        // Connected message came for waiting call
        MakeStateTransitionToTwoSinglesL( aCallId );
        }
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::HandleUnholdL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::HandleUnholdL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::HandleUnholdL() ");

    // Show bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );

    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    if ( !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        HandleHoldNoteL( aCallId, EFalse );
        }

    // Set CBAs
    UpdateInCallCbaL();
    
    SetTouchPaneButtons( EPhoneWaitingCallButtons ); 
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::MakeStateTransitionToTwoSinglesL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::MakeStateTransitionToTwoSinglesL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::MakeStateTransitionToTwoSinglesL() ");
    
    // Reset blocked keys list
    iStateMachine->PhoneStorage()->ResetBlockedKeysList();
    
    BeginUiUpdateLC();

    UpdateRemoteInfoDataL ( aCallId );
    // Show bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );

    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    if ( IsNumberEntryUsedL() )
        {
        if ( NeedToSendToBackgroundL() )
            {
            // Return phone to the background if send to background is needed.
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );

            UpdateCbaL( EPhoneCallHandlingInCallCBA );
            }
        else
            {
            // Show the number entry if it exists.
            SetNumberEntryVisibilityL(ETrue);
            }
        }
    else
        {
        // Set Two singles softkeys
        UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
        }

    SetTouchPaneButtons( EPhoneTwoSinglesButtons );
    EndUiUpdate();

    // Go to two singles state
    // CBA updates in above if-else conditions
    iStateMachine->ChangeState( EPhoneStateTwoSingles );
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::HandleDiallingL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::HandleDiallingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::HandleDiallingL() ");

    BeginUiUpdateLC();

    // Hide number entry if it exists
    SetNumberEntryVisibilityL(EFalse);

    // Display call setup
    DisplayCallSetupL( aCallId );

    // Set Hold flag to view ( DTMF menu label have to present )
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    SetTouchPaneButtons( EPhoneCallSetupButtons );

    EndUiUpdate();

    // Update call setup CBAs
    UpdateCbaL( EPhoneCallHandlingCallSetupCBA );

    // Go to Single And Waiting And Call Setup state
    iStateMachine->ChangeState( EPhoneStateSingleAndCallSetupAndWaiting );
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::DisplayCallSetupL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::DisplayCallSetupL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::DisplayCallSetupL() ");

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
// CPhoneSingleAndWaiting::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::UpdateInCallCbaL() ");
    TPhoneCmdParamInteger callIdParam;
    iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewGetExpandedBubbleCallId, &callIdParam );

    if ( callIdParam.Integer()==iSingleCallId )
        {
        CPhoneGsmInCall::UpdateInCallCbaL();
        } 
    else 
        {
        iCbaManager->SetCbaL(  EPhoneCallHandlingCallWaitingCBA );
        }
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::HandleErrorL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneSingleAndWaiting::HandleErrorL( const TPEErrorInfo& aErrorInfo )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndWaiting::HandleErrorL()");

    if( aErrorInfo.iErrorCode == ECCPErrorCCUserAlertingNoAnswer )
        {
        // Should not shown "Not Allowed" note, if active call.
        __PHONELOG1( EBasic, EPhoneUIStates,
            "PhoneUIControl: CPhoneSingleAndWaiting::HandleErrorL - aErrorInfo.iErrorCode =%d ",
            aErrorInfo.iErrorCode);
        }
    else
        {
        CPhoneState::HandleErrorL( aErrorInfo );
        }
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::HandleDisconnectingL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::HandleDisconnectingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndWaiting::HandleDisconnectingL( ) ");
    
    if( ( iSingleCallId == aCallId ) && IsVideoCall( aCallId ) )
        {
        __PHONELOG( EBasic, EPhoneControl, 
            "CPhoneSingleAndWaiting::HandleDisconnectingL EPhoneViewSetNeedToSendToBackgroundStatus" );
        
        // Keep phone on the foreground
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewSetNeedToSendToBackgroundStatus,
            &booleanParam );
        }
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::UpdateUiControlsL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::UpdateUiControlsL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndWaiting::UpdateUiControlsL( ) ");
    
    UpdateInCallCbaL();
    }

// End of File
