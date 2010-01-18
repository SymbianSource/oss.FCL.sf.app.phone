/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of Call Image plugin.
*
*/

    

// INCLUDE FILES
#include <AiwCommon.h>              // AIW classes and definitions.
#include <AiwCommon.hrh>            // AIW constants.
#include <AiwMenu.h>                // AIW Menu pane.
#include <aiwcallimageplugin.rsg>
#include <pslninternalcrkeys.h>
#include <centralrepository.h>
#include <data_caging_path_literals.hrh>
#include <aknnotedialog.h>
#include <DRMHelper.h>
#include <bautils.h>
#include <drmutility.h>
#include <AknsSrvClient.h>

#include "aiwcallimageplugin.h"     // This class interface.
#include "aiwcallimageplugin.hrh"   // AiwCallImagePlugin constants.



// CONSTANTS

// Drive letter.
_LIT( KResourceDrive, "z:" );

// Path to compiled resource file.
_LIT( KCallImageResFile, "aiwcallimageplugin.rsc" );

// Call Image mime types.
_LIT( KCallImagePluginMimeTypeImage, "IMAGE" );
_LIT( KCallImagePluginMimeTypeOTABitmap, "IMAGE/X-OTA-BITMAP" );
_LIT( KCallImagePluginSeparator, "/" );
// Extension for scalable vector graphics file.
_LIT( KAknsSkinSrvSvgFileExt, ".svg" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAiwCallImagePlugin::CAiwCallImagePlugin
// 
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CAiwCallImagePlugin::CAiwCallImagePlugin()
    {
    }
    
// -----------------------------------------------------------------------------
// CAiwCallImagePlugin::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAiwCallImagePlugin* CAiwCallImagePlugin::NewL()
    {
    CAiwCallImagePlugin* self = new( ELeave ) CAiwCallImagePlugin;
    return self;
    }

// -----------------------------------------------------------------------------
// CAiwCallImagePlugin::~CAiwCallImagePlugin
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CAiwCallImagePlugin::~CAiwCallImagePlugin()
    {
    }

// -----------------------------------------------------------------------------
// CAiwCallImagePlugin::InitialiseL
// 
// 
// -----------------------------------------------------------------------------
//
void CAiwCallImagePlugin::InitialiseL(
    MAiwNotifyCallback& /*aFrameworkCallback*/,
    const RCriteriaArray& /*aInterest*/)
    {
    // Stub. Not used.
    }

// -----------------------------------------------------------------------------
// CAiwCallImagePlugin::HandleServiceCmdL
// 
// 
// -----------------------------------------------------------------------------
//
void CAiwCallImagePlugin::HandleServiceCmdL(
    const TInt& aCmdId,
    const CAiwGenericParamList& aInParamList,
    CAiwGenericParamList& /*aOutParamList*/,
    TUint aCmdOptions,
    const MAiwNotifyCallback* /*aCallback*/ )
    {
    
    // Cancel bit must always be checked. Support can be implemented if necessary.
    if( aCmdOptions & KAiwOptCancel )
        {
        return;
        }
        
    // Handle only call command.
    if ( aCmdId == KAiwCmdAssign )
        {
        // Check if there is call image path parameter.
        TInt index = 0;
        const TAiwGenericParam* genericParam = NULL; 
            
        genericParam = aInParamList.FindFirst( 
            index,
            EGenericParamFile,
            EVariantTypeDesC );
            
        if ( index >= 0 && genericParam )
            {
             User::LeaveIfError( SetCallImagePathL( genericParam ) );
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CAiwCallImagePlugin::SetCallImagePathL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CAiwCallImagePlugin::SetCallImagePathL( const TAiwGenericParam* aPathParam )
    {
    TInt error( KErrNone );
    TBool drm( EFalse );
    TBool iDrmProtected( EFalse ); 
    RFile fileHandle;
    TUint fileMode = EFileShareReadersOrWriters | EFileStream | EFileRead;
    RFs fs = CCoeEnv::Static()->FsSession();
     
    User::LeaveIfError(fileHandle.Open(fs, aPathParam->Value().AsDes(), fileMode));
    CleanupClosePushL(fileHandle);
     
    DRM::CDrmUtility* drmUtil = DRM::CDrmUtility::NewLC();
    iDrmProtected = drmUtil->IsProtectedL(fileHandle);
     
    CleanupStack::PopAndDestroy(2); // fileHandle, drmUtil
    
    if( !iDrmProtected )
        {
        CDRMHelper* drmHelper = CDRMHelper::NewL( *CCoeEnv::Static() );
        drmHelper->CanSetAutomated( aPathParam->Value().AsDes(), drm );
        delete drmHelper;
        }

	//Before setting image, check is the image corruputed
	RAknsSrvSession skinsrv;
	User::LeaveIfError( skinsrv.Connect() );
	CleanupClosePushL( skinsrv );
	CFbsBitmap* bmp = NULL;
	CFbsBitmap* mask = NULL;
     // no way currently of checking corrupted svg:s
     if ( ( aPathParam->Value().AsDes().Right( 4 ) ).CompareF( KAknsSkinSrvSvgFileExt ) )
    	{
		 // (-1, -1) means that image is just decoded, not used    	
   	 	TRAP( error, skinsrv.DecodeWallpaperImageL( aPathParam->Value().AsDes(), TSize(-1,-1), bmp, mask ) );
   	 	if( error )
   	 		{
   	 		//File is corrupted
   	 		ShowNoteL( error );
    		CleanupStack::PopAndDestroy( &skinsrv );
    		//Need to return KErrNone because there would happen a Leave upper in callstack. This how
    		//AIW plugin can show more accurate error note.   	 		
   	 		return KErrNone;
	  	 	}
    	}
    CleanupStack::PopAndDestroy( &skinsrv );

    CRepository* pathCen = CRepository::NewL ( KCRUidThemes ); 
    CleanupStack::PushL( pathCen );
    if ( drm )
        {
        error = pathCen->Set( KThemesCallImagePath, aPathParam->Value().AsDes() );
        // If path setting fails do not set image support on.
        if ( error == KErrNone )
            {
            error = pathCen->Set( KThemesCallImageSetting, ETrue );    
            }
        ShowNoteL( error );
        }
    else // drm check failed show error note
        {
        ShowDRMErrorNoteL();
        }
    
    CleanupStack::PopAndDestroy(); // pathCen
    
    return error;
    }


// -----------------------------------------------------------------------------
// CAiwCallImagePlugin::ShowDRMErrorNoteL
// 
// 
// -----------------------------------------------------------------------------
//
void CAiwCallImagePlugin::ShowDRMErrorNoteL()
    {
    // Resource file definition.   
    TFileName callImageRsc( KResourceDrive );
    callImageRsc.Append( KDC_RESOURCE_FILES_DIR );
    callImageRsc.Append( KCallImageResFile );

    CCoeEnv* env = CCoeEnv::Static();
    BaflUtils::NearestLanguageFile( env->FsSession(), callImageRsc );
    TInt resourceOffSet = env->AddResourceFileL( callImageRsc );
    
    CAknNoteDialog* dlg = new(ELeave) CAknNoteDialog;
    CAknNoteDialog::TTimeout timeout = CAknNoteDialog::ELongTimeout;
    dlg->SetTone( CAknNoteDialog::EErrorTone );       
    dlg->PrepareLC( R_CALL_IMAGE_DRM_NOTE );
    dlg->SetTimeout( timeout );
    dlg->RunDlgLD();
    // Free resource.
    env->DeleteResourceFile( resourceOffSet );
    }
    
// -----------------------------------------------------------------------------
// CAiwCallImagePlugin::ShowNoteL
// 
// 
// -----------------------------------------------------------------------------
//
void CAiwCallImagePlugin::ShowNoteL( TInt aError )
    {
    TInt resourceId = R_CALL_IMAGE_OK_NOTE;
    // Resource file definition.   
    TFileName callImageRsc( KResourceDrive );
    callImageRsc.Append( KDC_RESOURCE_FILES_DIR );
    callImageRsc.Append( KCallImageResFile );

    CCoeEnv* env = CCoeEnv::Static();
    BaflUtils::NearestLanguageFile( env->FsSession(), callImageRsc );
    TInt resourceOffSet = env->AddResourceFileL( callImageRsc );
    
    CAknNoteDialog* dlg = new(ELeave) CAknNoteDialog;
    CAknNoteDialog::TTimeout timeout = CAknNoteDialog::ELongTimeout;
    
    if ( aError != KErrNone )
        {
        resourceId = R_CALL_IMAGE_ERROR_NOTE;
        dlg->SetTone( CAknNoteDialog::EErrorTone );
        }         
    dlg->PrepareLC( resourceId );
    dlg->SetTimeout( timeout );
    dlg->RunDlgLD();
    // Free resource.
    env->DeleteResourceFile( resourceOffSet );
    }

// -----------------------------------------------------------------------------
// CAiwCallImagePlugin::InitializeMenuPaneL
// 
// 
// -----------------------------------------------------------------------------
//
void CAiwCallImagePlugin::InitializeMenuPaneL(
    CAiwMenuPane& aMenuPane,
    TInt aIndex,
    TInt /* aCascadeId */,
    const CAiwGenericParamList& aInParamList )
    {
    TPtrC mimeTypeString = GetAiwParamAsDescriptor(aInParamList, EGenericParamMIMEType);
    
    // Insert menu only if supported MIME type is given
    if ( ( mimeTypeString != KNullDesC ) && 
        (IsMimeTypeSupportedL(mimeTypeString) ) )   
        {
        // Resource file definition.   
        TFileName resFile( KResourceDrive );
        resFile.Append( KDC_RESOURCE_FILES_DIR );
        resFile.Append( KCallImageResFile );
                
        aMenuPane.AddMenuItemsL( 
            resFile, 
            R_CALL_IMAGE_MENU, 
            KAiwCmdAssign, 
            aIndex);
        }
    }

// -----------------------------------------------------------------------------
// CAiwCallImagePlugin::HandleMenuCmdL
// 
// 
// -----------------------------------------------------------------------------
//
void CAiwCallImagePlugin::HandleMenuCmdL(
    TInt aMenuCmdId, 
    const CAiwGenericParamList& aInParamList,
    CAiwGenericParamList& aOutParamList,
    TUint aCmdOptions,
    const MAiwNotifyCallback* aCallback )
    {
    // Cancel bit must always be checked. Support can be implemented if necessary.
    if( aCmdOptions & KAiwOptCancel )
        {
        return;
        }
        
    // Handle only call image command.
    if ( aMenuCmdId == ECallImage )
        {
        // Menu commands are handled as service commands.
        HandleServiceCmdL(
            KAiwCmdAssign,
            aInParamList,
            aOutParamList,
            aCmdOptions,
            aCallback );
        }
    }

// -----------------------------------------------------------------------------
// Implements getting a AIW parameter as descriptor.
// 
// 
// -----------------------------------------------------------------------------
//    
TPtrC CAiwCallImagePlugin::GetAiwParamAsDescriptor(
		const CAiwGenericParamList& aParamList,
		TGenericParamId aParamType)
	{
	TInt index(0);
	const TAiwGenericParam* genericParam = NULL;
    genericParam = aParamList.FindFirst( 
        index,
        aParamType,
        EVariantTypeDesC );
        
    if ( index >= 0 && genericParam )
        {
        // Get the data
        return genericParam->Value().AsDes();
        }
	else
    	{
		return KNullDesC();
    	}
	}  

// -----------------------------------------------------------------------------
// Implements checking if a given MIME type is supported or not.
// 
// 
// -----------------------------------------------------------------------------
//
TBool CAiwCallImagePlugin::IsMimeTypeSupportedL( const TDesC& aMimeTypeString )
    {   
    // Check for a type separator in the string
    TInt pos = aMimeTypeString.Find( KCallImagePluginSeparator );

    // Leave if no separator was found.. the MIME
    // standard requires it   
    if ( pos == KErrNotFound )
        {
        User::Leave( KErrArgument );
        }
    
    // Copy the full Mime type string (needed for uppercase)
    HBufC* fullBuf = aMimeTypeString.AllocLC();
    TPtr fullString = fullBuf->Des();
    fullString.UpperCase();

	// Construct the compare string    
    TPtrC compareString( aMimeTypeString.Left( pos ) );

    TBool ret(EFalse);
    
    // Perform the comparison
    // Mime type case:  IMAGE/* except IMAGE/X-OTA-BITMAP
    if ( compareString.CompareF( KCallImagePluginMimeTypeImage ) == 0 &&
        !( fullString.CompareF( KCallImagePluginMimeTypeOTABitmap ) == 0 ) )
        {
        ret = ETrue;
        }
        
	CleanupStack::PopAndDestroy( fullBuf );
    
	return ret;    
    }
    
// End of file
