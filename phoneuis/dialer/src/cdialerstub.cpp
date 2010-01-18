/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of dialer stub.
*
*/


// INCLUDE FILES
#include "cdialer.h"

#include <aknenv.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CDialer::NewL
//
// Symbian OS two phased constructor for telephony phone dialer mode
// ---------------------------------------------------------------------------
//
EXPORT_C CDialer* CDialer::NewL( const CCoeControl& aContainer, 
                                 const TRect& aRect )
    {
    CDialer* self = new( ELeave )CDialer ( );    
    CleanupStack::PushL( self );
    self->ConstructL( aContainer , aRect );
    CleanupStack::Pop();    // self
    return self;
    }

// Destructor
EXPORT_C CDialer::~CDialer()
    {
    // Empty
    }

// ---------------------------------------------------------------------------
// CDialer::ConstructL
//
// Symbian OS two phased constructor for phone dialer more.
// ---------------------------------------------------------------------------
//
void CDialer::ConstructL( 
    const CCoeControl& aContainer, 
    const TRect& aRect )
    {    
    // set window
    SetContainerWindowL( aContainer );
    SetParent( const_cast<CCoeControl*>(&aContainer) );
    iParentControl = &aContainer;
    SetRect( aRect );
    }

// Constructor
CDialer::CDialer( ) :
    iEikEnv( *CEikonEnv::Static() )
    {
    // Empty    
    }

// -----------------------------------------------------------------------------
// CDialer::NumberEntry
//
// Return pointer to MNumberEntry interface.
// -----------------------------------------------------------------------------
//
EXPORT_C MNumberEntry* CDialer::NumberEntry()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// CDialer::SetNumberEntryObserver
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CDialer::SetNumberEntryObserver( MNumberEntryObserver& /*aObserver*/ ) 
    {
    // Empty
    }


// Methods from MNumberEntry

// ---------------------------------------------------------------------------
// CDialer::CreateNumberEntry
//  
// Creates number entry in phone dialer mode. In video DTMF mode
// only sets CBA.
// ---------------------------------------------------------------------------
//
void CDialer::CreateNumberEntry()
    {
    // Empty
    }
    
    
// ---------------------------------------------------------------------------
// CDialer::GetNumberEntry
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CDialer::GetNumberEntry( ) const
    {
    return NULL;
    }
    
// ---------------------------------------------------------------------------
// CDialer::IsNumberEntryUsed
//  
// ---------------------------------------------------------------------------
//
TBool CDialer::IsNumberEntryUsed( ) const
    {
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// CDialer::SetNumberEntryVisible
//  
// ---------------------------------------------------------------------------
//
void CDialer::SetNumberEntryVisible( const TBool& /*aVisibility */)
    {
    // Empty
    }

    
// ---------------------------------------------------------------------------
// CDialer::SetTextToNumberEntry
//  
// ---------------------------------------------------------------------------
//
void CDialer::SetTextToNumberEntry( const TDesC& /*aDesC */)
    {
    // Empty
    }
    
// ---------------------------------------------------------------------------
// CDialer::GetTextFromNumberEntry
//  
// ---------------------------------------------------------------------------
//
void CDialer::GetTextFromNumberEntry( TDes& aDesC )
    {
    aDesC = KNullDesC();
    }
    
// ---------------------------------------------------------------------------
// CDialer::RemoveNumberEntry
//  
// ---------------------------------------------------------------------------
//
void CDialer::RemoveNumberEntry( )
    {
    iIsUsed = EFalse;    
    }
    
// ---------------------------------------------------------------------------
// CDialer::ChangeEditorMode
//  
// ---------------------------------------------------------------------------
//
TInt CDialer::ChangeEditorMode( TBool /*aDefaultMode */)
    {
    return KErrNotSupported;
    }
    
// ---------------------------------------------------------------------------
// CDialer::OpenVKBL
//  
// ---------------------------------------------------------------------------
//
void CDialer::OpenVkbL()
    {
    // Empty
    }
    
// ---------------------------------------------------------------------------
// CDialer::GetEditorMode
//  
// ---------------------------------------------------------------------------
//
TInt CDialer::GetEditorMode() const
    {
    return KErrNotSupported;
    }
    
// ---------------------------------------------------------------------------
// CDialer::ResetEditorToDefaultValues
//  
// ---------------------------------------------------------------------------
//
void CDialer::ResetEditorToDefaultValues()
    {
    // Empty
    }
    
// ---------------------------------------------------------------------------
// CDialer::SetNumberEntryPromptText
//  
// ---------------------------------------------------------------------------
//    
void CDialer::SetNumberEntryPromptText( const TDesC& /*aPromptText*/ ) 
    {
    // Empty
    }    

// ---------------------------------------------------------------------------
// CDialer::EnableTactileFeedback
//  
// ---------------------------------------------------------------------------
//    
void CDialer::EnableTactileFeedback( const TBool /*aEnable*/ )
    {
    // Empty
    }
// Functions from CCoeControl

// ---------------------------------------------------------------------------
// CDialer::SizeChanged
// 
// Called by framework when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CDialer::SizeChanged()
    {
    // Empty
    }

// ---------------------------------------------------------------------------
// CDialer::PositionChanged
// ---------------------------------------------------------------------------
//
void CDialer::PositionChanged()
    {
    // Empty
    }

// ---------------------------------------------------------------------------
// CDialer::CountComponentControls
//
// Returns contained controls in phone dialer mode or in video dtmf mode.
// ---------------------------------------------------------------------------
//
TInt CDialer::CountComponentControls() const
    {
    return 0;
    }


// ---------------------------------------------------------------------------
// CDialer::ComponentControl(TInt aIndex) const
//
// Returns contained control by given index.
// ---------------------------------------------------------------------------
//
CCoeControl* CDialer::ComponentControl( TInt /*aIndex*/ ) const
    {
    return NULL;
    } 

// ---------------------------------------------------------------------------
// CDialer::Draw
// ---------------------------------------------------------------------------
//
void CDialer::Draw( const TRect& /*aRect*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CDialer::FocusChanged
// ---------------------------------------------------------------------------
//
void CDialer::FocusChanged(TDrawNow /*aDrawNow*/)
    {
    }

// ---------------------------------------------------------------------------
// CDialer::PrepareForFocusGainL
//  
// ---------------------------------------------------------------------------
//
void CDialer::PrepareForFocusGainL( )
    {
    }

// ---------------------------------------------------------------------------
// CDialer::HandleResourceChange
//  
// ---------------------------------------------------------------------------
//
void CDialer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    }

// -----------------------------------------------------------------------------
// CDialer::HandlePointerEventL
//
// -----------------------------------------------------------------------------
//
void CDialer::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    // Calling base class implementation
    CCoeControl::HandlePointerEventL(aPointerEvent);    
    }
    
// ---------------------------------------------------------------------------
// CDialer::MakeVisible
// ---------------------------------------------------------------------------
//
void CDialer::MakeVisible( TBool /*aVisible*/ )
	{
	}    

// Private methods


// -----------------------------------------------------------------------------
// CDialer::LoadResourceL
//
// -----------------------------------------------------------------------------
//
void CDialer::LoadResourceL()
    {
    // Empty
    }

// -----------------------------------------------------------------------------
// CDialer::UnLoadResources
//
// -----------------------------------------------------------------------------
//    
void CDialer::UnLoadResources()
    {
    // Empty
    }
    
// ---------------------------------------------------------------------------
// CDialer::ComponentControlForDialerMode
// 
// Returns contained control by given index in ohonedialer mode.
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CDialer::ComponentControlForDialerMode( const TInt /*aIndex*/ ) const
    {
    return NULL;
    }

// ---------------------------------------------------------------------------
// CDialer::EdwinState
// 
// Returns edwin state of the editor.
//  
// ---------------------------------------------------------------------------
//
CAknEdwinState* CDialer::EdwinState()
    {
    return NULL;
    } 

// ---------------------------------------------------------------------------
// CDialer::UpdateVkbEditorFlagsL
//  
// ---------------------------------------------------------------------------
//
void CDialer::UpdateVkbEditorFlagsL()
    {
    // Empty
    }

// End of File
