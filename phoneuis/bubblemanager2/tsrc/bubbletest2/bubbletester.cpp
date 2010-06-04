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

#include <QLocalSocket>

#include "bubbletester.h"


BubbleTester::BubbleTester() : mLocalSocket(new QLocalSocket())
{
}

BubbleTester::~BubbleTester()
{
    mLocalSocket->disconnectFromServer();
    delete mLocalSocket;
}

bool BubbleTester::connectToServer()
{
    mLocalSocket->connectToServer("bubbletestserver");

    // This logic needs to be improved
    bool success = mLocalSocket->waitForConnected(500);

    qDebug() << "Socket Connect status: " << success;

    if (success) {
        connect(mLocalSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    }

    return success;
}

void BubbleTester::readData()        
{
    QByteArray inputByteArray = mLocalSocket->readAll();
    QDataStream inputDataStream(inputByteArray);
    QString textDocument;
    inputDataStream >> textDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!mDomDocument.setContent(textDocument, true, &errorStr, &errorLine,
                                &errorColumn)) {
        qDebug() << "Cannot read tester data!";
    } else {
        emit dataChanged();
    }
}

QList<QString> BubbleTester::bubbles()
{
    QDomNodeList list = mDomDocument.elementsByTagName("bubble");

    QList<QString> testBubbles;

    for (int i=0; i<list.count(); i++ ) {
        QDomNode node = list.at(i);
        if (node.isElement()) {
            QDomElement e = node.toElement();
            QString id = e.attribute("id");
            testBubbles.append(id);
        }
    }

    return testBubbles;
}

QString BubbleTester::dataField( const QString&  bubble, const QString& fieldName )
{
    QString text("");
    QDomElement elem = bubbleElement(bubble);

    if (!elem.isNull()) {
        QDomNodeList list = elem.elementsByTagName(fieldName);

        if (list.count()) {
            QDomNode node = list.at(0);
            if (node.isElement()) {
                QDomElement e = node.toElement();
                text = e.text();
            }
        }
    }

    return text;
}

QDomElement BubbleTester::bubbleElement(const QString&  bubble)
{
    QDomNodeList list = mDomDocument.elementsByTagName("bubble");

    QDomElement elem;

    for (int i=0; i<list.count(); i++ ) {
        QDomNode node = list.at(i);
        if (node.isElement()) {
            QDomElement e = node.toElement();
            QString id = e.attribute("id");
            if (id == bubble) {
                elem = e;
                break;
            }
        }
    }

    return elem;
}

QString BubbleTester::dataField(const QString& fieldName)
{
    QString text("");
    QDomNodeList list = mDomDocument.elementsByTagName(fieldName);

    if (list.count()) {
        QDomNode node = list.at(0);
        if (node.isElement()) {
            QDomElement e = node.toElement();
            text = e.text();
        }
    }

    return text;
}
