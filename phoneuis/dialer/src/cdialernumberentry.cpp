/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Manager for numberentry 
*
*/


// INCLUDE FILES
#include <eikenv.h>
#include <eikapp.h>
#include <gulicon.h>
#include <w32std.h>
#include <baclipb.h>            // CClipboard
#include <dialer.rsg>
#include <eikimage.h>           // CEikImage
#include <barsread.h>           // TResourceReader
#include <eiklabel.h> 
#include <aknappui.h>
#include <AknUtils.h>
#include <AknsDrawUtils.h>
#include <applayout.cdl.h>
#include <AknLayoutFont.h>
#include <AknLayout2Def.h>
#include <AknPhoneNumberEditor.h>
#include <AknLayout2ScalableDef.h>
#include <AknsBasicBackgroundControlContext.h>
#include <AknsFrameBackgroundControlContext.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>

#include "cdialernumberentry.h"
#include "dialercommon.h"
#include "dialertrace.h"
#include "dialer.hrh"
#include "mnumberentry.h"
#include    <dialingextensioninterface.h>
#include    <phoneappcommands.hrh>

const TInt KNumberEntryControlCount = 2; //  = number entry, label

_LIT( KPhoneValidChars, "0123456789*#+pwPW" );

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// NewL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
CDialerNumberEntry* CDialerNumberEntry::NewL( 
    const CCoeControl& aContainer )
    {
    CDialerNumberEntry* self = 
        new( ELeave )CDialerNumberEntry( aContainer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();    // self
    return self;
    }


// ---------------------------------------------------------------------------
// ConstructL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::ConstructL()
    {   
    DIALER_PRINT("numberentry::ConstructL<");  
    BaseConstructL();
 
    // Create number entry editor    
    iEditor = new( ELeave ) CAknPhoneNumberEditor;
    iEditor->SetContainerWindowL( *this );
    iEditor->SetParent( this );
    iEditor->SetMopParent( this );
    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC( reader, R_INPUTFIELD );
    ConstructEditorFromResourceL( reader );
    
    iFrameContext = CAknsFrameBackgroundControlContext::NewL(
            KAknsIIDNone, TRect(0,0,4,4), TRect(1,1,3,3), EFalse );    
            
    iFrameContext->SetFrame( KAknsIIDQsnFrCall2Rect );      
    
    iFrameContext->SetParentContext( 
        AknsDrawUtils::ControlContextOfParent(this) );           
                
    CleanupStack::PopAndDestroy();    // reader    
    iEditor->SetObserver( this );
    
    iLabel = new( ELeave ) CEikLabel;
    iLabel->SetContainerWindowL( *this );
    iLabel->SetParent( this );
    iLabel->SetMopParent( this ); 
    iLabel->SetTextL( KNullDesC );
    iLabel->MakeVisible( EFalse );    
    CheckLabelSkinningColor();
    
    iAppUi = iEikonEnv->EikAppUi();
    __ASSERT_ALWAYS( iAppUi, DialerPanic( EDialerPanicNoApplicationInstance ) );
     
    ActivateL();
    DIALER_PRINT("numberentry::ConstructL>");
    }


// Constructor
CDialerNumberEntry::CDialerNumberEntry(
    const CCoeControl& aContainer )
    : CDialerContainerBase ( const_cast<CCoeControl&>(aContainer)  )
	{
	}


// Destructor
CDialerNumberEntry::~CDialerNumberEntry()
    {
    AknsUtils::DeregisterControlPosition( this );
    
    delete iEditor;
    delete iFrameContext;
    delete iLabel;
    }


// ---------------------------------------------------------------------------
// CDialerNumberEntry::SetNumberEntryObserver 
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::SetNumberEntryObserver( 
                                MNumberEntryObserver& aObserver )
    {
    iObserver = &aObserver;
    }
        
// ---------------------------------------------------------------------------
// CDialerNumberEntry::SetFocus
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::SetFocus( TBool aFocus, TDrawNow aDrawNow )
    {
    DIALER_PRINT("numberentry::SetFocus<");
    
    CCoeControl::SetFocus( aFocus, aDrawNow );
    iEditor->SetFocus( aFocus );
     
    DIALER_PRINT("numberentry::SetFocus>");    
    }


// ---------------------------------------------------------------------------
// CDialerNumberEntry::TextLength 
// ---------------------------------------------------------------------------
//
TInt CDialerNumberEntry::TextLength()
    {
    return iEditor->TextLength();
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::GetNumberEntry
// ---------------------------------------------------------------------------
//
CCoeControl* CDialerNumberEntry::GetNumberEntry( ) const
    {
    return iEditor;
    }
    
// ---------------------------------------------------------------------------
// CDialerNumberEntry::CreateNumberEntry
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::CreateNumberEntry()
    {
    iEditor->SetFocus( ETrue );
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::ChangeEditorMode
// ---------------------------------------------------------------------------
//
TInt CDialerNumberEntry::ChangeEditorMode( TBool aDefaultMode )
    {
    return iEditor->ChangeEditorMode( aDefaultMode );
    }
    
// ---------------------------------------------------------------------------
// CDialerNumberEntry::GetEditorMode  
// ---------------------------------------------------------------------------
//
TInt CDialerNumberEntry::GetEditorMode() const
    {
    return iEditor->GetEditorMode();
    }
    
// ---------------------------------------------------------------------------
// CDialerNumberEntry::SetTextToNumberEntry
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::SetTextToNumberEntry( const TDesC& aDesC )
    {
    DIALER_PRINT("numberentry::SetTextToNumberEntry<");
        	
    iEditor->SetText( aDesC );   
    
    if ( iEasyDialer && iOperationMode == EModeEasyDialing )
        {
        TRAP_IGNORE( iEasyDialer->SetInputL( aDesC ) );
        }
    
    iEditor->DrawNow();
    
    DIALER_PRINT("numberentry::SetTextToNumberEntry>");
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::GetTextFromNumberEntry
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::GetTextFromNumberEntry( TDes& aDes )
    {
    iEditor->GetText( aDes );
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::Text
// ---------------------------------------------------------------------------
//
TPtrC CDialerNumberEntry::Text() const
    {
    return iEditor->Text();
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::ResetEditorToDefaultValues
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::ResetEditorToDefaultValues()
    {
    iEditor->SetFocus( EFalse );
    SetTextToNumberEntry( KNullDesC );
    iEditor->ResetEditorToDefaultValues();
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::SetNumberEntryPromptTextL
//  
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::SetNumberEntryPromptTextL( const TDesC& aPromptText )
    {
    if ( aPromptText.Length() )
        {
        iLabel->MakeVisible( ETrue );
        }
    else
        {
        iLabel->MakeVisible( EFalse );
        }
    
    iLabel->SetTextL( aPromptText );   
    }

// ----------------------------------------------------------------------------
// CDialerNumberEntry::HandleCommandL
// Handles NE specific commands.
// ----------------------------------------------------------------------------
//
TBool CDialerNumberEntry::HandleCommandL( TDialerCommandId aCommand)
    {
    DIALER_PRINTF("numberentry::HandleCommandL<:", aCommand); 
    	
    TBool handled( ETrue );
    switch ( aCommand )
        {
        case EDialerCmdTouchInput:  // open touch input (VKB)
            {
            StartVirtualKeyBoard();
            }
            break;
        default:
            {
            handled = EFalse;
            }
            break;
        }
        
    DIALER_PRINT("numberentry::HandleCommandL>");         
    return handled;
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::ClearEditorFlags
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::ClearEditorFlags()
    {
    iEditor->ResetEditorToDefaultValues();
    }

// ---------------------------------------------------------------------------
//  CDialerNumberEntry::HandleControlEventL
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::HandleControlEventL( CCoeControl* aControl, 
                                              TCoeEvent aEventType )
    {
    DIALER_PRINT("numberentry::HandleControlEventL<"); 
    
    if ( aControl == iEditor && aEventType == EEventStateChanged )
        {
        HandleEditorFormatting();
        
        InformNumberEntryState();        
        
        iEditor->DrawDeferred();
        }
    
    DIALER_PRINT("numberentry::HandleControlEventL>");                
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::OfferKeyEventL
// ---------------------------------------------------------------------------
//
TKeyResponse CDialerNumberEntry::OfferKeyEventL( 
                                            const TKeyEvent& aKeyEvent, 
                                            TEventCode aType )
    {       
    TKeyResponse handled = EKeyWasNotConsumed;
        
    // First, offer keyevent to easy dialer.
    if (iEasyDialer)
        {
        handled = iEasyDialer->OfferKeyEventL(aKeyEvent, aType);
        }
        
    // Then to number entry editor. 
    if (handled == EKeyWasNotConsumed)
        {
        handled = iEditor->OfferKeyEventL(aKeyEvent, aType);
        }
        
    return handled;
    }
    
// ---------------------------------------------------------------------------
// CDialerNumberEntry::CountComponentControls 
// ---------------------------------------------------------------------------
//
TInt CDialerNumberEntry::CountComponentControls() const
    {
    return KNumberEntryControlCount;
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::ComponentControl
// ---------------------------------------------------------------------------
//
CCoeControl* CDialerNumberEntry::ComponentControl( TInt aIndex ) const
    {
    __ASSERT_DEBUG( aIndex < KNumberEntryControlCount, 
                             User::Panic(_L("Dialer"), KErrArgument) );
    
    if ( aIndex == 0 )
        {
        return iEditor;
        }
    else if ( aIndex == 1 )
        {
        return iLabel;
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::Draw
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::Draw( const TRect& /*aRect*/ ) const
    {
    CWindowGc& gc = SystemGc( );
    gc.SetBrushColor( AKN_LAF_COLOR( 0 /** White - get from layout */ ));
    TRect rect = Rect();
    
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    
    // Draw the skin background of the parent 
    if ( !AknsDrawUtils::DrawFrame( skin,
                             (CWindowGc&) gc,  
                             iOuterRect,
                             iInnerRect,
                             KAknsIIDQsnFrCall2Rect,
                             KAknsIIDDefault ))
        {
        DIALER_PRINT("numberentry::Draw.Frame.failed"); 
        }
                            
    gc.SetBrushStyle( CGraphicsContext::ENullBrush );        
    }


// ---------------------------------------------------------------------------
// CDialerNumberEntry::SetVariety
// Set variety according to status
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::SetVariety()
    {
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::SetLayout
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::SetLayout()
    {
    AknsUtils::RegisterControlPosition( this );

    TDialerVariety variety( EDialerVarietyLandscape );
    if ( !Layout_Meta_Data::IsLandscapeOrientation() )
        {
        variety = EDialerVarietyPortrait;
        }      
        
    TRect parentRect( Rect() );
    TAknTextComponentLayout neLayout = ( iOperationMode == EModeEasyDialing ?
        AknLayoutScalable_Apps::dia3_numentry_pane_t1( variety ) :
        AknLayoutScalable_Apps::dialer2_ne_pane_t1( variety ) );

    AknLayoutUtils::LayoutControl(
        iEditor, parentRect, 
        neLayout.C(),
        neLayout.l(),
        neLayout.t(),
        neLayout.r(),
        neLayout.b(),
        neLayout.W(),
        neLayout.H()
        );

    RectFrameInnerOuterRects(
           parentRect,
           iOuterRect,
           iInnerRect );

    UpdateNumberEntryFormats();
      
    iFrameContext->SetFrameRects( iOuterRect, iInnerRect );
                
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    AknsDrawUtils::PrepareFrame( skin,
                                 iOuterRect,
                                 iInnerRect,
                                 KAknsIIDQsnFrCall2Rect,
                                 KAknsIIDDefault ); 
    // Label layout

    // Portrait variety is temporarily used also in landscape.
    // Landscape layout data is now unavailable. 
    TAknTextComponentLayout labelLayout = 
        AknLayoutScalable_Apps::dialer2_ne_pane_t2( EDialerVarietyPortrait );
    // Temporary solution 
    labelLayout.Setl( neLayout.l() );
    AknLayoutUtils::LayoutLabel( iLabel, parentRect, labelLayout );
    
    HandleEditorFormatting();
    iEditor->DrawDeferred();
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::ConstructEditorFromResourceL 
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::ConstructEditorFromResourceL( 
                                TResourceReader& aReader )
    {
    TInt maxChars = aReader.ReadInt16();
    TInt maxLines = aReader.ReadInt16();
    HBufC* truncation = aReader.ReadHBufCL();
    CleanupStack::PushL( truncation );
    TInt formatCount = aReader.ReadInt16();
    iEditor->ConstructL( maxChars, maxLines, formatCount, *truncation );  
    CleanupStack::PopAndDestroy( truncation );       
    iFormatCount = formatCount;

    for ( TInt ii = 0; ii < iFormatCount; ii++ )
        {
        CAknPhoneNumberEditor::TFormat format( aReader );
	    CalculateLayout( format, ii );
        iEditor->AddFormat( format );
        }

    iEditor->SetFormat( KDialerNELayoutLargeFont );
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::UpdateNumberEntryFormats
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::UpdateNumberEntryFormats( )
    {
    DIALER_PRINT("numberentry::UpdateNumberEntryFormats<");
    TInt currentFormat = iEditor->CurrentFormatIndex();

    for ( TInt ii = 0; ii < iFormatCount; ii++ )
        {
        CAknPhoneNumberEditor::TFormat format( iEditor->Format( ii ) );
		CalculateLayout( format, ii );
        iEditor->Format( ii ) = format;
        }

    iEditor->SetFormat( currentFormat );
    DIALER_PRINT("numberentry::UpdateNumberEntryFormats>");
    }
    
// ---------------------------------------------------------------------------
// CDialerNumberEntry::CalculateLayout
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::CalculateLayout( 
        CAknPhoneNumberEditor::TFormat& aFormat, TInt aNEVariety )
    {
    TAknLayoutText textsLine1Text;
    TAknLayoutText textsLine2Text;

    if ( iOperationMode == EModeEasyDialing )
        {
        TInt variety = Layout_Meta_Data::IsLandscapeOrientation() ? 1 : 0;
        textsLine1Text.LayoutText( Rect(), AknLayoutScalable_Apps::dia3_numentry_pane_t1(variety) );        
        textsLine2Text = textsLine1Text;
        }
    else
        {
        TInt variety = 0;
        if ( aNEVariety == KDialerNELayoutLargeFont )
            {
            variety = Layout_Meta_Data::IsLandscapeOrientation() ? 2 : 0;
            }
        else
            {
            variety = Layout_Meta_Data::IsLandscapeOrientation() ? 3 : 1;
            }

        //Line 1
        textsLine1Text.LayoutText( Rect(), 
            AknLayoutScalable_Apps::dialer2_ne_pane_t1( variety, 0, 0 ) );  
        //Line 2     
        textsLine2Text.LayoutText( Rect(), 
            AknLayoutScalable_Apps::dialer2_ne_pane_t1( variety, 0, 1 ) );
        }
    
    //take font from line 1
    aFormat.iFont = textsLine1Text.Font();
    const CAknLayoutFont* layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( aFormat.iFont );
    aFormat.iBottomBaselineOffset = layoutFont->BaselineToTextPaneBottom();

    // Calculate baselineSeparation from the difference of the second 
    // line and the first line bottom right Y coordinates.
    TInt baselineSeparation = 
        textsLine2Text.TextRect().iBr.iY - textsLine1Text.TextRect().iBr.iY;
    aFormat.iBaselineSeparation = baselineSeparation;

    // Outer rect of the text editor is the inner rect of the whole text box
    aFormat.iOuterRect = iInnerRect;
    
    aFormat.iNumLines = 1;
    
    // If multiline layout is to be used, the exact number of lines is calculated 
    // based on available area and the row height (i.e. baselineSeparation)
    if ( baselineSeparation > 0 && baselineSeparation < iInnerRect.Height() )
        {
        aFormat.iNumLines = ( iInnerRect.Height() / baselineSeparation );

        // Center editor vertically to the available area.
        TInt editorHeight = aFormat.iNumLines * baselineSeparation;
        TInt deltaHeight = ( iInnerRect.Height() - editorHeight ) / 2;
        aFormat.iOuterRect.Shrink( 0, deltaHeight );
        }
	}

// ---------------------------------------------------------------------------
// CDialerNumberEntry::HandleResourceChange  
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    
    if ( aType == KAknsMessageSkinChange )
        {
        CheckLabelSkinningColor(); 
        }
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::HandlePointerEventL  
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    // Normal pointer event handling and delegation to editor component 
    // happens only if we are already in focus. Otherwise we just take the
    // focus. This is to prevent accidental cursor movements as putting the
    // cursor back to the end of the text is difficult for right-aligned
    // text.
    if ( iEditor->IsFocused() )
        {
        CCoeControl::HandlePointerEventL( aPointerEvent );
        }
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::HandleEditorFormatting  
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::HandleEditorFormatting()
    {
    TInt format( KDialerNELayoutLargeFont );
  
    if ( !iEditor->WouldTextFitInFormat( KDialerNELayoutLargeFont )  )
        {
        format = KDialerNELayoutSmallFont;
        }
   
    if ( iEditor->CurrentFormatIndex() != format )
        {
        iEditor->DrawDeferred();
        iEditor->SetFormat( format );              
        iFrameContext->SetRect( Rect() );                
        }
    
    }

// ---------------------------------------------------------------------------
// CDialerNumberEntry::StartVirtualKeyBoard 
// ---------------------------------------------------------------------------
//
void CDialerNumberEntry::StartVirtualKeyBoard( )
    {
    // To change focus to VKB, if not called VKB will not come visible
    CEikonEnv::Static()->SyncNotifyFocusObserversOfChangeInFocus();
    iEditor->OpenVKB();
    }
  
// ---------------------------------------------------------------------------
// CDialerNumberEntry::MopSupplyObject
// ---------------------------------------------------------------------------
//
TTypeUid::Ptr CDialerNumberEntry::MopSupplyObject( TTypeUid aId )  
    {
    if ( aId.iUid == MAknsControlContext::ETypeId && iFrameContext )
        {
        return MAknsControlContext::SupplyMopObject( aId, iFrameContext );
        }
    else
        {
        return CCoeControl::MopSupplyObject( aId );
        } 
    }    
    
// -----------------------------------------------------------------------------
// CDialerNumberEntry::RectFrameInnerOuterRects
// -----------------------------------------------------------------------------
//
void CDialerNumberEntry::RectFrameInnerOuterRects( const TRect& aFrameRect,
                                              TRect& aOuterRect,
                                              TRect& aInnerRect )
    {
    if ( iOperationMode == EModeEasyDialing )
        {
        aOuterRect = Rect();
        TAknLayoutRect innerRectLayout;
        innerRectLayout.LayoutRect( aOuterRect, AknLayoutScalable_Apps::bg_dia3_numentry_pane_g1() );
        aInnerRect = innerRectLayout.Rect();
        }
    else
        {
        if ( Layout_Meta_Data::IsMirrored() )
            {
            TAknLayoutRect frameTopRight;        
            frameTopRight.LayoutRect( 
             aFrameRect, 
             AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g3().LayoutLine() );                
                    
            TAknLayoutRect frameBottomLeft;        
            frameBottomLeft.LayoutRect( 
             aFrameRect, 
             AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g4().LayoutLine() );                        
    
            aOuterRect = TRect( frameTopRight.Rect().iTl, 
                                frameBottomLeft.Rect().iBr );
                    
            aInnerRect = TRect( frameTopRight.Rect().iBr, 
                                frameBottomLeft.Rect().iTl );    
            }
        else
            {
            TAknLayoutRect frameTopLeft;        
            frameTopLeft.LayoutRect( 
             aFrameRect, 
             AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g2().LayoutLine() );
    
            TAknLayoutRect frameBottomRight;        
            frameBottomRight.LayoutRect( 
             aFrameRect, 
             AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g5().LayoutLine() );
            
            aOuterRect = TRect( frameTopLeft.Rect().iTl, 
                                frameBottomRight.Rect().iBr );
            aInnerRect = TRect( frameTopLeft.Rect().iBr, 
                                frameBottomRight.Rect().iTl );    
            }
        }
    }    
    
// -----------------------------------------------------------------------------
// CDialerNumberEntry::InformNumberEntryState()
// -----------------------------------------------------------------------------
//
void CDialerNumberEntry::InformNumberEntryState()
    {
    if ( !iNumberContents && iEditor->TextLength() )
        {
        // Hide prompt text
        if ( iLabel->Text()->Length() )
            {
            iLabel->MakeVisible( EFalse );     
            }         
        iNumberContents = ETrue;
        }
    else if ( iNumberContents && !iEditor->TextLength() )
        {  
        // Show prompt text
        if ( iLabel->Text()->Length() )
            {
            iLabel->MakeVisible( ETrue );     
            } 
        iNumberContents = EFalse;
        } 

    if ( iPreviousNumberEntryContent.Compare( iEditor->Text() ) != 0 )
        {
        iPreviousNumberEntryContent.Zero();
        iEditor->GetText( iPreviousNumberEntryContent );

        // The following stores the information of validity of the number for normal calls.
        // This information is needed so that we don't show call items for invalid numbers.
        TBool validPhoneNumber = Validate( iEditor->Text() );
        TInt commandId = validPhoneNumber ? EPhoneDialerValidNumber : EPhoneDialerInvalidNumber;
        
        TRAP_IGNORE( iAppUi->HandleCommandL( commandId ) );
    
        if ( iEasyDialer && iOperationMode == EModeEasyDialing )
            {
            TRAP_IGNORE( iEasyDialer->SetInputL( iEditor->Text() ) );
            }

        if ( iObserver )
            {
            iObserver->NumberEntryStateChanged( iNumberContents );
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CDialerNumberEntry::CheckLabelSkinningColor()
// -----------------------------------------------------------------------------
//
void CDialerNumberEntry::CheckLabelSkinningColor()
    {
    // Check skinning colour for the prompt text:
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRgb skinColor;
    TInt error = 
        AknsUtils::GetCachedColor( 
            skin, 
            skinColor, 
            KAknsIIDQsnTextColors,
            EAknsCIQsnTextColorsCG51 );
            
    if ( error == KErrNone )
        {
        // Ignore error
        TRAP_IGNORE( iLabel->OverrideColorL( EColorLabelText, skinColor ) );
        }   
    }
    
    
// CDialerNumberEntry::MakeVisible( TBool aVisible )
// -----------------------------------------------------------------------------
//	
void CDialerNumberEntry::MakeVisible( TBool aVisible )
	{
	CCoeControl::MakeVisible( aVisible );
	iEditor->MakeVisible( aVisible );
	}
	
	
// -----------------------------------------------------------------------------
// CDialerNumberEntry::SetEasyDialingPlugin
// -----------------------------------------------------------------------------
//
void CDialerNumberEntry::SetEasyDialingPlugin(CDialingExtensionInterface* aEasyDialer)
    {
    DIALER_PRINT("numberentry::SetEasyDialingPlugin");
    iEasyDialer = aEasyDialer;
    }

// -----------------------------------------------------------------------------
// CDialerNumberEntry::SetOperationMode
// -----------------------------------------------------------------------------
//
void CDialerNumberEntry::SetOperationMode( TDialerOperationMode aMode )
    {
    iOperationMode = aMode;
    }

// -----------------------------------------------------------------------------
// CDialerNumberEntry::Validate
//
// Copied from cphonekeys.
// -----------------------------------------------------------------------------
//
TBool CDialerNumberEntry::Validate( const TDesC& aString )
    {
    DIALER_PRINT("numberentry::Validate");
    TLex input( aString );
    TPtrC valid( KPhoneValidChars );

    while ( valid.Locate( input.Peek() ) != KErrNotFound )
        {
        input.Inc();
        }
    
    return !input.Remainder().Length();
    }

// End of File
