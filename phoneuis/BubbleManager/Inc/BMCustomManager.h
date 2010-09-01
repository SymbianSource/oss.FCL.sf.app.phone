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


#ifndef C_BUBBLECUSTOMMANAGER_H
#define C_BUBBLECUSTOMMANAGER_H

#include <e32base.h>
#include "BMBubbleManager.h"
#include "telbubblecustomelement.h"

class CBubbleHeader;
class CBubbleDefaultManager;
class TArrayItem;

/**
 *  Customization manager.
 *
 *  @lib BubbleManager.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CBubbleCustomManager ) : public CBase
    {
public:
    /**
     * Two-phased constructor.
     * @param aBubbleManager Bubble manager.
     */
    static CBubbleCustomManager* NewL( CBubbleManager& aBubbleManager );
    
    /**
    * Destructor.
    */
    virtual ~CBubbleCustomManager();

    /**
     * Adds custom element to call bubble. Ownership is not transferred.
     *
     * @since S60 5.0
     * @param aBubbleId Bubble identifier.
     * @param aElement Custom element.
     * @param aPriority Priority 0(high) - 255(low). 
     */
    void AddCustomElement( 
        const CBubbleManager::TBubbleId& aBubbleId,
        CTelBubbleCustomElement* aElement,
        TInt aPriority );
    
    /**
     * Removes custom element from call bubble.
     *
     * @since S60 5.0
     * @param aBubbleId Bubble identifier.
     * @param aElement Custom element.
     */
    void RemoveCustomElement(
        const CBubbleManager::TBubbleId& aBubbleId,
        CTelBubbleCustomElement* aElement );
    
    /**
     * Removes all custom elements related to given bubble.
     *
     * @since S60 5.0
     * @param aBubbleId Bubble identifier.
     */
    void RemoveCustomElements( const CBubbleManager::TBubbleId& aBubbleId );
    
    /**
     * Reserves custom element. Default elements are
     * shared between call bubbles.
     *
     * @since S60 5.0
     * @param aBubbleId Bubble identifier.
     * @param aElement Element type.
     * return Element instance.
     */
    CTelBubbleCustomElement* ReserveCustomElement( 
        const CBubbleHeader& aBubbleHeader,
        CTelBubbleCustomElement::TElementType aElement );
    
    /**
     * Reserves custom element. Default elements are
     * shared between call bubbles.
     *
     * @since S60 5.0
     * @param aBubbleId Bubble identifier.
     * @param aElement Element type.
     * @param aIsDefault Is ETrue, when default implementation is used.
     * return Element instance.
     */
    CTelBubbleCustomElement* ReserveCustomElement( 
        const CBubbleHeader& aBubbleHeader,
        CTelBubbleCustomElement::TElementType aElement,
        TBool& aIsDefault );
    
    /**
     * Releases custom element.
     *
     * @since S60 5.0
     * @param aElement Released custom element.
     */
    void ReleaseCustomElement( CTelBubbleCustomElement*& aCustomElement );
    
private:
    CTelBubbleCustomElement* DefaultElement( 
        const CBubbleHeader& aBubbleHeader,
        CTelBubbleCustomElement::TElementType aElement );

private:
    CBubbleCustomManager( CBubbleManager& aBubbleManager );
    void ConstructL();

private: // data
    CBubbleManager&       iBubbleManager;

    TFixedArray< TFixedArray< RArray<TArrayItem>, 
                              CTelBubbleCustomElement::ECustomElementCount >, 
                 KBubbleCallMaxAmount > iCustomizations;
    
    CBubbleDefaultManager* iDefaultManager; // owned
    };

#endif // C_BUBBLECUSTOMMANAGER_H
