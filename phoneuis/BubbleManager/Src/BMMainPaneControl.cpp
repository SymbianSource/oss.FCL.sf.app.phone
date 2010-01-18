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
* Description:  Call status indicator
*
*/


#include "BMBubbleManager.h"
#include "BMMainPaneControl.h"
#include "BMCustomManager.h"
#include "BMBubbleHeader.h"
#include "BMUtils.h"
#include "BMCallObjectManager.h"
#include "BMBubbleImageManager.h"

#include <telbubblecallimage.h>
#include <AknIconUtils.h>
#include <layoutmetadata.cdl.h>

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleMainPaneControl::CBubbleMainPaneControl( 
    CBubbleManager& aBubbleManager,  
    CBubbleCallObjectManager& aCallObjectManager ) : 
    iBubbleManager( aBubbleManager ), 
    iCallObjectManager ( aCallObjectManager )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CBubbleMainPaneControl::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CBubbleMainPaneControl* CBubbleMainPaneControl::NewL(
    CBubbleManager& aBubbleManager,
    CBubbleCallObjectManager& aCallObjectManager )
    {
    CBubbleMainPaneControl* self = 
        new( ELeave ) CBubbleMainPaneControl( aBubbleManager, 
                                               aCallObjectManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleMainPaneControl::~CBubbleMainPaneControl()
    {
    }

// ---------------------------------------------------------------------------
// ReadBubbleHeader
// ---------------------------------------------------------------------------
//
void CBubbleMainPaneControl::ReadBubbleHeader( 
    const CBubbleHeader& aHeader )
    {
    iBubble = aHeader.BubbleId();
    iBitmap = aHeader.CallObjectImage();
    iMask = aHeader.CallObjectImageMask();
    iThreeLinedBubble = ( aHeader.CNAP().Length() > 0 );
    
    if ( iBitmap )
        {
        CFbsBitmap* bitmap;
        CFbsBitmap* mask;
        TBool bitmapOwnership;
        TBool maskOwnership;
        
        TRAPD(err, PrepareBitmapsL( bitmap, 
                                    bitmapOwnership, 
                                    mask, 
                                    maskOwnership,
                                    aHeader.CallObjectImageType() == 
                                    CBubbleHeader::EThemeImage ) );
        
        if ( err )
            {
            return;
            }

        iCallImage = iBubbleManager.CustomManager().ReserveCustomElement(
                aHeader, CTelBubbleCustomElement::ECallImage );
        SizeChanged();
        
        if ( iCallImage->ControlType() == 
             CTelBubbleCustomElement::EBubbleCallImage )
            {
            CTelBubbleCallImage* image = 
                static_cast<CTelBubbleCallImage*> ( iCallImage->Control() );
            image->SetImage( bitmap, EFalse, mask, maskOwnership );
            } 
        
        iIsUsed = ETrue;
        MakeVisible( ETrue );
        }
    }

// ---------------------------------------------------------------------------
// PrepareBitmapsL
// ---------------------------------------------------------------------------
//
void CBubbleMainPaneControl::PrepareBitmapsL( 
    CFbsBitmap*& aBitmap,
    TBool& aBitmapOwnership, 
    CFbsBitmap*& aMask,
    TBool& aMaskOwnership,
    TBool aIsScalable )
    {
    aBitmap = iBitmap;
    aMask = iMask;
    aBitmapOwnership = EFalse;
    aMaskOwnership = EFalse;
    
    if ( aIsScalable )
        {
        // Switch to preloaded image when available
        if ( iCallObjectManager.CallThemeImageIcon() )
            {
            aBitmap = iCallObjectManager.CallThemeImageIcon()->Bitmap();
            aMask = iCallObjectManager.CallThemeImageIcon()->Mask();
            }
    
        // Secure that SVG image is initialized
        User::LeaveIfError( AknIconUtils::SetSize( aBitmap,
             iBubbleManager.MainPaneImageRect().Size(),
             EAspectRatioPreservedSlice ) != KErrNone );
        }
    
    TSize imageSize = aBitmap->SizeInPixels();
    TSize tinyImageSize( iBubbleManager.MainPaneImageRect( ETrue ).Size() );
    TBool fullScreen = !( ( imageSize.iWidth <= tinyImageSize.iWidth ) &&
                          ( imageSize.iHeight <= tinyImageSize.iHeight ) );  
    
    if ( iBubbleManager.IsTouchCallHandling() &&
         fullScreen )
        {
        // Attach display mask to image.
        CEikImage* displayMask = new( ELeave ) CEikImage;
        CleanupStack::PushL( displayMask ); 
    
        TBMIcons maskId = 
            Layout_Meta_Data::IsLandscapeOrientation() ?
            EQgn_indi_call4_image_mask_lsc :
            EQgn_indi_call4_image_mask_prt;
    
        iBubbleManager.ImageManager().SetBitmapToImage( 
            displayMask,
            maskId );
    
        User::LeaveIfError( AknIconUtils::SetSize( 
            const_cast<CFbsBitmap*> (displayMask->Bitmap()),
            imageSize,
            EAspectRatioNotPreserved ) );
            
        if ( aMask )
            {
            // Combine display mask and image mask.
            CFbsBitmap* combinedMask = 
                BubbleUtils::CreateCombinedMaskL(
                    displayMask->Bitmap(),
                    aMask );        
            aMask = combinedMask;
            }
        else
            {
            aMask = 
                const_cast<CFbsBitmap*> ( displayMask->Bitmap());
            displayMask->SetPictureOwnedExternally( ETrue );
            }                        
            
        aMaskOwnership = ETrue;    
        CleanupStack::PopAndDestroy( displayMask );
        }    
    }

// ---------------------------------------------------------------------------
// Reset
// ---------------------------------------------------------------------------
//
void CBubbleMainPaneControl::Reset()
    {
    if ( iIsUsed )
        {
        iIsUsed = EFalse;
        MakeVisible( EFalse );
        iBubble = KErrNotFound;
        
        if ( iCallImage && 
             iCallImage->ControlType() == 
             CTelBubbleCustomElement::EBubbleCallImage )
            {
            CTelBubbleCallImage* image = 
                static_cast<CTelBubbleCallImage*> ( iCallImage->Control() );
            image->SetImage( NULL, EFalse, NULL, EFalse );
            }
        
        iBubbleManager.CustomManager().ReleaseCustomElement( iCallImage );
        }
    }

// ---------------------------------------------------------------------------
// DrawBitmaps
// ---------------------------------------------------------------------------
//
void CBubbleMainPaneControl::DrawBitmaps( const TRect& aRect ) const
    {
    if ( iCallImage )
        {
        // ToDo: propagate to components
        iCallImage->Control()->DrawForeground( aRect );
        }
    }

// ---------------------------------------------------------------------------
// BubbleId
// ---------------------------------------------------------------------------
//
CBubbleManager::TBubbleId CBubbleMainPaneControl::BubbleId() const
    {
    return iBubble;
    }

// ---------------------------------------------------------------------------
// IsUsed
// ---------------------------------------------------------------------------
//
TBool CBubbleMainPaneControl::IsUsed() const
    {
    return iIsUsed;
    }

// ---------------------------------------------------------------------------
// SizeChanged
// ---------------------------------------------------------------------------
//
void CBubbleMainPaneControl::SizeChanged()
    {
    if ( iCallImage )
        {
        iCallImage->Control()->SetRect( Rect() );
        if ( iCallImage->ControlType() == 
             CTelBubbleCustomElement::EBubbleCallImage )
            {
            CTelBubbleCallImage* image = 
                static_cast<CTelBubbleCallImage*> ( iCallImage->Control() );
            TRect imageArea( iBubbleManager.MainPaneImageRect( 
                                 ETrue, iThreeLinedBubble ) );
            
            image->SetImagePlacingArea( imageArea );
            }
        }
    }

// END OF FILE

