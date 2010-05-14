/*
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
* Description: 
*
*/

#ifndef XQSERVICEREQUEST_H
#define XQSERVICEREQUEST_H

#include <QVariant>
#include <QString>

class XQServiceRequest : public QObject
{
    Q_OBJECT

public:
    XQServiceRequest(QString const& api, QString const& method, bool const& sync = true);
    ~XQServiceRequest();
    bool send();
    bool send(QVariant& retValue);
    void setSynchronous(const bool &synchronous);

signals:
    void requestCompleted(const QVariant& value) ;
    void requestError(int err);
};
#endif
