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
* Description:  Animated icon.
*
*/

#ifndef BUBBLEDATA_H
#define BUBBLEDATA_H

#include <QObject>
#include <QDomDocument>

class BubbleData : public QObject
{
public:
    BubbleData( QObject *parent = 0 );

    void createDomFromFile(QIODevice *device);

    // generic
    QString dataField(const QString& fieldName);
    void setDataField(const QString& fieldName, const QString& value);

    // bubble related
    QString dataField(const QString&  bubble, const QString& fieldName);
    void setDataField(const QString&  bubble, const QString& fieldName, const QString& value);

    const QDomDocument& document() const;

private:
    QDomElement bubbleElement(const QString&  bubble);

private:
    QDomDocument mDomDocument;
};

#endif // BUBBLEDATA_H
