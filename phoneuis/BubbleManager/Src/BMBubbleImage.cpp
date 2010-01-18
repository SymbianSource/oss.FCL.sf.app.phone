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
* Description:  CEikImage wrapper.
*
*/


// INCLUDE FILES
#include    "BMBubbleImage.h"  //definition
#include    <AknsUtils.h>
#include    <AknIconUtils.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CBubbleImage::CBubbleImage
// ---------------------------------------------------------------------------
//
CBubbleImage::CBubbleImage()
: iIsBackgroundImage ( EFalse )
    {
    }

// Destructor
CBubbleImage::~CBubbleImage()
    {
    AknsUtils::DeregisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CBubbleImage::SetAsBackgroundImage
// Set the image as background image
//  
// ---------------------------------------------------------------------------
//
void CBubbleImage::SetAsBackgroundImage( TBool aIsBackground )
    {
    iIsBackgroundImage = aIsBackground;
    }

// ---------------------------------------------------------------------------
// CBubbleImage::SizeChanged
// called by framework when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleImage::SizeChanged()
    { 
    AknsUtils::RegisterControlPosition( this );

    if ( this->Bitmap() )
        {
        if ( iIsBackgroundImage )
            {
            // For background call bubble graphic, 
            // aspect ratio can not be preserved.
            AknIconUtils::SetSize( 
                const_cast<CFbsBitmap*> ( this->Bitmap() ), 
                Rect().Size(), EAspectRatioNotPreserved );
            }
        else
            {
            AknIconUtils::SetSize( 
                const_cast<CFbsBitmap*> ( this->Bitmap() ), 
                Rect().Size() );
            }
        }

    CEikImage::SizeChanged();
    }

// ---------------------------------------------------------------------------
// CBubbleImage::PositionChanged
// ---------------------------------------------------------------------------
//
void CBubbleImage::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    CEikImage::PositionChanged();
    }

// End of File
