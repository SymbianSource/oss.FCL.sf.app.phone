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
#include <hbiconitem.h>

#include "bubbletest.h"
#include "bubbleimagewidget.h"
#include "bubbleimagemanager.h"

class ut_BubbleImageWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSetImage();

    void testSetKeepSquareShape();

    void testSizeHint();

private:
    QString fileNameWithPath(const QString& fileName);    

private:
    BubbleImageWidget* mImage;
    HbMainWindow* mMainWindow;
    BubbleImageManager* mImageManager;
    HbIconItem* mAvatar;
};

void ut_BubbleImageWidget::initTestCase()
{
    mMainWindow = new HbMainWindow();
    mImageManager = new BubbleImageManager();
    mImage = new BubbleImageWidget(*mImageManager);
    mMainWindow->addView(mImage);
    mMainWindow->show();

    mAvatar = qgraphicsitem_cast<HbIconItem*>(
            static_cast<HbWidget*>(mImage)->primitive("default_avatar"));
    QVERIFY(mAvatar);
    QVERIFY(mAvatar->alignment()==Qt::AlignCenter);
    QVERIFY(mAvatar->aspectRatioMode()==Qt::KeepAspectRatioByExpanding);
}

void ut_BubbleImageWidget::cleanupTestCase()
{
    delete mMainWindow;
    delete mImageManager;
}

void ut_BubbleImageWidget::testSetImage()
{
    mImage->setImage("");
    QVERIFY(mAvatar->iconName()=="qtg_large_avatar");
    QVERIFY(mAvatar->isVisible());

    QSignalSpy spy( mImageManager, SIGNAL(pixmapReady(QString)));
    mImage->setImage(":/data/pixmap.png");
    QVERIFY(mAvatar->isNull());
    QVERIFY(!mAvatar->isVisible());
    QVERIFY(spy.count()==1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString()==":/data/pixmap.png");

    mImage->show();
    QTest::qWait(100);
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

void ut_BubbleImageWidget::testSizeHint()
{
    mImage->setKeepSquareShape(true);
    QSizeF hint = mImage->preferredSize();
    // ..
}

BUBBLE_TEST_MAIN(ut_BubbleImageWidget)
#include "ut_bubbleimagewidget.moc"