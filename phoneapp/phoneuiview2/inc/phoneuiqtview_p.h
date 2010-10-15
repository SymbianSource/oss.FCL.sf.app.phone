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

#ifndef PHONEUIQTVIEW_P_H
#define PHONEUIQTVIEW_P_H

#include <QObject>
#include <qsysteminfo.h>
#include <qmobilityglobal.h>
#include <QMap>

class BubbleManager;
class HbAction;
class HbToolBar;
class HbVolumeSliderPopup;
class Dialpad;
class XQKeyCapture;
class DialpadKeyHandler;
class PhoneUIQtView;
class HbMainWindow;
class HbIcon;
class HbWidget;
class HbMenu;

QTM_USE_NAMESPACE

class PhoneUIQtViewPrivate : public QObject
{
    Q_OBJECT

public:

    /*!
       Constructor
    */
    PhoneUIQtViewPrivate(HbMainWindow &window, PhoneUIQtView &parent);
    
    /*!
       Destructor
    */
    virtual ~PhoneUIQtViewPrivate();

    /*!
        \fn QString networkName()
    */
    QString networkName();
    
    /*!
        \fn void addBubbleCommand()
    */
    void addBubbleCommand(int bubbleId, HbAction* action);

    /*!
        \fn void clearBubbleCommands ()
    */
    void clearBubbleCommands(int bubbleId);
    
    /*!
        \fn void addParticipantListAction ()
    */
    void addParticipantListAction(int commandId, const QString &text, const HbIcon &icon);

    /*!
        \fn void clearBubbleCommands ()
    */
    void clearParticipantListActions();

    /*!
        \fn void volumeSliderValue ()
    */
    int volumeSliderValue();

    /*!
        \fn void setVolumeSliderValue (int value)
    */
    void setVolumeSliderValue(
            int value,
            int commandId, 
            int maxVolumeValue, 
            int minVolumeValue);

    /*!
        \fn void setExpandAction ()
    */
    void setExpandAction(int bubbleId, int commandId);
    
        /*!
        \fn void removeExpandAction ()
    */    
    void removeExpandAction(int bubbleId);
    
    /*!
        \fn void showDialpad()
    */    
    void showDialpad();
    
    /*!
        \fn void hideDialpad()
    */    
    void hideDialpad();
    
    /*!
        \fn void setBackButtonVisible()
    */
    void setBackButtonVisible(bool visible);

    /*!
        \fn void captureKey()
    */
    void captureKey(Qt::Key key, bool capture);
    
    /*
        \fn void setRestrictedMode()
     */
    void setRestrictedMode(bool restrictedMode);
    
    /*
        \fn void replaceActions(HbWidget &widget, 
            const QList<HbAction *> actions) const;
     */
    void replaceActions(HbWidget &widget, 
            const QList<HbAction *> &actions) const;

public slots:

    /*!
        \fn void removeVolumeSlider()
    */
    void removeVolumeSlider();
    
    /*!
        \fn void volumeSliderChanged(int value)
    */
    void volumeSliderChanged(int value);

    /*!
        \fn bool eventFilter(QObject *watched, QEvent * event)
    */
    bool eventFilter(QObject *watched, QEvent * event);
        
    /*!
        \fn void backButtonClicked(Qt::Orientation orientation)
    */    
    void backButtonClicked();
    
    /*!
        \fn void onEditorContentChanged()
    */
    void onEditorContentChanged();
    
    /*!
        \fn void dialpadClosed()
    */
    void dialpadClosed();
    
    /*!
        \fn void networkNameChanged(
                QSystemNetworkInfo::NetworkMode mode, const QString &netName)
    */
    void networkNameChanged(
            QSystemNetworkInfo::NetworkMode mode, const QString &netName);
    
    /*!
        \fn void PhoneUIQtView::actionTriggerred()
    */
    void actionTriggerred();


    /*!
        \fn void setDialpadPosition(Qt::Orientation orientation)
        
        Sets dialpad position.
    */
    void setDialpadPosition(Qt::Orientation orientation);
    

    
public:
    PhoneUIQtView       &m_parent;
    HbMainWindow        &m_window;
    BubbleManager       *m_bubbleManager;
    QMap<int , QList<HbAction *> > m_bubbleActionMap;
    HbVolumeSliderPopup *m_volumeSlider;
    QMap<int , HbAction*> m_expandActionMap;
    QList<HbAction *>   m_participantListActions;
    int                 m_volumeCommandId;
    HbAction            *m_backAction;
    Dialpad             *m_dialpad;
    XQKeyCapture        *m_keyCapture;
    QSystemNetworkInfo  *m_networkInfo;
    QList<Qt::Key>      m_keyCaptures;
    DialpadKeyHandler   *m_dialpadKeyHandler;
    bool                m_restrictedMode;
    HbMenu              *m_optionsMenu;
};

#endif // PHONEUIQTVIEW_P_H
