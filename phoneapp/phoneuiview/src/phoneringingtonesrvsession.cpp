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
* Description:  Phone ringing tone server session.
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <e32svr.h>

#include "mphoneringingtonesrv.h"
#include "phoneringingtonesrv.hrh"
#include "phoneringingtonesrvsession.h"
#include "phoneuiviewtrace.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneRingingToneServerSession::CPhoneRingingToneServerSession
// -----------------------------------------------------------------------------
//
CPhoneRingingToneServerSession::CPhoneRingingToneServerSession( MPhoneRingingToneServer& aServer )
    : iServer( aServer )
    {
    }


// -----------------------------------------------------------------------------
// CPhoneRingingToneServerSession::~CPhoneRingingToneServerSession
// -----------------------------------------------------------------------------
//
CPhoneRingingToneServerSession::~CPhoneRingingToneServerSession()
    {
    iServer.DecrementSessions();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServerSession::NewL
// -----------------------------------------------------------------------------
//
CPhoneRingingToneServerSession* CPhoneRingingToneServerSession::NewL( MPhoneRingingToneServer& aServer )
    {
    CPhoneRingingToneServerSession* self = new (ELeave) CPhoneRingingToneServerSession( aServer );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServerSession::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServerSession::ConstructL()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneServerSession::ConstructL" );
    iServer.IncrementSessions();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneServerSession::ServiceL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneServerSession::ServiceL( const RMessage2& aMessage )
    {
    PHONEUIVIEW_PRINTF( "CPhoneRingingToneServerSession::ServiceL: func=%d", aMessage.Function() );

    TBool noComplete = EFalse;

    switch ( aMessage.Function() )
        {
        case ESetExtendedSecurityNeeded:
            iServer.SetExtendedSecurityNeeded( aMessage.Int0() );
            break;
        case ESetToneFileSizeLimit:
            iServer.SetToneFileSizeLimit( aMessage.Int0() );
            break;
        case ESetAudioRingingTone:
            iServer.SetRingingToneL( aMessage );
            break;
        case ESetDefaultRingingTone:
            iServer.SetDefaultRingingToneL( aMessage );
            break;

        // Playback control
        case EPlayAudioRingTone:
            noComplete = iServer.PlayAudioRingTone( aMessage );
            break;
        case EPlayTtsTone:
            iServer.PlayTtsTone( aMessage );
            break;
        case EPlayDefaultTone:
            iServer.PlayDefaultTone( aMessage );
            break;
        case EPlayBackupTone:
            iServer.PlayBackupTone( aMessage );
            break;
        case EPlaySilentTone:
            iServer.PlaySilentTone();
            break;
        case EBeepOnce:
            iServer.BeepOnce( aMessage );
            break;
        case EPlayUnsecureVoIPTone:
            iServer.PlayUnsecureVoIPTone();
            break;

        case EMuteRingingTone:
            iServer.MuteRingingTone();
            break;
        case EStopPlaying:
            iServer.StopPlaying();
            break;
        default:
            break;
        }

    if ( !noComplete )
        {
        aMessage.Complete( KErrNone );
        }
    }
