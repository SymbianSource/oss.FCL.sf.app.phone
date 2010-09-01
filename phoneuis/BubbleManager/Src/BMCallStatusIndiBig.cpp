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
* Description:  Big call status indicator
*
*/

#include "BMCallStatusIndiBig.h"
#include "BMBubbleImageManager.h"
#include "BMBubbleHeader.h"

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleCallStatusIndiBig::CBubbleCallStatusIndiBig( 
    CBubbleImageManager& aImageManager ) :
    CBubbleCallStatusIndi( aImageManager )
    {
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CBubbleCallStatusIndiBig* CBubbleCallStatusIndiBig::NewL(
        CBubbleImageManager& aImageManager )
    {
    CBubbleCallStatusIndiBig* self = 
        new( ELeave ) CBubbleCallStatusIndiBig( aImageManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleCallStatusIndiBig::~CBubbleCallStatusIndiBig()
    {
    }


// ---------------------------------------------------------------------------
// SetImageAndMask
// ---------------------------------------------------------------------------
//
void CBubbleCallStatusIndiBig::SetImageAndMask()
    {
    TInt image = KErrNotFound;  
    TInt mask = KErrNotFound; 
    
    switch ( iCallState )
        {
        case CBubbleManager::EOutgoing:
            if ( iCallFlags & CBubbleManager::EVideo )
                {
                image = EQgn_indi_call_video_1;
                mask  = EQgn_indi_call_video_1_mask;
                }
            else if ( iCallFlags & CBubbleManager::EVoIPCall )
                {
                image = EQgn_indi_call_voip_waiting_1;
                mask  = EQgn_indi_call_voip_waiting_1_mask;
                }
            else
                {
                image = EQgn_indi_call_waiting_1;
                mask  = EQgn_indi_call_waiting_1_mask;
                }
            break;
        case CBubbleManager::EAlertToDisconnected:
            if ( iCallFlags & CBubbleManager::EVideo )
                {
                image = EQgn_indi_call_video_disconn;
                mask  = EQgn_indi_call_video_disconn_mask;
                }
            else if ( iCallFlags & CBubbleManager::EVoIPCall )
                {
                image = EQgn_indi_call_voip_callsta_disconn;
                mask  = EQgn_indi_call_voip_callsta_disconn_mask;
                }
            else
                {
                image = EQgn_indi_call_waiting_disconn;
                mask  = EQgn_indi_call_waiting_disconn_mask;
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
    }

// End of file
