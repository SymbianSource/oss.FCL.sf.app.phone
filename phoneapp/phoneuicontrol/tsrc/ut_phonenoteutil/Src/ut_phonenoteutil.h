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

#ifndef __UT_PHONENOTEUTIL_H__
#define __UT_PHONENOTEUTIL_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>



// FORWARD DECLARATIONS
class CPhoneViewCommandHandleMock;
class CCoeEnv;
class CPhoneStateMachineGSM;
class CPEEngineInfo;
class CPhoneMainResourceResolverMock;
class CSmcMockContext;
class CPhoneStateHandle;
class CPhoneMainErrorMessagesHandler;

// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( ut_phonenoteutil )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static ut_phonenoteutil* NewL();
        static ut_phonenoteutil* NewLC();
        ~ut_phonenoteutil();

    private: // Constructors

        ut_phonenoteutil();
        void ConstructL();

    private: // New methods

         void SetupL();
         void Teardown();
        
         void T_ConstructionL();
         void T_SendGlobalInfoNoteL();
         void T_SendGlobalWarningNoteL();
         void T_SendGlobalErrorNoteL();
         void T_SendGlobalNoteL2();
         void T_ShowNoteAndQueryL();
         void T_HandleErrorL();
         void T_HandleRemoteBusyNoteL();
        

    private: // Data

        CPhoneViewCommandHandleMock* iViewCommandHandle;
        CPhoneStateHandle* iCPhoneStateHandleMock;
        CCoeEnv* iCoeEnvMock;
        CPhoneStateMachineGSM* iStateMachine;
        CPEEngineInfo* iEngineInfo;
        CSmcMockContext* iMockContext;
        CPhoneMainResourceResolverMock* iCPhoneMainResourceResolverMock;
        CPhoneMainErrorMessagesHandler* iCPhoneMainErrorMessagesHandlerMock;
        
        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_PHONENOTEUTIL_H__
