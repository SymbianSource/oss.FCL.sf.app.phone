/*
* Copyright (c) 2007 - 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Manager for keypad and numberentry container.
*
*/


// INCLUDE FILES

#include <avkon.rsg>
#include <apgcli.h>
#include <apacmdln.h>
#include <eikspane.h>
#include <aknenv.h>                         // AKN_LAF_COLOR
#include <AknUtils.h>
#include <AknsUtils.h>
#include <aknappui.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>
#include <data_caging_path_literals.hrh>    // for KDC_APP_RESOURCE_DIR
#include <bautils.h>                        // for BaflUtils
#include <aknedsts.h>
#include <spsettings.h>
#include <dialingextensioninterface.h>
#include <easydialingcommands.hrh>
#include <dialer.rsg>

#include "cdialer.h"
#include "dialercommon.h"
#include "dialer.hrh"
#include "cdialerkeypadcontainer.h"
#include "cdialernumberentry.h"
#include "dialertrace.h"
#include "mnumberentry.h"
#include "cdialingextensionobserver.h"
#include "cdialertoolbarcontainer.h"

/// ROM drive.
_LIT( KDialerResourceFile, "dialer.rsc" );

// number entry, keypad area, easydialing, toolbar
const TInt KContainedControlsInTelephonyMode = 4;  

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CDialer::NewL
//
// Symbian OS two phased constructor for telephony phone dialer mode
// ---------------------------------------------------------------------------
//
EXPORT_C CDialer* CDialer::NewL( const CCoeControl& aContainer, 
                                 const TRect& aRect,
                                 MPhoneDialerController* aController )
    {
    CDialer* self = new( ELeave )CDialer();
    CleanupStack::PushL( self );
    self->ConstructL( aContainer, aRect, aController );
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
EXPORT_C CDialer::~CDialer()
    {
    DIALER_PRINT("CDialer::~CDialer<"); 
    AknsUtils::DeregisterControlPosition( this );

    delete iKeypadArea;
    delete iNumberEntry;

    delete iEasyDialer;
    delete iDialingExtensionObserver;
    delete iToolbar;

    UnLoadResources();
    DIALER_PRINT("CDialer::~CDialer>"); 
    }

// ---------------------------------------------------------------------------
// CDialer::ConstructL
//
// Symbian OS two phased constructor for phone dialer more.
// ---------------------------------------------------------------------------
//
void CDialer::ConstructL( 
    const CCoeControl& aContainer, 
    const TRect& aRect,
    MPhoneDialerController* aController )
    {    
    DIALER_PRINT("CDialer::ConstructL<");	
    LoadResourceL();    
    
    // set window
    SetContainerWindowL( aContainer );
    SetParent( const_cast<CCoeControl*>(&aContainer) );

    iNumberEntry = CDialerNumberEntry::NewL( *this );    

    iController = aController;
    
    iKeypadArea = CDialerKeyPadContainer::NewL( *this, EModeEasyDialing );    
    
    iToolbar = CDialerToolbarContainer::NewL( *this, iController );

    // try to create easydialing plugin. If plugin is not present, iEasydialer gets value NULL.
    LoadEasyDialingPlugin();

    iNumberEntry->SetEasyDialingPlugin( iEasyDialer );
    iNumberEntry->SetObserver( this );
    iNumberEntry->SetNumberEntryPromptTextL( iController->NumberEntryPromptTextL() );

    UpdateToolbar();

    SetRect( aRect );
    
    SetComponentsToInheritVisibility( EFalse );

    ActivateL();
    DIALER_PRINT("CDialer::ConstructL>");
    }

// Constructor
CDialer::CDialer( ) :
    iEikEnv( *CEikonEnv::Static() )
    {
    // Empty    
    }

// -----------------------------------------------------------------------------
// CDialer::NumberEntry
//
// Return pointer to MNumberEntry interface.
// -----------------------------------------------------------------------------
//
EXPORT_C MNumberEntry* CDialer::NumberEntry()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// CDialer::SetNumberEntryObserver
//
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CDialer::SetNumberEntryObserver( MNumberEntryObserver& aObserver ) 
    {
    iNumberEntry->SetNumberEntryObserver( aObserver );  
    }

// -----------------------------------------------------------------------------
// CDialer::SetControllerL
// -----------------------------------------------------------------------------
//
EXPORT_C void CDialer::SetControllerL( MPhoneDialerController* aController )
    {
    DIALER_PRINTF( "CDialer::SetControllerL, aController = %x", aController );
    __ASSERT_DEBUG( aController, DialerPanic(EDialerPanicNullController) );
    if ( aController && iController != aController )
        {
        iController = aController;
        iToolbar->SetContentProviderL( iController );
        iNumberEntry->SetNumberEntryPromptTextL( iController->NumberEntryPromptTextL() );
        SizeChanged();
        UpdateToolbar();
        }
    }

// ---------------------------------------------------------------------------
// CDialer::Controller
// ---------------------------------------------------------------------------
//
EXPORT_C MPhoneDialerController* CDialer::Controller()
    {
    return iController;
    }
            
// ---------------------------------------------------------------------------
// CDialer::GetEasyDialingInterface
// ---------------------------------------------------------------------------
//
EXPORT_C CDialingExtensionInterface* CDialer::GetEasyDialingInterface() const
    {
    DIALER_PRINT("CDialer::GetEasyDialingInterface");
    return iEasyDialer;
    }

// ---------------------------------------------------------------------------
// CDialer::UpdateToolbar
// ---------------------------------------------------------------------------
//
EXPORT_C void CDialer::UpdateToolbar()
    {
    if ( iToolbar && iNumberEntry && iController )
        {
        TBool numAvailable = ( iNumberEntry->TextLength() > 0 );
        iController->SetNumberEntryIsEmpty( !numAvailable );
        iToolbar->UpdateButtonStates();
        iToolbar->DrawDeferred();
        iKeypadArea->DrawDeferred(); // needed to remove drawing problem from leftmost button column
        }
    }

// ---------------------------------------------------------------------------
// CDialer::UpdateNumberEntryConfiguration
// ---------------------------------------------------------------------------
//
void CDialer::UpdateNumberEntryConfiguration()
    {
    TEditorType editorType = ENumericEditor;
    if ( iQwertyMode && iController->EasyDialingAllowed() )
        {
        TBool voipSupported( EFalse );
        CSPSettings* serviceProviderSettings = NULL;
        TRAPD( err, serviceProviderSettings = CSPSettings::NewL() );
        if ( !err )
            {
            voipSupported = serviceProviderSettings->IsFeatureSupported( 
                ESupportInternetCallFeature );            
            delete serviceProviderSettings;
            }

        if ( EasyDialingEnabled() || voipSupported )
            {
            editorType = EAlphanumericEditor;
            }
        }
    
    UpdateEdwinState( editorType );
    }

// ---------------------------------------------------------------------------
// CDialer::RelayoutAndDraw
// ---------------------------------------------------------------------------
//
EXPORT_C void CDialer::RelayoutAndDraw()
	{
	if ( iIsUsed )
		{
		SetSize( Size() );
		DrawDeferred();
		UpdateToolbar();
		}
	}

// Methods from MNumberEntry

// ---------------------------------------------------------------------------
// CDialer::CreateNumberEntry
//  
// Creates number entry in phone dialer mode. 
// ---------------------------------------------------------------------------
//
void CDialer::CreateNumberEntry()
    {
    DIALER_PRINT("CDialer::CreateNumberEntry<");
    
    iIsUsed = ETrue;    

    UpdateNumberEntryConfiguration();
    
    DIALER_PRINT("CDialer::CreateNumberEntry>");        
    }
    
// ---------------------------------------------------------------------------
// CDialer::GetNumberEntry
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CDialer::GetNumberEntry() const
    {
    CCoeControl* control( NULL );
    control = iNumberEntry->GetNumberEntry();
    return control;
    }
    
// ---------------------------------------------------------------------------
// CDialer::IsNumberEntryUsed
//  
// ---------------------------------------------------------------------------
//
TBool CDialer::IsNumberEntryUsed() const
    {
    return iIsUsed;
    }
    
// ---------------------------------------------------------------------------
// CDialer::SetNumberEntryVisible
//  
// ---------------------------------------------------------------------------
//
void CDialer::SetNumberEntryVisible( const TBool& /*aVisibility*/ )
    {
    }
    
// ---------------------------------------------------------------------------
// CDialer::SetTextToNumberEntry
//  
// ---------------------------------------------------------------------------
//
void CDialer::SetTextToNumberEntry( const TDesC& aDesC )
    {
    iNumberEntry->SetTextToNumberEntry( aDesC );
    }
    
// ---------------------------------------------------------------------------
// CDialer::GetTextFromNumberEntry
//  
// ---------------------------------------------------------------------------
//
void CDialer::GetTextFromNumberEntry( TDes& aDes )
    {
    iNumberEntry->GetTextFromNumberEntry( aDes );
    }
    
// ---------------------------------------------------------------------------
// CDialer::RemoveNumberEntry
//  
// ---------------------------------------------------------------------------
//
void CDialer::RemoveNumberEntry()
    {
    ResetEditorToDefaultValues();
    iIsUsed = EFalse;

    // easydialer change begins
    if (iEasyDialer)
        {
        TRAP_IGNORE( iEasyDialer->HandleCommandL( EEasyDialingClosePopup ) );
        iEasyDialer->Reset();
        }
    // easydialer change ends
    }
    
// ---------------------------------------------------------------------------
// CDialer::ChangeEditorMode
//  
// ---------------------------------------------------------------------------
//
TInt CDialer::ChangeEditorMode( TBool aDefaultMode )
    {
    TInt ret( KErrNotSupported );
    ret = iNumberEntry->ChangeEditorMode( aDefaultMode );
    return ret;
    }
    
// ---------------------------------------------------------------------------
// CDialer::OpenVKBL
//  
// ---------------------------------------------------------------------------
//
void CDialer::OpenVkbL()
    {
    iVirtualKeyBoardOpen = ETrue;
    UpdateEdwinState( EVirtualKeyboardEditor );
    
    iNumberEntry->HandleCommandL( EDialerCmdTouchInput );
    }

// ---------------------------------------------------------------------------
// CDialer::GetEditorMode
//  
// ---------------------------------------------------------------------------
//
TInt CDialer::GetEditorMode() const
    {
    TBool vkbOpen = EdwinState()->Flags() & EAknEditorFlagTouchInputModeOpened;
    return ( iQwertyMode || vkbOpen ) ? EAknEditorTextInputMode : 
                                  EAknEditorNumericInputMode;
    }
    
// ---------------------------------------------------------------------------
// CDialer::ResetEditorToDefaultValues
//  
// ---------------------------------------------------------------------------
//
void CDialer::ResetEditorToDefaultValues()
    {
    iNumberEntry->ResetEditorToDefaultValues();
    }
    
// ---------------------------------------------------------------------------
// CDialer::SetNumberEntryPromptText
//  
// ---------------------------------------------------------------------------
//    
void CDialer::SetNumberEntryPromptText( const TDesC& aPromptText ) 
    {
    TRAP_IGNORE( iNumberEntry->SetNumberEntryPromptTextL( aPromptText ) );
    }

// ---------------------------------------------------------------------------
// CDialer::EnableTactileFeedback
//  
// ---------------------------------------------------------------------------
//    
void CDialer::EnableTactileFeedback( const TBool aEnable )
    {
    iKeypadArea->EnableTactileFeedback( aEnable );
    }

// ---------------------------------------------------------
// CDialer::HandleQwertyModeChange
// ---------------------------------------------------------
//
EXPORT_C void CDialer::HandleQwertyModeChange( TInt aMode )
    {
    iQwertyMode = aMode;
    UpdateNumberEntryConfiguration();
    }

// ---------------------------------------------------------
// CDialer::HandleKeyboardLayoutChange
// ---------------------------------------------------------
// 
EXPORT_C void CDialer::HandleKeyboardLayoutChange()
    {
    }

// ---------------------------------------------------------------------------
// CDialer::HandleControlEventL
// ---------------------------------------------------------------------------
//
void CDialer::HandleControlEventL( CCoeControl* aControl, TCoeEvent aEventType )
    {
    switch ( aEventType )
        {
        case MCoeControlObserver::EEventRequestFocus:
            if ( aControl == iNumberEntry )
                {
                if ( iEasyDialer )
                    {
                    iEasyDialer->SetFocus( EFalse );
                    }
                iNumberEntry->SetFocus( ETrue );
                }
            break;
            
        default:
            break;
        }
    }

// Functions from CCoeControl

// ---------------------------------------------------------------------------
// CDialer::SizeChanged
// 
// Called by framework when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CDialer::SizeChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    TRect parentRect( Rect() );
       
    // Method is called before containers are created.
    if ( !iKeypadArea )
        {
        return;
        }
        
    TDialerVariety variety = ( Layout_Meta_Data::IsLandscapeOrientation() ?
        EDialerVarietyLandscape : EDialerVarietyPortrait );
    
    // keypad area
    TDialerOperationMode keypadOpMode = 
            ( EasyDialingEnabled() ? EModeEasyDialing : EModeDialer );
    iKeypadArea->SetOperationMode( keypadOpMode );
    AknLayoutUtils::LayoutControl(
        iKeypadArea, parentRect, 
        AknLayoutScalable_Apps::dia3_keypad_num_pane( variety ).LayoutLine() );

    // toolbar
    AknLayoutUtils::LayoutControl(
        iToolbar, parentRect, 
        AknLayoutScalable_Apps::dia3_keypad_fun_pane( variety ).LayoutLine() );

    // easy dial contacts list
    if ( iEasyDialer )
        {
        AknLayoutUtils::LayoutControl(
            iEasyDialer, parentRect, 
            AknLayoutScalable_Apps::dia3_listscroll_pane( variety ).LayoutLine() );
        }

    // number entry
    LayoutNumberEntry( parentRect, variety );   
    }

// ---------------------------------------------------------------------------
// CDialer::PositionChanged
// ---------------------------------------------------------------------------
//
void CDialer::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CDialer::CountComponentControls
//
// Returns contained controls in phone dialer mode
// ---------------------------------------------------------------------------
//
TInt CDialer::CountComponentControls() const
    {
    TInt count( KContainedControlsInTelephonyMode );
    
    if ( !iEasyDialer )
        {
        count--;
        }
    return count;
    }
    
// ---------------------------------------------------------------------------
// CDialer::ComponentControl(TInt aIndex) const
//
// Returns contained control by given index.
// ---------------------------------------------------------------------------
//
CCoeControl* CDialer::ComponentControl( TInt aIndex ) const
    {
    CCoeControl* currentControl(NULL);
    currentControl = ComponentControlForDialerMode( aIndex );
    return currentControl;
    } 

// ---------------------------------------------------------------------------
// CDialer::Draw
// ---------------------------------------------------------------------------
//
void CDialer::Draw( const TRect& /*aRect*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CDialer::FocusChanged
// ---------------------------------------------------------------------------
//
void CDialer::FocusChanged(TDrawNow aDrawNow)
    {
    if ( iEasyDialer )
        {           
        // Number entry is set to focus if dialer is in focus and easydialing plugin
        // is not in focus.
        TBool numberEntryFocus = IsFocused() && !iEasyDialer->IsFocused();
        iNumberEntry->SetFocus( numberEntryFocus, aDrawNow );
        }
    else
        {
        iNumberEntry->SetFocus( IsFocused(), aDrawNow );    
        }
    }
    
// ---------------------------------------------------------------------------
// CDialer::MakeVisible
// ---------------------------------------------------------------------------
//
void CDialer::MakeVisible( TBool aVisible )
    {
    CCoeControl::MakeVisible( aVisible );

    // Component control do not inherit visibility automatically as we
    // want to control their visibility separately.
    if ( iNumberEntry )
        {
        iNumberEntry->MakeVisible( aVisible );
        }

    if ( iKeypadArea )
        {
        iKeypadArea->MakeVisible( aVisible );
        }

    if ( iEasyDialer )
        {
        TBool edVisible = 
            ( aVisible && EasyDialingEnabled() );
        iEasyDialer->MakeVisible( edVisible );
        }
    if ( iToolbar )
        {
        iToolbar->MakeVisible( aVisible );
        }
    }

// ---------------------------------------------------------------------------
// CDialer::PrepareForFocusGainL
//  
// ---------------------------------------------------------------------------
//
void CDialer::PrepareForFocusGainL( )
    {
    if ( iVirtualKeyBoardOpen )
        {
        // Clear editor flags and report
        // edwin state changed.
        iVirtualKeyBoardOpen = EFalse;
        UpdateNumberEntryConfiguration();
        }
    }

// ---------------------------------------------------------------------------
// CDialer::HandleResourceChange
//  
// ---------------------------------------------------------------------------
//
void CDialer::HandleResourceChange( TInt aType )
    {
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        SizeChanged();
        }
    CCoeControl::HandleResourceChange( aType );
    }

// -----------------------------------------------------------------------------
// CDialer::HandlePointerEventL
//
// -----------------------------------------------------------------------------
//
void CDialer::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    // Calling base class implementation
    CCoeControl::HandlePointerEventL(aPointerEvent);    
    }

// Private methods

// -----------------------------------------------------------------------------
// CDialer::LoadResourceL
//
// -----------------------------------------------------------------------------
//
void CDialer::LoadResourceL()
    {
    TFileName path( KDriveZ );
    path.Append( KDC_RESOURCE_FILES_DIR );
    path.Append( KDialerResourceFile );
    
    RFs &fsSession= iEikEnv.FsSession();
    BaflUtils::NearestLanguageFile( fsSession, path ); 
    iResourceOffset = iEikEnv.AddResourceFileL( path );
    }

// -----------------------------------------------------------------------------
// CDialer::UnLoadResources
//
// -----------------------------------------------------------------------------
//    
void CDialer::UnLoadResources()
    {
    if ( iResourceOffset > 0 )
        {
        iEikEnv.DeleteResourceFile( iResourceOffset );
        iResourceOffset = 0;
        }
    }
    
// ---------------------------------------------------------------------------
// CDialer::ComponentControlForDialerMode
// 
// Returns contained control by given index in ohonedialer mode.
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CDialer::ComponentControlForDialerMode( const TInt aIndex ) const
    {
    CCoeControl* currentControl(NULL);
    
    switch ( aIndex )
        {
        case 0:
            currentControl = iNumberEntry;
            break;
        case 1:
            currentControl = iKeypadArea;
            break;
            
        case 2:
            currentControl = iToolbar;
            break;
        case 3:
            currentControl = iEasyDialer;
            break;
            
        default:
            {
            __ASSERT_DEBUG( EFalse, _L("CDialer::ComponentControl no such component defined"));
            }
        }

    return currentControl;
    } 

// ---------------------------------------------------------------------------
// CDialer::EdwinState
// 
// Returns edwin state of the editor.
//  
// ---------------------------------------------------------------------------
//
CAknEdwinState* CDialer::EdwinState() const
    {
    MCoeFepAwareTextEditor_Extension1* extension = 
        static_cast<MCoeFepAwareTextEditor_Extension1*>
              ( static_cast<CAknPhoneNumberEditor*> (GetNumberEntry()) );

    return static_cast< CAknEdwinState* >( extension->State( KNullUid ) );
    } 

// ---------------------------------------------------------------------------
// CDialer::UpdateEdwinState
//  
// ---------------------------------------------------------------------------
//
void CDialer::UpdateEdwinState( TEditorType aType )
    {
    CAknEdwinState* edwinState = EdwinState();
    
    switch ( aType )
        {
        case ENumericEditor:
            {
            iNumberEntry->ClearEditorFlags();
            }
            break;
        
        case EAlphanumericEditor:
        case EVirtualKeyboardEditor:
            // intended fall-through
            {
            TBool vkbOpen = edwinState->Flags() & EAknEditorFlagTouchInputModeOpened;
            TInt flags = EAknEditorFlagNoT9 | 
                         EAknEditorFlagLatinInputModesOnly |
                         EAknEditorFlagSelectionVisible;
            edwinState->SetDefaultInputMode( EAknEditorTextInputMode );
            edwinState->SetCurrentInputMode( EAknEditorTextInputMode );
            
            if ( EVirtualKeyboardEditor == aType || vkbOpen )
                {
                // Indicators would be shown after closing VKB unless disabled
                // here.
                flags = ( flags |= EAknEditorFlagNoEditIndicators );
                }
            else
                {
                iVirtualKeyBoardOpen = EFalse;
                }
            
            edwinState->SetFlags( flags );
            edwinState->SetPermittedInputModes( 
                EAknEditorNumericInputMode | EAknEditorTextInputMode );
            edwinState->SetPermittedCases( 
                EAknEditorUpperCase | EAknEditorLowerCase );
            edwinState->SetDefaultCase( EAknEditorLowerCase );
            edwinState->SetCurrentCase( EAknEditorLowerCase );
            edwinState->SetSpecialCharacterTableResourceId( 
                R_AVKON_URL_SPECIAL_CHARACTER_TABLE_DIALOG );
            edwinState->SetNumericKeymap( 
                EAknEditorAlphanumericNumberModeKeymap );
            edwinState->SetMenu();
            }
            break;
        
        default:
            DIALER_PRINT( "CDialer::ConfigureEditorSettings, DEFAULT" )
            ASSERT( EFalse );
        }
    
    TRAPD( result, edwinState->ReportAknEdStateEventL(
        MAknEdStateObserver::EAknEdwinStateEventStateUpdate ) );
    
    if ( KErrNone != result )
        {
        DIALER_PRINTF( "CDialer::ConfigureEditorSettings, RESULT: %d", result )
        }
    }

// ---------------------------------------------------------------------------
// CDialer::LoadEasyDialingPlugin
// ---------------------------------------------------------------------------
//
void CDialer::LoadEasyDialingPlugin()
    {
    DIALER_PRINT( "CDialer::LoadEasyDialingPlugin" )
    TRAPD( error, 
        {
        iEasyDialer = CDialingExtensionInterface::NewL();
        iEasyDialer->InitializeL( *this );
        
        iDialingExtensionObserver = CDialingExtensionObserver::NewL( iEasyDialer, iNumberEntry, this );
        iEasyDialer->AddObserverL( iDialingExtensionObserver );
        } );

    if ( error )
        {
        DIALER_PRINT( "CDialer::LoadEasyDialingPlugin, load failed" )
                
        delete iEasyDialer;        
        iEasyDialer = NULL;
        
        delete iDialingExtensionObserver;
        iDialingExtensionObserver = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CDialer::EasyDialingEnabled
// ---------------------------------------------------------------------------
//
TBool CDialer::EasyDialingEnabled() const
    {
    TBool easyDialEnabled = ( iEasyDialer &&
                              iEasyDialer->IsEnabled() &&
                              iController &&
                              iController->EasyDialingAllowed() );
    return easyDialEnabled;
    }

// ---------------------------------------------------------------------------
// CDialer::LayoutNumberEntry
// ---------------------------------------------------------------------------
//
void CDialer::LayoutNumberEntry( const TRect& aParent, TInt aVariety )
    {
    // Use larger number entry if Easy dialing is not currently enabled.
    if ( EasyDialingEnabled() )
        {
        iNumberEntry->SetOperationMode( EModeEasyDialing );
        AknLayoutUtils::LayoutControl(
            iNumberEntry, aParent, 
            AknLayoutScalable_Apps::dia3_numentry_pane( aVariety ).LayoutLine() );
        }
    else
        {
        iNumberEntry->SetOperationMode( EModeDialer );
        TAknLayoutRect neLayoutRect;
        neLayoutRect.LayoutRect( aParent, AknLayoutScalable_Apps::dia3_numentry_pane( aVariety ) );
        TAknLayoutRect edLayoutRect;
        edLayoutRect.LayoutRect( aParent, AknLayoutScalable_Apps::dia3_listscroll_pane( aVariety ) );

        // create rect which is union of layout rects for ED and NE
        TRect neRect( edLayoutRect.Rect().iTl, neLayoutRect.Rect().iBr );

        iNumberEntry->SetRect( neRect );
        }
    }

// End of File
