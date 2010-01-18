/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of CPhCntFetchContact class.
*
*/


#ifndef CPHCNTFETCHCONTACT_H
#define CPHCNTFETCHCONTACT_H

#include <e32base.h>

#include "cphcntasynctosync.h"
#include "MPhCntContactFetchObserver.h"

class MPhCntContactStores;
class MVPbkContactLink;
class MVPbkStoreContact;

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
NONSHARABLE_CLASS( CPhCntFetchContact ) : 
    public CPhCntAsyncToSync,
    public MPhCntContactFetchObserver
    {
public:
    static CPhCntFetchContact* NewL( MPhCntContactStores& aContactStores );

    static CPhCntFetchContact* NewLC( MPhCntContactStores& aContactStores );

    virtual ~CPhCntFetchContact();
    
    /**
     * Fetches contact, which aContactLink identifies.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */
    TInt FetchContact( 
        const MVPbkContactLink& aContactLink, 
        CPhCntContact*& aContact );
        
     /**
      * Fetches first contact from array of contact links.
      *
      * @since S60 v3.2
      * @param aContactLinkArray Contact link array as descriptor.
      * @param aContact Result of the fetch.
      * @return Error code.
      */
     TInt FetchContact( 
        const TDesC8& aContactLinkArray, 
        CPhCntContact*& aContact );
    
// from base class CPhCntAsyncToSync

    /**
     * From CPhCntAsyncToSync
     * ?description
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     */
     void DoMakeAsyncRequestL();

// from base class MPhCntContactFetchObserver

    /**
     * From MPhCntContactFetchObserver
     * ?description
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     */
     void ContactReceived( CPhCntContact* aContact, TInt aError );

private:

    CPhCntFetchContact( MPhCntContactStores& aContactStores );

    void ConstructL();

private: // data

    /**
     * Contact stores, where contact is fetched.
     * Not own.
     */
    MPhCntContactStores& iContactStores;
    
    /**
     * Received contact.
     * Not own.
     */
    CPhCntContact* iReceivedContact;
    
    /**
     * Link to the contact to be fetched.
     * Not own.
     */
    const MVPbkContactLink* iContactLink;

    };

#endif // CPHCNTFETCHCONTACT_H
