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
* Description:  Interface for finding voip contacts.
*
*/


#ifndef MPHCNTVOIPCONTACTFINDER_H
#define MPHCNTVOIPCONTACTFINDER_H

#include <e32std.h>

class MVPbkContactOperationBase;
class MVPbkContactFindObserver;

/**
 *  Interface for finding voip contacts.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
class MPhCntVoipContactFinder
    {
public:

    /**
     * Finds voip contacts, with given URI or partial URI. To cancel
     * the find, destroy the returned operation.
     * @since S60 v3.1
     * @param aSipURI Sip uri defining wanted matches.
     * @param aObserver Observer of the operation.
     * @return Operation.
     */
    virtual MVPbkContactOperationBase* FindVoipContactsL( 
        const TDesC& aSipURI,
        MVPbkContactFindObserver& aObserver ) const = 0;

protected:

    virtual ~MPhCntVoipContactFinder() {};
    
    };


#endif // MPHCNTVOIPCONTACTFINDER_H
