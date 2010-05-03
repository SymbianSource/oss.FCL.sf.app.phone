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
#include <cnwsession.h>
#include <xqsettingsmanager.h>
#include <xqsettingskey.h>
#include <settingsinternalcrkeys.h>
#include "infowidgetnetworkhandler.h"
#include "infowidgetlogging.h"


/*!
  \class InfoWidgetNetworkHandler
  \brief Handles network specific functionality of 
         Operator info widget
*/

/*!
    InfoWidgetNetworkHandler::InfoWidgetNetworkHandler()
*/
InfoWidgetNetworkHandler::InfoWidgetNetworkHandler(QObject *parent) 
    : 
    QObject(parent), 
    m_nwSession(0)
{
    DPRINT << ": IN";
    
    // Initialize key
    // ToDo: need to check if preference is ON initially
    enableMcn(); 
    
    CNWSession *session = 0;
    QT_TRAP_THROWING(session = CreateL(*this, m_nwInfo));
    m_nwSession.reset(session);
    
    DPRINT << ": OUT";
}

/*!
    InfoWidgetNetworkHandler::~InfoWidgetNetworkHandler()
*/
InfoWidgetNetworkHandler::~InfoWidgetNetworkHandler()
{
    DPRINT;

    // Disable MCN setting, 
    // no display client
    disableMcn(); 
}    

/*!
    InfoWidgetNetworkHandler::suspend()
*/
void InfoWidgetNetworkHandler::suspend()
{
    DPRINT;
    disableMcn(); 
}

/*!
    InfoWidgetNetworkHandler::suspend()
*/
void InfoWidgetNetworkHandler::resume()
{
    DPRINT;
    enableMcn(); 
}

/*!
    InfoWidgetNetworkHandler::serviceProviderName()
*/
QString InfoWidgetNetworkHandler::serviceProviderName() const 
{
    DPRINT;
    QString qBuf;
    qBuf = QString((QChar*)m_nwInfo.iSPName.Ptr(), 
        m_nwInfo.iSPName.Length());
    DPRINT << ": serviceProviderName: " << qBuf;
    
    return qBuf; 
}

/*!
    InfoWidgetNetworkHandler::serviceProviderNameDisplayRequired()
*/
bool InfoWidgetNetworkHandler::serviceProviderNameDisplayRequired() const 
{
    DPRINT;
    bool displayRequired(true); 
    
    if (m_nwInfo.iServiceProviderNameDisplayReq == 
            RMobilePhone::KDisplaySPNNotRequired) {
        displayRequired = false; 
    } 
    DPRINT << ": serviceProviderNameDisplayRequired: " <<
            displayRequired;
    
    return displayRequired; 
}

/*!
    InfoWidgetNetworkHandler::homeZoneTextTag()
*/
QString InfoWidgetNetworkHandler::homeZoneTextTag() const 
{
    DPRINT;
    QString qBuf;
    qBuf = QString((QChar*)m_nwInfo.iViagTextTag.Ptr(), 
        m_nwInfo.iViagTextTag.Length());
    DPRINT << ": homeZoneTextTag: " << qBuf;
    
    return qBuf; 
}

/*!
    InfoWidgetNetworkHandler::homeZoneIndicatorType()
*/
int InfoWidgetNetworkHandler::homeZoneIndicatorType() const 
{
    DPRINT;
    int indicatorType = m_nwInfo.iViagIndicatorType;
    DPRINT << ": homeZoneIndicatorType: " << indicatorType;
    
    return indicatorType; 
}

/*!
    InfoWidgetNetworkHandler::enableMcn()
*/
void InfoWidgetNetworkHandler::enableMcn() 
{
    DPRINT;
    if (!readMcnDisplayState()) {
        DPRINT << ": enabling mcn display cenrep";
        writeMcnDisplayState(true); 
    }
}

/*!
    InfoWidgetNetworkHandler::disableMcn()
*/
void InfoWidgetNetworkHandler::disableMcn()
{
    DPRINT;
    if (readMcnDisplayState()) {
        DPRINT << ": disabling mcn display cenrep";
        writeMcnDisplayState(false); 
    }
}

/*!
    InfoWidgetNetworkHandler::mcnName()
*/
QString InfoWidgetNetworkHandler::mcnName() const 
{
    DPRINT;
    QString qBuf;
    qBuf = QString((QChar*)m_nwInfo.iMCNName.Ptr(),
        m_nwInfo.iMCNName.Length());
    DPRINT << ": mcnName: " << qBuf;
    
    return qBuf; 
}

/*!
    InfoWidgetNetworkHandler::mcnIndicatorType()
*/
int InfoWidgetNetworkHandler::mcnIndicatorType() const 
{
    DPRINT;
    int indicatorType = m_nwInfo.iMCNIndicatorType;
    DPRINT << ": mcnIndicatorType: " << indicatorType;
    
    return indicatorType; 
}


/*!
    InfoWidgetNetworkHandler::networkRegistrationStatus()
*/
int InfoWidgetNetworkHandler::networkRegistrationStatus() const 
{
    DPRINT << ": registration status: " <<
            static_cast<int>(m_nwInfo.iRegistrationStatus);
    
    return static_cast<int>(m_nwInfo.iRegistrationStatus); 
}

/*!
    InfoWidgetNetworkHandler::isOnline()
    
    Check if network registration status is registered and 
    return status accordingly 
*/
bool InfoWidgetNetworkHandler::isOnline() const
{
    bool online(false); 
    
    switch (networkRegistrationStatus()) {
        case ENWRegisteredBusy: // Fall through
        case ENWRegisteredOnHomeNetwork: // Fall through
        case ENWRegisteredRoaming: 
            online = true;
            break; 
        default: 
            break; 
    }
    
    DPRINT << ": online: " << online;
    return online; 
}

/*!
    InfoWidgetNetworkHandler::logCurrentInfo()
*/
void InfoWidgetNetworkHandler::logCurrentInfo()
{
    DPRINT << ": IN"; 
    
    QString qBuf; 
    DPRINT << "iMCNIndicatorType " << m_nwInfo.iMCNIndicatorType;  
    qBuf = QString((QChar*)m_nwInfo.iMCNName.Ptr(),
        m_nwInfo.iMCNName.Length());
    DPRINT << "iMCNName " << qBuf;
    
    qBuf = QString((QChar*)m_nwInfo.iSPName.Ptr(),m_nwInfo.iSPName.Length());
    DPRINT << "iSPName " << qBuf;
    DPRINT << "iServiceProviderNameDisplayReq " 
        << m_nwInfo.iServiceProviderNameDisplayReq;
    
    DPRINT << "iViagIndicatorType " << m_nwInfo.iViagIndicatorType; 
    qBuf = QString((QChar*)m_nwInfo.iViagTextTag.Ptr(),
        m_nwInfo.iViagTextTag.Length());
    DPRINT << "iViagTextTag " << qBuf;
    
    DPRINT << ": OUT";
} 

/*!
    InfoWidgetNetworkHandler::HandleNetworkMessage()
*/
void InfoWidgetNetworkHandler::HandleNetworkMessage( 
    const TNWMessages aMessage )
{   
    DPRINT << ": message: " << aMessage;
    
    bool acceptedMessage = false;
    switch (aMessage) {
        case ENWMessageCurrentCellInfoMessage: // Fall through 
        case ENWMessageServiceProviderNameChange: // Fall through
        case ENWMessageNetworkRegistrationStatusChange: // Fall through
            acceptedMessage = true;
            break;
        default:
            // not interesting message, filter out
            DPRINT << ": message filtered out";
            acceptedMessage = false;
    }
    
    if (acceptedMessage) {
        int result = 0;
        QT_TRYCATCH_ERROR(result, emit networkDataChanged());
        if (0 != result) {
            DPRINT << ": Exception occured while emitting signal:" << result;
        }
    }
}

/*!
    InfoWidgetNetworkHandler::HandleNetworkError()
*/
void InfoWidgetNetworkHandler::HandleNetworkError( 
    const TNWOperation aOperation, 
    TInt aErrorCode )
{
    DPRINT << ": operation: " << aOperation <<  
        " error code: " << aErrorCode;
    
    // Reset invalid data  
    switch (aOperation) {
        case MNWMessageObserver::ENWGetServiceProviderName:
            m_nwInfo.iServiceProviderNameDisplayReq = 
                    RMobilePhone::KDisplaySPNNotRequired;
            m_nwInfo.iSPName.Zero();
        break;
        default: 
            break; 
    }
    
    // Emit error signal 
    int result = 0;
    QT_TRYCATCH_ERROR(result, emit networkError( 
        static_cast<int>(aOperation), static_cast<int>(aErrorCode)));
    if (0 != result) {
        DPRINT << ": Exception occured while emitting signal:" << result;
    }
}

/*!
    InfoWidgetNetworkHandler::writeMcnDisplayState()
    
    Handler for cenrep key writing   
*/
void InfoWidgetNetworkHandler::writeMcnDisplayState(bool enabled)
{
    DPRINT << ": IN";
    
    XQSettingsManager *settingsManager = new XQSettingsManager(0); 
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, 
        KCRUidNetworkSettings.iUid, KSettingsMcnDisplay);
    int value; 
    if (enabled) {
        value = 1; 
    } else {
        value = 0; 
    }
    bool result = settingsManager->writeItemValue(settingsKey, value );
    delete settingsManager;
    
    DPRINT << ": OUT"; 
}

/*!
    InfoWidgetNetworkHandler::readMcnDisplayState()
*/
bool InfoWidgetNetworkHandler::readMcnDisplayState()
{
    DPRINT << ": IN";
    bool mcnDisplayKeyOn(false); 
    
    XQSettingsManager *settingsManager = new XQSettingsManager(0); 
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, 
        KCRUidNetworkSettings.iUid, KSettingsMcnDisplay); 
    bool conversionOk(false); 
    int value = settingsManager->readItemValue(
            settingsKey,
            XQSettingsManager::TypeInt).toInt(&conversionOk);
    
    if (conversionOk && value == 1) {
        mcnDisplayKeyOn = true; 
    }
    delete settingsManager;
        
    DPRINT << ": OUT";
    return mcnDisplayKeyOn; 
}


// End of File. 
