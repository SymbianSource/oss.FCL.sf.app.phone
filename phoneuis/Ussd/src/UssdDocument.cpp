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
* Description:  USSD application document functionality. 
*
*/


// INCLUDE FILES
#include "UssdDocument.h"
#include "UssdAppUi.h"

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CUssdDocument::CUssdDocument
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUssdDocument::CUssdDocument( CEikApplication& aApp )
    : CAknDocument( aApp )
    {
    }


// -----------------------------------------------------------------------------
// CUssdDocument::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUssdDocument* CUssdDocument::NewL(
    CEikApplication& aApp )     // CUssdApp reference
    {
    CUssdDocument* self = new ( ELeave ) CUssdDocument(aApp);
    return self;
    }

    
// Destructor
CUssdDocument::~CUssdDocument()
    {
    }


// -----------------------------------------------------------------------------
// CUssdDocument::CreateAppUiL()
// constructs CUssdAppUi
//
// -----------------------------------------------------------------------------
CEikAppUi* CUssdDocument::CreateAppUiL()
    {
    return new(ELeave) CUssdAppUi;
    }


// End of File  
