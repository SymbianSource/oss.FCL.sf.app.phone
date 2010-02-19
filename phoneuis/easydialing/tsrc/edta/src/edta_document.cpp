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
#include "edta_appui.h"
#include "edta_document.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CEdtaDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CEdtaDocument* CEdtaDocument::NewL( CEikApplication& aApp )
    {
    CEdtaDocument* self = NewLC( aApp );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CEdtaDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CEdtaDocument* CEdtaDocument::NewLC( CEikApplication& aApp )
    {
    CEdtaDocument* self = new ( ELeave ) CEdtaDocument( aApp );

    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CEdtaDocument::CEdtaDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CEdtaDocument::CEdtaDocument( CEikApplication& aApp )
    : CAknDocument( aApp )
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CEdtaDocument::~CEdtaDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CEdtaDocument::~CEdtaDocument()
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CEdtaDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CEdtaDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it.
    // The framework takes ownership of this object.
    return ( static_cast <CEikAppUi*> ( new ( ELeave ) CEdtaAppUi ) );
    }

// End of File

