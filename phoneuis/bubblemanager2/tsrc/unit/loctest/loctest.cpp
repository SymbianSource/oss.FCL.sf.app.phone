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
* Description: Test cases to measure localisation metrics.
*
*/

#include <QtGui>
#include <QtTest/QtTest>

#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbaction.h>
#include <hbtoolbar.h>
#include <hbview.h>
#include <private/hbtextmeasurementutility_p.h> // hb "configure --developer" required
#include <hbdeviceprofile.h>

#include "bubbletest.h"
#include "bubblemanager2.h"

const int WAIT_TIME = 1000;

class LocTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testPortraitStrings();

    void changeOrientation();

    void testLandscapeStrings();

private:
    void testLabel(int bubbleId, const char *id);
    void testCli(int bubbleId, const char *id);

private:
    HbMainWindow*  mainWindow;
    BubbleManager* bubbleManager; // widget under test
    HbAction*      toolbarAction;
    HbView*        view; // not owned
};

void LocTest::initTestCase()
{
    mainWindow = new HbMainWindow;

    bubbleManager = new BubbleManager();
    connect(mainWindow,SIGNAL(orientationChanged(Qt::Orientation)),
            bubbleManager,SLOT(handleOrientationChange(Qt::Orientation)));

    view = mainWindow->addView( bubbleManager );

    HbToolBar *toolBar = new HbToolBar();
    toolbarAction = new HbAction(this);
    toolBar->addAction( toolbarAction );
    toolBar->addAction( toolbarAction );
    mainWindow->currentView()->setToolBar(toolBar);

    mainWindow->show();
    QTest::qWait( 3000 );
}

void LocTest::cleanupTestCase()
{
    delete mainWindow;
    delete toolbarAction;
}

void LocTest::testPortraitStrings()
{
    view->setTitle( "Measuring portrait" );

    // In portrait all text fields have same width, only font
    // is different between label and cli field.

    // create test bubble
    bubbleManager->startChanges();
    int bubbleId = bubbleManager->createCallHeader();
    bubbleManager->setState( bubbleId, BubbleManager::Incoming );
    HbAction action1("Answer", this);
    action1.setSoftKeyRole(QAction::PositiveSoftKey);
    bubbleManager->addAction( bubbleId, &action1 );
    HbAction action2("Reject", this);
    action2.setSoftKeyRole(QAction::NegativeSoftKey);
    bubbleManager->addAction( bubbleId, &action2 );
    bubbleManager->setCallObjectFromTheme(bubbleId);
    bubbleManager->endChanges();

    QTest::qWait( WAIT_TIME );

    // measure label texts
    testLabel(bubbleId,"txt_phone_other_calling");

    testLabel(bubbleId,"txt_phone_other_incoming_call");

    testLabel(bubbleId,"txt_phone_other_waiting");

    testLabel(bubbleId,"txt_phone_other_on_hold");

    testLabel(bubbleId,"txt_phone_other_disconnected");

    testLabel(bubbleId,"txt_phone_other_attempting");

    // measure cli texts
    testCli(bubbleId,"txt_phone_other_conference_call");

    testCli(bubbleId,"txt_phone_other_unknown_number");

    testCli(bubbleId,"txt_phone_other_remote_sim");

    testCli(bubbleId,"txt_phone_other_private_number");

    testCli(bubbleId,"txt_phone_other_emergency_call");

    // write results
    QTest::qWait( WAIT_TIME );
    HbDeviceProfile profile = HbDeviceProfile::profile(mainWindow);
    QFileInfo info(QCoreApplication::applicationFilePath());
    HbTextMeasurementUtility* utility = HbTextMeasurementUtility::instance();
    utility->writeReport(profile, info.baseName());
    utility->reset();

    QTest::qWait( WAIT_TIME );
    bubbleManager->startChanges();
    bubbleManager->removeCallHeader( bubbleId );
    bubbleManager->endChanges();
    QTest::qWait( WAIT_TIME );
}

void LocTest::changeOrientation()
{
    view->setTitle( "Changing orientation" );
    mainWindow->setOrientation(Qt::Horizontal);
    mainWindow->resize(640,360);
    QTest::qWait(3000);
}

void LocTest::testLandscapeStrings()
{
    view->setTitle( "Measuring landscape" );

    // In landscape text field width depends on call situation,
    // measure according to most narrow case.

    // create test bubble
    bubbleManager->startChanges();
    int bubbleId = bubbleManager->createCallHeader();
    bubbleManager->setState( bubbleId, BubbleManager::OnHold );
    HbAction action1("End call", this);
    action1.setSoftKeyRole(QAction::PositiveSoftKey);
    bubbleManager->addAction( bubbleId, &action1 );
    bubbleManager->setCallObjectFromTheme(bubbleId);
    bubbleManager->endChanges();


    // only in single call texts
    testLabel(bubbleId,"txt_phone_other_incoming_call");

    testLabel(bubbleId,"txt_phone_other_attempting");

    testCli(bubbleId,"txt_phone_other_emergency_call");

    // multicall
    bubbleManager->startChanges();
    int bubbleId2 = bubbleManager->createCallHeader();
    bubbleManager->setState( bubbleId2, BubbleManager::OnHold );
    int bubbleId3 = bubbleManager->createCallHeader();
    bubbleManager->setState( bubbleId3, BubbleManager::Waiting );
    HbAction action2("Replace", this);
    action2.setSoftKeyRole(QAction::PositiveSoftKey);
    bubbleManager->addAction( bubbleId3, &action1 );
    bubbleManager->setCallObjectFromTheme(bubbleId3);
    bubbleManager->endChanges();

    // measure label texts
    testLabel(bubbleId2,"txt_phone_other_calling");

    testLabel(bubbleId2,"txt_phone_other_waiting");

    testLabel(bubbleId2,"txt_phone_other_on_hold");

    testLabel(bubbleId2,"txt_phone_other_disconnected");

    // measure cli texts
    testCli(bubbleId2,"txt_phone_other_conference_call");

    testCli(bubbleId3,"txt_phone_other_unknown_number");

    testCli(bubbleId3,"txt_phone_other_remote_sim");

    testCli(bubbleId3,"txt_phone_other_private_number");

    // write results
    QTest::qWait( WAIT_TIME );
    HbDeviceProfile profile = HbDeviceProfile::profile(mainWindow);
    QFileInfo info(QCoreApplication::applicationFilePath());
    HbTextMeasurementUtility* utility = HbTextMeasurementUtility::instance();
    utility->writeReport(profile, info.baseName());
    utility->reset();

    QTest::qWait( WAIT_TIME );
    bubbleManager->startChanges();
    bubbleManager->removeCallHeader( bubbleId );
    bubbleManager->removeCallHeader( bubbleId2 );
    bubbleManager->removeCallHeader( bubbleId3 );
    bubbleManager->endChanges();
    QTest::qWait( WAIT_TIME );
}

void LocTest::testLabel(int bubbleId, const char *id)
{
    HbTextMeasurementUtility* utility = HbTextMeasurementUtility::instance();
    bubbleManager->startChanges();
    bubbleManager->setLabel( bubbleId, hbTrId(id));
    bubbleManager->endChanges();
    utility->measureItems(100);
    QTest::qWait( WAIT_TIME );
    bubbleManager->startChanges();
    bubbleManager->setLabel( bubbleId, "");
    bubbleManager->endChanges();
}

void LocTest::testCli(int bubbleId, const char *id)
{
    HbTextMeasurementUtility* utility = HbTextMeasurementUtility::instance();
    bubbleManager->startChanges();
    bubbleManager->setCli( bubbleId, hbTrId(id), Qt::ElideRight);
    bubbleManager->endChanges();
    utility->measureItems(100);
    QTest::qWait( WAIT_TIME );
    bubbleManager->startChanges();
    bubbleManager->setCli( bubbleId, "", Qt::ElideRight);
    bubbleManager->endChanges();
}

BUBBLE_TEST_MAIN(LocTest)
#include "loctest.moc"

