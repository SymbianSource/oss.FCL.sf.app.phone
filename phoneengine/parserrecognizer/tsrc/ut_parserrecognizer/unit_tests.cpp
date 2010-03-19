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
* Description:  Unit tests for ParserRecognizer.
*
*/

#include <QtTest/QtTest>

//#include <hbglobal_p.h>
#include "xqservicerequest.h"
#include "parserrecognizer.h"
#include "pevirtualengine.h"

QString apiString;
QString methodString;
bool sendCalled;

class TestParserRecognizer : public QObject
{
    Q_OBJECT
public:
    TestParserRecognizer();
    virtual ~TestParserRecognizer();

public slots:
    void initTestCase ();
    void cleanupTestCase ();
    void init ();
    void cleanup ();
        
private slots:
    void testActivateRfsDeep();
    void testActivateRfsNormal();
    void testActivateWarrantyMode();
    void testShowBTDeviceAddress();
    void testShowBTLoopback();
    void testActivateBTDebugMode();
    void testShowIMEI();
    void testShowVersion();
    void testShowWLANMacAddress();
    void testSSRequestFailed();

private:
    ParserRecognizer *parserRecognizer; // class under test
};

XQServiceRequest::XQServiceRequest(QString const& api, QString const& method, bool const& sync)
{
    Q_UNUSED(sync);
    apiString = api;
    methodString = method;
}

bool XQServiceRequest::send(QVariant& retValue)
{
    Q_UNUSED(retValue);
    sendCalled = true;
    return true;
}

XQServiceRequest::~XQServiceRequest()
{
}

TestParserRecognizer::TestParserRecognizer ()
{
}

TestParserRecognizer::~TestParserRecognizer ()
{
}

void TestParserRecognizer::initTestCase ()
{
}

void TestParserRecognizer::cleanupTestCase ()
{
}

void TestParserRecognizer::init ()
{
    apiString = "";
    methodString = "";
    sendCalled = false;
    parserRecognizer = new ParserRecognizer;
}

void TestParserRecognizer::cleanup ()
{
    delete parserRecognizer;
}

void TestParserRecognizer::testActivateRfsDeep()
{
    parserRecognizer->sendMessage(MEngineMonitor::EPEMessageActivateRfsDeep, 0);
    QCOMPARE(apiString, QString("com.nokia.services.telephony"));
    QCOMPARE(methodString, QString("activateDeepRestoreFactorySettings()"));
    QCOMPARE(sendCalled, true);
}

void TestParserRecognizer::testActivateRfsNormal()
{
    parserRecognizer->sendMessage(MEngineMonitor::EPEMessageActivateRfsNormal, 0);
    QCOMPARE(apiString, QString("com.nokia.services.telephony"));
    QCOMPARE(methodString, QString("activateNormalRestoreFactorySettings()"));
    QCOMPARE(sendCalled, true);
}

void TestParserRecognizer::testActivateWarrantyMode()
{
    parserRecognizer->sendMessage(MEngineMonitor::EPEMessageActivateWarrantyMode, 0);
    QCOMPARE(apiString, QString("com.nokia.services.telephony"));
    QCOMPARE(methodString, QString("activateWarrantyMode()"));
    QCOMPARE(sendCalled, true);
}

void TestParserRecognizer::testShowBTDeviceAddress()
{
    parserRecognizer->sendMessage(MEngineMonitor::EPEMessageShowBTDeviceAddress, 0);
    QCOMPARE(apiString, QString("com.nokia.services.bluetooth"));
    QCOMPARE(methodString, QString("showBluetoothDeviceAddress()"));
    QCOMPARE(sendCalled, true);
}

void TestParserRecognizer::testShowBTLoopback()
{
    parserRecognizer->sendMessage(MEngineMonitor::EPEMessageShowBTLoopback, 0);
    QCOMPARE(apiString, QString("com.nokia.services.bluetooth"));
    QCOMPARE(methodString, QString("showBluetoothLoopback()"));
    QCOMPARE(sendCalled, true);
}

void TestParserRecognizer::testActivateBTDebugMode()
{
    parserRecognizer->sendMessage(MEngineMonitor::EPEMessageBTDebugMode, 0);
    QCOMPARE(apiString, QString("com.nokia.services.bluetooth"));
    QCOMPARE(methodString, QString("activateBluetoothDebugMode()"));
    QCOMPARE(sendCalled, true);
}

void TestParserRecognizer::testShowIMEI()
{
    parserRecognizer->sendMessage(MEngineMonitor::EPEMessageShowIMEI, 0);
    QCOMPARE(apiString, QString("com.nokia.services.telephony"));
    QCOMPARE(methodString, QString("showIMEICode()"));
    QCOMPARE(sendCalled, true);
}

void TestParserRecognizer::testShowVersion()
{
    parserRecognizer->sendMessage(MEngineMonitor::EPEMessageShowVersion, 0);
    QCOMPARE(apiString, QString("com.nokia.services.telephony"));
    QCOMPARE(methodString, QString("showVersionNumber()"));
    QCOMPARE(sendCalled, true);
}

void TestParserRecognizer::testShowWLANMacAddress()
{
    parserRecognizer->sendMessage(MEngineMonitor::EPEMessageShowWlanMacAddress, 0);
    QCOMPARE(apiString, QString("com.nokia.services.wlan"));
    QCOMPARE(methodString, QString("showWLANMacAddress()"));
    QCOMPARE(sendCalled, true);
}

void TestParserRecognizer::testSSRequestFailed()
{
    parserRecognizer->sendMessage(MEngineMonitor::EPEMessageSSRequestFailed, 0);
    QCOMPARE(apiString, QString("com.nokia.services.telephony"));
    QCOMPARE(methodString, QString("supplementaryServiceRequestFailed()"));
    QCOMPARE(sendCalled, true);
}

QTEST_MAIN(TestParserRecognizer)
#include "unit_tests.moc"
