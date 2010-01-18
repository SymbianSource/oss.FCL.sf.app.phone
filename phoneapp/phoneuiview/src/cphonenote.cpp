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
* Description: Implementation of CPhoneNote class.
*
*/


// INCLUDE FILES
#include "cphonenote.h"
#include "phoneui.hrh"
#include <featmgr.h>

// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneNote::CPhoneNote( CEikDialog** aSelfPtr, MEikCommandObserver& aCommandObserver ) :
    CAknNoteDialog( aSelfPtr ),
    iCommandObserver( aCommandObserver ) 
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
// 
CPhoneNote::~CPhoneNote()
    {
    }

// ---------------------------------------------------------
// CPhoneNote::OfferKeyEventL
// ---------------------------------------------------------
//
TKeyResponse CPhoneNote::OfferKeyEventL( 
    const TKeyEvent& aKeyEvent,
    TEventCode /*aType*/ )
    {
	if ( ( aKeyEvent.iScanCode == EStdKeyNo || aKeyEvent.iCode == EKeyNo )
		&& iNoteType != EPhoneNoteSecurity )
		{
		OkToExitL( EPhoneCmdWaitNoteOk );
		return EKeyWasNotConsumed;
		}        
	else
		{
		// Let key events be handled by the application
	    return EKeyWasNotConsumed;		
		}
    }

// ---------------------------------------------------------
// CPhoneNote::OkToExitL
// ---------------------------------------------------------
//
TBool CPhoneNote::OkToExitL( TInt aCommand )
    {
    // Let the command observer process the command
    iCommandObserver.ProcessCommandL( aCommand );

    return EFalse; // so that the dialog will not disappear
    }
    
// ---------------------------------------------------------
// CPhoneNote::SetNoteType
// ---------------------------------------------------------
//
void CPhoneNote::SetNoteType( TPhoneNoteType aNoteType )
    {
    iNoteType = aNoteType;
    }

// ---------------------------------------------------------
// CPhoneNote::NoteType
// ---------------------------------------------------------
//
TPhoneNoteType CPhoneNote::NoteType()
    {
    return iNoteType;
    }

// ---------------------------------------------------------
// CPhoneNote::HandlePointerEventL
// ---------------------------------------------------------
//
void CPhoneNote::HandlePointerEventL( 
    const TPointerEvent& aPointerEvent )
    {
    if( ( iNoteType == EPhoneNoteError ) ||
        ( iNoteType == EPhoneNoteWarning ) ||
        ( iNoteType == EPhoneNoteInformation ) ||
        ( iNoteType ==  EPhoneNoteConfirmation) ||
        ( iNoteType == EPhoneNoteCustom ))
        {
        CEikDialog::HandlePointerEventL( aPointerEvent );
        }
     }

// ---------------------------------------------------------
// CPhoneNote::UpdateSoftkeysL
// ---------------------------------------------------------
//
void CPhoneNote::UpdateSoftkeysL( TInt aResourceId )
    {
    CEikButtonGroupContainer& buttonGroup = ButtonGroupContainer();
    buttonGroup.SetCommandSetL( aResourceId );
    
    buttonGroup.DrawDeferred();    
    }

// End of File
