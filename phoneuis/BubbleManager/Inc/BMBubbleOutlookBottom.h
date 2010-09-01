/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Class draws the biggest bubble on the bottom of the screen
*
*/


#ifndef BUBBLEOUTLOOKBOTTOM_H
#define BUBBLEOUTLOOKBOTTOM_H

// INCLUDES
#include    "BMBubbleOutlookTwoLined.h"

// CLASS DECLARATION

/**
*  CBubbleOutlookBottom container class
*
*  Class draws the biggest bubble on the bottom of the screen
*
*  @lib bubblemanager
*  @since 1.0
*/
class CBubbleOutlookBottom : public CBubbleOutlookTwoLined
    {
    public: // Constructors and destructor       

        /**
        * Default constructor.
        * @param aBubbleManager The main container.
        */
        CBubbleOutlookBottom( CBubbleManager& aBubbleManager );

        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL();
        
        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookBottom();
        
    public: // Functions from base classes
          
        /**
        * From CBubbleOutlook
        */
        virtual void ReadBubbleHeader( CBubbleHeader& aHeader );   
        
    private: // Functions from base classes
        
        /**
        * From CCoeControl
        */
        void SizeChanged();

        /**
        * From CCoeControl
        */
        void PositionChanged();

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
        * GetCall1BubbleBitmaps
        */
        void GetCall1BubbleBitmaps();
        
    };

#endif // BUBBLEOUTLOOKBOTTOM_H

// End of File
