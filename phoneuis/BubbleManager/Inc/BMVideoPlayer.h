/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Video player implementaion.
*
*/


#ifndef C_BUBBLEVIDEOPLAYER_H
#define C_BUBBLEVIDEOPLAYER_H

#include <videoplayer.h>

// FORWARD DECLARATIONS
class MBubbleVideoPlayerObserver;

/**
 *  Video ringing tone player
 *
 *  Uses CVideoPlayerUtility to play video clips.
 *
 *  @lib BubbleManager.lib
 *  @since S60 v3.2
 */
class CBubbleVideoPlayer : public CBase,
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
            EVideoOther = -1,
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
        static CBubbleVideoPlayer* NewL( 
            const TDesC& aRingingTone, 
            TInt aPriority, 
            TUint aPreference,
            MBubbleVideoPlayerObserver& aObserver, 
            RWindow& aVideoTarget );

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
        static CBubbleVideoPlayer* New( 
            const TDesC& aRingingTone, 
            TInt aPriority, 
            TUint aPreference,
            MBubbleVideoPlayerObserver& aObserver, 
            RWindow& aVideoTarget );

        /**
        * Destructor.
        */
        virtual ~CBubbleVideoPlayer();

    public:
        /**
        * Plays the loaded video.
        * @since Series 60 v3.2
        * @param aRingingType Ringing type.
        * @param aVolume take value from 1 to 10
        */
        void Play( TRingingType aRingType, TInt aVolume );

        /**
        * Stops video playback.
        * @since Series 60 v3.2
        */
        void StopPlaying();
        
        /**
        * Pauses video playback.
        * @since Series 60 v3.2
        * @return KErrNone is successful.
        */
        TInt PausePlaying();
        
        /**
        * Resumes video playback.
        * @since Series 60 v3.2
        */
        void ResumePlaying();
        
        /**
        * Make video to play silent.
        * @since Series 60 v3.2
        */
        void MuteAudio();

        /**
        * Get the state of the phone video player.
        * @since Series 60 v3.2
        * @return The status of video player
        */
        TVideoPlayerState State() const;

        /**
        * Get the resolution of the loaded video.
        * @since Series 60 v3.2
        * @return The resolution.
        */
        TVideoResolution VideoResolution() const;

        /**
        * Scales and crops video to cover target window.
        * @since Series 60 v3.2
        * @param aDisplayWindow Window for video.
        * @param aUncropPane Rectangle for cropping control.
        * @param aArbitaryScalingSupported Scaling capability of the device.
        */
        void AdjustToWindow( RWindow& aDisplayWindow,
                             const TRect& aUncropPane,
                             TBool aArbitaryScalingSupported = EFalse );
        
        /**
        * Scales and crops video to cover target window. This method
        * doens't use uncrop pane and crops video only when needed.
        * Allowed cropping can be set via KPhoneMaxVideoCrop.
        * Cropped video is centered to video pane.
        * This metdod should be used only when video player supports
        * arbitrary scaling of video image.
        * @since Series 60 v3.2
        * @param aDisplayWindow Window for video playback.
        */
        void AdjustToWindow2( RWindow& aDisplayWindow );                             

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
        CBubbleVideoPlayer();
        
        /**
        * C++ constructor.
        */
        CBubbleVideoPlayer( MBubbleVideoPlayerObserver& aObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( const TDesC& aFileName, 
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
        * RepeatsTrailPauseTimerCallback
        */
        static TInt RepeatsTrailPauseTimerCallback( TAny* aObj );
        
        /**
        * DoRingingRepeat
        */
        TInt DoRingingRepeat();
        
        /**
        * Frame size
        */
        TSize VideoFrameSize() const;
        
        /**
        * Calculates size for video player screen rectangle based on
        * canvas and video dimensions.   
        */
        static TSize CalculateScreenRectSize( 
            const TSize& aCanvasSize,
            const TSize& aVideoFrameSize,
            TBool aArbitraryScalingSupported );
            
        /**
        * Calculates aClipSize and aScreenSize based on aWindowSize
        * and aVideoFrameSize. aScreenSize is set to maximum without
        * exceeding the crop limit (KPhoneMaxVideoCrop). aClipSize 
        * is set match to scaled video image to prevent black stripes
        * around video.   
        */
        static void CalculateClipAndScreenRectSize( 
            const TSize& aWindowSize,
            const TSize& aVideoFrameSize,
            TSize& aClipSize,
            TSize& aScreenSize );            

    private: //Data

        MBubbleVideoPlayerObserver&  iObserver;
        CVideoPlayerUtility*         iVideoPlayer; // Owned
        TVideoPlayerState            iPlayerState;
        TBool                        iToBePlayed;

        // Ringing properties
        TInt                         iVolume;
        TRingingType                 iRingingType;

        // Ascending volume
        TInt                         iRampedVolume;
        CPeriodic*                   iVolumeRampTimer; // Owned
        
        // Pause between repeats
        CPeriodic*                   iRepeatsTrailPauseTimer; // Owned
        
        // Ringing tone
        RFile                        iFileHandle;
        
    };
    
#endif // C_BUBBLEVIDEOPLAYER_H
