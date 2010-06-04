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
* Description:  Telephony Service API to be used through Qt Highway.
*
*/

#ifndef TELEPHONYSERVICE_H
#define TELEPHONYSERVICE_H

#include <QObject>
#include <QString>
#include <xqserviceprovider.h>

class PhoneUIQtViewAdapter;


class TelephonyService : public XQServiceProvider
{
    Q_OBJECT
public:
    TelephonyService(PhoneUIQtViewAdapter *phoneView, QObject *parent = 0);
    ~TelephonyService();
    
public slots:

    /*!
        \fn void start()
        
        Start launches telephony UI service.
        serviceId '0': Brings phoneapp to foreground.
        serviceId '1': Starts incall dialer, note if there is no ongoing calls and client uses value '1'
                       then dialer launches on top of empty call handling.
        
        Usage example:
        int inCallDialer(1);
        XQServiceRequest snd("com.nokia.services.telephonyservices.starter", "start(int)", false);
        snd << inCallDialer;
        QVariant err;
        snd.send(err);
    */
    void start(const int serviceId);
    
private:
    
    PhoneUIQtViewAdapter *m_viewAdapter;
};

#endif // TELEPHONYSERVICE_H
