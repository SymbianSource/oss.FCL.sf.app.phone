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
* Description:  Default animation implementation.
*
*/

#include <AknBitmapAnimation.h>  // Animation definition
#include <eikimage.h>

#include "telbubbleanim.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CBubbleAnim::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleAnim* CTelBubbleAnim::NewL( TInt aFrameInterval )
    {
    CTelBubbleAnim* self = new( ELeave )CTelBubbleAnim();
    CleanupStack::PushL( self );
    self->ConstructL( aFrameInterval );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::~CTelBubbleAnim
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleAnim::~CTelBubbleAnim()
    {
    delete iAnimation;
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::StartAnimation
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleAnim::StartAnimationL()
    {
    iAnimation->StartAnimationL();
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::StopAnimation
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleAnim::StopAnimation()
    {
    iAnimation->CancelAnimation();
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::AddFrameToAnimationLD
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleAnim::AddFrameToAnimationLD( CEikImage* aFrame ) const
    {
    CleanupStack::PushL( aFrame );

    CBitmapFrameData* frame = CBitmapFrameData::NewL(
        MUTABLE_CAST( CFbsBitmap*, aFrame->Bitmap() ),
        MUTABLE_CAST( CFbsBitmap*, aFrame->Mask() ) );
    aFrame->SetPictureOwnedExternally( ETrue );
    CleanupStack::PushL( frame );
    
    frame->SetInterval( KErrNotFound );
    frame->SetBitmapsOwnedExternally( EFalse );
    iAnimation->BitmapAnimData()->AppendFrameL( frame ); // Takes ownership

    CleanupStack::Pop( frame );
    CleanupStack::PopAndDestroy( aFrame );
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::AddFrameToAnimationL
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleAnim::AddFrameToAnimationL( CEikImage& aFrame ) const
    {
    CBitmapFrameData* frame = CBitmapFrameData::NewL(
        MUTABLE_CAST( CFbsBitmap*, aFrame.Bitmap() ),
        MUTABLE_CAST( CFbsBitmap*, aFrame.Mask() ) );
    CleanupStack::PushL( frame );
    
    frame->SetInterval( KErrNotFound );
    frame->SetBitmapsOwnedExternally( ETrue );
    iAnimation->BitmapAnimData()->AppendFrameL( frame ); // Takes ownership

    CleanupStack::Pop( frame );
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::Reset
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleAnim::Reset() const
    {
    iAnimation->BitmapAnimData()->ResetFrameArray();
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::CountComponentControls
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CTelBubbleAnim::CountComponentControls() const
    {
    // do not return animation here!
    return 0;
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::Reset
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleAnim::SetContainerWindowL( const CCoeControl& aContainer )
    {
    CCoeControl::SetContainerWindowL( aContainer );

    iAnimation->SetContainerWindowL( aContainer );
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::Reset
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleAnim::SizeChanged()
    {
    CCoeControl::SizeChanged();
    
    iAnimation->SetRect( Rect() );
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::Draw
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleAnim::Draw( const TRect& /*aRect*/ ) const
    {
    // drawing is done in iAnimation
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::MakeVisible
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleAnim::MakeVisible( TBool aVisible )
    {
    CCoeControl::MakeVisible( aVisible );
    iAnimation->MakeVisible( aVisible );
    
    if( !aVisible )
        {
        StopAnimation();
        }
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::ConstructL
// ---------------------------------------------------------------------------
//
EXPORT_C void CTelBubbleAnim::ConstructL( TInt aFrameInterval )
    {
    iAnimation = CAknBitmapAnimation::NewL();

    iAnimation->MakeVisible( EFalse );
    iAnimation->SetContainerWindowL( *this );
    iAnimation->ActivateL();

    CBitmapAnimClientData* animData = iAnimation->BitmapAnimData();
    animData->SetPlayMode( CBitmapAnimClientData::ECycle );
    animData->SetFrameInterval( aFrameInterval );
    }

// ---------------------------------------------------------------------------
// CTelBubbleAnim::CTelBubbleAnim
// ---------------------------------------------------------------------------
//
EXPORT_C CTelBubbleAnim::CTelBubbleAnim()
    {
    }

// end of file

