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
* Description:  Unit tests for PhoneUIQtButtonsController.
*
*/

#include <QtTest/QtTest>
#include <mockservice.h>
#include "qtestmains60.h"
#include "phonecallheaderutil.h"
#include "pevirtualengine.h"
#include "cpeengineinfo.h"
#include "tphonecmdparamcallheaderdata.h"
#include "cphonemainresourceresolver.h"
#include "cphonecenrepproxy.h"
#include "phonerssbase.h"
#include <StringLoader.h>
#include <coemain.h>

_LIT(KSimulatedStringLoaderString,"1234567890");

class CPhoneMainResourceResolverMock : public CPhoneMainResourceResolver
    {    
    };

class CPhoneCenRepProxyMock : public CPhoneCenRepProxy
    { 
public:
    CPhoneCenRepProxyMock() {};
    };

void simulateStringLoad(TDes& buf, TInt /*id*/, CCoeEnv* /*coeenv*/)
{
    buf.Append(KSimulatedStringLoaderString());
}

class TestPhoneCallHeaderUtil : public QObject, MockService
{
    Q_OBJECT
public:
    TestPhoneCallHeaderUtil();
    virtual ~TestPhoneCallHeaderUtil();

public slots:
    void initTestCase ();
    void cleanupTestCase ();
    void init ();
    void cleanup (); 
    
private slots:
    void testSetIncomingCallHeaderParams();
    void testSetOutgoingCallHeaderParams();
    void testUpdateCallHeaderInfo();
    void testSetCallHeaderType();
    void testSetDivertIndication();
    void testGetRemoteInfoData();
    void testSetCallHeaderTexts();
    void testLabelText();
    void testEmergencyHeaderText();
    void testAttemptingEmergencyText();
    void testCallState();            
    void testLoadResource();
    
private:
    CPEEngineInfo* m_engineInfo;
    PhoneCallHeaderUtil* m_util;
    CPhoneMainResourceResolverMock* m_resourceResolverMock;
    CPhoneCenRepProxyMock* m_cenrepProxyMock;
};

TestPhoneCallHeaderUtil::TestPhoneCallHeaderUtil ()
{
    
}

TestPhoneCallHeaderUtil::~TestPhoneCallHeaderUtil ()
{
}

void TestPhoneCallHeaderUtil::initTestCase ()
{
    m_engineInfo = CPEEngineInfo::NewL();
    m_util = new PhoneCallHeaderUtil(*m_engineInfo);
    
    m_resourceResolverMock = new CPhoneMainResourceResolverMock();
    SmcDefaultValue<CPhoneMainResourceResolver*>::SetL( m_resourceResolverMock );
    
    m_cenrepProxyMock = new CPhoneCenRepProxyMock();
    SmcDefaultValue<CPhoneCenRepProxy*>::SetL( m_cenrepProxyMock );
}

void TestPhoneCallHeaderUtil::cleanupTestCase ()
{
    delete m_util;
    delete m_engineInfo;
    delete m_resourceResolverMock;
}

void TestPhoneCallHeaderUtil::init ()
{
    initialize();
}

void TestPhoneCallHeaderUtil::cleanup ()
{
    reset();
}

void TestPhoneCallHeaderUtil::testSetIncomingCallHeaderParams()
{
    TPhoneCmdParamCallHeaderData data;       
    
    const int callId = 1;
    const int serviceId = 100;
    _LIT(KPhoneNumber,"123456");
    _LIT(KCallerImage,"image.jpg");
    
    m_engineInfo->SetCallState(EPEStateRinging, callId);
    m_engineInfo->SetCallType(EPECallTypeCSVoice, callId);
    m_engineInfo->SetCallSecureStatus(EFalse, callId);
    m_engineInfo->SetSecureSpecified(ETrue);
    m_engineInfo->SetServiceId(callId, serviceId);
    m_engineInfo->SetRemotePhoneNumber(KPhoneNumber(), callId);
    m_engineInfo->SetRemotePhoneNumberType(EPEMobileNumber, callId);
    m_engineInfo->SetCallerImage(KCallerImage(), callId);
    m_engineInfo->SetIncomingCallForwarded(ETrue, callId);
    
    EXPECT(CPhoneMainResourceResolver,ResolveResourceID).with(EPhoneIncomingCallLabel);    
    m_util->SetIncomingCallHeaderParams(callId,EFalse,EFalse,&data);
    QVERIFY(verify());
    
    QVERIFY(data.CallState()==EPEStateRinging);
    QVERIFY(data.CallType()==EPECallTypeCSVoice);
    QVERIFY(data.Ciphering()==EFalse);
    QVERIFY(data.CipheringIndicatorAllowed()==ETrue);
    QVERIFY(data.ServiceId()==serviceId);
    QVERIFY(data.RemotePhoneNumber()==KPhoneNumber);
    QVERIFY(data.NumberType()==EPEMobileNumber);
    QVERIFY(data.Picture()==KCallerImage);
    QVERIFY(data.Diverted()==ETrue);
}

void TestPhoneCallHeaderUtil::testSetOutgoingCallHeaderParams()
{
    TPhoneCmdParamCallHeaderData data;
    
    const int callId = 1;
    const int serviceId = 100;
    _LIT(KPhoneNumber,"123456");
    _LIT(KCallerImage,"image.jpg");
    
    m_engineInfo->SetCallState(EPEStateDialing, callId);
    m_engineInfo->SetCallType(EPECallTypeCSVoice, callId);
    m_engineInfo->SetCallSecureStatus(EFalse, callId);
    m_engineInfo->SetSecureSpecified(ETrue);
    m_engineInfo->SetServiceId(callId, serviceId);
    m_engineInfo->SetRemotePhoneNumber(KPhoneNumber(), callId);
    m_engineInfo->SetRemotePhoneNumberType(EPEMobileNumber, callId);
    m_engineInfo->SetCallerImage(KCallerImage(), callId);
    
    EXPECT(CPhoneMainResourceResolver,ResolveResourceID).with(EPhoneOutgoingCallLabel);    
    m_util->SetOutgoingCallHeaderParams(callId,&data);
    QVERIFY(verify());
    
    QVERIFY(data.CallState()==EPEStateDialing);
    QVERIFY(data.CallType()==EPECallTypeCSVoice);
    QVERIFY(data.Ciphering()==EFalse);
    QVERIFY(data.CipheringIndicatorAllowed()==ETrue);
    QVERIFY(data.ServiceId()==serviceId);
    QVERIFY(data.RemotePhoneNumber()==KPhoneNumber);
    QVERIFY(data.NumberType()==EPEMobileNumber);
    QVERIFY(data.Picture()==KCallerImage);
}

void TestPhoneCallHeaderUtil::testUpdateCallHeaderInfo()
{
    TPhoneCmdParamCallHeaderData data;
    
    const int callId = 1;
    const int serviceId = 100;
    _LIT(KPhoneNumber,"123456");
    _LIT(KCallerImage,"image.jpg");
    
    m_engineInfo->SetRemotePhoneNumber(KPhoneNumber(), callId);
    m_engineInfo->SetRemotePhoneNumberType(EPEMobileNumber, callId);
    m_engineInfo->SetRemoteIdentity(RMobileCall::ERemoteIdentityAvailable, callId);
    m_engineInfo->SetCallerImage(KCallerImage(), callId);
    
    EXPECT(CPhoneMainResourceResolver,ResolveResourceID).with(EPhoneIncomingCallLabel);    
    m_util->UpdateCallHeaderInfo(1,0,0,&data);
    QVERIFY(verify());
    
    QVERIFY(data.CLIText()==KPhoneNumber);
    QVERIFY(data.Picture()==KCallerImage);          
}

void TestPhoneCallHeaderUtil::testSetCallHeaderType()
{
    m_util->SetCallHeaderType(EPECallTypeCSVoice);
    QVERIFY(m_util->CallHeaderType()==EPECallTypeCSVoice);
}

void TestPhoneCallHeaderUtil::testSetDivertIndication()
{
    m_util->SetDivertIndication(ETrue);
}

void TestPhoneCallHeaderUtil::testGetRemoteInfoData()
{
    TBuf<100> buf;
    const int callId = 1;
    _LIT(KPhoneNumber,"123456");
    
    m_engineInfo->SetRemotePhoneNumber(KPhoneNumber(), callId);
    m_engineInfo->SetRemotePhoneNumberType(EPEMobileNumber, callId);
    m_engineInfo->SetCallDirection(RMobileCall::EMobileTerminated, callId);
    m_engineInfo->SetRemoteIdentity(RMobileCall::ERemoteIdentityAvailable, callId);
    
    m_util->GetRemoteInfoData(callId,buf);
    QVERIFY(buf==KPhoneNumber());
}

void TestPhoneCallHeaderUtil::testSetCallHeaderTexts()
{
    TPhoneCmdParamCallHeaderData data;    
    
    const int callId = 1;
    const int serviceId = 100;
    _LIT(KPhoneNumber,"123456");
        
    m_engineInfo->SetRemotePhoneNumber(KPhoneNumber(), callId);
    m_engineInfo->SetRemotePhoneNumberType(EPEMobileNumber, callId);
    m_engineInfo->SetALSLine(CCCECallParameters::ECCELineTypePrimary);
    
    EXPECT(CPhoneMainResourceResolver,ResolveResourceID).with(EPhoneIncomingCallLabel);
    m_util->SetCallHeaderTexts(1,0,0,&data);

    QVERIFY(verify());                
}

void TestPhoneCallHeaderUtil::testLabelText()
{
    const int callId = 1;
   
    m_engineInfo->SetCallState(EPEStateHeld, callId);
    m_engineInfo->SetCallType(EPECallTypeCSVoice, callId);
    
    TBuf<100> label = m_util->LabelText(1);
    EXPECT(CPhoneMainResourceResolver,ResolveResourceID).with(EPhoneCallOnHold);
}

void TestPhoneCallHeaderUtil::testEmergencyHeaderText()
{
    _LIT( KResourceText, "emergency" );
    HBufC* text = KResourceText().AllocLC();
    EXPECT(StringLoader,LoadL).returns<HBufC*>(0);
    QVERIFY(m_util->EmergencyHeaderText()==KNullDesC);
    verify();
    EXPECT(StringLoader,LoadL).returns(text);   
    QVERIFY(m_util->EmergencyHeaderText()==text->Des());
    verify();
}

void TestPhoneCallHeaderUtil::testAttemptingEmergencyText()
{
    _LIT( KResourceText, "attempting" );
    HBufC* text = KResourceText().AllocLC();
    EXPECT(StringLoader,LoadL).returns<HBufC*>(0);
    QVERIFY(m_util->AttemptingEmergencyText()==KNullDesC);
    verify();
    EXPECT(StringLoader,LoadL).returns(text);   
    QVERIFY(m_util->AttemptingEmergencyText()==text->Des());
    verify();
}

void TestPhoneCallHeaderUtil::testCallState()
{
    m_engineInfo->SetCallState(EPEStateDialing,1);
    QVERIFY(m_util->CallState(1)==EPEStateDialing);    
}

void TestPhoneCallHeaderUtil::testLoadResource()
{
    TBuf<100> buf;
    
    EXPECT(CPhoneMainResourceResolver,ResolveResourceID).with(1).returns(1);
    EXPECT(StringLoader,Load).willOnce(invoke(simulateStringLoad));
    m_util->LoadResource(buf,1);
    QVERIFY(verify());
    QVERIFY(buf==KSimulatedStringLoaderString());
    
    _LIT( KResourceText, "resource" );
    HBufC* text = KResourceText().AllocLC();
    EXPECT(CPhoneMainResourceResolver,ResolveResourceID).with(1).returns(1);
    EXPECT(StringLoader,LoadL).returns(text);   
    HBufC* ret = m_util->LoadResource(1);
    QVERIFY(ret==text);
    delete text;
}

QTEST_MAIN_S60(TestPhoneCallHeaderUtil)
#include "unit_tests.moc"
