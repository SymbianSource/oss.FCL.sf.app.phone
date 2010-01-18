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
* Description:  Video ringing tone player
*
*/


// INCLUDE FILES
#include "CPhoneVideoPlayer.h"

#include "PhoneUI.pan"
#include "MPhoneVideoPlayerObserver.h"
#include "CPhoneRingingTone.h"
#include "PhoneConstants.h"
#include "PhoneLogger.h"

#include <eikenv.h>        // CEikEnv 
#include <coemain.h>       // CCoeEnv 
#include <VideoPlayer.h>   // CVideoPlayerUtility

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::CPhoneVideoPlayer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneVideoPlayer::CPhoneVideoPlayer(
    MPhoneVideoPlayerObserver& aObserver )
    :
    iObserver( aObserver ),
    iVideoPlayer( NULL ),
    iPlayerState( EVideoClosed ),
    iVolume( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneVideoPlayer::ConstructL( 
    const TDesC& aFileName, 
    TInt aPriority, 
    TUint aPreference,
    RWindow& aVideoTarget  )
    {
    // Screen and clip rectangles to window dimensions
    TPoint wndPosition( aVideoTarget.AbsPosition() );
    TSize wndSize( aVideoTarget.Size() );
    TRect wndRect( wndPosition, wndSize );
    
    // Create video player instance
    iVideoPlayer = CVideoPlayerUtility::NewL(
        *this,
        aPriority,
        static_cast<TMdaPriorityPreference>( aPreference ),
        CCoeEnv::Static()->WsSession(),
        *CCoeEnv::Static()->ScreenDevice(),    
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
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneVideoPlayer* CPhoneVideoPlayer::NewL(
    const CPhoneRingingTone& aRingingTone, 
    TInt aPriority, 
    TUint aPreference,
    MPhoneVideoPlayerObserver& aObserver, 
    RWindow& aVideoTarget,
    TBool aExtSecNeeded )
    {
    // Check the file DRM property if extend secure is needed.
    if ( aExtSecNeeded )
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneVideoPlayer::NewL - check DRM extend security" );
        if ( !aRingingTone.IsFileInRom() &&
             !aRingingTone.IsFileDrmProtected() )
            {
            __PHONELOG( EBasic, EPhoneUIView, "CPhoneVideoPlayer::NewL - DRM extend security permission denied" );
            User::Leave( KErrPermissionDenied );
            }
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneVideoPlayer::NewL - check DRM extend security - ok" );
        }
    
    CPhoneVideoPlayer* self = new(ELeave) CPhoneVideoPlayer( aObserver );
        
    CleanupStack::PushL( self );
    self->ConstructL( 
        aRingingTone.FileName(), 
        aPriority, 
        aPreference, 
        aVideoTarget );
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::New
// Two-phased constructor (non-leaving) 
// -----------------------------------------------------------------------------
//
CPhoneVideoPlayer* CPhoneVideoPlayer::New(
    const CPhoneRingingTone& aRingingTone,  
    TInt aPriority, 
    TUint aPreference,
    MPhoneVideoPlayerObserver& aObserver, 
    RWindow& aVideoTarget,
    TBool aExtSecNeeded )
    {
    CPhoneVideoPlayer* self = NULL;

    TRAP_IGNORE( self = CPhoneVideoPlayer::NewL(
        aRingingTone, 
        aPriority, 
        aPreference,
        aObserver,   
        aVideoTarget,
        aExtSecNeeded ) );
    
    return self; // this is NULL if NewL leaves
    }

// Destructor
CPhoneVideoPlayer::~CPhoneVideoPlayer()
    {
    if ( iVolumeRampTimer )
        {
        iVolumeRampTimer->Cancel();
        delete iVolumeRampTimer;
        }

    if ( iVideoPlayer )
        {
        iVideoPlayer->Close();
        delete iVideoPlayer;
        }

    iFileHandle.Close();     
             
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::Play
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneVideoPlayer::Play( 
    TRingingType aRingType, 
    TInt aVolume )
    {
    __PHONELOG2( 
        EBasic, 
        EPhoneUIView, 
        "CPhoneVideoPlayer::Play - aRingType(%d), aVolume(%d)",
        aRingType,
        aVolume );

    __ASSERT_DEBUG( iVideoPlayer, Panic( EPhoneViewGeneralError ) );
    
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

    return;
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::StopPlaying
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneVideoPlayer::StopPlaying()
    {
    __PHONELOG( EBasic, EPhoneUIView, "CPhoneVideoPlayer::StopPlaying" );

    iToBePlayed = EFalse;

    if ( iVolumeRampTimer )
        {
        iVolumeRampTimer->Cancel();
        }
    
    if ( iPlayerState == EVideoPlaying )
        {
        iVideoPlayer->Stop();
        iPlayerState = EVideoReady;
        }
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::PausePlaying
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
TInt CPhoneVideoPlayer::PausePlaying()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneVideoPlayer::StopPlaying ");
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
        
    return err;        
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::ResumePlaying
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CPhoneVideoPlayer::ResumePlaying()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneVideoPlayer::ResumePlaying ");
    
    if ( iPlayerState == EVideoPaused )
        {
        iVideoPlayer->Play(); 
        iPlayerState = EVideoPlaying;       
        }
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::MuteAudio
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneVideoPlayer::MuteAudio()
    {
    __PHONELOG( EBasic, EPhoneUIView, "CPhoneVideoPlayer::MuteAudio" );
    
    if ( iVolumeRampTimer )
        {
        iVolumeRampTimer->Cancel();
        }

    iVolume = 0;
    SetVolume( 0 );
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::SetVolume
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneVideoPlayer::SetVolume( TInt aVolume )
    {
    __PHONELOG1( 
        EBasic, 
        EPhoneUIView, 
        "CPhoneVideoPlayer::SetVolume - aVolume(%d)", 
        aVolume );
    
    TInt maxVolume( iVideoPlayer->MaxVolume() );
    maxVolume = ( maxVolume > 0 ) ? maxVolume : KPhoneVideoMaxVolumeLevel; 

    TInt scaledVolume = ( aVolume * maxVolume ) / KPhoneVideoMaxVolumeLevel;
    
    TRAP_IGNORE( iVideoPlayer->SetVolumeL( scaledVolume ) );
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::SetRingingType
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneVideoPlayer::SetRingingType( TRingingType aRingingType )
    {
    __PHONELOG1( 
        EBasic, 
        EPhoneUIView, 
        "CPhoneVideoPlayer::SetRingingType - aRingingType(%d)",
        aRingingType );
    
    if ( aRingingType == ETypeAscending )
        {
        if ( !iVolumeRampTimer )
            {
            iVolumeRampTimer = CPeriodic::New( CActive::EPriorityStandard );
            }

        if ( iVolumeRampTimer && !iVolumeRampTimer->IsActive() )
            {
            TCallBack cb( VolumeRampTimerCallback, this );

            SetVolume( KPhoneVideoMinVolumeLevel );
            iRampedVolume = KPhoneVideoMinVolumeLevel;
            iVolumeRampTimer->Start( 
                KPhoneVideoVolumeRampInterval, 
                KPhoneVideoVolumeRampInterval, 
                cb );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::VolumeRampTimerCallback
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPhoneVideoPlayer::VolumeRampTimerCallback( TAny* aObj )
    {
    return static_cast<CPhoneVideoPlayer*>( aObj )->DoVolumeRamp();
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::DoVolumeRamp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPhoneVideoPlayer::DoVolumeRamp()
    {
    if ( iRampedVolume < iVolume )
        {
        iRampedVolume = iRampedVolume + KPhoneVideoVolumeRampStep;
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
// CPhoneVideoPlayer::State
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPhoneVideoPlayer::TVideoPlayerState CPhoneVideoPlayer::State() const
    {
    return iPlayerState;
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::VideoResolution
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPhoneVideoPlayer::TVideoResolution CPhoneVideoPlayer::VideoResolution() const
    {
    TSize frameSize( VideoFrameSize() );
    
    // SubQCCIF video resolution is 128x96 pixels
    if ( frameSize.iWidth == KPhoneVideoSubQCCIFWidth && 
    	frameSize.iHeight == KPhoneVideoSubQCCIFHeight )
        {
        return EVideoSubQCIF;
        }

    return EVideoQCIF;
    }

// -----------------------------------------------------------------------------
// CPhoneVideoPlayer::VideoResolution
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
TSize CPhoneVideoPlayer::VideoFrameSize() const
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
// CPhoneVideoPlayer::AdjustToWindow
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CPhoneVideoPlayer::AdjustToWindow( RWindow& aDisplayWindow,
	const TRect& aUncropPane )
    {
    __ASSERT_DEBUG( iPlayerState == EVideoReady, Panic( EPhoneViewGeneralError ) );
        
    // Get video frame dimensions
    TSize frameSize( VideoFrameSize() );
    
    if ( frameSize.iWidth == 0 || frameSize.iHeight == 0  )
        {
        return;            
        }
    
    // Uncrop pane dimensions
    TSize uncropPaneSize( aUncropPane.Size() );
        
    // To cover display window:
    // The video is scaled to match to dimensions of uncrop pane defined
    // in Call Handling LAF. Video is centered to uncrop pane if exact
    // match is not possible. 
    // Assumption is that video can be scaled to 50,150 or 200
    // percent from its original size (can't be scaled freely). 
    
    /////////////////////////////
    // Calculate scaling factor
    /////////////////////////////
    TInt dScaleFactor( KBaseScaleFactor ); // use integer arithmetic 
    
    TInt xDelta( uncropPaneSize.iWidth - frameSize.iWidth );
    TInt yDelta( uncropPaneSize.iHeight - frameSize.iHeight );
            
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
        TInt xProp( ( KBaseScaleFactor * uncropPaneSize.iWidth ) / frameSize.iWidth );
        TInt yProp( ( KBaseScaleFactor * uncropPaneSize.iHeight ) / frameSize.iHeight );
        
        dScaleFactor = xProp > yProp ? xProp : yProp;
        }
    else if ( xDelta < 0 && yDelta < 0 ) 
        {
        // large, wide and tall -> downscale
        TInt xProp( ( KBaseScaleFactor * uncropPaneSize.iWidth ) / frameSize.iWidth );
        TInt yProp( ( KBaseScaleFactor * uncropPaneSize.iHeight ) / frameSize.iHeight );
        
        dScaleFactor = xProp > yProp ? xProp : yProp;      
        }
    else if ( xDelta > 0 && yDelta <= 0 )
        {
        // narrow -> enlarge
        dScaleFactor = ( KBaseScaleFactor * uncropPaneSize.iWidth ) / frameSize.iWidth;
        }
    else if ( yDelta > 0 && xDelta <= 0 )
        {
        // flat  -> enlarge
        dScaleFactor = ( KBaseScaleFactor * uncropPaneSize.iHeight ) / frameSize.iHeight;
        }
    else
        {
        // do nothing
        }
    
    // Convert to float: 0.5, 1.5, 2.0 ..
    TInt scaleFactor( dScaleFactor / KBaseScaleFactor );
    TInt remainder( dScaleFactor % KBaseScaleFactor );
    TReal32 fScaleFactor = (TReal) scaleFactor ;
    
    if ( scaleFactor > 0 ) // upscale
        {
        if ( remainder > KHalfBaseScale )
            {
            fScaleFactor = fScaleFactor + KFullScaleFactor;    
            }
        else if ( remainder > 0 )
            {
            fScaleFactor = fScaleFactor + KHalfScaleFactor;    
            }
        else // 0
            {
            }
        }
    else // downscale
        {
        if ( remainder > KHalfBaseScale )
            {
            fScaleFactor = KFullScaleFactor;    
            }
        else
            {
            fScaleFactor = KHalfScaleFactor;    
            }
        }
    
    ////////////////////////////////////////////////
    // Calculate scaled frame size (virtual canvas)
    ////////////////////////////////////////////////
    TReal32 canvasWidth = fScaleFactor * (TReal32)frameSize.iWidth;
    TReal32 canvasHeight = fScaleFactor * (TReal32)frameSize.iHeight;
    TSize canvasSize( (TInt)canvasWidth, (TInt)canvasHeight );
    
    ////////////////////////////////////////////////
    // Crop by centering displayRect to canvasRect
    ////////////////////////////////////////////////
    TRect canvasRect( aUncropPane.iTl, canvasSize );     
    TInt offsetX = (uncropPaneSize.iWidth - canvasSize.iWidth) / KCentering;
    TInt offsetY = (uncropPaneSize.iHeight - canvasSize.iHeight) / KCentering;
    canvasRect.Move( offsetX, offsetY );
    
    // Video is autoscaled by video player
    // iVideoPlayer->SetScaleFactorL(...);
    
    TRect clipRect( aDisplayWindow.AbsPosition(), aDisplayWindow.Size() );
    
    ////////////////////////////////////////////////
    // Update settings to player
    ////////////////////////////////////////////////
    TRAP_IGNORE( 
        iVideoPlayer->SetDisplayWindowL( 
            CCoeEnv::Static()->WsSession(),
            *CCoeEnv::Static()->ScreenDevice(),
            aDisplayWindow,
            canvasRect,
           clipRect ) );
               
    }


// =========================================================
// Callbacks from VideoPlayerutility
// =========================================================

// ---------------------------------------------------------
// see MVideoPlayerUtilityObserver::MvpuoOpenComplete
// ---------------------------------------------------------
void CPhoneVideoPlayer::MvpuoOpenComplete( TInt aError )
    {
    __PHONELOG1( 
        EBasic, 
        EPhoneUIView, 
        "CPhoneVideoPlayer::MvpuoOpenComplete - aError(%d)", 
        aError );

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
            MPhoneVideoPlayerObserver::EVideoPlayerInitializingFailure,
            aError );
        }
    }

// ---------------------------------------------------------
// see MVideoPlayerUtilityObserver::MvpuoPrepareComplete
// ---------------------------------------------------------
void CPhoneVideoPlayer::MvpuoPrepareComplete( TInt aError )
    {
    __PHONELOG1( 
        EBasic, 
        EPhoneUIView, 
        "CPhoneVideoPlayer::MvpuoPrepareComplete - aError(%d)", 
        aError );

    if ( aError == KErrNone )
        {
        iPlayerState = EVideoReady;
        iObserver.HandleVideoPlayerInitComplete();

        SetVolume( iVolume );
        SetRingingType( iRingingType );

        if ( iToBePlayed )
            {
            iVideoPlayer->Play();
            }
        }
    else  // Report error
        {
        iPlayerState = EVideoError;
        iObserver.HandleVideoPlayerError( 
            MPhoneVideoPlayerObserver::EVideoPlayerInitializingFailure,
            aError );
        }
    }

// ---------------------------------------------------------
// see MVideoPlayerUtilityObserver::MvpuoFrameReady
// ---------------------------------------------------------
void CPhoneVideoPlayer::MvpuoFrameReady( CFbsBitmap& /*aFrame*/,
                                         TInt /*aError*/ )
    {
    // NOP
    }

// ---------------------------------------------------------
// see MVideoPlayerUtilityObserver::MvpuoPlayComplete
// Trapping callback as leaving not allowed.
// ---------------------------------------------------------
void CPhoneVideoPlayer::MvpuoPlayComplete( TInt aError )
    {
    __PHONELOG1( 
        EBasic, 
        EPhoneUIView, 
        "CPhoneVideoPlayer::MvpuoPlayComplete - aError(%d)", 
        aError );

    if ( aError == KErrNone )
        {
        iPlayerState = EVideoReady;
        
        if ( iRingingType == ETypeRingingOnce )
            {
            iObserver.HandleVideoPlayerPlayingComplete();
            } 
        else // continue playing
            {
            iVideoPlayer->Play();
            iPlayerState = EVideoPlaying;
            }        
        }
    else // report error
        {
        iPlayerState = EVideoError;
        iObserver.HandleVideoPlayerError( 
            MPhoneVideoPlayerObserver::EVideoPlayerPlayingFailure,
            aError );
        }
    }
        
// ---------------------------------------------------------
// see MVideoPlayerUtilityObserver::MvpuoEvent
// Trapping callback as leaving not allowed.
// ---------------------------------------------------------
void CPhoneVideoPlayer::MvpuoEvent( const TMMFEvent& /*aEvent*/ )
    {
    // NOP
    }
    
//  End of File
