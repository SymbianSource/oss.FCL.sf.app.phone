/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Resource Manager
*
*/


// INCLUDE FILES
#include    "BMResourceManager.h"
#include    "BMBubbleManager.h"
#include    "BMUtils.h"
#include    "BMPanic.h"
#include    "BMBubbleImage.h"
#include    <eikimage.h>            //CEikImage
#include    <eiklabel.h>            //CEikLabel
#include    <AknsUtils.h>

// CONSTANTS
const TInt KBubbleMaxEikImages = 17;
const TInt KBubbleMaxEikLabels = 6;

// ================= MEMBER FUNCTIONS * ======================================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CBubbleResourceManager::CBubbleResourceManager( CBubbleManager& aParentWindow )
: iParentWindow( aParentWindow )
    {
    }

// Symbian OS default constructor can leave.
void CBubbleResourceManager::ConstructL()
    {
    // Set max values to each resource
    iMaxAmounts.Append( KBubbleMaxEikImages );
    iMaxAmounts.Append( KBubbleMaxEikLabels );

    const TInt kDifferentResources = iMaxAmounts.Count();

    // Zero all availabilities
    for ( TInt i = 0 ; i < kDifferentResources ; i++ )
        {
        iAvailabilities.Append( 0 );
        }
    

    // Create main array
    iResources = new( ELeave ) 
                    CArrayPtrFlat<CSingleResource>( kDifferentResources );
    iResources->SetReserveL( kDifferentResources );

    // Create arrays inside the main array and add them to the main array
    TInt resource = 0;
    for ( resource = 0 ; resource < kDifferentResources ; resource++ )
        {
        CSingleResource* single = new( ELeave ) 
                                    CSingleResource( iMaxAmounts[resource] );
        CleanupStack::PushL( single );
        single->SetReserveL( iMaxAmounts[resource] );
        iResources->AppendL( single );
        CleanupStack::Pop(); // single
        }

    //Create components to individual arrays:
    for ( resource = 0 ; resource < kDifferentResources ; resource++ )
        {
        TInt maxCellAmount = iMaxAmounts[resource];
        CSingleResource* single = iResources->At( resource );
        switch( resource )
            {
            case EBMEikImage:
                {
                for ( TInt i = 0 ; i < maxCellAmount ; i++ )
                    {
                    CBubbleImage* newImage = new( ELeave ) CBubbleImage;
                    CleanupStack::PushL( newImage );
                    newImage->SetContainerWindowL( iParentWindow );
                    newImage->SetParent( &iParentWindow );
                    newImage->SetPictureOwnedExternally( ETrue );
                    newImage->SetPicture( NULL );
                    newImage->MakeVisible( EFalse );
                    single->AppendL( newImage );
                    CleanupStack::Pop(); //newImage
                    }
                break;
                }
            case EBMEikLabel:
                {
                for ( TInt i = 0 ; i < maxCellAmount ; i++ )
                    {
                    CEikLabel* newText = new( ELeave ) CEikLabel;
                    CleanupStack::PushL( newText );
                    newText->SetContainerWindowL( iParentWindow );
                    newText->SetParent( &iParentWindow );
                    newText->SetBufferReserveLengthL( KBubbleLabelMaxLength );
                    newText->SetTextL( KNullDesC );
                    single->AppendL( newText );
                    CleanupStack::Pop(); //newText
                    }
                break;
                }
            default:
                Panic( EBMPanicUnhandledSwitchCase );
            }
        }
    }

// Two-phased constructor.
CBubbleResourceManager* CBubbleResourceManager::NewL( 
    CBubbleManager& aParentWindow )
    {
    CBubbleResourceManager* self = 
                        new (ELeave) CBubbleResourceManager( aParentWindow );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

    
// Destructor
CBubbleResourceManager::~CBubbleResourceManager()
    {
    if ( iResources )
        {
        // Destroy the inner arrays
        for ( TInt i = 0 ; i < iResources->Count() ; i++ )
            {
            if ( iResources->At( i ) )
                {
                iResources->At( i )->ResetAndDestroy();
                }
            }
        iResources->ResetAndDestroy();
        delete iResources;
        }

    iAvailabilities.Close();
    iMaxAmounts.Close();
    }



// ---------------------------------------------------------------------------
// CBubbleResourceManager::ActivateL
// Activates all the controls.
// 
// ---------------------------------------------------------------------------
//
void CBubbleResourceManager::ActivateL()
    {
    TInt resourcesTotal = iResources->Count();
    for ( TInt oneResource = 0 ; oneResource < resourcesTotal ; oneResource++ )
        {
        CSingleResource* singleResource = iResources->At( oneResource );
        TInt elementsTotal = singleResource->Count();
        for ( TInt element = 0 ; element < elementsTotal ; element++)
            {
            singleResource->At( element )->ActivateL();
            }
        }
    }

// ---------------------------------------------------------------------------
// CBubbleResourceManager::ReserveEikLabel
// Gets free label and returns pointer to it.
// 
// ---------------------------------------------------------------------------
//
CEikLabel* CBubbleResourceManager::ReserveEikLabel()
    {
    CEikLabel* label = NULL;
    ReserveResource( (CCoeControl*&)label , EBMEikLabel );
    return label;
    }

// ---------------------------------------------------------------------------
// CBubbleResourceManager::ReleaseEikLabel
// Zeros the label and releases it.
// 
// ---------------------------------------------------------------------------
//
void CBubbleResourceManager::ReleaseEikLabel( CEikLabel*& aLabel )
    {
    if ( aLabel )
        {
        BubbleUtils::AddTextToEikLabel( aLabel , KNullDesC );
        }
    ReleaseResource( (CCoeControl*&)aLabel , EBMEikLabel );
    }

// ---------------------------------------------------------------------------
// CBubbleResourceManager::ReserveEikImage
// Gets new image object and sets it visible.
// 
// ---------------------------------------------------------------------------
//
CEikImage* CBubbleResourceManager::ReserveEikImage( 
    TBool aIsBackgroundImage )
    {
    CBubbleImage* image = NULL;
    ReserveResource( ( CCoeControl*& )image , EBMEikImage );
    if ( image )
        {
        if( aIsBackgroundImage )
            {
            image->SetAsBackgroundImage( ETrue );
            }
        image->MakeVisible( ETrue );
        }
    return image;
    }

// ---------------------------------------------------------------------------
// CBubbleResourceManager::ReleaseEikImage
// NULLs the bitmaps, makes invisible and releases the image.
// 
// ---------------------------------------------------------------------------
//
void CBubbleResourceManager::ReleaseEikImage( CEikImage*& aImage )
    {
    if ( aImage )
        {
        aImage->SetPicture( NULL , NULL );
        aImage->SetPictureOwnedExternally( ETrue );
        aImage->MakeVisible( EFalse );
        static_cast<CBubbleImage*>( aImage )->SetAsBackgroundImage( EFalse );
        }
    ReleaseResource( (CCoeControl*&)aImage , EBMEikImage );
    }


// ---------------------------------------------------------------------------
// CBubbleResourceManager::ReserveResource
// Reserves some CCoeControl. Uses the second parameter for indexing
// the main arrays.
// ---------------------------------------------------------------------------
//
void CBubbleResourceManager::ReserveResource( CCoeControl*& aResource , 
                                              TBubbleResource aType )
    {
    TUint amount = iResources->At( aType )->Count();
    // 1 mean is reserved and 0 free slot
    TUint& available = iAvailabilities[aType]; 
    TUint bit = 1; // start from first bit
    for ( TUint slot = 0 ; slot < amount ; slot++ )
        {
        // if flag is one the seat is taken...
        if ( available&bit )
            {
            bit = bit<<1; //move to next bit of for next round
            continue;
            }

        // so we found a free slot
        aResource = iResources->At( aType )->At( slot );
        if ( aResource != NULL )
            {
            available |= bit; // set the corrensponding bit 
                              // for reserved indicator
            }
        break;
        }
    __ASSERT_DEBUG( aResource != NULL , Panic( EBMPanicImages ) );
    }

// ---------------------------------------------------------------------------
// CBubbleResourceManager::ReleaseResource
// Releases CCoeControl object.
// 
// ---------------------------------------------------------------------------
//
void CBubbleResourceManager::ReleaseResource( CCoeControl*& aResource , 
                                              TBubbleResource aType )
    {
    if ( aResource == NULL )
        {
        return;
        }

    CSingleResource* single = iResources->At( aType );
    TUint amount = single->Count();
    TUint& available = iAvailabilities[aType]; // 1 mean is reserved 
                                               // and 0 free slot
    TUint bit = 1; // start from first bit
    for ( TUint slot = 0 ; slot < amount ; slot++ )
        {
        if ( single->At( slot ) == aResource )
            {
            __ASSERT_DEBUG( available&bit , Panic( EBMPanicImages ) );
            available &= ~bit; // zero the corrensponding bit 
                               // for freed indicator
            aResource = NULL;
            break;
            }
        bit = bit<<1; //move to next bit of for next round
        }
    __ASSERT_DEBUG( aResource == NULL , Panic( EBMPanicImages ) );
    }

//  End of File  
