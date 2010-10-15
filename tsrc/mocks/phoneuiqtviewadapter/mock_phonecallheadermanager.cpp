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
#include <QDebug>
#include <smcmockclassincludes.h>
#include <mpeengineinfo.h>
#include "phonecallheadermanager.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::PhoneCallHeaderManager
// -----------------------------------------------------------------------------
//
PhoneCallHeaderManager::PhoneCallHeaderManager( 
        PhoneBubbleWrapper & bubbleWrapper,
        PhoneUIQtViewIF & view,
        QObject * parent ) :
        QObject( parent ),
        m_bubbleWrapper(bubbleWrapper),
        m_view(view)
    {
    
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::~PhoneCallHeaderManager
// -----------------------------------------------------------------------------
//
PhoneCallHeaderManager::~PhoneCallHeaderManager(  )
    {
    
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::setEngineInfo
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderManager::setEngineInfo( 
        MPEEngineInfo * engineInfo )
    {
    SMC_MOCK_METHOD1( void, MPEEngineInfo *, engineInfo )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::createCallHeader
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderManager::createCallHeader( 
        int callId )
    {
    SMC_MOCK_METHOD1( void, int, callId )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::createEmergencyCallHeader
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderManager::createEmergencyCallHeader( 
        int callId )
    {
    SMC_MOCK_METHOD1( void, int, callId )
    }

// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::handleCipheringInfoChange
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderManager::handleCipheringInfoChange( 
        int callId )
    {
    SMC_MOCK_METHOD1( void, int, callId )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::createConferenceBubble
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderManager::createConferenceBubble( 
        int callId )
    {
    SMC_MOCK_METHOD1( void, int, callId )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::removeConferenceBubble
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderManager::removeConferenceBubble(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::removeCallFromConference
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderManager::removeCallFromConference( 
        int callId )
    {
    SMC_MOCK_METHOD1( void, int, callId )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::setPrivateFromConference
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderManager::setPrivateFromConference( 
        int callId )
    {
    SMC_MOCK_METHOD1( void, int, callId )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::setExpandedConferenceCallHeader
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderManager::setExpandedConferenceCallHeader(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderManager::removeAllCallHeaders
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderManager::removeAllCallHeaders(  )
    {
    SMC_MOCK_METHOD0( void )
    }



