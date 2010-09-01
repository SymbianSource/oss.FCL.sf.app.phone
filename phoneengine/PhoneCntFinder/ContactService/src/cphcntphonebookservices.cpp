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
* Description:  Phonebook services wrapper.
*
*/


#include <MVPbkContactLinkArray.h>
#include <MVPbkContactLink.h>
#include "MPhCntContactManager.h"
#include "cphcntphonebookservices.h"
#include "tphcntservicerequestparamfactory.h"
#include "mphcntservicerequestparam.h"
#include "CPhCntService.h"
#include "tphcntcontactlinkresult.h"
#include "cphcntfetchcontact.h"
#include "CPhCntContact.h"
#include "cphcntcontactid.h"
#include "tphcntnullserviceresult.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntPhonebookServices* CPhCntPhonebookServices::NewL(
    MPhCntContactStores& aContactStores,
    MPhCntContactManager& aContactManager )
    {
    CPhCntPhonebookServices* self = 
        new( ELeave ) CPhCntPhonebookServices( aContactManager );
    CleanupStack::PushL( self );
    self->ConstructL( aContactStores );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntPhonebookServices::~CPhCntPhonebookServices()
    {
    delete iService;
    delete iFetchContact;
    delete iParamFactory;
    }

// ---------------------------------------------------------------------------
// Gets user select phonenumber from phonebook.
// ---------------------------------------------------------------------------
//
void CPhCntPhonebookServices::GetUserSelectedPhoneNumberL(
    CPhCntContactId*& aContactId, 
    HBufC*& aPhoneNumber )
    {
    GetUserSelectedNumberL( aContactId, aPhoneNumber, EFalse );
    }

// ---------------------------------------------------------------------------
// Gets user select phonenumber from phonebook.
// ---------------------------------------------------------------------------
//
void CPhCntPhonebookServices::GetUserSelectedVoIPAddressL(
    CPhCntContactId*& aContactId, 
    HBufC*& aVoIPAddress )
    {
    GetUserSelectedAddressL( aContactId, aVoIPAddress );
    }

// ---------------------------------------------------------------------------
// Gets user select dtmf number from phonebook.
// ---------------------------------------------------------------------------
//    
void CPhCntPhonebookServices::GetUserSelectedDtmfNumberL(
    CPhCntContactId*& aContactId,
    HBufC*& aDtmfNumber )
    {
    GetUserSelectedNumberL( aContactId, aDtmfNumber, ETrue );
    }
 
// ---------------------------------------------------------------------------
// Cancels request.
// ---------------------------------------------------------------------------
//
void CPhCntPhonebookServices::CancelRequest()
    {
    delete iService;
    iService = NULL;
    }

// ---------------------------------------------------------------------------
// Creates CPhCntService if needed.
// ---------------------------------------------------------------------------
//
void CPhCntPhonebookServices::CreateCntServiceL()
    {
    if ( !iService )
        {
        iService = CPhCntService::NewL();
        }
    }
    
// ---------------------------------------------------------------------------
// Creates new contact to phonebook.
// ---------------------------------------------------------------------------
//    
void CPhCntPhonebookServices::CreateNewContactL( 
    const TDesC& aPhoneNumber )
    {
    CreateCntServiceL();
   
    MPhCntServiceRequestParam* param = 
        iParamFactory->CreateCreateNewContactRequestParamLC( aPhoneNumber );
        
    TPhCntNullServiceResult noResult;
    iService->ExecuteRequestL( *param, noResult );
    
    CleanupStack::PopAndDestroy( 1 ); // param
    }
    
// ---------------------------------------------------------------------------
// Updates existing contact from phonebook.
// ---------------------------------------------------------------------------
//    
void CPhCntPhonebookServices::UpdateExistingContactL( 
    const TDesC& aPhoneNumber )
    {
    CreateCntServiceL();
    MPhCntServiceRequestParam* param = 
        iParamFactory->CreateUpdateContactRequestParamLC( aPhoneNumber );
        
    TPhCntNullServiceResult noResult;
    iService->ExecuteRequestL( *param, noResult );
    
    CleanupStack::PopAndDestroy( 1 ); // param
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntPhonebookServices::CPhCntPhonebookServices(
    MPhCntContactManager& aContactManager ) : 
    iContactManager( aContactManager )
    {
    }


// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntPhonebookServices::ConstructL(
    MPhCntContactStores& aContactStores )
    {
    iFetchContact = CPhCntFetchContact::NewL( aContactStores );
    CreateCntServiceL();
    iParamFactory = CPhCntServiceRequestParamFactory::NewL( iContactManager );
    }

// ---------------------------------------------------------------------------
// Gets contact links
// ---------------------------------------------------------------------------
//    
HBufC8* CPhCntPhonebookServices::GetContactLinksLC(
    MPhCntServiceRequestParam& aParam )
    {
    CreateCntServiceL();
    HBufC8* linkToContact = NULL;
    TPhCntContactLinkResult result( linkToContact );
    iService->ExecuteRequestL( aParam, result );
    CleanupStack::PushL( linkToContact );
    return linkToContact;
    }

// ---------------------------------------------------------------------------
// Gets contact links and fetches the contact pointed by link
// ---------------------------------------------------------------------------
// 
CPhCntContact* CPhCntPhonebookServices::GetUserSelectedContactLC( 
    MPhCntServiceRequestParam& aParam )
    {
    CPhCntContact* contact = NULL;
    HBufC8* linkToContact = GetContactLinksLC( aParam );
    if( linkToContact && linkToContact->Length() > 0 ) 
        {
        User::LeaveIfError( 
            iFetchContact->FetchContact( *linkToContact, contact ) );
        }
    else 
        {
        User::Leave( KErrNotFound );
        }
    CleanupStack::PopAndDestroy( linkToContact );
    CleanupStack::PushL( contact );
    return contact;
    }

// ---------------------------------------------------------------------------
// Sets result to client.
// ---------------------------------------------------------------------------
// 
void CPhCntPhonebookServices::SetResultsL( 
    const TDesC& aReceivedNumber, 
    const CPhCntContactId* const aReceivedContactId,
    HBufC*& aClientResultNumber,
    CPhCntContactId*& aClientResultContactId ) const
    {
    if( aReceivedNumber.Length() > 0 && aReceivedContactId ) 
        {
        HBufC* number = aReceivedNumber.AllocLC();
        aClientResultContactId = aReceivedContactId->CloneL();
        aClientResultNumber = number; 
        CleanupStack::Pop( number ); 
        }
    else 
        {
        User::Leave( KErrNotFound );
        }    
    }

// ---------------------------------------------------------------------------
// Gets user selected number from phonebook.
// ---------------------------------------------------------------------------
// 
void CPhCntPhonebookServices::GetUserSelectedNumberL(
    CPhCntContactId*& aContactId,
    HBufC*& aNumber, TBool aDTMFWanted )
    {
   
    MPhCntServiceRequestParam* param = NULL;
    if( aDTMFWanted )
        {
        param = 
            iParamFactory->CreateGetUserSelectedDtmfNumberLinkLC( 
                iContactManager );
        }
    else
        {
        param = iParamFactory->CreateGetUserSelectedPhoneNumberLinkParamLC();
        }
        
    CPhCntContact* contact = GetUserSelectedContactLC( *param );
        
    if( contact )
        {
        TPtrC number;
        if( aDTMFWanted )
            {
            number.Set( contact->Dtmf() );        
            }
        // If dtmf not valid, then use phonenumber        
        if( number.Length() == 0 )
            {
            number.Set( contact->Number() );
            }
        SetResultsL( 
            number, 
            contact->ContactId(), 
            aNumber, 
            aContactId );
        CleanupStack::PopAndDestroy( contact );
        }
    else 
        {
        User::Leave( KErrNotFound );
        }
    CleanupStack::PopAndDestroy( 1 );  // param
    }

// ---------------------------------------------------------------------------
// Gets user selected VoIP Address from phonebook.
// ---------------------------------------------------------------------------
// 
void CPhCntPhonebookServices::GetUserSelectedAddressL(
    CPhCntContactId*& aContactId,
    HBufC*& aVoIPAddress )
    {  
    MPhCntServiceRequestParam* param = NULL;
    param = iParamFactory->CreateGetUserSelectedVoIPAddressLinkParamLC(
                iContactManager );
 
    CPhCntContact* contact = GetUserSelectedContactLC( *param );
        
    if( contact )
        {
        TPtrC number;

        if( number.Length() == 0 )
            {
            number.Set( contact->Number() );
            }
        
        SetResultsL( 
            number, 
            contact->ContactId(), 
            aVoIPAddress, 
            aContactId );
        CleanupStack::PopAndDestroy( contact );
        }
    else 
        {
        User::Leave( KErrNotFound );
        }
    CleanupStack::PopAndDestroy( 1 ); // contact
    }

// ---------------------------------------------------------------------------
// Gets a phone number for a contact link.
// ---------------------------------------------------------------------------
// 
void CPhCntPhonebookServices::GetPhoneNumberL(
    const TDesC8& aContactLink, 
    const CPhCntSingleItemFetch::TCallType aCallType,
    HBufC*& aNumber,
    HBufC8*& aFieldLink )    
	{			
	// Setup AIW parameters	for a service request		
	MPhCntServiceRequestParam* param = 
	    iParamFactory->CreateGetPhoneNumberFromContactParamLC( 
	        aContactLink, aCallType );
	// Execute the service request
	CPhCntContact* contact = GetUserSelectedContactLC( *param );	
	
	// If a contact was found
	if( contact )
	    {
	    if( contact->Number().Length() > 0 &&
	        contact->ContactLink() )
            {        
            aNumber = contact->Number().AllocLC();
            aFieldLink = contact->ContactLink()->PackLC();
            CleanupStack::Pop( 2, aNumber ); // aNumber & aFieldLink
            }
        CleanupStack::PopAndDestroy( contact );
	    }
    else 
        {
        User::Leave( KErrNotFound );
        }			
	CleanupStack::PopAndDestroy( 1 ); // param.	
	}
