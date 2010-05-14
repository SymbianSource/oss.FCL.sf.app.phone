/*!
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Recognizes the parser messages that needs to be notified to 
*               the world using QtHighway.
*/

#include <xqservicerequest.h>
#include <pevirtualengine.h>
#include "parserrecognizer.h"
#include "qtphonelog.h"

ParserRecognizer::ParserRecognizer(QObject* parent) 
    : 
    QObject(parent),
    m_currentRequest(0)
{
}

ParserRecognizer::~ParserRecognizer()
{
    delete m_currentRequest;
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
    
    case MEngineMonitor::EPEMessageShowVersion:
        api = "com.nokia.services.devicemanager";
        method = "showVersionNumber()";
        break;
    
    case MEngineMonitor::EPEMessageSSRequestFailed:
        api = "com.nokia.services.telephony";
        method = "supplementaryServiceRequestFailed()";
        break;
    
    default:
      recognized = false;
      break;        
    }
    
    if (recognized && (!m_currentRequest)) {
        PHONE_DEBUG2("ParserRecognizer::sendMessage api:", api);
        PHONE_DEBUG2("ParserRecognizer::sendMessage method:", method);
        m_currentRequest = new XQServiceRequest(api, method, false);
        // Due to a Qt Highway bug in assignment operator implementation we 
        // need to set request as asynchronous with a setter function.
        m_currentRequest->setSynchronous(false);
        connect(
            m_currentRequest, SIGNAL(requestCompleted(const QVariant &)), 
            this, SLOT(requestCompleted(const QVariant &)));
        connect(
            m_currentRequest, SIGNAL(requestError(int)), 
            this, SLOT(requestError(int)));
        
        int exceptionAsError = 0;
        bool requestOk = false;
        QT_TRYCATCH_ERROR(
            exceptionAsError, requestOk = m_currentRequest->send());
        if ((0 != exceptionAsError) || (!requestOk)) {
            PHONE_DEBUG2("ParserRecognizer::sendMessage exceptionAsError:", 
                exceptionAsError);
            PHONE_DEBUG2("ParserRecognizer::sendMessage requestOk:", 
                requestOk);
            requestCompleted(QVariant());
        }
    }
}

void ParserRecognizer::requestCompleted(const QVariant &returnValue)
{
    PHONE_DEBUG("ParserRecognizer::requestCompleted");
    Q_UNUSED(returnValue);
    
    delete m_currentRequest;
    m_currentRequest = NULL;
}

void ParserRecognizer::requestError(int error)
{
    PHONE_DEBUG2("ParserRecognizer::requestError", error);
    
    delete m_currentRequest;
    m_currentRequest = NULL;
}
