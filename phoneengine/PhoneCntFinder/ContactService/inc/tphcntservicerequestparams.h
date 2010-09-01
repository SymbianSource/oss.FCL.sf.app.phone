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
* Description:  Classes for service requests made to CPhCntService
*
*/


#ifndef TPHCNTSERVICEREQUESTPARAMS_H
#define TPHCNTSERVICEREQUESTPARAMS_H

#include <e32base.h>
#include <AiwCommon.hrh>
#include <RVPbkContactFieldDefaultPriorities.h>

#include "mphcntservicerequestparam.h"
#include "CPhCntSingleItemFetch.h"

class CAiwGenericParamList;
class MPhCntContactManager;
class MVPbkContactLinkArray;

/**
 *  Base class for service request params.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntServiceRequestParams ) : 
    public CBase, 
    public MPhCntServiceRequestParam
    {

public:

    /** 
     * Destructor.
     *
     * @since S60 v3.1
     */
    ~CPhCntServiceRequestParams();
    
// From base class MPhCntServiceRequestParam

    /**
     * From base class MPhCntServiceRequestParam
     * 
     * @since S60 v3,1
     * @see MPhCntServiceRequestParam
     */
    TAiwServiceCommands Command() const;
    
    /**
     * From base class MPhCntServiceRequestParam
     * 
     * @since S60 v3,1
     * @see MPhCntServiceRequestParam
     */
    const CAiwGenericParamList& InParamList() const;
    
protected:
    
    /**
     * Constructor.
     *
     * @since S60 v3.1
     * @param aAiwCommand AIW command.
     */
    CPhCntServiceRequestParams( TAiwServiceCommands aAiwCommand );
    
    void BaseConstructL();
    

private: // data

    /**
     * Command used with aiw.
     */
    TAiwServiceCommands iAiwCommand;
    
protected:

    /**
     * Param list used with AIW.
     * Own.
     */
    CAiwGenericParamList* iGenericParamList;
    
    /**
     * Default numbers.
     * Own.
     */
    RVPbkContactFieldDefaultPriorities iDefaultPriorities; 
    };
    
/**
 *  Request parameters for creating new contact.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntCreateNewContactParams ) 
    : public CPhCntServiceRequestParams
    {
public:
    
    /**
     * Static constructor.
     * 
     * @since S60 v3.1
     * @param aPhoneNumber Phone number of the new contact.
     */
    static CPhCntCreateNewContactParams* NewL( 
        const TDesC& aPhoneNumber );  
    
    /**
     * Static constructor.
     * 
     * @since S60 v3.1
     * @param aPhoneNumber Phone number of the new contact.
     */
    static CPhCntCreateNewContactParams* NewLC(
        const TDesC& aPhoneNumber );  
    
private:
    
    CPhCntCreateNewContactParams( const TDesC& aPhoneNumber );
    
    void ConstructL();
    
private:
    
    /**
     * Phone number set to created contact.
     * Not own.
     */
    const TDesC& iPhoneNumber;
    
    };
    
/**
 *  Request parameters for updating existing contact.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntUpdateExistingContact ) 
    : public CPhCntServiceRequestParams
    {
public:
    
    /**
     * Static constructor.
     * 
     * @since S60 v3.1
     * @param aPhoneNumber Phone number to be added to existing contact..
     */
    static CPhCntUpdateExistingContact* NewL( 
        const TDesC& aPhoneNumber );  
    
    /**
     * Static constructor.
     * 
     * @since S60 v3.1
     * @param aPhoneNumber Phone number to be added to existing contact..
     */
    static CPhCntUpdateExistingContact* NewLC(
        const TDesC& aPhoneNumber );  
    
private:
    
    CPhCntUpdateExistingContact( const TDesC& aPhoneNumber );
    
    void ConstructL();
    
private:
    
    /**
     * Phone number set to created contact.
     * Not own.
     */
    const TDesC& iPhoneNumber;
    
    };
class CVPbkFieldTypeSelector;
    
/**
 *  Request parameter for getting user selected phone number link.
 *
 *  @lib PhoneCntFinder.lib
 *  @since  v3.2
 */
NONSHARABLE_CLASS( CPhCntGetUserSelectedPhoneNumberLink ) : 
    public CPhCntServiceRequestParams
    {
public:

    /**
     * Static constructor.
     * 
     * @since S60 v3.1
     * @param aCallUsedWithLSK ETrue - Left softkey is 'Call'
     *                   EFalse - Left softkey is 'Select'
     * @param aContactManager Reference to contact manager.
     */
    static CPhCntGetUserSelectedPhoneNumberLink* NewLC( TBool aCallUsedWithLSK,
                                        MPhCntContactManager& aContactManager );
    
    ~CPhCntGetUserSelectedPhoneNumberLink();

private:
    
    CPhCntGetUserSelectedPhoneNumberLink();
    
    void ConstructL( TBool aCallUsedWithLSK, 
                     MPhCntContactManager& aContactManager );

private: // data

	/**
     * Filter
     * Own.
     */
    CVPbkFieldTypeSelector* iContactViewFilter; 
    };
    
/**
 *  Request parameter for getting user selected voip address link.
 *
 *  @lib PhoneCntFinder.lib
 *  @since  v3.2
 */
NONSHARABLE_CLASS( CPhCntGetUserSelectedVoIPAddressLink ) : 
    public CPhCntServiceRequestParams
    {
public:

    /**
     * Static constructor.
     * 
     * @since S60 v3.1
     */
    static CPhCntGetUserSelectedVoIPAddressLink* NewLC(
        MPhCntContactManager& aContactManager,
        TBool aCallUsedWithLSK = ETrue );
    
private:
    
    CPhCntGetUserSelectedVoIPAddressLink();
    
    void ConstructL(
        MPhCntContactManager& aContactManager,
        TBool aCallUsedWithLSK );

private:
        
    /**
     * Filter
     * Own.
     */
    CVPbkFieldTypeSelector* iContactViewFilter; 
    };

/**
 * Request parameter for getting user selected dtmf number link.
 *
 * @lib PhoneCntFinder.lib
 * @since v3.2
 */
NONSHARABLE_CLASS( CPhCntGetUserSelectedDtmfNumberLink ) : 
    public CPhCntServiceRequestParams
    {
public:

    /**
     * Static constructor.
     *
     * @since S60 v3.2
     * @param aContactManager Reference to contact manager.
     */
    static CPhCntGetUserSelectedDtmfNumberLink* NewLC(
        MPhCntContactManager& aContactManager );
        
    ~CPhCntGetUserSelectedDtmfNumberLink();
    
private:

    CPhCntGetUserSelectedDtmfNumberLink();
    
    void ConstructL( MPhCntContactManager& aContactManager );
    
private:

    /**
     * Filter
     * Own.
     */
    CVPbkFieldTypeSelector* iContactViewFilter; 
    };



/**
 * Request parameter for getting a phone number from a contact link.
 *
 * @lib PhoneCntFinder.lib
 * @since v3.2
 */
NONSHARABLE_CLASS( CPhCntGetPhoneNumberReqParam ) : 
    public CPhCntServiceRequestParams
    {
public:

    /**
     * Static constructor.
     *
     * @since S60 v3.2
     * @param aContactLink Contact link
     */
    static CPhCntGetPhoneNumberReqParam* NewL(
    	MPhCntContactManager& aContactManager,
    	MVPbkContactLinkArray& aLinkArray,
    	const CPhCntSingleItemFetch::TCallType aCallType
        );
        
    ~CPhCntGetPhoneNumberReqParam();
    
private:

    CPhCntGetPhoneNumberReqParam();
    
    void ConstructL( 
    	MPhCntContactManager& aContactManager, 
    	MVPbkContactLinkArray& aLinkArray, 
    	const CPhCntSingleItemFetch::TCallType aCallType
    	);
    
private: // data

	/**
     * Filter
     * Own.
     */
    CVPbkFieldTypeSelector* iContactViewFilter; 
    };

#endif // TPHCNTSERVICEREQUESTPARAMS_H
