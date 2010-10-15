/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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


bool m_EPhoneViewPlayHandsFreeActivatedTone_called;

#include <QDebug>
#include <xqsystemtoneservice.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// XQSystemToneService::XQSystemToneService
// -----------------------------------------------------------------------------
//
XQSystemToneService::XQSystemToneService( 
        QObject * parent )
    :
    QObject( parent)
    {
    
    }

// -----------------------------------------------------------------------------
// XQSystemToneService::~XQSystemToneService
// -----------------------------------------------------------------------------
//
XQSystemToneService::~XQSystemToneService(  )
    {
    m_EPhoneViewPlayHandsFreeActivatedTone_called = false;
    }


// -----------------------------------------------------------------------------
// XQSystemToneService::playTone
// -----------------------------------------------------------------------------
//
void XQSystemToneService::playTone( 
        ToneType toneType )
    {
    Q_UNUSED(toneType);
    m_EPhoneViewPlayHandsFreeActivatedTone_called = true;
    }


// -----------------------------------------------------------------------------
// XQSystemToneService::playAlarm
// -----------------------------------------------------------------------------
//
void XQSystemToneService::playAlarm( 
        AlarmType alarmType,
        unsigned int & contextId )
    {
    Q_UNUSED(alarmType);
    Q_UNUSED(contextId);
    }


// -----------------------------------------------------------------------------
// XQSystemToneService::stopAlarm
// -----------------------------------------------------------------------------
//
void XQSystemToneService::stopAlarm( 
        unsigned int contextId )
    {
    Q_UNUSED(contextId);
    }


// -----------------------------------------------------------------------------
// XQSystemToneService::playAlarm
// -----------------------------------------------------------------------------
//
void XQSystemToneService::playAlarm( 
        ToneType toneType,
        unsigned int & contextId )
    {
    Q_UNUSED(toneType);
    Q_UNUSED(contextId);
    }


// -----------------------------------------------------------------------------
// XQSystemToneService::toneStarted
// -----------------------------------------------------------------------------
//
void XQSystemToneService::toneStarted(  )
    {

    }


// -----------------------------------------------------------------------------
// XQSystemToneService::alarmStarted
// -----------------------------------------------------------------------------
//
void XQSystemToneService::alarmStarted( 
        unsigned int contextId )
    {
    Q_UNUSED(contextId);
    }


// -----------------------------------------------------------------------------
// XQSystemToneService::alarmFinished
// -----------------------------------------------------------------------------
//
void XQSystemToneService::alarmFinished( 
        unsigned int contextId )
    {
    Q_UNUSED(contextId);
    }


