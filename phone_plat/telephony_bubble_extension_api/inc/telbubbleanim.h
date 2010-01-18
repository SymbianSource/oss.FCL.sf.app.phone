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
* Description:  Extension control for window server animation.
*
*/



#ifndef TELBUBBLEANIM_H
#define TELBUBBLEANIM_H

#include <coecntrl.h>

class CAknBitmapAnimation;
class CBitmapAnimClientData;
class CEikImage;

/**
 *  Extension control for window server animation.
 *
 *
 *  @lib BMCustomization.lib
 *  @since S60 v5.0
 */
class CTelBubbleAnim : public CCoeControl
    {
public:
    /**
    * Two phased constructor.
    * @param aFrameInterval Sets time between showed frames.
    *                       Frames are played in cycle mode.
    */
    IMPORT_C static CTelBubbleAnim* NewL( TInt aFrameInterval );
    
    /** destructor */
    IMPORT_C virtual ~CTelBubbleAnim();

public:
    /** Starts animation - this is called from BubbleManager */
    IMPORT_C void StartAnimationL();
    
    /** Stop animation */
    IMPORT_C void StopAnimation();
    
    /**
     * Adds frame to animation.
     * @param aFrame Frame to be added. Parameter is deleted before 
     *               returning. Ownership of bitmaps transferred.
     */
    IMPORT_C void AddFrameToAnimationLD( CEikImage* aFrame ) const;

    /**
     * Adds frame to animation.
     * @param aFrame Frame to be added. 
     *               Ownership of bitmaps not transferred.
     */
    IMPORT_C void AddFrameToAnimationL( CEikImage& aFrame ) const;
    
    /**
     * Resets animation. Also frame array is reseted.
     */
    IMPORT_C void Reset() const;

protected: // from CCoeControl

    /** returns 0 for animation */
    IMPORT_C virtual TInt CountComponentControls() const;

    /** set container window */
    IMPORT_C virtual void SetContainerWindowL( 
        const CCoeControl& aContainer );
    
    /** sets new size for animation */
    IMPORT_C virtual void SizeChanged();

    /** starts animation */
    IMPORT_C virtual void Draw( const TRect& aRect ) const;
    
    /** handles animation visibility */
    IMPORT_C virtual void MakeVisible( TBool aVisible );
    
protected:
    IMPORT_C virtual void ConstructL( TInt aFrameInterval );
    IMPORT_C CTelBubbleAnim();
    
protected:
    CAknBitmapAnimation* iAnimation; // owned
    };

#endif // TELBUBBLEANIM_H

// end of file
