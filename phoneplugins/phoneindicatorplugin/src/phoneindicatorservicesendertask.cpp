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

#include "phoneindicatorservicesendertask.h"

#include <xqaiwrequest.h>
#include <xqappmgr.h>
#include <logsservices.h>


PhoneIndicatorServiceSenderTask::PhoneIndicatorServiceSenderTask(Interaction interaction):
        m_interaction(interaction)
{
}

void PhoneIndicatorServiceSenderTask::run()
{    
    XQApplicationManager appManager;
    QList<QVariant> args;
    QString service;
    QString interface;
    QString operation;
    QVariantHash hash;
    QVariantMap map;
    
    switch(m_interaction){
        case OpenMissedCallView:
            service = "logs";
            interface = "com.nokia.symbian.ILogsView";
            operation = "show(QVariantMap)";
            map.insert("view_index",QVariant((int)LogsServices::ViewMissed));
            map.insert("show_dialpad",QVariant(false));
            map.insert("dialpad_text", QVariant(QString()));
            args.append(QVariant(map));
            break;
        case OpenCallUi:            
            service = "phoneui";
            interface = "com.nokia.symbian.IStart";
            operation = "start(int)";
            args << (int)0;
            break;
        case OpenDiverSettingsView:
            interface = "com.nokia.symbian.ICpPluginLauncher";
            operation = "launchSettingView(QString,QVariant)";
            args << QVariant("cptelephonyplugin.dll");            
            hash["view"] = "divert_view";
            hash["heading"] = "txt_phone_subhead_telephone";
            args << hash;
            break;
        default:            
            break;
    }
    QScopedPointer<XQAiwRequest> request( service.isEmpty() ? 
        appManager.create( interface, operation, false):
        appManager.create(service, interface, operation, false));
    if ( request == NULL ){
        return;       
    }   
    request->setArguments(args);
    request->send();
}
