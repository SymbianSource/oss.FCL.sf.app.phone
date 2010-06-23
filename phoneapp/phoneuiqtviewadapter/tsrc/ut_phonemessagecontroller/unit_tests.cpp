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
* Description:  Unit tests for PhoneNoteController.
*
*/

#include <QtTest/QtTest>
#include <QtGui>
#include <hbapplication.h>
#include <QSignalSpy>
#include "phoneconstants.h"
#include "cphonecenrepproxy.h"
#include "cphonepubsubproxy.h"
#include <settingsinternalcrkeys.h>
#include "phonemessagecontroller.h"
#include "tphonecmdparamsfidata.h"

QString mService;
QString mMessage;
bool mSend;
QList<QVariant> mList;


#define PHONE_QT_MESSAGE_CONTROLLER_TEST_MAIN(TestObject) \
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

class TestPhoneMessageController : public QObject
{
    Q_OBJECT
public:
    TestPhoneMessageController();
    virtual ~TestPhoneMessageController();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup(); 
    
private slots:
    void testOpenSoftRejectEditor();


private:
    QString softRejectText();
    
private:
    PhoneMessageController *m_messageController; // class under test
};

TestPhoneMessageController::TestPhoneMessageController()
{
}

TestPhoneMessageController::~TestPhoneMessageController()
{
}

void TestPhoneMessageController::initTestCase()
{
    m_messageController = new PhoneMessageController();
}

void TestPhoneMessageController::cleanupTestCase()
{
    delete m_messageController;
}

void TestPhoneMessageController::init()
{
}

void TestPhoneMessageController::cleanup()
{
}

void TestPhoneMessageController::testOpenSoftRejectEditor()
{
    QString text = softRejectText();
    TPhoneCmdParamSfiData sfiParam;
    sfiParam.SetNumber(_L("1234567"));
    sfiParam.SetName(_L("Tester"));
    
    m_messageController->openSoftRejectMessageEditor(&sfiParam);
    
    QVERIFY( mService == "com.nokia.services.hbserviceprovider.conversationview" );
    QVERIFY( mMessage == "send(QString,QString,QString)" );
    QVERIFY( mList.contains("1234567") );
    QVERIFY( mList.contains("Tester") );
    QVERIFY( mList.contains(text) );
    QVERIFY( mSend );
}

QString TestPhoneMessageController::softRejectText()
{
    QString messageBody;
    // Get message body
    TInt softRejectDefaultInUseValue = 0;
    const TInt err = CPhoneCenRepProxy::Instance()->GetInt(
        KCRUidTelephonySettings,
        KSettingsSoftRejectDefaultInUse,
        softRejectDefaultInUseValue );
    
    if (softRejectDefaultInUseValue) {
        HBufC* softRejectTxt = NULL;
        TRAP_IGNORE( softRejectTxt = HBufC::NewL( KPhoneSMSLength ) );
        
        if (softRejectTxt) {
            TPtr string( softRejectTxt->Des() );
        
            // Default txt used or not
            CPhoneCenRepProxy::Instance()->GetString(
                KCRUidTelephonySettings,
                KSettingsSoftRejectText,
                string );
            
            messageBody = QString::fromUtf16(string.Ptr(), string.Length());
            delete softRejectTxt;
        }
    } else {
        messageBody = hbTrId("txt_phone_setlabel_soft_reject_val_default_text");
    }
    
    return messageBody;
}

PHONE_QT_MESSAGE_CONTROLLER_TEST_MAIN(TestPhoneMessageController)
#include "unit_tests.moc"
