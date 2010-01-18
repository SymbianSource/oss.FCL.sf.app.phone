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
* Description:  Base class for media readers.
*
*/


#ifndef BMMEDIAREADER_H
#define BMMEDIAREADER_H

#include <e32base.h>
#include "BMMediaReaderInterface.h"

class MBubbleMediaReaderObserver;

/**
 *  Base class for media readers.
 *
 *  @lib BubbleManager
 *  @since S60 v3.2
 */
class CBubbleMediaReader : public CBase,
                           public MBubbleMediaReader
    {
public:    
    virtual ~CBubbleMediaReader();

    /**
     * Derived class should call this when image or new 
     * animation frame has been read to frame buffer.
     *
     * @since S60 v3.2
     */
    void HandleReadingComplete();
    
    /**
     * Derived class should call this in error situation.
     *
     * @since S60 v3.2
     * @param aError System wide error code.
     */
    void HandleReadingError( TInt aError );
    
    
protected: // from MBubbleMediaReader
    
    virtual void StartReadingL( 
        const TSize& aTargetSize, 
        const TReal& aScaleFactor,
        const TRect& aClipRect, 
        MBubbleMediaReaderObserver* aObserver ) = 0;

    virtual TInt SetScaleAndClip(
        const TSize& aTargetSize,
        const TReal& aScaleFactor,
        const TRect& aClipRect ) = 0;        
    
    virtual void CancelReading() = 0;
    
    virtual const TSize SourceSize() const = 0;
    
    virtual TBool IsAnimation() const = 0;
    
    virtual const CFbsBitmap* FrameBuffer() const;
    
    virtual const CFbsBitmap* Mask() const;
    
    virtual void Play() = 0;
    
    virtual void Stop() = 0;

public:
    CBubbleMediaReader();

protected: // data
    // Not owned
    const CFbsBitmap* iFrameBuffer;
    // Not owned
    const CFbsBitmap* iMask;
    // Not owned
    MBubbleMediaReaderObserver* iObserver;
    };


#endif // BMMEDIAREADER_H
