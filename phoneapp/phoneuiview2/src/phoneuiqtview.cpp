/*!
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <hbaction.h>
#include <hbtoolbar.h>
#include <hbnamespace.h>
#include <bubblemanager2.h>
#include <hblineedit.h>
#include <hbmenu.h>
#include <dialpad.h>

#include "phoneuiqtview_p.h"
#include "phoneuiqtview.h"
#include "qtphonelog.h"

PhoneUIQtView::PhoneUIQtView (HbMainWindow &window, QGraphicsItem *parent) :
    HbView(parent),
    m_priv(new PhoneUIQtViewPrivate(window, *this))
{
    PHONE_TRACE
    // Set network name
    setTitle(m_priv->networkName());
    
    // Call handling widget
    setWidget(m_priv->m_bubbleManager);

    // change exit softkey to back button
    setNavigationAction(m_priv->m_backAction);

    // Set restricted mode off, normal state
    setRestrictedMode(false);
}

PhoneUIQtView::~PhoneUIQtView ()
{
}

BubbleManagerIF& PhoneUIQtView::bubbleManager()
{
    return *m_priv->m_bubbleManager;
}

void PhoneUIQtView::addBubbleCommand (
    int bubbleId,
    HbAction* action )
{
    PHONE_TRACE
    m_priv->addBubbleCommand(bubbleId, action);
}

void PhoneUIQtView::addParticipantListAction(
    int commandId,
    const QString& text,
    const HbIcon& icon)
{
    PHONE_TRACE
    m_priv->addParticipantListAction(commandId, text, icon);
}

void PhoneUIQtView::clearParticipantListActions()
{
    PHONE_TRACE
    m_priv->clearParticipantListActions();
}

void PhoneUIQtView::clearBubbleCommands (int bubbleId)
{
    PHONE_TRACE
    m_priv->clearBubbleCommands(bubbleId);
}

void PhoneUIQtView::setToolbarActions(
        const QList<HbAction *> &newToolBarActions)
{
    PHONE_TRACE
    m_priv->replaceActions(*toolBar(), newToolBarActions);

    if ( m_priv->m_window.orientation() == Qt::Horizontal ) {
        toolBar()->setOrientation(Qt::Horizontal);
    }
    // update toolbar
    toolBar()->update();
}

void PhoneUIQtView::hideToolbar ()
{
    toolBar()->hide();
}

void PhoneUIQtView::showToolbar ()
{
    setFocus();
    toolBar()->show();
}

int PhoneUIQtView::volumeSliderValue ()
{
    return m_priv->volumeSliderValue();
}

void PhoneUIQtView::setVolumeSliderValue (
        int value, int commandId, int maxVolumeValue, int minVolumeValue)
{
    PHONE_TRACE
    m_priv->setVolumeSliderValue(
            value, commandId, maxVolumeValue, minVolumeValue);
}

void PhoneUIQtView::removeVolumeSlider()
{
    PHONE_TRACE
    m_priv->removeVolumeSlider();
}

void PhoneUIQtView::setExpandAction(int bubbleId, int commandId)
{
    PHONE_TRACE
    m_priv->setExpandAction(bubbleId, commandId);
}

void PhoneUIQtView::removeExpandAction(int bubbleId)
{
    PHONE_TRACE
    m_priv->removeExpandAction(bubbleId);
}

void PhoneUIQtView::showDialpad()
{
    m_priv->showDialpad();
}

void PhoneUIQtView::hideDialpad()
{
    m_priv->hideDialpad();
}

bool PhoneUIQtView::isDialpadVisible()
{
    return m_priv->m_dialpad->isVisible();
}

QString PhoneUIQtView::dialpadText()
{
    return m_priv->m_dialpad->editor().text();
}

void PhoneUIQtView::clearAndHideDialpad()
{
    m_priv->m_dialpad->editor().setText(QString(""));
    m_priv->m_dialpad->closeDialpad();
}

void PhoneUIQtView::clearDialpad()
{
    m_priv->m_dialpad->editor().setText(QString(""));
}

void PhoneUIQtView::bringToForeground()
{
    m_priv->m_window.show(); 
    m_priv->m_window.raise();
}

void PhoneUIQtView::hide()
{
    m_priv->m_window.hide();   
    m_priv->m_window.lower();     
}

void PhoneUIQtView::setMenuActions(
        const QList<HbAction*> &actions)
{   
    PHONE_TRACE
    HbMenu *optionsMenu = &menuReference();
    m_priv->replaceActions(*optionsMenu, actions);
    
    bool visible = false;
    foreach (HbAction* action, actions) {
        if(action->isVisible()){
            visible = true;
            break;
        }
    }
    PHONE_DEBUG2("-  visible", visible);
    HbView* view = m_priv->m_window.currentView();
    if (!visible && !m_priv->m_optionsMenu) {
        PHONE_DEBUG("- takeMenu");
        //takeMenu() - Removes the menu from the view and returns it to the caller.
        //ownership of the menu is transfered.        
        m_priv->m_optionsMenu = view->takeMenu();
        
    } else if (visible && m_priv->m_optionsMenu) {
        // ownership of the menu is transfered
        view->setMenu(m_priv->m_optionsMenu);
        m_priv->m_optionsMenu = 0;
    }
}

HbMenu &PhoneUIQtView::menuReference()
{
    PHONE_TRACE
    HbMenu* optionsMenu = m_priv->m_optionsMenu ?
            m_priv->m_optionsMenu : menu();
    return *optionsMenu;
}

void PhoneUIQtView::captureKey(Qt::Key key, bool capture)
{
    m_priv->captureKey(key, capture);
}

void PhoneUIQtView::handleOrientationChange(
        Qt::Orientation orientation)
{
    PHONE_TRACE
    if (orientation==Qt::Horizontal) {
        toolBar()->setOrientation(Qt::Horizontal);
    }
}

void PhoneUIQtView::shutdownPhoneApp()
{
    PHONE_TRACE
    QCoreApplication::quit();
}

void PhoneUIQtView::setBackButtonVisible(bool visible)
{
    if (!m_priv->m_restrictedMode) {
        m_priv->m_backAction->setEnabled(visible);
    }
}

void PhoneUIQtView::setRestrictedMode(bool restrictedMode)
{
    PHONE_TRACE
    m_priv->setRestrictedMode(restrictedMode);
}
