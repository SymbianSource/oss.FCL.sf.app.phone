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
* Description: Implementation of CPhoneQueryDialog class.
*
*/


// INCLUDE FILES
#include "cphonequerydialog.h"
#include "phoneui.hrh"

// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneQueryDialog::CPhoneQueryDialog( const TTone& aTone ) :
	CAknQueryDialog( aTone )
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
// 
CPhoneQueryDialog::~CPhoneQueryDialog()
    {
    }

// ---------------------------------------------------------
// CPhoneQueryController::NewL()
// Two-phased constructor
// ---------------------------------------------------------
//
CPhoneQueryDialog* CPhoneQueryDialog::NewL( const TTone& aTone )
	{
	CPhoneQueryDialog* self = new( ELeave )CPhoneQueryDialog( aTone );
	return self;	
	}

// ---------------------------------------------------------
// CPhoneQueryDialog::OfferKeyEventL
// ---------------------------------------------------------
//
TKeyResponse CPhoneQueryDialog::OfferKeyEventL( 
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
	if ( aKeyEvent.iScanCode == EStdKeyNo || aKeyEvent.iCode == EKeyNo )
		{
		OkToExitL( EPhoneCmdDoNothing );
		return EKeyWasNotConsumed;
		}
	else
		{
		return CAknQueryDialog::OfferKeyEventL( aKeyEvent, aType );
 		}
    }

// End of File
