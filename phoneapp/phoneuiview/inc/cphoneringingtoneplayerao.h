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
#include "mphonevideoplayerobserver.h"
#include "mphonecenrepobserver.h"
#include "mphonetimer.h"
#include "mphonevideoplayer.h"
#include "phoneringingtoneclient.h"

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
    private MPhoneTimer,
    private MPhoneVideoPlayerObserver
    {
    public:
        // States for this active object
        enum TState
            {
            EIdleState,  
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
        * @param aVolume Volume used to play the ringing tone.
        * @param aRingingType Ringing type.
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
        * @see MPhoneTimer
        */
        virtual void HandleTimeOutL();

        /**
        * This function is called for video player errors.
        * @param aEvent The error event happend in initializing or playing.
        * @param aError The error code return by MMF video player. 
        */
        virtual void HandleVideoPlayerError( 
            TPhoneVideoPlayerErrorEvent aEvent,
            TInt aError );

        /**
        * This function is called when video player initialization complete
        */
        virtual void HandleVideoPlayerInitComplete();

        /**
        * This function is called after the playing completes successfully
        */
        virtual void HandleVideoPlayerPlayingComplete();

        /**
        * C++ default constructor.
        */
        CPhoneRingingTonePlayerAO();

        /**
        * Symbian 2nd phase constructor.
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
        * Convert ringing type from PhoneAppEnging to the ringing type in 
        * MPhoneVideoPlayer.
        * @since Series 60 3.1
        * @param aRingingType the ringing type form PhoneAppEngine.
        * @return ringing type of MPhoneVideoPlayer.
        */
        MPhoneVideoPlayer::TPlayMode ConvertVideoRingingType(
            TProfileRingingType aRingingType );

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
         * @param aSizeLimitKB  the size limit in kB is set to this variable.
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

    private:    // Data

        // Indicate which tone is playing or need to be played:
        // EIdle - idle,
        // EVideoTonePlaying - playing video ringing tone
        // ESilentVideoTonePlaying  - silent playing video ringing tone
        // EPersonalVideoTonePlaying - personal playing video ringing tone
        enum TTonePlayingStatus
            {
            EIdle,
            EVideoTonePlaying,
            ESilentVideoTonePlaying,
            EPersonalVideoTonePlaying
            };

        // Current playing status.
        TTonePlayingStatus iTonePlayingStatus;

        // Audio/Video tone, not owned
        CPhoneRingingTone* iAudioVideoRingingTone;

        // Volume for backup tone playing.
        TInt iVolume;

        // Ringing type for backup tone playing.
        TProfileRingingType iRingingType;

        // Timer.
        CPhoneTimer* iTimer;

        // DRM extend security
        TBool iExtSecNeeded;

        // Video ringing tone player
        MPhoneVideoPlayer* iVideoPlayer;

        // State of this active object
        TState iState;

        // Voice call ringing tone file size max value.
        TInt iToneFileSizeLimitKB;

        // Flag. Indicates video scaling capability of the device.
        TBool iArbitraryVideoScaling;

        // Ringing tone server.
        RPhoneRingingTonePlayer iToneServer;

    };

#endif      // CPHONERINGINGTONEPLAYERAO_H

// End of File
