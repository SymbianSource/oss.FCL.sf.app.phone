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
* Description:  Video player controller.
*
*/


// INCLUDES
#include    <AudioPreference.h>
#include    "BMVideoController.h"
#include    "BMVideoPlayer.h"
#include    "BMBubbleVideoContainer.h"
#include    "BMVideoPlaybackObserver.h"
#include    "BMTrace.h"

// ======== LOCAL FUNCTIONS ========
static CBubbleVideoPlayer::TRingingType ConvertRingingType(
    CBubbleManager::TBubbleVideoPlayMode aRingingMode );    

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleVideoController::CBubbleVideoController(
    CBubbleManager& aBubbleManager ) :
    CActive( CActive::EPriorityStandard ),
    iBubbleManager( aBubbleManager )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CBubbleVideoController* CBubbleVideoController::NewL(
    CBubbleManager& aBubbleManager )
    {
    CBubbleVideoController* self = new( ELeave ) CBubbleVideoController( 
        aBubbleManager );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    
    return self;    
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleVideoController::~CBubbleVideoController()
    {
    Cancel();
    
    if ( iPlayer )
        {
        iPlayer->StopPlaying();
        }
    
    Reset();
    }

// ---------------------------------------------------------------------------
// Reset
// ---------------------------------------------------------------------------
//    
void CBubbleVideoController::Reset()
    {
    delete iPlayer;
    iPlayer = NULL;
    delete iContainer;
    iContainer = NULL;
    delete iFileName;    
    iFileName = NULL;
    iVolumeLevel = 0;
    iObserver = NULL;
    }

// ---------------------------------------------------------------------------
// PrepareToPlayVideo
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::PrepareToPlayVideo( 
    const TDesC& aFileName,
    CBubbleManager::TBubbleVideoPlayMode aPlayMode,
    TInt aVolumeLevel,
    TBool aArbitraryScaling,
    MBubbleVideoPlaybackObserver* aObserver )
    {
    delete iFileName;
    iFileName = aFileName.Alloc();
    iPlayMode = aPlayMode;
    iVolumeLevel = aVolumeLevel;
    iArbitraryScaling = aArbitraryScaling;
    iObserver = aObserver;
    }

// ---------------------------------------------------------------------------
// CreatePlayerAndPlay
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::CreatePlayerAndPlay()
    {
    BUBBLE_PRINT("VideoCtrl::Play>");
    TRAPD( err, DoPlayL() );
   
    if ( err != KErrNone )
        {
        ReportErrorAsync( MBubbleVideoPlaybackObserver::EPlayerInitFailure,
                          err );            
        }
    BUBBLE_PRINTF("VideoCtrl::Play<:", err );                        
    }

// ---------------------------------------------------------------------------
// StopAndDeleteVideoPlayer
// ---------------------------------------------------------------------------
//    
void CBubbleVideoController::StopAndDeletePlayer()
    {
    BUBBLE_PRINT("VideoCtrl::StopAndDel");
    if ( iPlayer )
        {
        iPlayer->StopPlaying();
        }
    // To delete player.
    Reset();
    }
    
// ---------------------------------------------------------------------------
// Stop
// ---------------------------------------------------------------------------
//    
void CBubbleVideoController::StopPlaying()
    {
    BUBBLE_PRINT("VideoCtrl::Stop");
    if ( iPlayer )
        {
        iPlayer->StopPlaying();
        }
    }    

// ---------------------------------------------------------------------------
// Mute
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::MutePlaying()
    {
    BUBBLE_PRINT("VideoCtrl::Mute");
    if ( iPlayer )
        {
        iPlayer->MuteAudio();    
        }
    }

// ---------------------------------------------------------------------------
// HandleLayoutChange
// ---------------------------------------------------------------------------
//    
void CBubbleVideoController::HandleLayoutChange()
    {
    BUBBLE_PRINT("VideoCtrl::HLC>");
    if ( iPlayer && ( iPlayer->State() >= CBubbleVideoPlayer::EVideoReady ) )
        {
        BUBBLE_PRINT("VideoCtrl::HLC>1");
        iPlayer->PausePlaying();
        EnableRedraws();    
        LayoutContainer();
        if ( iArbitraryScaling )
            {
            iPlayer->AdjustToWindow2( iContainer->GetWindow() );     
            }
        else
            {
            iPlayer->AdjustToWindow( iContainer->GetWindow(), 
                                     iUncropPane,
                                     EFalse );    
            }            
        iContainer->DrawNow();
        DisableRedraws();
        iPlayer->ResumePlaying();
        }
    else if ( iContainer )
        {
        BUBBLE_PRINT("VideoCtrl::HLC>2");
        LayoutContainer();    
        }
    BUBBLE_PRINT("VideoCtrl::HLC<");                
    }
    
// ---------------------------------------------------------------------------
// SetQcifVideoRects
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::SetQcifVideoPaneRects(
    const TRect& aVideoPane,
    const TRect& aUncropPane )
    {
    iVideoPaneRectQcif = aVideoPane;
    iUncropPaneRectQcif = aUncropPane;
    }
    
// ---------------------------------------------------------------------------
// SetSubQcifVideoRects
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::SetSubQcifVideoPaneRects(
    const TRect& aVideoPane,
    const TRect& aUncropPane )
    {
    iVideoPaneRectSubQcif = aVideoPane;
    iUncropPaneRectSubQcif = aUncropPane;   
    }    
                            
// ---------------------------------------------------------------------------
// SetVideoPaneBackgroundFrame
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::SetVideoPaneBackgroundFrame( 
    const TAknsItemID& aFrameId,
    const TRect& aOuterRect,
    const TRect& aInnerRect )
    {
    TBool skinChanged = ( aFrameId == iBgFrameId ) ? EFalse : ETrue;
    
    iBgFrameId = aFrameId;
    iBgFrameOuterRect = aOuterRect;
    iBgFrameInnerRect = aInnerRect;
    
    if ( iContainer && skinChanged )
        {
        UpdateContainerBackground();
        }
    }
    
// ---------------------------------------------------------------------------
// ConvertRingingType
// ---------------------------------------------------------------------------
//    
CBubbleVideoPlayer::TRingingType ConvertRingingType(
    CBubbleManager::TBubbleVideoPlayMode aRingingMode )
    {
    CBubbleVideoPlayer::TRingingType aRingingType
        = CBubbleVideoPlayer::ETypeRinging; 
    
    switch ( aRingingMode )
        {
        case CBubbleManager::EPlayInLoop:
            aRingingType = CBubbleVideoPlayer::ETypeRinging;
            break;
        case CBubbleManager::EPlayAscending:
            aRingingType = CBubbleVideoPlayer::ETypeAscending;
            break;
        case CBubbleManager::EPlayOnce:
            aRingingType = CBubbleVideoPlayer::ETypeRingingOnce;
            break;
        default:
            break;
        }
    
    return aRingingType;
    }    

// ---------------------------------------------------------------------------
// DoPlayL
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::DoPlayL()
    {
    if ( !iContainer )
        {
        // Create container window for video playback
        iContainer = CBubbleVideoContainer::NewL( &iBubbleManager ); 
        LayoutContainer();
        // Container window is set visible just before play. 
        iContainer->MakeVisible( EFalse );
        iContainer->ActivateL();
        }
    
    // Create video player
    delete iPlayer;
    iPlayer = NULL;
    iPlayer = CBubbleVideoPlayer::NewL( 
              *iFileName,
              KAudioPriorityPhoneCall, 
              KAudioPrefIncomingCall,
              *this, 
              iContainer->GetWindow() );

    // Play
    iPlayer->Play( ConvertRingingType(iPlayMode), iVolumeLevel );
    }

// ---------------------------------------------------------------------------
// LayoutContainer
// ---------------------------------------------------------------------------
//    
void CBubbleVideoController::LayoutContainer()
    {
    if ( !iContainer )
        {
        return;    
        }
    
    CBubbleVideoPlayer::TVideoResolution resolution =
        CBubbleVideoPlayer::EVideoQCIF; 
    if ( iPlayer && 
         ( iPlayer->State() >= CBubbleVideoPlayer::EVideoReady ) )
        {
        resolution = iPlayer->VideoResolution();    
        }
        
    TPoint offset = iBubbleManager.PositionRelativeToScreen();
    if ( resolution == CBubbleVideoPlayer::EVideoSubQCIF &&
         !iArbitraryScaling )
        {
        iContainer->SetRect( iVideoPaneRectSubQcif );
        iUncropPane = iUncropPaneRectSubQcif;
        iUncropPane.Move( offset );
        }
    else
        {
        iContainer->SetRect( iVideoPaneRectQcif );
        iUncropPane = iUncropPaneRectQcif;
        iUncropPane.Move( offset );
        }        
    
    UpdateContainerBackground();
    }
    
// ---------------------------------------------------------------------------
// UpdateContainerBackground
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::UpdateContainerBackground()
    {
    // Set relative to container position
    TPoint cntPosition = iContainer->Position();
    TRect outerCntRect( iBgFrameOuterRect );
    TRect innerCntRect( iBgFrameInnerRect );
    outerCntRect.Move( -cntPosition );
    innerCntRect.Move( -cntPosition );
    iContainer->SetBackgroundFrame( iBgFrameId, 
                                    outerCntRect, 
                                    innerCntRect );    
    }
    
// ---------------------------------------------------------------------------
// ReportErrorAsync
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::ReportErrorAsync(
    MBubbleVideoPlaybackObserver::TBubbleVideoPlaybackError aErrorType,
    TInt aErrorCode )
    {
    BUBBLE_PRINT("VideoCtrl::RErrA");
    Cancel();
    iState = EReportingError;
    iErrorType = aErrorType;
    iErrorCode = aErrorCode;
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    SetActive();    
    }
    
// ---------------------------------------------------------------------------
// ReportEventAsync
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::ReportEventAsync(
    MBubbleVideoPlaybackObserver::TBubbleVideoPlaybackEvent aEvent )
    {
    BUBBLE_PRINT("VideoCtrl::REvnA");
    Cancel();
    iState = EReportingEvent;
    iEventCode = aEvent;
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    SetActive();    
    }    


// ---------------------------------------------------------------------------
// From class MBubbleVideoPlayerObserver.
// HandleVideoPlayerError
// ---------------------------------------------------------------------------
// 
void CBubbleVideoController::HandleVideoPlayerError( 
    TBubbleVideoPlayerErrorEvent aEvent,
    TInt aError )
    {
    BUBBLE_PRINTF("VideoCtrl::HVPE.evn:", aEvent );
    BUBBLE_PRINTF("VideoCtrl::HVPE.err:", aError );
    switch( aEvent )
        {
        case EVideoPlayerInitializingFailure:
            ReportErrorAsync( 
                MBubbleVideoPlaybackObserver::EPlayerInitFailure, aError );    
            break;
        case EVideoPlayerPlayingFailure:
            ReportErrorAsync( 
                MBubbleVideoPlaybackObserver::EPlaybackFailure, aError );
            break;
        default:
            break;            
        }
    }

// ---------------------------------------------------------------------------
// From class MBubbleVideoPlayerObserver.
// HandleVideoPlayerInitComplete
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::HandleVideoPlayerInitComplete()
    {
    BUBBLE_PRINT("VideoCtrl::HVPInitComp");
    if ( iObserver )
        {
        ReportEventAsync( 
            MBubbleVideoPlaybackObserver::EPlayerInitComplete );
        }
    
    if ( iArbitraryScaling )
        {
        iPlayer->AdjustToWindow2( iContainer->GetWindow() );    
        }
    else
        {
        if ( iPlayer->VideoResolution() == CBubbleVideoPlayer::EVideoSubQCIF  )
            {
            // relayout for subqcif
            LayoutContainer();    
            }
        iPlayer->AdjustToWindow( iContainer->GetWindow(), 
                                 iUncropPane,
                                 iArbitraryScaling );               
        }
    
    // Make container visible before playin starts.
    iContainer->MakeVisible( ETrue );
     // Draw container now to prevent playback time redraw event.
    iContainer->DrawNow();
    DisableRedraws(); 
    }

// ---------------------------------------------------------------------------
// From class MBubbleVideoPlayerObserver.
// HandleVideoPlayerPlayingComplete
// ---------------------------------------------------------------------------
// 
void CBubbleVideoController::HandleVideoPlayerPlayingComplete()
    {
    BUBBLE_PRINT("VideoCtrl::HVPPlayComp");
    if ( iObserver )
        {
        ReportEventAsync(
            MBubbleVideoPlaybackObserver::EPlayingComplete );    
        }
    }

// ---------------------------------------------------------------------------
// From class MBubbleVideoPlayerObserver.
// HandleVideoPlayerPlayingComplete
// ---------------------------------------------------------------------------
//    
void CBubbleVideoController::HandleVideoPlayerBlittingAreaDefined( 
    const TRect& aBlitRect )
    {
    if ( iArbitraryScaling )
        {
        TPoint offset = iBubbleManager.PositionRelativeToScreen();
        TRect containerRect( aBlitRect );
        containerRect.Move( -offset );
        iContainer->SetRect( containerRect );    
        }
    }

// ---------------------------------------------------------------------------
// From class CActive.
// RunL
// ---------------------------------------------------------------------------
//    
void CBubbleVideoController::RunL()
    {
    BUBBLE_PRINT("VideoCtrl::RunL");
    switch ( iState )    
        {
        case EReportingError:
            if ( iObserver )
                {
                EnableRedraws();
                iObserver->HandleBubbleVideoPlaybackError( 
                    iErrorType, iErrorCode );                        
                }
            break;
        case EReportingEvent:
            if ( iObserver )
                {
                iObserver->HandleBubbleVideoPlaybackEvent( 
                    iEventCode );                        
                }
            break;            
        default:
            break;            
        }
    
    iState = EIdle;
    }

// ---------------------------------------------------------------------------
// From class CActive.
// DoCancel
// ---------------------------------------------------------------------------
//        
void CBubbleVideoController::DoCancel()
    {
    // Request is completed immediately before SetActive.
    }

// ---------------------------------------------------------------------------
// EnableRedraws
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::EnableRedraws()
    {
    iContainer->SetRedrawDisabled( EFalse );    
    }

// ---------------------------------------------------------------------------
// DisableRedraws
// ---------------------------------------------------------------------------
//
void CBubbleVideoController::DisableRedraws()
    {
    if ( iArbitraryScaling )    
        {
        // Container is covered entirely by video only
        // when arbitrary scaling is supported.
        iContainer->SetRedrawDisabled( ETrue );    
        }    
    }

// End of File
