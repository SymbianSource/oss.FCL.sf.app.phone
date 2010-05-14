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
* Description:  Dial API to be used through Qt Highway.
*
*/

#ifndef DIALSERVICE_H
#define DIALSERVICE_H

#include <QObject>
#include <QString>
#include <xqserviceprovider.h>
#include "mpecallcontrolif.h"
#include "mpecallsettersif.h"

class DialService : public XQServiceProvider
{
    Q_OBJECT
public:
    DialService(MPECallControlIF &call, MPECallSettersIF &parameters, QObject *parent = 0);
    ~DialService();
    
public slots:
    /*!
        \fn dial(const QString& number)
        
        This method makes a cellular switched dial command to
        Phone Application. It is intended to be used via Qt Highway.
        
        Usage example:
        XQServiceRequest snd("com.nokia.symbian.ICallDial","dial(QString)");
        snd << "0501234567";        
    */
    int dial(const QString& number);
    
    /*!
        \fn dial(const QString& number)
        
        This method makes a cellular switched dial command to
        Phone Application. It is intended to be used via Qt Highway.
        Caller's name is shown according to the given Phonebook contact
        identifier.
        
        Usage example:
        XQServiceRequest snd("com.nokia.symbian.ICallDial","dial(QString,int)");
        snd << "0501234567" << 123456;        
    */
    int dial(const QString& number, int contactId);
    
    /*!
        \fn dialVideo(const QString& number)
        
        This method makes a video call dial command to
        Phone Application. It is intended to be used via Qt Highway.
        
        Usage example:
        XQServiceRequest snd("com.nokia.symbian.ICallDial","dial(QString)");
        snd << "0501234567";        
    */
    void dialVideo(const QString& number);

    /*!
        \fn dial(const QString& number)
        
        This method makes a video call dial command to
        Phone Application. It is intended to be used via Qt Highway.
        Caller's name is shown according to the given Phonebook contact
        identifier.
        
        Usage example:
        XQServiceRequest snd("com.nokia.symbian.ICallDial","dial(QString,int)");
        snd << "0501234567" << 123456;        
    */
    void dialVideo(const QString& number, int contactId);
    
    /*!
        \fn dialVoip(const QString& address)
        
        This method makes a voip call dial command to
        Phone Application. It is intended to be used via Qt Highway.
        
        Usage example:
        XQServiceRequest snd("com.nokia.symbian.ICallDial","dialVoip(QString)");
        snd << "address@domain";        
    */
    void dialVoip(const QString& address);
    
    /*!
        \fn dialVoip(const QString& address, int contactId)
        
        This method makes a voip call dial command to
        Phone Application. It is intended to be used via Qt Highway.
        Caller's name is shown according to the given Phonebook contact
        identifier.
        
        Usage example:
        XQServiceRequest snd("com.nokia.symbian.ICallDial","dialVoip(QString,int)");
        snd << "address@domain" << 123456;        
    */
    void dialVoip(const QString& address, int contactId);
    
    /*!
        \fn dialVoipService(const QString& address, int contactId)
        
        This method makes a voip call dial command to
        Phone Application. It is intended to be used via Qt Highway.
        Call is made by given service (id).
        
        Usage example:
        XQServiceRequest snd("com.nokia.symbian.ICallDial","dialVoipService(QString,int)");
        snd << "address@domain" << 123;        
    */
    void dialVoipService(const QString& address, int serviceId);
    
    /*!
        \fn dialVoipService(const QString& address, int serviceId, int contactId)
        
        This method makes a voip call dial command to
        Phone Application. It is intended to be used via Qt Highway.
        Call is made by given service (id).
        Caller's name is shown according to the given Phonebook contact
        identifier.
        
        Usage example:
        XQServiceRequest snd("com.nokia.symbian.ICallDial","dialVoipService(QString,int,int)");
        snd << "address@domain" << 123 << 12345;        
    */
    void dialVoipService(const QString& address, int serviceId, int contactId);
    
private:
    /*!
        \fn QString simplified(QString& number)
        
        Returns a string that has whitespaces, '(', ')', '-', '[', and ']' chars removed 
    */
    static QString simplified(const QString &number) Q_REQUIRED_RESULT;


private:
    MPECallControlIF &m_call;
    MPECallSettersIF &m_parameters;    
};

#endif // DIALERSERVICE_H
