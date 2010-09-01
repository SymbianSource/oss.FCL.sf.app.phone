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
* Description:  Reader for JPEG/GIF/BMP images (ImageHandlingLib wrapper)
*
*/


#ifndef BMIMAGEREADER_H
#define BMIMAGEREADER_H

#include <MIHLViewerObserver.h>

#include "BMMediaReader.h"

class MIHLFileImage;
class MIHLBitmap;
class MIHLImageViewer;

/**
 *  Reader for JPEG/GIF/BMP images (ImageHandlingLib wrapper)
 *
 *  @lib BubbleManager
 *  @since S60 v3.2
 */
class CBubbleImageReader : public CBubbleMediaReader,
                           public MIHLViewerObserver
    {
public:    

    static CBubbleImageReader* NewL( 
        const TDesC& aFileName );
	
    virtual ~CBubbleImageReader();

private: // from CBubbleMediaReader
    
    void  StartReadingL(
        const TSize& aTargetSize,
        const TReal& aScaleFactor,
        const TRect& aClipRect, 
        MBubbleMediaReaderObserver* aObserver );
        
    TInt SetScaleAndClip(
        const TSize& aTargetSize,
        const TReal& aScaleFactor,
        const TRect& aClipRect );        
    
    void  CancelReading();
    
    const TSize SourceSize() const;
    
    TBool IsAnimation() const;
    
    void  Play();
    
    void  Stop();
    
private: // from MIHLViewerObserver
    
    void ViewerBitmapChangedL();
    
    void ViewerError( TInt aError );     
    

private:
    
    CBubbleImageReader();

    void ConstructL( const TDesC& aFileName );

private: // data
    
    // Owned
    MIHLFileImage* iSourceImage;
    // Owned
    MIHLBitmap* iDestinationBitmap;
    // Owned
    MIHLImageViewer* iViewerEngine;
    
    };

#endif // BMIMAGEREADER_H
