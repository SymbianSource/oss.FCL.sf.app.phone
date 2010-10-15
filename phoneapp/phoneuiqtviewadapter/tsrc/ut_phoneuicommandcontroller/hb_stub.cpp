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
* Description:
*
*/

#include <hbaction.h>


HbAction::HbAction(QObject *parent)
     : QAction(parent), d_ptr(0)
{
}

HbAction::HbAction(const QString &text, QObject *parent)
     : QAction(text, parent), d_ptr(0)
{
}

HbAction::~HbAction() {}
void HbAction::setIcon(const HbIcon &icon) {}

#include <hbicon.h>

class HbIconPrivate
{
 public:
     HbIconPrivate(){};
     ~HbIconPrivate(){};
     
     QAtomicInt ref;
};

HbIcon::HbIcon() {};
HbIcon::HbIcon(const QString& string) {};
HbIcon::~HbIcon() {};
   
#include <qgraphicswidget.h>

void QGraphicsWidget::addAction(QAction *action)
{
}

void QGraphicsWidget::removeAction(QAction *action)
{
}

