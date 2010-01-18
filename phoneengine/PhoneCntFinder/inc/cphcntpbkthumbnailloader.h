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
* Description:  Loads thumbnail from contact engine.
*
*/


#ifndef CPHCNTPBKTHUMBNAILLOADER_H
#define CPHCNTPBKTHUMBNAILLOADER_H

#include <e32base.h>
#include <MPbkThumbnailOperationObservers.h>

#include "cphcntthumbnailloaderbase.h"

class MPhoneCntPbkOwner;
class CPhCntContactId;
class MPbkThumbnailOperation;
class CPbkContactItem;
class CPbkThumbnailManager;
class MPbkThumbnailGetObserver;


/**
 *  Loads thumbnail from contact engine.
 *
 *  @lib S60 PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntPbkThumbnailLoader ) : 
    public CPhCntThumbnailLoaderBase,
    private MPbkThumbnailGetObserver
    {
public:
    
    /**
     * Static constructor.
     *
     * @since S60 v3.1
     * @param aOwner Owner of phone book.
     */
    static CPhCntPbkThumbnailLoader* NewL( MPhoneCntPbkOwner& aOwner );

    /**
     * Static constructor.
     *
     * @since S60 v3.1
     * @param aOwner Owner of phone book.
     */
    static CPhCntPbkThumbnailLoader* NewLC( MPhoneCntPbkOwner& aOwner );

    /**
     * Destructor.
     *
     * @since S60 v3.1
     */
    virtual ~CPhCntPbkThumbnailLoader();

// From base class CPhCntThumbnailLoaderBase
    
    /**
     * Starts the loading of thumbnail from contact, which is 
     * identified by contact id.
     *
     * @since S60 v3.1
     * @param aContactId Contact id, which identifies the contact where thumbnail
     * is loaded.
     */
     void DoLoadL( const CPhCntContactId& aContactId );
     
// From base class MPhCntThumbnailLoader
        
    /**
     * From MPhCntThumbnailLoader
     * 
     * @since S60 v3.1
     * @see MPhCntThumbnailLoader
     */
    void Release();
    
    /**
     * From MPhCntThumbnailLoader
     * 
     * @since S60 v3.1
     * @see MPhCntThumbnailLoader
     */
    void Cancel();
     
private:

    /**
     * From MPbkThumbnailGetObserver, called when 
     * thumbnail loading is completed.
     *
     * @param aOperation the completed operation.
     * @param aBitmap The loaded bitmap. Callee is responsible of 
     *                the bitmap.
     */
    void PbkThumbnailGetComplete(
        MPbkThumbnailOperation& aOperation, 
        CFbsBitmap* aBitmap );

    /**
     * From MPbkThumbnailGetObserver, called if 
     * the thumbnail loading fails.
     *
     * @param aOperation the failed operation.
     * @param aError error code of the failure.
     */
    void PbkThumbnailGetFailed(
        MPbkThumbnailOperation& aOperation, 
        TInt aError );
   
private:

    CPhCntPbkThumbnailLoader( 
        MPhoneCntPbkOwner& aOwner );

    void ConstructL();
    
    /**
     * Clears all pbk stuff.
     */
    void ClearPbk( TBool aMore = ETrue );

private: // data
    
    /**
     * Owner of contact access.
     * Not own.
     */
    MPhoneCntPbkOwner& iOwner;
   
    /**
     * Thumnail loading operation.
     * own.
     */
    MPbkThumbnailOperation* iPbkGetOp;
    
    /**
     * Contact.
     * Own.
     */
    CPbkContactItem* iPbkItem;
    
    /**
     * Thumbnail manager.
     * Own.
     */
    CPbkThumbnailManager* iPbkManager;
    };


#endif // CPHCNTPBKTHUMBNAILLOADER_H
