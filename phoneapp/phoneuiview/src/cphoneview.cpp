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
* Description:  The application view of PhoneUI
*
*/


// INCLUDE FILES
#include "phoneconstants.h"
#include "cphoneview.h"

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
#include "mphoneviewobserver.h" 

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
// CPhoneView::CPhoneView
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneView::CPhoneView( CEikButtonGroupContainer& aCba ) :
    iCba ( aCba )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneView::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneView::ConstructL( TRect aRect )
    {
    // Create background control context for skins.
    iBgContext = CAknsLayeredBackgroundControlContext::NewL(
        KAknsIIDWallpaper, aRect, ETrue /*Parent absolute*/,
        EPhoneBgLayersN );

    // Create bitmap
    iBitmap = new (ELeave) CFbsBitmap;
    iBitmap->Reset();

    CreateWindowL();
    DrawableWindow()->SetPointerGrab( ETrue );

    SetRect( aRect );
    }

// -----------------------------------------------------------------------------
// CPhoneView::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneView* CPhoneView::NewL( TRect aRect, CEikButtonGroupContainer& aCba )
    {
    CPhoneView* self = new ( ELeave ) CPhoneView( aCba );
    
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CPhoneView::~CPhoneView()
    {
     __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::~CPhoneView");
    delete iBgContext;
    delete iBitmap;
    iRegion.Close();
    }

// -----------------------------------------------------------------------------
// CPhoneView::ViewId
// -----------------------------------------------------------------------------
//
TVwsViewId CPhoneView::ViewId() const
    {
    TVwsViewId id( 
        iEikonEnv->EikAppUi()->Application()->AppDllUid(), 
        KUidViewId );
    return id;
    }

// -----------------------------------------------------------------------------
// CPhoneView::ViewActivatedL
// -----------------------------------------------------------------------------
//
void CPhoneView::ViewActivatedL( 
    const TVwsViewId& /*aPrevViewId*/, 
    TUid aCustomMessageId, 
    const TDesC8& /*aCustomMessage*/ )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::ViewActivatedL()");
    switch ( aCustomMessageId.iUid )
        {
        case KTouchDiallerViewCommand:
            {
            if ( iDialerActivation )
                {
                if ( iSecurityMode )
                    {
                    static_cast<MEikCommandObserver*>( iEikonEnv->EikAppUi() )
						->ProcessCommandL( EPhoneViewOpenNumberEntry );
                    return;
                    }
                 else
                    {
                    iEikonEnv->WsSession().ClearAllRedrawStores();
                    SetActivatePreviousApp( ETrue );            
                    static_cast<MEikCommandObserver*>( iEikonEnv->EikAppUi() )
                    ->ProcessCommandL( EPhoneViewOpenNumberEntry );
                    } 
                }
            }
            break;
        case KTouchCallHandlingVievCommand:
            {
            iEikonEnv->WsSession().ClearAllRedrawStores();
            SetActivatePreviousApp( EFalse );
            static_cast<MEikCommandObserver*>( iEikonEnv->EikAppUi() )
                ->ProcessCommandL( EPhoneViewOpenCallHandling );  
            }
            break;
        default:
            break;
        }
    
    if ( iObserver )
        {  
        iObserver->PhoneViewActivatedL();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneView::ViewDeactivated
// -----------------------------------------------------------------------------
//
void CPhoneView::ViewDeactivated()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::ViewDeactivated()");
    SetActivatePreviousApp( EFalse );
  
    DrawDeferred();
    }

// -----------------------------------------------------------------------------
// CPhoneView::SetControl
// 
// Change the control. 
// -----------------------------------------------------------------------------
//
void CPhoneView::SetControl( CCoeControl* aControl )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::SetControl()");
    iControl = aControl;
    UpdateControlRect();
    iControl->MakeVisible( ETrue );
    }
    
// -----------------------------------------------------------------------------
// CPhoneView::Control
// -----------------------------------------------------------------------------
//
CCoeControl* CPhoneView::Control() const
    {
    return iControl;    
    }    
   
// -----------------------------------------------------------------------------
// CPhoneView::ComponentControl
// -----------------------------------------------------------------------------
//
CCoeControl* CPhoneView::ComponentControl( TInt /*aIndex*/ ) const
    {
    return iControl;        
    }
    
// -----------------------------------------------------------------------------
// CPhoneView::CountComponentControls
// -----------------------------------------------------------------------------
//
TInt CPhoneView::CountComponentControls() const
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::CountComponentControls()");
    TInt count = 0;

    if ( iControl )
        {
        count = 1;
        }

    return count;
    }

// -----------------------------------------------------------------------------
// CPhoneView::SetBitmap
// -----------------------------------------------------------------------------
//
void CPhoneView::SetBitmap( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::SetBitmap()");
    TPhoneCmdParamBitmap* bitmapParam = static_cast<TPhoneCmdParamBitmap*>(
        aCommandParam );
    
    if ( !iSecurityMode )
        {
        TInt err = KErrNone;
        if ( bitmapParam->Bitmap()->Handle() )
            {
            err = iBitmap->Duplicate( bitmapParam->Bitmap()->Handle() );
            }
        else
            {
            iBitmap->Reset();
            }
            
        if ( err == KErrNone )
            {
            DrawDeferred();
            }    
        }
    }

// -----------------------------------------------------------------------------
// CPhoneView::GetBitmap
// -----------------------------------------------------------------------------
//
void CPhoneView::GetBitmap( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::GetBitmap()");
    TPhoneCmdParamBitmap* bitmapParam = static_cast<TPhoneCmdParamBitmap*>(
        aCommandParam );

    bitmapParam->SetBitmap( iBitmap );
    }

// -----------------------------------------------------------------------------
// CPhoneView::MopSupplyObject
// -----------------------------------------------------------------------------
//
TTypeUid::Ptr CPhoneView::MopSupplyObject( TTypeUid aId )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::MopSupplyObject()");
    // Required during rendering of the background skin in Draw()
    if (aId.iUid == MAknsControlContext::ETypeId)
        {
        return MAknsControlContext::SupplyMopObject( aId, iBgContext );
        }
    return CCoeControl::MopSupplyObject( aId );
    }

// -----------------------------------------------------------------------------
// CPhoneView::SetRect
// -----------------------------------------------------------------------------
//    
void CPhoneView::SetRect( const TRect &aRect )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::SetRect()");
    if ( !AknLayoutUtils::PenEnabled() )
        {
        CCoeControl::SetRect( aRect );
        }
    else
        {
        if ( Layout_Meta_Data::IsLandscapeOrientation() )
            {
            // In landscape and in security mode aRect is ok.
            CCoeControl::SetRect( aRect );    
            }
        else
            {     
            TRect controlRect( aRect );
            TAknLayoutRect viewRect;
            
            TRect screenRect;
            AknLayoutUtils::LayoutMetricsRect( 
                AknLayoutUtils::EScreen, 
                screenRect  );    
            
            viewRect.LayoutRect( 
                screenRect, 
                TAknWindowComponentLayout::Compose(
                    AknLayoutScalable_Avkon::application_window( 0 ),
                    AknLayoutScalable_Avkon::main_pane( 8 ) ) );   
            
            // reduce toolbar
            TAknLayoutRect toolbarRect;
            toolbarRect.LayoutRect( 
                screenRect, 
                TAknWindowComponentLayout::Compose(
                AknLayoutScalable_Avkon::application_window( 0 ),
                AknLayoutScalable_Avkon::popup_toolbar2_fixed_window( 0 ) ) );
            
            if ( toolbarRect.Rect().Intersects( controlRect )  )
                {
                __PHONELOG( EBasic, EPhoneUIView,"CPhoneView::SetRect Update ");            
                controlRect.iBr.iY -= toolbarRect.Rect().Height();
                }            
                     
            CCoeControl::SetRect( controlRect );
            }                
        }        
    }

// -----------------------------------------------------------------------------
// CPhoneView::Draw
// -----------------------------------------------------------------------------
//
void CPhoneView::Draw(  const TRect& aRect ) const
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::Draw()");
    TRect rect = Rect();

    if ( !rect.iTl.iY )
        {
        CWindowGc& gc = SystemGc();

        // Draw main area.
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );
        
        if ( !AknsDrawUtils::Background( skin, cc, this, gc, aRect ) )
            {
            gc.SetPenStyle(CGraphicsContext::ENullPen);
            gc.SetBrushColor( AKN_LAF_COLOR( 0 ) ); // 0 for white
            gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
            gc.DrawRect( aRect ); 
            }

        // Frozen Active Idle background image.
        // TBD: set member variable from states to prevent display of
        // image if emergency or security view
        else if ( iBitmap && ( iBitmap->Handle() != 0 ) ) 
            {
            gc.BitBlt( TPoint( 0, 0 ), iBitmap );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneView::HandlePointerEventL
// -----------------------------------------------------------------------------
//
void CPhoneView::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    if ( CapturesPointer() )
        {
        // Point Capture enabled 
        // Modify position of pointerevent so that CBA gets valid data.
        TPointerEvent pointerEvent = aPointerEvent; 
        TPoint phoneviewPoint = PositionRelativeToScreen();
        TPoint buttonGroupPoint =  iCba.ButtonGroup()->AsControl()->PositionRelativeToScreen();
        
        TPoint screenPoint = buttonGroupPoint-phoneviewPoint; 
        
        TPoint styluspoint =  aPointerEvent.iPosition;
        TPoint currentpointer = styluspoint - screenPoint;

        pointerEvent.iPosition = currentpointer;
        iCba.ButtonGroup()->AsControl()->HandlePointerEventL( pointerEvent );
        }
    // Calling base class implementation
    CCoeControl::HandlePointerEventL(aPointerEvent);    
    }

// -----------------------------------------------------------------------------
// CPhoneView::ActivateL
// -----------------------------------------------------------------------------
//
void CPhoneView::ActivateL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::ActivateL()");
    CCoeControl::ActivateL();
    }

// -----------------------------------------------------------------------------
// CPhoneView::SizeChanged()
// 
// Expects that the controls are initialised otherwise.
// -----------------------------------------------------------------------------
//
void CPhoneView::SizeChanged()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::SizeChanged()");
    AknsUtils::RegisterControlPosition( this );

    TRect screen;
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EScreen, screen );    
    iBgContext->SetLayerImage( EPhoneBgFirstLayer, KAknsIIDWallpaper );
    iBgContext->SetParentPos( screen.iTl );
    iBgContext->SetLayerRect( EPhoneBgFirstLayer, screen );            

    UpdateControlRect();
    }

// -----------------------------------------------------------------------------
// CPhoneView::PositionChanged
// -----------------------------------------------------------------------------
//
void CPhoneView::PositionChanged()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::PositionChanged()");
    AknsUtils::RegisterControlPosition( this );
    }
    
// -----------------------------------------------------------------------------
// CPhoneView::FocusChanged
// -----------------------------------------------------------------------------
//    
void CPhoneView::FocusChanged( TDrawNow /*aDrawNow*/ )
    {
    iControl->SetFocus( IsFocused () );
    }    

// -----------------------------------------------------------------------------
// CPhoneView::UpdateControlRect
// -----------------------------------------------------------------------------
//
void CPhoneView::UpdateControlRect()
    {    
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::UpdateControlRect()");
    if ( iControl )
        {
        if ( iControl->Rect() != Rect() )
            {
            iControl->SetRect( Rect() );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneView::HandleResourceChangeL
// -----------------------------------------------------------------------------
//
void CPhoneView::HandleResourceChange( TInt aType )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneView::HandleResourceChange()");

    if ( aType == KAknsMessageSkinChange || 
         aType == KEikDynamicLayoutVariantSwitch )
        {
        DrawDeferred();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneView::SetSecurityMode
// -----------------------------------------------------------------------------
//
void CPhoneView::SetSecurityMode ( TBool aStatus )
    {
    iSecurityMode = aStatus;
    }
    
// -----------------------------------------------------------------------------
// CPhoneView::GetActivatePreviousApp
// -----------------------------------------------------------------------------
//
TBool CPhoneView::GetActivatePreviousApp()
    {
    return iActivatePreviousApp;       
    }

// -----------------------------------------------------------------------------
// CPhoneView::SetActivatePreviousApp
// -----------------------------------------------------------------------------
//
void CPhoneView::SetActivatePreviousApp( const TBool aStatus )
    {
    __PHONELOG1( EBasic, 
        EPhoneUIView,"CPhoneView::SetActivatePreviousApp() aStatus %d", 
        aStatus );
    iActivatePreviousApp = aStatus;        
    }
 
// -----------------------------------------------------------------------------
// CPhoneView::SetPhoneAppViewToDialer
// -----------------------------------------------------------------------------
//
void CPhoneView::SetPhoneAppViewToDialer( const TBool aStatus )
    {
    __PHONELOG1( EBasic, 
        EPhoneUIView,"CPhoneView::SetPhoneAppViewToDialer() aStatus %d", 
        aStatus );
    iPhoneAppViewToDialer = aStatus;
    }
// -----------------------------------------------------------------------------
// CPhoneView::CapturePointerEvents
// -----------------------------------------------------------------------------
//
void CPhoneView::CapturePointerEvents( const TBool aCapture )
    {
    SetPointerCapture( aCapture );
    }

// -----------------------------------------------------------------------------
// CPhoneView::PhoneAppViewToDialer
// -----------------------------------------------------------------------------
//
TBool CPhoneView::PhoneAppViewToDialer()
    {
    __PHONELOG1( EBasic, 
        EPhoneUIView,"CPhoneView::PhoneAppViewToDialer() iPhoneAppViewToDialer %d", 
        iPhoneAppViewToDialer );
    return iPhoneAppViewToDialer;       
    }

// -----------------------------------------------------------------------------
// CPhoneView::SetViewObserver
// -----------------------------------------------------------------------------
//
void CPhoneView::SetPhoneViewObserver( MPhoneViewObserver& aObserver )
    {
    iObserver = &aObserver;
    }

// -----------------------------------------------------------------------------
// CPhoneView::SetDialerActivation
// -----------------------------------------------------------------------------
//
void CPhoneView::SetDialerActivation( TBool aActivation )
    {
    __PHONELOG1( EBasic, 
        EPhoneUIView,"CPhoneView::SetDialerActivation() aActivation %d", 
        aActivation );
    iDialerActivation = aActivation;
    }
// End of File

