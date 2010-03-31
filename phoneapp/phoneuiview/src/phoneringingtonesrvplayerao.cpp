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
* Description:  Active object of the ringing tone player.
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <f32file.h>
#include <utility.h>
#include <AudioPreference.h>

#include "phoneui.pan"
#include "cphoneringingtone.h"
#include "cphoneaudioplayer.h"
#include "cphonetimer.h"
#include "phoneringingtonesrvplayerao.h"
#include "phoneringingtonesrv.h"
#include "phoneconstants.h"
#include "phoneuiviewtrace.h"

#ifdef __WINS__
#include <data_caging_path_literals.hrh>
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::NewL
// -----------------------------------------------------------------------------
//
CPhoneRingingToneSrvPlayerAO* CPhoneRingingToneSrvPlayerAO::NewL( RFs& aFs )
    {
    CPhoneRingingToneSrvPlayerAO* self =
        new ( ELeave ) CPhoneRingingToneSrvPlayerAO( aFs );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::CPhoneRingingToneSrvPlayerAO
// -----------------------------------------------------------------------------
//
CPhoneRingingToneSrvPlayerAO::CPhoneRingingToneSrvPlayerAO( RFs &aFs ) :
    CActive( CActive::EPriorityStandard ),
    iFs( aFs ),
    iAsyncDeletePlayers( EPlayerCount )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::ConstructL()
    {
    for ( TInt i = EPlayerFirst; i <= EPlayerLast; i++ )
        {
        User::LeaveIfError(
            iAsyncDeletePlayers.Append( NULL ) );
        }

    // Construct timer.
    iTimer = CPhoneTimer::NewL();
    iTtsDelayTimer = CPhoneTimer::NewL();

    ConstructPlayers();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::~CPhoneRingingToneSrvPlayerAO
// -----------------------------------------------------------------------------
//
CPhoneRingingToneSrvPlayerAO::~CPhoneRingingToneSrvPlayerAO()
    {
    Cancel();

    iAsyncDeletePlayers.ResetAndDestroy();
    iAsyncDeletePlayers.Close();

    CleanupPlayers();

    delete iTimer;
    delete iTtsDelayTimer;
    delete iDefaultRingingTone;
    delete iAudioVideoRingingTone;

    if ( iClientMessage.Handle() )
        {
        iClientMessage.Complete( KErrNone );
        }
    }


// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::PlayAudioRingTone
// -----------------------------------------------------------------------------
//
TBool CPhoneRingingToneSrvPlayerAO::PlayAudioRingTone(
    TInt aVolume,
    TProfileRingingType aRingingType,
    const RMessage2& aMessage )
    {

    TBool messageSaved = EFalse;
    iTimer->Cancel();

    // Release any old client (shouldn't happen)
    if ( iClientMessage.Handle() )
        {
        iClientMessage.Complete( KErrNone );
        }

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayAudioRingTone <" );
    PHONEUIVIEW_PRINTF2(
        "CPhoneRingingToneSrvPlayerAO::PlayAudioRingTone - aVolume(%d), aRingingType(%d)",
        aVolume,
        aRingingType );

    if ( !iAudioVideoRingingTone ||
         !CheckAndHandleToneSizeLimit( iAudioVideoRingingTone ) )
        {
        PlayDefaultTone( aVolume, aRingingType );
        }
    else if ( aRingingType == EProfileRingingTypeSilent )
        {
        PlaySilentTone();
        }
    else if ( aRingingType == EProfileRingingTypeBeepOnce )
        {
        BeepOnce( aVolume );
        }
    else
        {
        // Construct player for line 1.
        if ( !iAudioPlayer )
            {
            iAudioPlayer = ConstructTonePlayer(
                *iAudioVideoRingingTone,
                EPlayerAudio );

            if ( !iTimer->IsActive() )
                {
                PHONEUIVIEW_PRINT( "RING TONE FAILSAFE TIMER START!!" );
                // Start timer, since the player is constructed.
                iTimer->After( KPhoneMaxRingingWaiting, this );
                }
            }

        // Start playing.
        if ( !iAudioPlayer )
            {
            iTimer->Cancel();
            PlayDefaultTone( aVolume, aRingingType );
            }
        else
            {
            PHONEUIVIEW_PRINT(
                "CPhoneRingingToneSrvPlayerAO::PlayAudioRingTone - Start playing audio" );
            iVolume = aVolume;
            iRingingType = aRingingType;
            AddTtsPlaybackIfNeeded( aRingingType );
            iTonePlayingStatus = EAudioTonePlaying;

            // Save the client message which will be completed after init/error/timeout
            iClientMessage = aMessage;
            messageSaved = ETrue;

            iAudioPlayer->Play(
                ConvertRingingType( aRingingType ),
                aVolume,
                iTtsToneToBePlayed );
            }
        }

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayAudioRingTone >" );

    return messageSaved;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::BeepOnce
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::BeepOnce( TInt aVolume )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::BeepOnce <" );
    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::BeepOnce - aVolume(%d)", aVolume );

    if ( !iBeepOncePlayer )
        {
        ConstructSequencePlayer( EPlayerBeepOnce );
        }

    if ( iBeepOncePlayer )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::BeepOnce - Play" );
        iTonePlayingStatus = EBeepOnce;
        iBeepOncePlayer->Play( CPhoneAudioPlayer::ETypeRingingOnce,
            aVolume, EFalse );
        }
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::BeepOnce >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO,::PlaySilentTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::PlaySilentTone()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlaySilentTone <" );

    if ( !iSilentPlayer )
        {
        ConstructSequencePlayer( EPlayerSilent );
        }

    __ASSERT_DEBUG( iSilentPlayer, Panic( EPhoneViewGeneralError ) );

    if ( iSilentPlayer )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlaySilentTone - play" );
        iTonePlayingStatus = ESilentTonePlaying;
        iSilentPlayer->Play( CPhoneAudioPlayer::ETypeRinging, 0, EFalse );
        }

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlaySilentTone >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::PlayUnsecureVoIPTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::PlayUnsecureVoIPTone()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayUnsecureVoIPTone <" );
    if ( !iUnsecureVoIPTonePlayer )
        {
        ConstructSequencePlayer( EPlayerUnsecureVoIP );
        }
    __ASSERT_DEBUG( iUnsecureVoIPTonePlayer, Panic( EPhoneViewGeneralError ) );

    if ( iUnsecureVoIPTonePlayer )
        {
        iTonePlayingStatus = EUnsecureVoIPTonePlaying;
        iUnsecureVoIPTonePlayer->Play( CPhoneAudioPlayer::ETypeRingingOnce,
            KPhoneDefaultVolume, EFalse );
        }

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayUnsecureVoIPTone >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::MuteRingingTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::MuteRingingTone()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::MuteRingingTone <" );

    CPhoneAudioPlayer* activePlayer = ActiveAudioPlayer();
    if ( activePlayer )
        {
        activePlayer->MutePlaying();
        }

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::MuteRingingTone >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::StopPlaying
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::StopPlaying()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::StopPlaying <" );

    // Complete waiting client just in case
    if ( iClientMessage.Handle() )
        {
        iClientMessage.Complete( KErrNone );
        }

    iTtsToneToBePlayed = EFalse;
    iTTsTimeOutCounter = 0;
    iTtsDelayTimer->Cancel();
    if ( iTtsPlayer )
        {
        iTtsPlayer->StopPlaying();
        delete iTtsPlayer;
        iTtsPlayer = NULL;
        }
    iTimer->Cancel();

    switch( iTonePlayingStatus )
        {
        case EAudioTonePlaying:
            if ( iAudioPlayer )
                {
                // Commented stopping out, since with this, the screen is jammed
                // when REComSession::FinalClose() is reached in
                // CPhoneAudioPlayer's destructor.
                // Audio player is stopped anyway in its destructor.
                // Fixes EKKI-7WYG2G and JKAN-7XNGQU.
                // iAudioPlayer->StopPlaying();
                delete iAudioPlayer;
                iAudioPlayer = NULL;
                }
            break;
        case EDefaultTonePlaying:
            if ( iDefaultPlayer )
                {
                iDefaultPlayer->StopPlaying();
                delete iDefaultPlayer;
                iDefaultPlayer = NULL;
                }
            break;
        case EBeepOnce:
            if ( iBeepOncePlayer )
                {
                iBeepOncePlayer->StopPlaying();
                delete iBeepOncePlayer;
                iBeepOncePlayer = NULL;
                }
            break;
        case EBackupTonePlaying:
            if ( iBackupPlayer )
                {
                iBackupPlayer->StopPlaying();
                }
            break;
        case ESilentTonePlaying:
            if ( iSilentPlayer )
                {
                iSilentPlayer->StopPlaying();
                delete iSilentPlayer;
                iSilentPlayer = NULL;
                }
            break;
        case EUnsecureVoIPTonePlaying:
            if ( iUnsecureVoIPTonePlayer )
                {
                iUnsecureVoIPTonePlayer->StopPlaying();
                delete iUnsecureVoIPTonePlayer;
                iUnsecureVoIPTonePlayer = NULL;
                }
            break;
        case EIdle:
            break;
        default:
            Panic( EPhoneViewCaseNotHandled );
            break;
        }

    iTonePlayingStatus = EIdle;

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::StopPlaying >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::PlayTtsTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::PlayTtsTone(
    const TDesC& aTextToSay,
    TInt aVolume,
    TProfileRingingType aRingingType )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayTtsTone <" );
    PHONEUIVIEW_PRINTF2(
        "CPhoneRingingToneSrvPlayerAO::PlayTtsTone - aVolume(%d), aRingingType(%d)",
        aVolume, aRingingType );

    if ( iTtsPlayer )
        {
        delete iTtsPlayer;
        iTtsPlayer = NULL;
        }

    //Set ringingtype for Tts.
    iTtsRingingType = aRingingType;
    iTtsVolume = aVolume;

    // TTS should be a bit louder, than main component.
    // No need to check for overflows, they are handled in SET_PHONE_VOLUME macro.

    TPtrC ttsHeader( KPhoneTtsHeader );
    HBufC* preparedString =
        HBufC::New( aTextToSay.Length() + ttsHeader.Length() );

    if ( !preparedString )
        {
        PHONEUIVIEW_PRINTF3(
            "CPhoneRingingToneSrvPlayerAO::PlayTtsTone: Couldn't allocate buf %d + %d = %d",
            aTextToSay.Length(), ttsHeader.Length(), aTextToSay.Length()+ttsHeader.Length() );
        }
    else
        {
        TPtr pPreparedString = preparedString->Des();
        pPreparedString.Append( ttsHeader );
        pPreparedString.Append( aTextToSay );

        PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::PlayTtsTone - about to say %S",
            &pPreparedString );

        TRAPD(
            error,
            iTtsPlayer = CPhoneAudioPlayer::NewTtsL(
               *preparedString,
               KAudioPriorityPhoneCall,
               KAudioPrefTextToSpeechCallHardCoded,
               *this,
               EPlayerTts ) );
        if ( error == KErrNone )
            {
            // To be played when default or personal tone will be played
            iTtsToneToBePlayed = ETrue;
            }
        PHONEUIVIEW_PRINTF2(
            "CPhoneRingingToneSrvPlayerAO::PlayTtsTone - error(%d), iTtsPhoneToBePlayed(%d)",
            error,
            iTtsToneToBePlayed );

        delete preparedString;
        }

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayTtsTone >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::HandleAudioPlayerError
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::HandleAudioPlayerError(
    TPhoneAudioPlayerErrorEvent /*aEvent*/,
    TInt aError,
    TInt /*aId*/ )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandleAudioPlayerError <" );
    // This is called from CPhoneAudioPlayer - we do not
    // want to delete instances from which this method
    // is called.

    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::HandleAudioPlayerError: error = %d", aError );

    const TBool deleteAll = ( aError == KErrServerTerminated );

    if ( deleteAll )
        {
        // KErrServerTerminated is considered as fatal error. So we
        // need to delete everything.

        for ( TInt i = EPlayerFirst; i <= EPlayerLast; i++ )
            {
            DeletePlayerAsync( static_cast< TPlayerId >( i ) );
            }

        // Now all member audio players point to NULL, except
        // those players in iAsyncDeletePlayers. Recreate
        // players.

        ConstructPlayers(); // Ignore error code.
        }

    iTimer->Cancel();
    DoHandlePlayerError( !deleteAll, EFalse );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::HandleAudioPlayerInitComplete
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::HandleAudioPlayerInitComplete( TInt aId )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandleAudioPlayerInitComplete <" );

    // This is called from CPhoneAudioPlayer - we do not
    // want to delete instances from which this method
    // is called.

    // Complete waiting client
    if ( iClientMessage.Handle() )
        {
        iClientMessage.Complete( KErrNone );
        }

    if ( iTonePlayingStatus == EAudioTonePlaying && aId == EPlayerAudio ||
         iTonePlayingStatus == EDefaultTonePlaying && aId == EPlayerDefault )
        {
        PHONEUIVIEW_PRINT( "RING TONE FAILSAFE TIMER STOP!!" );
        iTimer->Cancel();
        }

    if ( iTtsToneToBePlayed && ( aId != EPlayerTts ) )
        {
        // Start TTS timer just before "normal ringtone component" starts
        // playing, i.e. after init completed.
        if ( iTtsDelayIndex < iTtsDelaysCount )
            {
            if ( !iTtsDelayTimer->IsActive() )
                {
                // There are TTS iterations to be played yet.
                iTtsDelayTimer->After(
                    KPhoneTtsDelays[iTtsDelayIndex],
                    TCallBack( HandleTtsDelayTimeout, this ) );
                }
            }
        }

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandleAudioPlayerInitComplete >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete( TInt aId )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete <" );
    // This is called from CPhoneAudioPlayer - we do not
    // want to delete instances from which this method
    // is called.

    // Complete waiting client (shouldn't reach this far, but who knows)
    if ( iClientMessage.Handle() )
        {
        iClientMessage.Complete( KErrNone );
        }

    PHONEUIVIEW_PRINTF2(
        "CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete - aId(%d), iRingingType(%d)",
        aId,
        iRingingType );

    PHONEUIVIEW_PRINTF2(
        "CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete - iTtsToneToBePlayed(%d), iTtsDelayIndex(%d)",
        iTtsToneToBePlayed,
        iTtsDelayIndex );

    if ( aId == EPlayerTts )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete - resuming after TTS" );

        // TTS playing complete, normal tone player should volume up.
        if ( ( iTtsToneToBePlayed ) && ( ++iTtsDelayIndex < iTtsDelaysCount ) )
            {
            PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete - resuming after TTS first time" );

            //Callers name is said once. increase tone player volume.
            SolveNewVolumeAndRamptime( ESaidOnce );

            if ( !iTtsDelayTimer->IsActive() )
                {
                // There are TTS iterations to be played yet.
                iTtsDelayTimer->After( KPhoneTtsDelays[iTtsDelayIndex],
                    TCallBack( HandleTtsDelayTimeout,this ) );
                }
            }
        else
            {
            PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete - resuming after TTS second time" );
            SolveNewVolumeAndRamptime( ESaidTwice );
            }

        // No further processing. All TTS player-related events are not
        // "real" ringtone playing completions.
        return;
        }

    //Case: RingintonePlayer has completed playing with following set: ring once and
    // TTS is activated
    else if ( iRingingType == EProfileRingingTypeRingingOnce &&
        iTtsToneToBePlayed )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete - ring once and TTS" );
         if ( iTtsDelayIndex < iTtsDelaysCount )
            {
            ReStartRingingTonePlayer();
            //Do not destroy player yet. Return.
            return;
            }
        }
    //Case: RingintonePlayer has completed playing with following set: ringing
    //and TTS is activated. We need  to restart ringintoneplayer and new TTS
    //iterations are required.
    else if ( iRingingType == EProfileRingingTypeRinging && iTtsToneToBePlayed )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete - ringing and TTS" );
        if ( iTtsDelayIndex == iTtsDelaysCount )
            {
            //Case: Ringingtype is EProfileRingingTypeRinging. New TTS
            //iterations are required.
            PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete - need to restart TTS sequence" );
            //TTS has completed its iterations,set index to zero.
            iTtsDelayIndex = 0;
            if ( !iTtsDelayTimer->IsActive() )
                {
                //Restart TTS sequence
                iTtsDelayTimer->After(
                    KPhoneTtsDelays[iTtsDelayIndex],
                    TCallBack( HandleTtsDelayTimeout, this ) );
                }
            }

        ReStartRingingTonePlayer();
        //Do not destroy player yet. Return
        return;
        }

    PHONEUIVIEW_PRINTF(
        "CPhoneRingingToneSrvPlayerAO::HandlePlayingComplete - delete player - iTonePlayingStatus(%d)",
        iTonePlayingStatus );
    switch( iTonePlayingStatus )
        {
        case EAudioTonePlaying:
            DeletePlayerAsync( EPlayerAudio );
            iAudioPlayer = NULL;
            break;

        case EDefaultTonePlaying:
            DeletePlayerAsync( EPlayerDefault );
            iDefaultPlayer = NULL;
            break;

        case ESilentTonePlaying:
            DeletePlayerAsync( EPlayerSilent );
            iSilentPlayer = NULL;
            break;

        case EUnsecureVoIPTonePlaying:
            DeletePlayerAsync( EPlayerUnsecureVoIP );
            iUnsecureVoIPTonePlayer = NULL;
            break;
        case EBeepOnce:
            DeletePlayerAsync( EPlayerBeepOnce );
            iBeepOncePlayer = NULL;
            break;

        case EBackupTonePlaying:
            break;

        default:
            Panic( EPhoneViewCaseNotHandled );
            break;
        }

    iTonePlayingStatus = EIdle;
    }


// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::HandleTimeOutL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::HandleTimeOutL()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandleTimeOutL <" );


    // Guarding timer for startup has expired


    PHONEUIVIEW_PRINT( "RING TONE FAILSAFE TIMER TRIGGERED!!" );

    // Start new audio player synchronously.
    DoHandlePlayerError( ETrue, ETrue );

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandleTimeOutL >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::DoHandlePlayerError
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::DoHandlePlayerError(
    TBool aDelete,
    TBool aSync )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::DoHandlePlayerError <" );

    PHONEUIVIEW_PRINTF(
        "CPhoneRingingToneSrvPlayerAO::DoHandlePlayerError: iTonePlayingStatus = %d",
        iTonePlayingStatus );

    // Complete waiting client
    if ( iClientMessage.Handle() )
        {
        iClientMessage.Complete( KErrNone );
        }

    switch ( iTonePlayingStatus )
        {
        case EAudioTonePlaying:
            if ( aDelete )
                {
                if ( aSync )
                    {
                    delete iAudioPlayer;
                    }
                else
                    {
                    DeletePlayerAsync( EPlayerAudio );
                    }

                iAudioPlayer = NULL;
                }

#ifdef __WINS__
            PlayAudioRingTone( iVolume, iRingingType, iClientMessage );
            iTonePlayingStatus = EDefaultTonePlaying;
#else
            PlayDefaultTone( iVolume, iRingingType );
#endif
            break;
        case EDefaultTonePlaying:
            if ( aDelete )
                {
                if ( aSync )
                    {
                    delete iDefaultPlayer;
                    }
                else
                    {
                    DeletePlayerAsync( EPlayerDefault );
                    }

                iDefaultPlayer = NULL;
                }

            PlayBackupTone( iVolume, iRingingType );
            break;
        case ESilentTonePlaying:
        case EBeepOnce:
        case EBackupTonePlaying:
            break;
        default:
            break;
        }
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::DoHandlePlayerError >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::PlayDefaultTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::PlayDefaultTone(
    TInt aVolume,
    TProfileRingingType aRingingType )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayDefaultTone()" );

    if( aRingingType == EProfileRingingTypeSilent ||
        aRingingType == EProfileRingingTypeBeepOnce )
        {
        return;
        }

    if ( !iDefaultRingingTone )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayDefaultTone - default tone does not exist, play backup..." );
        PlayBackupTone( aVolume, aRingingType );
        return;
        }

    // Construct player.
    if ( !iDefaultPlayer )
        {
        iDefaultPlayer = ConstructTonePlayer(
            *iDefaultRingingTone,
            EPlayerDefault );
        if ( !iTimer->IsActive() )
            {
            PHONEUIVIEW_PRINT( "RING TONE FAILSAFE TIMER START!!" );
            iTimer->After( KPhoneMaxRingingWaiting, this );
            }
        }

    // Start playing.
    if ( !iDefaultPlayer )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayDefaultTone - player not constructed, play backup..." );
        iTimer->Cancel();
        PlayBackupTone( aVolume, aRingingType );
        }
    else
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayDefaultTone - play" );
        iVolume = aVolume;
        iRingingType = aRingingType;
        iTonePlayingStatus = EDefaultTonePlaying;
        iDefaultPlayer->Play(
            ConvertRingingType( aRingingType ), aVolume, iTtsToneToBePlayed );
        }

    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::PlayBackupTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::PlayBackupTone(
    TInt aVolume,
    TProfileRingingType aRingingType )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayBackupTone()" );
    __ASSERT_DEBUG( iBackupPlayer, Panic( EPhoneViewGeneralError ) );

    if ( !iBackupPlayer )
        {
        ConstructPlayers();
        }

    if ( iBackupPlayer )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::PlayBackupTone - play" );
        iTonePlayingStatus = EBackupTonePlaying;

        // Cease playing text-to-speech, if necessary
        iTtsToneToBePlayed = EFalse;
        iTTsTimeOutCounter = 0;
        iTtsDelayTimer->Cancel();
        if ( iTtsPlayer )
            {
            iTtsPlayer->StopPlaying();
            delete iTtsPlayer;
            iTtsPlayer = NULL;
            }

        iBackupPlayer->Play(
            ConvertRingingType( aRingingType ), aVolume, EFalse );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::ConstructTonePlayer
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer* CPhoneRingingToneSrvPlayerAO::ConstructTonePlayer(
    const CPhoneRingingTone& aRingingTone,
    TInt aId )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::ConstructTonePlayer" );
    return CPhoneAudioPlayer::New(
        aRingingTone,
        KAudioPriorityPhoneCall,
        KAudioPrefIncomingCall,
        *this,
        aId,
        iMdaServer,
        aId == EPlayerDefault ? EFalse : iExtSecNeeded );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::ConvertRingingType
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer::TRingingType CPhoneRingingToneSrvPlayerAO::ConvertRingingType(
    TProfileRingingType aRingingType )
    {
    CPhoneAudioPlayer::TRingingType ringingType = 
        CPhoneAudioPlayer::ETypeRinging;
    switch ( aRingingType )
        {
        case EProfileRingingTypeRinging:
            ringingType = CPhoneAudioPlayer::ETypeRinging;
            break;

        case EProfileRingingTypeAscending:
            ringingType = CPhoneAudioPlayer::ETypeAscending;
            break;

        case EProfileRingingTypeRingingOnce:
            ringingType = CPhoneAudioPlayer::ETypeRingingOnce;
            break;

        default:
            break;
        }
    return ringingType;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::ConstructPlayers
// -----------------------------------------------------------------------------
//
TInt CPhoneRingingToneSrvPlayerAO::ConstructPlayers()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::ConstructPlayers <" );
    TRAPD( err, ConstructPlayersL() );

    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::ConstructPlayers: err = %d", err );
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::ConstructPlayers >" );
    return err;
    }


// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::ConstructPlayersL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::ConstructPlayersL()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::ConstructPlayersL <" );
    if ( !iMdaServer )
        {
        iMdaServer = CMdaServer::NewL();
        }

    // Construct Backup player.
    if ( !iBackupPlayer )
        {
        TFileName defaultSoundFile( KDriveZ );
        defaultSoundFile.Append( KPhoneDefaultSoundFile );

        PHONEUIVIEW_PRINTF(
            "CPhoneRingingToneSrvPlayerAO::ConstructPlayersL: construct backup player, path = %S",
            &defaultSoundFile );
        iBackupPlayer = CPhoneAudioPlayer::NewL(
            defaultSoundFile,
            KAudioPriorityPhoneCall,
            KAudioPrefIncomingCall,
            *this,
            EPlayerBackup,
            iMdaServer
            );
        }

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::ConstructPlayersL >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::ConstructSequencePlayer
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::ConstructSequencePlayer( TPlayerId aId )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::ConstructSequencePlayer" );

    // Construct players.
    TRAPD( err, ConstructSequencePlayerL( aId ) );
    if ( err )
        {
        PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::ConstructSequencePlayer - err(%d)", err );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::ConstructSequencePlayerL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::ConstructSequencePlayerL( TPlayerId aId )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::ConstructSequencePlayerL <" );

    // Construct beep once player
    if ( aId == EPlayerBeepOnce )
        {
#ifdef __WINS__
        TParse* fp = new (ELeave) TParse();
        fp->Set( KPhoneBeepOnceSoundFile, &KDC_RESOURCE_FILES_DIR, NULL );
        TFileName fileName( fp->FullName() );
        delete fp;

        iBeepOncePlayer = CPhoneAudioPlayer::NewL(
            fileName,
            KAudioPriorityPhoneCall,
            KAudioPrefIncomingCall,
            *this,
            EPlayerBeepOnce );
#else
        iBeepOncePlayer = CPhoneAudioPlayer::NewSeqL(
            KPhoneBeepSequence(),
            KAudioPriorityPhoneCall,
            KAudioPrefIncomingCall,
            *this,
            EPlayerBeepOnce );
#endif // __WINS__
        }

    // Construct Silent Player.
    else if ( aId == EPlayerSilent )
        {
#ifdef __WINS__
        TParse* fp = new (ELeave) TParse();
        fp->Set( KPhoneSilentSoundFile, &KDC_RESOURCE_FILES_DIR, NULL );
        TFileName fileName( fp->FullName() );
        delete fp;

        iSilentPlayer = CPhoneAudioPlayer::NewL(
            fileName,
            KAudioPriorityPhoneCall,
            KAudioPrefIncomingCall,
            *this,
            EPlayerSilent );
#else
        iSilentPlayer = CPhoneAudioPlayer::NewSeqL(
            KPhoneNoSoundSequence(),
            KAudioPriorityPhoneCall,
            KAudioPrefIncomingCall,
            *this,
            EPlayerSilent );
#endif // __WINS__
        }
    // Construct unsecure VoIP tone player.
    else if ( aId == EPlayerUnsecureVoIP )
        {
#ifdef __WINS__
        TParse* fp = new (ELeave) TParse();
        fp->Set( KPhoneBeepOnceSoundFile, &KDC_RESOURCE_FILES_DIR, NULL );
        TFileName fileName( fp->FullName() );
        delete fp;

        iUnsecureVoIPTonePlayer = CPhoneAudioPlayer::NewL(
            fileName,
            KAudioPrioritySpecialInformationTone,
            KAudioPrefSpecialInformation,
            *this,
            EPlayerUnsecureVoIP );
#else
        iUnsecureVoIPTonePlayer = CPhoneAudioPlayer::NewSeqL(
            KPhoneUnsecureVoIPCall(),
            KAudioPrioritySpecialInformationTone,
            KAudioPrefSpecialInformation,
            *this,
            EPlayerUnsecureVoIP );
#endif // __WINS__
        }

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::ConstructSequencePlayerL >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::CleanupPlayers
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::CleanupPlayers()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::CleanupPlayers <" );

    delete iAudioPlayer;
    iAudioPlayer = NULL;

    delete iDefaultPlayer;
    iDefaultPlayer = NULL;

    delete iBeepOncePlayer;
    iBeepOncePlayer = NULL;

    delete iSilentPlayer;
    iSilentPlayer = NULL;

    delete iUnsecureVoIPTonePlayer;
    iUnsecureVoIPTonePlayer = NULL;

    delete iBackupPlayer;
    iBackupPlayer = NULL;

    delete iMdaServer;
    iMdaServer = NULL;

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::CleanupPlayers >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::DeletePlayerAsync
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::DeletePlayerAsync( TPlayerId aPlayer )
    {
    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::DeletePlayerAsync(%d) <", aPlayer );

    __ASSERT_DEBUG( aPlayer >= EPlayerFirst && aPlayer < iAsyncDeletePlayers.Count(),
        Panic( EPhoneViewInvariant ) );

    // Player is not deleted here, but in RunL.
    Cancel();

    if ( iAsyncDeletePlayers[ aPlayer ] )
        {
        delete iAsyncDeletePlayers[ aPlayer ];
        iAsyncDeletePlayers[ aPlayer ] = NULL;
        }

    CPhoneAudioPlayer** players[] =
        {
        &iAudioPlayer,
        &iBeepOncePlayer,
        &iSilentPlayer,
        &iUnsecureVoIPTonePlayer,
        &iDefaultPlayer,
        &iBackupPlayer,
        &iTtsPlayer
        };

    __ASSERT_DEBUG( iAsyncDeletePlayers.Count() > aPlayer, Panic( EPhoneUtilsIndexOutOfBounds ) );

    iAsyncDeletePlayers[ aPlayer ] = *(players[ aPlayer ]);
    *(players[ aPlayer ]) = NULL;

    iState = EDeletingAudioPlayer;

    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    SetActive();

    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::DeletePlayerAsync(%d) >", aPlayer );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::RunL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::RunL()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::RunL <" );

    switch ( iState )
        {
        case EDeletingAudioPlayer:
            {
            for ( TInt i = EPlayerFirst; i <= EPlayerLast; i++ )
                {
                delete iAsyncDeletePlayers[ i ];
                iAsyncDeletePlayers[ i ] = NULL;
                }
            }
            break;

        case EIdleState:
        default:
            break;
        } // switch iState

    iState = EIdleState;

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::RunL >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::DoCancel
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::DoCancel()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::DoCancel" );
    // Request is completed immediately before SetActive.
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::HandleTtsDelayTimeout
// -----------------------------------------------------------------------------
//
TInt CPhoneRingingToneSrvPlayerAO::HandleTtsDelayTimeout( TAny* aObject )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandleTtsDelayTimeout <" );
    static_cast<CPhoneRingingToneSrvPlayerAO*>( aObject )->
        DoHandleTtsDelayTimeout();

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::HandleTtsDelayTimeout >" );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::DoHandleTtsDelayTimeout
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::DoHandleTtsDelayTimeout()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::DoHandleTtsDelayTimeout()" );
    PHONEUIVIEW_PRINTF2(
        "CPhoneRingingToneSrvPlayerAO::DoHandleTtsDelayTimeout - iTtsRingingType(%d), iTTsTimeOutCounter(%d)",
        iTtsRingingType, iTTsTimeOutCounter );

    CPhoneAudioPlayer* currPlayer = GetCurrentlyActiveAudioPlayerWithTTs();

    if ( !currPlayer )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::DoHandleTtsDelayTimeout - null current player" );
        return;
        }
    TInt volume( 0 );
     //Ascending case. TTs player needs to be ascending.
    if ( iTtsRingingType == EProfileRingingTypeAscending )
        {
        //Volume needs to be different in different ascending steps
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::DoHandleTtsDelayTimeout - play ascending" );
        if ( !iTTsTimeOutCounter )
            {
            //We are saying callers name for the first time in ascending mode.
            //Say callers name by volume level KTtsVolumeMin.
            volume = KTtsVolumeMin;

            iTtsPlayer->Play(
                ConvertRingingType( EProfileRingingTypeRingingOnce ),
                volume,
                iTtsToneToBePlayed );
            }
        else
            {
            //Checks the case that ringingtone is very quiet. Then do not play
            //TTS too loud
            volume = iTtsVolume < KTtsVolumeMin ? KTtsVolumeMin : KTtsVolumeAscendingRepeat;

            //We are saying callers name for the second time in ascending mode.
            //Say callers name by volume level KTtsVolumeAscendingRepeat and decrease current players volume
            //to KPlayerVolumeAscendingRepeat. RampTime is zero
            currPlayer->SetNewVolumeAndRamptime( KPlayerVolumeAscendingRepeat, 0 );
            iTtsPlayer->Play(
                ConvertRingingType( EProfileRingingTypeRingingOnce ),
                volume,
                iTtsToneToBePlayed );
            }

        }
    else //Normal ringing case.
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::DoHandleTtsDelayTimeout - play normal" );
        currPlayer->SetNewVolumeAndRamptime( iTtsVolume-5, 0 );
        iTtsPlayer->Play(
            ConvertRingingType( EProfileRingingTypeRingingOnce ),
            iTtsVolume,
            iTtsToneToBePlayed );
        }

     iTTsTimeOutCounter++;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::AddTtsPlaybackIfNeeded
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::AddTtsPlaybackIfNeeded(
        TProfileRingingType /*aRingingType*/ )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::AddTtsPlaybackIfNeeded" );
    if ( iTtsToneToBePlayed )
        {
        iTtsDelayIndex = 0;
        iTtsDelaysCount = KPhoneTtsDelaysCount;
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::ExtendedSecurity
// -----------------------------------------------------------------------------
//
TBool CPhoneRingingToneSrvPlayerAO::ExtendedSecurity() const
    {
    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::ExtendedSecurity: %d", iExtSecNeeded );
    return iExtSecNeeded;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::SolveNewVolumeAndRamptime
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::SolveNewVolumeAndRamptime( TTtsStatus aStatus )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::SolveNewVolumeAndRamptime <" );

    CPhoneAudioPlayer* currPlayer = NULL;
    currPlayer = GetCurrentlyActiveAudioPlayerWithTTs();
    if ( !currPlayer )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::SolveNewVolumeAndRamptime - null current player" );
        return;
        }
    TInt ramptime( 0 );
    //Here is handled the cases when Callers name is said. Sequence is:
    //3 sec. tone + name + 4 sec. tone + name +the tone until the end.
    switch ( aStatus )
        {
        case ESaidOnce:
            if ( iTtsRingingType == EProfileRingingTypeAscending )
                {
                //The ramptime could be ( 4 )* KPhoneTtsAscendingStep but now
                //we are setting this to 0 because when Say callers name is said
                //for first time: play ringing tone on level 3 for four secs.
                ramptime = 0;
                PHONEUIVIEW_PRINTF(
                    "CPhoneRingingToneSrvPlayerAO::SolveNewVolumeAndRamptime - said once ascending - ramptime(%d)",
                    ramptime );
                currPlayer->SetNewVolumeAndRamptime( KPlayerVolumeAscendingRepeat, ramptime );
                }
            else
                {
                 //Normal ringingtone case. Adjust volume back to profile level.
                PHONEUIVIEW_PRINTF(
                    "CPhoneRingingToneSrvPlayerAO::SolveNewVolumeAndRamptime - said once normal - ramptime(%d)",
                    ramptime );
                currPlayer->SetNewVolumeAndRamptime( iTtsVolume, 0 );
                }
             break;

        case ESaidTwice:
            if ( iTtsRingingType == EProfileRingingTypeAscending )
                {
                TInt vol = iTtsVolume;
                if ( vol > KPlayerVolumeAscendingRepeat )
                    {
                    vol = vol - KTtsVolumeAscendingDecrease;
                    }

                //TTS playing complete for second time. increase tone player volume.
                ramptime = ( vol )*KPhoneTtsAscendingStep;
                PHONEUIVIEW_PRINTF(
                    "CPhoneRingingToneSrvPlayerAO::SolveNewVolumeAndRamptime - said twice ascending - ramptime(%d)",
                    ramptime );
                currPlayer->SetNewVolumeAndRamptime( iTtsVolume, ramptime );
                }
            else
                {
                 //Normal ringingtone case. Adjust volume back to profile level.
                PHONEUIVIEW_PRINTF(
                    "CPhoneRingingToneSrvPlayerAO::SolveNewVolumeAndRamptime - said twice normal - ramptime(%d)",
                    ramptime );
                currPlayer->SetNewVolumeAndRamptime( iTtsVolume, 0 );
                }
            break;

        default:
            break;
        }
    }


// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::GetCurrentlyActiveAudioPlayerWithTTs
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer*
CPhoneRingingToneSrvPlayerAO::GetCurrentlyActiveAudioPlayerWithTTs()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::GetCurrentlyActiveAudioPlayerWithTTs" );

    if( iAudioPlayer )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::GetCurrentlyActiveAudioPlayerWithTTs - audio player" );
        return iAudioPlayer;
        }
    else if ( iDefaultPlayer )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::GetCurrentlyActiveAudioPlayerWithTTs - default player" );
        return iDefaultPlayer;
        }
    else if( iBackupPlayer )
        {
        PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::GetCurrentlyActiveAudioPlayerWithTTs - backup player" );
        return iBackupPlayer;
        }
    else
        {
        return NULL;
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::ReStartRingingTonePlayer
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::ReStartRingingTonePlayer()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::ReStartRingingTonePlayer <" );

    //First get currently active player which plays among TTS player.
    CPhoneAudioPlayer* currPlayer = NULL;
    currPlayer = GetCurrentlyActiveAudioPlayerWithTTs();
    if ( currPlayer )
        {
        currPlayer->ReStartPlaying();
        }

    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::ReStartRingingTonePlayer >" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::CheckAndHandleToneSizeLimit
// -----------------------------------------------------------------------------
//
TBool CPhoneRingingToneSrvPlayerAO::CheckAndHandleToneSizeLimit(
     CPhoneRingingTone* aRingingTone )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::CheckAndHandleToneSizeLimit" );
    TBool bValidSize = ETrue;

    // If user has somehow managed to get a too large file as ringing tone,
    // play default tone instead.
    if ( iToneFileSizeLimitKB )
        {
        if ( CheckToneFileSize( aRingingTone->FileName(), iToneFileSizeLimitKB) != KErrNone )
            {
            bValidSize = EFalse;
            }
        }
    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::CheckAndHandleToneSizeLimit - bValidSize(%d)", bValidSize );
    return bValidSize;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::CheckToneFileSize
// -----------------------------------------------------------------------------
//
TInt CPhoneRingingToneSrvPlayerAO::CheckToneFileSize( const TDesC& aFile, TInt aSizeLimitKB )
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::CheckToneFileSize" );

    // Get file size
    TInt size = 0;
    TInt error = KErrNone;

    TEntry entry;
    if ( KErrNone == iFs.Entry( aFile, entry ))
        {
        size = entry.iSize;
        }

    // Check
    aSizeLimitKB *= Kkilo;
    if ( aSizeLimitKB && size > aSizeLimitKB )
        {
        error = KErrTooBig;
        }

    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::CheckToneFileSize: size = %d", size );
    return error;
    }


// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::ActiveAudioPlayer
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer* CPhoneRingingToneSrvPlayerAO::ActiveAudioPlayer()
    {
    PHONEUIVIEW_PRINT( "CPhoneRingingToneSrvPlayerAO::ActiveAudioPlayer" );

    switch( iTonePlayingStatus )
        {
        case EAudioTonePlaying:
            if ( iAudioPlayer )
                {
                return iAudioPlayer;
                }
            break;

        case EDefaultTonePlaying:
            if ( iDefaultPlayer )
                {
                return iDefaultPlayer;
                }
            break;

        case EBeepOnce:
            if ( iBeepOncePlayer )
                {
                return iBeepOncePlayer;
                }
            break;

        case EBackupTonePlaying:
            if ( iBackupPlayer )
                {
                return iBackupPlayer;
                }
            break;

        case ESilentTonePlaying:
            if ( iSilentPlayer )
                {
                return iSilentPlayer;
                }
            break;

        case EUnsecureVoIPTonePlaying:
            if ( iUnsecureVoIPTonePlayer )
                {
                return iUnsecureVoIPTonePlayer;
                }
            break;

        default:
            break;
        }

    return NULL;
    }


// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::SetExtendedSecurityNeeded
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::SetExtendedSecurityNeeded( TBool aNeeded )
    {
    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::SetExtendedSecurityNeeded: %d", aNeeded );

    iExtSecNeeded = aNeeded;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::SetToneFileSizeLimit
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::SetToneFileSizeLimit( TInt aLimit )
    {
    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::SetToneFileSizeLimit: %d", aLimit );

    iToneFileSizeLimitKB = aLimit;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::SetRingingToneL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::SetRingingToneL( const TDesC& aFilename )
    {
    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::SetRingingToneL: %S", &aFilename );

    if ( !iAudioVideoRingingTone )
        {
        iAudioVideoRingingTone = CPhoneRingingTone::NewL( aFilename );
        }
    else if ( iAudioVideoRingingTone->FileName().Compare( aFilename ) )
        {
        iAudioVideoRingingTone->SetFileName( aFilename );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingToneSrvPlayerAO::SetDefaultRingingToneL
// -----------------------------------------------------------------------------
//
void CPhoneRingingToneSrvPlayerAO::SetDefaultRingingToneL( const TDesC& aFilename )
    {
    PHONEUIVIEW_PRINTF( "CPhoneRingingToneSrvPlayerAO::SetDefaultRingingToneL: %S", &aFilename );

    if ( !iDefaultRingingTone )
        {
        iDefaultRingingTone = CPhoneRingingTone::NewL( aFilename );
        }
    else if ( iDefaultRingingTone->FileName().Compare( aFilename ) )
        {
        iDefaultRingingTone->SetFileName( aFilename );
        }
    }

//  End of File
