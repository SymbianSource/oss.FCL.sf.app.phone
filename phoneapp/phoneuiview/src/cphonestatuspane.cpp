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
* Description: Implementation of CPhoneTextTitlePane class.
*
*/


// INCLUDE FILES
#include <aknappui.h>
#include <eikspane.h>
#include <akntitle.h>
#include <aknnavi.h>
#include <aknnavide.h>
#include <aknnavilabel.h>
#include <coemain.h>
#include <avkon.rsg>
#include <avkon.hrh>
#include <AknIndicatorContainer.h>
#include <barsread.h>
#include <phoneui.rsg>
#include <featmgr.h>
#include <AknIconUtils.h> 
#include <AknUtils.h> 
#include <aknlayoutscalable_avkon.cdl.h> 

#include "phoneui.pan"
#include "cphonestatuspane.h"
#include "mphonestatuspaneobserver.h"
#include "cphonerecoverysystem.h"

#include "tphonecommandparam.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparambitmap.h"
#include "phonelogger.h"

#include <StringLoader.h>

// CONSTANTS

// MODULE DATA STRUCTURES

/**
*  It is title pane containing text.
*/
class CPhoneTextTitlePane : 
    public CBase, 
    private MPhoneStatusPaneObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CPhoneTextTitlePane* NewLC( 
            CPhoneStatusPane& aStatusPaneManager,
            TInt aTextResourceId );
        
        /**
        * Destructor.
        */
        ~CPhoneTextTitlePane();

    public: // Functions from base classes

        /**
        * From MPhoneStatusPaneObserver, handles activation of title pane.
        */
        void HandleTitlePaneActiveL( TBool aActive );
        
    private:
        
        /**
        * C++ constructor.
        */
        CPhoneTextTitlePane(
            CPhoneStatusPane& aStatusPaneManager,
            TInt aTextResourceId );
      
        /**
        * Symbian OS constructor.
        */
        void ConstructL();
        
    private:  // Data

        // Status pane manager.
        CPhoneStatusPane& iStatusPaneManager;
        
        // Text resource id.
        const TInt iTextResourceId;

    };

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPhoneStatusPane::Instance
// Initializes the singleton object
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPhoneStatusPane* CPhoneStatusPane::Instance()
    {
    CPhoneStatusPane* instance = static_cast<CPhoneStatusPane*> 
        ( CCoeEnv::Static ( KUidPhoneStatusPaneHandlerSingleton ) );
    
    if( !instance )
        {
        TRAPD( err, instance = CPhoneStatusPane::NewL() );
        if ( err )
	        {
	        Panic( EPhoneUtilsCouldNotCreateSingleton );	
	        }
        }
    return instance;
    }

// ---------------------------------------------------------
// CPhoneStatusPane::CPhoneStatusPane
// ---------------------------------------------------------
//
// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneStatusPane::CPhoneStatusPane( ) :
    CCoeStatic( KUidPhoneStatusPaneHandlerSingleton, EThread ),
    iTitlePaneList( NULL ),
    iStatusPane( NULL ),
    iTitlePane( NULL ),
    iTitlePanePicture( NULL ),
    iTitlePanePictureMask( NULL ),
    iNaviPane( NULL ),
    iNaviDecorator( NULL )
    {
    }

// Symbian OS default constructor can leave.
void CPhoneStatusPane::ConstructL()
    {
    iTitlePaneList = new ( ELeave ) CArrayFixFlat< THandlerTag > 
        ( KPhoneTitlePaneListGranularity );

    // Get pointer for title pane
    iStatusPane = static_cast<CAknAppUi*>( CEikonEnv::Static()->
        EikAppUi() )->StatusPane();
    iTitlePane = static_cast<CAknTitlePane*>( iStatusPane->ControlL( 
        TUid::Uid( EEikStatusPaneUidTitle ) ) );
    iTitlePane->SetNumberOfVisibleTextRows( 1 );
    
    // Initialize display of title pane information
    iTitlePaneTextIsDisplayed = ETrue;

    // Get pointer for navigation pane
    iNaviPane = static_cast<CAknNavigationControlContainer*>
        ( iStatusPane->ControlL(TUid::Uid(EEikStatusPaneUidNavi)) );

    if ( !FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) )
        {
        // Construct navi pane decorator
        iNaviDecorator = iNaviPane->CreateNavigationLabelL( KNullDesC() );
        iNaviPane->PushL( *iNaviDecorator );
        }
    
    // Get recovery Id from recovery system
    iUpdateTitlePaneRecoveryId = CPhoneRecoverySystem::Instance()->AddL( 
        TCallBack( UpdateTitlePaneCallBackL, this ), 
        CTeleRecoverySystem::EPhonePriorityStandard,
        CTeleRecoverySystem::EPhoneStateIdle );
    }

// Constructor
CPhoneStatusPane* CPhoneStatusPane::NewL()
    {
    CPhoneStatusPane* self = new (ELeave) CPhoneStatusPane();
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// Destructor
CPhoneStatusPane::~CPhoneStatusPane()
    {
    if ( iTitlePaneList )
        {
        iTitlePaneList->Reset();
        }
    delete iTitlePaneList;
    
    iStatusPane = NULL;
    iTitlePane = NULL;
    
    if( iTitlePanePicture )
        {
        delete iTitlePanePicture;
        iTitlePanePicture = NULL;
        }
    if( iTitlePanePictureMask )
        {
        delete iTitlePanePictureMask;
        iTitlePanePictureMask = NULL;
        }
    
    iNaviPane = NULL;
    
    if ( iNaviDecorator )
          {
          delete iNaviDecorator;
          iNaviDecorator = NULL;
          }
     
    CPhoneRecoverySystem::Remove( iUpdateTitlePaneRecoveryId );
    }

// ---------------------------------------------------------
// CPhoneStatusPane::UpdateTitlePane
// ---------------------------------------------------------
//
void CPhoneStatusPane::UpdateTitlePane()
    {
    CPhoneRecoverySystem::Instance()->RecoverNow( 
        iUpdateTitlePaneRecoveryId,
        CTeleRecoverySystem::EPhonePriorityStandard );
    }

// ---------------------------------------------------------
// CPhoneStatusPane::UpdateTitlePaneCallBackL
// ---------------------------------------------------------
//
TInt CPhoneStatusPane::UpdateTitlePaneCallBackL( TAny* aAny )
    {
    reinterpret_cast<CPhoneStatusPane*>( aAny )->DoUpdateTitlePaneL();
    return KErrNone;
    }

// ---------------------------------------------------------
// CPhoneStatusPane::DoUpdateTitlePaneL
// ---------------------------------------------------------
//
void CPhoneStatusPane::DoUpdateTitlePaneL()
    {
    if ( iTitlePaneTextIsDisplayed )
        {
        iTitlePane->SetTextL( iTitleText );
        }
    else
        {
        // Title pane takes ownership of the picture
        RenderBitmapOnTitlePaneL();
        }
   }


// ---------------------------------------------------------
// CPhoneStatusPane::RenderBitmapOnTitlePaneL
// ---------------------------------------------------------
//
void CPhoneStatusPane::RenderBitmapOnTitlePaneL()
    {
    if( !iTitlePanePicture )
        {
        return;
        }

    TSize size;

    TRect titlePaneRect;
    AknLayoutUtils::LayoutMetricsRect(
        AknLayoutUtils::ETitlePane, titlePaneRect );

    TAknLayoutRect oplogoLayout;

    // stacon
    oplogoLayout.LayoutRect( titlePaneRect,
        AknLayoutScalable_Avkon::title_pane_stacon_g2(0).LayoutLine() );

    size = oplogoLayout.Rect().Size();

    // CAknIcon takes ownership of bitmaps.
    CFbsBitmap* dupMain = new ( ELeave ) CFbsBitmap;
    CleanupStack::PushL( dupMain );
    User::LeaveIfError(
        dupMain->Duplicate( iTitlePanePicture->Handle() ) );

    CFbsBitmap* dupMask = NULL;
    if ( iTitlePanePictureMask )
        {
        dupMask = new ( ELeave ) CFbsBitmap;
        CleanupStack::PushL( dupMask );
        User::LeaveIfError(
            dupMask->Duplicate( iTitlePanePictureMask->Handle() ) );
        }

    //bitmapIcon is not pushed to cleanupstack, ownership
    //is transferred later
    CAknIcon* bitmapIcon = CAknIcon::NewL();
    bitmapIcon->SetMask( dupMask ); // ownership transferred
    if ( iTitlePanePictureMask )
        {
        CleanupStack::Pop( dupMask );
        }

    bitmapIcon->SetBitmap( dupMain ); // ownership transferred
    CleanupStack::Pop( dupMain );

    //ownership of bitmapIcon is transferred
    CAknIcon* scalableIcon = AknIconUtils::CreateIconL( bitmapIcon );
    CleanupStack::PushL( scalableIcon );

    AknIconUtils::SetSize( scalableIcon->Bitmap(), size,
                           EAspectRatioPreservedAndUnusedSpaceRemoved );
    AknIconUtils::SetSize( scalableIcon->Mask(), size,
                           EAspectRatioPreservedAndUnusedSpaceRemoved );

    // Title pane takes ownership of icons.
    CFbsBitmap* bitmap = new ( ELeave ) CFbsBitmap;
    CleanupStack::PushL( bitmap );
    User::LeaveIfError( bitmap->Duplicate( scalableIcon->Bitmap()->Handle() ) );

    // Check if icon has mask
    CFbsBitmap* bitmapMask = NULL;
    if ( scalableIcon->Mask() )
        {
        bitmapMask = new ( ELeave ) CFbsBitmap;
        CleanupStack::PushL( bitmapMask );
        User::LeaveIfError(
            bitmapMask->Duplicate( scalableIcon->Mask()->Handle() ) );
        }

    // Set the icon in title pane...
    iTitlePane->SetPicture( bitmap, bitmapMask );

    if ( bitmapMask )
        {
        CleanupStack::Pop( bitmapMask );
        }
    CleanupStack::Pop( bitmap );
    CleanupStack::PopAndDestroy( scalableIcon );
    }

// ---------------------------------------------------------
// CPhoneStatusPane::SetTitlePaneContentL
// ---------------------------------------------------------
//
void CPhoneStatusPane::SetTitlePaneContentL( const TDesC& aContent )
    {
    iTitleText = aContent;
    iTitlePane->SetTextL( iTitleText );
    iTitlePaneTextIsDisplayed = ETrue;
   }

// ---------------------------------------------------------
// CPhoneStatusPane::SetTitlePanePicture
// ---------------------------------------------------------
//
void CPhoneStatusPane::SetTitlePanePictureL( 
    TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBitmap )
        {
        TPhoneCmdParamBitmap* bitmapParam = static_cast<TPhoneCmdParamBitmap*>(
            aCommandParam );
        
        delete iTitlePanePicture;
        iTitlePanePicture = NULL;
        iTitlePanePicture = bitmapParam->Bitmap();
        
        delete iTitlePanePictureMask;
        iTitlePanePictureMask = NULL;
        iTitlePanePictureMask = bitmapParam->MaskBitmap();
        
        RenderBitmapOnTitlePaneL();
        iTitlePaneTextIsDisplayed = EFalse;
        }
    }

// ---------------------------------------------------------
// CPhoneStatusPane::IsVisible
// ---------------------------------------------------------
//
TBool CPhoneStatusPane::IsVisible() const
    {
    return iStatusPane->IsVisible();
    }

// ---------------------------------------------------------
// CPhoneStatusPane::TitlePane
// ---------------------------------------------------------
//
CAknTitlePane& CPhoneStatusPane::TitlePane() const
    {
    return *iTitlePane;
    }

// ---------------------------------------------------------
// CPhoneStatusPane::StatusPane
// ---------------------------------------------------------
//
CEikStatusPane& CPhoneStatusPane::StatusPane() const
    {
    return *iStatusPane;
    }

// ---------------------------------------------------------
// CPhoneStatusPane::NaviPane
// ---------------------------------------------------------
//
CAknNavigationControlContainer& CPhoneStatusPane::NaviPane() const
    {
    return *iNaviPane;
    }

// ---------------------------------------------------------
// CPhoneStatusPane::NaviDecorator
// ---------------------------------------------------------
//
CAknNavigationDecorator& CPhoneStatusPane::NaviDecorator() const
    {
    return *iNaviDecorator;
    }

// ---------------------------------------------------------
// CPhoneStatusPane::AddTitlePaneHandlerL
// ---------------------------------------------------------
//
void CPhoneStatusPane::AddTitlePaneHandlerL( 
    MPhoneStatusPaneObserver& aObserver )
    {
    THandlerTag titleTag;
    titleTag.iObserver = &aObserver;
    titleTag.iLayer = -1; //-1 means it's not reserved
    iTitlePaneList->AppendL( titleTag );
    }

// ---------------------------------------------------------
// CPhoneStatusPane::ReserveTitlePane
// ---------------------------------------------------------
//
void CPhoneStatusPane::ReserveTitlePane( 
    MPhoneStatusPaneObserver& aObserver )
    {
    //notify the inactive of the topmost handler
    THandlerTag* topmostHandler = FindTopmostTitlePaneHandler();
    if ( topmostHandler && 
        topmostHandler->iObserver != &aObserver )
        {
        NotifyHandlerOnActivation( *topmostHandler, EFalse );
        }

    THandlerTag* titleTag = FindTitlePaneHandler( aObserver );
    if ( titleTag )
        {
        if ( topmostHandler )
            {
            titleTag->iLayer = topmostHandler->iLayer+1;
            }
        else
            {
            titleTag->iLayer = 0; // 0 is the lowest one
            }
        }
    }

// ---------------------------------------------------------
// CPhoneStatusPane::ReleaseTitlePane
// ---------------------------------------------------------
//
void CPhoneStatusPane::ReleaseTitlePane( 
    MPhoneStatusPaneObserver& aObserver )
    {
    THandlerTag* titleTag = FindTitlePaneHandler( aObserver );
    if ( titleTag )
        {
        titleTag->iLayer = -1; //put it to the unreserved status
        THandlerTag* topmostHandler = FindTopmostTitlePaneHandler();
        if ( topmostHandler )
            {
            NotifyHandlerOnActivation( *topmostHandler, ETrue );
            }
        }
    }

// ---------------------------------------------------------
// CPhoneStatusPane::RemoveTitlePaneHandler
// ---------------------------------------------------------
//
void CPhoneStatusPane::RemoveTitlePaneHandler( 
    MPhoneStatusPaneObserver& aObserver )
    {
    TInt count = iTitlePaneList->Count();
    for ( TInt i = 0; i<count; i++ ) 
        {
        THandlerTag& handler = iTitlePaneList->At( i );
        if ( &aObserver == handler.iObserver )
            {
            iTitlePaneList->Delete( i );
            break;
            }
        }
    THandlerTag* topmostHandler = FindTopmostTitlePaneHandler();
    if ( topmostHandler )
        {
        NotifyHandlerOnActivation( *topmostHandler, ETrue );
        }
    return;
    }


// ---------------------------------------------------------
// CPhoneStatusPane::IsTitlePaneVisible
// ---------------------------------------------------------
//
TBool CPhoneStatusPane::IsTitlePaneVisible( 
    MPhoneStatusPaneObserver& aObserver ) const
    {
    THandlerTag* titleTag = FindTitlePaneHandler( aObserver );
    if ( titleTag )
        {
        return ( titleTag->iLayer != -1 ) && 
            ( titleTag->iLayer == LargestLayerInTitlePaneList() );
        }
    return EFalse;
    }

// ---------------------------------------------------------
// CPhoneStatusPane::CreateTextTitlePaneLC
// ---------------------------------------------------------
//
CBase* CPhoneStatusPane::CreateTextTitlePaneLC( 
    TInt aTextResourceId )
    {
    return CPhoneTextTitlePane::NewLC( *this, aTextResourceId );
    }

// -----------------------------------------------------------------------------
// CPhoneStatusPane::GetShape
// -----------------------------------------------------------------------------
//
void CPhoneStatusPane::GetShape( TRegion& aRegion )
    {
    TRAPD( err, iStatusPane->GetShapeL( aRegion, ETrue, ETrue ) );
    if ( err != KErrNone )
        {
        aRegion.Clear();
        }
    }

// ---------------------------------------------------------
// CPhoneStatusPane::LargestLayerInTitlePaneList
// ---------------------------------------------------------
//
TInt CPhoneStatusPane::LargestLayerInTitlePaneList() const
    {
    THandlerTag* topmostHandler = FindTopmostTitlePaneHandler();
    if ( topmostHandler )
        {
        return topmostHandler->iLayer;
        }
    return -1; //-1 means no item in the list yet
    }

// ---------------------------------------------------------
// CPhoneStatusPane::FindTopmostTitlePaneHandler
// ---------------------------------------------------------
//
THandlerTag* CPhoneStatusPane::FindTopmostTitlePaneHandler() const
    {
    TInt largest = -1;
    THandlerTag* handler = NULL;
    TInt count = iTitlePaneList->Count();
    for ( TInt i = 0; i<count; i++ )
        {
        THandlerTag& currentHandler = iTitlePaneList->At( i );
        if ( largest < currentHandler.iLayer )
            {
            largest = currentHandler.iLayer;
            handler = &currentHandler;
            }
        }
    return handler;
    }

// ---------------------------------------------------------
// CPhoneStatusPane::FindTitlePaneHandler
// ---------------------------------------------------------
//
THandlerTag* CPhoneStatusPane::FindTitlePaneHandler( 
    MPhoneStatusPaneObserver& aObserver ) const
    {
    TInt count = iTitlePaneList->Count();
    for ( TInt i = 0; i<count; i++ ) 
        {
        THandlerTag* handler = &iTitlePaneList->At( i );
        if ( &aObserver == handler->iObserver )
            {
            return handler;
            }
        }
    return NULL;
    }

// ---------------------------------------------------------
// CPhoneStatusPane::NotifyHandlerOnActivation
// ---------------------------------------------------------
//
void CPhoneStatusPane::NotifyHandlerOnActivation( 
    THandlerTag& aHandlerTag,
    TBool aActive )
    {
    TRAP_IGNORE( aHandlerTag.iObserver->HandleTitlePaneActiveL( aActive ) );
    }

// ---------------------------------------------------------
// CPhoneStatusPane::CreateEmptyIndicatorContainerL
// ---------------------------------------------------------
//
CAknIndicatorContainer* CPhoneStatusPane::CreateEmptyIndicatorContainerL()
    {
    CCoeControl* container = StatusPane().
    ContainerControlL( TUid::Uid( EEikStatusPaneUidIndic ) );
    
    CAknIndicatorContainer* indicatorContainer =
         new (ELeave) CAknIndicatorContainer(
             CAknIndicatorContainer::ENaviPaneEditorIndicators );
 
     CleanupStack::PushL( indicatorContainer );
     
     indicatorContainer->SetContainerWindowL( *container );
 
     TResourceReader reader;
     CCoeEnv::Static()->CreateResourceReaderLC( reader,
             R_PHONEUI_EMPTY_INDICATORS ); 
     indicatorContainer->ConstructFromResourceL( reader );
 
     CleanupStack::PopAndDestroy();  // resource reader
     
     TRect controlRect = StatusPane().PaneRectL( TUid::Uid( EEikStatusPaneUidIndic ) );
     indicatorContainer->SetRect( TRect( controlRect.Size() ) );
     indicatorContainer->ActivateL();
 
     CleanupStack::Pop( indicatorContainer ); // indicatorContainer
 
     return indicatorContainer;
     }

//
// IMPLEMENTATION OF CPHONETEXTTITLEPANE 
//
// ---------------------------------------------------------
// CPhoneTextTitlePane::NewLC
// ---------------------------------------------------------
//
CPhoneTextTitlePane* CPhoneTextTitlePane::NewLC( 
    CPhoneStatusPane& aStatusPaneManager,
    TInt aTextResourceId )
    {
    CPhoneTextTitlePane* self = 
        new (ELeave) CPhoneTextTitlePane(
            aStatusPaneManager,
            aTextResourceId );

    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// Destructor      
CPhoneTextTitlePane::~CPhoneTextTitlePane()
    {
    iStatusPaneManager.ReleaseTitlePane( *this );
    iStatusPaneManager.RemoveTitlePaneHandler( *this );
    }

// ---------------------------------------------------------
// CPhoneTextTitlePane::HandleTitlePaneActiveL
// ---------------------------------------------------------
//
void CPhoneTextTitlePane::HandleTitlePaneActiveL( 
    TBool /*aActive*/ )
    {
    if ( iStatusPaneManager.IsTitlePaneVisible( *this ) )
        {
        HBufC* text = StringLoader::LoadL( iTextResourceId );
        iStatusPaneManager.TitlePane().SetText( text );
        }
    }
        
// -----------------------------------------------------------------------------
// CPhoneTextTitlePane::CPhoneTextTitlePane
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneTextTitlePane::CPhoneTextTitlePane(
    CPhoneStatusPane& aStatusPaneManager,
    TInt aTextResourceId ) :
    iStatusPaneManager( aStatusPaneManager ),
    iTextResourceId( aTextResourceId )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneTextTitlePane::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneTextTitlePane::ConstructL()
    {
    iStatusPaneManager.AddTitlePaneHandlerL( *this );
    iStatusPaneManager.ReserveTitlePane( *this );

    HandleTitlePaneActiveL( ETrue );
    }  

//  End of File  
