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
#include <hbwidget.h>

#include "bubbletest.h"
#include "bubbleanimiconitem.h"

class Container : public HbWidget
{
public:
    Container()
    {
        mItem = new BubbleAnimIconItem(50,this);
        mItem->appendIcon(":/data/qtg_indi_call_connect.svg");
        mItem->appendIcon(":/data/qtg_indi_call_wait.svg");
        QGraphicsLinearLayout* layout = new
            QGraphicsLinearLayout;
        layout->addItem(mItem);
        setLayout(layout);
    }

    void reset()
    {
        mItem->reset();
    }

private:
    BubbleAnimIconItem* mItem;
};

class ut_BubbleAnimIconItem : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testItem();

private:
    HbMainWindow* mMainWindow;
    Container* mContainer;
    int mStyleBaseId;
};

void ut_BubbleAnimIconItem::initTestCase()
{
    mMainWindow = new HbMainWindow();
    mContainer = new Container;
    mMainWindow->addView(mContainer);
    mContainer->hide();
    mMainWindow->show();
}

void ut_BubbleAnimIconItem::cleanupTestCase()
{
    delete mMainWindow;
}

void ut_BubbleAnimIconItem::testItem()
{
    mContainer->show();
    QTest::qWait(200);
    mContainer->hide();
}

BUBBLE_TEST_MAIN(ut_BubbleAnimIconItem)
#include "ut_bubbleanimiconitem.moc"
