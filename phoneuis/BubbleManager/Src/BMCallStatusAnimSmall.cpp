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
* Description:  Small call status animation
*
*/

#include "BMCallStatusAnimSmall.h"
#include "BMBubbleImageManager.h"
#include "BMBubbleHeader.h"

#include <eikimage.h>

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleCallStatusAnimSmall::CBubbleCallStatusAnimSmall( 
    CBubbleImageManager& aImageManager ) :
    CBubbleCallStatusAnim( aImageManager )
    {
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CBubbleCallStatusAnimSmall* CBubbleCallStatusAnimSmall::NewL(
        CBubbleImageManager& aImageManager )
    {
    CBubbleCallStatusAnimSmall* self = 
        new( ELeave ) CBubbleCallStatusAnimSmall( aImageManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleCallStatusAnimSmall::~CBubbleCallStatusAnimSmall()
    {
    }

// ---------------------------------------------------------------------------
// SetAnimationContentL
// ---------------------------------------------------------------------------
//
void CBubbleCallStatusAnimSmall::SetAnimationContentL()
    {
    TInt bitmap;  
    TInt mask;
    
    CEikImage* image = new(ELeave) CEikImage;
    CleanupStack::PushL( image );
    
    // frame 1
    if ( iCallFlags & CBubbleManager::EVideo )
        {
        bitmap = EQgn_indi_call_video_callsta_1;
        mask  = EQgn_indi_call_video_callsta_1_mask;
        }
    else if ( iCallFlags & CBubbleManager::EVoIPCall )
        {
        bitmap = EQgn_indi_call_voip_active;
        mask  = EQgn_indi_call_voip_active_mask;
        }
    else
        {
        bitmap = EQgn_indi_call_active;
        mask  = EQgn_indi_call_active_mask;
        }
    
    iImageManager.SetBitmapToImage(
        image,
        (TBMIcons)bitmap,
        (TBMIcons)mask );
    
    AddFrameToAnimationL( *image );
    
    // frame 2
    if ( iCallFlags & CBubbleManager::EVideo )
        {
        bitmap = EQgn_indi_call_video_callsta_2;
        mask  = EQgn_indi_call_video_callsta_2_mask;
        }
    else if ( iCallFlags & CBubbleManager::EVoIPCall )
        {
        bitmap = EQgn_indi_call_voip_active_2;
        mask  = EQgn_indi_call_voip_active_2_mask;
        }
    else
        {
        bitmap = EQgn_indi_call_active_2;
        mask  = EQgn_indi_call_active_2_mask;
        }
    
    iImageManager.SetBitmapToImage(
        image,
        (TBMIcons)bitmap,
        (TBMIcons)mask );
    
    AddFrameToAnimationL( *image );
    
    CleanupStack::PopAndDestroy( image );
    }

// End of file
