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
* Description:  Implementation of CPslnCallImagePluginDRM class.
*
*/


// INCLUDE FILES
#include    "PslnCallImagePluginDRM.h"

#include    <DRMHelper.h>
#include    <coemain.h>
#include    <aknnotewrappers.h>
#include    <Psln.rsg>
#include    <drmutility.h>

// Logging
#include    "PslnCallImagePluginLogger.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCPslnCallImagePluginDRM::CPslnCallImagePluginDRM
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CPslnCallImagePluginDRM::CPslnCallImagePluginDRM()
    {
    }

// -----------------------------------------------------------------------------
// CPslnCallImagePluginDRM::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPslnCallImagePluginDRM::ConstructL()
    {
    __CALLLOGSTRING("CPslnCallImagePluginDRM::ConstructL");
    iDRMHelper = CDRMHelper::NewL( *CCoeEnv::Static() );
    }

// -----------------------------------------------------------------------------
// CPslnCallImagePluginDRM::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPslnCallImagePluginDRM* CPslnCallImagePluginDRM::NewL()
    {
    CPslnCallImagePluginDRM* self = new( ELeave ) CPslnCallImagePluginDRM();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CPslnCallImagePluginDRM::~CPslnCallImagePluginDRM
// Destructor.
// -----------------------------------------------------------------------------
//
CPslnCallImagePluginDRM::~CPslnCallImagePluginDRM()
    {
    __CALLLOGSTRING("CPslnCallImagePluginDRM::~CPslnCallImagePluginDRM");
    delete iDRMHelper;
    iDRMHelper = NULL;
    }

// -----------------------------------------------------------------------------
// CPslnCallImagePluginDRM::IsFileValidL
// Empty implementation.
// -----------------------------------------------------------------------------
//
TBool CPslnCallImagePluginDRM::VerifySelectionL(const MDesCArray* aSelectedFiles)
    {
    __CALLLOGSTRING("CPslnCallImagePluginDRM::VerifySelectionL START");

    TBool canbeautomated( EFalse );
    TBool iDrmProtected( EFalse );
    TInt res( KErrNone );
    RFile fileHandle;
    TUint fileMode = EFileShareReadersOrWriters | EFileStream | EFileRead;
    RFs fs = CCoeEnv::Static()->FsSession();
    
    if( aSelectedFiles->MdcaCount()<1 )
        {
        __CALLLOGSTRING("CPslnCallImagePluginDRM::VerifySelectionL zero items");
        __CALLLOGSTRING("CPslnCallImagePluginDRM::VerifySelectionL RETURN TRUE");
        // Download item
        return ETrue;
        }
 
    User::LeaveIfError(fileHandle.Open(fs, aSelectedFiles->MdcaPoint(0), fileMode));
    CleanupClosePushL(fileHandle);
     
    DRM::CDrmUtility* drmUtil = DRM::CDrmUtility::NewLC();
    iDrmProtected = drmUtil->IsProtectedL(fileHandle);
     
    CleanupStack::PopAndDestroy(2); // fileHandle, drmUtil
    
    if( !iDrmProtected )
        {
        res = iDRMHelper->SetAutomatedType( CDRMHelper::EAutomatedTypeWallpaper );
        __CALLLOGSTRING1("CPslnCallImagePluginDRM::VerifySelectionL SetAutomatedType called, result=%d", res );
        if ( res == KErrNone )
            {        
            res = iDRMHelper->CanSetAutomated(
                aSelectedFiles->MdcaPoint(0), 
                canbeautomated );
            }
        }
        
    __CALLLOGSTRING1("CPslnCallImagePluginDRM::VerifySelectionL Automated called, value=%d", canbeautomated);
    __CALLLOGSTRING1("CPslnCallImagePluginDRM::VerifySelectionL Automated called, result=%d", res);
    if (!res && !canbeautomated)
        {
        ShowErrorNoteL( R_PSLN_QTN_DRM_PREV_RIGHTS_SET );
        }
    else if (res || !canbeautomated)
        {
        iDRMHelper->HandleErrorL(res, aSelectedFiles->MdcaPoint(0) );
        }
    
    __CALLLOGSTRING("CPslnCallImagePluginDRM::VerifySelectionL RETURNING");
    return canbeautomated;
    }
    
// -----------------------------------------------------------------------------
// CPslnCallImagePluginDRM::ShowErrorNoteL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPslnCallImagePluginDRM::ShowErrorNoteL( TInt  aResourceId  ) const
    {
    HBufC* errorText = 
        CCoeEnv::Static()->AllocReadResourceLC( aResourceId );
    CAknInformationNote* note = new( ELeave ) CAknInformationNote( EFalse );
    note->ExecuteLD( *errorText );
    CleanupStack::PopAndDestroy( errorText );
    }

// End of File
