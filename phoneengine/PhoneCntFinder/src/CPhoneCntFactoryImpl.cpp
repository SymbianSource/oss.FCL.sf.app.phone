/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Factory class's implementation.
*
*/



// INCLUDE FILES
#include    "CPhoneCntFactoryImpl.h"
#include    "CPhCntThumbnailLoaderImpl.h"
#include    "CPhCntRfsHandlerImpl.h"
#include    "CPhCntMmiSecurityImpl.h"
#include    "CPhoneCntSaveAddToName.h"
#include    "CPhoneCntPbkOwner.h"
#include    "CPhCntMatcherImpl.h"
#include    "CPhCntProfileEngineImpl.h"

#include    <CProfileChangeNotifyHandler.h>
#include    <MProfileChangeObserver.h>

#include    "CPhCntSingleItemFetchService.h"
#include    "cphcntspeeddialimpl.h"
#include    "cphcntvpbkthumbnailloader.h"
#include    "CPhCntContactStores.h"
#include    "cphcntvpbkcontactid.h"
#include    "CPhCntContactManager.h"
#include    "cphcntphonebookservices.h"
#include    "cphcntmatchervoipimpl.h"
#include    "CPhCntMatcherImpl.h"
#include    "CPhCntAIWService.h"
#include    "tphcntservicerequestparamfactory.h"
#include    "cphcntcontactdataselectionimpl.h"
#include    "cphcntstoreloaderimpl.h"

#include    <coemain.h>     // CONE
#include    <secui.h>       // For TSecUi
#include    <featmgr.h>     // Feature Manager
#include    <talogger.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCntFactoryImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhCntFactoryImpl* CPhCntFactoryImpl::NewL()
    {
    CPhCntFactoryImpl* self = new (ELeave) CPhCntFactoryImpl;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::~CPhCntFactoryImpl
// ---------------------------------------------------------------------------
//
CPhCntFactoryImpl::~CPhCntFactoryImpl()
    {
    delete iPhonebookServices;
    delete iContactStores;

    if ( iSecUiInit )
        {
        TSecUi::UnInitializeLib();
        }

    iPbkResource.Close();
    delete iPbkOwner;
    FeatureManager::UnInitializeLib();
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateSpeedDialMonitorL
// ---------------------------------------------------------------------------
//
CPhCntSpeedDialMonitor* CPhCntFactoryImpl::CreateSpeedDialMonitorL()
    {
    CreateContactStoreAndManagerL();
    return CPhCntSpeedDialImpl::NewL( *iPbkOwner );
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateThumbnailLoaderL
// ---------------------------------------------------------------------------
//
CPhCntThumbnailLoader* CPhCntFactoryImpl::CreateThumbnailLoaderL(
        MPhCntThumbnailLoaderObserver& aObserver )
    {
    CreateContactStoreAndManagerL();
    
    CPhCntContactStores* contactStores = 
        CPhCntContactStores::NewL( *iPbkOwner->ContactManager() );
    CleanupStack::PushL( contactStores );
    
    CPhCntVPbkThumbnailLoader* loader = 
        CPhCntVPbkThumbnailLoader::NewL( *contactStores );
    CleanupStack::Pop( contactStores );
    CleanupStack::PushL( loader );
    
    CPhCntThumbnailLoaderImpl* impl =  
        CPhCntThumbnailLoaderImpl::NewL( aObserver, *iPbkOwner, *loader );
    CleanupStack::Pop( loader );
    
    return impl;
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateSingleItemFetchL
// ---------------------------------------------------------------------------
//
CPhCntSingleItemFetch* CPhCntFactoryImpl::CreateSingleItemFetchL()
    {
    CreatePhonebookServicesL();
    return CPhCntSingleItemFetchService::NewL( 
        *iContactStores, *iPbkOwner->ContactManager() );    
    }
// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateContactDataSelectionL
// ---------------------------------------------------------------------------
//       
CPhCntContactDataSelection* CPhCntFactoryImpl::CreateContactDataSelectionL()
    {
    CreatePhonebookServicesL();
    CPhCntAiwService* service = CPhCntAiwService::NewL();
    CleanupStack::PushL( service );
    CPhCntServiceRequestParamFactory* factory = 
        CPhCntServiceRequestParamFactory::NewLC( *iPbkOwner->ContactManager() );
    CPhCntContactStores* contactStores = 
        CPhCntContactStores::NewL( *iPbkOwner->ContactManager() );
    CleanupStack::PushL( contactStores );
    
        
    CPhCntContactDataSelectionImpl* impl = 
        CPhCntContactDataSelectionImpl::NewL(
            service,
            factory,
            contactStores );
    CleanupStack::Pop( 3, service );
    return impl;
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateSaveAddToNameL
// ---------------------------------------------------------------------------
//
CPhoneCntSaveAddToName* CPhCntFactoryImpl::CreateSaveAddToNameL()
    {
    CreatePhonebookServicesL();
    
    CPhoneCntSaveAddToName* phoneSaveAddToName =  
        CPhoneCntSaveAddToName::NewL( *iPhonebookServices );
    return phoneSaveAddToName;
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateRfsHandlerL
// ---------------------------------------------------------------------------
//
CPhCntRfsHandler* CPhCntFactoryImpl::CreateRfsHandlerL()    
    {
    return CPhCntRfsHandlerImpl::NewL();
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateMmiSecurityL
// ---------------------------------------------------------------------------
//
CPhCntMmiSecurity* CPhCntFactoryImpl::CreateMmiSecurityL()
    {
    if ( !iSecUiInit )
        {
        TSecUi::InitializeLibL();
        iSecUiInit = ETrue;
        }
        
    return CPhCntMmiSecurityImpl::NewL();
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateContactMatcherL
// 
//  Create contact matcher
// ---------------------------------------------------------------------------
//
CPhCntMatcher* CPhCntFactoryImpl::CreateContactMatcherL()
    {
    CreateContactStoreAndManagerL();
    CPhCntMatcherImpl* matcher = NULL;
    if( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        matcher = CPhCntMatcherVoIPImpl::NewL( *iPbkOwner );
        }
    else 
        {
        matcher = CPhCntMatcherImpl::NewL( *iPbkOwner );
        }

    return matcher;
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateProfileEngineL
// ---------------------------------------------------------------------------
//
CPhCntProfileEngine* CPhCntFactoryImpl::CreateProfileEngineL()
    {
    return CPhCntProfileEngineImpl::NewL();
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateProfileEngineNotifyHandlerL
// ---------------------------------------------------------------------------
//
CBase* CPhCntFactoryImpl::CreateProfileEngineNotifyHandlerL( 
        MProfileChangeObserver* aObserver )
    {
    return CProfileChangeNotifyHandler::NewL( aObserver );
    }
    
// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateContactIdL
// ---------------------------------------------------------------------------
//    
CPhCntContactId* CPhCntFactoryImpl::CreateContactIdL()
    {
    CreateContactStoreAndManagerL();
    MVPbkContactLink* nullLink = NULL;
    return CPhCntVPbkContactId::NewL( nullLink, *iPbkOwner->ContactManager() );
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateContactIdL
// ---------------------------------------------------------------------------
//    
CPhCntContactId* CPhCntFactoryImpl::CreateContactIdL( 
    TContactItemId aContactId )
    {
    CreateContactStoreAndManagerL();
    return CPhCntVPbkContactId::NewL( aContactId, *iPbkOwner->ContactManager() );
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateContactIdL
// ---------------------------------------------------------------------------
//    
CPhCntContactId* CPhCntFactoryImpl::CreateContactIdL( 
    const TDesC8& aContactId )
    {
    CreateContactStoreAndManagerL();
    return CPhCntVPbkContactId::NewL( aContactId, *iPbkOwner->ContactManager() );
    }

// ---------------------------------------------------------------------------
// CPhCntFactoryImpl::CreateContactStoreLoaderL
// ---------------------------------------------------------------------------
//
MPhCntStoreLoader* CPhCntFactoryImpl::CreateContactStoreLoaderL()
    {
    CreateContactStoreAndManagerL();
    
    return CPhCntStoreLoaderImpl::NewL( *(iPbkOwner->ContactManager() ) );
    }

// -----------------------------------------------------------------------------
// CPhCntFactoryImpl::CPhCntFactoryImpl
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhCntFactoryImpl::CPhCntFactoryImpl()
: iPbkResource( *CCoeEnv::Static() )
    {
    }

// -----------------------------------------------------------------------------
// CPhCntFactoryImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhCntFactoryImpl::ConstructL()
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntFactoryImpl::ConstructL" );
    FeatureManager::InitializeLibL();
    }

// -----------------------------------------------------------------------------
// Creates phonebook services.
// -----------------------------------------------------------------------------
//    
void CPhCntFactoryImpl::CreatePhonebookServicesL()
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntFactoryImpl::CreatePhonebookServicesL" );
    CreateContactStoreAndManagerL();
        
    if( !iPbkResource.IsOpen() )
        {
        iPbkResource.OpenL();  
        iPhonebookServices = 
            CPhCntPhonebookServices::NewL( 
                *iContactStores, 
                *iPbkOwner->ContactManager() );
        }
    }
    
// -----------------------------------------------------------------------------
// Creates phonebook services.
// -----------------------------------------------------------------------------
//   
void CPhCntFactoryImpl::CreateContactStoreAndManagerL()
    {
    if( !iPbkOwner )
        {
        iPbkOwner = CPhoneCntPbkOwner::NewL( );
        CPhCntContactManager* contactManager = iPbkOwner->ContactManager();
        iContactStores = CPhCntContactStores::NewL( *contactManager );
        }
    }

// ================= OTHER EXPORTED FUNCTIONS ==============

// ---------------------------------------------------------------------------
// CreateCntFactoryL
// 
//  Creates instance of contact factory.
// ---------------------------------------------------------------------------
//
EXPORT_C CPhCntFactory* CreateCntFactoryL()
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntFactoryImpl::CreateCntFactoryL" );
    return CPhCntFactoryImpl::NewL();
    }

// End of File
