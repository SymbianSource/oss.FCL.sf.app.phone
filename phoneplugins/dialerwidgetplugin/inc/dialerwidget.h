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
* Description:
*
*/

#ifndef DIALERWIDGET_H
#define DIALERWIDGET_H

#include <hswidget.h>


#ifndef HOMESCREEN_TEST
    #define HOMESCREEN_TEST_CLASS(aClassName)
    #define HOMESCREEN_TEST_FRIEND_CLASS(aClassName)
#else
    #define HOMESCREEN_TEST_CLASS(aClassName) class aClassName;
    #define HOMESCREEN_TEST_FRIEND_CLASS(aClassName) friend class aClassName;
#endif //HOMESCREEN_TEST

HOMESCREEN_TEST_CLASS(TestDialerWidgetPlugin)


class DialerWidgetEngine;
class HbFrameItem;    
class HbIconItem;
class HbTextItem;
class HbTouchArea;

class DialerWidget : public HsWidget
{
    Q_OBJECT

public:
    DialerWidget(QGraphicsItem* parent = 0, Qt::WindowFlags flags = 0);
    ~DialerWidget();
    
public slots:
    void startDialer();
   
    // from HS fw
    void onInitialize();
    void onShow();
    void onHide();
    void onUninitialize();
    
    // from engine
    void onEngineException(const int& exc);
    
    void onMissedCallsCountChange(const int& count);
    
signals:
    void error();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void handleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    bool sceneEvent(QEvent *event);
    StartResult onStart();
    StopResult onStop();
    SuspendResult onSuspend();
    ResumeResult onResume();
    
private:    
    void createPrimitives();
    void setBackgroundToNormal();
    void setBackgroundToPressed();    
    
private:
    
    HbFrameItem         *m_background;
    HbFrameItem         *m_badgeBackground;
    HbTextItem          *m_text;
    HbTouchArea         *m_touchArea;
    DialerWidgetEngine  *m_engine;
    
    HOMESCREEN_TEST_FRIEND_CLASS(TestDialerWidgetPlugin)
};

#endif
