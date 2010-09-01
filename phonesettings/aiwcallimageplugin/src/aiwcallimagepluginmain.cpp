/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Global functions for aiwcallimageplugin.
*
*/



// INCLUDE FILES
#include    <ecom.h>
#include    <implementationproxy.h>

#include    "aiwcallimageplugin.hrh"
#include    "aiwcallimageplugin.h"


// CONSTANTS

// Holds implementation table.
const TImplementationProxy KAiwCallImagePluginImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( 
        KAiwCallImagePluginImplementationUid, 
        CAiwCallImagePlugin::NewL )
    };

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// 
// Returns implementation table and updates aTableCount parameter to hold
// amount of elements in table.
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( 
    TInt& aTableCount )
    {
    aTableCount = 
        sizeof( KAiwCallImagePluginImplementationTable ) / 
        sizeof( TImplementationProxy );

    return KAiwCallImagePluginImplementationTable;
    }

//  End of File  
