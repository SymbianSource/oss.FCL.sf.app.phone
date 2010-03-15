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
    // Need to check iViewCommandHandle validity here to not
    // trigger a high/can panic error in a Codescanner run.
    // coverity[var_compare_op]
    if ( iViewCommandHandle )
        {
        TPhoneCmdParamBoolean dtmfSendFlag;
        dtmfSendFlag.SetBoolean( EFalse );
        TRAP_IGNORE( iViewCommandHandle->ExecuteCommandL( EPhoneViewSetDtmfOptionsFlag, 
    	&dtmfSendFlag ) );
        }
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
// CPhoneConferenceAndSingleAndWaiting::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndSingleAndWaiting::OpenMenuBarL()");
    TInt resourceId = NULL;

   // Set specific flag to view so that DTMF menu item available
    TPhoneCmdParamBoolean dtmfSendFlag;
    dtmfSendFlag.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetDtmfOptionsFlag, 
    	&dtmfSendFlag );
  

    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateHeld );
    iViewCommandHandle->HandleCommandL( EPhoneViewGetCallIdByState, 
        &callStateData );
    if( callStateData.CallId() == KConferenceCallId )
        {
        // Conference call is on hold and single is active
        if ( IsNumberEntryVisibleL() )
            {
            resourceId = EPhoneCallActiveHeldConfAndWaitingMenubarWithNumberEntry;
            }
        else if ( IsConferenceBubbleInSelectionMode() )
            {
            resourceId = EPhoneConfCallParticipantsDropMenubar;    
            }                    
        else
            {
            resourceId = EPhoneCallActiveHeldConfAndWaitingMenubar;
            }
        }
    else if( callStateData.CallId() >= 0 )
        {
        // Single call is on hold and conference is active
        if ( IsNumberEntryVisibleL() )
            {
            resourceId = EPhoneConfCallActiveHeldAndWaitingMenubarWithNumberEntry;
            }
        else if ( IsConferenceBubbleInSelectionMode() )
            {
            resourceId = EPhoneConfCallParticipantsDropMenubar;    
            }                    
        else
            {
            resourceId = EPhoneConfCallActiveHeldAndWaitingMenubar;
            }
        }
    else
        {
        return; // negative call id, don't do anything            
        }

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingleAndWaiting::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndSingleAndWaiting::HandleIdleL()");
        
    // Effect is shown when dialer exist.
    TBool effectStarted ( EFalse );
    if ( !NeedToReturnToForegroundAppL() )
        {
        BeginTransEffectLC( ENumberEntryOpen );
        effectStarted = ETrue; 
        }
         
    BeginUiUpdateLC();    
    // Set touch controls
    SetTouchPaneButtonEnabled( EPhoneCallComingCmdAnswer );
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
    if ( effectStarted )
        {
        EndTransEffect();    
        }
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
    if ( !IsNumberEntryUsedL() )
        {
        // Close menu bar, if number entry isnt open.
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
        }
  
    SetTouchPaneButtons( EPhoneWaitingCallButtons ); 
    SetTouchPaneButtonEnabled( EPhoneCallComingCmdAnswer );
    
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

    if ( !IsNumberEntryUsedL() )
        {
        // Close menu bar, if number entry isnt open.
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );       
        }

    if ( IsNumberEntryUsedL() )
        {
        if ( NeedToReturnToForegroundAppL() ) 
            {
            // Return phone to the background if send to background is needed.
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );

            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetControlAndVisibility );
 
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
        // If numberentry is not open just check NeedToReturnToForegroundAppL and 
        // sendbackround if needed.
        if ( NeedToReturnToForegroundAppL() ) 
            {
            // Return phone to the background if send to background is needed.
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
            }
        }  

    SetTouchPaneButtons( EPhoneConferenceAndSingleButtons );       
    SetTouchPaneButtonEnabled( EPhoneCallComingCmdAnswer );
    SetTouchPaneButtonDisabled( EPhoneInCallCmdPrivate );
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
        
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

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
                    EPhoneNoteTextCallNotAllowed );		 				
	 			}
            break;
            
        default:
            // do base operation
            CPhoneConferenceAndSingle::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingleAndWaiting::HandleErrorL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneConferenceAndSingleAndWaiting::HandleErrorL( 
        const TPEErrorInfo& aErrorInfo )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingleAndWaiting::HandleErrorL()");
    
    if( aErrorInfo.iErrorCode == ECCPErrorCCUserAlertingNoAnswer )
        {
        // Should not shown "No Answer" note
        __PHONELOG1( EBasic, EPhoneUIStates,
       "PhoneUIStates: CPhoneConferenceAndSingleAndWaiting::HandleErrorL - aErrorInfo.iErrorCode =%d ",
            aErrorInfo.iErrorCode);
        }
    else
        {
        CPhoneState::HandleErrorL( aErrorInfo );
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

                SetTouchPaneButtons( EPhoneIncomingCallButtons );
                UpdateSilenceButtonDimming();
                SetTouchPaneButtonEnabled( EPhoneCallComingCmdAnswer );
                SetRingingTonePlaybackL( callStateData.CallId() );      
                SetToolbarDimming( ETrue );
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
            SetTouchPaneButtonEnabled( EPhoneCallComingCmdAnswer );

            iStateMachine->ChangeState( EPhoneStateWaitingInSingle );        
            }
            break;
         case ETwoActiveCalls:
            {
            // Go to Two Singles And Waiting state
            UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );

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
