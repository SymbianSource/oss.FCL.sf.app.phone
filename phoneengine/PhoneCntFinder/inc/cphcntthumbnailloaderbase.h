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


#ifndef CPHCNTTHUMBNAILLOADERBASE_H
#define CPHCNTTHUMBNAILLOADERBASE_H


#include <e32base.h>
#include "mphcntthumbnailloader.h"

/**
 *  Base class for thumbnail loaders.
 *
 *  Holds the loading observer and offers interface 
 *  for notifying the observer.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntThumbnailLoaderBase ) : 
    public CBase,
    public MPhCntThumbnailLoader
    {
public:

// from base class MPhCntThumbnailLoader

     /**
      * From MPhCntThumbnailLoader.
      *
      * @since S60 v3.1
      * @see MPhCntThumbnailLoader
      */
    void LoadL( 
        const CPhCntContactId& aContactId,
        MPhCntLoaderObserver& aLoadingObserver );

protected:

    virtual ~CPhCntThumbnailLoaderBase();

    CPhCntThumbnailLoaderBase();
    
    /**
     * Notifies the observer.
     */
    void NotifyObserver( CFbsBitmap* aBitmap, TInt aError );
    
private:

    /**
     * Template method, for doing the actual loading in base class.
     */
    virtual void DoLoadL( const CPhCntContactId& aContactId ) = 0;
    
private: // Data

    /** 
     * Observer of loading thumbnail.
     * Not own.
     */
    MPhCntLoaderObserver* iLoadingObserver;

    };

#endif // CPHCNTTHUMBNAILLOADERBASE_H
