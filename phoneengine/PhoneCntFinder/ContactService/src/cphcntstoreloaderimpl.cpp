/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CPhCntStoreLoaderImpl class.
*
*/


#include <CVPbkContactLinkArray.h>
#include <TVPbkContactStoreUriPtr.h>
#include <MVPbkContactStoreList.h>
#include "cphcntstoreloaderimpl.h"
#include "MPhCntContactManager.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPhCntStoreLoaderImpl::CPhCntStoreLoaderImpl
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CPhCntStoreLoaderImpl::CPhCntStoreLoaderImpl( MPhCntContactManager& aManager )
        :
        iContactManager( aManager )
    {

    }


// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CPhCntStoreLoaderImpl* CPhCntStoreLoaderImpl::NewL( 
        MPhCntContactManager& aManager )
    {
    CPhCntStoreLoaderImpl* self = CPhCntStoreLoaderImpl::NewLC( aManager );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CPhCntStoreLoaderImpl* CPhCntStoreLoaderImpl::NewLC( 
        MPhCntContactManager& aManager )
    {
    CPhCntStoreLoaderImpl* self 
        = new( ELeave ) CPhCntStoreLoaderImpl( aManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CPhCntStoreLoaderImpl::~CPhCntStoreLoaderImpl()
    {
    iObserver = NULL;
    }

// ---------------------------------------------------------------------------
// Secondphase constructor.
// ---------------------------------------------------------------------------
//
void CPhCntStoreLoaderImpl::ConstructL()
    {
    BaseConstructL();
    }
// ---------------------------------------------------------------------------
// From class MPhCntStoreLoader.
// Checks is the specified contact store loaded.
// ---------------------------------------------------------------------------
//
TBool CPhCntStoreLoaderImpl::IsContactStoreLoaded( 
        const TDesC8& aContactLink ) const
    {
    TBool isStoreLoaded( EFalse );
    TRAP_IGNORE( isStoreLoaded = IsContactStoreLoadedL( aContactLink ) );
    
    return isStoreLoaded;
    }


// ---------------------------------------------------------------------------
// From class MPhCntStoreLoader.
// Starts asynchronous loading process for the store specified by the given
// contact link.
// ---------------------------------------------------------------------------
//
void CPhCntStoreLoaderImpl::LoadContactStoreL( const TDesC8& aContactLink,
        MPhCntStoreLoaderObserver& aObserver )
    {
    __ASSERT_ALWAYS( NULL == iObserver, User::Leave( KErrInUse ) );
    
    iObserver = &aObserver;
    iContactManager.LoadContactStoreL( aContactLink, *this );
    }


// ---------------------------------------------------------------------------
// From class MPhCntStoreLoaderObserver.
// Called when contact store loading process is completed. 
// ---------------------------------------------------------------------------
//
void CPhCntStoreLoaderImpl::ContactStoreLoadingCompleted( 
        MVPbkContactStore* aStore, TInt aErrorCode )
    {
    // Loading with contact store uri was completed  
    if ( iStoreUri )
        {
        ResponseReceived( aErrorCode );
        iStoreUri = NULL;
        }
    // Loading with contact link was completed   
    else if ( iObserver )
        {
        iObserver->ContactStoreLoadingCompleted( aStore, aErrorCode );
        iObserver = NULL;
        }
    else
        {
        ASSERT( NULL != iObserver );
        }
    }

// ---------------------------------------------------------------------------
// CPhCntStoreLoaderImpl::LoadContactStoreWithUri.
// Loads contact store (specified by the given URI) synchronously.
// ---------------------------------------------------------------------------
//
TInt CPhCntStoreLoaderImpl::LoadContactStoreWithUri( const TDesC& aStoreUri )
    {      
    TInt err = KErrNone;
    TBool storeLoaded = EFalse;
    TRAP_IGNORE( storeLoaded = IsContactStoreLoadedL( aStoreUri ) );
    if ( !storeLoaded )
        {
        iStoreUri = &aStoreUri;
        // convert Asynch store loading to synch 
        err = MakeAsyncRequest();
        }
    return err;
    }

// ---------------------------------------------------------------------------
// From class CPhCntAsyncToSync
// Issues asynchronous request for contact store loading.
// ---------------------------------------------------------------------------
//
void CPhCntStoreLoaderImpl::DoMakeAsyncRequestL()
    {
    __ASSERT_ALWAYS( NULL != iStoreUri, User::Leave( KErrArgument ) );
    iContactManager.LoadContactStoreWithUriL( *iStoreUri, *this );
    }

// ---------------------------------------------------------------------------
// CPhCntStoreLoaderImpl::IsContactStoreLoadedL.
// Checks is the contact store (specified by the given contact link) loaded.
// ---------------------------------------------------------------------------
//
TBool CPhCntStoreLoaderImpl::IsContactStoreLoadedL( 
        const TDesC8& aContactLink ) const
    {
    CVPbkContactLinkArray* linkArray = CVPbkContactLinkArray::NewLC( 
        aContactLink, iContactManager.ContactStoresL() );
    
    TBool storeLoaded( linkArray->Count() != 0 );
    CleanupStack::PopAndDestroy( linkArray );
    
    return storeLoaded;
    }

// ---------------------------------------------------------------------------
// CPhCntStoreLoaderImpl::IsContactStoreLoadedL.
// Checks is the contact store (specified by the given URI) loaded.
// ---------------------------------------------------------------------------
//
TBool CPhCntStoreLoaderImpl::IsContactStoreLoadedL( 
        const TDesC16& aContactStoreUri ) const
    {
    MVPbkContactStoreList& storeList = iContactManager.ContactStoresL();
    TVPbkContactStoreUriPtr uriPtr( aContactStoreUri );
    MVPbkContactStore* store = storeList.Find( uriPtr );
    return ( NULL != store );
    }

