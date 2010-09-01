/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of TPhCntContactField class.
*
*/


#include <VPbkFieldType.hrh>
#include <TVPbkFieldTypeParameters.h>
#include <VPbkEng.rsg>
#include <MVPbkStoreContactField.h>

#include "MPhCntContactManager.h"
#include "tphcntcontactfield.h"
#include "PhCntTrace.h"

// Speed dial related constants
const TInt KPhCntPositionNotSet = 0;
const TInt KPhCntPositionMin = 1;
const TInt KPhCntPositionMax = 9;


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TPhCntContactField::TPhCntContactField() :
    iDataPointedByContactLink( EFalse ),
    iContactFieldId( 0 )
    {
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//    
TPhCntContactField::TPhCntContactField(
    TInt aContactFieldId ) :
    iDataPointedByContactLink( EFalse ),
    iContactFieldId( aContactFieldId )
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//    
TPhCntContactField::~TPhCntContactField()
    {
    }
   

// ---------------------------------------------------------------------------
// Gives the contact field data.
// ---------------------------------------------------------------------------
//
const TDesC& TPhCntContactField::Data() const
    {
    return iContactFieldData;
    }

// ---------------------------------------------------------------------------
// Checks if this resolver is received datas resolver. If so then sets
// the contact field data.
// ---------------------------------------------------------------------------
//
void TPhCntContactField::Resolve(
    TInt aContactFieldId, 
    const TDesC& aFieldData,
    TBool aPointedData )
    {
    if( iContactFieldId == aContactFieldId )
        {
        SetData( aFieldData, aPointedData );
        }
    }
 
// ---------------------------------------------------------------------------
// Sets the contact field data if the possible earlier data is not 
// data which has been pointed by contact link.
// ---------------------------------------------------------------------------
//   
void TPhCntContactField::SetData( 
    const TDesC& aData,
    TBool aPointedData )
    {
    if( !iDataPointedByContactLink )
        {
        iDataPointedByContactLink = aPointedData;
        PRINTF( "PhCnt: ContactField.SetData: %S", &aData );
        iContactFieldData.Set( aData );
        }
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TPhCntPhoneNumber::TPhCntPhoneNumber() : 
    TPhCntContactField( EVPbkVersitNameTEL )
    {
    }
    
// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
TPhCntPhoneNumber::~TPhCntPhoneNumber()
    {
    iAllNumbers.Close();
    }

// ---------------------------------------------------------------------------
// Gives the phone number pointed by contact link
// ---------------------------------------------------------------------------
//    
const TPhCntNumber& TPhCntPhoneNumber::Number() const
    {
    return iPointedNumber;
    }

// ---------------------------------------------------------------------------
// Gives all numbers resolved.
// ---------------------------------------------------------------------------
//    
const RArray<TPhCntNumber>& TPhCntPhoneNumber::AllNumbers() const
    {
    return iAllNumbers;
    }
 
// ---------------------------------------------------------------------------
// Resolves phone number and its type.
// ---------------------------------------------------------------------------
//   
void TPhCntPhoneNumber::ResolveL(
    TInt aContactFieldId, 
    const TDesC& aFieldData,
    TBool aPointedData,
    MPhCntContactManager& aContactManager,
    MVPbkStoreContactField& aContactField )
    {        	
    
    // Evaluate fields type information
    const MPhCntMatch::TNumberType numberType = PhoneNumberType( aContactFieldId );    
    if ( numberType != MPhCntMatch::ENone ) 
    	{    									
    	TBool speedDial = EFalse;    									
		// Test all speed dial positions
		for( TInt positionIndex = KPhCntPositionMin; positionIndex <= KPhCntPositionMax; positionIndex++ )
			{
			if ( aContactManager.HasSpeedDialL( positionIndex, aContactField ) )			
				{					
				speedDial = ETrue;
				// Add phone number & position specific entry
				SetNumber( aFieldData, numberType, aPointedData, positionIndex ); 				
				}
			}				      
		if ( !speedDial )
			{
			// Add at least one phone number entry without any positions
			SetNumber( aFieldData, numberType, aPointedData, KPhCntPositionNotSet );	
			}		
        }              		        
    }

// ---------------------------------------------------------------------------
// Evaluates the type of a phone number field.
// ---------------------------------------------------------------------------
//   
MPhCntMatch::TNumberType TPhCntPhoneNumber::PhoneNumberType( 
	TInt aContactFieldId )
	{
	MPhCntMatch::TNumberType numberType = MPhCntMatch::ENone;
	
    switch( aContactFieldId ) 
        {
        case R_VPBK_FIELD_TYPE_MOBILEPHONEGEN:
        case R_VPBK_FIELD_TYPE_MOBILEPHONEWORK:
        case R_VPBK_FIELD_TYPE_MOBILEPHONEHOME:
            numberType = MPhCntMatch::EMobileNumber;            
            break;
            
        case R_VPBK_FIELD_TYPE_VOIPHOME:
        case R_VPBK_FIELD_TYPE_VOIPWORK:
        case R_VPBK_FIELD_TYPE_VOIPGEN:
        case R_VPBK_FIELD_TYPE_IMPP:
        case R_VPBK_FIELD_TYPE_SIP:
            numberType = MPhCntMatch::EVoipNumber;            
            break;
            
        case R_VPBK_FIELD_TYPE_LANDPHONEHOME:
        case R_VPBK_FIELD_TYPE_LANDPHONEWORK:
        case R_VPBK_FIELD_TYPE_LANDPHONEGEN:
            
            numberType = MPhCntMatch::EStandardNumber;            
            break;
            
        case R_VPBK_FIELD_TYPE_PAGERNUMBER:
            numberType = MPhCntMatch::EPagerNumber;            
            break;
            
        case R_VPBK_FIELD_TYPE_FAXNUMBERGEN:
        case R_VPBK_FIELD_TYPE_FAXNUMBERHOME:
        case R_VPBK_FIELD_TYPE_FAXNUMBERWORK:
           numberType = MPhCntMatch::EFaxNumber;           
           break;
           
        case R_VPBK_FIELD_TYPE_VIDEONUMBERHOME:
        case R_VPBK_FIELD_TYPE_VIDEONUMBERWORK:
        case R_VPBK_FIELD_TYPE_VIDEONUMBERGEN:
            numberType = MPhCntMatch::EVideoNumber;
            break;
            
        case R_VPBK_FIELD_TYPE_ASSTPHONE:
			numberType = MPhCntMatch::EAssistantNumber;
			break;
		
        case R_VPBK_FIELD_TYPE_CARPHONE:
            numberType = MPhCntMatch::ECarNumber;
            break;                   
                            
        default:
            // Not a phone number.
            break;
        }
        return numberType;	
    }
 
// ---------------------------------------------------------------------------
// Sets number 
// ---------------------------------------------------------------------------
//   
void TPhCntPhoneNumber::SetNumber( 
    const TDesC& aNumber, 
    MPhCntMatch::TNumberType aNumberType, 
    TBool aPointedField,
    TInt aSpeedDialPosition )
    {
    if( aPointedField && !iDataPointedByContactLink ) 
        {
        iDataPointedByContactLink = ETrue;
        iPointedNumber.Set( aNumber, aNumberType, aSpeedDialPosition );    
        }
    iAllNumbers.Append( TPhCntNumber( aNumber, aNumberType, aSpeedDialPosition ) );
    }
    
    
