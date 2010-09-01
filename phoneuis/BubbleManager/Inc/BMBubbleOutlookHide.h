/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Draws hiding bubble on top
*
*/


#ifndef BUBBLEOUTLOOKTOPHIDE_H
#define BUBBLEOUTLOOKTOPHIDE_H

// INCLUDES
#include    <calslbs.h>
#include    "BMBubbleOutlook.h"

// FORWARD DECLARATIONS
class CEikLabel; 
class CEikImage;
class CBubbleImageManager;
class CBubbleHeader;


// CLASS DECLARATION

/**
*  CBubbleOutlookHide container class
*
*  Draws hiding bubble on top
*
*  @lib bubblemanager
*  @since 1.0
*/
class CBubbleOutlookHide : public CBubbleOutlook  
    {
    public: // Constructors and destructor
        
        /**
        * Default constructor.
        * @param aBubbleManager Main container
        */
        CBubbleOutlookHide( CBubbleManager& aBubbleManager );
        
        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL();
        
        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookHide();

    public: // Functions from base classes
        
        /**
        * From CBubbleOutlook
        */
        void Reset( );
        
        /**
        * From CBubbleOutlook
        */
        void ReadBubbleHeader( CBubbleHeader& aHeader );
        
        /**
        * From CBubbleOutlook
        */
        virtual void DrawTimerCostNow();
        
        /**
        * From CBubbleOutlook
        */
        virtual void DrawCLINow();

        /**
        * From CBubbleOutlook
        */
        void DrawBitmaps( CBitmapContext& aGc ) const;

        /**
        * From CBubbleOutlook.
        */
        virtual void HandleAnimationStartL() const;

        /**
        * From CBubbleOutlook.
        */
        virtual void DrawCallHeaderText();
        
    private: // Functions from base classes
             
        /**
        * From CCoeControl
        */
        void SizeChanged();
        
        
    private: //data
        
        // Bubble image
        CEikImage* iBubble;
    };

#endif // BUBBLEOUTLOOKTOPHIDE_H

// End of File
