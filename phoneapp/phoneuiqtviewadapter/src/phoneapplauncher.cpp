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

#include <devicelockaccessapi.h>
#include <xqservicerequest.h>
#include <xqappmgr.h>
#include <xqrequestinfo.h>
#include <xqaiwdecl.h>

#include "phoneapplauncher.h"
#include "qtphonelog.h"

/*!
    PhoneAppLauncher::PhoneAppLauncher.
 */
PhoneAppLauncher::PhoneAppLauncher(QObject *parent)
    : QObject(parent)
{
    PHONE_TRACE;
}


/*!
    PhoneAppLauncher::~PhoneAppLauncher.
 */
PhoneAppLauncher::~PhoneAppLauncher()
{
    PHONE_TRACE;
}

/*!
    PhoneAppLauncher::launchMessaging.
 */
void PhoneAppLauncher::launchMessaging(
        const QString &toField,
        const QString &name,
        const QString &messageBody)
{
    QList<QVariant> arguments;
    arguments.append(QVariant(toField));
    arguments.append(QVariant(name));
    arguments.append(QVariant(messageBody));
    sendServiceRequest(
        "com.nokia.services.hbserviceprovider",
        "conversationview",
        "send(QString,QString,QString)",
        arguments,
        true);
}

/*!
    PhoneAppLauncher::launchContacts.
 */
void PhoneAppLauncher::launchContacts()
{
    QList<QVariant> arguments;
    sendServiceRequest(
        "com.nokia.services.phonebookappservices",
        "Launch",
        "launch()",
        arguments,
        true);
}

/*!
    PhoneAppLauncher::launchLogs.
 */
void PhoneAppLauncher::launchLogs(
        int activatedView,
        bool showDialpad,
        const QString &dialpadText)
{
    QVariantMap map;
    map.insert(XQLOGS_VIEW_INDEX, QVariant(activatedView));
    map.insert(XQLOGS_SHOW_DIALPAD, QVariant(showDialpad));
    map.insert(XQLOGS_DIALPAD_TEXT, QVariant(dialpadText));
    QList<QVariant> args;
    args.append(QVariant(map));
    sendServiceRequest(
        "logs",
        XQI_LOGS_VIEW,
        XQOP_LOGS_SHOW,
        args,
        false);
}

/*!
    PhoneAppLauncher::sendServiceRequest.
 */
void PhoneAppLauncher::sendServiceRequest(
        const QString &service, 
        const QString &interface,
        const QString &operation,
        const QList<QVariant> &arguments, 
        const bool foreground)
{
    int err = -1;
    TRAP_IGNORE(
        // Allow application launch only when device is unlocked
        // If locked ask devicelock query
        CDevicelockAccessApi *devicelockAccessApi = CDevicelockAccessApi::NewL();
        err = devicelockAccessApi->DisableDevicelock();
        delete devicelockAccessApi;
    )
    
    if ((err == KErrNone) || (err == KErrAlreadyExists)) {
        PHONE_TRACE3(service, operation, arguments);
        XQApplicationManager appManager;
        QScopedPointer<XQAiwRequest> request( 
            service.isEmpty() ? 
            appManager.create(interface, operation, false) :
            appManager.create(service, interface, operation, false) );
        if (request == NULL) {
            PHONE_TRACE1("service not found");
            return;
        }

        if (foreground) {
            XQRequestInfo info;
            info.setForeground(true);
            request->setInfo(info);
        }
        
        request->setArguments(arguments);
        QVariant retValue(-1);
        if (!request->send(retValue)) {
            int error = request->lastError();
            PHONE_TRACE2("send failed, error %d", request->lastError());
        }
    }
}

// end of file

