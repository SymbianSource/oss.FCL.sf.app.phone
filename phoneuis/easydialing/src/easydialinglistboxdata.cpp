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
* Description:  Easy dialing list box data.
*
*/


// INCLUDE FILES

#include "easydialinglistboxdata.h"
#include "easydialinglistbox.h"
#include "easydialinglistboxview.h"
#include "easydialinglistboxitemdrawer.h"
#include "easydialingpanics.h"

#include "easydialingcontactdatamanager.h"
#include <easydialing.mbg>
#include <phonebook2.mbg>
#include <phonebook2ece.mbg>

// AvKON and drawing header files
#include <aknlayoutscalable_avkon.cdl.h>
#include <akniconconfig.h>
#include <gulicon.h>
#include <AknBidiTextUtils.h>
#include <bidivisual.h>

#include <aknlistloadertfx.h>
#include <aknlistboxtfxinternal.h>
#include <aknlistboxtfx.h>

#include <akntransitionutils.h>
#include <avkon.rsg>


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// KHighlightSeparatorChar is character used to separate matching and non-matching
// portions of contact names. 
const TInt KHighlightSeparatorChar = 0x1F;
_LIT( KHighlightSeparatorCharAsLit, "\x1F" );


// KContactNameFontHeightPercent is contact name font height relative to list item height.
const TInt KContactNameFontHeightPercent = 35;

// KCompanyNameFontHeightPercent is company name font height relative to list item height.
const TInt KCompanyNameFontHeightPercent = 30;

// KTextBoundingBoxHeightPercent gives the text bounding box height in percentages 
// relative to font height. This must be over 100, or part of the text cuts off.
const TInt KTextBoundingBoxHeightPercent = 120;

// KTextPlacementPercent controls how text is placed vertically within its bounding box. 
// The value is between 0 and 100. 0 means in top part, 50 mean in the middle, 100 means in the
// bottom. 
const TInt KTextPlacementPercent = 70;

// KMarginXPercent defines a width of horizontal margin used in many places. In relation to
// the width of the item rectangle.
const TInt KMarginXPercent = 2;

// KMarginYPercent defines a height of vertical margin. In relation to
// the height of the item rectangle. Currently used only with contact thumbnail.
const TInt KMarginYPercent = 4;

// KContacNameYOffsetPercent defines the vertical placement of contact name in relation to 
// item height.
const TInt KContactNameYOffsetPercent = 10;

// KCompanyNameYOffsetPercent defines the vertical placement of company name in relation to 
// item height.
const TInt KCompanyNameYOffsetPercent = 60;

// KArrowIconSizePercent defines the size of action menu icon relative to item height.
const TInt KArrowIconSizePercent = 20;

// KMatchingTextMarginInPixels the absolute pixel width of highlight margin. Highlight margin
// is an extra space in highlight boundary to make the text look less crowded.
const TInt KMatchingTextMarginInPixels = 3;

// KFavoriteIconSizePercent is size of favorite icon relative to text height.
const TInt KFavoriteIconSizePercent = 65;

// KMatchingTextMarginInPixels is the absolute pixel value for rounding used in highlight 
// rectangle.
const TInt KHighligthRectangleRoundingYInPixels = 4;

// KThumbnailAspectRatio is the aspect ratio of contact thumbnail in percents. 133 for instance
// is 4:3 aspect ration.
const TInt KThumbnailAspectRatio = 133;

const TInt KCent = 100;

const TInt KMaxRunInfoArrayCount = 20;

// IMPLEMENTATION SPECIFIC CONSTANTS
// The mif file from where you would like to show the 
// icon on the screen.
_LIT( KFavouriteIconBitmapFile, "\\resource\\apps\\phonebook2.mif" );
_LIT( KEasyDialingBitmapFile, "\\resource\\apps\\easydialing.mif" );
_LIT( KPhonebook2EceBitmapFile, "\\resource\\apps\\phonebook2ece.mif" );

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// GLOBAL FUNCTION PROTOTYPES
TRect ContactImageBoundingBox( const TRect& aItemRect );
TRect ArrowIconBoundingBox(const TRect& aItemRect);

// LOCAL FUNCTION PROTOTYPES
static TRect ContactNameBoundingBox(
        const TRect& aItemRect, 
        const CFont* aContactNameFont, 
        TBool aIsCurrentItem,
        TBool aIsFavourite, 
        TBool aThumbnailsShown );
static TRect CompanyNameBoundingBox(
        const TRect& aItemRect, 
        const CFont* aCompanyNameFont, 
        TBool aIsCurrentItem,
        TBool aThumbnailsShown );
static TRect FavouriteIconBoundingBox( const TRect& aContactNameBoundingBox );
static TRect MirrorLayoutBoundingBox(const TRect& aSourceRect, TRect& aBoundingBoxRect);
static TInt BaseLineOffset( const TRect& aTextBoundingBox, const CFont* aFont );
static TBool ContainsRightToLeftText( const TDesC& aDesc );
static TInt HighlightSeparatorCount( const TDesC& aText );
static HBufC* ConvertToVisualAndClipLC( const TDesC& aText, const CFont& aFont, const TRect& aBoundingBox ); 

static void ClipTextToWidth(
        TDes& aText,
        const CFont& aFont,
        TInt aMaxWidthInPixels,
        TBool& aMatch );

static TBool DrawPieceOfText(
        const TRect& aBoundingBox,
        TInt& aXOffset,
        CWindowGc &aGc, 
        const TDesC& aText,
        TBool aMatch,
        const CFont* aFont,
        const CEasyDialingListBoxData::TExtendedColors& aColors,
        TBool aHighLight);

static TInt DrawTextWithMatchHighlightL(
        const TRect& aBoundingBox,
        CWindowGc &aGc, 
        const TDesC& aText,
        const CFont* aFont,
        const CEasyDialingListBoxData::TExtendedColors& aColors,
        TBool aHighLight );

static TInt CalculateTextWidth(
        const TRect& aBoundingBox,
        const TDesC& aText,
        const CFont* aFont );

static TBool CalculateTextPieceWidth(
        const TRect& aBoundingBox,
        TInt& aXOffset,
        const TDesC& aText,
        TBool aMatch,
        const CFont* aFont );

// FORWARD DECLARATIONS


/*
 * ==============================================================================
 * 
 * 
 * class EasyDialingListBoxData::TExtendedColors
 * 
 * 
 * ==============================================================================
 */


// -----------------------------------------------------------------------------
// TExtendedColors
// 
// -----------------------------------------------------------------------------
//       
CEasyDialingListBoxData::TExtendedColors::TExtendedColors() :
CFormattedCellListBoxData::TColors(),
iMatchingText(KRgbBlack),
iMatchingBack(KRgbDarkYellow)
    {
    }


/*
 * ==============================================================================
 * 
 * 
 * class CEasyDialingListBoxData
 * 
 * 
 * ==============================================================================
 */


// -----------------------------------------------------------------------------
// CEasyDialingListBoxData
// 
// -----------------------------------------------------------------------------
//       
CEasyDialingListBoxData::CEasyDialingListBoxData() :
CFormattedCellListBoxData()
    {
    }

// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//       
CEasyDialingListBoxData::~CEasyDialingListBoxData()
    {
    // Release fonts. ReleaseFont function can cope with null pointer
    // so we don't need to null check them.
    CWsScreenDevice& screenDev = *( CEikonEnv::Static()->ScreenDevice() );
    screenDev.ReleaseFont( iContactNameFont );
    screenDev.ReleaseFont( iCompanyNameFont );
    
    delete iArrowPointingRight;
    delete iArrowPointingLeft;
    delete iColorBitmap;
    delete iDummyThumbnail;
    delete iFavouriteIcon;
    
    iContactDataManager = NULL;
    }

// -----------------------------------------------------------------------------
// NewL
// 
// -----------------------------------------------------------------------------
//       
CEasyDialingListBoxData* CEasyDialingListBoxData::NewL()
    {
    CEasyDialingListBoxData* self = new (ELeave) CEasyDialingListBoxData();
    
    CleanupStack::PushL( self );
    self->ConstructLD();
    CleanupStack::Pop( self );
 
    return self;
    }


// -----------------------------------------------------------------------------
// DrawData
// 
// -----------------------------------------------------------------------------
//       
void CEasyDialingListBoxData::DrawData(
    const TListItemProperties& aProperties, 
    CWindowGc& aGc,
    const TDesC* aText,
    const TRect& aRect,
    TBool aHighlight,
    const TExtendedColors& aColors ) const
    {
    const TRect &aItemRect = aRect;
    
    if ( aHighlight )
        {
        DrawHighlight( aGc, aItemRect );
        }
    
    // Draw the actual items.
    DrawDataFormatted( aProperties, aGc, aText, aItemRect, 
            aHighlight, aColors );
    }


// -----------------------------------------------------------------------------
// ConstructLD
// 
// -----------------------------------------------------------------------------
//       
void CEasyDialingListBoxData::ConstructLD()
    {
    CFormattedCellListBoxData::ConstructLD();
    
    // EasyDialing bitmap file is attempted to be read from the same directory where the
    // executed binary is located
    TFileName dllFileName;
    Dll::FileName( dllFileName );
     
    TParse parse;
    User::LeaveIfError(parse.Set(KEasyDialingBitmapFile, &dllFileName, NULL));
    TFileName bitmapFileName(parse.FullName());
      
    CFbsBitmap* bm;
    CFbsBitmap* mask;
    
    // iArrowPointingLeft is the icon displayed for the selected item in EasyDialingListBox
    AknIconUtils::CreateIconL( bm, mask, bitmapFileName, EMbmEasydialingQgn_indi_org_arrow_left, 
            EMbmEasydialingQgn_indi_org_arrow_left_mask );

    iArrowPointingLeft = CGulIcon::NewL( bm, mask );

    AknIconUtils::CreateIconL( bm, mask, bitmapFileName, EMbmEasydialingQgn_indi_org_arrow_right, 
            EMbmEasydialingQgn_indi_org_arrow_right_mask );

    iArrowPointingRight = CGulIcon::NewL( bm, mask );

    // Only mask for the icons are used. iColorBitmap is used for making the icon 
    // to follow text color changes according to skin.
    iColorBitmap = new (ELeave) CFbsBitmap;

    // Contact default thumbnail is not available in themes. It is read from phonebook resource. 
    AknIconUtils::CreateIconL( bm, mask, KPhonebook2EceBitmapFile,
            EMbmPhonebook2eceQgn_prop_pb_thumb_unknown, EMbmPhonebook2eceQgn_prop_pb_thumb_unknown_mask );
    iDummyThumbnail = CGulIcon::NewL( bm, mask );
    
    // Create the favourite icon bitmap and mask
    AknIconUtils::CreateIconL( bm, mask, KFavouriteIconBitmapFile, 
            EMbmPhonebook2Qgn_prop_pb_topc, EMbmPhonebook2Qgn_prop_pb_topc_mask );
    
    iFavouriteIcon = CGulIcon::NewL( bm, mask );
    }


// -----------------------------------------------------------------------------
// DrawHighlight
// 
// -----------------------------------------------------------------------------
//       
void CEasyDialingListBoxData::DrawHighlight( CWindowGc &aGc, const TRect &aItemRect ) const
    {
    MAknListBoxTfxInternal* transApi = CAknListLoader::TfxApiInternal( &aGc );
    if ( transApi )
        {
        transApi->Invalidate(MAknListBoxTfxInternal::EListHighlight );
        transApi->BeginRedraw( MAknListBoxTfxInternal::EListHighlight, aItemRect );
        transApi->StartDrawing( MAknListBoxTfxInternal::EListHighlight );
        aGc.SetClippingRect( iControl->Rect() );
        }

    TAknLayoutRect outerRect;
    TAknLayoutRect innerRect;
    outerRect.LayoutRect( aItemRect, TAknWindowComponentLayout::Compose(
            AknLayoutScalable_Avkon::list_highlight_pane_cp1(),
            AknLayoutScalable_Avkon::list_highlight_pane_g10_cp1() ).LayoutLine() );
    innerRect.LayoutRect( aItemRect, TAknWindowComponentLayout::Compose(
            AknLayoutScalable_Avkon::list_highlight_pane_cp1(),
            AknLayoutScalable_Avkon::list_highlight_pane_g1_cp1() ).LayoutLine() );
    MAknsControlContext *cc = AknsDrawUtils::ControlContext( Control() );

    if ( !cc ) 
        {
        cc = SkinBackgroundContext();
        }

    if ( cc )
        {
        aGc.SetPenStyle( CGraphicsContext::ENullPen );
        AknsDrawUtils::DrawFrame(
                AknsUtils::SkinInstance(), 
                aGc,
                outerRect.Rect(),
                innerRect.Rect(),
                KAknsIIDQsnFrList,
                KAknsIIDQsnFrListCenter );
        }

    if ( transApi )
        {
        aGc.CancelClippingRect();
        transApi->StopDrawing();
        transApi->EndRedraw( MAknListBoxTfxInternal::EListHighlight );
        }
    }


// -----------------------------------------------------------------------------
// DrawDataFormatted
// 
// -----------------------------------------------------------------------------
// 
// ToDo: SetSize function could be called once for all static data in SizeChanged.
// Applies at least to icons.
void CEasyDialingListBoxData::DrawDataFormatted( 
        TListItemProperties /* aProperties */,
        CWindowGc& aGc,
        const TDesC* aText,
        const TRect& aItemRect,
        TBool aHighlight,
        const TExtendedColors& aColors ) const
    {
    TPtrC cellText;

    TInt error = TextUtils::ColumnText( cellText , 0, aText );
    __ASSERT_DEBUG( error == KErrNone, EasyDialingPanic( EEasyDialingPanicInvalidListBoxModelString ) );
    __ASSERT_DEBUG( iContactNameFont, EasyDialingPanic( EEasyDialingNoFontFound ) );
    __ASSERT_DEBUG( iCompanyNameFont, EasyDialingPanic( EEasyDialingNoFontFound ) );
    
    MAknListBoxTfxInternal *transApi = CAknListLoader::TfxApiInternal( &aGc );
    if ( transApi )
        {
        transApi->StartDrawing( MAknListBoxTfxInternal::EListItem );
        if ( transApi->EffectsDisabled() )
            {
            aGc.SetClippingRect( iControl->Rect() );
            }
        }

    TRect boundingBox = ContactImageBoundingBox( aItemRect );
    if ( AknLayoutUtils::LayoutMirrored() )
        {
        boundingBox = MirrorLayoutBoundingBox( aItemRect, boundingBox );
        }
    
    //Draws the Contact Thumbnail Icon if exists else draws the dummy contact thumbnail
    TBool fav = ContactThumbnailDrawing( aGc, boundingBox, cellText );   

    error = TextUtils::ColumnText( cellText , 1, aText );
    __ASSERT_DEBUG( error == KErrNone, EasyDialingPanic( EEasyDialingPanicInvalidListBoxModelString ) );

    boundingBox = ContactNameBoundingBox( aItemRect,
                                          iContactNameFont,
                                          aHighlight,
                                          fav,
                                          iContactDataManager->GetContactThumbnailSetting() );
    TRect nameRectUnMirrored = boundingBox; // used for favourite star drawing

    if ( AknLayoutUtils::LayoutMirrored() )
        {
        boundingBox = MirrorLayoutBoundingBox( aItemRect, boundingBox );
        }

    // favourite icon size is set the same as contact name bounding box height.
    TInt favouriteIconSize = boundingBox.Height();
    
    TRect arrowRect = ArrowIconBoundingBox( aItemRect );
    if ( AknLayoutUtils::LayoutMirrored() )
        {
        arrowRect = MirrorLayoutBoundingBox( aItemRect, arrowRect );
        }
    
    // Draw arrow icon if the item is in focus.
    if ( aHighlight )
        {
        DrawArrowIcon( aGc, arrowRect );
        }

    TInt textWidth( 0 );
    TInt err( KErrNone );
    TRAP( err, textWidth = DrawTextWithMatchHighlightL(
            boundingBox, aGc, cellText, iContactNameFont, aColors, aHighlight ) );

    if ( !err && TextUtils::ColumnText( cellText , 2, aText ) == KErrNone ) 
        {
        TRect companyNameBoundingBox = CompanyNameBoundingBox( 
                aItemRect, iCompanyNameFont, aHighlight, iContactDataManager->GetContactThumbnailSetting() );
        if ( AknLayoutUtils::LayoutMirrored() )
            {
            companyNameBoundingBox = MirrorLayoutBoundingBox( aItemRect, companyNameBoundingBox );
            }
        TRAP( err, DrawTextWithMatchHighlightL(
                companyNameBoundingBox, aGc, cellText, iCompanyNameFont, aColors, aHighlight ) );
        }

    if ( !err && fav )
        {
        // Draws the Favourite Icon
        DrawFavouriteIcon( aGc, nameRectUnMirrored, aItemRect );
        }
    
    if ( transApi )
        {
        aGc.CancelClippingRect();
        transApi->StopDrawing();
        }
    }

// -----------------------------------------------------------------------------
// ContactThumbnailDrawing
// 
// Draws the Contact Thumbnail Icon if any else draws the dummy contact thumbnail
// -----------------------------------------------------------------------------

TBool CEasyDialingListBoxData::ContactThumbnailDrawing(CWindowGc& aGc, TRect aBoundingBox, TPtrC aCellText) const
    {
    TBool fav(EFalse);
    CFbsBitmap* thumbnail(NULL);
    TBool isLoaded = iContactDataManager->GetThumbnailAndFav(aCellText, thumbnail, fav);
    if ( isLoaded && thumbnail )
        {
        // center the thumbnail in its rect
        TSize size(thumbnail->SizeInPixels());
        TInt xOffset = (aBoundingBox.Width() - size.iWidth) / 2;
        TInt yOffset = (aBoundingBox.Height() - size.iHeight) / 2;
        TPoint tl(aBoundingBox.iTl.iX + xOffset, aBoundingBox.iTl.iY + yOffset);
        TRect sourceRect( TPoint(0,0),size);
        aGc.BitBlt(tl, thumbnail, sourceRect);
        }
    else if ( isLoaded && iContactDataManager->GetContactThumbnailSetting() )
        {
        // draw dummy thumnbnail, but only if we know that the contact doesn't
        // have a thumbnail, and thumbnail drawing is enabled.
        AknIconUtils::SetSize(iDummyThumbnail->Bitmap(), aBoundingBox.Size());
        AknIconUtils::SetSize(iDummyThumbnail->Mask(), aBoundingBox.Size());
        aGc.BitBltMasked( aBoundingBox.iTl, iDummyThumbnail->Bitmap(),
                TRect( TPoint(0,0), aBoundingBox.Size() ), 
                iDummyThumbnail->Mask(), ETrue );
        }
    return fav;
    }


// -----------------------------------------------------------------------------
// DrawArrowIcon
//  Draws the Action Menu 
// -----------------------------------------------------------------------------
void CEasyDialingListBoxData::DrawArrowIcon( 
        CWindowGc& aGc, TRect aArrowRect) const
    {
    // Action Menu Arrow for opening the Action Menu
    if ( AknLayoutUtils::LayoutMirrored() )
        {
        // For mirrored layout the UI logic is mirrored as well
        AknIconUtils::SetSize( iArrowPointingLeft->Mask(), aArrowRect.Size() );
        aGc.BitBltMasked( aArrowRect.iTl, iColorBitmap,
                TRect( TPoint(0,0), aArrowRect.Size() ), 
                iArrowPointingLeft->Mask(), ETrue );

        }
    else
        {
        AknIconUtils::SetSize( iArrowPointingRight->Mask(), aArrowRect.Size() );
        aGc.BitBltMasked( aArrowRect.iTl, iColorBitmap,
                TRect( TPoint(0,0), aArrowRect.Size() ), 
                iArrowPointingRight->Mask(), ETrue );
        }
    }

// -----------------------------------------------------------------------------
// DrawFavouriteIcon
// 
// Draws the Favourite Icon
// -----------------------------------------------------------------------------
void CEasyDialingListBoxData::DrawFavouriteIcon(
        CWindowGc& aGc, 
         TRect aNameRectUnMirrored,
         TRect aEffectiveRect) const
    {
    TRect favouriteIconBoundingBox;

    favouriteIconBoundingBox = FavouriteIconBoundingBox( aNameRectUnMirrored );

    if ( AknLayoutUtils::LayoutMirrored() )
        {
        favouriteIconBoundingBox = MirrorLayoutBoundingBox(aEffectiveRect, favouriteIconBoundingBox);
        }

    aGc.SetBrushStyle( CGraphicsContext::ENullBrush );
    TRect sourceRect( TPoint(0,0), favouriteIconBoundingBox.Size() );

    if( AknsUtils::SkinInstance()->GetCachedItemData(KAknsIIDQgnFsContactsFavorite) )
        {
        AknsDrawUtils::DrawCachedImage( AknsUtils::SkinInstance(), aGc, sourceRect, 
                KAknsIIDQgnFsContactsFavorite );
        }
    else
        {
        // Set size for the bitmap and mask
        AknIconUtils::SetSize( iFavouriteIcon->Bitmap(), favouriteIconBoundingBox.Size() );
        AknIconUtils::SetSize( iFavouriteIcon->Mask(), favouriteIconBoundingBox.Size() );
        aGc.BitBltMasked( favouriteIconBoundingBox.iTl , iFavouriteIcon->Bitmap(), 
                sourceRect, iFavouriteIcon->Mask(), ETrue);
        }
    }
// -----------------------------------------------------------------------------
// SetContactDataManager
// 
// -----------------------------------------------------------------------------
// 
void CEasyDialingListBoxData::SetContactDataManager(CEasyDialingContactDataManager* aContactDataManager)
    {
    iContactDataManager = aContactDataManager;
    }

// -----------------------------------------------------------------------------
// HandleItemSizeChange
// 
// -----------------------------------------------------------------------------
//
void CEasyDialingListBoxData::HandleItemSizeChange()
    {
    TInt height = iControl->ItemHeight();
    TSize size( height, height );
    TRAP_IGNORE( UpdateColorBitmapL( size ) );
    
    // Obtain fonts.
    ObtainFonts( height );
    
    // TODO: also bounding boxes could be updated and stored here
    }

// -----------------------------------------------------------------------------
// ObtainFonts
// 
// -----------------------------------------------------------------------------
//       
void CEasyDialingListBoxData::ObtainFonts( TInt aItemHeight )
    {
    CWsScreenDevice& screenDev = *( CEikonEnv::Static()->ScreenDevice() );
    
    // Release previous fonts. ReleaseFont function can cope with null pointers
    // so we don't need to null check them.
    screenDev.ReleaseFont( iContactNameFont );
    screenDev.ReleaseFont( iCompanyNameFont );
    
    // Get a logical font to a basis for our own fonts.
    const CFont* logicalFont = AknLayoutUtils::FontFromId( EAknLogicalFontSecondaryFont );

    // Extract font information
    TFontSpec fontSpec = logicalFont->FontSpecInTwips();
    
    // Calculate contact name font height in TWIPs.
    TInt fontHeightPixels = aItemHeight * KContactNameFontHeightPercent / KCent;
    TInt fontHeightTwips = screenDev.VerticalPixelsToTwips( fontHeightPixels );
    
    // Set height, weight, and posture.
    fontSpec.iHeight = fontHeightTwips;
    fontSpec.iFontStyle.SetStrokeWeight( EStrokeWeightBold );
    fontSpec.iFontStyle.SetPosture( EPostureUpright );
    
    // Obtain contact name font
    TInt err = screenDev.GetNearestFontToDesignHeightInTwips( iContactNameFont, fontSpec );
    __ASSERT_DEBUG( err == KErrNone, EasyDialingPanic( EEasyDialingNoFontFound ) ); 
    
    // Calculate company name font height in TWIPs.
    fontHeightPixels = aItemHeight * KCompanyNameFontHeightPercent / KCent;
    fontHeightTwips = screenDev.VerticalPixelsToTwips( fontHeightPixels );
    fontSpec.iHeight = fontHeightTwips;
    fontSpec.iFontStyle.SetStrokeWeight( EStrokeWeightNormal );
    
    // Obtain company name font.  
    err = screenDev.GetNearestFontToDesignHeightInTwips( iCompanyNameFont, fontSpec );
    __ASSERT_DEBUG( err == KErrNone, EasyDialingPanic( EEasyDialingNoFontFound ) ); 
   }


// -----------------------------------------------------------------------------
// UpdateColorBitmapL
// ColorBitmap is redrawn when UI layout or text color (Theme) changes
// -----------------------------------------------------------------------------
//       
void CEasyDialingListBoxData::UpdateColorBitmapL( const TSize& aSize )
    {
    TRgb color;
    // Get the HighLighted text color in ListBox from the Theme
    AknsUtils::GetCachedColor( AknsUtils::SkinInstance(), color, 
                               KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG10 );
    
    // Create a bitmap with the similar display mode than what the device currently
    // uses for using it as an offscreen bitmap
    AknIconConfig::TPreferredDisplayMode mode;
    AknIconConfig::PreferredDisplayMode( mode, AknIconConfig::EImageTypeOffscreen );
    User::LeaveIfError( iColorBitmap->Create( aSize, mode.iBitmapMode ) );

    // Create a new drawing device and graphics context for enabling drawing to
    // the offscreen bitmap
    CFbsBitmapDevice* destinationDevice = CFbsBitmapDevice::NewL( iColorBitmap );
    CleanupStack::PushL(destinationDevice);

    CFbsBitGc* destinationGc;
    User::LeaveIfError( destinationDevice->CreateContext( destinationGc ) );

    // Set the color and style for pen and brush and draw a rectangle to the
    // bitmap graphics
    destinationGc->SetPenColor( color );
    destinationGc->SetPenStyle( CGraphicsContext::ESolidPen );
    destinationGc->SetBrushColor( color );
    destinationGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    destinationGc->DrawRect( TRect( TPoint( 0,0 ), aSize ) );

    // Colorbitmap is ready, cleanup
    delete destinationGc;
    CleanupStack::PopAndDestroy(destinationDevice); 
    }

// -----------------------------------------------------------------------------
// SetEDLBXControl
// 
// -----------------------------------------------------------------------------
//       
void CEasyDialingListBoxData::SetEDLBXControl(CEasyDialingListBox* aControl)
    {
    iControl = aControl;
    }


/*
 * ==============================================================================
 * 
 * Local functions
 * 
 * ==============================================================================
 */


// -----------------------------------------------------------------------------
// ContactImageBoundingBox
// Calculates the area where contact thumbnail is confined
// -----------------------------------------------------------------------------
//
TRect ContactImageBoundingBox(const TRect& aItemRect)
    {
    TInt leftMargin = aItemRect.Width() * KMarginXPercent / KCent;
    TInt topMargin = KMarginYPercent * aItemRect.Height() / KCent;
    TInt bottomMargin = topMargin;
    TInt width = ((aItemRect.Height() - topMargin - bottomMargin) * KThumbnailAspectRatio) / KCent;

    return TRect(
            aItemRect.iTl.iX + leftMargin,
            aItemRect.iTl.iY + topMargin,
            aItemRect.iTl.iX + leftMargin + width,
            aItemRect.iBr.iY - bottomMargin);
    }

// -----------------------------------------------------------------------------
// ContactNameBoundingBox
// Calculates the area to which the contact name and possible match highlights
// are confined.
//      
// -----------------------------------------------------------------------------
//
static TRect ContactNameBoundingBox(
        const TRect& aItemRect,
        const CFont* aContactNameFont,
        TBool aIsCurrentItem,
        TBool aIsFavourite,
        TBool aThumbnailsShown )
    {
    // Position X will contain the starting position of text from left side of item rect.
    TInt positionX = aItemRect.Width() * KMarginXPercent / KCent;
    
    if (aThumbnailsShown)
        {
        // If contact image is shown, text starts from right side of contact picture + margin.
        TRect contactImageBoundingBox = ContactImageBoundingBox( aItemRect );
        positionX += contactImageBoundingBox.Width();
        positionX += aItemRect.Width() * KMarginXPercent / KCent;
        }
    
    TInt topMargin = KContactNameYOffsetPercent * aItemRect.Height() / KCent;
    TInt height = KTextBoundingBoxHeightPercent * aContactNameFont->FontMaxHeight() / KCent;
    TInt rightMargin = KMarginXPercent * aItemRect.Width() / KCent;
    
    // Reserve space for communication launcher icon.
    // Communication launcher icon is shown only id item is highlighted.
    if ( aIsCurrentItem )
        {
        rightMargin += KArrowIconSizePercent * aItemRect.Height() / KCent;
        }
    
    // If item is favourite, reserve space for favourite icon. Icon dimensions are the same as bounding box height.
    if ( aIsFavourite )
        {
        rightMargin += height;
        }
    
    return TRect(
        aItemRect.iTl.iX + positionX,
        aItemRect.iTl.iY + topMargin,
        aItemRect.iTl.iX + aItemRect.Width() - rightMargin,
        aItemRect.iTl.iY + topMargin + height);
    }


// -----------------------------------------------------------------------------
// CompanyNameBoundingBox
// Calculates the area to which the company name and possible match highlights
// are confined.
// -----------------------------------------------------------------------------
//
static TRect CompanyNameBoundingBox(
        const TRect& aItemRect, 
        const CFont* aCompanyNameFont, 
        TBool aIsCurrentItem,
        TBool aThumbnailsShown )
    {
    // Position X will contain the starting position of text from left side of item rect.
    TInt positionX = aItemRect.Width() * KMarginXPercent / KCent;
    
    if (aThumbnailsShown)
        {
        // If contact image is show, text starts from right side of contact picture + margin.
        TRect contactImageBoundingBox = ContactImageBoundingBox( aItemRect );
        positionX += contactImageBoundingBox.Width();
        positionX += aItemRect.Width() * KMarginXPercent / KCent;
        }
    
    TInt topMargin = KCompanyNameYOffsetPercent * aItemRect.Height() / KCent;
    TInt height = KTextBoundingBoxHeightPercent * aCompanyNameFont->FontMaxHeight() / KCent;
    TInt rightMargin = KMarginXPercent * aItemRect.Width() / KCent;

    // Reserve space for communication launcher icon.
    // Communication launcher icon is shown only id item is highlighted.
    if ( aIsCurrentItem )
        {
        rightMargin += KArrowIconSizePercent * aItemRect.Height() / KCent;
        }
    
    return TRect(
        aItemRect.iTl.iX + positionX,
        aItemRect.iTl.iY + topMargin,
        aItemRect.iTl.iX + aItemRect.Width() - rightMargin,
        aItemRect.iTl.iY + topMargin + height);
    }


// -----------------------------------------------------------------------------
// ArrowIconBoundingBox
// Calculates the area to which the action menu icon is drawn.
// -----------------------------------------------------------------------------
//
TRect ArrowIconBoundingBox(const TRect& aItemRect)
    {
    TInt iconSize = KArrowIconSizePercent * aItemRect.Height() / KCent;
    TInt rightMargin = KMarginXPercent * aItemRect.Width() / KCent;
    TInt positionX = aItemRect.iBr.iX - rightMargin - iconSize;
    TInt topMargin = ( aItemRect.Height() - iconSize ) / 2; // Icon is vertically centered.

    return TRect(
        positionX,
        aItemRect.iTl.iY + topMargin,
        positionX + iconSize,
        aItemRect.iTl.iY + topMargin + iconSize );
    }


// -----------------------------------------------------------------------------
// FavouriteIconBoundingBox
// Calculates the area to which the favourite icon is drawn.
// Favourite icon is drawn right beside contact name bounding box.
// -----------------------------------------------------------------------------
//
static TRect FavouriteIconBoundingBox( const TRect& aContactNameBoundingBox )
    {
    // Favourite icons place is right beside contact name bounding box.
    // Contact name bounding box is calculated so that there is room for the icon.
    TInt shrinkMargin = ( KCent - KFavoriteIconSizePercent ) * aContactNameBoundingBox.Height() / ( 2 * KCent );
    TRect rect(
            aContactNameBoundingBox.iBr.iX,
            aContactNameBoundingBox.iTl.iY,
            aContactNameBoundingBox.iBr.iX + aContactNameBoundingBox.Height(),
            aContactNameBoundingBox.iBr.iY);
    rect.Shrink( shrinkMargin, shrinkMargin );
    return rect;
    }



// -----------------------------------------------------------------------------
// BaseLineOffset
//
// -----------------------------------------------------------------------------
//
static TInt BaseLineOffset( const TRect& aTextBoundingBox, const CFont* aFont )
    {
    TInt fontHeight = aFont->FontMaxHeight();
    TInt topMargin = KTextPlacementPercent * (aTextBoundingBox.Height() - fontHeight) / KCent;
    return fontHeight + topMargin - aFont->FontMaxDescent();
    }


// -----------------------------------------------------------------------------
// MirrorLayoutBoundingBox
//
// -----------------------------------------------------------------------------
//
static TRect MirrorLayoutBoundingBox(const TRect& aSourceRect, TRect& aBoundingBoxRect)
    {
    return TRect(
            aSourceRect.iTl.iX + aSourceRect.iBr.iX - aBoundingBoxRect.iBr.iX,
            aBoundingBoxRect.iTl.iY,
            aSourceRect.iTl.iX + aSourceRect.iBr.iX - aBoundingBoxRect.iTl.iX,
            aBoundingBoxRect.iBr.iY);
    }


// -----------------------------------------------------------------------------
// ClipTextToWidth
// Cuts the text in the given pixel width. Also considers match highlight
// issues.
// NOTE: Can change the aMatch parameter if considers that there is too little
// width for matching highlight.
// NOTE 2: This function cannot handle right-to-left or bidirectional text currently.
// These cases must be handled elsewhere.
// -----------------------------------------------------------------------------
//

_LIT( KThreeDots, "..." );

static void ClipTextToWidth(
        TDes& aText,
        const CFont& aFont,
        TInt aMaxWidthInPixels,
        TBool& aMatch)
    {
    TInt minimumWidth = aFont.TextWidthInPixels( KThreeDots );
    
    // If this is a matching piece of text, also match text marginals need to be counted.
    if ( aMatch )
        {
        minimumWidth += 2 * KMatchingTextMarginInPixels;
        }
    
    if ( minimumWidth > aMaxWidthInPixels )
        {
        
        // Not enough space for any text. 
        aText.Zero();
        aMatch = EFalse; // No match highlight shown.
        return;
        }
    
    if ( aMatch )
        {
        aMaxWidthInPixels -= 2 * KMatchingTextMarginInPixels;
        }
    
    AknTextUtils::ClipToFit( aText, aFont, aMaxWidthInPixels );    
    }


// -----------------------------------------------------------------------------
// DrawPieceOfText
// Draws a piece of text of contact, either matching or not matching.
// Updates aXOffset argument by adding drawn text width.
//
// @return: ETrue, if there was enough space for the text to draw. 
// NOTE: CWindowGc font must be set before calling this function. 
// -----------------------------------------------------------------------------
//
static TBool DrawPieceOfText(
        const TRect& aBoundingBox,
        TInt& aXOffset,
        CWindowGc &aGc, 
        const TDesC& aText,
        TBool aMatch,
        const CFont* aFont,
        const CEasyDialingListBoxData::TExtendedColors& aColors,
        TBool aHighLight )
    {    
    if (aText.Length() == 0) 
        {
        return ETrue;
        }
    
    HBufC* newText = aText.Alloc();
    if ( !newText )
        {
        return EFalse;
        }
    
    TPtr textPtr = newText->Des();
    
    // textWidth is the width needed for the text. 
    TInt textWidth = AknBidiTextUtils::MeasureTextBoundsWidth( *aFont, textPtr, CFont::TMeasureTextInput::EFVisualOrder );
    
    // whole width includes also possible highlight margins. 
    TInt wholeWidth = textWidth + ( aMatch ? 2 * KMatchingTextMarginInPixels : 0);
    
    // availableWidth is the space left for drawing. 
    TInt availableWidth = aBoundingBox.Width() - aXOffset;
    
    if ( availableWidth < wholeWidth )
        {
        ClipTextToWidth( textPtr, *aFont, availableWidth, aMatch );
        wholeWidth = availableWidth;
        if ( aMatch )
            {
            textWidth = wholeWidth - (2 * KMatchingTextMarginInPixels);
            }
        else
            {
            textWidth = wholeWidth;
            }
        }
    
    // textBox is rectangle for text without highlight.
    TRect textBox( aBoundingBox );
    textBox.iTl.iX += aXOffset;
    textBox.iBr.iX = textBox.iTl.iX + textWidth;
    
    TInt baseLineOffset = BaseLineOffset( textBox, aFont );
    
    if ( aMatch )
        {
        // highlightBox rectangle for text with highlight.
        // Compared to no highlight, it has extra margins on both sides.
        TRect highlightBox( textBox );
        highlightBox.iBr.iX = highlightBox.iTl.iX + wholeWidth;
        
        // Also boundingBox is shifted right for one margin width.
        textBox.iTl.iX += KMatchingTextMarginInPixels;
        textBox.iBr.iX += KMatchingTextMarginInPixels;
       
        // Outline of matching highlight is of same colour as text.
        // This looks quite OK.
        aGc.SetPenColor( aColors.iMatchingText );
        aGc.SetBrushStyle( CGraphicsContext::ESolidBrush );
        aGc.SetBrushColor( aColors.iMatchingBack );
        
        aGc.DrawRoundRect( highlightBox, TSize( KMatchingTextMarginInPixels, KHighligthRectangleRoundingYInPixels ) );
        
        aGc.SetBrushStyle( CGraphicsContext::ENullBrush );
        }
    else 
        {
        if ( aHighLight )
            {
            aGc.SetPenColor( aColors.iHighlightedText );
            }
        else 
            {
            aGc.SetPenColor( aColors.iText );
            }
        }
    
    aGc.DrawText( textPtr, textBox, baseLineOffset );
    delete newText;
    aXOffset += wholeWidth;
    return ETrue;
    }


// -----------------------------------------------------------------------------
// DrawTextWithMatchHighlight
// Draws a string with match highlight. Highlighted and non-highlighted
// parts are separated with KHighlightSeparatorChar.
// The first text part is not highlighted and from that on highlight
// is on on every other text piece.
// 
// -----------------------------------------------------------------------------
//
static TInt DrawTextWithMatchHighlightL(
        const TRect& aBoundingBox,
        CWindowGc &aGc, 
        const TDesC& aText,
        const CFont* aFont,
        const CEasyDialingListBoxData::TExtendedColors& aColors,
        TBool aHighLight)
    {
    TInt xOffset = 0;
    
    HBufC* visualBuf = ConvertToVisualAndClipLC( aText, *aFont, aBoundingBox );
    
    TInt calculatedTextWidth = CalculateTextWidth( aBoundingBox, *visualBuf, aFont );
    
    if ( AknLayoutUtils::LayoutMirrored() )
        {
        xOffset = aBoundingBox.Width() - calculatedTextWidth;
        }
    TPtrC textPiece;
    TInt textPieceIndex = 0;
    TBool match = EFalse;

    aGc.UseFont( aFont );
    aGc.SetBrushStyle( CGraphicsContext::ENullBrush );

    while ( TextUtils::ColumnText( textPiece , textPieceIndex, visualBuf, KHighlightSeparatorChar) == KErrNone ) 
        {
        if (! DrawPieceOfText( aBoundingBox, xOffset, aGc, textPiece, match, aFont, aColors, aHighLight ))
            {
            // If there was not enough space for this piece of text, exit the loop stop drawing further pieces.
            break;
            }

        // Toggle match
        match = !match;

        ++textPieceIndex;
        }
    
    CleanupStack::PopAndDestroy( visualBuf );

    aGc.DiscardFont();
    
    return calculatedTextWidth;
    }

// -----------------------------------------------------------------------------
// CalculateTextWidth
// Calculates the width of the text and returns it 
// -----------------------------------------------------------------------------
//
static TInt CalculateTextWidth(const TRect& aBoundingBox, const TDesC& aText, const CFont* aFont )
    {
    
    TInt xOffset = 0;
    TPtrC textPiece;
    TInt textPieceIndex = 0;
    TBool match = EFalse;

    while ( TextUtils::ColumnText( textPiece , textPieceIndex, &aText, KHighlightSeparatorChar) == KErrNone ) 
        {
        if (! CalculateTextPieceWidth( aBoundingBox, xOffset, textPiece, match, aFont ))
            {
            // If there was not enough space for this piece of text, exit the loop stop drawing further pieces.
            break;
            }

        // Toggle match
        match = !match;

        ++textPieceIndex;
        }

    return xOffset;
    }

// -----------------------------------------------------------------------------
// CalculateTextPieceWidth
// 
// Calculates the width of the text piece of highlighted text. 
// The function is aware of the available width for the text, and can take
// possible clippings into account. 
//
// The available width is given by parameters aBoundingBox (space for all text
// pieces) and aXOffset (now much of that space has already been used).
//
// The function adds the text width to the aXOffset reference parameter.
// Returns EFalse, if there is no more space for new text pieces, otherwise
// ETrue.
//
// This function contains the same logic as function DrawPieceOfText.
// -----------------------------------------------------------------------------
//
static TBool CalculateTextPieceWidth(
        const TRect& aBoundingBox,
        TInt& aXOffset,
        const TDesC& aText,
        TBool aMatch,
        const CFont* aFont )
    {    
    if (aText.Length() == 0) 
        {
        return ETrue;
        }

    // textWidth is the width needed for the text. 
    TInt textWidth = AknBidiTextUtils::MeasureTextBoundsWidth( *aFont, aText, CFont::TMeasureTextInput::EFVisualOrder );

    // whole width includes also possible highlight margins. 
    TInt wholeWidth = textWidth + ( aMatch ? 2 * KMatchingTextMarginInPixels : 0);

    // availableWidth is the space left for drawing. 
    TInt availableWidth = aBoundingBox.Width() - aXOffset;

    if ( availableWidth < wholeWidth )
        {

        // We get to this branch, if there is not enough space for the text piece.
        HBufC* newText = aText.Alloc();
        if ( !newText )
            {
            return EFalse;
            }

        TPtr textPtr = newText->Des();
        
        // Clip the text so that it fits the space.
        ClipTextToWidth( textPtr, *aFont, availableWidth, aMatch );

        if ( textPtr.Length() > 0 )
            {
            wholeWidth = AknBidiTextUtils::MeasureTextBoundsWidth( *aFont, textPtr, CFont::TMeasureTextInput::EFVisualOrder );
            wholeWidth += (aMatch ? 2 * KMatchingTextMarginInPixels : 0);

            aXOffset += wholeWidth;
            }

        delete newText;

        return EFalse;
        }

    aXOffset += wholeWidth;

    return ETrue;
    }

// -----------------------------------------------------------------------------
// ContainsRightToLeftText
// 
// Returns true if argument descriptor contains right-to-left text.
// -----------------------------------------------------------------------------
//
static TBool ContainsRightToLeftText( const TDesC& aDesc )
    {
    TBool rtlFound = EFalse;
    
    // TRunInfoArray contains information of the directionalities of the different sections of the aText    
    TBidirectionalState::TRunInfo array[ KMaxRunInfoArrayCount ];
    
    // Initialize the TBidiLogicalToVisual converter for making the conversion from logical to visual order
    TBidiLogicalToVisual converter( aDesc, array, KMaxRunInfoArrayCount );
    
    // Do the reordering. Amount of different directionality sections is returned.
    TInt count( converter.Reorder() );
    // If there are more directionality blocks than we are prepared to handle, just ignore
    // the rest. Those shouldn't fit the screen anyway.
    count = Min( count, KMaxRunInfoArrayCount );
    
    for ( TInt i = 0 ; i < count && !rtlFound ; i++ )
        {
        // iDirection is 0 for left-to-right text.
        if (array[i].iDirection)
            {
            rtlFound = ETrue;
            }
        }

    return rtlFound;
    }

// -----------------------------------------------------------------------------
// ConvertToVisualAndClipL
// 
// Clip bidirectional text to given box and convert it to visual order, ensuring
// that the match highlights don't get broken in the process. Result is given
// as new heap descriptor which is left to CleanupStack.
// NOTE1: No clipping happens here if given descriptor contains only 
// left-to-right text. 
// NOTE2: It's assumed that there can be highlights only in pure LTR and RTL
// texts. Highlights for mixed LTR-RTL text cannot be handled properly.
// -----------------------------------------------------------------------------
//
static HBufC* ConvertToVisualAndClipLC( const TDesC& aText, 
                                       const CFont& aFont, 
                                       const TRect& aBoundingBox )
    {
    HBufC* buf = HBufC::NewLC( aText.Length() + KAknBidiExtraSpacePerLine );
    TPtr ptr = buf->Des();
    ptr.Copy( aText );
    
    // Calling AknBidiTextUtils::ConvertToVisualAndClip doesn't work correctly 
    // with the highlight separator characters (they are not considered to be 
    // zero-length). To minimise the problem, we call the function only when 
    // necessary, i.e. when given text really contains RTL text. This should be 
    // considered as temporary solutions because now the problem of excessive 
    // truncation still remains with RTL languages.
    if ( ContainsRightToLeftText( ptr ) )
        {
        AknBidiTextUtils::ConvertToVisualAndClipL(
                ptr, aFont, aBoundingBox.Width(),
                aBoundingBox.Width() );
        
        // If there's an odd number of highlight separators in the RTL text, 
        // then the matching and and not-matching parts have gone 
        // off-sync in the visual conversion (because parts are drawn starting 
        // from left and first part is always interpreted as not-matching part).
        // Fix this by adding one highlight separator.
        TInt sepCount = HighlightSeparatorCount( ptr );
        if ( sepCount % 2 )
            {
            if ( ptr.Length() == ptr.MaxLength() )
                {
                // There's no more space available. We need to reallocate the 
                // buffer in order to fit the extra separator character.
                HBufC* newBuf = buf->ReAllocL( buf->Length() + 1 );
                CleanupStack::Pop( buf ); // original buf deleted by ReAllocL
                buf = newBuf;
                CleanupStack::PushL( buf );
                ptr.Set( buf->Des() );
                }
            ptr.Insert( 0, KHighlightSeparatorCharAsLit );
            }
        }
    
    return buf;
    }

// -----------------------------------------------------------------------------
// HighlightSeparatorCount
// 
// Returns number of match highlight separator characters in the text
// -----------------------------------------------------------------------------
//
static TInt HighlightSeparatorCount( const TDesC& aText )
    {
    TInt len = aText.Length();
    TInt sepCount = 0;
    
    for ( TInt i = 0; i < len; i++ )
        {
        if ( aText[i] == KHighlightSeparatorChar )
            {
            sepCount++;
            }
        }
    
    return sepCount;
    }

//  End of File.

