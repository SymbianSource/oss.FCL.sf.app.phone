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
*     Base class for big bubbles (bottom, bottom right, middle).
*
*/


#ifndef CBUBBLEOUTLOOKONELINED_H
#define CBUBBLEOUTLOOKONELINED_H

//  INCLUDES
#include <calslbs.h>
#include "BMBubbleOutlook.h"
#include "BMBubbleManager.h"

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
class CBubbleOutlookOneLined : public CBubbleOutlook
    {
    public:  // Destructor

        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookOneLined();
        
    protected: // Constructors

        /**
        * C++ default constructor.
        * @param aBubbleManager Main container
        */
        CBubbleOutlookOneLined( CBubbleManager& aBubbleManager );

        /**
        * 2nd phase constructor
        */
        void ConstructL();

    public: // Functions from base classes

        /**
        * From CBubbleOutlook.
        */
        virtual void Reset();
        
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
        virtual void ReleaseBitmaps();

        /**
        * From CBubbleOutlook.
        */
        virtual void RebuildBitmaps();


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

        // Current call header
        CBubbleHeader* iHeader;
        
        // Bubble image
        CEikImage* iBubble;

        // Small call indication
        CTelBubbleCustomElement* iSmallCallIndication;
        
        // Type indication #1
        CEikImage* iTypeIndication1;
        // Type indication #2
        CEikImage* iTypeIndication2;
        // Cyphering off indication
        CEikImage* iCyphOffImage;
        
        // Text label
        mutable CEikLabel* iTextLine1;
        // Pointer to original text.
        TPtrC iFullText1;
        // Clipping direction
        CBubbleManager::TPhoneClippingDirection iText1ClipDirection;
    };

#endif      // CBUBBLEOUTLOOKONELINED_H
            
// End of File
