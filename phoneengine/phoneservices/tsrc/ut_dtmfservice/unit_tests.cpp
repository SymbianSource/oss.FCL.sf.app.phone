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
* Description:  Unit tests for DTMFService.
*
*/

#include <QtTest/QtTest>

//#include <hbglobal_p.h>
#include "dtmfservice.h"

class TestDTMFService : public QObject, public MPECallControlIF, public MPECallSettersIF
{
    Q_OBJECT
public:
    TestDTMFService();
    virtual ~TestDTMFService();    

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
    void testExecuteKeySequence ();
    void testPlayDTMFTone ();
    void testStopDTMFPlay ();

private:
    DTMFService *m_dtmfService; // class under test
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
    bool m_handlePlayDtmfLeave;
    ushort keyValue;
};

TestDTMFService::TestDTMFService ()
{
}

TestDTMFService::~TestDTMFService ()
{
}

void TestDTMFService::initTestCase ()
{
}

void TestDTMFService::cleanupTestCase ()
{
}

void TestDTMFService::init ()
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
    m_handlePlayDtmfLeave = false;
    keyValue = -1;
    m_dtmfService = new DTMFService (*this, *this, this);
}

void TestDTMFService::cleanup ()
{
    delete m_dtmfService;
}

void TestDTMFService::SetKeyCode( const TChar& aKeyCode )
{
    m_setKeyCodeCalled = true;
    keyValue = aKeyCode;
}

void TestDTMFService::SetPhoneNumber( const TPEPhoneNumber& aPhoneNumber )
{
    m_setPhoneNumberCalled = true;
    m_phoneNumber = aPhoneNumber;	
}

void TestDTMFService::SetCallTypeCommand( const TPECallType& aCallType )
{
    m_setCallTypeCommandCalled = true;
    m_callType = aCallType;	
}

void TestDTMFService::HandlePlayDTMFL()
{
    m_handlePlayDTMFLCalled = true;
    
    if (m_handlePlayDtmfLeave)
        User::Leave(KErrNotFound);
}

TInt TestDTMFService::HandleEndDTMF()
{
    m_handleEndDTMFCalled = true;
    return 0;
}

void TestDTMFService::SetContactId2( const TInt aContactId )
{
    m_setContactIdCalled = true;
    m_contactId = aContactId;	
}

void TestDTMFService::SetServiceIdCommand( TUint32 aServiceId )
{

}

TInt TestDTMFService::HandleDialServiceCall( const TBool aClientCall )
{
    m_handleDialCallCalled = true;
    m_clientCall = aClientCall;
    return KErrNone;
}

void TestDTMFService::testExecuteKeySequence()
{
    m_dtmfService->executeKeySequence(QString("*#0000#"));
    QString keySequence((QChar*)m_phoneNumber.Ptr(), m_phoneNumber.Length());
    QVERIFY (m_setPhoneNumberCalled == true);
    QCOMPARE (keySequence, QString("*#0000#"));
}

void TestDTMFService::testPlayDTMFTone()
{
    QChar six('6');
    m_dtmfService->playDTMFTone(six);
    QVERIFY (m_setKeyCodeCalled == true);
    QCOMPARE (keyValue, six.unicode());
    QVERIFY (m_handlePlayDTMFLCalled == true);
    
    m_handlePlayDtmfLeave = true;
    
    TRAP_IGNORE(m_dtmfService->playDTMFTone(six));
    
    m_handlePlayDtmfLeave = false;
}

void TestDTMFService::testStopDTMFPlay()
{
    m_dtmfService->stopDTMFPlay();
    QVERIFY (m_handleEndDTMFCalled == true);
}

QTEST_MAIN(TestDTMFService)
#include "unit_tests.moc"
