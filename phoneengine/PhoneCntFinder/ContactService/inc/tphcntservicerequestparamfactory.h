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
* Description:  Factory for creating service request params.
*
*/


#ifndef TPHCNTSERVICEREQUESTPARAMFACTORY_H
#define TPHCNTSERVICEREQUESTPARAMFACTORY_H

#include <e32base.h>

#include "mphcntservicerequestparamfactory.h"

class MPhCntServiceRequestParam;
class MPhCntContactManager;

/**
 *  Factory class for creating service request params.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntServiceRequestParamFactory ): 
    public CBase, 
    public MPhCntServiceRequestParamFactory
    {
public:
 
    /**
     * Static constructor.
     * @since S60 v3.2
     */
    static CPhCntServiceRequestParamFactory* NewL( 
        MPhCntContactManager& aContactManager );
    
    /**
     * Static constructor.
     * @since S60 v3.2
     */
    static CPhCntServiceRequestParamFactory* NewLC( 
        MPhCntContactManager& aContactManager );
   
    /**
     * Destructor.
     *
     * @since S60 v3.1
     */
    ~CPhCntServiceRequestParamFactory();
    
// From base class MPhCntServiceRequestParamFactory

    /**
     * From base class MPhCntServiceRequestParamFactory
     *
     * @since S60 v3.1
     * @see MPhCntServiceRequestParamFactory
     */
    MPhCntServiceRequestParam* 
        CreateCreateNewContactRequestParamLC( const TDesC& aPhoneNumber ) const;
        
    /**
     * From base class MPhCntServiceRequestParamFactory
     *
     * @since S60 v3.1
     * @see MPhCntServiceRequestParamFactory
     */  
    MPhCntServiceRequestParam* 
        CreateUpdateContactRequestParamLC( const TDesC& aPhoneNumber ) const;
        
    /**
     * From base class MPhCntServiceRequestParamFactory
     *
     * @since S60 v3.1
     * @see MPhCntServiceRequestParamFactory
     */  
    MPhCntServiceRequestParam* 
        CreateGetUserSelectedPhoneNumberLinkParamLC() const;
        
    /**
     * Creates paramter for fetching user selected phonenumber.
     * The left soft key is 'Select."
     *
     * @since S60 v3.2
     * @return New instance of request parameter. Ownership given.
     */
    MPhCntServiceRequestParam* 
        CreateGetUserSelectedPhoneNumberLC() const;
    
    /**
     * From base class MPhCntServiceRequestParamFactory
     *
     * @since S60 v3.1
     * @see MPhCntServiceRequestParamFactory
     */ 
    MPhCntServiceRequestParam* 
        CreateGetUserSelectedDtmfNumberLinkLC( 
            MPhCntContactManager& aContactManager ) const;

    /**
     * From base class MPhCntServiceRequestParamFactory
     *
     * @since S60 v3.2
     * @see MPhCntServiceRequestParamFactory
     */  
    MPhCntServiceRequestParam* 
        CreateGetUserSelectedVoIPAddressLinkParamLC( 
            MPhCntContactManager& aContactManager ) const;

    /**
     * Creates paramter for fetching user selected VoIPumber.
     * The left soft key is 'Select."
     *
     * @since S60 v3.2
     * @return New instance of request parameter. Ownership given.
     */
    MPhCntServiceRequestParam* 
        CreateGetUserSelectedVoIPAddressLC( 
            MPhCntContactManager& aContactManager ) const;
    
    /**
     * From base class MPhCntServiceRequestParamFactory
     *
     * @since S60 v3.2
     * @see MPhCntServiceRequestParamFactory
     */
    MPhCntServiceRequestParam*         
        CreateGetPhoneNumberFromContactParamLC( const TDesC8& aContactLink,
            const CPhCntSingleItemFetch::TCallType aCallType ) const;
    /**
     * From base class MPhCntServiceRequestParamFactory
     *
     * @since S60 v3.2
     * @see MPhCntServiceRequestParamFactory
     */   	
    MPhCntServiceRequestParam* CreateGetPhoneNumberFromContactParamL(
        const TDesC8& aContactLink,
        const CPhCntSingleItemFetch::TCallType aCallType ) const;
            
        
protected:
        
    CPhCntServiceRequestParamFactory( MPhCntContactManager* aContactManager );
    
private: // Data

    /**
     * Contact manager.
     * Not own.
     */
    MPhCntContactManager* iContactManager;

    };

#endif // TPHCNTSERVICEREQUESTPARAMFACTORY_H
