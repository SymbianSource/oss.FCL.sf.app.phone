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
* Description:  Implementation of CT_VideoDtmfDialer class. API testing.
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include "T_VideoDtmfDialer.h"
#include <SettingServerClient.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::CT_VideoDtmfDialer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CT_VideoDtmfDialer::CT_VideoDtmfDialer( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CT_VideoDtmfDialer::ConstructL()
    {
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
    RSettingServer settingServer;
    TInt ret = settingServer.Connect();
    if(ret != KErrNone)
        {
        User::Leave(ret);
        }
    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings; 
    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);
    if(ret != KErrNone)
        {
        User::Leave(ret);
        } 
    // Close Setting server session
    settingServer.Close();

    TFileName logFileName;
    
    if(loggerSettings.iAddTestCaseTitle)
        {
        TName title;
        TestModuleIf().GetTestCaseTitleL(title);
        logFileName.Format(KT_VideoDtmfDialerLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KT_VideoDtmfDialerLogFile);
        }

    iLog = CStifLogger::NewL( KT_VideoDtmfDialerLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );

    }

// -----------------------------------------------------------------------------
// CT_VideoDtmfDialer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CT_VideoDtmfDialer* CT_VideoDtmfDialer::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CT_VideoDtmfDialer* self = new (ELeave) CT_VideoDtmfDialer( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// Destructor
CT_VideoDtmfDialer::~CT_VideoDtmfDialer()
    { 
    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {
    return ( CScriptBase* ) CT_VideoDtmfDialer::NewL( aTestModuleIf );
    }


//  End of File
