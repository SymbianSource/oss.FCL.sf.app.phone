/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneSingleAndAlerting class.
*
*/


// INCLUDES
#include <pevirtualengine.h>
#include <cpephonemodelif.h>
#include "cphonesingleandalerting.h"
#include "tphonecmdparamboolean.h"
#include "mphonestatemachine.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamcallstatedata.h"
#include "phoneviewcommanddefinitions.h"
#include "phoneui.hrh"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "phonestatedefinitionsgsm.h"
#include "phonelogger.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneSingleAndAlerting::CPhoneSingleAndAlerting( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneAlerting( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneSingleAndAlerting::~CPhoneSingleAndAlerting()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneSingleAndAlerting::~CPhoneSingleAndAlerting()
    {
    }

// -----------------------------------------------------------
// CPhoneSingleAndAlerting::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneSingleAndAlerting::ConstructL()
    {
    CPhoneAlerting::ConstructL();
    }

// -----------------------------------------------------------
// CPhoneSingleAndAlerting::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneSingleAndAlerting* CPhoneSingleAndAlerting::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneSingleAndAlerting* self = new( ELeave ) CPhoneSingleAndAlerting( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneSingleAndAlerting::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneSingleAndAlerting::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndAlerting::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;
            
        default:
            CPhoneAlerting::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }

// --------------------------------------------------------------
// CPhoneSingleAndAlerting::HandleKeyMessageL
// --------------------------------------------------------------
//
void CPhoneSingleAndAlerting::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndAlerting::HandleKeyMessageL()");
    switch ( aCode )
        {
        // send-key
        case EKeyYes:
            if ( CPhoneState::IsNumberEntryUsedL() )
                {
                // send a manual control sequence
                CPhoneState::CallFromNumberEntryL();    
                }
            else
                {
                // Show not allowed note
                CPhoneState::SendGlobalErrorNoteL( 
                    EPhoneNoteTextNotAllowed, ETrue );
                }
            break;

        // end-key
        case EKeyNo:
            if ( aMessage == EPhoneKeyLongPress )
                {
                iStateMachine->SendPhoneEngineMessage(
                    CPEPhoneModelIF::EPEMessageTerminateAllConnections );        
                }
            else
                {
                DisconnectOutgoingCallL();               
                }
                
            if ( CPhoneState::IsNumberEntryUsedL() )
                {
                // Remove number entry from screen
                iViewCommandHandle->ExecuteCommandL( 
                      EPhoneViewRemoveNumberEntry );
                // Do state-specific operation when number entry is cleared
                HandleNumberEntryClearedL();
                }
            break;
            
        default:
            // do base operation
            CPhoneAlerting::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneSingleAndAlerting::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneSingleAndAlerting::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndAlerting::HandleConnectedL()");

    // Find the alerting call
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnecting );                    
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
        
    if( callStateData.CallId() == aCallId )
        {    
        // Keep Phone in the foreground
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewSetNeedToSendToBackgroundStatus, &booleanParam );
        
        // Close menu bar, if it is displayed
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

        CPhoneState::BeginUiUpdateLC();
            
        // Show bubble
        TPhoneCmdParamCallHeaderData callHeaderParam;
        callHeaderParam.SetCallState( EPEStateConnected );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
            &callHeaderParam );

        CPhoneState::SetTouchPaneButtons( EPhoneTwoSinglesButtons );
        CPhoneState::EndUiUpdate();
              
        // Set Hold flag to view
        TPhoneCmdParamBoolean holdFlag;
        holdFlag.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );  
        
        
        // Set Two singles softkeys
        UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
        
        // Go to two singles state
        iStateMachine->ChangeState( EPhoneStateTwoSingles );
        }
    else
        {
        // Show bubble
        TPhoneCmdParamCallHeaderData callHeaderParam;
        callHeaderParam.SetCallState( EPEStateConnected );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
            &callHeaderParam );  
            
        TPhoneCmdParamBoolean holdFlag;
        holdFlag.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
        }    
    }
// -----------------------------------------------------------
// CPhoneSingleAndAlerting::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneSingleAndAlerting::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndAlerting::HandleIdleL()");
    
    BeginUiUpdateLC();
    
    // Remove call 
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    if ( !TopAppIsDisplayedL() )
        {
        // Continue displaying current app but set up the 
        // idle screen in the background
        SetupIdleScreenInBackgroundL();
        }
        
    TPhoneCmdParamInteger countParam;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetCountOfActiveCalls, &countParam);
    
    if ( countParam.Integer() )
        {
        SetTouchPaneButtons( EPhoneIncallButtons );    
        // Set Hold flag to view
        TPhoneCmdParamBoolean holdFlag;
        holdFlag.SetBoolean( ETrue );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
        UpdateCbaL( EPhoneCallHandlingInCallCBA );
        iStateMachine->ChangeState( EPhoneStateSingle );
        }
    else
        {
        // Display call termination note, if necessary
        DisplayCallTerminationNoteL();
        SetTouchPaneButtons( EPhoneCallSetupButtons );
        UpdateCbaL( EPhoneCallHandlingInCallCBA );
        iStateMachine->ChangeState( EPhoneStateAlerting );
        }

    EndUiUpdate();
    }

// -----------------------------------------------------------
// CPhoneSingleAndAlerting::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneSingleAndAlerting::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneSingleAndAlerting::OpenMenuBarL()");
    TInt resourceId;

    if ( IsNumberEntryVisibleL() )
        {
        resourceId = EPhoneAlertingAndHeldCallMenuBarWithNumberEntry;
        }
    else
        {
        resourceId = EPhoneAlertingAndHeldCallMenuBar;
        }

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
    }

// -----------------------------------------------------------
// CPhoneSingleAndAlerting::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneSingleAndAlerting::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneSingleAndAlerting::UpdateInCallCbaL() ");
    UpdateCbaL( EPhoneCallHandlingInCallCBA );
    }
    
        
// End of File
