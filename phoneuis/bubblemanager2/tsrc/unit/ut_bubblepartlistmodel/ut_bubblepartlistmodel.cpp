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
#include <hbtextitem.h>
#include <hbiconitem.h>

#include "bubbletest.h"
#include "bubbleparticipantlistmodel.h"

class ut_BubbleParticipantListModel : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    void test_addParticipant();
    void test_bubbleId();
    void test_dataModel();
    void test_reset();

private:
    BubbleParticipantListModel *mModel;
};

void ut_BubbleParticipantListModel::initTestCase()
{
    mModel = new BubbleParticipantListModel();
}

void ut_BubbleParticipantListModel::cleanupTestCase()
{
    delete mModel;
}

void ut_BubbleParticipantListModel::test_addParticipant()
{
    mModel->addParticipant(1,"Bubble-1",8);
    mModel->addParticipant(2,"Bubble-2",9);
    mModel->addParticipant(3,"Bubble-3",10);

    QVERIFY(mModel->rowCount()==3);
}

void ut_BubbleParticipantListModel::test_bubbleId()
{
    QVERIFY(mModel->bubbleId(1)==2);
    QVERIFY(mModel->bubbleId(4)==-1);
}

void ut_BubbleParticipantListModel::test_dataModel()
{
    QModelIndex index = mModel->index(1,0);

    QVERIFY(index.data(Qt::DecorationRole).toInt()==9);
    QVERIFY(index.data(Qt::DisplayRole).toString()=="Bubble-2");
    QVERIFY(index.data(Qt::UserRole).isNull());

    index = mModel->index(4,0);
    QVERIFY(index.data(Qt::DisplayRole).isNull());
}

void ut_BubbleParticipantListModel::test_reset()
{
    mModel->reset();
    QVERIFY(mModel->rowCount()==0);
}

BUBBLE_TEST_MAIN(ut_BubbleParticipantListModel)
#include "ut_bubblepartlistmodel.moc"
