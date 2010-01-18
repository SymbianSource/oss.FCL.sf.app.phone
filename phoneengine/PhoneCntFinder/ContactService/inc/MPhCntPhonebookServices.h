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
* Description:  Interface for phonebook services.
*
*/


#ifndef MPHCNTPHONEBOOKSERVICES_H
#define MPHCNTPHONEBOOKSERVICES_H

#include <e32std.h>
#include "CPhCntSingleItemFetch.h"

class CPhCntContactId;
class MPhCntServiceRequestParam;
class CPhCntContact;


/**
 *  Interface for phonebook services.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( MPhCntPhonebookServices )
    {

public:

    /**
     * Virtual destructor.
     * 
     * @since S60 v3.2
     */
    virtual ~MPhCntPhonebookServices() {};

    /**
     * Gets user selected phone number and contact id.
     *
     * @since S60 v3.2
     * @param aContactId Contact id of the contact, may be null
     * @param aPhoneNumber Phone number, may be null.
     * @leaves KErrCancel - User canceled the fetch
     *         Other system wide error codes.
     */
    virtual void GetUserSelectedPhoneNumberL( 
        CPhCntContactId*& aContactId, 
        HBufC*& aPhoneNumber ) = 0;

    /**
     * Gets user selected voip address and contact id.
     *
     * @since S60 v5.0
     * @param aContactId Contact id of the contact, may be null
     * @param aVoIPAddress voip address, may be null.
     * @leaves KErrCancel - User canceled the fetch
     *         Other system wide error codes.
     */
    virtual void GetUserSelectedVoIPAddressL( 
        CPhCntContactId*& aContactId, 
        HBufC*& aVoIPAddress ) = 0;
    
    /**
     * Gets user selected dtmf number and contact id.
     *
     * @since S60 v3.2
     * @param aContactId Contact id of the contact, may be null
     * @param aDtmfNumber Dtmf number, may be null.
     * @leaves KErrCancel - User canceled the fetch
     *         Other system wide error codes.
     */    
    virtual void GetUserSelectedDtmfNumberL(
        CPhCntContactId*& aContactId,
        HBufC*& aDtmfNumber ) = 0;
        
    /**
     * Create new contact to phonebook.
     *
     * @since S60 v3.2
     * @param aPhoneNumber Number of the new contact.
     */
    virtual void CreateNewContactL( const TDesC& aPhoneNumber ) = 0;
    
    /**
     * Adds phonenumber to existing contact.
     *
     * @since S60 v3.2
     * @param aPhoneNumber Phone number to be added to existing contact.
     */
    virtual void UpdateExistingContactL( const TDesC& aPhoneNumber ) = 0;  

	/**
	 * Gets a phone number from the contact.
	 * @param aContactLink Contact link.
	 * @param aCallType Number type.
	 * @param aNumber User selected phone number.
	 * @param aFieldLink Contact link (field link) to the number selected by user.
	 */
	virtual void GetPhoneNumberL(
		const TDesC8& aContactLink, 
		const CPhCntSingleItemFetch::TCallType aCallType,
    	HBufC*& aNumber,
    	HBufC8*& aFieldLink ) = 0;
    	
    /**
     * Gets contact selected by user. 
     *
     * @since S60 v3.2
     * @param aParam Parameters passed to AIW.
     * @return Contact, selected by user. Ownership contact.
     */   
    virtual CPhCntContact* GetUserSelectedContactLC( 
        MPhCntServiceRequestParam& aParam ) = 0;   
    
    /**
	 * Cancels outstanding request.
	 *
	 * @since S60 v3.2
	 */
    virtual void CancelRequest() = 0;

protected:

    };


#endif // MPHCNTPHONEBOOKSERVICES_H
