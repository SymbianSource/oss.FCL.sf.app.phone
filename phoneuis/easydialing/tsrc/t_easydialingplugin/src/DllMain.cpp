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
* Description:  EUnit test file.
*
*/
//  EXTERNAL INCLUDES
#include <CEUnitTestSuite.h>
#include "CEasyDialingPlugin_Test.h"

/**
* T_HelloWorld.dll test suite factory function.
*/
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEasyDialingPlugin_Test* rootSuite = CEasyDialingPlugin_Test::NewL();
    
    if (rootSuite == NULL)
        {
        User::Panic(_L("Koe"), 3);
        }

    return rootSuite;
    }
