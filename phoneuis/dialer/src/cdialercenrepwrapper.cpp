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
* Description:  Wrapper class for Central Repository handling.
*               Using this helper class is most helpful when one class needs
*               to observe state changes in several repositories.
*/

#include <centralrepository.h>
#include "cdialercenrepwrapper.h"

// ---------------------------------------------------------------------------
// CDialerCenRepWrapper::CDialerCenRepWrapper
//
// ---------------------------------------------------------------------------
//
CDialerCenRepWrapper::CDialerCenRepWrapper( 
        MDialerCenRepObserver& aObserver, TUid aRepository ) :
    iObserver( aObserver ), iRepositoryId( aRepository )
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CDialerCenRepWrapper::~CDialerCenRepWrapper
//
// ---------------------------------------------------------------------------
//
CDialerCenRepWrapper::~CDialerCenRepWrapper()
    {
    if ( iCenRepNotifyHandler )
        {
        iCenRepNotifyHandler->StopListening();
        delete iCenRepNotifyHandler;
        }
    delete iRepository;
    }

// ---------------------------------------------------------------------------
// CDialerCenRepWrapper::NewLC
//
// ---------------------------------------------------------------------------
//
CDialerCenRepWrapper* CDialerCenRepWrapper::NewLC( 
        MDialerCenRepObserver& aObserver, TUid aRepository )
    {
    CDialerCenRepWrapper* self = 
            new (ELeave) CDialerCenRepWrapper( aObserver, aRepository );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CDialerCenRepWrapper::NewL
//
// ---------------------------------------------------------------------------
//
CDialerCenRepWrapper* CDialerCenRepWrapper::NewL( 
        MDialerCenRepObserver& aObserver, TUid aRepository )
    {
    CDialerCenRepWrapper* self = 
            CDialerCenRepWrapper::NewLC( aObserver, aRepository );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CDialerCenRepWrapper::ConstructL
//
// ---------------------------------------------------------------------------
//
void CDialerCenRepWrapper::ConstructL()
    {
    iRepository = CRepository::NewL( iRepositoryId );
    iCenRepNotifyHandler = CCenRepNotifyHandler::NewL( *this, *iRepository );
    iCenRepNotifyHandler->StartListeningL();
    }

// ---------------------------------------------------------------------------
// CDialerCenRepWrapper::HandleNotifyGeneric
//
// ---------------------------------------------------------------------------
//
void CDialerCenRepWrapper::HandleNotifyGeneric( TUint32 aId )
    {
    iObserver.HandleRepositoryChange( iRepositoryId, aId );
    }

// ---------------------------------------------------------------------------
// CDialerCenRepWrapper::Get
//
// ---------------------------------------------------------------------------
//
TInt CDialerCenRepWrapper::Get( TUint32 aKey, TInt& aValue )
    {
    return iRepository->Get( aKey, aValue );
    }

// end of file

