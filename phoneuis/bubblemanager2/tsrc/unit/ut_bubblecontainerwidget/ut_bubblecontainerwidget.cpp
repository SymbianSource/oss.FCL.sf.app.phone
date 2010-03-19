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

#include <QtGui>
#include <QtTest/QtTest>

#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbaction.h>
#include <hbmainwindow.h>
#include <hbinstance.h>
#include <hbstyle.h>

#include "bubbletest.h"
#include "bubblecontainerwidget.h"

class ut_BubbleContainerWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testLeftMouseClick();
    void testRightMouseClick();
    void testMouseMove();

    //void testInvalidStylePlugin();

private:
    BubbleContainerWidget* mContainer;
    HbMainWindow* mMainWindow;
    int mStyleBaseId;
};

void ut_BubbleContainerWidget::initTestCase()
{
    mMainWindow = new HbMainWindow();
    mContainer = new BubbleContainerWidget(BUBBLE_STYLE_PLUGIN);
    mMainWindow->addView(mContainer);
    mMainWindow->show();
}

void ut_BubbleContainerWidget::cleanupTestCase()
{
    delete mMainWindow;
}

void ut_BubbleContainerWidget::testLeftMouseClick()
{
    QSignalSpy spy(mContainer,SIGNAL(clicked()));

    QPointF clickPos = mContainer->scenePos() +
                       mContainer->rect().center();
    QPoint windowPos = mMainWindow->mapFromScene( clickPos );

    QTest::mouseClick( mMainWindow->viewport(), Qt::LeftButton, 0, windowPos,100 );
    QCOMPARE( spy.count(), 1 );
}

void ut_BubbleContainerWidget::testRightMouseClick()
{
    QSignalSpy spy(mContainer,SIGNAL(clicked()));

    QPointF clickPos = mContainer->scenePos() +
                       mContainer->rect().center();
    QPoint windowPos = mMainWindow->mapFromScene( clickPos );

    QTest::mouseClick( mMainWindow->viewport(), Qt::RightButton, 0, windowPos,100 );
    QCOMPARE( spy.count(), 0 );
}

void ut_BubbleContainerWidget::testMouseMove()
{
    QSignalSpy spy(mContainer,SIGNAL(clicked()));

    QPointF clickPos = mContainer->scenePos() +
                       mContainer->rect().center();
    QPoint windowPos = mMainWindow->mapFromScene( clickPos );
    QPoint windowPos2 = windowPos+QPoint(10,10);

    // QTest::mouseMove() doesn't work
    QTest::mouseMove( mMainWindow->viewport(), windowPos2 );
    QCOMPARE( spy.count(), 0 );
}

/*void ut_BubbleContainerWidget::testInvalidStylePlugin()
{
    // this just shouldn't crash
    BubbleContainerWidget *cnt =
        new BubbleContainerWidget("c:\foobar.dll");
    HbView* view = mMainWindow->addView(cnt);
    mMainWindow->setCurrentView(view);
}*/

BUBBLE_TEST_MAIN(ut_BubbleContainerWidget)
#include "ut_bubblecontainerwidget.moc"
