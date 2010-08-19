/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Button for touch pane.
*
*/

#include <AknBidiTextUtils.h>
#include <AknsDrawUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <gulicon.h>
#include <barsread.h> 
#include <AknsFrameBackgroundControlContext.h>
#include <AknsConstants.h>
#include <phoneappcommands.hrh>

#include "BMTouchPaneButton.h"
#include "BMTouchPaneInterface.h"
#include "BMPanic.h"

const TInt KDimmedTransparency = 77;
const TInt KBubbleButtonText = 255;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CBubbleTouchPaneButton::CBubbleTouchPaneButton(
    MBubbleTouchPaneIconProvider* aIconProvider,
    const TInt aFlags ) :
    CAknButton( aFlags ),
    iIconProvider( aIconProvider )
    {
    }
    
// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CBubbleTouchPaneButton* CBubbleTouchPaneButton::NewL( 
        MBubbleTouchPaneIconProvider* aIconProvider, 
        const TInt aFlags )
    {
    CBubbleTouchPaneButton* self = 
    new(ELeave) CBubbleTouchPaneButton( aIconProvider, aFlags );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor 
// -----------------------------------------------------------------------------
//
void CBubbleTouchPaneButton::ConstructL()
    {
    CAknButton::ConstructL();
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleTouchPaneButton::~CBubbleTouchPaneButton()
    {
    iCommands.Close();   
    }
        
// ---------------------------------------------------------------------------
// CBubbleTouchPaneButton::ConstructFromResouceL
// ---------------------------------------------------------------------------
//
void CBubbleTouchPaneButton::ConstructFromResouceL( TResourceReader& aReader )
    {
    
    TInt commandCount = aReader.ReadInt16();
    for ( TInt i=0; i < commandCount; i++ )    
        {
        TInt command = aReader.ReadInt16();
        iCommands.AppendL( command );
        }
    
    aReader.ReadInt16(); // this is mystery?
    
    CAknButton::ConstructFromResourceL( aReader );
    
    SelectTextColor();
    
    // Customize icons
    if ( iIconProvider )
        {
        LoadCustomIcons( *iIconProvider );
        }
    
    if ( State()->Icon() )
        {
        SetIconScaleMode( EAspectRatioPreserved );
        }
    }

// ---------------------------------------------------------------------------
// CBubbleTouchPaneButton::LoadCustomIconsL
// ---------------------------------------------------------------------------
//    
void CBubbleTouchPaneButton::LoadCustomIcons(
    MBubbleTouchPaneIconProvider& aIconProvider )
    {
    TInt commandCount = iCommands.Count();
    
    for ( TInt i=0; i < commandCount; i++ )    
        {
        CGulIcon* icon = NULL;
           
        TRAPD( err, icon = aIconProvider.GetIconForCommandL( 
                               iCommands[i] ) );
     
        if ( !err && icon )       
            {
            CAknButtonState* state = State( i );
            // State takes ownership of icon
            state->SetIcon( icon );                
            }
        }    
    }

// ---------------------------------------------------------------------------
// CBubbleTouchPaneButton::CommandId
// ---------------------------------------------------------------------------
//
TInt CBubbleTouchPaneButton::CommandId( TBool aPreviousState ) const
    {
    __ASSERT_DEBUG( iCommands.Count(), Panic( EBMPanicTouchPane ) );
    
    TInt command;
    if ( iCommands.Count() > 1 )
        {
        TInt index =  StateIndex();
        if ( aPreviousState )
            {
            // return command from previous state
            index -= 1;
            if ( index < 0 )
                {
                index = iCommands.Count() - 1;    
                }
            }
        
        command = iCommands[index];            
        }
    else
        {
        command = iCommands[0];    
        }        
    
    return command;    
    }    

// ---------------------------------------------------------------------------
// CBubbleTouchPaneButton::SetState
// ---------------------------------------------------------------------------
//    
void CBubbleTouchPaneButton::SetState( TInt aCommand,
                                       TBool aDrawNow )
    {
    for ( TInt i=0; i < iCommands.Count(); i++ )
        {
        if ( iCommands[i] == aCommand )
            {
            if ( i != StateIndex() )
                {
                SetCurrentState( i, aDrawNow );                
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CBubbleTouchPaneButton::SetDimmed
// ---------------------------------------------------------------------------
//
void CBubbleTouchPaneButton::SetDimmed( TBool aDimmed )
    {
    CAknButton::SetDimmed( aDimmed );
    DrawDeferred();                
    }

// ---------------------------------------------------------------------------
// CBubbleTouchPaneButton::HasState
// ---------------------------------------------------------------------------
//   
TBool CBubbleTouchPaneButton::HasState( TInt aCommand ) const
    {
    TBool hasState = EFalse;
    
    for ( TInt i=0; i < iCommands.Count(); i++ )
        {
        if ( iCommands[i] == aCommand )
            {
            hasState = ETrue;
            break;    
            }
        }
    
    return hasState;
    }
   

// ---------------------------------------------------------------------------
// Draw button, text and icon.
// ---------------------------------------------------------------------------
// 
void CBubbleTouchPaneButton::Draw( const TRect& aRect ) const
    {
    TAknsItemID frameId( KAknsIIDQsnFrButtonNormal );
    TAknLayoutRect centerLayout;
    centerLayout.LayoutRect( aRect,
        AknLayoutScalable_Avkon::toolbar_button_pane_g1().LayoutLine() );
    TRect innerRect = centerLayout.Rect();

    CWindowGc& gc = SystemGc();
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    TInt commandCount = iCommands.Count();
    
    for ( TInt i=0; i < commandCount; i++ )    
        {
        TRAP_IGNORE( frameId = iIconProvider->GetButtonForCommandL( 
                                   iCommands[i]));

        if ( iButtonPressed )
            {
            frameId = SelectPressedButton( frameId );
            }
        else if ( IsDimmed() )
            {
            frameId = SelectDimmedButton( frameId );
            }
    
        iBgContext->SetFrame( frameId );
        iBgContext->SetCenter( KAknsIIDQsnFrButtonCenterNormal );
        iBgContext->SetFrameRects( aRect, innerRect );
    
        if ( !AknsDrawUtils::Background( skin, 
                                        iBgContext, 
                                        NULL, 
                                        gc, 
                                        aRect, 
                                        KAknsDrawParamNoClearUnderImage ) )
            {
            gc.SetBrushColor( KRgbRed );
            gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
            gc.DrawRect( aRect );
            }
        LayoutIconAndText( gc );
        }
    }

// --------------------------------------------------------------------------
// Draw text and icon according to the layout.
// --------------------------------------------------------------------------

void CBubbleTouchPaneButton::LayoutIconAndText( CWindowGc& aGc ) const
    {
    TRect parentRect( Rect() );
    CAknButtonState* state = State();
    if ( !state  ||       
         !state->Icon() ||
         !state->Text().Length() )
        {
        return;
        }    

    TRgb penColor;
    TRgb brushColor;
    GetTextColors( penColor ); 
    
    // buffer for visually ordered text
    TBuf< KBubbleButtonText + KAknBidiExtraSpacePerLine> visualText; 
    TInt clipWidth = iLayoutText.TextRect().Width();
    
    // bidi processing - using AknBidiTextUtils.
    AknBidiTextUtils::ConvertToVisualAndClip(
        state->Text(),
        visualText,
        *iLayoutText.Font(),
        clipWidth,
        clipWidth );
    
    iLayoutText.DrawText( aGc, visualText, EFalse, penColor );
    
    const CGulIcon* icon = GetCurrentIcon();
    if ( icon )
        {
        CFbsBitmap* buttonBmp = icon->Bitmap();
        CFbsBitmap* buttonMask = icon->Mask();
   
        if( buttonBmp && buttonMask )
            {
            aGc.BitBltMasked( iIconRect.iTl, buttonBmp,
                    iIconRect.Size(), buttonMask, ETrue );
            }
        }
    }

// ---------------------------------------------------------------------------
// CBubbleTouchPaneButton::SetLayout
// ---------------------------------------------------------------------------
//
void CBubbleTouchPaneButton::SetLayout( const TAknLayoutText aLayoutText,
                                        const TRect aIconRect ) 
    {
    iLayoutText = aLayoutText;
    iIconRect = aIconRect;
    }    

// ---------------------------------------------------------------------------
// CBubbleTouchPaneButton::HandleResourceChange
// ---------------------------------------------------------------------------
//
void CBubbleTouchPaneButton::HandleResourceChange( TInt aType )
    {
    CAknButton::HandleResourceChange( aType );
    
    if ( aType == KAknsMessageSkinChange &&
         iIconProvider )
        {
        LoadCustomIcons( *iIconProvider );
        }    
    }

// -----------------------------------------------------------------------------
// Gets the correct text color.
// -----------------------------------------------------------------------------
//
void CBubbleTouchPaneButton::GetTextColors( TRgb& aPenColor ) const 
   { 
   if ( IsDimmed() ) 
       {
       // Using parameter EColorButtonTextDimmed does not return valid 
       // color for the text that is used in dimmed button. We have 
       // to use default text and set it transparent.
       aPenColor = iEikonEnv->ControlColor( EColorButtonText, *this );
       aPenColor.SetAlpha( KDimmedTransparency );
       }
   else if ( iButtonPressed )
       {
       aPenColor = iEikonEnv->ControlColor( EColorButtonTextPressed, *this );
       }
   else
       {
       aPenColor = iEikonEnv->ControlColor( EColorButtonText, *this );
       }
   }

// -----------------------------------------------------------------------------
// Selects the correct pressed button.
// -----------------------------------------------------------------------------
//
TAknsItemID CBubbleTouchPaneButton::SelectPressedButton( TAknsItemID frameId ) const
   { 
   if( frameId == KAknsIIDQsnFrButtonNormalAnswer )
       {
       return KAknsIIDQsnFrButtonPressedAnswer;
       }
   else if( frameId == KAknsIIDQsnFrButtonNormalReject )
       {
       return KAknsIIDQsnFrButtonPressedReject;
       }
   else
       {
       return KAknsIIDQsnFrButtonPressed;
       }
   }

// -----------------------------------------------------------------------------
// Selects the correct dimmed button.
// -----------------------------------------------------------------------------
//
 TAknsItemID CBubbleTouchPaneButton::SelectDimmedButton( TAknsItemID frameId ) const
   {
   if( frameId == KAknsIIDQsnFrButtonNormalAnswer )
       {
       return KAknsIIDQsnFrButtonInactiveAnswer;
       }
   else if( frameId == KAknsIIDQsnFrButtonNormalReject )
       {
       return KAknsIIDQsnFrButtonInactiveReject;
       }
   else
       {
       return KAknsIIDQsnFrButtonInactive;
       }
   }

// -----------------------------------------------------------------------------
// Selects the correct text color.
// -----------------------------------------------------------------------------
//
void CBubbleTouchPaneButton::SelectTextColor() 
    { 
    TAknsQsnTextColorsIndex textColorIndex( EAknsCIQsnTextColorsCG65 );
    TInt command = CommandId( EFalse );

    if ( command == EPhoneCallComingCmdAnswer )
        {
        textColorIndex = EAknsCIQsnTextColorsCG82;
        }
    else if ( command == EPhoneCallComingCmdReject )
        {
        textColorIndex = EAknsCIQsnTextColorsCG83;
        }

    // function button text color
    SetTextColorIds( KAknsIIDQsnTextColors, textColorIndex );
    }
