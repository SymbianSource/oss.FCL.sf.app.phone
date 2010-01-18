/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Media reader interface.
*
*/


#ifndef MBUBBLEMEDIAREADER_H
#define MBUBBLEMEDIAREADER_H

class CFbsBitmap;
class MBubbleMediaReaderObserver;

/**
 *  Media reader interface.
 *
 *  @lib BubbleManager
 *  @since S60 v3.2
 */
class MBubbleMediaReader
    {

public:
    // === LOADING CONTROL ====================================================
    
    /**
     * Factory method to create a reader for given file.
     *
     * @since S60 v3.2
     * @param aTargetSize Image/frame is loaded to this size.
     * @param aScaleFactor Scaling factor (normally the ratio 
     *                     between target size and clip rect).
     * @param aClipRect Clip rect, relative to source dimensions.
     * @param aObserver Observer for reader events.
     */
    virtual void StartReadingL(
        const TSize& aTargetSize,
        const TReal& aScaleFactor,
        const TRect& aClipRect, 
        MBubbleMediaReaderObserver* aObserver  ) = 0;
    
    /**
     * Update scale and clip parameters for currently viewed bitmap.
     *
     * @since S60 v3.2
     * @param aTargetSize  Maximum size for destination bitmap.
     * @param aScaleFactor Scaling factor (normally the ratio between target
                           size and clip rect).
     * @param aClipRect    Clip rect, relative to source dimensions.
     * @return System wide error code.
     */
    virtual TInt SetScaleAndClip(
        const TSize& aTargetSize,
        const TReal& aScaleFactor,
        const TRect& aClipRect ) = 0;
    
    /**
     * Cancels reading in progress.
     *
     * @since S60 v3.2
     */
    virtual void CancelReading() = 0;
    
    // === DATA ACCESS ========================================================
    /**
     * Gets size of source image/frame.
     *
     * @since S60 v3.2
     * @return Size of source image.
     */
    virtual const TSize SourceSize() const = 0;
    
    /**
     * Checks if source is animation.
     *
     * @since S60 v3.2
     * @return ETrue if source is animation.
     */
    virtual TBool IsAnimation() const = 0;
    
    /**
     * Gets pointer to frame buffer.
     *
     * @since S60 v3.2
     * @return Pointer to frame buffer.
     */
    virtual const CFbsBitmap* FrameBuffer() const = 0;
    
    /**
     * Gets mask for the current frame in frame buffer.
     *
     * @since S60 v3.2
     * @return Pointer to mask frame.
     */
    virtual const CFbsBitmap* Mask() const = 0;
    
    // === ANIMATION CONTROL ==================================================
    /**
     * Starts animation.
     *
     * @since S60 v3.2
     */
    virtual void Play() = 0;
    
    /**
     * Stops animation.
     *
     * @since S60 v3.2
     */
    virtual void Stop() = 0;
    
    // Virtual destructor
    virtual ~MBubbleMediaReader() {};
    };


#endif // MBUBBLEMEDIAREADER_H
