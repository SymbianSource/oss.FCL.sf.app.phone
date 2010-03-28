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
*     Draws expanded conference call
*
*/


#ifndef BUBBLEOUTLOOKCONFERENCE_H
#define BUBBLEOUTLOOKCONFERENCE_H

// INCLUDES
#include    <calslbs.h>
   
#include    "BMBubbleOutlook.h"
#include    "BMCallHeader.h"
#include    "BMConfPane.h"

// FORWARD DECLARATIONS
class CEikLabel;       
class CEikImage;
class CBubbleImageManager;
class CConfHeader;


// CLASS DECLARATION

/**
*  CBubbleOutlookConference container class
*
*  Object handles expanded conference call.
*
*  @lib bubblemanager 
*  @since 1.0
*/
class CBubbleOutlookConference : public CBubbleOutlook
{

    public: // Constructors and destructor
       
        /**
        * Symbian OS default constructor.
        * @param aBubbleManager Main container
        */
        CBubbleOutlookConference( CBubbleManager& aBubbleManager );

        /**
        * Symbian OS 2nd phase constructor.
        * @param aCallAmount Amount of panes allocated.
        */
        void ConstructL( const TUint8& aCallAmount );

        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookConference();

    public: // New functions
    
        /**
        * Draws given row
        * @param aRow Row number to be drawn.
        */
        void DrawRowNow( CBubbleManager::TRowNumber aRow );


    public:// Functions from base classes

        /**
        * From CBubbleOutlook 
        */
        void Reset();

        /**
        * From CBubbleOutlook 
        */
        void ReadBubbleHeader( CBubbleHeader& aHeader );

        /**
        * From CBubbleOutlook 
        */
        virtual void DrawTimerCostNow();

        /**
        * From CBubbleOutlook
        */
        virtual void DrawCLINow();

        /**
        * From CBubbleOutlook
        */
        void DrawBitmaps( CBitmapContext& aGc ) const;

        /**
        * From CBubbleOutlook.
        */
        virtual void HandleAnimationStartL() const;

        /**
        * From CBubbleOutlook.
        */
        virtual void DrawCallHeaderText();

    private: // Functions from base classes

       /**
        * From CCoeControl Called when rect changes
        */
        void SizeChanged();

       /**
        * From CCoeControl
        */
        TInt CountComponentControls() const;

       /**
        * From CCoeControl
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

       /**
        * From CCoeControl
        */
        void Draw( const TRect& aRect ) const;

       /**
        * From CCoeControl
        */ 
        void HandlePointerEventL( const TPointerEvent& aPointerEvent );

    private:
    
        /**
        * DoCall1Layout
        */
        void DoCall1Layout();

        /**
        * DoCall2LayoutL
        */            
        void DoCall2LayoutL();
        
        /**
        * DoCall4Layout
        */            
        void DoCall4LayoutL();
        
        /**
        * GetCall1BubbleBitmaps
        */
        void GetCall1BubbleBitmaps();

    private: //data

        // call amount in conference
        TUint8 iPaneAmount; 

        // all the information
        CBubbleConfHeader* iHeader; 

        // speech bubble image
        CEikImage* iBubble; 
        // Timer control
        CEikLabel* iTimerCost; 

        // for drawing
        CArrayPtrFlat<CBubbleConfPane>* iConfPanes; 
        // for fetching info
        CArrayPtrFlat<CBubbleCallHeader>* iCalls; 
        
    private:

        friend class CT_CBubbleOutlookConference;
    
};

#endif // BUBBLEOUTLOOKCONFERENCE_H

// End of File
