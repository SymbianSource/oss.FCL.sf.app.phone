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
* Description:  Extension control for displaying call image.
*
*/


#ifndef TELBUBBLECALLIMAGE_H
#define TELBUBBLECALLIMAGE_H

#include <coecntrl.h>

class CFbsBitmap;

/**
 *  Extension control for displaying call image.
 *
 *
 *  @lib BMCustomization.lib
 *  @since S60 v5.0
 */
class CTelBubbleCallImage : public CCoeControl
    {
public:
    /** Two phase constructor */
    IMPORT_C static CTelBubbleCallImage* NewL();

    /** destructor */
    IMPORT_C virtual ~CTelBubbleCallImage();

public:
    /**
     * Set call image. The bitmaps are set by Phone application,
     * when it has created bitmaps from the call image.
     * @param aImage Call image bitmap.
     * @param aImageOwnership ETrue, if ownership is transferred.
     * @param aMask  Mask bitmap when exists.
     * @param aMaskOwnership  ETrue, if ownership is transferred.  
     */
    IMPORT_C void SetImage( CFbsBitmap* aImage, 
                            TBool aImageOwnership = EFalse,
                            CFbsBitmap* aMask = NULL,
                            TBool aMaskOwnership = EFalse );
    
    /**
     * Set image placing area. This is set by Phone application.
     * @param aRect 
     */
    IMPORT_C void SetImagePlacingArea( TRect& aRect );
    
protected:    
    /**
     * Called when image is changed.
     */
    IMPORT_C virtual void ImageChanged();

    /**
     * Returns pointer to image bitmap.
     */
    IMPORT_C const CFbsBitmap* Image() const;

    /**
     * Returns pointer to mask bitmap.
     */
    IMPORT_C const CFbsBitmap* Mask() const;
    
    /**
     * Returns ETrue if image covers this control entirely.
     */
    IMPORT_C TBool IsFullScreenImage() const;
    
    /**
     * Returns offset centering image to this control.
     * For non fullscreen images this offset is relative
     * to image placing area.
     */
    IMPORT_C const TPoint& Offset() const;
    
    /**
     * Returns source rect centering image to this control.
     * For non-fullscreen images this offset is relative
     * to image placing area.
     */
    IMPORT_C const TRect& SourceRect() const;
    
    /**
     * Returns image placing area. Can be used to position
     * non-fullscreen sized images. 
     * @return Image placing area.
     */
    IMPORT_C const TRect& ImagePlacingArea() const;
    
protected: // from CCoeControl
    IMPORT_C virtual void Draw( const TRect& /*aRect*/ ) const;
    IMPORT_C virtual void SizeChanged();

protected:
    IMPORT_C virtual void ConstructL();
    IMPORT_C CTelBubbleCallImage();
    
private:
    TRect iSourceRect;
    TPoint iOffset;
    TRect iImagePlacingArea;
    CFbsBitmap *iImage;
    CFbsBitmap *iMask;
    TBool iImageOwnership;
    TBool iMaskOwnership;
    };

#endif // TELBUBBLECALLIMAGE_H

// end of file
