/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declares main application class.
*
*/


// INCLUDE FILES
#include    "UssdApp.h"
#include    "UssdDocument.h"
#include 	<eikstart.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUssdApp::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CUssdApp::AppDllUid() const
    {
    return KUidussd;
    }


// -----------------------------------------------------------------------------
// CUssdApp::CreateDocumentL()
// Creates CUssdDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CUssdApp::CreateDocumentL()
    {
    return CUssdDocument::NewL(*this);
    }


// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// NewApplication() 
// Constructs CUssdApp
// Returns: CApaDocument*: created application object
// -----------------------------------------------------------------------------
//
LOCAL_C CApaApplication* NewApplication()
    {
    return new CUssdApp;
    }

// -----------------------------------------------------------------------------
// E32Main implements
// 
// It is called when executable is started. 
//
// Returns: error code.
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }

// End of File
