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
#include <TelephonyAudioRoutingProxy.h>
#include <TelephonyAudioRouting.h>
#include <MTelephonyAudioRoutingObserver.h>
#include "telephonyAudioRoutingSession.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTelephonyAudioRoutingProxy::CTelephonyAudioRoutingProxy
// -----------------------------------------------------------------------------
//
CTelephonyAudioRoutingProxy::CTelephonyAudioRoutingProxy( 
        MTelephonyAudioRoutingObserver & aObserver )
    	:iObserver(aObserver),
	iAvailableOutputs(0, NULL, NULL)
    //CTelephonyAudioRouting( /*aObserver*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRoutingProxy::ConstructL
// -----------------------------------------------------------------------------
//
void CTelephonyAudioRoutingProxy::ConstructL(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRoutingProxy::NewL
// -----------------------------------------------------------------------------
//
CTelephonyAudioRoutingProxy * CTelephonyAudioRoutingProxy::NewL( 
        MTelephonyAudioRoutingObserver & aObserver )
    {
    SMC_MOCK_NEWL1( CTelephonyAudioRoutingProxy *, MTelephonyAudioRoutingObserver &, aObserver )

    CTelephonyAudioRoutingProxy* self = new( ELeave ) CTelephonyAudioRoutingProxy(aObserver);
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRoutingProxy::~CTelephonyAudioRoutingProxy
// -----------------------------------------------------------------------------
//
CTelephonyAudioRoutingProxy::~CTelephonyAudioRoutingProxy(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRoutingProxy::AvailableOutputs
// -----------------------------------------------------------------------------
//
const TArray <CTelephonyAudioRouting::TAudioOutput > & CTelephonyAudioRoutingProxy::AvailableOutputs(  )
    {
    SMC_MOCK_METHOD0( const TArray <TAudioOutput > & )
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRoutingProxy::Output
// -----------------------------------------------------------------------------
//
CTelephonyAudioRouting::TAudioOutput CTelephonyAudioRoutingProxy::Output(  )
    {
    SMC_MOCK_METHOD0( TAudioOutput )
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRoutingProxy::SetOutputL
// -----------------------------------------------------------------------------
//
void CTelephonyAudioRoutingProxy::SetOutputL( 
        TAudioOutput aOutput )
    {
    SMC_MOCK_METHOD1( void, TAudioOutput, aOutput )
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRoutingProxy::PreviousOutput
// -----------------------------------------------------------------------------
//
CTelephonyAudioRouting::TAudioOutput CTelephonyAudioRoutingProxy::PreviousOutput(  )
    {
    SMC_MOCK_METHOD0( TAudioOutput )
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRoutingProxy::SetShowNote
// -----------------------------------------------------------------------------
//
TInt CTelephonyAudioRoutingProxy::SetShowNote( 
        TBool aMode )
    {
    SMC_MOCK_METHOD1( TInt, TBool, aMode )
    }


// -----------------------------------------------------------------------------
// CTelephonyAudioRoutingProxy::GetShowNote
// -----------------------------------------------------------------------------
//
TInt CTelephonyAudioRoutingProxy::GetShowNote( 
        TBool & aMode )
    {
    if( aMode != ETrue || aMode != EFalse )
        {
        aMode = EFalse;
        }
  
    SMC_MOCK_METHOD1( TInt, TBool &, aMode )
    }


