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


#include "infowidgetsathandler.h"
#include "infowidgetsathandlerprivate.h"
#include "infowidgetlogging.h"
#include <rsatservice.h>

/*!
  \class InfoWidgetSatHandler
  \brief Handles SIM Application Toolkit specific functionality of 
         Operator info widget
*/

/*!
 */
InfoWidgetSatHandler::InfoWidgetSatHandler(QObject *parent):
    QObject(parent) , m_connected(false)
{
    DPRINT << ": IN";
   
    d_ptr = new InfoWidgetSatHandlerPrivate(this, m_satService);
    
    DPRINT << ": OUT";
}

/*!
 */
InfoWidgetSatHandler::~InfoWidgetSatHandler()
{
    DPRINT << ": IN";
    delete d_ptr;
    DPRINT << ": OUT";
} 

/*!
   InfoWidgetSatHandler::connect
 */
void InfoWidgetSatHandler::connect(bool connect)
{
    DPRINT << ": IN : connected = " << m_connected << " : connect = " << connect;
    if(connect && !m_connected){
        DPRINT << "connect and startObserving() ";
        m_connected = d_ptr->connect();
        if(m_connected){
            d_ptr->startObserving();
        }
    }else if (!connect && m_connected){
        DPRINT << "disconnect and stopObserving() ";
        m_connected = d_ptr->disconnect();
    }
    DPRINT << ": OUT : connected = " << m_connected;
}

/*!
   InfoWidgetSatHandler::logCurrentInfo
 */
void InfoWidgetSatHandler::logCurrentInfo()
{
    DPRINT << ": IN"; 
    DPRINT << "SAT IdleModeText: " << m_displayText;
    DPRINT << ": OUT";    
} 

/*!
   InfoWidgetSatHandler::satDisplayText
 */
const QString& InfoWidgetSatHandler::satDisplayText() const
{
    DPRINT << ": text: " << m_displayText; 
    return m_displayText;
}

/*!
   InfoWidgetSatHandler::setSatDisplayText
 */
void InfoWidgetSatHandler::setSatDisplayText(const QString& displayText)
{
    DPRINT << ": display text: " << displayText;
    m_displayText = displayText;
}

/*!
   InfoWidgetSatHandler::handleIdleModeTxtMessage
 */
void InfoWidgetSatHandler::handleIdleModeTxtMessage( int idleResult )
{
    DPRINT << ": handleIdleModeTxtMessage: " << idleResult;
    if(m_connected){
        m_satService.SetIdleModeTextResponse((RSatService::TSATIdleResult)idleResult);
    }
    else{
        //TODO: What's correct response if widget isn't visible? For now it's permanen problem
        m_satService.SetIdleModeTextResponse( RSatService::ESATIdleCmdBeyondMeCapabilities );
    }
 
    //Pass the result
    emit handleMessage(idleResult);
}

/*!
   InfoWidgetSatHandler::handleSatError
 */
void InfoWidgetSatHandler::handleSatError( int operation, int errorCode )
{
    DPRINT << ": satError : operation: " << operation << ": errorCode: "<<errorCode;
    m_satService.SetIdleModeTextResponse( RSatService::ESATIdleMeUnableToProcessCmd );
    emit handleError(operation, errorCode);
}

// End of File. 

