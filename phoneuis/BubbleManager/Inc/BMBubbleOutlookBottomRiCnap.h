/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Bottom right outlook with 3 lines
*
*/


#ifndef CBUBBLEOUTLOOKBOTTOMRIGHTCNAP_H
#define CBUBBLEOUTLOOKBOTTOMRIGHTCNAP_H

//  INCLUDES
#include "BMBubbleOutlookThreeLined.h"

// CLASS DECLARATION

/**
 *  Bottom right outlook with 3-lines.
 *
 *
 *  @lib BubbleManager
 *  @since S60 v5.0
 */
class CBubbleOutlookBottomRightCnap :public CBubbleOutlookThreeLined
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        * @param aBubbleManager Main container.
        */
        CBubbleOutlookBottomRightCnap( CBubbleManager& aBubbleManager );

        /**
        * 2nd phase constructor
        */
        void ConstructL();

        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookBottomRightCnap();


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

#endif // CBUBBLEOUTLOOKBOTTOMRIGHTCNAP_H  
            
// End of File
