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
* Description:  Phone UI's Qt view.
*
*/
#include <hbinstance.h>
#include <QSignalMapper>
#include <hbaction.h>
#include <hbtoolbar.h>
#include <hbvolumesliderpopup.h>
#include <hbnamespace.h>
#include <bubblemanager2.h>
#include <hblineedit.h>
#include <hbmenu.h>

#include <xqserviceutil.h>
#include <xqkeycapture.h>
#include <dialpad.h>

#include "phoneuiqtview.h"
#include "phoneaction.h"
#include "qtphonelog.h"

PhoneUIQtView::PhoneUIQtView (HbMainWindow &window, QGraphicsItem *parent) :
    HbView (parent),
    m_window(window),
    m_volumeSlider (0),
    m_expandSignalMapper(0),
    m_participantListSignalMapper(0),
    m_volumeCommandId(0),
    m_keyCapture(0),
    m_networkInfo(0)
{
    // Set network name
    m_networkInfo = new QSystemNetworkInfo(this);
    QString networkName = m_networkInfo->networkName(QSystemNetworkInfo::GsmMode);
    connect(m_networkInfo, SIGNAL (networkNameChanged(QSystemNetworkInfo::NetworkMode,QString)), this, SLOT(networkNameChanged(QSystemNetworkInfo::NetworkMode, QString)));
    setTitle(networkName);

    // Capturing long press of end key
    m_keyCapture = new XqKeyCapture();
    m_keyCapture->captureLongKey(Qt::Key_No);
    m_keyCapture->captureKey(Qt::Key_No);
    
    // Dialpad
    m_dialpad = new Dialpad(m_window);
    m_dialpad->setCallButtonEnabled(false);
    m_dialpad->setTapOutsideDismiss(true);
    connect(&m_dialpad->editor(),SIGNAL(contentsChanged()),
            SLOT(onEditorContentChanged()));
    connect(m_dialpad,SIGNAL(aboutToClose()),this,
                SLOT(dialpadClosed()));
                
    // Call handling widget
    m_bubbleManager = new BubbleManager (this);
    setWidget(m_bubbleManager);

    // Set event filter
    m_window.installEventFilter(this);

    m_signalMapper = new QSignalMapper (this);
    connect(m_signalMapper, SIGNAL (mapped (int)), this, SIGNAL (command (int)));
    connect(&m_window,SIGNAL(orientationChanged(Qt::Orientation)),
            this,SLOT(handleOrientationChange(Qt::Orientation)));

    m_menuSignalMapper = new QSignalMapper(this);
    connect(m_menuSignalMapper, SIGNAL(mapped(int)), this, SIGNAL(command(int)));

    m_bubbleManager->handleOrientationChange(m_window.orientation());

    // change exit softkey to back button
    m_backAction = new HbAction(Hb::BackNaviAction, this);
    connect(m_backAction, SIGNAL(triggered()), this, SLOT(backButtonClicked()));
    setNavigationAction(m_backAction);

    createToolBarActions();
}

PhoneUIQtView::~PhoneUIQtView ()
{

    foreach (HbAction *action, m_toolbarActions ) {
        delete action;
    }
    m_window.removeEventFilter(this);
    delete m_volumeSlider;
    delete m_dialpad;
}

BubbleManagerIF& PhoneUIQtView::bubbleManager()
{
    return *m_bubbleManager;
}

void PhoneUIQtView::addBubbleCommand (
    int bubbleId,
    const PhoneAction& action )
{
    HbAction* bubbleAction = new HbAction ();
    bubbleAction->setText (action.text());
    bubbleAction->setIcon (action.icon());
    setActionRole(action,*bubbleAction);
    m_bubbleManager->addAction (bubbleId, bubbleAction);

    QList<int> bubbles = m_bubbleMap.keys();
    bool found(false);

    for ( int i=0; i<bubbles.size(); ++i ) {
        if (bubbleId==bubbles[i]){
            connect(bubbleAction, SIGNAL (triggered ()), m_bubbleMap.value(bubbleId), SLOT (map ()));
            m_bubbleMap.value(bubbleId)->setMapping(bubbleAction, action.command());
            m_bubbleActionMap.value(bubbleId)->append(bubbleAction);
            found = true;
        }
    }

    if (!found) {
        QSignalMapper *mapper = new QSignalMapper();
        connect(mapper, SIGNAL (mapped (int)), this, SIGNAL (command (int)));
        connect(bubbleAction, SIGNAL (triggered ()), mapper, SLOT (map ()));
        mapper->setMapping (bubbleAction, action.command());
        QList<HbAction *> *actionList = new QList<HbAction *>();
        actionList->append( bubbleAction );
        m_bubbleActionMap.insert(bubbleId,actionList);
        m_bubbleMap.insert(bubbleId,mapper);
    }
}

void PhoneUIQtView::addParticipantListAction(
    int commandId,
    const QString& text,
    const HbIcon& icon)
{
    HbAction* action = new HbAction ();
    action->setText (text);
    action->setIcon (icon);
    m_bubbleManager->addParticipantListAction(action);

    if (!m_participantListSignalMapper) {
        m_participantListSignalMapper = new QSignalMapper();
        connect(m_participantListSignalMapper, SIGNAL (mapped (int)), this, SIGNAL (command (int)));
    }

    connect(action, SIGNAL (triggered ()), m_participantListSignalMapper, SLOT (map ()));
    m_participantListSignalMapper->setMapping (action, commandId);
    m_participantListActions.append( action );
}

void PhoneUIQtView::clearParticipantListActions()
{

    if (m_participantListSignalMapper) {
        m_bubbleManager->clearParticipantListActions();

        foreach (HbAction *action, m_participantListActions ) {
            m_participantListSignalMapper->removeMappings(action);
            delete action;
        }

        m_participantListActions.clear();
        delete m_participantListSignalMapper;
        m_participantListSignalMapper = 0;
    }

}

void PhoneUIQtView::clearBubbleCommands (int bubbleId)
{
    m_bubbleManager->clearActions (bubbleId);
    QSignalMapper *mapper = m_bubbleMap.value(bubbleId);

    if (mapper) {
        QList<HbAction *> *actions = m_bubbleActionMap.value(bubbleId);

        foreach (HbAction *action, *actions ) {
            mapper->removeMappings(action);
            delete action;
        }

        actions->clear();
        m_bubbleMap.remove(bubbleId);
        m_bubbleActionMap.remove(bubbleId);
        delete mapper;
        delete actions;
    }

}

void PhoneUIQtView::setToolbarActions(const QList<PhoneAction*>& actions)
{
    // clear current toolbar actions
    for (int i=0;i<toolBar()->actions().count();++i) {
        m_signalMapper->removeMappings(
                static_cast<HbAction*>(toolBar()->actions().at(i)));
    }

    QList<QAction*> toolBarActions = toolBar()->actions();

    if (toolBarActions.size()<actions.size()) {
        for (int i=toolBarActions.size(); i<actions.size(); ++i) {
            toolBar()->addAction(m_toolbarActions.at(i));
        }
    } else if (toolBarActions.size()>actions.size()) {
        for (int i=toolBarActions.size(); 0<i; --i) {
            if (i>actions.size()) {
                HbAction* action = static_cast<HbAction*>(toolBarActions.at(i-1));
                toolBar()->removeAction(action);
            }
        }
    }

    for (int i=0; i<toolBar()->actions().size(); ++i) {

        if (i<actions.count()) {
            HbAction* action = static_cast<HbAction*>(toolBar()->actions().at(i));
            action->setText(actions.at(i)->text());
            action->setIcon(actions.at(i)->icon());
            action->setDisabled(actions.at(i)->isDisabled());

            m_signalMapper->setMapping(action, actions.at(i)->command());
        }
    }

    if ( m_window.orientation() == Qt::Horizontal ) {
        toolBar()->setOrientation(Qt::Horizontal);
    }
    // update toolbar
    toolBar()->update();
}

void PhoneUIQtView::hideToolbar ()
{
    toolBar()->hide ();
}

void PhoneUIQtView::showToolbar ()
{
    setFocus();
    toolBar()->show();
}

int PhoneUIQtView::volumeSliderValue ()
{
    if (m_volumeSlider) {
        return m_volumeSlider->value ();
    } else {
        return -1;
    }
}

void PhoneUIQtView::removeVolumeSlider ()
{
    if (m_volumeSlider) {
        if (m_volumeSlider->isVisible()) {
            m_volumeSlider->hide();
        }
        m_volumeSlider->deleteLater();
        m_volumeSlider = 0;
    }
}

void PhoneUIQtView::volumeSliderClosed ()
{
    removeVolumeSlider();
}

void PhoneUIQtView::setVolumeSliderValue (
        int value, int commandId, int maxVolumeValue, int minVolumeValue)
{
    m_volumeCommandId = commandId;

    if (!m_volumeSlider) {
        m_volumeSlider = new HbVolumeSliderPopup ();
        m_volumeSlider->setDismissPolicy(HbDialog::TapOutside);
        m_volumeSlider->setTimeout (10000); // TODO: 10 seconds for now, replace with correct value when spec is ready and says what it is
        connect(m_volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeSliderChanged(int)));
        connect(m_volumeSlider, SIGNAL(aboutToClose()), this, SLOT(volumeSliderClosed()));
    }


    if (m_volumeSlider->minimum() != minVolumeValue ||
            m_volumeSlider->maximum() !=  maxVolumeValue  ) {
        m_volumeSlider->setRange (minVolumeValue, maxVolumeValue);
    }

    if (value != m_volumeSlider->value())
        m_volumeSlider->setValue (value);

    if (false == m_volumeSlider->isVisible()) {
        m_volumeSlider->show();
    }
}

void PhoneUIQtView::volumeSliderChanged(int value)
{
    Q_UNUSED (value);
    emit command (m_volumeCommandId);
}

void PhoneUIQtView::setExpandAction(int bubbleId, int commandId)
{
    removeExpandAction(bubbleId);

    HbAction* action = new HbAction();
    m_bubbleManager->setExpandAction(bubbleId, action);

    if (!m_expandSignalMapper) {
        m_expandSignalMapper = new QSignalMapper(this);
        connect(m_expandSignalMapper, SIGNAL (mapped (int)),
                this, SIGNAL (command (int)));
    }

    connect(action, SIGNAL (triggered ()), m_expandSignalMapper, SLOT (map ()));
    m_expandSignalMapper->setMapping(action, commandId);

    m_expandActionMap.insert(bubbleId,action);
}

void PhoneUIQtView::removeExpandAction(int bubbleId)
{
    if (m_expandActionMap.contains(bubbleId)) {
        m_bubbleManager->setExpandAction(bubbleId, 0);
        HbAction* action = m_expandActionMap.value(bubbleId);
        m_expandSignalMapper->removeMappings(action);
        m_expandActionMap.remove(bubbleId);
        delete action;
    }
}

void PhoneUIQtView::showDialpad()
{
    if (false == m_dialpad->isVisible()) {
        setDialpadPosition();
        m_dialpad->openDialpad();
    }
}

void PhoneUIQtView::hideDialpad()
{
    if (true == m_dialpad->isVisible())
        m_dialpad->closeDialpad();
}

bool PhoneUIQtView::isDialpadVisible()
{
    return m_dialpad->isVisible();
}

QString PhoneUIQtView::dialpadText()
{
    return m_dialpad->editor().text();
}

void PhoneUIQtView::clearAndHideDialpad()
{
    m_dialpad->editor().setText(QString(""));
    hideDialpad();
}

void PhoneUIQtView::bringToForeground()
{
    m_window.show();
    m_window.raise();
}

void PhoneUIQtView::setMenuActions(const QList<PhoneAction*>& actions)
{

    for (int i=menu()->actions().count(); 0<i; --i) {
        HbAction* action = static_cast<HbAction*>(menu()->actions().at(i-1));
        m_menuSignalMapper->removeMappings(action);
        menu()->removeAction(action);
        delete action;
    }

    for (int i=0; i<actions.count(); ++i) {
        HbAction* action = new HbAction();
        action->setText(actions.at(i)->text());
        menu()->addAction(action);
        connect(action, SIGNAL(triggered()), m_menuSignalMapper, SLOT(map()));
        m_menuSignalMapper->setMapping(action, actions.at(i)->command());
    }

}

HbMenu &PhoneUIQtView::menuReference()
{
    return *menu();
}

void PhoneUIQtView::handleOrientationChange(Qt::Orientation orientation)
{
    if (orientation==Qt::Horizontal) {
        toolBar()->setOrientation(Qt::Horizontal);
    }

    m_bubbleManager->handleOrientationChange(orientation);

    setDialpadPosition();
}

void PhoneUIQtView::backButtonClicked()
{
    XQServiceUtil::toBackground(true);
}

void PhoneUIQtView::onEditorContentChanged()
{
    m_dialpad->setCallButtonEnabled(
        m_dialpad->editor().text().length());
}

void PhoneUIQtView::dialpadClosed()
{
    emit dialpadIsAboutToClose();
}

bool PhoneUIQtView::eventFilter(QObject * /*watched*/, QEvent * event)
{
    PHONE_DEBUG2("PhoneUIQtView::eventFilter event type:", event->type());
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        PHONE_DEBUG2("PhoneUIQtView::eventFilter pressed key:", keyEvent->key());
        PHONE_DEBUG2("PhoneUIQtView::eventFilter isAutoRepeat:", keyEvent->isAutoRepeat());
        emit keyPressed(keyEvent);        
        keyEvent->accept();
        
        return false;
    } else if(event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        PHONE_DEBUG2("PhoneUIQtView::eventFilter released key:", keyEvent->key());
        emit keyReleased(keyEvent);
        keyEvent->accept();
        
        return false;
    } else if (event->type() == QEvent::WindowActivate){
        PHONE_DEBUG("PhoneUIQtView::eventFilter WindowActivate");
        emit windowActivated();
        return false;
    } else if (event->type() == QEvent::WindowDeactivate){
        PHONE_DEBUG("PhoneUIQtView::eventFilter WindowDeactivate");
        emit windowDeactivated();
        return false;
    }else{
        return false;
    }
}

void PhoneUIQtView::setDialpadPosition()
{
    // workaround to tsw error JMKN-83NAPU (fix coming in MCL wk14)
    // QRectF screenRect(m_window.layoutRect());
    QRectF screenRect = (m_window.orientation() == Qt::Horizontal) ?
                        QRectF(0,0,640,360) : QRectF(0,0,360,640);
                        	
    if (m_window.orientation() == Qt::Horizontal) {
            // dialpad takes half of the screen
        m_dialpad->setPos(QPointF(screenRect.width()/2,
                                  this->scenePos().y()));
        m_dialpad->setPreferredSize(screenRect.width()/2,
                                           (screenRect.height()-scenePos().y()));
    } else {
        // dialpad takes 65% of the screen height
        qreal screenHeight = screenRect.height();
        m_dialpad->setPos(QPointF(0,
                                  screenHeight/2.25));
        m_dialpad->setPreferredSize(screenRect.width(),
                                    screenHeight-screenHeight/2.25);
    }
}

void PhoneUIQtView::setActionRole(const PhoneAction& pa, HbAction& action)
{
    if (pa.actionRole()==PhoneAction::Accept) {
        action.setSoftKeyRole(QAction::PositiveSoftKey);
    } else if (pa.actionRole()==PhoneAction::Decline) {
        action.setSoftKeyRole(QAction::NegativeSoftKey);
    }
}

void PhoneUIQtView::createToolBarActions()
{
    for (int i=0;i<4;++i) {
        HbAction* action = new HbAction();
        connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
        m_toolbarActions.append(action);
    }
}

void PhoneUIQtView::shutdownPhoneApp()
{
    PHONE_DEBUG("PhoneUIQtView::shutdownPhoneApp");
    QCoreApplication::quit();
}

void PhoneUIQtView::setBackButtonVisible(bool visible)
{
    if (visible) {
        setNavigationAction(m_backAction);
        }
    else {
        setNavigationAction(0);
    }
}

void PhoneUIQtView::networkNameChanged(QSystemNetworkInfo::NetworkMode mode, const QString &netName)
{
    if(mode == QSystemNetworkInfo::GsmMode) {
        setTitle(netName);
    }	
}
