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
* Description:  Implements VoIP specific matching methods.
*
*/


#ifndef CPHCNTMATCHERVOIPIMPL_H
#define CPHCNTMATCHERVOIPIMPL_H

#include "CPhCntMatcherImpl.h"

class CPhCntVoipContactMatchStrategy;
class CPhCntStoreLoaderImpl;

/**
 *  Implements VoIP specific matching methods.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CPhCntMatcherVoIPImpl ) : public CPhCntMatcherImpl
    {
public:
    static CPhCntMatcherVoIPImpl* NewL( const MPhoneCntPbkOwner& aOwner );

    virtual ~CPhCntMatcherVoIPImpl();
    
// From base class CPhCntMatcher

    /**
     * From CPhCntMatcher
     * Match number to phonebook.
     * @since S60 v3.2
     * @param aMatch Found match, owership tranferred. NULL if not found any.
     * @param aNumber Number to match against.
     * @param aAllowUserNameMatch If true user name match is allowed.
     * @param aCharsForMatching Characters for user name match. Match is started from
     *                          the rigth of the user name part. (see CS call match)
     * @return Error code: KErrNone - one match found
     *                     KErrNotFound - no matches found
     *                     other - normal Symbian OS error
     */    
    TInt MatchVoipNumber(
        MPhCntMatch*& aMatch,
        const TDesC& aNumber,
        TBool aAllowUserNameMatch,
        MDesCArray* aContactStoreUris,
        TInt aCharsForMatching = 0 );
        
    /**
     * From CPhCntMatcher
     * From CPhCntMatcher, gets VoIP call contact info from phonebook
     * by contact id.
     * @since Series60 3.2
     * @param aMatch for found match, owership tranferred. Empty if not found.
     * @param aContactId for current contact.
     * @return Error code: KErrNone - VoIP call contact found
     *                     KErrNotFound - no VoIP call contact found
     *                     other - normal Symbian OS error
     */
    TInt MatchVoipNumber(
        MPhCntMatch*& aMatch,
        const CPhCntContactId& aContactId );

    
    /**
     * From CPhCntMatcher
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
    
    CPhCntMatcherVoIPImpl( const MPhoneCntPbkOwner& aOwner );
    
    void ConstructL();
    
    /**
     * Determines if contact has other numbers than VoIP numbers.
     * 
     * @since S60 v3.2
     */
    TBool HasCSNumbers( 
        const CPhCntContact* const aContact );

    /**
     * From CPhCntMatcher. Used for delayed construction.
     * 
     * @since S60 v3.2
     */
    TInt CreateMatcher();

private: // data

      
    /**
     * Match strategy used for Voip calls.
     * Own.
     */
    CPhCntVoipContactMatchStrategy* iVoipMatchStrategy;

    /**
     * Used for loading service specific contact stores 
    */
    CPhCntStoreLoaderImpl* iContactStoreLoader;
    
    };

#endif // CPHCNTMATCHERVOIPIMPL_H
