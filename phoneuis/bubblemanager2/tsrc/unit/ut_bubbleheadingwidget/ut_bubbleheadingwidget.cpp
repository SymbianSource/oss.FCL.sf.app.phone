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
#include <hbaction.h>
#include <hbmainwindow.h>
#include <hbinstance.h>
#include <hbstyle.h>

#include "bubbletest.h"
#include "bubbleheadingwidget.h"
#include "bubbleheader.h"
#include "bubblemanagerif.h"

class ut_BubbleHeadingWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanupTest();

    void testThreeLinedHeading();
    void testTwoLinedHeading();
    void testOneLinedHeading();

private:
    BubbleHeadingWidget* mHeading;
    HbMainWindow* mMainWindow;
    int mStyleBaseId;
};

void ut_BubbleHeadingWidget::initTestCase()
{
    mMainWindow = new HbMainWindow();
    mHeading = new BubbleHeadingWidget(BUBBLE_STYLE_PLUGIN);
    mMainWindow->addView(mHeading);
    mMainWindow->show();
}

void ut_BubbleHeadingWidget::cleanupTestCase()
{
    delete mMainWindow;
}

void ut_BubbleHeadingWidget::cleanupTest()
{
    mHeading->hide();
    mHeading->reset();
}

void ut_BubbleHeadingWidget::testThreeLinedHeading()
{
    BubbleHeader header;
    header.setCli("John Doe",Qt::ElideRight);
    header.setSecondaryCli("12345",Qt::ElideLeft);
    header.setText("",Qt::ElideRight);
    header.setTimerCost("0:00");
    header.setCallState(BubbleManagerIF::Active);
    header.setCallFlag(BubbleManagerIF::NoCiphering);
    mHeading->setLineCount(3);
    Q_ASSERT(mHeading->lineCount()==3);
    mHeading->readBubbleHeader(header);
    mHeading->show();
    QTest::qWait(1000);
    mHeading->updateTimerDisplayNow();
}

void ut_BubbleHeadingWidget::testTwoLinedHeading()
{
    BubbleHeader header;
    header.setCli("John Doe",Qt::ElideRight);
    header.setSecondaryCli("12345",Qt::ElideLeft);
    header.setText("",Qt::ElideRight);
    header.setTimerCost("0:00");
    header.setCallState(BubbleManagerIF::Active);
    header.setCallFlag(BubbleManagerIF::NoCiphering);

    mHeading->setLineCount(2);
    Q_ASSERT(mHeading->lineCount()==2);
    mHeading->readBubbleHeader(header);
    mHeading->show();
    QTest::qWait(100);
    mHeading->updateTimerDisplayNow();
}

void ut_BubbleHeadingWidget::testOneLinedHeading()
{
    BubbleHeader header;
    header.setCli("John Doe",Qt::ElideRight);
    header.setSecondaryCli("12345",Qt::ElideLeft);
    header.setText("",Qt::ElideRight);
    header.setTimerCost("0:00");
    header.setCallState(BubbleManagerIF::Active);
    header.setCallFlag(BubbleManagerIF::NoCiphering);

    mHeading->setLineCount(1);
    Q_ASSERT(mHeading->lineCount()==1);
    header.setCallState(BubbleManagerIF::OnHold);
    mHeading->readBubbleHeader(header);
    mHeading->show();
    QTest::qWait(100);
    mHeading->updateTimerDisplayNow();
}

BUBBLE_TEST_MAIN(ut_BubbleHeadingWidget)
#include "ut_bubbleheadingwidget.moc"
