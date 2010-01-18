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
* Description:  Resolves contact fields.
*
*/



#ifndef CPHCNTCONTACTFIELDSRESOLVER_H
#define CPHCNTCONTACTFIELDSRESOLVER_H

#include <e32base.h>

#include "tphcntcontactfield.h"
#include "MPhCntMatch.h"
#include "mphcntcontactfields.h"

class MVPbkContactLink;
class MVPbkStoreContact;
class MPbk2ContactNameFormatter;
class MVPbkStoreContactField;
class MPhCntContactManager;

/**
 *  Resolves contact's fields.
 *
 *  @lib PhoneCntFinder
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
NONSHARABLE_CLASS( CPhCntContactFieldsResolver ) : 
    public CBase, public MPhCntContactFields
    {
public:

    /**
     * Static constructor.
     *
     * @since S60 v3.1
     * @param aContactLink Link pointing to the contact, which fields are resolved.
     * @param aContact Contact which fields are resolved.
     * @param aNameFormatter Contact title formatter.
     * @return New instance
     */
    static CPhCntContactFieldsResolver* NewL(
        MVPbkContactLink* aContactLink,
        MVPbkStoreContact* aContact,
        MPhCntContactManager& aContactManager );

    /**
     * Static constructor.
     *
     * @since S60 v3.1
     * @param aContactLink Link pointing to the contact, which fields are resolved.
     * @param aContact Contact which fields are resolved.
     * @param aNameFormatter Contact title formatter.
     * @return New instance
     */
    static CPhCntContactFieldsResolver* NewLC(
        MVPbkContactLink* aContactLink,
        MVPbkStoreContact* aContact,
        MPhCntContactManager& aContactManager );
    
    /**
     * Destructor.
     */    
    virtual ~CPhCntContactFieldsResolver();

// from base class MPhCntContactFields
    
    /**
     * From base class MPhCntContactFields
     * Get the number type.
     * @since S60 v3.1
     * @return Phonebook number type.
     */
    MPhCntMatch::TNumberType NumberType() const;
    
    /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return Contact link, ownership is not given.
     */
    MVPbkContactLink* ContactLink() const;
    
    /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return First name. Empty if there isn't one specified.
     */
    TPtrC FirstName() const;

    /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return Last name. Empty if there isn't one specified.
     */
    TPtrC LastName() const;

    /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return Company name. Empty if there isn't one specified.
     */
    TPtrC CompanyName() const;

    /**    
     * From base class MPhCntContactFields
     * Returns the matched number.
     * @since S60 v3.1
     * @return Phone number. Empty if there isn't one specified.
     */
    TPtrC Number() const;
    
    /**
     * From base class MPhCntContactFields
     * 
     * @since S60 v3.2
     * @see MPhCntContactFields
     */
    TPtrC Dtmf() const;

    /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return Ringing tone. Empty if there isn't one specified.
     */
    TPtrC PersonalRingingTone() const;
    
    /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return Second name. Empty if there isn't one specified.
     */
    TPtrC SecondName() const;
    
    /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return First name's pronunction. May be zero length.
     */
    TPtrC FirstNamePronunciation() const;
     
    /**
     * From base class MPhCntContactFields
     * Last name pronunciation.
     * 
     * @since S60 v3.1
     * @return Last name's pronunction. May be zero length.
     */
    TPtrC LastNamePronunciation() const;
    
    /**
     * From base class MPhCntContactFields
     * Company name pronunciation.
     * 
     * @since S60 v3.1
     * @return Company name's pronunction. May be zero length.
     */
    TPtrC CompanyNamePronunciation() const;
    
    /**
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return Contact title. Ownership given.
     */
    HBufC* GetContactTitleL();
    
     /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return Array of contact links to groups, where this contact belongs
     */
    MVPbkContactLinkArray* ContactGroupsLC() const;
    
     /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return Contact store, where this contacts information is stored.
     */
    MVPbkStoreContact& StoreContact() const;
    
    /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return Thumbnail field. Null if no thumbnail field defined.
     */
    const MVPbkFieldType* ThumbnailField() const;
    
    /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return Call image. Empty if there isn't one specified.
     */
    TPtrC CallImage() const;
    
    /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return Call text. Empty if there isn't one specified.
     */
    TPtrC CallText() const;

    /**    
     * From base class MPhCntContactFields
     * @since S60 v3.1
     * @return ETrue if contact has thumbnail field.
     */
    TBool HasThumbnail() const;
    
    /**
     * From base class MPhCntContactFields
     *
     * @since S60 v3.2.
     * @return Array of phone numbers.
     */     
    const RArray<TPhCntNumber>& AllNumbers() const;
    
    /**
     * From base class MPhCntContactFields
     *
     * @since S60 v3.2.
     * @return Array of predefined DTMF strings.
     */        
    CDesCArray* AllDtmfNumbers() const;     

private:

    CPhCntContactFieldsResolver(
        MVPbkContactLink* aContactLink,
        MVPbkStoreContact* aContact,
        MPhCntContactManager& aContactManager );

    void ConstructL();
    
    void ResolveFieldL( 
        MVPbkStoreContactField& aContactField,
        TBool aFieldPointedByContactLink );
    
    void ResolveFieldDataL( 
        MVPbkStoreContactField& aContactField, 
        const TPtrC aFieldTextData,
        TBool aFieldPointedByContactLink );

private: // data

    
    /**
     * Resolvers for single fields
     */
    TPhCntPhoneNumber iPhoneNumberResolver;
    TPhCntContactField iFirstNameResolver;
    TPhCntContactField iLastNameResolver;
    TPhCntContactField iCompanyNameResolver;
    TPhCntContactField iSecondNameResolver;
    TPhCntContactField iRingingToneResolver;
    TPhCntContactField iFirstNamePronunciationResolver;
    TPhCntContactField iLastNamePronunciationResolver;
    TPhCntContactField iCompanyNamePronunciationResolver;
    TPhCntContactField iCallImageResolver;
    TPhCntContactField iCallTextResolver;
    TPhCntContactField iDtmfResolver;
        
    /**
     * Link to the contact.
     * Own.
     */
    MVPbkContactLink* iContactLink;
    
    /**
     * Contact
     * Own.
     */
    MVPbkStoreContact* iContact;
    
    /**
     * Handle to virtual phonebook.
     * Not Own.
     */
    MPhCntContactManager& iContactManager;

    /**
     * Contact title formatter.
     * Not own.
     */
    MPbk2ContactNameFormatter& iNameFormatter;
    
    /**
     * Thumbnails field, if any
     * Not own.
     */
    const MVPbkFieldType* iThumbnailField;

    /**
    * All DTMF numbers in array. Owned.
    */
    CDesCArray* iAllDtmfNumbers;
    };

#endif // CPHCNTCONTACTFIELDSRESOLVER_H
