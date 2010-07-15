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
* Description:  Matcher implementation
*
*/

#ifndef CPHCNTCNTMATCHERIMPL_H
#define CPHCNTCNTMATCHERIMPL_H

#include <e32base.h>
#include "CPhCntMatcher.h"
#include "MVPbkContactLink.h"

class MPhCntContactStores;
class CPhCntContactManager;
class CPhCntContact;
class CPhCntMatchContact;
class CPhCntFetchContact;
class MVPbkContactLink;
class CPhCntFoundContacts;
class CCntNumberParser;
class MPhoneCntPbkOwner;
class MPhCntContactMatchStrategy;
class CVPbkContactStoreUriArray;


/**
 * CPhCntMatcher implementation.
 *
 * @since S60 v3.1
 * @lib PhoneCntFinder.lib.
 */
NONSHARABLE_CLASS( CPhCntMatcherImpl ) : public CPhCntMatcher
    {
public:

    /**
     * Static constructor.
     *
     * @since S60 v3.1
     * @param aOwner Phonebook owner.
     */
    static CPhCntMatcherImpl* NewL( const MPhoneCntPbkOwner& aOwner );

    /**
     * Destructor.
     *
     * @since S60 v3.1
     */
    ~CPhCntMatcherImpl();

// From base class CPhCntMatcher

    /**
     * From base class CPhCntMatcher
     *
     * @since S60 v3.1
     * @see CPhCntMatcher.
     */
    TInt MatchNumber(
        MPhCntMatch*& aMatch,
        const TDesC& aTelNumber );

    /**
     * From base class CPhCntMatcher
     *
     * @since S60 v3.1
     * @see CPhCntMatcher.
     */
    TInt MatchNumber(
        MPhCntMatch*& aMatch,
        const TDesC& aTelNumber,
        const CPhCntContactId& aContactId );


// NOT USED BY ANYONE?
        /**
    * Match number to phonebook.
    * @param aMatch Found match, owership tranferred. NULL if not found any.
    * @param aTelNumber Number to match against.
    * @param aContactId Current contact.
    * @param aFieldId Current field id (index from CnmModel);
    * @return Error code: KErrNone - one match found
    *                     KErrNotFound - no matches found
    *                     KErrAlreadyExists - several matches -> no match
    *                     KErrUnderflow - Number too short for matching
    *                     KErrAccessDenied - Can't get phonebook instance
    *                     other - normal Symbian OS error
    */
    TInt MatchNumber(
        MPhCntMatch*& /*aMatch*/,
        const TDesC& /*aTelNumber*/,
        const CPhCntContactId& /*aContactId*/,
        MPhCntMatch::TNumberType /*aNumberType*/ )
        {
        return KErrNotFound;
        };

    /**
    * Match number to phonebook.
    * @param aMatch Found match, owership tranferred. NULL if not found any.
    * @param aMatchString Number to match against.
    * @return Error code: KErrNone - one match found
    *                     KErrNotFound - no matches found
    *                     other - normal Symbian OS error
    */
    TInt MatchVoipNumber(
        MPhCntMatch*& aMatch,
        const TDesC& aMatchString,
        TBool aAllowUserNameMatch,
        MDesCArray* aContactStoreUris,
        TInt aCharsForMatching = 0 );

    /**
    * From CPhCntMatcher, gets VoIP call contact info from phonebook
    * by contact id.
    * @since Series60 3.0
    * @param aMatch for found match, owership tranferred. Empty if not found.
    * @param aMatchString Number to match against.
    * @param aContactId for current contact.
    * @return Error code: KErrNone - VoIP call contact found
    *                     KErrNotFound - no VoIP call contact found
    *                     other - normal Symbian OS error
    */
    TInt MatchVoipNumber(
        MPhCntMatch*& aMatch,
        const TDesC& aMatchString,
        const CPhCntContactId& aContactId );


    /**
     * From CPhCntMatcher
     * Determines if contact has other type of numbers than
     * voip numbers.
     *
     * @since S60 v3.2.
     * @param aContactId Id of the contact.
     * @return ETrue - CS numbers found from contact.
     *         EFalse - No CS numbers found from contact.
     */
    TBool HasCSNumbers(
        const CPhCntContactId& aContactId );

private:

    /**
     * Tries to get a contact with known number.
     *
     * @since S60 v3.1
     * @param aMatch Contact
     * @param aTelNumber Phone number.
     * @return Error code.
     */
    TInt GetContactL(
        MPhCntMatch*& aMatch,
        const TDesC& aTelNumber );

    /**
     * Match contacts against product specific stores.
     *
     * @since S60 v3.2
     * @param aLinkArray Array of contacts to match
     * @return Error code.
     */
    TInt MatchFromAdditionalStore( const MVPbkContactLinkArray& aLinkArray ) const;


protected:

    CPhCntMatcherImpl( const MPhoneCntPbkOwner& aOwner );

    void ConstructL();

    /**
     * Delayed on-demand based construction (grabs leaves).
     * Should be called from functions where a member attribute
     * (those constructed here) is referenced.
     */
    TInt CreateMatcher();

    /**
     * Delayed on-demand based construction (leaving).
     */
    void DoCreateMatcherL();

    /**
     * Fetches contact pointed by aContactLink from
     * contact stores.
     *
     * @since S60 v3.1
     * @param aMatch Fetched contact. Is null if contact was not found.
     * @param aContactLink Link to the contact.
     * @param aOrigNumber Phone number that was used
     * to find the contact.
     * @return Error code.
     */
    TInt FetchContact(
        CPhCntContact*& aMatch,
        const MVPbkContactLink& aContactLink,
        const TDesC& aOrigNumber );



protected: // Data

    /**
     * Already found contacs.
     * Own.
     */
    CPhCntFoundContacts* iFoundContacts;

    /**
     * Contact mathching operation.
     * Own.
     */
    CPhCntMatchContact* iMatchContact;


    /**
     * Contact fetching operation.
     * Own.
     */
    CPhCntFetchContact* iFetchContact;


    /**
     * Contact manager.
     * Not own.
     */
    CPhCntContactManager& iContactManager;

private: // Data

    /**
     * Contact stores
     * Own.
     */
    MPhCntContactStores* iContactStores;

    /**
     * Owner of phone book.
     * Not own.
     */
    const MPhoneCntPbkOwner& iPbkOwner;

    /**
     * Match strategy used for CS calls.
     * Own.
     */
    MPhCntContactMatchStrategy* iCSMatchStrategy;

    };

#endif // CPHCNTCNTMATCHERIMPL_H
