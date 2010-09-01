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
* Description:  Classes for service requests made to CPhCntService
*
*/

#include <coehelp.h>
#include <AiwGenericParam.h>
#include <AiwContactAssignDataTypes.h>
#include <AiwGenericParam.hrh>
#include <AiwContactSelectionDataTypes.h>
#include <CVPbkFieldTypeSelector.h>
#include <CVPbkContactStoreUriArray.h>
#include <MVPbkContactLinkArray.h>
#include <RVPbkContactFieldDefaultPriorities.h>
#include <talogger.h>

#include "tphcntservicerequestparams.h"
#include "MPhCntContactManager.h"
#include "cphcntcontactstoreuris.h"
#include "CPhCntSingleItemFetch.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntServiceRequestParams::CPhCntServiceRequestParams(
    TAiwServiceCommands aAiwCommand ) : 
    iAiwCommand( aAiwCommand )
    {
    }
    
CPhCntServiceRequestParams::~CPhCntServiceRequestParams()
    {
    delete iGenericParamList;
    iDefaultPriorities.Close();
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CPhCntServiceRequestParams::BaseConstructL()
    {
    iGenericParamList = CAiwGenericParamList::NewL();
    }
    
// ---------------------------------------------------------------------------
// Gives the command
// ---------------------------------------------------------------------------
//
TAiwServiceCommands CPhCntServiceRequestParams::Command() const
    {
    return iAiwCommand;
    }
    
// ---------------------------------------------------------------------------
// Gives the in param list
// ---------------------------------------------------------------------------
//
const CAiwGenericParamList& CPhCntServiceRequestParams::InParamList() const
    {
    return *iGenericParamList;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntCreateNewContactParams::CPhCntCreateNewContactParams(
    const TDesC& aPhoneNumber ) : 
    CPhCntServiceRequestParams( KAiwCmdAssign ),
    iPhoneNumber( aPhoneNumber )
    {        
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntCreateNewContactParams* CPhCntCreateNewContactParams::NewL(
    const TDesC& aPhoneNumber )
    {
    CPhCntCreateNewContactParams* self = 
        CPhCntCreateNewContactParams::NewLC( aPhoneNumber );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//    
CPhCntCreateNewContactParams* CPhCntCreateNewContactParams::NewLC(
    const TDesC& aPhoneNumber )
    {
    CPhCntCreateNewContactParams* self = 
        new( ELeave )CPhCntCreateNewContactParams( aPhoneNumber );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CPhCntCreateNewContactParams::ConstructL()
    {
    BaseConstructL();
    
    AiwContactAssign::TAiwSingleContactAssignDataV1 data = 
        AiwContactAssign::TAiwSingleContactAssignDataV1();
    
	// By default an existing contact is opened, we'll override this behaviour
	// by setting the ECreateNewContact flag
    data.SetFlags( AiwContactAssign::ECreateNewContact );

    iGenericParamList->AppendL(
        TAiwGenericParam(
            EGenericParamPhoneNumber,
            TAiwVariant( iPhoneNumber ) ) );
            
    iGenericParamList->AppendL( 
        TAiwGenericParam(
            EGenericParamContactAssignData,
            TAiwVariant( 
                AiwContactAssign::TAiwSingleContactAssignDataV1Pckg( data ) ) ) );
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntUpdateExistingContact::CPhCntUpdateExistingContact(
    const TDesC& aPhoneNumber ) : 
    CPhCntServiceRequestParams( KAiwCmdAssign ),
    iPhoneNumber( aPhoneNumber )
    {        
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntUpdateExistingContact* CPhCntUpdateExistingContact::NewL(
    const TDesC& aPhoneNumber )
    {
    CPhCntUpdateExistingContact* self = 
        CPhCntUpdateExistingContact::NewLC( aPhoneNumber );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//    
CPhCntUpdateExistingContact* CPhCntUpdateExistingContact::NewLC(
    const TDesC& aPhoneNumber )
    {
    CPhCntUpdateExistingContact* self = 
        new( ELeave )CPhCntUpdateExistingContact( aPhoneNumber );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CPhCntUpdateExistingContact::ConstructL()
    {
    BaseConstructL();
    
    AiwContactAssign::TAiwSingleContactAssignDataV1 data = 
        AiwContactAssign::TAiwSingleContactAssignDataV1();
        
    iGenericParamList->AppendL(
        TAiwGenericParam(
            EGenericParamPhoneNumber,
            TAiwVariant( iPhoneNumber ) ) );
            
    iGenericParamList->AppendL( 
        TAiwGenericParam(
            EGenericParamContactAssignData,
            TAiwVariant( 
                AiwContactAssign::TAiwSingleContactAssignDataV1Pckg( data ) ) ) );
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntGetUserSelectedPhoneNumberLink*   
    CPhCntGetUserSelectedPhoneNumberLink::NewLC(
        TBool aCallUsedWithLSK, MPhCntContactManager& aContactManager )
    {
    CPhCntGetUserSelectedPhoneNumberLink* self = 
        new( ELeave )CPhCntGetUserSelectedPhoneNumberLink();
    CleanupStack::PushL( self );
    self->ConstructL( aCallUsedWithLSK, aContactManager );
    return self;
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntGetUserSelectedPhoneNumberLink::CPhCntGetUserSelectedPhoneNumberLink() : 
    CPhCntServiceRequestParams( KAiwCmdSelect )
    {
    }
    
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//  
CPhCntGetUserSelectedPhoneNumberLink::~CPhCntGetUserSelectedPhoneNumberLink()
    {
    delete iContactViewFilter;
    }
    

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
// 
void CPhCntGetUserSelectedPhoneNumberLink::ConstructL(
    TBool aCallUsedWithLSK, MPhCntContactManager& aContactManager )
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntGetUserSelectedPhoneNumberLink::ConstructL" );
    BaseConstructL();
    iContactViewFilter = aContactManager.CreateFieldTypeSelectorL();

    aContactManager.AppendFilterToSelectorL( *iContactViewFilter,   
        EVPbkContactViewFilterPhoneNumber );
	
    TAiwSingleItemSelectionDataV3 data = TAiwSingleItemSelectionDataV3();
	  
    if( aCallUsedWithLSK )
        {
        data.SetAddressSelectType( EAiwCallItemSelect );	    
        }
    else
        {
        data.SetAddressSelectType( EAiwPhoneNumberSelect );
        }
	
	data.SetDefaultPriorities( iDefaultPriorities ); 	
    data.SetFetchFilter( iContactViewFilter );
        
    iGenericParamList->AppendL(
        TAiwGenericParam(
            EGenericParamContactSelectionData,
            TAiwVariant( TAiwSingleItemSelectionDataV3Pckg( data ) ) ) );            

	// Pass an array of currently active contact store uris
    CPhCntContactStoreUris& storeUris = aContactManager.ContactStoreUrisL();
	CVPbkContactStoreUriArray* uriArray = storeUris.ActiveContactStoresL(); 
	CleanupStack::PushL( uriArray );
	if ( uriArray->Count() > 0 ) 
		{ 
		HBufC8* packedUris = uriArray->PackLC(); 
		iGenericParamList->AppendL( 
	        TAiwGenericParam( 
            	EGenericParamContactStoreUriArray, 
            	TAiwVariant( *packedUris ) 
            	) 
           	); 
		CleanupStack::PopAndDestroy( packedUris ); 
		}
	CleanupStack::PopAndDestroy( uriArray );
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntGetUserSelectedVoIPAddressLink*   
    CPhCntGetUserSelectedVoIPAddressLink::NewLC(
        MPhCntContactManager& aContactManager,
        TBool aCallUsedWithLSK )
    {
    CPhCntGetUserSelectedVoIPAddressLink* self = 
        new( ELeave )CPhCntGetUserSelectedVoIPAddressLink();
    CleanupStack::PushL( self ); 
    self->ConstructL( aContactManager, aCallUsedWithLSK );
    return self;
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntGetUserSelectedVoIPAddressLink::CPhCntGetUserSelectedVoIPAddressLink() : 
    CPhCntServiceRequestParams( KAiwCmdSelect )
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
// 
void CPhCntGetUserSelectedVoIPAddressLink::ConstructL(
    MPhCntContactManager& aContactManager,
    TBool aCallUsedWithLSK )
    {
    BaseConstructL();

    // Construct empty filter
    iContactViewFilter = aContactManager.CreateFieldTypeSelectorL();

    // Append the filter object with suitable criteria
    aContactManager.AppendFilterToSelectorL( *iContactViewFilter,
        EVPbkContactViewFilterVoIP );
    aContactManager.AppendFilterToSelectorL( *iContactViewFilter,   
        EVPbkContactViewFilterPhoneNumber );
    
    iDefaultPriorities.Append( EVPbkDefaultTypeVoIP ); 
                     
    TAiwSingleItemSelectionDataV3 data = TAiwSingleItemSelectionDataV3();
    
    
    if( aCallUsedWithLSK )
        {
        data.SetCommAddressSelectType( EAiwCommVOIPCall );
        data.SetAddressSelectType( EAiwVoIPItemSelect );	    
        }
    else
        {
        data.SetCommAddressSelectType( EAiwCommVOIPCall );
        }
    
    
    data.SetFetchFilter( iContactViewFilter );
	data.SetDefaultPriorities( iDefaultPriorities ); 	
	
    iGenericParamList->AppendL(
        TAiwGenericParam(
            EGenericParamContactSelectionData,
            TAiwVariant(TAiwSingleItemSelectionDataV3Pckg( data ) ) ) );

	// Pass an array of currently active contact store uris
    CPhCntContactStoreUris* storeUris = CPhCntContactStoreUris::NewL();
	CleanupStack::PushL( storeUris );
	CVPbkContactStoreUriArray* uriArray = storeUris->ActiveContactStoresL();
	CleanupStack::PushL( uriArray );
	if ( uriArray->Count() > 0 ) 
		{ 
		HBufC8* packedUris = uriArray->PackLC(); 
		iGenericParamList->AppendL( 
	        TAiwGenericParam( 
            	EGenericParamContactStoreUriArray, 
            	TAiwVariant(*packedUris) 
            	) 
           	); 
		CleanupStack::PopAndDestroy( packedUris ); 
		}
	CleanupStack::PopAndDestroy( uriArray );
	CleanupStack::PopAndDestroy( storeUris );
    }



// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//    
CPhCntGetUserSelectedDtmfNumberLink* 
    CPhCntGetUserSelectedDtmfNumberLink::NewLC(
        MPhCntContactManager& aContactManager )
    {
    CPhCntGetUserSelectedDtmfNumberLink* self = 
        new( ELeave )CPhCntGetUserSelectedDtmfNumberLink();
    CleanupStack::PushL( self );
    self->ConstructL( aContactManager );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//  
CPhCntGetUserSelectedDtmfNumberLink::~CPhCntGetUserSelectedDtmfNumberLink()
    {
    delete iContactViewFilter;
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntGetUserSelectedDtmfNumberLink::CPhCntGetUserSelectedDtmfNumberLink() : 
    CPhCntServiceRequestParams( KAiwCmdSelect )
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntGetUserSelectedDtmfNumberLink::ConstructL(
    MPhCntContactManager& aContactManager )
    {
    BaseConstructL();

    // Construct empty filter
    iContactViewFilter = aContactManager.CreateFieldTypeSelectorL();

    // Append the filter object with suitable criteria
    aContactManager.AppendFilterToSelectorL( *iContactViewFilter,
        EVPbkContactViewFilterDTMF );
    aContactManager.AppendFilterToSelectorL( *iContactViewFilter,   
        EVPbkContactViewFilterPhoneNumber );
                     
    TAiwSingleItemSelectionDataV3 data;
    data.SetAddressSelectType(EAiwDTMFPhoneNumberSelect);
    data.SetFetchFilter( iContactViewFilter );
	data.SetDefaultPriorities( iDefaultPriorities ); 	
                 
    iGenericParamList->AppendL(
        TAiwGenericParam( 
            EGenericParamContactSelectionData,
            TAiwVariant( TAiwSingleItemSelectionDataV3Pckg( data ) ) ) );
	// Pass an array of currently active contact store uris
    CPhCntContactStoreUris* storeUris = CPhCntContactStoreUris::NewL();
	CleanupStack::PushL( storeUris );				
	CVPbkContactStoreUriArray* uriArray = storeUris->ActiveContactStoresL(); 
	CleanupStack::PushL( uriArray );
	if ( uriArray->Count() > 0 ) 
		{ 
		HBufC8* packedUris = uriArray->PackLC(); 
		iGenericParamList->AppendL( 
	        TAiwGenericParam( 
            	EGenericParamContactStoreUriArray, 
            	TAiwVariant(*packedUris) 
            	) 
           	); 
		CleanupStack::PopAndDestroy( packedUris ); 
		}
	CleanupStack::PopAndDestroy( uriArray );
	CleanupStack::PopAndDestroy( storeUris );
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//    
CPhCntGetPhoneNumberReqParam* 
    CPhCntGetPhoneNumberReqParam::NewL(
    	MPhCntContactManager& aContactManager, 
        MVPbkContactLinkArray& aLinkArray,
        const CPhCntSingleItemFetch::TCallType aCallType )
    {
    CPhCntGetPhoneNumberReqParam* self = 
        new( ELeave )CPhCntGetPhoneNumberReqParam();
    CleanupStack::PushL( self );
    self->ConstructL( aContactManager, aLinkArray, aCallType );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//  
CPhCntGetPhoneNumberReqParam::~CPhCntGetPhoneNumberReqParam()
    {    
    delete iContactViewFilter;
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntGetPhoneNumberReqParam::CPhCntGetPhoneNumberReqParam() : 
    CPhCntServiceRequestParams( KAiwCmdSelect )
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntGetPhoneNumberReqParam::ConstructL(
	MPhCntContactManager& aContactManager,
    MVPbkContactLinkArray& aLinkArray,
    const CPhCntSingleItemFetch::TCallType aCallType )
    {
    BaseConstructL();
                
	// Set filtering
	TVPbkContactViewFilter filter = EVPbkContactViewFilterPhoneNumber;	    		                    		
	TAiwSingleItemSelectionDataV3 data; 
	data.SetAddressSelectType(  EAiwCallItemSelect );
	switch( aCallType )
		{				
		case CPhCntSingleItemFetch::ECallPhoneNumber:			
    		iDefaultPriorities.Append( EVPbkDefaultTypePhoneNumber );    		
    		break;
    	case CPhCntSingleItemFetch::ECallVoip:
    		iDefaultPriorities.Append( EVPbkDefaultTypeVoIP );
    		filter = EVPbkContactViewFilterVoIP;
    		data.SetCommAddressSelectType( EAiwCommVOIPCall );
    		data.SetAddressSelectType(  EAiwVOIPSelect ); 		
    		break;
    	case CPhCntSingleItemFetch::ECallVideoNumber:
    		iDefaultPriorities.Append( EVPbkDefaultTypeVideoNumber );
    		filter = EVPbkContactViewFilterVideoNumber;
    		break;    	    		
    	default:
            break;
		}
		
	// Construct empty filter
    iContactViewFilter = aContactManager.CreateFieldTypeSelectorL();

    // Append the filter object with suitable criteria
    aContactManager.AppendFilterToSelectorL( *iContactViewFilter, filter );   			
                                                 		
	data.SetFetchFilter( iContactViewFilter ); 
	data.SetDefaultPriorities( iDefaultPriorities ); 		
	
    iGenericParamList->AppendL(
        TAiwGenericParam(
            EGenericParamContactSelectionData,
            TAiwVariant( TAiwSingleItemSelectionDataV3Pckg( data ) ) ) );
            
	// Contact link array		
	HBufC8* packedLinks = aLinkArray.PackLC();
	iGenericParamList->AppendL( 
	    TAiwGenericParam( 
	    	EGenericParamContactLinkArray, 
	    	TAiwVariant( *packedLinks ) 
	    	) 
	   	); 
   	CleanupStack::PopAndDestroy( packedLinks );
   			
    }

