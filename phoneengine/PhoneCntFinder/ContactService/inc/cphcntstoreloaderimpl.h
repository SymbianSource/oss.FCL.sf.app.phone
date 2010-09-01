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
* Description:  Phone contact data store loader implementation.
*
*/


#ifndef C_PHCNTSTORELOADERIMPL_H
#define C_PHCNTSTORELOADERIMPL_H

#include "mphcntstoreloader.h"
#include "mphcntstoreloaderobserver.h"
#include "cphcntasynctosync.h"

class MPhCntContactManager;
class MVPbkContactStore;

/**
 *  Implements services to load contact data stores.
 *
 *  @lib phonecntfinder
 *  @since S60 9.1
 */
NONSHARABLE_CLASS( CPhCntStoreLoaderImpl ) 
        : 
        public CPhCntAsyncToSync,
        public MPhCntStoreLoader, 
        public MPhCntStoreLoaderObserver
    {

public:

    /**
     * Two-phased constructor.
     * @param aManager  Contact manager.
     */
    static CPhCntStoreLoaderImpl* NewL( MPhCntContactManager& aManager );

    /**
     * Two-phased constructor.
     * @param aManager  Contact manager.
     */
    static CPhCntStoreLoaderImpl* NewLC( MPhCntContactManager& aManager );
    
    /**
    * Destructor.
    */
    virtual ~CPhCntStoreLoaderImpl();

// from base class MPhCntStoreLoader

    /**
     * From MPhCntStoreLoader.
     *
     * @since S60 v9.1
     * @see MPhCntStoreLoader.
     */
    TBool IsContactStoreLoaded( const TDesC8& aContactLink ) const;
    
    /**
     * From MPhCntStoreLoader.
     * 
     * @since S60 v9.1
     * @see MPhCntStoreLoader.
     */
    void LoadContactStoreL( const TDesC8& aContactLink,
        MPhCntStoreLoaderObserver& aObserver );

// from base class MPhCntStoreLoaderObserver

    /**
     * From MPhCntStoreLoaderObserver.
     *
     * @since S60 v9.1
     * @see MPhCntStoreLoaderObserver.
     */
    void ContactStoreLoadingCompleted( 
        MVPbkContactStore* aStore, TInt aErrorCode );
public:   
    /**
     * Loads contact store with uri
     * 
     * @since S60 v5.1
     * @param aStoreUri Uri of the store to be loaded.
     */
    TInt LoadContactStoreWithUri( const TDesC& aStoreUri );
    
protected:
    /**
     * From CPhCntAsyncToSync.
     *
     * @since S60 v3.2
     * @see CPhCntAsyncToSync.    
     */
    void DoMakeAsyncRequestL();    
private:

    CPhCntStoreLoaderImpl( MPhCntContactManager& aManager );

    void ConstructL();

    TBool IsContactStoreLoadedL( const TDesC8& aContactLink ) const;

    TBool IsContactStoreLoadedL( const TDesC16& aContactStoreUri ) const;
private: // data

    /**
     * Contact manager.
     * Not own.
     */
    MPhCntContactManager& iContactManager;
    
    /**
     * Observer for contact store loading process.
     * Not own.
     */
    MPhCntStoreLoaderObserver* iObserver;

    /**
     * Pointer to uri of the store to be loaded.
     * Not own.
     */
    const TDesC* iStoreUri;
    };

#endif // C_PHCNTSTORELOADERIMPL_H

