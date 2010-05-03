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
* Description:  Unit tests for PhoneResourceAdapter.
*
*/

#include <QtTest/QtTest>
#include <QtGui>
#include <hbapplication.h>
#include <QSignalSpy>
#include <QVariant>
#include <hbaction.h>
//#include <hbglobal_p.h>
#include <phoneappvoipcommands.hrh>
#include "phoneresourceadapter.h"
#include "phoneresourceids.h"
#include "phoneui.hrh"

#define PHONE_QT_RESOURCE_ADAPTER_TEST_MAIN(TestObject) \
int main(int argc, char *argv[]) \
{ \
    HbApplication app(argc, argv); \
    TestObject tc; \
    QResource::registerResource("../hbcore.rcc"); \
    int ret = QTest::qExec(&tc, argc, argv); \
    /* Core dump if HbIconLoader instance is not destroyed before the application instance. */ \
    /* HbIconLoader uses QCoreApplication::aboutToQuit() signal to destroy itself. */ \
    /* app.exec() where the signal is normally emitted is not called here. */ \
    /* So, invoking the signal explicitly. */ \
    QMetaObject::invokeMethod(&app, "aboutToQuit", Qt::DirectConnection); \
    return ret; \
}

class TestPhoneResourceAdapter : public QObject
{
    Q_OBJECT
public:
    TestPhoneResourceAdapter();
    virtual ~TestPhoneResourceAdapter();

public slots:
    void initTestCase ();
    void cleanupTestCase ();
    void init ();
    void cleanup (); 
    
private slots:
    void testConvert ();
    void testConvertToString ();
    void testConvertToString2 ();
    void testDefaultToolbarResourceId();
    void testConvertCommandToString();
    void testConvertToHbActions();

private:
    void testIncallToolbar (int id); // helper function
    
private:
    PhoneResourceAdapter *m_resourceAdapter; // class under test
};

TestPhoneResourceAdapter::TestPhoneResourceAdapter ()
{
}

TestPhoneResourceAdapter::~TestPhoneResourceAdapter ()
{
}

void TestPhoneResourceAdapter::initTestCase ()
{
    m_resourceAdapter = PhoneResourceAdapter::Instance (this);
}

void TestPhoneResourceAdapter::cleanupTestCase ()
{
    delete m_resourceAdapter;
}

void TestPhoneResourceAdapter::init ()
{
}

void TestPhoneResourceAdapter::cleanup ()
{
}

void TestPhoneResourceAdapter::testConvert ()
{
    // For coverage
    m_resourceAdapter = PhoneResourceAdapter::Instance (this);
    PhoneUIQtButtonsController* buttonsController = 
        m_resourceAdapter->buttonsController ();
    

    QMap<PhoneAction::ActionType, PhoneAction *> map;
    PhoneAction *action = map [PhoneAction::LeftButton];

    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_INCOMINGCALL_CBA);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::ToolbarButton1];
    QCOMPARE (action->icon (), HbIcon("qtg_mono_send"));    
    action = map [PhoneAction::ToolbarButton2];
    QCOMPARE (action->icon (), HbIcon("qtg_mono_speaker_off"));
    QCOMPARE (action->command (), (int)EPhoneCallComingCmdSilent);    
    
    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_INCOMINGCALL_REJECT_CBA);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::ToolbarButton1];
    QCOMPARE (action->icon (), HbIcon("qtg_mono_send"));    
    action = map [PhoneAction::ToolbarButton2];
    QCOMPARE (action->icon (), HbIcon("qtg_mono_reject_call"));
    //Command is disabled
    //QCOMPARE (action->command (), (int)EPhoneCallComingCmdReject);    
     
    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_INCOMINGCALL_SOFT_REJECT_CBA);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::ToolbarButton1];
    QCOMPARE (action->icon (), HbIcon("qtg_mono_send"));    
    action = map [PhoneAction::ToolbarButton2];
    QCOMPARE (action->icon (), HbIcon("qtg_mono_speaker_off"));
    //Command is disabled
    //QCOMPARE (action->command (), (int)EPhoneCallComingCmdReject); 

    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_CALLWAITING_CBA);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::ToolbarButton1];
    QCOMPARE (action->icon (), HbIcon("qtg_mono_send"));    
    action = map [PhoneAction::ToolbarButton2];
    QCOMPARE (action->icon (), HbIcon("qtg_mono_join_call"));
    QCOMPARE (action->command (), (int)EPhoneInCallCmdJoin);    
    
    testIncallToolbar( R_PHONEUI_DIALER_CBA );
    testIncallToolbar( R_PHONEUI_INCALL_DIALER_CBA );
    testIncallToolbar( R_PHONEUI_CALLHANDLING_CALLSETUP_EMPTY_DTMFDIALER_CBA );
    testIncallToolbar( R_PHONEUI_CALLHANDLING_INCALL_UNHOLD_CBA );
    testIncallToolbar( R_PHONEUI_CALLHANDLING_INCALL_UNMUTE_CBA );
    
    map = m_resourceAdapter->convert (R_PHONEUI_MTCAL_INCOMING_CALL);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );

    map = m_resourceAdapter->convert (R_PHONEUI_INCALL_CALL_DISCONNECTED);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );
    
    map = m_resourceAdapter->convert (R_PHONEUI_INCALL_CALL_HELD);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );

    map = m_resourceAdapter->convert (R_PHONEUI_MULTC_WAITING);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );

    map = m_resourceAdapter->convert (R_PHONEUI_INCALL_CALL_NUMBER, 3);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );
    
    map = m_resourceAdapter->convert (R_PHONEUI_TIME_DURAT_LONG_WITH_ZERO);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );
    
    map = m_resourceAdapter->convert (R_PHONEUI_OUT_GOING_CALL);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );

    map = m_resourceAdapter->convert (R_PHONEUI_MTCAL_CLI_WITHHELD);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );
    
    map = m_resourceAdapter->convert (R_PHONEUI_MTCAL_CALL);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );
    
    map = m_resourceAdapter->convert (R_PHONEUI_EMERGENCY_CALL_HEADER);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );

    map = m_resourceAdapter->convert (R_PHONEUI_ATTEMPTING_EMERGENCY_CALL_TEXT);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );
    
    map = m_resourceAdapter->convert (R_PHONEUI_EMERGENCY_CALL_BUTTONS);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::LeftButton];
    QVERIFY( false == action->text().isEmpty() );
    QCOMPARE (action->icon (), HbIcon("qtg_mono_end_call"));
    QCOMPARE (action->command (), (int)EPhoneCmdEnd);
    
    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_EMERGENCY_CBA);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::ToolbarButton1];
    QCOMPARE (action->icon (), HbIcon("qtg_mono_dialer"));
    action = map [PhoneAction::ToolbarButton2];
    QCOMPARE (action->icon (), HbIcon("qtg_mono_speaker"));
    QCOMPARE (action->command (), (int)EPhoneInCallCmdActivateIhf);        
    
    map = m_resourceAdapter->convert (R_PHONEUI_CONFERENCE_CALL);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );
    
    TBuf<10> buffer(_L("123"));
    map = m_resourceAdapter->convert (R_PHONEUI_SENDING_DTMF_WAIT_NOTE_TEXT, &buffer);
    QCOMPARE (map.size(), 1);
    action = map[PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );
    
    map = m_resourceAdapter->convert(R_PHONEUI_DTMF_WAIT_CHARACTER_CONFIRMATION_QUERY_TEXT, &buffer);
    QCOMPARE(map.size(), 1);
    action = map[PhoneAction::Text];
    QVERIFY( false == action->text().isEmpty() );
    
    // TODO
    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_INCALL_HANDSET_CBA);
    
    map = m_resourceAdapter->convert (0);
    QCOMPARE (map.size (), 0);
}

void TestPhoneResourceAdapter::testConvertToString ()
{
    m_resourceAdapter = PhoneResourceAdapter::Instance (this);
        
    QString testString = m_resourceAdapter->convertToString(R_PHONE_ERROR_CALL_NOT_ALLOWED); 
    QVERIFY( false == testString.isEmpty() );
    
    QString testString2 = m_resourceAdapter->convertToString(R_ERROR_NOTE_NOT_ALLOWED); 
    QVERIFY( false == testString2.isEmpty() );
    
    QString testString3 = m_resourceAdapter->convertToString(R_NOTETEXT_INVALID_PHONENUMBER); 
    QVERIFY( false == testString3.isEmpty() );
    
    QString testString4 = m_resourceAdapter->convertToString(R_NOTETEXT_NO_ANSWER); 
    QVERIFY( false == testString4.isEmpty() );
    
    QString testString5 = m_resourceAdapter->convertToString(R_PHONE_ERROR_NUMBER_BUSY); 
    QVERIFY( false == testString5.isEmpty() );
    
    QString testString6 = m_resourceAdapter->convertToString(R_PHONE_SS_NOTIFICATION_MOCAL_WAITING_TEXT); 
    QVERIFY( false == testString6.isEmpty() );
    
    QString testString7 = m_resourceAdapter->convertToString(R_PHONE_SS_NOTIFICATION_INCAL_INFO_HOLD_TEXT); 
    QVERIFY( false == testString7.isEmpty() );
    
    QString testString8 = m_resourceAdapter->convertToString(R_INCAL_INFO_HOLD_TEXT); 
    QVERIFY( false == testString8.isEmpty() );
    
    QString testString9 = m_resourceAdapter->convertToString(R_INCAL_REMOTE_ACTIVE_TEXT); 
    QVERIFY( false == testString9.isEmpty() );
    
    QString testString10 = m_resourceAdapter->convertToString(R_INCAL_INFO_CONF_HOLD_TEXT); 
    QVERIFY( false == testString10.isEmpty() );
    
    QString testString11 = m_resourceAdapter->convertToString(R_PHONE_INCALL_INFO_NO_NETWORK_SUPPORT); 
    QVERIFY( false == testString11.isEmpty() );
    
    QString testString12 = m_resourceAdapter->convertToString(R_PHONE_ERROR_EMERGENCY_CALLS_ONLY); 
    QVERIFY( false == testString12.isEmpty() );
    
    QString testString13 = m_resourceAdapter->convertToString(R_NOTETEXT_NUMBER_NOT_IN_USE); 
    QVERIFY( false == testString13.isEmpty() );
    
    QString testString14 = m_resourceAdapter->convertToString(R_NOTETEXT_NETWORK_BUSY); 
    QVERIFY( false == testString14.isEmpty() );
    
    QString testString15 = m_resourceAdapter->convertToString(R_NOTETEXT_ERROR_IN_CONNECTION); 
    QVERIFY( false == testString15.isEmpty() );
    
    QString testString16 = m_resourceAdapter->convertToString(R_PHONEUI_ERROR_EMERGENCY_ATTEMPT_FAILED); 
    QVERIFY( false == testString16.isEmpty() );   
    
    QString testString17 = m_resourceAdapter->convertToString(R_PHONE_SS_NOTIFICATION_INCAL_TRANSFERRED_TEXT); 
    QVERIFY( false == testString17.isEmpty() );    
 
    QString testString18 = m_resourceAdapter->convertToString(R_PHONE_ERROR_NO_SERVICE); 
    QVERIFY( false == testString18.isEmpty() ); 
    
    QString testString99 = m_resourceAdapter->convertToString(0); 
    QCOMPARE( testString99, QString ("") );
}

void TestPhoneResourceAdapter::testConvertToString2 ()
{
    m_resourceAdapter = PhoneResourceAdapter::Instance (this);
        
    QString testString = m_resourceAdapter->convertToString(
            R_PHONE_TEXT_COLP_CONNECTED, QString("123456"));
    
    QVERIFY( false == testString.isEmpty() );
       
    QString testString2 = m_resourceAdapter->convertToString(0, QString()); 
    QCOMPARE( testString2, QString ("") );
}

void TestPhoneResourceAdapter::testIncallToolbar (int id)
{
    QMap<PhoneAction::ActionType, PhoneAction *> map;
    PhoneAction *action;
    map = m_resourceAdapter->convert (id);
    QCOMPARE (map.size (), 4);
    
    switch(id) {
    case R_PHONEUI_CALLHANDLING_INCALL_UNMUTE_CBA:
        action = map [PhoneAction::ToolbarButton1];
        QCOMPARE (action->icon (), HbIcon("qtg_mono_dialer"));
        action = map [PhoneAction::ToolbarButton2];
        QCOMPARE (action->icon (), HbIcon("qtg_mono_mic_unmute"));
        QCOMPARE (action->command (), (int)EPhoneInCallCmdUnmute);    
        action = map [PhoneAction::ToolbarButton3];
        QCOMPARE (action->icon (), HbIcon("qtg_mono_speaker"));
        QCOMPARE (action->command (), (int)EPhoneInCallCmdActivateIhf);        
        break;
    default:    
        action = map [PhoneAction::ToolbarButton1];
        QCOMPARE (action->icon (), HbIcon("qtg_mono_dialer"));
        action = map [PhoneAction::ToolbarButton2];
        QCOMPARE (action->icon (), HbIcon("qtg_mono_mic_mute"));
        QCOMPARE (action->command (), (int)EPhoneInCallCmdMute);    
        action = map [PhoneAction::ToolbarButton3];
        QCOMPARE (action->icon (), HbIcon("qtg_mono_speaker"));
        QCOMPARE (action->command (), (int)EPhoneInCallCmdActivateIhf);
        break;
    }
}

void TestPhoneResourceAdapter::testDefaultToolbarResourceId()
{
    QVERIFY(m_resourceAdapter->defaultToolbarResourceId()==
            R_PHONEUI_CALLHANDLING_INCOMINGCALL_CBA);
}

void TestPhoneResourceAdapter::testConvertCommandToString ()
{
    m_resourceAdapter = PhoneResourceAdapter::Instance (this);
    
    QString testString = m_resourceAdapter->convertCommandToString(
            EPhoneInCallCmdEndAllCalls);    
    QVERIFY( false == testString.isEmpty() );
    
    testString = m_resourceAdapter->convertCommandToString(
            EPhoneInCallCmdTransfer);    
    QVERIFY( false == testString.isEmpty() );
    
    testString = m_resourceAdapter->convertCommandToString(
            EPhoneInCallCmdSwitchToVideo);    
    QVERIFY( false == testString.isEmpty() );
    
    testString = m_resourceAdapter->convertCommandToString(
            EPhoneCmdHandoverToWlan);    
    QVERIFY( false == testString.isEmpty() );
    
    testString = m_resourceAdapter->convertCommandToString(
            EPhoneCmdHandoverToGsm);    
    QVERIFY( false == testString.isEmpty() );
    
    testString = m_resourceAdapter->convertCommandToString(0); 
    QCOMPARE( testString, QString ("") );
}

void TestPhoneResourceAdapter::testConvertToHbActions()
{
    m_resourceAdapter = PhoneResourceAdapter::Instance (this);
    
    QList<HbAction*> testList = m_resourceAdapter->convertToHbActions(R_PHONEUI_DTMF_WAIT_CHARACTER_CONFIRMATION_QUERY);    
    QCOMPARE( testList.count(), 2 );
    QCOMPARE( (int)EPhoneInCallCmdContinueSendingDtmfString, 
              testList.at(0)->data().toInt());
    QCOMPARE( (int)EPhoneInCallCmdCancelSendingDtmfString, 
              testList.at(1)->data().toInt());
    qDeleteAll(testList);
    testList.clear();
    
    testList = m_resourceAdapter->convertToHbActions(R_PHONEUI_SENDING_DTMF_WAIT_NOTE);    
    QCOMPARE( testList.count(), 1 );
    QCOMPARE( (int)EPhoneInCallCmdCancelSendingDtmfString, 
              testList.at(0)->data().toInt());
    qDeleteAll(testList);
    testList.clear();
    
    testList = m_resourceAdapter->convertToHbActions(0);    
    QCOMPARE( testList.count(), 0 );
}

PHONE_QT_RESOURCE_ADAPTER_TEST_MAIN(TestPhoneResourceAdapter)
#include "unit_tests.moc"
