/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service to show dialogs for user to select contact data.
*
*/


#ifndef CPHCNTCONTACTDATASELECTIONIMPL_H
#define CPHCNTCONTACTDATASELECTIONIMPL_H

#include <e32base.h>

#include "cphcntcontactdataselection.h"
#include "tphcntcontactlinkresult.h"
#include "mphcntaiwservice.h"
#include "MPhCntContactFetchObserver.h"
#include "MPhCntMatch.h"

class MPhCntServiceRequestParamFactory;
class MPhCntServiceRequestParam;
class MPhCntContactStores;

/**
 * User selected data from phonebook services.
 *
 * @lib PhoneCntFinder
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPhCntSelectedDataImpl ) : public CPhCntSelectedData
    {
public:
    
    /**
     * Static constructor.
     *
     * @param aContact Selected contact. Note that aContact needs to
     * be valid as long NewL is executed. So CPhCntContact can be 
     * deleted after call to this NewL
     */
    static CPhCntSelectedDataImpl* NewL( CPhCntContact& aContact );
    
    ~CPhCntSelectedDataImpl();
    
// from base class CPhCntSelectedData

    const TDesC& Data() const;
    
    /**
     * Phonebook's link, which points to the selected
     * data.
     */
    const TDesC8& FieldLink() const;
    
    /**
     * Number type
     */
    MPhCntMatch::TNumberType NumberType() const;
   
    
private:
    CPhCntSelectedDataImpl();
    
    void ConstructL( CPhCntContact& aContact );
    
private: // Data

    /**
     * Own.
     */
    HBufC8* iFieldLink;
    
    /**
     * Own
     */
    HBufC* iSelectedData;
    
    /**
     * Specifies number type for selection
     */
    MPhCntMatch::TNumberType iNumberType;
    
    };

/**
 *  Implements the CPhCntContactDataSelection interface
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CPhCntContactDataSelectionImpl ) : 
    public CPhCntContactDataSelection,
    private MPhCntAiwServiceObserver,
    private MPhCntContactFetchObserver
    {

public:

    static CPhCntContactDataSelectionImpl* NewL(
        MPhCntAiwService* aService,
        MPhCntServiceRequestParamFactory* aParamFactory,
        MPhCntContactStores* iContactStores );
    
    virtual ~CPhCntContactDataSelectionImpl();

// from base class CPhCntContactDataSelection

    /**
     * From CPhCntContactDataSelection
     *
     * @see CPhCntContactDataSelection
     * @since S60 v3.2
     */
    TBool IsActive() const;
    
    /**
     * From CPhCntContactDataSelection
     * ?description
     *
     * @see CPhCntContactDataSelection
     * @since S60 v3.2
     */
    void Cancel();

    /**
     * From CPhCntContactDataSelection
     *
     * @see CPhCntContactDataSelection
     * @since S60 v3.2
     */
    void SelectPhoneNumberForCallL(
        const TDesC8& aContactLink,
        const TCallType& aCallType,
        MPhCntSelectionObserver& aObserver );
        
private:

// from base class MPhCntAiwServiceObserver

    /**
     * From MPhCntAiwServiceObserver
     * Indication that service has completed.
     *
     * @since S60 v3.2
     * @param aError Error code
     */ 
    void RequestComplete( TInt aError );
    
// from base class MPhCntContactFetchObserver

    /**
     * From MPhCntContactFetchObserver
     * Indication that contact is fetched.
     *
     * @since S60 v3.2
     * @param aError Error code
     */ 
    void ContactReceived( CPhCntContact* aContact, TInt aError );
   
private:

    CPhCntContactDataSelectionImpl( 
        MPhCntAiwService* aService,
        MPhCntServiceRequestParamFactory* aParamFactory,
        MPhCntContactStores* aContactStores );

    void ConstructL();

private: // data

        /**
         * Service interface for accessing phonebook services.
         * Own.
         */
        MPhCntAiwService* iService;
        
        /** 
         * Factory to create service request parameters.
         * Own.
         */
        MPhCntServiceRequestParamFactory* iParamFactory;
        
        /**
         * Parameters used with service.
         * Own.
         */
        MPhCntServiceRequestParam* iServiceParameter;
        
        /** 
         * Result from service.
         * Own.
         */
        TPhCntContactLinkResult iServiceResult;
        
        /**
         * Field link from service.
         * Own.
         */
        HBufC8* iFieldLink;
        
        
        /** 
         * Observer for the request.
         * Not own.
         */
        MPhCntSelectionObserver* iObserver;
        
        /**
         * Store to fetch contact.
         * Own.
         */
        MPhCntContactStores* iContactStores;

    };

#endif // CPHCNTCONTACTDATASELECTIONIMPL_H
