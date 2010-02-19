/*
* Copyright (c) 2008 - 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The dialer view of PhoneUI
*
*/


// INCLUDE FILES
#include "phoneconstants.h"
#include "cphonedialerview.h"

#include <eikenv.h>
#include <eikapp.h> 
#include <eikappui.h>
#include <eikcmobs.h>

#include <w32std.h>          // iWindowSrvSession

#include <eikimage.h>
#include <coemain.h>
#include <gulutil.h>         // DrawUtils, egul.lib 
#include <aknappui.h>
#include <AknsLayeredBackgroundControlContext.h>
#include <AknsDrawUtils.h>
#include <AknsUtils.h>

#include "cphonestatuspane.h"
#include "tphonecmdparambitmap.h"
#include "phonelogger.h"

#include <AknUtils.h> // needed for AKN_LAF_COLOR in 3.0 environment


#include "phoneui.hrh"
#include "phoneviewcommanddefinitions.h"
// Scalable UI 
#include <aknlayout.cdl.h>
#include <layoutmetadata.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknStatuspaneUtils.h>

enum TPhoneBgLayers
    {
    EPhoneBgFirstLayer          = 0,
    EPhoneBgLayersN             = 1
    };

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPhoneDialerView::CPhoneDialerView
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneDialerView::CPhoneDialerView() :
    iSecurityMode ( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::ConstructL( TRect aRect )
    {
    // Create background control context for skins.
    iBgContext = CAknsLayeredBackgroundControlContext::NewL(
        KAknsIIDQsnBgScreen, aRect, ETrue /*Parent absolute*/,
        EPhoneBgLayersN );

    CreateWindowL();
    DrawableWindow()->SetPointerGrab( ETrue );

    SetComponentsToInheritVisibility( ETrue );
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneDialerView* CPhoneDialerView::NewL( TRect aRect )
    {
    CPhoneDialerView* self = new ( ELeave ) CPhoneDialerView();
    
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CPhoneDialerView::~CPhoneDialerView()
    {
     __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneDialerView::~CPhoneDialerView");
    delete iBgContext;
    iRegion.Close();
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::ViewId
// -----------------------------------------------------------------------------
//
TVwsViewId CPhoneDialerView::ViewId() const
    {
    TVwsViewId id( 
        iEikonEnv->EikAppUi()->Application()->AppDllUid(), 
        KUidViewId );
    return id;
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::ViewActivatedL
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::ViewActivatedL( 
    const TVwsViewId& /*aPrevViewId*/, 
    TUid /*aCustomMessageId*/, 
    const TDesC8& /*aCustomMessage*/ )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::ViewDeactivated
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::ViewDeactivated()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::SetControl
// 
// Change the control. 
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::SetControl( CCoeControl* aControl )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneDialerView::SetControl()");
    iControl = aControl;
    UpdateControlRect();
    iControl->MakeVisible( ETrue );
    }

    
// -----------------------------------------------------------------------------
// CPhoneDialerView::Control
// -----------------------------------------------------------------------------
//
CCoeControl* CPhoneDialerView::Control() const
    {
    return iControl;    
    }    
    
// -----------------------------------------------------------------------------
// CPhoneDialerView::ComponentControl
// -----------------------------------------------------------------------------
//
CCoeControl* CPhoneDialerView::ComponentControl( TInt /*aIndex*/ ) const
    {
    return iControl;        
    }
    
// -----------------------------------------------------------------------------
// CPhoneDialerView::CountComponentControls
// -----------------------------------------------------------------------------
//
TInt CPhoneDialerView::CountComponentControls() const
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneDialerView::CountComponentControls()");
    TInt count = 0;

    if ( iControl )
        {
        count = 1;
        }

    return count;
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::MopSupplyObject
// -----------------------------------------------------------------------------
//
TTypeUid::Ptr CPhoneDialerView::MopSupplyObject( TTypeUid aId )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneDialerView::MopSupplyObject()");
    // Required during rendering of the background skin in Draw()
    if (aId.iUid == MAknsControlContext::ETypeId)
        {
        return MAknsControlContext::SupplyMopObject( aId, iBgContext );
        }
    return CCoeControl::MopSupplyObject( aId );
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::SetRect
// -----------------------------------------------------------------------------
//    
void CPhoneDialerView::SetRect( const TRect& aRect )
    {
    CCoeControl::SetRect( aRect );
    iControlRect = Rect();
    // Forward size change to the control only if we are visible. Otherwise the
    // control rect is just srored and relayed to control when we do become
    // visible. This way we can avoid unnecessary relayout work for example when
    // status bar size changes or screen is rotated while in-call ui is active.
    if ( IsVisible() )
        {
        UpdateControlRect();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::Draw
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::Draw( const TRect& aRect ) const
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneDialerView::Draw()");
    TRect rect = Rect();

    if ( !rect.iTl.iY )
        {
        CWindowGc& gc = SystemGc();
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );
        
        if ( iSecurityMode || 
            !AknsDrawUtils::Background( skin, cc, this, gc, aRect ) )
            {
            gc.SetPenStyle(CGraphicsContext::ENullPen);
            gc.SetBrushColor( AKN_LAF_COLOR( 0 ) ); // 0 for white
            gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
            gc.DrawRect( aRect ); 
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::HandlePointerEventL
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    // Calling base class implementation
    CCoeControl::HandlePointerEventL( aPointerEvent );    
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::ActivateL
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::ActivateL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneDialerView::ActivateL()");
    CCoeControl::ActivateL();
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::SizeChanged()
// 
// Expects that the controls are initialised otherwise.
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::SizeChanged()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneDialerView::SizeChanged()");
    AknsUtils::RegisterControlPosition( this );

    TRect screen;
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EScreen, screen );    
    iBgContext->SetLayerImage( EPhoneBgFirstLayer, KAknsIIDQsnBgScreen );
    iBgContext->SetParentPos( screen.iTl );
    iBgContext->SetLayerRect( EPhoneBgFirstLayer, screen ) ;
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::PositionChanged
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::PositionChanged()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneDialerView::PositionChanged()");
    AknsUtils::RegisterControlPosition( this );
    }
    
// -----------------------------------------------------------------------------
// CPhoneDialerView::FocusChanged
// -----------------------------------------------------------------------------
//    
void CPhoneDialerView::FocusChanged( TDrawNow /*aDrawNow*/ )
    {
    iControl->SetFocus( IsFocused () );
    }    

// -----------------------------------------------------------------------------
// CPhoneDialerView::UpdateControlRect
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::UpdateControlRect()
    {    
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneDialerView::UpdateControlRect()");
    if ( iControl )
        {
        if ( !AknLayoutUtils::PenEnabled() )
            {
            if ( Rect() != iControl->Rect() )
                {
                iControl->SetRect( Rect() );
                }    
            }
        else
            {
            if ( iControlRect != iControl->Rect() )
                {
                iControl->SetRect( iControlRect );
                }
            } 
        }
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::HandleResourceChangeL
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::HandleResourceChange( TInt aType )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneDialerView::HandleResourceChange()");

    if ( aType == KAknsMessageSkinChange || 
         aType == KEikDynamicLayoutVariantSwitch )
        {
        DrawDeferred();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::MakeVisible
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::MakeVisible( TBool aVisible )
    {
    if ( aVisible )
        {
        // Deferred control rect size update before view becomes visible.
        // We don't relay size changes to our control while view is invisible
        // to avoid unnecessary layout recalculations.
        UpdateControlRect();
        }
    CCoeControl::MakeVisible( aVisible );
    }

// -----------------------------------------------------------------------------
// CPhoneDialerView::SetSecurityMode
// -----------------------------------------------------------------------------
//
void CPhoneDialerView::SetSecurityMode ( TBool aStatus )
    {
    if ( iSecurityMode != aStatus ) 
        {
        iSecurityMode = aStatus;
        SizeChanged();    
        }
    }
    
// -----------------------------------------------------------------------------
// CPhoneDialerView::IsSecurityMode
// -----------------------------------------------------------------------------
//
TBool CPhoneDialerView::IsSecurityMode()    
    {
    return iSecurityMode;
    }

// End of File
