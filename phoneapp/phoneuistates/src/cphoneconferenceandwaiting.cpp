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
* Description: Implementation of CPhoneConferenceAndWaiting class.
*
*/


// INCLUDES
#include <StringLoader.h>
#include <cpephonemodelif.h>
#include <featmgr.h>
#include <telephonyvariant.hrh>
#include "cphoneconferenceandwaiting.h"
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
#include "cphonecenrepproxy.h"
#include "mphonestorage.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneConferenceAndWaiting::CPhoneConferenceAndWaiting( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneConference( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::~CPhoneConferenceAndWaiting()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConferenceAndWaiting::~CPhoneConferenceAndWaiting()
    {
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::ConstructL()
    {
    CPhoneConference::ConstructL();
    
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    iRingingCallId = callStateData.CallId();
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConferenceAndWaiting* CPhoneConferenceAndWaiting::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneConferenceAndWaiting* self = new( ELeave ) CPhoneConferenceAndWaiting( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndWaiting::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageDialing:
            HandleDiallingL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageConferenceIdle:
            HandleConferenceIdleL();
            break;
            
        case MEngineMonitor::EPEMessageWentOneToOne:
            HandleWentOneToOneL( aCallId );
            break;

        case MEngineMonitor::EPEMessageHeldConference:            
        case MEngineMonitor::EPEMessageConnectedConference:
            {
            TPhoneCmdParamInteger callIdParam;
            iViewCommandHandle->ExecuteCommandL( 
                        EPhoneViewGetExpandedBubbleCallId, &callIdParam );
            CPhoneConference::HandlePhoneEngineMessageL( aMessage, 
                            aCallId );
            if ( KConferenceCallId == callIdParam.Integer() )
                {
                iViewCommandHandle->ExecuteCommand(EPhoneSetConferenceExpanded);
                }
            }
            break;
            
        default:
            CPhoneConference::HandlePhoneEngineMessageL( aMessage, 
                aCallId );
            break;
        }
    }

// --------------------------------------------------------------
// CPhoneConferenceAndWaiting::HandleKeyMessageL
// --------------------------------------------------------------
//
void CPhoneConferenceAndWaiting::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneConferenceAndWaiting::HandleKeyMessageL()");
    switch ( aCode )
        {
        case EKeyYes:  // send-key
            if( IsNumberEntryVisibleL() )
                {
                CallFromNumberEntryL();
                }
            else
                {
                iStateMachine->SendPhoneEngineMessage(
                    MPEPhoneModel::EPEMessageAnswer ); 
                }
            break;
            
        default:
            CPhoneConference::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::HandleKeyEventL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode /*aEventCode*/ )
    {
    if( EKeyDeviceF == aKeyEvent.iCode )
        {
        __PHONELOG( EBasic, EPhoneUIStates,
            "CPhoneConferenceAndWaiting::HandleKeyMessageL-deviceF" );
        HandleHoldSwitchL();
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneConferenceAndWaiting::HandleConnectedL()");
    TPhoneCmdParamCallStateData callStateData;
    if ( iRingingCallId == KErrNotFound )
        {
        TPhoneCmdParamCallStateData callStateData;
        callStateData.SetCallState( EPEStateRinging );
        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );
        iRingingCallId = callStateData.CallId();   
        }
    
    if ( iRingingCallId == aCallId )
        {
        // not a member of conference call
        MakeStateTransitionToConferenceAndSingleL( aCallId );
        }   
    else
        {
        // member of conference call which is on hold
        TPhoneCmdParamCallHeaderData callHeaderParam;
        callHeaderParam.SetCallState( EPEStateConnected );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
            &callHeaderParam );
        }   
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::MakeStateTransitionToConferenceAndSingleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::MakeStateTransitionToConferenceAndSingleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneConferenceAndWaiting::MakeStateTransitionToConferenceAndSingleL()");
    if( FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke )
        && iStateMachine->PhoneStorage()->IsScreenLocked() )
        {
        EnableCallUIL();
        }
    
    iStateMachine->PhoneStorage()->ResetBlockedKeysList();
    BeginUiUpdateLC();
    UpdateRemoteInfoDataL ( aCallId );
    
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
    
    if ( IsNumberEntryUsedL() )
        {
        if ( NeedToSendToBackgroundL() )
            {
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
    SetTouchPaneButtons( EPhoneConferenceAndSingleButtons );
    EndUiUpdate();
    iStateMachine->ChangeState( EPhoneStateConferenceAndSingle );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneConferenceAndWaiting::HandleIdleL()");
    if ( iRingingCallId == KErrNotFound )
        {
        TPhoneCmdParamCallStateData callStateData;
        callStateData.SetCallState( EPEStateRinging );
        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );
        iRingingCallId = callStateData.CallId();   
        }
    
    if( iRingingCallId == aCallId )
        {
        
        BeginUiUpdateLC();
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
            
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
                // Return phone to the background if send to background is needed.
                iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
                }
            }
        
        if( FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke ) 
            && iStateMachine->PhoneStorage()->IsScreenLocked() )
            {
            EnableCallUIL();
            }
        iStateMachine->PhoneStorage()->ResetBlockedKeysList();
        SetTouchPaneButtons( EPhoneConferenceButtons );
        EndUiUpdate();
        iStateMachine->ChangeState( EPhoneStateConference );
        }
    else
        {
        // Idle message came for conference member
        CPhoneConference::HandleIdleL( aCallId ); 
        
        TPhoneCmdParamBoolean conferenceBubbleExists;
        iViewCommandHandle->ExecuteCommandL( 
                EPhoneViewGetIsConference,
                &conferenceBubbleExists );
        if( !conferenceBubbleExists.Boolean() )
            {
            HandleConferenceIdleL();
            }
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::HandleConferenceIdleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::HandleConferenceIdleL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndWaiting::HandleConferenceIdleL()");
    BeginUiUpdateLC();
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveConferenceBubble );
    
    TPhoneCmdParamInteger activeCallCount;
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewGetCountOfActiveCalls, &activeCallCount );
    switch( activeCallCount.Integer() )
        {
        case ENoActiveCalls: // Go to incoming state
            {
            TPhoneCmdParamCallHeaderData callHeaderParam;
            callHeaderParam.SetCallState( EPEStateRinging );
            
            // Set Hold flag to view
            TPhoneCmdParamBoolean holdFlag;
            holdFlag.SetBoolean( EFalse );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
              
            BringIncomingToForegroundL(); 
            SetCallHeaderTextsForCallComingInL( iRingingCallId, EFalse, &callHeaderParam );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, 
                iRingingCallId ,&callHeaderParam );
            iCbaManager->UpdateIncomingCbaL( iRingingCallId );
            
            if ( IsNumberEntryVisibleL() )
                {
                SetNumberEntryVisibilityL(EFalse);   
                }
            CheckDisableHWKeysAndCallUIL();
            
            SetTouchPaneButtons( EPhoneIncomingCallButtons );
            SetRingingTonePlaybackL( iRingingCallId );
            SetBackButtonActive(EFalse);
            iStateMachine->ChangeState( EPhoneStateIncoming );
            }
            break;
            
        case EOneActiveCall:
            if ( IsNumberEntryUsedL() )
                {
                SetNumberEntryVisibilityL(ETrue);
                }
            UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
            CheckDisableHWKeysAndCallUIL();
            SetTouchPaneButtons( EPhoneWaitingCallButtons );
            iStateMachine->ChangeState( EPhoneStateWaitingInSingle );
            break;
            
        case ETwoActiveCalls:
            if ( IsNumberEntryUsedL() )
                {
                SetNumberEntryVisibilityL(ETrue);
                }
            UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
            CheckDisableHWKeysAndCallUIL();
            SetTouchPaneButtons( EPhoneWaitingCallButtons );
            iStateMachine->ChangeState( EPhoneStateTwoSinglesAndWaiting );
            break; 
            
        default:
          // Too many active calls to handle reliable state change now.
          // Waiting for all conference member idles
          // This case may happen when 'End all calls' because conferenceIdle may 
          // come earlier than idle for conference member
           break;
        }
    EndUiUpdate();
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndWaiting::UpdateInCallCbaL() ");
    UpdateCbaL ( EPhoneCallHandlingCallWaitingCBA );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::HandleDiallingL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::HandleDiallingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndWaiting::HandleDiallingL()");
    BeginUiUpdateLC();
    SetNumberEntryVisibilityL(EFalse);
    DisplayCallSetupL( aCallId );
    SetTouchPaneButtons( EPhoneCallSetupButtons );
    EndUiUpdate();
    UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
    iStateMachine->ChangeState( EPhoneStateConferenceAndWaitingAndCallSetup );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::HandleWentOneToOneL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::HandleWentOneToOneL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndWaiting::HandleWentOneToOneL()");
    BeginUiUpdateLC();
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewPrivateFromConference, aCallId );
    
    if ( iRingingCallId == KErrNotFound )
        {
        TPhoneCmdParamCallStateData callStateData;
        callStateData.SetCallState( EPEStateRinging );
        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );
        iRingingCallId = callStateData.CallId();
        }
        
    TPhoneCmdParamBoolean conferenceBubbleExists;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetIsConference, 
        &conferenceBubbleExists );
    if( conferenceBubbleExists.Boolean() && 
        ( iRingingCallId != KErrNotFound ) )
        {
        SetTouchPaneButtons( EPhoneWaitingCallButtons );
        UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
        iStateMachine->ChangeState( EPhoneStateConferenceAndSingleAndWaiting );  
        }
    EndUiUpdate();
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::HandleCommandL
// -----------------------------------------------------------
//
TBool CPhoneConferenceAndWaiting::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneConferenceAndWaiting::HandleCommandL() ");
    TBool commandStatus = ETrue;
    switch( aCommand )
        {
        case EPhoneCmdUpdateUiControls:
            UpdateUiControlsL();
            break;

        default:
            commandStatus = CPhoneConference::HandleCommandL( aCommand );
            break;
        }
    return commandStatus;
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::UpdateUiControlsL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::UpdateUiControlsL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndWaiting::UpdateUiControlsL( ) ");
    iViewCommandHandle->ExecuteCommand(EPhoneSetConferenceExpanded);
    UpdateCbaL( EPhoneCallHandlingInCallCBA );
    }

// End of File
