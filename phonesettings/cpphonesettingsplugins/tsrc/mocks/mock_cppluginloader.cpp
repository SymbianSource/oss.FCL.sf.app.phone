/* 
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#include <QDebug>
#include <smcmockclassincludes.h>
#include "cppluginloader.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CpPluginLoader::loadCpPlugin
// -----------------------------------------------------------------------------
//
CpPluginInterface * CpPluginLoader::loadCpPlugin( 
        const QString & pluginFile )
    {
    SMC_MOCK_METHOD1( CpPluginInterface *, const QString &, pluginFile )
    }


// -----------------------------------------------------------------------------
// CpPluginLoader::loadPlatCpPlugin
// -----------------------------------------------------------------------------
//
CpPluginPlatInterface * CpPluginLoader::loadPlatCpPlugin( 
        const QString & pluginFile )
    {
    SMC_MOCK_METHOD1( CpPluginPlatInterface *, const QString &, pluginFile )
    }


