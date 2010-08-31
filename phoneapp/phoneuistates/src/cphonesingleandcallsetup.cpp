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
* Description: Implementation of CPhoneSingleAndCallSetup class.
*
*/


// INCLUDES
#include <featmgr.h>
#include <cpephonemodelif.h>
#include "cphonesingleandcallsetup.h"
#include "mphonestatemachine.h"
#include "phoneviewcommanddefinitions.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "phonestatedefinitionsgsm.h"
#include "phonelogger.h"
#include "phonerssbase.h"
#include "phoneui.hrh"
#include "tphonecmdparamglobalnote.h"
#include "cphonemainresourceresolver.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneSingleAndCallSetup::CPhoneSingleAndCallSetup( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneCallSetup( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetup::~CPhoneSingleAndCallSetup()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneSingleAndCallSetup::~CPhoneSingleAndCallSetup()
    {
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetup::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetup::ConstructL()
    {
    CPhoneCallSetup::ConstructL();
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetup::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneSingleAndCallSetup* CPhoneSingleAndCallSetup::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneSingleAndCallSetup* self = new( ELeave ) CPhoneSingleAndCallSetup( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetup::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetup::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
         "CPhoneSingleAndCallSetup::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageConnecting:
            HandleConnectingL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;
            
        default:
            CPhoneCallSetup::HandlePhoneEngineMessageL( aMessage, 
                aCallId );
            break;
        }
    }

// --------------------------------------------------------------
// CPhoneSingleAndCallSetup::HandleKeyMessageL
// --------------------------------------------------------------
//
void CPhoneSingleAndCallSetup::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndCallSetup::HandleKeyMessageL()");
    switch ( aCode )
        {
        // send-key
        case EKeyYes:
            if ( CPhoneState::IsNumberEntryUsedL() )
                {
                // send a manual control sequence
                CPhoneState::CallFromNumberEntryL();    
                }
            else
                {
                // Show not allowed note
                CPhoneState::SendGlobalErrorNoteL( 
                    EPhoneNoteTextNotAllowed, ETrue );
                }
            break;

        // end-key
        case EKeyNo:
            if ( aMessage == EPhoneKeyLongPress )
                {
                iStateMachine->SendPhoneEngineMessage(
                    CPEPhoneModelIF::EPEMessageTerminateAllConnections );        
                }
            else
                {
                // Fetch active call's id from view
                TPhoneCmdParamCallStateData callStateData;
                // Find the dialing call
                callStateData.SetCallState( EPEStateDialing );
                iViewCommandHandle->HandleCommandL(
                    EPhoneViewGetCallIdByState, &callStateData ); 
            
                if( callStateData.CallId() > KErrNotFound )
                    {
                    // Release the call
                    iStateMachine->SetCallId( 
                        callStateData.CallId() );
                    iStateMachine->SendPhoneEngineMessage( 
                        MPEPhoneModel::EPEMessageRelease );
                    }
                else
                    {
                    callStateData.SetCallState( EPEStateDisconnecting );
                    iViewCommandHandle->HandleCommandL(
                        EPhoneViewGetCallIdByState, &callStateData );
                    
                     if ( callStateData.CallId() > KErrNotFound )
                        {    
                        // Release the call
                        iStateMachine->SetCallId( 
                            callStateData.CallId() );
                        iStateMachine->SendPhoneEngineMessage( 
                            MPEPhoneModel::EPEMessageRelease );   
                        }
                    }          
                }
            break;
            
        default:
            // do base operation
            CPhoneCallSetup::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }


// -----------------------------------------------------------
// CPhoneSingleAndCallSetup::HandleConnectingL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetup::HandleConnectingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndCallSetup::HandleConnectingL()");
    
    BeginUiUpdateLC();
    
    UpdateRemoteInfoDataL ( aCallId );
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveGlobalNote );
    
    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );

    // Stop capturing keys
    CaptureKeysDuringCallNotificationL( EFalse );

    // Remove the number entry if it isn't DTMF dialer
    if ( !iOnScreenDialer || !IsNumberEntryVisibleL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }

    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnecting );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );

    EndUiUpdate();
        
    // Set Hold flag to view EFalse that dtmf menu item not delete
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    SetTouchPaneButtons( EPhoneCallSetupAndSingleButtons );
    
    // Go to alerting state
    UpdateCbaL( EPhoneCallHandlingInCallCBA );
    iStateMachine->ChangeState( EPhoneStateAlertingInSingle );
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetup::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetup::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndCallSetup::HandleConnectedL()");
    
    BeginUiUpdateLC();
    
    // Show bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );

    // Capturing keys and number entry must be removed because some
    // networks jump over connecting state directly to connected state.
    CaptureKeysDuringCallNotificationL( EFalse );
    
    // Remove the number entry if it isn't DTMF dialer
    if ( !iOnScreenDialer || !IsNumberEntryVisibleL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }
    
    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    SetTouchPaneButtons( EPhoneTwoSinglesButtons );
    
    EndUiUpdate();        
    
     // Go to two singles state
    UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
    iStateMachine->ChangeState( EPhoneStateTwoSingles );
    
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetup::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneSingleAndCallSetup::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndCallSetup::HandleIdleL()");
    
    BeginUiUpdateLC();
 
    // Remove call 
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

    // Find out do we have single or outgoing call left
    TPhoneCmdParamInteger activeCallCount;
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewGetCountOfActiveCalls, &activeCallCount );
        
    if( activeCallCount.Integer() )
        {
        CheckIfRestoreNEContentAfterDtmfDialer();
 
        if ( IsNumberEntryUsedL() )
            {
            // Show the number entry if it exists
            SetNumberEntryVisibilityL(ETrue);
            }
        else
            {
            // Set incall CBAs
            UpdateCbaL( EPhoneCallHandlingInCallCBA );    
            }
            
        SetTouchPaneButtons( EPhoneIncallButtons );    
        // UnCapture keys callsetup fails
        CaptureKeysDuringCallNotificationL( EFalse );
        // Setup call was terminated
        iStateMachine->ChangeState( EPhoneStateSingle );            
        }
    else
        {
        // Display call termination note, if necessary
        DisplayCallTerminationNoteL();

        // Single call was terminated
        SetTouchPaneButtons( EPhoneCallSetupButtons );
        SetToolbarButtonLoudspeakerEnabled();
        // Update call setup CBAs
        UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
        iStateMachine->ChangeState( EPhoneStateCallSetup );            
        }
        
    EndUiUpdate();
    }

// -----------------------------------------------------------
// CPhoneSingleAndCallSetup::HandleCommandL
// -----------------------------------------------------------
//
TBool CPhoneSingleAndCallSetup::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndCallSetup::HandleCommandL()");
    TBool commandStatus = ETrue;

    switch( aCommand )
        {
        case EPhoneInCallCmdEndThisOutgoingCall:
            {
            // Fetch  call's id from view
            TPhoneCmdParamCallStateData callStateData;
            callStateData.SetCallState( EPEStateDialing );
            iViewCommandHandle->HandleCommandL(
                EPhoneViewGetCallIdByState, &callStateData );
                
            if ( callStateData.CallId() > KErrNotFound )
                {
                // Release the call
                iStateMachine->SetCallId( 
                    callStateData.CallId() );
                iStateMachine->SendPhoneEngineMessage( 
                    MPEPhoneModel::EPEMessageRelease );    
                }
            else
                {
                callStateData.SetCallState( EPEStateDisconnecting );
                iViewCommandHandle->HandleCommandL(
                    EPhoneViewGetCallIdByState, &callStateData );
                
                 if ( callStateData.CallId() > KErrNotFound )
                    {    
                    // Release the call
                    iStateMachine->SetCallId( 
                        callStateData.CallId() );
                    iStateMachine->SendPhoneEngineMessage( 
                        MPEPhoneModel::EPEMessageRelease );   
                    }
                __PHONELOG( EOnlyFatal, EPhoneUIStates, 
                    "CPhoneSingleAndCallSetup::HandleCommandL EPhoneInCallCmdEndThisOutgoingCall has negative call id!" );   
                }
            }
            break;
            
        default:
            commandStatus = CPhoneStateCallSetup::HandleCommandL( aCommand );
            break;
        }

    return commandStatus;
    }
 
// End of File

