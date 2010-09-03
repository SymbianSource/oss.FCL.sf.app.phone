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
// -----------------------------------------------------------
//
CPhoneSingleAndWaiting::~CPhoneSingleAndWaiting()
    {
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::ConstructL()
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
        case EKeyYes: // send-key
            {
            if( IsNumberEntryVisibleL() )
                {
                CallFromNumberEntryL();
                }
             else
                {
                // Close DTMF sending note if it exists
                iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );
                iStateMachine->SendPhoneEngineMessage(
                    CPEPhoneModelIF::EPEMessageAnswer );
                }
            break;
            }

        default:
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
    
    // Idle message came for active call
    if( iSingleCallId == aCallId )
        {
        BeginUiUpdateLC();
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

        // Get waiting callId
        TPhoneCmdParamCallStateData callStateData;
        callStateData.SetCallState( EPEStateRinging );
        iViewCommandHandle->HandleCommandL( EPhoneViewGetCallIdByState,
            &callStateData );
        
        __PHONELOG1( EBasic, EPhoneControl, 
            "CPhoneSingleAndWaiting::HandleIdleL EPEStateRinging CallId(%d)",
            callStateData.CallId() );
        
        // Display ringing bubble
        if ( callStateData.CallId() > KErrNotFound )
            {
            TPhoneCmdParamCallHeaderData callHeaderParam;
            callHeaderParam.SetCallState( EPEStateRinging );
            
            TPhoneCmdParamBoolean holdFlag;
            holdFlag.SetBoolean( EFalse );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
            
            SetCallHeaderTextsForCallComingInL( callStateData.CallId(), EFalse, &callHeaderParam );
            
            iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble,
                callStateData.CallId(),
                &callHeaderParam );

            if ( IsNumberEntryVisibleL() )
                {
                SetNumberEntryVisibilityL(EFalse);
                }
            
            CheckDisableHWKeysAndCallUIL();
            
            SetTouchPaneButtons( EPhoneIncomingCallButtons );
            BringIncomingToForegroundL();
            iCbaManager->UpdateIncomingCbaL( callStateData.CallId() );
            SetRingingTonePlaybackL( callStateData.CallId() );
            }
        SetBackButtonActive(EFalse);
        EndUiUpdate();
        iStateMachine->ChangeState( EPhoneStateIncoming );
        }
    else
        {
        BeginUiUpdateLC();
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

        // Idle message came for waiting call
        if ( IsNumberEntryUsedL() )
            {
            if ( NeedToSendToBackgroundL() )
                {
                iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
                UpdateCbaL( EPhoneCallHandlingInCallCBA );
                }
            else
                {
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
                iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
                }
            }
        
        if( FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke ) 
            && iStateMachine->PhoneStorage()->IsScreenLocked() )
            {
            EnableCallUIL();
            }
        iStateMachine->PhoneStorage()->ResetBlockedKeysList();
        SetTouchPaneButtons( EPhoneIncallButtons );
        EndUiUpdate();
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
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateHeld );
    TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
    TInt callLabelId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCallOnHold );

    StringLoader::Load( labelText, callLabelId, CCoeEnv::Static() ); 
    callHeaderParam.SetLabelText( labelText );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );

    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

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
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );

    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

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
    iStateMachine->PhoneStorage()->ResetBlockedKeysList();
    
    BeginUiUpdateLC();

    UpdateRemoteInfoDataL ( aCallId );
    
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );

    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    if ( IsNumberEntryUsedL() )
        {
        if ( NeedToSendToBackgroundL() )
            {
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
            UpdateCbaL( EPhoneCallHandlingInCallCBA );
            }
        else
            {
            SetNumberEntryVisibilityL(ETrue);
            }
        }
    else
        {
        UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
        }

    SetTouchPaneButtons( EPhoneTwoSinglesButtons );
    EndUiUpdate();
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
    SetNumberEntryVisibilityL(EFalse);
    DisplayCallSetupL( aCallId );

    // Set Hold flag to view ( DTMF menu label have to present )
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    SetTouchPaneButtons( EPhoneCallSetupButtons );
    EndUiUpdate();

    UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
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
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
    
    // Force telephony to the foreground
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );
    
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
        UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
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
