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
#include "ut_infowidgetengine.h"
#include "infowidgetengine.h"
#include "qtestmains60.h"
// mocked dependencies of the class under test
#include "infowidgetnetworkhandler.h"

const QString KHomeZoneTextTag("HomeZoneText0");    // max length 13
const TNWViagIndicatorType 
    KHomeZoneIndicatorType = ENWViagIndicatorTypeHomeZone;

const QString KMcnName("McnNameMcnNameMcnNam");     // max length 20 
const TNWMCNIndicatorType 
    KMcnIndicatorType = ENWMCNIndicatorTypeActive;

/*!
  UT_InfoWidgetEngine::UT_InfoWidgetEngine
 */
UT_InfoWidgetEngine::UT_InfoWidgetEngine() 
    : 
    m_infoWidgetEngine(0)
{
    
}


/*!
  UT_InfoWidgetEngine::~UT_InfoWidgetEngine
 */
UT_InfoWidgetEngine::~UT_InfoWidgetEngine()
{
    delete m_infoWidgetEngine;
}


/*!
  UT_InfoWidgetEngine::init
 */
void UT_InfoWidgetEngine::init()
{
    initialize();
    
    EXPECT(InfoWidgetNetworkHandler::logCurrentInfo);
    EXPECT(InfoWidgetNetworkHandler::homeZoneTextTag)
        .returns(KHomeZoneTextTag);
    EXPECT(InfoWidgetNetworkHandler::homeZoneIndicatorType)
        .returns(KHomeZoneIndicatorType);
    EXPECT(InfoWidgetNetworkHandler::mcnName).returns(KMcnName);
    EXPECT(InfoWidgetNetworkHandler::mcnIndicatorType)
        .returns(KMcnIndicatorType);
    
    m_infoWidgetEngine = new InfoWidgetEngine();
    
    QVERIFY(verify());
}


/*!
  UT_InfoWidgetEngine::cleanup
 */
void UT_InfoWidgetEngine::cleanup()
{
    reset();
    
    delete m_infoWidgetEngine;
    m_infoWidgetEngine = 0;
}


/*!
  UT_InfoWidgetEngine::t_modelData
 */
void UT_InfoWidgetEngine::t_modelData()
{
    const InfoWidgetEngine::ModelData& data = m_infoWidgetEngine->modelData();
    QVERIFY(data.homeZoneTextTag() == KHomeZoneTextTag);
    QVERIFY(data.homeZoneIndicatorType() == KHomeZoneIndicatorType);
    QVERIFY(data.mcnName() == KMcnName);
    QVERIFY(data.mcnIndicatorType() == KMcnIndicatorType);
}


/*!
  UT_InfoWidgetEngine::t_updateNetworkDataToModel
 */
void UT_InfoWidgetEngine::t_updateNetworkDataToModel()
{
    EXPECT(InfoWidgetNetworkHandler::logCurrentInfo);
    EXPECT(InfoWidgetNetworkHandler::homeZoneTextTag)
        .returns(KHomeZoneTextTag);
    EXPECT(InfoWidgetNetworkHandler::homeZoneIndicatorType)
        .returns(KHomeZoneIndicatorType);
    EXPECT(InfoWidgetNetworkHandler::mcnName).returns(KMcnName);
    EXPECT(InfoWidgetNetworkHandler::mcnIndicatorType)
        .returns(KMcnIndicatorType);
    QSignalSpy spy(m_infoWidgetEngine, SIGNAL(modelChanged()));
    
    m_infoWidgetEngine->updateNetworkDataToModel();
    
    const int KExpectedNumOfSignalEmissions = 1;
    QCOMPARE(spy.count(), KExpectedNumOfSignalEmissions);
    const QList<QVariant> &arguments = spy.at(0);
    QCOMPARE(arguments.count(), 0);
    
    QVERIFY(verify());
}


/*!
  UT_InfoWidgetEngine::t_updateSatDataToModel
 */
void UT_InfoWidgetEngine::t_updateSatDataToModel()
{
    m_infoWidgetEngine->updateSatDataToModel();
}


/*!
  UT_InfoWidgetEngine::t_updateLineDataToModel
 */
void UT_InfoWidgetEngine::t_updateLineDataToModel()
{
    m_infoWidgetEngine->updateLineDataToModel();
}


/*!
  UT_InfoWidgetEngine::t_handleNetworkError
 */
void UT_InfoWidgetEngine::t_handleNetworkError()
{
    m_infoWidgetEngine->handleNetworkError(0, 0);
}


/*!
  UT_InfoWidgetEngine::t_handleSatError
 */
void UT_InfoWidgetEngine:: t_handleSatError()
{
    m_infoWidgetEngine->handleSatError(0, 0);
}


/*!
  UT_InfoWidgetEngine::t_handleLineError
 */
void UT_InfoWidgetEngine:: t_handleLineError()
{
    m_infoWidgetEngine->handleLineError(0, 0);    
}

QTEST_MAIN_S60(UT_InfoWidgetEngine)
