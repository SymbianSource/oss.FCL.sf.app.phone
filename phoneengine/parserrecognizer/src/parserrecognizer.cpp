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
* Description:  Recognizes the parser messages that needs to be notified to the world
*               using QtHighway.
*
*/

#include <xqservicerequest.h>
#include <pevirtualengine.h>
#include "parserrecognizer.h"
#include "qtphonelog.h"

ParserRecognizer::ParserRecognizer(QObject* parent) : QObject (parent)
{
}

ParserRecognizer::~ParserRecognizer()
{
}

void ParserRecognizer::sendMessage(const int message, const int callId)
{
    Q_UNUSED(callId); // for now
    PHONE_DEBUG2("ParserRecognizer::sendMessage message:", message);
    QString api;
    QString method;
    bool recognized = true;
    
    switch(message) {
    case MEngineMonitor::EPEMessageActivateRfsDeep:
        api = "com.nokia.services.telephony";
        method = "activateDeepRestoreFactorySettings()";
        break;
        
    case MEngineMonitor::EPEMessageActivateRfsNormal:
        api = "com.nokia.services.telephony";
        method = "activateNormalRestoreFactorySettings()";
        break;
        
    case MEngineMonitor::EPEMessageActivateWarrantyMode:
        api = "com.nokia.services.telephony";
        method = "activateWarrantyMode()";
        break;
        
    case MEngineMonitor::EPEMessageShowBTDeviceAddress:
        api = "com.nokia.services.bluetooth";
        method = "showBluetoothDeviceAddress()";
        break;
        
    case MEngineMonitor::EPEMessageShowBTLoopback:
        api = "com.nokia.services.bluetooth";
        method = "showBluetoothLoopback()";
        break;
        
    case MEngineMonitor::EPEMessageBTDebugMode:
        api = "com.nokia.services.bluetooth";
        method = "activateBluetoothDebugMode()";
        break;

    case MEngineMonitor::EPEMessageShowIMEI:
        api = "com.nokia.services.telephony";
        method = "showIMEICode()";
        break;
        
    case MEngineMonitor::EPEMessageShowVersion:
        api = "com.nokia.services.telephony";
        method = "showVersionNumber()";
        break;
        
    case MEngineMonitor::EPEMessageShowWlanMacAddress:
        api = "com.nokia.services.wlan";
        method = "showWLANMacAddress()";
        break;
        
    case MEngineMonitor::EPEMessageSSRequestFailed:
        api = "com.nokia.services.telephony";
        method = "supplementaryServiceRequestFailed()";
        break;
              
    default:
      recognized = false;
      break;        
    }
    
    if(recognized) {
        PHONE_DEBUG2("ParserRecognizer::sendMessage api:", api);
        PHONE_DEBUG2("ParserRecognizer::sendMessage method:", method);
        XQServiceRequest snd(api, method);
        QVariant err;
        snd.send(err);
    }
}
