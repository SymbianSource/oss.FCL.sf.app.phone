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
* Description:  Video player implementation.
*
*/


// INCLUDE FILES
#include <coemain.h>       // CCoeEnv 
#include <eikenv.h>        // CEikEnv 
#include <videoplayer.h>   // CVideoPlayerUtility
#include "BMVideoPlayer.h"
#include "BMVideoPlayerObserver.h"
#include "BMPanic.h"
#include "BMTrace.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS
const TInt KBubbleVideoMinVolumeLevel = 0;
const TInt KBubbleVideoMaxVolumeLevel = 10;
const TInt KBubbleVideoVolumeRampInterval = 3000000;
const TInt KBubbleVideoVolumeRampStep = 1;
const TInt KBubbleVideoRepeatsTrailPause = 1000000;
const TInt KBubbleVideoIACoef = 1000; // Precision for fixed point math
const TInt KBubbleMaxVideoCrop = 0; // Allowed max cropping. 0, 1 .. 20 (%).
// The margin between aWindowRect and aClipRect parameters passed to player,
// which can be used to eliminate black fill (scaled image is matched to
// target area by clipping it).
const TInt KBubbleVideoClipMarginX = 2;
const TInt KBubbleVideoClipMarginY = 2;


// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES
TInt DivideAndCeil(const TInt aVal, const TInt aDiv)
    {
    return (((aVal%aDiv)>0) ? (TInt)((aVal/aDiv)+1):(TInt)(aVal/aDiv));
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleVideoPlayer::CBubbleVideoPlayer(
    MBubbleVideoPlayerObserver& aObserver )
    :
    iObserver( aObserver ),
    iVideoPlayer( NULL ),
    iPlayerState( EVideoClosed ),
    iVolume( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::ConstructL
// -----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::ConstructL( 
    const TDesC& aFileName, 
    TInt aPriority, 
    TUint aPreference,
    RWindow& aVideoTarget  )
    {
    BUBBLE_PRINT("VideoPlayer::Const>");
    // Screen and clip rectangles to window dimensions
    TPoint wndPosition( aVideoTarget.AbsPosition() );
    TSize wndSize( aVideoTarget.Size() );
    TRect wndRect( wndPosition, wndSize );
    
    // Create video player instance
    CCoeEnv* coeEnv = CCoeEnv::Static();
    iVideoPlayer = CVideoPlayerUtility::NewL(
        *this,
        aPriority,
        static_cast<TMdaPriorityPreference>( aPreference ),
        coeEnv->WsSession(),
        *coeEnv->ScreenDevice(),    
        aVideoTarget,
        wndRect, 
        wndRect );

    RFs fs = CEikonEnv::Static()->FsSession();
    User::LeaveIfError( iFileHandle.Open( fs,
                                          aFileName,
                                          EFileShareReadersOnly | 
                                          EFileStream | 
                                          EFileRead ) );        
    
    iVideoPlayer->OpenFileL( iFileHandle ); // async
    
    iPlayerState = EVideoOpening;
    BUBBLE_PRINT("VideoPlayer::Const<");
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::NewL
// -----------------------------------------------------------------------------
//
CBubbleVideoPlayer* CBubbleVideoPlayer::NewL(
    const TDesC& aRingingTone, 
    TInt aPriority, 
    TUint aPreference,
    MBubbleVideoPlayerObserver& aObserver, 
    RWindow& aVideoTarget )
    {
    CBubbleVideoPlayer* self = new(ELeave) CBubbleVideoPlayer( aObserver );
        
    CleanupStack::PushL( self );
    self->ConstructL( aRingingTone, 
                      aPriority, aPreference, 
                      aVideoTarget );
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::New
// -----------------------------------------------------------------------------
//
CBubbleVideoPlayer* CBubbleVideoPlayer::New(
    const TDesC& aRingingTone,  
    TInt aPriority, 
    TUint aPreference,
    MBubbleVideoPlayerObserver& aObserver, 
    RWindow& aVideoTarget )
    {
    CBubbleVideoPlayer* self = NULL;

    TRAPD( ignore, self = CBubbleVideoPlayer::NewL(
                              aRingingTone, 
                              aPriority, 
                              aPreference,
                              aObserver,   
                              aVideoTarget ) );
    ignore = ignore;                              
    
    return self; // this is NULL if NewL leaves
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CBubbleVideoPlayer::~CBubbleVideoPlayer()
    {
    if ( iVolumeRampTimer )
        {
        iVolumeRampTimer->Cancel();
        delete iVolumeRampTimer;
        }
        
    if ( iRepeatsTrailPauseTimer )
        {
        iRepeatsTrailPauseTimer->Cancel();
        delete iRepeatsTrailPauseTimer;    
        }

    if ( iVideoPlayer )
        {
        iVideoPlayer->Close();
        delete iVideoPlayer;
        }
        
    iFileHandle.Close();        
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::Play
// -----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::Play( TRingingType aRingType, 
                              TInt aVolume )
    {
    BUBBLE_PRINT("VideoPlayer::Play>");
    __ASSERT_DEBUG( iVideoPlayer, Panic( EBMPanicVideoPlayer ) );
    
    iRingingType = aRingType;
    iVolume = aVolume;

    switch( iPlayerState )
        {
        case EVideoOpening:
        case EVideoPreparing:
        case EVideoOpen:
            // see MvpuoPrepareComplete()
            iToBePlayed = ETrue;   
            break;
        
        case EVideoReady:
        case EVideoPaused:
            iVideoPlayer->Play();
            iPlayerState = EVideoPlaying;
            break;
        
        case EVideoPlaying:
        case EVideoClosed:
        case EVideoError:
        default:
            // NOP
            break;
        }

    BUBBLE_PRINT("VideoPlayer::Play<");
    return;
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::StopPlaying
// -----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::StopPlaying()
    {
    BUBBLE_PRINT("VideoPlayer::Stop>");
    iToBePlayed = EFalse;

    if ( iVolumeRampTimer )
        {
        iVolumeRampTimer->Cancel();
        }
        
    if ( iRepeatsTrailPauseTimer )        
        {
        iRepeatsTrailPauseTimer->Cancel();    
        }
    
    if ( iPlayerState == EVideoPlaying ||
         iPlayerState == EVideoPaused )
        {
        iVideoPlayer->Stop();
        iPlayerState = EVideoReady;
        }
    BUBBLE_PRINT("VideoPlayer::Stop<");        
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::PausePlaying
// -----------------------------------------------------------------------------
//    
TInt CBubbleVideoPlayer::PausePlaying()
    {
    BUBBLE_PRINT("VideoPlayer::Pause>");
    TInt err = KErrNone;
    
    if ( iPlayerState == EVideoPlaying )
        {
        TRAP( err, iVideoPlayer->PauseL() );
        if ( err == KErrNone )    
            {
            iPlayerState = EVideoPaused;    
            }
        }
    else
        {
        err = KErrNotReady;    
        }        

    BUBBLE_PRINTF("VideoPlayer::Pause<:", err);        
    return err;        
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::ResumePlaying
// -----------------------------------------------------------------------------
//    
void CBubbleVideoPlayer::ResumePlaying()
    {
    BUBBLE_PRINT("VideoPlayer::Resume>");
    if ( iPlayerState == EVideoPaused )
        {
        iVideoPlayer->Play(); 
        iPlayerState = EVideoPlaying;       
        }
    BUBBLE_PRINT("VideoPlayer::Resume<");        
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::MuteAudio
// -----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::MuteAudio()
    {
    BUBBLE_PRINT("VideoPlayer::Mute");
    if ( iVolumeRampTimer )
        {
        iVolumeRampTimer->Cancel();
        }

    iVolume = 0;
    SetVolume( 0 );
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::SetVolume
// -----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::SetVolume( TInt aVolume )
    {
    BUBBLE_PRINT("VideoPlayer::SetVol>");
    TInt maxVolume( iVideoPlayer->MaxVolume() );
    maxVolume = ( maxVolume > 0 ) ? maxVolume : KBubbleVideoMaxVolumeLevel; 

    TInt scaledVolume = ( aVolume * maxVolume ) / KBubbleVideoMaxVolumeLevel;
    
    TRAP_IGNORE( iVideoPlayer->SetVolumeL( scaledVolume ) );
    BUBBLE_PRINT("VideoPlayer::SetVol<");
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::SetRingingType
// -----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::SetRingingType( TRingingType aRingingType )
    {
    BUBBLE_PRINT("VideoPlayer::SetRingType");
    if ( aRingingType == ETypeAscending )
        {
        if ( !iVolumeRampTimer )
            {
            iVolumeRampTimer = CPeriodic::New( CActive::EPriorityStandard );
            }

        if ( iVolumeRampTimer && !iVolumeRampTimer->IsActive() )
            {
            TCallBack cb( VolumeRampTimerCallback, this );

            SetVolume( KBubbleVideoMinVolumeLevel );
            iRampedVolume = KBubbleVideoMinVolumeLevel;
            iVolumeRampTimer->Start( KBubbleVideoVolumeRampInterval, 
                                     KBubbleVideoVolumeRampInterval, 
                                     cb );
            }
        }
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::VolumeRampTimerCallback
// -----------------------------------------------------------------------------
//
TInt CBubbleVideoPlayer::VolumeRampTimerCallback( TAny* aObj )
    {
    return static_cast<CBubbleVideoPlayer*>( aObj )->DoVolumeRamp();
    }
    

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::DoVolumeRamp
// -----------------------------------------------------------------------------
//
TInt CBubbleVideoPlayer::DoVolumeRamp()
    {
    BUBBLE_PRINT("VideoPlayer::DoVolRamp");
    if ( iRampedVolume < iVolume )
        {
        iRampedVolume = iRampedVolume + KBubbleVideoVolumeRampStep;
        if ( iRampedVolume >= iVolume )
            {
            // target volume level reached
            iRampedVolume = iVolume;
            iVolumeRampTimer->Cancel();
            }

        SetVolume( iRampedVolume );
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::RepeatsTrailPauseTimerCallback
// -----------------------------------------------------------------------------
//
TInt CBubbleVideoPlayer::RepeatsTrailPauseTimerCallback( TAny* aObj )
    {
    return static_cast<CBubbleVideoPlayer*>( aObj )->DoRingingRepeat();    
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::DoRingingRepeat
// -----------------------------------------------------------------------------
//    
TInt CBubbleVideoPlayer::DoRingingRepeat()
    {
    iRepeatsTrailPauseTimer->Cancel();
    
    if ( iPlayerState == EVideoReady )
        {
        iVideoPlayer->Play();
        iPlayerState = EVideoPlaying;
        }

    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::State
// -----------------------------------------------------------------------------
//
CBubbleVideoPlayer::TVideoPlayerState CBubbleVideoPlayer::State() const
    {
    return iPlayerState;
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::VideoResolution
// -----------------------------------------------------------------------------
//
CBubbleVideoPlayer::TVideoResolution CBubbleVideoPlayer::VideoResolution() const
    {
    TSize frameSize( VideoFrameSize() );
    
    // SubQCCIF video resolution is 128x96 pixels
    if ( frameSize.iWidth == 128 && frameSize.iHeight == 96 )
        {
        return EVideoSubQCIF;
        }
    else if ( frameSize.iWidth == 176 && frameSize.iHeight == 144 )        
        {
        return EVideoQCIF;        
        }
    else
        {
        return EVideoOther;    
        }        
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::VideoResolution
// -----------------------------------------------------------------------------
//    
TSize CBubbleVideoPlayer::VideoFrameSize() const
    {
    TSize frameSize( 0,0 );
    
    TRAPD( err, iVideoPlayer->VideoFrameSizeL( frameSize ) );
    
    if ( err != KErrNone )
        {
        return TSize(0,0);
        }

    return frameSize;            
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::AdjustToWindow
// -----------------------------------------------------------------------------
//    
void CBubbleVideoPlayer::AdjustToWindow( RWindow& aDisplayWindow,
                                        const TRect& aUncropPane,
                                        TBool aArbitaryScalingSupported )
    {
    BUBBLE_PRINT("VideoPlayer::AdjToWin>");
    __ASSERT_DEBUG( ( iPlayerState == EVideoReady || 
                      iPlayerState == EVideoPaused ), 
                    Panic( EBMPanicVideoPlayer ) );
                    
    // Get video frame dimensions
    TSize  frameSize( VideoFrameSize() );
    if ( frameSize.iWidth == 0 || frameSize.iHeight == 0 )
        {
        return; // invalid            
        }

    // Get window position and dimensions.
    TSize  wndSize( aDisplayWindow.Size() );
    TPoint wndPosition( aDisplayWindow.AbsPosition() );
    
    // Set screenRect equal to aDisplayWindow.
    TRect screenRect( wndPosition, wndSize );
    
    TVideoResolution resolution = VideoResolution();         
    if ( resolution == EVideoQCIF || resolution == EVideoSubQCIF )
        {
        // aUncropPane is valid only for qcif and sqcif.
        
        // The pane where uncropped video is positioned.
        TSize uncropPaneSize( aUncropPane.Size() );

        // Calculate size for aScreenRect passed to video player. Size is
        // calculated so that auto scaled video frame covers uncrop pane.
        TSize screenRectSize = CalculateScreenRectSize( 
            uncropPaneSize, frameSize, aArbitaryScalingSupported );
    
        // Position aScreenRect.    
        screenRect = TRect( aUncropPane.iTl, screenRectSize );     
        TInt offsetX = (uncropPaneSize.iWidth - screenRectSize.iWidth) / 2;
        TInt offsetY = (uncropPaneSize.iHeight - screenRectSize.iHeight) / 2;
        screenRect.Move( offsetX, offsetY );    
        }
    
    // Video is autoscaled by video player.
    // iVideoPlayer->SetScaleFactorL(...);
    
    // Clip to display window
    TRect clipRect( wndPosition, wndSize );
    
    // Update settings to video player.
    CCoeEnv* coeEnv = CCoeEnv::Static();
    TRAP_IGNORE( iVideoPlayer->SetDisplayWindowL( 
           coeEnv->WsSession(),
           *coeEnv->ScreenDevice(),
           aDisplayWindow,
           screenRect,
           clipRect ) );
    BUBBLE_PRINT("VideoPlayer::AdjToWin<");           
    }
    
// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::AdjustToWindow2
// -----------------------------------------------------------------------------
// 
void CBubbleVideoPlayer::AdjustToWindow2( 
    RWindow& aDisplayWindow )
    {
    BUBBLE_PRINT("VideoPlayer::AdjToWin2>");
    __ASSERT_DEBUG( ( iPlayerState == EVideoReady || 
                      iPlayerState == EVideoPaused ), 
                    Panic( EBMPanicVideoPlayer ) );
                    
    // Get video frame dimensions
    TSize  frameSize( VideoFrameSize() );
    if ( frameSize.iWidth == 0 || frameSize.iHeight == 0 )
        {
        return; // invalid            
        }
        
    // Get video container position and dimensions
    TSize  wndSize( aDisplayWindow.Size() );
    TPoint wndPosition( aDisplayWindow.AbsPosition() );        

    // Get sizes for clip and screen rectangles.
    TSize clipSize;
    TSize screenSize;
    CalculateClipAndScreenRectSize( wndSize,
                                    frameSize,
                                    clipSize,
                                    screenSize );
    
    // Center clip rectangle to aDisplayWindow
    TRect clipRect( wndPosition, clipSize );
    TInt offsetX = (wndSize.iWidth - clipSize.iWidth) / 2;
    TInt offsetY = (wndSize.iHeight - clipSize.iHeight) / 2;
    clipRect.Move( offsetX, offsetY );
    
    // Center screen rectangle to aDisplayWindow
    TRect screenRect( wndPosition, screenSize );
    offsetX = (wndSize.iWidth - screenSize.iWidth) / 2;
    offsetY = (wndSize.iHeight - screenSize.iHeight) / 2;
    screenRect.Move( offsetX, offsetY );
    
    iObserver.HandleVideoPlayerBlittingAreaDefined( clipRect );
    
    // Update rectangles to video player.
    CCoeEnv* coeEnv = CCoeEnv::Static();
    TRAP_IGNORE( iVideoPlayer->SetDisplayWindowL( 
           coeEnv->WsSession(),
           *coeEnv->ScreenDevice(),
           aDisplayWindow,
           screenRect,
           clipRect ) );
    BUBBLE_PRINT("VideoPlayer::AdjToWin2<");             
    }

// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::CalculateScreenRectSize
// -----------------------------------------------------------------------------
//
TSize CBubbleVideoPlayer::CalculateScreenRectSize( 
    const TSize& aCanvasSize,
    const TSize& aVideoFrameSize,
    TBool aArbitraryScalingSupported )
    {
    // Mismatch between canvas size and video size
    TInt xDelta( aCanvasSize.iWidth - aVideoFrameSize.iWidth );
    TInt yDelta( aCanvasSize.iHeight - aVideoFrameSize.iHeight );
    
    // Find minimum scaling factor to cover canvas by video.
    TInt dScaleFactor( KBubbleVideoIACoef ); // use integer arithmetic 
            
    if ( xDelta == 0 && yDelta == 0 )
        {
        // correct size, scaling not needed
        }
    else if ( xDelta < 0 && yDelta == 0 )
        {
        // wide, but cannot downscale -> just crop
        }
    else if ( yDelta < 0 && xDelta == 0 )
        {
        // tall, but cannot downscale -> just crop    
        }    
    else if ( xDelta > 0 && yDelta > 0 )
        {            
        // small, narrow and flat  -> enlarge
        TInt xProp( ( KBubbleVideoIACoef * aCanvasSize.iWidth ) / 
                    aVideoFrameSize.iWidth );
        TInt yProp( ( KBubbleVideoIACoef * aCanvasSize.iHeight ) / 
                    aVideoFrameSize.iHeight );
        
        dScaleFactor = xProp > yProp ? xProp : yProp;
        }
    else if ( xDelta < 0 && yDelta < 0 ) 
        {
        // large, wide and tall -> downscale
        TInt xProp( ( KBubbleVideoIACoef * aCanvasSize.iWidth ) / 
                    aVideoFrameSize.iWidth );
        TInt yProp( ( KBubbleVideoIACoef * aCanvasSize.iHeight ) / 
                    aVideoFrameSize.iHeight );
        
        dScaleFactor = xProp > yProp ? xProp : yProp;      
        }
    else if ( xDelta > 0 && yDelta <= 0 )
        {
        // narrow -> enlarge
        dScaleFactor = ( KBubbleVideoIACoef * aCanvasSize.iWidth ) / 
                       aVideoFrameSize.iWidth;
        }
    else if ( yDelta > 0 && xDelta <= 0 )
        {
        // flat  -> enlarge
        dScaleFactor = ( KBubbleVideoIACoef * aCanvasSize.iHeight) / 
                       aVideoFrameSize.iHeight;              
        }
    else
        {
        // do nothing
        }
    
    if ( !aArbitraryScalingSupported )
        {
        TInt remainder( dScaleFactor % KBubbleVideoIACoef );
        
        ///////////////////////////////////////////////////////
        // Fixed scales are 200, 162, 137, 112, 100, 50  ( %)
        // Fixed values not used: 325, 275, 250, 225, 
        ///////////////////////////////////////////////////////
        if ( dScaleFactor > ( 2 * KBubbleVideoIACoef ) )
            {
            dScaleFactor = ( 2 * KBubbleVideoIACoef ); // 200% is MAX    
            }
        else if ( remainder )
            {
            if ( dScaleFactor > KBubbleVideoIACoef )    
                {
                // upscale
                TInt scale = ( 100 * dScaleFactor ) / KBubbleVideoIACoef;
                
                if ( scale > 162 )
                    {
                    // 200%
                    dScaleFactor = 2 * KBubbleVideoIACoef;   
                    }
                else if ( scale > 137 )
                    {
                    // 162%
                    dScaleFactor = 162 * ( KBubbleVideoIACoef / 100 );    
                    }
                else if ( scale > 112 )
                    {
                    // 137%
                    dScaleFactor = 137 * ( KBubbleVideoIACoef / 100 );        
                    }
                else
                    {
                    // 112%
                    dScaleFactor = 112 * ( KBubbleVideoIACoef / 100 );    
                    }                    
                }
            else
                {
                // downscale    
                if ( remainder > ( KBubbleVideoIACoef / 2 ) )
                    {
                    dScaleFactor = KBubbleVideoIACoef; // 100%    
                    }
                else
                    {
                    dScaleFactor = ( KBubbleVideoIACoef / 2 ); // 50%    
                    }
                }
            }        
        }
    
    ////////////////////////////////////////////////
    // Calculate scaled frame size (virtual canvas)
    ////////////////////////////////////////////////
    TInt screenWidth = DivideAndCeil( 
                           ( dScaleFactor * aVideoFrameSize.iWidth ), 
                           KBubbleVideoIACoef );
    TInt screenHeight = DivideAndCeil( 
                           ( dScaleFactor * aVideoFrameSize.iHeight ), 
                           KBubbleVideoIACoef );
    
    return TSize( screenWidth, screenHeight );    
    }
    
// -----------------------------------------------------------------------------
// CBubbleVideoPlayer::CalculateClipAndScreenRectSize
// -----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::CalculateClipAndScreenRectSize( 
    const TSize& aWindowSize,
    const TSize& aVideoFrameSize,
    TSize& aClipSize,
    TSize& aScreenSize )    
    {
    // Check aspect ratios
    // aWindowSize or aVideoFrameSize cannot be zero in this phase.
    TInt windowAspectRatio = ( KBubbleVideoIACoef * aWindowSize.iWidth ) / 
                             aWindowSize.iHeight;
    TInt videoAspectRatio = ( KBubbleVideoIACoef * aVideoFrameSize.iWidth ) / 
                             aVideoFrameSize.iHeight;                             
    
    
    if ( videoAspectRatio == windowAspectRatio )
        {
        // Aspect ratios are same.
        
        // Set clip and screen rect equal to window.
        // Scaled video will cover window entirely.
        aClipSize = aWindowSize;
        aScreenSize = aWindowSize;            
        }
    else if ( windowAspectRatio > videoAspectRatio )
        {
        // Window is more wide screen than video.
        
        // Window height sets the limit.
        TInt maxHeight = aWindowSize.iHeight;

        // Calculate max scale according to allowed crop.
        // Formula:
        // ( 100 - crop )   visible    window_height x scale x video_width
        // -------------- = ------- =  -----------------------------------
        //      100       scaled_size  (scale)^2 x video_width x video_height
        //
        TInt dScaleFactor = ( maxHeight * 100 * KBubbleVideoIACoef ) /
            ( (100 - KBubbleMaxVideoCrop) * aVideoFrameSize.iHeight );
            
        TInt scaledWidth = 
            DivideAndCeil( ( dScaleFactor * aVideoFrameSize.iWidth ), 
                           KBubbleVideoIACoef );
        
        // Limit to window width
        if ( scaledWidth > aWindowSize.iWidth )
            {
            scaledWidth = aWindowSize.iWidth;
            dScaleFactor = ( KBubbleVideoIACoef * aWindowSize.iWidth ) / 
                             aVideoFrameSize.iWidth;                
            }
            
        TInt scaledHeight = 
            DivideAndCeil( ( dScaleFactor * aVideoFrameSize.iHeight ), 
                           KBubbleVideoIACoef );            
            
        aClipSize = TSize( scaledWidth, maxHeight );
        aScreenSize = TSize( scaledWidth  + ( 2 * KBubbleVideoClipMarginX ), 
                             scaledHeight + ( 2 * KBubbleVideoClipMarginY ) );    
        }
    else
        {
        // Video is more wide screen than window.
        
        // Window width sets the limit.
        TInt maxWidth = aWindowSize.iWidth;

        // Calculate max scale according to allowed crop.
        // Formula:
        // ( 100 - crop )   visible    window_width x scale x video_heigth
        // -------------- = ------- =  -----------------------------------
        //      100       scaled_size  (scale)^2 x video_width x video_height
        //
        TInt dScaleFactor = ( maxWidth * 100 * KBubbleVideoIACoef ) /
            ( (100 - KBubbleMaxVideoCrop) * aVideoFrameSize.iWidth );
                                       
            
        TInt scaledHeight = 
            DivideAndCeil( ( dScaleFactor * aVideoFrameSize.iHeight ), 
                           KBubbleVideoIACoef );
        
        // Limit to window height
        if ( scaledHeight > aWindowSize.iHeight )
            {
            scaledHeight = aWindowSize.iHeight;
            dScaleFactor = ( KBubbleVideoIACoef * aWindowSize.iHeight ) / 
                             aVideoFrameSize.iHeight;                
            }
            
        TInt scaledWidth = 
            DivideAndCeil( ( dScaleFactor * aVideoFrameSize.iWidth ), 
                           KBubbleVideoIACoef );            
            
        aClipSize = TSize( maxWidth, scaledHeight );
        aScreenSize = TSize( scaledWidth  + ( 2 * KBubbleVideoClipMarginX ), 
                             scaledHeight + ( 2 * KBubbleVideoClipMarginY ) );   
        }
    }

// ============================================================================
// Callbacks from VideoPlayerutility
// ============================================================================

// ----------------------------------------------------------------------------
// see MVideoPlayerUtilityObserver::MvpuoOpenComplete
// ----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::MvpuoOpenComplete( TInt aError )
    {
    BUBBLE_PRINTF("VideoPlayer::OpenComp:", aError);
    if ( aError == KErrNone )
        {
        // Prepare clip for playing
        iPlayerState = EVideoPreparing;
        iVideoPlayer->Prepare();
        }
    else // Report error
        {
        iPlayerState = EVideoError;
        iObserver.HandleVideoPlayerError( 
            MBubbleVideoPlayerObserver::EVideoPlayerInitializingFailure,
            aError );
        }
    }

// ----------------------------------------------------------------------------
// see MVideoPlayerUtilityObserver::MvpuoPrepareComplete
// ----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::MvpuoPrepareComplete( TInt aError )
    {
    BUBBLE_PRINTF("VideoPlayer::PrepComp:", aError);
    if ( aError == KErrNone )
        {
        iPlayerState = EVideoReady;
        iObserver.HandleVideoPlayerInitComplete();

        SetVolume( iVolume );
        SetRingingType( iRingingType );

        if ( iToBePlayed )
            {
            iVideoPlayer->Play();
            iPlayerState = EVideoPlaying;
            }
        }
    else  // Report error
        {
        iPlayerState = EVideoError;
        iObserver.HandleVideoPlayerError( 
            MBubbleVideoPlayerObserver::EVideoPlayerInitializingFailure,
            aError );
        }
    }

// ----------------------------------------------------------------------------
// see MVideoPlayerUtilityObserver::MvpuoFrameReady
// ----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::MvpuoFrameReady( CFbsBitmap& /*aFrame*/,
                                          TInt /*aError*/ )
    {
    // NOP
    }

// ----------------------------------------------------------------------------
// see MVideoPlayerUtilityObserver::MvpuoPlayComplete
// ----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::MvpuoPlayComplete( TInt aError )
    {
    BUBBLE_PRINTF("VideoPlayer::PlayComp:", aError);
    if ( aError == KErrNone )
        {
        iPlayerState = EVideoReady;
        
        if ( iRingingType == ETypeRingingOnce )
            {
            iObserver.HandleVideoPlayerPlayingComplete();
            } 
        else // continue playing
            {
            if ( !iRepeatsTrailPauseTimer )
                {
                iRepeatsTrailPauseTimer = 
                    CPeriodic::New( CActive::EPriorityStandard );
                }
            
            if ( iRepeatsTrailPauseTimer && 
                 !iRepeatsTrailPauseTimer->IsActive() )
                {
                TCallBack cb( RepeatsTrailPauseTimerCallback, this );
                iRepeatsTrailPauseTimer->Start( KBubbleVideoRepeatsTrailPause, 
                                         KBubbleVideoRepeatsTrailPause, 
                                         cb );
                }
            }        
        }
    else // report error
        {
        iPlayerState = EVideoError;
        iObserver.HandleVideoPlayerError( 
            MBubbleVideoPlayerObserver::EVideoPlayerPlayingFailure,
            aError );
        }
    }
        
// ----------------------------------------------------------------------------
// see MVideoPlayerUtilityObserver::MvpuoEvent
// ----------------------------------------------------------------------------
//
void CBubbleVideoPlayer::MvpuoEvent( const TMMFEvent& /*aEvent*/ )
    {
    // NOP
    }
    

//  End of File
