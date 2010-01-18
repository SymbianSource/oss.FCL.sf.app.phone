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
* Description:  Service for fetching single contact field.
*
*/


#ifndef CPHCNTSINGLEITEMFETCHSERVICE_H
#define CPHCNTSINGLEITEMFETCHSERVICE_H

#include <e32base.h>
#include "CPhCntSingleItemFetch.h"

class MPhCntContactStores;
class MPhCntPhonebookServices;
class MPhCntContactManager;
class CPhCntServiceRequestParamFactory;

NONSHARABLE_CLASS( CPhCntSelectionImpl ) : public CPhCntSelection
    {
public:
    
    /**
     * Static constructor.
     * Takes ownership of the parameters.
     */
    static CPhCntSelectionImpl* NewL( 
        const TDesC& aNumber,
        const CPhCntContactId& aContactId );
        
    ~CPhCntSelectionImpl();
    
    /**
     * From CPhCntSelection
     * @since s60 v3.2
     * @see CPhCntSelection
     */
    const TDesC& Number() const;

    /**
     * From CPhCntSelection
     * @since s60 v3.2
     * @see CPhCntSelection
     */
    const CPhCntContactId& ContactId() const;
    
private:

    CPhCntSelectionImpl();
    
    void ConstructL(
        const TDesC& aNumber,
        const CPhCntContactId& aContactId );
        
private: // Data

    /**
     * Number.
     * Own.
     */
    HBufC* iNumber;

    /**
     * Contact identifier.
     * Own.
     */
    CPhCntContactId* iContactId;
    
    };

/**
 * Service for fetching single contact item from phonebook.
 *
 * @since S60 v3.1
 * @lib PhoneCntFinder.lib
 */
NONSHARABLE_CLASS( CPhCntSingleItemFetchService ): public CPhCntSingleItemFetch
    {
public:
   	
   	/**
   	 * Static constructor.
   	 *
   	 * @since S60 v3.1
   	 * @param aServices Phonebooks services
   	 * @param aContactManager Contact manager
   	 */     
    static CPhCntSingleItemFetchService* NewL(
        MPhCntContactStores& aServices,
        MPhCntContactManager& aContactManager );

    /**
     * Destructor.
     * 
     * @since S60 v3.1
     */
    ~CPhCntSingleItemFetchService();
    
// From base class CPhCntSingleItemFetch

    /**
    * From base class CPhCntSingleItemFetch
    *
    * @see CPhCntSingleItemFetch
    * @since S60 3.1
    */
    TInt FetchLD( TFetchParams& aParams );
    
    /**
	* From base class CPhCntSingleItemFetch
    *
    * @see CPhCntSingleItemFetch
    * @since S60 3.1
	*/
    TInt FetchPhoneNumberLD( 
    	const TDesC8& aContactLink, 
    	const CPhCntSingleItemFetch::TCallType aCallType,
    	HBufC*& aNumber,
    	HBufC8*& aFieldLink );
    
    /**
	 * From base class CPhCntSingleItemFetch
     *
     * @see CPhCntSingleItemFetch
     * @since S60 3.2
	 */	
    CPhCntSelection* SelectPhoneNumberLD();
        
private:

    /**
    * Sets return values (aParams) for client when fetching
    * is completed.
    * 
    * @since S60 3.1
    * @param aParams Parameters from client.
    * @returns Possible error value
    */
    TInt SetFetchParams( TFetchParams& aParams,
        const TDesC& aPhoneNumber ) const;
        
protected:
        
    CPhCntSingleItemFetchService();
    
    void ConstructL( 
        MPhCntContactStores& aContactStores,
        MPhCntContactManager& aContactManage );
        
protected:

    /**
     * Service to fetch the phone number.
     * Own.
     */
    MPhCntPhonebookServices* iServices;
    
    /** 
     * Indication if this is destroyed.
     */
    TBool* iDestroyed;
    
    /**
     * Factory to create request params.
     * Own.
     */
    CPhCntServiceRequestParamFactory* iParamFactory;

    };

#endif // CPHCNTSINGLEITEMFETCHSERVICE_H