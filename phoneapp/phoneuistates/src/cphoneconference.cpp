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
* Description: Implementation of CPhoneConference class.
*
*/


// INCLUDES
#include <eikmenub.h> 
#include <eikenv.h>
#include <StringLoader.h>
#include <pevirtualengine.h>
#include <mpeengineinfo.h>
#include <cpephonemodelif.h>
#include <featmgr.h>
#include "cphoneconference.h"
#include "mphonestatemachine.h"
#include "phoneviewcommanddefinitions.h"
#include "phoneui.hrh"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamkeyevent.h"
#include "phonestatedefinitions.h"
#include "tphonecmdparamglobalnote.h"
#include "phonestatedefinitionsgsm.h"
#include "tphonecmdparamcallstatedata.h"
#include "cphonekeys.h"
#include "phonelogger.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneConference::CPhoneConference( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneGsmInCall( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneConference::~CPhoneConference()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConference::~CPhoneConference()
    {
    }

// -----------------------------------------------------------
// CPhoneConference::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneConference::ConstructL()
    {
    CPhoneGsmInCall::ConstructL();
    
    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );
    
  
    DefineAndSetHoldFlagL();
    
    }

// -----------------------------------------------------------
// CPhoneConference::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConference* CPhoneConference::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneConference* self = new( ELeave ) CPhoneConference( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneConference::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneConference::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;
        
        case MEngineMonitor::EPEMessageConferenceIdle:
            HandleConferenceIdleL();
            break;
            
        case MEngineMonitor::EPEMessageHeldConference:
            HandleHeldConferenceL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageConnectedConference:
            HandleConnectedConferenceL();
            break;

        case MEngineMonitor::EPEMessageAddedConferenceMember:
            UpdateConferenceSecurityStatusL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageWentOneToOne:
            HandleWentOneToOneL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageHeld:
            {
            TPhoneCmdParamCallHeaderData callHeaderParam;
            callHeaderParam.SetCallState( EPEStateHeld );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
                &callHeaderParam );
            }
            break;
            
        case MEngineMonitor::EPEMessageConnected:
            {
            TPhoneCmdParamCallHeaderData callHeaderParam;
            callHeaderParam.SetCallState( EPEStateConnected );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
                &callHeaderParam );
            }
            break;
            
        case MEngineMonitor::EPEMessageDialing:
            HandleDiallingL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageIncoming:
            HandleIncomingL( aCallId );
            break;

        default:
            CPhoneGsmInCall::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneConference:HandleCommandL
// -----------------------------------------------------------
//
TBool CPhoneConference::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleCommandL()");
    TBool commandStatus = ETrue;

    switch( aCommand )
        {   
        case EAknSoftkeyCancel:
            BeginUiUpdateLC();
            CloseSelectionListL();
            SetTouchPaneButtons( EPhoneConferenceButtons );
            EndUiUpdate();    
            UpdateCbaL( EPhoneCallHandlingInCallCBA );
            break;
    
        case EPhoneInCallCmdHold:
        case EPhoneInCallCmdConferenceHold:
            iStateMachine->SendPhoneEngineMessage( 
                CPEPhoneModelIF::EPEMessageHoldConference );
            break;
            
        case EPhoneInCallCmdUnhold:
        case EPhoneInCallCmdConferenceUnhold:
            iStateMachine->SendPhoneEngineMessage( 
                CPEPhoneModelIF::EPEMessageResumeConference );
            break;
            
        // Conference -> Drop participant
        case EPhoneInCallCmdDropSelection:
            OpenDropParticipantSelectionL();
            break;
        
        // Drop CBA    
        case EPhoneInCallCmdDrop:
            DropSelectedParticipantL();
            break;
            
        case EPhoneInCallCmdPrivateSelection:
            OpenPrivateSelectionL();
            break;
            
        case EPhoneInCallCmdPrivate:
            PrivateSelectedParticipantL();
            break;
          
        case EPhoneInCallCmdNewCall:
            LaunchNewCallQueryL();
            break;
            
        case EPhoneInCallCmdNewCallCall:
            CallFromNewCallQueryL();
            break;
            
        case EPhoneInCallCmdParticipants:
            OpenParticipantsListL();
            break;
            
        case EPhoneViewOpenNumberEntry:
            if ( IsConferenceBubbleInSelectionMode() )
                {
                CloseSelectionListL();
                SetTouchPaneButtons( EPhoneConferenceButtons );
                }
            commandStatus = CPhoneGsmInCall::HandleCommandL( aCommand );
            break;            

        default:
            commandStatus = CPhoneGsmInCall::HandleCommandL( aCommand );
            break;
        }

    return commandStatus;
    }

// -----------------------------------------------------------
// CPhoneConference::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneConference::UpdateInCallCbaL()
    {
    if ( iViewCommandHandle->HandleCommandL( EPhoneViewIsConferenceInExpandedMode ) 
        != EPhoneViewResponseSuccess )
        {
        UpdateCbaL( EPhoneCallHandlingInCallCBA );
        }    
    }

// -----------------------------------------------------------
// CPhoneConference::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneConference::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::OpenMenuBarL()");
    TInt resourceId;

    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
        {
        resourceId = EPhoneDtmfDialerMenubar;
        }
    else if ( IsNumberEntryVisibleL() )
        {
        resourceId = EPhoneConfCallMenubarWithNumberEntry;
        }
    else if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) 
              && IsConferenceBubbleInSelectionMode() )        
        {
        resourceId = EPhoneConfCallParticipantsMenubar;            
        }
    else
        {
        resourceId = EPhoneConfCallMenubar;
        }

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
    }

// -----------------------------------------------------------
// CPhoneConference::HandleKeyEventL
// -----------------------------------------------------------
//
void CPhoneConference::HandleKeyEventL( 
    const TKeyEvent& aKeyEvent, 
    TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleKeyEventL()");
    if ( IsConferenceBubbleInSelectionMode() )
        {
        if ( aKeyEvent.iCode == EKeyUpArrow ||
             aKeyEvent.iCode == EKeyDownArrow )
            {
            TPhoneCmdParamKeyEvent keyEventParam;
            keyEventParam.SetKeyEvent( aKeyEvent );
            keyEventParam.SetEventCode( aEventCode );    
            
            iViewCommandHandle->ExecuteCommandL( 
                EPhoneViewMoveHighLightInList, &keyEventParam );
            }
        }
    else
        {
        if ( CPhoneKeys::IsNumericKey( aKeyEvent, aEventCode ) )
            {
            // Handle numeric keys when key events are received 
            // in conference state.
            CPhoneGsmInCall::HandleNumericKeyEventL( aKeyEvent, aEventCode );
            }
        else
            {
            // Handle other key events.
            CPhoneState::HandleKeyEventL( aKeyEvent, aEventCode );
            }
        }
    }

// --------------------------------------------------------------
// CPhoneConference::HandleKeyMessageL
// --------------------------------------------------------------
//
void CPhoneConference::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleKeyMessageL()");
        
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
                ToggleHoldL();    
                }
            break;
            
        default:
            // do base operation
            CPhoneGsmInCall::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneConference::HandleNumberEntryClearedL
// -----------------------------------------------------------
//
void CPhoneConference::HandleNumberEntryClearedL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleNumberEntryClearedL()");
    // Update CBA when number entry is cleared
    UpdateInCallCbaL();
    }

// -----------------------------------------------------------
// CPhoneConference:HandleIdleL
// -----------------------------------------------------------
//
void CPhoneConference::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleIdleL()");
    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );

    // Stop capturing keys
    CaptureKeysDuringCallNotificationL( EFalse );

    // If dialler is not open then close menu bar.
    if ( !IsNumberEntryUsedL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
        }
        
    // If conference bubble still exists then we have conference and
    // single call
    TPhoneCmdParamBoolean conferenceBubbleExists;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetIsConference, 
        &conferenceBubbleExists ); 
        
    if( conferenceBubbleExists.Boolean() )
        {
        // Go to Conference And Single state
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewRemoveFromConference, aCallId ); 
        }
    else
        {
        // Remove call header
        iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewRemoveCallHeader, aCallId );    
        }
    }
    
// -----------------------------------------------------------
// CPhoneConference:HandleConferenceIdleL
// -----------------------------------------------------------
//
void CPhoneConference::HandleConferenceIdleL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleConferenceIdleL()");
    
    BeginTransEffectLC( ENumberEntryOpen );
    BeginUiUpdateLC();
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveConferenceBubble );

    TPhoneCmdParamInteger activeCallCount;
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewGetCountOfActiveCalls, &activeCallCount );

    switch( activeCallCount.Integer() )
        {
        case ENoActiveCalls:
            MakeStateTransitionToIdleL();
            break;
            
        case EOneActiveCall:
            {
            // Fetch ringing call's id from view
            TPhoneCmdParamCallStateData callStateData;
            callStateData.SetCallState( EPEStateRinging );
            iViewCommandHandle->HandleCommandL(
                EPhoneViewGetCallIdByState, &callStateData );
                
            if( callStateData.CallId() > KErrNotFound )
                {
                UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
                iStateMachine->ChangeState( EPhoneStateWaitingInSingle );    
                }
            else
                {
                MakeStateTransitionToSingleL();                    
                }
            }
            break;
            
        case ETwoActiveCalls:
            MakeStateTransitionToTwoSinglesL();
            break;
            
        default:
            MakeStateTransitionToTwoSinglesL();
            break;
        }
    
    EndUiUpdate();
    EndTransEffect();     
    }

// -----------------------------------------------------------
// CPhoneConference::HandleHeldConferenceL
// -----------------------------------------------------------
//
void CPhoneConference::HandleHeldConferenceL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleHeldConferenceL()");
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateHeld );
    
    TInt callLabelId;
    TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
    callLabelId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCallOnHold );

    StringLoader::Load( 
        labelText, 
        callLabelId, 
        CCoeEnv::Static() );        
    callHeaderParam.SetLabelText( labelText );
 
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewUpdateBubble, aCallId, &callHeaderParam );
       
    // Set Hold flag to view
    SetHoldFlagL();
    
    if ( !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        SendGlobalInfoNoteL( EPhoneInformationConferenceOnHold );
        }
    
    SetTouchPaneButtonDisabled( EPhoneInCallCmdPrivate );
    
    UpdateInCallCbaL();
    }


// -----------------------------------------------------------
// CPhoneConference::DefineAndSetHoldFlagL
// -----------------------------------------------------------
//
void CPhoneConference::DefineAndSetHoldFlagL()
    {
    // Find out is conference held or not
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallId( KConferenceCallId );
    iViewCommandHandle->HandleCommandL( EPhoneViewGetCallState,
            &callStateData );
    
    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
        
    if( callStateData.CallState() == EPEStateHeld )
        {
        holdFlag.SetBoolean( ETrue );
        }
    else
        {
        holdFlag.SetBoolean( EFalse );   
        }
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
    
    }

// -----------------------------------------------------------
// CPhoneConference::SetHoldFlagL
// -----------------------------------------------------------
//
void CPhoneConference::SetHoldFlagL()
    {
    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    }

// -----------------------------------------------------------
// CPhoneConference::HandleConnectedConferenceL
// -----------------------------------------------------------
//
void CPhoneConference::HandleConnectedConferenceL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleConnectedConferenceL()");
    // Update call state
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );

    TBuf<KPhoneCallHeaderLabelMaxLength> conferenceText( KNullDesC );
    TInt callLabelId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCLIConferenceCall );

    StringLoader::Load( 
        conferenceText, 
        callLabelId, 
        CCoeEnv::Static() );        
    callHeaderParam.SetLabelText( conferenceText );
 
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewUpdateBubble, KConferenceCallId, &callHeaderParam );

    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
    
    if ( !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        { 
        SendGlobalInfoNoteL( EPhoneInformationConferenceActiveted );
        }
    
    SetTouchPaneButtonEnabled( EPhoneInCallCmdPrivate );
    
    UpdateInCallCbaL();
    }

// -----------------------------------------------------------
// CPhoneConference::OpenDropParticipantSelectionL
// -----------------------------------------------------------
//
void CPhoneConference::OpenDropParticipantSelectionL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::OpenDropParticipantSelectionL()");
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewOpenConferenceList, &booleanParam );

    iCbaManager->SetCbaL( EPhoneDropParticipantCBA );
    }

// -----------------------------------------------------------
// CPhoneConference::DropSelectedParticipantL
// -----------------------------------------------------------
//
void CPhoneConference::DropSelectedParticipantL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::DropSelectedParticipantL()");
    // First fetch the call id matching the selected item 
    TPhoneViewResponseId response;
    TPhoneCmdParamInteger callId;
    response = iViewCommandHandle->HandleCommandL(
        EPhoneViewSelectedConfMember, &callId );    
        
    if( response == EPhoneViewResponseSuccess )
        {
        // Drop the call from conference
        iStateMachine->SetCallId( callId.Integer() );    
        iStateMachine->SendPhoneEngineMessage( 
            CPEPhoneModelIF::EPEMessageDropConferenceMember );            
        }
    }

// -----------------------------------------------------------
// CPhoneConference::OpenPrivateSelectionL
// -----------------------------------------------------------
//
void CPhoneConference::OpenPrivateSelectionL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::OpenPrivateSelectionL()");
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewOpenConferenceList, &booleanParam );

    iCbaManager->SetCbaL( EPhonePrivateParticipantCBA );
    }

// -----------------------------------------------------------
// CPhoneConference::PrivateSelectedParticipantL
// -----------------------------------------------------------
//
void CPhoneConference::PrivateSelectedParticipantL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::PrivateSelectedParticipantL()");
    // First fetch the call id matching the selected item 
    TPhoneViewResponseId response;
    TPhoneCmdParamInteger callId;
    response = iViewCommandHandle->HandleCommandL(
        EPhoneViewSelectedConfMember, &callId );
    
    if( response == EPhoneViewResponseSuccess )
        {
        // Drop the call from conference
        iStateMachine->SetCallId( callId.Integer() );    
        iStateMachine->SendPhoneEngineMessage( 
            CPEPhoneModelIF::EPEMessageGoOneToOne );
        
        // Update call view
        BeginUiUpdateLC();
        CloseSelectionListL();
        SetTouchPaneButtons( EPhoneConferenceButtons );
        EndUiUpdate();    
                
        UpdateCbaL( EPhoneCallHandlingInCallCBA );               
        }
    }

// -----------------------------------------------------------
// CPhoneConference::MakeStateTransitionToIdleL
// -----------------------------------------------------------
//
void CPhoneConference::MakeStateTransitionToIdleL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::MakeStateTransitionToIdleL()");
    
    SetDefaultFlagsL();

    if ( IsNumberEntryUsedL() )
        {
        // Show the number entry if it exists
        SetNumberEntryVisibilityL(ETrue);
        
        // Close dtmf dialer when call is disconnected.
        if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
            {            
            CloseDTMFEditorL();
            // Display idle screen and update CBAs
            DisplayIdleScreenL();
            }        
        }
    else if ( !TopAppIsDisplayedL() )
        {
        // Close menu bar, if it is displayed
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
        
        // Continue displaying current app but set up the 
        // idle screen in the background
        SetupIdleScreenInBackgroundL();
        }
    else
        {
        // Close menu bar, if it is displayed
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
        
         // Display idle screen
        DisplayIdleScreenL();
        }

    // Display call termination note, if necessary
    DisplayCallTerminationNoteL();

    UpdateCbaL( EPhoneEmptyCBA );
    // Go to idle state
    iStateMachine->ChangeState( EPhoneStateIdle );
    }

// -----------------------------------------------------------
// CPhoneConference::MakeStateTransitionToSingleL
// -----------------------------------------------------------
//
void CPhoneConference::MakeStateTransitionToSingleL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::MakeStateTransitionToSingleL()");
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
 
    if ( IsNumberEntryUsedL() )
        {
        // Show the number entry if it exists
        SetNumberEntryVisibilityL(ETrue);
        }
    SetTouchPaneButtons( EPhoneIncallButtons );    
  
    // Go to single state
    UpdateCbaL( EPhoneCallHandlingInCallCBA );
    iStateMachine->ChangeState( EPhoneStateSingle );        
    }

// -----------------------------------------------------------
// CPhoneConference::MakeTransitionToTwoSinglesL
// -----------------------------------------------------------
//
void CPhoneConference::MakeStateTransitionToTwoSinglesL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::MakeStateTransitionToTwoSinglesL()");
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    if ( IsNumberEntryUsedL() )
        {
        // Show the number entry if it exists
        SetNumberEntryVisibilityL(ETrue);
        }
        
    SetTouchPaneButtons( EPhoneTwoSinglesButtons );        
    
    // Set Two singles softkeys
    UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );

    // Go to two singles state
    iStateMachine->ChangeState( EPhoneStateTwoSingles );        
    }

// -----------------------------------------------------------
// CPhoneConference::CloseSelectionListL
// -----------------------------------------------------------
//
void CPhoneConference::CloseSelectionListL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::CloseSelectionListL()");
    // Close conference list
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewOpenConferenceList, &booleanParam );
    }

// -----------------------------------------------------------
// CPhoneConference::ToggleHold
// -----------------------------------------------------------
//
void CPhoneConference::ToggleHoldL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::ToggleHoldL()");
    TPhoneCmdParamBoolean hold;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetHoldFlag, &hold );
    
    if( hold.Boolean() )
        {
        iStateMachine->SendPhoneEngineMessage( 
            CPEPhoneModelIF::EPEMessageResumeConference );
        }
    else
        {
        iStateMachine->SendPhoneEngineMessage( 
            CPEPhoneModelIF::EPEMessageHoldConference );
        }
    }

// -----------------------------------------------------------
// CPhoneConference::HandleDiallingL
// -----------------------------------------------------------
//
void CPhoneConference::HandleDiallingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleDiallingL()");
    
    BeginTransEffectLC( ENumberEntryClose );
    BeginUiUpdateLC(); 
    
    CloseSelectionListL(); 
    
    SetNumberEntryVisibilityL(EFalse);
    
    // Display call setup 
    DisplayCallSetupL( aCallId );
    
    // Conference is understood as single call in buttons enumerations.
    SetTouchPaneButtons(EPhoneCallSetupAndSingleButtons); 

    EndUiUpdate();

    EndTransEffect();
    
    // Set Call Setup CBA 
    UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
      
    // Go to conference and call setup state
    iStateMachine->ChangeState( EPhoneStateConferenceAndCallSetup );
    }

// -----------------------------------------------------------
// CPhoneConference::DisplayCallSetupL
// -----------------------------------------------------------
//
void CPhoneConference::DisplayCallSetupL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::DisplayCallSetupL()");
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    // Remove dialogs if necessary
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );

    // Capture keys when the phone is dialling
    CaptureKeysDuringCallNotificationL( ETrue );

    // Bring Phone app in the foreground
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );

    // Display call setup header
    DisplayHeaderForOutgoingCallL( aCallId );
    }

// ---------------------------------------------------------
// CPhoneConference::CallFromNewCallQueryL
// ---------------------------------------------------------
//
void CPhoneConference::CallFromNewCallQueryL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::CallFromNewCallQueryL()");
    // First get the phone number from the dialog
    TPhoneCmdParamString phoneNumberParam;
    HBufC *content = HBufC::NewLC( KPhoneNumberEntryBufferSize );
    TPtr ptr( content->Des() );
    phoneNumberParam.SetString( &ptr );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetTextQueryContent,
        &phoneNumberParam );

    // Store the phone number
    iStateMachine->PhoneEngineInfo()->SetPhoneNumber( ptr );

    // clean up stack
    CleanupStack::PopAndDestroy( content );
  
    // Dial number
    DialVoiceCallL();
    }

// -----------------------------------------------------------
// CPhoneConference::HandleIncomingL
// -----------------------------------------------------------
//
void CPhoneConference::HandleIncomingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleIncomingL()");
    
    BeginUiUpdateLC();
 
    TPhoneCmdParamBoolean dialerParam;
    dialerParam.SetBoolean( ETrue );
    
    // Get allow waiting call header param value.
    AllowShowingOfWaitingCallHeaderL( dialerParam );
            
    CloseSelectionListL();

    // Close fast swap window if it's displayed
    CEikonEnv::Static()->DismissTaskList();

    // Check if HW Keys or Call UI should be disabled
    CheckDisableHWKeysAndCallUIL();
    
    SetTouchPaneButtons( EPhoneWaitingCallButtons );
    
    // Display incoming call
    DisplayIncomingCallL( aCallId, dialerParam );

    EndUiUpdate();

    // Go to incoming state
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
    iStateMachine->ChangeState( EPhoneStateConferenceAndWaiting );        
    }

// -----------------------------------------------------------
// CPhoneConference::DisplayIncomingCallL
// -----------------------------------------------------------
//
void CPhoneConference::DisplayIncomingCallL( 
    TInt aCallId, 
    const TPhoneCmdParamBoolean aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::DisplayIncomingCallL()");
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    // Cannot delete active note, e.g. New call query, 
    // but show waiting note with or without caller name
    if ( IsAnyQueryActiveL() ||  
        ( !aCommandParam.Boolean() && iOnScreenDialer ) )
        {
        CallWaitingNoteL( aCallId );        
        }
    else
        {
        // Remove any phone dialogs if they are displayed
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
        }

    // Indicate that the Phone needs to be sent to the background if
    // an application other than the top application is in the foreground
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( !TopAppIsDisplayedL() );
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewSetNeedToSendToBackgroundStatus,
        &booleanParam );

    // Bring Phone app in the foreground
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );

    // Set Phone as the top application
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
        &uidParam );

    DisplayHeaderForCallComingInL( aCallId, ETrue ); // waiting call 
    }    

// -----------------------------------------------------------
// CPhoneConference::HandleWentOneToOneL
// -----------------------------------------------------------
//
void CPhoneConference::HandleWentOneToOneL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleWentOneToOneL()");

    BeginUiUpdateLC();
    
    // Update conference bubble
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewPrivateFromConference, aCallId );
        
    // If conference bubble still exists then we have conference and
    // single call
    TPhoneCmdParamBoolean conferenceBubbleExists;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetIsConference, 
        &conferenceBubbleExists );                            
    if( conferenceBubbleExists.Boolean() )
        {
        // Go to Conference And Single state
      
        UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
        iStateMachine->ChangeState( EPhoneStateConferenceAndSingle );                    
        SetTouchPaneButtons( EPhoneConferenceAndSingleButtons );
        SetTouchPaneButtonDisabled( EPhoneInCallCmdPrivate );
        }
    else
        {            
        SetTouchPaneButtons( EPhoneTwoSinglesButtons );
        }       
    EndUiUpdate();
    }

// -----------------------------------------------------------
// CPhoneConference::OpenParticipantsListL
// -----------------------------------------------------------
//    
void CPhoneConference::OpenParticipantsListL()
    {
    BeginUiUpdateLC();
    
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( ETrue );
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewOpenConferenceList,
                                         &booleanParam );    

    SetTouchPaneButtons( EPhoneParticipantListButtons );
                                         
    EndUiUpdate();
    
    iCbaManager->SetCbaL( EPhoneParticipantListCBA );
     
    // Find out is conference held or not
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallId( KConferenceCallId );
    iViewCommandHandle->HandleCommandL( EPhoneViewGetCallState,
        &callStateData );
    
    if( callStateData.CallState() == EPEStateHeld )
        {
        //set Private button to Dimmed.        
        SetTouchPaneButtonDisabled( EPhoneInCallCmdPrivate );
        } 
    else
        {
        //set Private button to UnDimmed.        
         SetTouchPaneButtonEnabled( EPhoneInCallCmdPrivate );        
        }   
                                           
    }

// -----------------------------------------------------------
// CPhoneConference::IsConferenceBubbleInSelectionMode
// -----------------------------------------------------------
//
TBool CPhoneConference::IsConferenceBubbleInSelectionMode() const
    {
    TPhoneCmdParamBoolean booleanParam;
    
    iViewCommandHandle->ExecuteCommand( 
        EPhoneViewGetIsConferenceInSelectionMode, &booleanParam );
    
    return booleanParam.Boolean();
    }

// ---------------------------------------------------------
// CPhoneState::HandleCreateNumberEntryL
// ---------------------------------------------------------
//
void CPhoneConference::HandleCreateNumberEntryL( const TKeyEvent& aKeyEvent,
        TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneConference::HandleCreateNumberEntryL() ");
    if ( !IsConferenceBubbleInSelectionMode() )
        {
        CPhoneGsmInCall::HandleCreateNumberEntryL( aKeyEvent,aEventCode );
        }
    }
    
// -----------------------------------------------------------
// CPhoneConference::UpdateConferenceSecurityStatusL
// -----------------------------------------------------------
//
void CPhoneConference::UpdateConferenceSecurityStatusL( TInt aCallId )
    {
    __PHONELOG1( EBasic, EPhoneUIStates, 
        "CPhoneConference::UpdateConferenceSecurityStatusL() - callId = %d", aCallId );

    if( !iStateMachine->PhoneEngineInfo()->IsSecureCall( aCallId ) &&
        iStateMachine->PhoneEngineInfo()->IsSecureCall( KPEConferenceCallID ) )
        {
        TPhoneCmdParamCallHeaderData callHeaderParam;
        
        callHeaderParam.SetCiphering( EFalse );
        callHeaderParam.SetCipheringIndicatorAllowed(
            iStateMachine->PhoneEngineInfo()->SecureSpecified() );
        
        iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewCipheringInfoChange,
            KPEConferenceCallID,
            &callHeaderParam );
        }
    }

// End of File
