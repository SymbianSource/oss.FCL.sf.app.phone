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


#include <featmgr.h>         // Feature Manager
#include "CPhCntSingleItemFetchService.h"
#include "cphcntphonebookservices.h"
#include "tphcntservicerequestparamfactory.h"
#include "cphcntcontactid.h"
#include "CPhCntContact.h"

   
// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntSelectionImpl* CPhCntSelectionImpl::NewL(
    const TDesC& aNumber, 
    const CPhCntContactId& aContactId )
    {
    CPhCntSelectionImpl* self = new( ELeave )CPhCntSelectionImpl();
    CleanupStack::PushL( self );
    self->ConstructL( aNumber, aContactId );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//    
CPhCntSelectionImpl::CPhCntSelectionImpl()
    {
    }
    
// ---------------------------------------------------------------------------
// Second-phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntSelectionImpl::ConstructL(
    const TDesC& aNumber, 
    const CPhCntContactId& aContactId)
    {
    iNumber = aNumber.AllocL();
    iContactId = aContactId.CloneL();
    FeatureManager::InitializeLibL();
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
// 
CPhCntSelectionImpl::~CPhCntSelectionImpl()
    {
    delete iNumber;
    delete iContactId;
    FeatureManager::UnInitializeLib();
    }

// ---------------------------------------------------------------------------
// Returns number
// ---------------------------------------------------------------------------
//     
const TDesC& CPhCntSelectionImpl::Number() const
    {
    return *iNumber;
    }

// ---------------------------------------------------------------------------
// Returns contact id
// ---------------------------------------------------------------------------
// 
const CPhCntContactId& CPhCntSelectionImpl::ContactId() const
    {
    return *iContactId;
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntSingleItemFetchService* CPhCntSingleItemFetchService::NewL(
    MPhCntContactStores& aContactStores,
    MPhCntContactManager& aContactManager )
    {
    CPhCntSingleItemFetchService* self = 
        new( ELeave )CPhCntSingleItemFetchService();
    CleanupStack::PushL( self );
    self->ConstructL( 
        aContactStores, aContactManager );
    CleanupStack::Pop( self );
    return self;    
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//    
CPhCntSingleItemFetchService::~CPhCntSingleItemFetchService() 
    {
    delete iParamFactory;
    delete iServices;
    if( iDestroyed ) 
        {
        *iDestroyed = ETrue;
        }
    }
   
// ---------------------------------------------------------------------------
// From CPhCntSingleItemFetch
// Fetches phone number
// ---------------------------------------------------------------------------
//
TInt CPhCntSingleItemFetchService::FetchLD( 
    TFetchParams& aParams ) 
    {
    TBool destroyed = EFalse;
    iDestroyed = &destroyed;
    HBufC* number = NULL;
    TInt err = KErrNone;
    if( aParams.iType == CPhCntSingleItemFetch::EFetchDtmf )
        {
        TRAP( err, 
            iServices->GetUserSelectedDtmfNumberL( 
                aParams.iContactId, number ) );
        }
    else if( aParams.iType == CPhCntSingleItemFetch::EFetchNewCall )
        {
        TRAP( err, 
            iServices->GetUserSelectedPhoneNumberL( 
                aParams.iContactId, number ) );
        }
    else if( aParams.iType == CPhCntSingleItemFetch::EFetchNewPSCall && 
             FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        TRAP( err, 
            iServices->GetUserSelectedVoIPAddressL( 
                aParams.iContactId, number ) );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    // Do not touch member variables after this, because service is canceled
    // by deleting this instance. Deletion causes above request to complete and
    // execution will be here after deletion.
        
    TInt error = KErrNone;
    if( !err )
        {
        error = SetFetchParams( aParams, *number );
        }
    else if( err == KErrCancel )
        {
        error = KErrCancel;
        }
    else 
        {
        if( !destroyed )
            {
            iDestroyed = NULL;
            delete this;
            }
        User::Leave( err );     
        }
    delete number;
    if( !destroyed )
        {
        iDestroyed = NULL;
        delete this;
        }
    return error;
    }

// ---------------------------------------------------------------------------
// From CPhCntSingleItemFetch
// Fetches phone number
// ---------------------------------------------------------------------------
//
TInt CPhCntSingleItemFetchService::FetchPhoneNumberLD( 
    const TDesC8& aContactLink,
    const CPhCntSingleItemFetch::TCallType aCallType,
    HBufC*& aNumber,
    HBufC8*& aFieldLink ) 
    {
    TBool destroyed = EFalse;
    iDestroyed = &destroyed;    
    TInt err = KErrNone;
        
    TRAP( err, 
    	iServices->GetPhoneNumberL( aContactLink, aCallType, aNumber, aFieldLink ) 
    	);                        

    // Do not touch member variables after this, because service is canceled
    // by deleting this instance. Deletion causes above request to complete and
    // execution will be here after deletion.
    
    TInt error = KErrNone;    
    if( err )
    	{
    	delete aNumber;
    	aNumber = NULL;
    	delete aFieldLink;
    	aFieldLink = NULL;
	    if( err == KErrCancel )
	        {
	        error = KErrCancel;
	        }
	    else 
	        {
	        if( !destroyed )
	            {
	            iDestroyed = NULL;
	            delete this;
	            }
	        User::Leave( err );     
	        }	
    	}
                    
    if( !destroyed )
        {
        iDestroyed = NULL;
        delete this;
        }
    return error;
    }

// ---------------------------------------------------------------------------
// From CPhCntSingleItemFetch
// Fetches phone number
// ---------------------------------------------------------------------------
//    
CPhCntSelection* CPhCntSingleItemFetchService::SelectPhoneNumberLD()
    {
    TBool destroyed = EFalse;
    iDestroyed = &destroyed;    
    CPhCntSelectionImpl* selection = NULL;

    CPhCntContact* contact = NULL;
    TRAPD( err, 
        MPhCntServiceRequestParam* param = 
            iParamFactory->CreateGetUserSelectedPhoneNumberLC();
        contact = iServices->GetUserSelectedContactLC( *param );
        CleanupStack::Pop( contact );
        CleanupStack::PopAndDestroy( 1 ); // param
        );
    if( !destroyed )
        {
        iDestroyed = NULL;
        delete this;
        }
        
    if( !err )
        {
        CleanupStack::PushL( contact );
        selection = CPhCntSelectionImpl::NewL( 
            contact->Number(), *contact->ContactId() );
        CleanupStack::PopAndDestroy( contact );
        }
    else
        {
        // If the request is canceled then do not leave, because
        // of agreement with clients.
        if( err != KErrCancel )
            {
            User::Leave( err );    
            }
        }
        
    
    return selection;
    }
// ---------------------------------------------------------------------------
// Sets possible error values and return values.
// ---------------------------------------------------------------------------
// 
TInt CPhCntSingleItemFetchService::SetFetchParams( 
    TFetchParams& aParams,
    const TDesC& aReceivedPhoneNumber  ) const
    {
    TInt errorValue( KErrNone );
    
    if( aReceivedPhoneNumber.Length() >= aParams.iString->MaxLength() ) 
        {
        errorValue = KErrOverflow;
        }
    
    if( errorValue == KErrNone ) 
        {
        aParams.iString->Zero();
        aParams.iString->Copy( aReceivedPhoneNumber );
        }
    
    
    return errorValue;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//     
CPhCntSingleItemFetchService::CPhCntSingleItemFetchService()
    {
    }
    
// ---------------------------------------------------------------------------
// Seconds phase constructor
// ---------------------------------------------------------------------------
//     
void CPhCntSingleItemFetchService::ConstructL( 
    MPhCntContactStores& aContactStores,
    MPhCntContactManager& aContactManager )
    {
    iParamFactory = CPhCntServiceRequestParamFactory::NewL( aContactManager );
    iServices = CPhCntPhonebookServices::NewL( 
        aContactStores, aContactManager );
    }

 
