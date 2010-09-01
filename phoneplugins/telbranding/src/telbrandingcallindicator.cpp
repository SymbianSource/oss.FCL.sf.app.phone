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
* Description:  Branding specific call indication.
*
*/

#include <telbubbleextensiondata.h>
#include <AknsUtils.h>
#include <eikimage.h>
#include <callstatus.mbg> // From bubblemanager
#include <aknconsts.h>
#include <AknBitmapAnimation.h>  // Animation definition
#include <AknUtils.h>

#include "telbrandingcallindicator.h"


// ======== LOCAL FUNCTIONS ========
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::CTelBrandingCallIndicator
// ---------------------------------------------------------------------------
//
CTelBrandingCallIndicator::CTelBrandingCallIndicator( 
    MTelBubbleExtensionData& aCallData, CEikImage* aBrand ) :
    iCallData( aCallData ),
    iBrand( aBrand ),
    iCallIndicator( NULL )
    {
    }


// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::ConstructL
// ---------------------------------------------------------------------------
//
void CTelBrandingCallIndicator::ConstructL()
    {
    CTelBubbleAnim::ConstructL( 500 );
    }


// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::NewL
// ---------------------------------------------------------------------------
//
CTelBrandingCallIndicator* CTelBrandingCallIndicator::NewL(
    MTelBubbleExtensionData& aCallData, CEikImage* aBrand )
    {
    CTelBrandingCallIndicator* self = 
        CTelBrandingCallIndicator::NewLC( aCallData, aBrand );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::NewLC
// ---------------------------------------------------------------------------
//
CTelBrandingCallIndicator* CTelBrandingCallIndicator::NewLC(
    MTelBubbleExtensionData& aCallData, CEikImage* aBrand )
    {
    CTelBrandingCallIndicator* self = 
        new( ELeave ) CTelBrandingCallIndicator( aCallData, aBrand );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::~CTelBrandingCallIndicator
// ---------------------------------------------------------------------------
//
CTelBrandingCallIndicator::~CTelBrandingCallIndicator()
    {
    delete iBrand;
    delete iCallIndicator;
    }

// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::MakeVisible
// ---------------------------------------------------------------------------
//
void CTelBrandingCallIndicator::MakeVisible( TBool aVisible )
    {
    CTelBubbleAnim::MakeVisible( aVisible );

    if( aVisible )
        {
        TRAP_IGNORE( DoVisibleL() );
        }
    else
        {
        StopAnimation();
        }
    }

// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::Draw
// ---------------------------------------------------------------------------
//
void CTelBrandingCallIndicator::Draw( const TRect& /*aRect*/ ) const
    {
    CWindowGc& gc = SystemGc();
    
    // Position is botton right corner minus image size
    TPoint brandPlace = Rect().iBr - iBrand->MinimumSize();
    TPoint callIndPlace = Rect().iTl;

    if( AknLayoutUtils::LayoutMirrored() )
        {
        // Fix position when arabic variant is in use
        brandPlace.iX = Rect().iTl.iX;
        
        if( iCallIndicator )
            {
            callIndPlace.iX = Rect().iBr.iX - iCallIndicator->MinimumSize().iWidth;
            }
        
        }
    
    gc.BitBltMasked( brandPlace,
                     iBrand->Bitmap(),
                     iBrand->MinimumSize(),
                     iBrand->Mask(),
                     ETrue );
    
    if( iCallIndicator )
        {
        gc.BitBltMasked( callIndPlace,
            iCallIndicator->Bitmap(),
            iCallIndicator->MinimumSize(),
            iCallIndicator->Mask(),
            ETrue );
        }
    }

// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::SizeChanged
// ---------------------------------------------------------------------------
//
void CTelBrandingCallIndicator::SizeChanged()
    {
    CCoeControl::SizeChanged();
    TPoint place = Rect().iTl;
    
    TSize imageSize = Rect().Size();
    imageSize.iHeight /= 1.5;
    imageSize.iWidth /= 1.5;

    if( AknLayoutUtils::LayoutMirrored() )
        {
        // Fix position when arabic variant is in use
        place.iX = Rect().iBr.iX - imageSize.iWidth;
        }
    
    iAnimation->SetRect( TRect( place, imageSize ) );
    
    if ( iBrand->Bitmap() )
        {
        AknIconUtils::SetSize( 
            const_cast<CFbsBitmap*> ( iBrand->Bitmap() ), 
            imageSize );
        }
    
    if ( iCallIndicator )
        {
        AknIconUtils::SetSize( 
            const_cast<CFbsBitmap*> ( iCallIndicator->Bitmap() ), 
            imageSize );
        }
    
    }

// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::DoVisibleL
// ---------------------------------------------------------------------------
//
void CTelBrandingCallIndicator::DoVisibleL()
    {
    Reset();
    delete iCallIndicator;
    iCallIndicator = NULL;
    
    switch( iCallData.State() )
        {
        case MTelBubbleExtensionData::EOnHold:
            // Show icon
            iCallIndicator = GetImageL( 
                KAknsIIDQgnIndiCallHeld,
                EMbmCallstatusQgn_indi_call_held,
                EMbmCallstatusQgn_indi_call_held_mask );
            break;
        case MTelBubbleExtensionData::EAlertToDisconnected:
        case MTelBubbleExtensionData::EDisconnected:
            // Show icon
            iCallIndicator = GetImageL( 
                KAknsIIDQgnIndiCallDisconn,
                EMbmCallstatusQgn_indi_call_disconn,
                EMbmCallstatusQgn_indi_call_disconn_mask );
            break;
        case MTelBubbleExtensionData::EOutgoing:
        case MTelBubbleExtensionData::EActive:
            // Show icon
            iCallIndicator = GetImageL( 
                KAknsIIDQgnIndiCallActive,
                EMbmCallstatusQgn_indi_call_active,
                EMbmCallstatusQgn_indi_call_active_mask );
            break;
        case MTelBubbleExtensionData::EIncoming:
        case MTelBubbleExtensionData::EWaiting:
        case MTelBubbleExtensionData::EAlerting:
            // frame 1
            AddBrandedFrameToAnimationL( 
                KAknsIIDQgnIndiCallActive,
                EMbmCallstatusQgn_indi_call_active,
                EMbmCallstatusQgn_indi_call_active_mask );
            
            // frame 2
            AddBrandedFrameToAnimationL( 
                KAknsIIDQgnIndiCallActive2,
                EMbmCallstatusQgn_indi_call_active_2,
                EMbmCallstatusQgn_indi_call_active_2_mask );
            break;
            
        case MTelBubbleExtensionData::ENone:
        default:
            // Do nothing
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::ShowAnimationL
// ---------------------------------------------------------------------------
//
void CTelBrandingCallIndicator::ShowAnimationL()
    {


    }

// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::ShowImageL
// ---------------------------------------------------------------------------
//
void CTelBrandingCallIndicator::ShowImageL()
    {

    }

// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::AddBrandedFrameToAnimationL
// ---------------------------------------------------------------------------
//
void CTelBrandingCallIndicator::AddBrandedFrameToAnimationL( 
    TAknsItemID aSkinId, TInt aBitmap, TInt aMask )
    {
    CEikImage* image = GetImageL( aSkinId, aBitmap, aMask );
    AddFrameToAnimationLD( image );
    }

// ---------------------------------------------------------------------------
// CTelBrandingCallIndicator::GetImageL
// ---------------------------------------------------------------------------
//
CEikImage* CTelBrandingCallIndicator::GetImageL(
    TAknsItemID aSkinId, 
    TInt aBitmap, 
    TInt aMask )
    {
    CFbsBitmap* bitmap;
    CFbsBitmap* mask;
    
    CEikImage* image = new(ELeave) CEikImage;
    CleanupStack::PushL( image );
        
    AknsUtils::CreateIconL( 
        AknsUtils::SkinInstance(),
        aSkinId,
        bitmap,
        mask,
        KCallStatusBitmapFile,
        aBitmap,
        aMask
        );

    image->SetPicture( bitmap, mask );
    
    CleanupStack::Pop( image );
    return image;
    }

// End of file
