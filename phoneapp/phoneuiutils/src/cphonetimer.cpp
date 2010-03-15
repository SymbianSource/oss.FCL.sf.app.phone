/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*   See header.
*
*/


// INCLUDE FILES
#include "cphonetimer.h"
#include "phoneui.pan"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPhoneTimer::CPhoneTimer
// ---------------------------------------------------------
//
CPhoneTimer::CPhoneTimer( TInt aPriority ) :
    CTimer( aPriority)
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------
// CPhoneTimer::ConstructL
// ---------------------------------------------------------
//
void CPhoneTimer::ConstructL()
    {
    CTimer::ConstructL();
    }

// ---------------------------------------------------------
// CPhoneTimer::NewL
// ---------------------------------------------------------
//
EXPORT_C CPhoneTimer* CPhoneTimer::NewL( TInt aPriority )
    {
    CPhoneTimer* self = new (ELeave) CPhoneTimer( aPriority );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }
    
// ---------------------------------------------------------
// CPhoneTimer::~CPhoneTimer
// ---------------------------------------------------------
//
EXPORT_C CPhoneTimer::~CPhoneTimer()
    {
    Cancel();    
    }

// ---------------------------------------------------------
// CPhoneTimer::RunL()
// ---------------------------------------------------------
//
void CPhoneTimer::RunL()
    {
    if ( iStatus != KErrNone )
        {
        //error code is ignored, as CPeriodic. 
        return;
        }
   
    if ( !iTimerObserver )
        {
        iCallBack.CallBack();
        }
    else
        {
        iTimerObserver->HandleTimeOutL();
        }
    }

// ---------------------------------------------------------
// CPhoneTimer::After()
// ---------------------------------------------------------
//
EXPORT_C void CPhoneTimer::After( 
    TTimeIntervalMicroSeconds32 anInterval, 
    TCallBack aCallBack )
    {
    
    if ( IsActive() )
        {
        Cancel();
        }
    iTimerObserver = NULL;
    iCallBack = aCallBack;
    CTimer::After( anInterval );
    }

// ---------------------------------------------------------
// CPhoneTimer::After()
// ---------------------------------------------------------
//

EXPORT_C void CPhoneTimer::After( 
    TTimeIntervalMicroSeconds32 anInterval, 
    MPhoneTimer* aObserver )
    {
    
    __ASSERT_DEBUG( aObserver, Panic( EPhoneUtilsParameterNotInitialized ) );
    
    if ( IsActive() )
        {
        Cancel();
        }
    iTimerObserver = aObserver;
    CTimer::After( anInterval );
    }

// ---------------------------------------------------------
//  CPhoneTimer::CancelTimer
// ---------------------------------------------------------
//
EXPORT_C void CPhoneTimer::CancelTimer()
    {
    Cancel();
    }

// ---------------------------------------------------------
//  CPhoneTimer::DoCancel
// ---------------------------------------------------------
//
void CPhoneTimer::DoCancel()
    {
    iTimerObserver = NULL;
    CTimer::DoCancel();
    }

//  End of File  
