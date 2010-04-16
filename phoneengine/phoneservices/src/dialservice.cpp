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

#include <xqserviceutil.h>
#include <e32base.h>
#include <qregexp.h>
#include "dialservice.h"
#include "qtphonelog.h"


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
    PHONE_DEBUG2("DialService::dial number:", number);
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(number.utf16 ()));
    m_parameters.SetPhoneNumber (numberPtr);
    m_parameters.SetCallTypeCommand (EPECallTypeCSVoice);
    return m_call.HandleDialServiceCall ();
}

int DialService::dial(const QString& number, int contactId)
{
    PHONE_DEBUG4("DialService::dial number:", number, "contactId:", contactId);
    QString simplifiedNumber = simplified(number);
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(simplifiedNumber.utf16 ()));
    m_parameters.SetPhoneNumber (numberPtr);
    m_parameters.SetCallTypeCommand (EPECallTypeCSVoice);
    m_parameters.SetContactId2 (contactId);
    return m_call.HandleDialServiceCall (); 
}

void DialService::dialVideo(const QString& number)
{
    PHONE_DEBUG2("DialService::dialVideo number:", number);
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(number.utf16 ()));
    m_parameters.SetPhoneNumber (numberPtr);
    m_parameters.SetCallTypeCommand (EPECallTypeVideo);
    m_call.HandleDialServiceCall ();
}

void DialService::dialVideo(const QString& number, int contactId)
{
    PHONE_DEBUG4("DialService::dialVideo number:", number, "contactId:", contactId);
    QString simplifiedNumber = simplified(number);
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(simplifiedNumber.utf16 ()));
    m_parameters.SetPhoneNumber (numberPtr);
    m_parameters.SetCallTypeCommand (EPECallTypeVideo);
    m_parameters.SetContactId2 (contactId);
    m_call.HandleDialServiceCall ();    
}

void DialService::dialVoip(const QString& address)
{
    PHONE_DEBUG2("DialService::dialVoip number:", address);
    TPtrC16 numberPtr(reinterpret_cast<const TUint16*>(address.utf16 ()));
    m_parameters.SetPhoneNumber(numberPtr);
    m_parameters.SetCallTypeCommand(EPECallTypeVoIP);
    m_call.HandleDialServiceCall();    
}

void DialService::dialVoip(const QString& address, int contactId)
{
    PHONE_DEBUG4("DialService::dialVoip number:", address, "contactId:", contactId);
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(address.utf16 ()));
    m_parameters.SetPhoneNumber(numberPtr);
    m_parameters.SetCallTypeCommand(EPECallTypeVoIP);
    m_parameters.SetContactId2(contactId);
    m_call.HandleDialServiceCall();    
}

void DialService::dialVoipService(const QString& address, int serviceId)
{
    PHONE_DEBUG4("DialService::dialVoipService number:", address, "serviceId:", serviceId);
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(address.utf16 ()));
    m_parameters.SetPhoneNumber(numberPtr);
    m_parameters.SetCallTypeCommand(EPECallTypeVoIP);
    m_parameters.SetServiceIdCommand(serviceId);
    m_call.HandleDialServiceCall();    
}

void DialService::dialVoipService(
        const QString& address, int serviceId, int contactId)
{
    PHONE_DEBUG2("DialService::dialVoipService number:", address);
    PHONE_DEBUG2("DialService::dialVoipService serviceId:", serviceId);
    PHONE_DEBUG2("DialService::dialVoipService contactId:", contactId);
    TPtrC16 numberPtr (reinterpret_cast<const TUint16*>(address.utf16 ()));
    m_parameters.SetPhoneNumber(numberPtr);
    m_parameters.SetCallTypeCommand(EPECallTypeVoIP);
    m_parameters.SetServiceIdCommand(serviceId);
    m_parameters.SetContactId2(contactId);
    m_call.HandleDialServiceCall();    
}

QString DialService::simplified(const QString &number)
{
    QString simplifiedNumber = number;
    QRegExp rx(QString("[\\s,.\\[\\]\\(\\)\\-]"));
    simplifiedNumber.remove(rx);
    return simplifiedNumber;
}
