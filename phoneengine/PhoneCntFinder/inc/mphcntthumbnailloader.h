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
* Description:  Interface for loading thumbnails from contacts.
*
*/


#ifndef MPHCNTTHUMBNAILLOADER_H
#define MPHCNTTHUMBNAILLOADER_H

class CPhCntContactId;
class MPhCntThumbnailLoaderObserver;
class CFbsBitmap;

/**
 * Observer interface for observing thumbnail loading
 * of MPhCntThumbnailLoader.
 * 
 * @lib PhoneCntFinder.
 * @since S60 v3.1
 */
NONSHARABLE_CLASS( MPhCntLoaderObserver )
    {
public:

    /**
     * Indication that loading has been made.
     * 
     * @since S60 v3.0
     * @param Pointer of bitmap loaded, ownership is given to caller.
     * May be null if error occured during loading.
     * @param aError Error code of loading.
     */
    virtual void LoadingCompleted( CFbsBitmap* aBitmap, TInt aError ) = 0;
    };
 
 
/**
 *  Interface for thumbnail loader.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( MPhCntThumbnailLoader )
    {

public:
    
    /**
     * Destructor.
     *
     * @since S60 v3.1
     */
    virtual ~MPhCntThumbnailLoader() {};
    
     /**
      * Starts the loading of thumbnail from contact, which is 
      * identified by contact id.
      *
      * @since S60 v3.1
      * @param aContactId Contact id, which identifies the contact where thumbnail
      * is loaded.
      */
    virtual void LoadL( 
        const CPhCntContactId& aContactId,
        MPhCntLoaderObserver& aLoadingObserver ) = 0;
        
    /**
     * Indicats that loadings of thumbnails is done for now and 
     * resources can be released.
     *
     * @since S60 v3.1
     */
    virtual void Release() = 0;
    
    /**
     * Cancels the current loading.
     *
     * @since S60 v3.1
     */
    virtual void Cancel() = 0;

    };


#endif // MPHCNTTHUMBNAILLOADER_H
