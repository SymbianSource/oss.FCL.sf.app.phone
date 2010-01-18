/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Reader for JPEG/GIF/BMP images (ImageHandlingLib wrapper)
*
*/


#include <coemain.h>
#include <IHLImageFactory.h>
#include <IHLViewerFactory.h>
#include <MIHLFileImage.h>
#include <MIHLBitmap.h>
#include <MIHLImageViewer.h>
#include <IHLBitmapUtil.h>

#include "BMImageReader.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleImageReader::CBubbleImageReader()                                          
    {
    }


// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CBubbleImageReader::ConstructL( const TDesC& aFileName )
    {
    iSourceImage = IHLImageFactory::OpenFileImageL( 
        CCoeEnv::Static()->FsSession(),
        aFileName );
    }


// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CBubbleImageReader* CBubbleImageReader::NewL(
    const TDesC& aFileName )
    {
    CBubbleImageReader* self = new( ELeave ) CBubbleImageReader();
    CleanupStack::PushL( self );
    self->ConstructL( aFileName );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleImageReader::~CBubbleImageReader()
    {
    delete iViewerEngine;
    delete iDestinationBitmap;
    delete iSourceImage;
    }


// ---------------------------------------------------------------------------
// StartReadingL
// ---------------------------------------------------------------------------
//    
void CBubbleImageReader::StartReadingL(
    const TSize& aTargetSize,
    const TReal& aScaleFactor,
    const TRect& aClipRect, 
    MBubbleMediaReaderObserver* aObserver )
    {
    iObserver = aObserver;
    
    iDestinationBitmap = IHLBitmap::CreateL();
    
    iViewerEngine = IHLViewerFactory::CreateImageViewerL(
       aTargetSize,
       *iSourceImage,
       *iDestinationBitmap,
       *this );
       
    iViewerEngine->SetZoomRatio( aScaleFactor );
    iViewerEngine->SetViewerSize( aTargetSize );
    iViewerEngine->SetSourceRectPosition( aClipRect.iTl );
    }


// ---------------------------------------------------------------------------
// SetScaleAndClip
// ---------------------------------------------------------------------------
//
TInt CBubbleImageReader::SetScaleAndClip(
    const TSize& aTargetSize,
    const TReal& aScaleFactor,
    const TRect& aClipRect )
    {
    TInt err = KErrNone;
    
    if ( iViewerEngine )
        {
        iViewerEngine->SetZoomRatio( aScaleFactor );
        iViewerEngine->SetViewerSize( aTargetSize );
        iViewerEngine->SetSourceRectPosition( aClipRect.iTl );
        }
    else
        {
        err = KErrNotReady;    
        }        
    
    return err;
    }

// ---------------------------------------------------------------------------
// CancelReadingL
// ---------------------------------------------------------------------------
//    
void CBubbleImageReader::CancelReading()
    {
    delete iViewerEngine;
    iViewerEngine = NULL;
    }
    
// ---------------------------------------------------------------------------
// CancelReadingL
// ---------------------------------------------------------------------------
//
const TSize CBubbleImageReader::SourceSize() const 
    { 
    return iSourceImage->Size(); 
    }

// ---------------------------------------------------------------------------
// CancelReadingL
// ---------------------------------------------------------------------------
//
TBool CBubbleImageReader::IsAnimation() const
    { 
    //return iSourceImage->IsAnimation();
    return EFalse; // Not supported. 
    };    

// ---------------------------------------------------------------------------
// Play
// ---------------------------------------------------------------------------
//
void CBubbleImageReader::Play()
    {
    iViewerEngine->Play();
    }

// ---------------------------------------------------------------------------
// Stop
// ---------------------------------------------------------------------------
//
void CBubbleImageReader::Stop()
    {
    iViewerEngine->Stop();
    } 

// ---------------------------------------------------------------------------
// ViewerBitmapChangedL
// ---------------------------------------------------------------------------
//    
void CBubbleImageReader::ViewerBitmapChangedL()
    {
    iFrameBuffer = &iDestinationBitmap->Bitmap();
    iMask = iDestinationBitmap->HasMask() ? &iDestinationBitmap->Mask() :
        NULL;

    HandleReadingComplete();    
    }

// ---------------------------------------------------------------------------
// ViewerError
// ---------------------------------------------------------------------------
//
void CBubbleImageReader::ViewerError( TInt aError )
    {
    HandleReadingError( aError );    
    }

