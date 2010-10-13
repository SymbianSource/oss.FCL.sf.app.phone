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
* Description:  Contact matching strategy
*
*/


#ifndef CPHCNTCONTACTMATCHSTRATEGY_H
#define CPHCNTCONTACTMATCHSTRATEGY_H

#include <e32base.h>
#include <cenrepnotifyhandler.h>
#include <CVPbkPhoneNumberMatchStrategy.h>
#include <cntdb.h>  // KBestMatchingPhoneNumbers

#include "mphcntcontactmatchstrategy.h"
#include "mphcntcontactstoreeventobserver.h"

#include "MPhCntContactManager.h"

class CVPbkContactManager;
class MVPbkContactFindObserver;
class CVPbkPhoneNumberMatchStrategy;
class CRepository;
class CVPbkContactStoreUriArray;
class CPhCntContactStoreUris;
class CCntRawPhoneNumberExtractor;

// Digit count used to match CS number.
const TInt KPhCntMatchMin = 7;
const TInt KPhCntMatchMax = 11;
const TInt KPhCntMatchDefault = KBestMatchingPhoneNumbers;

/**
 *  Strategy for matcing CS contacts.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntContactMatchStrategy ) :
    public CBase,
    public MPhCntContactMatchStrategy,    
    private MPhCntContactStoreEventObserver
    {
public:

    /**
     * Static contstructor.
     *
     * @since S60 v3.1
     * @param aContactManager Contact manager.
     * @param aUriArray Array of contact store URI's used.
     * @param aObserver Observer for finding contacts.
     * @param aRemoveDuplicatesStrategy Contact dupe removal strategy
     */
    static CPhCntContactMatchStrategy* NewL(
        CVPbkContactManager& aContactManager,
        CPhCntContactStoreUris& aContactStoreUris,
        MVPbkContactFindObserver& aObserver,
        MPhCntContactManager::TDuplicateRemovalStrategy aRemoveDuplicatesStrategy );

    /**
     * Destructor.
     *
     * @since S60 v3.1
     */
    virtual ~CPhCntContactMatchStrategy();

// from base class MPhCntContactMatchStrategy

    /**
     * From MPhCntContactMatchStrategy
     *
     * @since S60 v3.1
     * @see MPhCntContactMatchStrategy
     */
    void FindMatchesL( const TDesC& aPhoneNumber );

private:    
    
    /**
     * Creates contact match strategy with given numbers
     * of digits.
     *
     * @since S60 v3.1
     * @return Error code.
     */
    TInt CreateContactMatchStrategy();

// From base class MPhCntContactStoreEventObserver

    /**
     * From MPhCntContactStoreEventObserver
     *
     * @since S60 v3.2
     * @see MPhCntContactStoreEventObserver.
     */
    void ContactStoreAvailabilityChanged();

    /**
     * Method to ease unit testing. Creates the actual instance of iMatchStrategy.
     */
    virtual TInt DoCreateMatchStrategy();
        
    /**
     * Removes postfix from aNumber    
     */
    TDesC* RemoveExtraCharactersLC( const TDesC& aNumber );

protected:

    CPhCntContactMatchStrategy(
        CVPbkContactManager& aContactManager,
        CPhCntContactStoreUris& aContactStoreUris,
        MVPbkContactFindObserver& aObserver,
        MPhCntContactManager::TDuplicateRemovalStrategy aRemoveDuplicatesStrategy );

    void ConstructL();

    TUint32 FillMatchFlags() const;
    
    virtual TInt ReadMatchDigitsValueL(); 
    
    /**
     * Makes the actual matching request using number
     * Declared virtual to ease unit testing.
     */
    virtual void DoMatchL( const TDesC& aNumber );   
    
    /**
     * Enables to inject match strategy to ease unit testing
     */
    void SetVPbkPhoneNumberMatchStrategy( 
            CVPbkPhoneNumberMatchStrategy* aMatchStrategy );
    
private: // data

    /**
     * Match strategy.
     * Own.
     */
    CVPbkPhoneNumberMatchStrategy* iMatchStrategy;
        
     /**
      * Contact manager is needed with match strategy.
      * Not own.
      */
    CVPbkContactManager& iContactManager;

    /**
     * Gives active contact store uris.
     * Not own.
     */
    CPhCntContactStoreUris& iContactStoreUris;

     /**
     * Observer of matching.
     * Now own.
     */
    MVPbkContactFindObserver& iObserver;

     /**
      * URI's of the active contact stores, where contacts are
      * looked from.
      * Own.
      */
    CVPbkContactStoreUriArray* iUriArray;

    /**
     * Number of digits used with matching.
     */
    TInt iNumberOfDigits;

    /**
     * Number extractor.
     * Own.
     */
    CCntRawPhoneNumberExtractor* iNumberExtractor;
    
    MPhCntContactManager::TDuplicateRemovalStrategy iRemoveDuplicatesStrategy;
    
    };

#endif // CPHCNTCONTACTMATCHSTRATEGY_H

