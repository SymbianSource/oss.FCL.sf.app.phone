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
* Description:  Easy dialing list box.
*
*/


// INCLUDE FILES

#include "easydialinglistbox.h"
#include "easydialinglistboxview.h"
#include "easydialinglistboxitemdrawer.h"
#include "easydialinglistboxdata.h"
#include "easydialingpanics.h"
#include "easydialingcontactdatamanager.h"
#include "easydialingcrkeys.h"

#include <easydialingpluginresources.rsg>

#include <phoneappcommands.hrh>
#include <phonebook2.mbg>
#include <phonebook2ece.mbg>

#include <ccappcommlauncherpluginrsc.rsg>

// AvKON and drawing header files

#include <aknpointereventsuppressor.h> 
#include <aknlongtapdetector.h> // Required for touch
#include <AknsFrameBackgroundControlContext.h>
#include <AknInfoPopupNoteController.h>  // tooltips

#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>
#include <gulicon.h>
#include <centralrepository.h>
#include <bautils.h>    // for BaflUtils

#include <aknlistloadertfx.h>
#include <aknlistboxtfxinternal.h>
#include <aknlistboxtfx.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

extern TRect ContactImageBoundingBox( const TRect& aItemRect );
extern TRect ActionMenuIconBoundingBox(const TRect& aItemRect);

// CONSTANTS
static const TInt KListBoxMarginWidth = 2;
static const TInt KMaxVisibleItemsPortrait = 3;
static const TInt KMaxVisibleItemsLandscape = 2;

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


/*
 * ==============================================================================
 * 
 * 
 * class CEasyDialingListBox
 * 
 * 
 * ==============================================================================
 */


// -----------------------------------------------------------------------------
// CEasyDialingListBox
// 
// -----------------------------------------------------------------------------
//
CEasyDialingListBox::CEasyDialingListBox() :
    CEikFormattedCellListBox()
    {
    }

// -----------------------------------------------------------------------------
// ~CEasyDialingListBox
// 
// -----------------------------------------------------------------------------
//
CEasyDialingListBox::~CEasyDialingListBox()
    {
    delete iBGContext;    
    delete iLongTapDetector;
    }

// -----------------------------------------------------------------------------
// ConstructL
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBox::ConstructL( TInt aFlags, 
        CEasyDialingContactDataManager* aContactDataManager )
    {
    CEikFormattedCellListBox::ConstructL( NULL, aFlags );
    
    iLongTapDetector = CAknLongTapDetector::NewL( this );
 
    CEasyDialingListBoxItemDrawer* drawer = static_cast<CEasyDialingListBoxItemDrawer*> (iItemDrawer);
    CEasyDialingListBoxData* data = drawer->EasyDialingCellData();
    data->SetContactDataManager(aContactDataManager);
    data->SetEDLBXControl(this);
    
    iBGContext = CAknsFrameBackgroundControlContext::NewL( KAknsIIDQsnFrPopupSub,
            Rect(), Rect(), EFalse );
       
    // ContactDataManager is accessed from SizeChanged. If list item size
    // changes -> thumbnail size changes
    iContactDataManager = aContactDataManager;
    
    drawer->SetColors();

    CreateScrollBarFrameL( ETrue );
    ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );
    }


// -----------------------------------------------------------------------------
// MakeViewClassInstanceL
// 
// -----------------------------------------------------------------------------
//
CListBoxView* CEasyDialingListBox::MakeViewClassInstanceL()
    {
    return (new ( ELeave ) CEasyDialingListBoxView);
    }


// -----------------------------------------------------------------------------
// ItemDrawer
// 
// -----------------------------------------------------------------------------
//
CEasyDialingListBoxItemDrawer* CEasyDialingListBox::ItemDrawer() const
    {
    return (CEasyDialingListBoxItemDrawer*) iItemDrawer;
    }


// -----------------------------------------------------------------------------
// FocusChanged
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBox::FocusChanged( TDrawNow aDrawNow )
    {
    if ( !IsFocused() )
        {
        CEasyDialingListBoxView* view = static_cast<CEasyDialingListBoxView*>( iView );
        view->SetCurrentItemIndexToNone();
        
        // Invalidate window area. 
        // This fixes the drawing problem that the top 
        // of listbox is not always redrawn on the area that overlaps 
        // the status pane area. 
        // TODO: This overlapping no longer happens in 9.2 so this may
        // now be unnecessary.
        Window().Invalidate();
        }
    CEikFormattedCellListBox::FocusChanged( aDrawNow );
    }


// -----------------------------------------------------------------------------
// CreateItemDrawerL
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBox::CreateItemDrawerL()
    {
    CEasyDialingListBoxData* celldata = CEasyDialingListBoxData::NewL();
    CleanupStack::PushL( celldata );
    iItemDrawer = new(ELeave) CEasyDialingListBoxItemDrawer(Model(), iEikonEnv->NormalFont(), celldata);
    CleanupStack::Pop( celldata );  
    }


// -----------------------------------------------------------------------------
// Draw
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBox::Draw(const TRect& aRect) const
    {
    MAknListBoxTfxInternal *transApi = CAknListLoader::TfxApiInternal( iItemDrawer->Gc() );
    if ( transApi )
        {
        transApi->SetListType( MAknListBoxTfxInternal::EListBoxTypeMainPane );
        transApi->BeginRedraw( MAknListBoxTfxInternal::EListView, this->Rect() );
        }
    
    if (!iView->RedrawDisabled() )
        {
        TRect clientRect( Rect() );
        TRect viewRect = iView->ViewRect();
        
        if ( transApi )
            {
            transApi->StartDrawing( MAknListBoxTfxInternal::EListView );
            }

        AknsDrawUtils::BackgroundBetweenRects( 
            AknsUtils::SkinInstance(), 
            iBGContext, 
            this, 
            *iItemDrawer->Gc(), 
            clientRect, 
            viewRect );
        
        if ( transApi )
            {
            transApi->StopDrawing();
            }
        }
    
    if ( iModel->NumberOfItems() )
        {
        // Draw the actual list
        iView->Draw(&aRect);
        }
    if ( transApi )
        {
        transApi->EndViewRedraw( aRect );
        }
    }


// -----------------------------------------------------------------------------
// HandleResourceChange
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBox::HandleResourceChange(TInt aType)
    {
    CEikFormattedCellListBox::HandleResourceChange( aType );
    iSBFrame->VerticalScrollBar()->HandleResourceChange( aType );
    iSBFrame->DrawScrollBarsNow();
    ItemDrawer()->SetColors();
    
    // Base call to HandleResourceChange justifies the list to even items
    // on the top. This causes problems when listbox height is not a multiple of
    // listitem heights. Fix scrolling manually for these cases.
    TRAP_IGNORE( HandleItemAdditionL() ); // needed to prevent drawing problems
    TInt scrollIndex = ( IsFocused() ? CurrentItemIndex() : iNumberOfNames - 1 );
    ScrollToMakeItemVisible( scrollIndex );
    
    DrawDeferred();
    }


// -----------------------------------------------------------------------------
// SizeChanged
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBox::SizeChanged()
    {
    CEikFormattedCellListBox::SizeChanged();
    
    // Set the listbox colors.
    // For some reason, calling this in HandleResourceChange is not enough, it does
    // not get called in situation it should.
    ItemDrawer()->SetColors();
   
    // resize scroll bar
    if ( iSBFrame )
        {
        TAknLayoutRect rect;
        TInt variety = ( Layout_Meta_Data::IsLandscapeOrientation() ? 1 : 0 );
    
        TAknWindowComponentLayout viewLayout( AknLayoutScalable_Apps::dia3_list_pane( variety ) );
        rect.LayoutRect( Rect(), viewLayout );
        TRect viewRect( rect.Rect() );
        // Add a bit of margin around the view as layout doesn't define any
        viewRect.Shrink( KListBoxMarginWidth, KListBoxMarginWidth );
        
        TAknWindowComponentLayout scrollBarLayout( AknLayoutScalable_Apps::scroll_pane_cp12() );
        rect.LayoutRect( Rect(), scrollBarLayout );
        TRect scrollBarRect( rect.Rect() );

        iView->SetViewRect( viewRect );
        
        CAknDoubleSpanScrollBar* scrollbar = static_cast <CAknDoubleSpanScrollBar*>( iSBFrame->VerticalScrollBar() );

        scrollbar->SetFixedLayoutRect( scrollBarRect );
        scrollbar->SetRect( scrollBarRect );
        
        TRect viewAndScrollBarRect( viewRect );
        
        if ( AknLayoutUtils::LayoutMirrored() )
            {
            viewAndScrollBarRect.iTl = scrollBarRect.iTl;
            }
        else
            {
            viewAndScrollBarRect.iBr = scrollBarRect.iBr;
            }
        iBGContext->SetFrameRects( Rect(), viewAndScrollBarRect );
        }
    }


// -----------------------------------------------------------------------------
// MopSupplyObject
// 
// -----------------------------------------------------------------------------
//
TTypeUid::Ptr CEasyDialingListBox::MopSupplyObject( TTypeUid aId )
    {
    if ( aId.iUid == MAknsControlContext::ETypeId && iBGContext )
        {
        return MAknsControlContext::SupplyMopObject( aId, iBGContext );
        }
    return CEikFormattedCellListBox::MopSupplyObject( aId );    
    }

// -----------------------------------------------------------------------------
// MakeVisible
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBox::MakeVisible( TBool aVisible )
    {
    if ( aVisible != IsVisible() )
        {
        CEikFormattedCellListBox::MakeVisible( aVisible );
        if ( iSBFrame )
            {
            CEikScrollBar* sb = iSBFrame->GetScrollBarHandle( CEikScrollBar::EVertical );
            if ( sb )
                {
                sb->MakeVisible( aVisible );
                if ( aVisible )
                    {
                    UpdateScrollBarThumbs();
                    }
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// SetMaxRect
//
// -----------------------------------------------------------------------------
//
void CEasyDialingListBox::SetMaxRect( TRect aMaxRect )
    {
    iMaxRect = aMaxRect;

    // Update the list item size according the maximum view rect size
    TInt variety = ( Layout_Meta_Data::IsLandscapeOrientation() ? 1 : 0 );
    TAknLayoutRect maxViewLayout;
    maxViewLayout.LayoutRect( aMaxRect, AknLayoutScalable_Apps::dia3_list_pane( variety ) );
    TRect maxViewRect( maxViewLayout.Rect() );
    maxViewRect.Shrink( KListBoxMarginWidth, KListBoxMarginWidth ); // layout data doens't include any margins but we have added some
    
    TInt maxItemsShown = ( variety ? KMaxVisibleItemsLandscape : KMaxVisibleItemsPortrait );
    TInt itemHeight = maxViewRect.Height() / maxItemsShown;
    TRAP_IGNORE( SetItemHeightL( itemHeight ) );

    // Calculate new thumbnail rect from item size
    TSize itemSize( maxViewRect.Width(), itemHeight );
    TRect thumbnailRect = ContactImageBoundingBox( itemSize );
    iContactDataManager->SetThumbnailSize( thumbnailRect.Size() ); // reloads thumbnails if the new size is different from old one

    // Inform listbox data class about changed list item size
    ItemDrawer()->EasyDialingCellData()->HandleItemSizeChange();
    }

// -----------------------------------------------------------------------------
// SetRectToNumberOfItems
// -----------------------------------------------------------------------------
//
void CEasyDialingListBox::SetRectToNumberOfItems( TInt aNumberOfNames ) 
    {
    iNumberOfNames = aNumberOfNames;

    TRect listboxRect( iMaxRect );
    
    // Make listbox smaller if maximum size is not needed to show
    // all list items.
    TInt requiredHeight = GetHeightBasedOnNumberOfItems( iNumberOfNames );
    if ( requiredHeight < listboxRect.Height() )
        {
        // Shrink rect height so that bottom of the rect doesn't move
        listboxRect.iTl.iY += ( listboxRect.Height() - requiredHeight );
        }
    
    SetRect( listboxRect );
    }



// -----------------------------------------------------------------------------
// SetFocusedWithKeyEventL
// -----------------------------------------------------------------------------
//
TKeyResponse CEasyDialingListBox::SetFocusedWithKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TKeyResponse keyResponse = EKeyWasNotConsumed;
    TInt keyCode = aKeyEvent.iCode;
            
    // Disable redrawing temporarily.
    // This is because OfferKeyEventL puts the focus to the wrong item,
    // and we don't want to show it flashing there.
    // Make sure that no leave happens before redraw is enabled again.
    iView->SetDisableRedraw( ETrue );
        
    TRAP_IGNORE( keyResponse = OfferKeyEventL( aKeyEvent, aType ) );
    
    TInt itemToFocus = ( keyCode == EKeyUpArrow ? iNumberOfNames - 1 : 0 );
    ScrollToMakeItemVisible( itemToFocus );
    SetCurrentItemIndex( itemToFocus );
        
    iView->SetDisableRedraw( EFalse );
    
    DrawDeferred();
    
    return keyResponse;
    }


// -----------------------------------------------------------------------------
// GetHeightBasedOnNumberOfItems
// 
// -----------------------------------------------------------------------------
//
TInt CEasyDialingListBox::GetHeightBasedOnNumberOfItems( TInt aNum ) const
    {
    return ( ItemHeight()*aNum + KListBoxMarginWidth*2 ); 
    }


// -----------------------------------------------------------------------------
// CurrentContactDataIndex
// 
// -----------------------------------------------------------------------------
//
TInt CEasyDialingListBox::CurrentContactDataIndex()
    {
    if ( CurrentItemIndex() >= 0 ) 
        {
        TPtrC itemText = Model()->ItemText( CurrentItemIndex() );
        TPtrC indexText;
        
        TInt error = TextUtils::ColumnText( indexText , 0, &itemText );
        __ASSERT_DEBUG( error == KErrNone, EasyDialingPanic( EEasyDialingPanicInvalidListBoxModelString ) );

        TUint id( 0 );
        TLex lex( indexText );
        error = lex.Val( id, EHex );
        __ASSERT_DEBUG( error == KErrNone, EasyDialingPanic( EEasyDialingPanicInvalidListBoxModelString ) );
        
        return id;
        }
    else 
        {
        return KErrNotFound;
        }
    }

// -----------------------------------------------------------------------------
// CurrentContactLinkLC
// 
// -----------------------------------------------------------------------------
//
HBufC8* CEasyDialingListBox::CurrentContactLinkLC()
    {
    TInt index = CurrentContactDataIndex();
    return iContactDataManager->ContactLinkLC( index );
    }


// -----------------------------------------------------------------------------
// HandleLongTapEventL
// Callback from CAknLongTapDetector.
// -----------------------------------------------------------------------------
//
void CEasyDialingListBox::HandleLongTapEventL( const TPoint& /*aPenEventLocation*/, 
                                               const TPoint& /*aPenEventScreenLocation*/ )
    {
    ReportListBoxEventL( static_cast<MEikListBoxObserver::TListBoxEvent>
            ( KEasyDialingContactLongTapped ) );
    iPointerLongPressHandled = ETrue;
    }


// -----------------------------------------------------------------------------
// HandlePointerEventL
// Pointer event handling within the EasyDialingListBox.
// -----------------------------------------------------------------------------
//
void CEasyDialingListBox::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    // Button down event starts new pointer press. Reset flags at this point.
    if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
        {
        iPointerLongPressHandled = EFalse;
        }
    
    iLongTapDetector->PointerEventL(aPointerEvent);
    if ( iPointerLongPressHandled )
        {
        // No further handling is made after long tap on list item
        // has been detected.
        return;
        }

    CEikFormattedCellListBox::HandlePointerEventL( aPointerEvent );
    }


// end of file

