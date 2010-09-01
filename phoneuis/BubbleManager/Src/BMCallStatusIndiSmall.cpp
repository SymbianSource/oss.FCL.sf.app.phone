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
* Description:  Small call status indicator
*
*/


#include "BMCallStatusIndiSmall.h"
#include "BMBubbleImageManager.h"
#include "BMBubbleHeader.h"

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleCallStatusIndiSmall::CBubbleCallStatusIndiSmall( 
    CBubbleImageManager& aImageManager ) :
    CBubbleCallStatusIndi( aImageManager )
    {
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CBubbleCallStatusIndiSmall* CBubbleCallStatusIndiSmall::NewL(
        CBubbleImageManager& aImageManager )
    {
    CBubbleCallStatusIndiSmall* self = 
        new( ELeave ) CBubbleCallStatusIndiSmall( aImageManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleCallStatusIndiSmall::~CBubbleCallStatusIndiSmall()
    {
    }

// ---------------------------------------------------------------------------
// SetImageAndMask
// ---------------------------------------------------------------------------
//
void CBubbleCallStatusIndiSmall::SetImageAndMask()
    {
    TInt image = KErrNotFound;  
    TInt mask = KErrNotFound; 
    
    switch ( iCallState )
        {
        case CBubbleManager::EOutgoing:
        case CBubbleManager::EActive:
            if ( iCallFlags&CBubbleManager::EVideo )
                {
                image = EQgn_indi_call_video_callsta_1;
                mask = EQgn_indi_call_video_callsta_1_mask;
                }   
            else if ( iCallFlags&CBubbleManager::EVoIPCall )
                {
                image = EQgn_indi_call_voip_active;
                mask = EQgn_indi_call_voip_active_mask;
                }
            else
                {
                image = EQgn_indi_call_active;
                mask = EQgn_indi_call_active_mask;
                }
            break;
        case CBubbleManager::EAlertToDisconnected:
        case CBubbleManager::EDisconnected:
            if ( iCallFlags&CBubbleManager::EVideo )
                {
                image = EQgn_indi_call_video_callsta_disconn;
                mask = EQgn_indi_call_video_callsta_disconn_mask;
                }   
            else if ( iCallFlags&CBubbleManager::EVoIPCall )
                {
                image = EQgn_indi_call_voip_disconn;
                mask = EQgn_indi_call_voip_disconn_mask;
                }    
            else
                {
                image = EQgn_indi_call_disconn;
                mask = EQgn_indi_call_disconn_mask;
                }

            break;
        case CBubbleManager::EOnHold:
            if ( iCallFlags&CBubbleManager::EVoIPCall )
                {
                image = EQgn_indi_call_voip_held;
                mask = EQgn_indi_call_voip_held_mask;
                }
            else 
                {
                image = EQgn_indi_call_held;
                mask = EQgn_indi_call_held_mask;
                }
            break;
        default:
            break;
        }
    
    if ( image != KErrNotFound && mask != KErrNotFound )
        {
        CEikImage* self = static_cast<CEikImage*>(this);
        iImageManager.SetBitmapToImage( 
           self, 
           (TBMIcons)image, 
           (TBMIcons)mask );
        }

    };

// End of file

