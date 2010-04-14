/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Holds data for one match result.
*
*/


#ifndef CPHCNTMATCHITEM_H
#define CPHCNTMATCHITEM_H

//  INCLUDES
#include <e32base.h>
#include <CNTDEF.H>     // TContactItemId

#include "CPhCntMatcher.h" // TFieldId
#include "mphcntmatch.h" // TPhCCliType

// CONSTANTS

// Panic literal for maching related panics.
_LIT( KPhCntMatchPanic, "PhCntMatch" );

/**
* Enumerates panic reasons.
*
* EPhCntMatchReferenceCountPanic 
*       - reference count is invalid  
* EPhCntMatchNeededObjectNotPresentPanic 
*       - instance is not present even if it is required to be.
* EPhCntMatchPhonebookNotReleasedPanic 
*       - phonebook has been released but still request of 
*         phonebook to be released is given.
*/
enum TPhCntMatchPanicReason
    {
    EPhCntMatchReferenceCountPanic,
    EPhCntMatchNeededObjectNotPresentPanic,
    EPhCntMatchPhonebookNotReleasedPanic
    };

// FORWARD DECLARATIONS
class CPbkContactItem;
class MPhCntMatchItemOwner;
class CContactIdArray;
class CPhCntPbkContactId;

// CLASS DECLARATION

/**
*  Holds data for one match result.
*  Many CPhCntMatch objects may use this.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCntMatchItem ) :public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aTelNumber Original phone number
        * @param aContact Matched contact item
        * @param aFieldId Matched field from the contact item
        * @param aOwner Own this i.e. handles delete
        * @return New instance
        */
        static CPhCntMatchItem* NewL( 
            const TDesC& aTelNumber,
            CPbkContactItem& aContact,
            const TFieldId aFieldId,
            MPhCntMatchItemOwner& aOwner );
        
        /**
        * Destructor.
        */
        virtual ~CPhCntMatchItem();

    public: // New functions
        
        /**
        * Get the compared phone number
        * @return The original phonenumber
        */
        inline const TDesC& OriginalNumber() const;

        /**
        * Add reference to this match item.
        */
        inline void AddReference();

        /**
        * Removes reference to this match item. If reference count
        * gets to zero the object is deleted.
        */
        void RemoveReference();

        /**
        * Returns reference count.
        * @return The count.
        */
        inline TInt ReferenceCount() const;

        /**
        * Get contact ID.
        * @return The ID.
        */
        inline CPhCntPbkContactId* ContactId() const;

        /**
        * Get number type
        * @return Phonebook number type
        */
        inline TInt NumberType() const;

        /**
        * Get contact's CLI
        * @param aCliText CLI is saved here. User deletes the object.
        * @return CLI type
        */
        MPhCntMatch::TCliType Cli( HBufC*& aCliText ) const;

        /**
        * First name field content.
        * @return The field. Empty if not specified.
        */
        TPtrC FirstName() const;

        /**
        * Last name field content.
        * @return The field. Empty if not specified.
        */
        TPtrC LastName() const;

        /**
        * Company name field content.
        * @return The field. Empty if not specified.
        */
        TPtrC CompanyName() const;

        /**
        * Number field content (matched field).
        * @return The field. Empty if not specified.
        */
        TPtrC Number() const;

        /**
        * Ringing tone field content.
        * @return The field. Empty if not specified.
        */
        TPtrC PersonalRingingTone() const;

        /**
        * Find if contact belongs to any contact groups given.
        * @param aGroupArray Compared contact groups.
        * @return ETrue is belongs at least one group from parameter.
        */
        TBool BelongsToGroups( const CArrayFix<TContactItemId>& aGroupArray ) const;

        /**
        * Returns text to speech text for this contact. Used in
        * text to speech ringing tone.
        * @since Series 60 3.0
        * @return descriptor, ownership passed.
        */
        HBufC* TextToSpeechTextL() const;
        
        /**
        * Call text field content.
        * @since Series 60 3.1
        * @return The field. Empty if not specified.
        */
        TPtrC CallText() const;

        /**
        * Call image field content.
        * @since Series 60 3.1
        * @return The field. Empty if not specified.
        */
        TPtrC CallImage() const;        
        
        /**
        * Check if contact has a thumbnail image.
        * @since Series 60 3.1
        * @return ETrue if thumbnail exists, otherwise EFalse.
        */
        TBool HasThumbnailImage() const;        
        
    private:

        /**
        * C++ constructor.
        */
        CPhCntMatchItem( const TFieldId aFieldId, MPhCntMatchItemOwner& aOwner);

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL( const TDesC& aTelNumber, CPbkContactItem& aContact );

        
        /**
        * Read all needed info from the contact.
        * @param aContact Contact item from Phonebook
        */
        void GetContactInfoL( CPbkContactItem& aContact );
        void ReadFirstNameL( CPbkContactItem& aContact );
        void ReadLastNameL( CPbkContactItem& aContact );
        void ReadCompanyNameL( CPbkContactItem& aContact );
        void ReadNumberL( CPbkContactItem& aContact );
        void ReadNumberType( CPbkContactItem& aContact );
        void ReadCliAndTypeL( CPbkContactItem& aContact );
        void ReadGroupsL( CPbkContactItem& aContact );
        void ReadRingingToneL( CPbkContactItem& aContact );
        void ReadTextToSpeechL( CPbkContactItem& aContact );
        void ReadCallObjectL( CPbkContactItem& aContact );

        /**
        * Allocate string and trim the result.
        * @since Series 60 3.0
        * @param aText text to be allocated.
        * @return trimmed version.
        */
        static HBufC* AllocAndTrimL( const TDesC* aText );

        /**
        * Returns ETrue if order of the names needs to be swapped.
        * Default order is first name followed by last name.
        *
        * @since Series 60 3.0
        * @return ETrue if order of the names needs to be swapped.
        */
        static TBool SwapNameOrder();

    private:    // Data
        // Original phone number buffer
        HBufC* iOriginalTelNumber;
        // Contact item id
        CPhCntPbkContactId* iContactId;
        // Field id of the contact
        TFieldId iFieldId;
        // Reference count to this match data
        TInt iReferenceCount;
        // Who can delete this (when reference count comes to 0)
        MPhCntMatchItemOwner& iOwner;

        // First name buffer
        HBufC* iFirstName;
        // Last name buffer
        HBufC* iLastName;
        // Company name buffer
        HBufC* iCompanyName;
        // Matched phone numer buffer
        HBufC* iNumber;
        // Number type
        TInt iNumberType;
        // CLI
        HBufC* iCli;
        // CLI type (name, number, company, ...)
        MPhCntMatch::TCliType iCliType;
        // Contact groups
        CContactIdArray* iGroups;
        // Path to ringing tone
        HBufC* iRingingTone;

        // Text to speech related variables:

        // First name reading
        HBufC* iFirstNameReading;
        // Last name reading
        HBufC* iLastNameReading;
        // Nick name
        HBufC* iSecondName;
        // Call text
        HBufC* iCallText;
		// Call image
        HBufC* iCallImage;
        // Has thumbnail image
        TBool iHasThumbnailImage;

    };

#include    "CPhCntMatchItem.inl"

#endif      // CPHCNTMATCHITEM_H  
            
// End of File
