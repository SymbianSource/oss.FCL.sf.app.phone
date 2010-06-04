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
#include "infowidgetsathandler.h"
#include <networkhandlingproxy.h>
#include "infowidgetlogging.h"
#include "infowidgetpreferences.h"

/*!
  \class InfoWidgetEngine
  \brief Engine functionality of 
         Operator info widget
*/


/*!
   InfoWidgetEngine::InfoWidgetEngine
 */
InfoWidgetEngine::InfoWidgetEngine(QObject *parent): 
    QObject(parent),
    m_networkHandler(new InfoWidgetNetworkHandler),
    m_satHandler(new InfoWidgetSatHandler)
{
    DPRINT << ": IN";
    
    QObject::connect(
        m_networkHandler.data(), SIGNAL(networkError(int, int)),
        this, SLOT(handleNetworkError(int, int))); 
    
    QObject::connect(
        m_networkHandler.data(), SIGNAL(networkDataChanged()),
        this, SLOT(updateNetworkDataToModel()));

    QObject::connect(m_satHandler.data(), 
            SIGNAL(handleError(int, int)),
            this, SLOT(handleSatError(int, int))); 
    
    QObject::connect(m_satHandler.data(), 
                SIGNAL(handleMessage(int)),
                this, SLOT(updateSatDataToModel())); 

    updateNetworkDataToModel();
    updateSatDataToModel();
    
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
    DPRINT << ": mcn type: " << m_modelData.mcnIndicatorType();
    DPRINT << ": service provider name: " << 
            m_modelData.serviceProviderName();
    DPRINT << ": service provider display required: " << 
            m_modelData.serviceProviderNameDisplayRequired(); 

    DPRINT << ": homezone text tag: " << 
            m_modelData.homeZoneTextTag();
    DPRINT << ": homezone indicator type: " << 
            m_modelData.homeZoneIndicatorType(); 
    DPRINT << ": active line: " << 
            m_modelData.activeLine(); 
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
    
    if (m_networkHandler->isOnline()) {
        // Read network handler data to model data
        m_modelData.setServiceProviderName(
                m_networkHandler->serviceProviderName());
        m_modelData.setServiceProviderNameDisplayRequired(
                m_networkHandler->serviceProviderNameDisplayRequired());
    
        m_modelData.setMcnName(m_networkHandler->mcnName()); 
        m_modelData.setMcnIndicatorType(
                m_networkHandler->mcnIndicatorType());
        
        m_modelData.setHomeZoneIndicatorType(
            m_networkHandler->homeZoneIndicatorType());
        m_modelData.setHomeZoneTextTag(
                m_networkHandler->homeZoneTextTag());
    } else {
        // Not registered to network, clear data
        m_modelData.setServiceProviderName(QString(""));
        m_modelData.setMcnName(QString(""));
        m_modelData.setHomeZoneTextTag(QString("")); 
    }
        
    emit modelChanged();
    
    DPRINT << ": OUT";
}

/*!
   InfoWidgetEngine::updateSatDataToModel
 */
void InfoWidgetEngine::updateSatDataToModel()
{
    DPRINT << ": IN";
    
    if (m_satHandler) {
        // Log current network data 
        m_satHandler->logCurrentInfo();
        // Read SAT handler data to model data
        m_modelData.setSatDisplayText(
                m_satHandler->satDisplayText());
        
        emit modelChanged(); 
    } 
     
    DPRINT << ": OUT";
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
void InfoWidgetEngine::handleNetworkError(
        int operation, int errorCode)
{
    DPRINT << ": operation: " << operation << 
            " error code: " << errorCode; 
}

/*!
   InfoWidgetEngine::handleSatError
 */
void InfoWidgetEngine::handleSatError(
        int operation, int errorCode)
{
    DPRINT << ": operation: " << operation << 
            " error code: " << errorCode; 
}

/*!
   InfoWidgetEngine::handleLineError
 */
void InfoWidgetEngine::handleLineError(
        int operation, int errorCode)
{
    DPRINT << ": operation: " << operation << " error code: " << errorCode; 
}

/*!
   InfoWidgetEngine::preferenceChanged
 */
void InfoWidgetEngine::preferenceChanged(
        int option, int displaySetting)
{
    DPRINT << "option: " << option << " displaySetting: " << displaySetting;
    switch(option){
    case InfoWidgetPreferences::DisplayMcn:
        if (displaySetting == InfoWidgetPreferences::DisplayOn) {
            m_networkHandler->enableMcn();
        } else {
            m_networkHandler->disableMcn();
        }   
        break; 
    case InfoWidgetPreferences::DisplaySatText:
        m_satHandler->connect(displaySetting);
        break;
    default:
        break;
    }
    DPRINT << ": OUT";
}

/*!
   InfoWidgetEngine::suspend
   
   Called when widget is deactivated 
   and widget should suspend all 
   possible activities 
 */
void InfoWidgetEngine::suspend() 
{
    DPRINT;
    m_networkHandler->suspend(); 
}

/*!
   InfoWidgetEngine::preferenceChanged
   
   Called when widget is activated 
   and widget can resume activities
 */
void InfoWidgetEngine::resume()
{
    DPRINT;
    m_networkHandler->resume(); 
}


// End of File. 

