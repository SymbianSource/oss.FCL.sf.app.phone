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
* Description:  Easy dialing contact data manager.
*
*/


#ifndef __EASYDIALINGCONTACTDATAMANAGER_H__
#define __EASYDIALINGCONTACTDATAMANAGER_H__

#include <MPbk2ImageOperationObservers.h>
#include <MPbk2StoreConfigurationObserver.h>
#include <MVPbkSingleContactOperationObserver.h>
#include <MVPbkContactStoreListObserver.h>
#include <MVPbkOperationObserver.h>
#include <MVPbkContactViewBase.h>
#include <TPbk2ImageManagerParams.h>
#include <MPbkGlobalSetting.h> // For name order setting
#include <MVPbkContactViewObserver.h>
#include "easydialingcontactdata.h"

// FORWARD DECLARATIONS
class CPbk2StoreConfiguration;
class CVPbkContactManager;
class MContactDataManagerObserver;
class MVPbkFieldType;
class CPbk2ImageManager;
class CVPbkTopContactManager;
class CRepository;

/**
 *  CEasyDialingContactDataManager
 *  Handles the loading and storing of thumbnail bitmaps
 */
class CEasyDialingContactDataManager : 
        public CBase,
        public MPbk2ImageGetObserver,
        public MVPbkSingleContactOperationObserver,
        public MVPbkContactStoreListObserver,
        public MVPbkOperationErrorObserver,
        public MVPbkOperationResultObserver<MVPbkContactViewBase*>,
        public MPbk2StoreConfigurationObserver,
        public MPbkGlobalSettingObserver,
        public MVPbkContactViewObserver
        
    {
public:
    
    /**
     * Contact name display order 
     */
    enum TNameOrder
        {
        EFirstnameLastname,
        ELastnameFirstname
        };
    
public: // constructor and destructor
    
    /**
    * Two-phased constructor
    */
    static CEasyDialingContactDataManager* NewL();
    
    /**
     * Destructor.
     */
    ~CEasyDialingContactDataManager();
    
private: // constructors
    /**
    * Constructor.
    */
    CEasyDialingContactDataManager();
    
    
    /**
     * Second phase constructor of the class.
     */
    void ConstructL();
    
public: // new methods
    /**
    * Sets the observer. Observer is notified when thumbnail loading is complete
    * @param aObserver
    */
    void SetObserver(MContactDataManagerObserver* aObserver);
    
    /**
     * Gets a handle to the VPbk contact manager
     */
    CVPbkContactManager& ContactManager();
    
    /**
    * Sets the size of the thumbnails. If the loaded thumbnail is larger than
    * the set size, thumnail is scaled to the size. If the set size is different
    * from the old one, deletes all loaded thumbnails, which causes them to be
    * reloaded when they are needed the next time.
    * @param aSize The size of the thumbnails
    */
    void SetThumbnailSize(const TSize& aSize);
    
    /**
    * Gets an id string for the thumbnail, if thumbnail exists for the contact.
    * Used when the listbox items are created.
    * @param aContact Contact link
    * @param aMatchThumbnail If true, tries to match the link to the existing thumbnails
    * @param aFav Is this contact a favourite
    * @return id string. caller needs to delete this.
    */
    HBufC* GetThumbnailIdL( MVPbkContactLink* aContact, TBool aMatchThumbnail, TBool aFav );
    
    /**
     * Gets internal index for a contact item with the given internal ID.
     */
    TInt IndexForId( const TDesC& aId ) const;
    
    /**
    * Gets the thumbnail for a contact item. If thumbnail is not loaded, starts
    * loading it.
    * @param aIndex Internal index for the contact, obtained with IndexForId
    * @param aThumbnail Outparam. Contact thumbnail. NULL if contact doesn't have thumbnail
    * @return true if contact data has been loaded
    */
    TBool GetThumbnail( TInt aIndex, CFbsBitmap*& aThumbnail );
    
    /**
     * Checks if contact is a favorite contact
     * @param   aIndex  Internal index fo the contact, obtained with IndexForId
     * @return  ETrue if contact is a favorite contact
     */
    TBool IsFav( TInt aIndex ) const;
    
    /**
     * Checks if contact is a SIM contact
     * @param   aIndex  Internal index fo the contact, obtained with IndexForId
     * @return  ETrue if contact is a SIM contact
     */
    TBool IsSimContact( TInt aIndex ) const;
    
    /**
     * Checks if contact is a service number contact
     * @param   aIndex  Internal index fo the contact, obtained with IndexForId
     * @return  ETrue if contact is a service number contact
     */
    TBool IsSdnContact( TInt aIndex ) const;
    
    /**
    * Deletes all loaded thumbnails and cancel asynchronous operations.
    */
    void Reset();
    
    /**
    * Returs ETrue if voice call is available for current contact.
    */
    TBool VoiceCallAvailable( TInt aIndex );
    
    
    /**
    * Returs ETrue if video call is available for current contact.
    */
    TBool VideoCallAvailable( TInt aIndex );
    
    
    /**
    * Returs ETrue if uni-editor is available for current contact.
    */
    TBool UniEditorAvailable( TInt aIndex );
    
    
    /**
    * Returns HBufC8 descriptor containing MVPbkContactLink
    * link packaged.
    */
    HBufC8* ContactLinkLC( TInt aIndex );
    
    
    /**
     * Sets the iContactThumbnailSetting member variable to either true or false 
     * which is read and if it is false then thumbnails are not shown 
     * @param aContactThumbnailSetting which is cenrep value for ContactThumbnailSetting
     */
    void SetContactThumbnailSetting( TInt aContactThumbnailSetting );
    
    /**
     * Gets the iContactThumbnailSetting member variable to either true or false 
     * which is read and if it is false then thumbnails are not shown 
     * returns iContactThumbnailSetting which is cenrep value for ContactThumbnailSetting
     */
    TBool GetContactThumbnailSetting( );
    

    /**
    * Flushes the asynchronous contact data of all contacts.
    */    
    void Reload();
    
    /**
    * Checks if contact link is a favourite. This is meant to be used for contacts that
    * don't yet have contact data item. Since this needs to go through all the favourites,
    * unnecessary calls should be avoided.
    */    
    TBool IsFavL( MVPbkContactLink* aLink );
    
    /**
    * @return the number of favourites in contact db
    */
    TInt NumberOfFavsL();
    
    /**
    * Creates contact link to a favourite contact.
    * @param aIndex Index of the favourite in favourites view
    * @return the newly created link (added to cleanupstack)
    */
    MVPbkContactLink* FavLinkLC( TInt aIndex );

    /**
    * Creates contact string for favourite contact. This string can be used directly
    * in the contact listbox. F.ex. "Firstname Surname\tCompanyname".
    * @param aIndex Index of the favourite in favourites view
    * @param aNameOrder Name ordering to be used
    * @return the newly created string (added to cleanupstack)
    */
    HBufC* FavContactStringLC( TInt aIndex, TNameOrder aNameOrder );
    
    /**
     * @return Contact name display order
     */
    TNameOrder NameOrder();

    /**
     * Get URIs of the contact stores which are currently shown in Phonebook.
     * @param   aUris   On return, contains heap descriptors identifying the contact
     *                  stores which are active. Ownership of the descriptors is given.
     */
    void GetCurrentStoreUrisL( RPointerArray<TDesC>& aUris );
    
    /**
     * Pause the loading of contact data. This is used to make sure listbox is responsive
     * and smooth during panning and flicking.
     * @param aPause Pause on/off
     */
    void Pause( TBool aPause );

public:
    
    /**
     * From MPbk2ImageGetObserver.
     */
    void Pbk2ImageGetComplete(MPbk2ImageOperation& aOperation, CFbsBitmap* aBitmap);
    
    
    /**
     * From MPbk2ImageGetObserver.
     */
    void Pbk2ImageGetFailed(MPbk2ImageOperation& aOperation, TInt aError);

public:
    
    /**
     * From MPbk2StoreConfigurationObserver
     */
    void ConfigurationChanged();

    /**
     * From MPbk2StoreConfigurationObserver
     */
    void ConfigurationChangedComplete();
    

public:
    
    /**
     * From MPbkGlobalSettingObserver.
     */
    void SettingChangedL( MPbkGlobalSetting::TPbkGlobalSetting aKey );
    
private: 
    
    /**
     * From MVPbkSingleContactOperationObserver.
     */
    void VPbkSingleContactOperationComplete(
            MVPbkContactOperationBase& aOperation,
            MVPbkStoreContact* aContact );

    
    /**
     * From MVPbkSingleContactOperationObserver.
     */
    void VPbkSingleContactOperationFailed(
            MVPbkContactOperationBase& aOperation, 
            TInt aError );

private:
    
    /**
     * From MVPbkContactStoreListObserver.
     */
    void StoreReady( MVPbkContactStore& aContactStore );
    
    /**
     * From MVPbkContactStoreListObserver.
     */
    void StoreUnavailable( MVPbkContactStore& aContactStore, TInt aReason );
    
    /**
     * From MVPbkContactStoreListObserver.
     */
    void HandleStoreEventL( MVPbkContactStore& aContactStore, 
                            TVPbkContactStoreEvent aStoreEvent );
    
    /**
     * From MVPbkContactStoreListObserver.
     */
    void OpenComplete();
    
private:
    /**
     * From MVPbkOperationErrorObserver.
     */
    void VPbkOperationFailed(
            MVPbkContactOperationBase* aOperation,
            TInt aError );

private:
    /**
     * From MVPbkOperationResultObserver.
     */
    void VPbkOperationResultCompleted(
            MVPbkContactOperationBase* aOperation,
            MVPbkContactViewBase* aOperationResult );

    
private:
    /**
     * From MVPbkContactViewObserver.
     * For observing changes to favourites view.
     */
    virtual void ContactViewReady(
            MVPbkContactViewBase& aView );

    virtual void ContactViewUnavailable(
            MVPbkContactViewBase& aView );

    virtual void ContactAddedToView(
            MVPbkContactViewBase& aView, 
            TInt aIndex, 
            const MVPbkContactLink& aContactLink );

    virtual void ContactRemovedFromView(
            MVPbkContactViewBase& aView, 
            TInt aIndex, 
            const MVPbkContactLink& aContactLink );

    virtual void ContactViewError(
            MVPbkContactViewBase& aView, 
            TInt aError, 
            TBool aErrorNotified );

private:
       
    void InitReady();
    
    TInt FindContactIndex( MVPbkContactLink* aContact );
    
    void LoadNextContactDataL();
    
    void DoHandleImageGetCompleteL(CFbsBitmap* aBitmap, TInt aIndex);
    
    void HandleError(TInt aError);
    
    void DoHandleContactOperationCompleteL( MVPbkStoreContact* aContact, TInt aIndex);
    
    void GetAvailableServicesL( MVPbkStoreContact* aContact, TInt aIndex );
    
    void InformObserver();
    
    void UpdateNameOrderL();
    
    void SetupFavStoreSearchedL();
    
    void SetStoreFlagsForContact( CEasyDialingContactData* aContactData ) const;

private:
    /** Phonebook store configuration. Owned. */
    CPbk2StoreConfiguration* iPbkStoreConfiguration;
    
    /** Array that contains all loaded contact data. Owned. */
    RPointerArray<CEasyDialingContactData> iContactDataArray;

    /** Array that contains indexes of thumbnails that are waiting to be loaded. Owned. */
    RArray<TInt> iWaitingContacts;

    /** Phonebook image manager. Owned. */
    CPbk2ImageManager* iImageManager;
    
    /** Parameters for image manager. Contains info about the size of thumbnails. */
    TPbk2ImageManagerParams iImageManagerParams;
    
    /** Field type. Not owned. */
    const MVPbkFieldType* iThumbnailFieldType;
    
    /** Image loading operation. Owned. */
    MPbk2ImageOperation* iImageOperation;
    
    /** Contact item for which a thumbnail is being loaded. Owned. */
    MVPbkStoreContact* iStoreContact;
    
    /** Virtual phonebook contact manager. Owned. */
    CVPbkContactManager* iContactManager;

    /** Contact data manager observer. Not owned. */
    MContactDataManagerObserver* iObserver;

    /** Top (i.e. favourite) contacts manager. Owned. */
    CVPbkTopContactManager* iVPbkTopContactManager;
    
    /** Virtual phonebook operation handle. Owned. */
    MVPbkContactOperationBase* iContactOperation;

    /** Variable which tells either to show or hide contact thumbnails. */
    TBool iContactThumbnailSetting;
    
    /** ETrue if asynchronous opening of the contac store is ready. */
    TBool iStoreReady;
    
    /** View to vpbk containing all favourite contacts. Owned. */
    MVPbkContactViewBase* iFavsView;
        
    /** Favourites view is ready to be used. */
    TBool iFavsViewReady;
    
    /** Contact store containing favourite contacts is configured to be searched. */
    TBool iFavStoreSearched;
    
    /** Virtual phonebook operation handle for getting favourites. Owned. */
    MVPbkContactOperationBase* iFavsOperation;
    
    /** Phonebook settings interface. */
    MPbkGlobalSetting* iPbkSettings;
    
    /** Contact name display order. */
    TNameOrder iNameOrder;
    
    /** Contact data manager pause status */
    TBool iPause;
    };

#endif //__EASYDIALINGTHUMBNAILMANAGER_H__
