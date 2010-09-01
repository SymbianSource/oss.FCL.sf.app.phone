/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CT_VideoDtmfDialerAppView class.
*
*/

#include <coemain.h>
#include <aknutils.h> 
#include <AknLayout.cdl.h>
#include <LayoutMetaData.cdl.h>
#include <AknLayoutScalable_Apps.cdl.h>
#include <AknLayoutScalable_Avkon.cdl.h>
#include <eikenv.h>
#include <eikapp.h> 
#include <eikappui.h>
#include <AknAppUi.h>
#include <eikspane.h>

#include <mnumberentry.h>
#include <cdialer.h>
#include <cvideodtmfdialer.h>

#include "T_VideoDtmfDialerAppView.h"

const TUid KUidViewId = { 0x10282DDD };

// Standard construction sequence
CT_VideoDtmfDialerAppView* CT_VideoDtmfDialerAppView::NewL(const TRect& aRect)
    {
    CT_VideoDtmfDialerAppView* self = CT_VideoDtmfDialerAppView::NewLC(aRect);
    CleanupStack::Pop(self);
    return self;
    }

CT_VideoDtmfDialerAppView* CT_VideoDtmfDialerAppView::NewLC(const TRect& aRect)
    {
    CT_VideoDtmfDialerAppView* self = new (ELeave) CT_VideoDtmfDialerAppView();
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
    }

CT_VideoDtmfDialerAppView::CT_VideoDtmfDialerAppView() 
    {
    }

CT_VideoDtmfDialerAppView::~CT_VideoDtmfDialerAppView()
    {
	// no implementation required
    }

void CT_VideoDtmfDialerAppView::ConstructL(const TRect& aRect)
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect(aRect);

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::ViewId
// -----------------------------------------------------------------------------
//
TVwsViewId CT_VideoDtmfDialerAppView::ViewId() const
    {
    TVwsViewId id( 
        CEikonEnv::Static()->EikAppUi()->Application()->AppDllUid(), 
        KUidViewId );
    return id;
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::ViewActivatedL
// -----------------------------------------------------------------------------
//
void CT_VideoDtmfDialerAppView::ViewActivatedL( 
    const TVwsViewId& /*aPrevViewId*/, 
    TUid /*aCustomMessageId*/, 
    const TDesC8& /*aCustomMessage*/ )
    {
    MakeVisible( ETrue );
    SetFocus( IsFocused() );
    DrawDeferred();
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::ViewDeactivated
// -----------------------------------------------------------------------------
//
void CT_VideoDtmfDialerAppView::ViewDeactivated()
    {
    DrawDeferred();
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::CountComponentControls
// -----------------------------------------------------------------------------
//
TInt CT_VideoDtmfDialerAppView::CountComponentControls() const
    {
    TInt count = 0;

    if ( iControl )
        {
        count++;
        }
        
    return count;
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::ComponentControl
// -----------------------------------------------------------------------------
//
CCoeControl* CT_VideoDtmfDialerAppView::ComponentControl( TInt /*aIndex*/ ) const
    {
    CCoeControl* control( NULL );

    if ( iControl )
        {
        control = iControl;
        }

    return control;        
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::OfferKeyEventL
// -----------------------------------------------------------------------------
//    
TKeyResponse CT_VideoDtmfDialerAppView::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType)
    {
    TKeyResponse response = EKeyWasNotConsumed;
    if( iControl && iNumberEntry)
        {
        CCoeControl* numberEntryControl = iNumberEntry->GetNumberEntry();
        if( numberEntryControl )
            {
            response = numberEntryControl->OfferKeyEventL( aKeyEvent, aType );    
            }
        }
    return response;
    }
    
// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::SetControl
// -----------------------------------------------------------------------------
//    
void CT_VideoDtmfDialerAppView::SetControl( CCoeControl* aControl, TBool aDrawNow )
    {
    if ( iControl )
        {
        iControl->SetFocus( EFalse );
        iPrevControl = iControl;    
        }
    
    iControl = aControl;
    
    if ( iControl )
        {
        UpdateControlRect();
        
        if ( iPrevControl )
            {
            iPrevControl->MakeVisible( EFalse );        
            iPrevControl = NULL;
            }
        
        iControl->MakeVisible( ETrue );
        DrawNow();
        iControl->SetFocus( IsFocused() );
        }
    
    if ( aDrawNow )        
        {
        DrawNow();    
        }
    }
    
// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::Control
// -----------------------------------------------------------------------------
//    
CCoeControl* CT_VideoDtmfDialerAppView::Control() const
    {
    return iControl;    
    }    

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::SetNumberEntry
// -----------------------------------------------------------------------------
//    
void CT_VideoDtmfDialerAppView::SetNumberEntry( MNumberEntry* aNumberEntry )
    {
    iNumberEntry = aNumberEntry;
    }

void CT_VideoDtmfDialerAppView::SizeChanged()
    {
    UpdateControlRect();
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::UpdateControlRect
// -----------------------------------------------------------------------------
//
void CT_VideoDtmfDialerAppView::UpdateControlRect()
    {    
    if ( iControl )
        {
        iControl->SetRect( Rect() );
        }
    }
    
// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::ActivateL
// -----------------------------------------------------------------------------
//
void CT_VideoDtmfDialerAppView::ActivateL()
    {
    CCoeControl::ActivateL();
    }
    
// -----------------------------------------------------------------------------
// CT_VideoDtmfDialerAppView::Draw
// -----------------------------------------------------------------------------
//    
void CT_VideoDtmfDialerAppView::Draw(const TRect& aRect ) const
    {
    // Get the standard graphics context 
    CWindowGc& gc = SystemGc();
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.SetBrushColor( KRgbGreen );
    // Gets the control's extent
    // Clears the screen
    gc.Clear(aRect);
    }

// End of file 

