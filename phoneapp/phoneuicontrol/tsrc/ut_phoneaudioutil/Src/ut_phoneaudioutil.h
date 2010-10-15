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

#ifndef __UT_PHONEAUDIOUTIL_H__
#define __UT_PHONEAUDIOUTIL_H__

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
NONSHARABLE_CLASS( ut_phoneaudioutil )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static ut_phoneaudioutil* NewL();
        static ut_phoneaudioutil* NewLC();
        ~ut_phoneaudioutil();

    private: // Constructors

        ut_phoneaudioutil();
        void ConstructL();

    private: // New methods

         void SetupL();
         void Teardown();
        
         void T_ConstructionL();
         void T_HandleAudioMuteChangedL();
         void T_HandleAudioOutputChangedL();
         void T_ChangeAudioVolumeL();
         void T_HandleAudioAvailableOutputChangedL();
         void T_ToggleMute();
         void T_SetHandsfreeModeL();
        

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

#endif      //  __UT_PHONEAUDIOUTIL_H__
