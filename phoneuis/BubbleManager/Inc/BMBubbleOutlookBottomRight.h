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
*     Draws bubble at bottom right i.e. in two call situation the second call
*
*/


#ifndef BUBBLEOUTLOOKBOTTOMRIGHT_H
#define BUBBLEOUTLOOKBOTTOMRIGHT_H

// INCLUDES
#include    "BMBubbleOutlookTwoLined.h"

// CLASS DECLARATION

/**
*  CBubbleOutlookBottomRight container class
*
*  Draws bubble at bottom right i.e. in two call situation the second call
*
*  @lib bubblemanager
*  @since 1.0
*/

class CBubbleOutlookBottomRight : public CBubbleOutlookTwoLined
    {
    public: // Constructors and destructor
        
        /**
        * Default constructor.
        * @param aBubbleManager Main container
        */
        CBubbleOutlookBottomRight( CBubbleManager& aBubbleManager );
        
        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL();
        
        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookBottomRight();

    public: // Functions from base classes
        
        /**
        * From CBubbleOutlook
        */
        void ReadBubbleHeader( CBubbleHeader& aHeader );
        
    private: // Functions from base classes
          
        /**
        * From CCoeControl
        */
        void SizeChanged();
        
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

#endif // BUBBLEOUTLOOKBOTTOMRIGHT_H

// End of File
