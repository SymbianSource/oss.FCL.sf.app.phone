/*
* Copyright (c) 2003-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Thin wrapper over CEikImage to make sure that 
*       AknsUtils::RegisterControlPosition is called.
*
*/


#ifndef BMBUBBLEIMAGE_H
#define BMBUBBLEIMAGE_H

// INCLUDES  
#include    <eikimage.h>

// FORWARD DECLARATIONS
// CLASS DECLARATION

/**
*  CBubbleImage control class
*
*  @lib bubblemanager
*  @since 2.0
*/
class CBubbleImage : public CEikImage
    {    
    public: // Constructors and destructor
        
        /**
        * Constructor.
        */
        CBubbleImage();

        /**
        * Destructor.
        */
        ~CBubbleImage();

    public: // New functions
        void SetAsBackgroundImage( TBool aIsBackground );
        
    private: // Functions from base classes
        
        /**
        * From CCoeControl
        */
        void SizeChanged();

        /**
        * From CCoeControl
        */
        void PositionChanged();

    private: // Data
        TBool iIsBackgroundImage;
    
    };

#endif // BUBBLEPLACE_H

// End of File
