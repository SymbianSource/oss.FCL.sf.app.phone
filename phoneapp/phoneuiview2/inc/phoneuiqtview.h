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

#ifndef PHONEUIQTVIEW_H
#define PHONEUIQTVIEW_H

#include <QObject>
#include <hbview.h>
#include "phoneuiqtviewif.h"
#include <bubblemanagerif.h>

class HbAction;
class PhoneUIQtViewPrivate;

#ifdef BUILD_PHONEUIQTVIEW
#define PHONEUIQTVIEW_EXPORT Q_DECL_EXPORT
#else
#define PHONEUIQTVIEW_EXPORT Q_DECL_IMPORT
#endif



class PHONEUIQTVIEW_EXPORT PhoneUIQtView : public HbView, public PhoneUIQtViewIF
{
    Q_OBJECT

public:

    /*!
       Constructor
    */
    PhoneUIQtView(HbMainWindow &window, QGraphicsItem *parent = 0);
    
    /*!
       Destructor
    */
    virtual ~PhoneUIQtView();
            
    /*!
        \fn void PhoneUIQtView::bubbleManager ()
        
        Returns BubbleManager interface.
    */
    BubbleManagerIF& bubbleManager();
    
    /*!
        \fn void PhoneUIQtView::addBubbleCommand ()
        
        Adds new command to bubble. Command id will be emitted as
        a parameter from view's command(int) signal. Ownership transferred.
    */
    void addBubbleCommand(int bubbleId, HbAction* action);

    /*!
        \fn void PhoneUIQtView::clearBubbleCommands ()
        
        Clears commands from bubble.
    */
    void clearBubbleCommands(int bubbleId);
    
    /*!
        \fn void PhoneUIQtView::addParticipantListAction ()
        
        Adds new patricipant list command to conference bubble. 
    */
    void addParticipantListAction(int commandId, const QString &text, const HbIcon &icon);

    /*!
        \fn void PhoneUIQtView::clearBubbleCommands ()
        
        Clears participant list commands from conference bubble.
    */
    void clearParticipantListActions();
    
    /*!
        \fn void PhoneUIQtViewIF::setToolbarActions()
        
       Sets toolbar actions. Ownership transferred.
    */
    void setToolbarActions(const QList<HbAction *> &actions);
    
    /*!
        \fn void PhoneUIQtView::hideToolbar ()
        
        Hides toolbar.
    */
    void hideToolbar();
    
    /*!
        \fn void PhoneUIQtView::showToolbar ()
        
        Shows toolbar.
    */
    void showToolbar();

    /*!
        \fn void PhoneUIQtView::volumeSliderValue ()
        
        Returns HbVolumeSliderPopup's volume value.
        If this volume slider isn't active -1 is returned.
    */
    int volumeSliderValue();

    /*!
        \fn void PhoneUIQtView::setVolumeSliderValue (int value)
        
        Sets slider value to HbVolumeSliderPopup.
    */
    void setVolumeSliderValue(
            int value,
            int commandId, 
            int maxVolumeValue, 
            int minVolumeValue);
    
    /*!
        \fn void PhoneUIQtView::removeVolumeSlider()
        
        Removes HbVolumeSliderPopup.
    */
    void removeVolumeSlider();
    
    /*!
        \fn void PhoneUIQtView::setExpandAction ()
        
        Sets expand action.
    */
    void setExpandAction(int bubbleId, int commandId);
    
        /*!
        \fn void PhoneUIQtView::removeExpandAction ()
        
        Removes expand action.
    */    
    void removeExpandAction(int bubbleId);
    
    /*!
        \fn void PhoneUIQtView::showDialpad()
        
        Shows dialpad widget.
    */    
    void showDialpad();
    
    /*!
        \fn void PhoneUIQtView::hideDialpad()
        
        Hides dialer widget.
    */    
    void hideDialpad();
    
    /*!
        \fn void PhoneUIQtView::isDialpadVisible()
        
        Checks is dialpad visible.
    */    
    bool isDialpadVisible();
    
    /*!
        \fn void PhoneUIQtView::dialpadText()
        
        Returns content of the dialpad.
    */  
    QString dialpadText();
    
    /*!
        \fn void PhoneUIQtView::dialpadString()
        
        Clears and hides dialpad.
    */
    void clearAndHideDialpad();
    
    /*!
        \fn void PhoneUIQtView::clearDialpad()
        
        Clears text from dialpad.
    */
    void clearDialpad();
    
    /*!
        \fn void PhoneUIQtView::bringToForeground()
        
        Brings to foreground.
    */
    void bringToForeground();
    
    /*!
        \fn void PhoneUIQtViewIF::hide()
        
        Lowers the phone to the bottom of the stack.
     */
    void hide();
    
    /*!
        \fn void PhoneUIQtViewIF::setToolbarActions()
        
       Sets menu actions for call handling. Ownership transferred.
    */
    void setMenuActions(const QList<HbAction *> &actions);
    
    /*!
        \fn void PhoneUIQtView::setBackButtonVisible()
        
        Set the Back command visibility in TitleBar
    */
    void setBackButtonVisible(bool visible);
    
    /*!
        \fn void PhoneUIQtViewIF::menuReference()
        
        Returns HbMenu reference.
    */
    HbMenu &menuReference();

    /*!
        \fn void PhoneUIQtViewIF::captureKey()
        
        Captures key.
    */
    void captureKey(Qt::Key key, bool capture);
    
    /*
        \fn void setRestrictedMode()
        
        Sets Ui to restricted mode. Decides which keyevent are allowed.
     */
    void setRestrictedMode(bool restrictedMode);
    
    /*!
        \fn void PhoneUIQtView::shutdownPhoneApp()
        
        Shutdown phone application.
    */
    void shutdownPhoneApp();
signals:

    /*!
        \fn void keyPressed (QKeyEvent *event)
        
        This signal is emitted when key is pressed down.
    */
    void keyPressed(QKeyEvent *event);

    /*!
        \fn void keyPressed (QKeyEvent *event)
        
        This signal is emitted when key is released.
    */
    void keyReleased(QKeyEvent *event);
    
    /*!
        \fn void command(int command)
        
        This signal is emitted when UI action is handled.
    */
    void command(int command);
    
    /*!
        \fn void dialpadIsAboutToClose()
        
        This signal is emitted when dialpad is about to close.
    */
    void dialpadIsAboutToClose();
    
    /*!
         \fn void windowActivated();
         
         This signal is emitted when window is activated
     */
    void windowActivated();
    
    /*!
         \fn void windowDeactivated();
         
         This signal is emitted when window is deactivated
     */
    void windowDeactivated();
    
public slots:
    /*!
        \fn void handleOrientationChange(Qt::Orientation orientation)
    */
    void handleOrientationChange(Qt::Orientation orientation);
    
private:
    friend class PhoneUIQtViewPrivate;
    PhoneUIQtViewPrivate *m_priv;
};

#endif // PHONEUIQTVIEW_H
