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
* Description:  Unit tests for Phone Services.
*
*/

#include <QtTest/QtTest>

//#include <hbglobal_p.h>
#include "phoneservices.h"
#include "dialservice.h"
#include "dtmfservice.h"

bool m_dialServiceConstructed;
bool m_dtmfServiceConstructed;
MPECallControlIF* m_dialServiceCallPointer;
MPECallSettersIF* m_dialServiceParameterPointer;
MPECallControlIF* m_dtmfServiceCallPointer;
MPECallSettersIF* m_dtmfServiceParameterPointer;

class TestPhoneServices : public QObject, public MPECallControlIF, public MPECallSettersIF
{
    Q_OBJECT
public:
    TestPhoneServices();
    virtual ~TestPhoneServices();    

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
    void testPhoneServiceStartup();

private:
    PhoneServices *m_phoneServices; // class under test
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
    ushort keyValue;
};

DialService::DialService(MPECallControlIF &call, MPECallSettersIF &parameters, QObject* parent) : 
    XQServiceProvider(QLatin1String("com.nokia.services.telephony"), parent), m_call (call), m_parameters (parameters)
{
    m_dialServiceConstructed = true;
    m_dialServiceCallPointer = &call;
    m_dialServiceParameterPointer = &parameters;
}

DialService::~DialService()
{
}

int DialService::dial(const QString& number)
{
    Q_UNUSED(number);
    return 0;
}

int DialService::dial(const QString& number, int contactId)
{
    Q_UNUSED(number);
    Q_UNUSED (contactId);
    return 0;
}

void DialService::dialVideo(const QString& number)
{
    Q_UNUSED(number);
}

void DialService::dialVideo(const QString& number, int contactId)
{
    Q_UNUSED(number);
    Q_UNUSED(contactId);
}

void DialService::dialVoip(const QString& address)
{
Q_UNUSED(address);
}

void DialService::dialVoip(const QString& address, int contactId)
{
Q_UNUSED(address);
Q_UNUSED(contactId);
}

void DialService::dialVoipService(const QString& address, int serviceId)
{
Q_UNUSED(address);
Q_UNUSED(serviceId);
}

void DialService::dialVoipService(
        const QString& address, int serviceId, int contactId)
{
Q_UNUSED(address);
Q_UNUSED(serviceId);
Q_UNUSED(contactId);
}

DTMFService::DTMFService(MPECallControlIF &call, MPECallSettersIF &parameters, QObject* parent) : 
    XQServiceProvider(QLatin1String("com.nokia.services.telephony.dtmf"), parent), m_call (call), m_parameters (parameters)
{
    m_dtmfServiceConstructed = true;
    m_dtmfServiceCallPointer = &call;
    m_dtmfServiceParameterPointer = &parameters;
}

DTMFService::~DTMFService()
{
}

void DTMFService::executeKeySequence(const QString& keySequence)
{
    Q_UNUSED(keySequence);
}

void DTMFService::playDTMFTone(const QChar& keyToPlay)
{
    Q_UNUSED(keyToPlay);
}

void DTMFService::stopDTMFPlay()
{
}

TestPhoneServices::TestPhoneServices ()
{
}

TestPhoneServices::~TestPhoneServices ()
{
}

void TestPhoneServices::initTestCase ()
{
}

void TestPhoneServices::cleanupTestCase ()
{
}

void TestPhoneServices::init ()
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
    m_dialServiceConstructed = false;
    m_dtmfServiceConstructed = false;
    
    m_phoneServices = new PhoneServices (*this, *this, this);

    QVERIFY(m_dialServiceConstructed == true);
    QVERIFY(m_dialServiceCallPointer == this);
    QVERIFY(m_dialServiceParameterPointer == this);
    
    QVERIFY(m_dtmfServiceConstructed == true);
    QVERIFY(m_dtmfServiceCallPointer == this);
    QVERIFY(m_dtmfServiceParameterPointer == this);
}

void TestPhoneServices::cleanup ()
{
    delete m_phoneServices;
}

void TestPhoneServices::SetKeyCode( const TChar& aKeyCode )
{
    m_setKeyCodeCalled = true;
    keyValue = aKeyCode;
}

void TestPhoneServices::SetPhoneNumber( const TPEPhoneNumber& aPhoneNumber )
{
    m_setPhoneNumberCalled = true;
    m_phoneNumber = aPhoneNumber;	
}

void TestPhoneServices::SetCallTypeCommand( const TPECallType& aCallType )
{
    m_setCallTypeCommandCalled = true;
    m_callType = aCallType;	
}

void TestPhoneServices::HandlePlayDTMFL()
{
    m_handlePlayDTMFLCalled = true;
}

TInt TestPhoneServices::HandleEndDTMF()
{
    m_handleEndDTMFCalled = true;
    return 0;
}

void TestPhoneServices::SetContactId2( const TInt aContactId )
{
    m_setContactIdCalled = true;
    m_contactId = aContactId;	
}

void TestPhoneServices::SetServiceIdCommand( TUint32 aServiceId )
{
	
}

TInt TestPhoneServices::HandleDialServiceCall( const TBool aClientCall )
{
    m_handleDialCallCalled = true;
    m_clientCall = aClientCall;
    return KErrNone;
}

void TestPhoneServices::testPhoneServiceStartup()
{

}

QTEST_MAIN(TestPhoneServices)
#include "unit_tests.moc"
