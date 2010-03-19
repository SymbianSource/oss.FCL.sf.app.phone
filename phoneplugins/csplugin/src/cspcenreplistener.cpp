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
* Description:  Central Repository listening
*
*/


// INCLUDE FILES
#include "cspcenreplistener.h"
#include <centralrepository.h>
#include "csppanic.pan"
#include "mcspcenrepobserver.h"
#include "csplogger.h"


// -----------------------------------------------------------------------------
// CSPCenRepListener::CSPCenRepListener
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSPCenRepListener* CSPCenRepListener::NewL(
        TUid aUid,
        TUint32 aMonitorSetting,
        MCSPCenRepObserver* aObserver
        )
    {
    CSPLOGSTRING( CSPINT, "CSPCenRepListener::NewL" )
    CSPCenRepListener* self = new( ELeave ) 
        CSPCenRepListener( aUid, aMonitorSetting, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    

// Destructor
CSPCenRepListener::~CSPCenRepListener()
    {
    Cancel();
    delete iRepository;
    }
    

// -----------------------------------------------------------------------------
// CSPCenRepListener::Get
// -----------------------------------------------------------------------------
//
TInt CSPCenRepListener::Get( TInt& aValue )
    {
    return ( iRepository->Get( iMonitorSetting, aValue ) );
    }


// -----------------------------------------------------------------------------
// CSPCenRepListener::DoCancel
// -----------------------------------------------------------------------------
//
void CSPCenRepListener::DoCancel()
    {
    __ASSERT_DEBUG( iRepository, Panic( ECSPPanicNoRepository ) );
    iRepository->NotifyCancel( iMonitorSetting );
    }


// -----------------------------------------------------------------------------
// CSPCenRepListener::RunError
// -----------------------------------------------------------------------------
//
TInt CSPCenRepListener::RunError(
        #ifdef _DEBUG
        TInt aError // Log the leavecode from RunL
        #else
        TInt /*aError*/
        #endif
        )
    {
    #ifdef _DEBUG
    CSPLOGSTRING2( CSPERROR, "CSPCenRepListener::RunError %d", aError );
    #endif
    return ( KErrNone );
    }


// -----------------------------------------------------------------------------
// CSPCenRepListener::RunL
// -----------------------------------------------------------------------------
//
void CSPCenRepListener::RunL()
    {
    CSPLOGSTRING( CSPINT, "CSPCenRepListener::RunL");

    // Don't resubmit the request on error
    // Central repositry completes notifications with id of setting
    // so check only that value of iStatus is negative
    User::LeaveIfError( iStatus.Int() < KErrNone ); 
    SubmitNotifyRequestL();

    TInt volume;
    // The loudspeaker volume has changed in repository.
    // Retrieve the current volume from repository.
    User::LeaveIfError( Get( volume ) );

    CSPLOGSTRING2( CSPINT, "CSPCenRepListener::RunL vol %d", volume);
    iObserver->HandleNotifyCenRepL( iUid, iMonitorSetting, volume ); 
    }


// -----------------------------------------------------------------------------
// CSPCenRepListener::CSPCenRepListener
// -----------------------------------------------------------------------------
//
CSPCenRepListener::CSPCenRepListener(
        TUid aUid,
        TUint32 aMonitorSetting,
        MCSPCenRepObserver* aObserver
        ) : CActive( EPriorityStandard ),
            iUid( aUid ),
            iMonitorSetting( aMonitorSetting ),
            iObserver( aObserver )
    {
    CSPLOGSTRING( CSPINT, "CSPCenRepListener::CSPCenRepListener, start");
    CActiveScheduler::Add( this );
    
    CSPLOGSTRING( CSPINT, "CSPCenRepListener::CSPCenRepListener, complete");
    }

        
// -----------------------------------------------------------------------------
// CSPCenRepListener::ConstructL
// -----------------------------------------------------------------------------
//
void CSPCenRepListener::ConstructL()
    {
    // Create repository instance
    iRepository = CRepository::NewL( iUid );
    // Start monitoring
    SubmitNotifyRequestL();
    }


// -----------------------------------------------------------------------------
// CSPCenRepListener::SubmitNotifyRequestL
// -----------------------------------------------------------------------------
//
void CSPCenRepListener::SubmitNotifyRequestL()
    {
    CSPLOGSTRING( CSPINT, "CSPCenRepListener::SubmitNotifyRequestL");
    __ASSERT_DEBUG( iRepository, Panic( ECSPPanicNoRepository ) );
    __ASSERT_DEBUG( !IsActive(), Panic( ECSPPanicRepositoryAlreadyActive ) );
    iRepository->NotifyRequest( iMonitorSetting, iStatus );
    SetActive();
    CSPLOGSTRING( CSPINT, "CSPCenRepListener::SubmitNotifyRequestL, complete");
    }

    
// End of file
