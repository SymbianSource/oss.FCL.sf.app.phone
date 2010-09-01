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
* Description:  Manager for contacts.
*
*/


#ifndef CPECONTACTMATCH_H
#define CPECONTACTMATCH_H

// INCLUDES
#include <cntdef.h>
#include <e32base.h>
#include <pevirtualengine.h>
#include <mphcntthumbnailloaderobserver.h>

// CONSTANTS
// Number of languages that use lastname-firstname order
const TInt KPENumberOfSwappedLanguages = 7;

// Languages which use lastname-firstname order
const TLanguage KPESwappedLanguages[ KPENumberOfSwappedLanguages ] = 
    {
    ELangHungarian,       // 17
    ELangTaiwanChinese,   // 29
    ELangHongKongChinese, // 30
    ELangPrcChinese,      // 31
    ELangJapanese,		  // 32
    ELangKorean,          // 65
    ELangVietnamese       // 96
    };

// FORWARD DECLARATIONS
class CPEContactHandling;
class CPhCntContactId;
class CPhCntFactory;
class CPhCntSingleItemFetch;
class CPhCntSpeedDialMonitor;
class CPhCntThumbnailLoader;
class MPEDataStore;
class MPhCntMatch;
class CPhCntMatcher;
class CPhCntThumbnailLoader;
class MPESimContactMatch;
class CTelMatchingExtension;
class CPEContactExtensionWrapper;

// CLASS DECLARATION

/**
*  Container for all Contact Handling owned instances of Phone Contact Finder
*/
class CPEContactMatch
    : public CBase,
      public MPhCntThumbnailLoaderObserver
    {
    public:  // Constructors and destructor      

        /**
        * Two-phased constructor.
        * @param aOwner, handle to contact handling interface / owner of this class
        * @param aDataStore, handle to internal engine info interface
        * @return an instance of class CPEContactMatch
        */
        IMPORT_C static CPEContactMatch* NewL(
            CPEContactHandling& aOwner,
            MPEDataStore& aDataStore );
        
        /**
        * Destructor.
        */
        ~CPEContactMatch();

    public: // New functions

        /**
        * Synchronous method for finding contact data specified with aContactId
        * @param aCallId, the call this contact is associated with
        * @return None
        */
        void MatchWithContactIdL( const TInt aCallId ); 

        /**
        * Search contact information synchronously matching with aPhoneNumber
        * @param aCallId, the call this contact is associated with
        * @return None (leaves with KErrNotFound if no match found or db in use)
        */
        void MatchWithNumberL( const TInt aCallId );

        /**
        * Returns contact picture data and call id association
        * NOTE! Ownership of the picture is transfered to caller.
        * @return contact thumbnail, equals NULL if not found
        */
        CFbsBitmap* ContactThumbnail();

        /**
        * Fetch number from speed dial location
        * @param aLocationIndex Index to look for
        * @param aNumber Filled with speed dial location phone number
        */
        void GetSpeedDialLocationL( 
            TInt aLocationIndex, 
            TPEPhoneNumber& aNumber );

    private:

        /**
        * C++ constructor.
        */
        CPEContactMatch(
            CPEContactHandling& aOwner,
            MPEDataStore& aDataStore );
        
        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        /**
        * Create thumbnail loader.
        */
        void ConstructThumbnailLoaderL();

        /**
        * Create speed dial command
        */
        void ConstructSpeedDialCommandL();

        /**
        * Copies contact information to remote info struct
        * @param aMatch a contact item 
        * @param aCallId, the call this contact is associated with
        */
        void CopyContactFieldsDataL(
            const MPhCntMatch& aMatch,
            const TInt aCallId );

        /**
        * Creates single item fetch.
        *
        * Ownership is passed to the client.
        *
        * @return instance of single item fetch.
        */
        CPhCntSingleItemFetch* CreateSingleItemFetchL();

        /**
         * Creates contact id.
         */
        CPhCntContactId* CreateContactIdL( const TDesC8& aContactId );

        /**
         * Creates contact id.
         */
        CPhCntContactId* CreateContactIdL( TContactItemId aContactId );

        /**
        * Starts loading thumbnail image
        * @param aContactId contact item with the desired thumbnail
        * @param aCallId call id for identifying which thumbnail is loaded
        * @return Symbian OS error code
        */
        TInt LoadThumbnail( const CPhCntContactId& aContactId, const TInt aCallId );

        /**
        * From MPhCntThumbnailLoaderObserver
        *
        * Called when thumbnail loading has been completed.
        * Ownership of the bitmap is transferred to the observer.
        *
        * @param aId id, never KPhCntThumbnailNullId.
        * @param aResult error code.
        * @param aBitmap bitmap.
        */
        void MpctloThumbnailLoaded(
            CPhCntThumbnailLoader::TThumbnailId aId,
            TInt aResult, 
            CFbsBitmap* aBitmap );

        /**
        * Resolves services implementation uid for matching (ECom plugin).
        * @leave Leaves with KErrNotFound if UID not found.
        * @param aServiceId Unique service id used in current call.
        * @return Implementation UID
        */       
        TUid ResolveServicesImplementationUidL( TUint32 aServiceId );
 
        /**
        * Makes voip contact matching.
        * @param aCallId Id of the currently used call
        * @param aMatch Contact match
        * @param aRemotePartyName Remote party name.
        */  
        void MatchWithVoipAdressL( 
                const TInt aCallId, 
                MPhCntMatch*& aMatch, 
                RBuf& aRemotePartyName );
        
        /**
        * Returns contact store uris that should be opened
        * before contact match. Store uris are fetched by service id.
        * @param aServiceId Service id of the ongoing call.
        * @return Array of uri's. NULL if not found for current service.
        */
        CDesCArray* StoreUrisForServiceL( TUint aServiceId );
        
        /**
        * Makes voip contact matching using extension plugin.
        * @since    S60 v5.1
        * @param    aCallId            Identifier of the currently used call.
        * @param    aMatch             Contact match.
        * @param    aRemotePartyName   Remote party name.
        */  
        void MatchWithVoipAddressUsingExtensionL( TInt aCallId, 
            MPhCntMatch*& aMatch, RBuf& aRemotePartyName );
        
        /**
         * Returns matching extension plugin for the given service. A new 
         * plugin is created if extension is not yet created for the service.
         * @since   S60 v5.1
         * @param   aServiceId  Service identifier.
         * @return  A matching extension instance.
         */
        CTelMatchingExtension& MatchingExtensionL( TUint aServiceId );
        
    private:    // Data

        // Handle to contact handling interface / owner of this class
        CPEContactHandling& iOwner;

        // Handle to internal engine info interface
        MPEDataStore& iDataStore;

        // Owned: library
        RLibrary iLibrary;

        // Owned: contact factory
        CPhCntFactory* iContactFactory;

        // Owned: contact matcher
        CPhCntMatcher* iContactMatcher;

        // Owned: sim contact matcher
        MPESimContactMatch* iSimContactMatcher;

        // Owned: thumbnail loader
        CPhCntThumbnailLoader* iContactThumbnailLoader;

        // Owned: speed dial monitor
        CPhCntSpeedDialMonitor* iSpeedDialCommand;

        // Owned: thumbnail image
        // Ownership is passed to client when it calls ContactThumbnail()
        CFbsBitmap* iThumbnailImage;

        // Identifier of the currently loading thumbnail
        // Used *exlusively* by this class to associate a thumbnail to a contact
        // This class uses a call id (TInt) for this purpose
        CPhCntThumbnailLoader::TThumbnailId iThumbnailId;
        
        /**
         * Contact extension plugins.
         * Own.
         */
        RPointerArray<CPEContactExtensionWrapper> iPlugins;
    };

#endif // CPECONTACTMATCH_H

// End of File
