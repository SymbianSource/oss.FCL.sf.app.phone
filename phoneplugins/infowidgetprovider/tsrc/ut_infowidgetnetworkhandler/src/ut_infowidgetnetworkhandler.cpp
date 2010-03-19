/*
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
#include <networkhandlingproxy.h>
#include <nwhandlingengine.h>
#include "ut_infowidgetnetworkhandler.h"
#include "infowidgetnetworkhandler.h"
#include "qtestmains60.h"

const QString KHomeZoneTextTag("HomeZoneText0");    // max length 13
const TNWViagIndicatorType 
    KHomeZoneIndicatorType = ENWViagIndicatorTypeHomeZone;

const QString KMcnName("McnNameMcnNameMcnNam");     // max length 20 
const TNWMCNIndicatorType 
    KMcnIndicatorType = ENWMCNIndicatorTypeActive;

/*!
  UT_InfoWidgetNetworkHandler::UT_InfoWidgetNetworkHandler
 */
UT_InfoWidgetNetworkHandler::UT_InfoWidgetNetworkHandler() 
    : 
    m_networkHandler(0)
{
    
}


/*!
  UT_InfoWidgetNetworkHandler::~UT_InfoWidgetNetworkHandler
 */
UT_InfoWidgetNetworkHandler::~UT_InfoWidgetNetworkHandler()
{
    delete m_networkHandler;
}

/*!
  UT_InfoWidgetNetworkHandler::generateException
 */
void UT_InfoWidgetNetworkHandler::generateException()
{
    throw std::bad_alloc();
}

/*!
  UT_InfoWidgetNetworkHandler::init
 */
void UT_InfoWidgetNetworkHandler::init()
{
    initialize();
    
    EXPECT(CreateL).willOnce(invoke(this, &initializeNetworkInfo));
    m_networkHandler =  new InfoWidgetNetworkHandler();
    
    QVERIFY(verify());
}


/*!
  UT_InfoWidgetNetworkHandler::cleanup
 */
void UT_InfoWidgetNetworkHandler::cleanup()
{
    reset();
    
    delete m_networkHandler;
    m_networkHandler = 0;
}

/*!
  UT_InfoWidgetNetworkHandler::t_homeZoneTextTag
 */
void UT_InfoWidgetNetworkHandler::t_homeZoneTextTag()
{
    QVERIFY(m_networkHandler->homeZoneTextTag() == KHomeZoneTextTag);
}


/*!
  UT_InfoWidgetNetworkHandler::t_homeZoneIndicatorType
 */
void UT_InfoWidgetNetworkHandler::t_homeZoneIndicatorType()
{
    QVERIFY(
        m_networkHandler->homeZoneIndicatorType() == KHomeZoneIndicatorType);
}


/*!
  UT_InfoWidgetNetworkHandler::t_mcnName
 */
void UT_InfoWidgetNetworkHandler::t_mcnName()
{
    QVERIFY(m_networkHandler->mcnName() == KMcnName);
}


/*!
  UT_InfoWidgetNetworkHandler::t_mcnIndicatorType
 */
void UT_InfoWidgetNetworkHandler::t_mcnIndicatorType()
{
    QVERIFY(m_networkHandler->mcnIndicatorType() == KMcnIndicatorType);
}


/*!
  UT_InfoWidgetNetworkHandler::t_HandleNetworkMessage
 */
void UT_InfoWidgetNetworkHandler::t_HandleNetworkMessage()
{
    QSignalSpy spy(m_networkHandler, SIGNAL(networkDataChanged()));
    
    // TODO: investigate, which messages indicate cell info change
// ACCEPTED MESSAGES    
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageCurrentCellInfoMessage);
    
// MESSAGES TO BE FILTERED OUT
// COMMON NETWORK MESSAGES                      0 - 99
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageNetworkInfoChange);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageNetworkModeChange);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageProgrammableOperatorInfoChange);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageNetworkProviderNameChange);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageServiceProviderNameChange);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageProgrammableOperatorInfoUpdating);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageNetworkProviderNameUpdating);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageServiceProviderNameUpdating);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageAllowRefresh);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageNetworkSelectionSettingChange);
// GSM SPECIFIC MESSAGES                        100 - 199
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageNetworkCellReselection);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageNetworkConnectionFailure);
//    m_networkHandler->HandleNetworkMessage(
//        MNWMessageObserver::ENWMessageCurrentCellInfoMessage);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageCurrentHomeZoneMessage);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageNetworkIndicatorChange);
// CDMA SPECIFIC MESSAGES                       200 - 299
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageNetworkEmergencyMode); 
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageVoicePrivacyStatusChange);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageStartSystemSearchRequestCompleteOk);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageStartSystemSearchRequestCompleteFail);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageStopProtocolStackRequestCompleteOk);
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageStopProtocolStackRequestCompleteFail);
    
    const int KExpectedNumOfSignalEmissions = 1;
    QCOMPARE(spy.count(), KExpectedNumOfSignalEmissions);
    
    const QList<QVariant> &arguments = spy.at(0);
    QCOMPARE(arguments.count(), 0);
}


/*!
  UT_InfoWidgetNetworkHandler::t_HandleNetworkMessageExceptionWhileEmittingSignal
 */
void UT_InfoWidgetNetworkHandler::t_HandleNetworkMessageExceptionWhileEmittingSignal()
{
    QObject::connect(
        m_networkHandler, SIGNAL(networkDataChanged()),
        this, SLOT(generateException())); 
    
    m_networkHandler->HandleNetworkMessage(
        MNWMessageObserver::ENWMessageCurrentCellInfoMessage);
}


/*!
  UT_InfoWidgetNetworkHandler::t_HandleNetworkError
 */
Q_DECLARE_METATYPE(MNWMessageObserver::TNWOperation)
void UT_InfoWidgetNetworkHandler::t_HandleNetworkError()
{
    QSignalSpy spy(m_networkHandler, SIGNAL(networkError(int, int)));
    qRegisterMetaType<MNWMessageObserver::TNWOperation>
        ("MNWMessageObserver::TNWOperation");
    
    MNWMessageObserver::TNWOperation operation = MNWMessageObserver::ENWNone;
    const int errorCode = -4;
    m_networkHandler->HandleNetworkError(operation, errorCode);
    
    const int KExpectedNumOfSignalEmissions = 1;
    QCOMPARE(spy.count(), KExpectedNumOfSignalEmissions);
    const QList<QVariant> &arguments = spy.at(0);
    QCOMPARE(arguments.count(), 2);
    
    QCOMPARE(
        qvariant_cast<MNWMessageObserver::TNWOperation>(arguments.at(0)), 
        operation);
    QCOMPARE(arguments.at(1).toInt(), errorCode);
}


/*!
  UT_InfoWidgetNetworkHandler::t_HandleNetworkErrorExceptionWhileEmittingSignal
 */
void UT_InfoWidgetNetworkHandler::t_HandleNetworkErrorExceptionWhileEmittingSignal()
{
    QObject::connect(
        m_networkHandler, SIGNAL(networkError(int, int)),
        this, SLOT(generateException())); 
    
    MNWMessageObserver::TNWOperation operation = MNWMessageObserver::ENWNone;
    const int errorCode = -4;
    m_networkHandler->HandleNetworkError(operation, errorCode);
}


/*!
  UT_InfoWidgetNetworkHandler::initializeNetworkInfo
 */
void UT_InfoWidgetNetworkHandler::initializeNetworkInfo(
    MNWMessageObserver& /*aObserver*/, TNWInfo& aTNWInfo)
{
    aTNWInfo.iViagTextTag.Copy(KHomeZoneTextTag.utf16());
    aTNWInfo.iViagIndicatorType = KHomeZoneIndicatorType;
    
    aTNWInfo.iMCNName.Copy(KMcnName.utf16());
    aTNWInfo.iMCNIndicatorType = KMcnIndicatorType;
}

QTEST_MAIN_S60(UT_InfoWidgetNetworkHandler)
