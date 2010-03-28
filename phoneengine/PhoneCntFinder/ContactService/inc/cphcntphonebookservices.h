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
* Description:  Phonebook services wrapper.
*
*/


#ifndef CPHCNTPHONEBOOKSERVICES_H
#define CPHCNTPHONEBOOKSERVICES_H

#include <e32base.h>

#include "MPhCntPhonebookServices.h"

class MPhCntService;
class MPhCntContactStores;
class CPhCntFetchContact;
class CPhCntContactId;
class MPhCntContactManager;
class MPhCntServiceRequestParam;
class CPhCntContact;
class MVPbkContactLinkArray;
class CPhCntServiceRequestParamFactory;

/**
 *  Phonebook services.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CPhCntPhonebookServices )
    : public CBase, 
    public MPhCntPhonebookServices
    {
public:

    /**
     * Static constructor.
     *
     * @since S60 v3.2
     * @param aContactStores Reference of contact stores.
     * @param aContactManager Reference to contact manager.
     */
    static CPhCntPhonebookServices* NewL( 
        MPhCntContactStores& aContactStores,
        MPhCntContactManager& aContactManager );

    /**
     * Destructor.
     *
     * @since S60 v3.2
     */     
    virtual ~CPhCntPhonebookServices();

// From base class MPhCntPhonebookServices

    /**
     * From MPhCntPhonebookServices
     *
     * @since S60 v3.2
     * @see MPhCntPhonebookServices
     */
    void GetUserSelectedPhoneNumberL( 
        CPhCntContactId*& aContactId, 
        HBufC*& aPhoneNumber );

    /**
     * From MPhCntPhonebookServices
     *
     * @since S60 v3.2
     * @see MPhCntPhonebookServices
     */
    void GetUserSelectedVoIPAddressL( 
        CPhCntContactId*& aContactId, 
        HBufC*& aVoIPAddress );
    
    /**
     * From MPhCntPhonebookServices
     *
     * @since S60 v3.2
     * @see MPhCntPhonebookServices
     */    
    void GetUserSelectedDtmfNumberL(
        CPhCntContactId*& aContactId,
        HBufC*& aDtmfNumber );
        
    /**
     * From MPhCntPhonebookServices
     *
     * @since S60 v3.2
     * @see MPhCntPhonebookServices
     */
    void CreateNewContactL( const TDesC& aPhoneNumber );
    
    /**
     * From MPhCntPhonebookServices
     *
     * @since S60 v3.2
     * @see MPhCntPhonebookServices
     */
    void UpdateExistingContactL( const TDesC& aPhoneNumber );
    
    /**
     * From MPhCntPhonebookServices
     *
     * @since S60 v3.2
     * @see MPhCntPhonebookServices
     */
    void GetPhoneNumberL(
	    const TDesC8& aContactLink,
	    const CPhCntSingleItemFetch::TCallType aCallType,
    	HBufC*& aNumber,
    	HBufC8*& aFieldLink );    
    	
    /**
     * From MPhCntPhonebookServices
     *
     * @since S60 v3.2
     * @see MPhCntPhonebookServices
     */   
    CPhCntContact* GetUserSelectedContactLC( 
        MPhCntServiceRequestParam& aParam );
    
    /**
     * From MPhCntPhonebookServices
     *
     * @since S60 v3.2
     * @see MPhCntPhonebookServices
     */     
    void CancelRequest();

protected:

    CPhCntPhonebookServices( MPhCntContactManager& aContactManager );

    void ConstructL( 
        MPhCntContactStores& aContactStores );
    
private:

    /**
     * Gets link to a contact, that user selects.
     * 
     * @since S60 v3.2
     * @param aParam Service request parameters.
     * @return Array of contact links.
     */
    HBufC8* GetContactLinksLC(
        MPhCntServiceRequestParam& aParam );
    
       
    /**
     * Sets result to client.
     *
     * @since S60 v3.2
     * @param aReceivedNumber Received number.
     * @param aReceivedContactId Received contact id.
     * @param aClientResultNumber Number given to client.
     * @param aClientResultContactId Id given to client.
     */
    void SetResultsL( 
        const TDesC& aReceivedNumber, 
        const CPhCntContactId* const aReceivedContactId,
        HBufC*& aClinetResultNumber,
        CPhCntContactId*& aClientResultContactId ) const;
     
    /**
     * Gets user selected number from phonebook.
     *
     * @param aContactId Contact id of the contact where the number is got.
     * @param aNumber Number user selected.
     * @param aDTMFWanted ETrue - User is selecting DTMF or phone number.
     *                    EFalse - User is selecting phone number.
     */
    void GetUserSelectedNumberL(
        CPhCntContactId*& aContactId,
        HBufC*& aNumber, TBool aDTMFWanted );
     
    /**
     * Creates CPhCntService if needed.
     * 
     * @since S60 v3.2
     */
    void CreateCntServiceL();

    /**
     * Gets user selected voip address from phonebook.
     *
     * @param aContactId Contact id of the contact where the number is got.
     * @param aVoIPAddress Address user selected.
     */
    void GetUserSelectedAddressL(
        CPhCntContactId*& aContactId,
        HBufC*& aVoIPAddress );

protected: // data

    /**
     * AIW service wrapper.
     * Own.
     */
    MPhCntService* iService;
    
    /**
     * For fetching contact from phonebook.
     * Own.
     */
    CPhCntFetchContact* iFetchContact;
    
    /**
     * Contact manager.
     * Not own.
     */
    MPhCntContactManager& iContactManager;
    
    /**
     * Factory to create params.
     * Own.
     */
    CPhCntServiceRequestParamFactory* iParamFactory;
    
    };

#endif // CPHCNTPHONEBOOKSERVICES_H
