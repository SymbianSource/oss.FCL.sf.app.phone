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
    
    // Fetch ringing call's id from view
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
        // send-key
        case EKeyYes:
            if( IsNumberEntryVisibleL() )
                {
                CallFromNumberEntryL();
                }
            else
                {
                // Number entry is below incoming call bubble so
                // answer the call
                iStateMachine->SendPhoneEngineMessage(
                    MPEPhoneModel::EPEMessageAnswer ); 
                }
            break;
            
        default:
            // do base operation
            CPhoneConference::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::HandleErrorL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneConferenceAndWaiting::HandleErrorL( 
        const TPEErrorInfo& aErrorInfo )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndWaiting::HandleErrorL()");
    
    if( aErrorInfo.iErrorCode == ECCPErrorCCUserAlertingNoAnswer )
        {
        // Should not shown "No Answer" note
        __PHONELOG1( EBasic, EPhoneUIStates,
       "PhoneUIStates: CPhoneConferenceAndWaiting::HandleErrorL - aErrorInfo.iErrorCode =%d ",
            aErrorInfo.iErrorCode);
        }
    else
        {
        CPhoneState::HandleErrorL( aErrorInfo );
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndWaiting::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneConferenceAndWaiting::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, 
        "CPhoneConferenceAndWaiting::OpenMenuBarL()");
        
    TInt resourceId( EPhoneConfAndCallWaitingMenubar );
    
    if( IsNumberEntryVisibleL() )
        {
        resourceId = EPhoneConfAndCallWaitingMenubarWithNumberEntry;
        }
    else if ( IsConferenceBubbleInSelectionMode() )
        {
        resourceId = EPhoneConfCallParticipantsMenubar;    
        }        
    
    TPhoneCmdParamBoolean booleanParam;
    const TBool videoWaiting = IsVideoCallRingingL();
    booleanParam.SetBoolean( videoWaiting );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetConferenceAndWaitingVideo,
        &booleanParam );
    
    
    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
                CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( resourceId ) );
                
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
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
        // Fetch ringing call's id from view
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
    
    // Effect is shown when dialer exist.
    TBool effectStarted ( EFalse );
    if ( !NeedToReturnToForegroundAppL() )
        {
        BeginTransEffectLC( ENumberEntryOpen );
        effectStarted = ETrue; 
        }
        
    BeginUiUpdateLC();    
         
    UpdateRemoteInfoDataL ( aCallId );
    
    if ( IsConferenceBubbleInSelectionMode() )
        {
        CloseSelectionListL();                
        }
    
    // Show bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
    
    
    if ( IsNumberEntryUsedL() )
        {
        if ( NeedToReturnToForegroundAppL() )
            {
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetControlAndVisibility );
            
            UpdateCbaL( EPhoneCallHandlingInCallCBA );
            }
        }
    else
        {
        UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
        }
    SetTouchPaneButtons( EPhoneConferenceAndSingleButtons );
    SetTouchPaneButtonDisabled( EPhoneInCallCmdPrivate );
    
    EndUiUpdate();
    if( effectStarted )
        {
        EndTransEffect();    
        }
    // CBA updates in above if-else conditions
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
        // Fetch ringing call's id from view
        TPhoneCmdParamCallStateData callStateData;
        callStateData.SetCallState( EPEStateRinging );
        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );
        iRingingCallId = callStateData.CallId();   
        }
    
    if( iRingingCallId == aCallId )
        {
        // Effect is shown when dialer exist.
        TBool effectStarted ( EFalse );
        if ( !NeedToReturnToForegroundAppL() )
            {
            BeginTransEffectLC( ENumberEntryOpen );
            effectStarted = ETrue;
            }
        
        BeginUiUpdateLC();    
 
        // Idle message came for waiting call            
        // Close menu bar, if it is displayed
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

        // Remove call 
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
            
        if ( IsNumberEntryUsedL() )
            {
            if ( NeedToReturnToForegroundAppL() ) 
                {
                // Return phone to the background if menu application is needed to foreground.
                iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
 
                iViewCommandHandle->ExecuteCommandL( EPhoneViewSetControlAndVisibility );
                
                UpdateCbaL( EPhoneCallHandlingInCallCBA );
                }
            }
        else
            {
            UpdateCbaL( EPhoneCallHandlingInCallCBA );
            // If numberentry is not open just check NeedToReturnToForegroundAppL and 
            // sendbackround if needed.
            if ( NeedToReturnToForegroundAppL() )
                {
                // Return phone to the background if send to background is needed.
                iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
                }
            }
        
        if ( iViewCommandHandle->HandleCommandL( EPhoneViewIsConferenceInExpandedMode ) 
                == EPhoneViewResponseSuccess )
            {
            SetTouchPaneButtons( EPhoneParticipantListButtons );
            }
        else
            {
            SetTouchPaneButtons( EPhoneConferenceButtons );    
            }
        EndUiUpdate();
        if ( effectStarted )
            {
            EndTransEffect();    
            }
         
        // Go to conference state
        // CBA updates in above if-else conditions
        iStateMachine->ChangeState( EPhoneStateConference );                    
        }
    else
        {
        // Idle message came for conference member
        CPhoneConference::HandleIdleL( aCallId );           
        
        TPhoneCmdParamBoolean conferenceBubbleExists;
        iViewCommandHandle->ExecuteCommandL( EPhoneViewGetIsConference,
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
 
    BeginTransEffectLC( ENumberEntryOpen );
    BeginUiUpdateLC();
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveConferenceBubble );
    
    TPhoneCmdParamInteger activeCallCount;
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewGetCountOfActiveCalls, &activeCallCount );
        
     // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
        
    switch( activeCallCount.Integer() )
        {
        case ENoActiveCalls:
            // Go to incoming state
            {
            // Display ringing bubble
            TPhoneCmdParamCallHeaderData callHeaderParam;
            callHeaderParam.SetCallState( EPEStateRinging );
            
            // Set Hold flag to view
            TPhoneCmdParamBoolean holdFlag;
            holdFlag.SetBoolean( EFalse );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
              
            // Bring up callhandling view
            BringIncomingToForegroundL(); 
           
            SetCallHeaderTextsForCallComingInL( iRingingCallId, EFalse, &callHeaderParam );
          
            iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, 
                iRingingCallId ,&callHeaderParam );
            iCbaManager->UpdateIncomingCbaL( iRingingCallId );
            UpdateSilenceButtonDimming();
            
            // Hide number entry if visible
            if ( IsNumberEntryVisibleL() )
                {
                SetNumberEntryVisibilityL(EFalse);   
                }
            
            SetTouchPaneButtons( EPhoneIncomingCallButtons );
            SetTouchPaneButtonEnabled( EPhoneCallComingCmdSilent );
            SetRingingTonePlaybackL( iRingingCallId );          
            SetToolbarDimming( ETrue );
            iStateMachine->ChangeState( EPhoneStateIncoming );
            break;   
            }   
            
        case EOneActiveCall:
            if ( IsNumberEntryUsedL() )
                {
                // Show the number entry if it exists
                SetNumberEntryVisibilityL(ETrue);
                }
            // Go to waiting in single state
            UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
            
            SetTouchPaneButtons( EPhoneWaitingCallButtons );             
            iStateMachine->ChangeState( EPhoneStateWaitingInSingle );
            break;
            
        case ETwoActiveCalls:
            if ( IsNumberEntryUsedL() )
                {
                // Show the number entry if it exists
                SetNumberEntryVisibilityL(ETrue);
                }
            // Go to two singles and waiting state
            UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
            
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
    EndTransEffect();  
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
    
    BeginTransEffectLC( ENumberEntryClose );
    BeginUiUpdateLC();
    // Hide number entry if it exists
    SetNumberEntryVisibilityL(EFalse);        

    // Display call setup 
    DisplayCallSetupL( aCallId );

    SetTouchPaneButtons( EPhoneCallSetupButtons ); 

    EndUiUpdate();
    EndTransEffect();
    
    // Update conference and waiting and call setup softkeys
    UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
    
    // Go to conference and waiting and call setup state
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
    
    // Update conference bubble
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewPrivateFromConference, aCallId );
    
    if ( iRingingCallId == KErrNotFound )
        {
        // Fetch ringing call's id from view
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
        // Set touch controls
        SetTouchPaneButtonDisabled( EPhoneCallComingCmdAnswer );
        SetTouchPaneButtons( EPhoneWaitingCallButtons );
        UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
        // Go to Conference And Single And Waiting state
        iStateMachine->ChangeState( EPhoneStateConferenceAndSingleAndWaiting );  
        }

    EndUiUpdate();
    }

// End of File
