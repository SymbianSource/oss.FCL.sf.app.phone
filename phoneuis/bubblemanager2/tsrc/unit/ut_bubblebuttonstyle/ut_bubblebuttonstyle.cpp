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

#include <QtGui>
#include <QtTest/QtTest>

#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbframeitem.h>
#include <hbiconitem.h>
#include <hbstyle.h>
#include <hbstyleoptionpushbutton.h>
#include <hbcolorscheme.h>

#include "bubbletest.h"
#include "bubblebuttonstyle.h"

class ut_BubbleButtonStyle : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testPushButtonFrame();
    void testPushButtonIcon();

private:
    BubbleButtonStyle *mStyle;
};

void ut_BubbleButtonStyle::initTestCase()
{
    mStyle = new BubbleButtonStyle();
}

void ut_BubbleButtonStyle::cleanupTestCase()
{
    delete mStyle;
}

void ut_BubbleButtonStyle::testPushButtonFrame()
{
    HbFrameItem* item = new HbFrameItem();
    HbStyleOptionPushButton option;
    option.backgroundFrameDrawer = 0;

    // Enabled ON
    option.state |= QStyle::State_On;
    option.state |= QStyle::State_Enabled;

    mStyle->setButtonColor(BubbleButtonStyle::Default);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_btn_pressed");

    mStyle->setButtonColor(BubbleButtonStyle::Green);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_btn_green_pressed");

    mStyle->setButtonColor(BubbleButtonStyle::Red);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_btn_red_pressed");

    // Enabled OFF
    option.state &= ~QStyle::State_On;
    option.state |= QStyle::State_Off;

    mStyle->setButtonColor(BubbleButtonStyle::Default);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_btn_normal");

    mStyle->setButtonColor(BubbleButtonStyle::Green);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_btn_green_normal");

    mStyle->setButtonColor(BubbleButtonStyle::Red);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_btn_red_normal");

    // Disabled
    option.state &= ~QStyle::State_Enabled;
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_btn_disabled");

    // Selected
    option.state |= QStyle::State_Selected;
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_btn_highlight");

    // Active
    option.state &= ~QStyle::State_Selected;
    option.state |= QStyle::State_Active;
    mStyle->setButtonColor(BubbleButtonStyle::Default);;
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_btn_normal");   

    delete item;

    // exception type
    HbIconItem* item2 = new HbIconItem();
    mStyle->updatePrimitive(item2,HbStyle::P_PushButton_background,&option);
    delete item2;
}

void ut_BubbleButtonStyle::testPushButtonIcon()
{
    HbIconItem* item = new HbIconItem();
    HbStyleOptionPushButton option;

    mStyle->setButtonColor(BubbleButtonStyle::Red);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_icon,&option);
    QVERIFY(item->color()==Qt::white);

    mStyle->setButtonColor(BubbleButtonStyle::Green);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_icon,&option);
    QVERIFY(item->color()==Qt::white);

    QColor color;
    color = HbColorScheme::color("qtc_button_normal");
    mStyle->setButtonColor(BubbleButtonStyle::Default);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_icon,&option);
    QVERIFY(item->color()==color);
}

BUBBLE_TEST_MAIN(ut_BubbleButtonStyle)
#include "ut_bubblebuttonstyle.moc"
