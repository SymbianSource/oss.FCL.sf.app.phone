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
* Description: 
*     Some utility functions to other classes
*
*/


#ifndef BUBBLEUTILS_H
#define BUBBLEUTILS_H

// INCLUDES
#include "BMBubbleManager.h"
#include <calslbs.h>

// FORWARD DECLARATIONS
class CBubbleHeader;
class CEikImage;
class CEikLabel;
class CBubbleImageManager;
class TAknLayoutRect;
class CBubbleCallObjectImage;
class CTelBubbleCustomElement;

// CLASS DECLARATION

class TBubbleLayoutRect
    {    
public:
    TBubbleLayoutRect();
    void SetRect( const TPoint& aPoint, const TSize& aSize );    
    void SetColor( const TRgb aColor );
    TRgb Color() const;
    TRect Rect() const;
private:
    TRgb iColor;
    TRect iRect;
    };


/**
*  CBubbleUtils class
*  Some utility functions to other classes
*
*  @lib bubblemanager.lib
*  @since 1.0
*/
class BubbleUtils
    {
    public: // Data types

        // Text place enumeration 
        enum KBubbleUtilsHeaderTextPlace
            {
            EBubbleHeaderCLI,
            EBubbleHeaderLabel,
            EBubbleHeaderTimerCost,
            EBubbleHeaderCNAP
            };

    public: // Member functions
        
        /**
        * Sets call type indicators in type pane
        * @param aHeader Header which contains the information
        * @param aTypeImage1 The selected (left one) bitmap is attached to 
        *                    this image
        * @param aTypeImage2 The selected (right one) bitmap is attached to 
        *                    this image
        * @param aBubbleManager Reference to BubbleManager
        * @return Is at least one image set?
        */   
        static TBool SetCallTypePane( const CBubbleHeader& aHeader, 
                                      CEikImage*& aTypeImage1, 
                                      CEikImage*& aTypeImage2, 
                                      CBubbleManager& aBubbleManager );

        /**
        * Sets ciphering off icon to image
        * @param aHeader Header which contains the information
        * @param aCypfOffImage The selected bitmap is attached to this image
        * @param aBubbleManager Reference to BubbleManager
        * @return Is image set?
        */
        static TBool SetCyphOffImage( const CBubbleHeader& aHeader, 
                                      CEikImage*& aCypfOffImage, 
                                      CBubbleManager& aBubbleManager );

        
        /**
        * Selects text to lines ( for bottom, bottom right and middle bubbles)
        * @param aHeader Header which contains the information
        * @param aFirstLineText Text which goes to line 1 in the bubble
        * @param aSecondLineText Text which goes to line 2 in the bubble
        * @param aTimerCost Text which goes to timer/cost in the bubble
        * @param aTextLine1 Component for resource allocation.
        * @param aTextLine2 Component for resource allocation.
        * @param aBubbleManager Bubblemanager reference.
        * @param aFirstLineDir First line text clipping direction
        * @param aSecondLineDir Second line text clipping direction
        * @param aTextLineNumber On return contains the text-part line number 
        */
        static void ChooseTextsToTwoLines( const CBubbleHeader& aHeader,
            TPtrC& aFirstLineText,
            TPtrC& aSecondLineText,
            CEikLabel*& aTimerCost,
            CEikLabel*& aTextLine1,
            CEikLabel*& aTextLine2,
            CBubbleManager& aBubbleManager,
            CBubbleManager::TPhoneClippingDirection& aFirstLineDir,
            CBubbleManager::TPhoneClippingDirection& aSecondLineDir,
            TUint8& aTextLineNumber );
        
        /**
        * Selects text to lines ( for bottom, bottom right and middle bubbles)
        * @param aHeader Header which contains the information
        * @param aFirstLineText Text which goes to line 1 in the bubble
        * @param aSecondLineText Text which goes to line 2 in the bubble
        * @param aThirdLineText Text which goes to line 2 in the bubble
        * @param aTimerCost Text which goes to timer/cost in the bubble
        * @param aTextLine1 Component for resource allocation.
        * @param aTextLine2 Component for resource allocation.
        * @param aTextLine3 Component for resource allocation.
        * @param aBubbleManager Bubblemanager reference.
        * @param aFirstLineDir First line text clipping direction
        * @param aSecondLineDir Second line text clipping direction
        * @param aThirdLineDir Third line text clipping direction
        * @param aTextLineNumber On return contains the text-part line number 
        * @since Series60 2.6
        */
        static void ChooseTextsToThreeLines( const CBubbleHeader& aHeader,
            TPtrC& aFirstLineText,
            TPtrC& aSecondLineText,
            TPtrC& aThirdLineText,
            CEikLabel*& aTimerCost,
            CEikLabel*& aTextLine1,
            CEikLabel*& aTextLine2,
            CEikLabel*& aTextLine3,
            CBubbleManager& aBubbleManager,
            CBubbleManager::TPhoneClippingDirection& aFirstLineDir,
            CBubbleManager::TPhoneClippingDirection& aSecondLineDir,
            CBubbleManager::TPhoneClippingDirection& aThirdLineDir,
            TUint8& aTextLineNumber );
            
        /**
        * Selects text to lines
        * @param aHeader Header which contains the information
        * @param aFirstLineText Text which goes to line 1 in the bubble
        * @param aSecondLineText Text which goes to line 2 in the bubble
        * @param aThirdLineText Text which goes to line 2 in the bubble
        * @param aTimerCost Text which goes to timer/cost in the bubble
        * @param aTextLine1 Component for resource allocation.
        * @param aTextLine2 Component for resource allocation.
        * @param aTextLine3 Component for resource allocation.
        * @param aTextLine4 Component for resource allocation.
        * @param aTextLine5 Component for resource allocation.
        * @param aBubbleManager Bubblemanager reference.
        * @param aFirstLineDir First line text clipping direction
        * @param aSecondLineDir Second line text clipping direction
        * @param aThirdLineDir Third line text clipping direction
        * @param aTextLineNumber On return contains the text-part line number 
        * @param aThreeLinesOfText Three lines of call text.
        * @since Series60 2.6
        */
        static void ChooseTextsToFiveLines( const CBubbleHeader& aHeader,
            TPtrC& aFirstLineText,
            TPtrC& aSecondLineText,
            TPtrC& aThirdLineText,
            CEikLabel*& aTextLine1,
            CEikLabel*& aTextLine2,
            CEikLabel*& aTextLine3,
            CEikLabel*& aTextLine4,
            CEikLabel*& aTextLine5,
            CBubbleManager& aBubbleManager,
            CBubbleManager::TPhoneClippingDirection& aFirstLineDir,
            CBubbleManager::TPhoneClippingDirection& aSecondLineDir,
            CBubbleManager::TPhoneClippingDirection& aThirdLineDir,
            TUint8& aTextLineNumber,
            TBool aThreeLinesOfText );            

        /**
        * Selects from longer text (format: "longone\tshort") which one to use
        * @param aText String containing at least long version of the text
        * @param aLabel Label which width has been set and the text will be 
        *               clipped to.
        * @param aClipDir Text's clipping direction
        */
        static void SetTextInLabel( 
                     const TDesC16& aText, 
                     CEikLabel*& aLabel ,
                     const CBubbleManager::TPhoneClippingDirection& aClipDir );
        
        /**
        * Sets given text to label - clips if needed.
        * @param aText String containing the used text
        * @param aLabel Label which width has been set and the text will be 
        *               clipped to.
        * @param aClipDir Text's clipping end
        */    
        static void ClipToLabel ( 
                    const TDesC16& aText, 
                    CEikLabel*& aLabel ,
                    const CBubbleManager::TPhoneClippingDirection& aClipDir );
        

        /**
        * Draws image to screen. Pointer check added to previos function.
        * @param aGc Context to draw to
        * @param aImage Drawable image - with mask!
        * @return EFalse if image was not drawn.
        */
        static TBool DrawMaskedImage( CBitmapContext& aGc , CEikImage* aImage );
        
        /**
        * Tries to set text to label.
        * @param aLabel Label to set the text to
        * @param aText The text.
        */
        static void AddTextToEikLabel( 
            CEikLabel*& aLabel , 
            const TDesC& aText);
        
        /**
        * Adds text to call header. Cuts the text if not fit.
        * @param aHeader The header
        * @param aTextPlace Enum for text position
        * @param aTextToSet Original text.
        * @param aMaxLength Text max length
        * @param aClipDirection If text is too long, then cnut it.
        */
        static void AddTextToHeader( CBubbleHeader& aHeader,
            const KBubbleUtilsHeaderTextPlace aTextPlace,
            const TDesC& aTextToSet,
            const TInt aMaxLength,
            const CBubbleManager::TPhoneClippingDirection aClipDirection );

        
        /**
        * The next three calls AknLayoutUtils's functions after they have 
        * checked the pointer.
        */ 
        static void LayoutControl( 
            CCoeControl* aControl, 
            const TRect& aControlParent, 
            const TAknWindowLineLayout& aLayout );
        
        static void LayoutBackgroundImage( 
            CEikImage* aImage, 
            const TRect& aParent, 
            const TAknWindowLineLayout& aLayout);

        static void LayoutImage( 
            CEikImage* aImage, 
            const TRect& aParent, 
            const TAknWindowLineLayout& aLayout);

        static void LayoutLabel( 
            CEikLabel* aLabel, 
            const TRect& aLabelParent, 
            const TAknTextLineLayout& aLayout );
        
        static void LayoutImage( 
            CEikImage* aImage, 
            const TRect& aParent, 
            const AknLayoutUtils::SAknLayoutControl& aLayout );
        
        static void LayoutCustomElement(
            CTelBubbleCustomElement* aElement, 
            const TRect& aParent, 
            const TAknWindowLineLayout& aLayout); 
        
        /**
        * Converts western number string to Arabic-Indic if needed.
        * @since 2.0
        * @param aResult Result buffer. aResult.MaxLength() must be greater 
                                        or equal to aSource.Length().
        * @param aSource Original string.
        */
        static void ConvertToArabicIndic( 
            TDes& aResult , 
            const TDesC& aSource );

        /**
        * Converts western number string to Arabic-Indic if needed.
        * @since 2.0
        * @param aDes Source and destination
        */
        static void ConvertToArabicIndic( TDes& aDes );

        /**
        * Converts western number string to Arabic-Indic if needed.
        * @since 2.0
        * @param aResult Original string.
        * @return Result buffer. NULL in OOM case. Ownership transferred.
        */
        static HBufC* ConvertToArabicIndic( const TDesC& aDes );


        /**
        * Set call type pane
        * @since 2.0
        * @param aBubblePlace Bubble place from CBubblePlace.
        * @param aParent Parent control
        * @param aTypeIndication1 Left indicator.
        * @param aTypeIndication2 Rigth indicator.
        */
        static void SetLayoutForCellTypePane( 
            const CBubblePlace::TPhoneBubblePlace& aBubblePlace,
            const TRect& aParent,
            CEikImage* aTypeIndication1,
            CEikImage* aTypeIndication2 );

        /**
        * Set cyphering off icon place
        * @since 2.0
        * @param aHeader Call information.
        * @param aBubblePlace Bubble place from CBubblePlace.
        * @param aParent Parent control
        * @param aCyphOffImage The image.
        */
        static void SetLayoutFofCypheringOffIcon(
            const CBubbleHeader& aHeader,
            const CBubblePlace::TPhoneBubblePlace& aBubblePlace,
            const TRect& aParent,
            CEikImage* aCyphOffImage );

        /**
        * Places thumbnail image
        * @param aBubblePlace Bubble place from CBubblePlace
        * @param aImage The thumbnail image
        * @param aClippingRect Clipping rectangle.
        * @param aShadow Thumbnail shadow
        * @param aParentRect Parent control.
        */
        static void PlaceThumbnail(
            const CBubblePlace::TPhoneBubblePlace& aBubblePlace,
            CEikImage* aImage , 
            TBubbleLayoutRect& aShadow,
            const TRect& aParentRect,
            CFbsBitmap* aThumbNailBitmap );


        /**
        * 
        * @param aParent Parent control.
        * @param aWidth Width of the thumbnail image.
        * @return ETrue if customized image, else EFalse.
        */
        static TBool IsCustomizedImage( const TRect& aParent, const TInt aWidth );

        /**
        * 
        * @param aParent Parent control.
        * @param aWidth Width of the thumbnail image.
        * @return TInt Internal type of the image used.
        */
        static TInt GetThumbnailImageType( const TRect& aParent, const TInt aWidth );
        /**
        * @param aLabel Label control to be resized.
        * @param aParent Parent control.
        * @param aWidth Width of the thumbnail image.
        */
        static void LabelExtent( CEikLabel* aLabel, 
                                 const TRect& aParent, 
                                 const TInt aWidth );
        
        /**
        * @param aFileName Icon file name.
        */
        static void BubbleIconFileName( TDes& aFileName );

        /**
        * @param aFileName Resource file name.
        */
        static void BubbleResourceFileName( TDes& aFileName );

        /**
        * Selects from longer text (format: "longone\tshort") which one to use
        * @param aText String containing at least long version of the text
        * @param aLabel Label which width has been set and the text will be 
        *               clipped to.
        * @param aClipDir Text's clipping direction
        */
        static void SetTextInLabel( 
                     const TDesC16& aText, 
                     CEikLabel& aLabel ,
                     const CBubbleManager::TPhoneClippingDirection& aClipDir );
                     
        /**
        * Sets given text to label - clips if needed.
        * @param aText String containing the used text
        * @param aLabel Label which width has been set and the text will be 
        *               clipped to.
        * @param aClipDir Text's clipping end
        */
        static void ClipToLabel ( 
                    const TDesC16& aText, 
                    CEikLabel& aLabel ,
                    const CBubbleManager::TPhoneClippingDirection& aClipDir );

        /**
        * Tries to set text to label.
        * @param aLabel Label to set the text to
        * @param aText The text.
        */                    
        static void AddTextToEikLabel( CEikLabel& aLabel , const TDesC& aText);
        
        /**
        * Creates bubble image from 9-pieceframe.
        * @since 3.1
        * @param aFrameID Skin id for frame.
        * @param aOuterRect Outer rectangle.
        * @param aInnerRect Inner rectangle.
        * @param aBubble Bubble image.
        */
        static void PrepareBubbleImageL(
            const TAknsItemID& aFrameID,
            const TRect& aOuterRect,
            const TRect& aInnerRect,    
            CEikImage*& aBubble );
            
        /**
        * Adds call object image to bubble image.
        * @since 3.1
        * @param aCOImage Image.
        * @param aCOImageMask Image mask.
        * @param aCallObjectRect Call object location in bubble.
        * @param aFrameID Mask frame for image fading.
        * @param aOuterRect Frame outer rectangle.
        * @param aInnerRect Frame inner rectangle.
        * @param aBubble Bubble image.
        * @param aDimmed ETrue if image is shown dimmed.
        */
        static void PrepareCallObjectToBubbleImageL(
            const CFbsBitmap* aCOImage,
            const CFbsBitmap* aCOImageMask,
            const TRect& aCallObjectRect,
            const TAknsItemID& aFrameID,
            const TRect& aOuterRect,
            const TRect& aInnerRect,
            CEikImage*& aBubble,
            TBool aDimmed = EFalse );                                    

        /**
        * Fallback background drawing. This method can be used to draw background,
        * if skin/bitmap graphichs cannot be used (e.g. due to OOM).
        * @param aRect Rectangle.
        */                    
        static void DrawBackgroundRect( CBitmapContext& aGc,
                                        const TRect& aRect );

        /**
        * Layouts call type indicators in call type pane.
        * @since S60 3.2
        * @param aParent Parent rectangle.
        * @param aCallTypePane Call type pane.
        * @param aTypeIndication1 First indicator.
        * @param aTypeIndication2 Second indicator.
        */                                        
        static void LayoutCallTypeIndicators(
            const TRect& aParent,
            const TAknWindowLineLayout& aCallTypePane,
            CEikImage* aTypeIndication1,
            CEikImage* aTypeIndication2 );                                         
        
        
        /**
        * Prepares call image attached to call header to be
        * ready for drawing.
        * @since S60 3.2
        * @param aHeader Call header.
        * @param aSize Displayable size for call image.
        */
        static void PrepareCallObjectImageL( 
            CBubbleHeader& aHeader, 
            const TSize& aSize );
            
        /**
        * Combines given masks to new mask bitmap.
        * @since S60 5.0
        * @param aPrimaryMask Mask treated as primary mask.
        * @param aSecondaryMask Mask treated as secondary mask.
        * @return The combined mask.
        */
        static CFbsBitmap* CreateCombinedMaskL(
           const CFbsBitmap* aPrimaryMask,
           const CFbsBitmap* aSecondaryMask );            

    private:
        static void DrawMaskedImage( CBitmapContext& aGc , CEikImage& aImage );


};

#endif //BUBBLEUTILS_H

// End of File
