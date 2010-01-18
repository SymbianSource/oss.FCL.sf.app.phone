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
* Description:  GPRS termination active object implementation.
*
*/



// INCLUDE FILES
#include "cpegprstermination.h"
#include <ccoutlinterface.h>
#include <talogger.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPEGprsTermination::CPEGprsTermination
// -----------------------------------------------------------------------------
//
CPEGprsTermination::CPEGprsTermination()
    : CActive( EPriorityStandard )
    {
    TEFLOGSTRING( KTAOBJECT, "cpecall: CPEGprsTermination::CPEGprsTermination" );
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CPEGprsTermination::CPEGprsTermination
// -----------------------------------------------------------------------------
//
CPEGprsTermination::~CPEGprsTermination()
    {
    TEFLOGSTRING( KTAOBJECT, "cpecall: CPEGprsTermination::~CPEGprsTermination" );
    Cancel();
    delete iInterface;
    }

// -----------------------------------------------------------------------------
// CPEGprsTermination::StartL
// -----------------------------------------------------------------------------
//
void CPEGprsTermination::StartL()
    {
    TEFLOGSTRING( KTAINT, "cpecall: CPEGprsTermination::StartL" );

    Cancel();
    delete iInterface;
    iInterface = NULL;

    iInterface = CCoUtlInterface::NewL();
    TEFLOGSTRING( KTAREQOUT, "cpecall: CPEGprsTermination::StartL CCoUtlInterface::Terminate()" );
    iInterface->Terminate( iStatus );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CPEGprsTermination::IsTerminating
// -----------------------------------------------------------------------------
//
TBool CPEGprsTermination::IsTerminating() const
    {
    TEFLOGSTRING( KTAINT, "cpecall: CPEGprsTermination::IsTerminating" );
    if ( !iInterface )
        {
        return EFalse;
        }
    else
        {
        CCoUtlInterface::TState current = 
            iInterface->CurrentState();
            
        return 
            ( current != CCoUtlInterface::EIdle ) &&
            ( current != CCoUtlInterface::EConfirm );
        }
    }

// -----------------------------------------------------------------------------
// CPEGprsTermination::RunL
// -----------------------------------------------------------------------------
//
void CPEGprsTermination::RunL()
    {
    TEFLOGSTRING2( KTAREQEND, "cpecall: CPEGprsTermination::RunL status: %d", iStatus.Int() );
    delete iInterface;
    iInterface = NULL;
    }

// -----------------------------------------------------------------------------
// CPEGprsTermination::DoCancel
// -----------------------------------------------------------------------------
//
void CPEGprsTermination::DoCancel()
    {
    TEFLOGSTRING( KTAREQEND, "cpecall: CPEGprsTermination::DoCancel" );
    if( iInterface )
        {
        iInterface->Cancel();
        }
    }

//  End of File  
