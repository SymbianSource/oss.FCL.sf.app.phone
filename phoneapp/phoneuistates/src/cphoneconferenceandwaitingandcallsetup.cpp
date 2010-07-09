/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneConferenceAndWaitingAndCallSetup class.
*
*/


// INCLUDES
#include <StringLoader.h>
#include <cpephonemodelif.h>
#include "cphoneconferenceandwaitingandcallsetup.h"
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

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneConferenceAndWaitingAndCallSetup::CPhoneConferenceAndWaitingAndCallSetup( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneConference( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::~CPhoneConferenceAndWaitingAndCallSetup()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConferenceAndWaitingAndCallSetup::~CPhoneConferenceAndWaitingAndCallSetup()
    {
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaitingAndCallSetup::ConstructL()
    {
    CPhoneConference::ConstructL();
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConferenceAndWaitingAndCallSetup* CPhoneConferenceAndWaitingAndCallSetup::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndWaitingAndCallSetup::NewL");
    CPhoneConferenceAndWaitingAndCallSetup* self = 
        new( ELeave ) CPhoneConferenceAndWaitingAndCallSetup( 
            aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaitingAndCallSetup::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneConferenceAndWaitingAndCallSetup::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageConnecting:
            iAlerting = ETrue;
            HandleConnectingL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;
        
        case MEngineMonitor::EPEMessageConferenceIdle:
            HandleConferenceIdleL();
            break;

        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;

        default:
            CPhoneConference::HandlePhoneEngineMessageL( aMessage, 
                aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::HandleCommandL
// -----------------------------------------------------------
//
TBool CPhoneConferenceAndWaitingAndCallSetup::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndWaitingAndCallSetup::HandleCommandL()");
    TBool commandStatus = ETrue;
    
    switch( aCommand )
        {   
        case EPhoneDtmfDialerCancel:
            {
            }
            break;    
    
        default:
            commandStatus = CPhoneConference::HandleCommandL( aCommand );
            break;
        }

    return commandStatus;
    }

// --------------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::HandleKeyMessageL
// --------------------------------------------------------------
//
void CPhoneConferenceAndWaitingAndCallSetup::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndWaitingAndCallSetup::HandleKeyMessageL");
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
                // Number entry is behind waiting call bubble.
                // We can't answer to call in this state so display
                // Not allowed -note.
                SendGlobalErrorNoteL( 
                    EPhoneNoteTextNotAllowed, ETrue );                                             
                 }
            break;
            
        default:
            // do base operation
            CPhoneConference::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaitingAndCallSetup::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneConferenceAndWaitingAndCallSetup::SetCallSetupCbaL() ");    
    
    if ( iAlerting )
        {
        UpdateCbaL ( EPhoneCallHandlingCallWaitingCBA );
        }
    else
        {
        UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::HandleConnectingL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaitingAndCallSetup::HandleConnectingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndWaitingAndCallSetup::HandleConnectingL");
    
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
    
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnecting );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
        
    // Remove the number entry if it isn't DTMF dialer
    if ( !iOnScreenDialer || !IsNumberEntryVisibleL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }
    
    EndUiUpdate();
        
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );    
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaitingAndCallSetup::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndWaitingAndCallSetup::HandleConnectedL");
    
    BeginUiUpdateLC();
    
    UpdateRemoteInfoDataL ( aCallId );
    
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
    
    SetTouchPaneButtons( EPhoneWaitingCallButtons );    
    EndUiUpdate();
    
    // Go to Conference And Single And Waiting state
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );  
    iStateMachine->ChangeState( EPhoneStateConferenceAndSingleAndWaiting );                
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::UpdateConnectingCbaL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaitingAndCallSetup::UpdateConnectingCbaL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneConferenceAndCallSetup::UpdateInCallCbaL() ");
        
    UpdateCbaL( EPhoneCallHandlingInCallCBA );
    } 
    
// -----------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::HandleConferenceIdleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaitingAndCallSetup::HandleConferenceIdleL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndWaitingAndCallSetup::HandleConferenceIdleL");
    
    BeginUiUpdateLC();
        
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveConferenceBubble );

    if ( IsNumberEntryUsedL() )
        {
        // Show the number entry if it exists
        SetNumberEntryVisibilityL(ETrue);
        }

    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnected );
    iViewCommandHandle->HandleCommandL( EPhoneViewGetCallIdByState,
        &callStateData );
 
    if( callStateData.CallId() > KErrNotFound )
        {        
        // Go to Single And Waiting And Call Setup state
        SetTouchPaneButtons( EPhoneCallSetupButtons );        
        // No need update cbas
        iStateMachine->ChangeState( EPhoneStateSingleAndCallSetupAndWaiting );
        }
    else 
        {
        callStateData.SetCallState( EPEStateHeld );
        iViewCommandHandle->HandleCommandL( EPhoneViewGetCallIdByState,
            &callStateData );
        if( callStateData.CallId() > KErrNotFound )
            {
            // Go to Single And Waiting And Call Setup state
            SetTouchPaneButtons( EPhoneCallSetupButtons ); 
            // No need update cbas         
            iStateMachine->ChangeState( EPhoneStateSingleAndCallSetupAndWaiting );
            }
        else
            {
            // Reset Hold flag to view
            TPhoneCmdParamBoolean holdFlag;
            holdFlag.SetBoolean( EFalse );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );       

            // Go to Call Setup And Waiting state
            SetTouchPaneButtons( EPhoneCallSetupButtons ); 

            // No need update cbas            
            iStateMachine->ChangeState( EPhoneStateCallSetupAndWaiting );
            }
        }
    EndUiUpdate();          
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaitingAndCallSetup::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaitingAndCallSetup::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
         "CPhoneConferenceAndWaitingAndCallSetup::HandleIdleL");        

    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );

    // Stop capturing keys
    CaptureKeysDuringCallNotificationL( EFalse );

    TPhoneCmdParamBoolean conferenceExistsForCallId;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetCallExistsInConference,
        aCallId, &conferenceExistsForCallId );
    
    if( conferenceExistsForCallId.Boolean() )
        {
        // Remove conference member from conference bubble
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveFromConference, 
            aCallId );            
        return;
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
            // Idle message came for last conference member.
            // Don't do anything, wait for ConferenceIdle message
            }            
        else
            {
            BeginUiUpdateLC();
            TPhoneCmdParamCallStateData callStateData;
            callStateData.SetCallState( EPEStateRinging );
            iViewCommandHandle->HandleCommandL( EPhoneViewGetCallIdByState,
                &callStateData );
            if( callStateData.CallId() > KErrNotFound )
                {
                CheckIfRestoreNEContentAfterDtmfDialer();
                
                if ( IsNumberEntryUsedL() )
                    {
                    // Show the number entry if callsetup failed with number busy
                    // etc reason.
                    SetNumberEntryVisibilityL(ETrue);
                    }
                
                // Go to Conference And Waiting state
                SetTouchPaneButtons( EPhoneWaitingCallButtons );
                UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );                 
                iStateMachine->ChangeState( EPhoneStateConferenceAndWaiting );
                }
            else
                {
                // Go to Conference And Call Setup state
                SetTouchPaneButtons( EPhoneCallSetupButtons );
                UpdateCbaL( EPhoneCallHandlingInCallCBA );                  
                iStateMachine->ChangeState( EPhoneStateConferenceAndCallSetup );
                }
            EndUiUpdate();             
            }
        }
    }
    
// End of File
