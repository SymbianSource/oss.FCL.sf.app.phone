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
* Description:  Video playback observer interface
*
*/


#ifndef M_BUBBLEVIDEOPLAYBACKOBSERVER_H
#define M_BUBBLEVIDEOPLAYBACKOBSERVER_H


/**
 *  Video playback observer interface
 *
 *
 *  @lib BubbleManager
 *  @since S60 v3.2
 */
class MBubbleVideoPlaybackObserver
    {
public:

    /**  Playback events */
    enum TBubbleVideoPlaybackEvent
        {
        EPlayerInitComplete,
        EPlaying,
        EPlayingComplete    
        };

    /**  Playback error types */        
    enum TBubbleVideoPlaybackError
        {
        EPlayerInitFailure,
        EPlaybackFailure    
        };        

    /**
     * HandleBubbleVideoPlaybackEvent
     *
     * @since S60 v3.2
     * @param aEvent Playback event.
     */
    virtual void HandleBubbleVideoPlaybackEvent( 
        TBubbleVideoPlaybackEvent aEvent ) = 0;        

    /**
     * HandleBubbleVideoPlaybackError
     *
     * @since S60 v3.2
     * @param aErrorType Playback error type.
     * @param aErrorCode System wide error code.
     */            
    virtual void HandleBubbleVideoPlaybackError(
        TBubbleVideoPlaybackError aErrorType, TInt aErrorCode ) = 0;
    };

#endif // M_BUBBLEVIDEOPLAYBACKOBSERVER_H
