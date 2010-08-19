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
* Description:  Matcher implementation
*
*/

#include <CVPbkPhoneNumberMatchStrategy.h>
#include <MVPbkContactLinkArray.h>
#include <CPbk2StoreConfiguration.h>
#include <MVPbkContactStoreProperties.h>
#include <MVPbkContactStore.h>
#include <CVPbkContactStoreUriArray.h>
#include <talogger.h>
#include <telephonyvariant.hrh>
#include <telinternalcrkeys.h>
#include <centralrepository.h>

#include "CPhCntMatcherImpl.h"
#include "cphcntmatchcontact.h"
#include "cphcntfetchcontact.h"
#include "cphcntfoundcontacts.h"
#include "CPhCntContact.h"
#include "CPhCntContactManager.h"
#include "CPhCntContactStores.h"
#include "MPhoneCntPbkOwner.h"
#include "cphcntvpbkcontactid.h"
#include "cphcntcontactmatchstrategy.h"

// Local functions
/**
 * Gets contact selection strategy from localvariation flag.
 */
TPhCntContactSelectionStrategy::TAllowSeveralMatches 
    GetContactSelectionStrategyL()
    {
    CRepository* repository = CRepository::NewLC( KCRUidTelVariation );
    TInt variationFlag;
    User::LeaveIfError( repository->Get( KTelVariationFlags, variationFlag ) );
    CleanupStack::PopAndDestroy( repository );
    
    if( variationFlag & KTelephonyLVFlagShowContactWhenSeveralMatches ) 
        {
        return TPhCntContactSelectionStrategy::EAllowSeveralMatches;
        }
    else
        {
        return TPhCntContactSelectionStrategy::EAllowSingleMatch;
        }
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntMatcherImpl* CPhCntMatcherImpl::NewL(
    const MPhoneCntPbkOwner& aOwner )
    {
    CPhCntMatcherImpl* self = new( ELeave )CPhCntMatcherImpl( aOwner );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntMatcherImpl::~CPhCntMatcherImpl()
    {
    delete iCSMatchStrategy;
    delete iFoundContacts;
    delete iContactStores;
    delete iMatchContact;
    delete iFetchContact;
    }

// ---------------------------------------------------------------------------
// From CPhCntMatcher
// Tries to find a contact which has aTelNumber.
// ---------------------------------------------------------------------------
//
TInt CPhCntMatcherImpl::MatchNumber(
    MPhCntMatch*& aMatch,
    const TDesC& aTelNumber )
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntMatcherImpl::MatchNumber" );
    TInt err = CreateMatcher();
    if ( !err )
        {
        // Check if we already have the contact.
        CPhCntContact* contact = iFoundContacts->FindContact( aTelNumber );

        err = KErrNone;
        if( !contact )
            {
            // Get contact from contact stores
            TRAPD( traperr, err = GetContactL( aMatch, aTelNumber ));
            if ( traperr )
                {
                err = traperr;
                }
            }
        else
            {
            aMatch = contact;
            }
        }
    TEFLOGSTRING2( KTAOBJECT, "CNT CPhCntMatcherImpl::MatchNumber %d " , err);
    return err;
    }

// ---------------------------------------------------------------------------
// From CPhCntMatcher
// Tries to find a contact which has aTelNumber.
// ---------------------------------------------------------------------------
//
TInt CPhCntMatcherImpl::MatchNumber(
    MPhCntMatch*& aMatch,
    const TDesC& aTelNumber,
    const CPhCntContactId& aContactId )
    {
    TInt err = CreateMatcher();
    if ( !err )
        {
        if( aContactId.IsValid() )
            {
            // Do we have existing contact for the link and number.
            const CPhCntVPbkContactId& contactId =
                static_cast<const CPhCntVPbkContactId&>( aContactId );
            const MVPbkContactLink& link = contactId.ContactLink();
            aMatch =
                iFoundContacts->FindContact( aTelNumber, link );
            if( !aMatch )
                {
                // Get the contact.
                CPhCntContact* match = NULL;
                err = FetchContact( match, link, aTelNumber );
                if( !err )
                    {
                    aMatch = match;
                    }
                }
            }
        else
            {
            err = MatchNumber( aMatch, aTelNumber );
            }
        }
    return err;
    }

// ---------------------------------------------------------------------------
// From CPhCntMatcher
// Empty implementation. Implemented in CPhCntMatcherVoIPImpl.
// ---------------------------------------------------------------------------
//
TInt CPhCntMatcherImpl::MatchVoipNumber(
    MPhCntMatch*& /*aMatch*/,
    const TDesC& /*aMatchString*/,
    TBool /*aAllowUserNameMatch*/,
    MDesCArray* /*aContactStoreUris*/,
    TInt /*aCharsForMatching*/ )
    {
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// From CPhCntMatcher
// Empty implementation. Implemented in CPhCntMatcherVoIPImpl.
// ---------------------------------------------------------------------------
//
TInt CPhCntMatcherImpl::MatchVoipNumber(
    MPhCntMatch*& /*aMatch*/,
    const TDesC& /*aMatchString*/,
    const CPhCntContactId& /*aContactId*/ )
    {
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// From CPhCntMatcher
// Empty implementation. Implemented in CPhCntMatcherVoIPImpl.
// ---------------------------------------------------------------------------
//
TBool CPhCntMatcherImpl::HasCSNumbers(
    const CPhCntContactId& /*aContactId*/ )
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// Gets contact from contact stores
// ---------------------------------------------------------------------------
//
TInt CPhCntMatcherImpl::GetContactL(
    MPhCntMatch*& aMatch,
    const TDesC& aTelNumber )
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntMatcherImpl::GetContactL" );
    // Try to find matching contact.
    const MVPbkContactLinkArray* linkArray = NULL;
    TInt err = MatchContactL( linkArray, aTelNumber, 
        MPhCntContactManager::EDontRemoveDuplicates );

    CPhCntContact* match( NULL );
    TInt index( KErrNotFound );

    // Apply exact match on additional stores first.
    // If match is found, don't care about other stores as
    // these come first.
    if( !err )
        {        
        const CVPbkContactStoreUriArray& additionalStores = iContactManager.AdditionalContactStoreUris();
        index = iContactSelectionStrategy.ApplyAdditonalStoreStrategy( *linkArray, additionalStores );  
        
        const TBool manyContactsFound = index == KManyContacts;
        const TBool singleContactFound = 
            index != KNoContact && index != KManyContacts;
        
        if ( singleContactFound )
            {
            FetchContact( match, linkArray->At( index ), aTelNumber );
            aMatch = match;
            return err;
            }
        else if ( manyContactsFound ) 
            {
            aMatch = match;
            return KErrNotFound;
            }
        }

    if(  !err && ( linkArray->Count() > 1 ) )
        {
        err = MatchContactL( linkArray, aTelNumber,
            MPhCntContactManager::ERemoveDuplicates );
        }
    if ( !err )
        {
        index = iContactSelectionStrategy.ApplyStrategy( *linkArray );        
        }

    // Finally fetch contact details
    if ( index != KNoContact )
        {
        FetchContact( match, linkArray->At( index ), aTelNumber );
        aMatch = match;
        }
    else
        {
        err = KErrNotFound;
        }
    
    TEFLOGSTRING2( KTAMESIN,"CNT CPhCntMatcherImpl::GetContactL,err: %d", err );    
    return err;
    }

// ---------------------------------------------------------------------------
// Matches contact
// ---------------------------------------------------------------------------
//
TInt CPhCntMatcherImpl::MatchContactL( 
    const MVPbkContactLinkArray*& aContactLinkArray, 
    const TDesC& aTelNumber,
    MPhCntContactManager::TDuplicateRemovalStrategy aRemoveDuplicatesStrategy )
    {
    delete iCSMatchStrategy;
    iCSMatchStrategy = NULL;
    iCSMatchStrategy = iContactManager.CreateContactMatchStrategyL( 
        *iMatchContact,
        aRemoveDuplicatesStrategy );
    
    const TInt err( iMatchContact->MatchContact( aContactLinkArray, aTelNumber,
        *iCSMatchStrategy ) );
    return err;
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntMatcherImpl::CPhCntMatcherImpl( const MPhoneCntPbkOwner& aOwner ) :
    iContactManager( *aOwner.ContactManager() ),
    iPbkOwner( aOwner )
    {
    }

// ---------------------------------------------------------------------------
// Delayed on-demand based construction
// ---------------------------------------------------------------------------
//
void CPhCntMatcherImpl::DoCreateMatcherL()
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntMatcherImpl::DoCreateMatcherL" );
    if ( !iContactStores )
        {
        iFoundContacts = CPhCntFoundContacts::NewL();
        iContactStores = CPhCntContactStores::NewL( iContactManager );
        iMatchContact = CPhCntMatchContact::NewL();
        iFetchContact = CPhCntFetchContact::NewL( *iContactStores );
        }
    }

// ---------------------------------------------------------------------------
// Delayed on-demand based construction
// ---------------------------------------------------------------------------
//
TInt CPhCntMatcherImpl::CreateMatcher()
    {
    TRAPD( err, DoCreateMatcherL() );
    return err;
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntMatcherImpl::ConstructL()
    {
    const TPhCntContactSelectionStrategy::TAllowSeveralMatches strategy = 
        GetContactSelectionStrategyL();
    iContactSelectionStrategy.SetContactSelectionStrategy( strategy );    
    }

// ---------------------------------------------------------------------------
// Fetches contact
// ---------------------------------------------------------------------------
//
TInt CPhCntMatcherImpl::FetchContact(
    CPhCntContact*& aMatch,
    const MVPbkContactLink& aContactLink,
    const TDesC& aTelNumber )
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntMatcherImpl::FetchContact" );
    CPhCntContact* contact = NULL;
    TInt err = iFetchContact->FetchContact( aContactLink,  contact );
    if( !err )
        {
        TRAP( err, iFoundContacts->AddL( contact, aTelNumber ) );
        if( err )
            {
            delete contact;
            }
        else
            {
            aMatch = contact;
            }
        }
    return err;
    }

