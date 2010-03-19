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
    map = m_resourceAdapter->convert (R_PHONEUI_INCOMING_CALL_BUTTONS);
    QCOMPARE (map.size (), 2);
    PhoneAction *action = map [PhoneAction::LeftButton];
    QCOMPARE (action->text (), QString ("Answer"));
    QCOMPARE (action->icon (), HbIcon(":/pri_large_button_answer.svg"));
    QCOMPARE (action->command (), (int)EPhoneCallComingCmdAnswer);
    action = map [PhoneAction::RightButton];
    QCOMPARE (action->text (), QString ("Reject"));
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_button_reject.svg"));
    QCOMPARE (action->command (), (int)EPhoneCallComingCmdReject);
    
    map = m_resourceAdapter->convert (R_PHONEUI_INCALL_BUTTONS);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::LeftButton];
    QCOMPARE (action->text (), QString ("Hold"));
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_button_hold.svg"));
    QCOMPARE (action->command (), (int)EPhoneInCallCmdHold);
    action = map [PhoneAction::RightButton];
    QCOMPARE (action->text (), QString ("End call"));
    QCOMPARE (action->icon (), HbIcon(":/pri_large_button_endcall.svg"));
    QCOMPARE (action->command (), (int)EPhoneInCallCmdEndThisActiveCall);
    
    map = m_resourceAdapter->convert (R_PHONEUI_INCALL_HELD_BUTTONS);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::LeftButton];
    QCOMPARE (action->text (), QString ("Unhold"));
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_button_onhold.svg"));
    QCOMPARE (action->command (), (int)EPhoneInCallCmdUnhold);
    action = map [PhoneAction::RightButton];
    QCOMPARE (action->text (), QString ("End call"));
    QCOMPARE (action->icon (), HbIcon(":/pri_large_button_endcall.svg"));
    QCOMPARE (action->command (), (int)EPhoneInCallCmdEndThisActiveCall);
    
    map = m_resourceAdapter->convert (R_PHONEUI_WAITING_CALL_BUTTONS);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::LeftButton];
    QCOMPARE (action->text (), QString ("Answer"));
    QCOMPARE (action->icon (), HbIcon(":/pri_large_button_answer.svg"));
    QCOMPARE (action->command (), (int)EPhoneCallComingCmdAnswer);
    action = map [PhoneAction::RightButton];
    QCOMPARE (action->text (), QString ("Reject"));
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_button_reject.svg"));
    QCOMPARE (action->command (), (int)EPhoneCallComingCmdReject);

    map = m_resourceAdapter->convert (R_PHONEUI_TWO_SINGLES_BUTTONS);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::LeftButton];
    QCOMPARE (action->text (), QString ("Swap"));
    QCOMPARE (action->icon (), HbIcon(":/qgn_indi_button_swap.svg"));
    QCOMPARE (action->command (), (int)EPhoneInCallCmdSwap);
    action = map [PhoneAction::RightButton];
    QCOMPARE (action->text (), QString ("End call"));
    QCOMPARE (action->icon (), HbIcon(":/pri_large_button_endcall.svg"));
    QCOMPARE (action->command (), (int)EPhoneInCallCmdEndThisActiveCall);    
    
    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_INCOMINGCALL_CBA);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::ToolbarButton1];
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_comp_message.svg"));    
    action = map [PhoneAction::ToolbarButton2];
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_silence.svg"));
    QCOMPARE (action->command (), (int)EPhoneCallComingCmdSilent);    
    
    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_INCOMINGCALL_REJECT_CBA);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::ToolbarButton1];
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_comp_message.svg"));    
    action = map [PhoneAction::ToolbarButton2];
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_silence.svg"));
    //Command is disabled
    //QCOMPARE (action->command (), (int)EPhoneCallComingCmdReject);    
     
    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_INCOMINGCALL_SOFT_REJECT_CBA);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::ToolbarButton1];
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_comp_message.svg"));    
    action = map [PhoneAction::ToolbarButton2];
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_silence.svg"));
    //Command is disabled
    //QCOMPARE (action->command (), (int)EPhoneCallComingCmdReject); 

    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_CALLWAITING_CBA);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::ToolbarButton1];
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_comp_message.svg"));    
    action = map [PhoneAction::ToolbarButton2];
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_join.svg"));
    QCOMPARE (action->command (), (int)EPhoneInCallCmdJoin);    
    
    testIncallToolbar( R_PHONEUI_DIALER_CBA );
    testIncallToolbar( R_PHONEUI_INCALL_DIALER_CBA );
    testIncallToolbar( R_PHONEUI_CALLHANDLING_CALLSETUP_EMPTY_DTMFDIALER_CBA );
    testIncallToolbar( R_PHONEUI_CALLHANDLING_INCALL_UNHOLD_CBA );
    testIncallToolbar( R_PHONEUI_CALLHANDLING_INCALL_UNMUTE_CBA );
    
    map = m_resourceAdapter->convert (R_PHONEUI_MTCAL_INCOMING_CALL);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("calling"));

    map = m_resourceAdapter->convert (R_PHONEUI_INCALL_CALL_DISCONNECTED);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("disconnected"));
    
    map = m_resourceAdapter->convert (R_PHONEUI_INCALL_CALL_HELD);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("on hold"));

    map = m_resourceAdapter->convert (R_PHONEUI_MULTC_WAITING);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("waiting"));

    map = m_resourceAdapter->convert (R_PHONEUI_INCALL_CALL_NUMBER, 3);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("Call 3"));

    map = m_resourceAdapter->convert (R_PHONEUI_CALL_SETUP_BUTTONS);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::LeftButton];
    QCOMPARE (action->text (), QString ("End call"));
    QCOMPARE (action->icon (), HbIcon(":/pri_large_button_endcall.svg"));
    QCOMPARE (action->command (), (int)EPhoneInCallCmdEndThisOutgoingCall);
    
    map = m_resourceAdapter->convert (R_PHONEUI_TIME_DURAT_LONG_WITH_ZERO);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("%:0%H%:1%T%:2%S%:3"));
    
    map = m_resourceAdapter->convert (R_PHONEUI_OUT_GOING_CALL);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("Calling"));

    map = m_resourceAdapter->convert (R_PHONEUI_MTCAL_CLI_WITHHELD);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("Private number"));
    
    map = m_resourceAdapter->convert (R_PHONEUI_MTCAL_CALL);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("Call"));
    
    map = m_resourceAdapter->convert (R_PHONEUI_EMERGENCY_CALL_HEADER);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("Emergency call"));

    map = m_resourceAdapter->convert (R_PHONEUI_ATTEMPTING_EMERGENCY_CALL_TEXT);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("Attempting"));
    
    map = m_resourceAdapter->convert (R_PHONEUI_EMERGENCY_CALL_BUTTONS);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::LeftButton];
    QCOMPARE (action->text (), QString ("End call"));
    QCOMPARE (action->icon (), HbIcon(":/pri_large_button_endcall.svg"));
    QCOMPARE (action->command (), (int)EPhoneCmdEnd);
    
    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_EMERGENCY_CBA);
    QCOMPARE (map.size (), 2);
    action = map [PhoneAction::ToolbarButton1];
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_dialler.svg"));
    action = map [PhoneAction::ToolbarButton2];
    QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_loudsp_unmute.svg"));
    QCOMPARE (action->command (), (int)EPhoneInCallCmdActivateIhf);        
    
    map = m_resourceAdapter->convert (R_PHONEUI_CONFERENCE_CALL);
    QCOMPARE (map.size (), 1);
    action = map [PhoneAction::Text];
    QCOMPARE (action->text (), QString ("Conference"));
    
    TBuf<10> buffer(_L("123"));
    map = m_resourceAdapter->convert (R_PHONEUI_SENDING_DTMF_WAIT_NOTE_TEXT, &buffer);
    QCOMPARE (map.size(), 1);
    action = map[PhoneAction::Text];
    QCOMPARE (action->text(), QString("Sending:\n123"));
    
    map = m_resourceAdapter->convert(R_PHONEUI_DTMF_WAIT_CHARACTER_CONFIRMATION_QUERY_TEXT, &buffer);
    QCOMPARE(map.size(), 1);
    action = map[PhoneAction::Text];
    QCOMPARE(action->text(), QString("Send string:\n123"));
    
    // TODO
    map = m_resourceAdapter->convert (R_PHONEUI_CALLHANDLING_INCALL_HANDSET_CBA);
    
    map = m_resourceAdapter->convert (0);
    QCOMPARE (map.size (), 0);
}

void TestPhoneResourceAdapter::testConvertToString ()
{
    m_resourceAdapter = PhoneResourceAdapter::Instance (this);
        
    QString testString = m_resourceAdapter->convertToString(R_PHONE_ERROR_CALL_NOT_ALLOWED); 
    QCOMPARE( testString, QString ("Call not allowed") );
    
    QString testString2 = m_resourceAdapter->convertToString(R_ERROR_NOTE_NOT_ALLOWED); 
    QCOMPARE( testString2, QString ("Not allowed") );
    
    QString testString3 = m_resourceAdapter->convertToString(R_NOTETEXT_INVALID_PHONENUMBER); 
    QCOMPARE( testString3, QString ("Invalid phone number") );
    
    QString testString4 = m_resourceAdapter->convertToString(R_NOTETEXT_NO_ANSWER); 
    QCOMPARE( testString4, QString ("No answer") );
    
    QString testString5 = m_resourceAdapter->convertToString(R_PHONE_ERROR_NUMBER_BUSY); 
    QCOMPARE( testString5, QString ("Number busy") );
    
    QString testString6 = m_resourceAdapter->convertToString(R_PHONE_SS_NOTIFICATION_MOCAL_WAITING_TEXT); 
    QCOMPARE( testString6, QString ("Waiting") );
    
    QString testString7 = m_resourceAdapter->convertToString(R_PHONE_SS_NOTIFICATION_INCAL_INFO_HOLD_TEXT); 
    QCOMPARE( testString7, QString ("On hold") );
    
    QString testString8 = m_resourceAdapter->convertToString(R_INCAL_INFO_HOLD_TEXT); 
    QCOMPARE( testString8, QString ("On hold") );
    
    QString testString9 = m_resourceAdapter->convertToString(R_INCAL_REMOTE_ACTIVE_TEXT); 
    QCOMPARE( testString9, QString ("Connected") );
    
    QString testString10 = m_resourceAdapter->convertToString(R_INCAL_INFO_CONF_HOLD_TEXT); 
    QCOMPARE( testString10, QString ("Conference on hold") );
    
    QString testString11 = m_resourceAdapter->convertToString(R_PHONE_INCALL_INFO_NO_NETWORK_SUPPORT); 
    QCOMPARE( testString11, QString ("No network support for video call") );
    
    QString testString12 = m_resourceAdapter->convertToString(0); 
    QCOMPARE( testString12, QString ("") );
}

void TestPhoneResourceAdapter::testConvertToString2 ()
{
    m_resourceAdapter = PhoneResourceAdapter::Instance (this);
        
    QString testString = m_resourceAdapter->convertToString(
            R_PHONE_TEXT_COLP_CONNECTED, QString("123456"));
    
    QCOMPARE( testString, QString ("Connected to 123456") );
       
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
        QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_dialler.svg"));
        action = map [PhoneAction::ToolbarButton2];
        QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_unmute.svg"));
        QCOMPARE (action->command (), (int)EPhoneInCallCmdUnmute);    
        action = map [PhoneAction::ToolbarButton3];
        QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_loudsp_unmute.svg"));
        QCOMPARE (action->command (), (int)EPhoneInCallCmdActivateIhf);        
        break;
    default:    
        action = map [PhoneAction::ToolbarButton1];
        QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_dialler.svg"));
        action = map [PhoneAction::ToolbarButton2];
        QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_mute.svg"));
        QCOMPARE (action->command (), (int)EPhoneInCallCmdMute);    
        action = map [PhoneAction::ToolbarButton3];
        QCOMPARE (action->icon (), HbIcon(":/qtg_large_tb_loudsp_unmute.svg"));
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
    QCOMPARE( testString, QString ("End all calls") );
    
    testString = m_resourceAdapter->convertCommandToString(
            EPhoneInCallCmdTransfer);    
    QCOMPARE( testString, QString ("Transfer") );
    
    testString = m_resourceAdapter->convertCommandToString(
            EPhoneInCallCmdSwitchToVideo);    
    QCOMPARE( testString, QString ("Call back with video call") );
    
    testString = m_resourceAdapter->convertCommandToString(
            EPhoneCmdHandoverToWlan);    
    QCOMPARE( testString, QString ("Handover to WLAN") );
    
    testString = m_resourceAdapter->convertCommandToString(
            EPhoneCmdHandoverToGsm);    
    QCOMPARE( testString, QString ("Handover to GSM") );
    
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
