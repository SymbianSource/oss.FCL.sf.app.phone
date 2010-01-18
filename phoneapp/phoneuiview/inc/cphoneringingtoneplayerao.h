/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Active object which implements ringing tone functionality
*
*/


#ifndef CPHONERINGINGTONEPLAYERAO_H
#define CPHONERINGINGTONEPLAYERAO_H

// INCLUDES
#include <e32base.h>
#include <Profile.hrh>
#include "mphoneaudioplayerobserver.h"
#include "mphonevideoplayerobserver.h"
#include "mphonecenrepobserver.h"
#include "cphoneaudioplayer.h"
#include "mphonetimer.h"
#include "mphonevideoplayer.h"

// FORWARD DECLARATIONS
//class MPhoneRingingToneObserver;
class CPhoneAudioPlayer;
class CPhoneTimer;
class CMdaServer;
class MPhoneDisplayProvider;
class CPhoneRingingTone;
class TPhoneCommandParam;

// CLASS DECLARATION

/**
* It implements ringing tone functionality. 
* Note that when issuing Play-command to audio side, ALWAYS set the tone type
* before the Play() is called. This is to ensure that correct tone is tried to
* be played. 
*
* @since 1.0
*/
class CPhoneRingingTonePlayerAO :
    public CActive,
    public MPhoneCenRepObserver,
    private MPhoneAudioPlayerObserver,
    private MPhoneTimer,
    private MPhoneVideoPlayerObserver
    {
    public:
        // States for this active object
        enum TState
            {
            EIdleState,  
            EDeletingAudioPlayer,
            EDeletingVideoPlayer,
            EPlayingDefaultVideo
            };

    public:
        
        /**
        * Two-phased constructor.
        * @return new instance.
        */
        static CPhoneRingingTonePlayerAO* NewL();
        
        /**
        * Destructor.
        */
        ~CPhoneRingingTonePlayerAO();

        /**
        * Play ring tone
        * @param aCommandParam a command param
        */
        void PlayRingToneL( TPhoneCommandParam *aCommandParam );

        /**
        * Play audio ring tone 
        */
        void PlayAudioRingTone( 
            TInt aVolume, 
            TProfileRingingType aRingingType );

        /**
        * Playing the Beep once.
        * @param aVolume volume used to play the ringing tone.
        */
        void BeepOnce( TInt aVolume );

        /**
        * Play silent tone. No_sound.wav will be played. If vibration alert
        * is ON, it will vibrate.
        */
        void PlaySilentTone();

        /**
        * Play unsecure VoIP tone.
        * Play tone if secure call fails and call is changed to unsecure call
        * @since    S60 5.0
        */
        void PlayUnsecureVoIPTone();
        
        /**
        * Stops playing the ringing tone.
        * Method does not do anything if ringing tone is not playing.
        */
        void StopPlaying();

        /**
        * Continues video playback ( and ringing tone ) with volume 0
        * Stream not closed ( If music player was ongoing ) 
        */
        void MuteRingingToneOnAnswer();
          
        /**
        * Continues video playback with muted audio.
        * Ringing tone is stopped.
        */
        void MuteRingingTone();
          
        /**
        * Sets video player.
        * @since Series 60 v3.2
        * @param aVideoPlayer Video player reference.
        */
        void SetVideoPlayer( MPhoneVideoPlayer* aVideoPlayer );


        /**
        * Play TTS tone simultaneously with Personal or Default tones.
        * This method does not command phone to play TTS immediately. Instead 
        * it registers a request to play TTS, when Personal or default tones 
        * are played.
        *
        * @since Series 60 3.0
        * @param aTextToSay Text, that should be pronounced by the 
        * Text-To-Speech engine. This is normal descriptor, no UTF-8 
        * and no "(tts)" prefix
        * @param aVolume Volume for the TTS-tone, Does not effect the volume 
        * of the normal tone, that will be played simulteneously with the TTS
        * @param aRingingType ringing type.
        */
        void PlayTtsTone(
            const TDesC& aTextToSay,
            TInt aVolume,
            TProfileRingingType aRingingType );

        /**
        * Play video ring tone.
        * @since Series 60 3.1
        * @param aRingingTone Ringing tone to be played.
        * @param aVolume Volume used to play the ringing tone.
        * @param aRingingType Ringing type.
        */
        void PlayVideoRingingTone( 
            const CPhoneRingingTone& aRingingTone, 
            TInt aVolume, 
            TProfileRingingType aRingingType,
            TBool aPersonalTone = EFalse );

        /**
        * Checks if extended security required.
        * @since Series 60 3.1
        * @return ETrue if extended security is required.
        */
        TBool ExtendedSecurity() const;
   
        /**
        * This function is called when Mediator receives response to the sent
        * ShowMultiMediaRingingTone command.
        * @param aStatus - Standard Symbian error code indicating the
        *                  success of the command.
        */
		    void ShowMultimediaRingingToneResponseL( TInt aStatus );
		
    public: // From MPhoneCenRepObserver

        /**
        * Handle the change of the setting from Central Repository
        * @param aUid identifing the central repository UID.
        * @param aId central repository ID.
        */
        virtual void HandleCenRepChangeL( 
            const TUid& aUid,
            const TUint aId );


    private:

        /**
        * Handle the error of audio file player.
        */
        virtual void HandleAudioPlayerError( 
            TPhoneAudioPlayerErrorEvent aEvent, 
            TInt aError, 
            TInt aId = KPhoneAudioPlayerNotSpecified );

        /**
        * Handle audio player initialize complete.
        */
        virtual void HandleAudioPlayerInitComplete(
            TInt aId = KPhoneAudioPlayerNotSpecified );

        /**
        * Handle auido file playing complete successfully.
        */
        virtual void HandlePlayingComplete( 
            TInt aId = KPhoneAudioPlayerNotSpecified );

        /**
        * @see MPhoneTimer
        */
        virtual void HandleTimeOutL();

        /**
        * HandleVideoPlayerError
        */
        virtual void HandleVideoPlayerError( 
            TPhoneVideoPlayerErrorEvent aEvent,
            TInt aError );

        /**
        * HandleVideoPlayerInitComplete
        */
        virtual void HandleVideoPlayerInitComplete();

        /**
        * HandleVideoPlayerPlayingComplete
        */
        virtual void HandleVideoPlayerPlayingComplete();

        /** 
        * Enumerates identifiers for each player:
        * EPlayerAudio - audio
        * EPlayerBeepOnce - beep once
        * EPlayerSilent - silent
        * EPlayerDefault - default
        * EPlayerBackup - backup
        *
        * EPlayerFirst and EPlayerLast are alias
        * to other identifiers.
        * EPlayerFirst - first player
        * EPlayerLast - last player
        */
        enum TPlayerId
            {
            EPlayerFirst,
            EPlayerAudio = EPlayerFirst,
            EPlayerBeepOnce,
            EPlayerSilent,
            EPlayerUnsecureVoIP,
            EPlayerDefault,
            EPlayerBackup,
            EPlayerTts, 
            EPlayerLast = EPlayerTts,
            EPlayerCount
            };
            
        /**
        * Enumerates how many times TTs is played
        * ESaidOnce -once
        * ESaidTwice twice 
        */    
        enum TTtsStatus
            {
            ESaidOnce,
            ESaidTwice
            };

        /**
        * C++ default constructor.
        */
        CPhoneRingingTonePlayerAO();

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

        /**
        * Play default tone.
        * @param aVolume volume used for the playing.
        * @param aRingingType ringing type.
        */
        void PlayDefaultTone( 
            TInt aVolume, 
            TProfileRingingType aRingingType );

        /**
        * Play backup tone.
        * @param aVolume volume used for the playing.
        * @param aRingingType ringing type.
        */
        void PlayBackupTone( 
            TInt aVolume, 
            TProfileRingingType aRingingType );

        /**
        * Do construct tone.
        * @param aRingingTone Ringing tone.
        * @return The instance of the player. NULL is returned if it's failed.
        */
        CPhoneAudioPlayer* ConstructTonePlayer( 
            const CPhoneRingingTone& aRingingTone,
            TInt aId );

        /**
        * Handle audio player error.
        * @param aDelete ETrue if deletion of the player allowed, 
        *                EFalse otherwise.
        * @param aSync ETrue if synchronous deletion of players is allowed,
        *              EFalse otherwise.
        */
        void DoHandlePlayerError(
            TBool aDelete,
            TBool aSync );

        /**
        * Convert ringing type from PhoneAppEnging to the ringing type in 
        * CPhoneAudioPlayer.
        * @param aRingingType the ringing type form PhoneAppEngine.
        * @return ringing type of CPhoneAudioPlayer.
        */
        CPhoneAudioPlayer::TRingingType ConvertRingingType(
            TProfileRingingType aRingingType );

        /**
        * Convert ringing type from PhoneAppEnging to the ringing type in 
        * MPhoneVideoPlayer.
        * @since Series 60 3.1
        * @param aRingingType the ringing type form PhoneAppEngine.
        * @return ringing type of MPhoneVideoPlayer.
        */
        MPhoneVideoPlayer::TPlayMode ConvertVideoRingingType(
            TProfileRingingType aRingingType );

        /**
        * Construct media server and backup tone player.
        */
        TInt ConstructPlayers();

        /**
        * Construct media server and backup tone player.
        */
        void ConstructPlayersL();

        /**
        * Construct sequence players.
        * @param aPlayer player to be constructed.
        */
        void ConstructSequencePlayer(
            TPlayerId aPlayer );

        /**
        * Construct sequence players.
        * @param aPlayer player to be constructed.
        */
        void ConstructSequencePlayerL(
            TPlayerId aPlayer );

        /**
        * Cleanup players.
        */
        void CleanupPlayers();

        /**
        * Deletes player asynchronoysly.
        * @param aPlayer player to be deleted.
        */
        void DeletePlayerAsync(
            TPlayerId aPlayer );
       /**
        * Plays default L1 or L2 video ring tone asynchronously.
        * @since Series 60 3.1
        */
        void PlayDefaultVideoAsync();

        /**
        * Deletes video player and plays default L1 or L2
        * tone asynchronously.
        * @since Series 60 3.1
        */
        void DeleteVideoPlayerAndPlayDefaultAsync();

        /**
        * @see CActive::RunL.
        */
        void RunL();

        /**
        * @see CActive::DoCancel.
        */
        void DoCancel();
        
        /**
        * Wrapper of DoHandleTtsDelayTimeout, that can be used as TCallback. 
        * Immediately passes control to DoHandleTtsDelayTimeout method.
        * @since Series 60 3.0
        * @param object Object to call DoHandleTtsDelayTimeout() on, . 
        *        instance of CPhoneRingingTonePlayer.
        * @return KErrNone.
        */
        static TInt HandleTtsDelayTimeout( TAny* object );
        
        /**
        * It is called after TTS delay elapsed. Starts playing TTS.
        */
        void DoHandleTtsDelayTimeout();
        
        /**
        * Checks if TTS component should be played for the current ringtone 
        * playback and initializes the TTS playback. This method is to be 
        * called from within PlayXXXXTone methods
        * @since Series 60 3.0
        * @param aRingingType ringing type
        */
        void AddTtsPlaybackIfNeeded( TProfileRingingType aRingingType );
             
        /**
        * Returns currently active player which is used wit TTS player.
        * @return pointer to CPhoneAudioPlayer.
        */
        CPhoneAudioPlayer* GetCurrentlyActiveAudioPlayerWithTTs();   
         
        /**
        * TTs cases only. Calculates new volume and ramptimes
        * for Ascending TTS ringing case and normal TTS ringing case.
        * @param aStatus -status of TTs player.
        */         
        void SolveNewVolumeAndRamptime( TTtsStatus aStatus );   
        
        /**
        * Retarts currently active ringingtone player.
        */
        void ReStartRingingTonePlayer();
        
         /**
         * Checks if ringing tone file's size is larger than the size limit.
         * If size limit is exceeded, default tone is played instead.
         * @since 3.1
         * @param aRingingTone  Tone to check.
         * @return EFalse if the limit was exceeded and thus default tone played.
         *         ETrue  if tone file's size was valid and the checked tone 
         *                can be played.
         */
        TBool CheckAndHandleToneSizeLimit( CPhoneRingingTone* aRingingTone );

         /**
         * Checks if ringing tone file's size is larger than the size limit.
         * @since 3.1
         * @param aFile   File to check.
         *        aSizeLimitKB  the size limit in kB is set to this variable.
         * @return KErrNone        if the file can be used as a ringing tone.
         *         KErrTooBig        if the file size limit is exceeded.
         *         Other error value if error happened during size checking.
         */
        TInt CheckToneFileSize( const TDesC& aFile, TInt aSizeLimitKB );
        
         /**
         * Reads from central repository the file size limit of following tones:
         * -contact's personal ringing tone
         * -voice call line1
         * -voice call line2
         * Value 0 means all sizes are allowed.
         * @since 3.1
         * @param aSizeLimitKB  the size limit in kB is read to this variable.
         * @return leaves on error.
         */
        void GetMaxToneFileSizeL( TInt& aSizeLimitKB ) const;
        
        /**
        * Returns currently active audio player.
        * @return Currently active audio player or NULL if none active.
        */
        CPhoneAudioPlayer* ActiveAudioPlayer();
        
        /**
        * Mutes the ringing tone.
        */
        void DoMuteRingingTone();
        

    private:    // Data
        
        // Audio ring tone player. Memory allocated during construction.
        // After constructed, no leave may be caused during audio playing
        CPhoneAudioPlayer* iAudioPlayer;

        // Audio player for beep once.
        CPhoneAudioPlayer* iBeepOncePlayer;

        // Audio player for silent tone.
        CPhoneAudioPlayer* iSilentPlayer;
        
        // Audio player for unsecure VoIP tone.
        CPhoneAudioPlayer* iUnsecureVoIPTonePlayer;
        
        // Default player.
        CPhoneAudioPlayer* iDefaultPlayer;
        
        // Tts player.
        CPhoneAudioPlayer* iTtsPlayer;

        // Backup ringing tone. Incase all default player failed.
        CPhoneAudioPlayer* iBackupPlayer;
        
		// Default player
        CPhoneRingingTone* iDefaultRingingTone;
        
        // Media server.
        CMdaServer* iMdaServer;

        // Indicate which tone is playing or need to be played:
        // EIdle - idle,
        // EAudioTonePlaying - audio playing,
        // EBeepOnce - beep once playing,
        // ESilentTonePlaying - silent tone playing,
        // EUnsecureVoIPTonePlaying - unsecure tone playing (VoIP),
        // EDefaultTonePlaying - default tone playing,
        // EBackupTonePlaying - backup tone playing,
        // EVideoTonePlaying - playing video ringing tone
        // ESilentVideoTonePlaying  - silent playing video ringing tone
        // EPersonalVideoTonePlaying - personal playing video ringing tone
        enum TTonePlayingStatus
            {
            EIdle,
            EAudioTonePlaying,
            EBeepOnce,
            ESilentTonePlaying,
            EUnsecureVoIPTonePlaying,
            EDefaultTonePlaying,
            EBackupTonePlaying,
            EVideoTonePlaying,
            ESilentVideoTonePlaying,
            EPersonalVideoTonePlaying
            };

        // Current playing status.
        TTonePlayingStatus iTonePlayingStatus;
        
        // True if Tts is currently playing. Is different from 
        // iTonePlayingStatus, because TTS is played in
        // parallel with the other tone types
        TBool iTtsTonePlaying;
        
        // Index of the TTS delay that is "executed" currently or is to be 
        // "executed" after the current playing of the TTS tone. Is zero based
        TInt iTtsDelayIndex;
        
        // True if TTS tone should be played when the next playing of the 
        // default or personal tone is fired.
        TBool iTtsToneToBePlayed;

        // Audio/Video tone, not owned
        CPhoneRingingTone* iAudioVideoRingingTone;

        // Volume for backup tone playing.
        TInt iVolume;
        
        // Volume of the text pronounced by the TTS engine
        TInt iTtsVolume;

        // Ringing type for backup tone playing.
        TProfileRingingType iRingingType;

        // Timer.
        CPhoneTimer* iTimer;

        // Timer, that tracks delays before playing TTS
        CPhoneTimer* iTtsDelayTimer;

        // Number of times TTS tone should be played for the CURRENT ringtone
        // If ringing tone is to be played just once, iTtsDelayCount == 1,
        // otherwise iTtsDelayCount == KTtsDelayCount
        TInt iTtsDelaysCount;

        // Player to be deleted asynchronously.
        RPointerArray<CPhoneAudioPlayer> iAsyncDeletePlayers;

        // DRM extend security
        TBool iExtSecNeeded;
 
        // Video ringing tone player
        MPhoneVideoPlayer* iVideoPlayer;
    
        // State of this active object
        TState iState;
              
        // RingingType for TtsPlayer only.
        TProfileRingingType iTtsRingingType;
        
        // Counter for TTs timeouts
        TInt iTTsTimeOutCounter;
        
        // Voice call ringing tone file size max value.
        TInt iToneFileSizeLimitKB;             

        // Flag. Indicates video scaling capability of the device.
        TBool iArbitraryVideoScaling;

    };

#endif      // CPHONERINGINGTONEPLAYERAO_H
            
// End of File
