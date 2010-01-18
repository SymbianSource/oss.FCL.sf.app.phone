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
* Description:  Resolves contact's fields
*
*/


#include <MVPbkContactLink.h>
#include <MVPbkStoreContact.h>
#include <MPbk2ContactNameFormatter.h>
#include <MVPbkStoreContactFieldCollection.h>
#include <MVPbkFieldType.h>
#include <MVPbkContactFieldData.h>
#include <MVPbkContactFieldTextData.h>
#include <MVPbkContactFieldUriData.h>
#include <TVPbkFieldVersitProperty.h>
#include <VPbkEng.rsg>
#include <badesca.h>

#include "MPhCntContactManager.h"
#include "cphcntcontactfieldsresolver.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntContactFieldsResolver::CPhCntContactFieldsResolver(
    MVPbkContactLink* aContactLink,
    MVPbkStoreContact* aContact,
    MPhCntContactManager& aContactManager ) : 
    iFirstNameResolver( R_VPBK_FIELD_TYPE_FIRSTNAME ),
    iLastNameResolver(R_VPBK_FIELD_TYPE_LASTNAME),
    iCompanyNameResolver( R_VPBK_FIELD_TYPE_COMPANYNAME ),
    iSecondNameResolver( R_VPBK_FIELD_TYPE_SECONDNAME ),
    iRingingToneResolver( R_VPBK_FIELD_TYPE_RINGTONE ),
    iFirstNamePronunciationResolver( R_VPBK_FIELD_TYPE_FIRSTNAMEREADING ),
    iLastNamePronunciationResolver( R_VPBK_FIELD_TYPE_LASTNAMEREADING ),
    iCompanyNamePronunciationResolver( R_VPBK_FIELD_TYPE_COMPANYNAME ),
    iCallImageResolver( R_VPBK_FIELD_TYPE_CALLEROBJIMG ),
    iCallTextResolver( R_VPBK_FIELD_TYPE_CALLEROBJTEXT ),
    iDtmfResolver( R_VPBK_FIELD_TYPE_DTMFSTRING ),    	    
	iContactLink( aContactLink ),
    iContact( aContact ),    
    iContactManager( aContactManager ),
    iNameFormatter( aContactManager.ContactNameFormatter() )
    {
   
    }


// ---------------------------------------------------------------------------
// Secondphase constructor.
// ---------------------------------------------------------------------------
//
void CPhCntContactFieldsResolver::ConstructL()
    {
    iAllDtmfNumbers = new ( ELeave ) CDesCArrayFlat( 2 );
    // Check if contactlink is pointing directly to some contact field.
    // If there is pointed field, then it is going to be in resolved contact fields.
    // For example there migth be many phonenumbers in contact, but the 
    // we want to have only the pointed number resolved.
    MVPbkStoreContactField* field = iContact->Fields().
        RetrieveField( *iContactLink );
    if( field != NULL ) 
        {
        ResolveFieldL( *field, ETrue );
        }
    
    // Go through the contact's field and resolve the fields.              
    MVPbkStoreContactFieldCollection& fields = iContact->Fields();
    const TInt fieldCount( fields.FieldCount() );
    for( TInt i = 0; i < fieldCount; i++ ) 
        {
        MVPbkStoreContactField& field = fields.FieldAt( i );
        ResolveFieldL( field, EFalse );
        }
    }
    
// ---------------------------------------------------------------------------
// Resolves contact's fields
// ---------------------------------------------------------------------------
//
void CPhCntContactFieldsResolver::ResolveFieldL( 
    MVPbkStoreContactField& aContactField, 
    TBool aFieldPointedByContactLink )
    {
     MVPbkContactFieldData& fieldData = aContactField.FieldData();
        
    const MVPbkFieldType* fieldType = aContactField.BestMatchingFieldType();
    if( fieldType ) 
        {
        const TInt fieldId( fieldType->FieldTypeResId() );
        if( fieldData.DataType() == EVPbkFieldStorageTypeText )
            {
            const TPtrC fieldTextData = 
                MVPbkContactFieldTextData::Cast( fieldData ).Text();
            
            ResolveFieldDataL( aContactField,
                               fieldTextData,
                               aFieldPointedByContactLink );   
                                       
            // DTMF field is only used if the contact link was pointing to it.
            if( aFieldPointedByContactLink )
                {
                iDtmfResolver.Resolve( 
                    fieldId, 
                    fieldTextData, 
                    aFieldPointedByContactLink );
                }
                
             if ( fieldId == R_VPBK_FIELD_TYPE_DTMFSTRING )
                {
                TRAP_IGNORE( iAllDtmfNumbers->AppendL( fieldTextData ) );
                }               
            }
        else if ( fieldData.DataType() == EVPbkFieldStorageTypeUri )
            {
            MVPbkContactFieldUriData& textData =
                MVPbkContactFieldUriData::Cast( fieldData );
            TPtrC fieldTextData = textData.Text();
            
            ResolveFieldDataL( aContactField,
                               fieldTextData,
                               aFieldPointedByContactLink );
                                       
            }
            
        // Check for thumbnail field.
        if( fieldId == R_VPBK_FIELD_TYPE_THUMBNAILPIC )
            {
            iThumbnailField = fieldType;    
            }
                
        }
    }

// ---------------------------------------------------------------------------
// Resolves contact's fields
// ---------------------------------------------------------------------------
//
void CPhCntContactFieldsResolver::ResolveFieldDataL( 
    MVPbkStoreContactField& aContactField,
    const TPtrC aFieldTextData,
    TBool aFieldPointedByContactLink )
    {
    MVPbkContactFieldData& fieldData = aContactField.FieldData();
        
    const MVPbkFieldType* fieldType = aContactField.BestMatchingFieldType();
    const TInt fieldId( fieldType->FieldTypeResId() );
           
    iPhoneNumberResolver.ResolveL( fieldId, aFieldTextData,
        aFieldPointedByContactLink, iContactManager, aContactField );
    iFirstNameResolver.Resolve(  fieldId, aFieldTextData,
        aFieldPointedByContactLink );
    iLastNameResolver.Resolve(  fieldId, aFieldTextData,
        aFieldPointedByContactLink );
    iCompanyNameResolver.Resolve(  fieldId, aFieldTextData,
        aFieldPointedByContactLink );
    iSecondNameResolver.Resolve(  fieldId, aFieldTextData,
        aFieldPointedByContactLink );
    iRingingToneResolver.Resolve(  fieldId, aFieldTextData,
        aFieldPointedByContactLink );
    iFirstNamePronunciationResolver.Resolve( fieldId, aFieldTextData,
        aFieldPointedByContactLink );
    iLastNamePronunciationResolver.Resolve( fieldId, 
            aFieldTextData, aFieldPointedByContactLink ); 
    iCompanyNamePronunciationResolver.Resolve( fieldId,
            aFieldTextData, aFieldPointedByContactLink );
    
    iCallImageResolver.Resolve( fieldId, 
                                aFieldTextData, 
                                aFieldPointedByContactLink ); 
    
    iCallTextResolver.Resolve( fieldId, 
                               aFieldTextData, 
                               aFieldPointedByContactLink );    

    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntContactFieldsResolver* CPhCntContactFieldsResolver::NewL(
    MVPbkContactLink* aContactLink,
    MVPbkStoreContact* aContact,
    MPhCntContactManager& aContactManager )
    {
    CPhCntContactFieldsResolver* self = 
        CPhCntContactFieldsResolver::NewLC(
            aContactLink, aContact, aContactManager );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntContactFieldsResolver* CPhCntContactFieldsResolver::NewLC(
    MVPbkContactLink* aContactLink,
    MVPbkStoreContact* aContact,
    MPhCntContactManager& aContactManager ) 
    {
    CPhCntContactFieldsResolver* self = 
        new( ELeave ) CPhCntContactFieldsResolver(
            aContactLink,
            aContact,
            aContactManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntContactFieldsResolver::~CPhCntContactFieldsResolver()
    {
    delete iContact;
    delete iContactLink;
    delete iAllDtmfNumbers;
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
MVPbkContactLink* CPhCntContactFieldsResolver::ContactLink() const 
    {   
    return iContactLink;
    }
  
// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
// 
MPhCntMatch::TNumberType
    CPhCntContactFieldsResolver::NumberType() const   
    {
    return iPhoneNumberResolver.Number().Type();
    }    

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContactFieldsResolver::FirstName() const 
    {
    return iFirstNameResolver.Data();
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContactFieldsResolver::LastName() const 
    {
    return iLastNameResolver.Data();
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContactFieldsResolver::CompanyName() const 
    {
    return iCompanyNameResolver.Data();
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContactFieldsResolver::Number() const
    {
    return iPhoneNumberResolver.Number().Number();
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//   
TPtrC CPhCntContactFieldsResolver::Dtmf() const
    {
    return iDtmfResolver.Data();
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContactFieldsResolver::PersonalRingingTone() const 
    {
    return iRingingToneResolver.Data();
    }
    
// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContactFieldsResolver::SecondName() const
    {
    return iSecondNameResolver.Data();
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContactFieldsResolver::FirstNamePronunciation() const
    {
    return iFirstNamePronunciationResolver.Data();
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//     
TPtrC CPhCntContactFieldsResolver::LastNamePronunciation() const
    {
    return iLastNamePronunciationResolver.Data();
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//    
TPtrC CPhCntContactFieldsResolver::CompanyNamePronunciation() const
    {
    return iCompanyNamePronunciationResolver.Data();
    }
    
// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
HBufC* CPhCntContactFieldsResolver::GetContactTitleL()
    {
    return iNameFormatter.GetContactTitleOrNullL(
        iContact->Fields(), 
        MPbk2ContactNameFormatter::EPreserveLeadingSpaces | 
        MPbk2ContactNameFormatter::EUseSeparator |
        MPbk2ContactNameFormatter::EReplaceNonGraphicChars );
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
MVPbkContactLinkArray* CPhCntContactFieldsResolver::ContactGroupsLC() const
    {
    return iContact->GroupsJoinedLC();
    }
 
// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//   
 MVPbkStoreContact& CPhCntContactFieldsResolver::StoreContact() const
    {
    return *iContact;
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//       
const MVPbkFieldType* CPhCntContactFieldsResolver::ThumbnailField() const
    {
    return iThumbnailField;
    }
    
// ---------------------------------------------------------------------------
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
// 
TPtrC CPhCntContactFieldsResolver::CallImage() const 
    {
    return iCallImageResolver.Data();
    }

// ---------------------------------------------------------------------------    
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
TPtrC CPhCntContactFieldsResolver::CallText() const 
    {
    return iCallTextResolver.Data();
    }

// ---------------------------------------------------------------------------    
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//
TBool CPhCntContactFieldsResolver::HasThumbnail() const 
    {
    return ( iThumbnailField != NULL );
    }        

// ---------------------------------------------------------------------------    
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
// 
const RArray<TPhCntNumber>& CPhCntContactFieldsResolver::AllNumbers() const
    {
    return iPhoneNumberResolver.AllNumbers();
    }

// ---------------------------------------------------------------------------    
// From class MPhCntContactFields
// ---------------------------------------------------------------------------
//        
CDesCArray* CPhCntContactFieldsResolver::AllDtmfNumbers() const
    {
    return iAllDtmfNumbers;
    }
