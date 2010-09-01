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
* Description:  Manages default implementations for customizable elements. 
*
*/


#ifndef C_BUBBLEDEFAULTMANAGER_H
#define C_BUBBLEDEFAULTMANAGER_H

#include <e32base.h>
#include "BMBubbleManager.h"
#include "telbubblecustomelement.h"

class CBubbleHeader;

/**
 *  Customization manager.
 *
 *  @lib BubbleManager.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CBubbleDefaultManager ) : public CBase
    {
public:
    /**
     * Two-phased constructor.
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     */
    static CBubbleDefaultManager* NewL( CBubbleManager& aBubbleManager );
    
    /**
    * Destructor.
    */
    virtual ~CBubbleDefaultManager();

    /**
     * Reserves big call indicator element.
     *
     * @since S60 5.0
     * @param aHeader Bubble header.
     * @return Element instance.
     */
    CTelBubbleCustomElement* ReserveBigCallIndicatorElement(
        const CBubbleHeader& aHeader );
    
    /**
     * Reserves number type icon element.
     *
     * @since S60 5.0
     * @param aHeader Bubble header.
     * @return Element instance.
     */
    CTelBubbleCustomElement* ReserveNumberTypeIconElement(
        const CBubbleHeader& aHeader );
    
    /**
     * Reserves small call indicator element.
     *
     * @since S60 5.0
     * @param aHeader Bubble header.
     * @return Element instance.
     */
    CTelBubbleCustomElement* ReserveSmallCallIndicatorElement(
        const CBubbleHeader& aHeader );
    
    /**
     * Reserves call image element.
     *
     * @since S60 5.0
     * @param aHeader Bubble header.
     * @return Element instance.
     */
    CTelBubbleCustomElement* ReserveCallImageElement(
         const CBubbleHeader& aHeader );
    
    /**
     * Releases default element.
     *
     * @since S60 5.0
     * @param aElement Released element.
     */
    void ReleaseElement( CTelBubbleCustomElement*& aElement );    

private:
    // Default implementations
    enum TBubbleDefaultElements
        {
        EBMDefaultCallStatusIndiBig,
        EBMDefaultCallStatusIndiSmall,
        EBMDefaultCallStatusAnimBig,
        EBMDefaultCallStatusAnimSmall,
        EBMDefaultNumberTypeIcon,
        EBMDefaultCallImageControl
        };    

private:
    CBubbleDefaultManager( CBubbleManager& aBubbleManager );
    void ConstructL();
    CTelBubbleCustomElement* ReserveElement( TBubbleDefaultElements aType );
    
private: // data
    CBubbleManager&       iBubbleManager;

    // default elements
    typedef CArrayPtrFlat<CTelBubbleCustomElement> CSingleElement;
    // array of max elements
    RArray<TUint> iMaxAmounts;       
    // array of resource arrays
    CArrayPtrFlat<CSingleElement>* iElements; 
    // array of info about usage
    RArray<TUint> iAvailabilities;
    };

#endif // C_BUBBLEDEFAULTMANAGER_H
