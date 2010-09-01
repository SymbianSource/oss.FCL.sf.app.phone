/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contact matching strategy
*
*/


#include <CVPbkContactManager.h>
#include <CVPbkContactStoreUriArray.h>
#include <cntdb.h>  // KBestMatchingPhoneNumbers

#include "cphcntcontactmatchstrategy.h"
#include "cphcntcontactstoreuris.h"
#include "CPhoneRawMatchNumberExtractor.h"




// ======== MEMBER FUNCTIONS ========


CPhCntContactMatchStrategy::CPhCntContactMatchStrategy(
    CVPbkContactManager& aContactManager,
    CPhCntContactStoreUris& aContactStoreUris,
    MVPbkContactFindObserver& aObserver,
    TUint32 aMatchFlags ) :
    iContactManager( aContactManager ),
    iContactStoreUris( aContactStoreUris ),
    iObserver( aObserver ),
    iMatchFlags( aMatchFlags )
    {
    iContactStoreUris.SetObserver( *this );
    }


// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntContactMatchStrategy::ConstructL()
    {
    User::LeaveIfError( CreateContactMatchStrategy() );
    
    iNumberExtractor = new( ELeave )CCntRawPhoneNumberExtractor();
    iNumberExtractor->ConstructL();
    }


// ---------------------------------------------------------------------------
// Static constructor.
// ---------------------------------------------------------------------------
//
CPhCntContactMatchStrategy* CPhCntContactMatchStrategy::NewL(
    CVPbkContactManager& aContactManager,
    CPhCntContactStoreUris& aContactStoreUris,
    MVPbkContactFindObserver& aObserver,
    TUint32 aMatchFlags )
    {
    CPhCntContactMatchStrategy* self = 
        new( ELeave ) CPhCntContactMatchStrategy( 
            aContactManager,
            aContactStoreUris, 
            aObserver,
            aMatchFlags );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CPhCntContactMatchStrategy::~CPhCntContactMatchStrategy()
    {
    if( iNumberExtractor )
        {
        iNumberExtractor->Release();    
        }
    delete iMatchStrategy;
    delete iUriArray;
    }

// ---------------------------------------------------------------------------
// From MPhCntContactMatchStrategy
// Starts to find contacts which has aPhoneNumber.
// ---------------------------------------------------------------------------
//
void CPhCntContactMatchStrategy::FindMatchesL( const TDesC& aPhoneNumber )
    {
    if( iMatchStrategy )
        {
        DoMatchL( aPhoneNumber );
        }
    else
        {
        const TInt err( CreateContactMatchStrategy() );
        if( !err )
            {
            DoMatchL( aPhoneNumber );
            }
        else
            {
            User::Leave( KErrNotFound );            
            }
        }
    }

// ---------------------------------------------------------------------------
// Creates contact match strategy and destroys the old one.
// ---------------------------------------------------------------------------
//    
TInt CPhCntContactMatchStrategy::CreateContactMatchStrategy()
    {
    TInt err = KErrNone;
    if( !iUriArray )
        {
        TRAP( err, iUriArray = iContactStoreUris.ActiveContactStoresL() );
        }
    
    if( iUriArray )
        {
        delete iMatchStrategy;
        iMatchStrategy = NULL;
        err = DoCreateMatchStrategy();
         
        }
    else
        {
        err = KErrGeneral;
        }
    
    return err;                
    }

// ---------------------------------------------------------------------------
// From base class MPhCntContactStoreEventObserver
// Updates match strategy.
// ---------------------------------------------------------------------------
//
void CPhCntContactMatchStrategy::ContactStoreAvailabilityChanged()
    {
    delete iUriArray;
    iUriArray = NULL;
    CreateContactMatchStrategy();
    }

// ---------------------------------------------------------------------------
// Creates contact match strategy
// ---------------------------------------------------------------------------
//     
TInt CPhCntContactMatchStrategy::DoCreateMatchStrategy()
    {
    CVPbkPhoneNumberMatchStrategy::TConfig config( 
            KBestMatchingPhoneNumbers,
            *iUriArray,
            CVPbkPhoneNumberMatchStrategy::EVPbkSequentialMatch, 
            iMatchFlags
            );
    TRAPD( err, iMatchStrategy = CVPbkPhoneNumberMatchStrategy::NewL( 
                    config, 
                    iContactManager, 
                    iObserver ) );       
    return err;
    }

// ---------------------------------------------------------------------------
// Makes the actual matching request.
// ---------------------------------------------------------------------------
//     
void CPhCntContactMatchStrategy::DoMatchL( 
    const TDesC& aNumber )
    {
    iMatchStrategy->MatchL( aNumber );
    }
