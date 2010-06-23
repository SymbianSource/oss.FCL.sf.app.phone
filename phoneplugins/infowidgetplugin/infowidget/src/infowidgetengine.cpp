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
#include "infowidgetengine.h"
#include "infowidgetnetworkhandler.h"
#include "infowidgetsathandler.h"
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
    
    // Connect network handler signals 
    QObject::connect(
        m_networkHandler.data(), SIGNAL(networkError(int, int)),
        this, SLOT(handleNetworkError(int, int))); 
    QObject::connect(
        m_networkHandler.data(), SIGNAL(networkDataChanged()),
        this, SLOT(updateNetworkDataToModel()));

    // Connect SAT handler signals 
    QObject::connect(m_satHandler.data(), 
            SIGNAL(handleError(int, int)),
                this, SLOT(handleSatError(int, int))); 
    QObject::connect(m_satHandler.data(), 
            SIGNAL(handleMessage(int)),
                this, SLOT(updateSatDataToModel())); 

    // Update initial model data
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
   
   Updates Network Handler's network data to model. 
 */
void InfoWidgetEngine::updateNetworkDataToModel()
{
    DPRINT << ": IN"; 
    
    if (m_networkHandler->sessionExists() && 
        m_networkHandler->isOnline()) {
    
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
   
   Updates SAT handler's SAT data to model.
 */
void InfoWidgetEngine::updateSatDataToModel()
{
    DPRINT;
    if (m_satHandler) {
        m_modelData.setSatDisplayText(
                m_satHandler->satDisplayText());
        
        emit modelChanged(); 
    } 
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
    DWARNING << ": operation: " << operation << 
            " error code: " << errorCode; 
}

/*!
   InfoWidgetEngine::handleSatError
 */
void InfoWidgetEngine::handleSatError(
        int operation, int errorCode)
{
    DWARNING << ": operation: " << operation << 
            " error code: " << errorCode; 
}

/*!
   InfoWidgetEngine::handleLineError
 */
void InfoWidgetEngine::handleLineError(
        int operation, int errorCode)
{
    DWARNING << ": operation: " << 
            operation << " error code: " << errorCode; 
}

/*!
   InfoWidgetEngine::handlePreferencesChanged
 */
void InfoWidgetEngine::handlePreferencesChanged(
        InfoWidgetPreferences::Options options)
{
    DPRINT; 
    if (options.testFlag(InfoWidgetPreferences::DisplayMcn)){
            m_networkHandler->enableMcn(); 
        } else {
            m_networkHandler->disableMcn();
        }

    if (options.testFlag(InfoWidgetPreferences::DisplaySatText)){
            m_satHandler->connect(true);
        } else {
            m_satHandler->connect(false);
        }
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

