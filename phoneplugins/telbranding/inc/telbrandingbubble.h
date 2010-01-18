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
* Description:  Wraps bubble into one class.
*
*/

#ifndef C_CTELBRANDINGBUBBLE_H
#define C_CTELBRANDINGBUBBLE_H

#include <telbubblecustomelement.h>

class MTelBubbleExtensionData;
class CTelBrandingServerHandler;
class CTelBubbleCustomElement;
class MTelBubbleExtensionObserver;


/**
 *  Wraps bubble into one class.
 *  Owns bubble images.
 *
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CTelBrandingBubble ) : public CBase 
    {
public:
    static CTelBrandingBubble* NewLC(
        MTelBubbleExtensionData& aCallData, 
        CTelBrandingServerHandler& aServer );

    /**
    * Destructor.
    */
    virtual ~CTelBrandingBubble();

    
public: // New functions
    TInt BubbleId() const;

    void AttachElements( MTelBubbleExtensionObserver& aObs ) const;

    void DetechElements( MTelBubbleExtensionObserver& aObs ) const;
    
private:
    /**
     * Creates and adds elements into array
     */
    void CreateCallIndicationElementL( 
            CTelBrandingServerHandler& aServer,
            CTelBubbleCustomElement::TElementType aType );
   
    /**
     * Creates and adds elements into array in priority order.
     *  -Element with most highest priority last must be added last.
     *  -Lowest priority first.
     */
    void AddBrandingImageWithPriorityL( 
        const TDesC8& aImageType,
        CTelBrandingServerHandler& aServer,
        CTelBubbleCustomElement::TElementType aType );
    
    /**
     * Helper, adds element to array
     */
    void AddElementToArrayL( CTelBubbleCustomElement* aElement );

private:
    CTelBrandingBubble( MTelBubbleExtensionData& aCallData );

    void ConstructL( 
        CTelBrandingServerHandler& aServer );

private: // data
    /**
     * Bubble specific data
     */
    MTelBubbleExtensionData& iCallData;

    /**
     * Owned images
     */
    RPointerArray<CTelBubbleCustomElement> iElements;
    };


#endif // C_CTELBRANDINGBUBBLE_H
