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
* Description:  Video player observer interface.
*
*/


#ifndef M_BUBBLEVIDEOPLAYEROBSERVER_H
#define M_BUBBLEVIDEOPLAYEROBSERVER_H

/**
 *  Video player observer interface.
 *
 *
 *  @lib BubbleManager
 *  @since S60 v3.2
 */
class MBubbleVideoPlayerObserver  
    {
    public:
        // Enumerates different failure events.
        //
        enum TBubbleVideoPlayerErrorEvent
            {
            EVideoPlayerInitializingFailure, // Failure during initialization
            EVideoPlayerPlayingFailure // Failure during playback
            };
        
        /**
        * This method is called when playing error occurs.
        *
        * @since S60 v3.2
        * @param aEvent Error event.
        * @param aError Error code.
        */
        virtual void HandleVideoPlayerError( 
            TBubbleVideoPlayerErrorEvent aEvent,
            TInt aError ) = 0;

        /**
        * This method is called when player initialization is complete.
        *
        * @since S60 v3.2
        */
        virtual void HandleVideoPlayerInitComplete() = 0;

        /**
        * This method is called when video clip has played to end.
        *
        * @since S60 v3.2
        */
        virtual void HandleVideoPlayerPlayingComplete() = 0;
        
        /**
        * This method is called when player knows exact blitting
        * area, the screen area that will be covered by video image.
        * Observer may relayout the container window accordingly.
        *
        * @since S60 v3.2
        * @param aBlitRect Blit rectangle relative to screen.
        */
        virtual void HandleVideoPlayerBlittingAreaDefined( 
            const TRect& aBlitRect ) = 0;
    };

#endif // M_BUBBLEVIDEOPLAYEROBSERVER_H

// End of File
