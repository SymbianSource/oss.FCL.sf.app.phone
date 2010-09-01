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
#include <aknsoundsystem.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>
#include <data_caging_path_literals.hrh>    // for KDC_APP_RESOURCE_DIR
#include <bautils.h>                        // for BaflUtils
#include <aknedsts.h>
#include <spsettings.h>
#include <dialingextensioninterface.h>
#include <easydialingcommands.hrh>
#include <dialer.rsg>
#include <phoneui.rsg>
#include <featmgr.h>

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

// -----------------------------------------------------------------------------
// CDialer::~CDialer
//
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CDialer::~CDialer()
    {
    DIALER_PRINT("CDialer::~CDialer<"); 
    #ifdef RD_SCALABLE_UI_V2
    iPeninputServer.RemovePenUiActivationHandler( this );
    iPeninputServer.Close();
    #endif
    AknsUtils::DeregisterControlPosition( this );

    delete iKeypadArea;
    delete iNumberEntry;

    delete iEasyDialer;
    delete iDialingExtensionObserver;
    delete iToolbar;
    
    iComponentControls.Close();

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
    iComponentControls.Append( iNumberEntry );

    iController = aController;
    
    // Keypad and toolbar are not used in small displays.
    if ( ! FeatureManager::FeatureSupported(  KFeatureIdFfSmallScreenTouch ) ) 
        {
        iKeypadArea = CDialerKeyPadContainer::NewL( *this, EModeEasyDialing );
        iComponentControls.Append( iKeypadArea );
        
        iToolbar = CDialerToolbarContainer::NewL( *this, iController );
        iComponentControls.Append( iToolbar );
        }

    // try to create easydialing plugin. If plugin is not present, iEasydialer gets value NULL.
    LoadEasyDialingPlugin();

    iNumberEntry->SetEasyDialingPlugin( iEasyDialer );
    iNumberEntry->SetObserver( this );
    iNumberEntry->SetNumberEntryPromptTextL( iController->NumberEntryPromptTextL() );

    UpdateToolbar();

    SetRect( aRect );
    
    SetComponentsToInheritVisibility( EFalse );
    
    // By default, numeric keysounds are disabled.
    DisableNumericKeySounds( ETrue );

    ActivateL();
    #ifdef RD_SCALABLE_UI_V2
    if ( AknLayoutUtils::PenEnabled() )
        {
        User::LeaveIfError( iPeninputServer.Connect() );
        iPeninputServer.AddPenUiActivationHandler( this, EPluginInputModeAll ); 
        iVirtualKeyBoardOpen = iPeninputServer.IsVisible();
        }
    #endif
    DIALER_PRINT("CDialer::ConstructL>");
    }

// -----------------------------------------------------------------------------
// CDialer::CDialer
//
// Default constructor
// -----------------------------------------------------------------------------
//
CDialer::CDialer()
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
        if ( iToolbar )
            {
            iToolbar->SetContentProviderL( iController );
            }
        iNumberEntry->SetNumberEntryPromptTextL( iController->NumberEntryPromptTextL() );
        SizeChanged();
        UpdateToolbar();
        UpdateNumberEntryConfiguration();
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
        
        if ( iKeypadArea )
            {
            iKeypadArea->DrawDeferred(); // needed to remove drawing problem from leftmost button column
            }
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
    
    // Numeric keysound are disabled in if not in alpha mode. 
    // In numeric mode numeric keys should play DTMF only.
    DisableNumericKeySounds( editorType != EAlphanumericEditor );
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
    
    if ( !iIsUsed )
        {
        iIsUsed = ETrue;
        UpdateNumberEntryConfiguration();
        }
    
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
        TRAP_IGNORE( iEasyDialer->HandleCommandL( EEasyDialingClosePopup ); );
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
    return ( iEditorType == ENumericEditor ) ? 
        EAknEditorNumericInputMode : EAknEditorTextInputMode;
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
    if ( iKeypadArea )
        {
        iKeypadArea->EnableTactileFeedback( aEnable );
        }
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
       
    TDialerVariety variety = ( Layout_Meta_Data::IsLandscapeOrientation() ?
        EDialerVarietyLandscape : EDialerVarietyPortrait );
    
    
    // keypad area
    if ( iKeypadArea ) 
        {
        TDialerOperationMode keypadOpMode = 
                ( EasyDialingEnabled() ? EModeEasyDialing : EModeDialer );
        
        iKeypadArea->SetOperationMode( keypadOpMode );
        AknLayoutUtils::LayoutControl(
            iKeypadArea, parentRect, 
            AknLayoutScalable_Apps::dia3_keypad_num_pane( variety ).LayoutLine() );
        }

    
    // toolbar
    if ( iToolbar )
        {
        AknLayoutUtils::LayoutControl(
            iToolbar, parentRect, 
            AknLayoutScalable_Apps::dia3_keypad_fun_pane( variety ).LayoutLine() );
        }

    // Use hybrid keyboard mode if there is no visible virtual keypad.
    iHybridKeyboardMode = !iKeypadArea;
    
    // easy dial contacts list
    if ( iEasyDialer )
        {
        AknLayoutUtils::LayoutControl(
            iEasyDialer, parentRect, 
            AknLayoutScalable_Apps::dia3_listscroll_pane( variety ).LayoutLine() );
        
        iEasyDialer->SetKeyboardMode( 
            iHybridKeyboardMode ?
            CDialingExtensionInterface::EHybridQwerty :
            CDialingExtensionInterface::EDefaultKeyboard );
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
    return iComponentControls.Count();
    }

// ---------------------------------------------------------------------------
// CDialer::ComponentControl(TInt aIndex) const
//
// Returns contained control by given index.
// ---------------------------------------------------------------------------
//
CCoeControl* CDialer::ComponentControl( TInt aIndex ) const
    {
    return iComponentControls[ aIndex ];
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
void CDialer::FocusChanged( TDrawNow aDrawNow )
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
    
    RFs& fsSession= ControlEnv()->FsSession();
    BaflUtils::NearestLanguageFile( fsSession, path ); 
    iResourceOffset = ControlEnv()->AddResourceFileL( path );
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
        ControlEnv()->DeleteResourceFile( iResourceOffset );
        iResourceOffset = 0;
        }
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
    iEditorType = aType;
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
            
            // Use numeric input mode if in hybrid mode and virtual keyboard
            // is not open. This signals to AvKON that hybrid mode needs to
            // be used.
            TInt inputMode = ( iHybridKeyboardMode && !vkbOpen ) ?
                EAknEditorNumericInputMode : EAknEditorTextInputMode;
            edwinState->SetDefaultInputMode( inputMode );
            edwinState->SetCurrentInputMode( inputMode );
            
            if ( EVirtualKeyboardEditor == aType || vkbOpen )
                {
                // Indicators would be shown after closing VKB unless disabled
                // here.
                flags = ( flags |= EAknEditorFlagNoEditIndicators );
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
// CDialer::SetNumericKeySounds
// ---------------------------------------------------------------------------
//
void CDialer::DisableNumericKeySounds( TBool aDisable )
    {
    CAknAppUi *appUi = static_cast<CAknAppUi*>( ControlEnv()->AppUi() );
    CAknKeySoundSystem* keySounds = appUi->KeySounds();
    
    if ( aDisable && !iNumericKeySoundsDisabled )
        {
        // Disable numeric key sounds. This is done only if key sounds were not disabled previously,
        // to avoid situation that there were multiple key sound contexts in the stack. 
        TRAP_IGNORE( keySounds->PushContextL( R_PHONEUI_DEFAULT_SKEY_LIST ) );
        }
    else if ( !aDisable && iNumericKeySoundsDisabled )
        {
        // Remove numeric key sound disabling.
        keySounds->PopContext();
        }

    iNumericKeySoundsDisabled = aDisable;
    }

// ---------------------------------------------------------------------------
// CDialer::LoadEasyDialingPlugin
// ---------------------------------------------------------------------------
//
void CDialer::LoadEasyDialingPlugin()
    {
    DIALER_PRINT( "CDialer::LoadEasyDialingPlugin" )
            
    if ( FeatureManager::FeatureSupported( KFeatureIdProductIncludesHomeScreenEasyDialing ) )
        {
        TRAPD( error, 
            {
            iEasyDialer = CDialingExtensionInterface::NewL();
            iEasyDialer->InitializeL( *this );
            
            iDialingExtensionObserver = CDialingExtensionObserver::NewL( iEasyDialer, iNumberEntry, this );
            iEasyDialer->AddObserverL( iDialingExtensionObserver );
            
            iComponentControls.Append( iEasyDialer );
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
    TAknLayoutRect neLayoutRect;
    neLayoutRect.LayoutRect( aParent, AknLayoutScalable_Apps::dia3_numentry_pane( aVariety ) );
    TRect neRect = neLayoutRect.Rect();

    if ( EasyDialingEnabled() )
        {
        iNumberEntry->SetOperationMode( EModeEasyDialing );
        
        // Layout doesn't define any margin between number entry and contact list.
        // Add a small margin here.
        neRect.iTl.iY += 2;
        }
    else
        {
        iNumberEntry->SetOperationMode( EModeDialer );
        
        // Use rect which is an union of layout rects for ED and NE
        TAknLayoutRect edLayoutRect;
        edLayoutRect.LayoutRect( aParent, AknLayoutScalable_Apps::dia3_listscroll_pane( aVariety ) );
        neRect.iTl.iY = edLayoutRect.Rect().iTl.iY;
        }

    iNumberEntry->SetRect( neRect );
    }

// ---------------------------------------------------------------------------
// CDialer::OnPeninputUiDeactivated
//
// Gets called when the virtual keyboard editor is closed.
// ---------------------------------------------------------------------------
//
void CDialer::OnPeninputUiDeactivated()
    {
    DIALER_PRINT( "CDialer::OnPeninputUiDeactivated" )
    iVirtualKeyBoardOpen = EFalse;
    FocusChanged( EDrawNow );
    if ( iEasyDialer )
        {
        TRAP_IGNORE( iEasyDialer->HandleCommandL( EEasyDialingVkbClosed ) );
        }
    }

// ---------------------------------------------------------------------------
// CDialer::OnPeninputUiActivated
//
// Gets called when the virtual keyboard editor is opened.
// ---------------------------------------------------------------------------
//
void CDialer::OnPeninputUiActivated()
    {
    DIALER_PRINT( "CDialer::OnPeninputUiActivated" )
    iVirtualKeyBoardOpen = ETrue;
    if ( iEasyDialer )
        {
        TRAP_IGNORE( iEasyDialer->HandleCommandL( EEasyDialingVkbOpened ) );
        }   
    }
// End of File
