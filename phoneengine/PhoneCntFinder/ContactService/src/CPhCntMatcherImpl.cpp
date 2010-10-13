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
#include <talogger.h>



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
    delete iCSMatchStrategy;
    iCSMatchStrategy = 0;
    iCSMatchStrategy = iContactManager.CreateContactMatchStrategyL( *iMatchContact,
            MPhCntContactManager::EDontRemoveDuplicates ); 
    TInt err( iMatchContact->MatchContact( linkArray, aTelNumber,
        *iCSMatchStrategy ) );

    CPhCntContact* match( NULL );
    TInt index( KErrNotFound );

    // Apply exact match on additional stores first.
    // If match is found, don't care about other stores as
    // these come first.
    if(  !err && ( linkArray->Count() > 1 ) )
        {
        index = MatchFromAdditionalStore( *linkArray );

        // Single match on additonal stores
        if ( index != KErrNotFound && index != KErrAlreadyExists )
            {
            FetchContact( match, linkArray->At( index ), aTelNumber );
            aMatch = match;
            return err;
            }
        else if ( index == KErrAlreadyExists ) //Several matches on additional stores
            {
            aMatch = match;
            return KErrNotFound;
            }
        }

    if(  !err && ( linkArray->Count() > 1 ) )
        {
        delete iCSMatchStrategy;
        iCSMatchStrategy = 0;
        iCSMatchStrategy = iContactManager.CreateContactMatchStrategyL(
            *iMatchContact, MPhCntContactManager::ERemoveDuplicates ); 
        err = iMatchContact->MatchContact( linkArray, aTelNumber,
            *iCSMatchStrategy );
        }
    if ( !err )
        {
        // If only one contact, no additional checks needed
        if ( linkArray->Count() == 1 )
            {
            index = 0;
            }
        }


    // Finally fetch contact details
    if ( index != KErrNotFound )
        {
        FetchContact( match, linkArray->At( index ), aTelNumber );
        }

    aMatch = match;
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

// --------------------------------------------------------------------------
// CPhCntMatcherImpl::MatchFromAdditionalStore
// --------------------------------------------------------------------------
//
TInt CPhCntMatcherImpl::MatchFromAdditionalStore(
    const MVPbkContactLinkArray& linkArray ) const
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntMatcherImpl::MatchFromAdditionalStore" );
    TInt ret( KErrNotFound );
    TInt found(0); // count of found contacts from additional stores.

    for ( TInt i = 0; i < linkArray.Count(); i++ )
        {
        TVPbkContactStoreUriPtr uri =
            linkArray.At( i ).ContactStore().StoreProperties().Uri();

        // Compare if contact is from additional store.
        if ( iContactManager.AdditionalContactStoreUris().IsIncluded( uri ) )
            {
            // Contact found from additional store.
            found++;
            ret = i;
            }
        }

    if ( found > 1)
        {
        // Multiple matches from additional stores -> no match.
        ret = KErrAlreadyExists;
        }

    return ret;
    }

