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
* Description:  Offers services from phonebook.
*
*/

#ifndef CPHCNTCONTACTMANAGER_H
#define CPHCNTCONTACTMANAGER_H

#include <e32base.h>
#include <MVPbkContactStoreListObserver.h>
#include "MPhCntContactManager.h"
#include "mphcntvoipcontactfinder.h"

class CVPbkContactManager;
class CVPbkContactStoreUriArray;
class MPbk2ContactNameFormatter;
class CPbk2SortOrderManager;
class CVPbkContactIdConverter;
class CVPbkSpeedDialAttribute;
class CPbk2ImageManager;
class CVPbkFieldTypeRefsList;
class CPhCntContactStoreUris;
class MPhCntStoreLoaderObserver;

/**
 *  Offers services from phonebook.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntContactManager ): 
    public CBase, 
    public MPhCntContactManager, 
    public MVPbkContactStoreListObserver,
    public MPhCntVoipContactFinder
    {
public:
    
    /**
     * Static constructor.
     *
     * @since S60 v3.1
     */
    static CPhCntContactManager* NewL();
    
    /**
     * Destructor.
     *
     * @Since S60 v3.1
     */
    ~CPhCntContactManager();
    
    /**
     * Gives reference to contact manager.
     *
     * @since S60 v3.2
     * @return Contact manager.
     */
    CVPbkContactManager& ContactManager() const;
    
    /**
     * Return list of additional contact stores
     * @since S60 v3.2
     * @return Array of additional contact stores
     */
    const CVPbkContactStoreUriArray& AdditionalContactStoreUris();

    
// From base class MPhCntContactManager

	/**
	 * From MPhCntContactManager.
	 *
	 * @since S60 v3.1
	 * @see MPhCntContactManager.
	 */
    MVPbkContactOperationBase* RetrieveContactL(
        const MVPbkContactLink& aLink,
        MVPbkSingleContactOperationObserver& aObserver );
       
    /**
	 * From MPhCntContactManager.
	 *
	 * @since S60 v3.1
	 * @see MPhCntContactManager.
	 */ 
    MVPbkContactStoreList& ContactStoresL() const;
   
    /**
     * From MPhCntContactManager.
	 *
	 * @since S60 v3.1
	 * @see MPhCntContactManager.
	 */
    MPhCntContactMatchStrategy* CreateContactMatchStrategyL(
            MVPbkContactFindObserver& aObserver,
            TDuplicateRemovalStrategy aStrategy );
   
    /**
     * From MPhCntContactManager.
     *
     * @since S60 v3.1
     * @see MPhCntContactManager.
     */         
    MPbk2ContactNameFormatter& ContactNameFormatter();
   
    /**
     * From MPhCntContactManager.
     *
     * @since S60 v3.1
     * @see MPhCntContactManager.
     */
    MVPbkContactLinkArray* ConvertContactIdsToLinksL(  
        const CArrayFix<TContactItemId>& aContactId ) const;
    
    /**
     * From MPhCntContactManager.
     *
     * @since S60 v3.1
     * @see MPhCntContactManager.
     */    
    MVPbkContactLink* ConvertContactIdToLinkL( 
        TContactItemId aContactId ) const;   
        
    /**
     * From MPhCntContactManager.
     *
     * @since S60 v3.1
     * @see MPhCntContactManager.
     */
    TContactItemId ConvertContactLinkToContactId(
        const MVPbkContactLink& aContactLink ) const;
    
    /**
     * From MPhCntContactManager.
     *
     * @since S60 v3.2
     * @see MPhCntContactManager.
     */   
    MVPbkContactLink* ConvertDescriptorToLinkL( const TDesC8& aPackedLink ) const;
     
    /**
     * From MPhCntContactManager.
     *
     * @since S60 v3.1
     * @see MPhCntContactManager.
     */   
    MVPbkContactOperationBase* RetrieveSpeedDialContactLinkL(
        const TInt aSpeedDialPosition,
        MVPbkContactFindObserver& aObserver );
        
    /**
     * From MPhCntContactManager.
     *
     * @since S60 v3.2
     * @see MPhCntContactManager.
     */   
	TBool HasSpeedDialL(        
		const TInt aSpeedDialPosition,
		const MVPbkStoreContactField& aField );
    
    /**
     * Retrieves contacts thumbnail. To cancel the thumbnail
     * fetch, delete operation.
     *
     * @since S60 v3.1
     * @param aContact Contact, which thumbnail is fetched.
     * @param aThumbnailFieldType Thumbnails field type.
     * @param aObserver Observer of the fetch.
     */
    MPbk2ImageOperation* RetrieveImageL(
        MVPbkStoreContact& aStoreContact,
        const MVPbkFieldType& aThumbnailFieldType,
        MPbk2ImageGetObserver& aObserver );
    
    /**
     * From MPhCntContactManager.
     *
     * @since S60 v3.2
     * @see MPhCntContactManager.
     */   
    CVPbkFieldTypeSelector* CreateFieldTypeSelectorL() const;
    
    /**
     * From MPhCntContactManager.
     *
     * @since S60 v3.2
     * @see MPhCntContactManager.
     */  
    void AppendFilterToSelectorL( CVPbkFieldTypeSelector& aSelector,
        TVPbkContactViewFilter aFilter );

    /**
     * From MPhCntContactManager. 
     *
     * @since   S60 v9.1
     * @see MPhCntContactManager.
     */
    void LoadContactStoreL( const TDesC8& aContactLink,
        MPhCntStoreLoaderObserver& aObserver );

    /**
     * From MPhCntContactManager. 
     *
     * @since   S60 v9.1
     * @see MPhCntContactManager.
     */    
    void LoadContactStoreWithUriL(
        const TDesC& aStoreUri,
        MPhCntStoreLoaderObserver& aObserver );
 
    /**
     * From MPhCntContactManager. 
     *
     * @since   S60 v9.1
     * @see MPhCntContactManager.
     */
    CPhCntContactStoreUris& ContactStoreUrisL();
	
// From base class MPhCntContactFinder
    
    /**
     * From base class MPhCntVoipContactFinder
     * Finds voip contacts, with given URI or partial URI. To cancel
     * the find, destroy the returned operation.
     * @since S60 v3.1
     * @param aSipURI Sip uri defining wanted matches.
     * @param aObserver Observer of the operation.
     * @return Operation.
     */
    MVPbkContactOperationBase* FindVoipContactsL( 
        const TDesC& aSipURI,
        MVPbkContactFindObserver& aObserver ) const;

// From base class MVPbkContactStoreListObserver
    
    /**
     * From MVPbkContactStoreListObserver.
     * 
     * @since S60 v3.1
     * @see MVPbkContactStoreListObserver
     */	
    void OpenComplete();
    
// From base class MVPbkContactStoreObserver 

    /**
     * From MVPbkContactStoreObserver.
     * 
     * @since S60 v3.1
     * @see MVPbkContactStoreObserver
     */
    void StoreReady( MVPbkContactStore& aContactStore );
    
    /**
     * From MVPbkContactStoreObserver.
     * 
     * @since S60 v3.1
     * @see MVPbkContactStoreObserver
     */
    void StoreUnavailable( MVPbkContactStore& aContactStore, TInt aReason );
    
    /**
     * From MVPbkContactStoreObserver.
     * 
     * @since S60 v3.1
     * @see MVPbkContactStoreObserver
     */
    void HandleStoreEventL(
        MVPbkContactStore& aContactStore, 
        TVPbkContactStoreEvent aStoreEvent);
    
private:
        
    CPhCntContactManager();
    
    void ConstructL();
    
    /**
     * Adds voip field types.
     * 
     * @since S60 v3.1
     * @param aVoipFieldType VoIP field type.
     */
    void AddVoipFieldTypeL( const MVPbkFieldType* aVoipFieldType );
    
private:

    /**
     * Contact manager.
     * Own.
     */
    CVPbkContactManager* iContactManager;
    
    /**
     * Contact name formatter.
     * Own.
     */
    MPbk2ContactNameFormatter* iContactNameFormatter;
    
    /**
     * Sort order.
     * Own.
     */
    CPbk2SortOrderManager* iSortOrder;
    
    /**
     * Contact id converter.
     * Own.
     */
    CVPbkContactIdConverter* iContactIdConverter;
    
    /**
     * Contact store.
     * Not own.
     */
    MVPbkContactStore* iContactDBStore;
    
    /**
     * Speed dial.
     * Own 
     */
     CVPbkSpeedDialAttribute* iSpeedDialAttribute;
     
     /**
      * For retrieveing thumbnail from contact.
      * own.
      */
     CPbk2ImageManager* iImageManager;
     
     /**
      * For finding voip contacts.
      * Own.
      */
     CVPbkFieldTypeRefsList* iVoipFieldTypeList;
    
     /**
      * Contact store uris
      * Own.
      */
     CPhCntContactStoreUris* iContactStoreUris;
    
     /**
      * Store loader observer.
      * Not own.
      */
     MPhCntStoreLoaderObserver* iStoreLoaderObserver;
     
     /**
     * Stored contact store for callback usage.
     */
     MVPbkContactStore* iContactStore;
     
    };

#endif // CPHCNTCONTACTMANAGER_H
