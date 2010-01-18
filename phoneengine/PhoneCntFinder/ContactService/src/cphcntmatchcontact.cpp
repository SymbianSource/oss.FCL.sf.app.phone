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
* Description:  Matches contact
*
*/

#include <talogger.h>

#include <MVPbkContactLinkArray.h>

#include "cphcntmatchcontact.h"
#include "mphcntcontactmatchstrategy.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntMatchContact::CPhCntMatchContact( )
    {
    }


// ---------------------------------------------------------------------------
// Secondphase constructor
// ---------------------------------------------------------------------------
//
void CPhCntMatchContact::ConstructL()
    {
    BaseConstructL();
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntMatchContact* CPhCntMatchContact::NewL()
    {
    CPhCntMatchContact* self = CPhCntMatchContact::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntMatchContact* CPhCntMatchContact::NewLC()
    {
    CPhCntMatchContact* self = new( ELeave ) CPhCntMatchContact;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntMatchContact::~CPhCntMatchContact()
    {
    }
 

// ---------------------------------------------------------------------------
// Matches the contact.
// ---------------------------------------------------------------------------
//
TInt CPhCntMatchContact::MatchContact( 
    const MVPbkContactLinkArray*& aContactLinkArray, 
    const TDesC& aTelNumber,
    MPhCntContactMatchStrategy& aMatchStrategy )
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntMatchContact::MatchContact" );
    
    iMatchStrategy = &aMatchStrategy;
    iPhoneNumber.Set( aTelNumber );
    TInt error = MakeAsyncRequest();
    
    if( !error )
        {
        if( iFetchedContactLinks )        
            {
            const TInt numberOfFoundContactLinks( 
                iFetchedContactLinks->Count() );
            if( numberOfFoundContactLinks == 0 )
                {
                error = KErrNotFound;
                }
            else 
                {
                aContactLinkArray = iFetchedContactLinks;                
                }
            }
        else
            {
            error = KErrNotFound;
            }
        }
    
    if( error )
        {
        delete iFetchedContactLinks;
        iFetchedContactLinks = NULL;
        }
        
    return error;
    }
    
// ---------------------------------------------------------------------------
// From class CPhCntAsynctToSync.
// Makes the asynchronous request to find matches for phone number.
// ---------------------------------------------------------------------------
//
void CPhCntMatchContact::DoMakeAsyncRequestL()
    {
    iMatchStrategy->FindMatchesL( iPhoneNumber );
    }
