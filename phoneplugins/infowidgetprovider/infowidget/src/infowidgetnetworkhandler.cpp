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
#include "infowidgetnetworkhandler.h"
#include "infowidgetlogging.h"


/*!
 * InfoWidgetNetworkHandler::InfoWidgetNetworkHandler
 */
InfoWidgetNetworkHandler::InfoWidgetNetworkHandler(QObject *parent) 
    : 
    QObject(parent), 
    m_nwSession(0)
{
    DPRINT << ": IN";
    
    CNWSession *session = 0;
    QT_TRAP_THROWING(session = CreateL(*this, m_nwInfo));
    m_nwSession.reset(session);
    
    DPRINT << ": OUT";
}

/*!
 * InfoWidgetNetworkHandler::~InfoWidgetNetworkHandler
 */
InfoWidgetNetworkHandler::~InfoWidgetNetworkHandler()
{
    DPRINT << ": IN";
    
    DPRINT << ": OUT";
}    

/*!
 * InfoWidgetNetworkHandler::homeZoneTextTag
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
 * InfoWidgetNetworkHandler::homeZoneIndicatorType
 */
int InfoWidgetNetworkHandler::homeZoneIndicatorType() const 
{
    DPRINT;
    int indicatorType = m_nwInfo.iViagIndicatorType;
    DPRINT << ": homeZoneIndicatorType: " << indicatorType;
    
    return indicatorType; 
}

/*!
 * InfoWidgetNetworkHandler::mcnName
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
 * InfoWidgetNetworkHandler::mcnIndicatorType
 */
int InfoWidgetNetworkHandler::mcnIndicatorType() const 
{
    DPRINT;
    int indicatorType = m_nwInfo.iMCNIndicatorType;
    DPRINT << ": mcnIndicatorType: " << indicatorType;
    
    return indicatorType; 
}

/*!
 * InfoWidgetNetworkHandler::logCurrentInfo
 */
void InfoWidgetNetworkHandler::logCurrentInfo()
{
    DPRINT << ": IN"; 
    
    DPRINT << "iRegistrationStatus " << m_nwInfo.iRegistrationStatus;
    DPRINT << "iNetworkMode " << m_nwInfo.iNetworkMode;      
    DPRINT << "iStatus " << m_nwInfo.iStatus;           
    QString qBuf; 
    
    qBuf = QString((QChar*)m_nwInfo.iCountryCode.Ptr(),
        m_nwInfo.iCountryCode.Length());
    DPRINT << "iCountryCode " << qBuf;       
    
    qBuf = QString((QChar*)m_nwInfo.iNetworkId.Ptr(),
        m_nwInfo.iNetworkId.Length());
    DPRINT << "iNetworkId " << qBuf; 
    
    qBuf = QString((QChar*)m_nwInfo.iDisplayTag.Ptr(),
        m_nwInfo.iDisplayTag.Length());
    DPRINT << "iDisplayTag " << qBuf;
    
    qBuf = QString((QChar*)m_nwInfo.iShortName.Ptr(),
        m_nwInfo.iShortName.Length());
    DPRINT << "iShortName " << qBuf;       
    
    qBuf = QString((QChar*)m_nwInfo.iLongName.Ptr(),
        m_nwInfo.iLongName.Length());
    DPRINT << "iLongName " << qBuf;
    
    DPRINT << "iViagIndicatorType " << m_nwInfo.iViagIndicatorType; 

    qBuf = QString((QChar*)m_nwInfo.iViagTextTag.Ptr(),
        m_nwInfo.iViagTextTag.Length());
    DPRINT << "iViagTextTag " << qBuf;
    
    DPRINT << "iMCNIndicatorType " << m_nwInfo.iMCNIndicatorType;  
    
    qBuf = QString((QChar*)m_nwInfo.iMCNName.Ptr(),
        m_nwInfo.iMCNName.Length());
    DPRINT << "iMCNName " << qBuf;
    
    qBuf = QString((QChar*)m_nwInfo.iSPName.Ptr(),m_nwInfo.iSPName.Length());
    DPRINT << "iSPName " << qBuf;
    
    DPRINT << "iServiceProviderNameDisplayReq " 
        << m_nwInfo.iServiceProviderNameDisplayReq;
    
    qBuf = QString((QChar*)m_nwInfo.iNPName.Ptr(), m_nwInfo.iNPName.Length());
    DPRINT << "iNPName " << qBuf;
    
    qBuf = QString((QChar*)m_nwInfo.iOperatorNameInfo.iName.Ptr(),
        m_nwInfo.iOperatorNameInfo.iName.Length());
    DPRINT << "iOperatorNameInfo iName" << qBuf;
    DPRINT << "iOperatorNameInfo iType" << m_nwInfo.iOperatorNameInfo.iType;
    
    DPRINT << "iNoServerMode " << m_nwInfo.iNoServerMode;
    
    DPRINT << "iVoicePrivacyStatus " << m_nwInfo.iVoicePrivacyStatus;
    
    qBuf = QString((QChar*)m_nwInfo.iPLMNField.Ptr(),
        m_nwInfo.iPLMNField.Length());
    DPRINT << "iPLMNField " << qBuf;                    
    
    DPRINT << ": OUT";
} 

/*!
 * InfoWidgetNetworkHandler::HandleNetworkMessage
 */
void InfoWidgetNetworkHandler::HandleNetworkMessage( 
    const TNWMessages aMessage )
{   
    DPRINT << ": message: " << aMessage;
    
    bool acceptedMessage = false;
    switch (aMessage) {
        case ENWMessageCurrentCellInfoMessage:
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
 * InfoWidgetNetworkHandler::HandleNetworkError
 */
void InfoWidgetNetworkHandler::HandleNetworkError( 
    const TNWOperation aOperation, 
    TInt aErrorCode )
{
    DPRINT << ": operation: " << aOperation <<  
        " error code: " << aErrorCode;
    
    int result = 0;
    QT_TRYCATCH_ERROR(result, emit networkError( 
        static_cast<int>(aOperation), static_cast<int>(aErrorCode)));
    if (0 != result) {
        DPRINT << ": Exception occured while emitting signal:" << result;
    }
}

// End of File. 
