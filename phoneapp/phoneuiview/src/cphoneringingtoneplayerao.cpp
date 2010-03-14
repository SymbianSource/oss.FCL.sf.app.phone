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
* Description:  Ringing tone player.
*
*/


// INCLUDE FILES
#include "cphoneringingtoneplayerao.h"

#include <cntdef.h>
#include <f32file.h>
#include <featmgr.h>
#include <utility.h>
#include <AudioPreference.h>
#include <data_caging_path_literals.hrh>
#include <MusicPlayerInternalCRKeys.h>
#include <ProfileEngineDomainCRKeys.h>
#include <telephonyvariant.hrh>
#include <UikonInternalPSKeys.h>
#include <DRMHelperServerInternalCRKeys.h>
#include <DRMHelper.h>
#include <activeidle2domainpskeys.h>

#include "phoneui.pan"
#include "phoneui.hrh"
#include "mphoneringingtoneobserver.h"
#include "cphonetimer.h"
#include "cphonecenrepproxy.h"
#include "phoneconstants.h"
#include "phonelogger.h"
#include "cphoneringingtone.h"
#include "tphonecmdparamringtone.h"
#include "cphonepubsubproxy.h"
#include "cphonecenrepproxy.h"
#include "cphonemediatorsender.h"

// CONSTANTS

// Minimum acceptable length of parsed DRM message.
const TInt KPhoneMinDRMTextLength = 3;
// Value field of parsed DRM string.

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::NewL
// -----------------------------------------------------------------------------
//
CPhoneRingingTonePlayerAO* CPhoneRingingTonePlayerAO::NewL()
    {
    CPhoneRingingTonePlayerAO* self = 
        new ( ELeave ) CPhoneRingingTonePlayerAO();
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::CPhoneRingingTonePlayerAO
// -----------------------------------------------------------------------------
//
CPhoneRingingTonePlayerAO::CPhoneRingingTonePlayerAO() :
    CActive( CActive::EPriorityStandard ),
    iAsyncDeletePlayers( EPlayerCount )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::ConstructL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ConstructL()" );
    
    for ( TInt i = EPlayerFirst; i <= EPlayerLast; i++ )
        {
        User::LeaveIfError( 
            iAsyncDeletePlayers.Append( NULL ) );
        }

    // Construct timer.
    iTimer = CPhoneTimer::NewL();
    iTtsDelayTimer = CPhoneTimer::NewL();

    // Checks if DRM extend security is needed
    TInt extSecNeeded = KErrNone;
    TInt err = CPhoneCenRepProxy::Instance()->GetInt(
        KCRUidMusicPlayerFeatures,
        KRequireDRMInPlayback,
        extSecNeeded );
 
    iExtSecNeeded = ( ( err == KErrNone ) && extSecNeeded ) ? ETrue : EFalse;
    
    
    CPhoneCenRepProxy::Instance()->NotifyChangeL(
        KCRUidDRMHelperServer, 
        KDRMHelperServerNotificationPassive,
        this );
    

    // Check video player configuration.
    iArbitraryVideoScaling = 
    CPhoneCenRepProxy::Instance()->IsTelephonyFeatureSupported( 
            KTelephonyLVFlagArbitraryVideoScaling );
 
    // Construct the default ringing tone
    TBuf<KMaxFileName> defaultTone;
    err = CPhoneCenRepProxy::Instance()->GetString(            
        KCRUidProfileEngine,
        KProEngDefaultRingingTone,
        defaultTone );

    if ( err == KErrNone )
        {
        iDefaultRingingTone = CPhoneRingingTone::NewL( defaultTone );
        }

    ConstructPlayers();
    
    GetMaxToneFileSizeL( iToneFileSizeLimitKB );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::~CPhoneRingingTonePlayerAO
// -----------------------------------------------------------------------------
//
CPhoneRingingTonePlayerAO::~CPhoneRingingTonePlayerAO()
    {
    Cancel();
    
    iAsyncDeletePlayers.ResetAndDestroy();
    iAsyncDeletePlayers.Close();
        
    CleanupPlayers();

    delete iTimer;
    delete iTtsDelayTimer;
    delete iDefaultRingingTone;
    delete iAudioVideoRingingTone;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayRingToneL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayRingToneL( 
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayRingToneL()" );

    if ( aCommandParam->ParamId() == 
        TPhoneCommandParam::EPhoneParamIdRingTone )
        {
        TPhoneCmdParamRingTone* ringToneParam = 
            static_cast<TPhoneCmdParamRingTone*>( aCommandParam );

        const TInt volume = ringToneParam->Volume();
        TProfileRingingType ringingType = 
            static_cast<TProfileRingingType>( ringToneParam->RingingType() );
        
        if ( !iAudioVideoRingingTone )
            {
            iAudioVideoRingingTone = CPhoneRingingTone::NewL(
                ringToneParam->RingTone() );
            }
        else if ( iAudioVideoRingingTone->FileName().Compare(
            ringToneParam->RingTone() ) ) 
            {
            iAudioVideoRingingTone->SetFileName(
                ringToneParam->RingTone() );
            }
        
        // Caller contact text and image and image has
        // higher priority than video ringing tone set for caller.
        if ( iAudioVideoRingingTone->IsVideoRingingTone() &&
             ringToneParam->IsCallerImage() )
            {
            // Play only audio from video ringingtone
            PlayAudioRingTone(
                    volume,
                    ringingType );
            }
        else if ( iAudioVideoRingingTone->IsVideoRingingTone() &&
                !ringToneParam->IsCallerText() )
            {            
            // Play video ring tone
            const TBool ringToneIsPersonal = ringToneParam->Type() ==
                EPhoneRingTonePersonal;
            PlayVideoRingingTone( 
                *iAudioVideoRingingTone, 
                volume, 
                ringingType,
                ringToneIsPersonal );
            }
        else
            {
            // Play text to speech, if available
            if ( ringToneParam->TextToSay().Length() )
                {
                PlayTtsTone(
                    ringToneParam->TextToSay(),
                    volume,
                    ringingType );
                }
            // Play audio ring tone
            PlayAudioRingTone( 
                volume, 
                ringingType );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayAudioRingTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayAudioRingTone( 
    TInt aVolume, 
    TProfileRingingType aRingingType )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayAudioRingTone()" );
    __PHONELOG2( 
        EBasic,
        EPhoneUIView, 
        "CPhoneRingingTonePlayerAO::PlayAudioRingTone - aVolume(%d), aRingingType(%d)",
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
            __PHONELOG( 
                EBasic, 
                EPhoneUIView, 
                "CPhoneRingingTonePlayerAO::PlayAudioRingTone - Start playing audio" );
            iVolume = aVolume;
            iRingingType = aRingingType;
            AddTtsPlaybackIfNeeded( aRingingType );
            iTonePlayingStatus = EAudioTonePlaying;
            iAudioPlayer->Play( 
                ConvertRingingType( aRingingType ),
                aVolume,
                iTtsToneToBePlayed );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::BeepOnce
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::BeepOnce( TInt aVolume )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::BeepOnce()" );
    __PHONELOG1( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::BeepOnce - aVolume(%d)",
        aVolume );

    if ( !iBeepOncePlayer )
        {
        ConstructSequencePlayer( EPlayerBeepOnce );
        }

    if ( iBeepOncePlayer )
        {
        __PHONELOG( 
            EBasic, 
            EPhoneUIView, 
            "CPhoneRingingTonePlayerAO::BeepOnce - Play" );
        iTonePlayingStatus = EBeepOnce;
        iBeepOncePlayer->Play( CPhoneAudioPlayer::ETypeRingingOnce, 
            aVolume, EFalse );        
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlaySilentTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlaySilentTone()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlaySilentTone()" );

    if ( !iSilentPlayer )
        {
        ConstructSequencePlayer( EPlayerSilent );
        }

    __ASSERT_DEBUG( iSilentPlayer, Panic( EPhoneViewGeneralError ) );

    if ( iSilentPlayer )
        {
        __PHONELOG( 
            EBasic, 
            EPhoneUIView, 
            "CPhoneRingingTonePlayerAO::PlaySilentTone - play" );
        iTonePlayingStatus = ESilentTonePlaying;
        iSilentPlayer->Play( CPhoneAudioPlayer::ETypeRinging, 0, EFalse );        
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayUnsecureVoIPTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayUnsecureVoIPTone()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayUnsecureVoIPTone()" );
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
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::MuteRingingToneOnAnswer
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::MuteRingingToneOnAnswer()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::MuteRingingToneOnAnswer()" );
    
    if ( iVideoPlayer &&
            ( iTonePlayingStatus == EVideoTonePlaying ||
         iTonePlayingStatus == EPersonalVideoTonePlaying ) )
        {
        // Mute the video audio
        iVideoPlayer->MuteVideoRingTone();
        iTonePlayingStatus = ESilentVideoTonePlaying;
        iVolume = 0;
        }
    else
        {
        DoMuteRingingTone();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::MuteRingingTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::MuteRingingTone()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::MuteRingingTone()" );
    
    if ( iVideoPlayer &&
            ( iTonePlayingStatus == EVideoTonePlaying ||
         iTonePlayingStatus == EPersonalVideoTonePlaying )  )
        {
        // Mute the video audio
        iVideoPlayer->MuteVideoRingTone();
        iTonePlayingStatus = ESilentVideoTonePlaying;
        iVolume = 0;
        }
    else
        {
        StopPlaying();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::StopPlaying
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::StopPlaying()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::StopPlaying()" );

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
            if( iAudioPlayer )
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
            if( iBeepOncePlayer )
                {
                iBeepOncePlayer->StopPlaying();
                delete iBeepOncePlayer;
                iBeepOncePlayer = NULL;
                }
            break;
        case EBackupTonePlaying:
            if( iBackupPlayer )
                {
                iBackupPlayer->StopPlaying();
                }
            break;
        case ESilentTonePlaying:
            if( iSilentPlayer )
                {
                iSilentPlayer->StopPlaying();
                delete iSilentPlayer;
                iSilentPlayer = NULL;
                }
            break;
        case EUnsecureVoIPTonePlaying:
            if( iUnsecureVoIPTonePlayer )
                {
                iUnsecureVoIPTonePlayer->StopPlaying();
                delete iUnsecureVoIPTonePlayer;
                iUnsecureVoIPTonePlayer = NULL;
                }
            break;
        case EVideoTonePlaying: // video ringing tone
        case EPersonalVideoTonePlaying:
        case ESilentVideoTonePlaying:
            if ( iBeepOncePlayer != NULL )
                {
                iBeepOncePlayer->StopPlaying();
                delete iBeepOncePlayer;
                iBeepOncePlayer = NULL;                    
                }
                
            if ( iSilentPlayer != NULL )
                {
                iSilentPlayer->StopPlaying();
                delete iSilentPlayer;
                iSilentPlayer = NULL;                    
                }    
            if ( iVideoPlayer )
                {
                iVideoPlayer->StopVideoRingTone();
                }
            if ( iAudioVideoRingingTone )
                {
                delete iAudioVideoRingingTone;
                iAudioVideoRingingTone = NULL;    
                }
            break;
        case EIdle:
            // Do nothing here.
            return;
        default:
            Panic( EPhoneViewCaseNotHandled );
            break;
        }

    iTonePlayingStatus = EIdle;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayTtsTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayTtsTone( 
    const TDesC& aTextToSay, 
    TInt aVolume,
    TProfileRingingType aRingingType )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayTtsTone()" );
    __PHONELOG2( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayTtsTone - aVolume(%d), aRingingType(%d)",
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

    if ( preparedString )
        {
        TPtr pPreparedString = preparedString->Des();
        pPreparedString.Append( ttsHeader );
        pPreparedString.Append( aTextToSay );
           
        __PHONELOG1( 
            EBasic, 
            EPhoneUIView, 
            "CPhoneRingingTonePlayerAO::PlayTtsTone - about to say %S",
            &pPreparedString );

        TRAPD( 
            error, 
            iTtsPlayer = CPhoneAudioPlayer::NewTtsL( 
               *preparedString, 
               KAudioPriorityPhoneCall, 
               KAudioPrefTextToSpeechCallHardCoded,
               *this, 
               EPlayerTts ) );
        if( error == KErrNone ) 
            {
            // To be played when default or personal tone will be played
            iTtsToneToBePlayed = ETrue;
            }
        __PHONELOG2(
            EBasic,
            EPhoneUIView,
            "CPhoneRingingTonePlayerAO::PlayTtsTone - error(%d), iTtsPhoneToBePlayed(%d)",
            error, 
            iTtsToneToBePlayed );

        delete preparedString;  
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleAudioPlayerError
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleAudioPlayerError( 
    TPhoneAudioPlayerErrorEvent /*aEvent*/, 
    TInt aError, 
    TInt /*aId*/ )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleAudioPlayerError()" );
    // This is called from CPhoneAudioPlayer - we do not 
    // want to delete instances from which this method
    // is called.
    
     __PHONELOG1( EBasic, 
                 EPhoneUIView, 
                 "CPhoneRingingTonePlayerAO::HandleAudioPlayerError - error (%d)",
                 aError ); 

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
// CPhoneRingingTonePlayerAO::HandleAudioPlayerInitComplete
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleAudioPlayerInitComplete( TInt aId )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleAudioPlayerInitComplete()" );
    // This is called from CPhoneAudioPlayer - we do not 
    // want to delete instances from which this method
    // is called. 

    if ( iTonePlayingStatus == EAudioTonePlaying && aId == EPlayerAudio ||
        iTonePlayingStatus == EDefaultTonePlaying && aId == EPlayerDefault )
        {
        iTimer->Cancel();
        }
   
    if( iTtsToneToBePlayed && ( aId != EPlayerTts ) )  
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
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandlePlayingComplete
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandlePlayingComplete( TInt aId )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandlePlayingComplete()" );
    // This is called from CPhoneAudioPlayer - we do not 
    // want to delete instances from which this method
    // is called. 

    __PHONELOG2(
        EBasic, 
        EPhoneUIView,
        "CPhoneRingingTonePlayerAO::HandlePlayingComplete - aId(%d), iRingingType(%d)",
        aId,
        iRingingType );
    __PHONELOG2(
        EBasic, 
        EPhoneUIView,
        "CPhoneRingingTonePlayerAO::HandlePlayingComplete - iTtsToneToBePlayed(%d), iTtsDelayIndex(%d)",
        iTtsToneToBePlayed,
        iTtsDelayIndex );

    if ( aId == EPlayerTts ) 
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::HandlePlayingComplete - resuming after TTS" );

        // TTS playing complete, normal tone player should volume up.
        if ( ( iTtsToneToBePlayed ) && ( ++iTtsDelayIndex < iTtsDelaysCount ) )  
            {
            __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::HandlePlayingComplete - resuming after TTS first time" );
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
            __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::HandlePlayingComplete - resuming after TTS second time" );
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
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::HandlePlayingComplete - ring once and TTS" );
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
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::HandlePlayingComplete - ringing and TTS" );
        if ( iTtsDelayIndex == iTtsDelaysCount )
            {
            //Case: Ringingtype is EProfileRingingTypeRinging. New TTS
            //iterations are required.    
            __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::HandlePlayingComplete - need to restart TTS sequence" );
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

    __PHONELOG1( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::HandlePlayingComplete - delete player - iTonePlayingStatus(%d)",
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
            
        case EVideoTonePlaying:
        case EPersonalVideoTonePlaying:
            DeletePlayerAsync( EPlayerBeepOnce );
            iBeepOncePlayer = NULL;
            return; // beep once + video
            
        case EBackupTonePlaying:
            break;
            
        default:
            Panic( EPhoneViewCaseNotHandled );
            break;
        }

    iTonePlayingStatus = EIdle;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleTimeOutL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleTimeOutL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleTimeOutL()" );

    // Guarding timer for startup has expired
    if ( iTonePlayingStatus == EVideoTonePlaying ||
       iTonePlayingStatus == ESilentVideoTonePlaying ||   
       iTonePlayingStatus == EPersonalVideoTonePlaying )
      {
        iVideoPlayer->CancelVideoRingTone(); // close video ringtone display
       PlayDefaultTone( iVolume, iRingingType );
      }
    else
      {
      // Start new audio player synchronously.
      DoHandlePlayerError( ETrue, ETrue );
      }			
		
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::DoHandlePlayerError
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::DoHandlePlayerError(
    TBool aDelete,
    TBool aSync )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::DoHandlePlayerError()" );
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
            PlayAudioRingTone( iVolume, iRingingType );
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
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayDefaultTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayDefaultTone( 
    TInt aVolume, 
    TProfileRingingType aRingingType )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayDefaultTone()" );

    if( aRingingType == EProfileRingingTypeSilent ||
        aRingingType == EProfileRingingTypeBeepOnce )
        {
        return;
        }
        
    if ( !iDefaultRingingTone )
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayDefaultTone - default tone does not exist, play backup..." );
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
            iTimer->After( KPhoneMaxRingingWaiting, this );
            }
        }

    // Start playing.
    if ( !iDefaultPlayer )
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayDefaultTone - player not constructed, play backup..." );
        iTimer->Cancel();
        PlayBackupTone( aVolume, aRingingType );
        }
    else
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayDefaultTone - play" );
        iVolume = aVolume;
        iRingingType = aRingingType;
        iTonePlayingStatus = EDefaultTonePlaying;
        iDefaultPlayer->Play( 
            ConvertRingingType( aRingingType ), aVolume, iTtsToneToBePlayed );
        }

    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayBackupTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayBackupTone( 
    TInt aVolume, 
    TProfileRingingType aRingingType )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayBackupTone()" );
    __ASSERT_DEBUG( iBackupPlayer, Panic( EPhoneViewGeneralError ) );

    if ( !iBackupPlayer )
        {
        ConstructPlayers();
        }

    if ( iBackupPlayer )
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayBackupTone - play" );
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
// CPhoneRingingTonePlayerAO::ConstructTonePlayer
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer* CPhoneRingingTonePlayerAO::ConstructTonePlayer( 
    const CPhoneRingingTone& aRingingTone,
    TInt aId )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ConstructTonePlayer()" );
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
// CPhoneRingingTonePlayerAO::ConvertRingingType
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer::TRingingType CPhoneRingingTonePlayerAO::ConvertRingingType(
    TProfileRingingType aRingingType )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ConvertRingingType()" );

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
// CPhoneRingingTonePlayerAO::ConvertVideoRingingType
// -----------------------------------------------------------------------------
//
MPhoneVideoPlayer::TPlayMode CPhoneRingingTonePlayerAO::ConvertVideoRingingType(
    TProfileRingingType aRingingType )
    {
    MPhoneVideoPlayer::TPlayMode playMode;
        
    switch ( aRingingType )
        {
        case EProfileRingingTypeAscending:
            playMode = MPhoneVideoPlayer::EPlayAscending;
            break;
        case EProfileRingingTypeRingingOnce:
            playMode = MPhoneVideoPlayer::EPlayOnce;
            break;
        case EProfileRingingTypeRinging:
        default:
            playMode = MPhoneVideoPlayer::EPlayInLoop;
            break;
        }
    
    return playMode;
    }


// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::ConstructPlayers
// -----------------------------------------------------------------------------
//
TInt CPhoneRingingTonePlayerAO::ConstructPlayers()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ConstructPlayers()" );
    TRAPD( err, ConstructPlayersL() );
    return err;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::ConstructPlayersL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::ConstructPlayersL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ConstructPlayersL()" );

    // Construct players.
    if ( !iMdaServer )
        {
        iMdaServer = CMdaServer::NewL();
        }

    // Construct Backup player.
    if ( !iBackupPlayer )
        {
        TFileName defaultSoundFile( KDriveZ );
        defaultSoundFile.Append( KPhoneDefaultSoundFile );
        iBackupPlayer = CPhoneAudioPlayer::NewL( 
            defaultSoundFile,
            KAudioPriorityPhoneCall,
            KAudioPrefIncomingCall,
            *this,
            EPlayerBackup,
            iMdaServer 
            );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::ConstructSequencePlayer
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::ConstructSequencePlayer( TPlayerId aId )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ConstructSequencePlayer()" );

    // Construct players.
    TRAPD( err, ConstructSequencePlayerL( aId ) );
    if ( err )
        {
        __PHONELOG1( EBasic, EPhoneUIView, 
        "CPhoneRingingTonePlayerAO::ConstructSequencePlayer - err(%d)", err );    
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::ConstructSequencePlayerL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::ConstructSequencePlayerL( TPlayerId aId )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ConstructSequencePlayerL()" );
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
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::CleanupPlayers
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::CleanupPlayers()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::CleanupPlayers()" );

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
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::DeletePlayerAsync
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::DeletePlayerAsync( TPlayerId aPlayer )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::DeletePlayerAsync()" );
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
    iAsyncDeletePlayers[ aPlayer ] = 
        *(players[ aPlayer ]);
    *(players[ aPlayer ]) = NULL;

    iState = EDeletingAudioPlayer;

    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::RunL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::RunL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::RunL()" );

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

      case EPlayingDefaultVideo:
            {
            PlayAudioRingTone( iVolume, iRingingType );
            }
            break;

        case EIdleState:  
        default:
            break;
        } // switch iState
    
    iState = EIdleState;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::DoCancel
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::DoCancel()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::DoCancel()" );
    // Request is completed immediately before SetActive.
    }
    
// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleTtsDelayTimeout
// -----------------------------------------------------------------------------
//
TInt CPhoneRingingTonePlayerAO::HandleTtsDelayTimeout( TAny* object )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleTtsDelayTimeout()" );
    static_cast<CPhoneRingingTonePlayerAO*>( object )->
        DoHandleTtsDelayTimeout();
    return KErrNone;
    }
        
// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::DoHandleTtsDelayTimeout
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::DoHandleTtsDelayTimeout()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::DoHandleTtsDelayTimeout()" );
    __PHONELOG2( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::DoHandleTtsDelayTimeout - iTtsRingingType(%d), iTTsTimeOutCounter(%d)",
        iTtsRingingType, iTTsTimeOutCounter );

    CPhoneAudioPlayer* currPlayer = NULL;
    currPlayer = GetCurrentlyActiveAudioPlayerWithTTs();        
    
    if ( !currPlayer )
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::DoHandleTtsDelayTimeout - null current player" );
        return;
        }
    TInt volume( 0 );
     //Ascending case. TTs player needs to be ascending.
    if ( iTtsRingingType == EProfileRingingTypeAscending )
        {
        //Volume needs to be different in different ascending steps
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::DoHandleTtsDelayTimeout - play ascending" ); 
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
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::DoHandleTtsDelayTimeout - play normal" ); 
        currPlayer->SetNewVolumeAndRamptime( iTtsVolume-5, 0 );                       
        iTtsPlayer->Play(
            ConvertRingingType( EProfileRingingTypeRingingOnce ),
            iTtsVolume,
            iTtsToneToBePlayed );
        }
      
     iTTsTimeOutCounter++;
    }
    
// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::AddTtsPlaybackIfNeeded
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::AddTtsPlaybackIfNeeded( 
        TProfileRingingType /*aRingingType*/ )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::AddTtsPlaybackIfNeeded()" );
    if ( iTtsToneToBePlayed )   
        {
        iTtsDelayIndex = 0;
        iTtsDelaysCount = KPhoneTtsDelaysCount;
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::SetVideoPlayer
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::SetVideoPlayer( 
    MPhoneVideoPlayer* aVideoPlayer )
    {
    iVideoPlayer = aVideoPlayer;    
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayVideoRingingTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayVideoRingingTone( 
    const CPhoneRingingTone& aRingingTone, 
    TInt aVolume, 
    TProfileRingingType aRingingType,
    TBool aPersonalTone )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayVideoRingingTone()" );
    
    __ASSERT_DEBUG( iVideoPlayer, Panic( EPhoneViewGeneralError ) );
    
    iVolume = aVolume;
    iRingingType = aRingingType;
    TBool startTimer( ETrue );
    
     // Extended security check
    if ( ExtendedSecurity() )
        {
        if ( !aRingingTone.IsFileInRom() &&
             !aRingingTone.IsFileDrmProtected() )
            {
            __PHONELOG( EBasic, 
                        EPhoneUIView, 
                        "CPhoneRingingTonePlayerAO::HandleVideoPlayerError - PermissionDenied" );
            PlayDefaultTone( iVolume, iRingingType );
            return;
            }
        __PHONELOG( EBasic, 
                    EPhoneUIView, 
                    "CPhoneRingingTonePlayerAO::HandleVideoPlayerError - ExtSecChk ok" );            
            }
 
    __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayVideoRingingTone - play" );
    switch( aRingingType )
        {
        case EProfileRingingTypeSilent:
            iVideoPlayer->PlayVideoRingTone( 
                aRingingTone.FileName(),
                ConvertVideoRingingType( aRingingType ),
                0,
                iArbitraryVideoScaling,
                this );
            PlaySilentTone(); // To start vibra, video may fail.
            iVolume = 0; // for repeat
            startTimer = EFalse; // no need for fallback
            break;
        
        case EProfileRingingTypeBeepOnce:
            iVideoPlayer->PlayVideoRingTone( 
                aRingingTone.FileName(),
                ConvertVideoRingingType( aRingingType ),
                0,
                iArbitraryVideoScaling,
                this );
            BeepOnce( iVolume );
            iVolume = 0; // for repeat
            startTimer = EFalse; // no need for fallback
            break;
        default:
            iVideoPlayer->PlayVideoRingTone( 
                aRingingTone.FileName(),
                ConvertVideoRingingType( aRingingType ),
                iVolume,
                iArbitraryVideoScaling,
                this );
            break;                
        }

    if ( aPersonalTone )
        {
        iTonePlayingStatus = EPersonalVideoTonePlaying;
        }
    else
        {
        iTonePlayingStatus = EVideoTonePlaying;
        }

    if ( startTimer && !iTimer->IsActive() )
        {
        // Start timer to guard video opening
        iTimer->After( KPhoneMaxRingingWaiting, this );
        }
    
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleVideoPlayerError
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleVideoPlayerError( 
    TPhoneVideoPlayerErrorEvent /*aEvent*/,
    TInt aError )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleVideoPlayerError()" );

    if ( aError )
        {
        __PHONELOG1( EBasic, 
                     EPhoneUIView, 
                     "CPhoneRingingTonePlayerAO::HandleVideoPlayerError - error (%d)",
                     aError );        
        }
  
    // cancel guarding timer
    iTimer->Cancel();

    // to remove video window
    iVideoPlayer->CancelVideoRingTone();

    if ( iTonePlayingStatus == EPersonalVideoTonePlaying )
        {
        // Play default tone (active profile tone).
        if ( iAudioVideoRingingTone &&
             !iAudioVideoRingingTone->IsVideoRingingTone() )
            {
            PlayAudioRingTone( iVolume, iRingingType );
            }
        else // audio/video tone is video
            {
            PlayDefaultVideoAsync();
            }
        }
    else
        {
        // Play backup tone
        PlayDefaultTone( iVolume, iRingingType );
        }

    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleVideoPlayerInitComplete
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleVideoPlayerInitComplete()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleVideoPlayerInitComplete()" );
    
    // cancel guarding timer
    iTimer->Cancel();
    
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleVideoPlayerPlayingComplete
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleVideoPlayerPlayingComplete()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleVideoPlayerPlayingComplete()" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayDefaultVideoAsync
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayDefaultVideoAsync()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayDefaultVideoAsync()" );    
     
    Cancel();

    iState = EPlayingDefaultVideo; 

    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::ExtendedSecurity
// -----------------------------------------------------------------------------
//    
TBool CPhoneRingingTonePlayerAO::ExtendedSecurity() const
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ExtendedSecurity()" );
    return iExtSecNeeded;        
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::SolveNewVolumeAndRamptime
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::SolveNewVolumeAndRamptime( TTtsStatus aStatus )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::SolveNewVolumeAndRamptime()" );

    CPhoneAudioPlayer* currPlayer = NULL;
    currPlayer = GetCurrentlyActiveAudioPlayerWithTTs();
    if ( !currPlayer )
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::SolveNewVolumeAndRamptime - null current player" );
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
                __PHONELOG1( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::SolveNewVolumeAndRamptime - said once ascending - ramptime(%d)",
                    ramptime ); 
                currPlayer->SetNewVolumeAndRamptime( KPlayerVolumeAscendingRepeat, ramptime );
                }
            else
                {
                 //Normal ringingtone case. Adjust volume back to profile level. 
                __PHONELOG1( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::SolveNewVolumeAndRamptime - said once normal - ramptime(%d)",
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
                __PHONELOG1( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::SolveNewVolumeAndRamptime - said twice ascending - ramptime(%d)",
                    ramptime ); 
                currPlayer->SetNewVolumeAndRamptime( iTtsVolume, ramptime );
                }
            else
                {
                 //Normal ringingtone case. Adjust volume back to profile level. 
                __PHONELOG1( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::SolveNewVolumeAndRamptime - said twice normal - ramptime(%d)",
                    ramptime ); 
                currPlayer->SetNewVolumeAndRamptime( iTtsVolume, 0 );
                }
            break;
        
        default:
            break;
        }
    }


// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::GetCurrentlyActiveAudioPlayerWithTTs
// -----------------------------------------------------------------------------
//    
CPhoneAudioPlayer* 
CPhoneRingingTonePlayerAO::GetCurrentlyActiveAudioPlayerWithTTs()
    {    
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::GetCurrentlyActiveAudioPlayerWithTTs()" );

    if( iAudioPlayer )
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::GetCurrentlyActiveAudioPlayerWithTTs - audio player" ); 
        return iAudioPlayer;         
        }
    else if ( iDefaultPlayer )
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::GetCurrentlyActiveAudioPlayerWithTTs - default player" ); 
        return iDefaultPlayer;     
        }
    else if( iBackupPlayer )
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::GetCurrentlyActiveAudioPlayerWithTTs - backup player" ); 
        return iBackupPlayer;
        }
    else
        {
        return NULL;
        }
    }
    
// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::ReStartRingingTonePlayer
// -----------------------------------------------------------------------------
//      
void CPhoneRingingTonePlayerAO::ReStartRingingTonePlayer()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ReStartRingingTonePlayer()" );

    //First get currently active player which plays among TTS player.
    CPhoneAudioPlayer* currPlayer = NULL;
    currPlayer = GetCurrentlyActiveAudioPlayerWithTTs();   
    if ( currPlayer )
        {
        currPlayer->ReStartPlaying();
        }      
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::CheckAndHandleToneSizeLimit
// -----------------------------------------------------------------------------
//
TBool CPhoneRingingTonePlayerAO::CheckAndHandleToneSizeLimit(
     CPhoneRingingTone* aRingingTone )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::CheckAndHandleToneSizeLimit()" );
    
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
    __PHONELOG1( 
        EBasic,
        EPhoneUIView, 
        "CPhoneRingingTonePlayerAO::CheckAndHandleToneSizeLimit - bValidSize(%d)",
        bValidSize);
        
    return bValidSize;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::CheckToneFileSize
// -----------------------------------------------------------------------------
//
TInt CPhoneRingingTonePlayerAO::CheckToneFileSize( const TDesC& aFile, TInt aSizeLimitKB )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::CheckToneFileSize()" );
    
    // Get file size
    TInt size = 0;
    TInt error = KErrNone;
    RFs& fs = CCoeEnv::Static()->FsSession();       
    TEntry entry;
    if (KErrNone == fs.Entry( aFile, entry ))
		{
		size = entry.iSize;
		}    
        
    // Check
    aSizeLimitKB *= Kkilo;
    if ( aSizeLimitKB  &&  size > aSizeLimitKB )
        {
        error = KErrTooBig;
        }
    
    __PHONELOG1( 
        EBasic,
        EPhoneUIView, 
        "CPhoneRingingTonePlayerAO::CheckToneFileSize - size (%d)",
        size );    
    return error;
    }


// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::GetMaxToneFileSizeL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::GetMaxToneFileSizeL( TInt& aMaxSizeKB ) const
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::GetMaxToneFileSizeL()" );
    
    const TInt error = CPhoneCenRepProxy::Instance()->GetInt( 
            KCRUidProfileEngine,
            KProEngRingingToneMaxSize,
            aMaxSizeKB );
   
   	if ( error != KErrNone )
    	{
        aMaxSizeKB = 0;
    	}
	if ( aMaxSizeKB < 0 )
	    {
	     aMaxSizeKB = 0;
	    }
  
    __PHONELOG2( 
        EBasic,
        EPhoneUIView, 
        "CPhoneRingingTonePlayerAO::GetMaxToneFileSizeL - error (%d), aMaxSizeKB(%d)",
        error,
        aMaxSizeKB );	    
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::ActiveAudioPlayer
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer* CPhoneRingingTonePlayerAO::ActiveAudioPlayer()
	{
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ActiveAudioPlayer()" );

    switch( iTonePlayingStatus )
        {
        case EAudioTonePlaying:
            if( iAudioPlayer )
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
            if( iBeepOncePlayer )
                {
				return iBeepOncePlayer;
                }
            break;
            
        case EBackupTonePlaying:
            if( iBackupPlayer )
                {
                return iBackupPlayer;
                }
            break;
            
        case ESilentTonePlaying:
            if( iSilentPlayer )
                {
                return iSilentPlayer;
                }
            break;
            
        case EUnsecureVoIPTonePlaying:
            if( iUnsecureVoIPTonePlayer )
                {
                return iUnsecureVoIPTonePlayer;
                }
            break;
            
        case EVideoTonePlaying: // video ringing tone
        case EPersonalVideoTonePlaying:
        case ESilentVideoTonePlaying:
            if ( iBeepOncePlayer )
                {
                return iBeepOncePlayer;                    
                }    
            else if ( iSilentPlayer )
                {
                return iSilentPlayer;                    
                }    
            break;
            
        default:
            break;
        }

	return NULL;		
	}

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::DoMuteRingingTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::DoMuteRingingTone()
	{
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::DoMuteRingingTone()" );
	
	switch( iTonePlayingStatus )
		{
        case EVideoTonePlaying: // video ringing tone, fall through
        case EPersonalVideoTonePlaying: // fall through
        case ESilentVideoTonePlaying:
            if ( iVideoPlayer )
	            {
                iVideoPlayer->MuteVideoRingTone();
                return;	            	
	            }
	        break;
	        
	    default:
	    	break;
		}
	
	CPhoneAudioPlayer* activePlayer = ActiveAudioPlayer();
	if( activePlayer )
		{
		activePlayer->MutePlaying();
		}
	}

// -----------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleCenRepChangeL
// -----------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleCenRepChangeL( 
    const TUid& aUid,
    const TUint /*aId*/ )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleCenRepChangeL()");
    
    if ( aUid == KCRUidDRMHelperServer )
        {
        TBuf<256> tempBuf16( KNullDesC );
        
        User::LeaveIfError( CPhoneCenRepProxy::Instance()->GetString(
            KCRUidDRMHelperServer,
            KDRMHelperServerNotificationPassive,
            tempBuf16 ) );         
       
        const TBool idleIsTopApp = CPhonePubSubProxy::Instance()->Value(
            KPSUidAiInformation, KActiveIdleState ) == EPSAiForeground;
       
        // Quick validity check and    
        // The first Tuint8 is the times the content has been informed.
        // Note should not shown until Idle.
        if ( idleIsTopApp && tempBuf16.Length() >= KPhoneMinDRMTextLength )
            { 
            TBuf8<256> tempBuf8;
            TPtrC8 ptr((TUint8*)tempBuf16.Ptr(), tempBuf16.Size());
            HBufC8* buf8 = HBufC8::NewLC( 256 );
            buf8->Des().Copy(ptr);  
    	    // Extract URI etc. 
    	    // buf8 format:
    	    //<TUint8 aTimes><TUint8 aExpirationMark>
    	    //<TUint8 aPermissionType>
    	    //<TUint8 aAutomatedContentType><TDesC8 aContentID>
     	    TInt8 count = (TInt8)(*(buf8->Ptr())); 
    	    TChar mark = (TChar)(*(buf8->Ptr()+1));
    	    TInt8 permtype = (TChar)(*(buf8->Ptr()+2));
    	    TInt8 automcontenttype = (TChar)(*(buf8->Ptr()+3)); 
    	    TBuf8<256> curi = buf8->Right( buf8->Length()-4);
    		
    		if (curi.Ptr()[curi.Size()-1] == 0)
    		    {
    		    // The last character in the descriptor is 0, which means
    		    // that the original 16-bit desc was padded so
    		    // remove the last char...
    		    curi.SetLength(curi.Size()-1);
    		    }
    		        
            // Create DRM helper.     
            CDRMHelper* drmHelper = CDRMHelper::NewLC();            
            // Show notification.
            drmHelper->SetAutomatedType( (CDRMHelper::TDRMHelperAutomatedType)automcontenttype );
            TInt error = drmHelper->CheckRightsAmountL( curi );

            __PHONELOG1( EBasic, EPhoneUIView, 
                "CPhoneRingingTonePlayerAO::HandleCenRepChangeL > CheckRightsAmountL, error: %d"
                , error );
                           
            CleanupStack::PopAndDestroy( drmHelper ); 
            CleanupStack::PopAndDestroy( buf8 ); 
            } 
        }
    }
    
//  End of File  
