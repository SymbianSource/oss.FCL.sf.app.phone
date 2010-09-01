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
* Description:  Fetches contact
*
*/


#include "cphcntfetchcontact.h"
#include "MPhCntContactStores.h"
#include "MVPbkContactLink.h"
#include <CVPbkContactLinkArray.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntFetchContact::CPhCntFetchContact( 
    MPhCntContactStores& aContactStores ) :
    iContactStores( aContactStores )
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntFetchContact::ConstructL()
    {
    BaseConstructL();
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntFetchContact* CPhCntFetchContact::NewL(
    MPhCntContactStores& aContactStores )
    {
    CPhCntFetchContact* self = CPhCntFetchContact::NewLC( aContactStores );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntFetchContact* CPhCntFetchContact::NewLC( 
    MPhCntContactStores& aContactStores )
    {
    CPhCntFetchContact* self = 
        new( ELeave ) CPhCntFetchContact( aContactStores );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CPhCntFetchContact::~CPhCntFetchContact()
    {
    }

// ---------------------------------------------------------------------------
// Starts fetching the contact.
// ---------------------------------------------------------------------------
//
TInt CPhCntFetchContact::FetchContact( 
    const MVPbkContactLink& aContactLink,
    CPhCntContact*& aContact )
    {
    iContactLink = &aContactLink;
     // Converts asynchronous request to synchronous.
    const TInt error( MakeAsyncRequest() );
    if( !error )
        {
        aContact = iReceivedContact;
        iReceivedContact = NULL;
        }
    iContactLink = NULL;
    
    return error;
    }

// ---------------------------------------------------------------------------
// Starts fetching the first contact.
// ---------------------------------------------------------------------------
//    
TInt CPhCntFetchContact::FetchContact( 
    const TDesC8& aContactLinkArray, 
    CPhCntContact*& aContact )
    {
    CVPbkContactLinkArray* contactLinkArray = NULL;
    TRAPD( err, 
        contactLinkArray = 
            iContactStores.CreateContactLinkArrayL( aContactLinkArray ) );  
    
    if( !err )
        {
        if( contactLinkArray && contactLinkArray->Count() > 0 )
            {
            const MVPbkContactLink& contactLink = contactLinkArray->At( 0 );
            err = FetchContact( contactLink, aContact );
            }     
        else 
            {
            err = KErrNotFound;
            }
        delete contactLinkArray;
        }
 
    
    return err;
    }

// ---------------------------------------------------------------------------
// From class CPhCntAsyncToSync
// Fetches the contact from contact stores.
// ---------------------------------------------------------------------------
//
void CPhCntFetchContact::DoMakeAsyncRequestL()
    {
    iContactStores.FetchContactL( *iContactLink, *this );
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFetchObserver
// Takes the contact and notifies base class that contact has been received.
// ---------------------------------------------------------------------------
//
 void CPhCntFetchContact::ContactReceived( 
    CPhCntContact* aContact, 
    TInt aError )
    {
    iReceivedContact = aContact;    
    ResponseReceived( aError ); // Indicate that asynchronous operation is complete.
    }
