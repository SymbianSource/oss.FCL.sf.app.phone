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


#include <CVPbkContactLinkArray.h>
#include "tphcntservicerequestparamfactory.h"
#include "tphcntservicerequestparams.h"
#include "MPhCntContactManager.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntServiceRequestParamFactory::CPhCntServiceRequestParamFactory(
    MPhCntContactManager* aContactManager ) : 
    iContactManager( aContactManager )
    {
    }
    
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntServiceRequestParamFactory::~CPhCntServiceRequestParamFactory()
    {
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntServiceRequestParamFactory* CPhCntServiceRequestParamFactory::NewL(
    MPhCntContactManager& aContactManager )
    {
    CPhCntServiceRequestParamFactory* self = 
        CPhCntServiceRequestParamFactory::NewLC( aContactManager );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntServiceRequestParamFactory* CPhCntServiceRequestParamFactory::NewLC(
    MPhCntContactManager& aContactManager )
    {
    CPhCntServiceRequestParamFactory* self = 
        new( ELeave )CPhCntServiceRequestParamFactory( &aContactManager );
    CleanupStack::PushL( self );
    return self;
    }

// ---------------------------------------------------------------------------
// From MPhCntServiceRequestParamFactory
// Creates new instance.
// ---------------------------------------------------------------------------
//
MPhCntServiceRequestParam* 
    CPhCntServiceRequestParamFactory::CreateCreateNewContactRequestParamLC( 
        const TDesC& aPhoneNumber ) const
    {
    return CPhCntCreateNewContactParams::NewLC( aPhoneNumber );
    }

// ---------------------------------------------------------------------------
// From MPhCntServiceRequestParamFactory
// Creates new instance.
// ---------------------------------------------------------------------------
//    
MPhCntServiceRequestParam* 
    CPhCntServiceRequestParamFactory::CreateUpdateContactRequestParamLC( 
        const TDesC& aPhoneNumber ) const
    {
    return CPhCntUpdateExistingContact::NewLC( aPhoneNumber );
    }
    
// ---------------------------------------------------------------------------
// From MPhCntServiceRequestParamFactory
// Creates new instance.
// ---------------------------------------------------------------------------
// 
MPhCntServiceRequestParam* 
    CPhCntServiceRequestParamFactory::
        CreateGetUserSelectedPhoneNumberLinkParamLC() const
    {
    return CPhCntGetUserSelectedPhoneNumberLink::NewLC( ETrue, 
                                                        *iContactManager );
    }

// ---------------------------------------------------------------------------
// Creates new instance param instance
// ---------------------------------------------------------------------------
// 
MPhCntServiceRequestParam* 
    CPhCntServiceRequestParamFactory::
        CreateGetUserSelectedPhoneNumberLC() const
    {
    return CPhCntGetUserSelectedPhoneNumberLink::NewLC( EFalse, 
                                                        *iContactManager );
    }

// ---------------------------------------------------------------------------
// From MPhCntServiceRequestParamFactory
// Creates new instance.
// ---------------------------------------------------------------------------
// 
MPhCntServiceRequestParam* 
    CPhCntServiceRequestParamFactory::
        CreateGetUserSelectedVoIPAddressLinkParamLC(
            MPhCntContactManager& aContactManager ) const
    {
    return CPhCntGetUserSelectedVoIPAddressLink::NewLC( aContactManager );
    }

// ---------------------------------------------------------------------------
// Creates new instance param instance
// ---------------------------------------------------------------------------
// 
MPhCntServiceRequestParam* 
    CPhCntServiceRequestParamFactory::
        CreateGetUserSelectedVoIPAddressLC(
            MPhCntContactManager& aContactManager ) const
    {
    return CPhCntGetUserSelectedVoIPAddressLink::NewLC( aContactManager, EFalse );
    }

// ---------------------------------------------------------------------------
// From MPhCntServiceRequestParamFactory
// Creates new instance.
// ---------------------------------------------------------------------------
//     
MPhCntServiceRequestParam* 
    CPhCntServiceRequestParamFactory::CreateGetUserSelectedDtmfNumberLinkLC( 
        MPhCntContactManager& aContactManager ) const
    {
    return CPhCntGetUserSelectedDtmfNumberLink::NewLC( aContactManager );
    }
    
// ---------------------------------------------------------------------------
// From MPhCntServiceRequestParamFactory
// Creates new instance.
// ---------------------------------------------------------------------------
//    
MPhCntServiceRequestParam* 
    CPhCntServiceRequestParamFactory::
        CreateGetPhoneNumberFromContactParamLC( const TDesC8& aContactLink,
            const CPhCntSingleItemFetch::TCallType aCallType ) const
    {
    CVPbkContactLinkArray* linkArray = CVPbkContactLinkArray::NewLC( 
        aContactLink, iContactManager->ContactStoresL() );
    CPhCntGetPhoneNumberReqParam* param = 
        CPhCntGetPhoneNumberReqParam::NewL( *iContactManager, 
            *linkArray, aCallType );
    CleanupStack::PopAndDestroy( linkArray );
    CleanupStack::PushL( param );
    return param;   
    }

// ---------------------------------------------------------------------------
// From MPhCntServiceRequestParamFactory
// Creates new instance.
// ---------------------------------------------------------------------------
//    
MPhCntServiceRequestParam* 
    CPhCntServiceRequestParamFactory::CreateGetPhoneNumberFromContactParamL(
    const TDesC8& aContactLink,
    const CPhCntSingleItemFetch::TCallType aCallType ) const
    {
    CVPbkContactLinkArray* linkArray = CVPbkContactLinkArray::NewLC( 
        aContactLink, iContactManager->ContactStoresL() );
    CPhCntGetPhoneNumberReqParam* param = 
        CPhCntGetPhoneNumberReqParam::NewL( *iContactManager, 
            *linkArray, aCallType );
    CleanupStack::PopAndDestroy( linkArray );
    return param;        
    }

