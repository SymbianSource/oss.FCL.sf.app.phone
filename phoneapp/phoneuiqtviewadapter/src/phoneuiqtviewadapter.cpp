/*!
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Phone UI's symbian adapter for Qt view.
*
*/

#include "phoneuiqtviewadapter.h"
#include "phoneuiqtviewif.h"
#include "phonebubblewrapper.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamaudioavailability.h"
#include "tphonecmdparamaudiooutput.h"
#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamemergencycallheaderdata.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamkeycapture.h"
#include "cphonepubsubproxy.h"
#include "pevirtualengine.h"
#include "cphoneringingtonecontroller.h"
#include "phoneresourceadapter.h"
#include "phoneui.hrh"
#include "cphonemediatorfactory.h"
#include "phoneuiqtbuttonscontroller.h"
#include "phoneconstants.h"
#include "phonenotecontroller.h"
#include "telephonyservice.h"
#include "phoneuicommandcontroller.h"
#include "phonemessagecontroller.h"
#include "phoneindicatorcontroller.h"
#include "qtphonelog.h"
#include "phonevisibilityhandler.h"
#include "phoneapplauncher.h"
#include "cphonecenrepproxy.h"

#include <UikonInternalPSKeys.h>
#include <bubblemanagerif.h>
#include <hbaction.h>
#include <pevirtualengine.h>
#include <QtGlobal>
#include <xqserviceutil.h>
#include <hbinstance.h>
#include <QKeyEvent>
#include <hbextendedlocale.h>
#include <hbi18ndef.h>
#include <eikenv.h>
#include <w32std.h>
#include <hbstringutil.h>
#include <AknSgcc.h>
#include <AknCapServerClient.h>
#include <logsservices.h>



//CONSTANTS
static const int PHONE_CALL_NOT_FOUND = -1;

const TUid KCRUidTelTouchButtons = {0x2001B2E6};
const TUint32 KTelIncallTouchButtons   = 0x00000001;


inline Qt::TextElideMode clipToElide(
    TPhoneCmdParamCallHeaderData::TPhoneTextClippingDirection clip)
    {
    return clip == TPhoneCmdParamCallHeaderData::ELeft ? 
                   Qt::ElideLeft : Qt::ElideRight;
    }

PhoneUIQtViewAdapter::PhoneUIQtViewAdapter (PhoneUIQtViewIF &view, QObject *parent) :
    QObject (parent),
    m_view (view),
    m_idleUid(-1),
    m_bubbleWrapper(0),
    m_ringingtonecontroller(0),
    m_resourceAdapter(0),
    m_noteController(0),
    m_telephonyService(0),
    m_uiCommandController(0),
    m_messageController(0),
    m_indicatorController(0),
    m_dialpadAboutToClose(false),
    m_homeScreenToForeground(false),
    m_visibilityHandler(0),
    m_appLauncher(0),
    m_clearDialpadOnClose(true),
    m_speakerAsDefaultButton(false)
{
    m_bubbleWrapper = new PhoneBubbleWrapper(m_view.bubbleManager (), this);
    m_noteController = new PhoneNoteController(this);
    m_uiCommandController = new PhoneUiCommandController(view, this);

    TRAPD( error, m_ringingtonecontroller = CPhoneRingingToneController::NewL () );
    qt_symbian_throwIfError(error);
    m_resourceAdapter = PhoneResourceAdapter::Instance(this);

    // display initial toolbar to make client area size correct
    TPhoneCmdParamInteger intParam;
    intParam.SetInteger(m_resourceAdapter->defaultToolbarResourceId());
    setToolbarButtons(&intParam);

    m_telephonyService = new TelephonyService (this, this);
    m_visibilityHandler = new PhoneVisibilityHandler(view, this);
    m_indicatorController = new PhoneIndicatorController(*m_visibilityHandler,this);
    m_appLauncher = new PhoneAppLauncher(this);
    m_messageController = new PhoneMessageController(*m_appLauncher, this);
    
    int touchButton(-1);
    if ( KErrNone == CPhoneCenRepProxy::Instance()->GetInt(
            KCRUidTelTouchButtons,
            KTelIncallTouchButtons,
            touchButton ) ) {
        if ( touchButton == 2 ) {
            m_speakerAsDefaultButton = true;
        }    
    }

}

PhoneUIQtViewAdapter::~PhoneUIQtViewAdapter ()
{
    delete m_ringingtonecontroller;
}

void PhoneUIQtViewAdapter::ExecuteCommandL (TPhoneViewCommandId aCmdId)
{
    switch (aCmdId) {
    case EPhoneAppShutDown:
        {
        m_view.shutdownPhoneApp();
        }
        break;

    case EPhoneViewSetIdleTopApplication:
        {
            TPhoneCmdParamInteger uidParam;
            uidParam.SetInteger (idleAppUid());
            setTopApplication (&uidParam);
        }
        break;

    case EPhoneViewMuteRingToneOnAnswer:
        m_ringingtonecontroller->MuteRingingToneOnAnswer();
        break;

    case EPhoneViewStopRingTone:
        m_ringingtonecontroller->StopPlaying();
        break;

    case EPhoneViewMuteRingTone:
        m_ringingtonecontroller->MuteRingingTone();
        break;

    case EPhoneViewHideNaviPaneAudioVolume:
        m_view.removeVolumeSlider ();
        break;

    case EPhoneViewOpenDialer:
        openDialpad();
        break;

    case EPhoneViewRemoveConferenceBubble:
        removeConferenceBubble();
        break;

    case EPhoneViewSendToBackground:
    case EPhoneViewBringIdleToForeground:
        m_visibilityHandler->sendToBackground(m_homeScreenToForeground);
        m_homeScreenToForeground = false;
        break;
    case EPhoneViewRemoveAllCallHeaders:
        removeAllCallHeaders();
        break;
    case EPhoneViewRemoveNumberEntry:
    case EPhoneViewClearNumberEntryContent: // Fall through
        removeDialpad();
        break;
    case EPhoneViewAddToConference:
        addToConference();
        break;
    case EPhoneViewRemoveDtmfNote:
        m_noteController->removeDtmfNote();
        break;
    case EPhoneViewRemoveNote:
        m_noteController->removeNote();
        break;
    case EPhoneViewRemoveQuery:
        m_noteController->removeQuery();
        break;
    case EPhoneViewRemovePhoneDialogs:
        m_noteController->removeNote();
        m_noteController->removeQuery();
        break;
    case EPhoneViewRemoveGlobalNote:
    case EPhoneViewRemoveGlobalWaitNote:
        m_noteController->removeGlobalWaitNote();
        break;
    case EPhoneViewUpdateFSW:
        setHidden(true);
        break;
    default:
        break;
    }
 CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId );
}

void PhoneUIQtViewAdapter::ExecuteCommandL (TPhoneViewCommandId aCmdId, TInt aCallId)
{
    switch (aCmdId) {
    case EPhoneViewRemoveCallHeader:
        {
        m_bubbleWrapper->bubbleManager().startChanges();
        int bubble = m_bubbleWrapper->bubbles().value(aCallId);
        m_view.clearBubbleCommands(bubble);
        m_view.removeExpandAction(bubble);
        m_bubbleWrapper->removeCallHeader (aCallId);
        m_bubbleWrapper->bubbleManager().endChanges();
        
        if (!m_bubbleWrapper->bubbles().count()) {
            m_indicatorController->clearActiveCallData();
        }
        }
        break;
    case EPhoneViewRemoveFromConference:
        removeCallFromConference(aCallId);
        break;
    case EPhoneViewPrivateFromConference:
        setPrivateFromConference(aCallId);
        break;
    default:
        break;
    }
   CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId, aCallId );
}

void PhoneUIQtViewAdapter::ExecuteCommandL (TPhoneViewCommandId aCmdId, TPhoneCommandParam* aCommandParam)
{
    switch (aCmdId) {
    case EPhoneViewSetTopApplication:
        setTopApplication (aCommandParam);
        break;
    case EPhoneViewPlayRingTone:
        m_ringingtonecontroller->PlayRingToneL( aCommandParam );
        break;
    case EPhoneViewSetTouchPaneButtons:
        setTouchButtons (aCommandParam);
        setExpandActions();
        break;
    case EPhoneViewUpdateCba:
        setToolbarButtons (aCommandParam);
        break;
    case EPhoneViewSetHoldFlag:
        setCallHoldFlag (aCommandParam);
        break;
    case EPhoneViewGetHoldFlag:
        callHoldFlag (aCommandParam);
        break;
    case EPhoneViewSetNaviPaneAudioVolume:
        setAudioVolumeSliderValue (aCommandParam);
        break;
    case EPhoneViewActivateMuteUIChanges:
        setMuteIndication(aCommandParam);
        break;
    case EPhoneViewGetCountOfActiveCalls:
        activeCallCount(aCommandParam);
        break;
    case EPhoneViewActivateAudioPathUIChanges:
        setAudioPath(aCommandParam);
        break;
    case EPhoneViewGetExpandedBubbleCallId:
        expandedBubbleCallId(aCommandParam);
        break;
    case EPhoneViewGetIsConference:
        isConference(aCommandParam);
        break;
    case EPhoneViewBringAppToForeground:
        bringToForeground();
        break;
    case EPhoneViewShowGlobalNote:
        showGlobalNote(aCommandParam);
        break;
    case EPhoneViewSetNumberEntryVisible:
        setDialpadVisibility(aCommandParam);
        break;
    case EPhoneViewGetNumberEntryCount:
        getDialpadStringLength(aCommandParam);
        break;
    case EPhoneViewGetNumberFromEntry:
        getNumberFromDialpad(aCommandParam);
        break;
    case EPhoneViewShowNote:
        showNote(aCommandParam);
        break;
    case EPhoneViewShowQuery:
        m_noteController->showQuery(aCommandParam);
        break;
    case EPhoneViewOpenSoftRejectEditor:
        m_visibilityHandler->hideDeviceDialogs(false);
        m_messageController->openSoftRejectMessageEditor(aCommandParam);
        break;
    case EPhoneViewSetGlobalNotifiersDisabled:
        hideDeviceDialogs(aCommandParam);
        break;
    case EPhoneViewLaunchLogs: 
        openLogs(aCommandParam);
        break;
    default:
        break;
    }
    CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId, *aCommandParam );
}

void PhoneUIQtViewAdapter::ExecuteCommandL (TPhoneViewCommandId aCmdId, TInt aCallId,
    TPhoneCommandParam *aCommandParam)
{
    switch (aCmdId) {
    case EPhoneViewCreateCallHeader:
        createCallHeader (aCallId, aCommandParam);
        break;
    case EPhoneViewCreateEmergencyCallHeader:
        createEmergencyCallHeader (aCallId, aCommandParam);
        break;
    case EPhoneViewUpdateBubble:
        updateCallHeaderState (aCallId, aCommandParam);
        setExpandActions();
        break;
    case EPhoneViewUpdateCallHeaderRemoteInfoData:
        updateCallHeaderRemoteInfo (aCallId, aCommandParam);
        break;
    case EPhoneViewUpdateCallHeaderRemoteInfoDataAndLabel:
        updateCallHeaderRemoteInfoAndLabel(aCallId, aCommandParam);
        break;
    case EPhoneViewCreateConference:
        createConferenceBubble(aCallId, aCommandParam);
        break;
    case EPhoneViewGetCallExistsInConference:
        conferenceCallId(aCallId, aCommandParam);
        break;
    case EPhoneViewUpdateCallHeaderCallDuration:
    {
        TPhoneCmdParamInteger *time = static_cast<TPhoneCmdParamInteger *>(aCommandParam);
        QString duration = convertDuration(time->Integer());
        PHONE_DEBUG2("call duration:", duration);
        int bubbleId = m_bubbleWrapper->bubbleId(aCallId);
        
        if (-1 != bubbleId) {
            m_bubbleWrapper->bubbleManager().updateCallTime(bubbleId, duration);
        }
        break;
    }

    case EPhoneViewCipheringInfoChange:
    {
        handleCipheringInfoChange(aCallId, aCommandParam);
        break;
    }

    default:
        break;
    }
    CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId, aCallId, *aCommandParam );
}

void PhoneUIQtViewAdapter::ExecuteCommandL (TPhoneViewCommandId aCmdId, TInt aCallId,
    TDesC &aMessage)
{
    switch (aCmdId) {

    default:
        break;
    }
    CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId, aCallId, &aMessage );
}

TPhoneViewResponseId PhoneUIQtViewAdapter::HandleCommandL (TPhoneViewCommandId aCmdId)
{
    TPhoneViewResponseId response = EPhoneViewResponseSuccess;

    switch (aCmdId) {
    case EPhoneIsDTMFDialerVisible:
    case EPhoneIsCustomizedDialerVisible:
    case EPhoneViewGetNeedToSendToBackgroundStatus:
        // TODO: currently not supported
        response = EPhoneViewResponseFailed;
        break;
    case EPhoneViewGetNumberEntryIsVisibleStatus:
    case EPhoneViewGetNumberEntryIsUsedStatus: // Fall through
        {
        response = (m_view.isDialpadVisible() && !m_dialpadAboutToClose) ?
                        EPhoneViewResponseSuccess :
                        EPhoneViewResponseFailed;
        break;
        }
    default:
        break;
    }

    return response;
}

TPhoneViewResponseId PhoneUIQtViewAdapter::HandleCommandL (TPhoneViewCommandId aCmdId,
    TPhoneCommandParam *aCommandParam)
{
    TPhoneViewResponseId viewResponse = EPhoneViewResponseSuccess;

    switch (aCmdId) {
    case EPhoneViewGetCallIdByState:
        viewResponse = callIdByState (aCommandParam);
        break;
    case EPhoneViewSelectedConfMember:
        viewResponse = getSelectedConferenceMember (aCommandParam);
        break;
    default:
        break;
    }

    return viewResponse;
}

void PhoneUIQtViewAdapter::ExecuteCommand (TPhoneViewCommandId aCmdId)
{
    switch (aCmdId) {
    case EPhoneSetConferenceExpanded:
        m_bubbleWrapper->bubbleManager ().startChanges ();
        m_bubbleWrapper->bubbleManager().setExpandedConferenceCallHeader(
                m_bubbleWrapper->conferenceCallList().size());
        m_bubbleWrapper->bubbleManager ().endChanges ();
        break;
    case EPhoneViewBeginUpdate:
        m_bubbleWrapper->bubbleManager ().startChanges ();
        break;
    case EPhoneViewEndUpdate:
        m_bubbleWrapper->bubbleManager ().endChanges ();
        break;
    case EPhoneViewOpenContacts:
        openContacts();
        break;
    default:
        break;
    }
}

void PhoneUIQtViewAdapter::ExecuteCommand (TPhoneViewCommandId aCmdId, TPhoneCommandParam *aCommandParam)
{
    switch (aCmdId) {
    case EPhoneViewGetAudioVolumeLevel:
        writeAudioVolumeLevel (aCommandParam);
        break;
    case EPhoneViewGetNumberFromEntry:
        getNumberFromDialpad(aCommandParam);
        break;
    case EPhoneViewBackButtonActive: {
        TPhoneCmdParamBoolean *param = static_cast<TPhoneCmdParamBoolean *>(aCommandParam);
        m_view.setBackButtonVisible(param->Boolean());
		}
        break;
    case EPhoneViewHsToForegroundAfterCall: {
        TPhoneCmdParamBoolean *param = static_cast<TPhoneCmdParamBoolean *>(aCommandParam);
        m_homeScreenToForeground = param->Boolean();
        }
        break;
    case EPhoneViewSetIhfFlag: {
        TPhoneCmdParamBoolean *param = static_cast<TPhoneCmdParamBoolean *>(aCommandParam);

        PhoneResourceAdapter::Instance()->buttonsController()->
                setButtonFlags(PhoneUIQtButtonsController::Ihf, 
                               param->Boolean());
        }
        break;
    case EPhoneViewSetMuteFlag: {
        TPhoneCmdParamBoolean *param = static_cast<TPhoneCmdParamBoolean *>(aCommandParam);

        PhoneResourceAdapter::Instance()->buttonsController()->
                setButtonFlags(PhoneUIQtButtonsController::Mute, 
                               param->Boolean());
        }
        break;
    case EPhoneViewSetBlueToothFlag: {
        TPhoneCmdParamBoolean *param = static_cast<TPhoneCmdParamBoolean *>(aCommandParam);

        PhoneResourceAdapter::Instance()->buttonsController()->
                setButtonFlags(PhoneUIQtButtonsController::Btaa, 
                               param->Boolean());
        }
        break;
    case EPhoneViewSetBluetoothAvailableFlag: {
        TPhoneCmdParamBoolean *param = static_cast<TPhoneCmdParamBoolean *>(aCommandParam);
    
        PhoneResourceAdapter::Instance()->buttonsController()->
                setButtonFlags(PhoneUIQtButtonsController::BluetoothAvailable, 
                               param->Boolean());
        }
        break;
    case EPhoneViewStartCapturingKey: {
        TPhoneCmdParamKeyCapture *captureParam = 
                static_cast<TPhoneCmdParamKeyCapture *>(aCommandParam);        
        Qt::Key qtKey;
        
        if (convertKey(captureParam->KeyCode(), qtKey)) {
            m_view.captureKey(qtKey, true);
        }
    }
    break;
    case EPhoneViewStopCapturingKey: {
        TPhoneCmdParamKeyCapture *captureParam = 
                static_cast<TPhoneCmdParamKeyCapture *>(aCommandParam);        
        Qt::Key qtKey;
        
        if (convertKey(captureParam->KeyCode(), qtKey)) {
            m_view.captureKey(qtKey, false);
        }
    }
    break;
    default:
        break;
    }
}

const TDesC& PhoneUIQtViewAdapter::FetchContent ()
{
    return KNullDesC;
}

void PhoneUIQtViewAdapter::dialpadClosed()
{
    if (m_clearDialpadOnClose) {
        m_view.clearDialpad();
    }
    m_dialpadAboutToClose = true;
    m_clearDialpadOnClose = true;
    setCallMenu();
}

PhoneNoteController* PhoneUIQtViewAdapter::noteController() const
{
    return m_noteController;
}

void PhoneUIQtViewAdapter::keyReleased(QKeyEvent */*event*/)
{
    if (m_view.isDialpadVisible() &&
        false == m_dialpadAboutToClose ) {
        setDialpadMenu();
    }
}

void PhoneUIQtViewAdapter::handleWindowActivated()
{
    m_indicatorController->disableActiveCallIndicator();
}

void PhoneUIQtViewAdapter::handleWindowDeactivated()
{
    m_indicatorController->enableActiveCallIndicator();
}

void PhoneUIQtViewAdapter::setTopApplication (TPhoneCommandParam *commandParam)
{
    TPhoneCmdParamInteger *integerParam = static_cast<TPhoneCmdParamInteger *> (commandParam);
    CPhonePubSubProxy::Instance()->ChangePropertyValue(
        KPSUidUikon,
        KUikVideoCallTopApp,
        integerParam->Integer() );

    // Hide the Phone icon if it is not the top application
    // TODO: how to do this?
}

int PhoneUIQtViewAdapter::idleAppUid ()
{
// <-- QT HS START -->
   /* if ( m_idleUid == -1 ) {
        // Get Idle's UID from PubSub.
        m_idleUid = CPhonePubSubProxy::Instance()->Value (KPSUidAiInformation,
            KActiveIdleUid );
    }*/
    m_idleUid=0x20022F35;
// <-- QT HS END -->
    return m_idleUid;
}

void PhoneUIQtViewAdapter::createCallHeader(
    int callId,
    TPhoneCommandParam *commandParam )
{
    PHONE_DEBUG("PhoneUIQtViewAdapter::createCallHeader");
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdCallHeaderData);

    TPhoneCmdParamCallHeaderData &data =
        static_cast<TPhoneCmdParamCallHeaderData &> (*commandParam);

    m_bubbleWrapper->bubbleManager ().startChanges ();

    if (m_bubbleWrapper->bubbleManager().isConferenceExpanded())
        {
        m_bubbleWrapper->bubbleManager().setExpandedConferenceCallHeader(false);
        }

    int bubble = m_bubbleWrapper->createCallHeader (callId);
    m_bubbleWrapper->setState (callId, bubble, data.CallState ());        
    m_bubbleWrapper->setCli (bubble, data.CLIText (), clipToElide(data.CLITextClippingDirection()));
    m_bubbleWrapper->setServiceId(callId, data.ServiceId());
    m_bubbleWrapper->setSecondaryCli (bubble, data.CNAPText (), clipToElide(data.CNAPTextClippingDirection()));
    m_bubbleWrapper->setLabel (bubble, data.LabelText ());
    m_bubbleWrapper->setCallType (bubble, data.CallType ());
    m_bubbleWrapper->setDivert (bubble, data.Diverted ());
    m_bubbleWrapper->setCiphering(bubble, data.CipheringIndicatorAllowed(), data.Ciphering());

    if (data.Picture().Length()) {
        QString imagePath =
            QString::fromUtf16(data.Picture().Ptr(),data.Picture().Length());
        m_bubbleWrapper->bubbleManager().setCallObjectImage(bubble,imagePath);
    } else {
        m_bubbleWrapper->bubbleManager().setCallObjectFromTheme(bubble);
    }
    m_bubbleWrapper->bubbleManager ().endChanges ();
    
    if (1 == m_bubbleWrapper->bubbles().keys().count()) {
        setHidden(false);
    }
    m_indicatorController->setActiveCallData();
}

void PhoneUIQtViewAdapter::createEmergencyCallHeader(
    int callId,
    TPhoneCommandParam *commandParam)
{
    PHONE_DEBUG("PhoneUIQtViewAdapter::createEmergencyCallHeader");
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdEmergencyCallHeaderData);

    TPhoneCmdParamEmergencyCallHeaderData &data =
        static_cast<TPhoneCmdParamEmergencyCallHeaderData &> (*commandParam);

    m_bubbleWrapper->bubbleManager ().startChanges ();
    int bubble = m_bubbleWrapper->createCallHeader (callId);
    m_bubbleWrapper->setLabel (bubble, data.LabelText ());
    m_bubbleWrapper->setCli (bubble, data.HeaderText (), Qt::ElideRight);
    m_bubbleWrapper->setCiphering(bubble, data.CipheringIndicatorAllowed(), data.Ciphering());
    m_bubbleWrapper->bubbleManager ().endChanges ();
    
    m_indicatorController->setActiveCallData();
}

void PhoneUIQtViewAdapter::updateCallHeaderState (
    int callId,
    TPhoneCommandParam *commandParam)
{
    PHONE_DEBUG("PhoneUIQtViewAdapter::updateCallHeaderState");
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdCallHeaderData);

    TPhoneCmdParamCallHeaderData &data =
            static_cast<TPhoneCmdParamCallHeaderData &> (*commandParam);

    int bubble = m_bubbleWrapper->bubbleId (callId);
    if ( -1 != bubble ) {
        m_bubbleWrapper->bubbleManager ().startChanges ();
        m_bubbleWrapper->setState (callId, bubble, data.CallState ());
        m_bubbleWrapper->setLabel (bubble, data.LabelText ());
        m_bubbleWrapper->setDivert (bubble, data.Diverted ());
        m_bubbleWrapper->bubbleManager ().endChanges ();
    }
}

void PhoneUIQtViewAdapter::updateCallHeaderRemoteInfo (int callId, TPhoneCommandParam *commandParam)
{
    PHONE_DEBUG("PhoneUIQtViewAdapter::updateCallHeaderRemoteInfo");
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdCallHeaderData);

    TPhoneCmdParamCallHeaderData &data =
         static_cast<TPhoneCmdParamCallHeaderData &> (*commandParam);


     int bubble = m_bubbleWrapper->bubbleId (callId);
     if ( -1 != bubble ) {
         m_bubbleWrapper->bubbleManager ().startChanges ();
         m_bubbleWrapper->setCli (bubble, data.CLIText (),
             clipToElide(data.CLITextClippingDirection()));
         m_bubbleWrapper->setSecondaryCli (bubble, data.CNAPText (),
             clipToElide(data.CNAPTextClippingDirection()));
         m_bubbleWrapper->setDivert (bubble, data.Diverted ());
         m_bubbleWrapper->bubbleManager ().endChanges ();
     }
     m_indicatorController->setActiveCallData();

}

void PhoneUIQtViewAdapter::updateCallHeaderRemoteInfoAndLabel (int callId, TPhoneCommandParam *commandParam)
{
    PHONE_DEBUG("PhoneUIQtViewAdapter::updateCallHeaderRemoteInfoAndLabel");
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdCallHeaderData);

    TPhoneCmdParamCallHeaderData &data =
         static_cast<TPhoneCmdParamCallHeaderData &> (*commandParam);

     int bubble = m_bubbleWrapper->bubbleId (callId);
     if ( -1 != bubble ) {
         m_bubbleWrapper->bubbleManager ().startChanges ();
         m_bubbleWrapper->setCli (bubble, data.CLIText (),
             clipToElide(data.CLITextClippingDirection()));
         m_bubbleWrapper->setSecondaryCli (bubble, data.CNAPText (),
             clipToElide(data.CNAPTextClippingDirection()));
         m_bubbleWrapper->setLabel (bubble, data.LabelText ());
         m_bubbleWrapper->setDivert (bubble, data.Diverted ());
         m_bubbleWrapper->bubbleManager ().endChanges ();
     }
}

void PhoneUIQtViewAdapter::handleCipheringInfoChange(int callId, TPhoneCommandParam *commandParam)
{
    TPhoneCmdParamCallHeaderData *param =
        static_cast<TPhoneCmdParamCallHeaderData*>(commandParam);

    int bubble = m_bubbleWrapper->bubbleId(callId);        
    if ( -1 != bubble ) {        
        m_bubbleWrapper->bubbleManager().startChanges();
        m_bubbleWrapper->setCiphering(m_bubbleWrapper->bubbleId(callId),
                                      param->CipheringIndicatorAllowed(),
                                      param->Ciphering());
        m_bubbleWrapper->bubbleManager().endChanges();
    }
}

TPhoneViewResponseId PhoneUIQtViewAdapter::callIdByState (TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdCallStateData);

    TPhoneCmdParamCallStateData &stateParam =
        static_cast<TPhoneCmdParamCallStateData &>(*commandParam);

    stateParam.SetCallId (m_bubbleWrapper->callId (stateParam.CallState ()));
    return EPhoneViewResponseSuccess;
}

void PhoneUIQtViewAdapter::setTouchButtons (TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdBoolean);

    TPhoneCmdParamBoolean &param =
        static_cast<TPhoneCmdParamBoolean &>(*commandParam);
    bool emergencyCall = param.Boolean();

    m_bubbleWrapper->bubbleManager ().startChanges ();

    QList<int> bubbles = m_bubbleWrapper->bubbles().keys();

    m_resourceAdapter->buttonsController()->setButtonFlags(
            PhoneUIQtButtonsController::DisableJoin,
            (5 <= m_bubbleWrapper->conferenceCallList().count()));
    
    setCommonButtonFlags();

    for (int j = 0; j < bubbles.size(); ++j){
        int callId = bubbles.at(j);
        // Clear bubble
        m_view.clearBubbleCommands(m_bubbleWrapper->bubbles().value(callId));

		// Get bubble actions by call type
        QMap<PhoneAction::ActionType, PhoneAction *> actions =
            m_uiCommandController->pushButtonActionsForCall(
                               m_bubbleWrapper->callStates().value(callId),
                               emergencyCall,
                               m_bubbleWrapper->callStates(),
                               m_bubbleWrapper->serviceIds(),
                               m_bubbleWrapper->serviceIdByCallId(callId),
                               callId);

        QList<PhoneAction *> values = actions.values();
        for (int i = 0; i < values.size (); ++i) {
            PhoneAction *action = values.at (i);
            m_view.addBubbleCommand(m_bubbleWrapper->bubbles().value(callId),
                                    *action);
            delete action;
        }
    }

    setExpandedConferenceCallHeader();
    setBubbleSelectionFlag();
    setMenu();

    m_bubbleWrapper->bubbleManager ().endChanges ();
}

void PhoneUIQtViewAdapter::setToolbarButtons (TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdInteger);

    TPhoneCmdParamInteger &intParam =
        static_cast<TPhoneCmdParamInteger &>(*commandParam);

    setCommonButtonFlags();
    
    int callId(-1);
    int serviceId(-1);
    if ( 0<m_bubbleWrapper->callStates().keys().size() ) {
        int bubbleId = m_bubbleWrapper->bubbleManager().expandedBubble();
        callId = m_bubbleWrapper->callIdByBubbleId(bubbleId);
        serviceId = m_bubbleWrapper->serviceIdByCallId(callId);
    }
    
    QList<PhoneAction*> actions = m_uiCommandController->toolBarActions(
                                        intParam.Integer(),
                                        m_bubbleWrapper->callStates(),
                                        m_bubbleWrapper->serviceIds(),
                                        serviceId,
                                        callId );
    
    if (actions.count()) {
        m_view.setToolbarActions(actions);
    }

    qDeleteAll(actions);
}

void PhoneUIQtViewAdapter::setCallHoldFlag (TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdBoolean);

    TPhoneCmdParamBoolean &boolParam =
        static_cast<TPhoneCmdParamBoolean &>(*commandParam);

    m_resourceAdapter->buttonsController()->setButtonFlags(PhoneUIQtButtonsController::Hold,
                                                           boolParam.Boolean ());
}

void PhoneUIQtViewAdapter::callHoldFlag (TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdBoolean);

    TPhoneCmdParamBoolean &boolParam =
        static_cast<TPhoneCmdParamBoolean &>(*commandParam);
    boolParam.SetBoolean (m_resourceAdapter->buttonsController()->getButtonFlags(PhoneUIQtButtonsController::Hold));
}

void PhoneUIQtViewAdapter::writeAudioVolumeLevel (TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdInteger);

    TPhoneCmdParamInteger &intParam =
        static_cast<TPhoneCmdParamInteger &>(*commandParam);
    intParam.SetInteger (m_view.volumeSliderValue ());
}

void PhoneUIQtViewAdapter::setAudioVolumeSliderValue (TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdInteger);

    TPhoneCmdParamInteger &intParam =
        static_cast<TPhoneCmdParamInteger &>(*commandParam);

    m_view.setVolumeSliderValue(intParam.Integer(),
                                EPhoneInCallCmdSetVolumeLevel,
                                KPhoneVolumeMaxValue,
                                KPhoneVolumeMinValue );
}

void PhoneUIQtViewAdapter::setMuteIndication (TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdBoolean);
    TPhoneCmdParamBoolean &boolParam =
            static_cast<TPhoneCmdParamBoolean &>(*commandParam);
    m_bubbleWrapper->bubbleManager().setPhoneMuted(boolParam.Boolean());
}

void PhoneUIQtViewAdapter::activeCallCount (TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdInteger);

    TPhoneCmdParamInteger &integerParam =
        static_cast<TPhoneCmdParamInteger&>(*commandParam);
    integerParam.SetInteger (m_bubbleWrapper->activeCallCount());
}

void PhoneUIQtViewAdapter::setAudioPath (TPhoneCommandParam *commandParam)
    {
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdAudioOutput);

    TPhoneCmdParamAudioOutput &outputParam =
        static_cast<TPhoneCmdParamAudioOutput &>(*commandParam);

    m_resourceAdapter->buttonsController()->setButtonFlags(PhoneUIQtButtonsController::Ihf,
                              outputParam.AudioOutput() == EPELoudspeaker);
    m_resourceAdapter->buttonsController()->setButtonFlags(PhoneUIQtButtonsController::Wired,
                              outputParam.AudioOutput() == EPEWiredAudioAccessory);
    m_resourceAdapter->buttonsController()->setButtonFlags(PhoneUIQtButtonsController::Btaa,
                              outputParam.AudioOutput() == EPEBTAudioAccessory);
    }

void PhoneUIQtViewAdapter::expandedBubbleCallId(TPhoneCommandParam *commandParam)
    {
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdInteger);

    TPhoneCmdParamInteger &outputParam =
            static_cast<TPhoneCmdParamInteger &>(*commandParam);
    int callId = PHONE_CALL_NOT_FOUND;

    if ( 0<m_bubbleWrapper->callStates().keys().size() ) {
        int bubbleId = m_bubbleWrapper->bubbleManager().expandedBubble();
        callId = m_bubbleWrapper->callIdByBubbleId(bubbleId);
    }

    outputParam.SetInteger( callId );
    }

void PhoneUIQtViewAdapter::openDialpad()
    {
    m_dialpadAboutToClose = false;
    m_view.showDialpad();
    setDialpadMenu();
    }

void PhoneUIQtViewAdapter::createConferenceBubble(int callId, TPhoneCommandParam *commandParam)
{
    PHONE_DEBUG("PhoneUIQtViewAdapter::createConferenceBubble");
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdCallHeaderData);

    TPhoneCmdParamCallHeaderData &data =
         static_cast<TPhoneCmdParamCallHeaderData &> (*commandParam);

    int bubble = m_bubbleWrapper->createConferenceBubble(
            callId, data.CallState(), data.LabelText(), data.CLIText());

    m_bubbleWrapper->setServiceId(callId,data.ServiceId());
    m_bubbleWrapper->setCiphering(bubble, data.CipheringIndicatorAllowed(), data.Ciphering());
    
    setParticipantListActions();
}

void PhoneUIQtViewAdapter::conferenceCallId(int callId, TPhoneCommandParam *commandParam)
{
    PHONE_DEBUG("PhoneUIQtViewAdapter::conferenceCallId");
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdBoolean);

    TPhoneCmdParamBoolean*  conferenceDataValue =
        static_cast<TPhoneCmdParamBoolean*>(
                commandParam );

    conferenceDataValue->SetBoolean(m_bubbleWrapper->conferenceCallId(callId));
}

void PhoneUIQtViewAdapter::removeConferenceBubble()
{
    m_bubbleWrapper->bubbleManager().startChanges();
    m_view.removeExpandAction(m_bubbleWrapper->bubbleId(KConferenceCallId));
    m_view.clearParticipantListActions();
    m_bubbleWrapper->removeConferenceBubble();
    m_bubbleWrapper->bubbleManager().endChanges();
    if (!m_bubbleWrapper->bubbles().count()) {
        m_indicatorController->clearActiveCallData();
    }

}

void PhoneUIQtViewAdapter::isConference(TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdBoolean);

    TPhoneCmdParamBoolean*  conferenceDataValue =
        static_cast<TPhoneCmdParamBoolean*>(
                commandParam );

    conferenceDataValue->SetBoolean(0<m_bubbleWrapper->conferenceCallList().size());
}

void PhoneUIQtViewAdapter::removeCallFromConference(int callId)
{
    m_bubbleWrapper->bubbleManager().startChanges();
    m_bubbleWrapper->removeCallFromConference(callId);
    int bubbleId = m_bubbleWrapper->bubbles().value(callId);
    m_view.clearBubbleCommands(bubbleId);
    m_view.removeExpandAction(bubbleId);
    m_bubbleWrapper->removeCallHeader(callId);
    m_bubbleWrapper->bubbleManager().endChanges();
}

TPhoneViewResponseId PhoneUIQtViewAdapter::getSelectedConferenceMember(
        TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdInteger);
    TPhoneViewResponseId ret = EPhoneViewResponseSuccess;
    TPhoneCmdParamInteger*  integerValue =
        static_cast<TPhoneCmdParamInteger*>(
                commandParam );

    int bubbleId = m_bubbleWrapper->bubbleManager().selectionIdInConference();
    int callId = m_bubbleWrapper->callIdByBubbleId(bubbleId);

    if (callId == -1) {
        ret = EPhoneViewResponseFailed;
    } else {
        integerValue->SetInteger(callId);
    }

    return ret;
}

void PhoneUIQtViewAdapter::setPrivateFromConference(int callId)
{
    m_bubbleWrapper->bubbleManager().startChanges();
    m_bubbleWrapper->bubbleManager().setExpandedConferenceCallHeader(false);
    m_bubbleWrapper->removeCallFromConference(callId);
    m_view.clearBubbleCommands(m_bubbleWrapper->bubbles().value(callId));
    m_bubbleWrapper->bubbleManager().endChanges();
}

void PhoneUIQtViewAdapter::setExpandActions()
{
    /* If there is 2 or more calls ongoing then we should
     * enable expancion swap*/
    int heldBubble = -1;

    if ( 2 <= m_bubbleWrapper->callStates().values().size() ) {
        if (callsExists(EPEStateConnected,EPEStateHeld)) {
            heldBubble =
                m_bubbleWrapper->bubbleId(m_bubbleWrapper->callId(EPEStateHeld));
            m_view.setExpandAction(heldBubble, EPhoneInCallCmdSwap);
        } else if (callsExists(EPEStateConnected,EPEStateHeldConference)) {
            heldBubble =
                m_bubbleWrapper->bubbleId(m_bubbleWrapper->callId(EPEStateHeldConference));
            m_view.setExpandAction(heldBubble, EPhoneInCallCmdSwap);
        } else if (callsExists(EPEStateRinging,EPEStateHeld)) {
            heldBubble =
                m_bubbleWrapper->bubbleId(m_bubbleWrapper->callId(EPEStateHeld));
            m_view.setExpandAction(heldBubble, EPhoneInCallCmdUnhold);
        }

        QList<int> calls = m_bubbleWrapper->bubbles().keys();
        foreach (int call, calls) {
            int bubbleId = m_bubbleWrapper->bubbleId(call);
            if ( bubbleId != heldBubble ) {
                 m_view.setExpandAction(bubbleId, EPhoneCmdUpdateUiControls);
            }
        }
    }
}

bool PhoneUIQtViewAdapter::callsExists(int call1State, int call2State)
{
    bool callsExist(false);
    if ( m_bubbleWrapper->callStates().values().contains(call1State) &&
         m_bubbleWrapper->callStates().values().contains(call2State) ) {
        callsExist = true;
    }
    return callsExist;
}

void PhoneUIQtViewAdapter::setParticipantListActions()
{
    if ( 0<m_bubbleWrapper->conferenceCallList().size() ) {
        m_view.addParticipantListAction(
                EPhoneInCallCmdPrivate,
                hbTrId("txt_phone_button_private"),
                HbIcon("qtg_mono_private_call"));
        m_view.addParticipantListAction(
                EPhoneInCallCmdDrop,
                hbTrId("txt_phone_button_drop"),
                HbIcon("qtg_mono_drop_call"));
    }
}

void PhoneUIQtViewAdapter::setExpandedConferenceCallHeader()
{
    int callId = PHONE_CALL_NOT_FOUND;

    if ( 1==m_bubbleWrapper->bubbles().keys().size()
         || (1<m_bubbleWrapper->bubbles().keys().size()
         && false == m_bubbleWrapper->callStates().values().contains(EPEStateRinging)
         && false == m_bubbleWrapper->callStates().values().contains(EPEStateDialing)
         && false == m_bubbleWrapper->callStates().values().contains(EPEStateConnecting)
         && false == m_bubbleWrapper->callStates().values().contains(EPEStateHeldConference))) {

        int bubbleId = m_bubbleWrapper->bubbleManager().expandedBubble();
        callId = m_bubbleWrapper->callIdByBubbleId(bubbleId);
    }

    m_bubbleWrapper->bubbleManager().setExpandedConferenceCallHeader(
            (KConferenceCallId == callId));
}

void PhoneUIQtViewAdapter::bringToForeground()
{
    m_visibilityHandler->bringToForeground();
}

void PhoneUIQtViewAdapter::hideDeviceDialogs(TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdBoolean);
    TPhoneCmdParamBoolean* booleanParam =
        static_cast<TPhoneCmdParamBoolean*>(commandParam);
    m_visibilityHandler->hideDeviceDialogs(booleanParam->Boolean());
}

void PhoneUIQtViewAdapter::showGlobalNote(
        TPhoneCommandParam *commandParam)
{
    m_noteController->showGlobalNote(commandParam);
}

void PhoneUIQtViewAdapter::setDialpadVisibility(
        TPhoneCommandParam *commandParam)
{
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdBoolean);
    TPhoneCmdParamBoolean* booleanParam =
        static_cast<TPhoneCmdParamBoolean*>(commandParam);

    if (booleanParam->Boolean()) {
        m_dialpadAboutToClose = false;
        m_view.showDialpad();
    } else {
        m_clearDialpadOnClose = false;
        m_view.hideDialpad();
    }
}

void PhoneUIQtViewAdapter::removeAllCallHeaders()
{
    if (m_bubbleWrapper->conferenceCallList().size()) {
        removeConferenceBubble();
    }

    QList<int> callIds = m_bubbleWrapper->bubbles().keys();

    for (int i=0; i<callIds.size(); ++i) {
        int callId = callIds.at(i);
        if (KEmergencyCallId != callId) {
            m_bubbleWrapper->bubbleManager().startChanges();
            int bubble = m_bubbleWrapper->bubbles().value(callId);
            m_view.clearBubbleCommands(bubble);
            m_view.removeExpandAction(bubble);
            m_bubbleWrapper->removeCallHeader (callId);
            m_bubbleWrapper->bubbleManager().endChanges();
        }
    }
    if (!m_bubbleWrapper->bubbles().count()) {
        m_indicatorController->clearActiveCallData();
    }

}

void PhoneUIQtViewAdapter::getNumberFromDialpad(
        TPhoneCommandParam *commandParam)
{

    TPhoneCmdParamString* entryContent =
        static_cast<TPhoneCmdParamString*>(commandParam);
    
    QString westernNumber = 
        HbStringUtil::convertDigitsTo(m_view.dialpadText(), WesternDigit);
    
    (*entryContent->String()).Copy(westernNumber.utf16());         
}

void PhoneUIQtViewAdapter::getDialpadStringLength(
        TPhoneCommandParam *commandParam)
{

    TPhoneCmdParamInteger* integerParam =
        static_cast<TPhoneCmdParamInteger*>(commandParam);

    integerParam->SetInteger(m_view.dialpadText().length());
}

void PhoneUIQtViewAdapter::removeDialpad()
{
    m_view.clearAndHideDialpad();
}

void PhoneUIQtViewAdapter::addToConference()
{
    m_bubbleWrapper->addToConferenceBubble();
}

void PhoneUIQtViewAdapter::setMenu()
{
    if (m_view.isDialpadVisible() && false == m_dialpadAboutToClose) {
        setDialpadMenu();
    } else {
        setCallMenu();
    }
}

void PhoneUIQtViewAdapter::setCallMenu()
{
    if ( 0<m_bubbleWrapper->callStates().keys().size() ) {
        int bubbleId = m_bubbleWrapper->bubbleManager().expandedBubble();
        int callId = m_bubbleWrapper->callIdByBubbleId(bubbleId);
        m_uiCommandController->setCallMenuActions(
                m_bubbleWrapper->callStates(),
                m_bubbleWrapper->serviceIds(),
                m_bubbleWrapper->serviceIdByCallId(callId),
                callId );
    }

}

void PhoneUIQtViewAdapter::setDialpadMenu()
{
    m_uiCommandController->setDialpadMenuActions();
}

void PhoneUIQtViewAdapter::showNote(TPhoneCommandParam *commandParam)
{
    m_noteController->showNote(commandParam);
}

QString PhoneUIQtViewAdapter::convertDuration(TInt secs)
    {
    QTime init(0,0,0);
    QTime duration = init.addSecs(secs);
    HbExtendedLocale locale = HbExtendedLocale::system();
    QString durationString = locale.format(duration, QString(r_qtn_time_durat_long));
    return HbStringUtil::convertDigits(durationString); 
} 

void PhoneUIQtViewAdapter::setBubbleSelectionFlag()
{
    bool selectionFlag = ((m_bubbleWrapper->callStates().
                              values().contains(EPEStateRinging) &&
                           m_bubbleWrapper->conferenceCallList().count())||
                          m_bubbleWrapper->callStates().
                              values().contains(EPEStateDialing) ||
                          m_bubbleWrapper->callStates().
                              values().contains(EPEStateConnecting));

    m_bubbleWrapper->bubbleManager().setBubbleSelectionDisabled(selectionFlag);
}

void PhoneUIQtViewAdapter::setHidden(bool hidden)
{
    TRAP_IGNORE(SetHiddenL(hidden));
}

void PhoneUIQtViewAdapter::SetHiddenL(bool hidden)
{
    CEikonEnv* env = CEikonEnv::Static();
    
    if (env) {
        CApaWindowGroupName* windowGroupName =
            CApaWindowGroupName::NewLC(
                    env->WsSession(),
                    env->RootWin().Identifier() );
        
        windowGroupName->SetHidden( hidden );
        User::LeaveIfError(
                windowGroupName->SetWindowGroupName(
                        env->RootWin() ) );
        
        CleanupStack::PopAndDestroy( windowGroupName );

        CAknSgcClient::AknSrv()->UpdateTaskList();
    }
}
void PhoneUIQtViewAdapter::openContacts()
{
    m_appLauncher->launchContacts();
}

void PhoneUIQtViewAdapter::openLogs(TPhoneCommandParam *commandParam)
{
    TPhoneCmdParamString* entryContent =
            static_cast<TPhoneCmdParamString*>(commandParam);
    
    bool phoneVisible = m_visibilityHandler->phoneVisible();
    
    m_homeScreenToForeground = false;
    m_visibilityHandler->sendToBackground(m_homeScreenToForeground);
    
    if (phoneVisible) { 
        // Activate logs dialer only if telephone is on the top.
        m_appLauncher->launchLogs(
                LogsServices::ViewAll,
                true,
                QString::fromUtf16(
                    entryContent->String()->Ptr(),
                    entryContent->String()->Length()));
    } 
}

void PhoneUIQtViewAdapter::setCommonButtonFlags()
{
    m_resourceAdapter->buttonsController()->setButtonFlags(
            PhoneUIQtButtonsController::FullConference,
            (5 <= m_bubbleWrapper->conferenceCallList().count()));

    m_resourceAdapter->buttonsController()->setButtonFlags(
            PhoneUIQtButtonsController::IhfAsPushButton,
            m_speakerAsDefaultButton);
}

bool PhoneUIQtViewAdapter::convertKey(
        TKeyCode symbianKey, Qt::Key &qtKey)
{
    bool ret = false;
    
    switch(symbianKey) {
    case EKeyNo: {
        qtKey = Qt::Key_No;
        ret = true; 
    }
    break;
    default:
    break;
    }
    
    return ret;
}


