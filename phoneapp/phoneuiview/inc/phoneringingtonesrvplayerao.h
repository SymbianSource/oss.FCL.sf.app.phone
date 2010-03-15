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


#ifndef __PHONERINGINGTONESRVPLAYERAO_H
#define __PHONERINGINGTONESRVPLAYERAO_H

// INCLUDES
#include <e32base.h>
#include <Profile.hrh>

#include "MPhoneAudioPlayerObserver.h"
#include "MPhoneTimer.h"
#include "CPhoneAudioPlayer.h"

// FORWARD DECLARATIONS
class RFs;
class CPhoneTimer;
class CMdaServer;
class CPhoneRingingTone;

// CLASS DECLARATION
class CPhoneRingingToneSrvPlayerAO : 
    public CActive,
    private MPhoneAudioPlayerObserver,
    private MPhoneTimer
    {
    public:
        // States for this active object
        enum TState
            {
            EIdleState,
            EDeletingAudioPlayer
            };

    public:

        /**
        * Two-phased constructor.
        * @return new instance.
        */
        static CPhoneRingingToneSrvPlayerAO* NewL( RFs &aFs );

        /**
        * Destructor.
        */
        ~CPhoneRingingToneSrvPlayerAO();

        /**
        * Play audio ring tone. 
        * @param aVolume Volume used to play the ringing tone.
        * @param aRingingType Ringing type.
        * @param aMessage Message from Client.
        * @return ETrue if the client message should not be completed.
        */
        TBool PlayAudioRingTone( 
            TInt aVolume, 
            TProfileRingingType aRingingType,
            const RMessage2& aMessage );

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
        * Play tone if secure call fails and call is changed to unsecure call.
        */
        void PlayUnsecureVoIPTone();

        /**
        * Stops playing the ringing tone.
        * Method does not do anything if ringing tone is not playing.
        */
        void StopPlaying();

        /**
        * Mutes ringing tone.
        */
        void MuteRingingTone();

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
        * Checks if extended security required.
        * @since Series 60 3.1
        * @return ETrue if extended security is required.
        */
        TBool ExtendedSecurity() const;

    public:
        /**
        * Sets extended security required flag.
        * @param aNeeded Extended security required.
        */
        void SetExtendedSecurityNeeded( TBool aNeeded );
    
        /**
        * Sets extended security required flag.
        * @param aLimit Extended security required.
        */
        void SetToneFileSizeLimit( TInt aLimit );
    
        /**
        * Sets default ringing tone.
        * @param aFilename Ringing tone.
        */
        void SetDefaultRingingToneL( const TDesC& aFilename );
    
        /**
        * Sets ringing tone.
        * @param aFilename Ringing tone.
        */
        void SetRingingToneL( const TDesC& aFilename );

    private:

        /**
        * Handle the error of audio file player.
        * @param aEvent The error event happend in initializing or playing.
        * @param aError The error code return by audio server. Observer can 
        *               handle  these two error code for recoverable 
        *               and unrecoverable errors.
        * @param aId Indicate which audio player is out of working.
        */
        virtual void HandleAudioPlayerError( 
            TPhoneAudioPlayerErrorEvent aEvent, 
            TInt aError, 
            TInt aId = KPhoneAudioPlayerNotSpecified );

        /**
        * Handle audio player initialize complete.
        * @param aId Indicate which audio player is out of working.
        */
        virtual void HandleAudioPlayerInitComplete(
            TInt aId = KPhoneAudioPlayerNotSpecified );

        /**
        * Handle audio file playing complete successfully.
        * @param aId Indicate which audio player is out of working.
        */
        virtual void HandlePlayingComplete( 
            TInt aId = KPhoneAudioPlayerNotSpecified );

        /**
        * @see MPhoneTimer
        */
        virtual void HandleTimeOutL();

        /** 
        * Enumerates identifiers for each player:
        * EPlayerAudio - audio
        * EPlayerBeepOnce - beep once
        * EPlayerSilent - silent
        * EPlayerUnsecureVoIP - unsecure VoIP
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
        CPhoneRingingToneSrvPlayerAO( RFs& aFs );

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();

        /**
        * Do construct tone.
        * @param aRingingTone Ringing tone.
        * @param aId Indicates the audio player.
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
        * @param aObject Object to call DoHandleTtsDelayTimeout() on, 
        *        instance of CPhoneRingingTonePlayer.
        * @return KErrNone.
        */
        static TInt HandleTtsDelayTimeout( TAny* aObject );

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
        * Returns currently active audio player.
        * @return Currently active audio player or NULL if none active.
        */
        CPhoneAudioPlayer* ActiveAudioPlayer();

    private:    // Data
        // Handle to file server
        RFs &iFs;

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

        // Default ringing tone
        CPhoneRingingTone* iDefaultRingingTone;

        // Media server.
        CMdaServer* iMdaServer;

        // Indicate which tone is playing or need to be played:
        // EIdle - idle,
        // EAudioTonePlaying - audio playing,
        // EBeepOnce - beep once playing,
        // ESilentTonePlaying - silent tone playing,
        // EDefaultTonePlaying - default tone playing,
        // EBackupTonePlaying - backup tone playing,
        enum TTonePlayingStatus
            {
            EIdle,
            EAudioTonePlaying,
            EBeepOnce,
            ESilentTonePlaying,
            EUnsecureVoIPTonePlaying,
            EDefaultTonePlaying,
            EBackupTonePlaying
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

        // Normal ringing tone
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

        // State of this active object
        TState iState;

        // RingingType for TtsPlayer only.
        TProfileRingingType iTtsRingingType;

        // Counter for TTs timeouts
        TInt iTTsTimeOutCounter;

        // Voice call ringing tone file size max value.
        TInt iToneFileSizeLimitKB; 

        // Client message.
        RMessage2 iClientMessage;

    };

#endif //__PHONERINGINGTONESRVPLAYERAO_H
