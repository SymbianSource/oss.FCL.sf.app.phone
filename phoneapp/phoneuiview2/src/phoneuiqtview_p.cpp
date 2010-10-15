/*!
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Phone UI's Qt view private.
*
*/
#include <hbinstance.h>
#include <hbaction.h>
#include <hbvolumesliderpopup.h>
#include <hbnamespace.h>
#include <bubblemanager2.h>
#include <hblineedit.h>
#include <hbmenu.h>

#include <xqserviceutil.h>
#include <xqkeycapture.h>
#include <dialpad.h>
#include <dialpadkeyhandler.h>
#include "phoneuiqtview.h"
#include "phoneuiqtview_p.h"
#include "qtphonelog.h"

PhoneUIQtViewPrivate::PhoneUIQtViewPrivate(
        HbMainWindow &window, PhoneUIQtView &parent) :
    QObject(&parent),
    m_parent(parent),
    m_window(window),
    m_bubbleManager(new BubbleManager(&m_parent)),
    m_volumeSlider (0),
    m_volumeCommandId(0),
    m_backAction(0),
    m_dialpad(0),
    m_keyCapture(0),
    m_networkInfo(new QSystemNetworkInfo(this)),
    m_dialpadKeyHandler(0),
    m_restrictedMode(false),
    m_optionsMenu(0)
{
    PHONE_TRACE
    // Set network name
    connect(m_networkInfo, SIGNAL(
                networkNameChanged(QSystemNetworkInfo::NetworkMode,QString)),
            this, SLOT(
                networkNameChanged(QSystemNetworkInfo::NetworkMode, QString)));

    // Capturing long press of end key
    m_keyCapture = new XQKeyCapture();
    
    // Dialpad
    m_dialpad = new Dialpad(m_window);
    m_dialpad->setCallButtonEnabled(false);
    m_dialpad->setTapOutsideDismiss(true);
    connect(m_dialpad,SIGNAL(aboutToClose()),this,
                SLOT(dialpadClosed()));
    
    // Set event filter
    m_window.installEventFilter(this);

    connect(&m_window,SIGNAL(orientationChanged(Qt::Orientation)),
            this,SLOT(setDialpadPosition(Qt::Orientation)));
    
    connect(&m_window,SIGNAL(orientationChanged(Qt::Orientation)),
            m_bubbleManager,SLOT(handleOrientationChange(Qt::Orientation)));

    m_bubbleManager->handleOrientationChange(m_window.orientation());

    // change exit softkey to back button
    m_backAction = new HbAction(Hb::BackNaviAction, this);
    connect(m_backAction, SIGNAL(triggered()), this, SLOT(backButtonClicked()));

}

PhoneUIQtViewPrivate::~PhoneUIQtViewPrivate()
{
    foreach (QList<HbAction *> actions, m_bubbleActionMap) {
        qDeleteAll(actions);
    }
    
    qDeleteAll(m_participantListActions);
    qDeleteAll(m_expandActionMap);
    m_window.removeEventFilter(this);
    delete m_keyCapture;
    delete m_volumeSlider;
    delete m_dialpad;
    delete m_optionsMenu;
}

QString PhoneUIQtViewPrivate::networkName()
{
    QString networkName = m_networkInfo->networkName(QSystemNetworkInfo::WcdmaMode);
    if(networkName.isEmpty()) {
        networkName = m_networkInfo->networkName(QSystemNetworkInfo::GsmMode);
    }
    return networkName;
}

void PhoneUIQtViewPrivate::addBubbleCommand (
    int bubbleId,
    HbAction* action )
{
    PHONE_TRACE
    m_bubbleManager->addAction(bubbleId, action);

    QList<int> bubbles = m_bubbleActionMap.keys();
    bool found(false);

    for ( int i=0; i<bubbles.size(); ++i ) {
        if (bubbleId==bubbles[i]){
            connect(action, SIGNAL(triggered()), this, SLOT(actionTriggerred()));
            m_bubbleActionMap[bubbleId].append(action);
            found = true;
        }
    }

    if (!found) {
        connect(action, SIGNAL(triggered()), this, SLOT(actionTriggerred()));
        QList<HbAction *> actionList;
        actionList.append(action);
        m_bubbleActionMap.insert(bubbleId,actionList);
    }
}

void PhoneUIQtViewPrivate::addParticipantListAction(
    int commandId,
    const QString& text,
    const HbIcon& icon)
{
    PHONE_TRACE
    QScopedPointer<HbAction> action(new HbAction());
    action->setText(text);
    action->setIcon(icon);
    action->setProperty("command", commandId);
    m_bubbleManager->addParticipantListAction(action.data());

    connect(action.data(), SIGNAL(triggered()), this, SLOT(actionTriggerred()));
    m_participantListActions.append(action.take());
}

void PhoneUIQtViewPrivate::clearParticipantListActions()
{
    PHONE_TRACE
    m_bubbleManager->clearParticipantListActions();
    qDeleteAll(m_participantListActions);
    m_participantListActions.clear();
}

void PhoneUIQtViewPrivate::clearBubbleCommands (int bubbleId)
{
    PHONE_TRACE
    m_bubbleManager->clearActions(bubbleId);
    qDeleteAll(m_bubbleActionMap.take(bubbleId));
}

int PhoneUIQtViewPrivate::volumeSliderValue()
{
    if (m_volumeSlider) {
        return m_volumeSlider->value ();
    } else {
        return -1;
    }
}

void PhoneUIQtViewPrivate::removeVolumeSlider()
{
    if (m_volumeSlider) {
        if (m_volumeSlider->isVisible()) {
            m_volumeSlider->hide();
        }
        m_volumeSlider->deleteLater();
        m_volumeSlider = 0;
    }
}

void PhoneUIQtViewPrivate::setVolumeSliderValue (
        int value, int commandId, int maxVolumeValue, int minVolumeValue)
{
    PHONE_TRACE
    m_volumeCommandId = commandId;

    if (!m_volumeSlider) {
        m_volumeSlider = new HbVolumeSliderPopup();
        m_volumeSlider->setDismissPolicy(HbDialog::TapOutside);
        m_volumeSlider->setTimeout(10000); // TODO: 10 seconds for now, replace with correct value when spec is ready and says what it is
        connect(m_volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeSliderChanged(int)));
        connect(m_volumeSlider, SIGNAL(aboutToClose()), this, SLOT(removeVolumeSlider()));
    }


    if (m_volumeSlider->minimum() != minVolumeValue ||
            m_volumeSlider->maximum() !=  maxVolumeValue  ) {
        m_volumeSlider->setRange(minVolumeValue, maxVolumeValue);
    }

    if (value != m_volumeSlider->value()) {
        m_volumeSlider->setValue(value);
    }
    

    if (false == m_volumeSlider->isVisible()) {
        m_volumeSlider->show();
    }
}

void PhoneUIQtViewPrivate::volumeSliderChanged(int value)
{
    Q_UNUSED (value);
    emit m_parent.command(m_volumeCommandId);
}

void PhoneUIQtViewPrivate::setExpandAction(int bubbleId, int commandId)
{
    PHONE_TRACE
    removeExpandAction(bubbleId);

    QScopedPointer<HbAction> action(new HbAction());
    action->setProperty("command", commandId);
    m_bubbleManager->setExpandAction(bubbleId, action.data());

    connect(action.data(), SIGNAL(triggered()), this, SLOT(actionTriggerred()));

    m_expandActionMap.insert(bubbleId, action.take());
}

void PhoneUIQtViewPrivate::removeExpandAction(int bubbleId)
{
    PHONE_TRACE
    if (m_expandActionMap.contains(bubbleId)) {
        m_bubbleManager->setExpandAction(bubbleId, 0);
        delete m_expandActionMap.take(bubbleId);
    }
}

void PhoneUIQtViewPrivate::showDialpad()
{
    if (false == m_dialpad->isVisible()) {
        setDialpadPosition(m_window.orientation());
        m_dialpad->openDialpad();
    }
}

void PhoneUIQtViewPrivate::hideDialpad()
{
    if (true == m_dialpad->isVisible())
        m_dialpad->closeDialpad();
}

void PhoneUIQtViewPrivate::replaceActions(
        HbWidget &widget, const QList<HbAction *> &actions) const
{

    // Memorize old actions, so that widget doenst flash while updating
    QList<QAction*> oldActions = widget.actions();
    
    // Add new toolbar actions
    foreach (HbAction *newAction, actions) {
        newAction->setParent(&widget); // Transfer ownership
        connect(newAction, SIGNAL(triggered()), this, SLOT(actionTriggerred()));
        widget.addAction(newAction);
    }
    
    // Remove and delete old actions
    foreach (QAction *oldAction, oldActions) {
        widget.removeAction(oldAction);
        delete oldAction;
    }

}

void PhoneUIQtViewPrivate::captureKey(Qt::Key key, bool capture)
{
    if (capture) {
        if (!m_keyCaptures.contains(key)) {
            m_keyCapture->captureLongKey(key);
            m_keyCapture->captureKey(key);
            m_keyCaptures.append(key);
        }
    } else {
        if (m_keyCaptures.contains(key)) {
            m_keyCapture->cancelCaptureKey(key);
            m_keyCapture->cancelCaptureLongKey(key);
            m_keyCaptures.removeOne(key);
        }
    }
}

void PhoneUIQtViewPrivate::backButtonClicked()
{
    XQServiceUtil::toBackground(true);
}

void PhoneUIQtViewPrivate::onEditorContentChanged()
{
    m_dialpad->setCallButtonEnabled(
        m_dialpad->editor().text().length());
}

void PhoneUIQtViewPrivate::dialpadClosed()
{
    emit m_parent.dialpadIsAboutToClose();
}

bool PhoneUIQtViewPrivate::eventFilter(QObject *watched, QEvent * event)
{
    Q_UNUSED(watched);
    PHONE_TRACE2(":event type", event->type());
    
    // Allow send key only when there is callbutton enabled or no text in input field
    bool sendKeyAllowed = m_dialpad->isCallButtonEnabled() || 
            (m_dialpad->editor().text().length() == 0);
    
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        PHONE_DEBUG2("PhoneUIQtView::eventFilter pressed key:", keyEvent->key());
        PHONE_DEBUG2("PhoneUIQtView::eventFilter isAutoRepeat:", keyEvent->isAutoRepeat());
        if ( (keyEvent->key() != Qt::Key_Yes && keyEvent->key() != Qt::Key_Enter) ||
                sendKeyAllowed) {
            emit m_parent.keyPressed(keyEvent);        
            keyEvent->accept();
        }
        
        return false;
    } else if(event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        PHONE_DEBUG2("PhoneUIQtView::eventFilter released key:", keyEvent->key());
        if ( (keyEvent->key() != Qt::Key_Yes && keyEvent->key() != Qt::Key_Enter) ||
                sendKeyAllowed) {
            emit m_parent.keyReleased(keyEvent);
            keyEvent->accept();
        }
        return false;
    } else if (event->type() == QEvent::WindowActivate){
        PHONE_DEBUG("PhoneUIQtView::eventFilter WindowActivate");
        emit m_parent.windowActivated();
        return false;
    } else if (event->type() == QEvent::WindowDeactivate){
        PHONE_DEBUG("PhoneUIQtView::eventFilter WindowDeactivate");
        emit m_parent.windowDeactivated();
        return false;
    }else{
        return false;
    }
}

void PhoneUIQtViewPrivate::setDialpadPosition(Qt::Orientation orientation)
{
    PHONE_TRACE
    QRectF screenRect(m_window.layoutRect());
                            
    if (orientation == Qt::Horizontal) {
            // dialpad takes half of the screen
        m_dialpad->setPos(QPointF(screenRect.width()/2,
                m_parent.scenePos().y()));
        m_dialpad->setPreferredSize(screenRect.width()/2,
                (screenRect.height()-m_parent.scenePos().y()));
    } else {
        // dialpad takes 65% of the screen height
        qreal screenHeight = screenRect.height();
        m_dialpad->setPos(QPointF(0,
                                  screenHeight/2.25));
        m_dialpad->setPreferredSize(screenRect.width(),
                                    screenHeight-screenHeight/2.25);
    }
}

void PhoneUIQtViewPrivate::setBackButtonVisible(bool visible)
{
    if (!m_restrictedMode) {
        m_backAction->setEnabled(visible);
    }
}

void PhoneUIQtViewPrivate::setRestrictedMode(bool restrictedMode)
{
    PHONE_TRACE
    m_restrictedMode = restrictedMode;
    m_backAction->setEnabled(!restrictedMode);
    m_dialpad->setCallButtonEnabled(false);
    m_dialpad->editor().setText(""); // Clead dialpad
    if (m_restrictedMode) {
        delete m_dialpadKeyHandler;
        m_dialpadKeyHandler = 0;
        m_dialpadKeyHandler = new DialpadKeyHandler(
                m_dialpad, DialpadKeyHandler::EmergencyCall, this);
        disconnect(&m_dialpad->editor(),SIGNAL(contentsChanged()),
                this, SLOT(onEditorContentChanged())); // Let emergency handler do updating 
    } else {
        delete m_dialpadKeyHandler;
        m_dialpadKeyHandler = 0;
        // enable key sequence handling during a call
        m_dialpadKeyHandler = new DialpadKeyHandler(
                m_dialpad, DialpadKeyHandler::KeySequence, this);
        connect(&m_dialpad->editor(),SIGNAL(contentsChanged()),
                SLOT(onEditorContentChanged())); // Update our self
    }
    
}

void PhoneUIQtViewPrivate::networkNameChanged(
        QSystemNetworkInfo::NetworkMode mode, const QString &netName)
{
    if((mode == QSystemNetworkInfo::GsmMode) || 
       (mode == QSystemNetworkInfo::WcdmaMode)) {
        m_parent.setTitle(netName);
    }   
}


void PhoneUIQtViewPrivate::actionTriggerred()
{
    if (sender()->property("command").isValid()) {
        emit m_parent.command(sender()->property("command").toInt());
    } else {
        PHONE_TRACE1(": No command found!")
    }
}
