/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CPhCntSpeedDialContactL class.
*
*/


#include "cphcntspeeddialcontactlinkfetch.h"
#include "MPhCntContactManager.h"
#include "MVPbkContactLinkArray.h"
#include "MVPbkContactOperationBase.h"



// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CPhCntSpeedDialContactLinkFetch::CPhCntSpeedDialContactLinkFetch(
    MPhCntContactManager& aContactManager ) :
    iContactManager( aContactManager )
    {
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void CPhCntSpeedDialContactLinkFetch::ConstructL()
    {
    CPhCntContactLinkArrayFetch::BaseConstructL();
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CPhCntSpeedDialContactLinkFetch* CPhCntSpeedDialContactLinkFetch::NewL(
    MPhCntContactManager& aContactManager )
    {
    CPhCntSpeedDialContactLinkFetch* self = 
        CPhCntSpeedDialContactLinkFetch::NewLC( aContactManager );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CPhCntSpeedDialContactLinkFetch* CPhCntSpeedDialContactLinkFetch::NewLC(
    MPhCntContactManager& aContactManager )
    {
    CPhCntSpeedDialContactLinkFetch* self = 
        new( ELeave ) CPhCntSpeedDialContactLinkFetch( aContactManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CPhCntSpeedDialContactLinkFetch::~CPhCntSpeedDialContactLinkFetch()
    {
    }

// ---------------------------------------------------------------------------
// ?implementation_description
// ---------------------------------------------------------------------------
//
const MVPbkContactLink& CPhCntSpeedDialContactLinkFetch::FetchSpeedDialLinkL( 
    TInt aSpeedDialPosition )
    {
    iSpeedDialPosition = aSpeedDialPosition;
    TInt error( MakeAsyncRequest() );
    
    delete iOperation; 
    iOperation = NULL;
    
    const MVPbkContactLink* contactLink = NULL;
    if( !error ) 
        {
        if( iFetchedContactLinks && iFetchedContactLinks->Count() == 1 )
            {
            contactLink = &iFetchedContactLinks->At( 0 );
            }
        else
            {
            delete iFetchedContactLinks;
            iFetchedContactLinks = NULL;
            error = KErrNotFound;
            }
        }
    User::LeaveIfError( error );
    
    return *contactLink;
    }

// ---------------------------------------------------------------------------
// From CPhCntAsyncToSync
// ?implementation_description
// ---------------------------------------------------------------------------
//
void CPhCntSpeedDialContactLinkFetch::DoMakeAsyncRequestL()
    {
    if( !IsActive() )
        {
        iOperation = iContactManager.RetrieveSpeedDialContactLinkL( 
            iSpeedDialPosition, *this );
        }
    else
        {
        User::Leave( KErrInUse );
        }
    }
