/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Wrapper class for Central Repository handling.
*               Using this helper class is most helpful when one class needs
*               to observe state changes in several repositories.
*/

#ifndef DIALERCENREPWRAPPER_H
#define DIALERCENREPWRAPPER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <cenrepnotifyhandler.h>

// CALLBACK INTERFACE
NONSHARABLE_CLASS( MDialerCenRepObserver )
    {
public:
    virtual void HandleRepositoryChange( TUid aRepository, TUint32 aKey ) = 0;
    };

// FORWARD DECLARATIONS
class CRepository;

// CLASS DECLARATION

/**
 *  CDialerCenRepWrapper
 * 
 */
NONSHARABLE_CLASS( CDialerCenRepWrapper ): 
    public CBase, public MCenRepNotifyHandlerCallback
    {
public: // constructors and destructor

    /**
     * Destructor.
     */
    ~CDialerCenRepWrapper();

    /**
     * Two-phased constructor.
     */
    static CDialerCenRepWrapper* NewL( 
            MDialerCenRepObserver& aObserver, TUid aRepository );

    /**
     * Two-phased constructor.
     */
    static CDialerCenRepWrapper* NewLC( 
            MDialerCenRepObserver& aObserver, TUid aRepository );

private: // consturctors

    /**
     * Constructor for performing 1st stage construction
     */
    CDialerCenRepWrapper( 
            MDialerCenRepObserver& aObserver, TUid aRepository );

    /**
     * Symbian default constructor for performing 2nd stage construction
     */
    void ConstructL();

public: // from MCenRepNotifyHandlerCallback
    
    /**
     * @see MCenRepNotifyHandlerCallback
     */
    void HandleNotifyGeneric( TUint32 aId );
    
public: // new methods
    
    /**
     * Get integer value from the repository.
     * @see CRepository::Get
     */
    TInt Get( TUint32 aKey, TInt& aValue );
    
    // Getters for other data types may be added here if needed

private: // data

    /** Observer is notified on repository changes */
    MDialerCenRepObserver& iObserver;

    /** UID of the repository */
    TUid iRepositoryId;

    /** Repository handle. Owned. */
    CRepository* iRepository;

    /** Helper for reacting to repository changes. */
    CCenRepNotifyHandler* iCenRepNotifyHandler;
    };

#endif // DIALERCENREPWRAPPER_H
