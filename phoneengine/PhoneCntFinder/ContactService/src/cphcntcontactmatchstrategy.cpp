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
#include <centralrepository.h>
#include <telconfigcrkeys.h>
#include <CVPbkContactStoreUriArray.h>

#include "cphcntcontactmatchstrategy.h"
#include "cphcntcontactstoreuris.h"
#include "CPhoneRawMatchNumberExtractor.h"


// ======== MEMBER FUNCTIONS ========


CPhCntContactMatchStrategy::CPhCntContactMatchStrategy(
    CVPbkContactManager& aContactManager,
    CPhCntContactStoreUris& aContactStoreUris,
    MVPbkContactFindObserver& aObserver,
    MPhCntContactManager::TDuplicateRemovalStrategy aRemoveDuplicatesStrategy ) :
    iContactManager( aContactManager ),
    iContactStoreUris( aContactStoreUris ),
    iObserver( aObserver ),
    iRemoveDuplicatesStrategy( aRemoveDuplicatesStrategy )
    {
    iContactStoreUris.SetObserver( *this );
    }


// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntContactMatchStrategy::ConstructL()
    {        
    iNumberOfDigits = KPhCntMatchDefault;
    
    TInt matchDigitsValue = ReadMatchDigitsValueL();

    // If we can find a proper value from the cenrep, use it.
    if ( matchDigitsValue >= KPhCntMatchMin && matchDigitsValue <= KPhCntMatchMax )
        {
        iNumberOfDigits = matchDigitsValue;            
        }
    else if ( matchDigitsValue == KBestMatchingPhoneNumbers )
        {
        iNumberOfDigits = matchDigitsValue;
        }
                
    User::LeaveIfError( CreateContactMatchStrategy() );
    
    iNumberExtractor = new( ELeave )CCntRawPhoneNumberExtractor();
    iNumberExtractor->ConstructL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CPhCntContactMatchStrategy::ReadMatchDigitsValueL()
    {
    CRepository* cenRepSession = CRepository::NewL( KCRUidTelConfiguration );
    
    TInt matchDigitsValue (KErrNotFound);    
    // Find digit count to be used with matching.
    cenRepSession->Get( KTelMatchDigits, matchDigitsValue );
        
    delete cenRepSession;
    return matchDigitsValue;    
    }

// ---------------------------------------------------------------------------
// Static constructor.
// ---------------------------------------------------------------------------
//
CPhCntContactMatchStrategy* CPhCntContactMatchStrategy::NewL(
    CVPbkContactManager& aContactManager,
    CPhCntContactStoreUris& aContactStoreUris,
    MVPbkContactFindObserver& aObserver,
    MPhCntContactManager::TDuplicateRemovalStrategy aRemoveDuplicatesStrategy )
    {
    CPhCntContactMatchStrategy* self = 
        new( ELeave ) CPhCntContactMatchStrategy( 
            aContactManager,
            aContactStoreUris, 
            aObserver,
            aRemoveDuplicatesStrategy );
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
    TUint32 flags = FillMatchFlags();
    
    CVPbkPhoneNumberMatchStrategy::TConfig config( 
                        iNumberOfDigits,
                        *iUriArray,
                        CVPbkPhoneNumberMatchStrategy::EVPbkSequentialMatch, 
                        flags
                        );
            
    TRAPD( err, iMatchStrategy = CVPbkPhoneNumberMatchStrategy::NewL( 
                    config, 
                    iContactManager, 
                    iObserver ) );       
    return err;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//     
TUint32 CPhCntContactMatchStrategy::FillMatchFlags() const
    {    
    TUint32 flags = CVPbkPhoneNumberMatchStrategy::EVPbkMatchFlagsNone;
    
    if ( iRemoveDuplicatesStrategy == MPhCntContactManager::ERemoveDuplicates )
        {
        flags |= CVPbkPhoneNumberMatchStrategy::EVPbkDuplicatedContactsMatchFlag;
        }
        
    if ( iNumberOfDigits == KBestMatchingPhoneNumbers  )
        {
        flags |= CVPbkPhoneNumberMatchStrategy::EVPbkBestMatchingFlag;
        }
    
    return flags;
    }


// ---------------------------------------------------------------------------
// Makes the actual matching request.
// ---------------------------------------------------------------------------
//     
void CPhCntContactMatchStrategy::DoMatchL( 
    const TDesC& aNumber )
    {
    
    //extra characters should not be removed when using BestMatching algorithm
    if ( iNumberOfDigits == KBestMatchingPhoneNumbers )
        {
        iMatchStrategy->MatchL( aNumber );        
        }
    else 
        {
        TDesC* rawNumber = RemoveExtraCharactersLC( aNumber );
        iMatchStrategy->MatchL( *rawNumber );
        CleanupStack::PopAndDestroy( rawNumber );
        }
    }

// ---------------------------------------------------------------------------
//  Removes postfix from aNumber
// ---------------------------------------------------------------------------
// 
TDesC* CPhCntContactMatchStrategy::RemoveExtraCharactersLC( const TDesC& aNumber )
    {
    HBufC* rawNumber = HBufC::NewLC( aNumber.Length() );
    TPtr rawNumberPtr = rawNumber->Des();
    iNumberExtractor->ExtractRawNumber( aNumber, rawNumberPtr );
    return rawNumber;    
    }

// ---------------------------------------------------------------------------
// Enables to inject match strategy to ease unit testing
// ---------------------------------------------------------------------------
//
void CPhCntContactMatchStrategy::SetVPbkPhoneNumberMatchStrategy( 
        CVPbkPhoneNumberMatchStrategy* aMatchStrategy )
    {
    delete iMatchStrategy;
    iMatchStrategy = aMatchStrategy;
    }

