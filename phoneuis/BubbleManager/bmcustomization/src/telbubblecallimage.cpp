/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call image control.
*
*/


#include <telbubblecallimage.h>
#include <gdi.h>

// ======== LOCAL FUNCTIONS ========

/**
 * Draws rounded rect. Used for drawing frame to caller image bitmap.
 */
void DrawRoundRect(CWindowGc& aGC, TPoint aTopLeft, TSize aSize )
    {   
    const TRgb white( 255, 255, 255);
    aGC.SetPenColor( white );
    aGC.SetPenSize( TSize( 5, 5 ) );
    
    TPoint bottomRight = aTopLeft + aSize;
    TRect decoratorRect( aTopLeft, bottomRight );
    // Make rect litle larger so that it will be outside of the caller image, 
    // so that bitmap's corners are hidden.
    decoratorRect.Grow( 2, 2 ); 
    const TSize ellipsesSize( 10, 10 );
    aGC.DrawRoundRect( decoratorRect, ellipsesSize );
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleCallImage::CTelBubbleCallImage()
    {
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleCallImage* CTelBubbleCallImage::NewL()
    {
    CTelBubbleCallImage* self = new( ELeave )CTelBubbleCallImage();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleCallImage::~CTelBubbleCallImage()
    {
    if ( iImageOwnership )
        {
        delete iImage;
        }
    if ( iMaskOwnership )
        {
        delete iMask;
        }
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::ConstructL
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleCallImage::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::SetImage
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleCallImage::SetImage( 
    CFbsBitmap* aImage, 
    TBool aImageOwnership,
    CFbsBitmap* aMask,
    TBool aMaskOwnership )
    {
    if ( iImageOwnership )
        {
        delete iImage;
        }
    if ( iMaskOwnership )
        {
        delete iMask;
        }
    
    iImage = aImage;
    iMask = aMask;
    iImageOwnership = aImageOwnership;
    iMaskOwnership = aMaskOwnership;
    
    SizeChanged();
    ImageChanged();
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::ReleaseImage
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleCallImage::ImageChanged()
    {
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::ReleaseImage
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleCallImage::SetImagePlacingArea( TRect& aRect )
    {
    iImagePlacingArea = aRect;
    }
  
// ---------------------------------------------------------------------------
// CTelBubbleCallImage::ReleaseImage
// ---------------------------------------------------------------------------
//
EXPORT_C const CFbsBitmap* CTelBubbleCallImage::Image() const
    {
    return iImage;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::ReleaseImage
// ---------------------------------------------------------------------------
//
EXPORT_C const CFbsBitmap* CTelBubbleCallImage::Mask() const
    {
    return iMask;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::IsFullScreenImage
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CTelBubbleCallImage::IsFullScreenImage() const
    {
    TBool isFullScreen = ETrue;
    
    if ( iImage )
        {
        TSize imageSize = iImage->SizeInPixels();
        TSize thisSize( Rect().Size() );
        isFullScreen = ( imageSize.iWidth >= thisSize.iWidth ) ||
                       ( imageSize.iHeight >= thisSize.iHeight );
        }
        
    return isFullScreen;  
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::Offset
// ---------------------------------------------------------------------------
//
EXPORT_C const TPoint& CTelBubbleCallImage::Offset() const
    {
    return iOffset;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::SourceRect
// ---------------------------------------------------------------------------
//
EXPORT_C const TRect& CTelBubbleCallImage::SourceRect() const
    {
    return iSourceRect;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::ImagePlacingArea
// ---------------------------------------------------------------------------
//
EXPORT_C const TRect& CTelBubbleCallImage::ImagePlacingArea() const
    {
    return iImagePlacingArea;
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::SizeChanged
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleCallImage::SizeChanged()
    {
    if ( !iImage )
        {
        return;                    
        }
        
    // If bitmap is bigger than drawing area then clip from center of the image.
    TRect rect = Rect();
    iSourceRect = iImage->SizeInPixels();

    TInt offsetX = ( iSourceRect.Width() > rect.Width() ) ? 
                   ((iSourceRect.Width() - rect.Width()) / 2) : 0;
    TInt offsetY = ( iSourceRect.Height() > rect.Height() ) ? 
                   ((iSourceRect.Height() - rect.Height()) / 2) : 0;
    
    iSourceRect.Shrink( offsetX, offsetY ); 
    
    // If bitmap is smaller than the drawing area then center the image on it.
    iOffset.iX = ( iSourceRect.Width() < rect.Width() ) ? 
                ((rect.Width() - iSourceRect.Width()) / 2) : 0;
    
    iOffset.iY = 0;
    
    // lift the image higher if needed.
    if ( iSourceRect.Height() < rect.Height() )
        {
        // check if it fits totally above the bubble
        TInt height = iImagePlacingArea.iBr.iY - iImagePlacingArea.iTl.iY;
      
        if ( iSourceRect.Height() < height )
            {
            // if it fits then center it above the bubble
            iOffset.iY = ( height - iSourceRect.Height() ) / 2;
            }
        }
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::Draw
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleCallImage::Draw( const TRect& /*aRect*/ ) const
    {
    if ( iImage )
        {
        CWindowGc& gc = SystemGc();
    
        const TPoint topLeft = Rect().iTl;
        const TPoint bitmapTopLeft = topLeft + iOffset;
    
        // if we have y offset then the image wont go under the bubble in any case and so
        // no need for masking
        if ( iMask && iOffset.iY == 0 ) 
            {
            gc.BitBltMasked( bitmapTopLeft,
                         iImage, 
                         iSourceRect, 
                         iMask, 
                         EFalse );
            }
        else
            {
            gc.BitBlt( bitmapTopLeft,
                   iImage,
                   iSourceRect );    
            }
    
        DrawRoundRect( gc, bitmapTopLeft, iSourceRect.Size() );
        }
    }

// end of file
