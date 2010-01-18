/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CPEIdleStatusMonitor class 
*
*/


// INCLUDES
#include <talogger.h>
#include "cpephonemodel.h"
#include "cpeidlestatusmonitor.h"
#include <activeidle2domainpskeys.h>


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPEIdleStatusMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
CPEIdleStatusMonitor* CPEIdleStatusMonitor::NewL(
        CPEPhoneModel& aOwner ) 
    {
    TEFLOGSTRING( KTAOBJECT, "PE CPEIdleStatusMonitor::NewL ");
    CPEIdleStatusMonitor* self = new (ELeave) CPEIdleStatusMonitor( 
        aOwner );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return( self );
    }

// Destructor
CPEIdleStatusMonitor::~CPEIdleStatusMonitor()
    {
    TEFLOGSTRING( KTAOBJECT, "PE CPEIdleStatusMonitor::~CPEIdleStatusMonitor, start ");
    Cancel();
    iProperty.Close();
    TEFLOGSTRING( KTAOBJECT, "PE CPEIdleStatusMonitor::~CPEIdleStatusMonitor, complete ");
    }
                                       
// -----------------------------------------------------------------------------
// CPEIdleStatusMonitor::CPEIdleStatusMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPEIdleStatusMonitor::CPEIdleStatusMonitor(
        CPEPhoneModel& aOwner
        ): CActive( EPriorityStandard ),
           iOwner( aOwner )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CPEIdleStatusMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPEIdleStatusMonitor::ConstructL()
    {
    TEFLOGSTRING( KTAOBJECT, "PE CPEIdleStatusMonitor::ConstructL");
    User::LeaveIfError( iProperty.Attach( KPSUidAiInformation, KActiveIdleState ) );
       
    TInt value;
    iProperty.Get( KPSUidAiInformation, KActiveIdleState, value );
    
    if ( value == EPSAiForeground )
        {
        // Don't implement any new code that reserver anything after this.
        iOwner.ConstructContactHandlingPhaseTwoL();
        }
    else
        {
        iProperty.Subscribe( iStatus );
        SetActive();
        }
    }

// -----------------------------------------------------------------------------
// CPEIdleStatusMonitor::DoCancel
// Callback method from CActive.
// -----------------------------------------------------------------------------
//
void CPEIdleStatusMonitor::DoCancel()
    {   
    TEFLOGSTRING( KTAREQOUT, "PE CPEIdleStatusMonitor::DoCancel()" );
    iProperty.Cancel();
    }

// -----------------------------------------------------------------------------
// CPECenRepMonitor::RunL
// -----------------------------------------------------------------------------
//
void CPEIdleStatusMonitor::RunL()
    {
    TEFLOGSTRING2( KTAREQEND, 
        "PE CPEIdleStatusMonitor::RunL,iStatus: %d", iStatus.Int() );
   
    //  Now retrieve the value
    TInt value;
    iProperty.Get( KPSUidAiInformation, KActiveIdleState, value );
        
    if ( value == EPSAiForeground )
        {
        TEFLOGSTRING( KTAOBJECT, 
            "PE CPEIdleStatusMonitor::RunL, starting contact handling creation");
        // Don't implement any new code that reserver anything after this.
        iOwner.ConstructContactHandlingPhaseTwoL();
        TEFLOGSTRING( KTAOBJECT, 
            "PE CPEIdleStatusMonitor::RunL, starting contact handling creation done ");
        }
    else
        {
        iProperty.Subscribe( iStatus );
        SetActive();
        }
    }
    
// End of file
