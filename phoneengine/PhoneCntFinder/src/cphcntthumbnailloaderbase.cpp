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
* Description:  Base class for concrete thumbnailloaders.
*
*/


#include "cphcntthumbnailloaderbase.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntThumbnailLoaderBase::CPhCntThumbnailLoaderBase()
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CPhCntThumbnailLoaderBase::~CPhCntThumbnailLoaderBase()
    {
    }


// ---------------------------------------------------------------------------
// Notifies the observer.
// ---------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderBase::NotifyObserver( 
    CFbsBitmap* aBitmap, 
    TInt aError )
    {
    if( iLoadingObserver ) 
        {
        iLoadingObserver->LoadingCompleted( aBitmap, aError );
        iLoadingObserver = NULL;
        }
    }


// ---------------------------------------------------------------------------
// From class MPhCntThumbnailLoader
// Takes hold of observer and calls template method for actual loading of 
// thumbnail.
// ---------------------------------------------------------------------------
//
void CPhCntThumbnailLoaderBase::LoadL( 
    const CPhCntContactId& aContactId,
    MPhCntLoaderObserver& aLoadingObserver )
    {
    iLoadingObserver = &aLoadingObserver;
    DoLoadL( aContactId );
    }

