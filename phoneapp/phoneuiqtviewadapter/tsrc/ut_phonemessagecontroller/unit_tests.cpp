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
//#include <hbglobal_p.h>
#include "phonemessagecontroller.h"
#include "tphonecmdparamsfidata.h"

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
    TPhoneCmdParamSfiData sfiParam;
    sfiParam.SetNumber(_L("1234567"));
    sfiParam.SetName(_L("Tester"));
    
    m_messageController->openSoftRejectMessageEditor(&sfiParam);
}

PHONE_QT_MESSAGE_CONTROLLER_TEST_MAIN(TestPhoneMessageController)
#include "unit_tests.moc"
