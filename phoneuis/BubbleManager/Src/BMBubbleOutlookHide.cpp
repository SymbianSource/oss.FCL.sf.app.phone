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
* Description:  Outlook Hide
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookHide.h"
#include    "BMBubbleImageManager.h"
#include    "BMResourceManager.h"
#include    "BMBubbleHeader.h"
#include    "BMLayout.h"
#include    "BMUtils.h"

#include    <eiklabel.h> 
#include    <eikimage.h>
#include    <AknsUtils.h>

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CBubbleOutlookHide::CBubbleOutlookHide
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookHide::CBubbleOutlookHide( CBubbleManager& aBubbleManager) 
: CBubbleOutlook( aBubbleManager )
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookHide::ConstructL
// 
// ---------------------------------------------------------------------------
//

void CBubbleOutlookHide::ConstructL()
    {     
    CBubbleOutlook::ConstructL( );
    ActivateL();
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookHide::~CBubbleOutlookHide
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookHide::~CBubbleOutlookHide()
    {
    Reset();
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookHide::Reset
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookHide::Reset()
    {
    iBubbleManager.ResourceManager().ReleaseEikImage( iBubble );
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookHide::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookHide::ReadBubbleHeader( CBubbleHeader& /*aHeader*/ )
    {
    iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );
    iBubbleManager.ImageManager().SetBitmapToImage( 
        iBubble , 
        EQgn_graf_call_hidden_held , 
        EQgn_graf_call_hidden_held_mask );
    SizeChanged();
    }



// ---------------------------------------------------------------------------
// CBubbleOutlookHide::SizeChanged
// called by framwork when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookHide::SizeChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    BubbleUtils::LayoutBackgroundImage( 
        iBubble , 
        Rect(), 
        BubbleLayout::popup_call_audio_first_window_6_graphics_1() );
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookHide::DrawTimerCostNow
//
//  Pure virtual in base class.
// ---------------------------------------------------------------------------
//
void CBubbleOutlookHide::DrawTimerCostNow()
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookHide::DrawCLINow
//
//  Pure virtual in base class.
// ---------------------------------------------------------------------------
//
void CBubbleOutlookHide::DrawCLINow()
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookHide::DrawBitmaps
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookHide::DrawBitmaps( CBitmapContext& aGc ) const
    {
    aGc.SetBrushColor( AKN_LAF_COLOR( BubbleLayout::LayoutColourWhite() ) );
    BubbleUtils::DrawMaskedImage( aGc , iBubble );    
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookHide::HandleAnimationStartL
//
//  Pure virtual in base class.
// ---------------------------------------------------------------------------
//
void CBubbleOutlookHide::HandleAnimationStartL() const
    {
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlookHide::DrawCallHeaderText
// ---------------------------------------------------------------------------
//
void CBubbleOutlookHide::DrawCallHeaderText()
    {
    }


// End of File
