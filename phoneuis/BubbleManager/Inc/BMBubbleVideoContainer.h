/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Video Ringing Tone / Video window (DSA)
*
*/



#ifndef CBUBBLEVIDEOCONTAINER_H
#define CBUBBLEVIDEOCONTAINER_H

//  INCLUDES
#include <coecntrl.h>
#include <AknsDrawUtils.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Video Ringing Tone / Video window (DSA)
*
*  @lib BubbleManager
*  @since Series 60 3.1
*/
class CBubbleVideoContainer : public CCoeControl
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CBubbleVideoContainer* NewL( const CCoeControl* aParent );
        
        /**
        * Destructor.
        */
        virtual ~CBubbleVideoContainer();

    public: // New functions
        
        /**
        * Get window handle.
        * @since Series 60 3.1
        * @return Window handle
        */
        RWindow& GetWindow() const;
        
        /**
        * Set parameters needed to draw this control.
        * @since Series 60 3.1
        * @param aFrameId Skin id of the frame.
        * @param aOuterRect Outer frame rectangle.
        * @param aInnerRect Inner frame rectangle.
        */
        void SetBackgroundFrame( const TAknsItemID& aFrameId,
                                 TRect& aOuterRect,
                                 TRect& aInnerRect );
                                 
        /**
        * Sets redraw disabled/enabled.
        * Video player draws this area, when video is playing.
        * @since Series 60 3.2
        * @param aDisabled ETrue if redrawing is set disabled.
        */
        void SetRedrawDisabled( TBool aDisabled );

    private: // From CCoeControl
        /**
        * Draw
        */
        void Draw( const TRect& aRect ) const;
       
        /**
        * SizeChanged
        */
        void SizeChanged();
        
    private:

        /**
        * C++ default constructor.
        */
        CBubbleVideoContainer();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * C++ constructor
        */
        CBubbleVideoContainer(  const CCoeControl* aParent );

    private:    // Data
        // Parent control
        const CCoeControl* iParent;
        // Background frame parameters.
        TAknsItemID iFrameId;
        TRect iFrameOuterRect;
        TRect iFrameInnerRect; 
        // Redrawing flag
        TBool iRedrawDisabled;
    };

#endif      // CBUBBLEVIDEOCONTAINER_H   
            
// End of File

