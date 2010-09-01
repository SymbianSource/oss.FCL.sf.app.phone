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
*     Draw bubble on top i.e. in three call situation the first call
*
*/


#ifndef BUBBLEOUTLOOKTOPLEFT_H
#define BUBBLEOUTLOOKTOPLEFT_H

// INCLUDES
#include    "BMBubbleOutlookOneLined.h"

// CLASS DECLARATION

/**
*  CBubbleOutlookTopLeft container class
*
*  Draw bubble on top i.e. in three call situation the first call
*
*  @lib bubblemanager
*  @since 1.0
*/
class CBubbleOutlookTopLeft : public CBubbleOutlookOneLined
    {
    public: // Constructors and destructor
        
        /**
        * Default constructor.
        * @param aBubbleManager Main container.
        */
        CBubbleOutlookTopLeft( CBubbleManager& aBubbleManager);
        
        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL();
        
        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookTopLeft();

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

#endif // BUBBLEOUTLOOKTOPLEFT_H

// End of File
