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
* Description:  dial service
*
*/

#include <QDebug>
#include <xqserviceutil.h>
#include <e32base.h>
#include "dialservice.h"

DialService::DialService(MPECallControlIF &call, MPECallSettersIF &parameters, QObject* parent) : 
    XQServiceProvider(QLatin1String("com.nokia.services.telephony"), parent), m_call (call), m_parameters (parameters)
{
    publishAll();
}

DialService::~DialService()
{
}

int DialService::dial(const QString& number)
{
    qDebug () << "DialService::dial number:" << number;
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(number.utf16 ()));
    m_parameters.SetPhoneNumber (numberPtr);
    m_parameters.SetCallTypeCommand (EPECallTypeCSVoice);
    return m_call.HandleDialServiceCall ();
}

int DialService::dial(const QString& number, int contactId)
{
    qDebug () << "DialService::dial number:" << number;
    qDebug () << "DialService::dial contactId:" << contactId;
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(number.utf16 ()));
    m_parameters.SetPhoneNumber (numberPtr);
    m_parameters.SetCallTypeCommand (EPECallTypeCSVoice);
    m_parameters.SetContactId2 (contactId);
    return m_call.HandleDialServiceCall (); 
}

void DialService::dialVideo(const QString& number)
{
    qDebug () << "DialService::dialVideo number:" << number;
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(number.utf16 ()));
    m_parameters.SetPhoneNumber (numberPtr);
    m_parameters.SetCallTypeCommand (EPECallTypeVideo);
    m_call.HandleDialServiceCall ();
}

void DialService::dialVideo(const QString& number, int contactId)
{
    qDebug () << "DialService::dialVideo number:" << number;
    qDebug () << "DialService::dialVideo contactId:" << contactId;
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(number.utf16 ()));
    m_parameters.SetPhoneNumber (numberPtr);
    m_parameters.SetCallTypeCommand (EPECallTypeVideo);
    m_parameters.SetContactId2 (contactId);
    m_call.HandleDialServiceCall ();    
}

void DialService::dialVoip(const QString& address)
{
    qDebug () << "DialService::dialVoip number:" << address;
    TPtrC16 numberPtr(reinterpret_cast<const TUint16*>(address.utf16 ()));
    m_parameters.SetPhoneNumber(numberPtr);
    m_parameters.SetCallTypeCommand(EPECallTypeVoIP);
    m_call.HandleDialServiceCall();    
}

void DialService::dialVoip(const QString& address, int contactId)
{
    qDebug () << "DialService::dialVoip number:" << address;
    qDebug () << "DialService::dialVoip contactId:" << contactId;
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(address.utf16 ()));
    m_parameters.SetPhoneNumber(numberPtr);
    m_parameters.SetCallTypeCommand(EPECallTypeVoIP);
    m_parameters.SetContactId2(contactId);
    m_call.HandleDialServiceCall();    
}

void DialService::dialVoipService(const QString& address, int serviceId)
{
    qDebug () << "DialService::dialVoipService number:" << address;
    qDebug () << "DialService::dialVoipService serviceId:" << serviceId;
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(address.utf16 ()));
    m_parameters.SetPhoneNumber(numberPtr);
    m_parameters.SetCallTypeCommand(EPECallTypeVoIP);
    m_parameters.SetServiceIdCommand(serviceId);
    m_call.HandleDialServiceCall();    
}

void DialService::dialVoipService(
        const QString& address, int serviceId, int contactId)
{
    qDebug () << "DialService::dialVoipService number:" << address;
    qDebug () << "DialService::dialVoipService serviceId:" << serviceId;
    qDebug () << "DialService::dialVoipService contactId:" << contactId;
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(address.utf16 ()));
    m_parameters.SetPhoneNumber(numberPtr);
    m_parameters.SetCallTypeCommand(EPECallTypeVoIP);
    m_parameters.SetServiceIdCommand(serviceId);
    m_parameters.SetContactId2(contactId);
    m_call.HandleDialServiceCall();    
}
