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

#include "BMCustomManager.h"
#include "BMDefaultManager.h"
#include "BMBubbleHeader.h"
#include "BMPanic.h"


// ---------------------------------------------------------------------------
// TArrayItem class
// ---------------------------------------------------------------------------
//
class TArrayItem
    {
public:
    TArrayItem( 
        CTelBubbleCustomElement* aElement,
        TInt aPriority ): 
            iElement( *aElement ),
            iPriority( aPriority ) {};
        
    static TInt Compare( const TArrayItem& aFirst, const TArrayItem& aSecond )
        {
        if( aFirst.iPriority > aSecond.iPriority )
            {
            return 1;
            }
        else if( aFirst.iPriority < aSecond.iPriority )
            {
            return -1;
            }
        else
            {
            return 0;
            }
        }

    CTelBubbleCustomElement* Item() { return &iElement; }
private:
    CTelBubbleCustomElement& iElement;
    TInt iPriority;
    };

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleCustomManager::CBubbleCustomManager( CBubbleManager& aBubbleManager ) :
    iBubbleManager( aBubbleManager )
    {
    }


// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CBubbleCustomManager::ConstructL()
    {
    // create default implementations
    iDefaultManager = CBubbleDefaultManager::NewL( iBubbleManager );
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CBubbleCustomManager* CBubbleCustomManager::NewL(
    CBubbleManager& aBubbleManager )
    {
    CBubbleCustomManager* self = 
        new( ELeave ) CBubbleCustomManager( aBubbleManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleCustomManager::~CBubbleCustomManager()
    {
    for( TInt i = 0; i < iCustomizations.Count(); i++)
        {
        for( TInt j = 0; j < iCustomizations[j].Count(); j++ )
            {
            iCustomizations[i][j].Reset();
            iCustomizations[i][j].Close();
            }
        }
    
    delete iDefaultManager;
    }

// ---------------------------------------------------------------------------
// AddCustomElement
// ---------------------------------------------------------------------------
//
void CBubbleCustomManager::AddCustomElement( 
    const CBubbleManager::TBubbleId& aBubbleId,
    CTelBubbleCustomElement* aElement,
    TInt aPriority )
    {
    __ASSERT_DEBUG( aElement->Control(), Panic( EBMPanicCustomization ) ) ;
    TRAPD( err, aElement->Control()->SetContainerWindowL( iBubbleManager ) );
    
    if ( !err )
        {
        TArrayItem item( aElement, aPriority );
        iCustomizations[aBubbleId]
                       [aElement->ElementType()].InsertInOrderAllowRepeats(
            item, TLinearOrder<TArrayItem>(TArrayItem::Compare) );    
        }
    }

// ---------------------------------------------------------------------------
// RemoveCustomElement
// ---------------------------------------------------------------------------
//
void CBubbleCustomManager::RemoveCustomElement(
    const CBubbleManager::TBubbleId& aBubbleId,
    CTelBubbleCustomElement* aElement )
    {
    RArray<TArrayItem>& array = 
        iCustomizations[aBubbleId][aElement->ElementType()];
    
    for( TInt j = 0; j < array.Count(); j++ )
        {
        if( array[j].Item() == aElement )
            {
            array.Remove(j);
            return;
            }
        }
    }

// ---------------------------------------------------------------------------
// RemoveCustomElements
// ---------------------------------------------------------------------------
//
void CBubbleCustomManager::RemoveCustomElements( 
    const CBubbleManager::TBubbleId& aBubbleId )
    {
    for( TInt i = 0; i < iCustomizations[aBubbleId].Count(); i++ )
        {
        iCustomizations[aBubbleId][i].Reset();
        }
    }

// ---------------------------------------------------------------------------
// ReserveCustomElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement* CBubbleCustomManager::ReserveCustomElement( 
    const CBubbleHeader& aBubbleHeader,
    CTelBubbleCustomElement::TElementType aElement )
    {
    TBool isDefault;
    return ReserveCustomElement( aBubbleHeader, aElement, isDefault );
    }

// ---------------------------------------------------------------------------
// ReserveCustomElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement* CBubbleCustomManager::ReserveCustomElement( 
    const CBubbleHeader& aBubbleHeader,
    CTelBubbleCustomElement::TElementType aElement,
    TBool& aIsDefault )
    {
    const CBubbleManager::TBubbleId bubbleId = aBubbleHeader.BubbleId();
    CTelBubbleCustomElement* customElement = NULL;
    aIsDefault = EFalse;
    
    if( bubbleId != KBubbleConferenceId &&
        iCustomizations[bubbleId][aElement].Count() )
        {
        // Get custom implmentation.
        customElement = 
            iCustomizations[aBubbleHeader.BubbleId()][aElement][0].Item();
        customElement->Control()->MakeVisible( ETrue );
        }
    
    if ( !customElement )
        {
        // Get default implementation, if exists.
        customElement = DefaultElement( aBubbleHeader, aElement );
        if ( customElement )
            {
            aIsDefault = ETrue;
            }
        }
    
    return customElement;
    }

// ---------------------------------------------------------------------------
// ReleaseCustomElement
// ---------------------------------------------------------------------------
//
void CBubbleCustomManager::ReleaseCustomElement( 
    CTelBubbleCustomElement*& aCustomElement )
    {
    if ( aCustomElement )
        {
        aCustomElement->Control()->MakeVisible( EFalse );
        iDefaultManager->ReleaseElement( aCustomElement );
        aCustomElement = NULL;
        }
    }

// ---------------------------------------------------------------------------
// DefaultElement
// ---------------------------------------------------------------------------
//
CTelBubbleCustomElement* CBubbleCustomManager::DefaultElement( 
    const CBubbleHeader& aBubbleHeader,
    CTelBubbleCustomElement::TElementType aElement )
    {
    CTelBubbleCustomElement* element = NULL;
    
    switch( aElement )
        {
        case CTelBubbleCustomElement::EBigCallIndicator:
            {
            element = iDefaultManager->ReserveBigCallIndicatorElement(
                aBubbleHeader );
            }
            break;
        case CTelBubbleCustomElement::ENumberTypeIcon:
            {
            element = iDefaultManager->ReserveNumberTypeIconElement(
                aBubbleHeader );
            }
            break;
        case CTelBubbleCustomElement::ESmallCallIndicator:
            {
            element = iDefaultManager->ReserveSmallCallIndicatorElement(
                aBubbleHeader );
            }
            break;
        case CTelBubbleCustomElement::ECallImage:
            {
            element = iDefaultManager->ReserveCallImageElement(
                aBubbleHeader );
            }
            break;
        default:
            // NULL
            break;
        }
    
    if ( element )
        {
        // Make found element visible
        element->Control()->MakeVisible( ETrue );
        }
    
    return element;
    }

