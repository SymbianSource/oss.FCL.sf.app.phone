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
* Description:  Contact
*
*/


#ifndef CPHCNTCONTACT_H
#define CPHCNTCONTACT_H

#include <e32base.h>
#include "mphcntmatch.h"
#include "tphcntnumber.h"

class MPhCntContactFields;
class CPhCntFoundContacts;
class MPhCntContactManager;
class MVPbkContactLinkArray;
class MVPbkStoreContact;
class MPbk2ImageGetObserver;
class MPbk2ImageOperation;
class CPhCntVPbkContactId;
class TPhCntSipURI;
class CCntRawPhoneNumberExtractor;

/**
 *  Contact data.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntContact ) : public CBase, public MPhCntMatch 
    {
public:
    
    /**
     * Static constructor.
     *
     * @since S60 v3.1
     * @param aContactFields Contact fields, takes ownership.
     * @param aContactManager Contact manager.
     */
    static CPhCntContact* NewL( 
        MPhCntContactFields* aContactFields,
        MPhCntContactManager& aContactManager );
    
    /**
     * Destructor.
     */
    ~CPhCntContact();
    
public:

    /** 
     * The number, which was used to match this contact, should be
     * set here.
     *
     * @since S60 v3.1
     * @param aNumber Number, which was used to match this contact.
     */
    void SetOriginalNumberL( const TDesC& aNumber );
    
    /**
     * Gives the original number used for matching this contact.
     * 
     * @since S60 v3.1
     * @return Origianl number used for matching this contact.
     */
    const TDesC& OriginalNumber() const;
    
    /**
     * Sets the owner of this contact.
     *
     * @since S60 v3.1
     * @param aContactOwner Owner of this contact.
     */
    void SetOwner( CPhCntFoundContacts* aContactOwner );
    
    /**
     * Increase reference count.
     *
     * @since S60 v3.1
     */
    void IncreaseReferenceCount();
    
    /**
     * Gives all phone numbers that this contact has.
     * 
     * @since S60 v3.2
     * @return All phone numbers contact has.
     */
    virtual const RArray<TPhCntNumber>& AllNumbers() const;
    
    /**
     * Gives DTMF number, which was selected by user.
     *
     * @since S60 v3.2
     * @return DTMF number or empty descriptor if dmtf
     * was not defined for this contact.
     */
    virtual TPtrC Dtmf() const;
    
    /**
     * Sets the number that matched to the original number.
     * 
     * @since S60 v3.1
     * @param aNumber Number.
     */
    void SetMatchedVoipNumber( const TPhCntSipURI& aSipUri );

    
// From base class MPhCntMatch

    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    void Release();
    
    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    MPhCntMatch::TNumberType NumberType() const;
    
    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    CPhCntContactId* ContactId() const;
    
    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    MVPbkContactLink* ContactLink() const;

    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    TCliType Cli( HBufC*& aCliText ) const;

    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    TPtrC FirstName() const;

    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    TPtrC LastName() const;

    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    TPtrC CompanyName() const;

    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    TPtrC Number() const;

	/**
     * Finds a phone number for the requested speed dial position.
     * @since S60 v3.2
     * @param aPosition The speed dial position.
     * @return Phone number. Empty if there isn't one specified.
     */
    TPhCntNumber SpeedDialNumber( const TInt aPosition );

    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    TPtrC PersonalRingingTone() const;
    
    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    TBool BelongsToGroups( 
        const CArrayFix<TContactItemId>& aGroupArray ) const;

    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    HBufC* TextToSpeechTextL() const;
    
    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    TPtrC CallText() const;

    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    TPtrC CallImage() const;
    
    /**
     * From base class MPhCntMatch
     * @since S60 v3.1
     * @see MPhCntMatch
     */
    TBool HasThumbnailImage() const;  
    
    /**
     * Loads the thumbnail of this contact.
     * 
     * @since S60 3.1
     * @param aObserver Observer of the loading.
     * @return Operation, that can be canceled by deleting it. Ownership is given.
     */
    MPbk2ImageOperation* LoadThumbnailL(
        MPbk2ImageGetObserver& aObserver ); 
        
    /**
     * From base class MPhCntMatch
     * @since S60 v3.2
     * @see MPhCntMatch
     */
    CDesCArray& AllDtmfNumbers() const;          
        
      
        
protected:
        
    CPhCntContact(
        MPhCntContactFields* aContactFields,
        MPhCntContactManager& aContactManager );
    
    void ConstructL();
    
    /**
     * Determines if this contact belongs to a group of contacts.
     * 
     * @since S60 v3.1.
     * @param aGroup Array of contact links, which defines a group.
     * @return ETrue - This contact belongs to group.
     * EFalse - This contact does not belong to group.
     */
    TBool BelongsToGroupsL( MVPbkContactLinkArray* aGroups ) const;

private:
    
    /**
     * Get number type from number that matches with supplied number 
     * @since S60 v3.2
     * @param aNumber Number which type is unknown 
     * @return Number type
     */
    MPhCntMatch::TNumberType NumberType( const TDesC& aNumber ) const;
            
protected: // Data
    
    /**
     * Contacts fields
     * Own.
     */
    MPhCntContactFields* iContactFields;
    
    /** 
     * Number, which was used for matching this contact.
     * Own.
     */
    HBufC* iOriginalNumber;
    
    /**
     * Owner of this contact.
     * Not Own.
     */
    CPhCntFoundContacts* iOwner;
    
    /**
     * Number of references of this object.
     */
    TInt iReferenceCount;
    
    /**
     * Contact manager.
     * Not own.
     */
    MPhCntContactManager& iContactManager;
    
    /**
     * Contact identifier.
     * own.
     */
    CPhCntVPbkContactId* iContactId;
    
    /**
     * Matched voip number.
     * own.
     */
    HBufC* iMatchedVoipNumber;

    /**
     * Number extractor.
     * Own.
     */
    CCntRawPhoneNumberExtractor* iNumberExtractor;
    };

#endif // CPHCNTCONTACT_H
