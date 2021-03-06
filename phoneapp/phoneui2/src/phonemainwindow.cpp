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
* Description:  Main window class.
*
*/

#include <QEvent>
#include "phonemainwindow.h"
#include "phoneuihousehold.h"

#include "qtphonelog.h"

PhoneMainWindow::PhoneMainWindow(QWidget *parent): 
    HbMainWindow(parent)
{
    mHouseHold = new PhoneUiHouseHold (*this);
}

PhoneMainWindow::~PhoneMainWindow ()
{
    delete mHouseHold;
}

void PhoneMainWindow::onFocusLost()
{
    emit focusLost();
}
void PhoneMainWindow::onFocusGained()
{
    emit focusGained();
}

void PhoneMainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LocaleChange) {
        PHONE_DEBUG("PhoneMainWindow::changeEvent, language changed");
        mHouseHold->reloadTranslators();
    }
    HbMainWindow::changeEvent(event);
}

