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
* Description:  Wraps extension control and binds it to UI element. 
*
*/


#ifndef TELBUBBLECUSTOMELEMENT_H
#define TELBUBBLECUSTOMELEMENT_H

#include <e32base.h>

class CTelBubbleAnim;
class CTelBubbleLabel;
class CTelBubbleImage;
class CCoeControl;
class CTelBubbleCallImage;

/**
 *  Wraps extension control and binds it to UI element. 
 *
 *
 *  @lib BMCustomization.lib
 *  @since S60 v5.0
 * 
 * 
 *  Usage:
 * 
 *    // Display custom icon in big call indicator position
 *    CTelBubbleImage* image = CTelBubbleImage::NewL();
 *    image->SetPicture( bitmap, mask );             
 *    CTelBubbleCustomElement* element =
 *         CTelBubbleCustomElement::NewL( 
 *              image, 
 *              CTelBubbleCustomElement::EBigCallIndicator );
 * 
 */
NONSHARABLE_CLASS( CTelBubbleCustomElement ) : public CBase
    {
public:
    // Customized UI element
    enum TElementType
        {
        // Handset icon in initializing call states
        EBigCallIndicator = 0,
        // Number type icon ( ie. internet or video call )
        ENumberTypeIcon,
        // Handset icon
        ESmallCallIndicator,
        // Icon in call bubble (placeholder)
        EBrandImage,
        // Contact or default picture in main pane
        ECallImage,
        // Keep this last
        ECustomElementCount
        };
        
    // Custom control type
    enum TControlType
        {
        // CTelBubbleImage based control
        EBubbleImage,
        // CTelBubbleAnim based control
        EBubbleAnimation,
        // CCoeControl based control
        EBubbleControl,
        // CTelBubbleLabel based control
        EBubbleLabel,
        // CTelBubbleCallImage based control
        EBubbleCallImage
        };
    
public:
    /**
    * Two phase constructor
    * @param aControl Sets owned control, ownership is transferred
    * @param aElement Sets place where control is shown
    * @return New instance
    */
    IMPORT_C static CTelBubbleCustomElement* NewL( 
        CCoeControl* aControl, TElementType aElement );
    IMPORT_C static CTelBubbleCustomElement* NewL(
        CTelBubbleAnim* aControl, TElementType aElement );
    IMPORT_C static CTelBubbleCustomElement* NewL( 
        CTelBubbleLabel* aControl, TElementType aElement );
    IMPORT_C static CTelBubbleCustomElement* NewL(
        CTelBubbleImage* aControl, TElementType aElement );
    IMPORT_C static CTelBubbleCustomElement* NewL(
        CTelBubbleCallImage* aControl, TElementType aElement );
    
    /** Destructor */
    IMPORT_C virtual ~CTelBubbleCustomElement();
    
public:
    /**
     * Returns custom control.
     */
    IMPORT_C CCoeControl* Control() const;
    
    /**
     * Returns control type.
     */
    IMPORT_C TControlType ControlType() const;
    
    /**
     * Return constomized UI element.
     */
    IMPORT_C TElementType ElementType() const;
    
private:
    // C++ constructors
    CTelBubbleCustomElement( CCoeControl* aControl, 
                             TElementType aElement );
    CTelBubbleCustomElement( CTelBubbleAnim* aControl, 
                             TElementType aElement );
    CTelBubbleCustomElement( CTelBubbleLabel* aControl, 
                             TElementType aElement );
    CTelBubbleCustomElement( CTelBubbleImage* aControl, 
                             TElementType aElement );
    CTelBubbleCustomElement( CTelBubbleCallImage* aControl, 
                             TElementType aElement );
    
private:
    CCoeControl* iControl; // owned
    TControlType iControlType;
    TElementType iElementType;
    };

#endif // TELBUBBLECUSTOMELEMENT_H

// end of file
