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
* Description:  Interface for loading contact stores.
*
*/


#ifndef M_MPHCNTSTORELOADER_H
#define M_MPHCNTSTORELOADER_H

#include <e32base.h>

class MPhCntStoreLoaderObserver;

/**
 *  Interface for loading contact stores.
 *
 *  @lib phonecntfinder.lib
 *  @since S60 v9.1
 */
class MPhCntStoreLoader
    {

public:

    /**
    * Destructor.
    */
    virtual ~MPhCntStoreLoader() { };

    /**
     * Checks is the specified contact store loaded.
     *
     * @since   S60 v9.1
     * @param   aContactLink      A contact link containing store to load.
     * @return  ETrue if store is loaded, EFalse otherwise.
     */
    virtual TBool IsContactStoreLoaded( 
        const TDesC8& aContactLink ) const = 0;
    
    /**
     * Starts to load specified contact store if not yet loaded.
     * MPhCntStoreLoaderObserver::ContactStoreLoaded is called when contact
     * store is loaded and ready for use.
     *
     * @since   S60 v9.1
     * @param   aContactLink      A contact link containing store to load.
     * @param   aObserver         An observer for the loading process.
     */
    virtual void LoadContactStoreL( const TDesC8& aContactLink,
        MPhCntStoreLoaderObserver& aObserver ) = 0;
    };

#endif // M_MPHCNTSTORELOADER_H
