/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ECOM proxy table for gsvmbxplugin.
*
*/


// User includes
#include "gsvmbxplugin.h"

// System includes
#include <e32std.h>
#include <implementationproxy.h>


// Constants
const TImplementationProxy KGSVmbxPluginImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x2000B4D7, CGSVmbxPlugin::NewL )
    };


// ---------------------------------------------------------------------------
// ImplementationGroupProxy
// Gate/factory function
//
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount )
    {    
    aTableCount = sizeof( KGSVmbxPluginImplementationTable )
        / sizeof( TImplementationProxy );
    return KGSVmbxPluginImplementationTable;
    }


// End of File
