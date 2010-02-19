/*
* Copyright (c) 2009, 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Shows number keypad and generates keypress event when 
*                 buttons are pressed.
*
*/


// INCLUDE FILES
#include <gulicon.h>
#include <aknutils.h>
#include <aknbutton.h>
#include <akncontrol.h>
#include <aknsutils.h>
#include <aknsskininstance.h>
#include <aknsdrawutils.h>
#include <data_caging_path_literals.hrh> // for KDC_APP_RESOURCE_DIR
#include <touchfeedback.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <aknsframebackgroundcontrolcontext.h>

#include "dialercommon.h"
#include "dialertrace.h"
#include "cdialerkeypadbutton.h"

_LIT( KDialerMifFileName, "dialer.mif" );
static const TInt KIconMarginPercent = 5;
static const TInt KCent = 100;

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CDialerKeyPadButton::CDialerKeyPadButton( const TDesC& aNumberLabel,
                                          const TDesC& aPrimaryAlphaLabel,
                                          const TDesC& aSecondaryAlphaLabel,
                                          TInt aScanCode, 
                                          TInt aKeyCode,
                                          TMifDialer aButtonIconId, 
                                          TMifDialer aButtonIconMaskId,
                                          TInt aFlags ):
        CAknButton( aFlags ), 
        iScanCode( aScanCode ), 
        iKeyCode( aKeyCode ),
        iButtonIconId( aButtonIconId ),
        iButtonIconMaskId( aButtonIconMaskId ),
        iNumberLabel( aNumberLabel ),
        iPrimaryAlphaLabel( aPrimaryAlphaLabel ),
        iSecondaryAlphaLabel( aSecondaryAlphaLabel )
    {
    }

// -----------------------------------------------------------------------------
// CDialerKeyPadButton::NewLC
// Two-phased constructor.
// Constructs dialer button.
// -----------------------------------------------------------------------------
//

CDialerKeyPadButton* CDialerKeyPadButton::NewLC( const TDesC& aNumberLabel,
                                                 const TDesC& aPrimaryAlphaLabel,
                                                 const TDesC& aSecondaryAlphaLabel,
                                                 TInt aScanCode, 
                                                 TInt aKeyCode,
                                                 TMifDialer aButtonIconId, 
                                                 TMifDialer aButtonIconMaskId,
                                                 TInt aFlags )
    {
    CDialerKeyPadButton* self = new (ELeave) CDialerKeyPadButton( aNumberLabel, 
                                                                  aPrimaryAlphaLabel, 
                                                                  aSecondaryAlphaLabel, 
                                                                  aScanCode, 
                                                                  aKeyCode,
                                                                  aButtonIconId,
                                                                  aButtonIconMaskId,
                                                                  aFlags );
    CleanupStack::PushL( self );
    self->ConstructL( aFlags );
    return self;
    }

// -----------------------------------------------------------------------------
// CDialerKeyPadButton::ConstructL
// Symbian 2nd phase constructor for dialer button.
// -----------------------------------------------------------------------------
//
void CDialerKeyPadButton::ConstructL( TInt aFlags )
    {
    CAknButton::ConstructL( NULL, NULL, NULL, NULL, KNullDesC, KNullDesC, aFlags );
    UpdateIconL();
    
    // button text color
    SetTextColorIds( KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG65 );
    SetIconScaleMode( EAspectRatioPreserved );
    SetMargins( TMargins8(0,0,0,0) );
    AknsUtils::RegisterControlPosition( this );
    }
	
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CDialerKeyPadButton::~CDialerKeyPadButton()
    {
    AknsUtils::DeregisterControlPosition( this );
    
    MTouchFeedback* feedback = MTouchFeedback::Instance();
    if ( feedback )
        {
        feedback->RemoveFeedbackForControl( this );
        }
    }

// ---------------------------------------------------------------------------
// Draw dialer button,  text and icon.
// ---------------------------------------------------------------------------
// 
void CDialerKeyPadButton::Draw( const TRect& aRect ) const
    {
    TAknLayoutRect centerLayout;
    centerLayout.LayoutRect( aRect,
        AknLayoutScalable_Avkon::toolbar_button_pane_g1().LayoutLine() );
    TRect innerRect = centerLayout.Rect();

    CWindowGc& gc = SystemGc();
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    TAknsItemID frameId = KAknsIIDQsnFrButtonNormal;

    if ( iButtonPressed )
        {
        frameId = KAknsIIDQsnFrButtonPressed;
        }
    else if ( IsDimmed() )
        {
        frameId = KAknsIIDQsnFrButtonInactive;
        }
        
    iBgContext->SetFrame( frameId );
    iBgContext->SetCenter( KAknsIIDQsnFrButtonCenterNormal );
    iBgContext->SetFrameRects( aRect, innerRect );

    TBool bgDrawn = AknsDrawUtils::Background( skin, 
                                    iBgContext, 
                                    NULL, 
                                    gc, 
                                    aRect, 
                                    KAknsDrawParamNoClearUnderImage );
                                    
    __ASSERT_DEBUG( bgDrawn, DialerPanic( EDialerPanicDrawingError ) );
                                    
    DrawIconAndText( gc );
    }

// -----------------------------------------------------------------------------
// Respond to changes in the button size
// -----------------------------------------------------------------------------
//
void CDialerKeyPadButton::SizeChanged()
    {
    CAknButton::SizeChanged();
    AknsUtils::RegisterControlPosition( this );
    
    TRect buttonRect = Rect();
    TAknLayoutRect iconLayout;
    const CGulIcon* icon = GetCurrentIcon();

    if ( iOperationMode == EModeEasyDialing )
        {
        // Number layout.
        iNumberLayout.LayoutText( buttonRect,
            AknLayoutScalable_Apps::cell_dia3_key_num_pane_t1( iVariety ) );
        // Use different number layout if key doens't contain any alphabets or icon.
        if ( !icon && !iPrimaryAlphaLabel.Length() && !iSecondaryAlphaLabel.Length() )
            {
            iNumberLayout.LayoutText( buttonRect, 
                AknLayoutScalable_Apps::cell_dialer2_keypad_pane_t1(), 
                iNumberLayout.Font() );
            }

        // Alphabet layout is different if two rows of alphabets are given
        if ( iSecondaryAlphaLabel.Length() )
            {
            iPrimaryAlphaLayout.LayoutText( buttonRect, 
                AknLayoutScalable_Apps::cell_dia3_key_num_pane_t3( iVariety ) );
            iSecondaryAlphaLayout.LayoutText( buttonRect, 
                AknLayoutScalable_Apps::cell_dia3_key_num_pane_t4( iVariety ) );
            }
        else
            {
            iPrimaryAlphaLayout.LayoutText( buttonRect, 
                AknLayoutScalable_Apps::cell_dia3_key_num_pane_t2( iVariety ) );
            }

        // Icon layout
        iconLayout.LayoutRect( buttonRect, 
            AknLayoutScalable_Apps::cell_dialer2_keypad_pane_g2( 1 ) );
        iIconRect = iconLayout.Rect();

        // Icon doesn't fit to location set in the layout if key has any alpha label.
        // In that case, move the icon to upper-right corner.
        if ( iPrimaryAlphaLabel.Length() || iSecondaryAlphaLabel.Length() )
            {
            TSize iconSize = iIconRect.Size();
            TInt xMargin = buttonRect.Width() * KIconMarginPercent / KCent;
            TInt yMargin = buttonRect.Height() * KIconMarginPercent / KCent;
            TPoint iconPos( buttonRect.iBr.iX - iconSize.iWidth - xMargin, 
                            buttonRect.iTl.iY + yMargin );
            iIconRect.SetRect( iconPos, iconSize );
            }
        }
    else if ( iOperationMode == EModeDialer )
        {
        // Number layout
        iNumberLayout.LayoutText( buttonRect, 
            AknLayoutScalable_Apps::cell_dialer2_keypad_pane_t1() );
    
        // Icon layout
        iconLayout.LayoutRect( buttonRect,
            AknLayoutScalable_Apps::cell_dialer2_keypad_pane_g2( 1 ) );
        iIconRect = iconLayout.Rect();
        }
    else // video mode layout
        {
        // Number layout
        iNumberLayout.LayoutText( buttonRect, 
            AknLayoutScalable_Apps::cell_video_dialer_keypad_pane_t1() );
        
        // Icon layout
        iconLayout.LayoutRect( buttonRect, 
            AknLayoutScalable_Apps::cell_video_dialer_keypad_pane_g2( 2 ) );
        iIconRect = iconLayout.Rect();
        }

    SetIconSize( iIconRect.Size() );
    }

// -----------------------------------------------------------------------------
// Gets the correct text color.
// -----------------------------------------------------------------------------
//
void CDialerKeyPadButton::GetTextColors( TRgb& aPenColor, TRgb& aBrushColor ) const 
   { 
   aBrushColor = iEikonEnv->ControlColor( EColorControlBackground, *this ); 
   
   if ( iButtonPressed )
       {
       aPenColor = iEikonEnv->ControlColor( EColorButtonTextPressed, *this );
       }
   else
       {
       aPenColor = iEikonEnv->ControlColor( EColorButtonText, *this );
       }
   }

// --------------------------------------------------------------------------
// Draw text and icon according to the layout.
// --------------------------------------------------------------------------
//
void CDialerKeyPadButton::DrawIconAndText( CWindowGc& aGc ) const
    {
    TRect parentRect( Rect() );
    TAknLayoutText buttonText;
    TAknLayoutRect layoutRect;
    
    TRgb penColor;
    TRgb brushColor;
    GetTextColors( penColor, brushColor );
    
    // Don't ues logical-to-visual conversion, strings in our keypad resources are already
    // in visual order.
    TBool logToVisConv = EFalse;
    
    iNumberLayout.DrawText( aGc, iNumberLabel, logToVisConv, penColor );

    // Draw also alphabet label(s) in Easy Dialing mode
    if ( iOperationMode == EModeEasyDialing )
        {
        if ( iPrimaryAlphaLabel.Length() )
            {
            iPrimaryAlphaLayout.DrawText( aGc, iPrimaryAlphaLabel, logToVisConv, penColor );
            }
        if ( iSecondaryAlphaLabel.Length() )
            {
            iSecondaryAlphaLayout.DrawText( aGc, iSecondaryAlphaLabel, logToVisConv, penColor );
            }
        }
    
    const CGulIcon* icon = GetCurrentIcon();
    if ( icon )
        {
        CFbsBitmap* buttonBmp = icon->Bitmap();
        CFbsBitmap* buttonMask = icon->Mask();
   
        if ( buttonBmp && buttonMask )
            {
            aGc.BitBltMasked( iIconRect.iTl, buttonBmp,
                    iIconRect.Size(), buttonMask, ETrue );
            }
        }
    }

// --------------------------------------------------------------------------
// Set layout data for text.
// --------------------------------------------------------------------------
void CDialerKeyPadButton::SetVariety( TInt aVariety )
    {
    iVariety = aVariety;
    }

// --------------------------------------------------------------------------
// Set layout data for text.
// --------------------------------------------------------------------------
void CDialerKeyPadButton::SetOperationMode( TDialerOperationMode aMode ) 
    {
    iOperationMode = aMode;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//    
void CDialerKeyPadButton::SetNumLabel( const TDesC& aLabel )
    {
    iNumberLabel.Set( aLabel );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//    
void CDialerKeyPadButton::SetPrimaryAlphaLabel( const TDesC& aLabel )
    {
    iPrimaryAlphaLabel.Set( aLabel );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//    
void CDialerKeyPadButton::SetSecondaryAlphaLabel( const TDesC& aLabel )
    {
    iSecondaryAlphaLabel.Set( aLabel );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//    
TInt CDialerKeyPadButton::ScanCode() const
    {
    return iScanCode;
    }
    
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//    
TInt CDialerKeyPadButton::KeyCode() const
    {
    return iKeyCode;
    }    
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CDialerKeyPadButton::MapDialerIconToSkinIcon( 
        TInt aDialerIcon, TAknsItemID& aItemId ) const
    {    
     switch ( aDialerIcon )
        {       
        case EMbmDialerQgn_indi_dialer_voicemail:
            aItemId = KAknsIIDQgnIndiDialerVoicemail;
            break; 
        default:
            break;
        }
    }   

// ---------------------------------------------------------------------------
// Update icon when skin is changed
// ---------------------------------------------------------------------------
//   
void CDialerKeyPadButton::UpdateIconL()
    {
    if ( KDialerNoIcon != iButtonIconId )
        {
        TFileName mifPath( KDriveZ );
        mifPath.Append( KDC_APP_BITMAP_DIR );
        mifPath.Append( KDialerMifFileName );
        
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        CFbsBitmap* bitmap (NULL);
        CFbsBitmap* mask (NULL);
            
        // Get icon id.
        TAknsItemID skinItemId( KAknsIIDNone );  
        MapDialerIconToSkinIcon( iButtonIconId, skinItemId );
        
        AknsUtils::CreateColorIconLC( 
                skin, 
                skinItemId,
                KAknsIIDQsnIconColors, 
                EAknsCIQsnIconColorsCG30,
                bitmap, 
                mask, 
                mifPath, 
                iButtonIconId, 
                iButtonIconMaskId,
                KRgbBlack
                );
        
        CGulIcon* icon = CGulIcon::NewL( bitmap, mask );
        CleanupStack::Pop( 2 );
        
        State()->SetIcon( icon ); // icon ownership transfered
        }
    }  

// ---------------------------------------------------------------------------
// 
// Enable or disable audio but keep vibra feedback 
// ---------------------------------------------------------------------------
//   
void CDialerKeyPadButton::EnableAudioFeedback( const TBool aEnable )
    {
    MTouchFeedback* feedback = MTouchFeedback::Instance();
    if ( feedback )
        {
        feedback->EnableFeedbackForControl( this, ETrue, aEnable );
        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CDialerKeyPadButton::HandleResourceChange( TInt aType )
    {
   CAknButton::HandleResourceChange( aType );
   
    if ( KAknsMessageSkinChange == aType )
        {
        TRAP_IGNORE( UpdateIconL() );
        }    
    }
    
// End of File
