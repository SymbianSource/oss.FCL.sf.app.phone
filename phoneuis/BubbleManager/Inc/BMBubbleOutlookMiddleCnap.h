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
*     Outlook class for Cnap bubble on bottom.
*
*/


#ifndef CBUBBLEOUTLOOKMIDDLECNAP_H
#define CBUBBLEOUTLOOKMIDDLECNAP_H

//  INCLUDES
#include "BMBubbleOutlookThreeLined.h"

// CLASS DECLARATION

/**
*  Outlook class for Cnap bubble on bottom.
*
*  @lib bubblemanager
*  @since 1.2
*/
class CBubbleOutlookMiddleCnap :public CBubbleOutlookThreeLined
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        * @param aBubbleManager Main container.
        */
        CBubbleOutlookMiddleCnap( CBubbleManager& aBubbleManager );

        /**
        * 2nd phanse constructor
        */
        void ConstructL();

        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookMiddleCnap();


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

#endif      // CBUBBLEOUTLOOKMIDDLECNAP_H  
            
// End of File
