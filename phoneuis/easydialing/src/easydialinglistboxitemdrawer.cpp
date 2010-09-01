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
* Description:  Easy dialing listbox list item drawer.
*
*/


// INCLUDE FILES

#include "easydialinglistboxitemdrawer.h"
#include "easydialinglistboxdata.h"
#include "easydialinglistbox.h"
#include "easydialinglistboxview.h"

#include <aknlistloadertfx.h>
#include <aknlistboxtfxinternal.h>
#include <aknlistboxtfx.h>
#include <akntransitionutils.h>
#include <AknUtils.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

const TInt KTextColorFallBack = 215;
const TInt KMatchingTextColorFallBack  = 0;
const TInt KMatchingTextBackgroundFallBack = 243;

// FORWARD DECLARATIONS

/*
 * ==============================================================================
 * 
 * 
 * class CEasyDialingListBoxItemDrawer
 * 
 * 
 * ==============================================================================
 */

// -----------------------------------------------------------------------------
// CEasyDialingListBoxItemDrawer
// Constructor
// -----------------------------------------------------------------------------
//
CEasyDialingListBoxItemDrawer::CEasyDialingListBoxItemDrawer(
        MTextListBoxModel* aTextListBoxModel, 
        const CFont* aFont, 
        CFormattedCellListBoxData* aFormattedCellData) :
        CFormattedCellListBoxItemDrawer( aTextListBoxModel, aFont, aFormattedCellData)
    {
    
    }


// -----------------------------------------------------------------------------
// EasyDialingCellData
// 
// -----------------------------------------------------------------------------
//       
CEasyDialingListBoxData* CEasyDialingListBoxItemDrawer::EasyDialingCellData() const
    {
    return STATIC_CAST( CEasyDialingListBoxData*, iData );
    }


// -----------------------------------------------------------------------------
// DrawItem
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBoxItemDrawer::DrawItem( 
    TInt aItemIndex, 
    TPoint aItemRectPos, 
    TBool aItemIsSelected, 
    TBool aItemIsCurrent, 
    TBool aViewIsEmphasized, 
    TBool aViewIsDimmed) const 
    {
    CFormattedCellListBoxItemDrawer::DrawItem( aItemIndex, aItemRectPos, aItemIsSelected, aItemIsCurrent, aViewIsEmphasized, aViewIsDimmed);  
    }


// -----------------------------------------------------------------------------
// DrawItemText
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBoxItemDrawer::DrawItemText( 
    TInt aItemIndex,
    const TRect& aItemTextRect,
    TBool aItemIsCurrent,
    TBool aViewIsEmphasized,
    TBool /* aItemIsSelected */) const
    {
    MAknListBoxTfxInternal* transApi = CAknListLoader::TfxApiInternal( iGc );
    if ( transApi )
        {
        transApi->StartDrawing( MAknListBoxTfxInternal::EListNotSpecified );
        }
        
    iGc->SetPenColor(iTextColor);
    iGc->SetBrushColor(iBackColor);

    TPtrC temp = iModel->ItemText(aItemIndex);

    SetupGc(aItemIndex);
    if ( transApi )
        {
        transApi->StopDrawing();
        }

    CEasyDialingListBoxData::TExtendedColors colors;
    colors.iText = iTextColor;
    colors.iBack = iBackColor;
    colors.iHighlightedText = iHighlightedTextColor;
    colors.iHighlightedBack = iHighlightedBackColor;
    colors.iMatchingText = iMatchingText;
    colors.iMatchingBack = iMatchingBack;

    DrawBackgroundAndSeparatorLines( aItemTextRect );

    TBool highlightShown = ETrue;
    if ( ( FormattedCellData()->RespectFocus() && !aViewIsEmphasized ) )
        {
        if ( transApi )
            {
            transApi->Remove( MAknListBoxTfxInternal::EListHighlight );
            }

        highlightShown = EFalse;
        }
    
    // normal drawing without mark icon
    EasyDialingCellData()->DrawData(
            Properties(aItemIndex),
            *iGc,
            &temp,
            aItemTextRect,
            aItemIsCurrent && highlightShown,
            colors );
    
    if ( aItemTextRect.iBr.iY < iViewRect.iBr.iY && aItemIndex < iModel->NumberOfItems() - 1 )
        {
        if ( transApi )
            {
            transApi->StartDrawing( MAknListBoxTfxInternal::EListItem );
            }
        
        AknListUtils::DrawSeparator( *iGc, aItemTextRect, iTextColor );

        if ( transApi )
            {
            transApi->StopDrawing();
            }
        }
    }
        

// -----------------------------------------------------------------------------
// SetColors
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBoxItemDrawer::SetColors()
    {
    MAknsSkinInstance* skin = AknsUtils::SkinInstance(); 
    
    if ( !skin ) 
        {
        // These values are copied from "aknlist.cpp".
        // This branch should be never take, and can be considered protective programming.
        iTextColor = AKN_LAF_COLOR_STATIC( KTextColorFallBack);
        iHighlightedTextColor = AKN_LAF_COLOR_STATIC( KTextColorFallBack );
        iMatchingText = AKN_LAF_COLOR_STATIC( KMatchingTextColorFallBack );
        iMatchingBack = AKN_LAF_COLOR_STATIC( KMatchingTextBackgroundFallBack );
        }
    
    else 
        {
        AknsUtils::GetCachedColor( skin,
                iTextColor,
                KAknsIIDQsnTextColors,
                EAknsCIQsnTextColorsCG7 );
        
        AknsUtils::GetCachedColor( skin,
                iHighlightedTextColor,
                KAknsIIDQsnTextColors,
                EAknsCIQsnTextColorsCG10 );
        
        AknsUtils::GetCachedColor( skin,
                iMatchingText,
                KAknsIIDQsnTextColors,
                EAknsCIQsnTextColorsCG24 );
        
        AknsUtils::GetCachedColor( skin,
                iMatchingBack,
                KAknsIIDQsnHighlightColors,
                EAknsCIQsnHighlightColorsCG2 );
        }
    }



// -----------------------------------------------------------------------------
// DrawBackgroundAndSeparatorLines
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBoxItemDrawer::DrawBackgroundAndSeparatorLines( const TRect& aItemTextRect ) const
    {
    CCoeControl* control = FormattedCellData()->Control();

    MAknListBoxTfxInternal* transApi = CAknListLoader::TfxApiInternal( iGc );
    
    if ( transApi && !transApi->EffectsDisabled() )
        {
        MAknListBoxTfx* tfxApi = CAknListLoader::TfxApi( iGc );

        if ( tfxApi )
            {
            tfxApi->EnableEffects( ETrue );
            }
        }

    if ( transApi )
        {
        transApi->StartDrawing( MAknListBoxTfxInternal::EListView );
        }
    TBool bgDrawn( EFalse );
    
    if ( control )
        {        
        MAknsControlContext *cc = AknsDrawUtils::ControlContext( control );
                
        bgDrawn = AknsDrawUtils::DrawBackground( 
                AknsUtils::SkinInstance(), 
                cc, 
                control, 
                *Gc(), 
                aItemTextRect.iTl, 
                aItemTextRect, 
                KAknsDrawParamDefault );
        }
    
    if ( !bgDrawn )
        {
        iGc->Clear( aItemTextRect );
        }
    if ( transApi )
        {
        transApi->StopDrawing();
        }
    }

// end of file
