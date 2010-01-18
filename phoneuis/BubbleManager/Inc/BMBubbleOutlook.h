/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Abstract base class fror bubble outlooks
*
*/


#ifndef BUBBLEOUTLOOK_H
#define BUBBLEOUTLOOK_H

// INCLUDES
#include <coecntrl.h>
#include <AknsUtils.h>

// CONSTANT

// FORWARD DECLARATIONS
class CBubbleHeader;
class CBubbleManager;

// CLASS DECLARATION

/**
*  CBubbleOutlook container class
*
*  Abstract base class fror bubble outlooks
*
*  @lib bubblemanager
*  @since 1.0
*/
class CBubbleOutlook : public CCoeControl 
    {
    public: // Constructors and destructor
        
        /**
        * Destructor.
        */
        virtual ~CBubbleOutlook();
        
    protected:
        
        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL( );
        
        /**
        * Default constructor.
        * @param aBubbleManager Reference to the main component.
        */
        CBubbleOutlook( CBubbleManager& aBubbleManager );
        
    public: //member functions
        
        /**
        * To reset the outlook.
        */
        virtual void Reset( ) = 0;
        
        /**
        * Give header from which the outlook should read it's info.
        * @param aHeader The header.
        */
        virtual void ReadBubbleHeader( CBubbleHeader& aHeader ) = 0;
        
        /**
        * Update timer/cost value and redraw it.
        */
        virtual void DrawTimerCostNow() = 0;
        
        /**
        * Update CLI string and redraw it.
        */
        virtual void DrawCLINow() = 0;

        /**
        * Draw bitmaps to given context
        * @param aGc Context to draw to.
        */
        virtual void DrawBitmaps( CBitmapContext& aGc ) const = 0;

        /**
        * Start animation.
        */
        virtual void HandleAnimationStartL() const = 0;

        /**
        * Release anim
        * @since 2.0
        */
        virtual void ReleaseBitmaps();

        /**
        * Recreate the animation
        * @since 2.0
        */
        virtual void RebuildBitmaps();

        /**
        * Update call header text and redraw it.
        */
        virtual void DrawCallHeaderText() = 0;
        
        /**
        * Call object display on/off.
        * @since 3.1
        */
        void SetCallObjectDisplay( TBool aActive );
        
    protected: // New functions

        // Draws label
        void DrawLabelNow( const CCoeControl& aLabel );
        // Draws label if pointer is != NULL.
        void DrawLabelNow( const CCoeControl* aLabel );
        
    private: // Functions from base classes
        
        /**
        * From CCoeControl
        */
        virtual void SizeChanged( ) = 0;
        
        /**
        * From CCoeControl
        */
        virtual void PositionChanged();

        /**
        * From CCoeControl
        */
        virtual TInt CountComponentControls( ) const;
        
        /**
        * From CCoeControl
        */
        virtual CCoeControl* ComponentControl( TInt aIndex ) const;
        
        /**
        * From CCoeControl
        */
        virtual void Draw( const TRect& aRect ) const;
            
              
    protected: //data

        // Reference to the main control
        CBubbleManager& iBubbleManager;
        
        // Feature settings
        TBool iCallObjectDisplay;
        
        // Frame settings for Call2 layouts
        TAknsItemID iFrameId;
        TRect       iOuterRect;
        TRect       iInnerRect;
        
    };



#endif // BUBBLEOUTLOOK_H

// End of File
