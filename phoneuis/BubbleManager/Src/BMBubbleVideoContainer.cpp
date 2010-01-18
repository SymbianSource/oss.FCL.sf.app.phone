/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Video Ringing Tone / Video window (DSA)
*
*/



// INCLUDE FILES
#include "BMBubbleVideoContainer.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CBubbleVideoContainer::CBubbleVideoContainer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CBubbleVideoContainer::CBubbleVideoContainer( 
    const CCoeControl* aParent )
    : iParent( aParent )
    {
    }

// -----------------------------------------------------------------------------
// CBubbleVideoContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CBubbleVideoContainer::ConstructL( )
    {
    CreateWindowL( iParent );
    Window().SetNonFading( ETrue );
    }

// -----------------------------------------------------------------------------
// CBubbleVideoContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CBubbleVideoContainer* CBubbleVideoContainer::NewL(
    const CCoeControl* aParent )
    {
    CBubbleVideoContainer* self = new( ELeave ) 
        CBubbleVideoContainer( aParent );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    
    return self;
    }

    
// Destructor
CBubbleVideoContainer::~CBubbleVideoContainer()
    {
    
    }


// -----------------------------------------------------------------------------
// CBubbleVideoContainer::Draw
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CBubbleVideoContainer::Draw( const TRect& aRect ) const
    {
    if ( Window().DisplayMode() == EColor16MA )
        {
        CWindowGc& gc = SystemGc();
        gc.SetDrawMode( CGraphicsContext::EDrawModeWriteAlpha );
  	    gc.SetBrushColor( TRgb::Color16MA( 0 ) ); // alpha 0x00 shows video
  	    gc.Clear( aRect );
        }
    else if ( !iRedrawDisabled )
        {
        CWindowGc& gc = SystemGc();
        // Draw background frame graphics
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        AknsDrawUtils::DrawFrame( skin,
                                  gc,  
                                  iFrameOuterRect,
                                  iFrameInnerRect,
                                  iFrameId,
                                  KAknsIIDDefault );
        }
    }
    
// -----------------------------------------------------------------------------
// CBubbleVideoContainer::SizeChanged
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CBubbleVideoContainer::SizeChanged()
    {
    // NOP
    } 
    
// -----------------------------------------------------------------------------
// CBubbleVideoContainer::GetWindow
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RWindow& CBubbleVideoContainer::GetWindow() const
    {
    return Window();
    }

// -----------------------------------------------------------------------------
// CBubbleVideoContainer::SetBackgroundFrame
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CBubbleVideoContainer::SetBackgroundFrame( const TAknsItemID& aFrameId,
                                                TRect& aOuterRect,
                                                TRect& aInnerRect )
    {
    iFrameId = aFrameId;
    iFrameOuterRect = aOuterRect;
    iFrameInnerRect = aInnerRect;    
    }
    
// -----------------------------------------------------------------------------
// CBubbleVideoContainer::SetRedrawDisabled
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CBubbleVideoContainer::SetRedrawDisabled( TBool aDisabled )
    {
    iRedrawDisabled = aDisabled;    
    }
        
    
//  End of File
