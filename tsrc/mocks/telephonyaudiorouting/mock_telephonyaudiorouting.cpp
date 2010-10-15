/** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/
#include <smcmockclassincludes.h>
#include <telephonyaudiorouting.h>
#include <mtelephonyaudioroutingobserver.h>
#include <TelephonyAudioRoutingProxy.h>


// ============================ MEMBER FUNCTIONS ===============================

EXPORT_C CTelephonyAudioRouting* CTelephonyAudioRouting::NewL( 
        MTelephonyAudioRoutingObserver& aObserver )
    {
    CTelephonyAudioRoutingProxy* audioRoutingProxy = 
            CTelephonyAudioRoutingProxy::NewL(aObserver);
    return audioRoutingProxy;
    }

// -----------------------------------------------------------------------------
// CTelephonyAudioRouting::AvailableOutputs
// -----------------------------------------------------------------------------
//
const TArray<CTelephonyAudioRouting::TAudioOutput>& CTelephonyAudioRouting::AvailableOutputs( )

//const TArray<TAudioOutput>& CTelephonyAudioRouting::AvailableOutputs(  )
    {
    SMC_MOCK_METHOD0( const TArray <CTelephonyAudioRouting::TAudioOutput > & )
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRouting::Output
// -----------------------------------------------------------------------------
//
CTelephonyAudioRouting::TAudioOutput CTelephonyAudioRouting::Output(  )
    {
    SMC_MOCK_METHOD0( TAudioOutput )
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRouting::SetOutputL
// -----------------------------------------------------------------------------
//
void CTelephonyAudioRouting::SetOutputL( 
        TAudioOutput aOutput )
    {
    SMC_MOCK_METHOD1( void, TAudioOutput, aOutput )
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRouting::PreviousOutput
// -----------------------------------------------------------------------------
//
CTelephonyAudioRouting::TAudioOutput CTelephonyAudioRouting::PreviousOutput(  )
    {
    SMC_MOCK_METHOD0( TAudioOutput )
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRouting::SetShowNote
// -----------------------------------------------------------------------------
//
TInt CTelephonyAudioRouting::SetShowNote( 
        TBool aMode )
    {
    SMC_MOCK_METHOD1( TInt, TBool, aMode )
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRouting::GetShowNote
// -----------------------------------------------------------------------------
//
TInt CTelephonyAudioRouting::GetShowNote( 
        TBool & aMode )
    {
    SMC_MOCK_METHOD1( TInt, TBool &, aMode )
    }


