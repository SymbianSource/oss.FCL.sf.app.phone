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
* Description:  Implements VoIP specific matching methods.
*
*/


#include <MVPbkContactLinkArray.h>
#include <TVPbkContactStoreUriPtr.h>
#include <MVPbkContactStore.h>
#include <MVPbkContactStoreProperties.h>

#include "cphcntmatchervoipimpl.h"
#include "tphcntsipuri.h"
#include "cphcntfoundcontacts.h"
#include "cphcntmatchcontact.h"
#include "cphcntvoipcontactmatchstrategy.h"
#include "tphcntvoipmatchArray.h"
#include "MPhCntMatch.h"
#include "CPhCntContact.h"
#include "cphcntvpbkcontactid.h"
#include "cphcntfetchcontact.h"
#include "CPhCntContactManager.h"

#include "cphcntstoreloaderimpl.h" //

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntMatcherVoIPImpl* CPhCntMatcherVoIPImpl::NewL(
    const MPhoneCntPbkOwner& aOwner )
    {
    CPhCntMatcherVoIPImpl* self = 
        new( ELeave ) CPhCntMatcherVoIPImpl( aOwner );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntMatcherVoIPImpl::~CPhCntMatcherVoIPImpl()
    {
    delete iVoipMatchStrategy;
    delete iContactStoreLoader;
    }
    
// ---------------------------------------------------------------------------
// From class CPhCntMatcher
// Matches voip contacts
// ---------------------------------------------------------------------------
//    
TInt CPhCntMatcherVoIPImpl::MatchVoipNumber(
    MPhCntMatch*& aMatch,
    const TDesC& aMatchString,
    TBool aAllowUserNameMatch,
    MDesCArray* aContactStoreUris,
    TInt aCharsForMatching )
    {    
    TPhCntSipURI sipUri( aMatchString, aCharsForMatching );
    TInt err = CreateMatcher();    

    if ( err == KErrNone )
    {
    CPhCntContact* existingContact = 
        iFoundContacts->FindContact( sipUri.SipURI() );
    if( !existingContact )
        {
        // Check for service specific contact stores and
        // open stores if not open
        if ( aContactStoreUris )
            {
            TInt storeCount = aContactStoreUris->MdcaCount();
            for( TInt i = 0; i < storeCount ; i++ )
                {
                TPtrC storeUri = aContactStoreUris->MdcaPoint( i );
               
                err = iContactStoreLoader->LoadContactStoreWithUri( storeUri );              
                }  
            }

        const MVPbkContactLinkArray* linkArray = NULL;
        
        // Find possible contacts.
        if( aAllowUserNameMatch && !aCharsForMatching )
            {
            err = iMatchContact->MatchContact(
                linkArray, sipUri.UserNamePart(), *iVoipMatchStrategy );
            }
        else if ( aCharsForMatching )
            {
            err = iMatchContact->MatchContact(
                linkArray, sipUri.FixedUserNamePart(), *iVoipMatchStrategy );            
            }
        else
            {
            err = iMatchContact->MatchContact( 
                linkArray, sipUri.SipURI() , *iVoipMatchStrategy );    
            }
        
            
        MPhCntMatch* match = NULL;
        if( !err )
            {
            // Fetch all the matched contacts, because iVoipMatchStrategy
            // could give us a match that is matched to wrong contact field.
            const TInt matchedContacts( linkArray->Count() );

            // Find real matches.
            TPhCntVoipMatchArray voipMatches;
            for( TInt i = 0; i < matchedContacts; i++ )
                {
                // Get contacts from phone book.
                CPhCntContact* match = NULL;
                err = FetchContact( match, linkArray->At( i ), sipUri.SipURI() );
                if( !err )
                    {
                    TRAP_IGNORE( voipMatches.AppendL( match ) );
                    }
                else
                    {
                    // Error in fetching contacts 
                    break;
                    }
                }
                
            if( aAllowUserNameMatch || aCharsForMatching )   
                {
                match = voipMatches.FindFullOrUsernameMatch( sipUri, aCharsForMatching );
                }
            else
                {
                // Take the first match, that is voip contact. Gives NULL if match not found.
                match = voipMatches.FindFullMatch( sipUri );
                }
            
            // Release extra matches
            voipMatches.ReleaseMatches();
            }
        
        // If no error and match still null pointer, then no contacts were found.
        if( !err && !match )        
            {
            err = KErrNotFound;
            }
        else
            {
            aMatch = match;
            }
        }
    else
        {
        aMatch = existingContact;
        }
    }

    return err;
    }
    
// ---------------------------------------------------------------------------
// From class CPhCntMatcher
// Matches voip contacts
// ---------------------------------------------------------------------------
TInt CPhCntMatcherVoIPImpl::MatchVoipNumber(
            MPhCntMatch*& aMatch,
            const CPhCntContactId& aContactId )
    {
    const CPhCntVPbkContactId& contactId = 
            static_cast<const CPhCntVPbkContactId&>( aContactId );
            
    const MVPbkContactLink& contactLink = contactId.ContactLink();
    
    
    TInt err = CreateMatcher();    

    const TVPbkContactStoreUriPtr uri 
        = contactLink.ContactStore().StoreProperties().Uri(); 

    // Open contact store if not opened earlier
    iContactStoreLoader->LoadContactStoreWithUri( uri.UriDes() );
    
    if ( err == KErrNone )
    {
    CPhCntContact* existingContact = 
        iFoundContacts->FindContact( contactLink );
     
    
    if( !existingContact )
        {
        CPhCntContact* contact = NULL;
        err = iFetchContact->FetchContact( contactLink,  contact );

        if( !err )
            {
            const RArray<TPhCntNumber>& allNumbers = 
                contact->AllNumbers();
            const TInt count( allNumbers.Count() );
            for( TInt i = 0; i < count; i++ )
                {
                // Take first voip number and set it as contacts number.
                TPhCntNumber number = allNumbers[i];
                if( number.Type() == MPhCntMatch::EVoipNumber ||
                    number.Type() == MPhCntMatch::EMobileNumber ||
                    number.Type() == MPhCntMatch::EStandardNumber ||
                    number.Type() == CPhCntContact::EPagerNumber ||
                    number.Type() == CPhCntContact::EVideoNumber ||
                    number.Type() == CPhCntContact::EAssistantNumber ||
                    number.Type() == CPhCntContact::EFaxNumber ||
                    number.Type() == CPhCntContact::ECarNumber ) 
                    {
                    contact->SetMatchedVoipNumber( TPhCntSipURI( number.Number() ) );
                    break;
                    }
                }
            TPhCntSipURI sipUri( contact->Number() );
            TRAP( err, iFoundContacts->AddL( contact, sipUri.SipURI() ) );
            if( err )
                {
                delete contact;
                }
            else
                {
                aMatch = contact;
                }
         
            }
        }
    else
        {
        aMatch = existingContact;
        }
    }
    return err;
    }


// ---------------------------------------------------------------------------
// From class CPhCntMatcher
// Determines if contact has CS numbers.
// ---------------------------------------------------------------------------
//
TBool CPhCntMatcherVoIPImpl::HasCSNumbers( 
    const CPhCntContactId& aContactId )
    {
    TBool hasCSNumbers = EFalse;
    const CPhCntVPbkContactId& contactId = 
            static_cast<const CPhCntVPbkContactId&>( aContactId );
    
    if ( CreateMatcher() == KErrNone )
        {
            
    // Check if we have contact already.
    CPhCntContact* contact = 
        iFoundContacts->FindContact( contactId.ContactLink() );
        
    if( contact ) 
        {
        hasCSNumbers = HasCSNumbers( contact );
        contact->Release();
        }
    else
        {
        // Fetch the contact from Virtual phonebook.
        const TInt err = 
            iFetchContact->FetchContact( contactId.ContactLink(), contact );
        hasCSNumbers = HasCSNumbers( contact );
        
        // We can delete the contact, because it is not added to 
        // iFoundContacts.
        delete contact;
        }
    
        }

    return hasCSNumbers;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntMatcherVoIPImpl::CPhCntMatcherVoIPImpl(
    const MPhoneCntPbkOwner& aOwner ) : 
    CPhCntMatcherImpl( aOwner )
    {
    }


// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntMatcherVoIPImpl::ConstructL()
    {
    CPhCntMatcherImpl::ConstructL();
    iContactStoreLoader = CPhCntStoreLoaderImpl::NewL( iContactManager ); 
    }


// ---------------------------------------------------------------------------
// Determines if contat has other phone numbers than CS numbers.
// ---------------------------------------------------------------------------
//  
TBool CPhCntMatcherVoIPImpl::HasCSNumbers( 
    const CPhCntContact* const aContact )
    {
    
    TBool hasCSNumbers = EFalse;
    if( aContact )
        {
        const RArray<TPhCntNumber>& allNumbers = aContact->AllNumbers();
        const TInt count( allNumbers.Count() );
        for( TInt i = 0; i < count; i++ )
            {
            const MPhCntMatch::TNumberType type = allNumbers[i].Type();
            if( type != MPhCntMatch::ENone &&
                type != MPhCntMatch::EFaxNumber &&
                type != MPhCntMatch::EPagerNumber &&
                type != MPhCntMatch::EVoipNumber 
                )
                {
                hasCSNumbers = ETrue;
                }
            }
        }
    return hasCSNumbers;
    }

TInt CPhCntMatcherVoIPImpl::CreateMatcher()
    {
    TInt err = CPhCntMatcherImpl::CreateMatcher();
    if (!err && !iVoipMatchStrategy )
        {
        TRAP( err, iVoipMatchStrategy = 
            CPhCntVoipContactMatchStrategy::NewL( iContactManager, 
            *iMatchContact ) );
        }
    return err;
    }

    
// End of File
