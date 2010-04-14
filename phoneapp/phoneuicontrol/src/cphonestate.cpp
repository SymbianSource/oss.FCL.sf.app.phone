/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of CPhoneState class.
*
*/


// INCLUDE FILES
#include <avkon.rsg>
#include <bautils.h>
#include <eikenv.h>
#include <StringLoader.h>
#include <pevirtualengine.h>
#include <mpeclientinformation.h>
#include <mpeengineinfo.h>
#include <cpephonemodelif.h>
#include <featmgr.h>
#include <rconnmon.h>
#include <Profile.hrh>
#include <PUAcodes.hrh>
#include <AknUtils.h>
#include <telephonyvariant.hrh>
#include <aknsoundsystem.h>
#include <cphcntprofileengine.h>
#include <wlaninternalpskeys.h>
#include <btengdomainpskeys.h>
#include <btengdomaincrkeys.h>
#include <settingsinternalcrkeys.h>
#include <starterclient.h>
#include <RSSSettings.h>
#include <UikonInternalPSKeys.h>
#include <telephonydomainpstypes.h>
#include <telinformationpskeys.h>
#include <activeidle2domainpskeys.h>
#include <coreapplicationuisdomainpskeys.h>
#include <mccecall.h>
#include <ccpdefs.h>
#include <LogsDomainCRKeys.h>
#include <spsettings.h>
#include <ScreensaverInternalPSKeys.h>
#include <startupdomainpskeys.h>
#include <MediatorDomainUIDs.h>
#include <videotelcontrolmediatorapi.h>
#include <textresolver.h>
#include <phoneappvoipcommands.hrh>
#include <hwrmdomainpskeys.h>

#include "phoneui.pan"
#include "cphonestate.h"
#include "mphonestatemachine.h"
#include "mphoneviewcommandhandle.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamkeyevent.h"
#include "tphonecmdparamdynmenu.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamincallindicatordata.h"
#include "tphonecmdparamnote.h"
#include "tphonecmdparamKeycapture.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamquery.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparammessageeditor.h"
#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparambitmap.h"
#include "tphonecmdparamaudiooutput.h"
#include "tphonecmdparamaudioavailability.h"
#include "tphonecmdparamappinfo.h"
#include "tphonecmdparamtranseffect.h"
#include "tphonecmdparamringtone.h"
#include "tphonecmdparamcustomdialer.h"
#include "cphonekeys.h"
#include "phoneui.hrh"
#include "phonerssbase.h"
#include "cphonebtaadisconnecthandler.h"
#include "cphonemainresourceresolver.h"
#include "cphonecenrepproxy.h"
#include "cphonepubsubproxy.h"
#include "phoneviewcommanddefinitions.h"
#include "phonelogger.h"
#include "phonestatedefinitions.h"
#include "cphonetimer.h"
#include "cphonereconnectquery.h"
#include "mphoneerrormessageshandler.h"
#include "cphoneclearblacklist.h"
#include "mphonecustomization.h"
#include "cphonemainerrormessageshandler.h"
#include "cphoneaccessorybthandler.h"
#include "cphonemediatorfactory.h"
#include "mphonemediatormessage.h"
#include "cphonemediatorsender.h"
#include "cphonereleasecommand.h"
#include "cphonecontinueemergencycallcommand.h"
#include "cphonecallheadermanager.h"
#include "cphonenumberentrymanager.h"
#include "mphonesecuritymodeobserver.h"
#include "easydialingcommands.hrh"


// ================= MEMBER FUNCTIONS =======================

EXPORT_C CPhoneState::CPhoneState(
    MPhoneStateMachine* aStateMachine,
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aCustomization) :
    iStateMachine( aStateMachine ),
    iViewCommandHandle( aViewCommandHandle ),
    iCustomization( aCustomization ),
    iEnv( *CEikonEnv::Static() )
    {
    // Need to get current SimState for inherited classis
    iPreviousSimState = SimState();
    __ASSERT_ALWAYS(
        aStateMachine && aViewCommandHandle,
        Panic( EPhoneCtrlParameterNotInitialized ) );
    if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) )
        {
        iOnScreenDialer = ETrue;
        }
    }

EXPORT_C void CPhoneState::BaseConstructL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::BaseConstructL() ");
    if ( !iCbaManager )
        {
        iCbaManager = CPhoneCbaManager::NewL(
                        this,
                        *iViewCommandHandle,
                        *iStateMachine );
        }
    }

EXPORT_C CPhoneState::~CPhoneState()
    {
    if( iAlsLineChangeKeyPressTimer )
        {
        if( iAlsLineChangeKeyPressTimer->IsActive() )
            {
            iAlsLineChangeKeyPressTimer->CancelTimer();
            }
        delete iAlsLineChangeKeyPressTimer;
        }
    delete iNumberEntryManager;
    delete iCallHeaderManager;
    delete iCbaManager;
    }

// <-------------------------- PHONE ENGINE EVENTS --------------------------->

// -----------------------------------------------------------
// CPhoneState::HandlePhoneEngineMessageL
// Default handling for Phone Engine messages
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandlePhoneEngineMessageL(
    const TInt aMessage,
    TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandlePhoneEngineMessageL() ");

    CPhoneAccessoryBTHandler* accessoryBtHandler;

    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageNetworkRegistrationStatusChange:
            {
            __PHONELOG1( EBasic, EPhoneControl,
               "PHONEUI_ERROR: EPEMessageNetworkRegistrationStatusChange - RegistrationStatus =%d ",
               iStateMachine->PhoneEngineInfo()->NetworkRegistrationStatus());
            }
            break;

        case MEngineMonitor::EPEMessageAudioMuteChanged:
            HandleAudioMuteChangedL();
            break;

        case MEngineMonitor::EPEMessageAudioOutputChanged:
            HandleAudioOutputChangedL();
            // Go to current state implementation
            iCbaManager->UpdateInCallCbaL();
            break;

        case MEngineMonitor::EPEMessageAvailableAudioOutputsChanged:
            HandleAudioAvailableOutputChangedL();
            if ( iStateMachine->PhoneEngineInfo()->AudioOutput() != EPENotActive )
                {
                // Go to current state implementation
                iCbaManager->UpdateInCallCbaL();
                }
            break;

        case MEngineMonitor::EPEMessageChangedCallDuration:
            HandleChangedCallDurationL( aCallId );
            break;

        case MEngineMonitor::EPEMessageRemoteBusy:
            HandleRemoteBusyL( aCallId );
            break;

        case MEngineMonitor::EPEMessageCallWaiting:
            SendGlobalInfoNoteL( EPhoneWaitingText );
            break;

        case MEngineMonitor::EPEMessageProfileChanged:
            {
            TPhoneCmdParamBoolean keypadVolumeParam;
            if ( iStateMachine->PhoneEngineInfo()->KeypadVolume() == 0 )
                {
                iViewCommandHandle->ExecuteCommandL( EPhoneViewKeypadAudioDisabled );
                }
            else
                {
                iViewCommandHandle->ExecuteCommandL( EPhoneViewKeypadAudioEnabled );
                }
            UpdateProfileDisplayL();
            }
            break;

        case MEngineMonitor::EPEMessageRemoteTerminated:
            // Close menu bar, if it is displayed, for call ending note
            iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
            /* Flow through */
        case MEngineMonitor::EPEMessageDisconnecting:
            HandleDisconnectingL( aCallId );
            break;

        case MEngineMonitor::EPEMessageTransferDone:
            SendGlobalInfoNoteL( EPhoneInCallTransferred );
            break;

       case MEngineMonitor::EPEMessageInitiatedEmergencyCall:
            HandleInitiatedEmergencyCallL( aCallId );
            break;

       case MEngineMonitor::EPEMessageInitiatedEmergencyWhileActiveVideo:
           HandleInitiatedEmergencyWhileActiveVideoL();
           break;

        case MEngineMonitor::EPEMessageShowIMEI:
            // Stop playing DTMF tone
            iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );
            HandleShowImeiL();
            break;

        case MEngineMonitor::EPEMessageCallSecureStatusChanged:
            HandleCallSecureStatusChangeL( aCallId );

            if ( iCustomization )
                {
                iCustomization->HandlePhoneEngineMessageL( aMessage,
                    aCallId );
                }
            break;

        case MEngineMonitor::EPEMessageActivateWarrantyMode:
            // Stop playing DTMF tone
            iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );
            HandleShowLifeTimerL();
            break;

        case MEngineMonitor::EPEMessageIssuingUSSDRequest:
            {
             // Enable global notes
            TPhoneCmdParamBoolean globalNotifierParam;
            globalNotifierParam.SetBoolean( EFalse );
            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewSetGlobalNotifiersDisabled,
                &globalNotifierParam );

            if ( !IsOnScreenDialerSupported() && IsNumberEntryUsedL() )
                {
                // Remove number entry from screen
                iViewCommandHandle->ExecuteCommandL(
                    EPhoneViewRemoveNumberEntry );
                }
            else if ( IsOnScreenDialerSupported() )
                {
                NumberEntryClearL();
                }
            }
            break;

        case MEngineMonitor::EPEMessageIssuedUSSDRequest:
            // Go to current state implementation
            iCbaManager->UpdateInCallCbaL();
            break;

        // *#9990#
        case MEngineMonitor::EPEMessageShowBTLoopback:
            // Stop playing DTMF tone
            iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );
            accessoryBtHandler = CPhoneAccessoryBTHandler::NewLC(
                iViewCommandHandle, iStateMachine, this );
            accessoryBtHandler->ShowBTLoopbackL();
            CleanupStack::PopAndDestroy( accessoryBtHandler );
            break;

        // *#2820#
        case MEngineMonitor::EPEMessageShowBTDeviceAddress:
            // Stop playing DTMF tone
            iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );
            accessoryBtHandler = CPhoneAccessoryBTHandler::NewLC(
                iViewCommandHandle, iStateMachine, this );
            accessoryBtHandler->ShowBTAddressL();
            CleanupStack::PopAndDestroy( accessoryBtHandler );
            break;

        // *#7370#
        case MEngineMonitor::EPEMessageActivateRfsDeep:
            // Stop playing DTMF tone
            iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );
            // Launch RFS
            iViewCommandHandle->ExecuteCommandL( EPhoneViewLaunchRfsDeep );
            if ( !IsOnScreenDialerSupported() )
                {
                // Do state-specific operation when number entry is cleared
                HandleNumberEntryClearedL();
                }
            break;

        // *#7780#
        case MEngineMonitor::EPEMessageActivateRfsNormal:
            // Stop playing DTMF tone
            iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );
            // Launch RFS
            iViewCommandHandle->ExecuteCommandL( EPhoneViewLaunchRfsNormal );
            if ( !IsOnScreenDialerSupported() )
                {
                // Do state-specific operation when number entry is cleared
                HandleNumberEntryClearedL();
                }
            break;
        // *#62209526#
        case MEngineMonitor::EPEMessageShowWlanMacAddress:
            // Stop playing DTMF tone
            iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );
            ShowWlanMacAddressL();
            break;

        case MEngineMonitor::EPEMessageThumbnailLoadingCompleted:
            // Update call buble
            UpdateRemoteInfoDataL( aCallId );
            break;

        case MEngineMonitor::EPEMessageRemotePartyInfoChanged:
            UpdateRemoteInfoDataL ( aCallId );
            break;

        case MEngineMonitor::EPEMessageSIMStateChanged:
            __PHONELOG1( EBasic, EPhoneControl, "CPhoneState::HandlePhoneEngineMessageL - simStateChanged =%d", SimState()  );
            HandleSimStateChangedL();
            break;

        case MEngineMonitor::EPEMessageShowIncCallGroupIndex:
            HandleCugInUseNoteL();
            break;

        // *#2873#
        case MEngineMonitor::EPEMessageBTDebugMode:
            accessoryBtHandler = CPhoneAccessoryBTHandler::NewLC(
                iViewCommandHandle, iStateMachine, this );
            accessoryBtHandler->SetBTDebugModeL();
            CleanupStack::PopAndDestroy( accessoryBtHandler );
            break;

        default:

            TBool handled( EFalse );

            if ( iCustomization )
                {
                handled = iCustomization->HandlePhoneEngineMessageL(
                                aMessage, aCallId );
                }

            if ( EFalse == handled )
                {
            	MPhoneMediatorMessage* mediatorMessage = CPhoneMediatorFactory::Instance()->MediatorMessage( aMessage, aCallId );
            	if( mediatorMessage )
    	        	{
    	        	CleanupDeletePushL( mediatorMessage );
	        		mediatorMessage->ExecuteL();
	        		CleanupStack::PopAndDestroy( mediatorMessage );
	        		mediatorMessage = NULL;
    	        	}
                }
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneState::HandleAudioMuteChangedL
// Default handling for EPEMessageAudioMuteChanged message
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleAudioMuteChangedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleAudioMuteChangedL() ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );

    TPhoneCmdParamBoolean booleanParam;
    const TBool audioMute = iStateMachine->PhoneEngineInfo()->AudioMute();
    booleanParam.SetBoolean( audioMute );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewActivateMuteUIChanges,
        &booleanParam );
    }

// -----------------------------------------------------------
// CPhoneState::HandleAudioOutputChangedL
// Default handling for EPEMessageAudioOutputChanged message
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleAudioOutputChangedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleAudioOutputChangedL() ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );

    TPhoneCmdParamAudioOutput outputParam;

    // Output
    const TPEAudioOutput audioOutput =
        iStateMachine->PhoneEngineInfo()->AudioOutput();
    outputParam.SetAudioOutput( audioOutput );

    // view update
    iViewCommandHandle->ExecuteCommandL( EPhoneViewActivateAudioPathUIChanges,
        &outputParam );

    // Previous Output
    TPEPhoneAudioRouteParameters RouteParameters = iStateMachine->
        PhoneEngineInfo()->RouteParameters();
    const TPEAudioOutput previousOutput =
        RouteParameters.iPreviousOutput;

    // BT availability
    TBool btAvailable = iStateMachine->PhoneEngineInfo()->AudioOutputAvailable(
            EPEBTAudioAccessory );

    // Show note or BT disconnect handler
    if ( audioOutput != EPENotActive &&
         previousOutput == EPEBTAudioAccessory &&
         !btAvailable )
        {
        CPhoneBtaaDisconnectHandler::InstanceL()->HandleConnectionLostL();
        }
    else if ( audioOutput == EPEBTAudioAccessory && 
    		previousOutput > EPENotActive && 
    		previousOutput != EPEBTAudioAccessory &&
            btAvailable )
        {
        CPhoneAccessoryBTHandler* bt = CPhoneAccessoryBTHandler::NewLC(
            iViewCommandHandle, iStateMachine, this );
        bt->ShowBTActivatedL();
        CleanupStack::PopAndDestroy( bt );
        }
    else if ( audioOutput == EPENotActive &&
              CPhoneBtaaDisconnectHandler::InstanceL()->IsQuery() )
        {
        CPhoneBtaaDisconnectHandler::InstanceL()->Cancel();
        }
    else if ( RouteParameters.iShowNote && audioOutput == EPELoudspeaker )
        {
        CAknKeySoundSystem* keySounds =
               static_cast<CAknAppUi*>( iEnv.EikAppUi() )->KeySounds();
        keySounds->PlaySound( EAvkonSIDIHFActive );
        }
    }

// -----------------------------------------------------------
// CPhoneState::HandleSimStateChangedL
// Handles changes in the SIM card state.
// -----------------------------------------------------------
//
void CPhoneState::HandleSimStateChangedL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::HandleSimStateChangedL()" );
    CPhoneMainResourceResolver& resolver = *CPhoneMainResourceResolver::Instance();
    TPESimState simState = SimState();

    __PHONELOG2( EBasic, EPhoneControl, "SIM state was changed from %d to %d", iPreviousSimState, simState );

    switch ( simState )
        {
        case EPESimUsable: // Falls through.
        case EPESimReadable: // Falls through.
        case EPESimNotReady:
            {
            if ( iPreviousSimState == EPESimNotPresent )
                {
                __PHONELOG( EBasic, EPhoneControl, "SIM card was inserted, rebooting the phone" );
                RStarterSession starterSession;
                User::LeaveIfError( starterSession.Connect() );
                CleanupClosePushL( starterSession );

                HBufC* queryText = StringLoader::LoadLC( resolver.ResolveResourceID( EPhoneRebootRequired ) );

                TPhoneCmdParamQuery queryParams;
                queryParams.SetCommandParamId( TPhoneCommandParam::EPhoneParamRebootQuery );
                queryParams.SetQueryPrompt( *queryText );
                queryParams.SetDefaultCba( R_AVKON_SOFTKEYS_OK_EMPTY );
                iViewCommandHandle->ExecuteCommandL( EPhoneViewShowQuery, &queryParams );

                starterSession.Reset( RStarterSession::ESIMStatusChangeReset );
                CleanupStack::PopAndDestroy( 2, &starterSession ); // queryText
                }
            break;
            }
        case EPESimNotPresent:
            {
            if ( iPreviousSimState == EPESimUsable ||
                 iPreviousSimState == EPESimReadable ||
                 iPreviousSimState == EPESimNotReady )
                {
                __PHONELOG( EBasic, EPhoneControl, "SIM card was removed" );

                TPhoneCmdParamGlobalNote globalNoteParam;
                globalNoteParam.SetType( EAknGlobalInformationNote );
                globalNoteParam.SetTone( EAvkonSIDNoSound );

                globalNoteParam.SetTextResourceId(
                    CPhoneMainResourceResolver::Instance()->
                    ResolveResourceID( EPhoneSimRemoved ) );

                iViewCommandHandle->ExecuteCommandL(
                    EPhoneViewShowGlobalNote, &globalNoteParam );

                }
            // Show security note, if SIM not present and KFeatureIdFfSimlessOfflineSupport is disabled.
            else if ( !FeatureManager::FeatureSupported( KFeatureIdFfSimlessOfflineSupport ) &&
                    ( iPreviousSimState != EPESimNotSupported ) )
                {
                StartShowSecurityNoteL();
                }
            break;
            }
        default:
            break;
        }

    iPreviousSimState = simState;
    }

// -----------------------------------------------------------------------------
// CPhoneState::IsValidAlphaNumericKey
// Checks is the key event a number, a special character
// or if VoIP is enabled some other character
// -----------------------------------------------------------------------------
//
TBool CPhoneState::IsValidAlphaNumericKey( const TKeyEvent& aKeyEvent,
        TEventCode aEventCode )
    {
    TBool ret(EFalse);

    const TBool numericKeyEntered( CPhoneKeys::IsNumericKey(
          aKeyEvent, aEventCode ) );

    // a numeric key (1,2,3,4,6,7,8,9,0,+,*,p,w )
    // or
    // a letter from fullscreen qwerty, miniqwerty or handwriting
    // when voip is enabled
    if ( numericKeyEntered
        || IsAlphanumericSupportedAndCharInput( aKeyEvent ) )
        {
        ret= ETrue;
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// CPhoneState::CustomizeCBAForPhoneNumber
//
// -----------------------------------------------------------------------------
//
void CPhoneState::CustomizeCbaForPhoneNumberL()
    {
    if ( !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) &&
         iCustomization &&
         iCustomization->AllowAlphaNumericMode() )
        {
        // Get the number entry contents
        HBufC* phoneNumber = PhoneNumberFromEntryLC();

        TPhoneCmdParamInteger integerParam;

        //only charaters from set { 0, .., 9, *, #, +, p, w, P, W } used
        if ( CPhoneKeys::Validate( phoneNumber->Des()) )
            {
            integerParam.SetInteger( CPhoneMainResourceResolver::Instance()->
                                    ResolveResourceID( EPhoneNumberAcqCBA ) );
            }
        //other characters
        else
            {
            integerParam.SetInteger( iCustomization->CustomizeSoftKeys() );
            }

        iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateCba, &integerParam );
        CleanupStack::PopAndDestroy( phoneNumber );
        }

    }

// -----------------------------------------------------------------------------
// CPhoneState::IsTouchDTmfDialerOn
// -----------------------------------------------------------------------------
//
TBool CPhoneState::IsTouchDTmfDialerOn() const
    {
    TBool status( EFalse );
    if ( IsOnScreenDialerSupported() )
        {
        TBool isDialerVisible( EFalse );
        TRAP_IGNORE( isDialerVisible = IsDTMFEditorVisibleL() );

        if ( isDialerVisible )
            {
            status = ETrue;
            }
        }
    return status;
    }
// -----------------------------------------------------------------------------
// CPhoneState::SendDtmfKeyEventL
// send dtmf event when,
// -touch dtmf dialer is NOT visible
// -query is not active
// -menubar is not open
// -----------------------------------------------------------------------------
//
void CPhoneState::SendDtmfKeyEventL( const TKeyEvent& aKeyEvent,
               TEventCode aEventCode  )
    {

    if ( !IsTouchDTmfDialerOn()
        && !IsAnyQueryActiveL()
        && !IsMenuBarVisibleL() )
        {
        if( IsKeyEventFurtherProcessedL( aKeyEvent ))
            {
            // Send the key event to the phone engine.
            SendKeyEventL( aKeyEvent, aEventCode );
            }
        }

    }

// -----------------------------------------------------------------------------
// CPhoneState::IsKeyEventFurtherProcessed
// -----------------------------------------------------------------------------
//
TBool CPhoneState::IsKeyEventFurtherProcessedL( const TKeyEvent& aKeyEvent ) const
    {
    // While being in some special keyboard mode (Full Screen&mini QWERTY,
    // handwriting mode) FEP sends only EEventKey -event and thus manual
    // DTMFs can not be played with the user specified tone lengths. Also,
    // in general, DTMFs are tried to play only in numeric mode.
    TBool numericMode = iViewCommandHandle->HandleCommandL(
      EPhoneViewIsNumberEntryNumericMode ) == EPhoneViewResponseSuccess;

    // Key presses simulated by dialer are played even if in alphanumeric mode.
    const TBool simulatedByDialer = 
        ( ( aKeyEvent.iModifiers & ( EModifierNumLock | EModifierKeypad ) ) 
                == ( EModifierNumLock | EModifierKeypad ) );


    if ( ( ( aKeyEvent.iModifiers & EModifierSpecial ) != 0 || !numericMode ) && !simulatedByDialer )
        {
        return EFalse;
        }

    return ETrue;
    }

// -----------------------------------------------------------
// CPhoneState::HandleErrorL
// Default handling for HandleError message
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleErrorL( const TPEErrorInfo& aErrorInfo )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleErrorL()");

    __PHONELOG1( EBasic, EPhoneControl,
            "PhoneUIControl: CPhoneState::HandleErrorL - aErrorInfo.iErrorCode =%d ",
            aErrorInfo.iErrorCode );

    __PHONELOG1( EBasic, EPhoneControl,
        "PhoneUIControl: CPhoneState::HandleErrorL - aErrorInfo.iCallId =%d ",
        aErrorInfo.iCallId );


    // Sets touch buttons to correct status if error has occured.
    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        iViewCommandHandle->ExecuteCommand( EPhoneResetTouchButtons );
        }

    // Do the common error handling (display proper notes etc)
    CPhoneMainErrorMessagesHandler::Instance()->ShowErrorSpecificNoteL( aErrorInfo );

    switch( aErrorInfo.iErrorCode )
        {
        case ECCPErrorCCUserAlertingNoAnswer:
        case ECCPErrorCCResourceNotAvailable:
            {
            if( aErrorInfo.iCallId > KErrNotFound )
                {
                if ( iStateMachine->PhoneEngineInfo()->CallDirection(
                        aErrorInfo.iCallId ) != RMobileCall::EMobileTerminated )
                    {
                    if( IsVideoCall( aErrorInfo.iCallId ) )
                        {
                        // Active MO video call lost 3G network.
                        __PHONELOG1( EBasic, EPhoneControl,
                        "PhoneUIControl: CPhoneState::HandleErrorL - ShowReconnectQueryL vid 1, callid%d ",
                        aErrorInfo.iCallId );
                        CPhoneReconnectQuery::InstanceL()->ShowReconnectQueryL( ETrue );
                        }
                    else
                        {
                         __PHONELOG1( EBasic, EPhoneControl,
                            "PhoneUIControl: CPhoneState::HandleErrorL - No video call =%d ",
                            aErrorInfo.iCallId );
                        }
                    }
                }
            }
            break;

            case ECCPErrorCCServiceNotAvailable:
                {
                if( IsVideoCall( aErrorInfo.iCallId ) )
    				{
    			    CPhoneReconnectQuery::InstanceL()->ShowReconnectQueryL( EFalse );
    				}
                }
                break;

            case ECCPErrorBadRequest:
                {
                TPECallType callType =
                    iStateMachine->PhoneEngineInfo()->CallTypeCommand();

                if( callType == EPECallTypeVideo )
                    {
                        // Dialling MO video call cannot reach 3G network.
                        __PHONELOG1( EBasic, EPhoneControl,
                            "PhoneUIControl: CPhoneState::HandleErrorL - ShowReconnectQueryL vid 2, callid%d ",
                            aErrorInfo.iCallId );
                        CPhoneReconnectQuery::InstanceL()->ShowReconnectQueryL( ETrue );
                    }
                }
                break;

        case ECCPErrorVideoCallNotSupportedByNetwork:
        case ECCPErrorVideoCallSetupFailed:
        case ECCPErrorNotReached:
           // If call id found and seems to be Video Call
            if ( IsVideoCall( aErrorInfo.iCallId ) )
                {
                // Get active call count
                TPhoneCmdParamInteger activeCallCount;
                iViewCommandHandle->ExecuteCommandL(
                    EPhoneViewGetCountOfActiveCalls, &activeCallCount );

                if ( activeCallCount.Integer() == 0
                     || iStateMachine->PhoneEngineInfo()->CallDirection(
                        aErrorInfo.iCallId ) != RMobileCall::EMobileTerminated )
                    {
                    // Dialling MO video call attempted in 2G network or
                    // dialing MO video to unvalid number
                    // Reconnect query include video label if errorcode is unvalid number.
                    __PHONELOG1( EBasic, EPhoneControl,
                    "PhoneUIControl: CPhoneState::HandleErrorL - ShowReconnectQueryL vid 3, callid%d ",
                    aErrorInfo.iCallId );
                    CPhoneReconnectQuery::InstanceL()->ShowReconnectQueryL(
                              ECCPErrorNotReached == aErrorInfo.iErrorCode );
                    }
                }
            break;

        case ECCPErrorNoService:
            // No network -> hide volume popup
            iViewCommandHandle->ExecuteCommandL( EPhoneViewHideNaviPaneAudioVolume );
            break;

        case ECCPErrorSatControl:
            {
            // check, that there really was a call established before completing SAT request
            if( aErrorInfo.iCallId != KPECallIdNotUsed )
                {                
                // Complete sat request
                if ( iStateMachine->PhoneEngineInfo()->CallOrigin( aErrorInfo.iCallId ) == EPECallOriginSAT )
                    {
                    CompleteSatRequestL( aErrorInfo.iCallId );
                    }
                }

            // remove number entry
            if ( !IsOnScreenDialerSupported() && IsNumberEntryUsedL() )
                {
                iViewCommandHandle->ExecuteCommandL(
                    EPhoneViewRemoveNumberEntry );
                }
            else if ( IsOnScreenDialerSupported() )
                {
                NumberEntryClearL();
                }
            }
            break;

        default:
            break;
        }
    // clear call blacklist if call failure occurs
    CPhoneClearBlacklist::Instance()->ClearBlackListOnNextKey();
    }

// -----------------------------------------------------------
// CPhoneState::HandleChangedCallDurationL
// Default handling for EPEMessageChangedCallDuration message
// (other items were commented in a header).
// -----------------------------------------------------------
void CPhoneState::HandleChangedCallDurationL( TInt aCallId )
    {
     __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleChangedCallDurationL() ");
    // Get the call duration
    TTime time( 0 );
    TTimeIntervalSeconds timeInterval =
        iStateMachine->PhoneEngineInfo()->CallDuration( aCallId );
    time += timeInterval;

    // Read format string from AVKON resource
    TBuf<KPhoneMaxCharsInNote> timeDurationFormatString( KNullDesC );
    LoadResource( timeDurationFormatString, EPhoneCallDurationFormatString );

    // Format the time
    TBuf<KPhoneMaxTimeDisplayTextLength> timeString( KNullDesC );
    time.FormatL( timeString, timeDurationFormatString );

    // Localize time format
    AknTextUtils::LanguageSpecificNumberConversion( timeString );

    // update the call header call duration
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewUpdateCallHeaderCallDuration,
        aCallId,
        timeString );
    }

// -----------------------------------------------------------------------------
// CPhoneState::HandleRemoteBusyL
// -----------------------------------------------------------------------------
//
void CPhoneState::HandleRemoteBusyL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleRemoteBusyL( ) ");
    ShowNumberBusyNoteL();

    const TPECallType callType =
        iStateMachine->PhoneEngineInfo()->CallTypeCommand();

    if( callType == EPECallTypeVideo )
        {
         // Launch reconnect query including video call menu item
         __PHONELOG1( EBasic, EPhoneControl,
            "PhoneUIControl: CPhoneState::HandleRemoteBusyL - ShowReconnectQueryL vid 5, callid%d ",
                aCallId );
        CPhoneReconnectQuery::InstanceL()->ShowReconnectQueryL( ETrue );
        }
    else
        {        
        // Complete sat request
        if ( iStateMachine->PhoneEngineInfo()->CallOrigin( aCallId ) == EPECallOriginSAT )
            {
            CompleteSatRequestL( aCallId );
            }
        }
    }

// -----------------------------------------------------------
// CPhoneState::HandleDisconnectingL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleDisconnectingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleDisconnectingL( ) ");
    // If there is no call in connected state then EPhoneDtmfTextQuery and EPhoneDtmfListQueryDialog
    // must be dismmissed from UI, therefore we must set EPhoneViewSetNoConnectedCalls to ETrue this
    // way we can ensure that CPhoneQueryController doesnt relaunch EPhoneDtmfListQueryDialog.
    if ( !IsAnyConnectedCalls() )
        {
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean(ETrue);
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetNoConnectedCalls, &booleanParam );
        }

    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateDisconnecting );

    TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
    LoadResource( labelText, EPhoneInCallDisconnected );

    callHeaderParam.SetLabelText( labelText );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewStopRingTone );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );

    CPhoneClearBlacklist::Instance()->ClearBlackListOnNextKey();
    }

// -----------------------------------------------------------
// CPhoneState::IsCustomizedDialerVisibleL()
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsCustomizedDialerVisibleL() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsCustomizedDialerVisibleL( ) ");
    return iViewCommandHandle->HandleCommandL(
        EPhoneIsCustomizedDialerVisible ) == EPhoneViewResponseSuccess;
    }

// <------------------------------- KEY EVENTS ------------------------------->

// -----------------------------------------------------------
// CPhoneState::HandleKeyMessageL( aMessage, aKeyCode )
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleKeyMessageL(
    TPhoneKeyEventMessages /*aMessage*/,
    TKeyCode /*aKeyCode*/ )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleKeyMessageL( ) ");
    }

// -----------------------------------------------------------
// CPhoneState::HandleKeyEventL( aKeyEvent, aEventCode )
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleKeyEventL( ) ");
    if ( ( IsNumberEntryVisibleL() ) || OnlySideVolumeKeySupported() )
        {
        // Handle numeric keys when key events are received in single state
        HandleNumericKeyEventL( aKeyEvent, aEventCode );
        }
    else if ( aKeyEvent.iScanCode == EStdKeyUpArrow &&
              aEventCode == EEventKey )
        {
        // Increace audio volume
        IncreaseAudioVolumeL();
        }
    else if ( aKeyEvent.iScanCode == EStdKeyDownArrow &&
            aEventCode == EEventKey )
        {
        // Decreace audio volume
        DecreaseAudioVolumeL();
        }
    else
        {
        // Handle numeric keys when key events are received in single state
        HandleNumericKeyEventL( aKeyEvent, aEventCode );
        }
    }

// -----------------------------------------------------------
// CPhoneState::OnlySideVolumeKeySupported
// -----------------------------------------------------------
//
TBool CPhoneState::OnlySideVolumeKeySupported()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::OnlySideVolumeKeySupported( ) ");
    TBool onlySideVolumeKeySupported(EFalse);
    if ( !CPhoneCenRepProxy::Instance()->
            IsTelephonyFeatureSupported( KTelephonyLVFlagScrollVolumeKeys ) &&
         FeatureManager::FeatureSupported( KFeatureIdSideVolumeKeys ) )
        {
        onlySideVolumeKeySupported = ETrue;
        }
    __PHONELOG1( EBasic, EPhoneControl,
            "OnlySideVolumeKeySupported: %d",
            onlySideVolumeKeySupported );
    return onlySideVolumeKeySupported;
    }

// -----------------------------------------------------------
// CPhoneState::HandleKeyPressDurationL( aScanCode, aKeyPressDuration )
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleKeyPressDurationL(
    TKeyCode aCode,
    TTimeIntervalMicroSeconds /*aKeyPressDuration*/ )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleKeyPressDurationL( ) ");

    if( aCode == KPhoneDtmfHashCharacter )
        {
        if( iAlsLineChangeKeyPressTimer )
            {
            if( iAlsLineChangeKeyPressTimer->IsActive() )
                {
                iAlsLineChangeKeyPressTimer->Cancel();
                }
            }
        }
    }

// -----------------------------------------------------------
// CPhoneState::IsNoteVisibleL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsNoteVisibleL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsNoteVisibleL( ) ");
    TPhoneCmdParamBoolean booleanParam;
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewGetIsNoteVisible, &booleanParam );
    return booleanParam.Boolean();
    }

// -----------------------------------------------------------
// CPhoneState::IsMenuBarVisibleL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsMenuBarVisibleL() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsMenuBarVisibleL( ) ");
    return iViewCommandHandle->HandleCommandL(
        EPhoneViewIsMenuBarVisible ) ==
        EPhoneViewResponseSuccess;
    }

// -----------------------------------------------------------
// CPhoneState::HandleNumericKeyEventL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleNumericKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleNumericKeyEventL( ) ");

    TBool numberEntryUsed = IsNumberEntryUsedL();

    if ( numberEntryUsed && ( aKeyEvent.iRepeats == 0 ||
              aKeyEvent.iScanCode == EStdKeyBackspace ||
              aKeyEvent.iScanCode ==EStdKeyLeftArrow  ||              
              aKeyEvent.iScanCode == EStdKeyUpArrow  ||
              aKeyEvent.iScanCode == EStdKeyDownArrow  ||              
              aKeyEvent.iScanCode ==EStdKeyRightArrow ))
        {
        // Number entry exists but may be hidden
        KeyEventForExistingNumberEntryL( aKeyEvent, aEventCode );
        }

    }

// -----------------------------------------------------------------------------
// CPhoneState::IsAnyQueryActiveL
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsAnyQueryActiveL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsAnyQueryActiveL( ) ");
    TBool isActive( EFalse );

    // If IsNoteDismissableL returns true then shown note is dismissable by key event
    // and then there is no actual query and IsAnyQueryActiveL returns false.
    if ( !IsNoteDismissableL() )
        {
        // Check is query displayed
        TPhoneCmdParamBoolean isQueryDisplayed;
        iViewCommandHandle->ExecuteCommandL( EPhoneViewIsQuery, &isQueryDisplayed );
        // Check is Blocking Dialog active ( e.g. RFS query )
        TPhoneCmdParamBoolean blockingDialogStatus;
        iViewCommandHandle->ExecuteCommandL( EPhoneViewGetBlockingDialogStatus,
            &blockingDialogStatus );
        // Check is note displayed
        TPhoneCmdParamBoolean isNoteActive;
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewIsNoteActive, &isNoteActive );

        if ( isQueryDisplayed.Boolean() || blockingDialogStatus.Boolean() || isNoteActive.Boolean() )
            {
            isActive = ETrue;
            }
        }
    __PHONELOG1( EBasic, EPhoneControl, "CPhoneState::IsAnyQueryActiveL: %d ", isActive );
    return isActive;
    }

// -----------------------------------------------------------------------------
// CPhoneState::IsNoteDismissable
// -----------------------------------------------------------------------------
//
TBool CPhoneState::IsNoteDismissableL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsNoteDismissableL( ) ");
    TBool dismiss = EFalse;
    if ( !FeatureManager::FeatureSupported( KFeatureIdFfSimlessOfflineSupport ) ||
         (FeatureManager::FeatureSupported( KFeatureIdFfSimlessOfflineSupport ) &&
                  SimSecurityStatus() == ESimRejected )  )
        {
        // Check if note is dismissed by key event.
        TPhoneCmdParamBoolean isNoteDismissed;
        iViewCommandHandle->ExecuteCommandL(
                EPhoneViewIsActiveNoteDissmissableByKeyEvent, &isNoteDismissed );
        if ( isNoteDismissed.Boolean() )
            {
            dismiss = ETrue;
            }
        }
    __PHONELOG1( EBasic, EPhoneControl, "CPhoneState::IsNoteDismissableL: %d ", dismiss );
    return dismiss;
    }

// -----------------------------------------------------------
// CPhoneState::IsDialingExtensionInFocusL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsDialingExtensionInFocusL() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsDialingExtensionInFocusL( ) ");
    return iViewCommandHandle->HandleCommandL(
        EPhoneViewGetEasyDialingInFocusStatus ) ==
        EPhoneViewResponseSuccess;
    }

// -----------------------------------------------------------
// CPhoneState::SendKeyEventL
// -----------------------------------------------------------
//
void CPhoneState::SendKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SendKeyEventL( ) ");
    switch( aEventCode )
        {
        // EEventKey
        case EEventKey:
            // Send the key press to the phone engine, if applicable
            SendKeyPressL( aKeyEvent, aEventCode );
            break;

        // EEventKeyUp
        case EEventKeyUp:
            // Send a key up event for the last key code sent to
            // the phone engine
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageEndDTMF );
            break;

        default:
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneState::SendKeyPressL
// -----------------------------------------------------------
//
void CPhoneState::SendKeyPressL(
    const TKeyEvent& aKeyEvent,
    TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SendKeyPressL( ) ");
    const TBool dtmfToneKeyEntered =
        CPhoneKeys::IsDtmfTone( aKeyEvent, aEventCode );

    if ( dtmfToneKeyEntered ||
        aKeyEvent.iCode == EKeyBackspace )
        {
        // Get the number entry contents, if it exists
        if ( IsNumberEntryUsedL() )
            {
            HBufC* phoneNumber = PhoneNumberFromEntryLC();
            // Save the phone number
            __PHONELOG1( EBasic, EPhoneControl, "SetPhoneNumber: %S ", &phoneNumber->Des() );
            iStateMachine->PhoneEngineInfo()->SetPhoneNumber( *phoneNumber );

            // Convert key code to western.
            TBuf<1> buffer; // one character
            buffer.Append( aKeyEvent.iCode );
            __PHONELOG1( EBasic, EPhoneControl,
                "CPhoneState::SendKeyPressL(%S)",
                &buffer );
            AknTextUtils::ConvertDigitsTo( buffer, EDigitTypeWestern );
            __PHONELOG1( EBasic, EPhoneControl,
                "CPhoneState::SendKeyPressL(%S)",
                &buffer );
            TLex code( buffer );

            // Save the key code
            iStateMachine->PhoneEngineInfo()->SetKeyCode( code.Peek() );

            // Plays a DTMF tone if active call
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessagePlayDTMF );
            // remove the phone number from the cleanup stack
            CleanupStack::PopAndDestroy( phoneNumber );
            }
        }
    }

// <------------------------------ SYSTEM EVENTS ----------------------------->

// -----------------------------------------------------------
// CPhoneState::DynInitMenuPaneL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DynInitMenuPaneL(
    TInt aResourceId,
    CEikMenuPane* aMenuPane )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DynInitMenuPaneL( ) ");
    __ASSERT_DEBUG( aMenuPane && aResourceId,
        Panic( EPhoneCtrlParameterNotInitialized ) );

    // Save the number of digits in the number entry before processing
    // the menu pane
    if ( IsNumberEntryUsedL() )
        {
        TPhoneCmdParamBoolean serviceCodeParam;
            serviceCodeParam.SetBoolean(
            iStateMachine->PhoneEngineInfo()->PhoneNumberIsServiceCode() );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetServiceCodeFlag,
            &serviceCodeParam );
        }

    // Process the menu pane
    TPhoneCmdParamDynMenu dynMenuPane;
    dynMenuPane.SetResourceId( aResourceId );
    dynMenuPane.SetDynMenu( aMenuPane );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuPane, &dynMenuPane );

    if ( iCustomization )
        {
        // Menu pane is customized after default items are decided
        iCustomization->CustomizeMenuPaneL(aResourceId, aMenuPane);
        }
    }

// -----------------------------------------------------------
// CPhoneState::DynInitMenuBarL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DynInitMenuBarL(
    TInt aResourceId,
    CEikMenuBar* aMenuBar )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DynInitMenuBarL( ) ");
    __ASSERT_DEBUG( aMenuBar && aResourceId,
        Panic( EPhoneCtrlParameterNotInitialized ) );

    TPhoneCmdParamDynMenu dynMenuBar;
    dynMenuBar.SetResourceId( aResourceId );
    dynMenuBar.SetDynMenu( aMenuBar );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBar, &dynMenuBar );

    if ( iCustomization )
        {
        // Menu bar is customized after default items are decided
        iCustomization->CustomizeMenuBarL(aResourceId, aMenuBar);
        }

    }

// -----------------------------------------------------------
// CPhoneState::HandleSystemEventL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleSystemEventL( const TWsEvent& /*aEvent*/ )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleSystemEventL( ) ");
    }

// -----------------------------------------------------------
// CPhoneState::HandleForegroundEventL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleForegroundEventL( TBool aForeground )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleForegroundEventL( ) ");
    if ( !aForeground && IsNumberEntryUsedL() )
        {
        // Going out from foreground and number entry is open.
        // Make sure that dtmf playing is stopped ( EEventKeyUp may be missed ).
        iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );
        }
    }

// -----------------------------------------------------------
// CPhoneState::HandlePhoneForegroundEventL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandlePhoneForegroundEventL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandlePhoneForegroundEventL( ) ");

    // Force Idle to the foreground if a foreground event is received,
    // but Idle is the top application

    const TBool idleIsTopApp = iViewCommandHandle->HandleCommandL(
        EPhoneViewIsIdleTopApp ) ==
        EPhoneViewResponseSuccess ;

    if ( !IsOnScreenDialerSupported() && idleIsTopApp )
        {
        TBool activatePhone = CPhonePubSubProxy::Instance()->Value(
            KPSUidAiInformation, KActiveIdleState ) == EPSAiNumberEntry;
        if ( activatePhone )
            {
            // Set Phone as the top application
            TPhoneCmdParamInteger uidParam;
            uidParam.SetInteger( KUidPhoneApplication.iUid );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
                &uidParam );
            NumberEntryManagerL()->CreateNumberEntryL();
            SetNumberEntryVisibilityL(ETrue);
            }

        else
            {
            if ( !iStateMachine->SecurityMode()->IsSecurityMode() )
                {
                __PHONELOG( EBasic, EPhoneControl,
                  "CPhoneState::HandlePhoneForegroundEventL - Force Idle to the foreground" );
                // Bring Idle app to the foreground
                iViewCommandHandle->ExecuteCommandL( EPhoneViewBringIdleToForeground );
                }
            }
        }

     }

// -----------------------------------------------------------
// CPhoneState::HandlePhoneFocusLostEventL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandlePhoneFocusLostEventL()
    {
    // Notify that this method is called always when Idle is brought to foreground
    // See implementation in CPhoneAppUI::HandleWsEventL

    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandlePhoneFocusLostEventL( ) ");
    // Phone app focus lost -> close Dtmf dialer when visible
    if ( IsOnScreenDialerSupported() && IsDTMFEditorVisibleL() )
        {
        CloseDTMFEditorL();
        }
    else if ( IsOnScreenDialerSupported() && IsCustomizedDialerVisibleL() )
        {
        CloseCustomizedDialerL();
        }
    }
// ---------------------------------------------------------
// CPhoneState::HandleIdleForegroundEventL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleIdleForegroundEventL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleIdleForegroundEventL( ) ");
    // Empty implementation
    }

// -----------------------------------------------------------
// CPhoneState::HandleEnvironmentChangeL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleEnvironmentChangeL( const TInt aChanges )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleEnvironmentChangeL( ) ");
    __PHONELOG1( EBasic, EPhoneControl, "CPhoneState::HandleEnvironmentChangeL - Changes:  %d ", aChanges );
    // Update the profile display if required
    if ( aChanges &
        ( EChangesLocale | EChangesMidnightCrossover | EChangesSystemTime ) )
        {
        UpdateProfileDisplayL();
        }
    }

// -----------------------------------------------------------
// CPhoneState::HandlePhoneStartupL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandlePhoneStartupL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandlePhoneStartupL( ) ");
    // Empty implementation
    }

// -----------------------------------------------------------
// CPhoneState::HandlePropertyChangedL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandlePropertyChangedL(
    const TUid& aCategory,
    const TUint aKey,
    const TInt aValue )
    {

    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandlePropertyChangedL( ) ");

    __PHONELOG1( EBasic, EPhoneControl,
            "CPhoneState::HandlePropertyChangedL - aCategory= %d", aCategory  );
    __PHONELOG1( EBasic, EPhoneControl,
            "CPhoneState::HandlePropertyChangedL - aKey= %d", aKey  );
    __PHONELOG1( EBasic, EPhoneControl,
            "CPhoneState::HandlePropertyChangedL - aValue= %d", aValue  );
    if ( aCategory == KPSUidCtsyCallInformation )
        {
        // Call state event
        if ( aKey == KCTsyCallState )
            {
            // Update the incall indicator
            UpdateIncallIndicatorL( aValue );
            }
        }

    // Telephony information category
    else if ( aCategory == KPSUidTelInformation
              && SimState() == EPESimUsable )
        {
        // Telephony display event
        if ( aKey == KTelDisplayInfo )
            {
            __PHONELOG( EBasic, EPhoneControl, "CPhoneState::HandlePropertyChangedL - telephony display info received" );
            // Update the operator and profile display
            UpdateProfileDisplayL();
            }
        }
    else if ( aCategory == KPSUidStartup && aKey == KStartupSimSecurityStatus )
        {
        // Show security note, SIM is not valid.
        if ( aValue == ESimRejected 
                || aValue == ESimUnaccepted 
                || aValue == ESimInvalid )
            {
            __PHONELOG( EBasic, EPhoneControl, "CPhoneStateStartup::HandlePropertyChangedL - SimSecurity status received" );
            StartShowSecurityNoteL();
            }
        }
    else if ( aCategory == KPSUidHWRM && aKey == KHWRMGripStatus )
        {
        UpdateCbaSwivelStateChangedL();
        }
    }

// -----------------------------------------------------------
// CPhoneState::HandleCenRepChangeL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleCenRepChangeL(
    const TUid& /*aUid*/,
    const TUint /*aId*/ )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleCenRepChangeL( ) ");
    }

// -----------------------------------------------------------
// CPhoneState::NeedToReturnToForegroundAppL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::NeedToReturnToForegroundAppL() const
    {
    return iViewCommandHandle->HandleCommandL(
        EPhoneViewGetNeedToReturnToForegroundAppStatus ) ==
        EPhoneViewResponseSuccess;
    }

// -----------------------------------------------------------
// CPhoneState::TopAppIsDisplayedL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::TopAppIsDisplayedL() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::TopAppIsDisplayedL( ) ");
    return iViewCommandHandle->HandleCommandL(
        EPhoneViewGetTopApplicationIsDisplayedStatus ) ==
        EPhoneViewResponseSuccess;
    }

// -----------------------------------------------------------
// CPhoneState::TitlePaneIsVisibleL
// -----------------------------------------------------------
//
TBool CPhoneState::TitlePaneIsVisibleL() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::TitlePaneIsVisibleL( ) ");
    return iViewCommandHandle->HandleCommandL(
        EPhoneViewGetTitlePaneIsVisibleStatus ) ==
        EPhoneViewResponseSuccess;
    }

// <---------------------------- MENU AND CBA EVENTS ------------------------->

EXPORT_C TBool CPhoneState::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleCommandL( ) ");
    TBool commandStatus = ETrue;
    switch( aCommand )
        {
        case EPhoneEmergencyCmdExit:
            // this should be bypasses?
        case EPhoneDialerCallHandling:
        case EPhoneCmdBack:
            BeginTransEffectLC( ENumberEntryClose );
            // Remove number entry from screen
            iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
            // Do state-specific behaviour if number entry is cleared
            HandleNumberEntryClearedL();
            EndTransEffect();
            break;

        case EPhoneDialerCmdTouchInput:
            OpenVkbL();
            break;

        case EPhoneNumberAcqCmdSendMessage:
            // Open the mms editor
            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewSendMessage );
            if ( !IsOnScreenDialerSupported() )
                {
                // Remove number entry from screen
                iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
                // Do state-specific behaviour if number entry is cleared
                HandleNumberEntryClearedL();
                }
            break;

        case EPhoneNumberAcqCmdSave:
            // Open Create contact
            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewAddContact );
            break;

        case EPhoneNumberAcqCmdAddToName:
            // Open the message editor
            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewUpdateContact );
            break;

        case EPhoneNumberAcqCmdAddToContacts:
            {
            if ( IsOnScreenDialerSupported() )
                {
                TPhoneCmdParamQuery queryDialogParam;
                    queryDialogParam.SetQueryType( EPhoneContactsListQuery );
                    queryDialogParam.SetQueryResourceId(
                    CPhoneMainResourceResolver::Instance()->
                    ResolveResourceID( EPhoneAddtoContactsListQuery )  );

                 // Display dialog
                iViewCommandHandle->ExecuteCommandL( EPhoneViewShowQuery,
                    &queryDialogParam );
                }
            }
            break;

        case EPhoneCmdWaitNoteOk:
            // Remove number entry from screen
            iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );
            if ( !IsOnScreenDialerSupported() )
                {
                HandleNumberEntryClearedL();
                }
            break;

        case EPhoneInCallCmdEndThisOutgoingCall:
            DisconnectCallL();
            break;

        case EPhoneCmdYesAlsLineChange:
            if( IsAlsLineChangePossible() )
                {
                ChangeAlsLineL();
                }
            else
                {
                SendGlobalErrorNoteL( EPhoneLineBlockingNote );
                HandleNumberEntryClearedL(); // Set back CBAs
                }
            break;

        case EPhoneCmdNoAlsLineChange:
            if ( !IsOnScreenDialerSupported() )
                {
                HandleNumberEntryClearedL();
                }
            break;

        case EPhoneCmdYesBtDisconnectQuery:
        case EPhoneCmdNoBtDisconnectQuery:
            CPhoneBtaaDisconnectHandler::InstanceL()->HandleQueryDismissL( aCommand );
            // Make sure that query is removed
            // if there has been also another ( by Cover UI ).
            iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveQuery );
            break;

        case EPhoneInCallCmdDeactivateIhf: // fall through
        case EPhoneInCallCmdActivateIhf:
            SetHandsfreeModeL( aCommand == EPhoneInCallCmdActivateIhf );
             break;

        case EPhoneInCallCmdHandset:    // fall through
        case EPhoneInCallCmdBtHandsfree:
            CPhoneState::SetBTHandsfreeModeL(
                 aCommand == EPhoneInCallCmdBtHandsfree );
            break;

        case EPhoneInCallCmdActivatEPhonebook:
            {
            // Launch Phonebook application
            TPhoneCmdParamAppInfo appInfoParam;
            appInfoParam.SetAppUid( KPhoneUidAppPhonebook );
            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewActivateApp, &appInfoParam );
            }
            break;

        case EPhoneDialerCmdContacts:
            {
            // Launch Phonebook application
            TPhoneCmdParamAppInfo appInfoParam;
            appInfoParam.SetAppUid( KPhoneUidAppPhonebook );
            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewActivateApp, &appInfoParam );
            }
            break;

        case EPhoneViewYesSingleItemFetch:
            {
            if ( IsOnScreenDialerSupported() )
                {
                TBuf<KPhoneNumberEntryBufferSize> fetchContent;
                fetchContent = iViewCommandHandle->FetchContent();
                if ( fetchContent.Length() )
                    {
                    iViewCommandHandle->ExecuteCommandL(
                                            EPhoneViewSetNumberEntryContent,
                                            0,
                                            fetchContent );

                    CallFromNumberEntryL();
                    }
                }
            }
            break;
        case EPhoneNumberAcqCmdToggleNeAlphaMode:
        case EPhoneNumberAcqCmdToggleNeNumericMode:
            {
            // Toggle mode
            NumberEntryManagerL()->NumberEntryToggleAlphaNumericModeL();
            }
            break;

        case EPhoneCmdYesVideoFailedNoMemorySwitchToVoice:
            DialVoiceCallL();
            break;

        case EPhoneCmdNoVideoFailedNoMemorySwitchToVoice:
            if ( IsNumberEntryUsedL() )
                {
                // Show the number entry if it exists
                SetNumberEntryVisibilityL(ETrue);
                }
            else if ( NeedToReturnToForegroundAppL() )
                {
                // Continue displaying current app but set up the
                // idle screen in the background
                SetupIdleScreenInBackgroundL();
                }
            else
                {
                 // Display idle screen
                DisplayIdleScreenL();
                }
            break;

        case EPhoneInCallCmdSetVolumeLevel:
            {
            // This command comes from ui control, no need to update
            // value to control (second parameter set false).
            ChangeAudioVolumeL( GetVolumeLevel(), EFalse );
            }
            break;

        case EPhoneDialerCmdHelp:
            if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                TPtrC contextName;
                contextName.Set( KDATA_DIALER_HLP_MAIN() );
                iViewCommandHandle->ExecuteCommandL(
                    EPhoneViewLaunchHelpApplication, 0, contextName );
                }
            break;

        case EPhoneCmdVideoCallOutOfMemory:
            ShowVideoCallOutOfMemoryNoteL();
            DisconnectCallL();
            break;

        case EPhoneCmdUpdateCba:
        case EPhoneDialerValidNumber:
        case EPhoneDialerInvalidNumber:
        case EPhoneCmdBlockingDialogLaunched:
        case EPhoneCmdBlockingDialogClosed:
            {
            // these command ids are sent by easydialing to communicate to phone app
            HandleEasyDialingCommandsL( aCommand );
            }
            break;
                
        default:
            if ( IsOnScreenDialerSupported() )
                {
                // Offer command to view.
                // Easydialing commands are handled in view, too.
                TPhoneViewResponseId resId =
                            iViewCommandHandle->HandleCommandL( aCommand );

                if( resId == EPhoneViewResponseFailed )
                    {
                     commandStatus = EFalse;
                    }
                }
            else
                {
                commandStatus = EFalse;
                }
            break;
        }

    if( !commandStatus && iCustomization )
        {
        commandStatus = iCustomization->HandleCommandL( aCommand );
        }

    return commandStatus;
    }

// -----------------------------------------------------------------------------
// CPhoneState::ProcessCommandL
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneState::ProcessCommandL( TInt /*aCommand*/ )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::ProcessCommandL() ");
    // no implementation.
    return EFalse;
    }

// <-------------------------- REMOTE CONTROL EVENTS ------------------------->

// -----------------------------------------------------------------------------
// CPhoneState::HandleRemConCommandL
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneState::HandleRemConCommandL(
    TRemConCoreApiOperationId aOperationId,
    TRemConCoreApiButtonAction /*aButtonAct*/ )
    {
    TBool handled = EFalse;

    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::MrccatoCommand() ");
    switch ( aOperationId )
        {
        case ERemConCoreApiVolumeUp:
            __PHONELOG( EBasic, EPhoneControl, "CPhoneState::MrccatoCommand ERemConCoreApiVolumeUp" );
            IncreaseAudioVolumeL();
            handled = ETrue;
            break;

        case ERemConCoreApiVolumeDown:
            __PHONELOG( EBasic, EPhoneControl, "CPhoneState::MrccatoCommand ERemConCoreApiVolumeDown" );
            DecreaseAudioVolumeL();
            handled = ETrue;
            break;
        case ERemConCoreApiMute:
            {
            __PHONELOG( EBasic, EPhoneControl, "CPhoneState::MrccatoCommand ERemConCoreApiMute" );
            iStateMachine->PhoneEngineInfo()->AudioMute() ?
                iStateMachine->PhoneEngineInfo()->SetAudioMuteCommand( EFalse ):
                iStateMachine->PhoneEngineInfo()->SetAudioMuteCommand( ETrue );
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageSetAudioMute );
            handled = ETrue;
            break;
            }

         default:
             __PHONELOG1( EBasic, EPhoneControl, "CPhoneState::MrccatoCommand MrccatoCmd.Other(%d)", aOperationId );
            // Other commands ignored.
            break;
        }

    return handled;
    }

// -----------------------------------------------------------
// CPhoneState::DecreaseAudioVolume()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DecreaseAudioVolumeL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DecreaceAudioVolumeL( ) ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );

    TPEAudioOutput output( iStateMachine->PhoneEngineInfo()->AudioOutput() );
     __PHONELOG1( EBasic, EPhoneControl, "CPhoneState::DecreaseAudioVolumeL - audio output =%d", output );
    if( output == EPETTY )
        {
        SendGlobalInfoNoteL( EPhoneNoteTTYNoAudioControl );
        }
    else
        {
        TInt audioVolume( iStateMachine->PhoneEngineInfo()->AudioVolume() );
        audioVolume--;
        ChangeAudioVolumeL( audioVolume, ETrue );
        }
    }

// -----------------------------------------------------------
// CPhoneState::IncreaceAudioVolume()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::IncreaseAudioVolumeL()
    {
     __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IncreaceAudioVolumeL( ) ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );

    TPEAudioOutput output( iStateMachine->PhoneEngineInfo()->AudioOutput() );
     __PHONELOG1( EBasic, EPhoneControl, "CPhoneState::IncreaseAudioVolumeL - audio output =%d", output );
    if( output == EPETTY )
        {
        SendGlobalInfoNoteL( EPhoneNoteTTYNoAudioControl );
        }
    else
        {
        TInt audioVolume( iStateMachine->PhoneEngineInfo()->AudioVolume() );
        audioVolume++;
        ChangeAudioVolumeL( audioVolume, ETrue );
        }
    }

// -----------------------------------------------------------
// CPhoneState::ChangeAudioVolumeL()
// -----------------------------------------------------------
//
void CPhoneState::ChangeAudioVolumeL( TInt aLevel, TBool aUpdateControl )
    {
     __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::ChangeAudioVolumeL( ) ");
     __PHONELOG1( EBasic, EPhoneControl, "CPhoneState::ChangeAudioVolumeL - set volume =%d", aLevel );

    TInt valueToControl = aLevel;

    // sets value between 1 -10
    if ( aLevel>=KPhoneVolumeMinValue && aLevel<=KPhoneVolumeMaxValue )
        {
        iStateMachine->PhoneEngineInfo()->SetAudioVolumeCommand( aLevel );
        // Syncronizes audio volume level between engine and ui
        // causes EPEMessageAudioVolumeChanged message to phoneUI
        iStateMachine->SendPhoneEngineMessage(
            MPEPhoneModel::EPEMessageSetAudioVolume );
        }
    else
        {
        CAknKeySoundSystem* keySounds =
            static_cast<CAknAppUi*>( iEnv.EikAppUi() )
                ->KeySounds();

        if ( aLevel < KPhoneVolumeMinValue )
            {
            // Set the volume value to volume control
            valueToControl = KPhoneVolumeMinValue;
            keySounds->PlaySound( EAvkonSIDVolumeMinTone );
            }
        else // aLevel > KPhoneVolumeMaxValue
            {
            // Set the volume value to volume control
            valueToControl = KPhoneVolumeMaxValue;
            keySounds->PlaySound( EAvkonSIDVolumeMaxTone );
            }
        }

    if ( aUpdateControl )
        {
        // Update the volume display.
        // Upper max (11) and under min (-1)
        // values are also updated to volume popup.
        TPhoneCmdParamInteger volumeParam;
        volumeParam.SetInteger( valueToControl );
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewSetNaviPaneAudioVolume,
            &volumeParam );
        }
    }

// <-------------------------- COMMON STATE FUNCTIONS ------------------------>

// -----------------------------------------------------------
// CPhoneState::DialMultimediaCallL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DialMultimediaCallL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DialMultimediaCall() ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );
    iStateMachine->PhoneEngineInfo()->SetCallTypeCommand( EPECallTypeVideo );
    iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageDial );
    }

// -----------------------------------------------------------
// CPhoneState::DialVoiceCallL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DialVoiceCallL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DialVoiceCallL() ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );

    // Disable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
            &globalNotifierParam );
   iStateMachine->PhoneEngineInfo()->SetCallTypeCommand( EPECallTypeCSVoice );
    iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageDial );
    }

// -----------------------------------------------------------
// CPhoneState::DisconnectCallL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DisconnectCallL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DisconnectCallL( ) ");
    // Fetch active call's id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnected );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );

    if( callStateData.CallId() == KErrNotFound )
        {
        // No connected call, find the hold call
        callStateData.SetCallState( EPEStateHeld );
        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );

        if( callStateData.CallId() == KErrNotFound )
            {
            // No hold call, find the dialing call
            callStateData.SetCallState( EPEStateDialing );
            iViewCommandHandle->HandleCommandL(
                EPhoneViewGetCallIdByState, &callStateData );
            }

        if( callStateData.CallId() == KErrNotFound )
            {
            // No dialing call, find the outgoing call
            callStateData.SetCallState( EPEStateConnecting );
            iViewCommandHandle->HandleCommandL(
                EPhoneViewGetCallIdByState, &callStateData );
            }

        if( callStateData.CallId() == KErrNotFound )
            {
            // No active call, find the disconnectinging call
            callStateData.SetCallState( EPEStateDisconnecting );
            iViewCommandHandle->HandleCommandL(
                EPhoneViewGetCallIdByState, &callStateData );
            }
        }

    if( callStateData.CallId() > KErrNotFound )
        {
        // Release the call
        iStateMachine->SetCallId( callStateData.CallId() );
        if( IsVideoCall( callStateData.CallId() ) )
            {
            // Video call can be released only after we get response to VT Shutdown Command
            CPhoneMediatorFactory::Instance()->Sender()->IssueCommand( KMediatorVideoTelephonyDomain,
			                                                           			 KCatPhoneToVideotelCommands,
			                                                           			 EVtCmdReleaseDataport,
                                                                       TVersion( KPhoneToVideotelCmdVersionMajor,
                              	                                                 KPhoneToVideotelCmdVersionMinor,
                              	                                                 KPhoneToVideotelCmdVersionBuild ),
                                                                       KNullDesC8,
                                                                       CPhoneReleaseCommand::NewL( *iStateMachine ) );
            }
        else
            {
            // Release the call
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageRelease );
            }
        }
    else
        {
        __PHONELOG( EOnlyFatal, EPhoneControl,
            "CPhoneState::DisconnectCallL has negative call id!" );
        }   
   }

// -----------------------------------------------------------
// CPhoneState::DisplayIdleScreenL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DisplayIdleScreenL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DisplayIdleScreenL( ) ");

    // Don't remove reconnect query if it's shown
    if( !CPhoneReconnectQuery::InstanceL()->IsDisplayingQuery() )
        {
        // Remove dialogs if necessary
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
        }

    // Set Empty CBA
    iCbaManager->SetCbaL( EPhoneEmptyCBA );
    // Bring Idle app to the foreground
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringIdleToForeground );
    }

// -----------------------------------------------------------
// CPhoneState::SetupIdleScreenInBackgroundL()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::SetupIdleScreenInBackgroundL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SetupIdleScreenInBackgroundL( ) ");
    // Don't remove reconnect query if it's shown
    if( !CPhoneReconnectQuery::InstanceL()->IsDisplayingQuery() )
        {
        // Remove dialogs if necessary
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
        }
    // Return phone to the background
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );

    // Set Idle app as the top app
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetIdleTopApplication );

    // Set Empty CBA
    iCbaManager->SetCbaL( EPhoneEmptyCBA );
    }

// ---------------------------------------------------------
// CPhoneState::CallFromNumberEntryL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::CallFromNumberEntryL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::CallFromNumberEntryL( ) ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );

    if ( IsOnScreenDialerSupported() )
        {
        if ( IsDTMFEditorVisibleL() ||
             IsCustomizedDialerVisibleL() )
            {
            return;
            }

        else if( IsNumberEntryUsedL() )
            {
            // Query on top of dialer
            if ( IsAnyQueryActiveL() )
                {
                return;
                }
            else if ( IsMenuBarVisibleL() )
                {
                iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
                return;
                }
            // Open recent calls list when the number entry is empty
            TPhoneCmdParamInteger numberEntryCountParam;
            iViewCommandHandle->ExecuteCommandL( EPhoneViewGetNumberEntryCount,
            &numberEntryCountParam );
            TInt neLength( numberEntryCountParam.Integer() );
            TBool startLogs = neLength == 0 ? ETrue : EFalse;

            if ( startLogs )
                {
                iViewCommandHandle->HandleCommandL(
                EPhoneDialerCmdLog );
                return;
                }
            }
        }
    
    // If easydialing has focus, call should be initiated to focused contact.
    if ( IsDialingExtensionInFocusL() )
        {
        iViewCommandHandle->HandleCommandL( EEasyDialingVoiceCall );
        }
    else
        {  
        // Get the number entry contents
        HBufC* phoneNumber = PhoneNumberFromEntryLC();
        
        // Call the number
        iStateMachine->PhoneEngineInfo()->SetPhoneNumber( *phoneNumber );
    
        if ( phoneNumber->Des().Length() < KPhoneValidPhoneNumberLength )
            {
            // Closing effect is shown when dialer exist.
            BeginTransEffectLC( ENumberEntryClose );            
            iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
            EndTransEffect();
            
            HandleNumberEntryClearedL();
            }
    
        CleanupStack::PopAndDestroy( phoneNumber );        
    
        if ( !iCustomization || 
             !iCustomization->HandleCallFromNumberEntryL() )
            {
            // Customization didn't handle call. Dial voice call
            // as normally
            DialVoiceCallL();
            }
        }
    
    }

// -----------------------------------------------------------
// CPhoneState::DisplayHeaderForCallComingInL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DisplayHeaderForCallComingInL(
    TInt aCallId,
    TBool aWaitingCall )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DisplayHeaderForCallComingInL( ) ");
    CallheaderManagerL()->DisplayHeaderForCallComingInL( aCallId, aWaitingCall );
    }

// -----------------------------------------------------------
// CPhoneState::SetCallHeaderTextsForCallComingInL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::SetCallHeaderTextsForCallComingInL(
        TInt aCallId,
        TBool aWaitingCall,
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SetCallHeaderTextsForCallComingInL( ) ");
    CallheaderManagerL()->SetCallHeaderTextsForCallComingIn( aCallId, aWaitingCall, aCallHeaderData );
    }

// -----------------------------------------------------------
// CPhoneState::DisplayHeaderForOutgoingCallL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DisplayHeaderForOutgoingCallL(
    TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DisplayHeaderForOutgoingCallL( ) ");
    CallheaderManagerL()->DisplayHeaderForOutgoingCallL(aCallId);
    }

// -----------------------------------------------------------
// CPhoneState::UpdateSingleActiveCallL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::UpdateSingleActiveCallL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::UpdateSingleActiveCallL( ) ");

    // Stop capturing keys
    CaptureKeysDuringCallNotificationL( EFalse );

    BeginUiUpdateLC();

    // Update call state
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId,
        &callHeaderParam );

    // Update remote info data
    UpdateRemoteInfoDataL( aCallId );

    // Create call duration label if enabled
    TBool callDuration( EFalse );
    CPhoneCenRepProxy::Instance()->GetInt(
            KCRUidLogs, KLogsShowCallDuration, callDuration );

    if ( callDuration )
        {
        HandleChangedCallDurationL( aCallId );
        }

    EndUiUpdate();

     // Go to current state implementation
    iCbaManager->UpdateInCallCbaL();

    //Update state of switch to video or voice call touch button.
    TPECallType type = iStateMachine->PhoneEngineInfo()->CallType( aCallId );

    if( type == EPECallTypeVideo )
        {
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( ETrue );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetVideoCallFlag, &booleanParam );
        }
    else
        {
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetVideoCallFlag, &booleanParam );
        }

    // Check if application and number entry was open before incoming call. If so give control to number entry 
	// to prevent flickering
    if ( NeedToReturnToForegroundAppL() ||  IsAutoLockOn() )
        {
        // If number entry is used set control and visibility.
        if ( IsNumberEntryUsedL() )
           {
           iViewCommandHandle->ExecuteCommandL( EPhoneViewSetControlAndVisibility );
           }
        }
    }

// -----------------------------------------------------------
// CPhoneState::CaptureKeysDuringCallNotificationL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::CaptureKeysDuringCallNotificationL(
    TBool aCaptured )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::CaptureKeysDuringCallNotificationL( ) ");

    if ( AknLayoutUtils::PenEnabled() )
        {
        //Capture pointer events
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( aCaptured );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetPointerCapture, &booleanParam );
        }

    // Determine which view command to execute based on the capture status
    const TInt viewCommandId = aCaptured ?
        EPhoneViewStartCapturingKey :
        EPhoneViewStopCapturingKey;

    // Capture the App key
    TPhoneCmdParamKeyCapture appKeyCaptureParam;
    appKeyCaptureParam.SetKey( EStdKeyApplication0 );
    appKeyCaptureParam.SetKeyCode( EKeyApplication0 );
    appKeyCaptureParam.SetCaptureType( EPhoneKeyAllEvents );
    iViewCommandHandle->ExecuteCommandL( viewCommandId, &appKeyCaptureParam );

    // Capture the Camera key, if it exists
    if ( FeatureManager::FeatureSupported( KFeatureIdCamera ) )
        {
        TPhoneCmdParamKeyCapture cameraKeyCaptureParam;
        cameraKeyCaptureParam.SetKey( EStdKeyDevice7 );
        cameraKeyCaptureParam.SetKeyCode( EKeyDevice7 );
        cameraKeyCaptureParam.SetCaptureType( EPhoneKeyAllEvents );
        iViewCommandHandle->ExecuteCommandL( viewCommandId,
            &cameraKeyCaptureParam );
        }
    }

// -----------------------------------------------------------
// CPhoneState::IsWaitingCallL
// -----------------------------------------------------------
//
TBool CPhoneState::IsWaitingCallL( const TInt aCallId  )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsWaitingCallL( ) ");
    return CallheaderManagerL()->IsWaitingCallL( aCallId );
    }

// -----------------------------------------------------------
// CPhoneState::UpdateCallHeaderInfoL
// -----------------------------------------------------------
//
TPhoneCmdParamCallHeaderData CPhoneState::UpdateCallHeaderInfoL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::UpdateCallHeaderInfoL( ) ");
    return CallheaderManagerL()->UpdateCallHeaderInfoL( aCallId );
    }

// -----------------------------------------------------------
// CPhoneState::UpdateRemoteInfoDataL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::UpdateRemoteInfoDataL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::UpdateRemoteInfoDataL() ");
    TPhoneCmdParamCallHeaderData callHeaderParam = UpdateCallHeaderInfoL( aCallId );

    if( iCustomization )
        {
        TBuf<KCntMaxTextFieldLength> inCallNumberText( KNullDesC );
        // incall number text could be 'Call 1', 'Call 2', ...
        CallheaderManagerL()->GetInCallNumberTextL( aCallId, inCallNumberText );
        // to check if we have VoIP call in question and fix
        // parameters if needed
        iCustomization->ModifyCallHeaderTexts( aCallId, &callHeaderParam,
            inCallNumberText );
        }


    // Update the remote info data in the call header
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewUpdateCallHeaderRemoteInfoData,
        aCallId,
        &callHeaderParam );
    }

// -----------------------------------------------------------
// CPhoneState::SetCallHeaderType
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::SetCallHeaderType(
    const CBubbleManager::TPhoneCallTypeFlags aCallHeaderType )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::SetCallHeaderType() ");
    TRAP_IGNORE( CallheaderManagerL()->SetCallHeaderType( aCallHeaderType ) );
    }

// -----------------------------------------------------------
// CPhoneState::GetRemoteInfoDataL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::GetRemoteInfoDataL(
    TInt aCallId,
    TDes& aData )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::GetRemoteInfoDataL() ");
    CallheaderManagerL()->GetRemoteInfoDataL( aCallId, aData );
    }

// -----------------------------------------------------------
// CPhoneState::UpdateCbaSwivelStateChangedL
// -----------------------------------------------------------
//
void CPhoneState::UpdateCbaSwivelStateChangedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,"CPhoneState::UpdateCbaSwivelStateChangedL()" );

    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    TInt incomingCall = callStateData.CallId();

    TPhoneCmdParamInteger activeCallCount;
    iViewCommandHandle->ExecuteCommandL(
            EPhoneViewGetCountOfActiveCalls, &activeCallCount );

    if ( incomingCall > KErrNotFound )
        {
       if( activeCallCount.Integer() == ENoActiveCalls )
            {
            iCbaManager->UpdateIncomingCbaL( incomingCall );
            UpdateSilenceButtonDimming();
            }
        else
            {
            iCbaManager->UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
            }
        }
    else if ( activeCallCount.Integer() != ENoActiveCalls )
        {
        iCbaManager->UpdateInCallCbaL();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneState::IsSwivelClosed
// This function checks from PubSubProxy that is
// Swivel closed or not in product.
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsSwivelClosed() const
    {
    if( CPhoneCenRepProxy::Instance()->
            IsTelephonyFeatureSupported( KTelephonyLVFlagSwivelInDevice ))
        {
        return CPhonePubSubProxy::Instance()->
            Value( KPSUidHWRM, KHWRMGripStatus ) == EPSHWRMGripClosed;
        }
    else
        {
        return EFalse;
        }
    }

// <-------------------------- CONTEXT MENU ------------------------->

// -----------------------------------------------------------------------------
// CPhoneState::SetContextMenu
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneState::SetContextMenuL( TInt aResourceId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SetContextMenuL() ");

    TPhoneCmdParamInteger integerParam;
    TInt resId( CPhoneMainResourceResolver::Instance()->
                            ResolveResourceID( aResourceId ) );
    integerParam.SetInteger( resId );
    __PHONELOG1( EBasic, EPhoneControl,
                  "CPhoneState::SetContextMenuL : resId =%d",resId );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateContextMenu,
                                      &integerParam );
    }
// -----------------------------------------------------------------------------
// CPhoneState::UpdateInCallContextMenuL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneState::UpdateInCallContextMenuL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::UpdateInCallContextMenuL() ");

    SetContextMenuL( EPhoneNumberAcqMenubar );

    }

// -----------------------------------------------------------------------------
// CPhoneState::UpdateIncomingContextMenuL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneState::UpdateIncomingContextMenuL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::UpdateIncomingContextMenuL() ");

    if( CPhoneState::IsVideoCall ( aCallId ) )
        {
        SetContextMenuL( EPhoneIncomingVideoCallMenubar );
        }
    else
        {
        SetContextMenuL( EPhoneIncomingCallMenubar );
        }

    }

// -----------------------------------------------------------
// CPhoneState::ShowNoteL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::ShowNoteL( TInt aResourceId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::ShowNoteL( ) ");
    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );
    TPhoneCmdParamNote noteParam;
    noteParam.SetType( EPhoneNotePermanent );
    noteParam.SetResourceId( CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( aResourceId ) );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNote, &noteParam );
    }

// -----------------------------------------------------------
// CPhoneState::ShowQueryL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::ShowQueryL( TInt aResourceId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::ShowQueryL( ) ");
    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );
    TPhoneCmdParamQuery queryParam;
    queryParam.SetQueryType( EPhoneQueryDialog );
    queryParam.SetQueryResourceId( CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( aResourceId ) );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowQuery, &queryParam );
    }

// -----------------------------------------------------------
// CPhoneState::ShowTextQueryL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::ShowTextQueryL(
    TInt aDialogResourceId,
    TInt aDefaultCbaResourceId,
    TInt aContentCbaResourceId,
    TDes* aDataText,
    TBool aSendKeyEnabled )
    {
    __ASSERT_DEBUG( aDialogResourceId &&
        aDefaultCbaResourceId &&
        aContentCbaResourceId &&
        aDataText,
        Panic( EPhoneCtrlParameterNotInitialized ) );
    TPhoneCmdParamQuery queryDialogParam;
    queryDialogParam.SetQueryType( EPhoneTextQueryDialog );
    queryDialogParam.SetQueryResourceId( aDialogResourceId );
    queryDialogParam.SetDefaultCba( aDefaultCbaResourceId );
    queryDialogParam.SetContentCba( aContentCbaResourceId );
    queryDialogParam.SetDataText( aDataText );
    queryDialogParam.SetSendKeyEnabled( aSendKeyEnabled );

    // Display dialog
    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowQuery,
        &queryDialogParam );
    }

// -----------------------------------------------------------
// CPhoneState::DisplayCallTerminationNoteL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DisplayCallTerminationNoteL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DisplayCallTerminationNoteL( ) ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );
    if ( CheckIfShowCallTerminationNote() )
        {
        TBuf<KPhoneMaxCharsInNote> noteText;
        TBuf<KPhoneMaxCharsInNote> postCallLineText;

        // Post call Line 1: Call Summary
        LoadResource( postCallLineText, EPhoneNoteTextCallSummary );
        noteText.Append( postCallLineText );
        noteText.Append( KPhoneLineFeed );

        // Post call Line 2: Duration
        LoadResource( postCallLineText, EPhoneNoteTextCallDuration );
        noteText.Append( postCallLineText );
        noteText.Append( KPhoneLineFeed );

        // Post call Line 3: Time, get the format for last call time.
        LoadResource( postCallLineText, EPhoneCallDurationFormatString );
        TPtrC durationFormat( postCallLineText );

        // Get the last call duration
        TTime time( 0 );
        TTimeIntervalSeconds timeInterval =
            iStateMachine->PhoneEngineInfo()->CallDuration();
        time += timeInterval;

        // Format the time
        TBuf<KPhoneMaxTimeDisplayTextLength> timeString( KNullDesC );
        time.FormatL( timeString, durationFormat );

        AknTextUtils::DisplayTextLanguageSpecificNumberConversion( timeString );

        noteText.Append( timeString );

        TPhoneCmdParamGlobalNote globalNoteParam;
        globalNoteParam.SetType( EAknGlobalInformationNote );
        globalNoteParam.SetText( noteText );
        globalNoteParam.SetTone( CAknNoteDialog::ENoTone );
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    }

// ---------------------------------------------------------
// CPhoneState::SendGlobalInfoNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SendGlobalInfoNoteL( TInt aResourceId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SendGlobalInfoNoteL( ) ");
    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );
    if ( CPhonePubSubProxy::Instance()->Value(
            KPSUidUikon, KUikGlobalNotesAllowed ) == 1 )
        {
        // Re-enable global notes
        TPhoneCmdParamBoolean globalNotifierParam;
        globalNotifierParam.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
            &globalNotifierParam );

        TPhoneCmdParamGlobalNote globalNoteParam;

        globalNoteParam.SetType( EAknGlobalInformationNote );
        globalNoteParam.SetTextResourceId(
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( aResourceId ) );
        globalNoteParam.SetTone( EAvkonSIDInformationTone );

        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    }

// ---------------------------------------------------------
//  CPhoneUIController::SendGlobalWarningNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SendGlobalWarningNoteL( TInt aResourceId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SendGlobalWarningNoteL( ) ");
    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );
    if ( CPhonePubSubProxy::Instance()->Value(
            KPSUidUikon, KUikGlobalNotesAllowed ) == 1 ||
            SimState() == EPESimReadable )
        {
        // Re-enable global notes
        TPhoneCmdParamBoolean globalNotifierParam;
        globalNotifierParam.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
            &globalNotifierParam );

        TPhoneCmdParamGlobalNote globalNoteParam;
        globalNoteParam.SetType( EAknGlobalWarningNote );
        globalNoteParam.SetTextResourceId(
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( aResourceId ) );
        globalNoteParam.SetTone( EAvkonSIDWarningTone );

        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    }

// ---------------------------------------------------------
//  CPhoneUIController::SendGlobalErrorNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SendGlobalErrorNoteL( TInt aResourceId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SendGlobalErrorNoteL( ) ");
    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );
    if ( CPhonePubSubProxy::Instance()->Value(
            KPSUidUikon, KUikGlobalNotesAllowed ) == 1 )
        {
        // Re-enable global notes
        TPhoneCmdParamBoolean globalNotifierParam;
        globalNotifierParam.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
            &globalNotifierParam );

        TPhoneCmdParamGlobalNote globalNoteParam;
        globalNoteParam.SetType( EAknGlobalErrorNote );
        globalNoteParam.SetTextResourceId(
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( aResourceId ) );
        globalNoteParam.SetTone( CAknNoteDialog::EErrorTone );

        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    }

// ---------------------------------------------------------
// CPhoneState::SetHandsfreeModeL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SetHandsfreeModeL( TBool aHandsfreeMode )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SetHandsfreeModeL( ) ");
    CPhoneAccessoryBTHandler* bt = CPhoneAccessoryBTHandler::NewLC(
        iViewCommandHandle, iStateMachine, this );
    if ( !bt->SetHandsfreeModeL( aHandsfreeMode ))
        {
        SendGlobalErrorNoteL( EPhoneNoteTextNotAllowed );
        }
    CleanupStack::PopAndDestroy( bt );
    }

// ---------------------------------------------------------
// CPhoneState::SetBTHandsfreeModeL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SetBTHandsfreeModeL( TBool aHandsfreeMode )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SetBTHandsfreeModeL( ) ");
    CPhoneAccessoryBTHandler* bt = CPhoneAccessoryBTHandler::NewLC(
        iViewCommandHandle, iStateMachine, this );
    if ( !bt->SetBTHandsfreeModeL( aHandsfreeMode ))
        {
        SendGlobalErrorNoteL( EPhoneNoteTextNotAllowed );
        }
    CleanupStack::PopAndDestroy( bt );
    }

// <-------------------------- INTERNAL FUNCTIONS ------------------------>

// -----------------------------------------------------------
// CPhoneState::UpdateIncallIndicatorL
// -----------------------------------------------------------
//
void CPhoneState::UpdateIncallIndicatorL( TInt aCallState )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::UpdateIncallIndicatorL( ) ");
    TPhoneCmdParamIncallIndicatorData incallIndicatorParam;

    // Set the state
    incallIndicatorParam.SetCallState( aCallState );

    // Set mode
    incallIndicatorParam.SetMode(
       CPhonePubSubProxy::Instance()->Value(
            KPSUidCtsyCallInformation,
            KCTsyCallType ) );

    TInt activeCallId = GetActiveCallIdL();
    if ( activeCallId > KErrNone )
        {
        if ( iStateMachine->PhoneEngineInfo()->CallALSLine( activeCallId )
             == CCCECallParameters::ECCELineTypeAux )
            {
            incallIndicatorParam.SetLine2( ETrue );
            }
        }

    // Set the mute status
    const TBool audioMute = iStateMachine->PhoneEngineInfo()->AudioMute();
    incallIndicatorParam.SetMute( audioMute );

    // Set the voice privacy status
    if ( activeCallId > KErrNotFound &&
         activeCallId != KConferenceCallId  )
        {
        incallIndicatorParam.SetCiphering(
            iStateMachine->PhoneEngineInfo()->IsSecureCall( activeCallId ) );
        }
    else
        {
        incallIndicatorParam.SetCiphering( ETrue );
        }

    incallIndicatorParam.SetCipheringIndicatorAllowed(
        iStateMachine->PhoneEngineInfo()->SecureSpecified() );

    // Set the emergency status
    if( EPEStateIdle != iStateMachine->PhoneEngineInfo()->CallState( KPEEmergencyCallId ) )
        {
        incallIndicatorParam.SetEmergency( ETrue );
        }

    if( aCallState == EPSCTsyCallStateDisconnecting )
        {
        if ( TopAppIsDisplayedL() )
            {
            if ( ( !IsOnScreenDialerSupported() ) ||
                 ( IsOnScreenDialerSupported() && !IsNumberEntryVisibleL() ) )
                {
                // Phone application is in the foreground so we don't need to
                // display the little bubble. If we don't hide it here then
                // it will appear for a short time. We don't want that.
                incallIndicatorParam.SetLittleBubbleVisible( EFalse );
                }
            }
        }
    // TODO: Refactor -> this looks really dubious.
    else
        {
        incallIndicatorParam.SetLittleBubbleVisible( ETrue );
        }

    // Update the in-call indicator
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateIncallIndicator,
        &incallIndicatorParam );
    }

// -----------------------------------------------------------
// CPhoneState::UpdateProfileDisplayL
// -----------------------------------------------------------
//
void CPhoneState::UpdateProfileDisplayL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::UpdateProfileDisplayL( ) ");
    // Get the profile information
    const TInt profileId =
        iStateMachine->PhoneEngineInfo()->ProfileId();
    TPEProfileName profileName =
        iStateMachine->PhoneEngineInfo()->ProfileName();

    HBufC* profileString = HBufC::NewLC( profileName.Length() +
        KPhoneMaxDateDisplayTextLength );

    TPtr profileNameBuf = profileString->Des();
    profileNameBuf.Zero();
    profileNameBuf.Append( profileName );

    if ( ( profileId == EProfileGeneralId ) ||
        ( profileId == EProfileOffLineId ) )
        {
        // Show date instead of profile name on navi pane
        HBufC* buffer = HBufC::NewLC( KPhoneMaxDateDisplayTextLength );
        TPtr dateString( buffer->Des() );

        TTime time;
        time.HomeTime();

        // Get date string
        CCoeEnv* coeEnv = CCoeEnv::Static();

        HBufC* dateFormatString =
            StringLoader::LoadLC( R_QTN_DATE_USUAL_WITH_ZERO, coeEnv );
        time.FormatL( dateString, *dateFormatString );
        CleanupStack::PopAndDestroy( dateFormatString );

        // To arabic
        AknTextUtils::DisplayTextLanguageSpecificNumberConversion(
            dateString );

        //Get week day abbreviation
        TDayNameAbb wkDayAbb = TDayNameAbb();
        wkDayAbb.Set(time.DayNoInWeek());

        //Get time format string
        HBufC* timeFormat = StringLoader::LoadLC(
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneIdleTimeFormat ),
            coeEnv );

        //Set date and weekdays to time format
        profileNameBuf.Zero();

        StringLoader::Format(
            profileNameBuf,
            *timeFormat,
            1, // date
            dateString );

        dateString.Copy( profileNameBuf );

        StringLoader::Format(
            profileNameBuf,
            dateString,
            0, // weekday
            wkDayAbb );

        CleanupStack::PopAndDestroy( timeFormat );
        CleanupStack::PopAndDestroy( buffer );
        }

    // Set silence indicator based on the ringing profile
    const TProfileRingingType ringingType =
        iStateMachine->PhoneEngineInfo()->RingingType();
    if ( ringingType == EProfileRingingTypeSilent )
        {
        TBuf< 1 > silent;
        silent.Append( KPuaCodeSilentSymbol );
        profileNameBuf.Insert( 0, silent );
        }

    // Update the profile display on the navi pane
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetNaviPaneContent, 0,
        profileNameBuf );

    if ( profileId == EProfileOffLineId )
        {
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewSetTitlePaneContent,
            0,
            profileName );
        }
    else
        {
        UpdateOperatorDisplayL();
        }

    CleanupStack::PopAndDestroy( profileString );
    }

// -----------------------------------------------------------
// CPhoneState::UpdateOperatorDisplayL
// -----------------------------------------------------------
//
void CPhoneState::UpdateOperatorDisplayL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::UpdateOperatorDisplayL( ) ");
    __PHONELOG( EBasic, EPhoneControl, "CPhoneState::UpdateOperatorDisplayL" );
    // Get current title pane content.
    TTelTitleDisplay titleContent;
    titleContent.iDisplayTag.Zero();
    titleContent.iLogoHandle = 0;
    TPckg<TTelTitleDisplay> titlePckg( titleContent );

    RProperty::Get(
        KPSUidTelInformation,
        KTelDisplayInfo,
        titlePckg );

    if ( TitlePaneIsVisibleL() )
        {
        // There is a title pane icon.
        if ( titleContent.iLogoHandle != 0 )
            {
            // Set new logo.
            __PHONELOG( EBasic, EPhoneControl,
                "CPhoneState::UpdateOperatorDisplayL - logo found" );

            if( iLogoHandle != titleContent.iLogoHandle )
                {
                 __PHONELOG( EBasic, EPhoneControl,
                  "CPhoneState::UpdateOperatorDisplayL - set new logo" );

                 TPhoneCmdParamBitmap bitmapParam;

                 // Duplicate bitmap from handle.
                 CFbsBitmap* bitmap = new (ELeave) CFbsBitmap;
                 CleanupStack::PushL( bitmap );
                 User::LeaveIfError(
                         bitmap->Duplicate( titleContent.iLogoHandle ) );
                 bitmapParam.SetBitmap( bitmap );

                 // Duplicate and set bitmap mask if needed
                 CFbsBitmap* maskBitmap = NULL;
                 if( titleContent.iLogoMaskHandle != 0 )
                     {
                     maskBitmap = new( ELeave )CFbsBitmap;
                     CleanupStack::PushL( maskBitmap );
                     User::LeaveIfError( maskBitmap->Duplicate(
                             titleContent.iLogoMaskHandle ) );
                     bitmapParam.SetMaskBitmap( maskBitmap );
                     }

                 iViewCommandHandle->ExecuteCommandL(
                         EPhoneViewSetTitlePanePicture,
                         &bitmapParam );

                 // The title pane takes ownership of the bitmaps so no need
                 // to destroy it here.
                 if( maskBitmap )
                     {
                     // Pop maskBitmap only, if it has been created
                     CleanupStack::Pop( maskBitmap );
                     }
                 CleanupStack::Pop( bitmap );
                }
            }
        else
            {
            __PHONELOG1( EBasic, EPhoneControl,
            "CPhoneState::UpdateOperatorDisplayL - set text=%S", &titleContent.iDisplayTag );
            iLogoHandle = titleContent.iLogoHandle;

            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewSetTitlePaneContent,
                0,
                titleContent.iDisplayTag );
            }
        }

    // Check background image.
    TPhoneCmdParamBitmap savedBitmapParam;
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewGetBackgroundImageBitmap,
        &savedBitmapParam );

    if ( titleContent.iBackgroundImageHandle !=
        savedBitmapParam.Bitmap()->Handle() )
        {
        __PHONELOG2( EBasic, EPhoneControl,
        "CPhoneState::UpdateOperatorDisplayL - update background image since different titleContent(%d), saved(%d)",
            titleContent.iBackgroundImageHandle, savedBitmapParam.Bitmap()->Handle() );
        // Background image has changed. Duplicate bitmap from handle if
        // available; otherwise reset the background image
        CFbsBitmap* bitmap = new (ELeave) CFbsBitmap;
        CleanupStack::PushL( bitmap );
        TInt err = KErrNone;
        if ( titleContent.iBackgroundImageHandle != 0 )
            {
            err = bitmap->Duplicate( titleContent.iBackgroundImageHandle );
            }
        else
            {
            bitmap->Reset();
            }

        if ( err == KErrNone )
            {
            iBitmapRedrawCounter = titleContent.iBackgroundImageRedrawCounter;
            TPhoneCmdParamBitmap bitmapParam;
            bitmapParam.SetBitmap( bitmap );
            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewSetBackgroundImageBitmap,
                &bitmapParam );
            }

        CleanupStack::PopAndDestroy( bitmap );
        }
    else if ( titleContent.iBackgroundImageRedrawCounter !=
                  iBitmapRedrawCounter  )
        {
        iBitmapRedrawCounter = titleContent.iBackgroundImageRedrawCounter;
        iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateView );
        }
    }

// -----------------------------------------------------------
// CPhoneState::HandleInitiatedEmergencyCallL
// Default handling for EPEMessageInitiatedEmergencyCallL message
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneState::HandleInitiatedEmergencyCallL( TInt /*aCallId*/ )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleInitiatedEmergencyCallL( ) ");

    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );

    // Stop tone playing, if necessary
    iViewCommandHandle->ExecuteCommandL( EPhoneViewStopRingTone );

    // Reset Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    // Undim Answer button for sure.
    SetTouchPaneButtonEnabled( EPhoneCallComingCmdAnswer );

    // Go to emergency call state
    // No need update cba
    iStateMachine->ChangeState( EPhoneStateEmergency );
    }

// -----------------------------------------------------------
// CPhoneState::HandleInitiatedEmergencyWhileActiveVideoL
// -----------------------------------------------------------
//
void CPhoneState::HandleInitiatedEmergencyWhileActiveVideoL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleInitiatedEmergencyWhileActiveVideoL( ) ");

    // We have existing video call so need to release dataport before continuing
    // emergency call. Otherwise we will face problems with dataport use later.
    CPhoneMediatorFactory::Instance()->Sender()->IssueCommand( KMediatorVideoTelephonyDomain,
                                                                     KCatPhoneToVideotelCommands,
                                                                     EVtCmdReleaseDataport,
                                                               TVersion( KPhoneToVideotelCmdVersionMajor,
                                                                         KPhoneToVideotelCmdVersionMinor,
                                                                         KPhoneToVideotelCmdVersionBuild ),
                                                               KNullDesC8,
                                                               CPhoneContinueEmergencyCallCommand::NewL( *iStateMachine ) );
    }

// -----------------------------------------------------------
// CPhoneState::HandleShowImeiL
// -----------------------------------------------------------
//
void CPhoneState::HandleShowImeiL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleShowImeiL( ) ");

    if ( IsOnScreenDialerSupported() )
        {
        NumberEntryClearL();
        }
    else
        {
        // Remove number entry from screen
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }

    // Fetch IMEI
    TBuf<RMobilePhone::KPhoneSerialNumberSize> serialNumber;
    TPEPhoneIdentityParameters phoneIdentityParameters = iStateMachine->
        PhoneEngineInfo()->PhoneIdentityParameters();
    serialNumber = phoneIdentityParameters.iSerialNumber;

    // Add it to the resource string
    HBufC* buf = StringLoader::LoadLC(
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID(
        EPhonePhoneImeiString ), serialNumber );

    TPhoneCmdParamNote noteParam;
    noteParam.SetType( EPhoneNoteCustom );
    noteParam.SetResourceId( CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneInformationWaitNote ) );
    noteParam.SetText( *buf );

    // Display note
    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNote, &noteParam );

    CleanupStack::PopAndDestroy( buf );
    }

// -----------------------------------------------------------
// CPhoneState::HandleCallSecureStatusChangeL
// -----------------------------------------------------------
//
void CPhoneState::HandleCallSecureStatusChangeL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleCallSecureStatusChangeL( ) ");

    TBool ciphering( ETrue );
    TBool secureSpecified( ETrue );

    if ( aCallId > KErrNotFound )
        {
        ciphering = iStateMachine->PhoneEngineInfo()->IsSecureCall( aCallId );
        secureSpecified = iStateMachine->PhoneEngineInfo()->SecureSpecified();
        }

    TPhoneCmdParamCallHeaderData callHeaderParam;

    callHeaderParam.SetCiphering( ciphering );
    callHeaderParam.SetCipheringIndicatorAllowed( secureSpecified );

    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewCipheringInfoChange,
        aCallId,
        &callHeaderParam );
    }

// -----------------------------------------------------------
// CPhoneState::HandleShowLifeTimerL
// -----------------------------------------------------------
//
void CPhoneState::HandleShowLifeTimerL()
   {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleShowLifeTimerL( ) ");
    if ( IsOnScreenDialerSupported() )
        {
        NumberEntryClearL();
        }
    else
        {
        // Remove number entry from screen
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }

    // Fetch LifeTime
    TPELifeTimeData lifeTimeData = iStateMachine->
        PhoneEngineInfo()->LifeTimerData();

    TLocale locale;
    TBuf<KTimerTextLength> lifetimerText;
    lifetimerText.NumFixedWidth(
        lifeTimeData.iHours,
        EDecimal,
        KPhoneLifeTimerHoursLength );

    lifetimerText.Append(locale.TimeSeparator( KTimerMinuteSeparator ) );   // minute seperator

    TBuf<KPhoneLifeTimerMinutesLength> mins;
    mins.NumFixedWidth(
        lifeTimeData.iMinutes,
        EDecimal,
        KPhoneLifeTimerMinutesLength );

    lifetimerText.Append(mins);

    // Add it to the resource string
    HBufC* buf = StringLoader::LoadLC(
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID(
        EPhoneLifeTimeFormat ), lifetimerText );
    TPhoneCmdParamNote noteParam;
    noteParam.SetType( EPhoneNoteCustom );
    noteParam.SetResourceId( CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneInformationWaitNote ) );
    noteParam.SetText( *buf );

    // Display note
    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNote, &noteParam );

    CleanupStack::PopAndDestroy( buf );
    }

// ---------------------------------------------------------
// CPhoneState::IsVideoCall
// ---------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsVideoCall( const TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsVideoCall( ) ");
    TBool isVideoCall( EFalse );
    TRAP_IGNORE( isVideoCall = CallheaderManagerL()->IsVideoCall( aCallId ) );
    __PHONELOG1( EBasic, EPhoneControl, 
        "CPhoneState::IsVideoCall isVideoCall(%d)",
        isVideoCall );
    return isVideoCall;
    }

// ---------------------------------------------------------
// CPhoneState::ChangeAlsLineL
// ---------------------------------------------------------
//
void CPhoneState::ChangeAlsLineL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::ChangeAlsLineL( ) ");

    if ( !IsOnScreenDialerSupported() )
        {
        // Do state-specific operation when number entry is cleared
        HandleNumberEntryClearedL();
        }

    CCCECallParameters::TCCELineType currentLine;
    TSSSettingsAlsValue newLine( ESSSettingsAlsPrimary );

    currentLine = iStateMachine->PhoneEngineInfo()->ALSLine();

    if ( currentLine == CCCECallParameters::ECCELineTypePrimary )
        {
        newLine = ESSSettingsAlsAlternate;
        }
    else if ( currentLine == CCCECallParameters::ECCELineTypeAux )
        {
        newLine = ESSSettingsAlsPrimary;
        }

    RSSSettings ssSettings;
    TInt retValue = ssSettings.Open();
    __PHONELOG1( EBasic, EPhoneControl,
            "CPhoneState::ChangeAlsLineL : Open %d", retValue );
    if ( retValue == KErrNone )
        {
        retValue = ssSettings.Set( ESSSettingsAls, newLine );
        __PHONELOG1( EBasic, EPhoneControl,
            "CPhoneState::ChangeAlsLineL : Set %d", retValue );
        ssSettings.Close();
        }
    }

// ---------------------------------------------------------
// CPhoneState::GetActiveCallIdL()
// ---------------------------------------------------------
//
TInt CPhoneState::GetActiveCallIdL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::GetActiveCallId()( ) ");

    // Fetch active call's id from view
    TPhoneViewResponseId response;
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnected );
    response = iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );

    if ( response == EPhoneViewResponseSuccess &&
         callStateData.CallId() < 0 )  // no connected calls
        {
        // check for held call
        callStateData.SetCallState( EPEStateHeld );
        response = iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );
        }

    return callStateData.CallId();
    }

// ---------------------------------------------------------
// CPhoneState::SimState()
// ---------------------------------------------------------
//
EXPORT_C TPESimState CPhoneState::SimState() const
    {

    /*
    SIM states:

    EPESimStatusUninitialized = KPEStartupEnumerationFirstValue =100,
    EPESimUsable,       // The Sim card is fully usable.
    EPESimReadable,     // The SIM card is not fully usable, but the emergency number can be read.
    EPESimNotReady,     // The Sim card is present but not ready or usable.
    EPESimNotPresent,   // The Sim card is not present.
    EPESimNotSupported  // SIM/RUIM is not supported. Some CDMA phones do not support a SIM/RUIM at all.
    */
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SimState()");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );
    TPESimState simState = iStateMachine->PhoneEngineInfo()->SimState();

    __PHONELOG1( EBasic, EPhoneControl,
            "CPhoneState::SimState - value= %d", simState );
    return simState;
    }

// ---------------------------------------------------------
// CPhoneState::IsSimOk()
// ---------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsSimOk()
    {

    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsSimOk()");
    // Phone is interested on Sim state and sim security statuses
    // Check first Sim state status:
    TBool retVal( ETrue );
    switch ( SimState() )
        {
        case EPESimNotSupported:
            retVal = EFalse;
            break;

        case EPESimNotPresent:
            {
            if ( !FeatureManager::FeatureSupported( KFeatureIdOfflineMode ) ||
                 !FeatureManager::FeatureSupported( KFeatureIdFfSimlessOfflineSupport ) )
                {
                retVal = EFalse;
                }
            break;
            }

        default:
            break;
        }
    if ( retVal != EFalse )
        {
        // Sim state is valid, check also Sim security status
        switch ( SimSecurityStatus() )
            {
            case ESimRejected:
            case ESimUnaccepted:
            case ESimInvalid:
                retVal = EFalse;
                break;

            default:
                break;
            }
        }
    __PHONELOG1( EBasic, EPhoneControl,
            "CPhoneState::IsSimOK - value= %d", retVal );
    return retVal;
    }

// -----------------------------------------------------------
// CPhoneState::IsSimStateNotPresentWithSecurityModeEnabled
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsSimStateNotPresentWithSecurityModeEnabled()
    {
    if ( SimState() == EPESimNotPresent && iStateMachine->SecurityMode()->IsSecurityMode() )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// ---------------------------------------------------------
// CPhoneState::SetDivertIndication
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SetDivertIndication( const TBool aDivertIndication )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::SetDivertIndication()");
    TRAP_IGNORE( CallheaderManagerL()->SetDivertIndication( aDivertIndication ) );
    }

// ---------------------------------------------------------
// CPhoneState::StartAlsLineChangeTimerL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::StartAlsLineChangeTimerL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::StartAlsLineChangeTimerL()");
    TBool alsLineAvailable = iStateMachine->PhoneEngineInfo()->ALSLineSupport();

    if( alsLineAvailable )
        {
        if( !iAlsLineChangeKeyPressTimer )
            {
            iAlsLineChangeKeyPressTimer = CPhoneTimer::NewL();
            }

        iAlsLineChangeKeyPressTimer->After( KAlsLineChangeTimerValue,
            TCallBack( AlsLineChangeTimerCallbackL, this ) );
        }
    else
        {
         // Don't bother launching the timer. ALS not supported.
        __PHONELOG( EBasic, EPhoneControl,
            "CPhoneState::StartAlsLineChangeTimerL - ALS not supported " );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneState::StartShowSecurityNoteL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneState::StartShowSecurityNoteL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::StartShowSecurityNoteL ");

    // Remove number entry from screen
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );

    TPhoneCmdParamBoolean visibleMode;
    visibleMode.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetStatusPaneVisible, &visibleMode );
    
    iCbaManager->UpdateCbaL( EPhoneEmptyCBA );

    TPhoneCmdParamInteger uidParam;
    // Bring Phone app in the foreground
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );

    // Set Phone as the top application
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
        &uidParam );

    TInt resourceId ( KErrNone );


    switch( SimSecurityStatus() )
        {
			  case ESimRejected:
			      {
			      resourceId = CPhoneMainResourceResolver::Instance()->
              ResolveResourceID( EPhoneSimRejected );
            break;
			      }
			  case ESimUnaccepted:
			      {
			      // insert sim card -note
            resourceId = CPhoneMainResourceResolver::Instance()->
              ResolveResourceID( EPhoneSimUnaccepted );	
			      break;
			      }
			  case ESimInvalid:
			      {
			      resourceId = CPhoneMainResourceResolver::Instance()->
              ResolveResourceID( EPhoneSIMInvalidUICC );
			      break;
			      }
        default:
	          {
	          if ( SimState() == EPESimNotPresent )
                {
                // insert sim card -note
                resourceId = CPhoneMainResourceResolver::Instance()->
                    ResolveResourceID( EPhoneSimRemoved );
                }
	          break;	
	          }
        }

    if ( resourceId != KErrNone )
        {
        // Add it to the resource string
        HBufC* buf = StringLoader::LoadLC( resourceId );

        TPhoneCmdParamNote noteParam;

        noteParam.SetResourceId( CPhoneMainResourceResolver::Instance()->
           ResolveResourceID( EPhoneSecurityInformationNote ) );

        noteParam.SetText( *buf );
        noteParam.SetTone( CAknNoteDialog::EConfirmationTone );
        noteParam.SetType( EPhoneNoteSecurity );
        // Display note
        iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNote, &noteParam );

        CleanupStack::PopAndDestroy( buf );
           
           
        // Capture the App key
        TPhoneCmdParamKeyCapture appKeyCaptureParam;
        appKeyCaptureParam.SetKey( EStdKeyApplication0 );
        appKeyCaptureParam.SetKeyCode( EKeyApplication0 );
        appKeyCaptureParam.SetCaptureType( EPhoneKeyAllEvents );
        iViewCommandHandle->ExecuteCommandL( 
               EPhoneViewStartCapturingKey, &appKeyCaptureParam );
        }
    }

// ---------------------------------------------------------
// CPhoneState::AlsLineChangeTimerCallbackL
// ---------------------------------------------------------
//
TInt CPhoneState::AlsLineChangeTimerCallbackL( TAny* aAny )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::AlsLineChangeTimerCallbackL()");

    // Send a key up event for stopping keypad tone
    reinterpret_cast<CPhoneState*>( aAny )->
        iStateMachine->SendPhoneEngineMessage(
        MPEPhoneModel::EPEMessageEndDTMF );

    if ( !( reinterpret_cast<CPhoneState*>( aAny )->
            IsOnScreenDialerSupported() ) )
        {
        // If dialer is undefined remove the number entry.
        reinterpret_cast<CPhoneState*>( aAny )->
            iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }
    else
        {
        // If on screen dialer is in use just clear entry
        // do not remove dialer.
        reinterpret_cast<CPhoneState*>( aAny )->
            NumberEntryClearL();
        }

    // Show the als line changing confirmation query
    reinterpret_cast<CPhoneState*>( aAny )->
        ShowQueryL( EPhoneAlsLineChangeConfirmationQuery );

    return KErrNone;
    }

// ---------------------------------------------------------
// CPhoneState::ShowWlanMacAddressL
// ---------------------------------------------------------
//
void CPhoneState::ShowWlanMacAddressL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::ShowWlanMacAddressL()");
    if ( IsOnScreenDialerSupported() )
        {
        NumberEntryClearL();
        }
    else
        {
        // Remove number entry from screen
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }

    // Fetch WLAN MAC address
    TBuf8<KPhoneWlanMacAddressLength> address;
    RProperty::Get(
        KPSUidWlan,
        KPSWlanMacAddress,
        address );

    // Format fetched address
    TBuf<KPhoneWlanMacAddressLength> wlanMACAddress;
    for ( TInt i( 0 ); i < address.Length(); i++ )
        {
        // Set separator
        if( i > 0 )
            {
            wlanMACAddress.Append( KPhoneWlanSeparator );
            }
        // Set data
        TBuf<10> tmp;
        tmp.Format( KWLanMACDataFormat, address[i] );
        wlanMACAddress.Append( tmp );
        }

    // Now we need the localised text
    HBufC* wlanMacAddress = StringLoader::LoadLC(
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneWlanMacAddress ), wlanMACAddress );

    TPhoneCmdParamNote noteParam;
    noteParam.SetType( EPhoneNoteCustom );
    noteParam.SetResourceId( CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneInformationWaitNote ) );
    noteParam.SetText( *wlanMacAddress );
    noteParam.SetTone( CAknNoteDialog::EConfirmationTone );

    // Display note
    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNote, &noteParam );

    CleanupStack::PopAndDestroy( wlanMacAddress );
    }

// -----------------------------------------------------------
// CPhoneState::HandleAudioAvailableOutputChangedL
// Default handling for EPEMessageAvailableAudioOutputsChanged message
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneState::HandleAudioAvailableOutputChangedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleAudioAvailableOutputChangedL() ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );

    TPhoneCmdParamAudioAvailability outputParam;

    // Output
    const TPEAudioOutput audioOutput =
        iStateMachine->PhoneEngineInfo()->AudioOutput();

    // BT availability
    TBool btAvailable = iStateMachine->PhoneEngineInfo()->AudioOutputAvailable(
            EPEBTAudioAccessory );
    outputParam.SetBTAccAvailable( btAvailable );

    // Wired availability
    TBool wiredAvailable = iStateMachine->PhoneEngineInfo()->AudioOutputAvailable(
            EPEWiredAudioAccessory );
    outputParam.SetWiredAccAvailable( wiredAvailable );

    // BTA disconnect handler check
    if( btAvailable )
        {
        CPhoneBtaaDisconnectHandler::InstanceL()->Cancel();
        }

    // view update
    iViewCommandHandle->ExecuteCommandL( EPhoneViewActivateAudioAvailabilityUIChanges,
        &outputParam );
    }


// ---------------------------------------------------------
// CPhoneState::IsAlsLineChangePossible
// ---------------------------------------------------------
//
TBool CPhoneState::IsAlsLineChangePossible()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsAlsLineChangePossible( ) ");

    TBool isAlsLineChangePossible( ETrue );
    TSSSettingsAlsBlockingValue AlsBlockingValue( ESSSettingsAlsBlockingNotSupported );
    TInt value( 0 );

    RSSSettings ssSettings;
    TInt retValue = ssSettings.Open();

    if ( retValue == KErrNone )
        {
        ssSettings.Get( ESSSettingsAlsBlocking, value );
        ssSettings.Close();

        AlsBlockingValue = static_cast< TSSSettingsAlsBlockingValue > ( value );

        if( AlsBlockingValue == ESSSettingsAlsAlternate )
            {
            isAlsLineChangePossible = EFalse;
            }
        }

    return isAlsLineChangePossible;
    }

// ---------------------------------------------------------
// CPhoneState::ShowNumberBusyNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::ShowNumberBusyNoteL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::ShowNumberBusyNoteL( ) ");
    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );

    TInt resource( EPhoneNumberBusy );

    if( iCustomization )
        {
        // Get customized text resource for busy note
        resource = iCustomization->CustomizeBusyNoteText();
        }

    // Show number busy note
    TPhoneCmdParamGlobalNote globalNoteParam;
    globalNoteParam.SetType( EAknGlobalInformationNote );
    globalNoteParam.SetTextResourceId(
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resource ) );
    globalNoteParam.SetTone( EAvkonSIDInformationTone );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowGlobalNote,
        &globalNoteParam );
    }

// ---------------------------------------------------------
// CPhoneState::IsAutoLockOn
// ---------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsAutoLockOn() const
    {
    // Check if phone is locked
    const TBool phoneIsLocked =
                CPhonePubSubProxy::Instance()->Value(
                KPSUidCoreApplicationUIs,
                KCoreAppUIsAutolockStatus ) > EAutolockOff;

    return phoneIsLocked;
    }

// ---------------------------------------------------------
// CPhoneState::IsKeyLockOn
// ---------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsKeyLockOn() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsKeyLockOn( ) ");
    TPhoneCmdParamBoolean keyLockStatus;
    keyLockStatus.SetBoolean( EFalse );

    iViewCommandHandle->ExecuteCommand(
            EPhoneViewGetKeyLockStatus,
            &keyLockStatus );

    __PHONELOG1( EBasic,
            EPhoneControl,
            "CPhoneState::IsKeyLockOn() Lock Status: %d",
            keyLockStatus.Boolean() );
    return keyLockStatus.Boolean();
    }

// ---------------------------------------------------------
// CPhoneState::CompleteSatRequestL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::CompleteSatRequestL( const TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::CompleteSatRequestL( ) ");
    iStateMachine->SetCallId( aCallId );
    iStateMachine->SendPhoneEngineMessage(
        MPEPhoneModel::EPEMessageSatCallRequestCompleted );
    }

// ---------------------------------------------------------
// CPhoneState::SetTouchPaneButtons
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SetTouchPaneButtons( TInt aResourceId )
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        TPhoneCmdParamInteger integerParam;
        integerParam.SetInteger( CPhoneMainResourceResolver::Instance()->
                                 ResolveResourceID( aResourceId ) );
        TRAP_IGNORE( iViewCommandHandle->ExecuteCommandL(
                     EPhoneViewSetTouchPaneButtons,
                     &integerParam ) );

        SetTouchPaneVisible( ETrue );
        }
    }

// ---------------------------------------------------------
// CPhoneState::DeleteTouchPaneButtons
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::DeleteTouchPaneButtons()
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        TPhoneCmdParamInteger integerParam;
        integerParam.SetInteger( 0 );
        TRAP_IGNORE( iViewCommandHandle->ExecuteCommandL(
                        EPhoneViewSetTouchPaneButtons,
                        &integerParam ) );

        SetTouchPaneVisible( EFalse );
        }
    }

// ---------------------------------------------------------
// CPhoneState::SetTouchPaneVisible
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SetTouchPaneVisible( TBool aVisible )
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( aVisible );
        TRAP_IGNORE( iViewCommandHandle->ExecuteCommandL(
                        EPhoneViewSetTouchPaneVisible,
                        &booleanParam ) );
        }
    }

// ----------------------------------------------------------------------------
// CPhoneState::SetTouchButtonEnabled
// ----------------------------------------------------------------------------
//
EXPORT_C void CPhoneState::SetTouchPaneButtonEnabled( TInt aCommandId )
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        TPhoneCmdParamInteger commandParam;
        commandParam.SetInteger( aCommandId );
        iViewCommandHandle->ExecuteCommand( EPhoneViewEnableTouchButton,
                                            &commandParam );
        }
    }

// ----------------------------------------------------------------------------
// CPhoneState::SetTouchButtonDisabled
// ----------------------------------------------------------------------------
//
EXPORT_C void CPhoneState::SetTouchPaneButtonDisabled( TInt aCommandId )
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        TPhoneCmdParamInteger commandParam;
        commandParam.SetInteger( aCommandId );

        iViewCommandHandle->ExecuteCommand( EPhoneViewDisableTouchButton,
                                            &commandParam );
        }
    }

// ---------------------------------------------------------
// CPhoneState::HandleLongHashL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleLongHashL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::HandleLongHashL() ");

    TPhoneCmdParamInteger numberEntryCountParam;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetNumberEntryCount,
            &numberEntryCountParam );
    TInt neLength( numberEntryCountParam.Integer() );

    if( neLength == 1 )
        {
         if ( !iStateMachine->SecurityMode()->IsSecurityMode() )
            {
            OnlyHashInNumberEntryL();
            }
        }
    }

// -----------------------------------------------------------
// CPhoneState::OpenVKBL
// -----------------------------------------------------------
//
void CPhoneState::OpenVkbL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::OpenVKB() ");
    iViewCommandHandle->ExecuteCommandL( EPhoneViewOpenVirtualKeyBoard );
    }

// -----------------------------------------------------------
// CPhoneState::BeginUiUpdateLC
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::BeginUiUpdateLC()
    {

    iViewCommandHandle->ExecuteCommand( EPhoneViewBeginUpdate );

    TCleanupItem operation( UiUpdateCleanup, this );
    CleanupStack::PushL( operation );
    }

// -----------------------------------------------------------
// CPhoneState::BeginTransEffectLC
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::BeginTransEffectLC(  TStateTransEffectType aType )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::BeginTransEffectLC( ) ");
    TPhoneCmdParamTransEffect effectParam;

    switch ( aType )
        {
        case ENumberEntryOpen:
            effectParam.SetType( EPhoneTransEffectDialerOpen );
            break;
        case ENumberEntryClose:
            effectParam.SetType( EPhoneTransEffectDialerClose );
            break;
        case ENumberEntryCreate:
            effectParam.SetType( EPhoneTransEffectDialerCreate );
            break;
        default:
            effectParam.SetType( EPhoneTransEffectNone );
        }

    iViewCommandHandle->ExecuteCommand( EPhoneViewBeginTransEffect,  &effectParam );

    TCleanupItem operation( EffectCleanup, this );
    CleanupStack::PushL( operation );
    }

// -----------------------------------------------------------
// CPhoneState::EndUiUpdate
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::EndUiUpdate()
    {
    CleanupStack::PopAndDestroy(); // Call UiUpdateCleanup
    }

// -----------------------------------------------------------
// CPhoneState::EndTransEffect
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::EndTransEffect()
    {
    CleanupStack::PopAndDestroy(); // Call EffectCleanup
    }

// -----------------------------------------------------------
// CPhoneState::CheckIfShowTerminationNote
// This method is intended to be overridden in states
// that contain more info about decision.
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::CheckIfShowCallTerminationNote( )
    {
    TBool show = EFalse;
    TInt callSummaryActivated = 0;
    const TInt err = CPhoneCenRepProxy::Instance()->GetInt(
        KCRUidCommonTelephonySettings,
        KSettingsSummaryAfterCall,
        callSummaryActivated );

    if ( err == KErrNone && callSummaryActivated )
        {
          show = ETrue;
        }

    return show;
    }

// -----------------------------------------------------------------------------
// CPhoneState::HandleDtmfKeyTone
// Called from KeyEventForwarder
// Allow only special characters to pass from
// keyEventForwarder to phoneEngine
// -----------------------------------------------------------------------------
//

EXPORT_C void CPhoneState::HandleDtmfKeyToneL( const TKeyEvent& aKeyEvent,
                TEventCode aEventCode )
    {
    SendDtmfKeyEventL( aKeyEvent, aEventCode );
    }

// -----------------------------------------------------------------------------
// CPhoneState::UiUpdateCleanup
// -----------------------------------------------------------------------------
//
void CPhoneState::UiUpdateCleanup(TAny* aThis )
    {
    static_cast<CPhoneState*>( aThis )->iViewCommandHandle->ExecuteCommand(
        EPhoneViewEndUpdate );
    }

// -----------------------------------------------------------------------------
// CPhoneState::EffectCleanup
// -----------------------------------------------------------------------------
//
void CPhoneState::EffectCleanup(TAny* aThis )
    {
    TPhoneCmdParamTransEffect effectParam;
    effectParam.SetType( EPhoneTransEffectStop );

    static_cast<CPhoneState*>( aThis )->iViewCommandHandle->ExecuteCommand(
        EPhoneViewEndTransEffect, &effectParam );
    }

// -----------------------------------------------------------------------------
// CPhoneState::IsDTMFEditorVisibleL
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsDTMFEditorVisibleL() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsDTMFEditorVisibleL( ) ");
    return iViewCommandHandle->HandleCommandL(
        EPhoneViewIsDTMFEditorVisible ) ==
        EPhoneViewResponseSuccess;
    }

// -----------------------------------------------------------------------------
// CPhoneState::CloseDTMFEditorL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneState::CloseDTMFEditorL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::CloseDTMFEditorL()");
    if ( iOnScreenDialer ) // Touch
        {
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( EFalse );
        // Disable dialer DTMF mode
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetDtmfDialerViewVisible,
                                             &booleanParam );

        // Closing effect is shown when DTMF dialer exist.
        BeginTransEffectLC( ENumberEntryClose );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        EndTransEffect();
        }
    else // Non-Touch
        {
        // If dtmf query is visible then remove number entry
        // because it should not be shown if user has pressed end key.
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveQuery );
        }

    // Do state-specific behaviour if number entry is cleared
    HandleNumberEntryClearedL();
    }

// -----------------------------------------------------------
// CPhoneState::SetDefaultFlagsL()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::SetDefaultFlagsL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SetDefaultFlagsL()");
    iViewCommandHandle->ExecuteCommandL( EPhoneViewHideNaviPaneAudioVolume );

    // Reset Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    // Update FSW
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateFSW );

    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetEikonNotifiersDisabled,
        &globalNotifierParam );

    // uncapture App and Camera keys if not security mode
    if ( !iStateMachine->SecurityMode()->IsSecurityMode() )
        {
        CaptureKeysDuringCallNotificationL( EFalse );
        }
    
    CPhonePubSubProxy::Instance()->ChangePropertyValue(
                    KPSUidScreenSaver,
                    KScreenSaverAllowScreenSaver,
                    EPhoneScreensaverAllowed );
    }

// -----------------------------------------------------------
// CPhoneState::GetVolumeLevel
// -----------------------------------------------------------
//
TInt CPhoneState::GetVolumeLevel()
    {
    TPhoneCmdParamInteger integerParam;

    iViewCommandHandle->ExecuteCommand( EPhoneViewGetAudioVolumeLevel,
                                        &integerParam );
    return integerParam.Integer();
    }

// -----------------------------------------------------------
// CPhoneState::ShowVideoCallOutOfMemoryNoteL
// -----------------------------------------------------------
//
void CPhoneState::ShowVideoCallOutOfMemoryNoteL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneState::ShowVideoCallOutOfMemoryNoteL()" );

    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );

    // Bring Phone app in the foreground
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );

    // Set Phone as the top application
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
        &uidParam );

    // Display error note
    SExtendedError ext;
    ext.iComponent = KUidPhoneApplication;
    ext.iErrorNumber = KErrNoMemory;
    ext.iInformation = EFalse;
    TBuf<1> ignore;
    iAvkonAppUi->HandleError( KErrNoMemory, ext, ignore, ignore );
    }

// -----------------------------------------------------------------------------
// CPhoneState::SimSecurityStatus
// -----------------------------------------------------------------------------
//
TInt CPhoneState::SimSecurityStatus() const
    {
    /*
    Sim security statuses:

    ESimSecurityStatusUninitialized = KStartupEnumerationFirstValue,
    ESimRejected,   // The PUK code has been entered incorrectly, so the card is rejected.
    ESimUnaccepted  // The SIM lock is on, so the card is unaccepted.
    ESimInvalid     // The Sim inserted is not same as the one provided by the
                    // operator, so card is invalid.
    */
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SimSecurityStatus()" );
    return CPhonePubSubProxy::Instance()->Value(
                    KPSUidStartup, KStartupSimSecurityStatus );
    }

// -----------------------------------------------------------------------------
// CPhoneState::CallWaitingNoteL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneState::CallWaitingNoteL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::CallWaitingNoteL()" );

    TBuf< KPhoneContactNameMaxLength > callText( KNullDesC );

    // Set CLI text for the call header
    const TBool contactInfoAvailable =
        iStateMachine->PhoneEngineInfo()->RemoteName( aCallId ).Length() ||
        iStateMachine->PhoneEngineInfo()->RemoteCompanyName( aCallId ).Length();

    __PHONELOG1( EBasic, EPhoneControl,
        "CPhoneState::CallWaitingNoteL - contactInfoAvailable(%d)", contactInfoAvailable );

    __PHONELOG1( EBasic, EPhoneControl,
        "CPhoneState::CallWaitingNoteL - remote name(%S)",
        &iStateMachine->PhoneEngineInfo()->RemoteName( aCallId ) );

    __PHONELOG1( EBasic, EPhoneControl,
        "CPhoneState::CallWaitingNoteL - company name(%S)",
        &iStateMachine->PhoneEngineInfo()->RemoteCompanyName( aCallId ) );

    if ( contactInfoAvailable )
        {
        // Set Call Text flag to waiting note
        callText = iStateMachine->PhoneEngineInfo()->RemoteName( aCallId );

        // if no remote name available then set company name
        if ( callText == KNullDesC )
            {
            callText = iStateMachine->PhoneEngineInfo()->RemoteCompanyName( aCallId );
            }

        TPhoneCmdParamGlobalNote globalNoteParam;
        globalNoteParam.SetText( callText );
        globalNoteParam.SetType( EAknGlobalInformationNote );
        globalNoteParam.SetTone( EAvkonSIDInformationTone );
        globalNoteParam.SetTextResourceId(
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCallWaitingWithLabel ) );

        iViewCommandHandle->ExecuteCommandL(
                EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    else
        {
        SendGlobalInfoNoteL( EPhoneCallWaitingWithoutLabel );
        }
    }

// ----------------------------------------------------------------------------
// CPhoneState::SetRingingTonePlaybackL()
// ----------------------------------------------------------------------------
//
EXPORT_C void CPhoneState::SetRingingTonePlaybackL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneState::SetRingingTonePlaybackL()" );

    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume(
        iStateMachine->PhoneEngineInfo()->RingingVolume() );

    TArray< TContactItemId > alertGroups =
        iStateMachine->PhoneEngineInfo()->AlertForGroup();

    TInt alertGroupCount = alertGroups.Count();
    TInt contactGroupCount =
        iStateMachine->PhoneEngineInfo()->ContactGroups( aCallId ).Count();
    TBool alertingGroupFound( EFalse );

    // Check contents of both the alerting groups array and call contact group
    // array, if any of the group ids match then allow playing.
    if ( alertGroupCount > 0 )
        {
        for ( TInt i = 0 ; i < alertGroupCount ; i++ )
            {
            for ( TInt j = 0 ; j < contactGroupCount ; j++ )
                {
                if ( alertGroups[ i ] ==
                     ( iStateMachine->PhoneEngineInfo()->ContactGroups(
                           aCallId ) )[ j ] )
                    {
                    alertingGroupFound = ETrue;
                    }
                }
            }
        if ( alertingGroupFound == EFalse )
            {
            ringToneParam.SetRingingType( EProfileRingingTypeSilent );
            }
        else
            {
            ringToneParam.SetRingingType(
                iStateMachine->PhoneEngineInfo()->RingingType() );
            }
        }
    else // no alerting groups set
        {
        ringToneParam.SetRingingType(
            iStateMachine->PhoneEngineInfo()->RingingType() );
        }

    // Set ring tone
    if ( iStateMachine->PhoneEngineInfo()->PersonalRingingTone(
             aCallId ).Length() )
        {
        // Set the personal ring tone
        ringToneParam.SetRingTone(
            iStateMachine->PhoneEngineInfo()->PersonalRingingTone( aCallId ) );
        ringToneParam.SetType( EPhoneRingTonePersonal );
        }
    else if ( iStateMachine->PhoneEngineInfo()->RingingTone(
                  aCallId ).Length() )
        {
        // Set the profile ring tone
        ringToneParam.SetRingTone(
            iStateMachine->PhoneEngineInfo()->RingingTone( aCallId ) );
        ringToneParam.SetType( EPhoneRingToneProfile );

        if ( ( iStateMachine->PhoneEngineInfo()->CallerImage( aCallId ).Length() > 0 &&
                BaflUtils::FileExists( CCoeEnv::Static()->FsSession(),
                        iStateMachine->PhoneEngineInfo()->CallerImage( aCallId ) ) ) ||
               iStateMachine->PhoneEngineInfo()->HasCallerThumbnail( aCallId ) )
            {
            // Caller image set
            ringToneParam.SetCallerImageStatus( ETrue );
            }
        }

    // Set text to say, if enabled
    if ( iStateMachine->PhoneEngineInfo()->TextToSpeech() )
        {
        ringToneParam.SetTextToSay(
            iStateMachine->PhoneEngineInfo()->RemoteTextToSpeechText(
                aCallId ) );
        }

    if ( iStateMachine->PhoneEngineInfo()->CallerText( aCallId ).Length() > 0 )
        {
        ringToneParam.SetCallerTextStatus( ETrue );
        }

    // Play the ring tone
    iViewCommandHandle->ExecuteCommandL( EPhoneViewPlayRingTone, &ringToneParam );
    }

// -----------------------------------------------------------
// CPhoneState::HandleCugNoteL
// -----------------------------------------------------------
//
void CPhoneState::HandleCugInUseNoteL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleCugInUseNoteL() ");

    TInt cugIndex( 0 );

    RSSSettings ssSettings;
    TInt retValue = ssSettings.Open();

    if ( retValue == KErrNone )
        {
        ssSettings.Get( ESSSettingsCug, cugIndex  );
        }
    ssSettings.Close();

    if ( cugIndex )
        {
        // Add it to the resource string
        HBufC* buf = StringLoader::LoadLC(
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID(
            EPhoneInfoCugInUse ), cugIndex );

        TPhoneCmdParamGlobalNote globalNoteParam;
	    globalNoteParam.SetText( *buf );
	    globalNoteParam.SetType( EAknGlobalInformationNote );
	    globalNoteParam.SetTone( EAvkonSIDInformationTone );

	    iViewCommandHandle->ExecuteCommandL(
	            EPhoneViewShowGlobalNote, &globalNoteParam );

	    CleanupStack::PopAndDestroy( buf );
        }
    }

// -----------------------------------------------------------
// CPhoneState::CallheaderManagerL
// -----------------------------------------------------------
//
CPhoneCallHeaderManager* CPhoneState::CallheaderManagerL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::CallheaderManagerL() ");
    if ( !iCallHeaderManager )
        {
        iCallHeaderManager = CPhoneCallHeaderManager::NewL(
                *iViewCommandHandle,
                *iStateMachine,
                iCustomization );
        }
    return iCallHeaderManager;
    }

// -----------------------------------------------------------------------------
// CPhoneState::CloseCustomizedDialerL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneState::CloseCustomizedDialerL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::CloseCustomizedDialerL( ) ");

    // Set dialer back to default mode.
    iViewCommandHandle->HandleCommandL( EPhoneViewHideCustomizedDialer );

    // Closing effect is shown when customized dialer exist.
    BeginTransEffectLC( ENumberEntryClose );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );

    EndTransEffect();
    // Do state-specific behaviour if number entry is cleared
    HandleNumberEntryClearedL();
    }

// -----------------------------------------------------------------------------
// CPhoneState::CustomizedDialerMenuResourceId
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPhoneState::CustomizedDialerMenuResourceIdL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneState::CustomizedDialerMenuResourceId( ) ");
    // Get and return customized dialer menu resource id
    TPhoneCmdParamInteger integerParam;
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCustomizedDialerMenuResourceId,
        &integerParam );
    return integerParam.Integer();
    }

// -----------------------------------------------------------------------------
// CPhoneState::CustomizedDialerCbaResourceId
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPhoneState::CustomizedDialerCbaResourceIdL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,
           "CPhoneState::CustomizedDialerCbaResourceId( ) ");
    // Get and return customized dialer CBA resource id
    TPhoneCmdParamInteger integerParam;
    iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCustomizedDialerCbaResourceId,
        &integerParam );
    return integerParam.Integer();
    }

// -----------------------------------------------------------
// CPhoneState::ShowDtmfDialerL
// -----------------------------------------------------------
//
void CPhoneState::ShowDtmfDialerL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneState::ShowDtmfDialerL()" );

    // Set dialer to DTMF mode.
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetDtmfDialerViewVisible,
                                         &booleanParam );

    BeginTransEffectLC( ENumberEntryCreate );

    if ( IsNumberEntryUsedL() )
        {
        // Store the number entry content to cache
        if ( !IsNumberEntryContentStored() )
            {
            StoreNumberEntryContentL();
            }
        // Clear and display DTMF dialer
        NumberEntryClearL();
        SetNumberEntryVisibilityL(ETrue);
        }
    else
        {
        // Create and display DTMF dialer
        NumberEntryManagerL()->CreateNumberEntryL();
        }

    EndTransEffect();

    // Update CBA
    iCbaManager->UpdateInCallCbaL();
    }

// -----------------------------------------------------------
// CPhoneState::CheckIfRestoreNEContentAfterDtmfDialer
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::CheckIfRestoreNEContentAfterDtmfDialer()
    {
    if ( IsOnScreenDialerSupported() )
        {
        TBool isDialerVisible( EFalse );
        TRAP_IGNORE( isDialerVisible = IsDTMFEditorVisibleL() );

         if ( isDialerVisible )
             {
             TRAP_IGNORE( CloseDTMFEditorL() );
             }

         // if the DTMF dialer is used before the idle message, we have to
         // restore the original content of the number entry
         if ( IsNumberEntryContentStored() )
             {
             TBool isNumberEntryUsed( EFalse );
             TRAP_IGNORE( isNumberEntryUsed = IsNumberEntryUsedL() );

             if ( !isNumberEntryUsed )
                 {
                 TRAP_IGNORE( NumberEntryManagerL()->CreateNumberEntryL() );
                 }
             // Restore the number entry content from cache
             TRAP_IGNORE( RestoreNumberEntryContentL() );
             }
         }
    }

// -----------------------------------------------------------
// CPhoneState::IsOnScreenDialerSupported
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsOnScreenDialerSupported() const
    {
    return iOnScreenDialer;
    }

// ---------------------------------------------------------------------------
// CPhoneState::LoadResource
// ---------------------------------------------------------------------------
//
void CPhoneState::LoadResource( TDes& aData, const TInt aResource ) const
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::LoadResource() ");
    __PHONELOG1( EBasic, EPhoneControl, "CPhoneState::LoadResource - aResource: %d", aResource );
    StringLoader::Load(  aData,
                         CPhoneMainResourceResolver::Instance()->ResolveResourceID( aResource ),
                         &iEnv  );
    }

// -----------------------------------------------------------
// CPhoneState::IsAnyConnectedCalls
// -----------------------------------------------------------
//
TBool CPhoneState::IsAnyConnectedCalls()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::IsAnyConnectedCalls() ");
    TBool connectedCall(EFalse);
    if ( iStateMachine->PhoneEngineInfo()->CheckIfCallStateExists( EPEStateConnected ) ||
        iStateMachine->PhoneEngineInfo()->CheckIfCallStateExists( EPEStateConnecting ) ||
        iStateMachine->PhoneEngineInfo()->CheckIfCallStateExists( EPEStateConnectedConference ) )
        {
        connectedCall = ETrue;
        }
    __PHONELOG1( EBasic, EPhoneControl, "CPhoneState::IsAnyConnectedCalls: (%d)", connectedCall );
    return connectedCall;
    }

// -----------------------------------------------------------
// CPhoneState::NumberEntryContentL
// -----------------------------------------------------------
//
CPhoneNumberEntryManager* CPhoneState::NumberEntryManagerL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::NumberEntryContentL() ");
    if ( !iNumberEntryManager )
        {
        iNumberEntryManager = CPhoneNumberEntryManager::NewL(
                this,
                *iViewCommandHandle,
                *iStateMachine,
                iCustomization,
                *iCbaManager );
        }
    return iNumberEntryManager;
    }

// -----------------------------------------------------------
// CPhoneState::PhoneNumberFromEntryLC()
// -----------------------------------------------------------
//
EXPORT_C HBufC* CPhoneState::PhoneNumberFromEntryLC()
    {
    return NumberEntryManagerL()->PhoneNumberFromEntryLC();
    }

// -----------------------------------------------------------
// CPhoneState::IsNumberEntryUsedL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsNumberEntryUsedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsNumberEntryUsedL( ) ");
    return NumberEntryManagerL()->IsNumberEntryUsedL();
    }

// -----------------------------------------------------------
// CPhoneState::IsNumberEntryVisibleL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsNumberEntryVisibleL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::IsNumberEntryVisibleL( ) ");
    return NumberEntryManagerL()->IsNumberEntryVisibleL();
    }

// -----------------------------------------------------------
// CPhoneState::KeyEventForExistingNumberEntryL
// -----------------------------------------------------------
//
void CPhoneState::KeyEventForExistingNumberEntryL( const TKeyEvent& aKeyEvent,
    TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::KeyEventForExistingNumberEntryL( ) ");
    NumberEntryManagerL()->KeyEventForExistingNumberEntryL( aKeyEvent, aEventCode );
    }

// -----------------------------------------------------------
// CPhoneState::HandleNumberEntryClearedL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleNumberEntryClearedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleNumberEntryClearedL( ) ");
    NumberEntryManagerL()->HandleNumberEntryClearedL();
    }

// -----------------------------------------------------------
// CPhoneState::SetNumberEntryVisibilityL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::SetNumberEntryVisibilityL( const TBool aVisible )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::SetNumberEntryVisibilityL( ) ");
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( ETrue );
    if ( !aVisible )
        {
        booleanParam.SetBoolean( EFalse );
        }
    NumberEntryManagerL()->SetNumberEntryVisibilityL( booleanParam );
    }

// ---------------------------------------------------------
// CPhoneState::HandleCreateNumberEntryL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleCreateNumberEntryL(
        const TKeyEvent& /*aKeyEvent*/,
        TEventCode /*aEventCode*/ )
    {
    NumberEntryManagerL()->HandleCreateNumberEntryL();
    }

// -----------------------------------------------------------
// CPhoneState::IsNumberEntryContentStored
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsNumberEntryContentStored()
    {
    TBool isNumberEntryContentStored( EFalse );
    TRAP_IGNORE( isNumberEntryContentStored = NumberEntryManagerL()->IsNumberEntryContentStored() );
    return isNumberEntryContentStored;
    }

// -----------------------------------------------------------
// CPhoneState::StoreNumberEntryContentL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::StoreNumberEntryContentL()
    {
    NumberEntryManagerL()->StoreNumberEntryContentL();
    }

// -----------------------------------------------------------
// CPhoneState::RestoreNumberEntryContentL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::RestoreNumberEntryContentL()
    {
    NumberEntryManagerL()->RestoreNumberEntryContentL();
    }

// -----------------------------------------------------------
// CPhoneState::ClearNumberEntryContentCache
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::ClearNumberEntryContentCache()
    {
    TRAP_IGNORE( NumberEntryManagerL()->ClearNumberEntryContentCache() );
    }

// -----------------------------------------------------------
// CPhoneState::HandleNumberEntryEdited
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleNumberEntryEdited()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::HandleNumberEntryEdited() ");
    TRAP_IGNORE( NumberEntryManagerL()->HandleNumberEntryEdited() );
    }

// -----------------------------------------------------------------------------
// CPhoneState::IsAlphanumericSupportedAndCharInput
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneState::IsAlphanumericSupportedAndCharInput(
        const TKeyEvent& aKeyEvent )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::IsAlphanumericSupportedAndCharInput() ");
    TBool alphaNumericSupport(EFalse);
    TRAP_IGNORE( alphaNumericSupport = NumberEntryManagerL()->IsAlphanumericSupportedAndCharInput( aKeyEvent ) );
    return alphaNumericSupport;
    }

// ---------------------------------------------------------
// CPhoneState::OnlyHashInNumberEntryL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::OnlyHashInNumberEntryL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::OnlyHashInNumberEntryL( ) ");
    // 0.8 seconds has passed, start ALS line change timer
    StartAlsLineChangeTimerL();
    }

// -----------------------------------------------------------
// CPhoneState::NumberEntryClearL
// -----------------------------------------------------------
//
void CPhoneState::NumberEntryClearL()
    {
    NumberEntryManagerL()->NumberEntryClearL();
    }
	
// -----------------------------------------------------------
// CPhoneState::UpdateSilenceButtonDimming
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::UpdateSilenceButtonDimming()
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        const TProfileRingingType ringingType =
            iStateMachine->PhoneEngineInfo()->RingingType();
        
        TBool callIsAlerting =
                !(( ringingType == EProfileRingingTypeSilent ) ||
                  ( ringingType == EProfileRingingTypeBeepOnce ));

        if ( !callIsAlerting )
            {
            SetTouchPaneButtonDisabled( EPhoneCallComingCmdSilent );
            }
        }
    }
	
// ---------------------------------------------------------
// CPhoneState::SetToolbarDimming
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SetToolbarDimming( TBool aDimmed )
    {
	if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
		// Check that we are not undimming toolbar in security mode
		if ( !( !aDimmed && iStateMachine->SecurityMode()->IsSecurityMode() ) )
			{
			TPhoneCmdParamBoolean booleanParam;
			booleanParam.SetBoolean( aDimmed );
			TRAP_IGNORE( iViewCommandHandle->ExecuteCommandL(
				EPhoneViewSetToolbarDimming, &booleanParam ));
			}
        }
    }

// ---------------------------------------------------------
// CPhoneState::SetToolbarButtonLoudspeakerEnabled
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SetToolbarButtonLoudspeakerEnabled()
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        TPhoneCmdParamInteger integerParam;
        integerParam.SetInteger( EPhoneInCallCmdActivateIhf );
        TRAP_IGNORE( iViewCommandHandle->ExecuteCommandL(
            EPhoneViewEnableToolbarButton, &integerParam ));
        }
    }

// ---------------------------------------------------------
// CPhoneState::HandleEasyDialingCommandsL
// ---------------------------------------------------------
//
void CPhoneState::HandleEasyDialingCommandsL( TInt aCommandId )
    {
    switch ( aCommandId )
        {
        case EPhoneCmdUpdateCba:
            iCbaManager->UpdateInCallCbaL();
            break;
                
        case EPhoneDialerValidNumber:
            {
            TPhoneCmdParamBoolean command;
            command.SetBoolean( EFalse );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetInvalidCsPhoneNumberFlag, &command );           
            }
            break;
                
        case EPhoneDialerInvalidNumber:
            {
            TPhoneCmdParamBoolean command;
            command.SetBoolean( ETrue );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetInvalidCsPhoneNumberFlag, &command );
            }
            break;
            
        case EPhoneCmdBlockingDialogLaunched:
            {
            TPhoneCmdParamBoolean param;
            param.SetBoolean( ETrue );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetBlockingDialogStatus, &param );
            }
            break;
            
        case EPhoneCmdBlockingDialogClosed:
            {
            TPhoneCmdParamBoolean param;
            param.SetBoolean( EFalse );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetBlockingDialogStatus, &param );
            }
            break;
            
        default:
            break;
        }
    }

//  End of File

