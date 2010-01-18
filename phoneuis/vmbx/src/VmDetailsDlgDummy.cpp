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
* Description:  Dummy details dialog for global implementation
*
*/


// INCLUDE FILES
#include "VmDetailsDlgDummy.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVmDetailsDialog::CVmDetailsDialog
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVmDetailsDialog::CVmDetailsDialog()
    {
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVmDetailsDialog* CVmDetailsDialog::NewL()
    {
    CVmDetailsDialog* self = new( ELeave ) CVmDetailsDialog;
    return self;
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::~CVmDetailsDialog
// Destructor
// -----------------------------------------------------------------------------
//
CVmDetailsDialog::~CVmDetailsDialog()
    {
    }

// -----------------------------------------------------------------------------
// CVmDetailsDialog::ExecuteLD
// Dummy function
// -----------------------------------------------------------------------------
//
TInt CVmDetailsDialog::ExecuteLD( TInt /* aResourceId */ )
    {
    delete this;
    return 0;
    }

//  End of File
