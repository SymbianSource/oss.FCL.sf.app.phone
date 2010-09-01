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
* Description: Implementation of TPhoneCmdParamReconnectQuery class.
*
*/


// INCLUDE FILES
#include <ccauireconnectquery.h>

#include "tphonecmdparamreconnectquery.h" 
#include "phoneui.pan"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// TPhoneCmdParamReconnectQuery::TPhoneCmdParamReconnectQuery
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C TPhoneCmdParamReconnectQuery::TPhoneCmdParamReconnectQuery( 
	MCaUiReconnectQueryObserver& aObserver ):
    TPhoneCommandParam(),
    iObserver( aObserver ),
    iContactMatcher( NULL ),
    iPhoneNumber( KNullDesC ),
    iIncludeVideoCallOption( EFalse )
    {
    iParamId = EPhoneParamIdReconnectQuery;
    }

// ---------------------------------------------------------
// TPhoneCmdParamReconnectQuery::SetContactMatcher
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamReconnectQuery::SetContactMatcher( CPhCntMatcher* aPhCntMatcher )
    {
   	__ASSERT_DEBUG( aPhCntMatcher, 
   		Panic( EPhoneUtilsParameterNotInitialized ) );
    iContactMatcher = aPhCntMatcher;   
    }

// ---------------------------------------------------------
// TPhoneCmdParamReconnectQuery::SetPhoneNumber
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamReconnectQuery::SetPhoneNumber( const TPtrC aPhoneNumber )
    {
    iPhoneNumber.Set( aPhoneNumber );   
    }

// ---------------------------------------------------------
// TPhoneCmdParamReconnectQuery::SetIncludeVideoOption
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamReconnectQuery::SetIncludeVideoOption( 
    const TBool aIncludeVideoCallOption )
    {
    iIncludeVideoCallOption = aIncludeVideoCallOption;
    }

// ---------------------------------------------------------
// TPhoneCmdParamReconnectQuery::ReconnectQueryObserver
// ---------------------------------------------------------
//
EXPORT_C MCaUiReconnectQueryObserver& TPhoneCmdParamReconnectQuery::ReconnectQueryObserver() const
    {
    return iObserver;
	}

// ---------------------------------------------------------
// TPhoneCmdParamReconnectQuery::ContactMatcher
// ---------------------------------------------------------
//
EXPORT_C CPhCntMatcher* TPhoneCmdParamReconnectQuery::ContactMatcher() const
	{
   	return iContactMatcher;
   	}

// ---------------------------------------------------------
// TPhoneCmdParamReconnectQuery::PhoneNumber
// ---------------------------------------------------------
//
EXPORT_C const TPtrC TPhoneCmdParamReconnectQuery::PhoneNumber() const
   	{
   	return iPhoneNumber;
   	}

// ---------------------------------------------------------
// TPhoneCmdParamReconnectQuery::IncludeVideoCallOption
// ---------------------------------------------------------
//
EXPORT_C TBool TPhoneCmdParamReconnectQuery::IncludeVideoCallOption() const
   	{
   	return iIncludeVideoCallOption;
   	}

//  End of File  
