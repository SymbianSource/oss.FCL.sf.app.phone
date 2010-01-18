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
* Description:  Bubble outlook video ringtone.
*
*/


#ifndef C_BUBBLEOUTLOOKVIDEOCNAP_H
#define C_BUBBLEOUTLOOKVIDEOCNAP_H

// INCLUDES
#include    "BMBubbleOutlookThreeLined.h"

// CLASS DECLARATION

/**
*  CBubbleOutlookVideo container class
*
*  Class draws the video ringtone bubble.
*
*  @lib bubblemanager
*  @since 3.2
*/
class CBubbleOutlookVideoCnap : public CBubbleOutlookThreeLined
    {
    public: // Constructors and destructor       
        /**
        * Default constructor.
        * @param aBubbleManager The main container.
        */
        CBubbleOutlookVideoCnap( CBubbleManager& aBubbleManager );

        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL();
        
        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookVideoCnap();
        
    public: // Functions from base classes
          
        /**
        * From CBubbleOutlook
        */
        virtual void ReadBubbleHeader( CBubbleHeader& aHeader );
        
        /**
        * From CBubbleOutlook
        */
        void DrawBitmaps( CBitmapContext& aGc ) const;
        
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
        void LayoutVideoPanes( const TRect& aParentRect );        

    private:
        TRect iLineRect;
    };

#endif // C_BUBBLEOUTLOOKVIDEOCNAP_H

// End of File
