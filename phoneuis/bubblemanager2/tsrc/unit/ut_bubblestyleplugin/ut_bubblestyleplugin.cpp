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
#include <hbstyle.h>
#include <hbinstance.h>
#include <hbwidget.h>
#include <hbframeitem.h>
#include <hbframedrawer.h>
#include <hbtextitem.h>
#include <hbiconitem.h>

#include "bubbletest.h"
#include "bubblestyleplugin.h"
#include "bubbleprimitives.h"
#include "bubblestyleoption.h"
#include "bubbleanimiconitem.h"

class ut_BubbleStylePlugin : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testPrimitiveCount();
    void testLayoutPath();

    void testBubbleFrame();
    void testText1();
    void testText2();
    void testText3();
    void testCallStatusIcon();
    void testVideoCallStatusIcon();
    void testVoipCallStatusIcon();
    void testNumberTypeIcon();
    void testCipheringIcon();
    void testAvatar();

private:
    BubbleStylePlugin* mPluginInstance;
    HbStyleInterface*  mPlugin;
    HbWidget* mParent;
};

void ut_BubbleStylePlugin::initTestCase()
{
    mPluginInstance = new BubbleStylePlugin();
    mPlugin = static_cast<HbStyleInterface*>(mPluginInstance);
    mParent = new HbWidget();
}

void ut_BubbleStylePlugin::cleanupTestCase()
{
    delete mPluginInstance;
    delete mParent;
}

void ut_BubbleStylePlugin::testPrimitiveCount()
{
    QVERIFY(mPlugin->primitiveCount()==BP_Bubble_primitive_count);
}

void ut_BubbleStylePlugin::testLayoutPath()
{
    QVERIFY(mPlugin->layoutPath()==":/");
}

void ut_BubbleStylePlugin::testBubbleFrame()
{
    QGraphicsItem *item = mPlugin->createPrimitive(
        (HbStyle::Primitive)(BP_Bubble_frame),mParent);
    QVERIFY(item);
    QVERIFY(item->zValue()==-1.0);
    QVERIFY(item->parentItem()==mParent);

    HbFrameItem* frame = qgraphicsitem_cast<HbFrameItem*>(item);
    QVERIFY(frame);

    BubbleStyleOption option;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_Bubble_frame),
            &option);
    QVERIFY(frame->frameDrawer().frameGraphicsName()==
             "qtg_fr_list_normal");
}

void ut_BubbleStylePlugin::testText1()
{
    QGraphicsItem *item = mPlugin->createPrimitive(
        (HbStyle::Primitive)(BP_Text1_text),mParent);
    QVERIFY(item);
    QVERIFY(item->parentItem()==mParent);

    HbTextItem* text = qgraphicsitem_cast<HbTextItem*>(item);
    QVERIFY(text);

    BubbleStyleOption option;
    option.mText1 = "foo";
    option.mText1Clip = Qt::ElideRight;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_Text1_text),
            &option);
    QVERIFY(text->text()=="foo");
    QVERIFY(text->elideMode()==Qt::ElideRight);

}

void ut_BubbleStylePlugin::testText2()
{
    QGraphicsItem *item = mPlugin->createPrimitive(
        (HbStyle::Primitive)(BP_Text2_text),mParent);
    QVERIFY(item);
    QVERIFY(item->parentItem()==mParent);

    HbTextItem* text = qgraphicsitem_cast<HbTextItem*>(item);
    QVERIFY(text);

    BubbleStyleOption option;
    option.mText2 = "foo";
    option.mText2Clip = Qt::ElideRight;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_Text2_text),
            &option);
    QVERIFY(text->text()=="foo");
    QVERIFY(text->elideMode()==Qt::ElideRight);

}

void ut_BubbleStylePlugin::testText3()
{
    QGraphicsItem *item = mPlugin->createPrimitive(
        (HbStyle::Primitive)(BP_Text3_text),mParent);
    QVERIFY(item);
    QVERIFY(item->parentItem()==mParent);

    HbTextItem* text = qgraphicsitem_cast<HbTextItem*>(item);
    QVERIFY(text);

    BubbleStyleOption option;
    option.mText3 = "foo";
    option.mText3Clip = Qt::ElideLeft;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_Text3_text),
            &option);
    QVERIFY(text->text()=="foo");
    QVERIFY(text->elideMode()==Qt::ElideLeft);

}

void ut_BubbleStylePlugin::testCallStatusIcon()
{
    QGraphicsItem *item = mPlugin->createPrimitive(
        (HbStyle::Primitive)(BP_CallStatus_icon),mParent);
    QVERIFY(item);
    QVERIFY(item->parentItem()==mParent);

    BubbleAnimIconItem* icon = static_cast<BubbleAnimIconItem*>(item);
    QVERIFY(icon);

    BubbleStyleOption option;
    option.mCallState = BubbleManagerIF::Active;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::OnHold;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Disconnected;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::AlertToDisconnected;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Incoming;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Alerting;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Waiting;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Outgoing;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
}

void ut_BubbleStylePlugin::testVideoCallStatusIcon()
{
    QGraphicsItem *item = mPlugin->createPrimitive(
        (HbStyle::Primitive)(BP_CallStatus_icon),mParent);
    QVERIFY(item);
    QVERIFY(item->parentItem()==mParent);

    BubbleAnimIconItem* icon = static_cast<BubbleAnimIconItem*>(item);
    QVERIFY(icon);

    BubbleStyleOption option;
    option.mCallFlags |= BubbleManagerIF::Video;
    option.mCallState = BubbleManagerIF::Active;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::OnHold;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Disconnected;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::AlertToDisconnected;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Incoming;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Alerting;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Waiting;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Outgoing;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
}

void ut_BubbleStylePlugin::testVoipCallStatusIcon()
{
    QGraphicsItem *item = mPlugin->createPrimitive(
        (HbStyle::Primitive)(BP_CallStatus_icon),mParent);
    QVERIFY(item);
    QVERIFY(item->parentItem()==mParent);

    BubbleAnimIconItem* icon = static_cast<BubbleAnimIconItem*>(item);
    QVERIFY(icon);

    BubbleStyleOption option;
    option.mCallFlags |= BubbleManagerIF::VoIPCall;
    option.mCallState = BubbleManagerIF::Active;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::OnHold;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Disconnected;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::AlertToDisconnected;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Incoming;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Alerting;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Waiting;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
    option.mCallState = BubbleManagerIF::Outgoing;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_CallStatus_icon),
            &option);
}

void ut_BubbleStylePlugin::testNumberTypeIcon()
{
    QGraphicsItem *item = mPlugin->createPrimitive(
        (HbStyle::Primitive)(BP_NumberType_icon),mParent);
    QVERIFY(item);
    QVERIFY(item->parentItem()==mParent);

    HbIconItem* icon = qgraphicsitem_cast<HbIconItem*>(item);
    QVERIFY(icon);
    BubbleStyleOption option;
    // incoming call
    option.mCallState = BubbleManagerIF::Incoming;
    option.mCallFlags |= BubbleManagerIF::Diverted;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_NumberType_icon),
            &option);
    QVERIFY(icon->icon().iconName()==":/qgn_indi_call_diverted.svg");
    option.mCallState = BubbleManagerIF::Incoming;
    option.mCallFlags &= ~BubbleManagerIF::Diverted;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_NumberType_icon),
            &option);
    QVERIFY(icon->icon().iconName()=="");

    // waiting call
    option.mCallState = BubbleManagerIF::Waiting;
    option.mCallFlags |= BubbleManagerIF::Diverted;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_NumberType_icon),
            &option);
    QVERIFY(icon->icon().iconName()==":/qgn_indi_call_diverted.svg");
    option.mCallState = BubbleManagerIF::Waiting;
    option.mCallFlags &= ~BubbleManagerIF::Diverted;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_NumberType_icon),
            &option);
    QVERIFY(icon->icon().iconName()=="");
}

void ut_BubbleStylePlugin::testCipheringIcon()
{
    QGraphicsItem *item = mPlugin->createPrimitive(
        (HbStyle::Primitive)(BP_Ciphering_icon),mParent);
    QVERIFY(item);
    QVERIFY(item->parentItem()==mParent);

    HbIconItem* icon = qgraphicsitem_cast<HbIconItem*>(item);
    QVERIFY(icon);
    BubbleStyleOption option;
    option.mCallState = BubbleManagerIF::Active;
    option.mCallFlags |= BubbleManagerIF::NoCiphering;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_Ciphering_icon),
            &option);
    QVERIFY(icon->icon().iconName()=="qtg_mono_ciphering_off");
    option.mCallState = BubbleManagerIF::Incoming;
    option.mCallFlags &= ~BubbleManagerIF::NoCiphering;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_Ciphering_icon),
            &option);
    QVERIFY(icon->icon().iconName()=="");
}

void ut_BubbleStylePlugin::testAvatar()
{
    QGraphicsItem *item = mPlugin->createPrimitive(
        (HbStyle::Primitive)(BP_DefaultAvatar_icon),mParent);
    QVERIFY(item);
    QVERIFY(item->parentItem()==mParent);

    HbIconItem* icon = qgraphicsitem_cast<HbIconItem*>(item);
    QVERIFY(icon);
    BubbleStyleOption option;
    mPlugin->updatePrimitive(
            item, (HbStyle::Primitive)(BP_DefaultAvatar_icon),
            &option);
    QVERIFY(icon->icon().iconName()=="qtg_large_avatar");
}

BUBBLE_TEST_MAIN(ut_BubbleStylePlugin)
#include "ut_bubblestyleplugin.moc"
