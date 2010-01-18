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
* Description: Implementation of CPhoneDtmfSendingNote class.
*
*/


// INCLUDE FILES
#include "cphonedtmfsendingnote.h"
#include "phoneui.hrh"

// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneDtmfSendingNote::CPhoneDtmfSendingNote( CEikDialog** aSelfPtr, MEikCommandObserver& aCommandObserver ) :
    CPhoneNote( aSelfPtr, aCommandObserver ) 
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
// 
CPhoneDtmfSendingNote::~CPhoneDtmfSendingNote()
    {
    }

// ---------------------------------------------------------
// CPhoneDtmfSendingNote::OfferKeyEventL
// ---------------------------------------------------------
//
TKeyResponse CPhoneDtmfSendingNote::OfferKeyEventL( 
    const TKeyEvent& aKeyEvent,
    TEventCode /*aType*/ )
    {
	if ( aKeyEvent.iScanCode == EStdKeyNo || aKeyEvent.iCode == EKeyNo )
		{
		OkToExitL( EPhoneCmdWaitNoteOk );
		return EKeyWasNotConsumed;
		}
	else if( aKeyEvent.iScanCode == EStdKeyApplication0 ||
	         aKeyEvent.iScanCode == EStdKeyYes || aKeyEvent.iCode == EKeyYes )
		{
		// Don't consume application and Yes -key
		return EKeyWasNotConsumed;			
		}
	else
		{
		// Consume other key events
	    return EKeyWasConsumed;		
		}
    }

// End of File
