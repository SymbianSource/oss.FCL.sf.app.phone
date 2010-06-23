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
#include "bubbleimagemanager.h"

class ut_BubbleImageWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testPixmap();
    void testWidePixmap();
    void testDefaultAvatar();

    void testSetKeepSquareShape();

private:
    QString fileNameWithPath(const QString& fileName);    

private:
    BubbleImageWidget* mImage;
    HbMainWindow* mMainWindow;
    BubbleImageManager* mImageManager;
};

QString ut_BubbleImageWidget::fileNameWithPath(const QString& fileName)
{
#ifdef __WINS__
    return "c:/data/images/" + fileName;
#else
    return ":/data/" + fileName;
#endif
}

void ut_BubbleImageWidget::initTestCase()
{
    mMainWindow = new HbMainWindow();
    mImageManager = new BubbleImageManager();
    mImage = new BubbleImageWidget(*mImageManager);
    mMainWindow->addView(mImage);
    mMainWindow->show();
}

void ut_BubbleImageWidget::cleanupTestCase()
{
    delete mMainWindow;
    delete mImageManager;
}

void ut_BubbleImageWidget::testPixmap()
{    
    mImage->hide();
    mImage->setImage(fileNameWithPath("pixmap.png"));
    mImage->show();
    // set same image again
    QTest::qWait(500);
    // there is now way to verify using public API
}

void ut_BubbleImageWidget::testWidePixmap()
{
    mImage->hide();
    mImage->setImage(fileNameWithPath("pixmap_wide.png"));
    mImage->show();
    QTest::qWait(500);
    // there is now way to verify using public API
}

void ut_BubbleImageWidget::testDefaultAvatar()
{
    mImage->hide();
    mImage->setImage("");
    mImage->show();
    QTest::qWait(500);
    // there is now way to verify using public API
}

void ut_BubbleImageWidget::testSetKeepSquareShape()
{
    mImage->setKeepSquareShape(true);
    QVERIFY(mImage->keepSquareShape()==true);
    QVERIFY(mImage->sizePolicy().verticalPolicy()==QSizePolicy::Expanding);
    QVERIFY(mImage->sizePolicy().horizontalPolicy()==QSizePolicy::Fixed);
    mImage->setKeepSquareShape(false);
    QVERIFY(mImage->keepSquareShape()==false);
    QVERIFY(mImage->sizePolicy().verticalPolicy()==QSizePolicy::Expanding);
    QVERIFY(mImage->sizePolicy().horizontalPolicy()==QSizePolicy::Expanding);
}

BUBBLE_TEST_MAIN(ut_BubbleImageWidget)
#include "ut_bubbleimagewidget.moc"
