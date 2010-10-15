/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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
#include <hbstyleloader.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbStyleLoader::registerFilePath
// -----------------------------------------------------------------------------
//
bool HbStyleLoader::registerFilePath( 
        const QString & filePath )
    {
    SMC_MOCK_METHOD1( bool, const QString &, filePath )
    }


// -----------------------------------------------------------------------------
// HbStyleLoader::unregisterFilePath
// -----------------------------------------------------------------------------
//
bool HbStyleLoader::unregisterFilePath( 
        const QString & filePath )
    {
    SMC_MOCK_METHOD1( bool, const QString &, filePath )
    }


