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

#ifndef HSDIALERWIDGET_H
#define HSDIALERWIDGET_H

#include <HbWidget>

#ifndef HOMESCREEN_TEST
    #define HOMESCREEN_TEST_CLASS(aClassName)
    #define HOMESCREEN_TEST_FRIEND_CLASS(aClassName)
#else
    #define HOMESCREEN_TEST_CLASS(aClassName) class aClassName;
    #define HOMESCREEN_TEST_FRIEND_CLASS(aClassName) friend class aClassName;
#endif //HOMESCREEN_TEST

HOMESCREEN_TEST_CLASS(TestDialerWidgetPlugin)

class XQCallInfo;

class HsDialerWidget : public HbWidget
{
    Q_OBJECT

public:
    HsDialerWidget(QGraphicsItem* parent = 0, Qt::WindowFlags flags = 0);
    ~HsDialerWidget();
    
public slots:
    void startDialer();

    void onShow();
    void onHide();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) { Q_UNUSED(event) }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    XQCallInfo *mXQCallInfo;
    HOMESCREEN_TEST_FRIEND_CLASS(TestDialerWidgetPlugin)
};

#endif
