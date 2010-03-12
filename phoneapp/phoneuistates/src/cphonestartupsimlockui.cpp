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
 * Description: Implementation of CPhoneStartup class.
 *
 */

// INCLUDES
#include "phonerssbase.h"
#include "phonelogger.h"
#include "cphonestartupsimlockui.h"
#include "tphonecmdparamappinfo.h"
#include "TPhoneCmdParamBoolean.h"
#include "phoneconstants.h"
#include "cphonestatestartup.h"
#include "phonestatedefinitions.h"
#include "mphonestatemachine.h"
#include "tphonecmdparamkeycapture.h"
#include "tphonecmdparamboolean.h"
#include "cphonepubsubproxy.h"
#include "phoneconstants.h"
#include <startupdomainpskeys.h>
#include <oem/simlockuikeys.h>


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//

CPhoneStartupSimlockUi::CPhoneStartupSimlockUi(
        MPhoneStateMachine* aStateMachine,
        MPhoneViewCommandHandle* aViewCommandHandle, TBool aPEReady ) :
CPhoneStateStartup( aStateMachine, aViewCommandHandle, NULL ), iSimLockActive (EFalse)
    {
    iPEReady = aPEReady;
    }

// -----------------------------------------------------------
// CPhoneStartupSimlockUi::~CPhoneStartupSimlockUi()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------

CPhoneStartupSimlockUi::~CPhoneStartupSimlockUi()
    {
    }

// -----------------------------------------------------------
// CPhoneStartupSimlockUi::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneStartupSimlockUi::ConstructL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneStartupSimlockUi::ConstructL()" );

    CPhoneStateStartup::ConstructL();
    iSimLockActive = ETrue;
    EnterSimLockModeL();

    }

// -----------------------------------------------------------
// CPhoneStartupSimlockUi::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneStartupSimlockUi* CPhoneStartupSimlockUi::NewL(
        MPhoneStateMachine* aStateMachine,
        MPhoneViewCommandHandle* aViewCommandHandle, TBool aPEReady )

    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneStartupSimlockUi::NewL()");
    
    CPhoneStartupSimlockUi* self = new (ELeave) CPhoneStartupSimlockUi(
            aStateMachine, aViewCommandHandle, aPEReady );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;

    }

// -----------------------------------------------------------
// CPhoneStateStartup::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStartupSimlockUi::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneStartupSimlockUi::HandlePhoneEngineMessageL() " );
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessagePEConstructionReady:

            // phone start is now possible
            if ( !iSimLockActive )
                {
                CPhoneStateStartup::HandlePhoneEngineMessageL( aMessage, aCallId );
                }
            else
                {
                iPEReady = ETrue;
                }
            break;
            
        default:
        	CPhoneStateStartup::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneStartupSimlockUi::HandlePhoneStartupL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStartupSimlockUi::HandlePhoneStartupL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStartupSimlockUi::HandlePhoneStartupL() ");

    if( !iSimLockActive )
        {
        CPhoneStateStartup::HandlePhoneStartupL();
        }
    else
        {
        // set the flag in base class
        iPhoneReady = ETrue;
        }        
    }

// -----------------------------------------------------------
// CPhoneStartupSimlockUi::HandlePropertyChangedL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStartupSimlockUi::HandlePropertyChangedL(
        const TUid& aCategory, const TUint aKey, const TInt aValue )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,"CPhoneStartupSimlockUi::HandlePropertyChangedL( ) " );

    if ( aCategory == KSimLockProperty )
        {
        if ( aKey == ESimLockActiveStatus )
            {
            if ( aValue == KSimLockStatusDone )
                {
                // No longer in Sim Lock mode
                ExitSimLockModeL();
                }
            }
        }
    else
        {
        CPhoneState::HandlePropertyChangedL( aCategory, aKey, aValue );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneStartupSimlockUi::EnterSimLockModeL
// -----------------------------------------------------------------------------
//
void CPhoneStartupSimlockUi::EnterSimLockModeL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,"CPhoneStartupSimlockUi::EnterSimLockModeL( ) " );

    TPhoneCmdParamAppInfo appInfoParam;
    appInfoParam.SetAppUid( KSimLockUIApplicationUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewActivateApp, &appInfoParam );

    }
// -----------------------------------------------------------------------------
// CPhoneStartupSimlockUi::ExitSimLockModeL
// -----------------------------------------------------------------------------
//
void CPhoneStartupSimlockUi::ExitSimLockModeL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,"CPhoneStartupSimlockUi::ExitSimLockModeL( ) " );
    iSimLockActive = EFalse;
	
	if ( iPhoneReady )
		{
		CPhoneStateStartup::HandlePhoneStartupL();
    	}
	else if ( iPEReady )
		{
		CPhoneStateStartup::HandlePEConstructionReadyL( 0 );
		}

	//if iPhoneReady or iPEReady is EFalse, we should just wait.
    }

// End of File
