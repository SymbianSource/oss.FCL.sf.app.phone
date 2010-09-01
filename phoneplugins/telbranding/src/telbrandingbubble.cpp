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
* Description:  Wraps bubble into one class.
*
*/

#include "telbrandingbubble.h"
#include "telbrandingcallindicator.h"
#include "telbrandingserverhandler.h"
#include "telbrandingimage.h"

#include <telbubbleextensionobserver.h>
#include <telbubbleextensiondata.h>
#include <telbubblecustomelement.h>
#include <telbubbleimage.h>

// ======== CONSTANTS ========
_LIT8( KBrandImageId, "bubble_manager_brand_image" );
_LIT8( KCallIndicatorImageId, "bubble_manager_call_indicator_image" );


// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTelBrandingBubble::CTelBrandingBubble
// ---------------------------------------------------------------------------
//
CTelBrandingBubble::CTelBrandingBubble( MTelBubbleExtensionData& aCallData ) :
    iCallData( aCallData )
    {

    }


// ---------------------------------------------------------------------------
// CTelBrandingBubble::ConstructL
// ---------------------------------------------------------------------------
//
void CTelBrandingBubble::ConstructL(
    CTelBrandingServerHandler& aServer )
    {
    // Branded call indication
    TRAP_IGNORE( 
        CreateCallIndicationElementL( 
            aServer, 
            CTelBubbleCustomElement::ESmallCallIndicator ); 
        
        CreateCallIndicationElementL( 
            aServer, 
            CTelBubbleCustomElement::EBigCallIndicator ); 
        )
    
    if( !iElements.Count() )
        {
        // Basic brand image
        TRAP_IGNORE( AddBrandingImageWithPriorityL( 
                KBrandImageId, 
                aServer, 
                CTelBubbleCustomElement::EBrandImage ); 
            )
        }

    }

// ---------------------------------------------------------------------------
// CTelBrandingBubble::NewLC
// ---------------------------------------------------------------------------
//
CTelBrandingBubble* CTelBrandingBubble::NewLC(
    MTelBubbleExtensionData& aCallData, 
    CTelBrandingServerHandler& aServer )
    {
    CTelBrandingBubble* self = new( ELeave ) CTelBrandingBubble( aCallData );
    CleanupStack::PushL( self );
    self->ConstructL( aServer );
    return self;
    }


// ---------------------------------------------------------------------------
// CTelBrandingBubble::~CTelBrandingBubble
// ---------------------------------------------------------------------------
//
CTelBrandingBubble::~CTelBrandingBubble()
    {
    iElements.ResetAndDestroy();
    iElements.Close();
    }

// ---------------------------------------------------------------------------
// CTelBrandingBubble::CreateCallIndicationElementL
// ---------------------------------------------------------------------------
//
void CTelBrandingBubble::CreateCallIndicationElementL( 
    CTelBrandingServerHandler& aServer,
    CTelBubbleCustomElement::TElementType aType )
    {
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
    
    // Call indicator
    aServer.GetBrandIconL( 
        KCallIndicatorImageId,
        bitmap,
        mask );
    
    CEikImage* image = new(ELeave) CEikImage;
    image->SetPicture( bitmap, mask );
    CleanupStack::PushL( image );

    CTelBrandingCallIndicator* indicator = 
        CTelBrandingCallIndicator::NewL( iCallData, image );
    
    CleanupStack::Pop( image );
    CleanupStack::PushL( indicator );
    
    CTelBubbleCustomElement* customElement =
        CTelBubbleCustomElement::NewL( indicator, aType );
    
    CleanupStack::Pop( indicator );
    
    AddElementToArrayL( customElement );
    }

// ---------------------------------------------------------------------------
// CTelBrandingBubble::AddBrandingImageWithPriorityL
// ---------------------------------------------------------------------------
//
void CTelBrandingBubble::AddBrandingImageWithPriorityL( 
    const TDesC8& aImageType,
    CTelBrandingServerHandler& aServer,
    CTelBubbleCustomElement::TElementType aType )
    {
    CTelBrandingImage* image = 
        CTelBrandingImage::NewLC();
    
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
    
    aServer.GetBrandIconL( 
        aImageType,
        bitmap,
        mask );
    
    image->SetPicture( bitmap, mask );
    
    CTelBubbleCustomElement* customElement =
        CTelBubbleCustomElement::NewL( image, aType );
    
    CleanupStack::Pop( image );

    TInt lowerIndex = iElements.Count()-1;
    if( lowerIndex != KErrNotFound )
        {
        // Set previous element to lower priority
        CTelBrandingImage* lowerImage = 
            static_cast<CTelBrandingImage*>( iElements[lowerIndex]->Control() );
        image->SetLowerPriorityBranding( lowerImage );
        }
    
    AddElementToArrayL( customElement );
    }

// ---------------------------------------------------------------------------
// CTelBrandingBubble::AddElementToArrayL
// ---------------------------------------------------------------------------
//
void CTelBrandingBubble::AddElementToArrayL( CTelBubbleCustomElement* aElement )
    {
    CleanupStack::PushL( aElement );

    iElements.AppendL( aElement );

    CleanupStack::Pop( aElement );
    }

// ---------------------------------------------------------------------------
// CTelBrandingBubble::BubbleId
// ---------------------------------------------------------------------------
//
TInt CTelBrandingBubble::BubbleId() const
    {
    return iCallData.BubbleId();
    }

// ---------------------------------------------------------------------------
// CTelBrandingBubble::AttachElements
// ---------------------------------------------------------------------------
//
void CTelBrandingBubble::AttachElements( MTelBubbleExtensionObserver& aObs ) const
    {
    for( TInt i = 0; i < iElements.Count(); i++ )
        {
        aObs.AttachElement(
            BubbleId(), iElements[i] );
        }
    
    }

// ---------------------------------------------------------------------------
// CTelBrandingBubble::DetechElements
// ---------------------------------------------------------------------------
//
void CTelBrandingBubble::DetechElements( MTelBubbleExtensionObserver& aObs ) const
    {
    for( TInt i = 0; i < iElements.Count(); i++ )
        {
        aObs.DetachElement(
            BubbleId(), iElements[i] );
        }
    }

