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
* Description:  Contact identifier used with virtual phonebook
*
*/


#include "MPhCntContactManager.h"
#include "cphcntvpbkcontactid.h"
#include "MVPbkContactLink.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntVPbkContactId::CPhCntVPbkContactId( 
    MVPbkContactLink* aContactLink,
    MPhCntContactManager& aContactManager ) : 
    iContactLink( aContactLink ),
    iContactManager( aContactManager )
    {
    }


// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CPhCntVPbkContactId::ConstructL( 
    TContactItemId aContactItemId )
    {
    iContactLink = 
        iContactManager.ConvertContactIdToLinkL( aContactItemId );
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CPhCntVPbkContactId::ConstructL( const TDesC8& aContactLink )
    {
    iContactLink =
        iContactManager.ConvertDescriptorToLinkL( aContactLink );
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntVPbkContactId* CPhCntVPbkContactId::NewL(
    MVPbkContactLink* aContactLink,
    MPhCntContactManager& aContactManager )
    {
    CPhCntVPbkContactId* self = 
        CPhCntVPbkContactId::NewLC( aContactLink, aContactManager );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntVPbkContactId* CPhCntVPbkContactId::NewLC( 
    TContactItemId aContactId,
    MPhCntContactManager& aContactManager )
    {
    CPhCntVPbkContactId* self = 
        new( ELeave ) CPhCntVPbkContactId( 
            NULL, aContactManager );
    CleanupStack::PushL( self );
    self->ConstructL( aContactId );
    return self;
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntVPbkContactId* CPhCntVPbkContactId::NewL(
    TContactItemId aContactId,
    MPhCntContactManager& aContactManager )
    {
    CPhCntVPbkContactId* self = 
        CPhCntVPbkContactId::NewLC( aContactId, aContactManager );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntVPbkContactId* CPhCntVPbkContactId::NewLC( 
    MVPbkContactLink* aContactLink,
    MPhCntContactManager& aContactManager )
    {
    CPhCntVPbkContactId* self = 
        new( ELeave ) CPhCntVPbkContactId( 
            aContactLink,
            aContactManager );
    CleanupStack::PushL( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//    
CPhCntVPbkContactId* CPhCntVPbkContactId::NewL( 
    const TDesC8& aContactLink,
    MPhCntContactManager& aContactManager )
    {
    CPhCntVPbkContactId* self = 
        new( ELeave )CPhCntVPbkContactId( NULL, aContactManager );
    CleanupStack::PushL( self );
    self->ConstructL( aContactLink );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntVPbkContactId::~CPhCntVPbkContactId()
    {
    delete iContactLink;
    }

// ---------------------------------------------------------------------------
// Gives the contact link
// ---------------------------------------------------------------------------
//
const MVPbkContactLink& CPhCntVPbkContactId::ContactLink() const
    {
    return *iContactLink;
    }

// ---------------------------------------------------------------------------
// From class CPhCntContactId
// Clones this object.
// ---------------------------------------------------------------------------
//
CPhCntContactId* CPhCntVPbkContactId::CloneL() const
    {
    CPhCntContactId* clone = NULL;
    if( iContactLink ) 
        {
        MVPbkContactLink* clonedLink = iContactLink->CloneLC();
        clone = CPhCntVPbkContactId::NewL( clonedLink, iContactManager );
        CleanupStack::Pop(); // clonedLink
        }
    return clone;
    }
    
// ---------------------------------------------------------------------------
// From class CPhCntContactId
// Invalidates this contact id
// ---------------------------------------------------------------------------
//
void CPhCntVPbkContactId::Invalidate()
    {
    delete iContactLink;
    iContactLink = NULL;
    }
    
// ---------------------------------------------------------------------------
// From class CPhCntContactId
// Is valid
// ---------------------------------------------------------------------------
//
TBool CPhCntVPbkContactId::IsValid() const
    {
    TBool isValid = EFalse;
    if( iContactLink )
        {
        isValid = ETrue;
        }
    return isValid;
    }
    
// ---------------------------------------------------------------------------
// From class CPhCntContactId
// Contact id
// ---------------------------------------------------------------------------
//
TContactItemId CPhCntVPbkContactId::ContactId() const
    {
    TContactItemId contactId = KNullContactId;
    if( iContactLink ) 
        {
        contactId = iContactManager.ConvertContactLinkToContactId( *iContactLink );
        }
    return contactId;
    }
    
// ---------------------------------------------------------------------------
// From class CPhCntContactId
// Packs contact link.
// ---------------------------------------------------------------------------
//
HBufC8* CPhCntVPbkContactId::PackLC() const
    {
    HBufC8* pack = NULL;
    if( iContactLink )
        {
        pack = iContactLink->PackLC();
        }
    return pack;
    }
