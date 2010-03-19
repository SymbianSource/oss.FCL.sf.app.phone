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
* Description: Implementation of CPhoneSingleAndCallSetupAndWaiting class.
*
*/


// INCLUDES
#include <eikmenub.h> 
#include <eikenv.h>
#include <StringLoader.h>
#include <pevirtualengine.h>
#include <cpephonemodelif.h>
#include "cphonesingleandcallsetupandwaiting.h"
#include "mphonestatemachine.h"
#include "phoneviewcommanddefinitions.h"
#include "phoneui.hrh"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "cphonestateincall.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamcallstatedata.h"
#include "phonestatedefinitionsgsm.h"
#include "phonelogger.h"

enum TTerminatedCall
{
    ENull = 0,
    EHold,
    ERinging,
    EAlerting    
};

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneSingleAndCallSetupAndWaiting::CPhoneSingleAndCallSetupAndWaiting( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneGsmInCall( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting::~CPhoneSingleAndCallSetupAndWaiting()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneSingleAndCallSetupAndWaiting::~CPhoneSingleAndCallSetupAndWaiting()
    {
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetupAndWaiting::ConstructL()
    {
    CPhoneGsmInCall::ConstructL();

    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    iWaitingCallId = callStateData.CallId();
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneSingleAndCallSetupAndWaiting* CPhoneSingleAndCallSetupAndWaiting::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneSingleAndCallSetupAndWaiting* self = new( ELeave ) CPhoneSingleAndCallSetupAndWaiting( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// --------------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting::HandleKeyMessageL
// --------------------------------------------------------------
//
void CPhoneSingleAndCallSetupAndWaiting::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndCallSetupAndWaiting::HandleKeyMessageL() ");
    switch ( aCode )
        {
        // send-key
        case EKeyYes:
            if( IsNumberEntryVisibleL() )
                {
                CPhoneState::CallFromNumberEntryL();                            
                }
            else
                {
                // Number entry is behind waiting call bubble
                // We can't answer to waiting call in this state
                // so display Not allowed -note.                        
                CPhoneState::SendGlobalErrorNoteL( 
                    EPhoneNoteTextNotAllowed );
                }
            break;
            
        // end-key
        case EKeyNo:
            if ( aMessage == EPhoneKeyLongPress )
                {
                // Close all connections
                iStateMachine->SendPhoneEngineMessage(
                    MPEPhoneModel::EPEMessageTerminateAllConnections );
                 
                if ( IsNumberEntryContentStored() )
                    {
                    ClearNumberEntryContentCache();
                    }
                
                if ( CPhoneState::IsNumberEntryUsedL() )
                    {
                    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
                        {
                        CloseDTMFEditorL();
                        } 
                    else
                        {
                        // Remove number entry from screen
                        iViewCommandHandle->ExecuteCommandL( 
                            EPhoneViewRemoveNumberEntry );
                        // Do state-specific operation when number entry is cleared
                        HandleNumberEntryClearedL();
                        }
                    }
                if ( !TopAppIsDisplayedL() )
                    {
                    // Bring app to foreground
                    TPhoneCmdParamInteger uidParam;
                    uidParam.SetInteger( KUidPhoneApplication.iUid );
                    iViewCommandHandle->ExecuteCommandL(
                        EPhoneViewBringAppToForeground, &uidParam );
                    }
                }
            else
                {
                // handle end key
                DisconnectOutgoingCallL();              
                }
            break;
            
        default:
            // do base operation
            CPhoneGsmInCall::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetupAndWaiting::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndCallSetupAndWaiting::HandlePhoneEngineMessageL() ");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageConnecting:
            iAlerting = ETrue;
            HandleConnectingL( aCallId );
            break;
        
        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;
        
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;
                    
        case MEngineMonitor::EPEMessageRemoteBusy:
            // If call setup failed then stop capturing keys.
            // Flow through to default branch.
            CaptureKeysDuringCallNotificationL( EFalse );     
        default:
            CPhoneGsmInCall::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }


// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting:HandleCommandL
// -----------------------------------------------------------
//
TBool CPhoneSingleAndCallSetupAndWaiting::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndCallSetupAndWaiting::HandleCommandL() ");
    TBool commandStatus = ETrue;

    switch( aCommand )
        {               
        // 'End active call' from menu
        case EPhoneInCallCmdEndThisActiveCall:
            DisconnectOutgoingCallL();
            break;
        
        // 'End all calls' from menu
        case EPhoneInCallCmdEndAllCalls:
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageReleaseAll );
            break;
            
        case EPhoneDtmfDialerCancel:
            {
            CloseDTMFEditorL();
            }
            break;    
            
        default:
            commandStatus = CPhoneGsmInCall::HandleCommandL( aCommand );
            break;
        }

    return commandStatus;
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetupAndWaiting::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndCallSetupAndWaiting::OpenMenuBarL() ");
    TInt resourceId;

    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
        {
        resourceId = EPhoneDtmfDialerMenubar;
        }
    else if ( IsNumberEntryVisibleL() )
        {
        resourceId = EPhoneAlertingHeldAndWaitingCallMenuBarWithNumberEntry;
        }
    else
        {
        resourceId = EPhoneAlertingHeldAndWaitingCallMenuBar;
        }

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting:HandleIdleL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetupAndWaiting::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndCallSetupAndWaiting::HandleIdleL() ");
    // Fetch call ids for hold, ringing and alerting bubbles
    TInt holdCallId;
    TInt ringingCallId;
    TInt alertingCallId;
    TPhoneViewResponseId heldResponse;
    TPhoneViewResponseId ringingResponse;
    TPhoneViewResponseId alertingResponse;
    TPhoneCmdParamCallStateData callStateData;
    
    // Remove call 
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
    
    callStateData.SetCallState( EPEStateHeld );
    heldResponse = iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    holdCallId = callStateData.CallId();
    
    callStateData.SetCallState( EPEStateRinging );
    ringingResponse = iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    ringingCallId = callStateData.CallId();
                
    callStateData.SetCallState( EPEStateConnecting );
    alertingResponse = iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    alertingCallId = callStateData.CallId();
    
    if( heldResponse != EPhoneViewResponseSuccess ||
        ringingResponse != EPhoneViewResponseSuccess ||
        alertingResponse != EPhoneViewResponseSuccess )
        {
        // Some or all call id fetches were unsuccesfull.
        // Terminate all connections and return to idle state.
        
        // TBD: Kill all bubbles here

        iStateMachine->SendPhoneEngineMessage(
            MPEPhoneModel::EPEMessageTerminateAllConnections );
            
        CPhoneGsmInCall::HandleIdleL( aCallId );            
        return;
        }

    // Find out which call was terminated
    TTerminatedCall terminatedCall = ENull;
    if( holdCallId < 0 )
        {
        terminatedCall = EHold;            
        }
    else if( ringingCallId < 0 )
        {
        terminatedCall = ERinging;            
        }
    else if( alertingCallId < 0 )
        {
        terminatedCall = EAlerting;            
        }
    
    switch( terminatedCall )
        {
        case EHold:
            // Idle message came for hold call
            StateChangeToCallSetupAndWaitingL( aCallId );            
            break;
            
        case ERinging:
            // Idle message came for ringing call
            StateChangeToSingleAndAlertingL( aCallId );                    
            break;        
            
        case EAlerting:
            // Idle message came for alerting call
            StateChangeToSingleAndWaitingL( aCallId );            
            break;
            
        default:
            break;
        }

    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting:HandleConnectingL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetupAndWaiting::HandleConnectingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndCallSetupAndWaiting::HandleConnectingL() ");
    
    // Remove the number entry if it isn't DTMF dialer
    if ( !iOnScreenDialer || !IsNumberEntryVisibleL() || !IsDTMFEditorVisibleL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveGlobalNote );
    
    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );

    BeginUiUpdateLC();

    // Update bubble content
    UpdateRemoteInfoDataL( aCallId );
    
    // Update bubble type
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnecting );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
        
    EndUiUpdate();                

    // Set call waiting cba
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
    
    // Stop key Capture when the phone is connecting
    CaptureKeysDuringCallNotificationL( EFalse );           
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetupAndWaiting::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndCallSetupAndWaiting::HandleConnectedL()");
    // Keep Phone in the foreground
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewSetNeedToSendToBackgroundStatus, &booleanParam );
    
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    // Show bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
    
    // Capturing keys and number entry must be removed because some
    // networks jump over connecting state directly to connected state.
    CaptureKeysDuringCallNotificationL( EFalse );
    
    // Remove the number entry if it isn't DTMF dialer
    if ( !iOnScreenDialer || !IsNumberEntryVisibleL() || !IsDTMFEditorVisibleL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }
    
    HandleColpNoteL( aCallId );

    if ( aCallId != iWaitingCallId )
        {
        // Alerting call is connected
        StateChangeToTwoSinglesAndWaitingL( aCallId );
        }
    else   
        {
        // Waiting call is connected while alerting call is disconnecting
        // (manual control sequence '1' + Send). 
        StateChangeToTwoSinglesL( aCallId );                      
        }
    }
    
// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting:StateChangeToCallSetupAndWaitingL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetupAndWaiting::StateChangeToCallSetupAndWaitingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndCallSetupAndWaiting::StateChangeToCallSetupAndWaitingL() ");
    // Remove call 
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
        
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
    
    // Reset Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );       

    BeginUiUpdateLC();
    
    SetTouchPaneButtons( EPhoneCallSetupButtons );
    SetToolbarDimming( ETrue );
    SetToolbarButtonLoudspeakerEnabled();
    
    EndUiUpdate();
    
    // Go to Call Setup And Waiting state
    // No need to update cbas
    iStateMachine->ChangeState( EPhoneStateCallSetupAndWaiting );
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting:StateChangeToSingleAndWaitingL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetupAndWaiting::StateChangeToSingleAndWaitingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndCallSetupAndWaiting::StateChangeToSingleAndWaitingL() ");
    // Remove call 
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
    
    CheckIfRestoreNEContentAfterDtmfDialer();
      
    if ( CPhoneState::IsNumberEntryUsedL() )
        {
        // Show number entry
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( ETrue );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetNumberEntryVisible, &booleanParam );
        }

    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
    
        // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    BeginUiUpdateLC();
    
    SetTouchPaneButtons( EPhoneWaitingCallButtons );
    
    EndUiUpdate();
    // Go to Single And Waiting state  
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );  
    iStateMachine->ChangeState( EPhoneStateWaitingInSingle );        
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting:StateChangeToSingleAndAlertingL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetupAndWaiting::StateChangeToSingleAndAlertingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndCallSetupAndWaiting::StateChangeToSingleAndAlertingL() ");
    // Remove call 
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    // Remove the number entry if it isn't DTMF dialer
    if ( !iOnScreenDialer || !IsNumberEntryVisibleL() || !IsDTMFEditorVisibleL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }
    
    // Set Hold flag to view EFalse that dtmf menu item not delete
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    BeginUiUpdateLC();
    
    SetTouchPaneButtons( EPhoneCallSetupButtons );
    
    EndUiUpdate();
    // Go to Single And Alerting state
    UpdateCbaL( EPhoneCallHandlingInCallCBA );
    iStateMachine->ChangeState( EPhoneStateAlertingInSingle );        
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting::StateChangeToTwoSinglesL
// -----------------------------------------------------------
//
// <-- QT PHONE START --> 
void CPhoneSingleAndCallSetupAndWaiting::StateChangeToTwoSinglesL( TInt /*aCallId*/ )
// <-- QT PHONE END --> 
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndCallSetupAndWaiting::StateChangeToTwoSinglesL()");

    BeginUiUpdateLC();
    
    SetTouchPaneButtonEnabled( EPhoneCallComingCmdAnswer );
    SetTouchPaneButtons( EPhoneTwoSinglesButtons );
    
    EndUiUpdate();

    // Go to two singles state 
    UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
    iStateMachine->ChangeState( EPhoneStateTwoSingles );                     
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting::StateChangeToTwoSinglesAndWaitingL
// -----------------------------------------------------------
//
// <-- QT PHONE START --> 
void CPhoneSingleAndCallSetupAndWaiting::StateChangeToTwoSinglesAndWaitingL( TInt /*aCallId*/ )
// <-- QT PHONE END --> 
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndCallSetupAndWaiting::StateChangeToTwoSinglesAndWaitingL()");

    BeginUiUpdateLC();
    
    SetTouchPaneButtons( EPhoneWaitingCallButtons );
    SetTouchPaneButtonDisabled( EPhoneCallComingCmdAnswer );
    
    EndUiUpdate();

    // Go to two singles state and waiting
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
    iStateMachine->ChangeState( EPhoneStateTwoSinglesAndWaiting );
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetupAndWaiting::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetupAndWaiting::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndCallSetupAndWaiting::UpdateInCallCbaL() ");
    
    if ( iAlerting )
        {
        UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
        }
    else
        {
        UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
        }
    }
    
                

// End of File
