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



#ifndef BUBBLECALLOBJECTTUTILS_H
#define BUBBLECALLOBJECTTUTILS_H

//  INCLUDES
#include <gdi.h>
#include "BMBubbleManager.h"
#include <avkon.hrh>

// CONSTANTS

// DATA TYPES

// FORWARD DECLARATIONS
class CBubbleCallObjectImage;
class CBubbleImageReader;

// CLASS DECLARATION

/**
*  Utility methods for call object handling.
*
*  @lib BubbleManager
*  @since Series 60 3.1
*/
class BubbleCallObjectUtils
    {
    public:
        // Tiling mode
        enum TTileMode
            {
            ETileTopLeft,    // Tiling begin from top left corner
            ETileTopRight,   // Tiling begin from top right corner
            ETileCenterLeft, // Tiling begin from left, vertically centered
            ETileCenterRight // Tiling begin from right, vertically centered   
            };
            
        // Scale mode. Aspect ratio is always preserved.
        enum TScaleMode
            {
            EFillTarget,
            EMaximumFit    
            };
    
    public: // New functions
        ///////////////////////////////////////////////////////////////////////
        //
        // Construction and manipulation of CFbsBitmap objects.
        //
        ///////////////////////////////////////////////////////////////////////
        
        /**
        * Creates a new bitmap from the given bitmap by scaling it to the
        * given size. Proportions of the source bitmap are maintained so
        * cropping may be applied. If the given bitmap is very small it
        * is duplicated, not scaled, to fill the created bitmap.
        * @param aBitmap Source bitmap
        * @param aTargetSize Size of bitmap to be created.
        * @param aDisplayMode Display mode for bitmap created.
        * @param aTileMode Tiling mode.
        * @return New bitmap instance.
        */
        static CFbsBitmap* CreateBitmapL( 
            const CFbsBitmap* aBitmap, 
            const TSize& aTargetSize,
            TDisplayMode aDisplayMode,
            TTileMode aTileMode = ETileTopLeft );
            
        /**
        * Creates a new bitmap by cropping given bitmap to given size.
        * @param aBitmap Bitmap to be cropped.
        * @param aTargetSize Target size.
        * @param aDisplayMode Display mode for bitmap created.
        * @return New bitmap instance.
        */
        static CFbsBitmap* CreateCroppedBitmapL( 
            const CFbsBitmap* aBitmap, 
            const TSize& aTargetSize,
            TDisplayMode aDisplayMode );

        /**
        * Creates a new bitmap by tiling given bitmap to given size.
        * @param aBitmap Bitmap to be tiled.
        * @param aTargetSize Target size.
        * @param aDisplayMode Display mode for bitmap created.
        * @param aTileMode Tiling mode.
        * @return New bitmap instance.
        */            
        static CFbsBitmap* CreateTiledBitmapL( 
            const CFbsBitmap* aBitmap, 
            const TSize& aTargetSize,
            TDisplayMode aDisplayMode,
            TTileMode aTileMode );

        /**
        * Creates a new bitmap and renders given text given on it.
        * Text is rendered according to call2_cli_visual_text_pane
        * layout. Creates also mask bitmap. 
        * @param aText Text to be rendered.
        * @param aMainPaneRect Main pane rectangle.
        * @param aSize Size for created bitmap.
        * @param aTextColor Text color.
        * @param aAlpha Transparency of text: 0-255.
        * @param aDisplayMode Display mode for bitmap created.
        * @param aBitmap Created bitmap on return.
        * @param aMask Created mask on return.
        */            
        static void CreateImageBitmapsFromTextLC( 
            const TDesC& aText, 
            const TRect& aMainPaneRect,
            const TSize& aSize,
            const TRgb&  aTextColor,
            TUint aAlpha,
            TDisplayMode aDisplayMode,
            CFbsBitmap*& aBitmap,
            CFbsBitmap*& aMask );            
            
        /**
        * Calculates scaling factor for source size to fill target size. 
        * @param aSourceSize Source size.
        * @param aTargetSize Target size.
        * @param aScaleMode Scaling mode.
        * @return Scaling factor.
        */    
        static TReal ScalingFactor( 
            const TSize& aSourceSize,
            const TSize& aTargetSize,
            TScaleMode aScaleMode );
            
        /**
        * Calculates scale factor and clipping rectangle based on
        * source and target size. Clip rect is relative to source.
        * @param aSourceSize Source size.
        * @param aTargetSize Target size.
        * @param aScaleMode Scaling mode.
        * @param aScaleFactor Target size.
        * @param aClipRect Target size.
        * @return ETrue if source is tiny image i.e. not possible
        *         to scale it to fill target. 
        */    
        static TBool GetScaleFactorAndClipRect( 
            const TSize& aSourceSize,
            const TSize& aTargetSize,
            TScaleMode aScaleMode,
            TReal& aScaleFactor,
            TRect& aClipRect );
    };

#endif // BUBBLECALLOBJECTTUTILS_H   
            
// End of File
