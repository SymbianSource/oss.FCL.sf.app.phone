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
* Description:  Concrete interface for loading thumbail images.
*
*/


#ifndef CPHCNTTHUMBNAILLOADERIMPL_H
#define CPHCNTTHUMBNAILLOADERIMPL_H

// INCLUDES
#include    "CPhCntThumbnailLoader.h"
#include    "mphcntthumbnailloader.h"

// FORWARD DECLARATIONS
class MPhoneCntPbkOwner;
class MPhCntThumbnailLoaderObserver;
class CPbkContactItem;
class CPbkThumbnailManager;
class MPbkThumbnailOperation;
class CPhCntContactId;

// CLASS DECLARATION

/**
*  It loads thumbail images.
*
*  Note that it is considered that loading of thumbnail images is not critical.
*  Thus, if something fails during loading procedure, then we ignore the whole 
*  thumbnail.
*
*  However, observer will be informed of both success and failure.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCntThumbnailLoaderImpl )
    : public CPhCntThumbnailLoader,
      private MPhCntLoaderObserver
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhCntThumbnailLoaderImpl* NewL(
            MPhCntThumbnailLoaderObserver& aObserver,
            MPhoneCntPbkOwner& aPbkOwner,
            MPhCntThumbnailLoader& aLoader );
        
        /**
        * Destructor.
        */
        ~CPhCntThumbnailLoaderImpl();

    public: // Functions from base classes

        /**
        * From CPhCntThumbnailLoader
        */
        TInt Load( TThumbnailId& aId, const CPhCntContactId& aContactId );
        
        /**
        * From CPhCntThumbnailLoader
        */
        void Cancel( TThumbnailId aId );
        
        /**
         * Indication that loading has been made.
         * 
         * @since S60 v3.0
         * @param Pointer of bitmap loaded, ownership is given to caller.
         * May be null if error occured during loading.
         * @param aError Error code of loading.
         */
        void LoadingCompleted( CFbsBitmap* aBitmap, TInt aError );
       
    private:

        /**
        * It contains necessary information to load a thumbnail.
        */
        struct TThumbnailInfo
            {
            // It contains index.
            TThumbnailId iId;

            // Identifies the contact, which thumbnail is going to be loaded.
            CPhCntContactId* iContactId;
            };

        /**
        * It enumerates all states.
        */
        enum TState
            {
            ELoadFromContact,
            EProcessResult
            };

        /**
        * C++ constructor.
        */
        CPhCntThumbnailLoaderImpl(
            MPhCntThumbnailLoaderObserver& aObserver,
            MPhoneCntPbkOwner& aPbkOwner,
            MPhCntThumbnailLoader& aLoader );

        /**
        * Symbian OS constructor.
        */
        void ConstructL( MPhoneCntPbkOwner& aPbkOwner );

        /**
        * Attempts to start loading of thumbnail.
        */
        void AttemptLoadL( 
            TThumbnailId aId, 
            const CPhCntContactId& aContactId );

        /**
        * Starts loading next thumbnail.
        */
        void StartLoadL();

        /**
        * Cancels loading.
        */
        void CancelLoad();

        /**
        * Processes the result.
        */
        void ProcessResultL();

        /**
        * Finds position in array by thumbnail id.
        *
        * @param aId id.
        * @return position or KErrNotFound if not found.
        */
        TInt FindById( TThumbnailId aId );

        /**
        * Starts loading. If it failed, then simply inform
        * observer about failure.
        */
        void DoStartLoad();

        /**
        * Handles start.
        */
        void HandleStartL();
        
        /**
        * Handles start; calls HandleStartL in trap harnness.
        */
        void HandleStart();

        /**
        * Starts bridge.
        */ 
        void Start( TState aState );

        /**
        * Idle callback.
        */
        static TInt DoStart( TAny* aAny );
        
        /**
        * It gives thumbnail to the observer.
        *
        * Ownership of the bitmap is transferred to the client.
        *
        * @param aResult error code.
        * @param aBitmap bitmap or NULL if no bitmap loaded.
        */
        void Notify( 
            TThumbnailId aId, 
            TInt aResult, 
            CFbsBitmap* aBitmap );

        /**
        * Clears all pbk stuff.
        */
        void ClearPbk( TBool aMore = ETrue );

        /**
        * Compares ids of two structure.
        *
        * @return ETrue iff aFirst.iId == aSecond.iId.
        */
        static TBool CompareIds( 
            const TThumbnailInfo& aFirst,
            const TThumbnailInfo& aSecond );

        /**
        * It is enumeration of all panic reasons.
        */
        enum TPanicReason
            {
            EPanicInvariant,
            EPanicInvalidState
            };

        /**
        * Panics.
        *
        * @param aReason reason for panic.
        */
        static void Panic( TPanicReason aReason );
        
        void ClearThumbnailArray();
        
        void RemoveThumbnailArrayEntry( TInt aIndex );
        
    private:    // Data
        
        /**
        * It is type for array of thumbnail info.
        */
        typedef RArray< TThumbnailInfo > RThumbnailArray;

        // Ref: Observer.
        MPhCntThumbnailLoaderObserver& iObserver;       

        // Owned: array of thumbnails to load.
        RThumbnailArray iArray;

        // Owned: active bridge
        CIdle* iBridge;

        // INFORMATION OF LOADING..
        
        // Current state.
        TState iState;

        // Currently loading. System is loading iff iCurrent != KNullId.
        TThumbnailId iCurrent;                

        // Current bitmap.
        CFbsBitmap* iBitmap;
        
        /**
         * Thumbnail loader.
         * Own.
         */
        MPhCntThumbnailLoader* iLoader;

    };

#endif      // CPHCNTTHUMBNAILLOADERIMPL_H
            
// End of File
