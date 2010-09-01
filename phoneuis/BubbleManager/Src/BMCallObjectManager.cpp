/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call object loading.
*
*/



// INCLUDE FILES
#include	<akniconconfig.h>
#include    <AknsUtils.h>
#include    "BMCallObjectManager.h"
#include    "BMUtils.h"
#include    "BMCallObjectUtils.h"
#include    "BMPanic.h"
#include    "BMMediaReaderFactory.h"
#include    "BMMediaReaderInterface.h"
#include    "BMBubbleImageManager.h"

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS
const TInt KBMBubbleIdNone = -2;
const TInt KBMCallTextAlphaValue = 128;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CBubbleCallObjectManager::CBubbleCallObjectManager
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CBubbleCallObjectManager::CBubbleCallObjectManager( 
    CBubbleManager& aBubbleManager ) : iBubbleManager( aBubbleManager )
    {
    }
    
// -----------------------------------------------------------------------------
// CBubbleCallObjectManager::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CBubbleCallObjectManager::ConstructL()
    {
    iIdleProcessor = CIdle::NewL( CActive::EPriorityIdle );
    iLoadingState = EBMLoaderReady;
    iBubbleId = KBMBubbleIdNone; 
    } 

// -----------------------------------------------------------------------------
// CBubbleCallObjectManager::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//    
CBubbleCallObjectManager* CBubbleCallObjectManager::NewL( 
    CBubbleManager& aBubbleManager )
    {
    CBubbleCallObjectManager* self = 
        new( ELeave ) CBubbleCallObjectManager( aBubbleManager );
    
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );

    return self;    
    }
    
// Destructor
CBubbleCallObjectManager::~CBubbleCallObjectManager()
    {
    Reset();

    if ( iIdleProcessor )
        {
        iIdleProcessor->Cancel();            
        delete iIdleProcessor;
        }
    
    delete iCallThemeImageIcon;
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::Reset
//
// ---------------------------------------------------------------------------
//    
void CBubbleCallObjectManager::Reset()
    {
    iLoadingState = EBMLoaderReady;        
    iBubbleId = KBMBubbleIdNone;
    delete iMediaReader;
    iMediaReader = NULL;
    delete iCallText;
    iCallText = NULL;
    delete iCallImage;
    iCallImage = NULL;
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::LoadImageFromFile
//
// ---------------------------------------------------------------------------
//
void CBubbleCallObjectManager::LoadImageFromFile(
    const CBubbleManager::TBubbleId& aBubbleId,
    const TDesC& aFileName,
    const TImageLoadingParams& aParams )
    {
    if ( !aFileName.Length() )
        {
        return;            
        }
    
    if ( iLoadingState == EBMSuspended )
        {
        delete iCallImage;
        iCallImage = aFileName.Alloc();
        iImageLoadParams = aParams;
        iBubbleId = aBubbleId;
        return; // wait for resume
        }
    
    // queueing not supported, keep the existing
    if ( iLoadingState == EBMLoaderReady )
        {
        TRAPD( err, ReadFromFileL( aFileName, 
                                   aParams.iPreferredSize,
                                   aParams.iTinyImageSize ) );
        if ( err == KErrNone )
            {
            iLoadingState = EBMLoadingImage;
            delete iCallImage;
            iCallImage = aFileName.Alloc();
            iImageLoadParams = aParams;
            iBubbleId = aBubbleId;                                
            }    
        }
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::ReadFromFileL
//
// ---------------------------------------------------------------------------
//    
void CBubbleCallObjectManager::ReadFromFileL(
    const TDesC& aFileName,
    const TSize& aPreferredImageSize,
    const TSize& aTinyImageSize )
    {
    delete iMediaReader;
    iMediaReader = NULL;
    iMediaReader = BubbleMediaReaderFactory::CreateReaderL( aFileName );
    
    TSize sourceSize( iMediaReader->SourceSize() );
    
    TReal scaleFactor( 0 );
    TRect clipRect( 0, 0, 0, 0 );
    TSize targetSize( 0, 0 );
    
    // if the caller image (source size) is bigger than aPreferredImageSize (the whole caller-image area)
    // then some down scaling is required.
    if ( sourceSize.iHeight > aPreferredImageSize.iHeight && sourceSize.iWidth > aPreferredImageSize.iWidth )
        {
        targetSize = aPreferredImageSize;
        
        BubbleCallObjectUtils::GetScaleFactorAndClipRect(
             sourceSize,
             aPreferredImageSize,
             BubbleCallObjectUtils::EFillTarget,
             scaleFactor,
             clipRect ); 
        }
    else // no scaling. wide or tall images gets cropped from center if required.
        {
        targetSize = aPreferredImageSize;
        scaleFactor = 1;
        TInt x_offset = 0;
        TInt y_offset = 0;

        if ( sourceSize.iWidth > aPreferredImageSize.iWidth )
            {
            x_offset = ( sourceSize.iWidth - aPreferredImageSize.iWidth ) / 2;
            }
        if ( sourceSize.iHeight > aPreferredImageSize.iHeight )
            {
            y_offset = ( sourceSize.iHeight - aPreferredImageSize.iHeight ) / 2; 
            }

        clipRect = sourceSize;
                
        if ( x_offset > 0 || y_offset > 0 )
            {
            // clip from center of the source image
            clipRect.Shrink( x_offset, y_offset );
            }
        }

    iMediaReader->StartReadingL( targetSize, 
                                 scaleFactor, 
                                 clipRect, 
                                 this );   
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::LoadImageFromText
//
// ---------------------------------------------------------------------------
//    
void CBubbleCallObjectManager::LoadImageFromText(
    const CBubbleManager::TBubbleId& aBubbleId,
    const TDesC& aText,
    const TImageLoadingParams& aParams )
    {
    if ( !aText.Length() )
        {
        return;            
        }
        
    if ( iLoadingState == EBMSuspended )
        {
        delete iCallText;
        iCallText = aText.Alloc();    
        iImageLoadParams = aParams;
        iBubbleId = aBubbleId;
        return; // wait for resume
        }
    
    // queueing not supported, keep the existing
    if ( iLoadingState == EBMLoaderReady )    
        {
        delete iCallText;
        iCallText = aText.Alloc();        
        iBubbleId = aBubbleId;
        iImageLoadParams = aParams;
        TCallBack idleCallback( IdleProcessorCallback, this );
        iIdleProcessor->Cancel();
        iIdleProcessor->Start( idleCallback );
        iLoadingState = EBMLoadingText;    
        }
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::CancelCallObjectLoading
//
// ---------------------------------------------------------------------------
//    
void CBubbleCallObjectManager::CancelCallObjectLoading( 
    const CBubbleManager::TBubbleId& aBubbleId )
    {
    if ( iLoadingState != EBMLoaderReady &&
         iBubbleId == aBubbleId )    
        {
        Reset();
        iIdleProcessor->Cancel();
        }
    }
    
// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::Suspend
//
// ---------------------------------------------------------------------------
//    
void CBubbleCallObjectManager::Suspend()
    {
    if ( iLoadingState != EBMLoaderReady )
        {
        // Cancel image/text reading
        delete iMediaReader;
        iMediaReader = NULL;
        iIdleProcessor->Cancel();
        }

    // Set suspended state
    iLoadingState = EBMSuspended;
    }
    
// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::Resume
//
// ---------------------------------------------------------------------------
//    
TInt CBubbleCallObjectManager::Resume()
    {
    TInt err = KErrNone;
    
    if ( iLoadingState == EBMSuspended )
        {
        TRAP( err, DoResumeL() );            
        }
        
    return err;        
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::DoResumeL
//
// ---------------------------------------------------------------------------
//    
void CBubbleCallObjectManager::DoResumeL()
    {
    if ( iCallImage )    
        {
        CBubbleManager::TPhoneCallState callState =
                iBubbleManager.CallState( iBubbleId );
        
        if ( callState < CBubbleManager::EIncoming &&
             iImageLoadParams.iThumbnailSize != TSize(0,0) )
            {
            // switch to incall image size
            iImageLoadParams.iPreferredSize = 
                iImageLoadParams.iThumbnailSize;
            iImageLoadParams.iThumbnailSize = TSize(0,0);
            iImageLoadParams.iTinyImageSize = TSize(0,0);                   
            }
        
        ReadFromFileL( *iCallImage, 
                       iImageLoadParams.iPreferredSize,
                       iImageLoadParams.iTinyImageSize );
        
        iLoadingState = EBMLoadingImage;
        delete iCallImage;
        iCallImage = NULL;    
        }
    else if ( iCallText )        
        {
        TCallBack idleCallback( IdleProcessorCallback, this );
        iIdleProcessor->Cancel();
        iIdleProcessor->Start( idleCallback );
        iLoadingState = EBMLoadingText;    
        }
    else
        {
        Reset();    
        }        
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::LoadCallThemeImage
//
// ---------------------------------------------------------------------------
//            
void CBubbleCallObjectManager::LoadCallThemeImage(
    CBubbleImageManager& aImageManager,    
    const TSize& aSize )
    {
    delete iCallThemeImageIcon;
    iCallThemeImageIcon = NULL;
    
    TRAPD( err, AllocateCallThemeImageIconL( aImageManager ) );
    if ( !err )
        {
        iCallThemeImageSize = aSize;
        
        if ( iLoadingState == EBMLoaderReady )    
            {
            TCallBack idleCallback( IdleProcessorCallback, this );
            iIdleProcessor->Cancel();
            iIdleProcessor->Start( idleCallback );
            iLoadingState = EBMInitializingIcon;
            }
        }
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::AllocateCallThemeImageIconL
//
// ---------------------------------------------------------------------------
//
void CBubbleCallObjectManager::AllocateCallThemeImageIconL( 
        CBubbleImageManager& aImageManager )
    {
    CEikImage* themeImage = new( ELeave ) CEikImage;
    CleanupStack::PushL( themeImage );
    aImageManager.SetBitmapToImage( themeImage,
                                    EQgn_graf_call_image_1,
                                    EQgn_graf_call_image_1_mask );

    if ( themeImage->Bitmap() )
        {
        iCallThemeImageIcon = CGulIcon::NewL( 
                    const_cast<CFbsBitmap*> (themeImage->Bitmap()), 
                    const_cast<CFbsBitmap*> (themeImage->Mask()));
        }
    themeImage->SetPictureOwnedExternally( ETrue );
    CleanupStack::PopAndDestroy( themeImage );
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::ReleaseCallThemeImage
//
// ---------------------------------------------------------------------------
//
void CBubbleCallObjectManager::ReleaseCallThemeImage()
    {
    delete iCallThemeImageIcon;
    iCallThemeImageIcon = NULL;
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::ResizeCallThemeImage
//
// ---------------------------------------------------------------------------
//
void CBubbleCallObjectManager::ResizeCallThemeImage( const TSize& aSize )
    {
    iCallThemeImageSize = aSize;
    AknIconUtils::SetSize( iCallThemeImageIcon->Bitmap(),
                           iCallThemeImageSize,
                           EAspectRatioPreservedSlice );
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::CallThemeImageIcon
//
// ---------------------------------------------------------------------------
//
CGulIcon* CBubbleCallObjectManager::CallThemeImageIcon()
    {
    return iCallThemeImageIcon;
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::IdleProcessorCallback
//
// ---------------------------------------------------------------------------
//     
TInt CBubbleCallObjectManager::IdleProcessorCallback( TAny* aThis )
    {
    __ASSERT_ALWAYS( aThis, 
                     Panic( EBMPanicCallObjectManager ) );
                     
    CBubbleCallObjectManager* self = 
            static_cast<CBubbleCallObjectManager*>( aThis );
    
    TLoadingState state = self->iLoadingState;
    
    if ( state == EBMLoadingText )
        {
        TRAP_IGNORE( self->DoLoadImageInIdleL() );    
        }
    else if ( state == EBMInitializingIcon )
        {
        self->DoInitializeIcons();
        }
    else if ( state == EBMDeleting )        
        {
        self->DoDeleteReaderInIdle();
        }
    
    return KErrNone;        
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::DoLoadImageInIdleL
//
// ---------------------------------------------------------------------------
//
void CBubbleCallObjectManager::DoLoadImageInIdleL()
    {
    if ( iLoadingState == EBMLoadingText && iCallText )
        {
        AknIconConfig::TPreferredDisplayMode mode;
        AknIconConfig::PreferredDisplayMode( mode, 
                                             AknIconConfig::EImageTypeIcon);
        
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        TRgb skinColor;
        User::LeaveIfError( AknsUtils::GetCachedColor( 
                            skin, 
                            skinColor, 
                            KAknsIIDQsnTextColors,
                            EAknsCIQsnTextColorsCG51 ) );            
        
        CFbsBitmap* bitmap = NULL;
        CFbsBitmap* mask = NULL;
        
        BubbleCallObjectUtils::CreateImageBitmapsFromTextLC( 
            *iCallText, 
            iBubbleManager.Rect(),
            iImageLoadParams.iPreferredSize,
            skinColor,  // text color
            KBMCallTextAlphaValue, // semitransparent
            mode.iBitmapMode,
            bitmap,
            mask );
        CleanupStack::Pop(2); // bitmap, mask
                
        iBubbleManager.StartChanges();
        iBubbleManager.SetCallObjectImage( iBubbleId,
                                           bitmap,
                                           mask,
                                           ETrue );
        iBubbleManager.EndChanges();
            
        delete iCallText;
        iCallText = NULL;                
        }
    
    iLoadingState = EBMLoaderReady;        
    iBubbleId = KBMBubbleIdNone;
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::DoDeleteReaderInIdle
//
// ---------------------------------------------------------------------------
//    
void CBubbleCallObjectManager::DoDeleteReaderInIdle()
    {
    Reset();
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::DoInitializeIcons
//
// ---------------------------------------------------------------------------
//
void CBubbleCallObjectManager::DoInitializeIcons()
    {
    AknIconUtils::ExcludeFromCache( iCallThemeImageIcon->Bitmap() );
    AknIconUtils::SetSize( iCallThemeImageIcon->Bitmap(),
                           iCallThemeImageSize,
                           EAspectRatioPreservedSlice );
    iLoadingState = EBMLoaderReady;
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::FrameBufferDataChanged
//
// ---------------------------------------------------------------------------
//    
void CBubbleCallObjectManager::FrameBufferDataChanged()
    {
    TRAPD( err, DoHandleFrameBufferDataL() );
    
    if ( err != KErrNone )
        {
        TCallBack idleCallback( IdleProcessorCallback, this );
        iIdleProcessor->Cancel();
        iIdleProcessor->Start( idleCallback );
        iLoadingState = EBMDeleting;    
        }
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::SetCallObjectImageL
//
// ---------------------------------------------------------------------------
//     
void CBubbleCallObjectManager::DoHandleFrameBufferDataL()
    {
    const CFbsBitmap* bitmap = iMediaReader->FrameBuffer();
    const CFbsBitmap* mask = iMediaReader->Mask();
    
    if ( bitmap )
        {
        CBubbleManager::TPhoneCallState callState =
            iBubbleManager.CallState( iBubbleId );

        // copy the current frame
        CFbsBitmap* d_bitmap = new( ELeave ) CFbsBitmap;
        CleanupStack::PushL( d_bitmap );
        User::LeaveIfError( d_bitmap->Duplicate( bitmap->Handle() ) );
            
        CFbsBitmap* d_mask = NULL;
        if ( mask )
            {
            d_mask = new( ELeave ) CFbsBitmap;
            CleanupStack::PushL( d_mask );
            User::LeaveIfError( d_mask->Duplicate( mask->Handle() ) );    
            CleanupStack::Pop( d_mask );
            }
        CleanupStack::Pop( d_bitmap );
            
        // assign the copy to call header.
        if ( ( iLoadingState == EBMLoadingThumbnail ) &&
             ( callState > CBubbleManager::EActive ) )
            {
            // displaying fullscreen image, set as thumbnail
            iBubbleManager.SetThumbnail( iBubbleId,
                                         d_bitmap,
                                         d_mask,
                                         ETrue ); // Ownership transfer
            }
        else
            {
            iBubbleManager.StartChanges();
            iBubbleManager.SetCallObjectImage( iBubbleId,
                                               d_bitmap,
                                               d_mask,
                                               ETrue ); // Ownership transfer
            iBubbleManager.EndChanges();
            }            
   
        }
    
    // Delete media reader
    iBubbleId = KBMBubbleIdNone;
    TCallBack idleCallback( IdleProcessorCallback, this );
    iIdleProcessor->Cancel();
    iIdleProcessor->Start( idleCallback );
    iLoadingState = EBMDeleting;         
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::LoadThumbnailVersion
//
// ---------------------------------------------------------------------------
//
TBool CBubbleCallObjectManager::LoadThumbnailVersion()
    {
    TSize sourceSize( iMediaReader->SourceSize() );
    TReal scaleFactor;
    TRect clipRect;
    TBool isTiny = BubbleCallObjectUtils::GetScaleFactorAndClipRect(
        sourceSize,
        iImageLoadParams.iThumbnailSize,
        BubbleCallObjectUtils::EFillTarget,
        scaleFactor,
        clipRect );
            
    if ( !isTiny )
        {
        // scale image to thumbnail version
        iMediaReader->SetScaleAndClip( iImageLoadParams.iThumbnailSize, 
                                       scaleFactor,
                                       clipRect );
        return ETrue;            
        }
    else
        {
        // image is smaller than thumnail size
        return EFalse;            
        }                    
    }

// ---------------------------------------------------------------------------
// CBubbleCallObjectManager::ReaderError
//
// ---------------------------------------------------------------------------
// 
void CBubbleCallObjectManager::ReaderError( TInt /*aError*/ )
    {
    TCallBack idleCallback( IdleProcessorCallback, this );
    iIdleProcessor->Cancel();
    iIdleProcessor->Start( idleCallback );
    iLoadingState = EBMDeleting;
    }    

//  End of File  
