/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*             Declares container control for application.
*
*/


// INCLUDE FILES

#include    <ussd.rsg>
#include    <aknkeys.h>
#include    <eikedwin.h>
#include    <bldvariant.hrh>
#include    <featmgr.h>
#include    <centralrepository.h>
#include    <telinternalcrkeys.h>
#include <csxhelp/ussd.hlp.hrh>  // for help context

// For skinning.
#include    <AknsBasicBackgroundControlContext.h>
#include    <AknsDrawUtils.h>
#include    <AknsUtils.h>
#include    <txtglobl.h>

#include    "telephonyvariant.hrh"
#include    "UssdComms.h"
#include    "UssdNaviPane.h"
#include    "UssdAppUi.h"
#include    "UssdContainer.h"
#include    "UssdEditorLines.h"
#include    "UssdLayout.h"
#include    "ussd.hrh"
#include    "UssdApp.h" // for applicationUID
#include    "UssdLogger.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUssdContainer::CUssdContainer
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUssdContainer::CUssdContainer( CUssdAppUi& aAppUi ): iAppUi( aAppUi )
    {
    }


// -----------------------------------------------------------------------------
// CUssdContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUssdContainer::ConstructL( const TRect& aRect )
    {
    _LOGSTRING( "CUssdContainer::ConstructL =>" )
    CreateWindowL();

    // Create background control context for skins.
    iBgContext = CAknsBasicBackgroundControlContext::NewL(
        KAknsIIDQsnBgAreaMainMessage, aRect, ETrue /*Parent absolute*/ );

    // Create editor
    iEditor = CreateEdwinL();

    // Lines
    iLines = new ( ELeave ) CUssdEditorLines;
    iLines->ConstructL( this );

    TInt featureBitmask( 0 );

    // Connecting and initialization KCRUidTelVariation
    CRepository* repository = CRepository::NewL( KCRUidTelVariation );
    CleanupStack::PushL( repository );

    // Get the value from Central Repository
    repository->Get( KTelVariationFlags, featureBitmask );
    if ( featureBitmask & KTelephonyLVFlagUssdUIRemainingChars )
        {
        iNaviPaneHandler = CUssdNaviPane::NewL( iAppUi );
        }

    CleanupStack::PopAndDestroy( repository );

    // Finish the job
    SetRect( aRect );
    ActivateL();

    iEditor->SetFocus( ETrue );
    _LOGSTRING( "CUssdContainer::ConstructL <=" )
    }


// Destructor
CUssdContainer::~CUssdContainer()
    {
    _LOGSTRING( "CUssdContainer::~CUssdContainer =>" ) 
    delete iLines;

    AknsUtils::DeregisterControlPosition( iEditor );
    delete iEditor;
    delete iNaviPaneHandler;

    delete iBgContext; // For skinning, can be deleted, NULL if not exist.
    _LOGSTRING( "CUssdContainer::~CUssdContainer <=" )
    }

// -----------------------------------------------------------------------------
// CUssdContainer::HandleEdwinEventL
// Called by framework when the Editor is changed
//
// -----------------------------------------------------------------------------
//
void CUssdContainer::HandleEdwinEventL( CEikEdwin* /*aEdwin*/,
        TEdwinEvent aEventType )
    {
     switch( aEventType )
         {
         case EEventTextUpdate:
             {
             UpdateNavipaneMsgLengthL();
             break;
             }
         default:
             break;
         }
    }
    
// -----------------------------------------------------------------------------
// CUssdContainer::SizeChanged
// Called by framework when the view size is changed
//
// -----------------------------------------------------------------------------
//
void CUssdContainer::SizeChanged()
    {
    // Backgroud context
    iBgContext->SetParentPos( PositionRelativeToScreen() );
    iBgContext->SetRect( Rect() ) ;
    // Edwin
    AknLayoutUtils::LayoutEdwin(
        iEditor,
        Rect(),
        UssdLayout::MessageWritingTexts3() );

    // We need to make some room for scrollbar to fit
    // so the editor must be narrowed down a bit.

    TAknLayoutId layout = UssdLayout::CurrentLayout();
    TRect sbr = iEditor->ScrollBarFrame()->VerticalScrollBar()->Rect();

    if( layout != EAknLayoutIdABRW )
        {
        TPoint editorTLPoint(Rect().iTl.iX, iEditor->Rect().iTl.iY);

        iEditor->SetExtent( editorTLPoint ,
                    TSize( Rect().Width() - sbr.Width(), sbr.Height() ) );
        }
    else // right to left layout
        {
        TPoint editorTLPoint( ( Rect().iTl.iX + sbr.Width() ),
                                iEditor->Rect().iTl.iY);

        iEditor->SetExtent( editorTLPoint ,
                            TSize( Rect().Width() - sbr.Width(),
                            sbr.Height() ) );
        }

    TRAP_IGNORE( UpdateNavipaneMsgLengthL() );

    // overwite inputmode in portraitmode
    if ( Rect().Height() > Rect().Width() )
        {
        iEditor->SetAknEditorCurrentInputMode( EAknEditorNumericInputMode );
        }

    // Change the colour of the edwin if needed
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    if ( skin )
        {
        TRgb color;
        TInt error = AknsUtils::GetCachedColor(
            skin,
            color,
            KAknsIIDQsnTextColors,
            EAknsCIQsnTextColorsCG6 );

        if ( error == KErrNone )
            {
            TCharFormat charFormat;
            TCharFormatMask charFormatMask;
            ( reinterpret_cast< CGlobalText* >( iEditor->Text() ) )
                ->GetCharFormat( charFormat, charFormatMask, 0, 0 );

            charFormat.iFontPresentation.iTextColor = color;
            charFormatMask.SetAll();

            CCharFormatLayer* charFormatLayer = NULL;

            TRAP( error, charFormatLayer =
                CCharFormatLayer::NewL( charFormat,charFormatMask ) );

            if ( !error )
                {
                iEditor->SetCharFormatLayer( charFormatLayer );
                }
            }
        }

    // Set editor lines
    iLines->SetRect( Rect() );

    AknsUtils::RegisterControlPosition( iEditor );
    }


// -----------------------------------------------------------------------------
// CUssdContainer::CountComponentControls
//
//
// -----------------------------------------------------------------------------
//
TInt CUssdContainer::CountComponentControls() const
    {
    return 2; // editor and lines
    }


// -----------------------------------------------------------------------------
// CUssdContainer::ComponentControl
//
// -----------------------------------------------------------------------------
//
CCoeControl* CUssdContainer::ComponentControl( TInt aIndex ) const
    {
    switch ( aIndex )
        {
        case 0:
            return iEditor;
        case 1:
            return iLines;
        default:
            return NULL;
        }

    }


// -----------------------------------------------------------------------------
// CUssdContainer::Draw
//
// -----------------------------------------------------------------------------
//
void CUssdContainer::Draw( const TRect& aRect ) const
    {
    CWindowGc& gc = SystemGc();

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    if ( iBgContext )
        {
        AknsDrawUtils::Background(
            skin, iBgContext, this, gc, aRect);
        }
    else
        {
        // Otherwise clear the area
        gc.SetBrushColor( AKN_LAF_COLOR( UssdLayout::LayoutColourWhite() ) );
        DrawUtils::ClearBetweenRects( gc, aRect, iEditor->Rect() );
        }
    }


// -----------------------------------------------------------------------------
// CUssdContainer::OfferKeyEventL
// -----------------------------------------------------------------------------
//
TKeyResponse CUssdContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    _LOGSTRING3(
    "CUssdContainer::OfferKeyEventL =>,TKeyEvent::iCode=%d,TEventCode=%d",
    aKeyEvent.iCode, aType )
    TKeyResponse response = EKeyWasNotConsumed;

    // Special key handling comes first.
    if ( aKeyEvent.iScanCode == EStdKeyYes ) // Send key.
        {
        // The reason that the send key is handled via TStdScanCode rather than TKeyCode is that
        // we also have to be able to catch EEventKeyUp and EEventKeyDown key events in addition
        // to EEventKey. This is because TKeyEvent::iCode is always 0 when TEventCode is EEventKey.
        if ( iEditor->TextLength() )
            {
            if ( aType == EEventKey )
                {
                if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                    {
                    iAppUi.HandleCommandL( EUssdMenuItemSend );
                    }
                else
                    {
                    iAppUi.HandleCommandL( EUssdSoftkeySend );
                    }
                }
            response = EKeyWasConsumed; // Dialer should not be launched when the USSD string exists and send key is used.
            }
        }
    else if ( aKeyEvent.iCode == EKeyEscape )
        {
        iAppUi.HandleCommandL( EAknSoftkeyExit );
        response = EKeyWasConsumed;
        }
    else // Default keys are handled here.
        {
        response = iEditor->OfferKeyEventL( aKeyEvent, aType );

        if ( aType == EEventKey || aType == EEventKeyUp )
            {
            UpdateNavipaneMsgLengthL();
            }

        if ( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            // Change softkeys if needed.
            iAppUi.SetSoftkeySendVisibleL( iEditor->TextLength() != 0 );
            }

            iAppUi.Cba()->SetCommandSetL( R_USSD_SOFTKEYS_OPTIONS_EXIT_OPTIONS );
            iAppUi.Cba()->DrawNow();
        }
    _LOGSTRING2(
    "CUssdContainer::OfferKeyEventL <=, response=%d", response )
    return response;
    }


// -----------------------------------------------------------------------------
// CUssdContainer::Editor
// Give editor
// -----------------------------------------------------------------------------
//
CEikEdwin& CUssdContainer::Editor()
    {
    return *iEditor;
    }


// -----------------------------------------------------------------------------
// CUssdContainer::FocusChanged
//
// -----------------------------------------------------------------------------
//
void CUssdContainer::FocusChanged( TDrawNow aDrawNow )
    {
    iEditor->SetFocus( IsFocused(), aDrawNow );
    }



// -----------------------------------------------------------------------------
// CUssdContainer::GetHelpContext
// This function is called when Help application is launched.
//
// -----------------------------------------------------------------------------
//
void CUssdContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    // If help defined.
    if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
        {
        aContext.iMajor = KUidussd;
        aContext.iContext = KUSSD_HLP_EDITOR;
        }

    // If help not defined, do nothing
    }



// -----------------------------------------------------------------------------
// CUssdContainer::HandleResourceChange
// This function is called when Help application is launched.
//
// -----------------------------------------------------------------------------
//
void CUssdContainer::HandleResourceChange( TInt aType )
    {
    switch ( aType )
        {
        case KEikColorResourceChange:
        case KAknsMessageSkinChange:
            {
            SizeChanged();
            DrawDeferred();
            break;
            }

        default:
            {
            CCoeControl::HandleResourceChange( aType );
            }
        }
    }


// -----------------------------------------------------------------------------
// CUssdContainer::MopSupplyObject
//
// -----------------------------------------------------------------------------
//
TTypeUid::Ptr CUssdContainer::MopSupplyObject( TTypeUid aId )
    {
    if ( aId.iUid == MAknsControlContext::ETypeId )
        {
        return MAknsControlContext::SupplyMopObject( aId, iBgContext );
        }
    return CCoeControl::MopSupplyObject( aId );
    }


// -----------------------------------------------------------------------------
// CUssdContainer::CreateEdwinL
// Creates new edwin
//
// -----------------------------------------------------------------------------
//
CEikEdwin* CUssdContainer::CreateEdwinL()
    {
    CEikEdwin* newEditor = new ( ELeave ) CEikEdwin;
    CleanupStack::PushL( newEditor );
    newEditor->SetContainerWindowL( *this );
    //Set the editor observer
    newEditor->SetEdwinObserver( this );
    
    newEditor->SetAknEditorAllowedInputModes(
        EAknEditorNumericInputMode | EAknEditorTextInputMode );
    newEditor->SetAknEditorInputMode( EAknEditorNumericInputMode );
    newEditor->SetAknEditorNumericKeymap( EAknEditorStandardNumberModeKeymap );

    TInt editorFlags = EAknEditorFlagNoT9 | EAknEditorFlagEnableScrollBars;

    if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
        {
        editorFlags |= EAknEditorFlagLatinInputModesOnly;
        }

     if( FeatureManager::FeatureSupported( KFeatureIdPenSupport ) )
        {
        editorFlags |= EAknEditorFlagDeliverVirtualKeyEventsToApplication;
        }

    newEditor->SetAknEditorFlags( editorFlags );

    // Get the layout, i.e. variant.
    EVariantFlag variant = AknLayoutUtils::Variant();

    // According to layout, construct the editor.
    if ( variant == EApacVariant )
        {
        // APAC variant
        AknEditUtils::ConstructEditingL( newEditor , R_USSD_EDWIN_EDITOR_APAC );
        }
    else
        {
        // EEuropeanVariant (includes ABRW)
        AknEditUtils::ConstructEditingL( newEditor , R_USSD_EDWIN_EDITOR_ELAF );
        }

    CleanupStack::Pop( newEditor );
    return newEditor;
    }

// ---------------------------------------------------------
// CUssdContainer::UpdateNavipaneMsgLengthL
// ---------------------------------------------------------
void CUssdContainer::UpdateNavipaneMsgLengthL()
    {
    _LOGSTRING( "CUssdContainer::UpdateNavipaneMsgLengthL =>" )

    if ( iNaviPaneHandler )
        {

        TInt maxLen=0;
        TBuf<KUssdEditorMaxLenght> inputTexts;

        iEditor->GetText( inputTexts );

        maxLen = iNaviPaneHandler->UpdateMsgLengthL( inputTexts, this );

        // if pasted more than 91 chinese characters then it must be
        // cutted and set correct message length in navi pane
        if ( inputTexts.Length() > maxLen )
            {
            inputTexts.Copy( inputTexts.Left( maxLen ) );
            iEditor->SetTextL( &inputTexts );
            iNaviPaneHandler->UpdateMsgLengthL( inputTexts, this );
            iEditor->DrawNow();
            iEditor->SetCursorPosL( maxLen, EFalse );
            }

        iEditor->SetMaxLength( maxLen );

        }
    _LOGSTRING( "CUssdContainer::UpdateNavipaneMsgLengthL <=" )
    }

// End of File
