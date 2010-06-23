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
* Description: !!!!!!!!! STUB !!!!
*
*/

#include "phoneuiqtviewadapter.h"
#include "phoneuiqtviewif.h"

//CONSTANTS
bool phoneAppStart;
bool inCallDialerStart;
static const int PHONE_CALL_NOT_FOUND = -1;

PhoneUIQtViewAdapter::PhoneUIQtViewAdapter (PhoneUIQtViewIF &view, QObject *parent) : 
    QObject (parent), m_view (view), m_idleUid(-1)
{}

PhoneUIQtViewAdapter::~PhoneUIQtViewAdapter ()
{}

void PhoneUIQtViewAdapter::ExecuteCommandL (TPhoneViewCommandId aCmdId)
{
switch (aCmdId) {
case EPhoneViewOpenDialer:
        inCallDialerStart = true;
        break;
default:
        break;
    }
}

void PhoneUIQtViewAdapter::ExecuteCommandL (TPhoneViewCommandId aCmdId, TPhoneCommandParam* aCommandParam)
{
switch (aCmdId) {
case EPhoneViewBringAppToForeground:
        phoneAppStart = true;
        break;
default:
        break;
    }
}

int PhoneUIQtViewAdapter::idleAppUid ()
{
// <-- QT HS START -->);
    m_idleUid=0x20022F35;
// <-- QT HS END -->
    return m_idleUid;
}

void PhoneUIQtViewAdapter::ExecuteCommandL (TPhoneViewCommandId aCmdId, TInt aCallId)
{}
void PhoneUIQtViewAdapter::ExecuteCommandL (TPhoneViewCommandId aCmdId, TInt aCallId,
    TPhoneCommandParam *aCommandParam)
{}
void PhoneUIQtViewAdapter::ExecuteCommandL (TPhoneViewCommandId aCmdId, TInt aCallId, 
    TDesC &aMessage)
{}
TPhoneViewResponseId PhoneUIQtViewAdapter::HandleCommandL (TPhoneViewCommandId aCmdId)
{}
TPhoneViewResponseId PhoneUIQtViewAdapter::HandleCommandL (TPhoneViewCommandId aCmdId, 
    TPhoneCommandParam *aCommandParam)
{}
void PhoneUIQtViewAdapter::ExecuteCommand (TPhoneViewCommandId aCmdId)
{    Q_UNUSED (aCmdId);    }
void PhoneUIQtViewAdapter::ExecuteCommand (TPhoneViewCommandId aCmdId, TPhoneCommandParam *aCommandParam)
{}
const TDesC& PhoneUIQtViewAdapter::FetchContent ()
{    return KNullDesC;}
void PhoneUIQtViewAdapter::setTopApplication (TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::createCallHeader( 
    int callId, 
    TPhoneCommandParam *commandParam )
{}
void PhoneUIQtViewAdapter::createEmergencyCallHeader(
    int callId, 
    TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::updateCallHeaderState (
    int callId, 
    TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::updateCallHeaderRemoteInfo (int callId, TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::updateCallHeaderRemoteInfoAndLabel (int callId, TPhoneCommandParam *commandParam)
{}
TPhoneViewResponseId PhoneUIQtViewAdapter::callIdByState (TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::setTouchButtons (TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::setToolbarButtons (TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::callHoldFlag (TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::writeAudioVolumeLevel (TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::setAudioVolumeSliderValue (TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::setMuteIndication (TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::activeCallCount (TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::setAudioPath (TPhoneCommandParam *commandParam)
    {    }
void PhoneUIQtViewAdapter::expandedBubbleCallId(TPhoneCommandParam *commandParam)
    {    }
void PhoneUIQtViewAdapter::openDialpad()
    {    }
void PhoneUIQtViewAdapter::createConferenceBubble(int callId, TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::conferenceCallId(int callId, TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::removeConferenceBubble()
{}
void PhoneUIQtViewAdapter::isConference(TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::removeCallFromConference(int callId)
{}
TPhoneViewResponseId PhoneUIQtViewAdapter::getSelectedConferenceMember(
        TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::setPrivateFromConference(int callId)
{}
void PhoneUIQtViewAdapter::setExpandActions()
{}
bool PhoneUIQtViewAdapter::callsExists(int call1State, int call2State)
{}
void PhoneUIQtViewAdapter::setParticipantListActions()
{}
void PhoneUIQtViewAdapter::setExpandedConferenceCallHeader()
{}
void PhoneUIQtViewAdapter::bringToForeground()
{}
void PhoneUIQtViewAdapter::showGlobalNote(
        TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::setDialpadVisibility(
        TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::removeAllCallHeaders()
{}
void PhoneUIQtViewAdapter::getNumberFromDialpad(
        TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::getDialpadStringLength(
        TPhoneCommandParam *commandParam)
{}
void PhoneUIQtViewAdapter::removeDialpad()
{}

void PhoneUIQtViewAdapter::dialpadClosed()
{}

void PhoneUIQtViewAdapter::keyReleased(QKeyEvent */*event*/)
{}

void PhoneUIQtViewAdapter::handleWindowActivated()
{}

void PhoneUIQtViewAdapter::handleWindowDeactivated()
{}
