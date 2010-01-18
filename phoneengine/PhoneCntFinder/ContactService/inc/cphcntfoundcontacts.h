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
* Description:  Stores found contacts and keeps track of the references.
*
*/



#ifndef CPHCNTFOUNDCONTACTS_H
#define CPHCNTFOUNDCONTACTS_H

#include <e32base.h>

class CPhCntContact;

/**
 * Responsible to hold found contacts. Takes the ownership of the contacts.
 * The contacts must be removed using MPhCntContact::Release when contact
 * no longer needed. 
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntFoundContacts ) : public CBase
    {
public:

    static CPhCntFoundContacts* NewL();

    static CPhCntFoundContacts* NewLC();

    virtual ~CPhCntFoundContacts();

    /**
     * Adds contact. Takes the ownership.
     *
     * @since S60 v3.1
     * @param aContact Contact
     * @param aOriginalNumber Number used to match aContact.
     */
    void AddL( CPhCntContact* aContact, const TDesC& aOriginalNumber );
    
    /**
    * Removes and deletes contact. Even if contact
    * is not found, aContact is deleted.
    * 
    * @since S60 v3.1
    * @param aContact Contact to be deleted.
    */
    void Remove( CPhCntContact* aContact );
    
    /**
     * Finds a contact which has been stored here.
     *
     * @since S60 v3.1
     * @param aTelNumber Number used for matching contact.
     * @return Found contact or NULL if contact not found.
     * Ownership is NOT given to client. CPhCntContact
     * must be destroyed using Release method of CPhCntContact.
     */
    CPhCntContact* FindContact( const TDesC& aTelNumber );
    
    /**
     * Finds a contact which has been stored here.
     * 
     * @since S60 v3.1
     * @param aTelNumber Number used for matching contact.
     * @param aContactLink ContactLink of the wanted contact.
     * @return Found contact or NULL if contact not found.
     * Ownership is NOT given to client. CPhCntContact
     * can be destroyed using Release method of CPhCntContact.
     */
    CPhCntContact* FindContact( 
        const TDesC& aTelNumber,
        const MVPbkContactLink& aContactLink );
        
    /**
     * Finds contact which has been stored here.
     *
     * @since s60 v3.1
     * @param aContactLink Contact link of wanted contact.
     * @return Found contact or NULL if contact not found.
     * Ownership is NOT given to client. CPhCntContact
     * can be destroyed using Release method of CPhCntContact. 
     */
    CPhCntContact* FindContact(const MVPbkContactLink& aContactLink );
    
private:

    CPhCntFoundContacts();

    void ConstructL();

private: // data

    /**
     * Array of contacts already found.
     * Own. 
     */
    RPointerArray<CPhCntContact> iFoundContacts;
    };


#endif // CPHCNTFOUNDCONTACTS_H
