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
#include <cnetworklistener.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CNetworkListener::CNetworkListener
// -----------------------------------------------------------------------------
//
CNetworkListener::CNetworkListener( 
        MNetworkListenerObserver & aObserver ) :
    iRegistered( ETrue ),
    iObserver( aObserver )
    {
    
    }


// -----------------------------------------------------------------------------
// CNetworkListener::ConstructL
// -----------------------------------------------------------------------------
//
void CNetworkListener::ConstructL(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CNetworkListener::NewL
// -----------------------------------------------------------------------------
//
CNetworkListener * CNetworkListener::NewL( 
        MNetworkListenerObserver & aObserver )
    {
    //SMC_MOCK_NEWL1( CNetworkListener *, MNetworkListenerObserver &, aObserver )

    CNetworkListener* self = new( ELeave ) CNetworkListener( aObserver );
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CNetworkListener::~CNetworkListener
// -----------------------------------------------------------------------------
//
CNetworkListener::~CNetworkListener(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CNetworkListener::HandleNetworkMessage
// -----------------------------------------------------------------------------
//
void CNetworkListener::HandleNetworkMessage( 
        const TNWMessages aMessage )
    {
    SMC_MOCK_METHOD1( void, const TNWMessages, aMessage )
    }


// -----------------------------------------------------------------------------
// CNetworkListener::HandleNetworkError
// -----------------------------------------------------------------------------
//
void CNetworkListener::HandleNetworkError( 
        const TNWOperation aOperation,
        TInt aErrorCode )
    {
    SMC_MOCK_METHOD2( void, const TNWOperation, aOperation, 
        TInt, aErrorCode )
    }


// -----------------------------------------------------------------------------
// CNetworkListener::NWLostDelayCallBack
// -----------------------------------------------------------------------------
//
TInt CNetworkListener::NWLostDelayCallBack( 
        TAny * aParam )
    {
    //SMC_MOCK_METHOD1( TInt, TAny *, aParam )
    }


