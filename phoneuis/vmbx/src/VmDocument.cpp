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
* Description:  Document for Voice mailbox application.
*
*/


// INCLUDE FILES
#include "VmDocument.h"
#include "VmAppUi.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CVmDocument::~CVmDocument
// Destructor
// -----------------------------------------------------------------------------
//
CVmDocument::~CVmDocument()
    {
    }

// ----------------------------------------------------------------------------
// CVmDocument::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------    
//
CVmDocument* CVmDocument::NewL( CEikApplication& aApp )  // CVmApp reference
    {
    return new(ELeave) CVmDocument( aApp );
    }
 

// -----------------------------------------------------------------------------
// CVmDocument::CreateAppUiL
// constructs CVmAppUi
// -----------------------------------------------------------------------------
//
CEikAppUi* CVmDocument::CreateAppUiL()
    {
    return new(ELeave) CVmAppUi;
    }

// End of File
