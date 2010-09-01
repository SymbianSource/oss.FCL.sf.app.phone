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
* Description:  Thumbnail loader for loading thumbnails contact store.
*
*/


#ifndef CPHCNTVPBKTHUMBNAILLOADER_H
#define CPHCNTVPBKTHUMBNAILLOADER_H

#include <MPbk2ImageOperationObservers.h>

#include "cphcntthumbnailloaderbase.h"
#include "MPhCntContactFetchObserver.h"

class MPhCntContactStores;
class CPhCntContact;
class MVPbk2ImageOperation;

/**
 *  Loader for loading thumbnails from contact store.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntVPbkThumbnailLoader ) : 
    public CPhCntThumbnailLoaderBase,
    private MPhCntContactFetchObserver,
    private MPbk2ImageGetObserver
    {

public:

    /**
     * Static constructor.
     *
     * @since S60 v3.1
     * @param aContactStores Contact stores, where thunbnails are loaded.
     */
    static CPhCntVPbkThumbnailLoader* NewL( 
        MPhCntContactStores& aContactStores );

    /**
     * Static constructor.
     *
     * @since S60 v3.1
     * @param aContactStores Contact stores, where thunbnails are loaded.
     */
    static CPhCntVPbkThumbnailLoader* NewLC(
        MPhCntContactStores& aContactStores );

    /** 
     * Destructor.
     *
     * @since S60 v3.1
     */
    virtual ~CPhCntVPbkThumbnailLoader();
    
// From base class MPhCntThumbnailLoader.

    /**
     * From base class MPhCntThumbnailLoader.
     *
     * @since S60 v3.1
     * @see MPhCntThumbnailLoader
     */
    void Release();
    
    /**
     * From base class MPhCntThumbnailLoader.
     *
     * @since S60 v3.1
     * @see MPhCntThumbnailLoader
     */
    void Cancel();

protected:

// from base class CPhCntThumbnailLoaderBase
    
    /**
     * From CPhCntThumbnailLoaderBase
     *
     * @since S60 v3.1
     * @see CPhCntThumbnailLoaderBase
     */
    void DoLoadL( const CPhCntContactId& aContactId );
    
private:

// From base class MPhCntContactFetchObserver

    /**
     * From MPhCntContactFetchObserver
     *
     * @since S60 v3.1
     * @see MPhCntContactFectchObserver
     */
    void ContactReceived( CPhCntContact* aContact, TInt aError );
    
// from base class MPbk2ImageGetObserver

    /**
     * From base class MPbk2ImageGetObserver
     *
     * @since S60 v3.1
     * @see MPbk2ImageGetObserver
     */
    void Pbk2ImageGetComplete
        (MPbk2ImageOperation& aOperation, CFbsBitmap* aBitmap);

    /**
     * From base class MPbk2ImageGetObserver
     *
     * @since S60 v3.1
     * @see MPbk2ImageGetObserver
     */
    void Pbk2ImageGetFailed
        (MPbk2ImageOperation& aOperation, TInt aError);

    CPhCntVPbkThumbnailLoader( MPhCntContactStores& aContactStores );

    void ConstructL();

private: // data

    /**
     * Contact stores for fetching contact.
     * own.
     */
    MPhCntContactStores* iContactStores;
    
    /**
     * Contact received.
     * Own.
     */
    CPhCntContact* iReceivedContact;
    
    /**
     * Thumbnail loading operation
     * Own.
     */
    MPbk2ImageOperation* iOperation;

    };

#endif // CPHCNTVPBKTHUMBNAILLOADER_H
