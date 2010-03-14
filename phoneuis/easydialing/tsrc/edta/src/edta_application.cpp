/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Easy dialing test application.
*
*/

// INCLUDE FILES
#include "edta_document.h"
#include "edta_application.h"

// ============================ MEMBER FUNCTIONS ===============================

// UID for the application.
// this should correspond to the uid defined in the mmp file
const TUid KUidEdtaApp = { 0x2000FDDC };

// -----------------------------------------------------------------------------
// CEdtaApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CEdtaApplication::CreateDocumentL()
    {
    // Create an Edta document, and return a pointer to it
    return (static_cast<CApaDocument*>
                    ( CEdtaDocument::NewL( *this ) ) );
    }

// -----------------------------------------------------------------------------
// CEdtaApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CEdtaApplication::AppDllUid() const
    {
    // Return the UID for the Edta application
    return KUidEdtaApp;
    }

// End of File

