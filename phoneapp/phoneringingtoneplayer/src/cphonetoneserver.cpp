/*
* Copyright (c) 2010 - 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Ringing tone server.
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <e32svr.h>

#include "phoneringingtonetrace.h"
#include "cphonetoneserver.h"
#include "cphonetoneserversession.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneToneServer::CPhoneToneServer
// -----------------------------------------------------------------------------
//
CPhoneToneServer::CPhoneToneServer()
    : CServer2( EPriorityHigh )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServer::CPhoneToneServer <");
    PHONERINGINGTONE_PRINT("CPhoneToneServer::CPhoneToneServer >");    
    }

// -----------------------------------------------------------------------------
// CPhoneToneServer::~CPhoneToneServer
// -----------------------------------------------------------------------------
//
CPhoneToneServer::~CPhoneToneServer()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServer::~CPhoneToneServer <");
    PHONERINGINGTONE_PRINT("CPhoneToneServer::~CPhoneToneServer >");        
    }

// -----------------------------------------------------------------------------
// CPhoneToneServer::NewLC
// -----------------------------------------------------------------------------
//
CPhoneToneServer* CPhoneToneServer::NewLC()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServer::NewLC <");
    CPhoneToneServer* self = new( ELeave )CPhoneToneServer;
    CleanupStack::PushL( self );
    self->ConstructL();
    PHONERINGINGTONE_PRINT("CPhoneToneServer::NewLC >");    
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneToneServer::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneToneServer::ConstructL()
    { 
    PHONERINGINGTONE_PRINT("CPhoneToneServer::ConstructL <");  
    StartL( KRingingToneServerName );
    PHONERINGINGTONE_PRINT("CPhoneToneServer::ConstructL >");
    }

// -----------------------------------------------------------------------------
// CPhoneToneServer::NewSessionL
// -----------------------------------------------------------------------------
//
CSession2* CPhoneToneServer::NewSessionL( const TVersion& /*aVersion*/,  
                                          const RMessage2& /*aMessage*/ ) const
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServer::NewSessionL <");
    if ( iHasConnection )
        {
        // Server should be unique connectable.
        User::Leave( KErrAccessDenied  );
        //return NULL;
        }
    CSession2* session = CPhoneToneServerSession::NewL(*this );
    iHasConnection = ETrue;
    PHONERINGINGTONE_PRINT("CPhoneToneServer::NewSessionL >");
    return session;
    }

// -----------------------------------------------------------------------------
// CPhoneToneServer::ThreadEntryFunc
// -----------------------------------------------------------------------------
//
TInt CPhoneToneServer::ThreadEntryFunc( TAny* /*aPtr*/ )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServer::ThreadEntryFunc <");
    CTrapCleanup* cleanup = CTrapCleanup::New();
    CActiveScheduler* scheduler = new CActiveScheduler;

    // Check that all allocations were ok
    if ( !cleanup || !scheduler )
        {
        // Out of memory
        delete scheduler;
        delete cleanup;
        return KErrNoMemory;
        }
    // Install active scheduler
    CActiveScheduler::Install( scheduler );
    // Run the server
    TRAPD( err, CPhoneToneServer::RunServerL() );
    // Uninstall active scheduler
    CActiveScheduler::Install( NULL );
    // Cleanup
    delete scheduler;
    delete cleanup;

    PHONERINGINGTONE_PRINTF("CPhoneToneServer::ThreadEntryFunc - err(%d)",err); 
    PHONERINGINGTONE_PRINT("CPhoneToneServer::ThreadEntryFunc >");
    return err;
    }

// -----------------------------------------------------------------------------
// CPhoneToneServer::RunDown
// -----------------------------------------------------------------------------
//
void CPhoneToneServer::RunDown() const 
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServer::RunDown <");
    CActiveScheduler::Stop();
    PHONERINGINGTONE_PRINT("CPhoneToneServer::RunDown >");    
    }

// -----------------------------------------------------------------------------
// CPhoneToneServer::RunServerL
// -----------------------------------------------------------------------------
//
void CPhoneToneServer::RunServerL()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServer::RunServerL <");
    // Allocate server
    CPhoneToneServer* server = CPhoneToneServer::NewLC();
    // Notify that we're running
    RThread::Rendezvous( KErrNone );
    // Start active scheduler
    CActiveScheduler::Start();
    CleanupStack::PopAndDestroy(); // server
    PHONERINGINGTONE_PRINT("CPhoneToneServer::RunServerL >")
    }

// End of file.

