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
* Description:  Implementation of CVideoDTMFDialer class, stub.
*
*/


// INCLUDE FILES
#include "cvideodtmfdialer.h"

#include <aknenv.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::NewL
//
// Symbian OS two phased constructor for video dtmf mode
// ---------------------------------------------------------------------------
//
EXPORT_C CVideoDTMFDialer* CVideoDTMFDialer::NewL( const CCoeControl& aContainer, 
                                 CCoeControl& aVideoWindow, 
                                 const TRect& aRect )
    {
    CVideoDTMFDialer* self = new( ELeave )CVideoDTMFDialer ( );    
    CleanupStack::PushL( self );
    self->ConstructL( aContainer , aVideoWindow, aRect );
    CleanupStack::Pop();    // self
    return self;
    }

// Destructor
EXPORT_C CVideoDTMFDialer::~CVideoDTMFDialer()
    {
    // Empty
    }
    
// ---------------------------------------------------------------------------
// CVideoDTMFDialer::ConstructL
//
// Symbian OS two phased constructor for video telephony mode.
// ---------------------------------------------------------------------------
//
void CVideoDTMFDialer::ConstructL( 
    const CCoeControl& aContainer, 
    CCoeControl& /*aVideoWindow*/,
    const TRect& aRect )
    {
    // set window
    SetContainerWindowL( aContainer );
    SetParent( const_cast<CCoeControl*>(&aContainer) );
    iParentControl = &aContainer;
    SetRect( aRect );
    }

// Constructor
CVideoDTMFDialer::CVideoDTMFDialer( ) :
    iEikEnv( *CEikonEnv::Static() )
    {
    // Empty    
    }
    

// Functions from CCoeControl

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::SizeChanged
// 
// Called by framework when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CVideoDTMFDialer::SizeChanged()
    {
    // Empty
    }

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::PositionChanged
// ---------------------------------------------------------------------------
//
void CVideoDTMFDialer::PositionChanged()
    {
    // Empty
    }

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::CountComponentControls
//
// Returns contained controls in phone dialer mode or in video dtmf mode.
// ---------------------------------------------------------------------------
//
TInt CVideoDTMFDialer::CountComponentControls() const
    {
    return NULL;
    }

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::ComponentControl(TInt aIndex) const
//
// Returns contained control by given index.
// ---------------------------------------------------------------------------
//
CCoeControl* CVideoDTMFDialer::ComponentControl( TInt /*aIndex*/ ) const
    {
    return NULL;
    } 

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::Draw
// ---------------------------------------------------------------------------
//
void CVideoDTMFDialer::Draw( const TRect& /*aRect*/ ) const
    {
    // Empty
    }

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::HandleResourceChange
//  
// ---------------------------------------------------------------------------
//
void CVideoDTMFDialer::HandleResourceChange( TInt /*aType*/ )
    {
    // Empty
    }

// -----------------------------------------------------------------------------
// CVideoDTMFDialer::HandlePointerEventL
//
// -----------------------------------------------------------------------------
//
void CVideoDTMFDialer::HandlePointerEventL( const TPointerEvent& /*aPointerEvent*/ )
    {
    // Empty  
    }

// Private methods

// -----------------------------------------------------------------------------
// CVideoDTMFDialer::LoadResourceL
//
// -----------------------------------------------------------------------------
//
void CVideoDTMFDialer::LoadResourceL()
    {
    // Empty
    }

// -----------------------------------------------------------------------------
// CVideoDTMFDialer::UnLoadResources
//
// -----------------------------------------------------------------------------
//    
void CVideoDTMFDialer::UnLoadResources()
    {
    // Empty
    }


// ---------------------------------------------------------------------------
// CVideoDTMFDialer::ComponentControlForVideoMode
// 
// Returns contained control by given index in video DTMF mode.
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CVideoDTMFDialer::ComponentControlForVideoMode( const TInt /*aIndex*/ ) const
    {
    return NULL;
    } 

// End of File
