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

#include "ut_phoneaudioutil.h"
#include "phoneaudioutil.h"
#include "phonenoteutil.h"
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

ut_phoneaudioutil* ut_phoneaudioutil::NewL()
    {
    ut_phoneaudioutil* self = ut_phoneaudioutil::NewLC();
    CleanupStack::Pop();
    return self;
    }

ut_phoneaudioutil* ut_phoneaudioutil::NewLC()
    {
    ut_phoneaudioutil* self = new( ELeave ) ut_phoneaudioutil();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

ut_phoneaudioutil::~ut_phoneaudioutil()
    {
    CSmcMockContext::Release();
    }

ut_phoneaudioutil::ut_phoneaudioutil()
    {
    }

void ut_phoneaudioutil::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    iMockContext = CSmcMockContext::InstanceL();
    }

// - Test methods -----------------------------------------------------------

void ut_phoneaudioutil::SetupL(  )
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

void ut_phoneaudioutil::Teardown(  )
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
    
void ut_phoneaudioutil::T_ConstructionL(  )
    {
    // SetupL and Teardown does the test.
    }

void ut_phoneaudioutil::T_HandleAudioMuteChangedL(  )
    {
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" );
    
    PhoneAudioUtil::HandleAudioMuteChangedL();
    
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    }

void ut_phoneaudioutil::T_HandleAudioOutputChangedL(  )
    {
    PhoneAudioUtil::HandleAudioOutputChangedL();
    
    iEngineInfo->SetAudioOutput(EPEBTAudioAccessory, EPEHandset, EFalse);
    RArray<TPEAudioOutput> availableOutputs;
    availableOutputs.Append(EPEBTAudioAccessory);
    availableOutputs.Append(EPEHandset);
    availableOutputs.Append(EPELoudspeaker);
    iEngineInfo->SetAvailableAudioOutputs(availableOutputs.Array());
    
    iMockContext->ExpectCallL( "CPhoneAccessoryBTHandler::ShowBTActivatedL" );
    PhoneAudioUtil::HandleAudioOutputChangedL();
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    iEngineInfo->SetAudioOutput(EPENotActive, EPEBTAudioAccessory, EFalse);
    iMockContext->ExpectCallL( "CPhoneBtaaDisconnectHandler::IsQuery" ).ReturnsL(ETrue);
    iMockContext->ExpectCallL( "CPhoneBtaaDisconnectHandler::Cancel" );
    PhoneAudioUtil::HandleAudioOutputChangedL();
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    iEngineInfo->SetAudioOutput(EPELoudspeaker, EPEHandset, EFalse);
    iMockContext->ExpectCallL( 
            "CPhoneViewCommandHandleMock::ExecuteCommandL" ).WithL(EPhoneViewPlayHandsFreeActivatedTone);
    PhoneAudioUtil::HandleAudioOutputChangedL();
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    
    availableOutputs.Close();
    }

void ut_phoneaudioutil::T_ChangeAudioVolumeL(  )
    {   
    iEngineInfo->SetAudioOutput(EPETTY, EPENotActive, EFalse);
    iMockContext->ExpectCallL( "PhoneNoteUtil::SendGlobalInfoNoteL" );
    PhoneAudioUtil::DecreaseAudioVolumeL();
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    iMockContext->ExpectCallL( "PhoneNoteUtil::SendGlobalInfoNoteL" );
    PhoneAudioUtil::IncreaseAudioVolumeL();
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    iEngineInfo->SetAudioOutput(EPEHandset, EPENotActive, EFalse);
    
    iMockContext->ExpectCallL( "CPhoneStateMachine::SendPhoneEngineMessage" ).
            WithL(MPEPhoneModel::EPEMessageSetAudioVolume);
    PhoneAudioUtil::IncreaseAudioVolumeL();
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    iMockContext->ExpectCallL( "CPhoneStateMachine::SendPhoneEngineMessage" ).
            WithL(MPEPhoneModel::EPEMessageSetAudioVolume);
    PhoneAudioUtil::DecreaseAudioVolumeL();
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    iMockContext->ExpectCallL( "CPhoneStateMachine::SendPhoneEngineMessage" ).TimesL(0);
    PhoneAudioUtil::ChangeAudioVolumeL( -1, EFalse );
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    
    PhoneAudioUtil::GetVolumeLevel();
    }

void ut_phoneaudioutil::T_HandleAudioAvailableOutputChangedL(  )
    {
    iEngineInfo->SetAudioOutput(EPEBTAudioAccessory, EPEHandset, EFalse);
    RArray<TPEAudioOutput> availableOutputs;

    availableOutputs.Append(EPEHandset);
    availableOutputs.Append(EPELoudspeaker);
    iEngineInfo->SetAvailableAudioOutputs(availableOutputs.Array());
    
    iMockContext->ExpectCallL( "CPhoneBtaaDisconnectHandler::Cancel" ).TimesL(0);
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" );
    PhoneAudioUtil::HandleAudioAvailableOutputChangedL();
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    availableOutputs.Reset();
    availableOutputs.Append(EPEBTAudioAccessory);
    availableOutputs.Append(EPEHandset);
    availableOutputs.Append(EPELoudspeaker);
    iEngineInfo->SetAvailableAudioOutputs(availableOutputs.Array());
    iMockContext->ExpectCallL( "CPhoneBtaaDisconnectHandler::Cancel" );
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" );
    PhoneAudioUtil::HandleAudioAvailableOutputChangedL();
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    availableOutputs.Close();
    }

void ut_phoneaudioutil::T_ToggleMute(  )
    {
    iEngineInfo->SetAudioMute(EFalse);
    
    iMockContext->ExpectCallL( "CPhoneStateMachine::SendPhoneEngineMessage" ).
            WithL(MPEPhoneModel::EPEMessageSetAudioMute);
    PhoneAudioUtil::ToggleMute();
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    EUNIT_ASSERT( iEngineInfo->AudioMuteCommand() );
    iMockContext->Reset();
    
    iEngineInfo->SetAudioMute(ETrue);
    
    iMockContext->ExpectCallL( "CPhoneStateMachine::SendPhoneEngineMessage" ).
            WithL(MPEPhoneModel::EPEMessageSetAudioMute);
    PhoneAudioUtil::ToggleMute();
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    EUNIT_ASSERT( EFalse == iEngineInfo->AudioMuteCommand() );
    iMockContext->Reset();
    }

void ut_phoneaudioutil::T_SetHandsfreeModeL(  )
    {
    iMockContext->ExpectCallL( "CPhoneAccessoryBTHandler::SetHandsfreeModeL" ).ReturnsL(ETrue);
    iMockContext->ExpectCallL( "PhoneNoteUtil::SendGlobalErrorNoteL" ).TimesL(0);
    PhoneAudioUtil::SetHandsfreeModeL( ETrue );
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    iMockContext->ExpectCallL( "CPhoneAccessoryBTHandler::SetHandsfreeModeL" ).ReturnsL(EFalse);
    iMockContext->ExpectCallL( "PhoneNoteUtil::SendGlobalErrorNoteL" ).
            WithL(EPhoneNoteTextNotAllowed ,ETrue);
    PhoneAudioUtil::SetHandsfreeModeL( ETrue );
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    
    iMockContext->ExpectCallL( "CPhoneAccessoryBTHandler::SetBTHandsfreeModeL" ).ReturnsL(ETrue);
    iMockContext->ExpectCallL( "PhoneNoteUtil::SendGlobalErrorNoteL" ).TimesL(0);
    PhoneAudioUtil::SetBtHandsfreeModeL( ETrue );
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    iMockContext->ExpectCallL( "CPhoneAccessoryBTHandler::SetBTHandsfreeModeL" ).ReturnsL(EFalse);
    iMockContext->ExpectCallL( "PhoneNoteUtil::SendGlobalErrorNoteL" ).
            WithL(EPhoneNoteTextNotAllowed ,ETrue);
    PhoneAudioUtil::SetBtHandsfreeModeL( ETrue );
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    }

// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    ut_phoneaudioutil,
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
    "T_HandleAudioMuteChangedL",
    "Test1",
    "FUNCTIONALITY",
    SetupL, T_HandleAudioMuteChangedL, Teardown)

EUNIT_TEST(
    "Test2",
    "T_HandleAudioOutputChangedL",
    "Test2",
    "FUNCTIONALITY",
    SetupL, T_HandleAudioOutputChangedL, Teardown)
    
EUNIT_TEST(
    "Test3",
    "T_ChangeAudioVolumeL",
    "Test3",
    "FUNCTIONALITY",
    SetupL, T_ChangeAudioVolumeL, Teardown)
    
EUNIT_TEST(
    "Test4",
    "T_HandleAudioAvailableOutputChangedL",
    "Test4",
    "FUNCTIONALITY",
    SetupL, T_HandleAudioAvailableOutputChangedL, Teardown)
    
EUNIT_TEST(
    "Test5",
    "T_ToggleMute",
    "Test5",
    "FUNCTIONALITY",
    SetupL, T_ToggleMute, Teardown)
    
EUNIT_TEST(
    "Test6",
    "T_SetHandsfreeModeL",
    "Test6",
    "FUNCTIONALITY",
    SetupL, T_SetHandsfreeModeL, Teardown)
    
EUNIT_END_TEST_TABLE
