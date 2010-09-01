/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  View for Phone Application skinning.
*
*/


// INCLUDE FILES

// This plugin specific.
#include <PslnCallImagePluginRsc.rsg>
#include <pslncallimageplugin.mbg>
#include "PslnCallImagePlugin.h"
#include "PslnCallImagePluginContainer.h"
#include "PslnCallImagePluginDRM.h"

// General services.
#include <gulicon.h>
#include <bautils.h>
#include <eikfrlbd.h>
#include <StringLoader.h>
#include <aknViewAppUi.h>
#include <featmgr.h>
#include <ConeResLoader.h>
#include <pslninternalcrkeys.h>
#include <AknsSrvClient.h>
#include <aknnotewrappers.h>
#include <AknsWallpaperUtils.h>

// Psln Framework specific.
#include <pslnfwviewuids.h>
#include <psln.hrh>
#include <PslnCommon.rsg>

// General services
#include <MGFetch.h>

// Resources
#include <Psln.rsg>

// Logging
#include "PslnCallImagePluginLogger.h"

// CONSTANTS

// Path to mbm file.
_LIT( KPslnPhoneApplicationIconFileName, "z:PslnCallImagePlugin.mbm");

// Path to compiled resource file.
_LIT( KPslnCallImagePluginResourceFileName, "z:PslnCallImagePluginRsc.rsc" );

// Path to common personalization resources. This resource file is meant for 
// shared resources between application and plugins.
_LIT( KPslnCommonResourceFileName, "z:pslncommon.rsc" );

// Path to Psln application resource file. Plugin uses some localized texts from Psln's
// resources,
_LIT( KPslnApplicationResourceFileName, "z:Psln.rsc" );

// Extension for scalable vector graphics file.
_LIT( KAknsSkinSrvSvgFileExt, ".svg" );

// Selected file.
const TInt KPslnSelectedFile = 0;

// Location of this plugin view within Psln's tab group.
const TInt KPslnApplicationPhonePluginLocation = 5;

const TInt KPslnFileArrayGranularity = 3;

// Middle Softkey control ID.
const TInt KPslnMSKControlId = 3;

// ========================= MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CPslnCallImagePlugin::CPslnCallImagePlugin()
// 
// Constructor
// ----------------------------------------------------------------------------
//
CPslnCallImagePlugin::CPslnCallImagePlugin( CAknViewAppUi* aAppUi )
  : iResourceLoader( *iCoeEnv ), iResourceLoaderCommon( *iCoeEnv ), 
    iResourceLoaderPsln( *iCoeEnv ), iAppUi ( aAppUi )
    {
    }    

// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::NewL()
// 
// Symbian OS default constructor
// ---------------------------------------------------------------------------
CPslnCallImagePlugin* CPslnCallImagePlugin::NewL( TAny* aAppUi )
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::NewL");
    CAknViewAppUi* appUi = reinterpret_cast<CAknViewAppUi*>( aAppUi );
    CPslnCallImagePlugin* self = new( ELeave ) CPslnCallImagePlugin ( appUi );
    
    CleanupStack::PushL( self );
    self->ConstructL();    
    CleanupStack::Pop();
    
    return self;
    }

// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::ConstructL()
// 
// Symbian OS two-phased constructor
// ---------------------------------------------------------------------------
void CPslnCallImagePlugin::ConstructL()
    {
    // Find the resource file:
    TParse parse;
    parse.Set( KPslnCommonResourceFileName, &KDC_APP_RESOURCE_DIR, NULL );      
    TFileName* fileName = new (ELeave) TFileName( parse.FullName() );
    CleanupStack::PushL( fileName );
	
    // Open resource file:
    iResourceLoaderCommon.OpenL( *fileName );
    
    // Find the resource file:
    parse.Set( KPslnApplicationResourceFileName, &KDC_APP_RESOURCE_DIR, NULL );
    
	OpenLocalizedResourceFileL( 
        KPslnCallImagePluginResourceFileName, 
        iResourceLoader );

    CleanupStack::PopAndDestroy( fileName );

    BaseConstructL( R_PSLN_CALL_IMAGE_VIEW );    
    
    __CALLLOGSTRING("CPslnCallImagePlugin::ConstructL -> METHOD COMPLETED");
    }

// ----------------------------------------------------------------------------
// CPslnCallImagePlugin::~CPslnCallImagePlugin
// 
// Destructor
// ----------------------------------------------------------------------------
CPslnCallImagePlugin::~CPslnCallImagePlugin()
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::~CPslnCallImagePlugin -> START");
    iResourceLoaderCommon.Close();
    iResourceLoaderPsln.Close();
    iResourceLoader.Close();
    __CALLLOGSTRING("CPslnCallImagePlugin::~CPslnCallImagePlugin -> METHOD COMPLETED");
    }

// ---------------------------------------------------------------------------
// TUid CPslnCallImagePlugin::Id()
// 
// Returns view's ID.
// ---------------------------------------------------------------------------
TUid CPslnCallImagePlugin::Id() const
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::Id");
    return KPslnCallImagePluginUid;
    }

// ----------------------------------------------------------------------------
// CPslnCallImagePlugin::GetCaption
// 
// Return application/view caption.
// ----------------------------------------------------------------------------
//
void CPslnCallImagePlugin::GetCaptionL( TDes& aCaption ) const
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::GetCaptionL");
    
       // the resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_PSLN_CI_LIST_VIEW_CAPTION );
    if ( aCaption.MaxLength() >= result->Length() )
        {
        aCaption.Copy( *result );
        }
    delete result;
    }    

// ----------------------------------------------------------------------------
// CPslnCallImagePlugin::GetTabTextL
// 
// Return application/view caption.
// ----------------------------------------------------------------------------
//
void CPslnCallImagePlugin::GetTabTextL( TDes& aCaption ) const
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::GetTabTextL");
    
    // the resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_PSLN_CI_TAB_NAME );
    if ( aCaption.MaxLength() >= result->Length() )
        {
        aCaption.Copy( *result );
        }
    delete result;
    }

// ----------------------------------------------------------------------------
// CPslnCallImagePlugin::CreateIconL
// 
// Creates Main view icon.
// ----------------------------------------------------------------------------
//    
CGulIcon* CPslnCallImagePlugin::CreateIconL()
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::CreateIconL");

    // Find the resource file:
    TParse parse;
    parse.Set( KPslnPhoneApplicationIconFileName, &KDC_APP_BITMAP_DIR, NULL );
    TFileName fileName( parse.FullName() );
    
    CGulIcon* icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(), 
        KAknsIIDQgnPropPslnCimageSub, 
        fileName,
        EMbmPslncallimagepluginQgn_prop_psln_cimage_sub,
        EMbmPslncallimagepluginQgn_prop_psln_cimage_sub_mask
        );

    return icon;
    }

// -----------------------------------------------------------------------------
// CPslnCallImagePlugin::GetLocationTypeAndIndex()
//
//
// -----------------------------------------------------------------------------
//
void CPslnCallImagePlugin::GetLocationTypeAndIndex( 
            TPslnFWLocationType& aType, 
            TInt& aIndex ) const
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::GetLocationTypeAndIndex");
    aType = CPslnFWPluginInterface::EPslnFWSpecified;
    aIndex = KPslnApplicationPhonePluginLocation;
    }

// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::HandleCommandL(TInt aCommand)
// 
// Handles commands directed to this class.
// ---------------------------------------------------------------------------
//
void CPslnCallImagePlugin::HandleCommandL( TInt aCommand )
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::HandleCommandL -> START");
    switch ( aCommand )
        {
        case EPslnCmdAppDownload:
        case EPslnCmdAppActivate:
        case EAknSoftkeyOk:
            if ( iContainer->iListBox->CurrentItemIndex() == 
                 EPlsnCallImageThemeImage )
                {
                SetCallImageSettingL( EPlsnCRThemeImage );
                }
        	else if ( iContainer->iListBox->CurrentItemIndex() == 
        	          EPlsnCallImageUserDefinedImage )
            	{
            	// Don't launch more than one image selection at time
            	if ( !iVerifier )
            	    {
            	    __CALLLOGSTRING("CPslnCallImagePlugin::HandleCommandL -> SetCallImage");
                    SetCallImageL();
            	    }
           		}
           	else // EPlsnCallImageNone
           		{
           		TInt imageError = SetCallImagePath( KNullDesC );
	            if ( imageError != KErrNone )
	                {
	                __CALLLOGSTRING1("CPslnCallImagePlugin::HandleCommandL -> SetCallImagePath error: %d", imageError);
	        		HandleImageErrorsL( imageError );
	                }
	            SetCallImageSettingL( EPlsnCRCallImageNone );
           		}           		 	
           	// Update container.
        	static_cast<CPslnCallImagePluginContainer*>
        		(iContainer)->UpdateListBoxL();
        	CheckMiddleSoftkeyLabelL();
        	break;
        case EAknSoftkeyBack:
            if ( iAppUi->View( KPslnMainViewUid ) )
                {
                __CALLLOGSTRING("CPslnCallImagePlugin::HandleCommandL -> Activate PSLN");
                iAppUi->ActivateLocalViewL( KPslnMainViewUid );
                }
            else
                {
                iAppUi->HandleCommandL( aCommand );
                }
            break;
        case EPslnCmdAppHelp:
        case EAknCmdHelp:
            CPslnFWBaseView::HandleCommandL( aCommand );
            break;
        default:
            // Help and Exit are handled by AppUi.
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    __CALLLOGSTRING("CPslnCallImagePlugin::HandleCommandL -> METHOD COMPLETED");
    }

// ----------------------------------------------------------------------------
// CPslnCallImagePlugin::DynInitMenuPaneL
//
//
// -----------------------------------------------------------------------------
//
void CPslnCallImagePlugin::DynInitMenuPaneL(  
			TInt aResourceId, CEikMenuPane* aMenuPane  )
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::DynInitMenuPaneL -> START");
    if ( aResourceId == R_PSLN_GEN_VIEW_MENUPANE )
    	{
        // Since this is common resource it contains download - set it off.
        aMenuPane->SetItemDimmed( EPslnCmdAppDownload, ETrue );
        // Since this is common resource it contains activate - set it off.
        aMenuPane->SetItemDimmed( EPslnCmdAppActivate, ETrue );      	         
    	}
   else if ( aResourceId == R_PSLN_CI_BASIC_MENUPANE )
    	{
    	TInt active = static_cast<CPslnCallImagePluginContainer*>
                    (iContainer)->CurrentSelectionIndexL();
        // Set menu item is used when inactive item is focused and it 
		// activates the focused item.
    	if ( active != iContainer->iListBox->CurrentItemIndex() )
    		{
        	aMenuPane->SetItemDimmed( EPslnCmdAppActivate, EFalse );
        	aMenuPane->SetItemDimmed( EPslnCmdAppDownload, ETrue );
        	}
        else if ( ( active == iContainer->iListBox->CurrentItemIndex() ) && 
            ( active == EPlsnCallImageNone ||
              active == EPlsnCallImageThemeImage ) )
        	{
        	aMenuPane->SetItemDimmed( EPslnCmdAppActivate, ETrue );
        	aMenuPane->SetItemDimmed( EPslnCmdAppDownload, ETrue );
        	}
        else
        	{
        	aMenuPane->SetItemDimmed( EPslnCmdAppActivate, ETrue );
        	aMenuPane->SetItemDimmed( EPslnCmdAppDownload, EFalse );
        	}	
	
    	if( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            // Disable help if not supported
            aMenuPane->SetItemDimmed( EPslnCmdAppHelp, ETrue );
            }
    	}
    __CALLLOGSTRING("CPslnCallImagePlugin::DynInitMenuPaneL <- END");
    }

// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::SetBoolL
// 
// Sets boolean to KCRUidThemes key.
// ---------------------------------------------------------------------------
//
void CPslnCallImagePlugin::SetCallImageSettingL( const TInt aValue )
	{
	CRepository* setting = CRepository::NewL ( KCRUidThemes ); 
	CleanupStack::PushL( setting );     
	setting->Set( KThemesCallImageSetting, aValue );
	CleanupStack::PopAndDestroy( setting );
	}

// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::SetPathL
// 
// Sets path to KCRUidThemes key.
// ---------------------------------------------------------------------------
//
TInt CPslnCallImagePlugin::SetPathL( const TDesC& aPath )
	{
	CRepository* pathCen = CRepository::NewL ( KCRUidThemes ); 
	CleanupStack::PushL( pathCen );    
	TInt error = pathCen->Set( KThemesCallImagePath, aPath );
	CleanupStack::PopAndDestroy( pathCen );
	return error;
	}
	
// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::SetCallImageL
// 
// Sets call image.
// ---------------------------------------------------------------------------
//
void CPslnCallImagePlugin::SetCallImageL()
	{
	__CALLLOGSTRING("CPslnCallImagePlugin::SetCallImage -> START");
	
	TInt retVal( KErrNone );
	CDesCArrayFlat* files = 
    	new (ELeave) CDesCArrayFlat( KPslnFileArrayGranularity );
	CleanupStack::PushL( files );
	// Create DRM verifier
	iVerifier = CPslnCallImagePluginDRM::NewL();
	// Show images to user to select one	

	TBool selectedItem = MGFetch::RunL( *files, EImageFile, EFalse, iVerifier );

	if ( !iVerifier )
		{
        __CALLLOGSTRING("CPslnCallImagePlugin::SetCallImage DRM Error");
		retVal = KErrCancel;
	    }	    
	else if( selectedItem && ( files->MdcaCount() > 0 ) )
		{
	    __CALLLOGSTRING("CPslnCallImagePlugin::SetCallImage Fetch successful");
	    retVal = SetCallImagePath( files->MdcaPoint( KPslnSelectedFile ) );
	    if ( retVal != KErrNone )
	    	{
	    	__CALLLOGSTRING1("CPslnCallImagePlugin::SetCallImage -> SetCallImagePath error: %d", retVal );
	        HandleImageErrorsL( retVal );
	        }
	    }
	else
	   {
       __CALLLOGSTRING("CPslnCallImagePlugin::SetCallImage Fetch cancelled");
	   retVal = KErrCancel;
	   }
	   
	if ( retVal == KErrNone )
	    {
	    // no way currently of checking corrupted svg:s
	    if ( ( files->MdcaPoint( KPslnSelectedFile ).Right( 4 ) ).CompareF( KAknsSkinSrvSvgFileExt ) )
	    	{
	    	//Before setting image, check is the image corrputed
	    	RAknsSrvSession skinsrv;
	    	User::LeaveIfError( skinsrv.Connect() );
	    	CleanupClosePushL( skinsrv );
	    	CFbsBitmap* bmp = NULL;
	    	CFbsBitmap* mask = NULL;
			// (-1, -1) means that image is just decoded, not used    	
	   	 	TRAP( retVal, skinsrv.DecodeWallpaperImageL( files->MdcaPoint( KPslnSelectedFile ), TSize(-1,-1), bmp, mask ) );
	        CleanupStack::PopAndDestroy(); // skinsrv
	    	}

	    if( retVal == KErrNone )
	        {
	        // If image set without problems update setting.
	        SetCallImageSettingL( EPlsnCRUserDefinedImage );
	        }
	   	else
	   	     {
             //File is corrupted
             HandleImageErrorsL( retVal );
	   	     }
		}
	
	CleanupStack::PopAndDestroy( files );
	
	if ( iVerifier )
	    {
	    delete iVerifier;
	    iVerifier = NULL;
	    }
	__CALLLOGSTRING("CPslnCallImagePlugin::SetCallImage -> METHOD COMPLETED");
	}
// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::HandleImageErrorsL
// 
// Handles image setting errors.
// ---------------------------------------------------------------------------
//
void CPslnCallImagePlugin::HandleImageErrorsL( TInt aError )
    {
    __CALLLOGSTRING1("CPslnCallImagePlugin::HandleImageError aError: %d -> START", aError );    
    TInt resourceId = KErrNone;
    if ( aError == KErrNoMemory )
        {
        // Out of memory when handling the image,
        User::Leave( aError );
        }    
    else
        {
        // Image is corrupted or in wrong format
        resourceId = R_PSLN_IMAGE_CORRUPTED;        
        }

    // Show information note
    HBufC* prompt = iCoeEnv->AllocReadResourceLC( resourceId );
    CAknInformationNote* note = new (ELeave) CAknInformationNote( ETrue );
    note->ExecuteLD( *prompt );
    CleanupStack::PopAndDestroy( prompt );
    __CALLLOGSTRING("CPslnCallImagePlugin::HandleImageError -> METHOD COMPLETED");
    }

// -----------------------------------------------------------------------------
// CPslnCallImagePlugin::SetCallImagePath
// Set call image path.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPslnCallImagePlugin::SetCallImagePath( const TDesC& aImagePath )
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::SetCallImagePath -> START");
    TInt retVal(KErrNone);
    // If CentralRepository set fails caller will handle the error code.
    if( aImagePath.Length() >= 0 )
        {
        retVal = SetPathL( aImagePath );    
        }
    else
    	{
    	retVal = KErrPathNotFound;
    	}
    	
	__CALLLOGSTRING("CPslnCallImagePlugin::SetCallImage -> METHOD COMPLETED");
    return retVal;
    }

// ----------------------------------------------------------------------------
// CPslnCallImagePlugin::Container
// 
// Return handle to container class.
// ----------------------------------------------------------------------------
//
CPslnCallImagePluginContainer* CPslnCallImagePlugin::Container()
    {
    return static_cast<CPslnCallImagePluginContainer*>( iContainer );
    }

// ----------------------------------------------------------------------------
// CPslnCallImagePlugin::DoActivateL
// 
// First method called by the Avkon framwork to invoke a view.
// ----------------------------------------------------------------------------
//
void CPslnCallImagePlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                     TUid aCustomMessageId,
                                     const TDesC8& aCustomMessage )
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::DoActivateL");
    CPslnFWBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );    

    // If called from Psln - set tab group location.
    if ( iAppUi->View( KPslnMainViewUid ) )
        {
        // This tries to set the plugin to the given location in Tab.
        CPslnFWBaseView::SetTabIndex( KPslnApplicationPhonePluginLocation );
        // This is the location that PslnFW actually put the plugin into. 
        TInt ciRealLocation = CPslnFWBaseView::GetTabIndex();
        CPslnFWBaseView::SetNaviPaneL( ciRealLocation );
        }
    else
        {
        CPslnFWBaseView::SetNaviPaneL();
        }
        
    if ( iContainer )
        {
        TInt currentItem = static_cast<CPslnCallImagePluginContainer*>
                    (iContainer)->CurrentSelectionIndexL();
        // Set highlight to selected item.
        iContainer->iListBox->SetCurrentItemIndexAndDraw( currentItem );
        }
    // Check middle softkey.    
    CheckMiddleSoftkeyLabelL();
    }
                                
// ----------------------------------------------------------------------------
// CPslnCallImagePlugin::DoDeactivate
// 
// Called by the Avkon view framework when closing.
// ----------------------------------------------------------------------------
//
void CPslnCallImagePlugin::DoDeactivate()
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::DoDeactivate");
    CPslnFWBaseView::DoDeactivate();
    }

// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::NewContainerL()
// 
// Creates new iContainer.
// ---------------------------------------------------------------------------
//
void CPslnCallImagePlugin::NewContainerL()
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::NewContainerL");
    iContainer = new( ELeave ) CPslnCallImagePluginContainer;
    iContainer->SetMiddleSoftkeyObserver( this );
    }

// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::HandleListBoxSelectionL()
// 
// Handles events raised through a rocker key.
// ---------------------------------------------------------------------------
//
void CPslnCallImagePlugin::HandleListBoxSelectionL()
    {
    __CALLLOGSTRING("CPslnCallImagePlugin::HandleListBoxSelectionL");
    HandleCommandL( EAknSoftkeyOk );
    }

// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::CheckMiddleSoftkeyLabelL()
// 
// 
// ---------------------------------------------------------------------------
//
void CPslnCallImagePlugin::CheckMiddleSoftkeyLabelL()
    {  
    // First remove any prevous commands.
    RemoveCommandFromMSK();

    if ( iContainer )
        {
        TInt currentlyActive = static_cast<CPslnCallImagePluginContainer*>
                    (iContainer)->CurrentSelectionIndexL();
        TInt currentItem = iContainer->iListBox->CurrentItemIndex();
        // Set MSK label.
        if ( currentlyActive != currentItem && 
             ( currentItem == EPlsnCallImageNone ||
               currentItem == EPlsnCallImageThemeImage ) )  
            {
            // Set middle softkey as Apply, if:
            // a) 'None' is selected
            // b) highlight is on inactive selection
            CPslnFWBaseView::SetMiddleSoftKeyLabelL( 
                R_PSLN_MSK_ACTIVATE,
                EPslnCmdAppActivate );
            }
        else if( ( currentlyActive == currentItem ) && 
            ( currentlyActive == EPlsnCallImageNone ||
                currentlyActive == EPlsnCallImageThemeImage ) )
            {
            CPslnFWBaseView::SetMiddleSoftKeyLabelL( 
                R_TEXT_SOFTKEY_EMPTY, 
                EPslnCmdEmptyCommand );
            }
        else
            {
            // Otherwise set middle softkey as Change. 
            CPslnFWBaseView::SetMiddleSoftKeyLabelL( 
                R_PSLN_MSK_CHANGE,
                EPslnCmdAppDownload );
            }
        }
    }

// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::RemoveCommandFromMSK()
// 
// 
// ---------------------------------------------------------------------------
//
void CPslnCallImagePlugin::RemoveCommandFromMSK()
    {
    CEikButtonGroupContainer* cbaGroup = Cba();
    if ( cbaGroup )
        {
        cbaGroup->RemoveCommandFromStack( KPslnMSKControlId, EPslnCmdAppActivate );
        cbaGroup->RemoveCommandFromStack( KPslnMSKControlId, EPslnCmdAppDownload );
        }
    }
    
// ---------------------------------------------------------------------------
// CPslnCallImagePlugin::SetTitlePaneL()
// 
// Gives resource ID to be used as plugin view title.
// ---------------------------------------------------------------------------
//  
void CPslnCallImagePlugin::SetTitlePaneL( TInt& aResourceId )
    {
#ifdef RD_CONTROL_PANEL
    aResourceId = R_PSLN_TITLE_PANE_CALLIMAGE;
#endif // RD_CONTROL_PANEL
    }
    
// End of File
