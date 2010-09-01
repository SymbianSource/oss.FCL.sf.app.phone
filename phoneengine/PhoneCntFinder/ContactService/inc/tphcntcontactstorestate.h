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
* Description:  Holds state of a contact store.
*
*/


#ifndef TPHCNTCONTACTSTORESTATE_H
#define TPHCNTCONTACTSTORESTATE_H

#include <e32std.h>
#include <TVPbkContactStoreUriPtr.h>

/**
 *  Holds contact store state.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.2
 */
class TPhCntContactStoreState
    {
public:

    /**
     * Constructor.
     *
     * @since S60 v3.2
     * @param aContactStoreUri Uri of the contact store.
     */
    TPhCntContactStoreState( TVPbkContactStoreUriPtr aContactStoreUri );

    /**
     * Determines if contact store is available.
     *
     * @since S60 v3.2
     * @return Contact store availability
     */
    TBool IsAvailable() const;
    
    /**
     * Gives the URI of the contact store.
     * 
     * @since S60 v3.2
     * @return Contact store URI.
     */
    const TVPbkContactStoreUriPtr& ContactStoreUri() const;
    
    /**
     * Set availability.
     *
     * @since S60 v3.2
     * @param aAvailable - ETrue, store is available.
     *                     EFalse, store is unavailable.
     */
    void SetAvailability( TBool aIsAvailable );
    
private: // data

    /**
     * Uri of the contact store.
     */
    TVPbkContactStoreUriPtr iContactStoreUri;
    
    /**
     * Contact store availability.
     */
    TBool iIsAvailable;

    };

#endif // TPHCNTCONTACTSTORESTATE_H
