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

#define SCALE_FACTOR 1.5

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleMainPaneControl::CBubbleMainPaneControl( 
    CBubbleManager& aBubbleManager,  
    CBubbleCallObjectManager& aCallObjectManager ) : 
    iBubbleManager( aBubbleManager ), 
    iCallObjectManager ( aCallObjectManager ),
    iScaler( NULL ),
    iScaledImage( NULL )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CBubbleMainPaneControl::ConstructL()
    {
    iScaler = CTelBubbleImageScaler::NewL( *this );
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
    delete iScaler;
    iScaler = NULL;
    
    delete iScaledImage;
    iScaledImage = NULL;	
    }

// ---------------------------------------------------------------------------
// ReadBubbleHeader
// ---------------------------------------------------------------------------
//
void CBubbleMainPaneControl::ReadBubbleHeader( 
    const CBubbleHeader& aHeader )
    {
    iBubble = aHeader.BubbleId();
    TBool thumbnail( EFalse );
    TBool noScaling( EFalse );
    
    if ( aHeader.CallObjectImage() )
        {
        iBitmap = aHeader.CallObjectImage();
        iMask = aHeader.CallObjectImageMask();
        }
    else if ( aHeader.TnBitmap() )
        {
        iBitmap = aHeader.TnBitmap();
        iMask = aHeader.TnBitmapMask();
        thumbnail = ETrue;
        }
    else
        {
        iBitmap = NULL;
        }
    
    iThreeLinedBubble = ( aHeader.CNAP().Length() > 0 );
    
    // PrepareBitmapsL() needs to know if a brand new thumbnail is about to get prepared.
    if ( thumbnail && (iOldBitmap != NULL) && (iOldBitmap == iBitmap) && (iScaler->GetState() != CTelBubbleImageScaler::EScalingStarted) )
        {
        noScaling = ETrue;
        }
    
    if ( iBitmap )
        {
        iOldBitmap = iBitmap;
        
        CFbsBitmap* bitmap( NULL );
        CFbsBitmap* mask( NULL );
        TBool bitmapOwnership( EFalse );
        TBool maskOwnership( EFalse );
        
        TRAPD(err, PrepareBitmapsL( bitmap, 
                                    bitmapOwnership, 
                                    mask, 
                                    maskOwnership,
                                    aHeader.CallObjectImageType() == 
                                    CBubbleHeader::EThemeImage,
                                    noScaling ));
        
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
        
        // upscale thumbnails if loading it for the first time.
        if ( thumbnail && !noScaling && ( iScaler->GetState() != CTelBubbleImageScaler::EScalingStarted ) )
            {
            StartScaling( iBitmap );
            }
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
    TBool aIsScalable,
    TBool aThumbnail )
    {
    if ( aThumbnail )
        {
        aBitmap = iScaledImage;
        }
    else
        {
        aBitmap = iBitmap;
        }

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

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::ImageScalingComplete
//
// ---------------------------------------------------------------------------
//
void CBubbleMainPaneControl::ImageScalingComplete( TInt aError,
        CFbsBitmap* aBitmap )
    {
    if ( aError == KErrNone )
        {
        // Draw the scaled image
        iScaledImage = aBitmap;
        iScaler->SetState( CTelBubbleImageScaler::EScalingDone );
        
        CFbsBitmap* bitmap( NULL );
        CFbsBitmap* mask( NULL );
        TBool bitmapOwnership( EFalse );
        TBool maskOwnership( EFalse );
      
        TRAPD(err, PrepareBitmapsL( bitmap, 
                                    bitmapOwnership, 
                                    mask, 
                                    maskOwnership,
                                    EFalse,
                                    ETrue ) );
        
        if ( err )
            {
            return;
            }
                
        if ( iCallImage->ControlType() == 
             CTelBubbleCustomElement::EBubbleCallImage )
            {
            CTelBubbleCallImage* image = 
                static_cast<CTelBubbleCallImage*> ( iCallImage->Control() );
            image->SetImage( bitmap, EFalse, mask, maskOwnership );
            } 

        SizeChanged();
        
        iIsUsed = ETrue;
        MakeVisible( ETrue );
        
        Parent()->DrawNow(); // refreshing view with scaled image
        }
    else if ( aError == KErrCancel )
        {
        // error is ignored
        // This error is returned when a previous scaling is cancelled
        // while new scaling request is given to active object
        }
    else
        {
        // draw the unscaled image in error cases
    
        delete iScaledImage;
        iScaledImage = NULL;
        }
    
    iScaler->SetState( CTelBubbleImageScaler::EScalingDone );
    }

// ---------------------------------------------------------------------------
// CTelBubbleCallImage::StartScaling
//
// ---------------------------------------------------------------------------
//
void CBubbleMainPaneControl::StartScaling( CFbsBitmap *aSourceBitmap )
    {
    if ( !iScaler )
        {
        return;
        }
    
    iScaler->Cancel(); // cancels any outstanding scaling requests
    
    delete iScaledImage;
    iScaledImage = NULL;

    // create a bitmap for scaled size
    TRAPD( err, iScaledImage = new (ELeave) CFbsBitmap );
    if ( err != KErrNone )
        {
        // no scaling in error cases
        iScaler->SetState( CTelBubbleImageScaler::EScalingIdle );
        return;
        }

    TSize size = aSourceBitmap->SizeInPixels();
    TRect scaledRect( 0, 0, size.iWidth * SCALE_FACTOR, size.iHeight * SCALE_FACTOR );
    
    TRAP( err, iScaledImage->Create( scaledRect.Size(), aSourceBitmap->DisplayMode() ) );
    if ( err != KErrNone )
        {
        delete iScaledImage;
        iScaledImage = NULL;
        iScaler->SetState( CTelBubbleImageScaler::EScalingIdle );
        }
    else
        {
        //start scaling
        TRAPD( err, iScaler->StartScaleL( aSourceBitmap, iScaledImage ) );
        if (err != KErrNone)
            {
            iScaler->SetState( CTelBubbleImageScaler::EScalingIdle );
            delete iScaledImage;
            iScaledImage = NULL;
            }
        }
    }
	
// END OF FILE

