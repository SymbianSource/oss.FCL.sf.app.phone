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
* Description:  Easy dialing DLL main.
*
*/


// INCLUDE FILES

#include <e32std.h>
#include <implementationproxy.h>
#include "easydialingplugin.h"
#include "easydialingconstants.hrh"


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

const TImplementationProxy ImplementationTable[] =
    {
#ifdef __EABI__ 
    IMPLEMENTATION_PROXY_ENTRY( EASYDIALING_PLUGIN_ECOM_IMPL_UID, CEasyDialingPlugin::NewL ) 
#else
    {{EASYDIALING_PLUGIN_ECOM_IMPL_UID},  CEasyDialingPlugin::NewL }
#endif
    };

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ---------------------------------------------------------
// ImplementationGroupProxy
//
// Provides a key value pair table, this is used to identify
// Function used to return an instance of the proxy table.
// the correct construction function for the requested interface.
// ---------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy (TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

// ---------------------------------------------------------
// E32Dll
//
// Standard Symbian OS DLL entry point.
// ---------------------------------------------------------
//
#ifndef EKA2
TBool E32Dll(TDllReason)
    {
    return ETrue;
    }
#endif // EKA2
