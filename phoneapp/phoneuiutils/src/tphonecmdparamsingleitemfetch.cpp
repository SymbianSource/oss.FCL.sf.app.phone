/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of TPhoneCmdParamSingleItemFetch class.
*
*/


// INCLUDE FILES
#include "tphonecmdparamsingleitemfetch.h"
#include "phoneui.pan"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// TPhoneCmdParamSingleItemFetch::TPhoneCmdParamSingleItemFetch
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C TPhoneCmdParamSingleItemFetch::TPhoneCmdParamSingleItemFetch():
    TPhoneCommandParam(),
    iType( CPhCntSingleItemFetch::EFetchDtmf ),
    iTitlePaneResourceId( 0 ), 
    iCbaResourceId( 0 ), 
    iString( NULL )
    {
    iParamId = EPhoneParamIdSingleItemFetch;
    }

// ---------------------------------------------------------
// TPhoneCmdParamSingleItemFetch::SetType
// Sets the single item fetch type
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamSingleItemFetch::SetType( 
    CPhCntSingleItemFetch::TFetchType aType )
    {
    iType = aType;        
    }

// ---------------------------------------------------------
// TPhoneCmdParamSingleItemFetch::Type
// Returns the single item fetch type
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CPhCntSingleItemFetch::TFetchType 
    TPhoneCmdParamSingleItemFetch::Type()
    {
    return iType;    
    }
    
// ---------------------------------------------------------
// TPhoneCmdParamSingleItemFetch::SetTitlePaneResourceId
// Sets the title pane resource id
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamSingleItemFetch::SetTitlePaneResourceId( 
   TInt aTitlePaneResourceId )
    {
    __ASSERT_DEBUG( aTitlePaneResourceId, 
        Panic( EPhoneUtilsParameterNotInitialized ) );
    iTitlePaneResourceId = aTitlePaneResourceId;   
    }

// ---------------------------------------------------------
// TPhoneCmdParamSingleItemFetch::TitleResourceId
// Returns the title pane resource id
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt TPhoneCmdParamSingleItemFetch::TitlePaneResourceId() const
    {
    __ASSERT_DEBUG( iTitlePaneResourceId, 
        Panic( EPhoneUtilsInvariant ) );
    return iTitlePaneResourceId;
    }

// ---------------------------------------------------------
// TPhoneCmdParamSingleItemFetch::SetCbaResourceId
// Sets the single item fetch dialog CBA resource id
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamSingleItemFetch::SetCbaResourceId( 
   TInt aCbaResourceId )
    {
    __ASSERT_DEBUG( aCbaResourceId, 
        Panic( EPhoneUtilsParameterNotInitialized ) );
    iCbaResourceId = aCbaResourceId;   
    }

// ---------------------------------------------------------
// TPhoneCmdParamSingleItemFetch::CbaResourceId
// Returns the single item fetch dialog CBA resource id
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt TPhoneCmdParamSingleItemFetch::CbaResourceId() const
    {
    __ASSERT_DEBUG( iCbaResourceId, 
        Panic( EPhoneUtilsInvariant ) );
    return iCbaResourceId;
    }

// ---------------------------------------------------------
// TPhoneCmdParamSingleItemFetch::SetString
// Sets the string for the single item fetch dialog
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamSingleItemFetch::SetString( TDes* aString )
    {
    __ASSERT_DEBUG( aString, 
        Panic( EPhoneUtilsParameterNotInitialized ) );
    iString = aString;
    }

// ---------------------------------------------------------
// TPhoneCmdParamSingleItemFetch::String
// Returns the string for the single item fetch dialog
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TDes* TPhoneCmdParamSingleItemFetch::String() const
    {
    __ASSERT_DEBUG( iString, 
        Panic( EPhoneUtilsInvariant ) );
    return iString;
    }
   
//  End of File  
