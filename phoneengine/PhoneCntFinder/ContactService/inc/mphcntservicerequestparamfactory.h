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
* Description:  Abstract base class for param factory
*
*/


#ifndef MPHCNTSERVICEREQUESTPARAMFACTORY_H
#define MPHCNTSERVICEREQUESTPARAMFACTORY_H

#include "CPhCntSingleItemFetch.h"
#include "cphcntcontactdataselection.h"

class MPhCntServiceRequestParam;
class MVPbkContactLinkArray;
class MPhCntContactManager;

/**
 *  Abstract base class for creating AIW service request params.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( MPhCntServiceRequestParamFactory )
    {
public:

    virtual ~MPhCntServiceRequestParamFactory() {};
    
    /**
     * Creates new parameter for creating new contact.
     *
     * @since S60 v3.1
     * @param aPhoneNumber Phone number of new contact.
     * @return New instance.
     */
    virtual MPhCntServiceRequestParam* 
        CreateCreateNewContactRequestParamLC( const TDesC& aPhoneNumber ) const = 0;
        
    /**
     * Creates new paramater for updating existing contact.
     *
     * @since S60 v3.1
     * @param aPhoneNumber Phone number added to existing contact.
     * @return New param.
     */   
    virtual MPhCntServiceRequestParam* 
        CreateUpdateContactRequestParamLC( const TDesC& aPhoneNumber ) const = 0;
        
    /**
     * Creates new parameter for fetching user selected phone number link.
     *
     * @since S60 v3.1
     * @return New parameter instance.
     */
    virtual MPhCntServiceRequestParam* 
        CreateGetUserSelectedPhoneNumberLinkParamLC() const = 0;
        
    /**
     * Creates new parameter for fetching user selected dtmf number link.
     *
     * @since S60 v3.2
     * @param aContactManager Contact manager reference.
     * @return New parameter instance.
     */
    virtual MPhCntServiceRequestParam* 
        CreateGetUserSelectedDtmfNumberLinkLC( 
            MPhCntContactManager& aContactManager ) const = 0;

    /**
     * Creates new parameter for fetching user selected voip address link.
     *
     * @since S60 v5.0
     * @param aContactManager Contact manager reference.
     * @return New parameter instance.
     */  
    virtual MPhCntServiceRequestParam* 
        CreateGetUserSelectedVoIPAddressLinkParamLC( 
            MPhCntContactManager& aContactManager ) const = 0;

    /**
     * Creates parameter for fetching user selected VoIPumber.
     * The left soft key is 'Select."
     *
     * @since S60 v5.0
     * @param aContactManager Contact manager reference.
     * @return New parameter instance.
     */
    virtual MPhCntServiceRequestParam* 
        CreateGetUserSelectedVoIPAddressLC( 
            MPhCntContactManager& aContactManager ) const = 0;

    /**
     * Creates new parameter for fetching a phone number from
     * known contact, which is identified by contact link.
     * @since S60 v3.2
     * @param aContactLink Link to a contact.
     * @param aCallType Number type.
     * @return New parameter instance.
     */         
    virtual MPhCntServiceRequestParam*         
        CreateGetPhoneNumberFromContactParamLC( 
            const TDesC8& aContactLink,
            const CPhCntSingleItemFetch::TCallType aCallType ) const = 0;
    
    /**
     * Creates new parameter for fetching a phone number from
     * known contact, which is identified by contact link.
     * @since S60 v3.2
     * @param aContactLink Link to a contact.
     * @param aCallType Number type.
     * @return New parameter instance.
     */    	
    virtual MPhCntServiceRequestParam* CreateGetPhoneNumberFromContactParamL(
        const TDesC8& aContactLink,
        const CPhCntSingleItemFetch::TCallType aCallType ) const = 0;
        	
    };


#endif // MPHCNTSERVICEREQUESTPARAMFACTORY_H
