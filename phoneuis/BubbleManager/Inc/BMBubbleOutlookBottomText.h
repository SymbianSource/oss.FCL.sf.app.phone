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
*     Class draws the textual CLI bubble on the bottom of the screen
*
*/


#ifndef BUBBLEOUTLOOKBOTTOMTEXT_H
#define BUBBLEOUTLOOKBOTTOMTEXT_H

// INCLUDES
#include    "BMBubbleOutlookFiveLined.h"

// CLASS DECLARATION

/**
*  CBubbleOutlookBottomText container class
*
*  Class draws the textual CLI bubble on the bottom of the screen
*
*  @lib bubblemanager
*  @since 1.0
*/
class CBubbleOutlookBottomText : public CBubbleOutlookFiveLined
    {
    public: // Constructors and destructor       

        /**
        * Default constructor.
        * @param aBubbleManager The main container.
        */
        CBubbleOutlookBottomText( CBubbleManager& aBubbleManager );

        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL();
        
        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookBottomText();
        
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
        * DoCall2LayoutL
        */            
        void DoCall2LayoutL();
        
        /**
        * SetCallObjectTextToLabelsL
        */
        void SetCallObjectTextToLabelsL( const TDesC& aText,
                                         TInt aLabelCount,
                                         CEikLabel* aLabels[] );
    };

#endif // BUBBLEOUTLOOKBOTTOMTEXT_H

// End of File
