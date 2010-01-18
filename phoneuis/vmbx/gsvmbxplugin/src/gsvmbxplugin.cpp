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
* Description:  gsvmbxplugin implementation.
*
*/


// INCLUDE
#include    "gsvmbxplugin.h"
#include    "VMBLogger.h"
#include    <gsparentplugin.h>
#include    <gscommon.hrh>
#include    <gsvmbxpluginrsc.rsg>
#include    <gsvmbxplugin.mbg>
#include    <gsprivatepluginproviderids.h>
#include    <AknNullService.h>
#include    <bautils.h>
#include    <StringLoader.h>
#include    <centralrepository.h>

// Constants

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSVmbxPlugin::CGSVmbxPlugin()
// Constructor
//
// ---------------------------------------------------------------------------
//
CGSVmbxPlugin::CGSVmbxPlugin()
    : iResources( *iCoeEnv ), iNullService(NULL)
    {
    }


// ---------------------------------------------------------------------------
// CGSVmbxPlugin::CGSVmbxPlugin()
// Destructor
//
// ---------------------------------------------------------------------------
//
CGSVmbxPlugin::~CGSVmbxPlugin()
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::~CGSVmbxPlugin =>" );
    
    iResources.Close();
    delete iNullService;

    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::~CGSVmbxPlugin <=" );
    }


// ---------------------------------------------------------------------------
// CGSVmbxPlugin::ConstructL()
// Symbian OS two-phased constructor
//
// ---------------------------------------------------------------------------
//
void CGSVmbxPlugin::ConstructL()
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::ConstructL =>" );
    
    BaseConstructL();
    OpenLocalizedResourceFileL( KVmbxPluginResourceFileName, iResources );
    
	VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::ConstructL <=" );    
    }


// ---------------------------------------------------------------------------
// CGSVmbxPlugin::NewL()
// Static constructor
//
// ---------------------------------------------------------------------------
//
CGSVmbxPlugin* CGSVmbxPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSVmbxPlugin* self = new( ELeave ) CGSVmbxPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CGSVmbxPlugin::Id
// 
//
// -----------------------------------------------------------------------------
//
TUid CGSVmbxPlugin::Id() const
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::Id =>" );
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::Id <=" );
    return KGSVmbxPluginUID;
    }


// -----------------------------------------------------------------------------
// CGSVmbxPlugin::DoActivateL
//
//
// -----------------------------------------------------------------------------
//
void CGSVmbxPlugin::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                  TUid /*aCustomMessageId*/,
                                  const TDesC8& /*aCustomMessage*/ )
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::DoActivateL =>" );
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::DoActivateL <=" );
    }


// -----------------------------------------------------------------------------
// CGSVmbxPlugin::DoDeactivate
//
//
// -----------------------------------------------------------------------------
//
void CGSVmbxPlugin::DoDeactivate()
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::DoDeactivate =>" );    
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::DoDeactivate <=" );
    }


// ========================= From CGSPluginInterface ==================


// -----------------------------------------------------------------------------
// CGSVmbxPlugin::GetCaptionL
//
//
// -----------------------------------------------------------------------------
//
void CGSVmbxPlugin::GetCaptionL( TDes& aCaption ) const
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::GetCaptionL =>" );
    
    HBufC* result = StringLoader::LoadL( R_GS_VMBX_PLUGIN_CAPTION );
    aCaption.Copy( *result );
    delete result;
    
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::GetCaptionL <=" );
    }


// -----------------------------------------------------------------------------
// CGSVmbxPlugin::PluginProviderCategory
//
//
// -----------------------------------------------------------------------------
//
TInt CGSVmbxPlugin::PluginProviderCategory() const
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::PluginProviderCategory =>" );
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::PluginProviderCategory <=" );
    return KGSPluginProviderInternal;
    }


// -----------------------------------------------------------------------------
// CGSVmbxPlugin::ItemType()
//
//
// -----------------------------------------------------------------------------
//
TGSListboxItemTypes CGSVmbxPlugin::ItemType()
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::ItemType =>" );
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::ItemType <=" );
    return EGSItemTypeSettingDialog;
    }


// -----------------------------------------------------------------------------
// CGSVmbxPlugin::GetValue()
//
//
// -----------------------------------------------------------------------------
//
void CGSVmbxPlugin::GetValue( const TGSPluginValueKeys /*aKey*/,
                                      TDes& /*aValue*/ )
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::GetValue =>" );
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::GetValue <=" );
    }


// -----------------------------------------------------------------------------
// CGSVmbxPlugin::HandleSelection()
//
//
// -----------------------------------------------------------------------------
//
void CGSVmbxPlugin::HandleSelection(
    const TGSSelectionTypes /*aSelectionType*/ )
    {  
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::HandleSelection =>" );  
    TRAP_IGNORE( LaunchVoiceMailBoxAppL() );
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::HandleSelection <=" );
    }

// ---------------------------------------------------------------------------
// CGSVmbxPlugin::CreateIconL
//
//
// ---------------------------------------------------------------------------
//
CGulIcon* CGSVmbxPlugin::CreateIconL( const TUid aIconType )
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::CreateIconL =>" );  
    
    CGulIcon* icon;
    TParse* fp = new( ELeave ) TParse();
    CleanupStack::PushL( fp );
    
    fp->Set( KGSVmbxPluginIconDirAndName, &KDC_BITMAP_DIR, NULL );

    if ( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropSetConnConfig,
        fp->FullName(),
        EMbmGsvmbxpluginQgn_prop_cp_tele_vmail,
        EMbmGsvmbxpluginQgn_prop_cp_tele_vmail_mask );
        }    
    else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    CleanupStack::PopAndDestroy( fp );

    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::CreateIconL <=" );
    return icon;
    }


// -----------------------------------------------------------------------------
// CGSVmbxPlugin::OpenLocalizedResourceFileL()
// 
// Opens localized resource file.
// -----------------------------------------------------------------------------
//
void CGSVmbxPlugin::OpenLocalizedResourceFileL(
    const TDesC& aResourceFileName,
    RConeResourceLoader& aResourceLoader )
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::OpenLocalizedResourceFileL =>" );  
    
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect() );

    // Find the resource file:
    TParse parse;
    parse.Set( aResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );

    // Get language of resource file:
    BaflUtils::NearestLanguageFile( fsSession, fileName );

    // Open resource file:
    aResourceLoader.OpenL( fileName );

    // If leave occurs before this, close is called automatically when the
    // thread exits.
    fsSession.Close();
    
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::OpenLocalizedResourceFileL <=" );
    }

// -----------------------------------------------------------------------------
// CGSVmbxPlugin::LaunchVoiceMailBoxAppL()
//
// Launches Voice Mailbox application.
// -----------------------------------------------------------------------------
//
void CGSVmbxPlugin::LaunchVoiceMailBoxAppL()
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::LaunchVoiceMailBoxAppL =>" );
    
    // Get the correct application data
    RWsSession ws;
    User::LeaveIfError( ws.Connect() );
    CleanupClosePushL( ws );

    // Find the task with uid
    TApaTaskList taskList( ws );
    TApaTask task = taskList.FindApp( KGsVmbxAppUid );
    
    if ( task.Exists() )
        {
        task.BringToForeground();
        }
    else
        {
        // Launch Voice Mailbox application as embedded
        TAppInfo app( KGsVmbxAppUid, KGSVmbxApp );            
        EmbedAppL( app );
        }

    CleanupStack::PopAndDestroy(); // ws
    
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::LaunchVoiceMailBoxAppL <=" );    
    }
// -----------------------------------------------------------------------------
// CGSVmbxPlugin::EmbedAppL()
//
// Launches application as embedded.
// -----------------------------------------------------------------------------
//
void CGSVmbxPlugin::EmbedAppL( const TAppInfo& aApp )
    {
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::EmbedAppL =>" );
    
    // Launch settings app
    if ( iNullService )
        {
        delete iNullService;
        iNullService = NULL;
        }
    iNullService = CAknNullService::NewL( aApp.iUid, this );
    
    VMBLOGSTRING( "VMBXPLUGIN: CGSVmbxPlugin::EmbedAppL <=" ); 
    }

// End of file
