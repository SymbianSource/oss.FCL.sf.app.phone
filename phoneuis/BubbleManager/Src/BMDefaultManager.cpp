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
* Description:  Customization manager
*
*/

#include "BMDefaultManager.h"
#include "BMBubbleImageManager.h"
#include "BMBubbleHeader.h"
#include "BMCallStatusIndiBig.h"
#include "BMCallStatusIndiSmall.h"
#include "BMCallStatusAnimBig.h"
#include "BMCallStatusAnimSmall.h"
#include "BMNumberTypeIcon.h"
#include "BMPanic.h"
#include <telbubblecallimage.h>

// max amounts per type
const TInt KBubbleMaxCallStatusIndiBig  = 2;
const TInt KBubbleMaxCallStatusIndiSmall  = 7;
const TInt KBubbleMaxCallStatusAnimBig  = 2;
const TInt KBubbleMaxCallStatusAnimSmall  = 1;
const TInt KBubbleMaxNumberTypeIcon  = 2;
const TInt KBubbleMaxCallerImageDrawer = 1;

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleDefaultManager::CBubbleDefaultManager( CBubbleManager& aBubbleManager )
    : iBubbleManager( aBubbleManager )
    {
    }


// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CBubbleDefaultManager::ConstructL()
    {
    iMaxAmounts.Append( KBubbleMaxCallStatusIndiBig );
    iMaxAmounts.Append( KBubbleMaxCallStatusIndiSmall );
    iMaxAmounts.Append( KBubbleMaxCallStatusAnimBig );
    iMaxAmounts.Append( KBubbleMaxCallStatusAnimSmall );
    iMaxAmounts.Append( KBubbleMaxNumberTypeIcon );
    iMaxAmounts.Append( KBubbleMaxCallerImageDrawer );
    
    const TInt kDifferentElements = iMaxAmounts.Count();

    for ( TInt i = 0 ; i < kDifferentElements ; i++ )
        {
        iAvailabilities.Append( 0 );
        }

    // Create main array
    iElements = new( ELeave ) 
                    CArrayPtrFlat<CSingleElement>( kDifferentElements );
    iElements->SetReserveL( kDifferentElements );

    // Create arrays inside the main array and add them to the main array
    TInt resource = 0;
    for ( resource = 0 ; resource < kDifferentElements ; resource++ )
        {
        CSingleElement* single = new( ELeave ) 
                                    CSingleElement( iMaxAmounts[resource] );
        CleanupStack::PushL( single );
        single->SetReserveL( iMaxAmounts[resource] );
        iElements->AppendL( single );
        CleanupStack::Pop( single );
        }

    // Create implementations to individual arrays:
    for ( resource = 0 ; resource < kDifferentElements ; resource++ )
        {
        TInt maxCellAmount = iMaxAmounts[resource];
        CSingleElement* single = iElements->At( resource );
        switch( resource )
            {
            case EBMDefaultCallStatusIndiBig:
                {
                for ( TInt i = 0 ; i < maxCellAmount ; i++ )
                    {
                    CBubbleCallStatusIndiBig* indiControl = 
                        CBubbleCallStatusIndiBig::NewL( 
                            iBubbleManager.ImageManager() );
                    CleanupStack::PushL( indiControl );
                    indiControl->SetContainerWindowL( iBubbleManager );
                    indiControl->SetParent( &iBubbleManager );
                    CTelBubbleCustomElement* indiElem =  
                        CTelBubbleCustomElement::NewL( indiControl, 
                            CTelBubbleCustomElement::EBigCallIndicator );
                    CleanupStack::PushL( indiElem );
                    single->AppendL( indiElem );
                    CleanupStack::Pop(2,indiControl);
                    }
                break;
                }
                
            case EBMDefaultCallStatusIndiSmall:
                {
                for ( TInt i = 0 ; i < maxCellAmount ; i++ )
                    {
                    CBubbleCallStatusIndiSmall* indiControl = 
                        CBubbleCallStatusIndiSmall::NewL( 
                            iBubbleManager.ImageManager() );
                    CleanupStack::PushL( indiControl );
                    indiControl->SetContainerWindowL( iBubbleManager );
                    indiControl->SetParent( &iBubbleManager );
                    CTelBubbleCustomElement* indiElem =  
                        CTelBubbleCustomElement::NewL( indiControl, 
                            CTelBubbleCustomElement::ESmallCallIndicator );
                    CleanupStack::PushL( indiElem );
                    single->AppendL( indiElem );
                    CleanupStack::Pop(2,indiControl);
                    }
                break;
                }
                
            case EBMDefaultCallStatusAnimBig:
                {
                for ( TInt i = 0 ; i < maxCellAmount ; i++ )
                    {
                    CBubbleCallStatusAnimBig* animControl = 
                        CBubbleCallStatusAnimBig::NewL( 
                            iBubbleManager.ImageManager() );
                    CleanupStack::PushL( animControl );
                    animControl->SetContainerWindowL( iBubbleManager );
                    animControl->SetParent( &iBubbleManager );
                    CTelBubbleCustomElement* animElem =  
                        CTelBubbleCustomElement::NewL( animControl, 
                            CTelBubbleCustomElement::EBigCallIndicator );
                    CleanupStack::PushL( animElem );
                    single->AppendL( animElem );
                    CleanupStack::Pop(2,animControl);
                    }
                break;
                }
                
            case EBMDefaultCallStatusAnimSmall:
                {
                for ( TInt i = 0 ; i < maxCellAmount ; i++ )
                    {
                    CBubbleCallStatusAnimSmall* animControl = 
                        CBubbleCallStatusAnimSmall::NewL( 
                            iBubbleManager.ImageManager() );
                    CleanupStack::PushL( animControl );
                    animControl->SetContainerWindowL( iBubbleManager );
                    animControl->SetParent( &iBubbleManager );
                    CTelBubbleCustomElement* animElem =  
                        CTelBubbleCustomElement::NewL( animControl, 
                            CTelBubbleCustomElement::ESmallCallIndicator );
                    CleanupStack::PushL( animElem );
                    single->AppendL( animElem );
                    CleanupStack::Pop(2,animControl);
                    }
                break;
                }
                
            case EBMDefaultNumberTypeIcon:
                {
                for ( TInt i = 0 ; i < maxCellAmount ; i++ )
                    {
                    CBubbleNumberTypeIcon* numberTypeIcon = 
                        CBubbleNumberTypeIcon::NewL( 
                            iBubbleManager.ImageManager() );
                    CleanupStack::PushL( numberTypeIcon );
                    numberTypeIcon->SetContainerWindowL( iBubbleManager );
                    numberTypeIcon->SetParent( &iBubbleManager );
                    CTelBubbleCustomElement* numberTypeIconElem =  
                        CTelBubbleCustomElement::NewL( numberTypeIcon, 
                            CTelBubbleCustomElement::EBigCallIndicator );
                    CleanupStack::PushL( numberTypeIconElem );
                    single->AppendL( numberTypeIconElem );
                    CleanupStack::Pop( 2, numberTypeIcon );
                    }
                break;
                }
                
            case EBMDefaultCallImageControl:
                {
                for ( TInt i = 0 ; i < maxCellAmount ; i++ )
                    {
                    CTelBubbleCallImage* image = 
                        CTelBubbleCallImage::NewL();
                    CleanupStack::PushL( image );
                    image->SetContainerWindowL( iBubbleManager );
                    image->SetParent( &iBubbleManager );
                    CTelBubbleCustomElement* imageElem =  
                        CTelBubbleCustomElement::NewL( image, 
                            CTelBubbleCustomElement::ECallImage );
                    CleanupStack::PushL( imageElem );
                    single->AppendL( imageElem );
                    CleanupStack::Pop(2,image);
                    }
                break;
                }    
                
            default:
                Panic( EBMPanicUnhandledSwitchCase );
            }
        }    
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CBubbleDefaultManager* CBubbleDefaultManager::NewL(
    CBubbleManager& aBubbleManager )
    {
    CBubbleDefaultManager* self = 
        new( ELeave ) CBubbleDefaultManager( aBubbleManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleDefaultManager::~CBubbleDefaultManager()
    {
    if ( iElements )
        {
        for ( TInt i = 0 ; i < iElements->Count() ; i++ )
            {
            if ( iElements->At( i ) )
                {
                iElements->At( i )->ResetAndDestroy();
                }
            }
        iElements->ResetAndDestroy();
        delete iElements;
        }

    iAvailabilities.Close();
    iMaxAmounts.Close();    
    }

// ---------------------------------------------------------------------------
// ReserveCallIndicatorElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement* CBubbleDefaultManager::ReserveBigCallIndicatorElement(
    const CBubbleHeader& aHeader )
    {
    CTelBubbleCustomElement* element;
    CBubbleManager::TPhoneCallState state = aHeader.CallState();
    
    if ( state == CBubbleManager::EOutgoing ||
         state == CBubbleManager::EAlertToDisconnected )
        {
        // Image
        element = ReserveElement( EBMDefaultCallStatusIndiBig );
        CBubbleCallStatusIndi* indi = 
            static_cast<CBubbleCallStatusIndi*> ( element->Control() ); 
        indi->ReadBubbleHeader( aHeader );
        }
    else
        {
        // Animation
        element = ReserveElement( EBMDefaultCallStatusAnimBig );
        CBubbleCallStatusAnim* anim =
            static_cast<CBubbleCallStatusAnim*> ( element->Control() ); 
        anim->ReadBubbleHeader( aHeader );
        }    
    
    return element;
    }

// ---------------------------------------------------------------------------
// ReserveSmallCallIndicatorElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement* CBubbleDefaultManager::ReserveSmallCallIndicatorElement(
    const CBubbleHeader& aHeader )
    {
    CTelBubbleCustomElement* element;
    CBubbleManager::TPhoneCallState state = aHeader.CallState();
    
    if( state == CBubbleManager::EOutgoing ||
        state == CBubbleManager::EWaiting ||
        state == CBubbleManager::EAlerting )
        {
        // Show animation
        element = ReserveElement( EBMDefaultCallStatusAnimSmall );
        CBubbleCallStatusAnim* anim =
            static_cast<CBubbleCallStatusAnim*> ( element->Control() ); 
        anim->ReadBubbleHeader( aHeader );
        }
    else
        {
        // Show image
        element = ReserveElement( EBMDefaultCallStatusIndiSmall );
        CBubbleCallStatusIndi* indi = 
            static_cast<CBubbleCallStatusIndi*> ( element->Control() ); 
        indi->ReadBubbleHeader( aHeader );
        }
    
    return element;
    }

// ---------------------------------------------------------------------------
// ReserveNumberTypeIconElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement* CBubbleDefaultManager::ReserveNumberTypeIconElement(
     const CBubbleHeader& aHeader )
    {
    CTelBubbleCustomElement* element = 
        ReserveElement( EBMDefaultNumberTypeIcon );
    
    CBubbleNumberTypeIcon* icon = 
        static_cast<CBubbleNumberTypeIcon*> ( element->Control() ); 

    icon->ReadBubbleHeader( aHeader );
    
    return element;
    }

// ---------------------------------------------------------------------------
// ReserveCallerImageElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement* CBubbleDefaultManager::ReserveCallImageElement(
    const CBubbleHeader& /*aHeader*/ )
    {
    return ReserveElement( EBMDefaultCallImageControl );
    }

// ---------------------------------------------------------------------------
// ReserveElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement* CBubbleDefaultManager::ReserveElement(
    TBubbleDefaultElements aType )
    {
    CTelBubbleCustomElement* element = NULL;
    TUint amount = iElements->At( aType )->Count();
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
        element = iElements->At( aType )->At( slot );
        if ( element != NULL )
            {
            available |= bit; // set the corrensponding bit 
                              // for reserved indicator
            }
        break;
        }
    __ASSERT_DEBUG( element != NULL , 
                    Panic( EBMPanicCustomization ) );
    return element;
    }

// ---------------------------------------------------------------------------
// ReleaseElement
// ---------------------------------------------------------------------------
//
void CBubbleDefaultManager::ReleaseElement(
    CTelBubbleCustomElement*& aElement )
    {
    if ( aElement == NULL )
        {
        return;
        }

    for ( TInt i = 0 ; i < iElements->Count() ; i++ )
        {
        CSingleElement* single = iElements->At( i );
        TUint amount = single->Count();
        TUint& available = iAvailabilities[i]; // 1 mean is reserved 
                                                   // and 0 free slot
        TUint bit = 1; // start from first bit
        for ( TUint slot = 0 ; slot < amount ; slot++ )
            {
            if ( single->At( slot ) == aElement )
                {
                __ASSERT_DEBUG( available&bit, 
                                Panic( EBMPanicCustomization ) );
                available &= ~bit; // zero the corrensponding bit 
                                   // for freed indicator
                aElement = NULL;
                break;
                }
            bit = bit<<1; //move to next bit of for next round
            }
        }
    }

