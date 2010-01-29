/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Converts asynchronous call to synchronous.
*
*/


#include "cphcntasynctosync.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntAsyncToSync::CPhCntAsyncToSync()
    {
    }


// ---------------------------------------------------------------------------
// Second phase constructor.
// ---------------------------------------------------------------------------
//
void CPhCntAsyncToSync::BaseConstructL()
    {
    iWait = new( ELeave )CActiveSchedulerWait();
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntAsyncToSync::~CPhCntAsyncToSync()
    {
    delete iWait;
    }

// ---------------------------------------------------------------------------
// Converts asyncrhonous call to synchronous.
// ---------------------------------------------------------------------------
//
TInt CPhCntAsyncToSync::MakeAsyncRequest()
    {
    iResponseReceived = EFalse;
	// Return KErrInUse, if iWait is active.
	TInt err( IsActive() ? KErrInUse : KErrNone );
	if ( !err )
		{
		TRAP( err, DoMakeAsyncRequestL() );
    	}
	if( !err )
        {
        // Check that response is not already received, we cannot 
        // start the scheduler if response is received, otherwise
        // scheduler waits forever.
        if( !iResponseReceived )
            {
            iWait->Start();
            }    
        err = iResponseError;
        }
    return err;
    }
    
// ---------------------------------------------------------------------------
// Indication that response has been received from async request.
// ---------------------------------------------------------------------------
//
void CPhCntAsyncToSync::ResponseReceived( TInt aErrorCode )
    {
    iResponseReceived = ETrue;
    iResponseError = aErrorCode;
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }

// ---------------------------------------------------------------------------
// Checks if there are pending asynchronous requests available.
// ---------------------------------------------------------------------------
//
TBool CPhCntAsyncToSync::IsActive()
    {
    return iWait->IsStarted();
    }
    
