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

#include "cdialer.h"
#include "dialercommon.h"
#include <dialer.rsg>
#include "dialer.hrh"
#include "cdialerkeypadcontainer.h"
#include "cdialernumberentry.h"
#include "dialertrace.h"
#include "mnumberentry.h"

/// ROM drive.
_LIT( KDialerResourceFile, "dialer.rsc" );
// number entry, keypad area
const TInt KContainedControlsInTelephonyMode = 2;  

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CDialer::NewL
//
// Symbian OS two phased constructor for telephony phone dialer mode
// ---------------------------------------------------------------------------
//
EXPORT_C CDialer* CDialer::NewL( const CCoeControl& aContainer, 
                                 const TRect& aRect )
    {
    CDialer* self = new( ELeave )CDialer ( );    
    CleanupStack::PushL( self );
    self->ConstructL( aContainer , aRect );
    CleanupStack::Pop();    // self
    return self;
    }

// Destructor
EXPORT_C CDialer::~CDialer()
    {
    DIALER_PRINT("CDialer::~CDialer<"); 
    AknsUtils::DeregisterControlPosition( this );

    delete iKeypadArea;
    delete iNumberEntry;

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
    const TRect& aRect )
    {    
    DIALER_PRINT("CDialer::ConstructL<");	
    LoadResourceL();    
    
    // set window
    SetContainerWindowL( aContainer );
    SetParent( const_cast<CCoeControl*>(&aContainer) );
    iParentControl = &aContainer;

    iNumberEntry = CDialerNumberEntry::NewL( *this );    

    iKeypadArea = CDialerKeyPadContainer::NewL( *this,  EModeDialer );    
    
    SetRect( aRect );
    
    SetComponentsToInheritVisibility( ETrue );
    
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
    
    DIALER_PRINT("CDialer::CreateNumberEntry>");        
    }
    
// ---------------------------------------------------------------------------
// CDialer::GetNumberEntry
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CDialer::GetNumberEntry( ) const
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
TBool CDialer::IsNumberEntryUsed( ) const
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
void CDialer::GetTextFromNumberEntry( TDes& aDesC )
    {
    iNumberEntry->GetTextFromNumberEntry( aDesC );
    }
    
// ---------------------------------------------------------------------------
// CDialer::RemoveNumberEntry
//  
// ---------------------------------------------------------------------------
//
void CDialer::RemoveNumberEntry( )
    {
    ResetEditorToDefaultValues();
    iIsUsed = EFalse;
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
    UpdateVkbEditorFlagsL();
    
    iNumberEntry->HandleCommandL( EDialerCmdTouchInput );
    }

// ---------------------------------------------------------------------------
// CDialer::GetEditorMode
//  
// ---------------------------------------------------------------------------
//
TInt CDialer::GetEditorMode() const
    {
    return iVirtualKeyBoardOpen ? EAknEditorTextInputMode : 
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
    TRect parentRect(Rect()); 
       
    // Method is called before containers are created.
    if ( !iKeypadArea )
        {
        return;
        }
        
    TDialerVariety variety( EDialerVarietyLandscape );
    if (  !Layout_Meta_Data::IsLandscapeOrientation() )
        {
        variety = EDialerVarietyPortrait;
        }            
    // number entry
    AknLayoutUtils::LayoutControl(
        iNumberEntry, parentRect, 
        AknLayoutScalable_Apps::dialer2_ne_pane( variety ).LayoutLine() );
    // keypad area.
    AknLayoutUtils::LayoutControl(
        iKeypadArea, parentRect, 
        AknLayoutScalable_Apps::grid_dialer2_keypad_pane( variety ).LayoutLine() );
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
    TInt count(0);
    count = KContainedControlsInTelephonyMode;
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
    currentControl = ComponentControlForDialerMode( aIndex);
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
    iNumberEntry->SetFocus( IsFocused(), aDrawNow );    
    }
    
// ---------------------------------------------------------------------------
// CDialer::MakeVisible
// ---------------------------------------------------------------------------
//
void CDialer::MakeVisible( TBool aVisible )
	{
	CCoeControl::MakeVisible( aVisible );
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
        iNumberEntry->ClearEditorFlags();
        EdwinState()->ReportAknEdStateEventL(
                MAknEdStateObserver::EAknEdwinStateEventStateUpdate );
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
CAknEdwinState* CDialer::EdwinState()
    {
    MCoeFepAwareTextEditor_Extension1* extension = 
        static_cast<MCoeFepAwareTextEditor_Extension1*>
              ( static_cast<CAknPhoneNumberEditor*> (GetNumberEntry()) );

    return static_cast< CAknEdwinState* >( extension->State( KNullUid ) );
    } 

// ---------------------------------------------------------------------------
// CDialer::UpdateVkbEditorFlagsL
//  
// ---------------------------------------------------------------------------
//
void CDialer::UpdateVkbEditorFlagsL()
    {
    CAknEdwinState* edwinState = EdwinState();
    // Set flags, input mode, SCT, permitted modes,
    // keymapping and menu for alphanumeric virtual
    // keyboard.
    edwinState->SetCurrentInputMode( EAknEditorTextInputMode );
    edwinState->SetSpecialCharacterTableResourceId( 
        R_AVKON_URL_SPECIAL_CHARACTER_TABLE_DIALOG );    
    edwinState->SetFlags( EAknEditorFlagNoT9 |
                          EAknEditorFlagLatinInputModesOnly | 
                          EAknEditorFlagNoEditIndicators );            
    edwinState->SetPermittedInputModes( 
                          EAknEditorNumericInputMode |
                          EAknEditorTextInputMode );    
    edwinState->SetNumericKeymap( EAknEditorAlphanumericNumberModeKeymap );    
    edwinState->SetMenu();
    
    // Report state updated
    edwinState->ReportAknEdStateEventL(
                    MAknEdStateObserver::EAknEdwinStateEventStateUpdate );
    }

// End of File
