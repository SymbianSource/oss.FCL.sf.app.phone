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
* Description:  Implementation of CVideoDTMFDialer class.
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

#include "cvideodtmfdialer.h"
#include "dialercommon.h"
#include <dialer.rsg>
#include "dialer.hrh"
#include "cdialerkeypadcontainer.h"
#include "cdialervideocontainer.h"
#include "dialertrace.h"


_LIT( KDialerResourceFile, "dialer.rsc" );

// Video and keypad areas
const TInt KContainedControlsInVideoDTMFMode = 2; 

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::NewL
//
// Symbian OS two phased constructor for video dtmf mode
// ---------------------------------------------------------------------------
//
EXPORT_C CVideoDTMFDialer* CVideoDTMFDialer::NewL( const CCoeControl& aContainer, 
                                 CCoeControl& aVideoWindow, 
                                 const TRect& aRect )
    {
    CVideoDTMFDialer* self = new( ELeave )CVideoDTMFDialer ( );    
    CleanupStack::PushL( self );
    self->ConstructL( aContainer , aVideoWindow, aRect );
    CleanupStack::Pop();    // self
    return self;
    }

// Destructor
EXPORT_C CVideoDTMFDialer::~CVideoDTMFDialer()
    {
    DIALER_PRINT("CVideoDTMFDialer::~CVideoDTMFDialer<"); 
    AknsUtils::DeregisterControlPosition( this );

    delete iKeypadArea;
    delete iVideoWindow;
    
    UnLoadResources();

    DIALER_PRINT("CVideoDTMFDialer::~CVideoDTMFDialer>"); 
    }
    
// ---------------------------------------------------------------------------
// CVideoDTMFDialer::ConstructL
//
// Symbian OS two phased constructor for video telephony mode.
//
// ---------------------------------------------------------------------------
//
void CVideoDTMFDialer::ConstructL( 
    const CCoeControl& aContainer, 
    CCoeControl& aVideoWindow,
    const TRect& aRect )
    {    
    DIALER_PRINT("CVideoDTMFDialer::ConstructL<");
    LoadResourceL();    
    // set window
    SetContainerWindowL( aContainer );
    SetParent( const_cast<CCoeControl*>(&aContainer) );
    iParentControl = &aContainer;

    iVideoWindow = CDialerVideoContainer::NewL( *this, aVideoWindow );
    iKeypadArea = CDialerKeyPadContainer::NewL( *this, EModeVideoDTMF );
    
    SetRect( aRect );
    DIALER_PRINT("CVideoDTMFDialer::ConstructL>");        
    }

// Constructor
CVideoDTMFDialer::CVideoDTMFDialer( ) :
    iEikEnv( *CEikonEnv::Static() ) // codescanner::performance::eikonenvstatic
    {
    // Empty    
    }
    
// Functions from CCoeControl

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::SizeChanged
// 
// Called by framework when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CVideoDTMFDialer::SizeChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    TRect parentRect(Rect());
    
    // Method is called before containers are created.
    if ( !iKeypadArea )
        {
        return;
        }

    TVideoVariety variety( EVideoVarietyLandscape );
    if (  !Layout_Meta_Data::IsLandscapeOrientation() )
        {
        variety = EVideoVarietyPortrait;
        }

    // Layout video window.
    AknLayoutUtils::LayoutControl(
        iVideoWindow, parentRect, 
        AknLayoutScalable_Apps::video_down_pane_cp( variety ).LayoutLine() );

    // layout keypad area.
    AknLayoutUtils::LayoutControl(
        iKeypadArea, parentRect, 
        AknLayoutScalable_Apps::grid_video_dialer_keypad_pane( variety ).LayoutLine() );
    }

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::PositionChanged
//
// ---------------------------------------------------------------------------
//
void CVideoDTMFDialer::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::CountComponentControls
//
// Returns contained controls in phone dialer mode or in video dtmf mode.
//
// ---------------------------------------------------------------------------
//
TInt CVideoDTMFDialer::CountComponentControls() const
    {
    TInt count( 0 );
    count = KContainedControlsInVideoDTMFMode;
    return count;
    }

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::ComponentControl
//
// Returns contained control by given index.
//
// ---------------------------------------------------------------------------
//
CCoeControl* CVideoDTMFDialer::ComponentControl( TInt aIndex ) const
    {
    CCoeControl* currentControl( NULL );

    currentControl = ComponentControlForVideoMode( aIndex);

    return currentControl;
    } 

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::Draw
//
// ---------------------------------------------------------------------------
//
void CVideoDTMFDialer::Draw( const TRect& /*aRect*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::HandleResourceChange
//  
// ---------------------------------------------------------------------------
//
void CVideoDTMFDialer::HandleResourceChange( TInt aType )
    {
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        SizeChanged();
        }
    CCoeControl::HandleResourceChange( aType );
    }

// -----------------------------------------------------------------------------
// CVideoDTMFDialer::HandlePointerEventL
//
// -----------------------------------------------------------------------------
//
void CVideoDTMFDialer::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    // Calling base class implementation
    CCoeControl::HandlePointerEventL(aPointerEvent);    
    }

// Private methods

// -----------------------------------------------------------------------------
// CVideoDTMFDialer::LoadResourceL
//
// -----------------------------------------------------------------------------
//
void CVideoDTMFDialer::LoadResourceL()
    {
    TFileName path( KDriveZ );
    path.Append( KDC_RESOURCE_FILES_DIR );
    path.Append( KDialerResourceFile );
    
    RFs &fsSession= iEikEnv.FsSession();
    BaflUtils::NearestLanguageFile( fsSession, path ); 
    iResourceOffset = iEikEnv.AddResourceFileL( path );
    }

// -----------------------------------------------------------------------------
// CVideoDTMFDialer::UnLoadResources
//
// -----------------------------------------------------------------------------
//    
void CVideoDTMFDialer::UnLoadResources()
    {
    if ( iResourceOffset > 0 )
        {
        iEikEnv.DeleteResourceFile( iResourceOffset );
        iResourceOffset = 0;
        }
    }

// ---------------------------------------------------------------------------
// CVideoDTMFDialer::ComponentControlForVideoMode
// 
// Returns contained control by given index in video DTMF mode.
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CVideoDTMFDialer::ComponentControlForVideoMode( const TInt aIndex ) const
    {
    CCoeControl* currentControl(NULL);
    
    switch ( aIndex )
        {
        case 0:
            {
            currentControl = iKeypadArea;
            break;
            }
        case 1:
            {
            currentControl = iVideoWindow;
            break;
            }
        default:
            {
            __ASSERT_DEBUG( EFalse, 
            _L("CVideoDTMFDialer::ComponentControl no such component defined"));
            }
        }

    return currentControl;
    } 

// End of File
