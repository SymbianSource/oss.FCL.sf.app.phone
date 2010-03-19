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
* Description:  Adapter for converting Qt key event types to
*               Symbian types.
*
*/

#include <exception> // must be before e32base.h so uncaught_exception gets defined
#include "phoneuikeyeventadapter.h"
#include "phoneconstants.h"
#include <QKeyEvent>

PhoneUIKeyEventAdapter::PhoneUIKeyEventAdapter (MPhoneKeyEventHandler &keyEventHandler, QObject *parent) :
    QObject (parent), m_handler (keyEventHandler)
{

}

PhoneUIKeyEventAdapter::~PhoneUIKeyEventAdapter ()
{
    
}

void PhoneUIKeyEventAdapter::keyPressed (QKeyEvent *event)
{
    forwardKeyEvent(EEventKey, event);
}

void PhoneUIKeyEventAdapter::keyReleased (QKeyEvent *event)
{
    forwardKeyEvent(EEventKeyUp, event);    
}

void PhoneUIKeyEventAdapter::forwardKeyEvent(
        TEventCode symbianEventCode, QKeyEvent *event)
{
    TKeyEvent symbianKeyEvent;
    symbianKeyEvent.iCode = convertKeyCode(event);
    symbianKeyEvent.iRepeats = 0;
    symbianKeyEvent.iScanCode = convertKeyCode(event);
    
    TRAP_IGNORE(m_handler.HandleKeyEventL (symbianKeyEvent, symbianEventCode));
    
}

int PhoneUIKeyEventAdapter::convertKeyCode(QKeyEvent *event)
{
    int code(0);
    
    switch(event->key()) {
        case Qt::Key_Yes:
            code = EKeyYes;
            break;
        case Qt::Key_No:
            code = EKeyNo;
            break;
        case Qt::Key_NumberSign:
            code = KPhoneDtmfHashCharacter;
            break;
        case Qt::Key_Asterisk:
            code = KPhoneDtmfStarCharacter;
            break;
        default:
            code = event->key();
            break;
        }

    
    return code;
}
