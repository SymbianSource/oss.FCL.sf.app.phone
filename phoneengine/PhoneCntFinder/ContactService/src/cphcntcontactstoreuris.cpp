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
* Description:  Contact store Uris
*
*/

#include <CVPbkContactStoreUriArray.h>
#include <VPbkContactStoreUris.h>
#include <TVPbkContactStoreUriPtr.h>
#include <centralrepository.h>
#include <talogger.h>

#include "telinternalcrkeys.h"
#include "cphcntcontactstoreuris.h"
#include "mphcntcontactstoreeventobserver.h"

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntContactStoreUris* CPhCntContactStoreUris::NewL()
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntContactStoreUris::NewL" );
    CPhCntContactStoreUris* self = new( ELeave ) CPhCntContactStoreUris;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntContactStoreUris::~CPhCntContactStoreUris()
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntContactStoreUris::~CPhCntContactStoreUris" );
    delete iContactStoreUriArray;
    delete iAdditionalContactStoreUriArray;    
    iContactStoreStates.Close();
    }

// ---------------------------------------------------------------------------
// Contact store uris
// ---------------------------------------------------------------------------
//
CVPbkContactStoreUriArray& CPhCntContactStoreUris::ContactStores() const
    {
    return *iContactStoreUriArray;
    }
    
// ---------------------------------------------------------------------------
// Currently active contact stores.
// ---------------------------------------------------------------------------
//
CVPbkContactStoreUriArray* CPhCntContactStoreUris::ActiveContactStoresL() const
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntContactStoreUris::ActiveContactStoresL" );
    CVPbkContactStoreUriArray* array = CVPbkContactStoreUriArray::NewLC();
    const TInt count( iContactStoreStates.Count() );
    for( TInt i = 0; i < count; i++ )
        {
        const TPhCntContactStoreState& storeState = iContactStoreStates[i];
        if( storeState.IsAvailable() )
            {
            TEFLOGSTRING2( KTAOBJECT,
                    "CNT CPhCntContactStoreUris::ActiveContactStoresL - Append store: %S", 
                    &storeState.ContactStoreUri().UriDes() );
            array->AppendL( storeState.ContactStoreUri() );
            }
        }
    CleanupStack::Pop( array );
    return array;
    }

// ---------------------------------------------------------------------------
// Updates store availability
// ---------------------------------------------------------------------------
//
void CPhCntContactStoreUris::StoreReady( 
    TVPbkContactStoreUriPtr& aContactStoreUri )
    {
    TEFLOGSTRING2( KTAOBJECT, "CNT CPhCntContactStoreUris::StoreReady = %S", 
                &aContactStoreUri.UriDes() );
    SetContactStoreAvailability( aContactStoreUri, ETrue );
    }

// ---------------------------------------------------------------------------
// Updates store availability
// ---------------------------------------------------------------------------
//    
void CPhCntContactStoreUris::StoreUnavailable( 
    TVPbkContactStoreUriPtr& aContactStoreUri )
    {
    TEFLOGSTRING2( KTAOBJECT, "CNT CPhCntContactStoreUris::StoreUnavailable = %S", 
            &aContactStoreUri.UriDes() );
    SetContactStoreAvailability( aContactStoreUri, EFalse );
    }

// ---------------------------------------------------------------------------
// Sets observer
// ---------------------------------------------------------------------------
//     
void CPhCntContactStoreUris::SetObserver( 
    MPhCntContactStoreEventObserver& aObserver )
    {
    iObserver = &aObserver;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntContactStoreUris::CPhCntContactStoreUris()
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntContactStoreUris::ConstructL()
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntContactStoreUris::ConstructL" );
    iAdditionalContactStoreUriArray = CVPbkContactStoreUriArray::NewL();
        
    // Read additional, product configured contact stores.
    HBufC* productSpecificList = HBufC::NewLC( 
        NCentralRepositoryConstants::KMaxUnicodeStringLength );
    TPtr productSpecificListPtr( productSpecificList->Des() );
    ReadAdditionalContactStoreL( &productSpecificListPtr );
    
    // Append additional stores to contact store array
    TInt count( iAdditionalContactStoreUriArray->Count() );       
    for( TInt i = 0; i < count; i++ )
        {
        iContactStoreStates.AppendL( (*iAdditionalContactStoreUriArray)[i] );
        }            
    
    iContactStoreStates.AppendL( 
        TVPbkContactStoreUriPtr( 
            VPbkContactStoreUris::DefaultCntDbUri() ) );
    iContactStoreStates.AppendL( 
        TVPbkContactStoreUriPtr( 
            VPbkContactStoreUris::SimGlobalOwnNumberUri() ) );
    iContactStoreStates.AppendL( 
        TVPbkContactStoreUriPtr( 
            VPbkContactStoreUris::SimGlobalAdnUri() ) );
    iContactStoreStates.AppendL( 
        TVPbkContactStoreUriPtr( 
            VPbkContactStoreUris::SimGlobalFdnUri() ) );
    iContactStoreStates.AppendL( 
        TVPbkContactStoreUriPtr( 
            VPbkContactStoreUris::SimGlobalSdnUri() ) );
            
    const TInt count_2( iContactStoreStates.Count() );
    iContactStoreUriArray = CVPbkContactStoreUriArray::NewL();
    for( TInt i = 0; i < count_2; i++ )
        {
        TEFLOGSTRING3( KTAOBJECT, 
                "CNT CPhCntContactStoreUris::ConstructL - Append store  = %S, Available: %d", 
                &iContactStoreStates[i].ContactStoreUri().UriDes(),
                iContactStoreStates[i].IsAvailable() );
        iContactStoreUriArray->AppendL( iContactStoreStates[i].ContactStoreUri() );
        }
    CleanupStack::PopAndDestroy( productSpecificList );                    
    }

// ---------------------------------------------------------------------------
// Sets contact stores availability
// ---------------------------------------------------------------------------
//    
void CPhCntContactStoreUris::SetContactStoreAvailability( 
        TVPbkContactStoreUriPtr& aStoreUri, 
        TBool aIsAvailable )
    {
    TEFLOGSTRING( KTAOBJECT, "CPhCntContactStoreUris::SetContactStoreAvailability" );
    const TInt count( iContactStoreStates.Count() );
    for( TInt i = 0; i < count; i++ )
        {
        TPhCntContactStoreState& storeState = iContactStoreStates[i];
        if( storeState.ContactStoreUri().Compare( 
            aStoreUri, 
            TVPbkContactStoreUriPtr::EContactStoreUriAllComponents ) 
            == KErrNone ) 
            {
            TEFLOGSTRING2( KTAOBJECT, "SetAvailability: %d", aIsAvailable );
            storeState.SetAvailability( aIsAvailable );
            }
        }
    if( iObserver )
        {
        iObserver->ContactStoreAvailabilityChanged();
        }
    }
    
// ---------------------------------------------------------------------------
// Read list of additional contact stores from cenrep
// ---------------------------------------------------------------------------
//
void CPhCntContactStoreUris::ReadAdditionalContactStoreL( TDes* aProductSpecificList )
    {
    // Read The product specific VPBK stores from cenrep
    CRepository* repository = CRepository::NewLC( KCRUidTelVariation );
    
    TInt err( KErrNone );        
    err = repository->Get( KTelAdditionalStores, *aProductSpecificList );

    if ( err == KErrNone && aProductSpecificList->Length() )
        {
        ParseStoresL( *aProductSpecificList );
        } // if ( err == KErrNone && product...    
     CleanupStack::PopAndDestroy( repository );
    }


// ---------------------------------------------------------------------------
// Adds contact store.
// ---------------------------------------------------------------------------
//
void CPhCntContactStoreUris::AddContactStoreL( 
        const TVPbkContactStoreUriPtr& aContactStoreUri )
    {
    TPhCntContactStoreState candidate 
        = TPhCntContactStoreState( aContactStoreUri );
    
    TInt index = iContactStoreStates.Find( candidate, MatchUris );
    if ( KErrNotFound == index )
        {
        iContactStoreStates.AppendL( candidate );
        }
    }

// ---------------------------------------------------------------------------
// Removes contact store.
// ---------------------------------------------------------------------------
//
void CPhCntContactStoreUris::RemoveContactStore( 
        const TVPbkContactStoreUriPtr& aContactStoreUri )
    {
    TPhCntContactStoreState candidate 
        = TPhCntContactStoreState( aContactStoreUri );
    
    TInt index = iContactStoreStates.Find( candidate, MatchUris );
    if ( KErrNotFound != index )
        {
        iContactStoreStates.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// Parses store names from comma separated list.
// ---------------------------------------------------------------------------
//
void CPhCntContactStoreUris::ParseStoresL( const TDesC& aProductSpecificList )
    {
    const TChar KStoreSeparatorComma = ',';
    TChar chr;
    TLex storeToken ( aProductSpecificList );        
    HBufC* db;
    
    while ( !storeToken.Eos() )
        {
        // Skip spaces 
        // e.g. "  store1,store2"
        storeToken.SkipSpaceAndMark();  

        // if several commas in row, skip them all.
        while ( storeToken.Peek() == KStoreSeparatorComma )
            {
            // e.g. ",,store1,store2"
            storeToken.SkipAndMark(1);
            // Or ",[space],,"
            storeToken.SkipSpaceAndMark();
            }

        // Loop until ',' or eof to parse next store
        do 
            {
            chr = storeToken.Get();
            }
            while ( chr != KStoreSeparatorComma && !storeToken.Eos() );

        if ( !storeToken.Eos()  )
            {
            // Reverse not to include comma.
            storeToken.UnGet();
            }

        if ( storeToken.TokenLength() )
            {
            // Append to parsed stores array, so that visibility of
            // all additional DB URIs is maintained.
            db = HBufC::NewLC( storeToken.TokenLength() );            
            db->Des().Copy( storeToken.MarkedToken() ); 
            
            // Append to additional store array.
            iAdditionalContactStoreUriArray->AppendL( TVPbkContactStoreUriPtr( *db ) );
            CleanupStack::PopAndDestroy( db );  
            //CleanupStack::Pop( db );                      
            }

        } // while ( !storeToken.Eos() )..        
    }

// ---------------------------------------------------------------------------
// Additional Contact store uris
// ---------------------------------------------------------------------------
//
const CVPbkContactStoreUriArray& CPhCntContactStoreUris::AdditionalContactStores() const
    {
    return *iAdditionalContactStoreUriArray;
    }    

// ---------------------------------------------------------------------------
// Implements TIdentityRelation for class TPhCntContactStoreState.
// ---------------------------------------------------------------------------
//
TBool CPhCntContactStoreUris::MatchUris( 
        const TPhCntContactStoreState& aState1,
        const TPhCntContactStoreState& aState2 )
    {
    TVPbkContactStoreUriPtr uri1( aState1.ContactStoreUri() );
    TVPbkContactStoreUriPtr uri2( aState2.ContactStoreUri() );
    
    return ( 
        uri1.Compare( uri2, 
            TVPbkContactStoreUriPtr::EContactStoreUriAllComponents ) 
                == 0 );
    }


