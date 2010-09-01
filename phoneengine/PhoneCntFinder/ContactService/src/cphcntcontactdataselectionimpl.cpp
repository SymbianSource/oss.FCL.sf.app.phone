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


#include "cphcntcontactdataselectionimpl.h"
#include "mphcntservicerequestparamfactory.h"
#include "mphcntservicerequestparam.h"
#include "MPhCntContactStores.h"
#include "CPhCntContact.h"
#include "cphcntcontactid.h"

CPhCntSelectedDataImpl* CPhCntSelectedDataImpl::NewL( 
    CPhCntContact& aContact )
    {
    CPhCntSelectedDataImpl* self = new( ELeave )CPhCntSelectedDataImpl();
    CleanupStack::PushL( self );
    self->ConstructL( aContact );
    CleanupStack::Pop( self );
    return self;
    }
    
CPhCntSelectedDataImpl::~CPhCntSelectedDataImpl()
    {
    delete iSelectedData;
    delete iFieldLink;
    }
    
CPhCntSelectedDataImpl::CPhCntSelectedDataImpl()
    {
    }
    
void CPhCntSelectedDataImpl::ConstructL( CPhCntContact& aContact )
    {
    iSelectedData = aContact.Number().AllocL();
    iFieldLink = aContact.ContactId()->PackLC();
    iNumberType = aContact.NumberType();
    CleanupStack::Pop( iFieldLink );
    }
    
const TDesC& CPhCntSelectedDataImpl::Data() const
    {
    return *iSelectedData;
    }
    
const TDesC8& CPhCntSelectedDataImpl::FieldLink() const
    {
    return *iFieldLink;
    }

MPhCntMatch::TNumberType CPhCntSelectedDataImpl::NumberType() const
    {
    return iNumberType;
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntContactDataSelectionImpl::CPhCntContactDataSelectionImpl(
    MPhCntAiwService* aService,
    MPhCntServiceRequestParamFactory* aParamFactory,
    MPhCntContactStores* aContactStores ) : 
    iService( aService ),
    iParamFactory( aParamFactory ),
    iServiceResult( iFieldLink ),
    iContactStores( aContactStores )
    {
    }


// ---------------------------------------------------------------------------
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntContactDataSelectionImpl::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// static constructor
// ---------------------------------------------------------------------------
//
CPhCntContactDataSelectionImpl* CPhCntContactDataSelectionImpl::NewL(
    MPhCntAiwService* aService,
    MPhCntServiceRequestParamFactory* aParamFactory,
    MPhCntContactStores* aContactStores )
    {
    CPhCntContactDataSelectionImpl* self = new( ELeave ) 
        CPhCntContactDataSelectionImpl(
            aService, aParamFactory, aContactStores );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntContactDataSelectionImpl::~CPhCntContactDataSelectionImpl()
    {
    if( iService )
        {
        iService->Cancel();
        }
    delete iService;
    delete iParamFactory;
    delete iServiceParameter;
    delete iFieldLink;
    if( iContactStores )
        {
        iContactStores->CancelRequest();
        }
    delete iContactStores;
    }

// ---------------------------------------------------------------------------
// From class CPhCntContactDataSelection
// Determines if there is active request pending.
// ---------------------------------------------------------------------------
//
TBool CPhCntContactDataSelectionImpl::IsActive() const
    {
    return iService->IsRequestActive() | iContactStores->IsRequestActive();
    }
    
// ---------------------------------------------------------------------------
// From class CPhCntContactDataSelection
// ?implementation_description
// ---------------------------------------------------------------------------
//    
void CPhCntContactDataSelectionImpl::Cancel()
    {
    iService->Cancel();
    iContactStores->CancelRequest();
    iObserver = NULL;
    }

// ---------------------------------------------------------------------------
// From class CPhCntContactDataSelection
// Starts phone number selection.
// ---------------------------------------------------------------------------
//   
void CPhCntContactDataSelectionImpl::SelectPhoneNumberForCallL(
    const TDesC8& aContactLink,
    const TCallType& aCallType,
    MPhCntSelectionObserver& aObserver )
    {
    if( IsActive() ) 
        {
        User::Leave( KErrInUse );
        }
    iObserver = &aObserver;
    delete iServiceParameter;
    iServiceParameter = NULL;
    iServiceParameter = iParamFactory->CreateGetPhoneNumberFromContactParamL( 
        aContactLink,
        static_cast<CPhCntSingleItemFetch::TCallType>(aCallType ) );
    iService->ExecuteRequestL( *iServiceParameter, iServiceResult, *this );
    }
    
// ---------------------------------------------------------------------------
// From class MPhCntServiceObserver
// Indication from Aiw service that request has completed.
// ---------------------------------------------------------------------------
//  
void CPhCntContactDataSelectionImpl::RequestComplete( TInt aError )
    {
    if( !aError )
        {
        TRAPD( err, iContactStores->FetchFirstContactL( *iFieldLink, *this ) );
        if( iObserver && err )
            {
            iObserver->SelectionDone( NULL, err );
            }
        }
    else
        {
        if( iObserver )
            {
            iObserver->SelectionDone( NULL, aError );
            }
        }
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFetchObserver
// Indication from contact stores that contact is now received.
// ---------------------------------------------------------------------------
//  
void CPhCntContactDataSelectionImpl::ContactReceived( 
    CPhCntContact* aContact, TInt aError )
    {
    if( !aError )
        {
        CPhCntSelectedDataImpl* selectedData = NULL;
        TRAPD( err, 
            selectedData = CPhCntSelectedDataImpl::NewL( *aContact ) );
        iObserver->SelectionDone( selectedData, err );
        delete selectedData;
        }
    else
        {
        iObserver->SelectionDone( NULL, aError );
        }
    delete aContact;
    }
