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
* Description: Implementation of TPhoneCmdParamGlobalNote class.
*
*/


// INCLUDE FILES
#include <avkon.hrh>
#include "tphonecmdparamglobalnote.h"
#include "phoneui.pan" 

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// TPhoneCmdParamGlobalNote::TPhoneCmdParamGlobalNote
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C TPhoneCmdParamGlobalNote::TPhoneCmdParamGlobalNote():
    TPhoneCommandParam(),
    iType( EAknGlobalErrorNote ),
    iTextResourceId( 0 ),
    iText( KNullDesC ),
    iTone( EAvkonSIDErrorTone ),
    iWaitforReady ( EFalse )
    ,iSoftkeys( 0 )
    {
    iParamId = EPhoneParamIdGlobalNote;
    }

// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::SetType
// Sets the global note type
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamGlobalNote::SetType( TAknGlobalNoteType aType )
   {
   iType = aType;   
   }

// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::SetTextResourceId
// Sets the global note resource ID
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamGlobalNote::SetTextResourceId( 
   TInt aTextResourceId )
   {
    __ASSERT_DEBUG( aTextResourceId, 
    	Panic( EPhoneUtilsParameterNotInitialized ) );
   iTextResourceId = aTextResourceId;   
   }

// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::SetText
// Sets the global note text
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamGlobalNote::SetText( 
   const TDesC& aText )
   {
   iText.Set( aText );
   }

// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::SetTone
// Sets the global note tone
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamGlobalNote::SetTone( TInt aTone )
   {
   iTone = aTone;   
   }

// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::SetWaitForReady
// Sets the note to wait for completed
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamGlobalNote::SetWaitForReady( TBool aStatus )
   {
   iWaitforReady = aStatus;   
   }

// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::SetSoftkeys
// Sets the global note softkeys
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamGlobalNote::SetSoftkeys( TInt aSoftkeys )
   {
   iSoftkeys = aSoftkeys;   
   }

// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::Type
// Returns the global note type
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TAknGlobalNoteType TPhoneCmdParamGlobalNote::Type() const
   {
   return iType;
   }

// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::TextResourceId
// Returns the global note text resource ID
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt TPhoneCmdParamGlobalNote::TextResourceId() const
   {
   return iTextResourceId;
   }

// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::Text
// Returns the global note text
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C const TDesC& TPhoneCmdParamGlobalNote::Text() const
   {
   return iText;
   }

// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::Tone
// Returns the note tone
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt TPhoneCmdParamGlobalNote::Tone() const
   {
   return iTone;
   }

// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::WaitForReady
// Returns the status of wait for completed
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TBool TPhoneCmdParamGlobalNote::WaitForReady() const
   {
   return iWaitforReady;
   }
   
// ---------------------------------------------------------
// TPhoneCmdParamGlobalNote::Softkeys
// Returns the note softkeys
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt TPhoneCmdParamGlobalNote::Softkeys() const
   {
   return iSoftkeys;
   }

//  End of File  
