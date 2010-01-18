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
* Description:  Bottom outlook with 3 lines and call image
*
*/


#ifndef CBUBBLEOUTLOOKBOTTOMIMAGECNAP_H
#define CBUBBLEOUTLOOKBOTTOMIMAGECNAP_H


#include "BMBubbleOutlookThreeLined.h"

/**
 *  Bottom outlook with 3 lines and call image
 *
 *
 *  @lib BubbleManager
 *  @since S60 v5.0
 */
class CBubbleOutlookBottomImageCnap : public CBubbleOutlookThreeLined
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        * @param aBubbleManager Main container.
        */
        CBubbleOutlookBottomImageCnap( CBubbleManager& aBubbleManager );

        /**
        * 2nd phase constructor
        */
        void ConstructL();

        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookBottomImageCnap();


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
        
#endif // CBUBBLEOUTLOOKBOTTOMIMAGECNAP_H

// End of File
