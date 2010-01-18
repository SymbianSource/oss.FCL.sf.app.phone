/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Outlook Number Entry
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" 
#include    "BMBubbleOutlookNE.h"
#include    "BMBubbleImageManager.h"
#include    "BMBubbleHeader.h"
#include    "BMUtils.h"
#include    "BMResourceManager.h"
#include    "BMLayout.h"
#include    "BMLayout2.h"

#include    <eiklabel.h>
#include    <eikimage.h>
#include    <eikenv.h>
#include    <AknPhoneNumberEditor.h>
#include    <AknsBasicBackgroundControlContext.h>
#include    <AknsFrameBackgroundControlContext.h>
#include    <AknsDrawUtils.h>
#include    <AknLayoutFont.h>
#include    <applayout.cdl.h>
#include    <aknappui.h>
#include    <eikspane.h>
#include    <AknStatuspaneUtils.h>

//for resources
#include    <barsread.h>
#include    <Bubblemanager.rsg>

// CONSTANTS
const TUint8 KBubbleNE1LongLineFormat = 0;
const TUint8 KBubbleNE2LongLineFormat = 1;
const TUint8 KBubbleNE1ShortLineFormat = 2;
const TUint8 KBubbleNE2ShortLineFormat = 3;
const TUint8 KBubbleNEIdleStateFormat = 4;

const TInt KBubbleNEFormatOffsetIfNoStatusPane = 5;



// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::CBubbleOutlookNumberEntry
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookNumberEntry::CBubbleOutlookNumberEntry( 
                                    CBubbleManager& aBubbleManager,
                                    const CCoeControl& aMutedImage )
: CBubbleOutlook( aBubbleManager ) , iMutedImage( aMutedImage )

    {

    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::ConstructL
// 
// ---------------------------------------------------------------------------
//

void CBubbleOutlookNumberEntry::ConstructL()
    {
    iPlace = ENENone;
    iIsUsed = EFalse;
    iEditor = NULL;

    // Create skin background control context,
    // the actual image ID and rectangle will be set later
    if ( iCallObjectDisplay )
        {
        iFrameContext = CAknsFrameBackgroundControlContext::NewL(
            KAknsIIDNone, TRect(0,0,4,4), TRect(1,1,3,3), EFalse );
        }
    else
        {
        iSkinContext = CAknsBasicBackgroundControlContext::NewL(
            KAknsIIDNone, TRect(0,0,1,1), EFalse );
        }    
    
    // Create editor from resources:
    iEditor = new( ELeave ) CAknPhoneNumberEditor;
    iEditor->SetContainerWindowL( *this );
    iEditor->SetParent( this );

    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC( reader, R_BUBBLEMANAGER_EDITOR );
    ConstructEditorFromResourceAndLayoutL( reader );

    CleanupStack::PopAndDestroy();    // reader

    iEditor->SetObserver( this );
    iEditor->SetFormat( KBubbleNEIdleStateFormat );
    
    iStatusPane = static_cast<CAknAppUi*>(iEikonEnv->
        EikAppUi() )->StatusPane();

    CBubbleOutlook::ConstructL( );

    SetBlank();
    ActivateL();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::~CBubbleOutlookNumberEntry
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookNumberEntry::~CBubbleOutlookNumberEntry()
    {
    if ( iBubble )
        {
        iBubbleManager.ResourceManager().ReleaseEikImage( iBubble );
        }
    delete iEditor;
    delete iSkinContext;
    delete iFrameContext;
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::Reset
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::Reset( )
    {
    if ( iBubble )
        {
        iBubbleManager.ResourceManager().ReleaseEikImage( iBubble );
        }
    iPlace = ENENone;
    iIsUsed = EFalse;
    iEditor->SetFocus( EFalse );
    SetText( KNullDesC );
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::ReadBubbleHeader( CBubbleHeader& /*aHeader*/ )
    {
    // ignore
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::SetPlace
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::SetPlace( const TPhoneNumberEntryPlace& aPlace )
    {
    if ( iPlace == aPlace )
        {
        return;
        }

    iPlace = aPlace;

    if ( iCallObjectDisplay )
        {
        SetPlace2( iPlace );
        }
    else
        {
        SetPlace1( iPlace );
        }        

    SizeChanged();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::SetPlace1
//
//  
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookNumberEntry::SetPlace1( 
    const TPhoneNumberEntryPlace& aPlace )
    {
    switch ( aPlace )
        {
        case ENEBottom:
            {
            if ( iBubble )
                {
                iBubbleManager.ResourceManager().ReleaseEikImage( iBubble );
                }
        
            iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );
                    
            iBubbleManager.ImageManager().SetBitmapToImage( 
                iBubble , 
                EQgn_graf_call_rec_big , 
                EQgn_graf_call_rec_big_mask );
         
            iSkinContext->SetBitmap( KAknsIIDQgnGrafCallRecBig );
            break;
            }
        case ENEBottomRight:
            {
            if ( iBubble )
                {
                iBubbleManager.ResourceManager().ReleaseEikImage( iBubble );
                }
            
            iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );
            
            iBubbleManager.ImageManager().SetBitmapToImage( 
                iBubble , 
                EQgn_graf_call_rec_big_right , 
                EQgn_graf_call_rec_big_right_mask );
         
            iSkinContext->SetBitmap( KAknsIIDQgnGrafCallRecBigRight );
            break;
            }
        case ENEIdleState:
            {
            if ( iBubble )
                {
                iBubbleManager.ResourceManager().ReleaseEikImage( iBubble );
                }
         
            iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );
         
            iBubbleManager.ImageManager().SetBitmapToImage( 
                iBubble , 
                EQgn_graf_call_rec_bigger , 
                EQgn_graf_call_rec_bigger_mask );
         
            iSkinContext->SetBitmap( KAknsIIDQgnGrafCallRecBigger );
            break;            
            }
        case ENENone:
        default:
            break;
        }
    
    iSkinContext->SetParentContext( 
        AknsDrawUtils::ControlContextOfParent(this) );
    }    

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::SetPlace2
//  
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookNumberEntry::SetPlace2( 
    const TPhoneNumberEntryPlace& aPlace )
    {
    switch ( aPlace )
        {
        case ENEBottom:
            iFrameContext->SetFrame( KAknsIIDQsnFrCall2Rect );
            break;
        case ENEBottomRight:
            iFrameContext->SetFrame( KAknsIIDQsnFrCall2Rect );
            break;
        case ENEIdleState:
            iFrameContext->SetFrame( KAknsIIDQsnFrCall2Rect );
            break;            
        case ENENone:
        default:
            break;
        }    

    iFrameContext->SetParentContext( 
        AknsDrawUtils::ControlContextOfParent(this) );        
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::Place
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookNumberEntry::TPhoneNumberEntryPlace 
                                    CBubbleOutlookNumberEntry::Place() const
    {
    return iPlace;
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::SizeChanged
// called by framwork when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::SizeChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    
    if ( iCallObjectDisplay )
        {
        SizeChanged2();
        }
    else
        {
        SizeChanged1();    
        }        
    
    UpdateEditorFormats( iPlace );

    // DOES NOT LEAVE! Own implementation. L comes from observer
    // interface
    UpdateAndDrawEditor();
    }
    
// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::SizeChanged1
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::SizeChanged1()
    {
    switch ( iPlace )
        {
        case ENEBottom:
            BubbleUtils::LayoutBackgroundImage( 
                iBubble, 
                Rect(), 
                BubbleLayout::popup_number_entry_window_graphics_1( 0 ) );
            
            iSkinContext->SetRect( iBubble->Rect() );
            break;
            
        case ENEBottomRight:
            BubbleUtils::LayoutBackgroundImage( 
                iBubble , 
                Rect(), 
                BubbleLayout::popup_number_entry_window_graphics_1( 0 ) );

            iSkinContext->SetRect( iBubble->Rect() );
            break;
            
        case ENEIdleState:
            BubbleUtils::LayoutBackgroundImage( 
                iBubble, 
                Rect(), 
                BubbleLayout::popup_number_entry_window_graphics_1( 3 ) );

            iSkinContext->SetRect( iBubble->Rect() );
            break;            

        case ENENone:
        default:
            break;

        }
    }
    
// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::SizeChanged2
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::SizeChanged2()
    {
    TAknLayoutRect frameRect;
    TBool prepareFrame( EFalse );
    
    switch ( iPlace )
        {
        case ENEBottom:
        case ENEBottomRight:
           frameRect.LayoutRect( 
               Rect(), 
               BubbleLayout::popup_number_entry_window_graphics_1( 0 ) );
           prepareFrame = ETrue;     
           break;

        case ENEIdleState:
            frameRect.LayoutRect( 
                Rect(), 
                BubbleLayout::popup_number_entry_window_graphics_1( 3 ) );
            prepareFrame = ETrue;                
            break;            
        case ENENone:
        default:
            break;
        }
        
    if ( prepareFrame )        
        {
        BubbleLayout2::RectFrameInnerOuterRects(
           frameRect.Rect(),
           iOuterRect,
           iInnerRect );

        iFrameContext->SetFrameRects( iOuterRect, iInnerRect );
                
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        AknsDrawUtils::PrepareFrame( skin,
                                     iOuterRect,
                                     iInnerRect,
                                     KAknsIIDQsnFrCall2Rect,
                                     KAknsIIDDefault );            
        }        
    }    

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::CountComponentControls
//
//  
// ---------------------------------------------------------------------------
//
TInt CBubbleOutlookNumberEntry::CountComponentControls() const
    {
    return 1;
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::ComponentControl
//
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CBubbleOutlookNumberEntry::ComponentControl(TInt aIndex) const
    {
    if ( aIndex == 0 )
        {
        return iEditor;
        }
    return NULL;
    
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::DrawTimerCostNow
//
//  Pure virtual in base class.
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::DrawTimerCostNow()
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::DrawCLINow
//
//  Pure virtual in base class.
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::DrawCLINow()
    {
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::HandleControlEventL
//  
//  This must be non leaving function, because others use it too.
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::HandleControlEventL( CCoeControl* aControl, 
                                                     TCoeEvent aEventType)
    {
    if ( aControl != iEditor  
        && aEventType != EEventStateChanged 
        || iPlace == ENENone)
        return;

    UpdateAndDrawEditor();
    
	ReportEventL( EEventStateChanged );
    
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::OfferKeyEventL
//
//  
// ---------------------------------------------------------------------------
//
TKeyResponse CBubbleOutlookNumberEntry::OfferKeyEventL( 
                                            const TKeyEvent& aKeyEvent, 
                                            TEventCode aType )
    {
    return iEditor->OfferKeyEventL( aKeyEvent, aType );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::SetIsUsed
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::SetIsUsed( const TBool& aIsUsed )
    {
#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
    // Don't set in focus if transition effects are turned on
    // We don't want blinking cursor during transitions
    if ( !aIsUsed )
        {
        iEditor->SetFocus( aIsUsed );
        }
#else
    iEditor->SetFocus( aIsUsed );
#endif    
    iIsUsed = aIsUsed;    
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::IsUsed
//
//  
// ---------------------------------------------------------------------------
//
TBool CBubbleOutlookNumberEntry::IsUsed() const
    {
    return iIsUsed;
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::GetEditor
//
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CBubbleOutlookNumberEntry::GetEditor( ) const
    {
    return iEditor; 
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::SetText
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::SetText( const TDesC& aDesC )
    {
    iEditor->DrawDeferred();
    iEditor->SetText( aDesC );   
    iEditor->DrawDeferred();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::GetText
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::GetText( TDes& aDesC )
    {
    iEditor->GetText( aDesC );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::FocusChanged
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::FocusChanged( TDrawNow aDrawNow )
    {
    // Don't allow drawing
    iEditor->SetFocus( IsFocused(), ENoDrawNow );
    if ( aDrawNow == EDrawNow && iMutedImage.IsVisible() )
        {
        // muted image goes on top 
        // so it must be redrawn too
        DrawLabelNow( iMutedImage );
        }
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::DrawBitmaps
//
//  
// ---------------------------------------------------------------------------
//
#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
void CBubbleOutlookNumberEntry::DrawBitmaps( CBitmapContext& ) const
#else
void CBubbleOutlookNumberEntry::DrawBitmaps( CBitmapContext& aGc ) const
#endif
    {
    if ( iPlace == ENENone || !iIsUsed ) 
        {
        return;
        }

#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
    // Must use SystemGc instead of aGc when transition effects are turned on
    CWindowGc& aGc = SystemGc();
#endif

    if ( iCallObjectDisplay )
        {
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        if ( !AknsDrawUtils::DrawFrame( skin,
                                        (CWindowGc&) aGc,  
                                        iOuterRect,
                                        iInnerRect,
                                        KAknsIIDQsnFrCall2Rect,
                                        KAknsIIDDefault ) )
            {
            BubbleUtils::DrawBackgroundRect( aGc, Rect() );
            }       
        }
    else if ( !BubbleUtils::DrawMaskedImage( aGc , iBubble ) )
        {
        BubbleUtils::DrawBackgroundRect( aGc, Rect() );
        }
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::HandleAnimationStartL
//
//  Pure virtual in base class.
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::HandleAnimationStartL() const
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::ReleaseBitmaps
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::ReleaseBitmaps()
    {
    if ( iBubble )
        {
        iBubbleManager.ResourceManager().ReleaseEikImage( iBubble );
        }
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::RebuildBitmaps
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::RebuildBitmaps()
    {
    if ( iCallObjectDisplay )
        {
        return;            
        }
    
    switch ( iPlace )
        {
        case ENEBottom:
            iBubble = iBubbleManager.ResourceManager().ReserveEikImage(ETrue);
            iBubbleManager.ImageManager().SetBitmapToImage( 
                iBubble , 
                EQgn_graf_call_rec_big , 
                EQgn_graf_call_rec_big_mask );
            break;
        case ENEBottomRight:
            iBubble = iBubbleManager.ResourceManager().ReserveEikImage(ETrue);
            iBubbleManager.ImageManager().SetBitmapToImage( 
                iBubble , 
                EQgn_graf_call_rec_big_right , 
                EQgn_graf_call_rec_big_right_mask );
            break;
        case ENEIdleState:
            iBubble = iBubbleManager.ResourceManager().ReserveEikImage(ETrue);
            iBubbleManager.ImageManager().SetBitmapToImage( 
                iBubble , 
                EQgn_graf_call_rec_bigger , 
                EQgn_graf_call_rec_bigger_mask );
            break;
        case ENENone:
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::MopSupplyObject
// ---------------------------------------------------------------------------
//
TTypeUid::Ptr CBubbleOutlookNumberEntry::MopSupplyObject( TTypeUid aId )
    {
    if( !iCallObjectDisplay && 
        aId.iUid == MAknsControlContext::ETypeId && 
        iSkinContext )
        {
        return MAknsControlContext::SupplyMopObject( aId, iSkinContext );
        }
    else if ( iCallObjectDisplay && 
              aId.iUid == MAknsControlContext::ETypeId && 
              iFrameContext )
        {
        return MAknsControlContext::SupplyMopObject( aId, iFrameContext );    
        }
    else
        {
        return CBubbleOutlook::MopSupplyObject( aId );
        }
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::DrawCallHeaderText
//
//  Pure virtual in base class.
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::DrawCallHeaderText()
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::Draw
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::Draw( const TRect& /*aRect*/ ) const
    {
    if ( iPlace == ENENone || !iIsUsed ) 
        {
        return;
        }

    CWindowGc& gc = SystemGc( );
    gc.SetBrushColor( AKN_LAF_COLOR( BubbleLayout::LayoutColourWhite() ) );
    TRect rect = iEditor->Rect();

    // Draw the skin background of the parent 
    // (the bubble may be transparent)
    AknsDrawUtils::DrawBackground( AknsUtils::SkinInstance(), 
        AknsDrawUtils::ControlContextOfParent(this), this, gc, 
        rect.iTl, rect, KAknsDrawParamDefault );
    // Don't set clipping rect if transition effects are turned on
#ifndef RD_UI_TRANSITION_EFFECTS_PHASE2
    gc.SetClippingRect( rect );
#endif
    if ( iCallObjectDisplay )
        {
        // Draw bubble frame
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        if ( !AknsDrawUtils::DrawFrame( skin,
                                        (CWindowGc&) gc,  
                                        iOuterRect,
                                        iInnerRect,
                                        KAknsIIDQsnFrCall2Rect,
                                        KAknsIIDDefault ) )
            {
            BubbleUtils::DrawBackgroundRect( gc, Rect() );
            }                
        }
    else
        {
        if ( !BubbleUtils::DrawMaskedImage( gc , iBubble ) )
            {
            BubbleUtils::DrawBackgroundRect( gc, Rect() );
            }            
        }        
    gc.CancelClippingRect();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::ConstructEditorFromResourceAndLayoutL
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::ConstructEditorFromResourceAndLayoutL( 
    TResourceReader& aReader )
    {
    TInt maxChars = aReader.ReadInt16();
    TInt maxLines = aReader.ReadInt16();
    HBufC* truncation = aReader.ReadHBufCL();
    CleanupStack::PushL( truncation );
    TInt formatCount = aReader.ReadInt16();
    iEditor->ConstructL( maxChars, maxLines, formatCount, *truncation );
    CleanupStack::PopAndDestroy();  // truncation

    iFormatCount = formatCount;

    // get supporting layouts
    TRgb color;
    TRect numberEntryRect = NumberEntryRect( 0 , color );

    // get the relevant layouts
    TAknLayoutText texts1LongText;
    TAknLayoutText texts1ShortText;
    TAknLayoutText texts2LongLine1Text;
    TAknLayoutText texts2ShortLine1Text;
    TAknLayoutText texts2LongLine2Text;
    TAknLayoutText texts2ShortLine2Text;
    TAknLayoutText textsIdleLine1Text;
    TAknLayoutText textsIdleLine2Text;

    texts1LongText.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_1( 0 ) );
    texts1ShortText.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_1( 1 ) );
    texts2LongLine1Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_2( 0, 0 ) );
    texts2ShortLine1Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_2( 1, 0 ) );
    texts2LongLine2Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_2( 0, 1 ) );
    texts2ShortLine2Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_2( 1, 1 ) );
    textsIdleLine1Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_3( 1 ) );
    textsIdleLine2Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_3( 0 ) );        
        

    // although only specified for two line case, we need to have 
    // some kind of value for the one line case but as there is 
    // only one line, it isn't used.
    TInt baselineSeparation = texts2LongLine2Text.TextRect().iBr.iY 
        - texts2LongLine1Text.TextRect().iBr.iY;

    for (TInt ii = 0; ii < formatCount; ii++)
        {
        CAknPhoneNumberEditor::TFormat format( aReader );

        // override the format values using the layout data
        TInt next = ii;
        TBool noStatusPane = EFalse;
        if( ii > KBubbleNEIdleStateFormat )
            {
            next -= KBubbleNEFormatOffsetIfNoStatusPane;
            noStatusPane = ETrue;
            }
        
        const CAknLayoutFont* layoutFont;

        switch( next )
            {
            case KBubbleNE1LongLineFormat:
                format.iFont = texts1LongText.Font();
                layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( format.iFont);
                format.iBottomBaselineOffset = layoutFont->BaselineToTextPaneBottom();
                format.iOuterRect = texts1LongText.TextRect(); 
                break;
            case KBubbleNE2LongLineFormat:
                format.iFont = texts2LongLine1Text.Font();
                layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( format.iFont);
                format.iBottomBaselineOffset = layoutFont->BaselineToTextPaneBottom();

                format.iOuterRect = texts2LongLine2Text.TextRect();
                format.iOuterRect.iTl.iY -= format.iBottomBaselineOffset; // have to add extra gap
                format.iOuterRect.iTl.iY -= baselineSeparation; // have to add extra lines.
                break;
            case KBubbleNE1ShortLineFormat:
                format.iFont = texts1ShortText.Font();
                layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( format.iFont);
                format.iBottomBaselineOffset = layoutFont->BaselineToTextPaneBottom();
                format.iOuterRect = texts1ShortText.TextRect();
                break;
            case KBubbleNE2ShortLineFormat:
                format.iFont = texts2ShortLine1Text.Font();
                layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( format.iFont);
                format.iBottomBaselineOffset = layoutFont->BaselineToTextPaneBottom();
                format.iOuterRect = texts2ShortLine2Text.TextRect();
                format.iOuterRect.iTl.iY -= format.iBottomBaselineOffset; // have to add extra gap
                format.iOuterRect.iTl.iY -= baselineSeparation; // have to add extra lines.
                break;
            case KBubbleNEIdleStateFormat:
                format.iFont = textsIdleLine1Text.Font();
                layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( format.iFont);
                format.iBottomBaselineOffset = layoutFont->BaselineToTextPaneBottom();

                format.iOuterRect = textsIdleLine2Text.TextRect();
                format.iOuterRect.iTl.iY -= format.iBottomBaselineOffset; // have to add extra gap
                
                baselineSeparation = textsIdleLine2Text.TextRect().iBr.iY 
                    - textsIdleLine1Text.TextRect().iBr.iY;
                
                format.iOuterRect.iTl.iY -= baselineSeparation; // have to add extra lines.
                break;
            }

        if(noStatusPane)
            {
            format.iOuterRect.iTl.iY += BubbleLayout::MainPaneRect().iTl.iY;
            format.iOuterRect.iBr.iY += BubbleLayout::MainPaneRect().iTl.iY;
            }
        format.iBaselineSeparation = baselineSeparation;
        iEditor->AddFormat( format );
        }

    iEditor->SetFormat( KBubbleNEIdleStateFormat );
    UpdateEditorColor( color );
    }

// -----------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::NumberEntryRect
// -----------------------------------------------------------------------------
//
TRect CBubbleOutlookNumberEntry::NumberEntryRect( TInt aIndex, TRgb& aColor )
    {
    TRect mainRect = BubbleLayout::MainPaneRect();
    TAknLayoutRect numberEntryLayoutRect;
    numberEntryLayoutRect.LayoutRect( mainRect, 
        BubbleLayout::popup_number_entry_window( aIndex ) );

    aColor = numberEntryLayoutRect.Color();
    TRect numberEntryRect = numberEntryLayoutRect.Rect();
    // relative to mainpane
    numberEntryRect.iTl.iY -= mainRect.iTl.iY;
    numberEntryRect.iBr.iY -= mainRect.iTl.iY;
    numberEntryRect.iTl.iX -= mainRect.iTl.iX;
    numberEntryRect.iBr.iX -= mainRect.iTl.iX;

    return numberEntryRect ;
    }

// -----------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::UpdateEditorFormats
// -----------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::UpdateEditorFormats( 
    TPhoneNumberEntryPlace aPlace )
    {
    // get supporting layouts
    TRect numberEntryRect;
    TRgb color;
    if ( aPlace == ENEBottom )
        {
        numberEntryRect = NumberEntryRect( 0, color );
        }
    else if ( aPlace == ENEBottomRight )
        {
        numberEntryRect = NumberEntryRect( 1, color );
        }
    else if ( aPlace == ENEIdleState )
        {
        numberEntryRect = NumberEntryRect( 4, color );
        }        
    else
        {
        return;
        }

    // get the relevant layouts
    TAknLayoutText texts1LongText;
    TAknLayoutText texts1ShortText;
    TAknLayoutText texts2LongLine1Text;
    TAknLayoutText texts2ShortLine1Text;
    TAknLayoutText texts2LongLine2Text;
    TAknLayoutText texts2ShortLine2Text;
    TAknLayoutText textsIdleLine1Text;
    TAknLayoutText textsIdleLine2Text;

    texts1LongText.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_1( 0 ) );
    texts1ShortText.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_1( 1 ) );
    texts2LongLine1Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_2(0, 0));
    texts2ShortLine1Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_2(1, 0));
    texts2LongLine2Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_2(0, 1));
    texts2ShortLine2Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_2(1, 1));
    textsIdleLine1Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_3( 1 ) );
    textsIdleLine2Text.LayoutText( numberEntryRect, 
        BubbleLayout::popup_number_entry_window_texts_3( 0 ) );               

    // although only specified for two line case, we need to have 
    // some kind of value for the one line case but as there is 
    // only one line, it isn't used.
    TInt baselineSeparation = texts2LongLine2Text.TextRect().iBr.iY 
                            - texts2LongLine1Text.TextRect().iBr.iY;
    TInt currentFormat = iEditor->CurrentFormatIndex();

    for (TInt ii = 0; ii < iFormatCount; ii++)
        {
        CAknPhoneNumberEditor::TFormat format( iEditor->Format( ii ) );

        // override the format values using the layout data
        TInt next = ii;
        TBool noStatusPane = EFalse;
        if( ii > KBubbleNEIdleStateFormat )
            {
            next -= KBubbleNEFormatOffsetIfNoStatusPane;
            noStatusPane = ETrue;
            }

        const CAknLayoutFont* layoutFont;
        switch( next )
            {
            case KBubbleNE1LongLineFormat:
                format.iFont = texts1LongText.Font();
                layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( format.iFont);
                format.iBottomBaselineOffset = layoutFont->BaselineToTextPaneBottom();
                format.iOuterRect = texts1LongText.TextRect(); 
                break;
            case KBubbleNE2LongLineFormat:
                format.iFont = texts2LongLine1Text.Font();
                layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( format.iFont);
                format.iBottomBaselineOffset = layoutFont->BaselineToTextPaneBottom();
                format.iOuterRect = texts2LongLine2Text.TextRect();
                format.iOuterRect.iTl.iY -= format.iBottomBaselineOffset; // have to add extra gap
                format.iOuterRect.iTl.iY -= baselineSeparation; // have to add extra lines.
                break;
            case KBubbleNE1ShortLineFormat:
                format.iFont = texts1ShortText.Font();
                layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( format.iFont);
                format.iBottomBaselineOffset = layoutFont->BaselineToTextPaneBottom();
                format.iOuterRect = texts1ShortText.TextRect();
                break;
            case KBubbleNE2ShortLineFormat:
                format.iFont = texts2ShortLine1Text.Font();
                layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( format.iFont);
                format.iBottomBaselineOffset = layoutFont->BaselineToTextPaneBottom();
                format.iOuterRect = texts2ShortLine2Text.TextRect();
                format.iOuterRect.iTl.iY -= format.iBottomBaselineOffset; // have to add extra gap
                format.iOuterRect.iTl.iY -= baselineSeparation; // have to add extra lines.
                break;
            case KBubbleNEIdleStateFormat:
                format.iFont = textsIdleLine1Text.Font();
                layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( format.iFont);
                format.iBottomBaselineOffset = layoutFont->BaselineToTextPaneBottom();

                format.iOuterRect = textsIdleLine2Text.TextRect();
                format.iOuterRect.iTl.iY -= format.iBottomBaselineOffset; // have to add extra gap
                
                baselineSeparation = textsIdleLine2Text.TextRect().iBr.iY 
                    - textsIdleLine1Text.TextRect().iBr.iY;
                
                format.iOuterRect.iTl.iY -= baselineSeparation; // have to add extra lines.
                break;                
            };

        if(noStatusPane)
            {
            format.iOuterRect.iTl.iY += BubbleLayout::MainPaneRect().iTl.iY;
            format.iOuterRect.iBr.iY += BubbleLayout::MainPaneRect().iTl.iY;
            }
        format.iBaselineSeparation = baselineSeparation;

        iEditor->Format( ii ) = format;
        }

    iEditor->SetFormat( currentFormat );
    UpdateEditorColor( color );
    }


// ---------------------------------------------------------------------------
//  CBubbleOutlookNumberEntry::ChangeEditorMode
//  
// 
// ---------------------------------------------------------------------------
//
TInt CBubbleOutlookNumberEntry::ChangeEditorMode( TBool aDefaultMode )
    {
    return iEditor->ChangeEditorMode( aDefaultMode );
    }
    
// ---------------------------------------------------------------------------
//  CBubbleOutlookNumberEntry::GetEditorMode
//  
// 
// ---------------------------------------------------------------------------
//
TInt CBubbleOutlookNumberEntry::GetEditorMode() const
    {
    return iEditor->GetEditorMode();
    }
    
// ---------------------------------------------------------------------------
//  CBubbleOutlookNumberEntry::ResetEditorToDefaultValues
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::ResetEditorToDefaultValues()
    {
    iEditor->ResetEditorToDefaultValues();
    }

// -----------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::UpdateEditorColor
// -----------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::UpdateEditorColor( const TRgb aColor )
    {
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRgb skinColor = aColor;

    AknsUtils::GetCachedColor( 
        skin, 
        skinColor, 
        KAknsIIDQsnTextColors,
        EAknsCIQsnTextColorsCG51 );

    // Ignore error
    TRAP_IGNORE( iEditor->OverrideColorL( EColorControlText, skinColor ) );
    }

// -----------------------------------------------------------------------------
// CBubbleOutlookNumberEntry::UpdateAndDrawEditor
// -----------------------------------------------------------------------------
//
void CBubbleOutlookNumberEntry::UpdateAndDrawEditor()
    {
    TInt format = 0;
      
      if ( iPlace == ENEBottom )
          {
          if ( iEditor->CountFormats() < 2 )
              {
              return;
              }
          format = KBubbleNE1LongLineFormat;
          if ( !iEditor->WouldTextFitInFormat( format ) )
              {
              format = KBubbleNE2LongLineFormat;
              }
          }
      else if ( iPlace == ENEBottomRight )
          {
          if ( iEditor->CountFormats() < 4 )
              {
              return;
              }
          format = KBubbleNE1ShortLineFormat;
          if ( !iEditor->WouldTextFitInFormat( KBubbleNE1ShortLineFormat ) )
              {
              format = KBubbleNE2ShortLineFormat;
              }
          }
      else if ( iPlace == ENEIdleState )
          {
          if ( iEditor->CountFormats() < 9 )
              {
              return;
              }
          format = KBubbleNEIdleStateFormat;
          }

      // If there is no status pane the relative coordinates editor must be
      // moved 44 pixels downwards, because editor's coordinates are absolute - 
      // NOT relative to parent rect.
      if ( iStatusPane && !iStatusPane->IsVisible() &&
           !AknStatuspaneUtils::StaconPaneActive() )
          {
          format += KBubbleNEFormatOffsetIfNoStatusPane;
          }

      if (iEditor->CurrentFormatIndex() != format)
          {
          iEditor->SetFormat(format);
          if ( iBubble )
              {
              iEditor->DrawNow();
              if ( iMutedImage.IsVisible() )
                  {
                  iMutedImage.DrawNow();
                  }
              }
          }
    }


// End of File
