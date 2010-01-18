/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Thumbnail loading implementation.
*
*/


// INCLUDE FILES
#include    "CPhCntThumbnailLoaderImpl.h"
#include    "MPhoneCntPbkOwner.h"
#include    "MPhCntThumbnailLoaderObserver.h"
#include    "cphcntcontactid.h"
#include    "cphcntpbkcontactid.h"
#include    "cphcntpbkthumbnailloader.h"

#include    <CPbkContactItem.h>
#include    <CPbkContactEngine.h>
#include    <CPbkThumbnailManager.h>
#include    <fbs.h>

// CONSTANTS

// Granularity of thumbnail array.
const TInt KPhoneThumbnailArrayGranularity = 5;

// Maximum amount of thumbnails.
const TInt KPhoneThumbnailMax = 20;

// Panic literal for thumbnail related panics.
_LIT( KPhCntThumbnailLoaderPanic, "PhCntThumb" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhCntThumbnailLoaderImpl* CPhCntThumbnailLoaderImpl::NewL(
        MPhCntThumbnailLoaderObserver& aObserver,
        MPhoneCntPbkOwner& aPbkOwner,
        MPhCntThumbnailLoader& aLoader )
    {
    CPhCntThumbnailLoaderImpl* self = 
        new ( ELeave ) CPhCntThumbnailLoaderImpl( 
            aObserver,
            aPbkOwner,
            aLoader );

    CleanupStack::PushL( self );
    self->ConstructL( aPbkOwner );
    CleanupStack::Pop();

    return self;
    }
        
// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::~CPhCntThumbnailLoaderImpl
// -----------------------------------------------------------------------------
//
CPhCntThumbnailLoaderImpl::~CPhCntThumbnailLoaderImpl()
    {
    delete iBitmap;
    ClearThumbnailArray();
    iArray.Close();
    delete iLoader;
    delete iBridge;
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::Load
// -----------------------------------------------------------------------------
//
TInt CPhCntThumbnailLoaderImpl::Load( 
        TThumbnailId& aId, 
        const CPhCntContactId& aContactId )
    {
    // If condition doesn't hold, then definitely something is badly
    // wrong.
    if ( iArray.Count() < KPhoneThumbnailMax )
        {
        TRAPD( err, AttemptLoadL( aId, aContactId ) );

        if ( err != KErrNone )
            {
            aId = KPhCntThumbnailNullId;
            }

        return err;
        }
    else
        {
        aId = KPhCntThumbnailNullId;
        return KErrNotFound;
        }
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::Cancel
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::Cancel( 
        TThumbnailId aId )
    {
    TInt index = FindById( aId );

    // If not found, then we can ignore the call.
    if ( index == KErrNotFound )
        {
        return;
        }

    // Remove thumbnail loading information from array.
    RemoveThumbnailArrayEntry( index );
    
    // If we are currently loading this, then cancel loading.
    if ( iCurrent != KPhCntThumbnailNullId && aId == iCurrent )
        {      
        CancelLoad();
        }
    
    if ( iCurrent == KPhCntThumbnailNullId && iArray.Count() )
        {
        Start( ELoadFromContact );
        }
    }
    
void CPhCntThumbnailLoaderImpl::LoadingCompleted(
    CFbsBitmap* aBitmap,
    TInt aResult )
    {
    if( aResult == KErrNone )
        {
        if ( iCurrent != KPhCntThumbnailNullId )
            {

            iBitmap = aBitmap;
            Start( EProcessResult );       
            }
        else
            {
            delete aBitmap;
            }
        }
    else
        {
        if ( iCurrent != KPhCntThumbnailNullId )
            {
            TThumbnailId old = iCurrent;

            Cancel( old );
            Notify( old, aResult, NULL );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::CPhCntThumbnailLoaderImpl
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhCntThumbnailLoaderImpl::CPhCntThumbnailLoaderImpl(
        MPhCntThumbnailLoaderObserver& aObserver,
        MPhoneCntPbkOwner& /*aPbkOwner*/,
        MPhCntThumbnailLoader& aLoader )
    : iObserver( aObserver ),
      iArray( KPhoneThumbnailArrayGranularity ),
      iCurrent( KPhCntThumbnailNullId ),
      iLoader( &aLoader )
    {
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::ConstructL(
    MPhoneCntPbkOwner& /*aPbkOwner*/ )
    {
    iBridge = 
        CIdle::NewL(
            CActive::EPriorityStandard );
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::AttemptLoadL
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::AttemptLoadL( 
        TThumbnailId aId,
        const CPhCntContactId& aContactId )
    {
    // Create new thumbnail information.
    TThumbnailInfo info;
    info.iId = aId;
    info.iContactId = aContactId.CloneL();
    User::LeaveIfError( iArray.Append( info ) );

    // If there were no items, then we can start loading the first image.
    if ( iCurrent == KPhCntThumbnailNullId )
        {
        Start( ELoadFromContact );
        }
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::StartLoadL
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::StartLoadL()
    {
    if ( !iArray.Count() )
        {
        return;
        }

    TThumbnailInfo& info = iArray[ 0 ]; // first
    iCurrent = info.iId;

    // Clear pbk
    ClearPbk( ETrue ); 
    
    iLoader->LoadL( *info.iContactId, *this );
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::CancelLoad
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::CancelLoad()
    {
    iCurrent = KPhCntThumbnailNullId; 
    
    delete iBitmap;
    iBitmap = NULL;

    ClearPbk( iArray.Count() );
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::ProcessResultL
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::ProcessResultL()
    {
    if ( iCurrent == KPhCntThumbnailNullId )
        {
        return;
        }

    if ( !iBitmap )
        {
        User::Leave( KErrNotFound );
        }
    else
        {
        CFbsBitmap* bitmap = iBitmap;
        iBitmap = NULL;
        TThumbnailId old = iCurrent;
        Cancel( iCurrent );
        Notify( old, KErrNone, bitmap );
        }
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::HandleStartL
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::HandleStartL()
    {
    switch ( iState )
        {
        case ELoadFromContact:
            StartLoadL();
            break;

        case EProcessResult:
            ProcessResultL();
            break;

        default:
            Panic( EPanicInvalidState );
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::HandleStart
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::HandleStart()
    {
    TRAPD( err, HandleStartL() );

    if ( err != KErrNone && iCurrent != KPhCntThumbnailNullId )
        {
        TThumbnailId old = iCurrent;
        Cancel( iCurrent );
        Notify( old, err, NULL );
        }
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::Start
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::Start( TState aState )
    {
    iState = aState;
    iBridge->Cancel();
    iBridge->Start( TCallBack( DoStart, this ) );
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::DoStart
// -----------------------------------------------------------------------------
//
TInt CPhCntThumbnailLoaderImpl::DoStart( TAny* aAny )
    {
    CPhCntThumbnailLoaderImpl* loader =
        static_cast< CPhCntThumbnailLoaderImpl* >( aAny );

    loader->HandleStart();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::Notify
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::Notify( 
        TThumbnailId aId,
        TInt aResult, 
        CFbsBitmap* aBitmap )
    {
    iObserver.MpctloThumbnailLoaded( aId, aResult, aBitmap );
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::ClearPbk
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::ClearPbk( TBool aMore )
    {
   
    iLoader->Cancel();

    if ( !aMore )
        {
        iLoader->Release();
        }
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::FindById
// -----------------------------------------------------------------------------
//
TInt CPhCntThumbnailLoaderImpl::FindById( TThumbnailId aId )
    {
    TThumbnailInfo info;
    info.iId = aId;
    // info.iContactId doesn't matter.

    return 
        iArray.Find( 
            info,
            TIdentityRelation<TThumbnailInfo>( CompareIds ) );
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::CompareIds
// -----------------------------------------------------------------------------
//
TBool CPhCntThumbnailLoaderImpl::CompareIds( 
        const TThumbnailInfo& aFirst,
        const TThumbnailInfo& aSecond )
    {
    return aFirst.iId == aSecond.iId;
    }

// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::Panic
// -----------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderImpl::Panic( 
        TPanicReason aReason )
    {
    User::Panic( KPhCntThumbnailLoaderPanic, aReason );
    }
 
// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::ClearThumbnailArray
// -----------------------------------------------------------------------------
//   
void CPhCntThumbnailLoaderImpl::ClearThumbnailArray()
    {
    const TInt count( iArray.Count() );
    for( TInt i = 0; i < count; i++ ) 
        {
        RemoveThumbnailArrayEntry( i );
        }
    }
   
// -----------------------------------------------------------------------------
// CPhCntThumbnailLoaderImpl::ClearThumbnailArrayEntry
// -----------------------------------------------------------------------------
//        
void CPhCntThumbnailLoaderImpl::RemoveThumbnailArrayEntry( TInt aIndex )
    {
    TThumbnailInfo info = iArray[aIndex];
    delete info.iContactId;
    iArray.Remove( aIndex );
    }

//  End of File  
