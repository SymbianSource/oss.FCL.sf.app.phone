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
#include "ut_infowidget.h"
#include "qtestmains60.h"
#include <QString>

const QString KPreferenceOn("On"); 
const QString KPreferenceOff("Off"); 
const QString KMcnName("tnw1: 3233255435"); 

/*!
  UT_InfoWidget::UT_InfoWidget
 */
UT_InfoWidget::UT_InfoWidget() 
    : m_infoWidget(0)
{
    SmcDefaultValue<QString>::SetL("On");
    m_modelData.setActiveLine(1);
    m_modelData.setHomeZoneIndicatorType(1);
    m_modelData.setMcnName(KMcnName);
        
    SmcDefaultValue<const InfoWidgetEngine::ModelData &>::SetL(m_modelData);
}

/*!
  UT_InfoWidget::~UT_InfoWidget
 */
UT_InfoWidget::~UT_InfoWidget()
{
    delete m_infoWidget;
}

/*!
  UT_InfoWidget::init
 */
void UT_InfoWidget::init()
{
    initialize();
    
    m_infoWidget = new InfoWidget();
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::cleanup
 */
void UT_InfoWidget::cleanup()
{
    reset();
    
    delete m_infoWidget;
    m_infoWidget = 0;
}

/*!
  UT_InfoWidget::t_setHomeZoneDisplay
 */
void UT_InfoWidget::t_setHomeZoneDisplay()
{
    expect("InfoWidgetPreferences::setPreference");
    m_infoWidget->setHomeZoneDisplay(KPreferenceOn); 
    expect("InfoWidgetPreferences::preference");
    QVERIFY(m_infoWidget->homeZoneDisplay() == KPreferenceOn);
}

/*!
  UT_InfoWidget::t_setSatDisplay
 */
void UT_InfoWidget::t_setSatDisplay()
{
    expect("InfoWidgetPreferences::setPreference");
    m_infoWidget->setSatDisplay(KPreferenceOn); 
    expect("InfoWidgetPreferences::preference");
    QVERIFY(m_infoWidget->satDisplay() == KPreferenceOn);
}

/*!
  UT_InfoWidget::t_setActiveLineDisplay
 */
void UT_InfoWidget::t_setActiveLineDisplay()
{
    expect("InfoWidgetPreferences::setPreference");
    m_infoWidget->setActiveLineDisplay(KPreferenceOn); 
    expect("InfoWidgetPreferences::preference");
    QVERIFY(m_infoWidget->activeLineDisplay() == KPreferenceOn);
}

/*!
  UT_InfoWidget::t_setMcnDisplay
 */
void UT_InfoWidget::t_setMcnDisplay()
{
    expect("InfoWidgetPreferences::setPreference");
    m_infoWidget->setMcnDisplay(KPreferenceOn); 
    expect("InfoWidgetPreferences::preference");
    QVERIFY(m_infoWidget->mcnDisplay() == KPreferenceOn);
}

/*!
  UT_InfoWidget::t_readModel
 */
void UT_InfoWidget::t_readModel()
{
    m_infoWidget->readModel(); 
}

/*!
  UT_InfoWidget::t_handleModelError
 */
void UT_InfoWidget::t_handleModelError()
{
    int operation(0);
    int errorCode(0); 
    m_infoWidget->handleModelError(operation, errorCode); 
}

/*!
  UT_InfoWidget::t_onInitialize
 */
void UT_InfoWidget::t_onInitialize()
{
    m_infoWidget->onInitialize(); 
}

/*!
  UT_InfoWidget::t_onUninitialize
 */
void UT_InfoWidget::t_onUninitialize()
{
    m_infoWidget->onUninitialize(); 
}

/*!
  UT_InfoWidget::t_onShow
 */
void UT_InfoWidget::t_onShow()
{
    m_infoWidget->onShow(); 
}

/*!
  UT_InfoWidget::t_onHide
 */
void UT_InfoWidget::t_onHide()
{
    m_infoWidget->onHide(); 
}

void UT_InfoWidget::t_homeZoneDisplaySettingChanged()
{
    int setting(0); 
    m_infoWidget->homeZoneDisplaySettingChanged(setting); 
}

void UT_InfoWidget::t_mcnDisplaySettingChanged()
{
    int setting(0);
    m_infoWidget->mcnDisplaySettingChanged(setting); 
}

void UT_InfoWidget::t_activeLineDisplaySettingChanged()
{
    int setting(0);
    m_infoWidget->activeLineDisplaySettingChanged(setting); 
}

void UT_InfoWidget::t_satDisplaySettingChanged()
{
    int setting(0);
    m_infoWidget->satDisplaySettingChanged(setting); 
}

void UT_InfoWidget::t_settingsEditingFinished()
{
    m_infoWidget->settingsEditingFinished(); 
}


QTEST_MAIN_S60(UT_InfoWidget)
