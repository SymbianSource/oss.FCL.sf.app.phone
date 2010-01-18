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
* Description:  Document class implementation.
*
*/


// INCLUDE FILES
#include <apgwgnam.h>
#include "cphonedocument.h"
#include "cphoneappui.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneDocument::CPhoneDocument( CEikApplication& aApp ) :
    CAknDocument( aApp )
    {
    }

// ---------------------------------------------------------
// CPhoneDocument::NewL
// ---------------------------------------------------------
//
CPhoneDocument* CPhoneDocument::NewL( CEikApplication& aApp )
    {
    CPhoneDocument* self = new (ELeave) CPhoneDocument( aApp );
    
    return self;
    }

// Destructor
CPhoneDocument::~CPhoneDocument()
    {
    }

// ---------------------------------------------------------
// CPhoneDocument::CreateAppUiL
//
// Creates an instance of CPhoneAppUi and returns it.
// ---------------------------------------------------------
//
CEikAppUi* CPhoneDocument::CreateAppUiL()
    {
    return new (ELeave) CPhoneAppUI;
    }

// ---------------------------------------------------------
// CPhoneDocument::UpdateTaskNameL
// ---------------------------------------------------------
//
void CPhoneDocument::UpdateTaskNameL( CApaWindowGroupName* aWgName )
    {
    CAknDocument::UpdateTaskNameL( aWgName );
    aWgName->SetHidden( ETrue );
    }

//  End of File  
