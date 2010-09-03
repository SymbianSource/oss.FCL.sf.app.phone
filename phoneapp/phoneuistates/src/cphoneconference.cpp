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
            TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
            TInt callLabelId = CPhoneMainResourceResolver::Instance()->
                    ResolveResourceID( EPhoneCallOnHold );

            StringLoader::Load( labelText, callLabelId, CCoeEnv::Static() );
            callHeaderParam.SetLabelText( labelText );
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
            
        case EPhoneInCallCmdDrop:
            DropSelectedParticipantL();
            break;
            
        case EPhoneInCallCmdPrivate:
            PrivateSelectedParticipantL();
            break;
            
        case EPhoneInCallCmdPrivateSelection:
        case EPhoneInCallCmdDropSelection:
        case EPhoneInCallCmdNewCall:
        case EPhoneInCallCmdNewCallCall:
        case EPhoneInCallCmdParticipants:
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
    UpdateCbaL( EPhoneCallHandlingInCallCBA );
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
    if ( !CPhoneKeys::IsNumericKey( aKeyEvent, aEventCode ) )
        {
        // Handle other key events than numeric key events.
        CPhoneState::HandleKeyEventL( aKeyEvent, aEventCode );
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
        case EKeyYes: // send-key
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
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );
        
    // If conference bubble still exists then we have conference and
    // single call
    TPhoneCmdParamBoolean conferenceBubbleExists;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetIsConference, 
        &conferenceBubbleExists ); 
        
    if( conferenceBubbleExists.Boolean() )
        {
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewRemoveFromConference, aCallId ); 
        }
    else
        {
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
    callHeaderParam.SetCallState( EPEStateHeldConference );
    
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
    SetTouchPaneButtons( EPhoneConferenceButtons );
    UpdateInCallCbaL();
    }

// -----------------------------------------------------------
// CPhoneConference::HandleConnectedConferenceL
// -----------------------------------------------------------
//
void CPhoneConference::HandleConnectedConferenceL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleConnectedConferenceL()");
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnectedConference );
    
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
    
    SetTouchPaneButtons( EPhoneConferenceButtons );
    UpdateInCallCbaL();
    }

// -----------------------------------------------------------
// CPhoneConference::DropSelectedParticipantL
// -----------------------------------------------------------
//
void CPhoneConference::DropSelectedParticipantL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::DropSelectedParticipantL()");
    TPhoneViewResponseId response;
    TPhoneCmdParamInteger callId;
    response = iViewCommandHandle->HandleCommandL(
        EPhoneViewSelectedConfMember, &callId );    
        
    if( response == EPhoneViewResponseSuccess )
        {
        iStateMachine->SetCallId( callId.Integer() );    
        iStateMachine->SendPhoneEngineMessage( 
            CPEPhoneModelIF::EPEMessageDropConferenceMember );
        }
    }

// -----------------------------------------------------------
// CPhoneConference::PrivateSelectedParticipantL
// -----------------------------------------------------------
//
void CPhoneConference::PrivateSelectedParticipantL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::PrivateSelectedParticipantL()");
    TPhoneViewResponseId response;
    TPhoneCmdParamInteger callId;
    response = iViewCommandHandle->HandleCommandL(
        EPhoneViewSelectedConfMember, &callId );
    
    if( response == EPhoneViewResponseSuccess )
        {
        iStateMachine->SetCallId( callId.Integer() );    
        iStateMachine->SendPhoneEngineMessage( 
            CPEPhoneModelIF::EPEMessageGoOneToOne );
        BeginUiUpdateLC();
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
        SetNumberEntryVisibilityL(ETrue);
        }
    else if ( !TopAppIsDisplayedL() )
        {
        
        // Continue displaying current app but set up the 
        // idle screen in the background
        SetupIdleScreenInBackgroundL();
        }
    else
        {
        DisplayIdleScreenL();
        }
    DisplayCallTerminationNoteL();
    UpdateCbaL( EPhoneEmptyCBA );
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
    if ( IsNumberEntryUsedL() )
        {
        SetNumberEntryVisibilityL(ETrue);
        }
    SetTouchPaneButtons( EPhoneIncallButtons );
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
    if ( IsNumberEntryUsedL() )
        {
        SetNumberEntryVisibilityL(ETrue);
        }
        
    SetTouchPaneButtons( EPhoneTwoSinglesButtons );
    UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
    iStateMachine->ChangeState( EPhoneStateTwoSingles );
    }

// -----------------------------------------------------------
// CPhoneConference::ToggleHold
// -----------------------------------------------------------
//
void CPhoneConference::ToggleHoldL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::ToggleHoldL()");

    iStateMachine->SetCallId( KConferenceCallId );         
    iStateMachine->SendPhoneEngineMessage( 
        CPEPhoneModelIF::EPEMessageSwapConference );
    }

// -----------------------------------------------------------
// CPhoneConference::HandleDiallingL
// -----------------------------------------------------------
//
void CPhoneConference::HandleDiallingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConference::HandleDiallingL()");
    BeginUiUpdateLC(); 
    SetNumberEntryVisibilityL(EFalse);
    DisplayCallSetupL( aCallId );
    SetTouchPaneButtons(EPhoneCallSetupAndSingleButtons);
    EndUiUpdate();
    UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
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
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );
    DisplayHeaderForOutgoingCallL( aCallId );
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
    // Close fast swap window if it's displayed
    CEikonEnv::Static()->DismissTaskList();
    
    CheckDisableHWKeysAndCallUIL();
    DisplayIncomingCallL( aCallId, dialerParam );
    SetTouchPaneButtons( EPhoneWaitingCallButtons );
    EndUiUpdate();
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
    // Cannot delete active note, e.g. New call query, 
    // but show waiting note with or without caller name
    if ( IsAnyQueryActiveL() || !aCommandParam.Boolean() )
        {
        CallWaitingNoteL( aCallId ); 
        }
    else
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
        }
    // Indicate that the Phone needs to be sent to the background if
    // an application other than the top application is in the foreground
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( !TopAppIsDisplayedL() );
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewSetNeedToSendToBackgroundStatus,
        &booleanParam );

    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );
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
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewPrivateFromConference, aCallId );
        
    // If conference bubble still exists then we have conference and
    // single call
    TPhoneCmdParamBoolean conferenceBubbleExists;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetIsConference, 
        &conferenceBubbleExists ); 
    if( conferenceBubbleExists.Boolean() )
        {
        UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
        iStateMachine->ChangeState( EPhoneStateConferenceAndSingle );
        SetTouchPaneButtons( EPhoneConferenceAndSingleButtons );
        }
    else
        {
        SetTouchPaneButtons( EPhoneTwoSinglesButtons );
        }
    EndUiUpdate();
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
