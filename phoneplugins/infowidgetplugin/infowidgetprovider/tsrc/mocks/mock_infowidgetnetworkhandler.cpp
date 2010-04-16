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
#include <QDebug>
#include <smcmockclassincludes.h>
#include <networkhandlingproxy.h>
#include <cnwsession.h>
#include "infowidgetnetworkhandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// InfoWidgetNetworkHandler::InfoWidgetNetworkHandler
// -----------------------------------------------------------------------------
//
InfoWidgetNetworkHandler::InfoWidgetNetworkHandler( 
        QObject * parent )
    : 
    QObject(parent), 
    m_nwSession(0)
    {
    
    }


// -----------------------------------------------------------------------------
// InfoWidgetNetworkHandler::~InfoWidgetNetworkHandler
// -----------------------------------------------------------------------------
//
InfoWidgetNetworkHandler::~InfoWidgetNetworkHandler(  )
    {
    
    }


// -----------------------------------------------------------------------------
// InfoWidgetNetworkHandler::homeZoneTextTag
// -----------------------------------------------------------------------------
//
QString InfoWidgetNetworkHandler::homeZoneTextTag(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// InfoWidgetNetworkHandler::homeZoneIndicatorType
// -----------------------------------------------------------------------------
//
int InfoWidgetNetworkHandler::homeZoneIndicatorType(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// InfoWidgetNetworkHandler::mcnName
// -----------------------------------------------------------------------------
//
QString InfoWidgetNetworkHandler::mcnName(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// InfoWidgetNetworkHandler::mcnIndicatorType
// -----------------------------------------------------------------------------
//
int InfoWidgetNetworkHandler::mcnIndicatorType(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// InfoWidgetNetworkHandler::logCurrentInfo
// -----------------------------------------------------------------------------
//
void InfoWidgetNetworkHandler::logCurrentInfo(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// InfoWidgetNetworkHandler::HandleNetworkMessage
// -----------------------------------------------------------------------------
//
void InfoWidgetNetworkHandler::HandleNetworkMessage( 
        const TNWMessages aMessage )
    {
    SMC_MOCK_METHOD1( void, const TNWMessages, aMessage )
    }


// -----------------------------------------------------------------------------
// InfoWidgetNetworkHandler::HandleNetworkError
// -----------------------------------------------------------------------------
//
void InfoWidgetNetworkHandler::HandleNetworkError( 
        const TNWOperation aOperation,
        TInt aErrorCode )
    {
    SMC_MOCK_METHOD2( void, const TNWOperation, aOperation, 
        TInt, aErrorCode )
    }