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

#include "ut_phonecallutil.h"
#include "phonecallutil.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

#include "CPhoneStateMachineGSM.h"
#include "mock_cphoneviewcommandhandle.h"
#include "CPEEngineInfo.h"
#include "cphonestateincall.h"
#include "cphonestatehandle.h"
#include "CPhoneCenRepProxy.h"
#include "CPhoneMainResourceResolver.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include "phoneui.hrh"
#include <csmcmockcontext.h>
#include <msmcmockspecbuilder.h>
#include <coemain.h>
#include <PhoneUI.rsg>
#include "phoneresourceids.h"
#include "phonerssbase.h"
#include "CPhoneTerminateAllConnectionsCommand.h"

// needed for mocking CPhoneMainResourceResolver::Instance()
class CPhoneMainResourceResolverMock : public CPhoneMainResourceResolver{};

class CPhoneStateMachineGSMMock : public CPhoneStateMachineGSM{};


        

// - Construction -----------------------------------------------------------

ut_phonecallutil* ut_phonecallutil::NewL()
    {
    ut_phonecallutil* self = ut_phonecallutil::NewLC();
    CleanupStack::Pop();
    return self;
    }

ut_phonecallutil* ut_phonecallutil::NewLC()
    {
    ut_phonecallutil* self = new( ELeave ) ut_phonecallutil();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

ut_phonecallutil::~ut_phonecallutil()
    {
    CSmcMockContext::Release();
    }

ut_phonecallutil::ut_phonecallutil()
    {
    }

void ut_phonecallutil::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    iMockContext = CSmcMockContext::InstanceL();
    }

// - Test methods -----------------------------------------------------------

void ut_phonecallutil::SetupL(  )
    {         
    iCPhoneMainResourceResolverMock = new ( ELeave ) CPhoneMainResourceResolverMock();
    SmcDefaultValue<CPhoneMainResourceResolver*>::SetL( iCPhoneMainResourceResolverMock );
    
    iCoeEnvMock = new (ELeave ) CCoeEnv();
    SmcDefaultValue<CCoeEnv*>::SetL( iCoeEnvMock );
    
    iViewCommandHandle = new CPhoneViewCommandHandleMock();
    SmcDefaultValue<MPhoneViewCommandHandle*>::SetL( iViewCommandHandle );
    
    iStateMachine = CPhoneStateMachineGSMMock::NewL(iViewCommandHandle);
    SmcDefaultValue<MPhoneStateMachine*>::SetL( iStateMachine );
    
    iCPhoneStateHandleMock = CPhoneStateHandle::
            CreateL(iViewCommandHandle, _L("test"), TUid::Uid(1));
    SmcDefaultValue<CPhoneStateHandle*>::SetL( iCPhoneStateHandleMock );
    
    iEngineInfo = CPEEngineInfo::NewL();
    SmcDefaultValue<MPEEngineInfo*>::SetL( iEngineInfo );    
    }

void ut_phonecallutil::Teardown(  )
    {
    delete iStateMachine;
    iStateMachine = NULL;
    SmcDefaultValue<MPhoneStateMachine*>::Reset();
    
    delete iCPhoneStateHandleMock;
    iCPhoneStateHandleMock = NULL;
    SmcDefaultValue<CPhoneStateHandle*>::Reset();
    
    delete iViewCommandHandle;
    iViewCommandHandle = NULL;
    SmcDefaultValue<MPhoneViewCommandHandle*>::Reset();
    
    delete iEngineInfo;
    iEngineInfo = NULL;
    SmcDefaultValue<MPEEngineInfo*>::Reset();
    
    delete iCoeEnvMock;
    iCoeEnvMock = NULL;
    SmcDefaultValue<CCoeEnv*>::Reset();
    
    delete iCPhoneMainResourceResolverMock;
    iCPhoneMainResourceResolverMock = NULL;
    SmcDefaultValue<CPhoneMainResourceResolver*>::Reset();
    
    iMockContext->Reset();
    }
    
void ut_phonecallutil::T_ConstructionL(  )
    {
    // SetupL and Teardown does the test.
    }

void ut_phonecallutil::T_DialMultimediaCall(  )
    {
    iMockContext->ExpectCallL( "CPhoneStateMachine::SendPhoneEngineMessage" ).
                  WithL(MPEPhoneModel::EPEMessageDial);
    
    PhoneCallUtil::DialMultimediaCall();
    
    EUNIT_ASSERT_EQUALS( iEngineInfo->CallTypeCommand(), EPECallTypeVideo );
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    }

void ut_phonecallutil::T_DialVoiceCall(  )
    {   
    iMockContext->ExpectCallL( "CPhoneStateMachine::SendPhoneEngineMessage" ).
                  WithL(MPEPhoneModel::EPEMessageDial);
    
    PhoneCallUtil::DialVoiceCallL();
    
    EUNIT_ASSERT_EQUALS( iEngineInfo->CallTypeCommand(), EPECallTypeCSVoice );
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    }

void ut_phonecallutil::T_DisconnectCallL(  )
    {   
    iMockContext->ExpectCallL( "CPhoneStateMachine::SendPhoneEngineMessage" ).
                  WithL(MPEPhoneModel::EPEMessageRelease);
    
    PhoneCallUtil::DisconnectCallL();
    
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    }

void ut_phonecallutil::T_TestUtilsL(  )
    {
    TInt callId = 1;
    TInt invalidCallId = -1;

    iEngineInfo->SetCallType(EPECallTypeCSVoice, callId);
    EUNIT_ASSERT( EFalse == PhoneCallUtil::IsVideoCall( callId ) );
    EUNIT_ASSERT( EFalse == PhoneCallUtil::IsVideoCall( invalidCallId ) );
    
    iEngineInfo->SetCallType(EPECallTypeVideo, callId);
    EUNIT_ASSERT( PhoneCallUtil::IsVideoCall( callId ) );
    
    EUNIT_ASSERT( -1 == PhoneCallUtil::CallIdByState( EPEStateHeld ) );
    EUNIT_ASSERT( EFalse == PhoneCallUtil::CheckIfCallExists(
            EPEStateHeld, EPECallTypeCSVoice) );
    
    EUNIT_ASSERT( EFalse == PhoneCallUtil::IsAnyConnectedCalls() );
    iEngineInfo->SetCallState(EPEStateConnected, callId);
    EUNIT_ASSERT( PhoneCallUtil::IsAnyConnectedCalls() );
    iEngineInfo->SetCallState(EPEStateConnecting, callId);
    EUNIT_ASSERT( PhoneCallUtil::IsAnyConnectedCalls() );
    iEngineInfo->SetCallState(EPEStateConnectedConference, callId);
    EUNIT_ASSERT( PhoneCallUtil::IsAnyConnectedCalls() );
    }

// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    ut_phonecallutil,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "Test0",
    "T_ConstructionL",
    "Test0",
    "FUNCTIONALITY",
    SetupL, T_ConstructionL, Teardown)

EUNIT_TEST(
    "Test1",
    "T_DialMultimediaCall",
    "Test1",
    "FUNCTIONALITY",
    SetupL, T_DialMultimediaCall, Teardown)

EUNIT_TEST(
    "Test2",
    "T_DialVoiceCall",
    "Test2",
    "FUNCTIONALITY",
    SetupL, T_DialVoiceCall, Teardown)
    
EUNIT_TEST(
    "Test3",
    "T_DisconnectCallL",
    "Test3",
    "FUNCTIONALITY",
    SetupL, T_DisconnectCallL, Teardown)
    
EUNIT_TEST(
    "Test4",
    "T_TestUtilsL",
    "Test4",
    "FUNCTIONALITY",
    SetupL, T_TestUtilsL, Teardown)
    
EUNIT_END_TEST_TABLE
