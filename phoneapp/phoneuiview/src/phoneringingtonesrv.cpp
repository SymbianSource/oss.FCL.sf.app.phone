/*
* Copyright (c) 2002-2005,2007 Nokia Corporation and/or its subsidiary(-ies). 
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

#include "phoneringingtonesrv.h"
#include "phoneringingtonesrvsession.h"
#include "phoneringingtonesrvplayerao.h"
#include "phoneuiviewtrace.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::CPhoneRingingToneServer
// -----------------------------------------------------------------------------
//
CPhoneRingingToneServer::CPhoneRingingToneServer()
    : CServer2( EPriorityHigh )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::~CPhoneRingingToneServer
// -----------------------------------------------------------------------------
//
CPhoneRingingToneServer::~CPhoneRingingToneServer()
    {
    delete iPlayer; 
    iPlayer = 0;
    iFs.Close();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::NewLC
// -----------------------------------------------------------------------------
//
CPhoneRingingToneServer* CPhoneRingingToneServer::NewLC()
    {
    CPhoneRingingToneServer* self = new (ELeave) CPhoneRingingToneServer;

    CleanupStack::PushL(self);
    self->ConstructL();

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );

    // Allocate player
    iPlayer = CPhoneRingingToneSrvPlayerAO::NewL( iFs );

    // Try to start the server
    StartL( KRingingToneServerName );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::NewSessionL
// -----------------------------------------------------------------------------
//
CSession2* CPhoneRingingToneServer::NewSessionL( const TVersion& /*aVersion*/,  const RMessage2& /*aMessage*/ ) const
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneServerSession::NewSessionL" );

    return CPhoneRingingToneServerSession::NewL( *const_cast<CPhoneRingingToneServer*>(this) );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::StartSession
// -----------------------------------------------------------------------------
//
TInt CPhoneRingingToneServer::StartSession()
    {
    PHONEUIVIEW_PRINTF( "CPhoneRingingToneServer::StartSession err %d", iSessionCount );

    TInt err( KErrNone );

    if( !iSessionInUse )
        {
        iSessionInUse = ETrue;
        }
    else
        {
        err = KErrAccessDenied;
        }

    return err;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::CloseSession
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::CloseSession()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::CloseSession()" );

    CActiveScheduler::Stop();
    iSessionInUse = EFalse;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::ThreadEntryFunc
// -----------------------------------------------------------------------------
//
TInt CPhoneRingingToneServer::ThreadEntryFunc( TAny* /*aPtr*/ )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::ThreadEntryFunc <" );
    // Allocate cleanup stack, active scheduler and server
    CTrapCleanup* cleanup = CTrapCleanup::New();
    CActiveScheduler* scheduler = new CActiveScheduler;

    // Check that all allocations were ok
    if ( !cleanup || !scheduler )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::ThreadEntryFunc: OOM" );
        // Out of memory
        delete scheduler;
        delete cleanup;

        PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::ThreadEntryFunc >" );
        return KErrNoMemory;
        }

    // Install active scheduler
    CActiveScheduler::Install( scheduler );

    // Run the server
    TRAPD( err, CPhoneRingingToneServer::RunServerL() );

    PHONEUIVIEW_PRINTF( "CPhoneRingingToneServer::ThreadEntryFunc: RunServerL err %d", err );

    // Uninstall active scheduler
    CActiveScheduler::Install( NULL );

    // Cleanup
    delete scheduler;
    delete cleanup;

    return err;
    }


// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::RunServerL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::RunServerL()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::RunServerL <" );

    // Allocate server
    CPhoneRingingToneServer* server = CPhoneRingingToneServer::NewLC();

    // Notify that we're running
    RThread::Rendezvous( KErrNone );

    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::RunServerL: server is running!" );

    // Start active scheduler
    CActiveScheduler::Start();

    CleanupStack::PopAndDestroy(); // server

    // Won't stop until all sessions to the server are closed (last one calls CActiveScheduler::Stop)
    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::RunServerL >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::SetExtendedSecurityNeeded
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::SetExtendedSecurityNeeded( TBool aNeeded )
    {
    iPlayer->SetExtendedSecurityNeeded( aNeeded );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::SetToneFileSizeLimit
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::SetToneFileSizeLimit( TInt aLimit )
    {
    iPlayer->SetToneFileSizeLimit( aLimit );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::SetRingingToneL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::SetRingingToneL( const RMessage2& aMessage )
    {
    // Read filename from the message
    TFileName filename;
    TInt len = aMessage.GetDesLength( 0 );

    aMessage.ReadL( 0, filename );

    PHONEUIVIEW_PRINTF( "CPhoneRingingToneServer::SetRingingToneL: %S", &filename );

    // Give the ringing tone to the player
    iPlayer->SetRingingToneL( filename );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::SetDefaultRingingToneL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::SetDefaultRingingToneL( const RMessage2& aMessage )
    {
    // Read filename from the message
    TFileName filename;
    TInt len = aMessage.GetDesLength( 0 );

    aMessage.ReadL( 0, filename );

    PHONEUIVIEW_PRINTF( "CPhoneRingingToneServer::SetDefaultRingingToneL: %S", &filename );

    // Give the default tone to the player
    iPlayer->SetDefaultRingingToneL( filename );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::PlayAudioRingTone
// -----------------------------------------------------------------------------
//
TBool CPhoneRingingToneServer::PlayAudioRingTone( const RMessage2& aMessage )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::PlayAudioRingTone <" );

    // Read the parameters
    TInt volume = aMessage.Int0();
    TProfileRingingType ringingType = (TProfileRingingType)aMessage.Int1();

    PHONEUIVIEW_PRINTF2(
            "CPhoneRingingToneServer::PlayAudioRingTone: volume=%d, ringingType=%d", 
            volume, ringingType);

    TBool ret = iPlayer->PlayAudioRingTone( volume, ringingType, aMessage );

    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::PlayAudioRingTone >" );

    return ret;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::PlayTtsTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::PlayTtsTone( const RMessage2& aMessage )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::PlayTtsTone <" );

    // Read the parameters
    TInt volume = aMessage.Int1();
    TProfileRingingType ringingType = (TProfileRingingType)aMessage.Int2();

    TInt ttsLen = aMessage.GetDesLength( 0 );
    
    if( ttsLen <= 0 )
        {        
        PHONEUIVIEW_PRINTF("CPhoneRingingToneServer::PlayTtsTone: TTS length error: %d!", ttsLen);       
        PHONEUIVIEW_PRINT("CPhoneRingingToneServer::PlayTtsTone >");         
        return;          
        }
    
    HBufC* textToSay = HBufC::New( ttsLen );
    if ( !textToSay )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::PlayTtsTone: OOM!" );
        PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::PlayTtsTone >" );
        return;
        }

    TPtr ptr( textToSay->Des() );

    TInt err = aMessage.Read( 0, ptr );
    if ( err != KErrNone )
        {
        PHONEUIVIEW_PRINTF( "CPhoneRingingToneServer::PlayTtsTone: read err %d", err );
        delete textToSay;
        return;
        }

    PHONEUIVIEW_PRINTF3(
            "CPhoneRingingToneServer::PlayTtsTone: text=%S, volume=%d, ringingType=%d", 
            &(*textToSay), volume, ringingType );

    iPlayer->PlayTtsTone( *textToSay, volume, ringingType );

    // Cleanup
    delete textToSay;

    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::PlayTtsTone >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::PlayDefaultTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::PlayDefaultTone( const RMessage2& aMessage )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::PlayDefaultTone <" );

    // Read the parameters
    TInt volume = aMessage.Int0();
    TProfileRingingType ringingType = (TProfileRingingType)aMessage.Int1();

    PHONEUIVIEW_PRINTF2(
        "CPhoneRingingToneServer::PlayDefaultTone: volume=%d, ringingType=%d", 
        volume, ringingType );

    iPlayer->PlayDefaultTone( volume, ringingType );

    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::PlayDefaultTone >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::PlayBackupTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::PlayBackupTone( const RMessage2& aMessage )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::PlayBackupTone <" );

    // Read the parameters
    TInt volume = aMessage.Int0();
    TProfileRingingType ringingType = (TProfileRingingType)aMessage.Int1();

    PHONEUIVIEW_PRINTF2(
        "CPhoneRingingToneServer::PlayBackupTone: volume=%d, ringingType=%d", 
        volume, ringingType );

    iPlayer->PlayBackupTone( volume, ringingType );

    PHONEUIVIEW_PRINT( "CPhoneRingingToneServer::PlayBackupTone >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::PlaySilentTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::PlaySilentTone()
    {
    iPlayer->PlaySilentTone();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::BeepOnce
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::BeepOnce( const RMessage2& aMessage )
    {
    iPlayer->BeepOnce( aMessage.Int0() );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::PlayUnsecureVoIPTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::PlayUnsecureVoIPTone()
    {
    iPlayer->PlayUnsecureVoIPTone();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::MuteRingingTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::MuteRingingTone()
    {
    iPlayer->MuteRingingTone();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServer::StopPlaying
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServer::StopPlaying()
    {
    iPlayer->StopPlaying();
    }

