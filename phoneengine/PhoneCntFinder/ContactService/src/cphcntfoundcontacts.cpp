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
* Description:  Stores found contacts and keeps track of the references.
*
*/


#include <MVPbkContactLink.h>

#include "cphcntfoundcontacts.h"
#include "CPhCntContact.h"
#include <talogger.h>


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntFoundContacts::CPhCntFoundContacts() 
    {
    }

// ---------------------------------------------------------------------------
// Secondphase constructor.
// ---------------------------------------------------------------------------
//
void CPhCntFoundContacts::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntFoundContacts* CPhCntFoundContacts::NewL()
    {
    CPhCntFoundContacts* self = CPhCntFoundContacts::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntFoundContacts* CPhCntFoundContacts::NewLC()
    {
    CPhCntFoundContacts* self = new( ELeave ) CPhCntFoundContacts;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntFoundContacts::~CPhCntFoundContacts()
    {
    iFoundContacts.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// Adds contact-
// ---------------------------------------------------------------------------
//
void CPhCntFoundContacts::AddL( 
    CPhCntContact* aContact,
    const TDesC& aMatchingNumber )
    {
    aContact->SetOriginalNumberL( aMatchingNumber );
    iFoundContacts.AppendL( aContact );
    aContact->SetOwner( this );
    // There is already reference to aContact, because it is added.
    aContact->IncreaseReferenceCount();
    }
    
// ---------------------------------------------------------------------------
// Removes contact from array and deletes the contact.
// ---------------------------------------------------------------------------
//
void CPhCntFoundContacts::Remove( CPhCntContact* aContact )
    {
    const TInt errorOrIndex( iFoundContacts.Find( aContact ) );
    if( errorOrIndex >= 0 ) 
        {
        iFoundContacts.Remove( errorOrIndex );
        }
    delete aContact;
    }
    
// ---------------------------------------------------------------------------
// Finds contact, which number is same as aTelNumber
// ---------------------------------------------------------------------------
//
CPhCntContact* CPhCntFoundContacts::FindContact( const TDesC& aTelNumber )
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntFoundContacts::FindContact" );
    const TInt foundContactCount( iFoundContacts.Count() );
    CPhCntContact* foundContact = NULL;
    for( TInt i = 0; i < foundContactCount && !foundContact; i++ )
        {
        CPhCntContact* contact = iFoundContacts[i];
        if( aTelNumber.Compare( contact->OriginalNumber() ) == KErrNone )
            {
            foundContact = contact;
            foundContact->IncreaseReferenceCount();
            }
        }
    return foundContact;
    }
    
// ---------------------------------------------------------------------------
// Finds contact, which number and contact link are same.
// ---------------------------------------------------------------------------
//
CPhCntContact* CPhCntFoundContacts::FindContact( 
    const TDesC& aTelNumber,
    const MVPbkContactLink& aContactLink )
    {
    CPhCntContact* foundContact = FindContact( aContactLink );
    if( foundContact )
        {
        if( aTelNumber.Compare( foundContact->OriginalNumber() ) != KErrNone )
            {
            // contact did not match with the number, so release.
            foundContact->Release();
            foundContact = NULL;
            }
        }
    return foundContact;
    }

// ---------------------------------------------------------------------------
// Finds contact, which contact link matches with aContactLink
// ---------------------------------------------------------------------------
//    
CPhCntContact* CPhCntFoundContacts::FindContact(
    const MVPbkContactLink& aContactLink )
    {
    const TInt foundContactCount( iFoundContacts.Count() );
    CPhCntContact* foundContact = NULL;
    for( TInt i = 0; i < foundContactCount && !foundContact; i++ )
        {
        CPhCntContact* contact = iFoundContacts[i];
        if( aContactLink.IsSame( *contact->ContactLink() ) )
            {
            foundContact = contact;
            foundContact->IncreaseReferenceCount();
            }
        }
    return foundContact;
    }

    


