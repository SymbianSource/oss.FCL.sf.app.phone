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
#include "CEasyDialingPlugin_Test.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <f32file.h> 
#include <bautils.h> 

#include "easydialingplugin.h"
#include "dummyparentcontrol.h"

#include <cntdb.h> 
#include <cntitem.h> 
#include <cntfldst.h> 
#include <bautils.h> 


// - Construction -----------------------------------------------------------

CEasyDialingPlugin_Test* CEasyDialingPlugin_Test::NewL()
    {
    CEasyDialingPlugin_Test* self = CEasyDialingPlugin_Test::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

CEasyDialingPlugin_Test* CEasyDialingPlugin_Test::NewLC()
    {
    CEasyDialingPlugin_Test* self = new( ELeave ) CEasyDialingPlugin_Test();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CEasyDialingPlugin_Test::~CEasyDialingPlugin_Test()
    {
    RDebug::Print(_L("EDTest: enter desctructor"));
    
    if (iCEasyDialingPlugin)
        {
        iCEasyDialingPlugin->RemoveObserver( this );
        }
    delete iWait;
    delete iCEasyDialingPlugin;
    delete iDummyParentControl;
    RDebug::Print(_L("EDTest: exit desctructor"));
    }

CEasyDialingPlugin_Test::CEasyDialingPlugin_Test()
    {
    }


TInt CreateTestContactDatabaseL();

void CEasyDialingPlugin_Test::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    CreateTestContactDatabaseL();
    iWait = new( ELeave )CActiveSchedulerWait();
    iDummyParentControl = CDummyParentControl::NewL();
    
    iCEasyDialingPlugin = CEasyDialingPlugin::NewL();
    iCEasyDialingPlugin->InitializeL( *iDummyParentControl );
    iCEasyDialingPlugin->AddObserverL( this );
    
    
    RDebug::Print(_L("EDTest:Construct plugin exit"));
    }


void CEasyDialingPlugin_Test::HandleDialingExtensionEvent( MDialingExtensionObserver::TEvent aEvent )
    {
    if ( aEvent == MDialingExtensionObserver::ESearchComplete )
        {
        iWait->AsyncStop();
        }
    }

// - Test methods -----------------------------------------------------------



void CEasyDialingPlugin_Test::SetupL(  )
    {
    }
    

void CEasyDialingPlugin_Test::Teardown(  )
    {
    }


void CEasyDialingPlugin_Test::T_Global_OfferKeyEventLL(  )
    {
    
    TKeyEvent ke;
    ke.iCode = 32;
    ke.iModifiers = 0;
    ke.iRepeats = 0;
    ke.iScanCode = 32;
    
    iCEasyDialingPlugin->OfferKeyEventL( ke, EEventKey );
    EUNIT_ASSERT_DESC( iCEasyDialingPlugin, "OfferKeyEventL general assert");
    
    }
    

// -----------------------------------------------------------------------------
// T_Global_Search*
// These functions do all kind of searches with easy dialing and check that
// the count of matching contacts is right. These tests require that a specific
// test contact data base has been created with CreateTestContactDatabaseL().
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin_Test::T_Global_Search1(  )
    {
    TRAP_IGNORE( iCEasyDialingPlugin->SetInputL( _L("A") ));
    iWait->Start();
    // Now execution stops here until HandleDialingExtensionEvent has been called. 
    
    TInt count = iCEasyDialingPlugin->MatchingContactCount();
    EUNIT_ASSERT_EQUALS( count, 1 );
    }
   
void CEasyDialingPlugin_Test::T_Global_Search2()
	{
	TRAP_IGNORE( iCEasyDialingPlugin->SetInputL( _L("b") ));
    iWait->Start();
    // Now execution stops here until HandleDialingExtensionEvent has been called.
    
	TInt count = iCEasyDialingPlugin->MatchingContactCount();
    EUNIT_ASSERT_EQUALS( count, 1 );
	}

void CEasyDialingPlugin_Test::T_Global_Search3()
    {
    TRAP_IGNORE( iCEasyDialingPlugin->SetInputL( _L("C") ));
    iWait->Start();
    // Now execution stops here until HandleDialingExtensionEvent has been called.
    
    TInt count = iCEasyDialingPlugin->MatchingContactCount();
    EUNIT_ASSERT_EQUALS( count, 2 );
    }

void CEasyDialingPlugin_Test::T_Global_Search4()
    {
    TRAP_IGNORE( iCEasyDialingPlugin->SetInputL( _L("2") ));
    iWait->Start();
    // Now execution stops here until HandleDialingExtensionEvent has been called.
    
    TInt count = iCEasyDialingPlugin->MatchingContactCount();
    EUNIT_ASSERT_EQUALS( count, 4 );
    }

void CEasyDialingPlugin_Test::T_Global_Search5()
    {
    TRAP_IGNORE( iCEasyDialingPlugin->SetInputL( _L("3") ));
    iWait->Start();
    // Now execution stops here until HandleDialingExtensionEvent has been called.
    
    TInt count = iCEasyDialingPlugin->MatchingContactCount();
    EUNIT_ASSERT_EQUALS( count, 4 );
    }

void CEasyDialingPlugin_Test::T_Global_Search6()
    {
    TRAP_IGNORE( iCEasyDialingPlugin->SetInputL( _L("8286607") )); // should match to <Tauno P>alo
    iWait->Start();
    // Now execution stops here until HandleDialingExtensionEvent has been called.
    
    TInt count = iCEasyDialingPlugin->MatchingContactCount();
    EUNIT_ASSERT_EQUALS( count, 1 );
    }

void CEasyDialingPlugin_Test::T_Global_Search7()
    {
    TRAP_IGNORE( iCEasyDialingPlugin->SetInputL( _L("5569304") )); // should match to <Lloyd H>arold
    iWait->Start();
    // Now execution stops here until HandleDialingExtensionEvent has been called.
    
    TInt count = iCEasyDialingPlugin->MatchingContactCount();
    EUNIT_ASSERT_EQUALS( count, 1 );
    }

void CEasyDialingPlugin_Test::T_Global_Search8()
    {
    TRAP_IGNORE( iCEasyDialingPlugin->SetInputL( _L("3456407") )); // should match to <Filmi S>uomi
    iWait->Start();
    // Now execution stops here until HandleDialingExtensionEvent has been called.
    
    TInt count = iCEasyDialingPlugin->MatchingContactCount();
    EUNIT_ASSERT_EQUALS( count, 2 );
    }


    
// -----------------------------------------------------------------------------
// T_Global__CEasyDialingPluginL
// Destructor test 
// -----------------------------------------------------------------------------
//
void CEasyDialingPlugin_Test::T_Global__CEasyDialingPluginL(  )
    {
    iCEasyDialingPlugin->~CEasyDialingPlugin();
    iCEasyDialingPlugin = NULL;
    iCEasyDialingPlugin = CEasyDialingPlugin::NewL();
    EUNIT_ASSERT_DESC( ETrue, "Generated assert, replace with real");
    }







// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    CEasyDialingPlugin_Test,
    "Easy dialing plugin test",
    "UNIT" )

EUNIT_TEST(
    "OfferKeyEventL",
    "CEasyDialingPlugin",
    "OfferKeyEventL",
    "FUNCTIONALITY",
    SetupL, T_Global_OfferKeyEventLL, Teardown)
    
EUNIT_TEST(
    "SetInputLL",
    "CEasyDialingPlugin",
    "SetInputLL",
    "FUNCTIONALITY",
    SetupL, T_Global_Search1, Teardown)
    
EUNIT_TEST(
	"Tests matching contact count function.",
	"CEasyDialingPlugin_Test",
	"MatchingContactCount",
	"FUNCTIONALITY",
	SetupL, T_Global_Search2, Teardown )

EUNIT_TEST(
    "PCS search test function",
    "CEasyDialingPlugin_Test",
    "",
    "FUNCTIONALITY",
    SetupL, T_Global_Search3, Teardown )
    
EUNIT_TEST(
    "PCS search test function",
    "CEasyDialingPlugin_Test",
    "",
    "FUNCTIONALITY",
    SetupL, T_Global_Search4, Teardown )
    
EUNIT_TEST(
    "PCS search test function",
    "CEasyDialingPlugin_Test",
    "",
    "FUNCTIONALITY",
    SetupL, T_Global_Search5, Teardown )
    
EUNIT_TEST(
    "PCS search test function",
    "CEasyDialingPlugin_Test",
    "",
    "FUNCTIONALITY",
    SetupL, T_Global_Search6, Teardown )
    
EUNIT_TEST(
    "PCS search test function",
    "CEasyDialingPlugin_Test",
    "",
    "FUNCTIONALITY",
    SetupL, T_Global_Search7, Teardown )
    
EUNIT_TEST(
    "PCS search test function",
    "CEasyDialingPlugin_Test",
    "",
    "FUNCTIONALITY",
    SetupL, T_Global_Search8, Teardown )
    
EUNIT_TEST(
    "~CEasyDialingPlugin",
    "CEasyDialingPlugin",
    "~CEasyDialingPlugin",
    "FUNCTIONALITY",
    SetupL, T_Global__CEasyDialingPluginL, Teardown)
    
EUNIT_END_TEST_TABLE
