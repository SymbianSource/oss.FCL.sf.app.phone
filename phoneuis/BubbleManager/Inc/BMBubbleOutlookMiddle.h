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
*     Draws bubble in middle i.e. in two call situation the first call
*
*/


#ifndef BUBBLEOUTLOOKMIDDLE_H
#define BUBBLEOUTLOOKMIDDLE_H

// INCLUDES
#include    "BMBubbleOutlookTwoLined.h"

// CLASS DECLARATION

/**
*  CBubbleOutlookMiddle container class
*
*  @lib bubblemanager
*  @since 1.0
*/
class CBubbleOutlookMiddle : public CBubbleOutlookTwoLined
    {
    public: // Constructors and destructor
        
        /**
        * Default constructor.
        * @param aBubbleManager Main container
        */
        CBubbleOutlookMiddle( CBubbleManager& aBubbleManager );
        
        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL();
        
        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookMiddle();

    public: //member functions
        
        /**
        * Set bubble go under. For example active bubble can be under or on top 
        * of bottom right bubble. There is only one line shadow in 'under bubble'.
        * @param aIsUnder ETrue puts bubble under the second call.
        */
        void SetIsUnder( const TBool& aIsUnder );

    public: // Functions from base classes
        
        /**
        * From CBubbleOutlook
        */
        void Reset( );
        
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
        * DoCall2Layout (may leave!)
        */            
        void DoCall2LayoutL();
        
        /**
        * GetCall1BubbleBitmaps
        */
        void GetCall1BubbleBitmaps();        
        
    private: //data
        
        // Is under or on top of bottom right bubble
        TBool iIsUnder;

    };

#endif // BUBBLEOUTLOOKMIDDLE_H

// End of File
