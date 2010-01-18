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


#ifndef C_BUBBLEVIDEOCONTROLLER_H
#define C_BUBBLEVIDEOCONTROLLER_H

#include "BMVideoPlayerObserver.h"
#include "BMVideoPlaybackObserver.h"
#include "BMBubbleManager.h"
#include <AknsItemID.h>

// FORWARD DECLARATIONS
class CBubbleVideoPlayer;
class CBubbleVideoContainer;

/**
 *  Video player controller.
 *
 *
 *  @lib BubbleManager
 *  @since S60 3.2
 */
class CBubbleVideoController : public CActive,
                               private MBubbleVideoPlayerObserver
    {
public:
    // State
    enum TVideoControllerState
        {
        EIdle,
        EReportingError,
        EReportingEvent    
        };

public:
    /**
     * Two phased constructor.
     *
     * @since S60 3.2
     * @param aBubbleManager Bubble manager reference.
     */
    static CBubbleVideoController* NewL( 
        CBubbleManager& aBubbleManager );
        
    // Destructor
    virtual ~CBubbleVideoController();

    /**
     * Sets playing parameters.
     *
     * @since S60 3.2
     * @param aFileName Video clip to be played.
     * @param aPlayMode Play mode.
     * @param aVolumeLevel Volume level.
     * @param aArbitraryScaling Set ETrue if free video scaling.
     * @param aObserver Observer for playback events.
     */
    void PrepareToPlayVideo( 
        const TDesC& aFileName,
        CBubbleManager::TBubbleVideoPlayMode aPlayMode,
        TInt aVolumeLevel,
        TBool aArbitraryScaling,
        MBubbleVideoPlaybackObserver* aObserver );

    /**
     * Starts video playing.
     *
     * @since S60 3.2
     */
    void CreatePlayerAndPlay();
    
    /**
     * Stops video playing deletes player.
     *
     * @since S60 3.2
     */
    void StopAndDeletePlayer();
    
    /**
     * Stops video playing.
     *
     * @since S60 3.2
     */
    void StopPlaying();
    
    /**
     * Mutes video playing.
     *
     * @since S60 3.2
     */
    void MutePlaying();
    
    /**
     * To be called when screen orientation changes.
     *
     * @since S60 3.2
     */
    void HandleLayoutChange();
    
    /**
     * Set panes for qcif sized video. 
     *
     * @since S60 3.2
     */
    void SetQcifVideoPaneRects( const TRect& aVideoPane,
                                const TRect& aUncropPane );
                            
    /**
     * Set panes for subqcif sized video. 
     *
     * @since S60 3.2
     */
    void SetSubQcifVideoPaneRects( const TRect& aVideoPane,
                                   const TRect& aUncropPane );                            
                              
    /**
     * Set parameters needed to draw video pane.
     *
     * @since Series 60 3.2
     * @param aFrameId Skin id of the frame.
     * @param aOuterRect Outer frame rectangle.
     * @param aInnerRect Inner frame rectangle.
     */
    void SetVideoPaneBackgroundFrame( const TAknsItemID& aFrameId,
                                      const TRect& aOuterRect,
                                      const TRect& aInnerRect );                                                             

private: // From MBubbleVideoPlayerObserver
    void HandleVideoPlayerError( 
        TBubbleVideoPlayerErrorEvent aEvent,
        TInt aError );
    
    void HandleVideoPlayerInitComplete();

    void HandleVideoPlayerPlayingComplete(); 
    
    void HandleVideoPlayerBlittingAreaDefined( const TRect& aBlitRect );
    
private: // From CActive
    void RunL();
        
    void DoCancel();
    
private:
    void DoPlayL();
    void ReportErrorAsync( 
        MBubbleVideoPlaybackObserver::TBubbleVideoPlaybackError aErrorType,
        TInt aErrorCode );
    void ReportEventAsync( 
        MBubbleVideoPlaybackObserver::TBubbleVideoPlaybackEvent aEvent);
    void LayoutContainer();
    void UpdateContainerBackground();
    void Reset();
    inline void EnableRedraws();
    inline void DisableRedraws();
    
private:
    CBubbleVideoController( 
        CBubbleManager& aBubbleManager );

    void ConstructL();

private: // data
    CBubbleManager& iBubbleManager;
    
    // Playback parameters
    HBufC* iFileName; // Owned
    CBubbleManager::TBubbleVideoPlayMode iPlayMode;
    TInt iVolumeLevel;
    TBool iArbitraryScaling;
    MBubbleVideoPlaybackObserver* iObserver;
        
    // Player and ui controls
    CBubbleVideoPlayer*    iPlayer; // Owned
    CBubbleVideoContainer* iContainer; // Owned
    TRect                  iUncropPane; // Video cropping

    // Active object state
    TVideoControllerState iState;
    MBubbleVideoPlaybackObserver::TBubbleVideoPlaybackError iErrorType;
    TInt iErrorCode;
    MBubbleVideoPlaybackObserver::TBubbleVideoPlaybackEvent iEventCode;
    
    // Video layouts, set from video outlooks
    TRect iVideoPaneRectQcif;
    TRect iUncropPaneRectQcif;
    TRect iVideoPaneRectSubQcif;
    TRect iUncropPaneRectSubQcif;
    
    // Data for container window.
    TAknsItemID iBgFrameId;
    TRect iBgFrameOuterRect;
    TRect iBgFrameInnerRect;
    };

#endif // C_BUBBLEVIDEOCONTROLLER_H

// End of File
