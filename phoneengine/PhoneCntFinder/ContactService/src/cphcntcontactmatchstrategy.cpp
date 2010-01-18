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

// Digit count used to match CS number.
const TInt KPhCntMatchMin = 7;
const TInt KPhCntMatchMax = 11;
const TInt KPhCntMatchDefault = KPhCntMatchMin;

// ======== MEMBER FUNCTIONS ========


CPhCntContactMatchStrategy::CPhCntContactMatchStrategy(
    CVPbkContactManager& aContactManager,
    CPhCntContactStoreUris& aContactStoreUris,
    MVPbkContactFindObserver& aObserver,
    CVPbkPhoneNumberMatchStrategy::TVPbkPhoneNumberMatchFlags aMatchFlag ) :
    iContactManager( aContactManager ),
    iContactStoreUris( aContactStoreUris ),
    iObserver( aObserver ),
    iMatchFlag( aMatchFlag )
    {
    iContactStoreUris.SetObserver( *this );
    }


// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntContactMatchStrategy::ConstructL()
    {
    iCenRepSession = CRepository::NewL( KCRUidTelConfiguration );
    iCenRepNotifyHandler = 
        CCenRepNotifyHandler::NewL(
            *this, 
            *iCenRepSession, 
            CCenRepNotifyHandler::EIntKey,
            KTelMatchDigits );
            
    iCenRepNotifyHandler->StartListeningL();
    
    iNumberOfDigits = KPhCntMatchDefault;
    TInt sdMatchValue = KErrNotFound;

    // Find digit count to be used with matching.
    if ( iCenRepSession->Get( KTelMatchDigits, sdMatchValue )
         == KErrNone )
        {
        // If we can find a proper value from the cenrep, use it.
        if ( sdMatchValue >= KPhCntMatchMin && sdMatchValue <= KPhCntMatchMax )
            {
            iNumberOfDigits = sdMatchValue;
            }
        }
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
    CVPbkPhoneNumberMatchStrategy::TVPbkPhoneNumberMatchFlags aMatchFlag )
    {
    CPhCntContactMatchStrategy* self = 
        new( ELeave ) CPhCntContactMatchStrategy( 
            aContactManager,
            aContactStoreUris, 
            aObserver,
            aMatchFlag );
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
    
    if( iCenRepNotifyHandler )
        {
        iCenRepNotifyHandler->StopListening();    
        }
    delete iCenRepNotifyHandler;
    delete iCenRepSession;
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
        MatchL( aPhoneNumber );
        }
    else
        {
        const TInt err( CreateContactMatchStrategy() );
        if( !err )
            {
            MatchL( aPhoneNumber );
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
// From base class MCenRepNotifyHandlerCallback
// Notification from CenRep
// ---------------------------------------------------------------------------
//  
void CPhCntContactMatchStrategy::HandleNotifyString( 
    TUint32 aUid, 
    const TDesC16& aValue )
    {
    if ( aUid == KTelMatchDigits )
        {
        TLex lexer( aValue );
        TInt value;
        const TInt err( lexer.Val( value ) );
        
        if( !err ) 
            {
            if ( value < KPhCntMatchMin || value > KPhCntMatchMax )
                {
                iNumberOfDigits = KPhCntMatchDefault;
                }
            else
                {
                iNumberOfDigits = value;
                }
            CreateContactMatchStrategy();
            }
     
        }
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
// Removes extra characters from number and matches with raw number.
// ---------------------------------------------------------------------------
//         
void CPhCntContactMatchStrategy::MatchL( 
    const TDesC& aPhoneNumber )
    {
    HBufC* rawNumber = HBufC::NewLC( aPhoneNumber.Length() );
    TPtr rawNumberPtr = rawNumber->Des();
    iNumberExtractor->ExtractRawNumber( aPhoneNumber, rawNumberPtr );
    DoMatchL( *rawNumber );
    CleanupStack::PopAndDestroy( rawNumber );
    }

// ---------------------------------------------------------------------------
// Creates contact match strategy
// ---------------------------------------------------------------------------
//     
TInt CPhCntContactMatchStrategy::DoCreateMatchStrategy()
    {
    CVPbkPhoneNumberMatchStrategy::TConfig config( 
            iNumberOfDigits,
            *iUriArray,
            CVPbkPhoneNumberMatchStrategy::EVPbkSequentialMatch, 
            iMatchFlag
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
