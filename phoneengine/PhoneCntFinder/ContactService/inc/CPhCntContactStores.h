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
* Description:  Contact store
*
*/


#ifndef CPHCNTCONTACTSTORES_H
#define CPHCNTCONTACTSTORES_H

#include <e32base.h>
#include <MVPbkContactStoreListObserver.h>
#include <MVPbkSingleContactOperationObserver.h>
#include "MPhCntContactStores.h"

class MVPbkContactLink;
class MPhCntContactManager;
class MPhCntContactFetchObserver;


/**
 *  Contact store, where contact can be fetched.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntContactStores ): 
    public CBase, 
    public MPhCntContactStores,
    public MVPbkSingleContactOperationObserver
    {
public:

    /**
     * Static constructor.
     * 
     * @since S60 v3.1
     * @param aContactManager Contact manager.
     */
    static CPhCntContactStores* NewL( MPhCntContactManager& aContactManager );
    
    /**
     * Destructor.
     *
     * @since S60 v3.1
     */
    ~CPhCntContactStores();
    
// from base class MPhCntContactStores

    /**
     * From MPhCntContactStores.
     *
     * @since S60 v3.1
     * @see MPhCntContactStores
     */
    TBool IsRequestActive() const;
    
    /**
     * From MPhCntContactStores.
     *
     * @since S60 v3.1
     * @see MPhCntContactStores
     */
    void CancelRequest();
    
    /**
     * From MPhCntContactStores.
     *
     * @since S60 v3.1
     * @see MPhCntContactStores
     */
    void FetchContactL( const MVPbkContactLink& aLink, 
        MPhCntContactFetchObserver& aObserver );
    
    /**
     * From MPhCntContactStores.
     *
     * @since S60 v3.1
     * @see MPhCntContactStores
     */
    void FetchFirstContactL( const TDesC8& aContactLinks,
        MPhCntContactFetchObserver& aObserver );
    
    /**
     * From MPhCntContactStores.
     *
     * @since S60 v3.1
     * @see MPhCntContactStores
     */    
    CVPbkContactLinkArray* CreateContactLinkArrayL( 
        const TDesC8& aContactLinks );

// From base class MVPbkSingeContactOperationObserver

    /**
     * From MVPbkSingeContactOperationObserver.
     *
     * @since S60 v3.1
     * @see MVPbkSingeContactOperationObserver
     */
    void VPbkSingleContactOperationComplete(
        MVPbkContactOperationBase& aOperation,
        MVPbkStoreContact* aContact);
    
    /**
     * From MVPbkSingeContactOperationObserver.
     *
     * @since S60 v3.1
     * @see MVPbkSingeContactOperationObserver
     */
    void VPbkSingleContactOperationFailed(
        MVPbkContactOperationBase& aOperation, 
        TInt aError );
        
protected:
    
    CPhCntContactStores( MPhCntContactManager& aContactManager );
    
    void ConstructL();
    
private:

    /**
    * Handle to virtual phonebook.
    * Not Own.
    */
    MPhCntContactManager& iContactManager;
    
    /**
    * Contact fetch operation observer.
    * Not own.
    */
    MPhCntContactFetchObserver* iContactFetchObserver;
    
    /**
    * Contact operation
    * Own
    */
    MVPbkContactOperationBase* iRetrieveContactOperation;
 
    /** 
     * Link is stored here during the fetch.
     * Own.
     */    
    MVPbkContactLink* iLink;
            
    };


#endif // CPHCNTCONTACTSTORES_H