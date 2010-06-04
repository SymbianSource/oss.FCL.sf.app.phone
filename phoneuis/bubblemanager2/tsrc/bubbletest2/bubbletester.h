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

#ifndef BUBBLETESTER_H
#define BUBBLETESTER_H

#include <QObject>
#include <QDomDocument>

class QLocalSocket;

class BubbleTester : public QObject
{
    Q_OBJECT

public:
    BubbleTester();
    ~BubbleTester();

    bool connectToServer();

    QList<QString> bubbles();
    QString dataField(const QString& fieldName);
    QString dataField(const QString&  bubble, const QString& fieldName);
    QDomElement bubbleElement(const QString&  bubble);

signals:
    void dataChanged();

private slots:
    void readData();

private:
    QLocalSocket* mLocalSocket;
    QDomDocument  mDomDocument;
};

#endif // BUBBLETESTER_H
