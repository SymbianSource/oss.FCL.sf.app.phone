/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  
 *
 */

#include "phoneindicatorinterface.h"
#include "phoneindicators.h"

#include <QTime>
#include <QStringList> 
#ifdef Q_OS_SYMBIAN
#include <logsservices.h>
#include <xqservicerequest.h>
#include <xqrequestinfo.h>
#include <apgtask.h>
#include <xqaiwrequest.h>
#include <cppluginlauncher.h>


#endif

PhoneIndicatorInterface::PhoneIndicatorInterface(
                const QString &indicatorType,
                int typeIndex,
                Interaction interaction) :
        HbIndicatorInterface( indicatorType, HbIndicatorInterface::NotificationCategory,
        (interaction == InteractionNone) ? NoInteraction : InteractionActivated),
        m_typeIndex(typeIndex),
        m_interaction(interaction),
        m_primaryText(IndicatorInfos[typeIndex].primaryText),
        m_secondaryText(IndicatorInfos[typeIndex].secondaryText),
        m_icon(IndicatorInfos[typeIndex].icon)

{
}

PhoneIndicatorInterface::~PhoneIndicatorInterface()
{

}

bool PhoneIndicatorInterface::handleInteraction(InteractionType type)
{
    if (type == InteractionActivated) {
        switch (m_interaction) {            
        case OpenMissedCallView: {
#ifdef Q_OS_SYMBIAN
            XQServiceRequest snd("com.nokia.services.logsservices.starter",
                                 "start(int,bool)", false);
            snd << (int)LogsServices::ViewMissed;
            snd << false;
            int retValue;
            snd.send(retValue);
#endif
            }
            break;
        case SwitchBackToCall: {
#ifdef Q_OS_SYMBIAN
            int dialer(1);
            XQServiceRequest snd("com.nokia.services.telephony.uistarter", 
                    "start(int)", false);
            XQRequestInfo info;
            info.setForeground(true);
            snd.setInfo(info);
            snd << dialer;
            QVariant retValue;
            snd.send(retValue);
#endif
            }
            break;
        case OpenDiverSettingsView: {
            launchDivertSettingsView();
            }
            break;
        case Deactivate:
            emit deactivate();
            break;
        default:
            return false;
        }
        emit dataChanged();
    }
    return false;
}

QVariant PhoneIndicatorInterface::indicatorData(int role) const
{
    QVariantMap map = m_parameter.value<QVariantMap>();
    
    if (role == PrimaryTextRole) {
        return map.value( (QVariant(PrimaryTextRole)).toString()).toString();
    } else if (role == SecondaryTextRole ) {
        return map.value( (QVariant(SecondaryTextRole)).toString()).toString();
    } else if (role == MonoDecorationNameRole) {
        return m_icon;
    } else if (role == DecorationNameRole) {
        return map.value( (QVariant(DecorationNameRole)).toString()).toString();
    }
    return QVariant();
}

bool PhoneIndicatorInterface::handleClientRequest(RequestType type, const QVariant &parameter)
{
    bool handled(false);
    switch (type) {        
    case RequestActivate:
        if (m_parameter != parameter) {
            m_parameter = parameter;
            emit dataChanged();
        }
        handled =  true;
        break;
    default:
        m_parameter.clear();
    }

    return handled;
}

void PhoneIndicatorInterface::launchDivertSettingsView()
{

    XQAiwRequest *request = m_appMgr.create(
            "com.nokia.symbian.ICpPluginLauncher", 
            "launchSettingView(QString,QVariant)", 
            false);  // 
    
    if (!request) {
        return;
    }
    else {
        connect(request, SIGNAL(requestOk(QVariant)), SLOT(handleReturnValue(QVariant)));
        connect(request, SIGNAL(requestError(int,QString)), SLOT(handleError(int,QString)));
    }
    // Set arguments for request 
    QList<QVariant> args;
    args << (QString)"cptelephonyplugin.dll";
    args << (QVariant)"";            

    request->setArguments(args);
    request->setSynchronous(true);
    // Make the request
    if (!request->send()) {
        //report error     
    }
    delete request;

}

void PhoneIndicatorInterface::handleReturnValue(const QVariant &returnValue)
{      
    Q_UNUSED(returnValue);
}

void PhoneIndicatorInterface::handleError(int errorCode,const QString &errorMessage)
{
    Q_UNUSED(errorCode);
    Q_UNUSED(errorMessage);
}
