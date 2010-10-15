/*
* Copyright (c) 2020 Nokia Corporation and/or its subsidiary(-ies). 
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

#include "ut_phonenoteutil.h"
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
#include "tphonecmdparamnote.h" 
#include "tphonecmdparamquery.h"
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
#include "CPhoneMainErrorMessagesHandler.h"

// needed for mocking CPhoneMainResourceResolver::Instance()
class CPhoneMainResourceResolverMock : public CPhoneMainResourceResolver{};

class CPhoneMainErrorMessagesHandlerMock : public CPhoneMainErrorMessagesHandler
    {

    };

class CPhoneStateMachineGSMMock : public CPhoneStateMachineGSM{};


        

// - Construction -----------------------------------------------------------

ut_phonenoteutil* ut_phonenoteutil::NewL()
    {
    ut_phonenoteutil* self = ut_phonenoteutil::NewLC();
    CleanupStack::Pop();
    return self;
    }

ut_phonenoteutil* ut_phonenoteutil::NewLC()
    {
    ut_phonenoteutil* self = new( ELeave ) ut_phonenoteutil();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

ut_phonenoteutil::~ut_phonenoteutil()
    {
    CSmcMockContext::Release();
    }

ut_phonenoteutil::ut_phonenoteutil()
    {
    }

void ut_phonenoteutil::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    iMockContext = CSmcMockContext::InstanceL();
    }

// - Test methods -----------------------------------------------------------

void ut_phonenoteutil::SetupL(  )
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
    
    iCPhoneMainErrorMessagesHandlerMock = new ( ELeave ) CPhoneMainErrorMessagesHandlerMock();
    SmcDefaultValue<CPhoneMainErrorMessagesHandler*>::SetL( iCPhoneMainErrorMessagesHandlerMock );
    
    iEngineInfo = CPEEngineInfo::NewL();
    SmcDefaultValue<MPEEngineInfo*>::SetL( iEngineInfo );    
    }

void ut_phonenoteutil::Teardown(  )
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
    
    delete iCPhoneMainErrorMessagesHandlerMock;
    iCPhoneMainErrorMessagesHandlerMock = NULL;
    SmcDefaultValue<CPhoneMainErrorMessagesHandler*>::Reset();
    
    iMockContext->Reset();
    }
    
void ut_phonenoteutil::T_ConstructionL(  )
    {
    // SetupL and Teardown does the test.
    }

void ut_phonenoteutil::T_SendGlobalInfoNoteL(  )
    {
    TInt resource = 10;
    TBool notification = ETrue;
    
    TPhoneCmdParamGlobalNote globalNoteParam;
    iMockContext->ExpectCallL( "CPhoneMainResourceResolver::ResolveResourceID" ).
               WithL(resource).
               ReturnsL(resource);
    
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" ).
            WithL<TPhoneViewCommandId, TPhoneCommandParam&> ( EPhoneViewShowGlobalNote, globalNoteParam ); 
    
    PhoneNoteUtil::SendGlobalInfoNoteL( resource, notification);
    
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();

    iMockContext->ExpectCallL( "CPhoneMainResourceResolver::ResolveResourceID" ).
               WithL(resource).
               ReturnsL(resource);
    
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" ).
            WithL<TPhoneViewCommandId, TPhoneCommandParam&> ( EPhoneViewShowGlobalNote, globalNoteParam ); 
    
    PhoneNoteUtil::SendGlobalNoteL( resource, notification);
    
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    }

void ut_phonenoteutil::T_SendGlobalWarningNoteL(  )
    {
    TInt resource = 10;
    TBool notification = ETrue;
    
    TPhoneCmdParamGlobalNote globalNoteParam;
    iMockContext->ExpectCallL( "CPhoneMainResourceResolver::ResolveResourceID" ).
               WithL(resource).
               ReturnsL(resource);
    
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" ).
            WithL<TPhoneViewCommandId, TPhoneCommandParam&> ( EPhoneViewShowGlobalNote, globalNoteParam ); 
    
    PhoneNoteUtil::SendGlobalWarningNoteL( resource, notification);
    
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    }

void ut_phonenoteutil::T_SendGlobalErrorNoteL(  )
    {
    TInt resource = 10;
    TBool notification = ETrue;
    
    TPhoneCmdParamGlobalNote globalNoteParam;
    iMockContext->ExpectCallL( "CPhoneMainResourceResolver::ResolveResourceID" ).
               WithL(resource).
               ReturnsL(resource);
    
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" ).
            WithL<TPhoneViewCommandId, TPhoneCommandParam&> ( EPhoneViewShowGlobalNote, globalNoteParam ); 
    
    PhoneNoteUtil::SendGlobalErrorNoteL( resource, notification);
    
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    }

void ut_phonenoteutil::T_SendGlobalNoteL2(  )
    {
    _LIT(KTestText,"test");

    TPhoneCmdParamGlobalNote globalNoteParam;
    
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" ).
            WithL<TPhoneViewCommandId, TPhoneCommandParam&> ( EPhoneViewShowGlobalNote, globalNoteParam ); 
    
    PhoneNoteUtil::SendGlobalNoteL( EPhoneNotificationDialog, KTestText());
    
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    }

void ut_phonenoteutil::T_ShowNoteAndQueryL(  )
    {
    TInt resource = 10;
    TPhoneCmdParamNote noteParam;
    TPhoneCmdParamQuery queryParam;
    
    // Test ShowNoteL
    iMockContext->ExpectCallL( "CPhoneMainResourceResolver::ResolveResourceID" ).
               WithL(resource).
               ReturnsL(resource);
    
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" ).
            WithL<TPhoneViewCommandId, TPhoneCommandParam&> ( EPhoneViewShowNote, noteParam ); 
    
    PhoneNoteUtil::ShowNoteL( resource );
    
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    // Test ShowQueryL
    iMockContext->ExpectCallL( "CPhoneMainResourceResolver::ResolveResourceID" ).
               WithL(resource).
               ReturnsL(resource);
    
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" ).
            WithL<TPhoneViewCommandId, TPhoneCommandParam&> ( EPhoneViewShowQuery, queryParam ); 
    
    PhoneNoteUtil::ShowQueryL( resource );
    
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    HBufC* emptyString = HBufC::NewLC( KPhoneNumberEntryBufferSize );
    TPtr ptr( emptyString->Des() );
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" ).
            WithL<TPhoneViewCommandId, TPhoneCommandParam&> ( EPhoneViewShowQuery, queryParam ); 
    
    PhoneNoteUtil::ShowTextQueryL(resource,resource,resource,&ptr,ETrue);
    
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    CleanupStack::PopAndDestroy( emptyString );
    }

void ut_phonenoteutil::T_HandleErrorL(  )
    {
    TPEErrorInfo errorInfo;
    
    iMockContext->ExpectCallL( "CPhoneMainErrorMessagesHandler::ShowErrorSpecificNoteL" ).
            WithL<TPEErrorInfo&> ( errorInfo );
    iMockContext->ExpectCallL( "CPhoneClearBlacklist::ClearBlackListOnNextKey" );            
    PhoneNoteUtil::HandleErrorL(errorInfo);   
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    errorInfo.iErrorCode = ECCPErrorNoService;
    iMockContext->ExpectCallL( "CPhoneMainErrorMessagesHandler::ShowErrorSpecificNoteL" ).
            WithL<TPEErrorInfo&> ( errorInfo );
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" ).
            WithL( EPhoneViewHideNaviPaneAudioVolume ); 
    iMockContext->ExpectCallL( "CPhoneClearBlacklist::ClearBlackListOnNextKey" );
    PhoneNoteUtil::HandleErrorL(errorInfo);
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    
    errorInfo.iErrorCode = ECCPErrorSatControl;
    iMockContext->ExpectCallL( "CPhoneMainErrorMessagesHandler::ShowErrorSpecificNoteL" ).
            WithL<TPEErrorInfo&> ( errorInfo );
    iMockContext->ExpectCallL( "CPhoneViewCommandHandleMock::ExecuteCommandL" ).
            WithL( EPhoneViewClearNumberEntryContent );     
    iMockContext->ExpectCallL( "CPhoneClearBlacklist::ClearBlackListOnNextKey" );
    PhoneNoteUtil::HandleErrorL(errorInfo);
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    iMockContext->Reset();
    }

void ut_phonenoteutil::T_HandleRemoteBusyNoteL(  )
    {
    TInt callId = 1;
    iMockContext->ExpectCallL( "CPhoneMainErrorMessagesHandler::ShowErrorSpecificNoteL" );         
    PhoneNoteUtil::HandleRemoteBusyNoteL(callId);   
    EUNIT_ASSERT_EQUALS( KErrNone, iMockContext->VerifyExpectations() );
    }

// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    ut_phonenoteutil,
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
    "T_SendGlobalInfoNoteL",
    "Test1",
    "FUNCTIONALITY",
    SetupL, T_SendGlobalInfoNoteL, Teardown)

EUNIT_TEST(
    "Test2",
    "T_SendGlobalWarningNoteL",
    "Test2",
    "FUNCTIONALITY",
    SetupL, T_SendGlobalWarningNoteL, Teardown)
    
EUNIT_TEST(
    "Test3",
    "T_SendGlobalErrorNoteL",
    "Test3",
    "FUNCTIONALITY",
    SetupL, T_SendGlobalErrorNoteL, Teardown)
    
EUNIT_TEST(
    "Test4",
    "T_SendGlobalNoteL2",
    "Test4",
    "FUNCTIONALITY",
    SetupL, T_SendGlobalNoteL2, Teardown)
    
EUNIT_TEST(
    "Test5",
    "T_ShowNoteAndQueryL",
    "Test5",
    "FUNCTIONALITY",
    SetupL, T_ShowNoteAndQueryL, Teardown)
    
EUNIT_TEST(
    "Test6",
    "T_HandleRemoteBusyNoteL",
    "Test6",
    "FUNCTIONALITY",
    SetupL, T_HandleRemoteBusyNoteL, Teardown)
    
EUNIT_TEST(
    "Test7",
    "T_HandleErrorL",
    "Test7",
    "FUNCTIONALITY",
    SetupL, T_HandleErrorL, Teardown)
    
EUNIT_END_TEST_TABLE
