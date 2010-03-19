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
#include "bubbleimagewidget.h"

class ut_BubbleImageWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testPixmap();
    void testWidePixmap();
    void testVector();

private:
    BubbleImageWidget* mImage;
    HbMainWindow* mMainWindow;
    int mStyleBaseId;
};

void ut_BubbleImageWidget::initTestCase()
{
    mMainWindow = new HbMainWindow();
    mImage = new BubbleImageWidget();
    mMainWindow->addView(mImage);
    mMainWindow->show();
}

void ut_BubbleImageWidget::cleanupTestCase()
{
    delete mMainWindow;
}

void ut_BubbleImageWidget::testPixmap()
{    
    mImage->setImage(":/data/pixmap.png");
    mImage->update();
    // set same image again
    mImage->setImage(":/data/pixmap.png");
    QTest::qWait(100);
    // there is now way to verify using public API
}

void ut_BubbleImageWidget::testWidePixmap()
{
    mImage->setImage(":/data/pixmap_wide.png");
    mImage->update();
    QTest::qWait(100);
    // there is now way to verify using public API
}

void ut_BubbleImageWidget::testVector()
{
    mImage->setImage(":/data/vector.svg");
    mImage->update();
    QTest::qWait(100);
    // there is now way to verify using public API
}

BUBBLE_TEST_MAIN(ut_BubbleImageWidget)
#include "ut_bubbleimagewidget.moc"
