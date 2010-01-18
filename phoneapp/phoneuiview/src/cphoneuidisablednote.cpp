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
* Description: Implementation of CPhoneUIDisabledNote class.
*
*/


// INCLUDE FILES
#include "cphoneuidisablednote.h"
#include "aknnotedialog.h"
#include "phoneui.hrh"
#include <featmgr.h>
#include <aknnotedialog.h>
#include <eikdialg.h>
#include <aknnotecontrol.h>


// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneUIDisabledNote::CPhoneUIDisabledNote( CEikDialog** aSelfPtr, MEikCommandObserver& aCommandObserver )
    : CPhoneNote( aSelfPtr, aCommandObserver ) 
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
// 
CPhoneUIDisabledNote::~CPhoneUIDisabledNote()
    {
    }

// ---------------------------------------------------------
// CPhoneUIDisabledNote::OfferKeyEventL
// ---------------------------------------------------------
//
TKeyResponse CPhoneUIDisabledNote::OfferKeyEventL( 
    const TKeyEvent& aKeyEvent,
    TEventCode /*aType*/ )
    {
    // Let key events be handled by the application
	return EKeyWasNotConsumed;
    }

// ---------------------------------------------------------
// CPhoneUIDisabledNote::OkToExitL
// ---------------------------------------------------------
//
TBool CPhoneUIDisabledNote::OkToExitL( TInt aCommand )
    {
    // Let the command observer process the command
    iCommandObserver.ProcessCommandL( aCommand );

    return EFalse; // so that the dialog will not disappear
    }
    
// ---------------------------------------------------------
// CPhoneUIDisabledNote::SetNoteType
// ---------------------------------------------------------
//
void CPhoneUIDisabledNote::SetNoteType( TPhoneNoteType aNoteType )
    {
    iNoteType = aNoteType;
    }

// ---------------------------------------------------------
// CPhoneUIDisabledNote::NoteType
// ---------------------------------------------------------
//
TPhoneNoteType CPhoneUIDisabledNote::NoteType()
    {
    return iNoteType;
    }

// ---------------------------------------------------------
// CPhoneUIDisabledNote::HandlePointerEventL
// ---------------------------------------------------------
//
void CPhoneUIDisabledNote::HandlePointerEventL( 
    const TPointerEvent& /* aPointerEvent */ )
    {
    }

// ---------------------------------------------------------
// CPhoneUIDisabledNote::UpdateSoftkeysL
// ---------------------------------------------------------
//
void CPhoneUIDisabledNote::UpdateSoftkeysL( TInt aResourceId )
    {
    CEikButtonGroupContainer& buttonGroup = ButtonGroupContainer();
    buttonGroup.SetCommandSetL( aResourceId );
    
    buttonGroup.DrawDeferred();
    }

// ---------------------------------------------------------
// CPhoneUIDisabledNote::SetSizeAndPosition
// ---------------------------------------------------------
//
void CPhoneUIDisabledNote::SetSizeAndPosition( const TSize& aSize )
    {
    CAknNoteDialog::SetSizeAndPosition( aSize );
    
	// Override the original layout position to prevent note to hide the call bubble
	// Will be changed when layout is available	
    TRect clientRect = CEikonEnv::Static()->EikAppUi()->ClientRect();
    TRect noteRect;
    TInt leftMargin = (TInt) ( (clientRect.Width() - aSize.iWidth) / 2);    // Set note to center

    noteRect.SetRect( leftMargin,                // iTl.iX
            clientRect.iBr.iY - aSize.iHeight,   // iTl.iY
            leftMargin + aSize.iWidth,           // iBr.iX
            clientRect.iBr.iY );                 // iBr.iY
    
    SetRect( noteRect );
    }

// End of File
