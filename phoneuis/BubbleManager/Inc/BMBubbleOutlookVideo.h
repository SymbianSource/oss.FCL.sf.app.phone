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


#ifndef C_BUBBLEOUTLOOKVIDEO_H
#define C_BUBBLEOUTLOOKVIDEO_H

#include    "BMBubbleOutlookTwoLined.h"

/**
 *  Bubble outlook video ringtone.
 *
 *  @lib BubbleManager
 *  @since S60 v3.2
 */
 class CBubbleOutlookVideo : public CBubbleOutlookTwoLined
    {
    public: // Constructors and destructor       
        /**
        * Default constructor.
        * @param aBubbleManager The main container.
        */
        CBubbleOutlookVideo( CBubbleManager& aBubbleManager );

        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL();
        
        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookVideo();
        
    public: // Functions from base classes
          
        /**
        * From CBubbleOutlook
        */
        virtual void ReadBubbleHeader( CBubbleHeader& aHeader );
        
        /**
        * From CBubbleOutlook
        */
        virtual void DrawBitmaps( CBitmapContext& aGc ) const;
        
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
 
#endif // C_BUBBLEOUTLOOKVIDEO_H
