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
* Description:  Interface for contact fields.
*
*/

#ifndef MPHCNTCONTACTFIELDS_H
#define MPHCNTCONTACTFIELDS_H

#include <e32std.h>
#include "MPhCntMatch.h"
#include "tphcntnumber.h"

class MVPbkContactLinkArray;
class MVPbkStoreContact;
class MVPbkFieldType;

/**
 *  Interface for contact fields
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( MPhCntContactFields )
    {
public:
    
    /**
     * Destructor.
     *
     * @since S60 v3.1
     */
    virtual ~MPhCntContactFields() {};

    /**
     * Gives the matched numbers number type. Note that
     * this can be ENone when the contact fields were
     * fetched from contact stores without matching.
     *
     * @since S60 v3.1
     * @return Type of matched number.
     */
    virtual MPhCntMatch::TNumberType NumberType() const = 0;
        
    /**
     * Gives the contact's link where these contact fields
     * are from.
     *
     * @since S60 v3.1
     * @return Contact link, ownership not given.
     */
    virtual MVPbkContactLink* ContactLink() const = 0;
    
    /**
     * First name field from contact.
     *
     * @since S60 v3.1
     * @return Field text. Empty if there isn't one specified.
     */
    virtual TPtrC FirstName() const = 0;

    /**
     * Last name field from contact.
     *
     * @since S60 v3.1
     * @return Field text. Empty if there isn't one specified.
     */
    virtual TPtrC LastName() const = 0;

    /**
     * Company name field from contact.
     *
     * @since S60 v3.1
     * @return Field text. Empty if there isn't one specified.
     */
    virtual TPtrC CompanyName() const = 0;

    /**
     * Number that matched. Note that this may be 
     * zero length, if contactfields were fetched without 
     * matching.
     *
     * @since S60 v3.1
     * @return Field text. Empty if there isn't one specified.
     */
    virtual TPtrC Number() const = 0;
    
    /**
     * Dtmf number, which was pointed by contact link.
     *
     * @since S60 v3.2
     * @return Field text. Empty if there isn't one specified.
     */
    virtual TPtrC Dtmf() const = 0;

    /**
     * Ringing tone field from contact.
     *
     * @since S60 v3.1
     * @return Field text. Empty if there isn't one specified.
     */
    virtual TPtrC PersonalRingingTone() const = 0;
    
    /**
     * Second name from contact.
     *
     * @since S60 v3.1
     * @return Field text. Empty if there isn't one specified.
     */
    virtual TPtrC SecondName() const = 0;
    
    /**
     * First name pronunciation.
     * 
     * @since S60 v3.1
     * @return First name's pronunction. May be zero length.
     */
    virtual TPtrC FirstNamePronunciation() const = 0;
     
    /**
     * Last name pronunciation.
     * 
     * @since S60 v3.1
     * @return Last name's pronunction. May be zero length.
     */
    virtual TPtrC LastNamePronunciation() const = 0;
    
    /**
     * Company name pronunciation.
     * 
     * @since S60 v3.1
     * @return Company name's pronunction. May be zero length.
     */
    virtual TPtrC CompanyNamePronunciation() const = 0;
    
    /**
     * Creates contacts title. 
     *
     * @since S60 v3.1
     * @return Title of the contact, ownership is given.
     */
    virtual HBufC* GetContactTitleL() = 0;
    
    /**
     * Links to a contact groups that this contact information belongs to.
     * Ownership of links is given. May be NULL.
     *
     * @since S60 v3.1
     * @return Links to contact groups.
     */
    virtual MVPbkContactLinkArray* ContactGroupsLC() const = 0;
    
    /**
     * Contact information in the store.
     * 
     * @since S60 v3.1
     * @return Contact information
     */     
    virtual MVPbkStoreContact& StoreContact() const = 0;
    
    /**
     * Pointer to thumbnails field.
     * 
     * @since S60 v3.1
     * @return thumbnails field or null.
     */
    virtual const MVPbkFieldType* ThumbnailField() const = 0;
    
    /**
     * Call image field from contact.
     * 
     * @since S60 v3.1
     * @return Field text. Empty if there isn't one specified.
     */
    virtual TPtrC CallImage() const = 0;
    
    /**
     * Call text field from contact.
     * 
     * @since S60 v3.1
     * @return Field text. Empty if there isn't one specified.
     */
    virtual TPtrC CallText() const = 0;
    
    /**
     * Call text field from contact.
     * 
     * @since S60 v3.1
     * @return ETrue if contact has thumbnail field, otherwise EFalse.
     */
    virtual TBool HasThumbnail() const = 0;
    
    /**
     * Gives all phone numbers that contact has.
     *
     * @since S60 v3.2.
     * @return Array of phone numbers.
     */     
    virtual const RArray<TPhCntNumber>& AllNumbers() const = 0;

    /**
     * Gives all DTMF numbers that contact has.
     *
     * @since S60 v3.2.
     * @return Array of DTMF numbers.
     */     
    
    virtual CDesCArray* AllDtmfNumbers() const = 0; 
    };


#endif // MPHCNTCONTACTFIELDS_H
