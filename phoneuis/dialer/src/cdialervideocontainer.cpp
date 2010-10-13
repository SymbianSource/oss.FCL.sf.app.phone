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
* Description:  Manager for video keypad 
*
*/


// INCLUDE FILES
#include <eikenv.h>
#include <eikapp.h>
#include <gulicon.h>
#include <w32std.h>
#include <eikimage.h> // CEikImage
#include <barsread.h> // TResourceReader
#include <aknappui.h>
#include <AknUtils.h>
#include <AknsDrawUtils.h>
#include <AknLayoutFont.h>
#include <aknlayout2def.h>
#include <layoutmetadata.cdl.h>
#include <aknlayout2scalabledef.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <AknsBasicBackgroundControlContext.h>
#include <AknsFrameBackgroundControlContext.h>

#include "cdialervideocontainer.h"
#include "dialercommon.h"


// CONSTS
const TInt KNumberOfButtons = 0;    // Number of command buttons in container.
const TInt KVideoControlCount = KNumberOfButtons + 1; //  = Video window


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CDialerVideoContainer::NewL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
CDialerVideoContainer* CDialerVideoContainer::NewL( 
    const CCoeControl& aContainer,
    CCoeControl& aVideoWindow )
    {
    CDialerVideoContainer* self = 
        new( ELeave )CDialerVideoContainer( aContainer, aVideoWindow );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();    // self
    return self;
    }

// ---------------------------------------------------------------------------
// CDialerVideoContainer::ConstructL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
void CDialerVideoContainer::ConstructL()
    {    
    BaseConstructL();

    iVideoWindow.SetContainerWindowL( *this );
    iVideoWindow.SetParent( this );
    iVideoWindow.SetMopParent( this );

    ActivateL();
    }


// Constructor
CDialerVideoContainer::CDialerVideoContainer(
    const CCoeControl& aContainer, 
    CCoeControl& aVideoWindow )
    : CDialerContainerBase ( const_cast<CCoeControl&>(aContainer) ), 
      iVideoWindow ( aVideoWindow )
	{
	// Empty.
	}

// Destructor
CDialerVideoContainer::~CDialerVideoContainer()
    {
    // Empty.
    }

    
// Methods from base class

//
// Methods from CDialerContainerBase
//

// ---------------------------------------------------------------------------
// CDialerVideoContainer::SetFocus
//  
// ---------------------------------------------------------------------------
//
void CDialerVideoContainer::SetFocus( TBool aFocus, 
                                      TDrawNow /*aDrawNow*/ )
    {
    iVideoWindow.SetFocus( aFocus );
    }


// ---------------------------------------------------------------------------
// CDialerVideoContainer::SetVariety
// Set variety according to status
//  
// ---------------------------------------------------------------------------
//
void CDialerVideoContainer::SetVariety()
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        iVariety = EVideoVarietyLandscape;
        }
    else
        {
        iVariety = EVideoVarietyPortrait;
        }
    }

// ---------------------------------------------------------------------------
// CDialerVideoContainer::SetLayout
//  
// ---------------------------------------------------------------------------
//
void CDialerVideoContainer::SetLayout()
    {
    iVideoWindow.SetRect( Rect() );
    }


// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::CountComponentControls
//  
// ---------------------------------------------------------------------------
//
TInt CDialerVideoContainer::CountComponentControls() const
    {
    return KVideoControlCount;
    }

// ---------------------------------------------------------------------------
// CDialerVideoContainer::ComponentControl
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CDialerVideoContainer::ComponentControl( TInt aIndex ) const
    {
    __ASSERT_DEBUG( aIndex < KVideoControlCount, 
    User::Panic(_L("Dialer"), KErrArgument) );
    return &iVideoWindow;
    }

// ---------------------------------------------------------------------------
// CDialerVideoContainer::Draw
//  
// ---------------------------------------------------------------------------
//
void CDialerVideoContainer::Draw( const TRect& /*aRect */) const
    {
    CWindowGc& gc = SystemGc( );
    gc.SetBrushColor( AKN_LAF_COLOR( 0 /** White - get from layout */ ));
    TRect rect = Rect();
    
    // Draw the skin background of the parent 
    AknsDrawUtils::DrawBackground( AknsUtils::SkinInstance(), 
        AknsDrawUtils::ControlContextOfParent(this), this, gc, 
        rect.iTl, rect, KAknsDrawParamDefault );

    gc.SetBrushStyle( CGraphicsContext::ENullBrush );    
    }

// End of File
