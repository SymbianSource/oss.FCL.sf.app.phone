/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of CPhoneEmergency class.
*
*/


// INCLUDES
#include <eikmenub.h>
#include <StringLoader.h>
#include <ccpdefs.h>
#include <featmgr.h>
#include <cpephonemodelif.h>
#include <mpeengineinfo.h>
#include <AknUtils.h>

#include "cphoneemergency.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamemergencycallheaderdata.h"
#include "tphonecmdparamcallheaderdata.h"
#include "mphonestatemachine.h"
#include "phonestatedefinitionsgsm.h"
#include "phoneviewcommanddefinitions.h"
#include "phoneui.hrh"
#include "phonerssbase.h"
#include "cphonemainresourceresolver.h"
#include "phonelogger.h"
#include "cphonepubsubproxy.h"
#include "cphonekeys.h"
#include "tphonecmdparamaudiooutput.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneEmergency::CPhoneEmergency(
    MPhoneStateMachine* aStateMachine,
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) :
    CPhoneGsmInCall( aStateMachine, aViewCommandHandle, aPhoneCustomization ),
    iCallSetup( ETrue )
    {
    }

// -----------------------------------------------------------
// CPhoneEmergency::~CPhoneEmergency()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------

CPhoneEmergency::~CPhoneEmergency()
    {
    }

// -----------------------------------------------------------
// CPhoneEmergency::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneEmergency::ConstructL()
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::ConstructL()");
    CPhoneGsmInCall::ConstructL();
    }

// -----------------------------------------------------------
// CPhoneEmergency::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneEmergency* CPhoneEmergency::NewL(
    MPhoneStateMachine* aStateMachine,
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneEmergency* self = new (ELeave)
        CPhoneEmergency( aStateMachine, aViewCommandHandle, aPhoneCustomization );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------
// CPhoneEmergency::HandleNumberEntryClearedL()
// -----------------------------------------------------------
//
void CPhoneEmergency::HandleNumberEntryClearedL()
    {
    __LOGMETHODSTARTEND(EPhoneUIStates,
        "CPhoneEmergency::HandleNumberEntryClearedL()");
    UpdateInCallCbaL();
    }

// -----------------------------------------------------------
// CPhoneEmergency::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneEmergency::HandlePhoneEngineMessageL(
    const TInt aMessage,
    TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates,
        "CPhoneEmergency::HandlePhoneEngineMessageL() ");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageIdle:
            iCallSetup = ETrue;
            HandleIdleL( aCallId );
            iConnected = EFalse;
            break;

        case MEngineMonitor::EPEMessageDialing:
            HandleDialingL( aCallId );
            break;

        case MEngineMonitor::EPEMessageConnected:
            if( iCallSetup )
                {
                HandleConnectingL( aCallId );
                iCallSetup = EFalse;
                }
            HandleConnectedL( aCallId );
            iConnected = ETrue;
            break;

        case MEngineMonitor::EPEMessageConnecting:
            HandleConnectingL( aCallId );
            iCallSetup = EFalse;
            break;

        case MEngineMonitor::EPEMessageDisconnecting:
            // Explicitly override this handling from CPhoneStateInCall
            // where DTMF sending is cancelled and that causes emergency
            // progress bar destroyed
            CPhoneState::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;

        case MEngineMonitor::EPEMessageAudioOutputChanged:
            if ( iCallSetup )
                {
                HandleAudioOutputChangedL();
                }
            else
                {
                CPhoneGsmInCall::HandlePhoneEngineMessageL( aMessage, aCallId );
                }
            break;

        case MEngineMonitor::EPEMessageStoppedDTMF:      // fall through
        case MEngineMonitor::EPEMessageSentDTMF:         // fall through
        case MEngineMonitor::EPEMessageDTMFSendingAborted:
            CPhoneGsmInCall::HandlePhoneEngineMessageL( aMessage, aCallId );
            if ( iStateMachine->PhoneEngineInfo()->CallState( KPEEmergencyCallId ) == EPEStateDialing )
                {
                TPhoneCmdParamBoolean isProgressNoteVisible;
                iViewCommandHandle->ExecuteCommandL( EPhoneViewGetIsProgressNoteVisible,
                      &isProgressNoteVisible );

                if ( !isProgressNoteVisible.Boolean() )
                    {
                    ShowNoteL( EPhoneEmergencyConnectWaitNote );
                    UpdateSetupCbaL();
                    }
                 }
            break;
            
        case MEngineMonitor::EPEMessageColpNumberAvailable:
            {
            //Don't show COLP note during emergency call.
            return;                    
            }
            break; 

        default:
            CPhoneGsmInCall::HandlePhoneEngineMessageL(
                aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneEmergency::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneEmergency::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::HandleIdleL() ");

    // Re-enable global notes, this enables secui (if needed) to come on top
    // if call creation was rejected by user
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );

    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
        {
        CloseDTMFEditorL();
        }

    if ( aCallId == KEmergencyCallId )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
        // Remove emergency connecting note if still there
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );
        // Close menu bar, if it is displayed
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

        const TBool isSimStateNotPresentWithSecurityMode = IsSimStateNotPresentWithSecurityModeEnabled();

        // Sim IS not ok when making emergency call from Pin query, no note
        if ( (!IsSimOk() && !iStartupInterrupted) || isSimStateNotPresentWithSecurityMode )
            {
            StartShowSecurityNoteL();
            }
        else
            {
            if ( !TopAppIsDisplayedL() || iDeviceLockOn  )
                {
                // Continue displaying current app but set up the
                // idle screen in the background
                SetupIdleScreenInBackgroundL();
                }

            else if ( iOnScreenDialer && IsNumberEntryContentStored() )
                {
                if ( !IsNumberEntryUsedL() )
                    {
                    CreateNumberEntryL();
                    }
                // Restore the number entry content from cache
                RestoreNumberEntryContentL();
                SetNumberEntryVisibilityL(ETrue);
                }

            else if ( IsNumberEntryUsedL() )
                {
                // Show the number entry if it exists
                SetNumberEntryVisibilityL(ETrue);
                }

            else
                {
                 // Display idle screen
                DisplayIdleScreenL();
                }

            // Display call termination note, if necessary
            if ( iConnected )
                {
                DisplayCallTerminationNoteL();
                }
            }

        // Go to emergency idle state or to startup state if emergency call
        // initiated from PIN query
        if ( iStartupInterrupted )
            {
            iStartupInterrupted = EFalse;
            iStateMachine->ChangeState( EPhoneStateStartup );
            }
        else
            {
            if ( !isSimStateNotPresentWithSecurityMode )
                {
                SetDefaultFlagsL();
                }

            // As long as security note is not shown with feature flag
            // KFeatureIdFfSimlessOfflineSupport undef it is ok to do SetupIdleScreenInBackgroundL.
            if ( ( !IsNumberEntryUsedL() ) &&
                 !( IsSimStateNotPresentWithSecurityModeEnabled() &&
                    !FeatureManager::FeatureSupported( KFeatureIdFfSimlessOfflineSupport ) ) )
                {
                UpdateCbaL( EPhoneEmptyCBA );

                TPhoneCmdParamBoolean securityMode;
                iViewCommandHandle->ExecuteCommandL( EPhoneViewGetSecurityModeStatus, &securityMode );
                if ( !securityMode.Boolean() )
                    {
                    // Setup idle as next active app.
                    SetupIdleScreenInBackgroundL();
                    }
                }

            iStateMachine->ChangeState( EPhoneStateIdle );
            }
        }
    else
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveAllCallHeaders );
        }
    }


// -----------------------------------------------------------
// CPhoneEmergency::HandleDialingL
// -----------------------------------------------------------
//
void CPhoneEmergency::HandleDialingL( TInt aCallId )
    {
    // Discard all messages related to other calls than the emergency call
    // We trust that the ISA call server has taken care of them. However,
    // there could be a race condition between returning to Normal
    // mode (receiving a network registration status message from PE)
    // and receiving an EPEMessageDialling message. This can occur
    // because PE messages are processed asynchronously.
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::HandleDialingL() ");

    if ( aCallId == KPEEmergencyCallId )
        {
        if ( !IsSimOk() )
            {
            TPhoneCmdParamBoolean visibleMode;
            visibleMode.SetBoolean( ETrue );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetStatusPaneVisible, &visibleMode );
            }

        iDeviceLockOn = IsAutoLockOn();

        // Close menu bar, if it is displayed
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

        // Disable global notes when the phone is dialling
        TPhoneCmdParamBoolean globalNotifierParam;
        globalNotifierParam.SetBoolean( ETrue );
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewSetGlobalNotifiersDisabled,
            &globalNotifierParam );

        // Capture keys when the phone is dialling
        CaptureKeysDuringCallNotificationL( ETrue );

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

        // Hide the number entry if it is visible
        if ( IsNumberEntryUsedL() )
            {
            // Remove number entry
            iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
            }

        // Remove exit emergency mode query, if it exists
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveQuery );

        // Enable the volume display
        iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNaviPaneAudioVolume );

        SetTouchPaneButtons( EPhoneEmergencyCallButtons );

        ShowNoteL( EPhoneEmergencyConnectWaitNote );

        UpdateSetupCbaL();
        }
    }
// -----------------------------------------------------------
// CPhoneEmergency::HandleConnectingL
// -----------------------------------------------------------
//
void CPhoneEmergency::HandleConnectingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::HandleConnectingL() ");

    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveGlobalNote );

    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetEikonNotifiersDisabled,
          &globalNotifierParam );

    if(  TouchCallHandlingSupported() )
        {
        if ( !IsSimStateNotPresentWithSecurityModeEnabled() )
            {
            // Stop capturing keys
            CaptureKeysDuringCallNotificationL( EFalse );
            }
        }
    else if ( !iDeviceLockOn && SimState() == EPESimUsable )
        {
        // Stop capturing keys
        CaptureKeysDuringCallNotificationL( EFalse );
        }

    // Keep Phone in the foreground
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewSetNeedToSendToBackgroundStatus, &booleanParam );

    // Remove emergency connecting note
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );

    TPhoneCmdParamEmergencyCallHeaderData emergencyHeaderParam;
    // Set call header
    TBuf<KPhoneCallHeaderLabelMaxLength> headerText( KNullDesC );
    StringLoader::Load(
        headerText,
        CPhoneMainResourceResolver::Instance()->ResolveResourceID(
            EPhoneEmergencyCallHeader ),
        CCoeEnv::Static() );
    emergencyHeaderParam.SetHeaderText( headerText );

    // Set call header ciphering status
    emergencyHeaderParam.SetCiphering(
        iStateMachine->PhoneEngineInfo()->IsSecureCall( aCallId ) );
    emergencyHeaderParam.SetCipheringIndicatorAllowed(
        iStateMachine->PhoneEngineInfo()->SecureSpecified() );

    BeginUiUpdateLC();

    // Notify the view
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewCreateEmergencyCallHeader,
        aCallId,
        &emergencyHeaderParam );

    TPhoneCmdParamCallHeaderData headerParam;
    headerParam.SetCallState( EPEStateConnecting );
    // Notify the view
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewUpdateBubble,
        aCallId,
        &headerParam );

    EndUiUpdate();

    //Make sure that toolbar is not shown
    iViewCommandHandle->ExecuteCommandL( EPhoneViewHideToolbar );

    UpdateInCallCbaL();
    }

// -----------------------------------------------------------
// CPhoneEmergency::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneEmergency::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::HandleConnectedL() ");

    BeginUiUpdateLC();

    TPhoneCmdParamCallHeaderData emergencyHeaderParam;
    emergencyHeaderParam.SetCallState( EPEStateConnected );
    // Notify the view
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewUpdateBubble,
        aCallId,
        &emergencyHeaderParam );

    EndUiUpdate();
    UpdateInCallCbaL();
    }

// -----------------------------------------------------------
// CPhoneEmergency::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneEmergency::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::OpenMenuBarL() ");
    TInt resourceId;

    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
        {
        resourceId = EPhoneDtmfDialerMenubar;
        }
    else if ( IsNumberEntryVisibleL() )
        {
        resourceId = EPhoneCallHandlingEmergencyMenubarWithNumberEntry;
        }
    else
        {
        resourceId = EPhoneCallHandlingEmergencyMenubar;
        }

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger(
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen,
        &integerParam );
    }


// -----------------------------------------------------------
// CPhoneEmergency::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneEmergency::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::UpdateInCallCbaL() ");
    TInt resourceId = EPhoneCallHandlingEmergencyCBA;
    const TPEAudioOutput audioOutput =
        iStateMachine->PhoneEngineInfo()->AudioOutput();
    const TBool btAvailable =
        iStateMachine->PhoneEngineInfo()->AudioOutputAvailable( EPEBTAudioAccessory );

    __PHONELOG2( EOnlyFatal, EPhoneUIStates,
            "CPhoneEmergency - AudioOutput: %d, BTAvailable: %d",
            audioOutput, btAvailable );

    if ( IsNumberEntryVisibleL() )
        {
        if ( IsDTMFEditorVisibleL() )
            {
            resourceId = EPhoneDtmfDialerCBA;
            }
        else
            {
            resourceId = EPhoneInCallNumberAcqCBA;
            }
        }
    else if ( iDeviceLockOn || SimState() != EPESimUsable || iStartupInterrupted )
        {
        if ( TouchCallHandlingSupported() )
        	{
            resourceId = EPhoneCallHandlingEmergencyInCallNoOptions;
            }
        else if ( audioOutput == EPEWiredAudioAccessory || IsSwivelClosed())
            {
            resourceId = EPhoneCallHandlingEmergencyNoOptionsNoIhfCBA;
            }
        else if ( audioOutput == EPELoudspeaker )
            {
            resourceId = EPhoneCallHandlingEmergencyNoOptionsHandsetCBA;
            }
        else
            {
            resourceId = EPhoneCallHandlingEmergencyNoOptionsCBA;
            }
        }
    else
        {
        if ( UseEmergencyNoIhfCBA( audioOutput ) )
            {
            resourceId = EPhoneCallHandlingEmergencyNoIhfCBA;
            }
        else if ( UseHandsetEmergencyCBA( audioOutput ) )
            {
            resourceId = EPhoneCallHandlingEmergencyHandsetCBA;
            }
        else if ( ( audioOutput == EPEHandset ) 
        		&& ( btAvailable ) )
            {
            resourceId = EPhoneCallHandlingInCallBtaaCBA;
            }
        }

    iCbaManager->SetCbaL( resourceId );
    }

// --------------------------------------------------------------
// CPhoneEmergency::HandleKeyMessageL
// --------------------------------------------------------------
//
void CPhoneEmergency::HandleKeyMessageL(
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::HandleKeyMessageL()");

    switch ( aCode )
        {
        // end-key
        case EKeyNo:
            // handle end key
            if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
                {
                CloseDTMFEditorL();
                }
            DisconnectEmergencyCallL();
            break;

        // send-key
        case EKeyYes:
             if( iOnScreenDialer && IsNumberEntryVisibleL() )
                {
                TPhoneCmdParamInteger numberEntryCountParam;
                    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetNumberEntryCount,
                            &numberEntryCountParam );
                    TInt neLength( numberEntryCountParam.Integer() );
                if ( neLength )
                    {
                    // Show not allowed note
                    SendGlobalErrorNoteL( EPhoneNoteTextNotAllowed );
                    }
                else
                    {
                    // Logs to open
                    CallFromNumberEntryL();
                    }
                }
            else
                {
                // Show not allowed note
                SendGlobalErrorNoteL( EPhoneNoteTextNotAllowed );
                }
            break;

        default:
            {
            if ( TouchCallHandlingSupported() )
                {
                CPhoneGsmInCall::HandleKeyMessageL( aMessage, aCode ); 
                }
            else if ( !iDeviceLockOn && SimState() == EPESimUsable )   
                {
                // do base operation
                CPhoneGsmInCall::HandleKeyMessageL( aMessage, aCode );
                }
            break;
            }
        }
    }

// -----------------------------------------------------------
// CPhoneEmergency::HandleCommandL
// -----------------------------------------------------------
//
TBool CPhoneEmergency::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::HandleCommandL()");
    TBool commandStatus = ETrue;

    switch( aCommand )
        {
        case EPhoneInCallCmdDtmfManualQuery:
            if ( iStateMachine->PhoneEngineInfo()->CallState( KPEEmergencyCallId ) == EPEStateDialing )
                {
                // Remove emergency connecting note
                iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );
                }
            commandStatus = CPhoneGsmInCall::HandleCommandL( aCommand );
            break;

        case EPhoneDtmfDialerExit:      // fall through
        case EPhoneDtmfDialerCancel:
             {
             if ( iStateMachine->PhoneEngineInfo()->CallState( KPEEmergencyCallId ) == EPEStateDialing )
                 {
                 CloseDTMFEditorL();

                 TPhoneCmdParamBoolean isProgressNoteVisible;
                 iViewCommandHandle->ExecuteCommandL( EPhoneViewGetIsProgressNoteVisible,
                        &isProgressNoteVisible );

                 if ( !isProgressNoteVisible.Boolean() )
                     {
                     ShowNoteL( EPhoneEmergencyConnectWaitNote );
                     UpdateSetupCbaL();
                     }
                 }
             else
                 {
                 commandStatus = CPhoneGsmInCall::HandleCommandL( aCommand );
                 }
             }
             break;

        case EPhoneInCallCmdEndThisOutgoingCall:
        case EPhoneInCallCmdEndThisActiveCall:
            DisconnectEmergencyCallL();
            break;

        default:
            commandStatus = CPhoneGsmInCall::HandleCommandL( aCommand );
            break;
        }

    return commandStatus;
    }

// -----------------------------------------------------------
// CPhoneEmergency::DisconnectEmergencyCallL
// -----------------------------------------------------------
//
void CPhoneEmergency::DisconnectEmergencyCallL()
    {
    __LOGMETHODSTARTEND(EPhoneUIStates,
        "CPhoneEmergency::DisconnectEmergencyCallL()");
    // Release the call
    iStateMachine->SetCallId(
        KEmergencyCallId );
    iStateMachine->SendPhoneEngineMessage(
        CPEPhoneModelIF::EPEMessageRelease );
    }

// -----------------------------------------------------------
// CPhoneEmergency::HandleErrorL
// Emergency handling for HandleError message
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneEmergency::HandleErrorL( const TPEErrorInfo& aErrorInfo )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::HandleError()");

    switch( aErrorInfo.iErrorCode )
        {
        case ECCPErrorCCNoRouteToDestination:
        case ECCPErrorUnacceptableChannel:
        case ECCPErrorCCDestinationOutOfOrder:
        case ECCPErrorNetworkOutOfOrder:
        case ECCPErrorAccessInformationDiscarded:
        case ECCPErrorCCResourceNotAvailable:
        case ECCPErrorQualityOfServiceNotAvailable:
        case ECCPErrorInvalidCallReferenceValue:
        case ECCPErrorBadRequest:
        case ECCPErrorConnectionError:
        case ECCPErrorCCIncompatibleMessageInProtocolState:
        case ECCPErrorCCNoChannelAvailable:
        case ECCPErrorNetworkBusy:
        case ECCPEmergencyFailed:
            SendGlobalErrorNoteL( EPhoneNoteNoNetworkCallEmergency );
            break;

       default:
        __PHONELOG1(
            EOnlyFatal,
            EPhoneUIStates,
            "PHONEUI_ERROR: CPhoneEmergency::HandleError - Error send to PhoneState (err=%d)",
            aErrorInfo.iErrorCode);
            CPhoneState::HandleErrorL( aErrorInfo );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneEmergency::HandleKeyEventL
// -----------------------------------------------------------
//
void CPhoneEmergency::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::HandleKeyEventL( ) ");
    
    if ( TouchCallHandlingSupported() )
        {
        CPhoneState::HandleKeyEventL( aKeyEvent, aEventCode );
        }
    else
        {
		 if ( iDeviceLockOn && CPhoneKeys::IsNumericKey( aKeyEvent, aEventCode ) )
			{
			// Send the key event to the phone engine
			SendKeyEventL( aKeyEvent, aEventCode );
			}
		else
			{
			// Handle numeric keys when key events are received in idle state
			CPhoneState::HandleKeyEventL( aKeyEvent, aEventCode );
			}
        }
    }

// -----------------------------------------------------------
// CPhoneEmergency::SendKeyEventL
// -----------------------------------------------------------
//
void CPhoneEmergency::SendKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::SendKeyEventL( ) ");
    switch( aEventCode )
        {
        // EEventKey
        case EEventKey:
            {
            // Convert key code to western.
            TBuf<1> buffer; // one character
            buffer.Append( aKeyEvent.iCode );
            __PHONELOG1( EBasic, EPhoneControl,
                "CPhoneEmergency::SendKeyEventL(%S)",
                &buffer );
            AknTextUtils::ConvertDigitsTo( buffer, EDigitTypeWestern );
            __PHONELOG1( EBasic, EPhoneControl,
                "CPhoneEmergency::SendKeyEventL(%S)",
                &buffer );
            TLex code( buffer );
            // Save the key code
            iStateMachine->PhoneEngineInfo()->SetKeyCode( code.Peek() );
            // Plays a DTMF tone if active call
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessagePlayDTMF );
            }
            break;
        // EEventKeyUp
        case EEventKeyUp:
            // Send a key up event for the last key code sent to
            // the phone engine
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessageEndDTMF );
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPhoneEmergency::HandleAudioOutputChangedL
// for setup
// -----------------------------------------------------------------------------
//
void CPhoneEmergency::HandleAudioOutputChangedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneEmergency::HandleAudioOutputChangedL( ) ");

    // Audio Output
    TPhoneCmdParamAudioOutput outputParam;

    // Output
    const TPEAudioOutput audioOutput =
        iStateMachine->PhoneEngineInfo()->AudioOutput();

    UpdateSetupCbaL();

    // view update
    outputParam.SetAudioOutput( audioOutput );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewActivateAudioPathUIChanges,
        &outputParam );

    }

// -----------------------------------------------------------------------------
// CPhoneEmergency::SetStartupInterrupted
// -----------------------------------------------------------------------------
//
void CPhoneEmergency::SetStartupInterrupted( const TBool aStartupInterrupted )
    {
    iStartupInterrupted = aStartupInterrupted;
    }

// -----------------------------------------------------------
// CPhoneEmergency::UpdateSetupCbaL
// -----------------------------------------------------------
//
void CPhoneEmergency::UpdateSetupCbaL()
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneEmergency::UpdateSetupCbaL() ");

    TInt resourceId = EPhoneCallHandlingCallSetupCBA;

    const TPEAudioOutput audioOutput =
        iStateMachine->PhoneEngineInfo()->AudioOutput();

    if ( !( TouchCallHandlingSupported() ) 
    		&& iCallSetup && audioOutput != EPENotActive )
        {
        if ( audioOutput == EPELoudspeaker )
            {
            resourceId = EPhoneCallHandlingCallSetupToHandsetCBA;
            }
        else if ( audioOutput == EPEHandset )
            {
            resourceId =  EPhoneCallHandlingCallSetupToIhfCBA;
            }
        else
            {
            resourceId = EPhoneCallHandlingCallSetupCBA;
            }
        TPhoneCmdParamInteger integerParam;
        integerParam.SetInteger(
                CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( resourceId ) );

        iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateNoteSoftkeys,
                &integerParam );
        }
    }

// ---------------------------------------------------------
// CPhoneEmergency::HandleCreateNumberEntryL
//
// ---------------------------------------------------------
//
void CPhoneEmergency::HandleCreateNumberEntryL( const TKeyEvent& aKeyEvent,
        TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneEmergency::HandleCreateNumberEntryL() ");
    if ( !iCallSetup && !iStartupInterrupted )
        {
        CPhoneGsmInCall::HandleCreateNumberEntryL( aKeyEvent, aEventCode );
        }

    }

// -----------------------------------------------------------------------------
// CPhoneEmergency::HandleRemConCommandL
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneEmergency::HandleRemConCommandL(
    TRemConCoreApiOperationId aOperationId,
    TRemConCoreApiButtonAction aButtonAct )
    {
    TBool handled = EFalse;

    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneEmergency::MrccatoCommand() ");

    switch ( aOperationId )
        {
         case ERemConCoreApiMute:
             __PHONELOG( EBasic, EPhoneControl, "CPhoneEmergency::MrccatoCommand emerg. call no muting" );
             // Not Allowed
             break;
         default:
             handled = CPhoneState::HandleRemConCommandL( aOperationId, aButtonAct );
             break;
        }
    return handled;
    }

// --------------------------------------------------------------
// CPhoneEmergency::TouchCallHandlingSupported
// --------------------------------------------------------------
//
TBool CPhoneEmergency::TouchCallHandlingSupported () const
	{
	if ( FeatureManager::FeatureSupported ( KFeatureIdTouchCallHandling ) )
		{
		return ETrue;
		}
	else 
		{
		return EFalse;
		}
	}

// --------------------------------------------------------------
// CPhoneEmergency::UseEmergencyNoIhfCBA
// --------------------------------------------------------------
//
TBool CPhoneEmergency::UseEmergencyNoIhfCBA( const TPEAudioOutput& aAudioOutput ) const
	{
	if ( !( TouchCallHandlingSupported() ) 
		&& ( ( aAudioOutput == EPEWiredAudioAccessory ) || ( IsSwivelClosed() ) ) )
		{
		return ETrue;
		}
	else 
		{
		return EFalse;
		}
	}

// --------------------------------------------------------------
// CPhoneEmergency::UseHandsetEmergencyCBA
// --------------------------------------------------------------
//
TBool CPhoneEmergency::UseHandsetEmergencyCBA( const TPEAudioOutput& aAudioOutput ) const
    {
	if ( !( TouchCallHandlingSupported() ) 
		&& ( ( aAudioOutput == EPELoudspeaker ) || ( aAudioOutput == EPEBTAudioAccessory ) ) )
		{
		return ETrue;
		}
	else 
		{
		return EFalse;
		}
    }

// --------------------------------------------------------------
// CPhoneEmergency::HandlePhoneStartupL
// --------------------------------------------------------------
//
void CPhoneEmergency::HandlePhoneStartupL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneEmergency::HandlePhoneStartupL( ) ");
    iStartupInterrupted = EFalse;
    }

// End of File
