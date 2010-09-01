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
* Description:  Base class for contact link fetchers.
*
*/


#ifndef CPHCNTCONTACTLINKARRAYFETCH_H
#define CPHCNTCONTACTLINKARRAYFETCH_H

#include <MVPbkContactFindObserver.h>

#include "cphcntasynctosync.h"

class MVPbkContactLinkArray;

/**
 *  Base class for contact link fetchers.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntContactLinkArrayFetch ) :  
    public CPhCntAsyncToSync,
    public MVPbkContactFindObserver
    {

public: 
    
// from base class MVPbkContactFindObserver

    /**
     * From base class MVPbkContactFindObserver
     * Called when find is complete. Callee takes ownership of the results.
     * In case of an error during find, the aResults may contain only 
     * partial results of the find.
     *
     * @since S60 v3.1
     * @param aResults Array of contact links that matched the find.
     *                 Callee must take ownership of this object in
     *                 the end of the function, ie. in case the function
     *                 does not leave.
     */
    void FindCompleteL( MVPbkContactLinkArray* aResults );

    /**
     * From base class MVPbkContactFindObserver
     *
     * @since S60 v3.1
     * Called in case the find fails for some reason.
     * 
     * @param aError One of the system wide error codes.
     */
    void FindFailed( TInt aError );
    
protected:

    virtual ~CPhCntContactLinkArrayFetch();
    
    void BaseConstructL();

    CPhCntContactLinkArrayFetch();

protected: // data

    /**
     * Fetched contact links
     * Own, but derived class can destroy this if after deletion
     * iFetchedContactLinks pointer is set to NULL.
     */
    MVPbkContactLinkArray* iFetchedContactLinks;

    };

#endif // CPHCNTCONTACTLINKARRAYFETCH_H
