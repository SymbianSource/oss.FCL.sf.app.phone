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
* Description: Implementation of CPhoneConferenceAndSingleAndWaiting class.
*
*/


// INCLUDES
#include <StringLoader.h>
#include <cpephonemodelif.h>
#include "cphoneconferenceandsingleandwaiting.h"
#include "mphonestatemachine.h"
#include "phoneviewcommanddefinitions.h"
#include "tphonecmdparamcallheaderdata.h"
#include "phonestatedefinitionsgsm.h"
#include "phonelogger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamcallstatedata.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "tphonecmdparamglobalnote.h"
#include "phoneui.hrh"
#include "mphonestorage.h"
#include "cphonecenrepproxy.h"
#include <telephonyvariant.hrh>

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneConferenceAndSingleAndWaiting::CPhoneConferenceAndSingleAndWaiting(
    MPhoneStateMachine* aStateMachine,
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) :
    CPhoneConferenceAndSingle( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::~CPhoneConferenceAndSingleAndWaiting()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConferenceAndSingleAndWaiting::~CPhoneConferenceAndSingleAndWaiting()
    {
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::ConstructL()
    {
    CPhoneConferenceAndSingle::ConstructL();
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConferenceAndSingleAndWaiting* CPhoneConferenceAndSingleAndWaiting::NewL(
    MPhoneStateMachine* aStateMachine,
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneConferenceAndSingleAndWaiting* self = new( ELeave ) CPhoneConferenceAndSingleAndWaiting(
        aStateMachine, aViewCommandHandle, aPhoneCustomization );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::HandlePhoneEngineMessageL(
    const TInt aMessage,
    TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneConferenceAndSingleAndWaiting::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageAddedConferenceMember:
            HandleAddedConferenceMemberL( aCallId );
            break;

        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;

        default:
            CPhoneConferenceAndSingle::HandlePhoneEngineMessageL( aMessage,
                aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneConferenceAndSingleAndWaiting::HandleIdleL()");

    BeginUiUpdateLC();
    // Set touch controls
    SetTouchPaneButtons( EPhoneWaitingCallButtons );


    TPhoneCmdParamBoolean conferenceExistsForCallId;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetCallExistsInConference,
        aCallId, &conferenceExistsForCallId );

    if( conferenceExistsForCallId.Boolean() )
        {
        // Remove conference member from conference bubble
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveFromConference,
            aCallId );
        }
    else
        {
        // Remove call
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

        TPhoneCmdParamBoolean conferenceBubbleExists;
        iViewCommandHandle->ExecuteCommandL( EPhoneViewGetIsConference,
            &conferenceBubbleExists );
        if( !conferenceBubbleExists.Boolean() )
            {
            // Idle message came for conference member.
            MakeTransitionAccordingToActiveCallsL();
            }
        else
            {
            TPhoneCmdParamCallStateData callStateData;
            callStateData.SetCallState( EPEStateRinging );
            iViewCommandHandle->HandleCommandL( EPhoneViewGetCallIdByState,
                &callStateData );
            if( callStateData.CallId() >= 0 )
                {
                // We have Conference and Waiting calls left
                MakeStateTransitionToConferenceAndWaitingL( aCallId );
                }
            else
                {
                // We have Conference and Single calls left
                MakeStateTransitionToConferenceAndSingleL( aCallId );
                }
            }
        }
    EndUiUpdate();
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::HandleAddedConferenceMemberL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::HandleAddedConferenceMemberL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingleAndWaiting::HandleAddedConferenceMemberL");
    // Update conference bubble
    iViewCommandHandle->ExecuteCommandL( EPhoneViewAddToConference );

    UpdateConferenceSecurityStatusL( aCallId );

    MakeStateTransitionToConferenceAndWaitingL( KConferenceCallId );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::MakeStateTransitionToConferenceAndWaitingL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::MakeStateTransitionToConferenceAndWaitingL( TInt /*aCallId*/ )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneConferenceAndSingleAndWaiting::MakeStateTransitionToConferenceAndWaitingL()");

    SetTouchPaneButtons( EPhoneWaitingCallButtons );

    // Check if HW Keys or Call UI should be disabled
    CheckDisableHWKeysAndCallUIL();

    // Go to Conference And Waiting state
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
    iStateMachine->ChangeState( EPhoneStateConferenceAndWaiting );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::MakeStateTransitionToConferenceAndSingleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::MakeStateTransitionToConferenceAndSingleL( TInt /*aCallId*/ )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneConferenceAndSingleAndWaiting::MakeStateTransitionToConferenceAndSingleL()");

    if( /*FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke )
        */ 1 &&  iStateMachine->PhoneStorage()->IsScreenLocked() )
        {
        EnableCallUIL();
        }

    // Reset blocked keys list
    iStateMachine->PhoneStorage()->ResetBlockedKeysList();

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
        UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
        // If numberentry is not open just check NeedToSendToBackgroundL and
        // sendbackround if needed.
        if ( NeedToSendToBackgroundL() )
            {
            // Return phone to the background if send to background is needed.
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
            }
        }

    SetTouchPaneButtons( EPhoneConferenceAndSingleButtons );
    // Go to conference and single state
    // CBA updates in above if-else conditions
    iStateMachine->ChangeState( EPhoneStateConferenceAndSingle );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::HandleConferenceIdleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::HandleConferenceIdleL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneConferenceAndSingleAndWaiting::HandleConferenceIdleL()");

    BeginUiUpdateLC();

    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveConferenceBubble );

    // Remove any phone dialogs if they are displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );

    MakeTransitionAccordingToActiveCallsL();

    EndUiUpdate();
    }

// --------------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::HandleKeyMessageL
// --------------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::HandleKeyMessageL(
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneConferenceAndSingleAndWaiting::HandleKeyMessageL()");
    switch ( aCode )
        {
        // send-key
        case EKeyYes:
            if( IsNumberEntryVisibleL() )
                {
                CallFromNumberEntryL();
                }
            else
                {
                // Not allowed to answer
                SendGlobalWarningNoteL(
                    EPhoneNoteTextCallNotAllowed, ETrue );
                }
            break;

        default:
            // do base operation
            CPhoneConferenceAndSingle::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::HandleKeyEventL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode /*aEventCode*/ )
    {
    if( EKeyDeviceF == aKeyEvent.iCode )
        {
        __PHONELOG( EBasic, EPhoneUIStates,
            "CPhoneConferenceAndSingleAndWaiting::HandleKeyMessageL-deviceF" );
        HandleHoldSwitchL();
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneConferenceAndSingleAndWaiting::UpdateInCallCbaL() ");

    UpdateCbaL ( EPhoneCallHandlingCallWaitingCBA );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::MakeTransitionAccordingToActiveCallsL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::MakeTransitionAccordingToActiveCallsL()
    {
    TPhoneCmdParamInteger activeCallCount;
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewGetCountOfActiveCalls, &activeCallCount );

    switch( activeCallCount.Integer() )
         {
         case ENoActiveCalls: // Go to incoming state
            {
            // Get Waiting call's Call Id
            TPhoneCmdParamCallStateData callStateData;
            callStateData.SetCallState( EPEStateRinging );
            iViewCommandHandle->HandleCommandL( EPhoneViewGetCallIdByState,
                &callStateData );

            if ( callStateData.CallId() != KErrNotFound )
                {
                // Display ringing bubble
                TPhoneCmdParamCallHeaderData callHeaderParam;
                callHeaderParam.SetCallState( EPEStateRinging );

                SetCallHeaderTextsForCallComingInL( callStateData.CallId(), EFalse, &callHeaderParam );

                iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble,
                    callStateData.CallId(),
                    &callHeaderParam );

                // Capture keys when there is an incoming call
                CaptureKeysDuringCallNotificationL( ETrue );

                // Go to Incoming state
                iCbaManager->UpdateIncomingCbaL( callStateData.CallId() );

                // Check if HW Keys or Call UI should be disabled
                CheckDisableHWKeysAndCallUIL();

                SetTouchPaneButtons( EPhoneIncomingCallButtons );
                SetRingingTonePlaybackL( callStateData.CallId() );
                SetBackButtonActive(EFalse);
                iStateMachine->ChangeState( EPhoneStateIncoming );
                }
            else
                {
                SetDefaultFlagsL();
                UpdateCbaL( EPhoneEmptyCBA );
                iStateMachine->ChangeState( EPhoneStateIdle );
                }
            }
            break;
         case EOneActiveCall:
            {
            // Go to Single And Waiting state
            UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
            SetTouchPaneButtons( EPhoneWaitingCallButtons );

            // Check if HW Keys or Call UI should be disabled
            CheckDisableHWKeysAndCallUIL();

            iStateMachine->ChangeState( EPhoneStateWaitingInSingle );
            }
            break;
         case ETwoActiveCalls:
            {
            // Go to Two Singles And Waiting state
            UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );

            // Check if HW Keys or Call UI should be disabled
            CheckDisableHWKeysAndCallUIL();

            SetTouchPaneButtons( EPhoneWaitingCallButtons );
            iStateMachine->ChangeState( EPhoneStateTwoSinglesAndWaiting );
            }
            break;
         default:
           // Too many active calls to handle reliable state change now.
           // Waiting for all conference member idles
           // This case may happen when 'End all calls' because conferenceIdle may
           // come earlier than idle for conference member
            break;
         }

    }

// End of File
