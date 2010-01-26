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
* Description:  Outlook base class for five lined bubbles
*
*/



#ifndef CBUBBLEOUTLOOKFIVELINED_H
#define CBUBBLEOUTLOOKFIVELINED_H

//  INCLUDES
#include <calslbs.h>
#include "BMBubbleOutlook.h"
#include "BMBubbleManager.h"
#include "BMUtils.h"

// FORWARD DECLARATIONS
class CEikImage;
class CEikLabel;
class CBubbleImageManager;
class CBubbleHeader;
class CTelBubbleCustomElement;


// CLASS DECLARATION

/**
*  Outlook base class for five lined bubbles
*
*  @lib bubblemanager
*  @since Series60_1.2
*/
class CBubbleOutlookFiveLined :public CBubbleOutlook
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookFiveLined();

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
        virtual void DrawCallHeaderText();

    private:  // Functions from base classes
        
        /**
        * From CCoeControl
        */
        virtual void SizeChanged() = 0;
        
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
        
    protected:

        /**
        * C++ default constructor.
        * @param aBubbleManager Main container
        */
        CBubbleOutlookFiveLined( CBubbleManager& aBubbleManager );

        /**
        * 2nd phase constructor
        */
        void ConstructL();
        
    protected:    // Data
        // Current header
        CBubbleHeader* iHeader;
        // Text line number
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
        // Cyph off
        CEikImage* iCyphOffImage;
                
        // tn shadow
        TBubbleLayoutRect iTnImageShadow;

        // tn
        CEikImage* iTnImage;
        
        // 1st line label
        mutable CEikLabel* iTextLine1; // Text will be clipped and placed in 
        // 2nd line label
        mutable CEikLabel* iTextLine2; // Draw() const method.
        // 3rd line label
        mutable CEikLabel* iTextLine3;
        // 3rd line label
        mutable CEikLabel* iTextLine4;
        // 3rd line label
        mutable CEikLabel* iTextLine5;
        // Original buffer
        TPtrC iFullText1; 
        // Original buffer
        TPtrC iFullText2;
        // Original buffer
        TPtrC iFullText3;
        // Original buffer
        TPtrC iFullText4;
        // Original buffer
        TPtrC iFullText5;
        // clip dir
        CBubbleManager::TPhoneClippingDirection iText1ClipDirection;
        // clip dir
        CBubbleManager::TPhoneClippingDirection iText2ClipDirection;
        // clip dir
        CBubbleManager::TPhoneClippingDirection iText3ClipDirection;
        // Timer/cost label
        CEikLabel* iTimerCost;
        // Three lines of call text layout
        TBool iThreeLinedLayout;                                         
    
    };

#endif      // CBUBBLEOUTLOOKFIVELINED_H
            
// End of File