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
* Description:  Matches contact to phonenumber from phonebook.
*
*/


#ifndef CPHCNTMATCHCONTACT_H
#define CPHCNTMATCHCONTACT_H

#include <e32base.h>

#include "cphcntcontactlinkarrayfetch.h"

class MVPbkContactLinkArray;
class MPhCntContactManager;
class MPhCntContactMatchStrategy;


/**
 *  Matches contacts to phone number.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntMatchContact ): public CPhCntContactLinkArrayFetch
    {
public:

    /**
     * Static constructor.
     *
     * @since S60 v3.1
     */
    static CPhCntMatchContact* NewL();

    /**
     * Static constructor.
     *
     * @since S60 v3.1
     */
    static CPhCntMatchContact* NewLC();

    /**
     * Destructor.
     */
    virtual ~CPhCntMatchContact();

    /**
     * Finds contacts, which telephone number matches with aTelNumber.
     * Note that aContactLinkArray ownership is not given. Client has
     * to call MVPbkContactLink::CloneLC in order to take the ownership of the
     * link in array. If error returned is KErrNone then there is at least
     * one entry in aContactLinkArray.
     *
     * @since S60 v3.1
     * @param aTelNumber Number, which is matched with contacts.
     * @Param aContactLinkArray Array of found contacts.
     *        Ownership of array nor links inside is NOT given to caller.
     * @param aStrategy Matching strategy used for finding matches.
     * @return Error code, KErrNotFound, if contact not found
     *                     KErrAlreadyExists if many contacts found.
     */
    TInt MatchContact( const MVPbkContactLinkArray*& aContactLinkArray, 
                        const TDesC& aTelNumber,
                        MPhCntContactMatchStrategy& aStrategy );
    
protected:

// from base class CPhCntAsyncToSync

    /**
     * From CPhCntAsyncToSync
     * Makes the asynchronous request, which is converted to synchronous.
     * @since S60 v3.1
     */
    void DoMakeAsyncRequestL();

private:

    CPhCntMatchContact();

    void ConstructL();


private: // data

    /**
     * Contact matching strategy
     * Not own.
     */
    MPhCntContactMatchStrategy* iMatchStrategy;

    /**
     * Number to match contacts.
     */
    TPtrC iPhoneNumber;

    };



#endif // CPHCNTMATCHCONTACT_H
