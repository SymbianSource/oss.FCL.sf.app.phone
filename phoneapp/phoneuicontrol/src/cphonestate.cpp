/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <wlaninternalpskeys.h>
#include <btengdomainpskeys.h>
#include <btengdomaincrkeys.h>
#include <settingsinternalcrkeys.h>
#include <starterclient.h>
#include <rsssettings.h>
#include <UikonInternalPSKeys.h>
#include <telephonydomainpstypes.h>
#include <telinformationpskeys.h>
#include <coreapplicationuisdomainpskeys.h>
#include <mccecall.h>
#include <ccpdefs.h>
#include <LogsDomainCRKeys.h>
#include <spsettings.h>
#include <startupdomainpskeys.h>
#include <MediatorDomainUIDs.h>
#include <videotelcontrolmediatorapi.h>
#include <textresolver.h>
#include <phoneappvoipcommands.hrh>
#include <hwrmdomainpskeys.h>
#include <hal.h>

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
#include "tphonecmdparamkeycapture.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamquery.h"
#include "tphonecmdparamstring.h"
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
#include "tphonecmdparamsfidata.h" 
#include "mphonestorage.h"
#include "phoneconstants.h"
#include "cphoneclearblacklist.h"

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
            SetTouchPaneButtons(0);
            break;

        case MEngineMonitor::EPEMessageAvailableAudioOutputsChanged:
            HandleAudioAvailableOutputChangedL();
            if ( iStateMachine->PhoneEngineInfo()->AudioOutput() != EPENotActive )
                {
                // Go to current state implementation
                iCbaManager->UpdateInCallCbaL();
                SetTouchPaneButtons(0);
                }
            break;

        case MEngineMonitor::EPEMessageChangedCallDuration:
            HandleChangedCallDurationL( aCallId );
            break;

        case MEngineMonitor::EPEMessageRemoteBusy:
            HandleRemoteBusyL( aCallId );
            break;

        case MEngineMonitor::EPEMessageCallWaiting:
            // No need to send waiting notification for user.
            //SendGlobalInfoNoteL( EPhoneWaitingText, ETrue );
            break;

        case MEngineMonitor::EPEMessageProfileChanged:
            {
            }
            break;

        case MEngineMonitor::EPEMessageRemoteTerminated:
            /* Flow through */
        case MEngineMonitor::EPEMessageDisconnecting:
            HandleDisconnectingL( aCallId );
            break;

        case MEngineMonitor::EPEMessageTransferDone:
            SendGlobalInfoNoteL( EPhoneInCallTransferred, ETrue );
            break;

       case MEngineMonitor::EPEMessageInitiatedEmergencyCall:
            HandleInitiatedEmergencyCallL( aCallId );
            break;

       case MEngineMonitor::EPEMessageInitiatedEmergencyWhileActiveVideo:
           HandleInitiatedEmergencyWhileActiveVideoL();
           break;

       case MEngineMonitor::EPEMessageCallSecureStatusChanged:
           HandleCallSecureStatusChangeL( aCallId );

           if ( iCustomization )
               {
               iCustomization->HandlePhoneEngineMessageL( aMessage,
                   aCallId );
               }
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
    
    SetTouchPaneButtons(0);
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
                globalNoteParam.SetType( EPhoneMessageBoxInformation );

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
        && !IsAnyQueryActiveL() )
        {
        // Send the key event to the phone engine.
        SendKeyEventL( aKeyEvent, aEventCode );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneState::IsKeyEventFurtherProcessed
// -----------------------------------------------------------------------------
//
TBool CPhoneState::IsKeyEventFurtherProcessedL( const TKeyEvent& /*aKeyEvent*/ ) const
    {
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
    
    TInt ret = KErrNone;
    TInt isDisplayOn;
    
    ret = HAL::Get( HALData::EDisplayState, isDisplayOn );
    
    // Update only if the display is on or if HAL::Get returns an error, 
    // in which case display value cannot be trusted.
    if ( ret || isDisplayOn )
        {
        // Get the call duration
        TTimeIntervalSeconds seconds =
            iStateMachine->PhoneEngineInfo()->CallDuration( aCallId );

        TPhoneCmdParamInteger time;
        time.SetInteger(seconds.Int());
        iViewCommandHandle->ExecuteCommandL(EPhoneViewUpdateCallHeaderCallDuration, aCallId, &time);
        }
    }	

// -----------------------------------------------------------------------------
// CPhoneState::HandleRemoteBusyL
// -----------------------------------------------------------------------------
//
void CPhoneState::HandleRemoteBusyL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleRemoteBusyL( ) ");

    TPEErrorInfo info;
    info.iCallId = aCallId;
    info.iErrorCode = ECCPErrorBusy;
    info.iErrorType = EPECcp;
    CPhoneMainErrorMessagesHandler::Instance()->ShowErrorSpecificNoteL( info );
    
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
    }

// -----------------------------------------------------------
// CPhoneState::HandleDisconnectingL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleDisconnectingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleDisconnectingL( ) ");

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
// CPhoneState::HandleNumericKeyEventL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleNumericKeyEventL(
    const TKeyEvent& /*aKeyEvent*/,
    TEventCode /*aEventCode*/ )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleNumericKeyEventL( ) ");

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
            __PHONELOG1( EBasic, EPhoneControl, "SetPhoneNumber: %S ", phoneNumber );
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
    if ( aCategory == KPSUidTelInformation
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
        if ( aValue == ESimRejected ||  aValue == ESimUnaccepted )
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
// CPhoneState::NeedToSendToBackgroundL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneState::NeedToSendToBackgroundL() const
    {
    return iViewCommandHandle->HandleCommandL(
        EPhoneViewGetNeedToSendToBackgroundStatus ) ==
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

// <---------------------------- MENU AND CBA EVENTS ------------------------->

EXPORT_C TBool CPhoneState::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleCommandL( ) ");
    TBool commandStatus = ETrue;
    switch( aCommand )
        {
        case EPhoneEmergencyCmdExit:
            {
            }
            // this should be bypasses?
        case EPhoneDialerCallHandling:
        case EPhoneCmdBack:
            // Remove number entry from screen
            iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
            // Do state-specific behaviour if number entry is cleared
            HandleNumberEntryClearedL();
            break;

        case EPhoneDialerCmdTouchInput:
            break;

        case EPhoneNumberAcqCmdSendMessage:
            break;

        case EPhoneNumberAcqCmdSave:
            break;

        case EPhoneNumberAcqCmdAddToName:
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
            }
            break;

        case EPhoneNumberAcqSecurityDialer:
            {

            }
            break;

        case EPhoneDialerCmdContacts:
            {
            }
            break;

        case EPhoneNumberAcqCmdToggleNeAlphaMode:
        case EPhoneNumberAcqCmdToggleNeNumericMode:
            {

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
            else if ( NeedToSendToBackgroundL() )
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

        case EPhoneCmdVideoCallOutOfMemory:
            ShowVideoCallOutOfMemoryNoteL();
            DisconnectCallL();
            break;
            
        case EPhoneCallComingCmdSoftReject:
            // Open Soft reject message editor
            OpenSoftRejectMessageEditorL();
            break;
            
        case EPhoneInCallCmdContacts:
            iViewCommandHandle->ExecuteCommand( EPhoneViewOpenContacts );
            break;
            
        default:

            /*if ( IsOnScreenDialerSupported() )
                {
                // Offer command to view.
                TPhoneViewResponseId resId =
                            iViewCommandHandle->HandleCommandL( aCommand );

                if( resId == EPhoneViewResponseFailed )
                    {
                    commandStatus = EFalse;
                    }
                }
            else*/

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
        SendGlobalInfoNoteL( EPhoneNoteTTYNoAudioControl, ETrue );
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
        SendGlobalInfoNoteL( EPhoneNoteTTYNoAudioControl, ETrue );
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
EXPORT_C TBool CPhoneState::DisconnectCallL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DisconnectCallL( ) ");
    TPhoneCmdParamInteger callIdParam;
    iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewGetExpandedBubbleCallId, &callIdParam );

    TBool ret = EFalse;
    if( callIdParam.Integer() > KErrNotFound )
        {
        // Release the call
        iStateMachine->SetCallId( callIdParam.Integer() );

        if( IsVideoCall( callIdParam.Integer() ) )
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
        ret = ETrue;
        }
    else
        {
        __PHONELOG( EOnlyFatal, EPhoneControl,
            "CPhoneState::DisconnectCallL has negative call id!" );
        }

    return ret;
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
        if ( IsCustomizedDialerVisibleL() )
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

    // Get the number entry contents
    HBufC* phoneNumber = PhoneNumberFromEntryLC();

    // Call the number
    iStateMachine->PhoneEngineInfo()->SetPhoneNumber( *phoneNumber );

    if ( phoneNumber->Des().Length() < KPhoneValidPhoneNumberLength )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );

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
    // This key will be moved to some other area, but since key
    // is supported we can still use it.
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

    // Go to background if necessary
    if ( NeedToSendToBackgroundL() ||  IsAutoLockOn() )
        {
        }
    // If there is no need to send back ground and number entry is used then
    // we must show number entry.
    else if ( !NeedToSendToBackgroundL() && IsNumberEntryUsedL() )
        {
        SetNumberEntryVisibilityL(ETrue);
        }
    }

// -----------------------------------------------------------
// CPhoneState::CaptureKeysDuringCallNotificationL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::CaptureKeysDuringCallNotificationL(
    TBool /*aCaptured*/ )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::CaptureKeysDuringCallNotificationL( ) ");
    // Determine which view command to execute based on the capture status
    /*const TInt viewCommandId = aCaptured ?
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
        }*/
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
    TInt aCallHeaderType )
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
        globalNoteParam.SetType( EPhoneMessageBoxInformation );
        globalNoteParam.SetText( noteText );

        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    }

// ---------------------------------------------------------
// CPhoneState::SendGlobalInfoNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SendGlobalInfoNoteL( 
        TInt aResourceId, TBool aNotificationDialog )
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
        PhoneNotificationType type = aNotificationDialog ? 
            EPhoneNotificationDialog : EPhoneMessageBoxInformation;
        globalNoteParam.SetType( type );
        globalNoteParam.SetTextResourceId(
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( aResourceId ) );

        globalNoteParam.SetNotificationDialog( aNotificationDialog );
        
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    }

// ---------------------------------------------------------
//  CPhoneUIController::SendGlobalWarningNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SendGlobalWarningNoteL( 
        TInt aResourceId, TBool aNotificationDialog )
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
        PhoneNotificationType type = aNotificationDialog ? 
                    EPhoneNotificationDialog : EPhoneMessageBoxWarning;
        globalNoteParam.SetType( type );
        globalNoteParam.SetTextResourceId(
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( aResourceId ) );
  
        globalNoteParam.SetNotificationDialog( aNotificationDialog );
        
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    }

// ---------------------------------------------------------
//  CPhoneUIController::SendGlobalErrorNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SendGlobalErrorNoteL( 
        TInt aResourceId, TBool aNotificationDialog )
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
        PhoneNotificationType type = aNotificationDialog ? 
                    EPhoneNotificationDialog : EPhoneMessageBoxInformation;
        globalNoteParam.SetType( type );
        
        globalNoteParam.SetTextResourceId(
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( aResourceId ) );
        globalNoteParam.SetNotificationDialog( aNotificationDialog );

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
        SendGlobalErrorNoteL( EPhoneNoteTextNotAllowed, ETrue );
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
        SendGlobalErrorNoteL( EPhoneNoteTextNotAllowed, ETrue );
        }
    CleanupStack::PopAndDestroy( bt );
    }

// <-------------------------- INTERNAL FUNCTIONS ------------------------>

// -----------------------------------------------------------
// CPhoneState::UpdateProfileDisplayL
// -----------------------------------------------------------
//
void CPhoneState::UpdateProfileDisplayL()
    {
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
    TPhoneCmdParamBoolean isSecurityMode;
    TRAP_IGNORE( iViewCommandHandle->ExecuteCommandL( EPhoneViewGetSecurityModeStatus, &isSecurityMode ) );
    if ( SimState() == EPESimNotPresent && isSecurityMode.Boolean() )
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

    // Set security mode on.
    TPhoneCmdParamBoolean securityMode;
    securityMode.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetSecurityMode, &securityMode );

    // Remove number entry from screen
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
    
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

    if ( SimSecurityStatus() == ESimRejected )
        {
        resourceId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneSimRejected );
        }
    else if ( SimState() == EPESimNotPresent )
        {
        // insert sim card -note
        resourceId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneSimRemoved );
        }
    else if ( SimSecurityStatus() == ESimUnaccepted )
        {
        resourceId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneSimUnaccepted );
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

    // Show number busy notification
    TPhoneCmdParamGlobalNote globalNoteParam;
    globalNoteParam.SetType( EPhoneNotificationDialog );
    globalNoteParam.SetTextResourceId(
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resource ) );
    
    globalNoteParam.SetNotificationDialog( ETrue );
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

    __PHONELOG1( EBasic,
            EPhoneControl,
            "CPhoneState::IsAutoLockOn() Status: %d",
            phoneIsLocked );
    
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
EXPORT_C void CPhoneState::SetTouchPaneButtons( TInt /*aResourceId*/ )
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        TPhoneCmdParamBoolean muteParam;
        muteParam.SetBoolean( iStateMachine->PhoneEngineInfo()->AudioMute() );
        iViewCommandHandle->ExecuteCommand(EPhoneViewSetMuteFlag,&muteParam);
        
        const TPEAudioOutput audioOutput =
            iStateMachine->PhoneEngineInfo()->AudioOutput();

        TBool btAvailable = iStateMachine->PhoneEngineInfo()->AudioOutputAvailable(
            EPEBTAudioAccessory );
        
        TPhoneCmdParamBoolean btParam;
        btParam.SetBoolean( audioOutput == EPEBTAudioAccessory );        
        iViewCommandHandle->ExecuteCommand(EPhoneViewSetBlueToothFlag,&btParam);

        TPhoneCmdParamBoolean btAvailableParam;
        btAvailableParam.SetBoolean( btAvailable );        
        iViewCommandHandle->ExecuteCommand(
                EPhoneViewSetBluetoothAvailableFlag,&btAvailableParam);
               
        TBool emergency( EPEStateIdle != 
            iStateMachine->PhoneEngineInfo()->CallState( KPEEmergencyCallId ) );
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( emergency );

        TRAP_IGNORE( iViewCommandHandle->ExecuteCommandL(
                     EPhoneViewSetTouchPaneButtons,
                     &booleanParam ) );
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
        TPhoneCmdParamBoolean boolParam;
        TRAP_IGNORE( iViewCommandHandle->ExecuteCommandL(
                        EPhoneViewSetTouchPaneButtons,
                        &boolParam ) );
        }
    }

// ---------------------------------------------------------
// CPhoneState::HandleLongHashL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleLongHashL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneState::HandleLongHashL() ");

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
// CPhoneState::EndUiUpdate
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::EndUiUpdate()
    {
    CleanupStack::PopAndDestroy(); // Call UiUpdateCleanup
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
    TPhoneCmdParamBoolean isSecurityMode;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetSecurityModeStatus, &isSecurityMode );
    if ( !isSecurityMode.Boolean() )
        {
        CaptureKeysDuringCallNotificationL( EFalse );
        }

    // Restore keylock if phone has been locked before call.
    if ( iStateMachine->PhoneStorage()->NeedToEnableKeylock() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewEnableKeyLock );
        iStateMachine->PhoneStorage()->SetNeedToEnableKeylock( EFalse );
        }
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
        globalNoteParam.SetType( EPhoneNotificationDialog );

        globalNoteParam.SetTextResourceId(
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCallWaitingWithLabel ) );
        globalNoteParam.SetNotificationDialog( ETrue );
        
        iViewCommandHandle->ExecuteCommandL(
                EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    else
        {
        SendGlobalInfoNoteL( EPhoneCallWaitingWithoutLabel, ETrue );
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
        globalNoteParam.SetType( EPhoneMessageBoxInformation );
   
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

// -----------------------------------------------------------
// CPhoneState::CheckIfRestoreNEContentAfterDtmfDialer
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::CheckIfRestoreNEContentAfterDtmfDialer()
    {

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
// CPhoneState::ClearNumberEntryContentCache
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::ClearNumberEntryContentCache()
    {
    TRAP_IGNORE( NumberEntryManagerL()->ClearNumberEntryContentCache() );
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
// CPhoneState::GetBlockedKeyList
// -----------------------------------------------------------
//
EXPORT_C const RArray<TInt>& CPhoneState::GetBlockedKeyList() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::GetBlockedKeyList( ) ");
    return iStateMachine->PhoneStorage()->GetBlockedKeyList();
    }

// -----------------------------------------------------------
// CPhoneState::DisableHWKeysL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DisableHWKeysL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DisableHWKeysL( ) ");

    if( iStateMachine->PhoneStorage()->IsBlockedKeysListEmpty() )
        {
        __PHONELOG( EBasic, EPhoneControl, " CPhoneState::DisableHWKeysL HW Keys Disabled " );

        iStateMachine->PhoneStorage()->AppendBlockedKeysListL( EStdKeyNo );
        iStateMachine->PhoneStorage()->AppendBlockedKeysListL( EStdKeyYes );
        }
    }

// -----------------------------------------------------------
// CPhoneState::DisableCallUIL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::DisableCallUIL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::DisableCallUIL( ) ");

    // Set Call UI state to storage
    if( !iStateMachine->PhoneStorage()->IsScreenLocked() )
        {
        __PHONELOG( EBasic, EPhoneControl, " CPhoneState::DisableCallUIL CallUI Disabled " );

        // Show keys locked note
        TPhoneCmdParamNote noteParam;
        noteParam.SetType( EPhoneNoteUIDisabled );
        noteParam.SetResourceId( CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneInformationNote ) );
    
        HBufC* noteText = StringLoader::LoadLC( 
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneIncomingCallKeysLocked ) );
        noteParam.SetText( *noteText );
    
        iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNote, &noteParam );
        CleanupStack::PopAndDestroy( noteText );
    
        // Start capturing the key guard key
        TPhoneCmdParamKeyCapture appKeyCaptureParam;
        appKeyCaptureParam.SetKey( EStdKeyDevice7 );
        appKeyCaptureParam.SetKeyCode( EKeyDeviceF );
        appKeyCaptureParam.SetCaptureType( EPhoneKeyAllEvents );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewStartCapturingKey, &appKeyCaptureParam );
        
        // Set Call UI state to storage
        iStateMachine->PhoneStorage()->SetScreenLocked( ETrue );
        }
    }

// -----------------------------------------------------------
// CPhoneState::EnableCallUIL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::EnableCallUIL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::EnableCallUIL( ) ");

    if( iStateMachine->PhoneStorage()->IsScreenLocked() )
        {
        __PHONELOG( EBasic, EPhoneControl, " CPhoneState::EnableCallUIL CallUI Enabled " );

        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewDisableKeyLockWithoutNote );

        // Stop capturing the key guard key
        TPhoneCmdParamKeyCapture appKeyCaptureParam;
        appKeyCaptureParam.SetKey( EStdKeyDevice7 );
        appKeyCaptureParam.SetKeyCode( EKeyDeviceF );
        appKeyCaptureParam.SetCaptureType( EPhoneKeyAllEvents );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewStopCapturingKey, &appKeyCaptureParam );
        
        iStateMachine->PhoneStorage()->SetScreenLocked( EFalse );
        }
    }

// -----------------------------------------------------------
// CPhoneState::CheckDisableHWKeysAndCallUIL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::CheckDisableHWKeysAndCallUIL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneState::CheckDisableHWKeysAndCallUIL( ) ");

    if( IsKeyLockOn() || IsAutoLockOn() )
        {
        // Disable HW keys if needed
        if( CPhoneCenRepProxy::Instance()->
                IsTelephonyFeatureSupported( KTelephonyLVFlagDisableCallControlHardKeysWhileLocked ) )
            {
            DisableHWKeysL();
            }

        // Disable Call UI if needed
        if( FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke )
                && !CPhoneCenRepProxy::Instance()->
                IsTelephonyFeatureSupported( KTelephonyLVFlagAllowUnlockOnIncoming ) )
            {
            DisableCallUIL();
            iStateMachine->PhoneStorage()->SetNeedToEnableKeylock( ETrue );
            }
        }
    }

// -----------------------------------------------------------
// CPhoneState::HandleHoldSwitchL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleHoldSwitchL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneState::HandleHoldSwitchL( ) ");

    if( CPhoneCenRepProxy::Instance()->
            IsTelephonyFeatureSupported( KTelephonyLVFlagDisableCallControlHardKeysWhileLocked ) )
        {
        if( iStateMachine->PhoneStorage()->IsBlockedKeysListEmpty() )
            {
            // Disable HW Keys if needed
            DisableHWKeysL();
            }
        else
            {
            // Reset blocked keys list
            iStateMachine->PhoneStorage()->ResetBlockedKeysList();
            }
        }
    
    if( FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke ) 
            && !CPhoneCenRepProxy::Instance()->
            IsTelephonyFeatureSupported( KTelephonyLVFlagAllowUnlockOnIncoming ) )
        {
        if( iStateMachine->PhoneStorage()->IsScreenLocked() )
            {
            // Enable Call
            EnableCallUIL();
            }
        else
            {
            // Set keylock enabled
            iViewCommandHandle->ExecuteCommandL( EPhoneViewEnableKeyLockWithoutNote );
            // Disable Call
            DisableCallUIL();
            }
        }
        
        // Stop ringingtone
        iViewCommandHandle->ExecuteCommandL( EPhoneViewStopRingTone );
    }

// -----------------------------------------------------------
// CPhoneState::RetainPreviousKeylockStateL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::RetainPreviousKeylockStateL()
    {
    // Lock keypad, if keylock was enabled before incoming call.
    // Also reset PhoneStorage keylock status.
    if ( FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke ) &&
        iStateMachine->PhoneStorage()->NeedToEnableKeylock() )
        {
        iStateMachine->PhoneStorage()->SetNeedToEnableKeylock( EFalse );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewEnableKeyLockWithoutNote ); 
        }
    }

// ---------------------------------------------------------
// CPhoneState::HandleKeyLockEnabledL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::HandleKeyLockEnabled( TBool aKeylockEnabled )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneState::HandleKeyLockEnabledL( ) ");
    if( !FeatureManager::FeatureSupported( KFeatureIdFfTouchUnlockStroke )
        && CPhoneCenRepProxy::Instance()->
            IsTelephonyFeatureSupported( KTelephonyLVFlagDisableCallControlHardKeysWhileLocked ) )
        {
        if( !aKeylockEnabled )
            {
            // Keylock disabled
            // Reset blocked keys list
            iStateMachine->PhoneStorage()->ResetBlockedKeysList();
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
// CPhoneState::SetBackButtonActive
// ---------------------------------------------------------
//
EXPORT_C void CPhoneState::SetBackButtonActive( TBool aActive )
    {
    if(IsAutoLockOn() && aActive) {
        // keep back button dimmed device lock case
        return;
    }
    
    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( aActive );
        iViewCommandHandle->ExecuteCommand(
            EPhoneViewBackButtonActive, &booleanParam );
        }
    }

// -----------------------------------------------------------
// CPhoneState::OpenSoftRejectMessageEditorL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneState::OpenSoftRejectMessageEditorL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,
        "CPhoneState::OpenSoftRejectMessageEditorL ()" );

    // Fetch incoming call's id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );

    TInt ringingCallId( KErrNotFound );
    // Do nothing if negative incoming call id
    if( callStateData.CallId() > KErrNotFound  )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveQuery );
        
        ringingCallId = callStateData.CallId();
        }
    
    
    TPhoneCmdParamSfiData sfiDataParam;

    if (KErrNotFound != ringingCallId) 
        {
        if ( iStateMachine->PhoneEngineInfo()->RemoteName( ringingCallId ).Length() )
            {
            // store both the name and the number
            sfiDataParam.SetNumber( iStateMachine->PhoneEngineInfo()->RemotePhoneNumber( ringingCallId ) );
            sfiDataParam.SetName( iStateMachine->PhoneEngineInfo()->RemoteName( ringingCallId ) );
            }
        else
            {
            // store the number
            sfiDataParam.SetNumber( iStateMachine->PhoneEngineInfo()->RemotePhoneNumber( ringingCallId ) );
            }
        }

    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewOpenSoftRejectEditor, &sfiDataParam );
    }

//  End of File

