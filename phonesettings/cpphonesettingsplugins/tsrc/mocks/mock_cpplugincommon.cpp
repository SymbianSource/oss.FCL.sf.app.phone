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
#include "cpplugincommon.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CpSettingsWrapper::CpSettingsWrapper
// -----------------------------------------------------------------------------
//
CpSettingsWrapper::CpSettingsWrapper( 
        QObject * parent ) : QObject( parent )
    {
    
    }


// -----------------------------------------------------------------------------
// CpSettingsWrapper::~CpSettingsWrapper
// -----------------------------------------------------------------------------
//
CpSettingsWrapper::~CpSettingsWrapper(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CpSettingsWrapper::showCallDuration
// -----------------------------------------------------------------------------
//
bool CpSettingsWrapper::showCallDuration(  )
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// CpSettingsWrapper::setShowCallDuration
// -----------------------------------------------------------------------------
//
int CpSettingsWrapper::setShowCallDuration( bool value )
    {
    SMC_MOCK_METHOD1( int, bool, value )
    }


// -----------------------------------------------------------------------------
// CpSettingsWrapper::readVtVideoSending
// -----------------------------------------------------------------------------
//
int CpSettingsWrapper::readVtVideoSending(  )
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// CpSettingsWrapper::writeVtVideoSending
// -----------------------------------------------------------------------------
//
int CpSettingsWrapper::writeVtVideoSending( 
        int value )
    {
    SMC_MOCK_METHOD1( int, int, value )
    }


// -----------------------------------------------------------------------------
// CpSettingsWrapper::readSoftRejectText
// -----------------------------------------------------------------------------
//
void CpSettingsWrapper::readSoftRejectText( 
        QString & text, bool & userDefined )
    {
    //To improve coverage
    static int i=0;
    if(i==0){
        text = "Soittelen myöhemmin...";
        userDefined = true;
        i=1;
    }
    SMC_MOCK_METHOD2( void, QString &, text, bool, userDefined )
    }


// -----------------------------------------------------------------------------
// CpSettingsWrapper::writeSoftRejectText
// -----------------------------------------------------------------------------
//
int CpSettingsWrapper::writeSoftRejectText( 
        const QString & text, bool userDefined )
    {
    SMC_MOCK_METHOD2( int, const QString &, text, bool, userDefined )
    }


// -----------------------------------------------------------------------------
// CpSettingsWrapper::numberGroupingSupported
// -----------------------------------------------------------------------------
//
bool CpSettingsWrapper::numberGroupingSupported(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// CpSettingsWrapper::isFeatureCallWaitingDistiquishNotProvisionedEnabled
// -----------------------------------------------------------------------------
//
bool CpSettingsWrapper::isFeatureCallWaitingDistiquishNotProvisionedEnabled()
{
    SMC_MOCK_METHOD0( bool )
}

// -----------------------------------------------------------------------------
// CpSettingsWrapper::isPhoneOffline
// -----------------------------------------------------------------------------
//
bool CpSettingsWrapper::isPhoneOffline() const
{
    SMC_MOCK_METHOD0( bool )
}

// -----------------------------------------------------------------------------
// CpSettingsWrapper::isOngoingCall
// -----------------------------------------------------------------------------
//
bool CpSettingsWrapper::isOngoingCall() const
{
    SMC_MOCK_METHOD0( bool )
}

// -----------------------------------------------------------------------------
// Tools::voipSupported
// -----------------------------------------------------------------------------
//
bool Tools::voipSupported(  )
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// Tools::videoSupported
// -----------------------------------------------------------------------------
//
bool Tools::videoSupported(  )
    {
    SMC_MOCK_METHOD0( bool )
    }

// -----------------------------------------------------------------------------
// Tools::errorCodeTextMapping
// -----------------------------------------------------------------------------
//
bool Tools::errorCodeTextMapping( const int errorcode, QString & errorText )
    {
    SMC_MOCK_METHOD2( bool, int, errorcode, QString &, errorText )
    }
	
