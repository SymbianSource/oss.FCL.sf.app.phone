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

#ifndef __UT_CPHONECALLUTIL_H__
#define __UT_CPHONECALLUTIL_H__

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

// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( ut_phonecallutil )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static ut_phonecallutil* NewL();
        static ut_phonecallutil* NewLC();
        ~ut_phonecallutil();

    private: // Constructors

        ut_phonecallutil();
        void ConstructL();

    private: // New methods

         void SetupL();
         void Teardown();
        
         void T_ConstructionL();
         void T_DialMultimediaCall();
         void T_DialVoiceCall();
         void T_DisconnectCallL();
         void T_TestUtilsL();
        

    private: // Data

        CPhoneViewCommandHandleMock* iViewCommandHandle;
        CPhoneStateHandle* iCPhoneStateHandleMock;
        CCoeEnv* iCoeEnvMock;
        CPhoneStateMachineGSM* iStateMachine;
        CPEEngineInfo* iEngineInfo;
        CSmcMockContext* iMockContext;
        CPhoneMainResourceResolverMock* iCPhoneMainResourceResolverMock;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CPHONECALLUTIL_H__
