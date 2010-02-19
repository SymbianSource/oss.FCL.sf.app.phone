/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Easy dialing test application.
*
*/

// INCLUDE FILES
#include "edta_screentextbuffer.h"
#include "edta_debugprint.h"
#include "edta.pan"

#include <coemain.h>
#include <aknsbasicbackgroundcontrolcontext.h>
#include <eiksbfrm.h>
#include <eikenv.h>
#include <eikdef.h>
#include <aknappui.h>
#include <aknutils.h>


const TInt KLineWidthCent = 90; // Percent of total screen width
const TInt KHeightMarginCent = 7; // Percent of total height
const TInt KBottomMarginCent = 3; // Percent of total height
const TInt KLineHeightCent = 135; // How many percent is the line height of font height
const TInt KCent = 100;
const TInt KInitialScreenBufferSize = 20; // This many lines is initially allocated to the screen buffer.
const TInt KInitialScreenStartAmount = 5; // This many screen starts are initially allocated to screen buffer.


CEdtaScreenTextBuffer* CEdtaScreenTextBuffer::NewL( const TRect& aRect )
    {
    CEdtaScreenTextBuffer* self = CEdtaScreenTextBuffer::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

CEdtaScreenTextBuffer* CEdtaScreenTextBuffer::NewLC( const TRect& aRect )
    {
    CEdtaScreenTextBuffer* self = new ( ELeave ) CEdtaScreenTextBuffer;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// constructors
CEdtaScreenTextBuffer::CEdtaScreenTextBuffer()
    {
    }

void CEdtaScreenTextBuffer::ConstructL( const TRect& aRect )
    {
    CreateWindowL();
    CalculateL(aRect);
    ActivateL();
    }

// destructor
CEdtaScreenTextBuffer::~CEdtaScreenTextBuffer()
    {
    delete iSBFrame;
    delete iScreenStarts;

    if ( iText )
        {
        iText->ResetAndDestroy();
        delete iText;
        }
    }

void CEdtaScreenTextBuffer::SizeChanged()
    {
    // TRAP_IGNORE( CalculateL( Rect() ) );
    // DrawDeferred();
    }


// -----------------------------------------------------------------------------
// CEdtaScreenTextBuffer::Draw()
// -----------------------------------------------------------------------------
void CEdtaScreenTextBuffer::Draw( const TRect& aRect ) const
    {
	// DebugPrintF(_L("CEdtaScreenTextBuffer::Draw, rect: %d, %d - %d, %d"), aRect.iTl.iX, aRect.iTl.iY, aRect.iBr.iX, aRect.iBr.iY );

    __ASSERT_DEBUG( iText, Panic(EEdtaFieldNotInitialized,_L("CEdtaScreenTextBuffer::Draw: iText == NULL") ));
    __ASSERT_DEBUG( iScreenStarts, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::Draw: iScreenStarts == NULL") ));
    __ASSERT_DEBUG( iScreenStarts->Count() > 0, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::Draw: iScreenStarts has zero elements") ));

    CWindowGc& gc = SystemGc();

    gc.SetBrushColor( KRgbWhite );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.Clear( aRect );
    
    if ( IsFocused() )
        {
        TRect frame( aRect );
        frame.Shrink(3,3);
        frame.iBr.iX -= 10;
        gc.SetPenColor(KRgbBlack);
        gc.SetPenSize( TSize(2,2) );
        TSize cornerRounding(8,8);
        gc.DrawRoundRect( frame, cornerRounding );
        }
    
    gc.UseFont( iFont );

    // index of the first line on the screen in the text array
    TInt firstLine = 0 ;
    if((iScreenStarts ) && (iScreenStarts->Count() > iCurrentScreen))
        {
        firstLine = ( (*iScreenStarts)[ iCurrentScreen ] );
        }
    else
        {
        firstLine = (*iScreenStarts)[ iScreenStarts->Count() - 1 ];
        }

    // index of the last line on the screen in the text array
    TInt lastLine( firstLine + iLinesPerScreen - 1 );

    gc.SetBrushStyle( CGraphicsContext::ENullBrush );

    TPoint position( iTopBaseLineX, iTopBaseLineY );
    TPoint topLeft;
    TSize rectSize( iLineWidth, iBaseLineDelta +iFont->DescentInPixels() );

    for ( TInt index = firstLine; index < iText->Count() && index <= lastLine; index++, position.iY += iBaseLineDelta )
        {
        HBufC* text = (*iText)[ index ];

        if ( text )
            {
            topLeft = TPoint( position.iX, position.iY-iBaseLineDelta );
            gc.SetPenColor(KRgbBlack);
            gc.DrawText( *text, TRect( topLeft, rectSize ), iBaseLineDelta, iTextAlign );
            }
        }

    gc.DiscardFont();

    // DebugPrintF(_L("CEdtaScreenTextBuffer::Draw exit"));
    }


// -----------------------------------------------------------------------------
// CEdtaScreenTextBuffer::ActivateL()
// -----------------------------------------------------------------------------
void CEdtaScreenTextBuffer::ActivateL()
    {
    CCoeControl::ActivateL();
    UpdateScrollIndicatorL();
    }


// -----------------------------------------------------------------------------
// CEdtaScreenTextBuffer::SetTextL()
// -----------------------------------------------------------------------------
void CEdtaScreenTextBuffer::WriteLineL(TRefByValue<const TDesC> aText,... )
    {
    TBuf<KEdtaMaxLineWidth> bufLine;
    VA_LIST list;
    VA_START(list,aText);
    bufLine.FormatList(aText,list);

    if(!bufLine.Length())
        {
        iText->AppendL( NULL );
        }
    else
        {
        HBufC* line = HBufC::NewLC(bufLine.Length());
        *line = bufLine;
        iText->AppendL( line );
        CleanupStack::Pop(line);
        }

    // Updating scrollbars and screen starts etc are moved to UpdateScrollIndicatorL()

    // if text, last line is shown again in next screen
    iDoNotShowLastLineAgain = EFalse;

    DrawDeferred(); //New
    UpdateScrollIndicatorL();
    }


// -----------------------------------------------------------------------------
// CEdtaScreenTextBuffer::DeleteLastLine()
// -----------------------------------------------------------------------------
void CEdtaScreenTextBuffer::DeleteLastLine(TInt aCount)
    {
    __ASSERT_DEBUG( iText, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::DeleteLastLine: iText == NULL") ));
    __ASSERT_DEBUG( iText->Count() > 0, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::DeleteLastLine: iText has zero elements") ));

    while (aCount > 0)
        {
        // Avoiding memory leaks
        TInt lastIndex = iText->Count() - 1;

        // The first item in iText cannot be deleted, since UpdateScrollIndicatorL
        // relies on there being at least one item.
        if (lastIndex > 0)
            {
            HBufC* text = (*iText)[ lastIndex ];
            delete text;
            iText->Delete(lastIndex);
            }
        --aCount;
        }
    }



// -----------------------------------------------------------------------------
// CEdtaScreenTextBuffer::OfferKeyEventL()
// -----------------------------------------------------------------------------
TKeyResponse CEdtaScreenTextBuffer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                              TEventCode aType )
    {
    __ASSERT_DEBUG(iScreenStarts, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::OfferKeyEventL: iScreenStarts == NULL") ));
    __ASSERT_DEBUG(iScreenStarts->Count() > 0, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::OfferKeyEventL: iScreenStarts has zero elements") ));

    if ( aType == EEventKey && iScreenStarts->Count() > 1 )
        {
        switch ( aKeyEvent.iCode )
            {
            case EKeyUpArrow:
                if ( iCurrentScreen > 0 )
                    {
                    iCurrentScreen--;
                    DrawNow();
                    UpdateScrollIndicatorL();
                    }
                break;

            case EKeyDownArrow:
                if ( iCurrentScreen < iScreenStarts->Count() - 1 )
                    {
                    iCurrentScreen++;
                    DrawNow();
                    UpdateScrollIndicatorL();
                    }
                break;

            default:
                return EKeyWasNotConsumed;
            }
        }

    return EKeyWasConsumed;
    }



// -----------------------------------------------------------------------------
// CEdtaScreenTextBuffer::FocusChanged()
// -----------------------------------------------------------------------------
void CEdtaScreenTextBuffer::FocusChanged(TDrawNow aDrawNow)
    {
    if ( aDrawNow == EDrawNow )
        {
        DrawNow();
        }
    }


// -----------------------------------------------------------------------------
// CEdtaScreenTextBuffer::HandleScrollEventL()
// -----------------------------------------------------------------------------
void CEdtaScreenTextBuffer::HandleScrollEventL ( CEikScrollBar* aScrollBar, TEikScrollEvent aEventType)
    {
    //Only on page up/down,scroll up/down and drag events
    switch (aEventType)
        {
        case EEikScrollPageDown:
        case EEikScrollPageUp:
        case EEikScrollThumbDragVert:
        case EEikScrollUp:
        case EEikScrollDown:

            iCurrentScreen = aScrollBar->ThumbPosition();

            //Refresh now
            DrawNow();
            UpdateScrollIndicatorL();
            break;
        }
    }

// -----------------------------------------------------------------------------
// CEdtaScreenTextBuffer::UpdateScrollIndicatorL()
//
// This function assumes that
// - iText exists and has at least one element.
// - iScreenStarts exist and has at least one element.
// These conditions are met if CalculateL / ConstructL has been called.
// -----------------------------------------------------------------------------
void CEdtaScreenTextBuffer::UpdateScrollIndicatorL()
    {
    __ASSERT_DEBUG( iText, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::UpdateScrollIndicatorL: iText == NULL") ));
    __ASSERT_DEBUG( iText->Count() > 0, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::UpdateScrollIndicatorL: iText has zero elements") ));
    __ASSERT_DEBUG( iScreenStarts, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::UpdateScrollIndicatorL: iScreenStarts == NULL") ));
    __ASSERT_DEBUG( iScreenStarts->Count() > 0, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::UpdateScrollIndicatorL: iScreenStarts has zero elements") ));

    TInt lastLine( iText->Count() - 1 );
    TInt screenStart( (*iScreenStarts)[ iScreenStarts->Count() - 1 ] );

    TBool firstNewScreenHandled( EFalse );

    while ( lastLine >= screenStart + iLinesPerScreen )
        {
        if ( !firstNewScreenHandled && iDoNotShowLastLineAgain )
            {
            screenStart++;
            firstNewScreenHandled = ETrue;
            }

        // If the shows screen is the last on, scroll the view along with new lines.
        if (iCurrentScreen == iScreenStarts->Count() - 1)
            {
            iCurrentScreen++;
            }

        screenStart += iLinesPerScreen - 1;
        iScreenStarts->AppendL( screenStart );
        }

    if ( !iSBFrame )
        {
        iSBFrame = new( ELeave ) CEikScrollBarFrame( this, NULL, ETrue );

        iSBFrame->CreateDoubleSpanScrollBarsL(ETrue, EFalse); // non-window owning scrollbar
        iSBFrame->SetTypeOfVScrollBar(CEikScrollBarFrame::EDoubleSpan);

        iSBFrame->SetScrollBarFrameObserver(this);
        iSBFrame->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EAuto);
        }

    TEikScrollBarModel vSbarModel;
    vSbarModel.iThumbPosition = iCurrentScreen;
    vSbarModel.iScrollSpan = iScreenStarts->Count();
    vSbarModel.iThumbSpan = 1;

    TRect rect(Rect());
    TEikScrollBarFrameLayout layout;
    layout.iTilingMode = TEikScrollBarFrameLayout::EInclusiveRectConstant;

    // For EDoubleSpan type scrollbar
    if (vSbarModel.iThumbPosition + vSbarModel.iThumbSpan > vSbarModel.iScrollSpan)
        {
        // Not let scrollbar values overflow
        vSbarModel.iThumbPosition = vSbarModel.iScrollSpan - vSbarModel.iThumbSpan;
        }

    TAknDoubleSpanScrollBarModel vDsSbarModel(vSbarModel);
    iSBFrame->TileL(NULL, &vDsSbarModel, rect, rect, layout);
    iSBFrame->SetVFocusPosToThumbPos(vDsSbarModel.FocusPosition());
    }


// -----------------------------------------------------------------------------
// CEdtaScreenTextBuffer::CalculateL()
// Needs to be called in construction, and whenever layout changes.
// Screen orientation change, etc.
// -----------------------------------------------------------------------------
void CEdtaScreenTextBuffer::CalculateL(const TRect& aRect)
    {
    // DebugPrintF(_L("CEdtaScreenTextBuffer::CalculateL, rect: %d, %d - %d, %d"), aRect.iTl.iX, aRect.iTl.iY, aRect.iBr.iX, aRect.iBr.iY  );

    TRect rect(0,0,0,0);

    iCurrentScreen = 0;

    iFont = AknLayoutUtils::FontFromId(EAknLogicalFontSecondaryFont);

    // Calculate various text positioning parameters
    iBaseLineDelta = iFont->HeightInPixels() * KLineHeightCent / KCent;

    TInt mainPaneWidth( aRect.iBr.iX - aRect.iTl.iX );
    TInt mainPaneHeight( aRect.iBr.iY - aRect.iTl.iY );
    // Line width is 90% (by default) of client rect, and horizontal margins 10%
    iLineWidth = mainPaneWidth * KLineWidthCent / KCent;

    iTopBaseLineX = ( mainPaneWidth - iLineWidth ) / 2;

    // top margin is 7% (by default) of the client rect
    TInt topMargin = mainPaneHeight * KHeightMarginCent / KCent;
    iTopBaseLineY = topMargin + iFont->AscentInPixels();

    // minimum bottom margin is 3% (by default) of the client rect
    TInt bottomMargin = mainPaneHeight * KBottomMarginCent / KCent;
    iLinesPerScreen =
        ( mainPaneHeight - topMargin - bottomMargin ) / iBaseLineDelta;

    iTextAlign = CGraphicsContext::ELeft;

    // Text must not be deleted, otherwise all accumulated data will be lost.
    if ( !iText )
        {
        // Every text line on screen is one entry in this array
        iText = new( ELeave ) CArrayPtrFlat<HBufC>( KInitialScreenBufferSize );
        iText->AppendL(NULL); // Extra newline for visual reasons
        }

     if ( iScreenStarts )
        {
        delete iScreenStarts;
        iScreenStarts = NULL;
        }


    // This array contains indices for lines that start the subsequent
    // screens, for custom scrolling
    iScreenStarts = new( ELeave ) CArrayFixFlat<TInt>( KInitialScreenStartAmount );
    // Initialisation: first screen starts at line 0.
    iScreenStarts->AppendL( 0 );

    if ( iSBFrame )
        {
        delete iSBFrame;
        iSBFrame = NULL;
        }

    UpdateScrollIndicatorL();
    }


// -----------------------------------------------------------------------------
// CEdtaScreenTextBuffer::HandleResourceChange()
// -----------------------------------------------------------------------------
void CEdtaScreenTextBuffer::HandleResourceChange(TInt aType)
    {
    switch ( aType )
        {
        case KEikDynamicLayoutVariantSwitch :
            {
            TRAPD( error, CalculateL( iAvkonAppUi->ClientRect() ));

            if( error != KErrNone )
                {
                DebugPrintF(_L("CEdtaScreenTextBuffer::HandleResourceChange: CalculateL -method failed: %d"), error );
                }
            else
                {
                DrawNow();
                }
            }
            break;

        case KAknsMessageSkinChange :
            {
            __ASSERT_DEBUG(iSBFrame, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::HandleResourceChange: iSBFrame == NULL") ));

            TRAPD( error,
                {
                iSBFrame->CreateDoubleSpanScrollBarsL(ETrue, EFalse); // window owning scrollbar
                iSBFrame->SetTypeOfVScrollBar(CEikScrollBarFrame::EDoubleSpan);
                iSBFrame->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EOn);
                UpdateScrollIndicatorL();
                }
            );

            if( error != KErrNone )
                {
                DebugPrintF(_L("CEdtaScreenTextBuffer::HandleResourceChange: Create ScrollBars failed: %d"), error );
                __ASSERT_DEBUG( error == KErrNone, Panic(EEdtaFieldNotInitialized, _L("CEdtaScreenTextBuffer::HandleResourceChange => Skin change caused panic") ));
                }
            }

        default:
            {
            CCoeControl::HandleResourceChange(aType);
            }
            break;
        }
    }


// ---------------------------------------------------------
// CEdtaScreenTextBuffer::MopSupplyObject()
// Pass skin information if need.
// ---------------------------------------------------------
TTypeUid::Ptr CEdtaScreenTextBuffer::MopSupplyObject(TTypeUid aId)
    {
    return CCoeControl::MopSupplyObject(aId);
    }


// End of file
