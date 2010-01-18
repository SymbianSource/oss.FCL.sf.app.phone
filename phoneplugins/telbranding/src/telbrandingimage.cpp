/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Branding specific image.
*
*/


#include "telbrandingimage.h"


// ======== LOCAL FUNCTIONS ========
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTelBrandingImage::CTelBrandingImage
// ---------------------------------------------------------------------------
//
CTelBrandingImage::CTelBrandingImage() :
    iVisibilityAllowed( ETrue )
    {
    }


// ---------------------------------------------------------------------------
// CTelBrandingImage::ConstructL
// ---------------------------------------------------------------------------
//
void CTelBrandingImage::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CTelBrandingImage::NewL
// ---------------------------------------------------------------------------
//
CTelBrandingImage* CTelBrandingImage::NewL()
    {
    CTelBrandingImage* self = CTelBrandingImage::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CTelBrandingImage::NewLC
// ---------------------------------------------------------------------------
//
CTelBrandingImage* CTelBrandingImage::NewLC()
    {
    CTelBrandingImage* self = new( ELeave ) CTelBrandingImage;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CTelBrandingImage::~CTelBrandingImage
// ---------------------------------------------------------------------------
//
CTelBrandingImage::~CTelBrandingImage()
    {

    }

// ---------------------------------------------------------------------------
// CTelBrandingImage::MakeVisible
// ---------------------------------------------------------------------------
//
void CTelBrandingImage::MakeVisible( TBool aVisible )
    {
    if( iVisibilityAllowed && iLowerPriority )
        {
        // Inform lower priority element
        //  - Allow to be shown when this is not shown
        //  - Dont allow to show when this is shown
        iLowerPriority->SetVisibilityAllowed( !aVisible );
        }
    
    if( aVisible )
        {
        if( iVisibilityAllowed )
            {
            // Make visible since its allowed
            CTelBubbleImage::MakeVisible( aVisible );
            }
        else
            {
            // Do nothing when visibilty not allowed
            }
        }
    else
        {
        // Always make not visible
        CTelBubbleImage::MakeVisible( aVisible );
        }

    }

// ---------------------------------------------------------------------------
// CTelBrandingImage::SetVisibilityAllowed
// ---------------------------------------------------------------------------
//
void CTelBrandingImage::SetVisibilityAllowed( TBool aAllowed )
    {
    iVisibilityAllowed = aAllowed;
    
    if( iLowerPriority )
        {
        // Inform lower priority elements
        iLowerPriority->SetVisibilityAllowed( aAllowed );
        }
    
    if( IsVisible() && !iVisibilityAllowed )
        {
        // If visible and visibility not allowed
        CTelBubbleImage::MakeVisible( EFalse );
        }
    
    }

// ---------------------------------------------------------------------------
// CTelBrandingImage::SetLowerPriorityBranding
// ---------------------------------------------------------------------------
//
void CTelBrandingImage::SetLowerPriorityBranding( 
    MTelBrandingPriorityHandler* aHandler )
    {
    iLowerPriority = aHandler;
    }


