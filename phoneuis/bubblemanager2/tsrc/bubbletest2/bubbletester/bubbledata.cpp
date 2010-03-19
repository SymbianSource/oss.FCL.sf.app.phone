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

#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include "bubbledata.h"

BubbleData::BubbleData( QObject *parent ) : QObject(parent)
{
    QFile file(":/testdata.xml");

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical((QWidget*)parent, tr("Bubble tester"),
                              tr("Cannot read testdata.xml") );
        return;
    }

    createDomFromFile(&file);
}

void BubbleData::createDomFromFile(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!mDomDocument.setContent(device, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information((QWidget*)parent(), tr("Bubble tester"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
    }
}

QString BubbleData::dataField( const QString&  bubble, const QString& fieldName )
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

QDomElement BubbleData::bubbleElement(const QString&  bubble)
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

void BubbleData::setDataField( const QString&  bubble, const QString& fieldName, const QString& value )
{
    QDomElement elem = bubbleElement(bubble);
    if (elem.isNull()) {
        return;
    }

    QDomText domText = mDomDocument.createTextNode(value);
    QDomElement newElem = mDomDocument.createElement(fieldName);
    newElem.appendChild(domText);

    QDomNodeList list = elem.elementsByTagName(fieldName);

    if (list.count()) {
        QDomNode node = list.at(0);
        if (node.isElement()) {
            QDomNode parent = node.parentNode();
            parent.replaceChild(newElem,node);
        }
    }
}

QString BubbleData::dataField(const QString& fieldName)
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

void BubbleData::setDataField(const QString& fieldName, const QString& value)
{
    QDomText domText = mDomDocument.createTextNode(value);
    QDomElement newElem = mDomDocument.createElement(fieldName);
    newElem.appendChild(domText);

    QDomNodeList list = mDomDocument.elementsByTagName(fieldName);

    if (list.count()) {
        QDomNode node = list.at(0);
        if (node.isElement()) {
            QDomNode parent = node.parentNode();
            parent.replaceChild(newElem,node);
        }
    }

}

const QDomDocument& BubbleData::document() const
{
    return mDomDocument;
}

