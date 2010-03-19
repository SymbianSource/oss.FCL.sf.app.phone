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
* Description:  Unit tests for DialService.
*
*/

#include <QtTest/QtTest>

//#include <hbglobal_p.h>
#include "dialservice.h"

class TestDialService : public QObject, public MPECallControlIF, public MPECallSettersIF
{
    Q_OBJECT
public:
    TestDialService();
    virtual ~TestDialService();    

public slots:
    void initTestCase ();
    void cleanupTestCase ();
    void init ();
    void cleanup ();
    
public:
    void SetKeyCode( const TChar& aKeyCode );
    void SetPhoneNumber( const TPEPhoneNumber& aPhoneNumber );
    void SetCallTypeCommand( const TPECallType& aCallType );
    TInt HandleDialServiceCall( const TBool aClientCall = EFalse );
    void HandlePlayDTMFL();
    TInt HandleEndDTMF();
    void SetContactId2( const TInt aContactId );
    void SetServiceIdCommand( TUint32 aServiceId );
    
private slots:
    void testDial ();
    void testDial2 ();
    void testDialVideo ();
    void testDialVideo2 ();
    void testDialVoip();
    void testDialVoip2();
    void testDialVoipService();
    void testDialVoipService2();
//    void testExecuteKeySequence ();
//    void testPlayDTMFTone ();
//    void testStopDTMFPlay ();

private:
    DialService *m_DialService; // class under test
    bool m_setPhoneNumberCalled;
    bool m_setCallTypeCommandCalled;
    bool m_handleDialCallCalled;
    TPEPhoneNumber m_phoneNumber;
    TPECallType m_callType;
    bool m_clientCall;
    int m_contactId;
    bool m_setContactIdCalled;
    bool m_handleEndDTMFCalled;
    bool m_handlePlayDTMFLCalled;
    bool m_setKeyCodeCalled;
    bool m_setServiceIdCalled;
    int m_serviceId;
    ushort keyValue;
};

TestDialService::TestDialService ()
{
}

TestDialService::~TestDialService ()
{
}

void TestDialService::initTestCase ()
{
}

void TestDialService::cleanupTestCase ()
{
}

void TestDialService::init ()
{
    m_setPhoneNumberCalled = false;
    m_setCallTypeCommandCalled = false;
    m_handleDialCallCalled = false;
    m_clientCall = false;
    m_setContactIdCalled = false;
    m_contactId = -1;
    m_handleEndDTMFCalled = false;
    m_handlePlayDTMFLCalled = false;
    m_setKeyCodeCalled = false;
    keyValue = -1;
    m_DialService = new DialService (*this, *this, this);
}

void TestDialService::cleanup ()
{
    delete m_DialService;
}

void TestDialService::SetKeyCode( const TChar& aKeyCode )
{
    m_setKeyCodeCalled = true;
    keyValue = aKeyCode;
}

void TestDialService::SetPhoneNumber( const TPEPhoneNumber& aPhoneNumber )
{
    m_setPhoneNumberCalled = true;
    m_phoneNumber = aPhoneNumber;	
}

void TestDialService::SetCallTypeCommand( const TPECallType& aCallType )
{
    m_setCallTypeCommandCalled = true;
    m_callType = aCallType;	
}

void TestDialService::HandlePlayDTMFL()
{
    m_handlePlayDTMFLCalled = true;
}

TInt TestDialService::HandleEndDTMF()
{
    m_handleEndDTMFCalled = true;
    return 0;
}

void TestDialService::SetContactId2( const TInt aContactId )
{
    m_setContactIdCalled = true;
    m_contactId = aContactId;	
}

void TestDialService::SetServiceIdCommand( TUint32 aServiceId )
{
    m_setServiceIdCalled = true;
    m_serviceId = aServiceId;   
}

TInt TestDialService::HandleDialServiceCall( const TBool aClientCall )
{
    m_handleDialCallCalled = true;
    m_clientCall = aClientCall;
    return KErrNone;
}

void TestDialService::testDial()
{
    m_DialService->dial(QString("0501234567"));
    QString number((QChar*)m_phoneNumber.Ptr(), m_phoneNumber.Length());
    QVERIFY (m_setPhoneNumberCalled == true);
    QCOMPARE(number, QString("0501234567"));
    QVERIFY (m_setCallTypeCommandCalled == true);
    QCOMPARE((int)m_callType, (int)EPECallTypeCSVoice);
    QVERIFY (m_handleDialCallCalled == true);
    QVERIFY (m_clientCall == false);
}

void TestDialService::testDial2()
{
    m_DialService->dial(QString("0501234567"), 3127);
    QString number((QChar*)m_phoneNumber.Ptr(), m_phoneNumber.Length());
    QVERIFY (m_setPhoneNumberCalled == true);
    QCOMPARE(number, QString("0501234567"));
    QVERIFY (m_setCallTypeCommandCalled == true);
    QCOMPARE((int)m_callType, (int)EPECallTypeCSVoice);
    QVERIFY (m_setCallTypeCommandCalled == true);
    QVERIFY (m_setContactIdCalled == true);
    QCOMPARE (m_contactId, 3127);
    QVERIFY (m_clientCall == false);
}

void TestDialService::testDialVideo()
{
    m_DialService->dialVideo(QString("0501234567"));
    QString number((QChar*)m_phoneNumber.Ptr(), m_phoneNumber.Length());
    QVERIFY (m_setPhoneNumberCalled == true);
    QCOMPARE(number, QString("0501234567"));
    QVERIFY (m_setCallTypeCommandCalled == true);
    QCOMPARE((int)m_callType, (int)EPECallTypeVideo);
    QVERIFY (m_handleDialCallCalled == true);
    QVERIFY (m_clientCall == false);
}

void TestDialService::testDialVideo2()
{
    m_DialService->dialVideo(QString("+358501234567"), 12345);
    QString number((QChar*)m_phoneNumber.Ptr(), m_phoneNumber.Length());
    QVERIFY (m_setPhoneNumberCalled == true);
    QCOMPARE(number, QString("+358501234567"));
    QVERIFY (m_setCallTypeCommandCalled == true);
    QCOMPARE((int)m_callType, (int)EPECallTypeVideo);
    QVERIFY (m_setCallTypeCommandCalled == true);
    QVERIFY (m_setContactIdCalled == true);
    QCOMPARE (m_contactId, 12345);
    QVERIFY (m_clientCall == false);
}

void TestDialService::testDialVoip()
{
    m_DialService->dialVoip(QString("address@domain"));
    QString number((QChar*)m_phoneNumber.Ptr(), m_phoneNumber.Length());
    QVERIFY (m_setPhoneNumberCalled == true);
    QCOMPARE(number, QString("address@domain"));
    QVERIFY (m_setCallTypeCommandCalled == true);
    QCOMPARE((int)m_callType, (int)EPECallTypeVoIP);
}

void TestDialService::testDialVoip2()
{
    m_DialService->dialVoip(QString("address@domain"), 123);
    QString number((QChar*)m_phoneNumber.Ptr(), m_phoneNumber.Length());
    QVERIFY (m_setPhoneNumberCalled == true);
    QCOMPARE(number, QString("address@domain"));
    QVERIFY (m_setCallTypeCommandCalled == true);
    QCOMPARE((int)m_callType, (int)EPECallTypeVoIP);
    QVERIFY (m_setContactIdCalled == true);
    QCOMPARE (m_contactId, 123);
    QVERIFY (m_clientCall == false);
}

void TestDialService::testDialVoipService()
{
    m_DialService->dialVoipService(QString("address@domain"), 123);
    QString number((QChar*)m_phoneNumber.Ptr(), m_phoneNumber.Length());
    QVERIFY (m_setPhoneNumberCalled == true);
    QCOMPARE(number, QString("address@domain"));
    QVERIFY (m_setCallTypeCommandCalled == true);
    QCOMPARE((int)m_callType, (int)EPECallTypeVoIP);
    QVERIFY (m_setServiceIdCalled == true);
    QCOMPARE (m_serviceId, 123);
    QVERIFY (m_clientCall == false);
}

void TestDialService::testDialVoipService2()
{
    m_DialService->dialVoipService(QString("address@domain"), 123, 12345);
    QString number((QChar*)m_phoneNumber.Ptr(), m_phoneNumber.Length());
    QVERIFY (m_setPhoneNumberCalled == true);
    QCOMPARE(number, QString("address@domain"));
    QVERIFY (m_setCallTypeCommandCalled == true);
    QCOMPARE((int)m_callType, (int)EPECallTypeVoIP);
    QVERIFY (m_setServiceIdCalled == true);
    QCOMPARE (m_serviceId, 123);
    QVERIFY (m_setContactIdCalled == true);
    QCOMPARE (m_contactId, 12345);
    QVERIFY (m_clientCall == false);
}

/*
void TestDialService::testExecuteKeySequence()
{
    m_DialService->executeKeySequence(QString("*#0000#"));
    QString keySequence((QChar*)m_phoneNumber.Ptr(), m_phoneNumber.Length());
    QVERIFY (m_setPhoneNumberCalled == true);
    QCOMPARE (keySequence, QString("*#0000#"));
}

void TestDialService::testPlayDTMFTone()
{
    QChar six('6');
    m_DialService->playDTMFTone(six);
    QVERIFY (m_setKeyCodeCalled == true);
    QCOMPARE (keyValue, six.unicode());
    QVERIFY (m_handlePlayDTMFLCalled == true);
}

void TestDialService::testStopDTMFPlay()
{
    m_DialService->stopDTMFPlay();
    QVERIFY (m_handleEndDTMFCalled == true);
}
*/
QTEST_MAIN(TestDialService)
#include "unit_tests.moc"
