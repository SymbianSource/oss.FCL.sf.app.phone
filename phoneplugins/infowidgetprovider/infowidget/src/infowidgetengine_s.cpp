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
#include "infowidgetlogging.h"

/*!
   InfoWidgetEngine::InfoWidgetEngine
 */
InfoWidgetEngine::InfoWidgetEngine(QObject *parent): 
    QObject(parent)
{
    DPRINT;
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
   InfoWidgetEngine::setModelData
   
   Enables setting model initial data to engine.   
 */
void InfoWidgetEngine::setModelData(const InfoWidgetEngine::ModelData &data)
{
    DPRINT; 
    m_modelData = data;
}

/*!
   InfoWidgetEngine::updateNetworkDataToModel
 */
void InfoWidgetEngine::updateNetworkDataToModel()
{
    DPRINT;
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


void InfoWidgetEngine::handleNetworkError(int operation, int errorCode)
{
    DPRINT << ": operation: " << operation << " error code: " << errorCode; 
}

void InfoWidgetEngine::handleSatError(int operation, int errorCode)
{
    DPRINT << ": operation: " << operation << " error code: " << errorCode; 
}

void InfoWidgetEngine::handleLineError(int operation, int errorCode)
{
    DPRINT << ": operation: " << operation << " error code: " << errorCode; 
}

// End of File. 

