/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Default image implementation.
*
*/


#include <AknsUtils.h>


#include "telbubbleimage.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleImage::CTelBubbleImage()
    {
    }

// ---------------------------------------------------------------------------
// CBubbleAnim::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleImage* CTelBubbleImage::NewL()
    {
    CTelBubbleImage* self = new( ELeave )CTelBubbleImage();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleImage::~CTelBubbleImage()
    {
    AknsUtils::DeregisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CTelBubbleImage::SizeChanged
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleImage::SizeChanged()
    { 
    AknsUtils::RegisterControlPosition( this );

    if ( this->Bitmap() )
        {
        AknIconUtils::SetSize( 
            const_cast<CFbsBitmap*> ( this->Bitmap() ), 
            Rect().Size() );
        }

    CEikImage::SizeChanged();
    }

// ---------------------------------------------------------------------------
// CTelBubbleImage::PositionChanged
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleImage::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    CEikImage::PositionChanged();
    }

// ---------------------------------------------------------------------------
// CTelBubbleImage::ConstructL
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleImage::ConstructL()
    {
    }

// end of file
