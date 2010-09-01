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
* Description:  Application class.
*
*/


// INCLUDE FILES
#include    <bautils.h>
#include    <data_caging_path_literals.hrh>
#include    <eikstart.h>
#include    "phoneconstants.h"
#include    "cphoneapplication.h"
#include    "cphonedocument.h"
#include    "cphonelibrarycontainer.h"
#include    "cphonemainresourceresolver.h"
#include    "phoneconstants.h"
#include    "phonelogger.h"
#include    "cphonecenrepproxy.h"
#include    "cphonepubsubproxy.h"



// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPhoneApplication::CreateDocumentL
// Creates the document
// (other items were commented in a header).
// ---------------------------------------------------------
//
CApaDocument* CPhoneApplication::CreateDocumentL()
    {   
    return CPhoneDocument::NewL( *this ); 
    }

// ---------------------------------------------------------
// CPhoneApplication::AppDllUid
// Returns the application dll uid
// (other items were commented in a header).
// ---------------------------------------------------------
//
TUid CPhoneApplication::AppDllUid() const
    {
    return KUidPhoneApplication;
    }

// ---------------------------------------------------------
// CPhoneApplication::~CPhoneApplication()
// ---------------------------------------------------------
//
CPhoneApplication::~CPhoneApplication()
    {
    // Delete library handle container ( it unloads all factory DLLs )
    delete iLibraryContainer;
    }

// ================= OTHER EXPORTED FUNCTIONS ==============
//
// ---------------------------------------------------------
// NewApplication implements
// 
// Creates an instance of application.
//
// Returns: an instance of CPAApplication
// ---------------------------------------------------------
//
EXPORT_C CApaApplication* NewApplication()
    {
    return new CPhoneApplication;
    }

// ---------------------------------------------------------
// CPhoneApplication::ResourceFileName()
// Called by framework before creating a document. 
// ---------------------------------------------------------
//
TFileName CPhoneApplication::ResourceFileName() const
    {
    // Create path to default resource file name
    TFileName path( KDriveZ );
    path.Append( KDC_APP_RESOURCE_DIR );
    path.Append( KPhoneResourceFile );

    // Get correct resourse file among current language
    RFs &fsSession=iCoeEnv->FsSession();
    BaflUtils::NearestLanguageFile( fsSession, path ); 
    
    return path;
    }
// ---------------------------------------------------------
// CPhoneApplication::PreDocConstructL()
// Called by framework before creating a document. 
// ---------------------------------------------------------
//
void CPhoneApplication::PreDocConstructL()
    {   
    // Create container for factory DLLs. It is deleted as last thing when
    // application exists, in CPhoneApplication destructor. This ensures that DLLs,
    // which contain the implementation for the objects that are created in factory,
    // are usable as long as needed.
    iLibraryContainer = CPhoneLibraryContainer::NewL();

    // Create logger singleton instance.
    // We do not need pointer to it in here and class is deleted 
    // somewhere in coeenv.
    new (ELeave) CPhoneLogger(KUidPhoneUILoggerSingleton);

    // Create Central Repository Proxy
    CPhoneCenRepProxy::Instance();

    // Create Publish & Subscribe Proxy
    CPhonePubSubProxy::Instance();

    // Create main resource resolver
    CPhoneMainResourceResolver::Instance();

    CAknApplication::PreDocConstructL();
    }

// -----------------------------------------------------------------------------
// E32Main
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }

//  End of File  
