/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Utility methods for call object handling.
*
*/



// INCLUDE FILES
#include    "BMCallObjectUtils.h"
#include    "BMLayout2.h"
#include    "BMUtils.h"
#include    "BMPanic.h"
#include    <AknUtils.h>
#include    <AknBidiTextUtils.h>
#include    <AknLayoutFont.h>

// CONSTANTS
const TReal KBMCallObjectScaleFactorMax = 1.0;
_LIT( KSpaceChar, " " );


// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// BubbleCallObjectUtils::CreateBitmapL
//
// ---------------------------------------------------------------------------
//    
CFbsBitmap* BubbleCallObjectUtils::CreateBitmapL( 
    const CFbsBitmap* aBitmap, 
    const TSize& aTargetSize,
    TDisplayMode aDisplayMode,
    TTileMode aTileMode )
    {
    __ASSERT_ALWAYS( aBitmap, User::Leave( KErrArgument ) );
    
    // get scaling factor
    TReal scalingFactor = ScalingFactor( aBitmap->SizeInPixels(),
                                         aTargetSize,
                                         EFillTarget );
                                                  
    if ( scalingFactor > 1.0 ) // bitmap scaling not supported
        {
        // image smaller than target area is tiled
        return CreateTiledBitmapL( aBitmap, 
                                   aTargetSize, 
                                   aDisplayMode, 
                                   aTileMode );
        }
    else        
        {
        // image bigger than target area is clipped
        return CreateCroppedBitmapL( aBitmap,
                                     aTargetSize,
                                     aDisplayMode );    
        }
    }

// ---------------------------------------------------------------------------
// BubbleCallObjectUtils::CreateCroppedBitmapL
//
// ---------------------------------------------------------------------------
//    
CFbsBitmap* BubbleCallObjectUtils::CreateCroppedBitmapL( 
    const CFbsBitmap* aBitmap, 
    const TSize& aTargetSize,
    TDisplayMode aDisplayMode )
    {
    __ASSERT_ALWAYS( aBitmap, User::Leave( KErrArgument ) );
    
    // create bitmap
    CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
    CleanupStack::PushL( bitmap );
    
    // calculate cropping rectangle   
    TSize sourceBitmapSize( aBitmap->SizeInPixels() );
    TRect croppingRect( TPoint(0,0), sourceBitmapSize );
    
    TInt xDelta( sourceBitmapSize.iWidth - aTargetSize.iWidth );
    if ( xDelta < 0 )
        {
        // do not crop horizontally
        xDelta = 0;    
        }
    
    TInt yDelta( sourceBitmapSize.iHeight - aTargetSize.iHeight );
    if ( yDelta < 0 )
        {
        // do not crop vertically
        yDelta = 0;    
        }
        
    if ( xDelta == 0 && yDelta == 0 )
        {
        // cropping not required -> just duplicate
        User::LeaveIfError( bitmap->Duplicate( aBitmap->Handle() ) );
        CleanupStack::Pop( bitmap );
        return bitmap;                            
        }
    else
        {
        // shrink to cropping area
        croppingRect.Shrink( xDelta/2, yDelta/2 );        
        }
    
    
    // create bitmap to target size
    User::LeaveIfError( bitmap->Create( aTargetSize, 
                                        aDisplayMode ) );
    // crop
    CFbsBitmapDevice* bitmapDev = 
        CFbsBitmapDevice::NewL( bitmap );
    CleanupStack::PushL( bitmapDev );
    CFbsBitGc* bitmapCtx;
    User::LeaveIfError( bitmapDev->CreateContext( bitmapCtx ) );
    CleanupStack::PushL( bitmapCtx );                                               
    TRect targetRect( TPoint(0,0), aTargetSize );
    bitmapCtx->DrawBitmap( targetRect, aBitmap, croppingRect );    
    
    CleanupStack::PopAndDestroy( 2, bitmapDev );
    CleanupStack::Pop( bitmap );
    
    return bitmap;            
    }

// ---------------------------------------------------------------------------
// BubbleCallObjectUtils::CreateTiledBitmapL
//
// ---------------------------------------------------------------------------
//    
CFbsBitmap* BubbleCallObjectUtils::CreateTiledBitmapL( 
    const CFbsBitmap* aBitmap, 
    const TSize& aTargetSize,
    TDisplayMode aDisplayMode,
    TTileMode aTileMode )
    {
    __ASSERT_ALWAYS( aBitmap, User::Leave( KErrArgument ) );
    
    TSize sourceSize = aBitmap->SizeInPixels();
    TPoint offset(0,0);
    
    // calculate offset for brush origin
    
    // horizontal offset
    if ( aTargetSize.iWidth <= sourceSize.iWidth )
        {
        // wide image -> center horizontally
        offset.iX = ( sourceSize.iWidth - aTargetSize.iWidth ) / 2;    
        }
    else if ( ( aTileMode == ETileCenterRight ) ||
              ( aTileMode == ETileTopRight ) )
        {
        // begin tiling from right
        offset.iX = sourceSize.iWidth - ( aTargetSize.iWidth % 
                                          sourceSize.iWidth );    
        }
    
    // vertical offset
    if ( aTargetSize.iHeight <= sourceSize.iHeight )
        {
        // tall image -> center vertically 
        offset.iY = ( sourceSize.iHeight - aTargetSize.iHeight ) / 2;    
        }
    else if ( ( aTileMode == ETileCenterRight ) ||
              ( aTileMode == ETileCenterLeft ) )
        {
        // middle most tile is centered vertically
        TInt topMargin = ( aTargetSize.iHeight - sourceSize.iHeight ) / 2;
        if ( topMargin <= sourceSize.iHeight )
            {
            offset.iY = ( sourceSize.iHeight - topMargin );    
            }
        else
            {
            offset.iY = sourceSize.iHeight - ( topMargin % 
                                               sourceSize.iHeight );    
            }            
        }

    // create bitmap to target size
    CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
    CleanupStack::PushL( bitmap );
    User::LeaveIfError( bitmap->Create( aTargetSize, 
                                        aDisplayMode ) );            
    
    // create device and graphics context
    CFbsBitmapDevice* bitmapDev = CFbsBitmapDevice::NewL( bitmap );
    CleanupStack::PushL( bitmapDev );
    CFbsBitGc* bitmapCtx;
    User::LeaveIfError( bitmapDev->CreateContext( bitmapCtx ) );
    CleanupStack::PushL( bitmapCtx );         
    
    // do tiling by using patterned brush
    CFbsBitmap* pattern = new(ELeave) CFbsBitmap;
    CleanupStack::PushL( pattern );
    User::LeaveIfError( pattern->Duplicate( aBitmap->Handle() ) );
    
    TRect bitmapRect( TPoint(0,0), aTargetSize );
    bitmapCtx->SetPenStyle( CGraphicsContext::ENullPen ); 
    bitmapCtx->SetBrushStyle( CGraphicsContext::EPatternedBrush ); 
    bitmapCtx->UseBrushPattern( pattern );
    bitmapCtx->SetBrushOrigin( -offset );
    bitmapCtx->DrawRect( bitmapRect );
    bitmapCtx->DiscardBrushPattern();
    
    CleanupStack::PopAndDestroy( 3, bitmapDev );
    CleanupStack::Pop( bitmap ); 
    
    return bitmap;       
    }    

// ---------------------------------------------------------------------------
// BubbleCallObjectUtils::CreateImageBitmapsFromTextLC
//
// ---------------------------------------------------------------------------
//    
void BubbleCallObjectUtils::CreateImageBitmapsFromTextLC(
    const TDesC& aText,
    const TRect& aMainPaneRect, 
    const TSize& aSize,
    const TRgb&  aTextColor,
    TUint aAlpha,
    TDisplayMode aDisplayMode,
    CFbsBitmap*& aBitmap,
    CFbsBitmap*& aMask )
    {
    // create bitmap
    aBitmap= new(ELeave) CFbsBitmap;
    CleanupStack::PushL( aBitmap );
    User::LeaveIfError( aBitmap->Create( aSize, aDisplayMode ) );
    
    // create mask
    aMask = new(ELeave) CFbsBitmap;
    CleanupStack::PushL( aMask );
    User::LeaveIfError( aMask->Create( aSize, EGray256 ) );

    // create bitmap context
    CFbsBitmapDevice* bitmapDev = CFbsBitmapDevice::NewL( aBitmap );
    CleanupStack::PushL( bitmapDev );
    CFbsBitGc* bitmapGc;
    User::LeaveIfError( bitmapDev->CreateContext( bitmapGc ) ); 
    CleanupStack::PushL( bitmapGc );    
    
    // create mask context
    CFbsBitmapDevice* maskDev = CFbsBitmapDevice::NewL( aMask );
    CleanupStack::PushL( maskDev );
    CFbsBitGc* maskGc;
    User::LeaveIfError( maskDev->CreateContext( maskGc ) ); 
    CleanupStack::PushL( maskGc );
    
    // initialize bitmap
    bitmapGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    bitmapGc->SetPenStyle( CGraphicsContext::ENullPen );
    bitmapGc->SetBrushColor( KRgbWhite );
    bitmapGc->DrawRect( TRect( aSize ) );
    bitmapGc->SetPenStyle( CGraphicsContext::ESolidPen );
    bitmapGc->SetBrushStyle( CGraphicsContext::ENullBrush );
    // initialize mask
    maskGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    maskGc->SetPenStyle( CGraphicsContext::ENullPen );
    maskGc->SetBrushColor( KRgbBlack );
    maskGc->DrawRect( TRect( aSize ) );
    maskGc->SetPenStyle( CGraphicsContext::ESolidPen );
    maskGc->SetBrushStyle( CGraphicsContext::ENullBrush );
    
    // reserve text buffers
    TAknLayoutRect textPaneLayout;
    textPaneLayout.LayoutRect( aMainPaneRect,
                             BubbleLayout2::call2_cli_visual_text_pane() );
    TRect textPaneRect = textPaneLayout.Rect();                             
    
    TAknLayoutText textLayout;
    textLayout.LayoutText( textPaneRect, 
                           BubbleLayout2::call2_main_pane_text( 0 ) );
    const CFont* font = textLayout.Font();
    TInt lineWidth = textLayout.TextRect().Size().iWidth;
    TInt averageFontWidth( font->WidthZeroInPixels() );
    
    TInt maxCharsPerLine( lineWidth / averageFontWidth );
    maxCharsPerLine = maxCharsPerLine * 4; // to secure
    HBufC* logicalLineBuf = HBufC::NewL( maxCharsPerLine );
    CleanupStack::PushL( logicalLineBuf );
    HBufC* visualLineBuf = HBufC::NewL( maxCharsPerLine + 
                                        KAknBidiExtraSpacePerLine );
    CleanupStack::PushL( visualLineBuf );
    TPtr logicalLine( logicalLineBuf->Des() );
    TPtr visualLine( visualLineBuf->Des() );
    
    TRgb alphaColor( aAlpha, aAlpha, aAlpha );
    TInt currentRow = 0;
    TInt i = 0;
    TInt textLength = aText.Length();    
    const TInt numberOfRows = 
        BubbleLayout2::call2_cli_visual_text_number_of_rows();
    
    // shift gc origin
    TPoint offset( (aMainPaneRect.Size().iWidth - aSize.iWidth) / 2,
                   (aMainPaneRect.Size().iHeight - aSize.iHeight) / 2 );
    bitmapGc->SetOrigin( -offset );
    maskGc->SetOrigin( -offset );
    
    // render text
    while ( currentRow <= numberOfRows )
        {
        TAknLayoutText textLayout;
        textLayout.LayoutText( textPaneRect,
            BubbleLayout2::call2_main_pane_text( currentRow ) );
        
        TRect textRect = textLayout.TextRect();            
        if ( ( textRect.iBr.iY < offset.iY ) || 
             ( textRect.iTl.iY > ( offset.iY + aSize.iHeight ) ) )
            {
            // this line is outside the bitmap.
            currentRow++;
            continue;                
            }
        
        logicalLine.Zero();
        TBool clipped( EFalse );
        do // Find the clipping point
            {
            TChar c( aText[i] );
            if( logicalLine.Length() == maxCharsPerLine )
                {
                User::Leave( KErrOverflow );
                }
            logicalLine.Append( c );
            i++;
            if ( i == textLength )
                {
                if( logicalLine.Length() == maxCharsPerLine )
                    {
                    User::Leave( KErrOverflow );
                    }
                // start over to fill whole pane
                i = 0;
                logicalLine.Append( KSpaceChar );
                }
            
            visualLine.Zero();
            clipped = AknBidiTextUtils::ConvertToVisualAndClip(
                logicalLine,
                visualLine,
                *font,
                lineWidth,
                lineWidth,
                AknBidiTextUtils::EImplicit,
                0xFFFF );
                
            } while ( !clipped );
        
        // Remove the last character that caused clipping
        if ( logicalLine.Length() )
            {
            logicalLine.SetLength( logicalLine.Length() - 1 );                
            }
        
        AknBidiTextUtils::ConvertToVisualAndClip(
            logicalLine,
            visualLine,
            *font,
            lineWidth,
            lineWidth,
            AknBidiTextUtils::EImplicit,
            0xFFFF );
        
        textLayout.DrawText( *bitmapGc, visualLine, EFalse, aTextColor );
        textLayout.DrawText( *maskGc, visualLine, EFalse, alphaColor );
        
        // clipped -> go back by one        
        i = i > 0 ? (i - 1) : (textLength-1);                    
        
        currentRow++;
        }

    CleanupStack::PopAndDestroy(6, bitmapDev);
    }

// ---------------------------------------------------------------------------
// BubbleCallObjectUtils::ScalingFactor
//
// ---------------------------------------------------------------------------
//
TReal BubbleCallObjectUtils::ScalingFactor( 
    const TSize& aSourceSize,
    const TSize& aTargetSize,
    TScaleMode aScaleMode )
    {
    if ( aTargetSize == TSize(0,0) || aSourceSize == TSize(0,0) )
        {
        return 0;    
        }
    
    // check aspect ratios
    TReal targetAspectRatio = (TReal) aTargetSize.iWidth / 
                              (TReal) aTargetSize.iHeight;
    TReal sourceAspectRatio = (TReal) aSourceSize.iWidth  / 
                              (TReal) aSourceSize.iHeight;
    TReal scaleFactor = 1.0; 
    
    
    if ( sourceAspectRatio == targetAspectRatio )
        {
        // aspect ratios are same.
        return (TReal) aTargetSize.iWidth / (TReal) aSourceSize.iWidth;
        }
    else if ( targetAspectRatio > sourceAspectRatio )
        {
        // target wide more screen than source.
        if ( aScaleMode == EFillTarget )
            {
            // width defines the scale
            scaleFactor = (TReal) aTargetSize.iWidth / 
                          (TReal) aSourceSize.iWidth;    
            }
        else // EMaximumFit
            {
            // height defines the scale
            scaleFactor = (TReal) aTargetSize.iHeight / 
                          (TReal) aSourceSize.iHeight;    
            }            
        }
    else
        {
        // video is more wide screen than window.
        if ( aScaleMode == EFillTarget )
            {
            // height defines the scale
            scaleFactor = (TReal) aTargetSize.iHeight / 
                          (TReal) aSourceSize.iHeight;    
            }
        else // EMaximumFit
            {
            // width defines the scale
            scaleFactor = (TReal) aTargetSize.iWidth / 
                          (TReal) aSourceSize.iWidth;    
            }            
        }
    
    return scaleFactor;        
    }

// ---------------------------------------------------------------------------
// BubbleCallObjectUtils::GetScaleFactorAndClipRect
//
// ---------------------------------------------------------------------------
//    
TBool BubbleCallObjectUtils::GetScaleFactorAndClipRect( 
    const TSize& aSourceSize,
    const TSize& aTargetSize,
    TScaleMode aScaleMode,
    TReal& aScaleFactor,
    TRect& aClipRect )
    {
    TBool isTinyImage( EFalse );
    
    // scaling to fit main pane
    TReal scaleFactor = BubbleCallObjectUtils::ScalingFactor(
        aSourceSize,
        aTargetSize,
        aScaleMode );
    
    // set clipping rect
    TRect clipRect;
    
    if ( aScaleMode == EMaximumFit )
        {
        aScaleFactor = scaleFactor > KBMCallObjectScaleFactorMax ?
                       KBMCallObjectScaleFactorMax : scaleFactor; 
        aClipRect = TRect( aSourceSize ); // no clip   
        }
    else if ( scaleFactor == 0 )
        {
        aScaleFactor = scaleFactor;
        aClipRect = TRect( aSourceSize );    
        }
    else if ( scaleFactor > KBMCallObjectScaleFactorMax )
        {
        isTinyImage = ETrue;
        aScaleFactor = KBMCallObjectScaleFactorMax;
        TInt x_offset = 0;
        if ( aSourceSize.iWidth > aTargetSize.iWidth )
            {
            // center horizontally
            x_offset =
                ( aSourceSize.iWidth - aTargetSize.iWidth ) / 4;
            }
        TInt y_offset = 0;
        if ( aSourceSize.iHeight > aTargetSize.iHeight )
            {
            // center vertically
            y_offset =
                ( aSourceSize.iHeight - aTargetSize.iHeight ) / 4;    
            }
        
        aClipRect = TRect( aSourceSize );
        aClipRect.Shrink( x_offset, y_offset );    
        }
    else // 0 < scaleFactor < KBMCallObjectScaleFactorMax
        {
        aScaleFactor = scaleFactor;
        TSize relativeTargetSize;
        relativeTargetSize.iWidth = aTargetSize.iWidth / scaleFactor;
        relativeTargetSize.iHeight = aTargetSize.iHeight / scaleFactor;
        TInt x_offset = ( aSourceSize.iWidth - 
                          relativeTargetSize.iWidth ) / 2;
        TInt y_offset = ( aSourceSize.iHeight - 
                          relativeTargetSize.iHeight ) / 2;
        aClipRect = TRect( relativeTargetSize );
        if ( x_offset >= 0 && y_offset >= 0 )
            {
            // clip from center of the source image
            aClipRect.Move( x_offset, y_offset );    
            }
        }
    
    return isTinyImage;            
    }
    
//  End of File  
