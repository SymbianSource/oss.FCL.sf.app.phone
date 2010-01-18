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
* Description:  Details dialog for last message.
*
*/



// INCLUDE FILES
#include <bldvariant.hrh>
#include "VmDetailsDialog.h"
#include "VoiceMailboxAppPrivateCRKeys.h"
#include "VmDetDlgPropertyObserver.h"
#include <centralrepository.h>
#include "Vm.hrh"
#include <Vm.rsg>
#include <AknLists.h>
#include <AknTitle.h>
#include <NumberGrouping.h>
#include <NumberGroupingCRKeys.h>
#include <StringLoader.h>
#include "VMBLogger.h"

// CONSTANTS
const TInt KVmMaxLabelStringLength = 100;
const TInt KVmMaxLabelFreeTextLength = 255;
const TUint KVmDateSeparator = '/';
const TUint KVmTimeSeparator = ':';


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVmDetailsDialog::CVmDetailsDialog
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVmDetailsDialog::CVmDetailsDialog()
    {
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVmDetailsDialog::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::ConstructL =>" );
    CAknDialog::ConstructL( R_VMBX_DETAIL_MENUBAR );

    // Central Repository handle
    iSession = CRepository::NewL( KCRUidVoiceMailbox );
    iObserver = CVmDetDlgPropertyObserver::NewL( *this );
    StartObservingL();

    TInt pngErr( KErrNone );
    TInt pngSupported( 0 );
    CRepository* repository = CRepository::NewL( KCRUidNumberGrouping );
    pngErr = repository->Get( KNumberGrouping, pngSupported );
    delete repository;

    // create number grouping component if is supported.
    if ( KErrNone == pngErr && pngSupported != 0 )
        {
        iNumberGrouping = CPNGNumberGrouping::NewL( KVmMaxLabelStringLength );
        }
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::ConstructL <=" );
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVmDetailsDialog* CVmDetailsDialog::NewL()
    {
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::NewL =>" );
    CVmDetailsDialog* self = new( ELeave ) CVmDetailsDialog;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::NewL <=" );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CVmDetailsDialog::~CVmDetailsDialog
// Destructor
// -----------------------------------------------------------------------------
//
CVmDetailsDialog::~CVmDetailsDialog()
    {
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::~CVmDetailsDialog =>" );
    delete iOldTitlePaneText;
    iOldTitlePaneText = NULL;

    delete iObserver;
    delete iSession;
    delete iNumberGrouping;

    VMBLOGSTRING( "VMBX: CVmDetailsDialog::~CVmDetailsDialog <=" );
    }


// -----------------------------------------------------------------------------
// CVmDetailsDialog::PreLayoutDynInitL
// Called before the dialog is launched. Title pane is updated and listbox
// is filled with a last message information.
// -----------------------------------------------------------------------------
//
void CVmDetailsDialog::PreLayoutDynInitL()
    {
    // Take old title pane to variable.
    CAknTitlePane* titlePane = 
        static_cast< CAknTitlePane* >
            ( iEikonEnv->AppUiFactory()->StatusPane()->ControlL(
                TUid::Uid( EEikStatusPaneUidTitle ) ) );
    iOldTitlePaneText = titlePane->Text()->AllocL();

    // Set new title
    HBufC* titleText = StringLoader::LoadL( R_TITLE_DETAILS );
    titlePane->SetText( titleText ); //takes ownership
    titleText = NULL;


    // Get listbox pointer
    CAknSingleHeadingStyleListBox* listbox = 
        static_cast<CAknSingleHeadingStyleListBox*> 
            ( Control( KVmDetailsDialog ) );

    listbox->CreateScrollBarFrameL( ETrue );
    listbox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

    // populate array
    MDesCArray* itemList = listbox->Model()->ItemTextArray();
    CDesCArray* itemArray = static_cast<CDesCArray*>( itemList );
    PopulateListboxItemsL( *itemArray );
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::PopulateListboxItemsL
// Reads message details from shared data.
// -----------------------------------------------------------------------------
//
void CVmDetailsDialog::PopulateListboxItemsL( CDesCArray& aItemArray )
    {
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::PopulateListboxItemsL =>" );
    // To calculate the rect for the text box.
    CAknSingleHeadingStyleListBox* listbox = 
                   static_cast<CAknSingleHeadingStyleListBox*> 
                       ( Control(KVmDetailsDialog) );    
    CListBoxView* view = listbox->View();    
    TRect itemTextRect (TPoint( view->ItemPos(0) ), TSize(view->ItemSize() ) );
    aItemArray.Reset();

    // Used variables
    HBufC* heading = NULL;
    HBufC* data = NULL;
    TInt error = KErrNone;


    // Urgency
    heading = StringLoader::LoadLC( R_URGENCY );

    TInt urgency; 
    error = iSession->Get( KVmUrgencyLevel, urgency );
    if ( error == KErrNone )
        {
        switch ( urgency )
            {
            case 0: // Normal
            case 1: // Interactive
                {
                data = StringLoader::LoadLC( R_NORMAL );
                break;
                }
            case 2: // Urgent
                {
                data = StringLoader::LoadLC( R_URGENT );
                break;
                }
            case 3: // Emergency
                {
                data = StringLoader::LoadLC( R_EMERGENCY );
                break;
                }
            default:
                {
                User::Leave( KErrNotSupported );
                break;
                }
            }
        }
    else
        {
        data = KNullDesC().AllocLC();
        }
    
    AppendLineL( aItemArray, heading->Des(), data->Des(), itemTextRect );
    CleanupStack::PopAndDestroy( data );
    CleanupStack::PopAndDestroy( heading );


    // Date
    heading = StringLoader::LoadLC( R_DATE );

    data = HBufC::NewLC( KVmMaxLabelStringLength );
    TPtr datePtr = data->Des();
    if ( iSession->Get( KVmDateStamp, datePtr ) == KErrNone )
        {
        FormatDateStringL( datePtr );
        }
        
    AppendLineL( aItemArray, heading->Des(), data->Des(), itemTextRect );
    CleanupStack::PopAndDestroy( data );
    CleanupStack::PopAndDestroy( heading );

    // Time
    heading = StringLoader::LoadLC( R_TIME );

    data = HBufC::NewLC( KVmMaxLabelStringLength );
    TPtr timePtr = data->Des();
    if ( iSession->Get( KVmTimeStamp, timePtr ) == KErrNone )
        {
        FormatTimeStringL( timePtr );
        }

    AppendLineL( aItemArray, heading->Des(), data->Des(), itemTextRect );
    CleanupStack::PopAndDestroy( data );
    CleanupStack::PopAndDestroy( heading );

    // Callback number
    heading = StringLoader::LoadLC( R_CALL_BACK );

    data = HBufC::NewLC( KVmMaxLabelStringLength );
    TPtr callbackPtr = data->Des();
    iSession->Get( KVmCallbackNumber, callbackPtr ); // ignore error

    if ( iNumberGrouping )
        {
        iNumberGrouping->Set( callbackPtr );
        CleanupStack::PopAndDestroy( data );
        data = iNumberGrouping->FormattedNumber().AllocLC();
        }

    AppendLineL( aItemArray, heading->Des(), data->Des(), itemTextRect, ETrue );
    CleanupStack::PopAndDestroy( data );
    CleanupStack::PopAndDestroy( heading );

    // Text
    heading = StringLoader::LoadLC( R_TEXT );

    data = HBufC::NewLC( KVmMaxLabelFreeTextLength );
    TPtr textPtr = data->Des();
    iSession->Get( KVmUserData, textPtr ); // ignore error

    AppendLineL( aItemArray, heading->Des(), data->Des(), itemTextRect, ETrue );
    CleanupStack::PopAndDestroy( data );
    CleanupStack::PopAndDestroy( heading );
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::PopulateListboxItemsL <=" );
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::FormatDateStringL
// Formats date string
// -----------------------------------------------------------------------------
//
void CVmDetailsDialog::FormatDateStringL( TDes& aDateString )
    {
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::FormatDateStringL =>" );
    if ( !aDateString.Length() )
        {
        return;
        }

    // Check that we have two separators '/'
    TInt indexFirst = aDateString.Locate( KVmDateSeparator );
    TInt indexSecond = aDateString.LocateReverse( KVmDateSeparator );
    if ( indexFirst == indexSecond ) // Also -1 is covered
        {
        // Incorrect date format
        User::Leave( KErrCorrupt );
        }

    TLex lex = aDateString.Right( aDateString.Length() - indexSecond - 1 );
    TInt year;
    User::LeaveIfError( lex.Val( year ) );
    
    lex = aDateString.Mid( indexFirst + 1, indexSecond - indexFirst );
    TInt month;
    User::LeaveIfError( lex.Val( month ) );

    lex = aDateString.Left( indexFirst );
    TInt day;
    User::LeaveIfError( lex.Val( day ) );

    // Format date according the locale:
    TDateTime dateTime( year, TMonth( month - 1 ), day, 0, 0, 0, 0 );
    TTime time( dateTime );
    HBufC* dateFormat = StringLoader::LoadLC( R_QTN_DATE_USUAL_WITH_ZERO );
    time.FormatL( aDateString, *dateFormat );
    CleanupStack::PopAndDestroy( dateFormat ); //dateFormatString
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::FormatDateStringL <=" );
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::FormatTimeStringL
// Formats time string
// -----------------------------------------------------------------------------
//
void CVmDetailsDialog::FormatTimeStringL( TDes& aTimeString )
    {
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::FormatTimeStringL =>" );
    if ( !aTimeString.Length() )
        {
        VMBLOGSTRING( "VMBX: CVmDetailsDialog::FormatTimeStringL: \
        Empty time " );
        return;
        }

    // Check that we have two separators ':'
    TInt indexFirst = aTimeString.Locate( KVmTimeSeparator );
    TInt indexSecond = aTimeString.LocateReverse( KVmTimeSeparator );
    if ( indexFirst == indexSecond ) // Also -1 is covered
        {
        VMBLOGSTRING( "VMBX: CVmDetailsDialog::FormatTimeStringL \
        Incorrect date format" );
        // Incorrect date format
        User::Leave( KErrCorrupt );
        }

    TLex lex( aTimeString.Left( indexFirst ) );
    TInt hour;
    User::LeaveIfError( lex.Val( hour ) );
    
    lex = aTimeString.Mid( indexFirst + 1, indexSecond - indexFirst );
    TInt minute;
    User::LeaveIfError( lex.Val( minute ) );
    
    lex = aTimeString.Right( aTimeString.Length() - indexSecond - 1 );
    TInt second;
    User::LeaveIfError( lex.Val( second ) );

    // Format time according the locale:
    TDateTime dateTime( 0, TMonth(0), 0, hour, minute, second, 0 );
    TTime time( dateTime );
    HBufC* timeFormat = StringLoader::LoadLC( R_QTN_TIME_USUAL_WITH_ZERO );
    time.FormatL( aTimeString, *timeFormat );
    CleanupStack::PopAndDestroy( timeFormat ); //dateFormatString
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::FormatTimeStringL <=" );
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::AppendLineL
// Adds line to the listbox.
// -----------------------------------------------------------------------------
//
void CVmDetailsDialog::AppendLineL( CDesCArray& aItemArray, 
                                    const TDesC& aHeading, 
                                    const TDesC& aData, 
                                    TRect aItemTextRect,
                                    TBool aUseMultipleLines )
    {
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::AppendLineL =>" );
    TBuf<1> tabChar(_L("\t"));
    HBufC* combination = NULL;

    if ( aUseMultipleLines )
        {
        CArrayFixFlat<TPtrC>* wrappedTexts = 
            new (ELeave) CArrayFixFlat<TPtrC>( 2 );
        CleanupStack::PushL( wrappedTexts );

        // Layout used for CAknSingleHeadingStyleListBox 
        // is list_single_heading_pane
        // Layout text for list_single_heading_pane_2 (first row from No.2)
        TAknTextLineLayout layout = 
            AKN_LAYOUT_TEXT_List_pane_texts__single_heading__Line_2 (0 );
        TAknLayoutText text;
        text.LayoutText( aItemTextRect, layout );
        TRect rect = text.TextRect();

        // Wrap the long text to several lines.
        AknTextUtils::WrapToArrayL( aData, 
                    rect.Width() /* layout.iW */, 
                    *AknLayoutUtils::FontFromId( layout.FontId() /* iFont */),
                    *wrappedTexts );

        if ( wrappedTexts->Count() == 0 ) // i.e. empty text
            {
            // For creating the first line
            wrappedTexts->AppendL( KNullDesC() );
            }

        // Create the first line:
        combination = HBufC::NewLC( aHeading.Length() + 
                                    tabChar.Length() + 
                                    wrappedTexts->At( 0 ).Length() );

        TPtr comPtr = combination->Des();

        comPtr.Append( aHeading );
        comPtr.Append( tabChar );
        comPtr.Append( wrappedTexts->At( 0 ) );
        
        aItemArray.AppendL( comPtr );
        
        CleanupStack::PopAndDestroy( combination );
        combination = NULL;

        // Create the other lines
        for ( TInt i = 1 ; i < wrappedTexts->Count() ; i++ )
            {
            // combination is created without the header.
            combination = HBufC::NewLC( tabChar.Length() + 
                                    wrappedTexts->At( i ).Length() );

            comPtr.Set( combination->Des() );

            comPtr.Append( tabChar );
            comPtr.Append( wrappedTexts->At( i ) );

            aItemArray.AppendL( comPtr );

            CleanupStack::PopAndDestroy( combination );
            combination = NULL;
            }
        CleanupStack::PopAndDestroy( wrappedTexts );
        }
    else
        {
        // Create combination to one line in listbox.
        combination = HBufC::NewLC( aHeading.Length() + 
                                        tabChar.Length() + 
                                        aData.Length() );

        TPtr comPtr = combination->Des();
        
        comPtr.Append( aHeading );
        comPtr.Append( tabChar );
        comPtr.Append( aData );
        
        aItemArray.AppendL( comPtr );
        
        CleanupStack::PopAndDestroy( combination );
        }
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::AppendLineL =>" );
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::OfferKeyEventL
// Called when a key is pressed.
// -----------------------------------------------------------------------------
//
TKeyResponse CVmDetailsDialog::OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                             TEventCode aType )
    {
    // Special case: exiting with escape key
    if ( aKeyEvent.iCode == EKeyEscape )
        {
        SetOldTitleL();
        return CAknDialog::OfferKeyEventL( aKeyEvent, aType );
        }

    TKeyResponse res = EKeyWasNotConsumed;
    res = Control(KVmDetailsDialog)->OfferKeyEventL( aKeyEvent, aType );
    if (res != EKeyWasNotConsumed) 
        {
        return res;
        }
    
    res = CAknDialog::OfferKeyEventL( aKeyEvent, aType );
    return res;
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::OkToExitL
// Called when softkeys are pressed.
// -----------------------------------------------------------------------------
//
TBool CVmDetailsDialog::OkToExitL( TInt aButtonId )
    {
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::OkToExitL =>" );
    TBool retVal( EFalse );
    // Translate the button presses into commands
    if ( aButtonId == EAknSoftkeyBack )
        { 
        // terminate the diallog.
        SetOldTitleL();
        retVal = ETrue;
        }
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::OkToExitL <=" );
    return retVal;
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::SetOldTitleL
// Saves the old title text.
// -----------------------------------------------------------------------------
//
void CVmDetailsDialog::SetOldTitleL()
    {
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::SetOldTitleL =>" );
    if ( !iOldTitlePaneText )
        {
        VMBLOGSTRING( "VMBX: CVmDetailsDialog::SetOldTitleL: already set" );
        return; // already set
        }

    CAknTitlePane* titlePane = static_cast<CAknTitlePane*> 
        ( iEikonEnv->AppUiFactory()->StatusPane()->ControlL( 
            TUid::Uid( EEikStatusPaneUidTitle ) ) );
    titlePane->SetText( iOldTitlePaneText ); // takes ownership
    iOldTitlePaneText = NULL;
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::SetOldTitleL =>" );
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::HandlePropertyNotificationEventL
// Called when key value is set.
// -----------------------------------------------------------------------------
//
void CVmDetailsDialog::HandlePropertyNotificationEventL( const TDesC& aValue )
    {
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::HandlePropertyNotificationEventL \
    =>" );
    StartObservingL();
    TLex lexer( aValue );
    TInt value = KErrNotFound;
       User::LeaveIfError( lexer.Val( value ) );
    if ( value == 0 ) // message is not set
        {
        // Exit the details dialog
        TryExitL( EAknSoftkeyBack );
        }
    else // new message set
        {
        // Update listbox context:
        CAknSingleHeadingStyleListBox* listbox = 
           static_cast<CAknSingleHeadingStyleListBox*>( 
                                               Control( KVmDetailsDialog ) );
        MDesCArray* itemList = listbox->Model()->ItemTextArray();
        CDesCArray* itemArray = static_cast<CDesCArray*>( itemList );
        PopulateListboxItemsL( *itemArray );
        listbox->HandleItemAdditionL();
        }
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::HandlePropertyNotificationEventL \
    <=" );
    }
// -----------------------------------------------------------------------------
// CVmDetailsDialog::StartObservingL
// Starts observing for key value changes.
// -----------------------------------------------------------------------------
//
void CVmDetailsDialog::StartObservingL()
    {
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::StartObservingL =>" );
    iObserver->ObservePropertyChangeL( KCRUidVoiceMailbox, KVmSummaryInfoIsSet );
    VMBLOGSTRING( "VMBX: CVmDetailsDialog::StartObservingL <=" );
    }

//  End of File  
