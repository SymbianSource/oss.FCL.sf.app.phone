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
* Description:  Resolves and represents the contact field information
*
*/

#ifndef TPHCNTCONTACTFIELD_H
#define TPHCNTCONTACTFIELD_H


#include <e32base.h>
#include <TVPbkFieldVersitProperty.h>
#include "mphcntmatch.h"
#include "tphcntnumber.h"

class MPhCntContactManager;
class MVPbkStoreContactField;


/**
 *  Contact field from contact.
 *
 *  Resolves the type of the contact field ysing contact field id.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( TPhCntContactField ) 
    {
public:
    
    /**
     * Constructor.
     * 
     * @since S60 v3.1
     * @param aContactFieldId Contact field identifier.
     */
    TPhCntContactField( TInt aContactFieldId );
    
    /**
     * Constructor.
     *
     * @since S60 v3.1
     */
    TPhCntContactField();
    
    /**
     * Destructor.
     *
     * @since S60 v3.1
     */
    virtual ~TPhCntContactField();
        
    /** 
     * Resolves if the contact field data belongs to this
     * contact field. If the contact field data belongs to this then
     * the data is stored to this field.
     *
     * @since S60 v3.1
     * @param aContactFieldId aFieldData's field identifier.
     * @param aFieldData Data in the contact field.
     * @param aPointedField Was the field data pointed by contact link.
     */
    virtual void Resolve( 
        TInt aContactFieldId, 
        const TDesC& aFieldData, 
        TBool aPointedField );

    /**
     * Data of the field.
     *
     * @since S60 v3.1
     * @return Data.
     */
    const TDesC& Data() const;

protected:

    /**
     * Sets the data.
     */
    void SetData( const TDesC& aData, TBool aPointedData );
    
protected: // data 

    /**
     * Was the data set by pointed field.
     */
    TBool iDataPointedByContactLink;

private: // data

    /**
     * Data of the contact field.
     */
    TPtrC iContactFieldData;
    
    /**
     * Possible field id, which identifies this field.
     */
    TInt iContactFieldId;
    
    };
    
/**
 *  Phonenumber contact field.
 *
 *  Resolves phone numbers from virtual phonebook. 
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( TPhCntPhoneNumber ): public TPhCntContactField
    {
public:

    /**
     * Constructor.
     *
     * @since S60 v3.1
     */
    TPhCntPhoneNumber();
    
    /**
     * Destructor.
     *
     * @since S60 v3.1
     */
    ~TPhCntPhoneNumber();
    
    /**
     * Gives the phone number that was matched(pointed by VPB) from contact.
     * If there wasn't number that were pointed by VPB this number does
     * not contain information.
     *
     * @since S60 v3.1.
     * @return Possible number which was pointed by virtual phonebook
     */
    const TPhCntNumber& Number() const;
    
    /**
     * Gives array of all resolved numbers, including possible matched number.
     *
     * @since S60 v3.1.
     * @return Array of phone numbers.
     */     
    const RArray<TPhCntNumber>& AllNumbers() const;
            	
// from base class TPhCntContactField

    /**
     * From TPhCntContactField.
     *
     * @since S60 v3.1
     * @see TPhCntContactField.
     */
    void ResolveL( 
        TInt aContactFieldId, 
        const TDesC& aFieldData, 
        TBool aPointedField,
        MPhCntContactManager& aContactManager,
    	MVPbkStoreContactField& aContactField
        );                  
    
protected: 

	/**
     * Evaluates the type of a phone number field.
     * @param aContactFieldId Field to be resolved.
     * @return Resolved phone number type, 
     *         MPhCntMatch::ENone if the type was other than a phone number
     */
    MPhCntMatch::TNumberType PhoneNumberType( 
    	TInt aContactFieldId );    	
    	
    void SetNumber( const TDesC& aNumber, 
                    MPhCntMatch::TNumberType aNumberType, 
                    TBool aPointedField,
                    TInt aSpeedDialPosition );
        
private: // Data
    
    /**
     * Phone number that was pointed by contact link.
     */
    TPhCntNumber iPointedNumber;
    
    /**
     * Array of all resolved phone numbers, including
     * pointed number.
     * Own.
     */
    RArray<TPhCntNumber> iAllNumbers;

    };

#endif // TPHCNTCONTACTFIELD_H
