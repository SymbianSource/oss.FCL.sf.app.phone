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
* Description:  Voice mailbox application.
*
*/


// INCLUDE FILES
#include "VmApp.h"
#include "VmDocument.h"
#include <eikstart.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVmApp::AppDllUid
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CVmApp::AppDllUid() const
    {
    return KUidvm;
    }

// -----------------------------------------------------------------------------
// CVmApp::CreateDocumentL
// Creates CVmDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CVmApp::CreateDocumentL()
    {
    return CVmDocument::NewL( *this );
    }

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// NewApplication
// Constructs CVmApp
// Returns: CApaDocument*: created application object
// -----------------------------------------------------------------------------
//
LOCAL_C CApaApplication* NewApplication()
    {
    return new CVmApp;
    }

// -----------------------------------------------------------------------------
// E32Main implements
// It is called when executable is started. 
// Returns: error code.
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }



// End of File  
