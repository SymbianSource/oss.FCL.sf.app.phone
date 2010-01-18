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

#include "mphcntcontactmatchstrategy.h"
#include "mphcntcontactstoreeventobserver.h"

class CVPbkContactManager;
class MVPbkContactFindObserver;
class CVPbkPhoneNumberMatchStrategy;
class CRepository;
class CVPbkContactStoreUriArray;
class CPhCntContactStoreUris;
class CCntRawPhoneNumberExtractor;

/**
 *  Strategy for matcing CS contacts.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntContactMatchStrategy ) :
    public CBase,
    public MPhCntContactMatchStrategy,
    private MCenRepNotifyHandlerCallback,
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
     */
    static CPhCntContactMatchStrategy* NewL(
        CVPbkContactManager& aContactManager,
        CPhCntContactStoreUris& aContactStoreUris,
        MVPbkContactFindObserver& aObserver,
        CVPbkPhoneNumberMatchStrategy::TVPbkPhoneNumberMatchFlags aMatchFlag );

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

// From base class MCenRepNotifyHandlerCallback

    /**
     * From base class MCenRepNotifyHandlerCallback
     * This callback method is used to notify the client about
     * changes for string value keys, i.e. key type is EStringKey.
     *
     * @param aId Id of the key that has changed.
     * @param aNewValue The new value of the key.
     */
    void HandleNotifyString( TUint32 aId, const TDesC16& aNewValue );


// From base class MPhCntContactStoreEventObserver

    /**
     * From MPhCntContactStoreEventObserver
     *
     * @since S60 v3.2
     * @see MPhCntContactStoreEventObserver.
     */
    void ContactStoreAvailabilityChanged();

    /**
     * Removes postfix from aNumber and starts the matching.
     */
    void MatchL( const TDesC& aNumber );

    /**
     * Method to ease unit testing. Creates the actual instance of iMatchStrategy.
     */
    virtual TInt DoCreateMatchStrategy();
    
    /**
     * Makes the actual matching request using number
     * Declared virtual to ease unit testing.
     */
    virtual void DoMatchL( const TDesC& aNumber );

protected:

    CPhCntContactMatchStrategy(
        CVPbkContactManager& aContactManager,
        CPhCntContactStoreUris& aContactStoreUris,
        MVPbkContactFindObserver& aObserver,
        CVPbkPhoneNumberMatchStrategy::TVPbkPhoneNumberMatchFlags aMatchFlag );

    void ConstructL();

protected: // data
    CVPbkPhoneNumberMatchStrategy::TVPbkPhoneNumberMatchFlags iMatchFlag;
	
private: // data

    /**
     * Match strategy.
     * Own.
     */
    CVPbkPhoneNumberMatchStrategy* iMatchStrategy;

    /**
     * Central repository instance, for getting
     * number of numbers used with contact matching.
     * Own.
     */
    CRepository* iCenRepSession;

    /**
     * For getting notifications of number of numbers
     * used with contact matching changes.
     * Own.
     */
    CCenRepNotifyHandler* iCenRepNotifyHandler;

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

    };


#endif // CPHCNTCONTACTMATCHSTRATEGY_H
