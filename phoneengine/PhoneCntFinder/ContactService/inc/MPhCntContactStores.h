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
* Description:  Contact match strategy interface.
*
*/

#ifndef MPHCNTCONTACTSTORES_H
#define MPHCNTCONTACTSTORES_H

class MVPbkContactLink;
class MPhCntContactFetchObserver;
class CVPbkContactLinkArray;
class CVPbkPhoneNumberMatchStrategy;
class MVPbkContactFindObserver;

/**
 * Contact stores interface.
 *
 * @lib PhoneCntFinder.lib
 * @since S60 v3.1
 */
NONSHARABLE_CLASS( MPhCntContactStores ) 
    {
    
public:

    /**
     * Destructor.
     *
     * @since v3.1
     */
    virtual ~MPhCntContactStores() {};
    
    /**
     * Determines if request is pending.
     * @since S60 v3.2
     */
    virtual TBool IsRequestActive() const = 0;
    
    /**
     * Cancels outstanding request if any.
     */
    virtual void CancelRequest() = 0;
    
    /**
     * Fetches contact, which is pointed by contact link from 
     * currently open contact stores. Observer is notified
     * after the contact has been fetched or fetch was not
     * successfull. Note that new fetch should not be
     * started before the existing fetch has completed.
     *
     * @since S60 v3.1
     * @param aLink Link to the contact.
     * @param aObserver Observer of the fetch.
     */
     
    virtual void FetchContactL( const MVPbkContactLink& aLink, 
        MPhCntContactFetchObserver& aObserver ) = 0;
    
    /**
     * Creates contact link array from descriptor, which 
     * defines contact links.
     *
     * @since S60 v3.1
     * @param aContactLinks Contact links in descriptor.
     * @return Array of contactlinks, ownership given.
     */
    virtual CVPbkContactLinkArray* CreateContactLinkArrayL( 
        const TDesC8& aContactLinks ) = 0;
    
    /**
     * Fetches first contact in contact links, which are defined
     * by aContactLinks. Observer is notified
     * after the contact has been fetched or fetch was not
     * successfull. Note that new fetch should not be
     * started before the existing fetch has completed.
     *
     * @since S60 v3.1
     * @param aContactLinks Links to a contacts.
     * @param aObserver Observer of the fetch.
     */
    virtual void FetchFirstContactL( const TDesC8& aContactLinks,
        MPhCntContactFetchObserver& aObserver ) = 0;
        
    };

#endif