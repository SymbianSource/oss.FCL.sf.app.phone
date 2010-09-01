/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contact store Uris
*
*/


#ifndef CPHCNTCONTACTSTOREURIS_H
#define CPHCNTCONTACTSTOREURIS_H

#include <e32base.h>

#include "tphcntcontactstorestate.h"

class CVPbkContactStoreUriArray;
class TVPbkContactStoreUriPtr;
class MPhCntContactStoreEventObserver;

/**
 *  Updates list of active contact store uris.
 *
 *  @lib PhoneCntFiner.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CPhCntContactStoreUris ): public CBase
    {

public:

    static CPhCntContactStoreUris* NewL();

    virtual ~CPhCntContactStoreUris();

    /**
     * Gives reference to all contact store uris that
     * are used with contact matching.
     *
     * @since S60 v3.2
     * @return Uris of contact stores.
     */
    CVPbkContactStoreUriArray& ContactStores() const;
    
    /**
     * Gives URIs to currently active contact stores. The Uris
     * are in priority order ie the first uri is a uri to a contact store, 
     * where contact matching should be tried first
     *
     * @since S60 v3.2
     * @return Uris of contact stores currently active, ownership is given to client.
     */
    CVPbkContactStoreUriArray* ActiveContactStoresL() const;
    
    /**
     * Indication that contact store is ready to be used.
     *
     * @since S60 v3.2
     * @aContactStoreUri URI of the contact store.
     */
    void StoreReady( TVPbkContactStoreUriPtr& aContactStoreUri );
    
    /**
     * Indication that contact store has become unavailable.
     *
     * @since S60 v3.2
     * @aContactStoreUri URI of the contact store.
     */
    void StoreUnavailable( TVPbkContactStoreUriPtr& aContactStoreUri );
    
    /**
     * Sets observer for contact store events.
     *
     * @since S60 v3.2
     * @param aObserver Observer of the events.
     */
    void SetObserver( MPhCntContactStoreEventObserver& aObserver );
    
    /**
     * Returns array of additional contact stores.
     *
     * @since S60 v3.2
     * @return Reference to Uris of additional contact stores
     */
    const CVPbkContactStoreUriArray& AdditionalContactStores() const;

    /**
     * Adds contact store to the base set of stores.
     *
     * @since   S60 v9.1
     * @param   aContactStoreUri    Contact store to add.
     */
    void AddContactStoreL( 
        const TVPbkContactStoreUriPtr& aContactStoreUri );
    
    /**
     * Removes contact store from the base set of stores.
     *
     * @since   S60 v9.1
     * @param   aContactStoreUri    Contact store to remove.
     */
    void RemoveContactStore( 
        const TVPbkContactStoreUriPtr& aContactStoreUri );

private:

    CPhCntContactStoreUris();

    void ConstructL();
    
    void SetContactStoreAvailability( 
        TVPbkContactStoreUriPtr& aStoreUri, 
        TBool aIsAvailable );
        
    /**
     * Reads additional contact store URIs from CenRep.
     *
     * @since S60 v3.2
     * @param aProductSpecificList Product specific list of additional stores
     */
    void ReadAdditionalContactStoreL( TDes* aProductSpecificList );
    
    /**
     * Parses the additional contact store names from the string
     * fetched from CenRep.
     *
     * @since S60 v3.2
     * @param aProductSpecificList  Reference to product specific 
     *                              list of additional stores
     */
    void ParseStoresL( const TDesC& aProductSpecificList );

    /**
     * Implements TIdentityRelation for class TPhCntContactStoreState.
     *
     * @since S60 v9.1
     * @param aState1   First state object to compare.
     * @param aState2   Second state object to compare.
     * @return ETrue if objects are same.
     */
    static TBool MatchUris( 
        const TPhCntContactStoreState& aState1,
        const TPhCntContactStoreState& aState2 );
        
private: // data

    /**
     * Contact store uris used with contact matching.
     * own.
     */
    CVPbkContactStoreUriArray* iContactStoreUriArray;
    
    /**
     * Contact store uris used with contact matching.
     * own.
     */
    CVPbkContactStoreUriArray* iAdditionalContactStoreUriArray;

    
    /**
     * Contact stores states in priority order.
     * Own.
     */
    RArray<TPhCntContactStoreState> iContactStoreStates;
    
    /**
     * Observer for contact store events.
     * Not own.
     */
    MPhCntContactStoreEventObserver* iObserver;
    
    };


#endif // CPHCNTCONTACTSTOREURIS_H
