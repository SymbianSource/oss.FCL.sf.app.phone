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
#include <hbpushbutton.h>
#include <hbaction.h>

#include "bubbletest.h"
#include "bubbleutils.h"
#include "bubbleheader.h"
#include "bubblestyleoption.h"
#include "bubblebuttonstyle.h"

class BubbleButtonStyleMock : public BubbleButtonStyle
{
public:
    BubbleButtonStyleMock() {};
    BubbleButtonStyle::ButtonColor color() {return mButtonColor; };
};

class ut_BubbleUtils : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_setCallHeaderTexts3Lines_incoming();
    void test_setCallHeaderTexts3Lines_waiting();
    void test_setCallHeaderTexts3Lines_active();
    void test_setCallHeaderTexts3Lines_disconnected();
    void test_setCallHeaderTexts3Lines_outgoing();
    void test_setCallHeaderTexts3Lines_alerting();
    void test_setCallHeaderTexts3Lines_held();
    void test_setCallHeaderTexts3Lines_alerttodisconnected();

    void test_setCallHeaderTexts2Lines_waiting();
    void test_setCallHeaderTexts2Lines_alerttodisconnected();
    void test_setCallHeaderTexts2Lines_active();
    void test_setCallHeaderTexts2Lines_held();
    void test_setCallHeaderTexts2Lines_disconnected();
    void test_setCallHeaderTexts2Lines_outgoing();

    void test_compareHeaders();

    void test_stylePluginNameWithPath();

    void test_setButtonStyleForAction();

private:
};

void ut_BubbleUtils::initTestCase()
{
}

void ut_BubbleUtils::cleanupTestCase()
{
}

void ut_BubbleUtils::test_setCallHeaderTexts3Lines_incoming()
{
    // incoming call
    BubbleHeader header;
    BubbleStyleOption opt;
    
    header.setCallState( BubbleManager::Incoming );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "calling", Qt::ElideRight );
    
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
    
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "calling" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );
    
    // add secondary cli
    opt = BubbleStyleOption();
    header.setSecondaryCli( "12345", Qt::ElideLeft );
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
        
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "12345" );
    QVERIFY( opt.mText2Clip == Qt::ElideLeft );
    QVERIFY( opt.mText3 == "calling" );
}

void ut_BubbleUtils::test_setCallHeaderTexts3Lines_waiting()
{
    // incoming call
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::Waiting );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "waiting", Qt::ElideRight );

    BubbleUtils::setCallHeaderTexts3Lines(header, opt);

    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "waiting" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );

    // add secondary cli
    opt = BubbleStyleOption();
    header.setSecondaryCli( "12345", Qt::ElideLeft );
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);

    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "12345" );
    QVERIFY( opt.mText2Clip == Qt::ElideLeft );
    QVERIFY( opt.mText3 == "waiting" );
}

void ut_BubbleUtils::test_setCallHeaderTexts3Lines_active()
{
    // active call
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::Active );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "", Qt::ElideRight );
    
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
    
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "" );
    QVERIFY( opt.mText3 == "" );
    
    // add timer
    opt = BubbleStyleOption();
    header.setTimerCost( "00:00" );
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "00:00" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );
    header.setTimerCost( "" );
    
    // add secondary cli
    opt = BubbleStyleOption();
    header.setSecondaryCli( "12345", Qt::ElideLeft );
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
        
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "12345" );
    QVERIFY( opt.mText2Clip == Qt::ElideLeft );
    QVERIFY( opt.mText3 == "" );
    
    // add timer
    opt = BubbleStyleOption();
    header.setTimerCost( "00:00" );
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "12345" );
    QVERIFY( opt.mText2Clip == Qt::ElideLeft );
    QVERIFY( opt.mText3 == "00:00" );
    QVERIFY( opt.mText3Clip == Qt::ElideRight );
}

void ut_BubbleUtils::test_setCallHeaderTexts3Lines_disconnected()
{
    // disconnected call
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::Disconnected );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "discon", Qt::ElideRight );
    header.setTimerCost( "00:00" );
    
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "discon" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );
    
    // add secondary cli
    opt = BubbleStyleOption();
    header.setSecondaryCli( "12345", Qt::ElideLeft );
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
        
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "12345" );
    QVERIFY( opt.mText2Clip == Qt::ElideLeft );
    QVERIFY( opt.mText3 == "discon" );
    QVERIFY( opt.mText3Clip == Qt::ElideRight );
}

void ut_BubbleUtils::test_setCallHeaderTexts3Lines_outgoing()
{
    // outgoing call
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::Outgoing );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "calling", Qt::ElideRight );
    
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
    
    QVERIFY( opt.mText1 == "calling" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "john" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );
    
    // add secondary cli
    opt = BubbleStyleOption();
    header.setSecondaryCli( "12345", Qt::ElideLeft );
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
        
    QVERIFY( opt.mText1 == "calling" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "john" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "12345" );
    QVERIFY( opt.mText3Clip == Qt::ElideLeft );
}

void ut_BubbleUtils::test_setCallHeaderTexts3Lines_alerting()
{
    // alerting call
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::Alerting );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "calling", Qt::ElideRight );
    
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
    
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "" );
    QVERIFY( opt.mText3 == "" );
    
    // add secondary cli
    opt = BubbleStyleOption();
    header.setSecondaryCli( "12345", Qt::ElideLeft );
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
        
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "12345" );
    QVERIFY( opt.mText2Clip == Qt::ElideLeft );
    QVERIFY( opt.mText3 == "" );
}

void ut_BubbleUtils::test_setCallHeaderTexts3Lines_held()
{
    // held
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::OnHold );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "on hold", Qt::ElideRight );
    header.setTimerCost( "00:00" );
    
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
    
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "on hold" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );
    
    // add secondary cli
    opt = BubbleStyleOption();
    header.setSecondaryCli( "12345", Qt::ElideLeft );
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);
        
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "12345" );
    QVERIFY( opt.mText2Clip == Qt::ElideLeft );
    QVERIFY( opt.mText3 == "on hold" );
    QVERIFY( opt.mText3Clip == Qt::ElideRight );
}

void ut_BubbleUtils::test_setCallHeaderTexts3Lines_alerttodisconnected()
{
    // incoming call
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::Incoming );
    header.setCallState( BubbleManager::AlertToDisconnected );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "disconnected", Qt::ElideRight );

    BubbleUtils::setCallHeaderTexts3Lines(header, opt);

    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "disconnected" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );

    // add secondary cli
    opt = BubbleStyleOption();
    header.setSecondaryCli( "12345", Qt::ElideLeft );
    BubbleUtils::setCallHeaderTexts3Lines(header, opt);

    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "12345" );
    QVERIFY( opt.mText2Clip == Qt::ElideLeft );
    QVERIFY( opt.mText3 == "disconnected" );
}

void ut_BubbleUtils::test_setCallHeaderTexts2Lines_waiting()
{
    // waiting call
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::Waiting );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "waiting", Qt::ElideRight );

    BubbleUtils::setCallHeaderTexts2Lines(header, opt);

    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "waiting" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );

    // add secondary cli
    opt = BubbleStyleOption();
    header.setSecondaryCli("12345",Qt::ElideLeft);
    BubbleUtils::setCallHeaderTexts2Lines(header, opt);
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "waiting" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );
}

void ut_BubbleUtils::test_setCallHeaderTexts2Lines_alerttodisconnected()
{
    // waiting call
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::Waiting );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "disconnected", Qt::ElideRight );

    BubbleUtils::setCallHeaderTexts2Lines(header, opt);

    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "disconnected" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );

    // add secondary cli
    opt = BubbleStyleOption();
    header.setSecondaryCli("12345",Qt::ElideLeft);
    BubbleUtils::setCallHeaderTexts2Lines(header, opt);
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "disconnected" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );
}

void ut_BubbleUtils::test_setCallHeaderTexts2Lines_active()
{
    // active call
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::Active );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "", Qt::ElideRight );

    BubbleUtils::setCallHeaderTexts2Lines(header, opt);

    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "" );
    QVERIFY( opt.mText3 == "" );

    // add timer
    opt = BubbleStyleOption();
    header.setTimerCost( "00:00" );
    BubbleUtils::setCallHeaderTexts2Lines(header, opt);
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "00:00" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );
}

void ut_BubbleUtils::test_setCallHeaderTexts2Lines_held()
{
    // held
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::OnHold );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "on hold", Qt::ElideRight );
    header.setTimerCost( "00:00" );

    BubbleUtils::setCallHeaderTexts2Lines(header, opt);

    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "on hold" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );
}

void ut_BubbleUtils::test_setCallHeaderTexts2Lines_disconnected()
{
    // disconnected call
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState( BubbleManager::Disconnected );
    header.setCli( "john", Qt::ElideRight );
    header.setText( "discon", Qt::ElideRight );
    header.setTimerCost( "00:00" );

    BubbleUtils::setCallHeaderTexts2Lines(header, opt);
    QVERIFY( opt.mText1 == "john" );
    QVERIFY( opt.mText1Clip == Qt::ElideRight );
    QVERIFY( opt.mText2 == "discon" );
    QVERIFY( opt.mText2Clip == Qt::ElideRight );
    QVERIFY( opt.mText3 == "" );
}

void ut_BubbleUtils::test_setCallHeaderTexts2Lines_outgoing()
{
    // outgoing
    BubbleHeader header;
    BubbleStyleOption opt;

    header.setCallState(BubbleManager::Outgoing);
    header.setCli("john", Qt::ElideRight);
    header.setText("calling", Qt::ElideRight);
    header.setTimerCost("00:00");

    BubbleUtils::setCallHeaderTexts2Lines(header, opt);
    QCOMPARE(opt.mText1, QString("calling"));
    QCOMPARE(opt.mText1Clip, Qt::ElideRight);
    QCOMPARE(opt.mText2, QString("john"));
    QCOMPARE(opt.mText2Clip, Qt::ElideRight);
}

void ut_BubbleUtils::test_compareHeaders()
{
    BubbleHeader header1;
    BubbleHeader header2;

    header1.setCallState(BubbleManagerIF::Active);
    header2.setCallState(BubbleManagerIF::Waiting);
    QVERIFY( BubbleUtils::compareHeaders(&header1,&header2) == false );

    header1.setCallState(BubbleManagerIF::Active);
    header2.setCallState(BubbleManagerIF::OnHold);
    QVERIFY( BubbleUtils::compareHeaders(&header1,&header2) == true );
}

void ut_BubbleUtils::test_stylePluginNameWithPath()
{
#if defined(Q_OS_SYMBIAN)
    QString name(BubbleUtils::stylePluginNameWithPath("bubblestyleplugin.dll"));
    QVERIFY(name=="z:/resource/qt/plugins/phone/bubblestyleplugin.dll");
#elif defined(Q_OS_WIN)
    QString name(BubbleUtils::stylePluginNameWithPath("bubblestyleplugin.dll"));
    QVERIFY(name=="c:/hb/bin/bubblestyleplugin.dll");
#else // Q_OS_UNIX
    QString name(BubbleUtils::stylePluginNameWithPath("bubblestyleplugin.dll"));
    QVERIFY(name=="/home/lib/bubblestyleplugin.dll");
#endif
}

void ut_BubbleUtils::test_setButtonStyleForAction()
{
    HbPushButton button;
    BubbleButtonStyleMock style;
    button.setStyle(&style);
    HbAction action;

    action.setSoftKeyRole(QAction::NoSoftKey);
    BubbleUtils::setButtonStyleForAction(button,action);
    QVERIFY(style.color()==BubbleButtonStyle::Default);

    action.setSoftKeyRole(QAction::PositiveSoftKey);
    BubbleUtils::setButtonStyleForAction(button,action);
    QVERIFY(style.color()==BubbleButtonStyle::Green);

    action.setSoftKeyRole(QAction::NegativeSoftKey);
    BubbleUtils::setButtonStyleForAction(button,action);
    QVERIFY(style.color()==BubbleButtonStyle::Red);
}

BUBBLE_TEST_MAIN(ut_BubbleUtils)
#include "ut_bubbleutils.moc"
