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
#include "phonecallheaderutil.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::PhoneCallHeaderUtil
// -----------------------------------------------------------------------------
//
PhoneCallHeaderUtil::PhoneCallHeaderUtil( 
        MPEEngineInfo & engineInfo )
    : m_engineInfo(engineInfo)
    {
    
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::~PhoneCallHeaderUtil
// -----------------------------------------------------------------------------
//
PhoneCallHeaderUtil::~PhoneCallHeaderUtil(  )
    {
    
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::SetIncomingCallHeaderParams
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::SetIncomingCallHeaderParams( 
        const TInt aCallId,
        const TBool aWaitingCall,
        const TBool aVideoCall,
        TPhoneCmdParamCallHeaderData * aCallHeaderData )
    {
    SMC_MOCK_METHOD4( void, const TInt, aCallId, 
        const TBool, aWaitingCall, 
        const TBool, aVideoCall, 
        TPhoneCmdParamCallHeaderData *, aCallHeaderData )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::SetOutgoingCallHeaderParams
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::SetOutgoingCallHeaderParams( 
        const TInt aCallId,
        TPhoneCmdParamCallHeaderData * aCallHeaderData )
    {
    SMC_MOCK_METHOD2( void, const TInt, aCallId, 
        TPhoneCmdParamCallHeaderData *, aCallHeaderData )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::UpdateCallHeaderInfo
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::UpdateCallHeaderInfo( 
        const TInt aCallId,
        const TBool aWaitingCall,
        const TBool aVideoCall,
        TPhoneCmdParamCallHeaderData * aCallHeaderData )
    {
    SMC_MOCK_METHOD4( void, const TInt, aCallId, 
        const TBool, aWaitingCall, 
        const TBool, aVideoCall, 
        TPhoneCmdParamCallHeaderData *, aCallHeaderData )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::SetCallHeaderType
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::SetCallHeaderType( 
        TInt aCallHeaderType )
    {
    SMC_MOCK_METHOD1( void, TInt, aCallHeaderType )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::CallHeaderType
// -----------------------------------------------------------------------------
//
TInt PhoneCallHeaderUtil::CallHeaderType(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::SetDivertIndication
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::SetDivertIndication( 
        const TBool aDivertIndication )
    {
    SMC_MOCK_METHOD1( void, const TBool, aDivertIndication )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::GetRemoteInfoData
// -----------------------------------------------------------------------------
//
TBool PhoneCallHeaderUtil::GetRemoteInfoData( 
        const TInt aCallId,
        TDes & aData ) const
    {
    SMC_MOCK_METHOD2( TBool, const TInt, aCallId, 
        TDes &, aData )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::SetCallHeaderTexts
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::SetCallHeaderTexts( 
        const TInt aCallId,
        const TBool aWaitingCall,
        const TBool aVideoCall,
        TPhoneCmdParamCallHeaderData * aCallHeaderData )
    {
    SMC_MOCK_METHOD4( void, const TInt, aCallId, 
        const TBool, aWaitingCall, 
        const TBool, aVideoCall, 
        TPhoneCmdParamCallHeaderData *, aCallHeaderData )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::LabelText
// -----------------------------------------------------------------------------
//
const TDesC & PhoneCallHeaderUtil::LabelText( 
        TInt aCallId )
    {
    SMC_MOCK_METHOD1( const TDesC &, TInt, aCallId )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::EmergencyHeaderText
// -----------------------------------------------------------------------------
//
const TDesC & PhoneCallHeaderUtil::EmergencyHeaderText(  )
    {
    SMC_MOCK_METHOD0( const TDesC & )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::AttemptingEmergencyText
// -----------------------------------------------------------------------------
//
const TDesC & PhoneCallHeaderUtil::AttemptingEmergencyText(  )
    {
    SMC_MOCK_METHOD0( const TDesC & )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::CallState
// -----------------------------------------------------------------------------
//
TInt PhoneCallHeaderUtil::CallState( 
        TInt aCallId ) const
    {
    SMC_MOCK_METHOD1( TInt, TInt, aCallId )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::LoadResource
// -----------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::LoadResource( 
        TDes & aData,
        const TInt aResource ) const
    {
    SMC_MOCK_METHOD2( void, TDes &, aData, 
        const TInt, aResource )
    }


// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::LoadResource
// -----------------------------------------------------------------------------
//
HBufC * PhoneCallHeaderUtil::LoadResource( 
        const TInt aResource ) const
    {
    SMC_MOCK_METHOD1( HBufC *, const TInt, aResource )
    }


