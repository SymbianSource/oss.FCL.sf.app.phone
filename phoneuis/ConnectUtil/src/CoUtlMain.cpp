/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Global functions for Connection utility.
*
*/



// INCLUDE FILES
#include    <ecom.h>
#include    <implementationproxy.h>
#include    <CoUtlInterface.hrh>
#include    "CCoUtlImplementation.h"

// CONSTANTS

// Holds implementation table.
const TImplementationProxy KCoUtlImplementationTable[] =
    {
        IMPLEMENTATION_PROXY_ENTRY(
             KCoUtlImplementationUid, 
            CCoUtlImplementation::NewL )
    };

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// Returns implementation table and updates aTableCount parameter to hold
// amount of elements in table.
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = 
        sizeof( KCoUtlImplementationTable ) / 
        sizeof( TImplementationProxy );

    return KCoUtlImplementationTable;
    }

//  End of File  
