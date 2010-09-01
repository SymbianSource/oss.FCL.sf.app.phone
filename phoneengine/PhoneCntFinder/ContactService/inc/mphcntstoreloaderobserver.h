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
* Description:  Observer interface for contact data store loading process.
*
*/


#ifndef M_MPHCNTSTORELOADEROBSERVER_H
#define M_MPHCNTSTORELOADEROBSERVER_H

#include <e32base.h>

class MVPbkContactStore;

/**
 * Observer interface for contact data store loading process.
 *
 * @lib phonecntfinder.lib
 * @since S60  v9.1
 */
class MPhCntStoreLoaderObserver 
    {
public:
    
    /**
     * Callback to indicate that contact data store loading process is
     * completed. 
     *
     * @param   aStore          The contact store which was loaded or NULL if
     *                          error occured. Ownership is not transferred. 
     * @param   aErrorCode      System wide error code if error occured.
     */
    virtual void ContactStoreLoadingCompleted( 
        MVPbkContactStore* aStore, TInt aErrorCode ) = 0;
    };

#endif // M_MPHCNTSTORELOADEROBSERVER_H
