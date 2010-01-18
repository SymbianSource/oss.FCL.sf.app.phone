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
* Description:  Progress note handler.
*
*/

// INCLUDE FILES
#include "cphoneprogressnote.h"
// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneProgressNote::CPhoneProgressNote(
        TInt aFinalValue,
        TInt anIncrement,
        TInt anInterval,
        CEikDialog** aSelfPtr,
        MEikCommandObserver* aCommandObserver )
    : CAknProgressDialog( 
        aFinalValue, 
        anIncrement,
        anInterval,
        aSelfPtr ),
      iCommandObserver( aCommandObserver )
    {
    }

// ---------------------------------------------------------
// CPhoneProgressDialog::CPhoneProgressDialog
// ---------------------------------------------------------
//
CPhoneProgressNote::CPhoneProgressNote(
        CEikDialog** aSelfPtr,
        MEikCommandObserver* aCommandObserver )
    : CAknProgressDialog(
        aSelfPtr ),
      iCommandObserver( aCommandObserver )
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
// 
CPhoneProgressNote::~CPhoneProgressNote()
    {
    }

// ---------------------------------------------------------
// CPhoneProgressNote::OfferKeyEventL
// ---------------------------------------------------------
//
TKeyResponse CPhoneProgressNote::OfferKeyEventL( 
    const TKeyEvent& /*aKeyEvent*/,
    TEventCode /*aType*/ )
    {
    // Let key events be handled by the application
    return EKeyWasNotConsumed;
    }

// ---------------------------------------------------------
// CPhoneProgressNote::OkToExitL
// ---------------------------------------------------------
//
TBool CPhoneProgressNote::OkToExitL( TInt aCommand )
    {
    // Let the command observer process the command
    if ( iCommandObserver )
        {
        iCommandObserver->ProcessCommandL( aCommand );
        }
    return EFalse; // so that the dialog will not disappear
    }

// end of file
