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
* Description:  Unit tests for PhoneNoteController.
*
*/

#ifndef T_XQSERVICEREQUEST_H
#define T_XQSERVICEREQUEST_H

#include <QObject>
#include <QVariant>
#include <QtTest/qtestcase.h>

extern QString mService;
extern QString mMessage;
extern bool mSend;
extern QList<QVariant> mList;


/*!
    \class XQServiceRequest
*/
class XQServiceRequest : public QObject
{
Q_OBJECT

public:

    XQServiceRequest(const QString &service,
                     const QString &message,
                     const bool &synchronous = true)
    {
        mService = service;
        mMessage = message;
    }

    ~XQServiceRequest()
    {
    }

    bool send(QVariant &retValue)
    {
        mSend = true;
        return mSend;
    }
    
    template <typename T>
    inline bool send(T& retValue)
    {
        QVariant retData;
        bool ret=send(retData);
        retValue = qVariantValue<T>(retData);
        return ret;
    }

    void setArguments(QList<QVariant> list)
    {
        mList = list;
    }

Q_SIGNALS:

    void requestCompleted(const QVariant &value);

};

#endif //T_XQSERVICEREQUEST_H
