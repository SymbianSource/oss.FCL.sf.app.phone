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
* Description:  Client interface of the phone ringing tone server.
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <e32std.h>
#include <e32svr.h>

#include "phoneringingtoneclient.h"
#include "phoneringingtonesrv.h"
#include "phoneringingtonesrv.hrh"
#include "phoneuiviewtrace.h"

// LOCAL FUNCTION PROTOTYPES
static TInt StartServer();

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::RPhoneRingingTonePlayer
// -----------------------------------------------------------------------------
//
RPhoneRingingTonePlayer::RPhoneRingingTonePlayer() : RSessionBase()
    {
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::~RPhoneRingingTonePlayer
// -----------------------------------------------------------------------------
//
RPhoneRingingTonePlayer::~RPhoneRingingTonePlayer()
    {
    Close();
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::Connect
// -----------------------------------------------------------------------------
//
TInt RPhoneRingingTonePlayer::Connect()
    {
    PHONEUIVIEW_PRINT("RPhoneRingingTonePlayer::Connect <");
    TInt retries = 3;
    TInt err = KErrNone;

    while(retries--)
        {
        err = CreateSession( KRingingToneServerName, 
            TVersion( KRingingToneSrvVersionMajor,KRingingToneSrvVersionMinor,KRingingToneSrvVersionBuild ) );

        PHONEUIVIEW_PRINTF("RPhoneRingingTonePlayer::Connect: CreateSession err %d", err);
        if ( ( err != KErrNotFound ) && ( err != KErrServerTerminated ) )
            {
            break;
            }

        err = StartServer();

        PHONEUIVIEW_PRINTF("RPhoneRingingTonePlayer::Connect: StartServer err %d", err);
        if ( ( err != KErrNone ) && ( err != KErrAlreadyExists ) )
            {
            break;
            }
        }

    PHONEUIVIEW_PRINTF("RPhoneRingingTonePlayer::Connect, ret %d >", err);
    return err;
    }


// -----------------------------------------------------------------------------
// StartServer
// -----------------------------------------------------------------------------
//
TInt StartServer()
{
    PHONEUIVIEW_PRINT("RPhoneRingingTonePlayer - StartServer <");

    // Form thread name
    TTime time;
    time.UniversalTime();

    TFullName threadName;
    threadName.Format( _L("PhoneRingingToneThread%Ld"), time.Int64() );

    // Create thread
    RThread thread;
    TInt err = thread.Create( threadName, CPhoneRingingToneServer::ThreadEntryFunc, 0x8000, NULL, NULL );
    if ( err != KErrNone )
        {
        return err;
        }

    PHONEUIVIEW_PRINT("RPhoneRingingTonePlayer - StartServer: thread created");

    // Bump thread priority
    thread.SetPriority( EPriorityAbsoluteHigh );

    // Rendezvous with the thread
    TRequestStatus status;
    thread.Rendezvous( status );
    if ( status != KRequestPending )
        {
        thread.Kill( 0 );
        }
    else
        {
        thread.Resume();
        }

    PHONEUIVIEW_PRINT("RPhoneRingingTonePlayer - StartServer: waiting for rendezvous");

    // Wait for thread start or death
    User::WaitForRequest( status );
    thread.Close();

    PHONEUIVIEW_PRINTF("RPhoneRingingTonePlayer - StartServer: rendezvous: %d", status.Int());

    PHONEUIVIEW_PRINT("RPhoneRingingTonePlayer - StartServer >");
    return status.Int();
}

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::SetExtendedSecurityNeeded
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::SetExtendedSecurityNeeded( TBool aNeeded )
    {
    SendReceive( ESetExtendedSecurityNeeded, TIpcArgs( aNeeded ) );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::SetToneFileSizeLimit
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::SetToneFileSizeLimit( TInt aLimit )
    {
    SendReceive( ESetToneFileSizeLimit, TIpcArgs( aLimit ) );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::SetRingingToneL
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::SetRingingToneL( const TDesC& aFilename )
    {
    User::LeaveIfError( SendReceive( ESetAudioRingingTone, TIpcArgs( &aFilename ) ) );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::SetDefaultRingingToneL
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::SetDefaultRingingToneL( const TDesC& aFilename )
    {
    User::LeaveIfError( SendReceive( ESetDefaultRingingTone, TIpcArgs( &aFilename ) ) );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::PlayAudioRingTone
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::PlayAudioRingTone( TInt aVolume, TProfileRingingType aRingingType )
    {
    SendReceive( EPlayAudioRingTone, TIpcArgs( aVolume, aRingingType ) );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::PlayTtsTone
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::PlayTtsTone( const TDesC& aTextToSay, TInt aVolume, TProfileRingingType aRingingType )
    {
    SendReceive( EPlayTtsTone, TIpcArgs( &aTextToSay, aVolume, aRingingType ) );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::PlayDefaultTone
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::PlayDefaultTone( TInt aVolume, TProfileRingingType aRingingType )
    {
    SendReceive( EPlayDefaultTone, TIpcArgs( aVolume, aRingingType ) );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::PlayBackupTone
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::PlayBackupTone( TInt aVolume, TProfileRingingType aRingingType )
    {
    SendReceive( EPlayBackupTone, TIpcArgs( aVolume, aRingingType ) );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::PlaySilentTone
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::PlaySilentTone()
    {
    SendReceive( EPlaySilentTone );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::BeepOnce
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::BeepOnce( TInt aVolume )
    {
    SendReceive( EBeepOnce, TIpcArgs( aVolume ) );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::PlayUnsecureVoIPTone
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::PlayUnsecureVoIPTone()
    {
    SendReceive( EPlayUnsecureVoIPTone );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::MuteRingingTone
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::MuteRingingTone()
    {
    SendReceive( EMuteRingingTone );
    }

// -----------------------------------------------------------------------------
// RPhoneRingingTonePlayer::StopPlaying
// -----------------------------------------------------------------------------
//
void RPhoneRingingTonePlayer::StopPlaying()
    {
    SendReceive( EStopPlaying );
    }
