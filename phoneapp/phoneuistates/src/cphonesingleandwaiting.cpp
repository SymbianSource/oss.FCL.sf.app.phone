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
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneSingleAndWaiting::~CPhoneSingleAndWaiting()
    {
    if( iViewCommandHandle )
        {
        TPhoneCmdParamBoolean dtmfSendFlag;
        dtmfSendFlag.SetBoolean( EFalse );
        TRAP_IGNORE( iViewCommandHandle->ExecuteCommandL( 
                        EPhoneViewSetVideoCallDTMFVisibilityFlag,
                        &dtmfSendFlag ); )
        }
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::ConstructL()
// Constructor
// (other items were commented in a header).
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
// Constructor
// (other items were commented in a header).
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
            HandleDialingL( aCallId );
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
            LaunchNewCallQueryL();
            break;

        case EPhoneInCallCmdNewCallCall:
            CallFromNewCallQueryL();
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

        default:
            commandStatus = CPhoneGsmInCall::HandleCommandL( aCommand );
            break;
        }

    return commandStatus;
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::OpenMenuBarL() ");
    TInt resourceId ( KErrNone );


    if ( !IsAutoLockOn() )
        {
         if( IsVideoCallActiveL() || IsVideoCallRingingL() )
            {
            //DTMF hiding will be set to TRUE
            if( IsVideoCallActiveL() )
                {
                TPhoneCmdParamBoolean dtmfSendFlag;
                dtmfSendFlag.SetBoolean( ETrue );
                iViewCommandHandle->ExecuteCommandL( EPhoneViewSetVideoCallDTMFVisibilityFlag,
                        &dtmfSendFlag );
                }
            if ( IsNumberEntryVisibleL() )
                {
                resourceId = EPhoneVideoCallWaitingMenubarWithNumberEntry;
                }
            else
                {
                resourceId = EPhoneVideoCallWaitingMenubar;
                }
            }

        else // single voice and waiting voice
            {
            if ( IsNumberEntryVisibleL() )
                {
                resourceId = EPhoneCallWaitingMenubarWithNumberEntry;
                }
            else
                {
                resourceId = EPhoneCallWaitingMenubar;
                }
            }
        }
    else
        {
        resourceId = EPhoneCallWaitingLockMenubar;
        }


    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger(
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen,
        &integerParam );
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
        // send-key
        case EKeyYes:
            {
            if( IsNumberEntryVisibleL() )
                {
                // Send a manual control sequence by providing number
                // information with Dial command
                CallFromNumberEntryL();
                }
             else
                {
                // Close DTMF sending note if it exists
                iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );

                // Number entry is below incoming call bubble so
                // answer the call
                iStateMachine->SendPhoneEngineMessage(
                    CPEPhoneModelIF::EPEMessageAnswer );
                }
            break;
            }

        default:
            // do base operation
            CPhoneGsmInCall::HandleKeyMessageL( aMessage, aCode );
            break;
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
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

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
    
    if( iSingleCallId == aCallId )
        {
        // Idle message came for active call
        BeginUiUpdateLC();

        // Remove call
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

        // Get waiting callId
        TPhoneCmdParamCallStateData callStateData;
        callStateData.SetCallState( EPEStateRinging );
        iViewCommandHandle->HandleCommandL( EPhoneViewGetCallIdByState,
            &callStateData );
        
        __PHONELOG1( EBasic, EPhoneControl, 
            "CPhoneSingleAndWaiting::HandleIdleL EPEStateRinging CallId(%d)",
            callStateData.CallId() );
        
        if ( callStateData.CallId() > KErrNotFound )
            {
            // Display ringing bubble
            TPhoneCmdParamCallHeaderData callHeaderParam;
            callHeaderParam.SetCallState( EPEStateRinging );

            // Set Hold flag to view
            TPhoneCmdParamBoolean holdFlag;
            holdFlag.SetBoolean( EFalse );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
            
            SetCallHeaderTextsForCallComingInL( callStateData.CallId(), EFalse, &callHeaderParam );
            
            iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble,
                callStateData.CallId(),
                &callHeaderParam );

            // Hide number entry if visible
            if ( IsNumberEntryVisibleL() )
                {
                SetNumberEntryVisibilityL(EFalse);
                }

            SetTouchPaneButtons( EPhoneIncomingCallButtons );
            SetTouchPaneButtonEnabled( EPhoneCallComingCmdSilent );
            
            // Bring up callhandling view
            BringIncomingToForegroundL();

            //Set incoming cba
            iCbaManager->UpdateIncomingCbaL( callStateData.CallId() );
            UpdateSilenceButtonDimming();
            
            // Play ringtone
            SetRingingTonePlaybackL( callStateData.CallId() );
            }
        EndUiUpdate();
        SetToolbarDimming( ETrue );

        iStateMachine->ChangeState( EPhoneStateIncoming );
        }
    else
        {
        BeginUiUpdateLC();

        // Remove call
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
        // Idle message came for waiting call
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
            UpdateCbaL( EPhoneCallHandlingInCallCBA );
            // If numberentry is not open just check NeedToReturnToForegroundAppL and
            // sendbackround if needed.
            if ( NeedToReturnToForegroundAppL() )
                {
                // Return phone to the background if send to background is needed.
                iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
                }
            }
        
        if ( IsVideoCall( iSingleCallId ) )
            {
            TPhoneCmdParamInteger uidParam;
            uidParam.SetInteger( KVtUiAppUidValue.iUid );

            // Set video Phone as the top application
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
                &uidParam );
            }
        
        SetTouchPaneButtons( EPhoneIncallButtons );
        EndUiUpdate();
        // CBA updates in above if-else conditions
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

     // Display hold bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateHeld );

    TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
    TInt callLabelId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCallOnHold );

    StringLoader::Load( labelText, callLabelId, CCoeEnv::Static() );
    callHeaderParam.SetLabelText( labelText );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );

    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    if ( !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        HandleHoldNoteL( aCallId, ETrue );
        }

    UpdateInCallCbaL();
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
    // Close menu if focus is somewhere else than number entry.
    if ( !IsNumberEntryUsedL() )
        {
        // Close menubar
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
        }

    // Show bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );

    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    if ( !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        HandleHoldNoteL( aCallId, EFalse );
        }

    // Set CBAs
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::MakeStateTransitionToTwoSinglesL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::MakeStateTransitionToTwoSinglesL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::MakeStateTransitionToTwoSinglesL() ");
    BeginUiUpdateLC();
    UpdateRemoteInfoDataL ( aCallId );
    // Show bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );

    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    // Close dtmf dialer if exist.
    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
        {
        CloseDTMFEditorL();
        }
    else if ( iOnScreenDialer && IsCustomizedDialerVisibleL() )
        {
        CloseCustomizedDialerL();
        }
     
    if ( IsNumberEntryUsedL() && NeedToReturnToForegroundAppL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetControlAndVisibility );
        UpdateCbaL( EPhoneCallHandlingInCallCBA );
        }
    else
        {
        // Set Two singles state softkeys
        UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
        }
    
    SetTouchPaneButtons( EPhoneTwoSinglesButtons );
    EndUiUpdate();
    // CBA updates in above if-else conditions
    iStateMachine->ChangeState( EPhoneStateTwoSingles );
    }

// ---------------------------------------------------------
// CPhoneSingleAndWaiting::CallFromNewCallQueryL
// ---------------------------------------------------------
//
void CPhoneSingleAndWaiting::CallFromNewCallQueryL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::CallFromNewCallQueryL() ");
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

    DialVoiceCallL();
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::HandleDialingL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::HandleDialingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::HandleDialingL() ");
    // Display call setup
    DisplayCallSetupL( aCallId );
    // Go to Single And Waiting And Call Setup state
    iStateMachine->ChangeState( EPhoneStateSingleAndCallSetupAndWaiting );
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::DoStateSpecificCallSetUpDefinitionsL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneSingleAndWaiting::DoStateSpecificCallSetUpDefinitionsL()
    {
    // Set Hold flag to view ( DTMF menu label have to present )
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
    SetTouchPaneButtons( EPhoneCallSetupButtons );
    }

// -----------------------------------------------------------
// CPhoneSingleAndWaiting::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneSingleAndWaiting::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneSingleAndWaiting::UpdateInCallCbaL() ");
     UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
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
void CPhoneSingleAndWaiting::HandleDisconnectingL( TInt /*aCallId*/ )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneSingleAndWaiting::HandleDisconnectingL( ) ");
    }

// End of File
