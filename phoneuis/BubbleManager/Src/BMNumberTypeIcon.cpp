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
* Description:  Number type icon
*
*/


#include "BMNumberTypeIcon.h"
#include "BMBubbleImageManager.h"
#include "BMBubbleHeader.h"

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleNumberTypeIcon::CBubbleNumberTypeIcon( 
    CBubbleImageManager& aImageManager ) :
    iImageManager( aImageManager )
    {
    }


// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CBubbleNumberTypeIcon::ConstructL()
    {
    CTelBubbleImage::ConstructL();
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CBubbleNumberTypeIcon* CBubbleNumberTypeIcon::NewL(
        CBubbleImageManager& aImageManager )
    {
    CBubbleNumberTypeIcon* self = 
        new( ELeave ) CBubbleNumberTypeIcon( aImageManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleNumberTypeIcon::~CBubbleNumberTypeIcon()
    {
    }

// ---------------------------------------------------------------------------
// ReadBubbleHeader
// ---------------------------------------------------------------------------
//
void CBubbleNumberTypeIcon::ReadBubbleHeader( const CBubbleHeader& aHeader )
    {
    iNumberType = aHeader.NumberType();
    iCallFlags = aHeader.CallFlags();
    }

// ---------------------------------------------------------------------------
// MakeVisible
// ---------------------------------------------------------------------------
//
void CBubbleNumberTypeIcon::MakeVisible(TBool aVisible)
    {
    CTelBubbleImage::MakeVisible( aVisible );
    
    if( aVisible )
        {
        TInt picture = KErrNotFound;
        TInt pictureMask = KErrNotFound;
        
        switch ( iNumberType )
            {
            case CBubbleManager::EHome:
                if ( iCallFlags&CBubbleManager::EDiverted )
                    {
                    picture = EQgn_prop_nrtyp_home_div;
                    pictureMask = EQgn_prop_nrtyp_home_div_mask;
                    }
                else
                    {
                    picture = EQgn_prop_nrtyp_home;
                    pictureMask = EQgn_prop_nrtyp_home_mask;
                    }
                break;
            case CBubbleManager::EMobile:
                if ( iCallFlags&CBubbleManager::EDiverted )
                    {
                    picture = EQgn_prop_nrtyp_mobile_div;
                    pictureMask = EQgn_prop_nrtyp_mobile_div_mask;
                    }
                else
                    {
                    picture = EQgn_prop_nrtyp_mobile;
                    pictureMask = EQgn_prop_nrtyp_mobile_mask;
                    }
                break;
            case CBubbleManager::EWork:
                if ( iCallFlags&CBubbleManager::EDiverted )
                    {
                    picture = EQgn_prop_nrtyp_work_div;
                    pictureMask = EQgn_prop_nrtyp_work_div_mask;
                    }
                else
                    {
                    picture = EQgn_prop_nrtyp_work;
                    pictureMask = EQgn_prop_nrtyp_work_mask;
                    }
                break;
            case CBubbleManager::EPhone:
                if ( iCallFlags&CBubbleManager::EDiverted )
                    {
                    picture = EQgn_prop_nrtyp_phone_div;
                    pictureMask = EQgn_prop_nrtyp_phone_div_mask;
                    }
                else
                    {
                    picture = EQgn_prop_nrtyp_phone;
                    pictureMask = EQgn_prop_nrtyp_phone_mask;
                    }
                break;
            case CBubbleManager::EFaxNumber:
                if ( iCallFlags&CBubbleManager::EDiverted )
                    {
                    picture = EQgn_indi_call_diverted;
                    pictureMask = EQgn_indi_call_diverted_mask;
                    }
                else
                    {
                    picture = EQgn_prop_nrtyp_fax;
                    pictureMask = EQgn_prop_nrtyp_fax_mask;
                    }
                break;
            case CBubbleManager::EPager:
                if ( iCallFlags&CBubbleManager::EDiverted )
                    {
                    picture = EQgn_indi_call_diverted;
                    pictureMask = EQgn_indi_call_diverted_mask;
                    }
                else
                    {
                    picture = EQgn_prop_nrtyp_pager;
                    pictureMask = EQgn_prop_nrtyp_pager_mask;
                    }
                break;
            case CBubbleManager::ECar:
                if ( iCallFlags&CBubbleManager::EDiverted )
                    {
                    picture = EQgn_indi_call_diverted;
                    pictureMask = EQgn_indi_call_diverted_mask;
                    }
                else
                    {
                    picture = EQgn_prop_nrtyp_car;
                    pictureMask = EQgn_prop_nrtyp_car_mask;
                    }
                break;            
            case CBubbleManager::EAssistant:
                if ( iCallFlags&CBubbleManager::EDiverted )
                    {
                    picture = EQgn_indi_call_diverted;
                    pictureMask = EQgn_indi_call_diverted_mask;
                    }
                else
                    {
                    picture = EQgn_prop_nrtyp_assistant;
                    pictureMask = EQgn_prop_nrtyp_assistant_mask;
                    }
                break;            
            case CBubbleManager::ENotSet: // Flow through
            default:
                if ( iCallFlags&CBubbleManager::EDiverted )
                    {
                    picture = EQgn_indi_call_diverted;
                    pictureMask = EQgn_indi_call_diverted_mask;
                    }
                break;
            }

        if ( iCallFlags&CBubbleManager::EVideo )
            {
            // No number type icon for video call
            if ( iCallFlags&CBubbleManager::EDiverted )
                {
                picture = EQgn_indi_call_diverted;
                pictureMask = EQgn_indi_call_diverted_mask;
                }
            else
                {
                picture = KErrNotFound;
                pictureMask = KErrNotFound;            
                }            
            }
        
        if ( pictureMask == KErrNotFound )
            {
            CEikImage* self = static_cast<CEikImage*>(this);
            iImageManager.SetBitmapToImage( 
               self, 
               (TBMIcons)picture );
            }
        else
            {
            CEikImage* self = static_cast<CEikImage*>(this);
            iImageManager.SetBitmapToImage( 
               self, 
               (TBMIcons)picture, 
               (TBMIcons)pictureMask );
            }

        }
    else
        {
        SetPicture( NULL, NULL );
        }

    
    };

// End of file
