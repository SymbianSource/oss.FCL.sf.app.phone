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
* Description:  Finds voip contacts from phonebook
*
*/


#ifndef CPHCNTVOIPCONTACTMATCHSTRATEGY
#define CPHCNTVOIPCONTACTMATCHSTRATEGY

#include <e32base.h>
#include <MVPbkContactFindObserver.h>

#include "mphcntcontactmatchstrategy.h"

class MPhCntVoipContactFinder;
class MVPbkContactOperationBase;

/**
 *  Finds voip contacts from phonebook
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntVoipContactMatchStrategy ): 
    public CBase, 
    public MPhCntContactMatchStrategy,
    private MVPbkContactFindObserver
    {
public:
    
    static CPhCntVoipContactMatchStrategy* NewL(
        MPhCntVoipContactFinder& aVoipContactFinder,
        MVPbkContactFindObserver& aObserver );

    static CPhCntVoipContactMatchStrategy* NewLC(
        MPhCntVoipContactFinder& aVoipContactFinder,
        MVPbkContactFindObserver& aObserver );

    virtual ~CPhCntVoipContactMatchStrategy();

// from base class MPhCntContactMatchStrategy

    /**
     * From base class MPhCntContactMatchStrategy
     * Starts finding contacts, which contact field matches with aMatchString.
     *
     * @since S60 v3.1
     * @param aMatchString String which is matched against contact fields.
     */
    void FindMatchesL( const TDesC& aMatchString );

private:

    CPhCntVoipContactMatchStrategy( 
        MPhCntVoipContactFinder& aVoipContactFinder,
        MVPbkContactFindObserver& aObserver );

    void ConstructL();
    
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
     *                 the start of the function, ie. in case the function
     *                 leaves the results must be destroyed. The find
     *                 operation can be destroyed at the end of this callback.
     */
    void FindCompleteL( MVPbkContactLinkArray* aResults );

    /** 
     * From base class MVPbkContactFindObserver
     * Called in case the find fails for some reason. The find operation
     * can be destroyed at the end of this callback.
     * 
     * @Since S60 v3.1
     * @param aError One of the system wide error codes.
     *        KErrNotReady if store is not ready (not open or unavailable).
     */
    void FindFailed( TInt aError );

private: // data

    /**
     * Voip contact finder.
     * Not own.
     */
    MPhCntVoipContactFinder& iVoipContactFinder;
    
    /**
     * Find operation.
     * Own.
     */
    MVPbkContactOperationBase* iFindOperation;
    
    /**
     * Observer of the operation.
     * Not own.
     */
    MVPbkContactFindObserver& iObserver;

    };
#endif // CPHCNTVOIPCONTACTMATCHSTRATEGY
