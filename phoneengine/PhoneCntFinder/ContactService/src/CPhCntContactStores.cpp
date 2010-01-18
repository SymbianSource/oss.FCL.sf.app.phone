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
* Description:  Contact stores.
*
*/


#include "CPhCntContactStores.h"
#include "MPhCntContactManager.h"
#include "MPhCntContactFetchObserver.h"
#include "CPhCntContact.h"
#include "cphcntcontactfieldsresolver.h"
#include <MVPbkContactOperationBase.h>
#include <CVPbkContactLinkArray.h>
#include <MVPbkStoreContact.h>
#include <MVPbkContactLink.h>


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntContactStores* CPhCntContactStores::NewL( 
    MPhCntContactManager& aContactManager ) 
    {
    CPhCntContactStores* self = new( ELeave )CPhCntContactStores( aContactManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntContactStores::~CPhCntContactStores() 
    {
    CancelRequest();
    }

// ---------------------------------------------------------------------------
// From MPhCntContactStores
// Determines if request is active.
// ---------------------------------------------------------------------------
//     
TBool CPhCntContactStores::IsRequestActive() const
    {
    TBool isActive( EFalse );
    if( iRetrieveContactOperation )
        {
        isActive = ETrue;
        }
    return isActive;
    }

// ---------------------------------------------------------------------------
// From MPhCntContactStores
// Cancels the request.
// ---------------------------------------------------------------------------
//       
void CPhCntContactStores::CancelRequest()
    {
    delete iRetrieveContactOperation;
    iRetrieveContactOperation = NULL;
    }
 
// ---------------------------------------------------------------------------
// From MPhCntContactStores
// Fetches contact from contact store.
// ---------------------------------------------------------------------------
//   
void CPhCntContactStores::FetchContactL( 
    const MVPbkContactLink& aLink, 
    MPhCntContactFetchObserver& aObserver ) 
    {
    // If we already have somebody fetching contact...
    if ( iContactFetchObserver )
        {
        User::Leave(KErrInUse);
        }
            
    iLink = aLink.CloneLC();
    CleanupStack::Pop( 1 );
    iContactFetchObserver = &aObserver;
    iRetrieveContactOperation = 
        iContactManager.RetrieveContactL( *iLink, *this ); 
     
    }

// ---------------------------------------------------------------------------
// From MPhCntContactStores
// Fetches first contact pointed by contact links.
// ---------------------------------------------------------------------------
//    
void CPhCntContactStores::FetchFirstContactL( 
    const TDesC8& aContactLinks,
    MPhCntContactFetchObserver& aObserver ) 
    {
    if ( iContactFetchObserver )
        {
        User::Leave(KErrInUse);
        }
        
    CVPbkContactLinkArray* linkArray = 
        CreateContactLinkArrayL( aContactLinks );
    CleanupStack::PushL( linkArray );
    
    if( linkArray->Count() > 0 ) 
        {
        const MVPbkContactLink& link = linkArray->At( 0 );
        MVPbkContactLink* cloneLink = link.CloneLC();
        CleanupStack::Pop(); // cloneLink
        iLink = cloneLink;
        iContactFetchObserver = &aObserver;
        iRetrieveContactOperation = 
            iContactManager.RetrieveContactL( *iLink, *this ); 
        }
    else
        {
        aObserver.ContactReceived( NULL, KErrNotFound );
        }
    CleanupStack::PopAndDestroy( linkArray );
    }
    
// ---------------------------------------------------------------------------
// From MPhCntContactStores
// Creates contact link array
// ---------------------------------------------------------------------------
//
CVPbkContactLinkArray* CPhCntContactStores::CreateContactLinkArrayL( 
    const TDesC8& aLinks ) 
    {
    CVPbkContactLinkArray* linkArray = 
        CVPbkContactLinkArray::NewLC( 
            aLinks, iContactManager.ContactStoresL() );
    CleanupStack::Pop( linkArray );
    return linkArray;
    }
    
// ---------------------------------------------------------------------------
// From MVPbkSingeContactOperationObserver
// Resolves the received contact and notifies the observer about 
// contact reception.
// ---------------------------------------------------------------------------
//
void CPhCntContactStores::VPbkSingleContactOperationComplete(
    MVPbkContactOperationBase& /*aOperation*/,
    MVPbkStoreContact* aContact ) 
    {
    CancelRequest();

    CPhCntContact* contact = NULL;
    CPhCntContactFieldsResolver* resolver = NULL;
    TRAPD( err, 
        resolver = 
            CPhCntContactFieldsResolver::NewLC( 
                iLink, aContact, iContactManager );
            
        contact =  
            CPhCntContact::NewL( resolver, iContactManager );
        CleanupStack::Pop( resolver );
    )
        
    iContactFetchObserver->ContactReceived( contact, err );
    iContactFetchObserver = NULL;
    }

// ---------------------------------------------------------------------------
// From MVPbkSingeContactOperationObserver
// Notify observer that contact fetch failed.
// ---------------------------------------------------------------------------
//   
void CPhCntContactStores::VPbkSingleContactOperationFailed(
    MVPbkContactOperationBase& /*aOperation*/, 
    TInt aError ) 
    {
    CancelRequest();
    
    delete iLink;
    iLink = NULL;
    iContactFetchObserver->ContactReceived( NULL, aError );
    iContactFetchObserver = NULL;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//    
CPhCntContactStores::CPhCntContactStores( 
    MPhCntContactManager& aContactManager ) :
    iContactManager( aContactManager )
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor.
// ---------------------------------------------------------------------------
//    
void CPhCntContactStores::ConstructL()
    {
    }
