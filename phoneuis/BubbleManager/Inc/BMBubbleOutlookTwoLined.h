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
* Description: 
*     Base class for big bubbles (bottom, bottom right, middle).
*
*/


#ifndef CBUBBLEOUTLOOKTWOLINED_H
#define CBUBBLEOUTLOOKTWOLINED_H

//  INCLUDES
#include <calslbs.h>
#include "BMBubbleOutlook.h"
#include "BMBubbleManager.h"
#include "BMUtils.h"

// FORWARD DECLARATIONS
class CBubbleHeader;
class CEikLabel; 
class CEikImage;
class CBubbleImageManager;
class CBubbleAnimationControl;
class CTelBubbleCustomElement;

// CLASS DECLARATION

/**
*  Base class for big bubbles (bottom, bottom right, middle).
*
*  @lib bubblemanager
*  @since 1.0
*/
class CBubbleOutlookTwoLined : public CBubbleOutlook
    {
    public:  // Destructor

        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookTwoLined();
        
    protected: // Constructors

        /**
        * C++ default constructor.
        * @param aBubbleManager Main container
        */
        CBubbleOutlookTwoLined( CBubbleManager& aBubbleManager );

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();
        
    public: // Functions from base classes

        /**
        * From CBubbleOutlook.
        */
        virtual void Reset( );
        
        /**
        * From CBubbleOutlook.
        */
        virtual void ReadBubbleHeader( CBubbleHeader& aHeader );
        
        /**
        * From CBubbleOutlook.
        */
        virtual void DrawTimerCostNow();
        
        /**
        * From CBubbleOutlook.
        */
        virtual void DrawCLINow();

        /**
        * From CBubbleOutlook.
        */
        virtual void DrawBitmaps( CBitmapContext& aGc ) const;

        /**
        * From CBubbleOutlook.
        */
        virtual void HandleAnimationStartL() const;

        /**
        * From CBubbleOutlook.
        */
        virtual void DrawCallHeaderText();
        
    private:  // Functions from base classes
        
        /**
        * From CCoeControl
        */
        virtual void SizeChanged( ) = 0;
        
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
        
    private: // New functions

        /**
        * Draw Call1 bitmaps
        */
        void DrawCall1Bitmaps( CBitmapContext& aGc ) const;
        
        /**
        * Draw Call2 bitmaps
        */
        void DrawCall2Bitmaps( CBitmapContext& aGc ) const;

    protected:  // Data

        // Current header
        CBubbleHeader* iHeader;
        // Line number where the label is set
        TUint8 iTextLineNumber;
        
        // Bubble image
        CEikImage* iBubble;
        
        // Big call indication / customized
        CTelBubbleCustomElement* iBigCallIndicator;
        // Small call indication
        CTelBubbleCustomElement* iSmallCallIndication;
        // Number type
        CTelBubbleCustomElement* iNumberType;
        // Call type indicator #1
        CEikImage* iTypeIndication1;
        // Call type indicator #2
        CEikImage* iTypeIndication2;
        // Cyphering off
        CEikImage* iCyphOffImage;

        // tn shadow
        TBubbleLayoutRect iTnImageShadow;
        
        // Thumbnail image
        CEikImage* iTnImage;

        // 1st line label
        mutable CEikLabel* iTextLine1; // Text will be clipped and placed in 
        // 2nd line text
        mutable CEikLabel* iTextLine2; // Draw() const method.
        // Original buffer
        TPtrC iFullText1; 
        // Original buffer
        TPtrC iFullText2;
        // Clip dir
        CBubbleManager::TPhoneClippingDirection iText1ClipDirection;
        // Clip dir
        CBubbleManager::TPhoneClippingDirection iText2ClipDirection;
        
        // Timer cost
        CEikLabel* iTimerCost;
        // Brand image.
        CTelBubbleCustomElement* iBrandImage;
    };

#endif      // CBUBBLEOUTLOOKTWOLINED_H
            
// End of File
