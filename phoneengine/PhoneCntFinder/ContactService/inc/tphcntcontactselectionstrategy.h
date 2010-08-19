/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implements contact selection strategy.
*
*/


#ifndef TPHCNTCONTACTSELECTIONSTRATEGY_H_
#define TPHCNTCONTACTSELECTIONSTRATEGY_H_

// System includes
#include <e32std.h>
#include <TVPbkContactStoreUriPtr.h>

// Forward declarations
class MVPbkContactLinkArray;
class CVPbkContactStoreUriArray;

// Constants
const TInt KNoContact = -1;
const TInt KManyContacts = -2;


/**
 *  Implements contact selection strategy.  
 *
 */
NONSHARABLE_CLASS( TPhCntContactSelectionStrategy ) 
    {
public:
    
    /** Defines the options for contact selection strategy */
    enum TAllowSeveralMatches
        {
        EAllowSingleMatch,
        EAllowSeveralMatches        
        };
    
public:
    TPhCntContactSelectionStrategy( );
    
public:
    /**
     * Sets the contact selection strategy used in conjunction with 
     * ApplyStrategy -method. 
     *
     * @param aSeveralMatchesAllowed Option for contact selection strategy. 
     */
    void SetContactSelectionStrategy( TAllowSeveralMatches aSeveralMatchesAllowed );
    
    /**
     * Returns the selected contact according to strategy.
     * If a call to SetContactSelectionStrategy is not made,
     * EAllowSingleMatch strategy is applied.
     *
     * @param aContactLinkArray Array of matching contacts.
     * @return index in array of selected contact. 
     *         KNoContact if empty array is passed. 
     *         KNoContact if several matches and EAllowSingleMatch set.
     */
    TInt ApplyStrategy( const MVPbkContactLinkArray& aContactLinkArray );
    
    /**
     * Returns the selected contact if single match from
     * additional contact stores is found.
     *
     * @param aContactLinkArray Array of matching contacts.
     * @param aAdditionalStoreUriArray Array of additional contact stores.
     * @return index in array of selected contact. 
     *         KNoContact if empty array is passed. 
     *         KManyContacts if several matches.
     */
    TInt ApplyAdditonalStoreStrategy( const MVPbkContactLinkArray& aContactLinkArray,
                                      const CVPbkContactStoreUriArray& aAdditionalStoreUriArray );
                                          
protected:        
    /**
     * Returns the URI of the contact store of the contact.     
     *
     * @param aContactLinkArrayIndex Specifies the contact.     
     * @return URI of the contact store.    
     */
    virtual TVPbkContactStoreUriPtr GetContactStoreUri( TInt aContactLinkArrayIndex );
    
private:
    TInt ApplySingleMatchStrategy( );
    TInt ApplySeveralMatchesStrategy( );    
    
private:
    TAllowSeveralMatches iSeveralMatchesAllowed;
    
    /* Not owned */
    const MVPbkContactLinkArray* iContactLinkArray;
    };


#endif /* TPHCNTCONTACTSELECTIONSTRATEGY_H_ */
