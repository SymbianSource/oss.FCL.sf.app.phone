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
* Description:  Contact data.
*
*/


#include <CPbk2ImageManager.h>

#include <featmgr.h>         // Feature Manager
#include "CPhCntContact.h"
#include "mphcntcontactfields.h"
#include "cphcntfoundcontacts.h"
#include "MPhCntContactManager.h"
#include "MVPbkContactLink.h"
#include "MVPbkContactLinkArray.h"
#include "PhCntTrace.h"
#include "cphcntvpbkcontactid.h"
#include "tphcnttxttospeech.h"

#include "tphcntsipuri.h"
#include "CPhoneRawMatchNumberExtractor.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntContact* CPhCntContact::NewL( 
    MPhCntContactFields* aContactFields,
    MPhCntContactManager& aContactManager )
    {
    CPhCntContact* self = 
        new( ELeave )CPhCntContact( 
            aContactFields, aContactManager );
            
    CleanupStack::PushL( self);
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
  
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//  
CPhCntContact::~CPhCntContact()
    {
    delete iContactFields;
    delete iOriginalNumber;
    delete iContactId;
        
    delete iMatchedVoipNumber;
    if( iNumberExtractor )
        {
        iNumberExtractor->Release();    
        }
    FeatureManager::UnInitializeLib();
    }
    
// ---------------------------------------------------------------------------
// Setter
// ---------------------------------------------------------------------------
//
void CPhCntContact::SetOriginalNumberL( const TDesC& aNumber )
    {
    HBufC* number = aNumber.AllocL();
    delete iOriginalNumber;
    iOriginalNumber = number;
    }

// ---------------------------------------------------------------------------
// Getter
// ---------------------------------------------------------------------------
//    
const TDesC& CPhCntContact::OriginalNumber() const
    {
    return *iOriginalNumber;
    }
    
// ---------------------------------------------------------------------------
// Sets the owner.
// ---------------------------------------------------------------------------
//
void CPhCntContact::SetOwner( CPhCntFoundContacts* aOwner )
    {
    iOwner = aOwner;
    }
    
// ---------------------------------------------------------------------------
// Increases reference count.
// ---------------------------------------------------------------------------
//
void CPhCntContact::IncreaseReferenceCount()
    {
    iReferenceCount++;
    }

// ---------------------------------------------------------------------------
// All phone numbers
// ---------------------------------------------------------------------------
//    
const RArray<TPhCntNumber>& CPhCntContact::AllNumbers() const
    {
    return iContactFields->AllNumbers();
    }

// ---------------------------------------------------------------------------
// DTMF number
// ---------------------------------------------------------------------------
//    
TPtrC CPhCntContact::Dtmf() const
    {
    return iContactFields->Dtmf();
    }
    
// ---------------------------------------------------------------------------
// Sets matched number in Voip case. 
// ---------------------------------------------------------------------------
//   
void CPhCntContact::SetMatchedVoipNumber( 
    const TPhCntSipURI& aSipUri )
    {
    delete iMatchedVoipNumber;
    iMatchedVoipNumber = NULL;
	//just ignore possible alloc failure
	TRAP_IGNORE( iMatchedVoipNumber = HBufC::NewL( 
		aSipUri.SipURI().Length() ) )
    if( iMatchedVoipNumber ) 
        {
        iMatchedVoipNumber->Des().Append( aSipUri.SipURI() );
        }
    }
    
// ---------------------------------------------------------------------------
// Releases this contact if no more references found.
// ---------------------------------------------------------------------------
//
void CPhCntContact::Release() 
    {
    iReferenceCount--;
    // TODO: Panic if reference count < 0
    if( iReferenceCount == 0 )
        {
        // TODO: Panic if iOwner == NULL;
        iOwner->Remove( this );    
        }
    }
    
// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
CPhCntContactId* CPhCntContact::ContactId() const 
    {   
    return iContactId;
    }

// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
MVPbkContactLink* CPhCntContact::ContactLink() const 
    {   
    return iContactFields->ContactLink();
    }

// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
MPhCntMatch::TNumberType CPhCntContact::NumberType() const   
    {
    MPhCntMatch::TNumberType numberType = iContactFields->NumberType();
    // check if numbertype was found
    if ( numberType == MPhCntMatch::ENone )  
        {
        // if not, then try to find numbertype among all numbers
        if ( iOriginalNumber )
            {
            numberType = NumberType( *iOriginalNumber );
            }
        }
    if( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        // If we have iMatchedNumber then there was voip match.
        if( iMatchedVoipNumber ) 
            {
            numberType = MPhCntMatch::EVoipNumber;
            }
        }
    return numberType;
    }    

// ---------------------------------------------------------------------------
// Get number type from number that matches to supplied number 
// Getter
// ---------------------------------------------------------------------------
//    
MPhCntMatch::TNumberType CPhCntContact::NumberType( const TDesC& aNumber ) const   
    {
    const RArray<TPhCntNumber>& allNumbers = AllNumbers();		
    MPhCntMatch::TNumberType numberType = MPhCntMatch::ENone;
    // try to find number from allnumbers array
    for ( TInt i = 0; i < allNumbers.Count(); i++ )
        {
        TPhCntNumber number( allNumbers[i] );
        // compare 7 last digits
        TBuf<KPhMatcherNumberCount> contactNumber;
        TBuf<KPhMatcherNumberCount> originalNumber;
    	iNumberExtractor->ExtractRawNumber( number.Number(), contactNumber ); 
    	iNumberExtractor->ExtractRawNumber( aNumber, originalNumber ); 
    
        if ( originalNumber.Compare( contactNumber ) == KErrNone )
            {
            // number found, get numbertype
            numberType = number.Type();
            break;
            }
        }   
    return numberType;
    } 
       
// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Constructs the cli
// ---------------------------------------------------------------------------
//
MPhCntMatch::TCliType CPhCntContact::Cli( HBufC*& aCliText ) const
    {
    TCliType cliType = ECliEmpty;
    
    HBufC* contactTitle = NULL;
    TRAP_IGNORE( contactTitle = iContactFields->GetContactTitleL() );
    
    if( contactTitle ) 
        {
        aCliText = contactTitle;
        cliType = ECliName;
        }
    else
        {
        // No contact title so try to use phonenumber
        const TPtrC phoneNumber = Number();
        if( phoneNumber.Length() > 0 )
            {
            contactTitle = phoneNumber.Alloc();
            if( contactTitle ) 
                {
                cliType = ECliNumber;
                }
            }
        }
        
    if( cliType == ECliName )
        {
        // Is the contact title as contact title.
        if( CompanyName().Compare( contactTitle->Des() ) == 0 &&
            FirstName().Length() == 0 &&
            LastName().Length() == 0 )
            {
            cliType = ECliCompany;
            }
        }
        
    aCliText = contactTitle;
    return cliType;
    }

// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContact::FirstName() const 
    {
    return iContactFields->FirstName();
    }

// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContact::LastName() const 
    {
    return iContactFields->LastName();
    }

// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContact::CompanyName() const 
    {
    return iContactFields->CompanyName();
    }

// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContact::Number() const
    {
    TPtrC number = iContactFields->Number();
    if( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        if( iMatchedVoipNumber ) 
            {
            number.Set( *iMatchedVoipNumber );
            }
        }
    return number;
    }
    
// ---------------------------------------------------------------------------    
// Finds a phone number for the requested speed dial position.
// ---------------------------------------------------------------------------
//         
TPhCntNumber CPhCntContact::SpeedDialNumber( const TInt aPosition )
	{
	TPhCntNumber contactNumber;
	const RArray<TPhCntNumber>& allNumbers = AllNumbers();		
    const TInt count( allNumbers.Count() );
	for ( TInt numberIndex = 0; numberIndex < count && contactNumber.Type() == MPhCntMatch::ENone; numberIndex++ )
		{
		TPhCntNumber number( allNumbers[numberIndex] );	
		if ( number.Position() == aPosition )
			{						
			contactNumber.Set( number.Number(), number.Type(), number.Position() );		
			}
		}	
	return contactNumber;
	}    

// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContact::PersonalRingingTone() const 
    {
    return iContactFields->PersonalRingingTone();
    }
 
// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
// 
CDesCArray& CPhCntContact::AllDtmfNumbers() const 
    {
    return *iContactFields->AllDtmfNumbers();
    }
    
// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Determines if this contact belongs to some of the groups
// ---------------------------------------------------------------------------
//   
TBool CPhCntContact::BelongsToGroups( 
    const CArrayFix<TContactItemId>& aGroupArray ) const 
    {
    PRINT( "PhCnt: Contact.BelongsToGroups" );
    TBool belongsToGroup = EFalse;
    MVPbkContactLinkArray* groupLinks = NULL;
    TRAP_IGNORE( 
        groupLinks = 
            iContactManager.ConvertContactIdsToLinksL( aGroupArray );
        belongsToGroup = BelongsToGroupsL( groupLinks );
        );
    delete groupLinks;
    return belongsToGroup;
    }

// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
HBufC* CPhCntContact::TextToSpeechTextL() const 
    {
    TPhCntTxtToSpeech txtToSpeech( 
        iContactFields->FirstName(), 
        iContactFields->SecondName(),
        iContactFields->LastName(),
        iContactFields->CompanyName(),
        iContactFields->FirstNamePronunciation(),
        iContactFields->LastNamePronunciation(),
        iContactFields->CompanyNamePronunciation(),
        User::Language() );
    
    HBufC* textToSpeech = NULL;
    txtToSpeech.GetTextToSpeechL( textToSpeech );
    return textToSpeech;
    }

// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContact::CallImage() const
    {
    return iContactFields->CallImage();
    }
    
// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContact::CallText() const
    {
    return iContactFields->CallText();
    }
    
// ---------------------------------------------------------------------------
// From base class MPhCntMatch
// Getter
// ---------------------------------------------------------------------------
//
TBool CPhCntContact::HasThumbnailImage() const
    {
    return iContactFields->HasThumbnail();
    }
    
// ---------------------------------------------------------------------------
// Loads thumbnail of this contact.
// ---------------------------------------------------------------------------
//
MPbk2ImageOperation* CPhCntContact::LoadThumbnailL(
    MPbk2ImageGetObserver& aObserver )
    {
    const MVPbkFieldType* thumbnailFieldType = 
        iContactFields->ThumbnailField();
    
    MPbk2ImageOperation* oper = NULL;
    if( thumbnailFieldType ) 
        {
        oper = iContactManager.RetrieveImageL( 
                    iContactFields->StoreContact(),
                    *thumbnailFieldType,
                    aObserver );    
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    return oper;
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntContact::CPhCntContact( 
    MPhCntContactFields* aContactFields,
    MPhCntContactManager& aContactManager ) :
    iContactFields( aContactFields ),
    iContactManager( aContactManager )
    {
    
    } 

// ---------------------------------------------------------------------------
// Secondphase constructor
// ---------------------------------------------------------------------------
//
void CPhCntContact::ConstructL(  ) 
    {
    iOriginalNumber = KNullDesC().AllocL();
    MVPbkContactLink* contactLink = iContactFields->ContactLink();
    MVPbkContactLink* clonedLink = NULL;
    if( contactLink ) 
        {
        clonedLink = contactLink->CloneLC();
        CleanupStack::Pop(); // clonedLink
        }
    iContactId = CPhCntVPbkContactId::NewL( clonedLink, iContactManager );
    iNumberExtractor = new( ELeave )CCntRawPhoneNumberExtractor();
    iNumberExtractor->ConstructL();
    FeatureManager::InitializeLibL();
    }

// ---------------------------------------------------------------------------
// Checks if this belongs to any of the groups.
// ---------------------------------------------------------------------------
//    
TBool CPhCntContact::BelongsToGroupsL( 
    MVPbkContactLinkArray* aGroups ) const
    {
    TBool belongsToGroups = EFalse;
    if( aGroups )
        {
        MVPbkContactLinkArray* groupsThisContactBelongsTo =
            iContactFields->ContactGroupsLC();

        if( groupsThisContactBelongsTo )
            {
            PRINTF( "PhCnt: Contact.BelongsToGroups.group count.%d", groupsThisContactBelongsTo->Count() );
            for( TInt i = 0; i < aGroups->Count() && !belongsToGroups; i++ )
                {
                const MVPbkContactLink& linkToGroup( aGroups->At( i ) );
                
                // Find returns index if found from array.
                if( groupsThisContactBelongsTo->Find( linkToGroup ) >= 0 )
                    {
                    belongsToGroups = ETrue;
                    }
                }
            CleanupStack::PopAndDestroy(); // groupsThisContactBelongsTo
            }
        }
    return belongsToGroups;
    }
