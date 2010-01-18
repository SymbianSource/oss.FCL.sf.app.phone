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
* Description:  Interface for phonebook services.
*
*/


#ifndef MPHCNTCONTACTMANAGER_H
#define MPHCNTCONTACTMANAGER_H

#include <cntdef.h>
#include <VPbkContactView.hrh>

class MVPbkContactLink;
class MVPbkSingleContactOperationObserver;
class MVPbkContactOperationBase;
class MVPbkContactStoreList;
class MVPbkContactFindObserver;
class MPhCntContactMatchStrategy;
class MPbk2ContactNameFormatter;
class CVPbkContactIdConverter;
class MVPbkContactLinkArray;
class MVPbkContactFindObserver;
class MPbk2ImageOperation;
class MPbk2ImageGetObserver;
class MVPbkFieldType;
class MVPbkStoreContact;
class MVPbkStoreContactField;
class CVPbkFieldTypeSelector;
class MPhCntStoreLoaderObserver;
class CPhCntContactStoreUris;

/**
 * Interface for phonebook services.
 *
 * @since S60 v3.1
 * @lib PhoneCntFinder.lib
 */
NONSHARABLE_CLASS(  MPhCntContactManager )
    {
public:
    
    /**
     * Retrieves contact from phonebook.
     * 
     * @since S60 v3.1
     * @param aLink Link to the contact.
     * @param aObserver Observer for the retrieval.
     * @return Handle to the retrieval operation.
     */
    virtual MVPbkContactOperationBase* RetrieveContactL(
        const MVPbkContactLink& aLink,
        MVPbkSingleContactOperationObserver& aObserver) = 0;
        
    /**
     * Gives the list of contact stores.
     * 
     * @since S60 v3.1
     * @return List of contact stores.
     */
    virtual MVPbkContactStoreList& ContactStoresL() const = 0;
    
    /**
     * Factory method for creating contact match strategy.
     *
     * @since S60 v3.1
     * @param aObserver Contact match strategy observer.
     * @return New contact match strategy instance.
     */
    virtual MPhCntContactMatchStrategy* CreateContactMatchStrategyL(
        MVPbkContactFindObserver& aObserver,
        TBool aRemoveDuplicates )= 0;
    
    /**
     * Gives access to contact name formatter.
     *
     * @since S60 v3.1
     * @return Contact name formatter.
     */    
    virtual MPbk2ContactNameFormatter& ContactNameFormatter() = 0;
    
    /**
     * Converts contact ids to contact links.
     * 
     * @since S60 v3.1
     * @param aContactId Array of contact ids.
     * @return Array of contact links. Ownership given.
     */
    virtual MVPbkContactLinkArray* ConvertContactIdsToLinksL(  
        const CArrayFix<TContactItemId>& aContactId ) const = 0;
        
    /**
     * Converts contact id to contact link.
     * 
     * @since S60 v3.1
     * @param aContactId Contact id from Contact DB.
     * @return Concact link, ownership is given to caller.
     * If the aContactId is KNullContactId then then returned value is NULL.
     */
    virtual MVPbkContactLink* ConvertContactIdToLinkL( 
        TContactItemId aContactId ) const = 0;
        
    /**
     * Converts contact link to contact id.
     *
     * @since S60 v3.1
     * @param aContactLink Reference to contactlink.
     * @return Contact id.
     */
    virtual TContactItemId ConvertContactLinkToContactId(
        const MVPbkContactLink& aContactLink ) const = 0;
        
    /**
     * Converts packed link descriptor to contact link.
     * 
     * @since S60 v3.2
     * @param aPackeLink Link prepresented as desctiptor.
     * @return Contact link. Returns null if aPackedLink length is 0.
     */
    virtual MVPbkContactLink* ConvertDescriptorToLinkL( 
        const TDesC8& aPackedLink ) const = 0;
        
    /**
     * Retrieves contact, which has been assinged to speeddial position
     * aSpeedDialPosition.
     *
     * @since S60 v3.1
     * @param aSpeedDialPosition Speed dial position.
     * @param aObserver Observer for operation.
     * @return Operation, ownership is given to caller.
     */
    virtual MVPbkContactOperationBase* RetrieveSpeedDialContactLinkL(
        const TInt aSpeedDialPosition,
        MVPbkContactFindObserver& aObserver ) = 0;

	/**
     * Indicates whether the speed dial attribute has been set for the given position.
     *
     * @since S60 v3.2
     * @see MPhCntContactManager.
     * @param aSpeedDialPosition Speed dial position.
     * @param aField Field to be evaluated.
     * @return ETrue if aField has a matching speed dial position,
     *         EFalse otherwise.     
     */    
	virtual TBool HasSpeedDialL(      
		const TInt aSpeedDialPosition,  
		const MVPbkStoreContactField& aField ) = 0;        

    /**
     * Retrieves contacts image. To cancel the thumbnail
     * fetch, delete operation.
     *
     * @since S60 v3.1
     * @param aContact Contact, which image is fetched.
     * @param aFieldType Field type.
     * @param aObserver Observer of the fetch.
     */
    virtual MPbk2ImageOperation* RetrieveImageL(
        MVPbkStoreContact& aStoreContact,
        const MVPbkFieldType& aFieldType,
        MPbk2ImageGetObserver& aObserver ) = 0;
        
    /** 
     * Creates field type selector.
     *
     * @since S60 v3.2
     */    
    virtual CVPbkFieldTypeSelector* CreateFieldTypeSelectorL() const = 0;
    
    /**
     * Appends filter to selector.
     *
     * @since S60 v3.2
     * @param aSelector Selector to append the filter.
     * @param aFilter The filter to be appended.
     */
    virtual void AppendFilterToSelectorL( CVPbkFieldTypeSelector& aSelector,
        TVPbkContactViewFilter aFilter ) = 0;
   
    /**
     * Loads the store specified in the contact link and adds the store to 
     * the list of stores handled by this manager.
     *
     * @since   S60 v9.1
     * @param   aContactLink    A contact link containing store to load.
     * @param   aObserver       An observer for the loading process.
     */
    virtual void LoadContactStoreL( const TDesC8& aContactLink,
        MPhCntStoreLoaderObserver& aObserver ) = 0;
   
    /**
     * Loads the store specified by the URI and adds the store to 
     * the list of stores handled by this manager.
     *
     * @since   S60 v9.1
     * @param   aStoreUri       A URI of the store to load.
     * @param   aObserver       An observer for the loading process.
     */
    virtual void LoadContactStoreWithUriL( 
        const TDesC& aStoreUri,
        MPhCntStoreLoaderObserver& aObserver ) = 0;
		
    /** 
     * Return contact store uri's.
     *
     * @since S60 v9.1
     */ 
    virtual CPhCntContactStoreUris& ContactStoreUrisL() = 0;
	
protected:

    MPhCntContactManager(){};
    
    virtual ~MPhCntContactManager() {};
    };

#endif 