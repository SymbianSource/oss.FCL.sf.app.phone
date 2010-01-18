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
* Description:  Video ringing tone player.
*
*/


#ifndef CPHONEVIDEOPLAYER_H
#define CPHONEVIDEOPLAYER_H

// INCLUDES

#include <VideoPlayer.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CFileMan;
class CVideoPlayerUtility;
class MPhoneVideoPlayerObserver;
class CPhoneRingingTone;

// CLASS DECLARATION

/**
*  Video ringing tone player
*  Uses CVideoPlayerUtility to play video clips.
*
*  @lib PhoneUIView.lib
*  @since Series 60 3.1
*/
class CPhoneVideoPlayer : 
    public CBase,
    public MVideoPlayerUtilityObserver
    {
    public:
        enum TVideoPlayerState // from MMF-VideoPlayer IF spec.
            {
            EVideoError = -1,
            EVideoClosed,
            EVideoOpening,
            EVideoOpen,
            EVideoPreparing,
            EVideoReady, // player's stopped state
            EVideoPlaying,
            EVideoPaused
            };

        enum TRingingType
            {
            ETypeRinging,
            ETypeRingingOnce,
            ETypeAscending
            };

        enum TVideoResolution
            {
            EVideoQCIF,
            EVideoSubQCIF
            };

    public:  // Constructor and destructor

        /**
        * Two-phased constructor.
        * @param aRingingTone The ringing tone
        * @param aPriority The priority for the playing
        * @param aPreference The preference value
        * @param aObserver Video player observer
        * @param aVideo Window for video
        * @return The instance of the video player.
        */
        static CPhoneVideoPlayer* NewL( 
            const CPhoneRingingTone& aRingingTone, 
            TInt aPriority, 
            TUint aPreference,
            MPhoneVideoPlayerObserver& aObserver, 
            RWindow& aVideoTarget,
            TBool aExtSecNeeded = EFalse );

        /**
        * Two-phased constructor (non leaving).
        * @param aFileName The ringing tone
        * @param aPriority The priority for the playing
        * @param aPreference The preference value
        * @param aObserver Video player observer
        * @param aVideo Window for video
        * @return The instance of the video player. NULL is
        *         returned if creation failed.
        */
        static CPhoneVideoPlayer* New( 
            const CPhoneRingingTone& aRingingTone, 
            TInt aPriority, 
            TUint aPreference,
            MPhoneVideoPlayerObserver& aObserver, 
            RWindow& aVideoTarget,
            TBool aExtSecNeeded = EFalse );

        /**
        * Destructor.
        */
        virtual ~CPhoneVideoPlayer();

    public:
        /**
        * Plays the loaded video.
        * @since Series 60 3.1
        * @param aRingingType Ringing type.
        * @param aVolume take value from 1 to 10
        */
        void Play( TRingingType aRingType, TInt aVolume );

        /**
        * Stops video playback.
        * @since Series 60 3.1
        */
        void StopPlaying();

       /**
        * Pauses video playback.
        * @since Series 60 3.1
        * @return KErrNone is successful.
        */
        TInt PausePlaying();
        
        /**
        * Resumes video playback.
        * @since Series 60 3.1
        */
        void ResumePlaying();
        
        /**
        * Make video to play silent.
        * @since Series 60 3.1
        */
        void MuteAudio();

        /**
        * Get the state of the phone video player.
        * @since Series 60 3.1
        * @return The status of video player
        */
        TVideoPlayerState State() const;

        /**
        * Get the resolution of the loaded video.
        * @since Series 60 3.1
        * @return The resolution.
        */
        TVideoResolution VideoResolution() const;

        /**
        * Scales and crops video to cover target window.
        * @since Series 60 3.1
        * @param aDisplayWindow Window for video.
        * @param aUncropPane Rectangle for cropping control.
        */
        void AdjustToWindow( RWindow& aDisplayWindow,
        					 const TRect& aUncropPane );

    private: // Functions from MVideoPlayerUtilityObserver
        /** 
        * MvpuoOpenComplete
        */
        void MvpuoOpenComplete( TInt aError );

        /** 
        * MvpuoPrepareComplete
        */
        void MvpuoPrepareComplete( TInt aError );

        /** 
        * MvpuoFrameReady
        */
        void MvpuoFrameReady( CFbsBitmap& aFrame ,TInt aError );

        /** 
        * MvpuoPlayComplete
        */
        void MvpuoPlayComplete( TInt aError );
        
        /** 
        * MvpuoEvent
        */
        void MvpuoEvent( const TMMFEvent& aEvent );
    
    private: // Constructors

        /**
        * C++ default constructor.
        */
        CPhoneVideoPlayer();
        
        /**
        * C++ constructor.
        */
        CPhoneVideoPlayer( MPhoneVideoPlayerObserver& aObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( 
            const TDesC& aFileName, 
            TInt aPriority, 
            TUint aPreference,
            RWindow& aVideoTarget );

    private:
        /**
        * SetRingingType
        */
        void SetRingingType( TRingingType aRingingType );

        /**
        * SetVolume
        */
        void SetVolume( TInt aVolume );

        /**
        * VolumeRampTimerCallback
        */
        static TInt VolumeRampTimerCallback( TAny* aObj );

        /**
        * DoVolumeRamp
        */
        TInt DoVolumeRamp();
        
        /**
        * Frame size
        */
        TSize VideoFrameSize() const;

    private: //Data

        MPhoneVideoPlayerObserver&  iObserver;
        CVideoPlayerUtility*        iVideoPlayer;
        TVideoPlayerState           iPlayerState;
        TBool                       iToBePlayed;

        // Ringing properties
        TInt                        iVolume;
        TRingingType                iRingingType;

        // Ascending volume
        TInt                        iRampedVolume;
        CPeriodic*                  iVolumeRampTimer;
        
        // Ringing tone
        RFile                       iFileHandle;
        
    };

#endif // CPHONEVIDEOPLAYER
