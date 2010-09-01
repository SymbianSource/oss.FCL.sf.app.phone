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
/*
-----------------------------------------------------------------------------
This file has been generated with EUnit Pro
http://www.digia.com/eunit
-----------------------------------------------------------------------------
*/
#ifndef __CEASYDIALINGPLUGIN_TEST_H__
#define __CEASYDIALINGPLUGIN_TEST_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include "easydialingplugin.h"

// FORWARD DECLARATIONS

class CDummyParentControl;

/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( CEasyDialingPlugin_Test ) : public CEUnitTestSuiteClass, public MDialingExtensionObserver
    {
    public:  // Constructors and destructor

        static CEasyDialingPlugin_Test* NewL();
        static CEasyDialingPlugin_Test* NewLC();
        ~CEasyDialingPlugin_Test();
        
        void HandleDialingExtensionEvent( MDialingExtensionObserver::TEvent aEvent );

    private: // Constructors

        CEasyDialingPlugin_Test();
        void ConstructL();

    private: // New methods

         void SetupL();
        
         void Teardown();
        
         void T_Global_OfferKeyEventLL();
        
         void T_Global_Search1();
         void T_Global_Search2();
         void T_Global_Search3();
         void T_Global_Search4(); 
         void T_Global_Search5(); 
         void T_Global_Search6(); 
         void T_Global_Search7(); 
         void T_Global_Search8();
         
         void T_Global__CEasyDialingPluginL();

    private: // Data

        CEasyDialingPlugin* iCEasyDialingPlugin;
        
        CActiveSchedulerWait* iWait;
        
        CDummyParentControl* iDummyParentControl;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __CEASYDIALINGPLUGIN_TEST_H__
