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
* Description:  Contact manager
*
*/

#include <CVPbkContactManager.h>
#include <CVPbkContactStoreUriArray.h>
#include <TVPbkContactStoreUriPtr.h>
#include <MVPbkContactStoreList.h>
#include <MPbk2ContactNameFormatter.h>
#include <CPbk2SortOrderManager.h>
#include <Pbk2ContactNameFormatterFactory.h>
#include <CVPbkContactIdConverter.h>
#include <MVPbkContactStore.h>
#include <MVPbkContactLink.h>
#include <CVPbkContactLinkArray.h>
#include <MVPbkContactAttributeManager.h>
#include <CVPbkSpeedDialAttribute.h>
#include <CPbk2ImageManager.h>
#include <MVPbkStoreContact.h>
#include <VPbkContactStoreUris.h>
#include <MVPbkFieldType.h>
#include <CVPbkFieldTypeRefsList.h>
#include <VPbkEng.rsg>
#include <MVPbkContactStoreProperties.h>
#include <CVPbkFieldTypeSelector.h>
#include <VPbkContactViewFilterBuilder.h>
#include <talogger.h>

#include "cphcntcontactmatchstrategy.h"
#include "CPhCntContactManager.h"
#include "CPhCntContact.h"
#include "cphcntcontactstoreuris.h"
#include "mphcntstoreloaderobserver.h"

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntContactManager* CPhCntContactManager::NewL()
    {
    CPhCntContactManager* self = new( ELeave )CPhCntContactManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntContactManager::~CPhCntContactManager()
    {
    delete iVoipFieldTypeList;
    delete iImageManager;
    delete iSpeedDialAttribute;
    delete iContactIdConverter;
    delete iContactNameFormatter;
    delete iSortOrder;
    if( iContactManager )
        {
        TRAP_IGNORE( iContactManager->ContactStoresL().CloseAll( *this ) );
        }
    delete iContactManager;
    delete iContactStoreUris;
    }

// ---------------------------------------------------------------------------
// Gives contact manager.
// ---------------------------------------------------------------------------
//
CVPbkContactManager& CPhCntContactManager::ContactManager() const
    {
    return *iContactManager;
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Retrieves contact from contact stores.
// ---------------------------------------------------------------------------
//
MVPbkContactOperationBase* CPhCntContactManager::RetrieveContactL(
    const MVPbkContactLink& aLink,
    MVPbkSingleContactOperationObserver& aObserver)
    {
    return iContactManager->RetrieveContactL( aLink, aObserver );
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Gives contact stores.
// ---------------------------------------------------------------------------
//
MVPbkContactStoreList& CPhCntContactManager::ContactStoresL() const
    {
    return iContactManager->ContactStoresL();
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Creates contact match strategy.
// ---------------------------------------------------------------------------
//
MPhCntContactMatchStrategy* CPhCntContactManager::CreateContactMatchStrategyL(
    MVPbkContactFindObserver& aObserver,
    TBool aRemoveDuplicates )
    {
    return CPhCntContactMatchStrategy::NewL(
        *iContactManager,
        *iContactStoreUris,
        aObserver,
        aRemoveDuplicates ?
        CVPbkPhoneNumberMatchStrategy::EVPbkDuplicatedContactsMatchFlag :
        CVPbkPhoneNumberMatchStrategy::EVPbkMatchFlagsNone );
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Converts contact ids to contact links.
// ---------------------------------------------------------------------------
//
MVPbkContactLinkArray* CPhCntContactManager::ConvertContactIdsToLinksL(
        const CArrayFix<TContactItemId>& aContactId ) const
    {
    CVPbkContactLinkArray* contactLinks = CVPbkContactLinkArray::NewLC();

    const TInt contactIdCount( aContactId.Count() );
    for( TInt i = 0; i < contactIdCount; i++ )
        {
        MVPbkContactLink* link =
            ConvertContactIdToLinkL( aContactId.At( i ) );

        if( link )
            {
            contactLinks->AppendL( link );
            }
        }
    CleanupStack::Pop( contactLinks );
    return contactLinks;
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Converts contact id to contact link.
// ---------------------------------------------------------------------------
//
MVPbkContactLink* CPhCntContactManager::ConvertContactIdToLinkL(
    TContactItemId aContactId ) const
    {
    MVPbkContactLink* link = NULL;
    if( aContactId != KNullContactId )
        {
        link = iContactIdConverter->IdentifierToLinkLC( aContactId );
        CleanupStack::Pop(); // link
        }
    return link;
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Converts contact link to contact id.
// ---------------------------------------------------------------------------
//
TContactItemId CPhCntContactManager::ConvertContactLinkToContactId(
    const MVPbkContactLink& aContactLink ) const
    {
    return iContactIdConverter->LinkToIdentifier( aContactLink );
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Converts contact link descriptor to contact link.
// ---------------------------------------------------------------------------
//
MVPbkContactLink* CPhCntContactManager::ConvertDescriptorToLinkL(
    const TDesC8& aPackedLink ) const
    {
    MVPbkContactLink* link = NULL;
    if( aPackedLink.Length() > 0 )
        {
        MVPbkContactLinkArray* links =
            iContactManager->CreateLinksLC( aPackedLink );
        if( links && links->Count() > 0 )
            {
            link = links->At( 0 ).CloneLC();
            CleanupStack::Pop(); // Link from CloneLC
            }
        CleanupStack::PopAndDestroy(); // Links
        }
    return link;
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Retrieves speed dial contact link.
// ---------------------------------------------------------------------------
//
MVPbkContactOperationBase* CPhCntContactManager::RetrieveSpeedDialContactLinkL(
        const TInt aSpeedDialPosition,
        MVPbkContactFindObserver& aObserver )
    {
    MVPbkContactAttributeManager& attributeManager =
        iContactManager->ContactAttributeManagerL();

    iSpeedDialAttribute->SetIndex( aSpeedDialPosition );


    return attributeManager.ListContactsL(
                *iSpeedDialAttribute,
                aObserver );
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Indicates whether the field had a speed dial attribute set.
// ---------------------------------------------------------------------------
//
TBool CPhCntContactManager::HasSpeedDialL(
		const TInt aSpeedDialPosition,
		const MVPbkStoreContactField& aField )
    {
    TBool result = EFalse;

    MVPbkContactAttributeManager& attributeManager =
        iContactManager->ContactAttributeManagerL();

	CVPbkSpeedDialAttribute* attr = CVPbkSpeedDialAttribute::NewL( aSpeedDialPosition );
    CleanupStack::PushL( attr );

    result = attributeManager.HasFieldAttributeL(
    			*attr,
    			aField );

	CleanupStack::PopAndDestroy( attr );

	return result;
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Retrieves contacts image.
// ---------------------------------------------------------------------------
//
MPbk2ImageOperation* CPhCntContactManager::RetrieveImageL(
    MVPbkStoreContact& aStoreContact,
    const MVPbkFieldType& aFieldType,
    MPbk2ImageGetObserver& aObserver )
    {

    return iImageManager->GetImageAsyncL(
        	NULL,
            aStoreContact,
            aFieldType,
            aObserver );
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Creates field type selector.
// ---------------------------------------------------------------------------
//
CVPbkFieldTypeSelector* CPhCntContactManager::CreateFieldTypeSelectorL() const
    {
    return CVPbkFieldTypeSelector::NewL( iContactManager->FieldTypes() );
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Appends filter to selector.
// ---------------------------------------------------------------------------
//
void CPhCntContactManager::AppendFilterToSelectorL(
    CVPbkFieldTypeSelector& aSelector,
    TVPbkContactViewFilter aFilter )
    {
    VPbkContactViewFilterBuilder::BuildContactViewFilterL(
		aSelector,
		aFilter,
		*iContactManager );
    }


//-----------------------------------------------------------------------------
// From base class MPhCntContactFinder.
// Loads the contact store specified in the given contact link.
//-----------------------------------------------------------------------------
//
void CPhCntContactManager::LoadContactStoreL( const TDesC8& aContactLink,
        MPhCntStoreLoaderObserver& aObserver )
    {
    __ASSERT_ALWAYS( NULL == iStoreLoaderObserver, User::Leave( KErrInUse ) );

    MVPbkContactLink* link = ConvertDescriptorToLinkL( aContactLink );

    // Link conversion has a side effect of loading store plug-in, but
    // make sure that store really gets loaded.
    const TVPbkContactStoreUriPtr uri
        = link->ContactStore().StoreProperties().Uri();
    delete link;
    iContactManager->LoadContactStoreL( uri );

    // Start asynchronous opening of the store.
    MVPbkContactStore* store = iContactManager->ContactStoresL().Find( uri );
    __ASSERT_ALWAYS( NULL != store, User::Leave( KErrNotFound ) );

    store->OpenL( *this );
    iStoreLoaderObserver = &aObserver;
    }

//-----------------------------------------------------------------------------
// From base class MPhCntContactManager.
//-----------------------------------------------------------------------------
//
CPhCntContactStoreUris& CPhCntContactManager::ContactStoreUrisL()
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntContactManager::ContactStoreListL" );
    return *iContactStoreUris;
    }

//-----------------------------------------------------------------------------
// From base class MPhCntContactFinder.
// Constructs fieldtype list, which identifies voip fields and
// performs the find for voip fields.
//-----------------------------------------------------------------------------
//
MVPbkContactOperationBase* CPhCntContactManager::FindVoipContactsL(
    const TDesC& aSipURI,
    MVPbkContactFindObserver& aObserver ) const
    {
    return iContactManager->FindL( aSipURI, *iVoipFieldTypeList, aObserver );
    }

// ---------------------------------------------------------------------------
// From MPhCntContactManager
// Gives contact name formatter.
// ---------------------------------------------------------------------------
//
MPbk2ContactNameFormatter& CPhCntContactManager::ContactNameFormatter()
    {
    return *iContactNameFormatter;
    }

// ---------------------------------------------------------------------------
// From MVPbkContactStoreListObserver
// Indication that contact stores has been opened.
// ---------------------------------------------------------------------------
//
void CPhCntContactManager::OpenComplete()
    {
    if ( iStoreLoaderObserver )
        {      
        iStoreLoaderObserver->ContactStoreLoadingCompleted(
           iContactStore, KErrNone );
        iStoreLoaderObserver = NULL;
        }
    }

// ---------------------------------------------------------------------------
// From MVPbkContactStoreListObserver
// Indication that contact store has been opened.
// ---------------------------------------------------------------------------
//
void CPhCntContactManager::StoreReady(
    MVPbkContactStore& aContactStore )
    {
    
    TVPbkContactStoreUriPtr uri = aContactStore.StoreProperties().Uri();

    if ( !iContactStoreUris->ContactStores().IsIncluded( uri ) )
        {
        TRAP_IGNORE( iContactStoreUris->AddContactStoreL( uri ) );
        }
    
    iContactStore = &aContactStore;
     
    iContactStoreUris->StoreReady( uri );
    }

// ---------------------------------------------------------------------------
// From MVPbkContactStoreListObserver
// Indication that contact store has become unavailable.
// ---------------------------------------------------------------------------
//
void CPhCntContactManager::StoreUnavailable(
    MVPbkContactStore& aContactStore,
    TInt /*aReason*/ )
    {
    TVPbkContactStoreUriPtr uri = aContactStore.StoreProperties().Uri();
    iContactStoreUris->StoreUnavailable( uri );
    }

// ---------------------------------------------------------------------------
// From MVPbkContactStoreListObserver
// Contact store event.
// ---------------------------------------------------------------------------
//
void CPhCntContactManager::HandleStoreEventL(
    MVPbkContactStore& aContactStore,
    TVPbkContactStoreEvent aStoreEvent )
    {
    switch( aStoreEvent.iEventType )
        {
        case TVPbkContactStoreEvent::EStoreRestoreBeginning:
        case TVPbkContactStoreEvent::EStoreBackupBeginning:
            StoreUnavailable( aContactStore, KErrNone );
            break;

        case TVPbkContactStoreEvent::EStoreBackupRestoreCompleted:
            StoreReady( aContactStore );
            break;
        default:
            break;
        }
    }

//-----------------------------------------------------------------------------
// From base class MPhCntContactFinder.
// Loads the contact store specified with URI.
//-----------------------------------------------------------------------------
void CPhCntContactManager::LoadContactStoreWithUriL(
    const TDesC& aStoreUri,
    MPhCntStoreLoaderObserver& aObserver )
    {
    __ASSERT_ALWAYS( NULL == iStoreLoaderObserver, User::Leave( KErrInUse ) );
    iContactManager->LoadContactStoreL( aStoreUri );

    // Start asynchronous opening of the store.
    MVPbkContactStore* store = iContactManager->ContactStoresL().Find( aStoreUri );
    __ASSERT_ALWAYS( NULL != store, User::Leave( KErrNotFound ) );

    store->OpenL( *this );
    iStoreLoaderObserver = &aObserver;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntContactManager::CPhCntContactManager()
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntContactManager::ConstructL()
    {
    iContactStoreUris = CPhCntContactStoreUris::NewL();

    iContactManager =
        CVPbkContactManager::NewL( iContactStoreUris->ContactStores() );

    // Open stores so that they are accesible.
    iContactManager->ContactStoresL().OpenAllL( *this );

    const MVPbkFieldTypeList& fieldTypes = iContactManager->FieldTypes();

    iSortOrder = CPbk2SortOrderManager::NewL( fieldTypes );

    iContactNameFormatter = Pbk2ContactNameFormatterFactory::CreateL(
            KNullDesC, fieldTypes,
            *iSortOrder );
    MVPbkContactStoreList& contactStores = iContactManager->ContactStoresL();
    TVPbkContactStoreUriPtr uri(VPbkContactStoreUris::DefaultCntDbUri() );
    iContactDBStore = contactStores.Find( uri );
    iContactIdConverter = CVPbkContactIdConverter::NewL( *iContactDBStore );
    iSpeedDialAttribute = CVPbkSpeedDialAttribute::NewL();

    iImageManager = CPbk2ImageManager::NewL( *iContactManager );

    iVoipFieldTypeList = CVPbkFieldTypeRefsList::NewL();
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_VOIPGEN ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_VOIPHOME ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_VOIPWORK ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_MOBILEPHONEHOME ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_MOBILEPHONEWORK ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_MOBILEPHONEGEN ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_LANDPHONEHOME ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_LANDPHONEWORK ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_LANDPHONEGEN ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_CARPHONE ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_IMPP ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_SIP ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_PAGERNUMBER ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_FAXNUMBERGEN ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_FAXNUMBERHOME ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_FAXNUMBERWORK ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_VIDEONUMBERHOME ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_VIDEONUMBERWORK ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_VIDEONUMBERGEN ) );
    AddVoipFieldTypeL( fieldTypes.Find( R_VPBK_FIELD_TYPE_ASSTPHONE ) );
    }

// ---------------------------------------------------------------------------
// Adds voip field type to array.
// ---------------------------------------------------------------------------
//
void CPhCntContactManager::AddVoipFieldTypeL(
    const MVPbkFieldType* aVoipFieldType )
    {
    if( aVoipFieldType )
        {
        iVoipFieldTypeList->AppendL( *aVoipFieldType );
        }
    }

// ---------------------------------------------------------------------------
// Return list of additional contact store uris.
// ---------------------------------------------------------------------------
//
const CVPbkContactStoreUriArray& CPhCntContactManager::AdditionalContactStoreUris()
    {
    return iContactStoreUris->AdditionalContactStores();
    }
