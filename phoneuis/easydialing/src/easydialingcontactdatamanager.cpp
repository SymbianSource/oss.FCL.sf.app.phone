/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Easy dialing contact data manager.
*
*/

#include <e32std.h>
#include <fbs.h>
#include <CPbk2ImageManager.h>
#include <CVPbkContactManager.h>
#include <MVPbkContactStoreList.h>
#include <MVPbkContactLink.h>
#include <CPbk2StoreConfiguration.h>
#include <CVPbkContactStoreUriArray.h>
#include <VPbkContactStoreUris.h>
#include <TVPbkContactStoreUriPtr.h>
#include <CVPbkTopContactManager.h>
#include <MVPbkContactOperationBase.h>
#include <TVPbkStoreContactAnalyzer.h>
#include <MVPbkFieldType.h>
#include <VPbkEng.rsg>
#include <MVPbkContactViewBase.h>
#include <MVPbkBaseContactFieldCollection.h>
#include <MVPbkContactFieldTextData.h>
#include <MVPbkContactFieldData.h>
#include <CVPbkContactLinkArray.h>
#include <MVPbkContactLink.h>
#include <MVPbkContactOperationBase.h>
#include <MVPbkOperationObserver.h>
#include <MVPbkContactStore.h>
#include <MVPbkContactStoreProperties.h>
#include <MVPbkStoreContact.h>
#include <PbkGlobalSettingFactory.h>
#include <centralrepository.h>

#include "easydialingcontactdata.h"
#include "easydialingcontactdatamanager.h"
#include "easydialingutils.h"
#include "mcontactdatamanagerobserver.h"
#include "easydialingcrkeys.h"
#include "easydialinglogger.h"

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::NewL
// ---------------------------------------------------------------------------
//
CEasyDialingContactDataManager* CEasyDialingContactDataManager::NewL()
    {
    CEasyDialingContactDataManager* self = 
            new ( ELeave ) CEasyDialingContactDataManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::CEasyDialingContactDataManager
// ---------------------------------------------------------------------------
//
CEasyDialingContactDataManager::CEasyDialingContactDataManager()
        : iPbkStoreConfiguration(NULL), 
          iImageOperation(NULL), 
          iContactManager(NULL), 
          iContactOperation(NULL),
          iContactThumbnailSetting(ETrue), 
          iStoreReady(EFalse)
    {
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::~CEasyDialingContactDataManager
// ---------------------------------------------------------------------------
//
CEasyDialingContactDataManager::~CEasyDialingContactDataManager()
    {
    Reset();
    iWaitingContacts.Close();
    iContactDataArray.Close();
    delete iImageManager;
    delete iVPbkTopContactManager;
    delete iImageOperation;
    delete iContactOperation;
    delete iFavsView;

    if ( iContactManager )
        {
        TRAP_IGNORE( iContactManager->ContactStoresL().CloseAll( *this ) );
        }
    delete iContactManager;
    
    if ( iPbkStoreConfiguration )
        {
        iPbkStoreConfiguration->RemoveObserver( *this );
        }
    delete iPbkStoreConfiguration;

    if ( iPbkSettings )
        {
        iPbkSettings->Close();
        }
    delete iPbkSettings;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::ConstructL
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::ConstructL()
    {
    // Create contact store configuration handler
    iPbkStoreConfiguration = CPbk2StoreConfiguration::NewL();
    iPbkStoreConfiguration->AddObserverL( *this );
    
    // Create manager for all the supported contact stores
    CVPbkContactStoreUriArray* supportedStores = 
            iPbkStoreConfiguration->SupportedStoreConfigurationL();
    CleanupStack::PushL( supportedStores );
    iContactManager = CVPbkContactManager::NewL( *supportedStores );
    CleanupStack::PopAndDestroy( supportedStores );
    
    // Open the stores and start observing them
    iContactManager->ContactStoresL().OpenAllL( *this );

    iImageManager = CPbk2ImageManager::NewL(*iContactManager);
    
    iVPbkTopContactManager = CVPbkTopContactManager::NewL( *iContactManager );

    iImageManagerParams.iFlags = TPbk2ImageManagerParams::EScaleImage | TPbk2ImageManagerParams::EKeepAspectRatio;
    iThumbnailFieldType = iContactManager->FieldTypes().Find( R_VPBK_FIELD_TYPE_THUMBNAILPIC );

    // Read easydialing setting from cenrep.
    CRepository* cenrep = CRepository::NewL( KCRUidEasyDialSettings );
    CleanupStack::PushL( cenrep );
    User::LeaveIfError( cenrep->Get( KEasyDialingContactThumbnails, iContactThumbnailSetting ) );
    CleanupStack::PopAndDestroy( cenrep );
    
    // Start observing Phoenbook setting for the name order
    iPbkSettings = PbkGlobalSettingFactory::CreatePersistentSettingL();
    iPbkSettings->ConnectL( MPbkGlobalSetting::EGeneralSettingCategory );
    iPbkSettings->RegisterObserverL( this );

    // Get name order from Phonebook settings
    UpdateNameOrderL();
    
    // Check if store with favourites is configured to be searched.
    SetupFavStoreSearchedL();
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::SetObserver
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::SetObserver(MContactDataManagerObserver* aObserver)
    {
    iObserver = aObserver;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::ContactManager
// ---------------------------------------------------------------------------
//
CVPbkContactManager& CEasyDialingContactDataManager::ContactManager()
    {
    return *iContactManager;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::SetThumbnailSize
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::SetThumbnailSize(const TSize& aSize)
    {
    if (iImageManagerParams.iSize != aSize)
        {
        iImageManagerParams.iSize = aSize;
        Reload();
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::GetThumbnailIdL
// ---------------------------------------------------------------------------
//
HBufC* CEasyDialingContactDataManager::GetThumbnailIdL(
        MVPbkContactLink* aContact, TBool aMatchThumbnails, TBool aFav )
    {
    LOGSTRING("CEasyDialingContactDataManager: GetThumbnailIdL");
    TInt newIndex(KErrNotFound);
    if (aMatchThumbnails)
        {
        newIndex = FindContactIndex( aContact );
        }
    
    LOGSTRING1("newIndex = %d", newIndex);
    if (newIndex == KErrNotFound)
        {
        MVPbkContactLink* newLink = aContact->CloneLC();
        CEasyDialingContactData* newData = new (ELeave) CEasyDialingContactData(newLink);
        CleanupStack::Pop(); // newLink
        newIndex = iContactDataArray.Count();
        CleanupStack::PushL(newData);
        iContactDataArray.AppendL(newData);
        CleanupStack::Pop(newData);
        SetStoreFlagsForContact(newData);
        }
    
    iContactDataArray[ newIndex ]->SetFav( aFav );
    
    HBufC* id = HBufC::NewL( sizeof(TInt) * 2 ); // space for hex representation of TInt
    TPtr ptr = id->Des();
    ptr.Num(newIndex, EHex);
    LOGSTRING1("CEasyDialingContactDataManager: GetThumbnailIdL returns %S", id);
    return id;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::IndexForId 
// ---------------------------------------------------------------------------
//
TInt CEasyDialingContactDataManager::IndexForId( const TDesC& aId ) const
    {
    TUint idVal(0);
    TLex lex(aId);
    lex.Val(idVal, EHex);
    return idVal;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::GetThumbnail 
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactDataManager::GetThumbnail( TInt aIndex, CFbsBitmap*& aThumbnail )
    {
    LOGSTRING("CEasyDialingContactDataManager: GetThumbnail");
    
    TBool retVal;
    
    CEasyDialingContactData* contactData = iContactDataArray[aIndex];
    if ( contactData->IsLoaded() )
        {
        aThumbnail = contactData->Thumbnail();
        retVal = ETrue;
        }
    else
        {
        aThumbnail = NULL;
        retVal = EFalse;
        if (iWaitingContacts.Find(aIndex) == KErrNotFound)
            {
            LOGSTRING1("iWaitingContacts.Append %d", aIndex);
            iWaitingContacts.Append(aIndex);
            TRAPD(err, LoadNextContactDataL());
            if (err)
                {
                HandleError(err);
                }
            }
        }    
    LOGSTRING("CEasyDialingContactDataManager: GetThumbnail Exit");
    return retVal;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::IsFav
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactDataManager::IsFav( TInt aIndex ) const
    {
    CEasyDialingContactData* contactData = iContactDataArray[aIndex];
    return contactData->Fav();
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::IsSimContact
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactDataManager::IsSimContact( TInt aIndex ) const
    {
    CEasyDialingContactData* contactData = iContactDataArray[aIndex];
    return contactData->IsSimContact();
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::IsSdnContact
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactDataManager::IsSdnContact( TInt aIndex ) const
    {
    CEasyDialingContactData* contactData = iContactDataArray[aIndex];
    return contactData->IsSdnContact();
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::IsFavL
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactDataManager::IsFavL( MVPbkContactLink* aLink )
    {
    if ( iFavsView && iFavsViewReady && iFavsView->IndexOfLinkL( *aLink ) > KErrNotFound )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::NumberOfFavsL
// ---------------------------------------------------------------------------
//
TInt CEasyDialingContactDataManager::NumberOfFavsL()
    {
    if ( iFavsView && iFavsViewReady && iFavStoreSearched )
        {
        return iFavsView->ContactCountL();
        }
    else
        {
        return 0;
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::FavLinkLC
// ---------------------------------------------------------------------------
//
MVPbkContactLink* CEasyDialingContactDataManager::FavLinkLC( TInt aIndex )
    {
    if ( !iFavsView || !iFavsViewReady || !iFavStoreSearched )
        {
        // LC function should not return normally unless it has actually
        // put something to cleanup stack
        User::Leave( KErrNotFound );
        }
    return iFavsView->ContactAtL( aIndex ).CreateLinkLC();
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::FavContactStringLC
// ---------------------------------------------------------------------------
//
HBufC* CEasyDialingContactDataManager::FavContactStringLC( TInt aIndex, TNameOrder aNameOrder )
    {
    const MVPbkBaseContactFieldCollection& fields = iFavsView->ContactAtL( aIndex ).Fields();
    TPtrC firstName;
    TPtrC lastName;
    TPtrC companyName;
    for ( TInt i = 0; i < fields.FieldCount(); i++ )
        {
        const MVPbkFieldType* fieldType = fields.FieldAt( i ).MatchFieldType( 0 );
        switch ( fieldType->FieldTypeResId() )
            {
            case R_VPBK_FIELD_TYPE_LASTNAME:
                {
                const MVPbkContactFieldTextData& data = 
                        MVPbkContactFieldTextData::Cast( fields.FieldAt( i ).FieldData() );
                lastName.Set( data.Text() );
                }
                break;
            case R_VPBK_FIELD_TYPE_FIRSTNAME:
                {
                const MVPbkContactFieldTextData& data = 
                        MVPbkContactFieldTextData::Cast( fields.FieldAt( i ).FieldData() );
                firstName.Set( data.Text() );
                }
                break;
            case R_VPBK_FIELD_TYPE_COMPANYNAME:
                {
                const MVPbkContactFieldTextData& data = 
                        MVPbkContactFieldTextData::Cast( fields.FieldAt( i ).FieldData() );
                companyName.Set( data.Text() );
                }
                break;
            default:
                break;
            }
        }
    
    return EasyDialingUtils::CreateContactStringLC( firstName, lastName, companyName, aNameOrder );
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::NameOrder
// ---------------------------------------------------------------------------
//
CEasyDialingContactDataManager::TNameOrder CEasyDialingContactDataManager::NameOrder()
    {
    return iNameOrder;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::GetCurrentStoreUrisL
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::GetCurrentStoreUrisL( RPointerArray<TDesC>& aUris )
    {
    CVPbkContactStoreUriArray* storeUris = iPbkStoreConfiguration->CurrentConfigurationL();
    CleanupStack::PushL( storeUris );
    for ( TInt i = 0 ; i < storeUris->Count() ; ++i )
        {
        HBufC* dbUri = (*storeUris)[i].UriDes().AllocLC();
        aUris.AppendL( dbUri );
        CleanupStack::Pop( dbUri );
        }
    CleanupStack::PopAndDestroy( storeUris );
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::Pause
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::Pause( TBool aPause )
    {
    if ( iPause != aPause )
        {
        iPause = aPause;
        
        if ( !iPause && iWaitingContacts.Count() )
            {
            TRAPD( err, LoadNextContactDataL());
            if ( err )
                {
                HandleError(err);
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::InitReady
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::InitReady()
    {
    // Initialization is ready. we can start loading contact data, if needed.
    // (It is unlikely that there is any data to load at this point)
    iStoreReady = ETrue;
    if ( iWaitingContacts.Count() )
        {
        TRAPD( err, LoadNextContactDataL() )
        if ( err )
            {
            HandleError( err );
            }
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::FindContactIndex
// ---------------------------------------------------------------------------
//
TInt CEasyDialingContactDataManager::FindContactIndex( MVPbkContactLink* aContact )
    {
    TInt index( KErrNotFound );

    for ( TInt i = 0; i < iContactDataArray.Count(); i++ )
        {
        if ( iContactDataArray[i]->ContactLink()->IsSame( *aContact ) )
            {
            index = i;
            break;
            }
        }
    
    return index;
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::LoadNextContactDataL
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::LoadNextContactDataL()
    {
    LOGSTRING("CEasyDialingContactDataManager: LoadNextContactDataL");
    if ( !iImageOperation && !iContactOperation && iWaitingContacts.Count() && iStoreReady && !iPause )
        {
        // first we need to load the contact item
        const TInt index( iWaitingContacts[0] );
        
        if ( index >= 0 && index < iContactDataArray.Count() )
            {
            CEasyDialingContactData* data = iContactDataArray[index];
            iContactOperation = iContactManager->RetrieveContactL( *(data->ContactLink()), *this);
            if (!iContactOperation)
                {
                data->LoadingComplete();
                RDebug::Print(_L("iWaitingContacts.Remove %d"), iWaitingContacts[0]);
                iWaitingContacts.Remove(0);
                LoadNextContactDataL();
                }
            }
        }
    LOGSTRING("CEasyDialingContactDataManager: LoadNextContactDataL Exit");
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::PbkImageGetComplete
// From MPbkImageGetObserver
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::Pbk2ImageGetComplete(MPbk2ImageOperation& aOperation, CFbsBitmap* aBitmap)
    {
    LOGSTRING("CEasyDialingContactDataManager: Pbk2ImageGetComplete");
    TInt index( KErrNotFound );
    if ( iWaitingContacts.Count() > 0 )
        {
        index = iWaitingContacts[0];
        LOGSTRING1("iWaitingContacts.Remove %d", iWaitingContacts[0]);
        iWaitingContacts.Remove(0);
        }
    
    delete &aOperation;
    iImageOperation = NULL;
    delete iStoreContact;
    iStoreContact = NULL;
        
    TRAPD(err, DoHandleImageGetCompleteL(aBitmap, index));
    if (err)
        {
        HandleError(err);
        }
    LOGSTRING("CEasyDialingContactDataManager: Pbk2ImageGetComplete Exit");
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::PbkImageGetFailed
// From MPbkImageGetObserver
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::Pbk2ImageGetFailed(MPbk2ImageOperation& aOperation, TInt /*aError*/)
    {
    LOGSTRING("CEasyDialingContactDataManager: Pbk2ImageGetFailed");
    Pbk2ImageGetComplete(aOperation, NULL);
    }

// -----------------------------------------------------------------------------
// ConfigurationChanged
// From MPbk2StoreConfigurationObserver
// Called when contact store configuration changes
// -----------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::ConfigurationChanged()
    {
    TRAP_IGNORE( SetupFavStoreSearchedL() );
    iObserver->StoreConfigurationChanged();
    }

// -----------------------------------------------------------------------------
// ConfigurationChangedComplete
// From MPbk2StoreConfigurationObserver
// Called when contact store configuration change has been informed to all
// observers
// -----------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::ConfigurationChangedComplete()
    {
    // no implementation needed
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::SettingChangedL
// From MPbkGlobalSettingObserver
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::SettingChangedL( MPbkGlobalSetting::TPbkGlobalSetting aKey )
    {
    if ( aKey == MPbkGlobalSetting::ENameOrdering )
        {
        UpdateNameOrderL();
        if ( iObserver )
            {
            iObserver->NameOrderChanged();
            }
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::UpdateNameOrderL
// Update name order according to Phonebook setting
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::UpdateNameOrderL()
    {
    /*
    * Phonebook name ordering flag, integer value, possible values:
    * 0: name order Lastname Firstname
    * 1: name order Firstname Lastname
    * 2: name order undefined
    */
    TInt nameOrderSetting;
    iPbkSettings->Get( MPbkGlobalSetting::ENameOrdering, nameOrderSetting );
    
    switch ( nameOrderSetting )
        {
        case 0:
            {
            iNameOrder = ELastnameFirstname;
            break;
            }
        case 1:
            {
            iNameOrder = EFirstnameLastname;
            break;
            }
        case 2:
        default:
            {
            // Decide name order based on UI language: lastname-firstname
            // for Chinese, firstname-lastname for the rest of languages.
            TLanguage uiLang = User::Language();
            if ( uiLang == ELangPrcChinese || 
                 uiLang == ELangHongKongChinese ||
                 uiLang == ELangTaiwanChinese )
                {
                iNameOrder = ELastnameFirstname;
                }
            else
                {
                iNameOrder = EFirstnameLastname;
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::DoHandleImageGetCompleteL
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::DoHandleImageGetCompleteL(CFbsBitmap* aBitmap, TInt aIndex)
    {
    LOGSTRING("CEasyDialingContactDataManager: DoHandleImageGetCompleteL");
    if ( aIndex >= 0 && aIndex < iContactDataArray.Count() ) 
        {
        CEasyDialingContactData* data = iContactDataArray[aIndex];
        data->LoadingComplete();
        if (aBitmap)
            {
            data->SetThumbnail(aBitmap);
            }
        }
    LoadNextContactDataL();
    InformObserver();
    LOGSTRING("CEasyDialingContactDataManager: DoHandleImageGetCompleteL Exit");
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::Reset
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::Reset()
    {
    LOGSTRING("CEasyDialingContactDataManager: Reset");
    delete iImageOperation;
    iImageOperation = NULL;
    delete iContactOperation;
    iContactOperation = NULL;
    delete iStoreContact;
    iStoreContact = NULL;
    iWaitingContacts.Reset();
    iContactDataArray.ResetAndDestroy();
    iPause = EFalse;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::VoiceCallAvailable
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactDataManager::VoiceCallAvailable( TInt aIndex )
    {
    LOGSTRING("CEasyDialingContactDataManager: VoiceCallAvailable");

    TBool ret( EFalse );
    
    if( aIndex >= 0 )
        {
        // If the parsed index is valid, return the availability.
        ret = iContactDataArray[ aIndex ]->VoiceCallAvailable();
        }
    
    LOGSTRING1("CEasyDialingContactDataManager: VoiceCallAvailable returns %d", ret);
    return ret;
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::VideoCallAvailable
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactDataManager::VideoCallAvailable( TInt aIndex )
    {
    LOGSTRING("CEasyDialingContactDataManager: VideoCallAvailable");

    TBool ret( EFalse );
    
    if( aIndex >= 0 )
        {
        // If the parsed index is valid, return the availability.
        ret = iContactDataArray[ aIndex ]->VideoCallAvailable();
        }
    
    LOGSTRING1("CEasyDialingContactDataManager: VideoCallAvailable returns %d", ret);
    return ret;
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::UniEditorAvailable
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactDataManager::UniEditorAvailable( TInt aIndex )
    {
    LOGSTRING("CEasyDialingContactDataManager: UniEditorAvailable");

    TBool ret( EFalse );
    
    if( aIndex >= 0 )
        {
        // If the parsed index is valid, return the availability.
        ret = iContactDataArray[ aIndex ]->UniEditorAvailable();
        }
    
    LOGSTRING1("CEasyDialingContactDataManager: UniEditorAvailable returns %d", ret);
    return ret;
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::ContactLinkLC
// ---------------------------------------------------------------------------
//
HBufC8* CEasyDialingContactDataManager::ContactLinkLC( TInt aIndex )
    {
    LOGSTRING( "CEasyDialingContactDataManager: ContactLinkLC" );

    if ( aIndex < 0 || aIndex >= iContactDataArray.Count() )
        {
        User::Leave( KErrArgument );
        }
    
    return iContactDataArray[ aIndex ]->ContactLink()->PackLC();
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::HandleError
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::HandleError(TInt /*aError*/)
    {
    LOGSTRING("CEasyDialingContactDataManager: HandleError");
    // Something went wrong, but there is no need to panic.
    // Cancel async operations and notify observer that we are done.
    iWaitingContacts.Reset();
    delete iImageOperation;
    iImageOperation = NULL;
    delete iContactOperation;
    iContactOperation = NULL;
    
    // Also mark all contact data as loaded. Otherwise it would just be
    // loaded again, which would cause infinite loop if there is a permanent
    // problem.
    for ( TInt i = 0; i < iContactDataArray.Count(); i++ )
        {
        iContactDataArray[i]->LoadingComplete();
        }
    
    if ( iObserver )
        {
        iObserver->AllContactDataLoaded();
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::StoreReady
// from MVPbkContactStoreListObserver
// @param aContactStore The store that is ready.
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::StoreReady( MVPbkContactStore& /*aContactStore*/ )
    {
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::StoreUnavailable
// from MVPbkContactStoreListObserver
// Called when a contact store becomes unavailable.
// @param aContactStore The store that became unavailable.
// @param aReason The reason why the store is unavailable.
//                This is one of the system wide error codes.
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::StoreUnavailable( MVPbkContactStore& /*aContactStore*/,
        TInt /*aReason*/ )
    {
    // Opening some contact database failed. Easydialing can operate without
    // any database connections, but thumbnails and favourite stars can't be 
    // shown, and availability of action menu items can't be checked.
    //   Of course, if easydialing can't open the database, probably PCSServer
    // can't open it either...
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::HandleStoreEventL
// from MVPbkContactStoreListObserver
// Called when changes occur in the contact store.
// IGNORED because ED plugin reacts to contact modifications by listening
// to PCS cache update events.
// @param aContactStore A store whose event it is.
// @param aStoreEvent The event that has occurred.
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::HandleStoreEventL( MVPbkContactStore& /*aContactStore*/,
        TVPbkContactStoreEvent /*aStoreEvent*/ )
    {
    }

// -----------------------------------------------------------------------------
// CEasyDialingPlugin::OpenComplete
// From MVPbkContactStoreListObserver.
// Called when all contact stores are ready to be used, signals
// the next engine state.
//
// -----------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::OpenComplete()
    {
    // next open the favourites view
    TRAPD( err, iVPbkTopContactManager->GetTopContactsViewL( *this, *this ) );
    if ( err )
        {
        HandleError( err );
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::VPbkOperationFailed
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::VPbkOperationFailed(
        MVPbkContactOperationBase* /*aOperation*/,
        TInt /*aError*/ )
    {
    // Loading list of favourite contacts failed.
    // Continue as if none of the contacts are favourited.
    delete iFavsOperation;
    iFavsOperation = NULL;
    delete iFavsView;
    iFavsView = NULL;
    iFavsViewReady = EFalse;

    InitReady();
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::VPbkOperationResultCompleted
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::VPbkOperationResultCompleted(
        MVPbkContactOperationBase* /*aOperation*/,
        MVPbkContactViewBase* aOperationResult )
    {
    delete iFavsOperation;
    iFavsOperation = NULL;
    delete iFavsView;
    iFavsView = aOperationResult;
    iFavsViewReady = ETrue;
    
    // Leave can be safely ignored. Notifications of favourites view changes
    // will not work, but otherwise Easydialing will work correctly.
    TRAP_IGNORE( iFavsView->AddObserverL( *this ) );
    
    InitReady();
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::ContactViewReady
// From MVPbkContactViewObserver.
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::ContactViewReady(
        MVPbkContactViewBase& aView )
    {
    LOGSTRING("CEasyDialingContactDataManager: ContactViewReady");
    
    if ( iFavsView == &aView )
        {
        iFavsViewReady = ETrue;
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::ContactViewUnavailable
// From MVPbkContactViewObserver.
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::ContactViewUnavailable(
        MVPbkContactViewBase& aView )
    {
    LOGSTRING("CEasyDialingContactDataManager: ContactViewUnavailable");
    
    if ( iFavsView == &aView )
        {
        iFavsViewReady = EFalse;
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::ContactAddedToView
// From MVPbkContactViewObserver.
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::ContactAddedToView(
        MVPbkContactViewBase& aView, 
        TInt /*aIndex*/, 
        const MVPbkContactLink& /*aContactLink*/ )
    {
    LOGSTRING("CEasyDialingContactDataManager: ContactAddedToView");
    
    if ( iFavsView == &aView )
        {
        iObserver->FavouritesChanged();
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::ContactRemovedFromView
// From MVPbkContactViewObserver.
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::ContactRemovedFromView(
        MVPbkContactViewBase& aView, 
        TInt /*aIndex*/, 
        const MVPbkContactLink& /*aContactLink*/ )
    {
    LOGSTRING("CEasyDialingContactDataManager: ContactRemovedFromView");
    
    if ( iFavsView == &aView )
        {
        iObserver->FavouritesChanged();
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::ContactViewError
// From MVPbkContactViewObserver.
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::ContactViewError(
        MVPbkContactViewBase& aView, 
        TInt /*aError*/, 
        TBool /*aErrorNotified*/ )
    {
    LOGSTRING("CEasyDialingContactDataManager: ContactViewError");
    
    if ( iFavsView == &aView )
        {
        iFavsViewReady = EFalse;
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::DoHandleContactOperationCompleteL
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::DoHandleContactOperationCompleteL(
        MVPbkStoreContact* aContact, TInt aIndex)
    {
    LOGSTRING("CEasyDialingContactDataManager: DoHandleContactOperationCompleteL");
    if (aContact)
        {
        
        __ASSERT_DEBUG((!iStoreContact && !iImageOperation), User::Panic(_L("CEasyDialingContactDataManager"), 1));
        iStoreContact = aContact;
        
        // Find out the available communication methods for the contact.
        GetAvailableServicesL( aContact, aIndex );
        
        // Next initiate async thumbnail get operation.
        
        if (GetContactThumbnailSetting()) // reads the iContactThumbnailSetting value if it is false we dont fetch images
            {
            if (iImageManager->HasImage(*iStoreContact, *iThumbnailFieldType))
                {
                iImageOperation = iImageManager->GetImageAsyncL(
                        &iImageManagerParams,
                        *iStoreContact,
                        *iThumbnailFieldType,
                        *this);
                }
            }
        if (!iImageOperation)
            {
            CEasyDialingContactData *tn = iContactDataArray[aIndex];
            tn->LoadingComplete();
            LOGSTRING1("iWaitingContacts.Remove %d", iWaitingContacts[0]);
            iWaitingContacts.Remove(0);
            delete iStoreContact;
            iStoreContact = NULL;
            LoadNextContactDataL();
            InformObserver();
            }
        }
    else
        {
        // Opening contact failed. Mark contact data loaded, so it's not opened again.
        CEasyDialingContactData* data = iContactDataArray[aIndex];
        data->LoadingComplete();
        LOGSTRING1("iWaitingContacts.Remove %d", iWaitingContacts[0]);
        iWaitingContacts.Remove(0);
        LoadNextContactDataL();
        InformObserver();
        }
    LOGSTRING("CEasyDialingContactDataManager: DoHandleContactOperationCompleteL Exit");
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::GetAvailableServicesL
//
// Reads from parameter contact which services, i.e. communication methods,
// can be used with this contact. 
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::GetAvailableServicesL( MVPbkStoreContact* aContact, TInt aIndex )
    {
    LOGSTRING1("CEasyDialingContactDataManager: GetAvailableServicesL index = %d", aIndex);
    TVPbkStoreContactAnalyzer analyzer( *iContactManager, aContact );
    
    CEasyDialingContactData* contactData = iContactDataArray[aIndex];
    
    // Voice call is available if either circuit-switched call or voip call is available.
    TBool csdCall = (analyzer.HasFieldL( VPbkFieldTypeSelectorFactory::EVoiceCallSelector ) != KErrNotFound);
    TBool voipCall = (analyzer.HasFieldL( VPbkFieldTypeSelectorFactory::EVOIPCallSelector ) != KErrNotFound);
    
    contactData->SetVoiceCallAvailable( csdCall || voipCall );
    
    contactData->SetVideoCallAvailable(
            analyzer.HasFieldL( VPbkFieldTypeSelectorFactory::EVideoCallSelector ) != KErrNotFound );
    
    contactData->SetUniEditorAvailable(
            analyzer.HasFieldL( VPbkFieldTypeSelectorFactory::EUniEditorSelector ) != KErrNotFound );
    
    LOGSTRING("CEasyDialingContactDataManager: GetAvailableServicesL Exit");
    }


// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::VPbkSingleContactOperationComplete
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::VPbkSingleContactOperationComplete(
        MVPbkContactOperationBase& aOperation,
        MVPbkStoreContact* aContact )
    {
    LOGSTRING("CEasyDialingContactDataManager: VPbkSingleContactOperationComplete");
    delete &aOperation;
    iContactOperation = NULL;
    if ( iWaitingContacts.Count() > 0 )
        {
        TInt index = iWaitingContacts[0];
        LOGSTRING1("VPbkSingleContactOperationComplete, Index=%d", index);
        TRAPD(err, DoHandleContactOperationCompleteL(aContact, index));
        if ( err )
            {
            HandleError(err);
            }
        }
    
    LOGSTRING("CEasyDialingContactDataManager: VPbkSingleContactOperationComplete Exit");
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::VPbkSingleContactOperationFailed
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::VPbkSingleContactOperationFailed(
        MVPbkContactOperationBase& /*aOperation*/, 
        TInt /*aError*/ )
    {
    LOGSTRING("CEasyDialingContactDataManager: VPbkSingleContactOperationFailed");
    delete iContactOperation;
    iContactOperation = NULL;
    TInt index = iWaitingContacts[0];
    LOGSTRING1("VPbkSingleContactOperationFailed, Index=%d", index);
    TRAPD(err, DoHandleContactOperationCompleteL(NULL, index));
    if (err)
        {
        HandleError(err);
        }
    LOGSTRING("CEasyDialingContactDataManager: VPbkSingleContactOperationFailed Exit");
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::InformObserver
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::InformObserver()
    {
    if (iObserver && !iWaitingContacts.Count())
        {
        iObserver->AllContactDataLoaded();
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::SetContactThumbnailSetting
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::SetContactThumbnailSetting( TInt aContactThumbnailSetting )
    {
    iContactThumbnailSetting = aContactThumbnailSetting;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::GetContactThumbnailSetting
// ---------------------------------------------------------------------------
//
TBool CEasyDialingContactDataManager::GetContactThumbnailSetting( )
    {
    return iContactThumbnailSetting;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::Reload
// ---------------------------------------------------------------------------
//
void  CEasyDialingContactDataManager::Reload( )
    {
    LOGSTRING("CEasyDialingContactDataManager: Reload");
    
    for ( TInt i = 0 ; i < iContactDataArray.Count() ; i++ )
        {
        iContactDataArray[ i ]->DeleteThumbnail();
        }
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::SetupFavStoreSearchedL
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::SetupFavStoreSearchedL()
    {
    // Check if favourite contact store (i.e. the default contact store) is one
    // of the stores configured to be searched.
    CVPbkContactStoreUriArray* storeUris = 
            iPbkStoreConfiguration->CurrentConfigurationL();
    iFavStoreSearched = 
            storeUris->IsIncluded( VPbkContactStoreUris::DefaultCntDbUri() );
    delete storeUris;
    }

// ---------------------------------------------------------------------------
// CEasyDialingContactDataManager::SetStoreFlagsForContact
// ---------------------------------------------------------------------------
//
void CEasyDialingContactDataManager::SetStoreFlagsForContact( 
        CEasyDialingContactData* aContactData ) const
    {
    MVPbkContactLink* link = aContactData->ContactLink();
    const TDesC& uri = link->ContactStore().StoreProperties().Uri().UriDes();
    
    TBool isSim = ( uri.Compare( VPbkContactStoreUris::SimGlobalAdnUri() ) == 0 );
    aContactData->SetSimContact( isSim );
    
    TBool isSdn = ( uri.Compare( VPbkContactStoreUris::SimGlobalSdnUri() ) == 0 );
    aContactData->SetSdnContact( isSdn );
    }


//  End of File

