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

#include "infowidgetengine.h"
#include "infowidgetnetworkhandler.h"
#include <networkhandlingproxy.h>
#include "infowidgetlogging.h"
#include <xqsettingsmanager.h>
#include <xqsettingskey.h>
#include <SettingsInternalCRKeys.h>

/*!
   InfoWidgetEngine::InfoWidgetEngine
 */
InfoWidgetEngine::InfoWidgetEngine(QObject *parent): 
    QObject(parent),
    m_networkHandler(new InfoWidgetNetworkHandler)
{
    DPRINT << ": IN";
    
    QObject::connect(
        m_networkHandler.data(), SIGNAL(networkError(int, int)),
        this, SLOT(handleNetworkError(int, int))); 
    
    QObject::connect(
        m_networkHandler.data(), SIGNAL(networkDataChanged()),
        this, SLOT(updateNetworkDataToModel()));
    
    XQSettingsManager *settingsManager = new XQSettingsManager(0); 
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, 
        KCRUidNetworkSettings.iUid, KSettingsMcnDisplay); 
    bool result = settingsManager->writeItemValue(settingsKey, 1 );
    delete settingsManager;

    updateNetworkDataToModel(); 
    
    DPRINT << ": OUT";
}

/*!
   InfoWidgetEngine::~InfoWidgetEngine
 */
InfoWidgetEngine::~InfoWidgetEngine()
{
    DPRINT;
}    

/*!
   InfoWidgetEngine::logModelData
   
   Utility function for logging model data
 */
void InfoWidgetEngine::logModelData()
{
    DPRINT << ": mcn name: " << m_modelData.mcnName(); 
    DPRINT << ": service provider name: " << m_modelData.serviceProviderName();
    DPRINT << ": homezone text tag: " << m_modelData.homeZoneTextTag();
    
    DPRINT << ": mcn type: " << m_modelData.mcnIndicatorType(); 
    DPRINT << ": active line: " << m_modelData.activeLine(); 
    DPRINT << ": homezone indicator type: " << m_modelData.homeZoneIndicatorType(); 
} 

/*!
   InfoWidgetEngine::modelData
   
   Getter for model data. 
 */
const InfoWidgetEngine::ModelData& InfoWidgetEngine::modelData() const
{
    DPRINT;
    return m_modelData; 
}

/*!
   InfoWidgetEngine::updateNetworkDataToModel
 */
void InfoWidgetEngine::updateNetworkDataToModel()
{
    DPRINT << ": IN"; 
    
    m_networkHandler->logCurrentInfo();
    
    // Read network handler data to model data
    m_modelData.setHomeZoneIndicatorType(
        m_networkHandler->homeZoneIndicatorType());
    m_modelData.setHomeZoneTextTag(m_networkHandler->homeZoneTextTag()); 
    m_modelData.setMcnName(m_networkHandler->mcnName()); 
    m_modelData.setMcnIndicatorType(m_networkHandler->mcnIndicatorType());
    
    emit modelChanged();
    
    DPRINT << ": OUT";
}

/*!
   InfoWidgetEngine::updateSatDataToModel
 */
void InfoWidgetEngine::updateSatDataToModel()
{
    DPRINT;
}

/*!
   InfoWidgetEngine::updateLineDataToModel
 */
void InfoWidgetEngine::updateLineDataToModel()
{
    DPRINT;
}

/*!
   InfoWidgetEngine::handleNetworkError
 */
void InfoWidgetEngine::handleNetworkError(int operation, int errorCode)
{
    DPRINT << ": operation: " << operation << " error code: " << errorCode; 
}

/*!
   InfoWidgetEngine::handleSatError
 */
void InfoWidgetEngine::handleSatError(int operation, int errorCode)
{
    DPRINT << ": operation: " << operation << " error code: " << errorCode; 
}

/*!
   InfoWidgetEngine::handleLineError
 */
void InfoWidgetEngine::handleLineError(int operation, int errorCode)
{
    DPRINT << ": operation: " << operation << " error code: " << errorCode; 
}

// End of File. 

