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

#include "phonecallheadermanager.h"
#include "phonecallheaderutil.h"
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
#include "phonerssbase.h"
#include "cphonemediatorfactory.h"
#include "cphonemediatorsender.h"
#include "phoneviewcommanddefinitions.h"

#include <bubblemanagerif.h>
#include <pevirtualengine.h>
#include <mpeengineinfo.h>


//CONSTANTS


inline Qt::TextElideMode clipToElide(
    TPhoneCmdParamCallHeaderData::TPhoneTextClippingDirection clip)
    {
    return clip == TPhoneCmdParamCallHeaderData::ELeft ? 
                   Qt::ElideLeft : Qt::ElideRight;
    }

PhoneCallHeaderManager::PhoneCallHeaderManager(PhoneBubbleWrapper &bubbleWrapper, 
        PhoneUIQtViewIF &view, QObject *parent) :
    QObject(parent),
    m_bubbleWrapper(bubbleWrapper),
    m_view(view),
    m_callHeaderUtil(0),
    m_resourceAdapter(0)
{
    
}

PhoneCallHeaderManager::~PhoneCallHeaderManager ()
{
    delete m_callHeaderUtil;
}



void PhoneCallHeaderManager::setEngineInfo(MPEEngineInfo* engineInfo)
{
    m_engineInfo = engineInfo;
}

void PhoneCallHeaderManager::createCallHeader(
    int callId)
{
    TPhoneCmdParamCallHeaderData data;
    
    if (isIncoming(callId)) {
        callHeaderUtil()->SetIncomingCallHeaderParams( 
                callId, 
                isWaitingCall(callId),
                isVideoCall(callId),  
                &data );
    } else {
        callHeaderUtil()->SetOutgoingCallHeaderParams(
                callId,
                &data);
    }

    m_bubbleWrapper.bubbleManager().startChanges ();

    if (m_bubbleWrapper.bubbleManager().isConferenceExpanded())
        {
        m_bubbleWrapper.bubbleManager().setExpandedConferenceCallHeader(false);
        }

    int bubble = m_bubbleWrapper.createCallHeader(callId);
    m_bubbleWrapper.setState(callId, bubble, data.CallState ());        
    m_bubbleWrapper.setCli(bubble, data.CLIText(), clipToElide(data.CLITextClippingDirection()));
    m_bubbleWrapper.setServiceId(callId, data.ServiceId());
    m_bubbleWrapper.setSecondaryCli(bubble, data.CNAPText (), clipToElide(data.CNAPTextClippingDirection()));
    m_bubbleWrapper.setLabel(bubble, data.LabelText ());
    m_bubbleWrapper.setCallType(bubble, data.CallType ());
    m_bubbleWrapper.setDivert(bubble, data.Diverted ());
    m_bubbleWrapper.setCiphering(bubble, data.CipheringIndicatorAllowed(), data.Ciphering());

    if (data.Picture().Length()) {
        QString imagePath =
            QString::fromUtf16(data.Picture().Ptr(),data.Picture().Length());
        m_bubbleWrapper.bubbleManager().setCallObjectImage(bubble,imagePath);
    } else {
        m_bubbleWrapper.bubbleManager().setCallObjectFromTheme(bubble);
    }
    m_bubbleWrapper.bubbleManager().endChanges ();

    CPhoneMediatorFactory::Instance()->Sender()->SendEvent(EPhoneViewCreateCallHeader, callId, data);
}

void PhoneCallHeaderManager::createEmergencyCallHeader(int callId)
{
    m_bubbleWrapper.bubbleManager().startChanges ();
    int bubble = m_bubbleWrapper.createCallHeader (callId);
    m_bubbleWrapper.setLabel(bubble, callHeaderUtil()->AttemptingEmergencyText());
    m_bubbleWrapper.setCli (bubble, callHeaderUtil()->EmergencyHeaderText(), Qt::ElideRight);
    m_bubbleWrapper.setState(callId, bubble, EPEStateDialing);
    m_bubbleWrapper.setCiphering(bubble, m_engineInfo->SecureSpecified(), m_engineInfo->ServiceId(callId));
    m_bubbleWrapper.bubbleManager().endChanges ();

}

void PhoneCallHeaderManager::removeCallHeader(int callId)
{
    m_bubbleWrapper.bubbleManager().startChanges();
    int bubble = m_bubbleWrapper.bubbles().value(callId, -1);
    if ( bubble != -1 ) {
        m_view.clearBubbleCommands(bubble);
        m_view.removeExpandAction(bubble);
        m_bubbleWrapper.removeCallHeader(callId);
    }
    m_bubbleWrapper.bubbleManager().endChanges();
}

void PhoneCallHeaderManager::updateCallHeaderState(int callId)
{

    int bubble = m_bubbleWrapper.bubbleId(callId);
    if ( -1 != bubble ) {
        m_bubbleWrapper.bubbleManager ().startChanges ();
        m_bubbleWrapper.setState(callId, bubble, callHeaderUtil()->CallState(callId));
        m_bubbleWrapper.setLabel(bubble, callHeaderUtil()->LabelText(callId));
        m_bubbleWrapper.setDivert(bubble, m_engineInfo->IncomingCallForwarded(callId));
        m_bubbleWrapper.bubbleManager().endChanges ();
    }
}

void PhoneCallHeaderManager::updateCallHeaderRemoteInfo(int callId)
{

    TPhoneCmdParamCallHeaderData data;

     int bubble = m_bubbleWrapper.bubbleId(callId);
     if ( -1 != bubble ) {
         callHeaderUtil()->UpdateCallHeaderInfo( 
                 callId, 
                 isWaitingCall(callId),
                 isVideoCall(callId),  
                 &data );
         
         m_bubbleWrapper.bubbleManager().startChanges();
         m_bubbleWrapper.setCli(bubble, data.CLIText(),
             clipToElide(data.CLITextClippingDirection()));
         m_bubbleWrapper.setSecondaryCli(bubble, data.CNAPText (),
             clipToElide(data.CNAPTextClippingDirection()));
         m_bubbleWrapper.setDivert(bubble, m_engineInfo->IncomingCallForwarded(callId));
         m_bubbleWrapper.bubbleManager().endChanges ();
     }

}

void PhoneCallHeaderManager::updateCallHeaderRemoteInfoAndLabel (int callId)
{
    TPhoneCmdParamCallHeaderData data;

     int bubble = m_bubbleWrapper.bubbleId(callId);
     if ( -1 != bubble ) {
         callHeaderUtil()->UpdateCallHeaderInfo( 
                 callId, 
                 isWaitingCall(callId),
                 isVideoCall(callId),  
                 &data );
         
         m_bubbleWrapper.bubbleManager().startChanges ();
         m_bubbleWrapper.setCli(bubble, data.CLIText (),
             clipToElide(data.CLITextClippingDirection()));
         m_bubbleWrapper.setSecondaryCli(bubble, data.CNAPText (),
             clipToElide(data.CNAPTextClippingDirection()));
         m_bubbleWrapper.setLabel(bubble, data.LabelText ());
         m_bubbleWrapper.setDivert(bubble, data.Diverted ());
         m_bubbleWrapper.bubbleManager().endChanges ();
     }
}

void PhoneCallHeaderManager::handleCipheringInfoChange(int callId)
{
    if (callId == KPEConferenceCallID) {
        bool secure(true);
        for (int i=0; i<m_bubbleWrapper.conferenceCallList().count(); i++) {
            if ( false == m_engineInfo->IsSecureCall(m_bubbleWrapper.conferenceCallList().at(i)) ) {
                secure = false;
                break;
            }
        }
        m_bubbleWrapper.bubbleManager().startChanges();
        m_bubbleWrapper.setCiphering(m_bubbleWrapper.bubbleId(callId),
                                     m_engineInfo->SecureSpecified(),
                                     secure);
        m_bubbleWrapper.bubbleManager().endChanges();
    } else {
        int bubble = m_bubbleWrapper.bubbleId(callId);        
        if ( -1 != bubble ) {        
            m_bubbleWrapper.bubbleManager().startChanges();
            m_bubbleWrapper.setCiphering(bubble,
                                         m_engineInfo->SecureSpecified(),
                                         m_engineInfo->IsSecureCall(callId));
            m_bubbleWrapper.bubbleManager().endChanges();
        }
    }
}




void PhoneCallHeaderManager::createConferenceBubble(int callId)
{
    m_bubbleWrapper.bubbleManager().startChanges();
    TBuf<KPhoneCallHeaderLabelMaxLength> conferenceText( KNullDesC );
    callHeaderUtil()->LoadResource(conferenceText, EPhoneCLIConferenceCall);
    int bubble = m_bubbleWrapper.createConferenceBubble(
            callId, callHeaderUtil()->CallState(callId), callHeaderUtil()->LabelText(callId), conferenceText);
    
    m_bubbleWrapper.setServiceId(callId, m_engineInfo->ServiceId(callId));
    m_bubbleWrapper.setCiphering(bubble, m_engineInfo->SecureSpecified(), m_engineInfo->IsSecureCall(callId));
    m_bubbleWrapper.bubbleManager().endChanges();
}


void PhoneCallHeaderManager::removeConferenceBubble()
{
    m_bubbleWrapper.bubbleManager().startChanges();
    m_view.removeExpandAction(m_bubbleWrapper.bubbleId(KConferenceCallId));
    m_view.clearParticipantListActions();
    m_bubbleWrapper.removeConferenceBubble();
    m_bubbleWrapper.bubbleManager().endChanges();
}

void PhoneCallHeaderManager::removeCallFromConference(int callId)
{
    m_bubbleWrapper.bubbleManager().startChanges();
    m_bubbleWrapper.removeCallFromConference(callId);
    int bubbleId = m_bubbleWrapper.bubbles().value(callId);
    m_view.clearBubbleCommands(bubbleId);
    m_view.removeExpandAction(bubbleId);
    m_bubbleWrapper.removeCallHeader(callId);
    m_bubbleWrapper.bubbleManager().endChanges();
}


void PhoneCallHeaderManager::setPrivateFromConference(int callId)
{
    m_bubbleWrapper.bubbleManager().startChanges();
    m_bubbleWrapper.bubbleManager().setExpandedConferenceCallHeader(false);
    m_bubbleWrapper.removeCallFromConference(callId);
    m_view.clearBubbleCommands(m_bubbleWrapper.bubbles().value(callId));
    m_bubbleWrapper.bubbleManager().endChanges();
}


void PhoneCallHeaderManager::setExpandedConferenceCallHeader()
{
    int callId = -1;

    if ( 1==m_bubbleWrapper.bubbles().keys().size()
         || (1<m_bubbleWrapper.bubbles().keys().size()
         && false == m_bubbleWrapper.callStates().values().contains(EPEStateRinging)
         && false == m_bubbleWrapper.callStates().values().contains(EPEStateDialing)
         && false == m_bubbleWrapper.callStates().values().contains(EPEStateConnecting)
         && false == m_bubbleWrapper.callStates().values().contains(EPEStateHeldConference))) {

        int bubbleId = m_bubbleWrapper.bubbleManager().expandedBubble();
        callId = m_bubbleWrapper.callIdByBubbleId(bubbleId);
    }

    m_bubbleWrapper.bubbleManager().startChanges();
    m_bubbleWrapper.bubbleManager().setExpandedConferenceCallHeader(
            (KConferenceCallId == callId));
    m_bubbleWrapper.bubbleManager().endChanges();
}


void PhoneCallHeaderManager::removeAllCallHeaders()
{
    if (m_bubbleWrapper.conferenceCallList().size()) {
        removeConferenceBubble();
    }

    QList<int> callIds = m_bubbleWrapper.bubbles().keys();

    for (int i=0; i<callIds.size(); ++i) {
        int callId = callIds.at(i);
        if (KEmergencyCallId != callId) {
            m_bubbleWrapper.bubbleManager().startChanges();
            int bubble = m_bubbleWrapper.bubbles().value(callId);
            m_view.clearBubbleCommands(bubble);
            m_view.removeExpandAction(bubble);
            m_bubbleWrapper.removeCallHeader (callId);
            m_bubbleWrapper.bubbleManager().endChanges();
        }
    }
}

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::IsVoiceCall
// ---------------------------------------------------------------------------
//
bool PhoneCallHeaderManager::isVoiceCall(int callId) const
    {
    if( callId < 0 )
        {
        return ( m_engineInfo->CallTypeCommand()
                    == EPECallTypeCSVoice ||
                m_engineInfo->CallTypeCommand()
                    == EPECallTypeVoIP );  
        }
    
    return ( m_engineInfo->CallType( callId )
                == EPECallTypeCSVoice ||
             m_engineInfo->CallType( callId )
                == EPECallTypeVoIP );
    }


// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::IsVideoCall
// ---------------------------------------------------------------------------
//
bool PhoneCallHeaderManager::isVideoCall(int callId) const
    {
    if( callId < 0 )
        {
        return ( m_engineInfo->CallTypeCommand()
            == EPECallTypeVideo );  
        }
    return ( m_engineInfo->CallType( callId )
            == EPECallTypeVideo );
    }

bool PhoneCallHeaderManager::isIncoming(int callId) const
    {
    return callId >= 0 && 
           m_engineInfo->CallState( callId ) == EPEStateRinging;
    }

bool PhoneCallHeaderManager::isWaitingCall(int callId) const
    {
    bool waiting( EFalse );
    if ( callId >= 0 && 
         m_engineInfo->CallState( callId ) == EPEStateRinging )
        {
        if( m_bubbleWrapper.activeCallCount() > 0 )
            {
            waiting = ETrue;
            }
        }
    return waiting;
    }

PhoneCallHeaderUtil *PhoneCallHeaderManager::callHeaderUtil()
{
    if (!m_callHeaderUtil) {
        m_callHeaderUtil = new PhoneCallHeaderUtil(*m_engineInfo);
    }

    return m_callHeaderUtil;
}


