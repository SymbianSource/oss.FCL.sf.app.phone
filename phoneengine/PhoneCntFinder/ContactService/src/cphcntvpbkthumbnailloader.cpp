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
* Description:  Thumbnail loader for loading thumbnails from VirtualPhonebook
*
*/


#include <CPbk2ImageManager.h>

#include "cphcntvpbkthumbnailloader.h"
#include "MPhCntContactStores.h"
#include "cphcntvpbkcontactid.h"
#include "CPhCntContact.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntVPbkThumbnailLoader::CPhCntVPbkThumbnailLoader(
    MPhCntContactStores& aContactStores ) : 
    iContactStores( &aContactStores )
    {
    }


// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CPhCntVPbkThumbnailLoader::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// static constructor
// ---------------------------------------------------------------------------
//
CPhCntVPbkThumbnailLoader* CPhCntVPbkThumbnailLoader::NewL(
    MPhCntContactStores& aContactStores )
    {
    CPhCntVPbkThumbnailLoader* self = 
        CPhCntVPbkThumbnailLoader::NewLC( aContactStores );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// static constructor
// ---------------------------------------------------------------------------
//
CPhCntVPbkThumbnailLoader* CPhCntVPbkThumbnailLoader::NewLC(
    MPhCntContactStores& aContactStores )
    {
    CPhCntVPbkThumbnailLoader* self = 
        new( ELeave ) CPhCntVPbkThumbnailLoader( aContactStores );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// destructor.
// ---------------------------------------------------------------------------
//
CPhCntVPbkThumbnailLoader::~CPhCntVPbkThumbnailLoader()
    {
    delete iContactStores;
    delete iOperation;
    delete iReceivedContact;
    }
    
    
// ---------------------------------------------------------------------------
// From class MPhCntThumbnailLoader
// ---------------------------------------------------------------------------
//
void CPhCntVPbkThumbnailLoader::Cancel()
    {
    delete iReceivedContact;
    iReceivedContact = NULL;
    delete iOperation;
    iOperation = NULL;
    }

// ---------------------------------------------------------------------------
// From class MPhCntThumbnailLoader
// ---------------------------------------------------------------------------
//
void CPhCntVPbkThumbnailLoader::Release()
    {
    // Nothing to do.
    }


// ---------------------------------------------------------------------------
// From class CPhCntThumbnailLoaderBase
// Starts the loading of thumbnail
// ---------------------------------------------------------------------------
//
void CPhCntVPbkThumbnailLoader::DoLoadL( const CPhCntContactId& aContactId ) 
    {
    // Cast is safe because compile time variation makes sure that 
    // CPhCntContactId is only subclassed by CPhCntVPbkContactId.
    const CPhCntVPbkContactId& contactId = 
        reinterpret_cast<const CPhCntVPbkContactId&>( aContactId );
        
    // Fetch the contact.
    iContactStores->FetchContactL( contactId.ContactLink(), *this );
    }
 
// ---------------------------------------------------------------------------
// From class MPhCntContactFetchObserver
// Indication that contact has been fetched from contact stores.
// Starts loading the thumbnail from contact.
// ---------------------------------------------------------------------------
//   
void CPhCntVPbkThumbnailLoader::ContactReceived( 
    CPhCntContact* aContact, 
    TInt aError )
    {
    if( aError == KErrNone )
        {
        iReceivedContact = aContact;
        TRAPD( err, iOperation = iReceivedContact->LoadThumbnailL( *this ) );
        if( err != KErrNone ) 
            {
            NotifyObserver( NULL, err );
            delete iReceivedContact;
            iReceivedContact = NULL;
            }
        }
    else 
        {
        NotifyObserver( NULL, aError );
        }
    }

// ---------------------------------------------------------------------------
// From class MPbk2ImageGetObserver
// ---------------------------------------------------------------------------
//  
void CPhCntVPbkThumbnailLoader::Pbk2ImageGetComplete( 
    MPbk2ImageOperation& /*aOperation*/, 
    CFbsBitmap* aBitmap )
    {
    delete iOperation;
    iOperation = NULL;
    
    delete iReceivedContact;
    iReceivedContact = NULL;
    
    NotifyObserver( aBitmap, KErrNone );
    }

// ---------------------------------------------------------------------------
// From class MPbk2ImageGetObserver
// ---------------------------------------------------------------------------
//
void CPhCntVPbkThumbnailLoader::Pbk2ImageGetFailed( 
    MPbk2ImageOperation& /*aOperation*/, 
    TInt aError )
    {
    delete iOperation;
    iOperation = NULL;
    
    delete iReceivedContact;
    iReceivedContact = NULL;
    
    NotifyObserver( NULL, aError );
    }

