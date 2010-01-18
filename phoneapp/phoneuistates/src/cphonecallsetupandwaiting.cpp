/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneCallSetupAndWaiting class.
*
*/


// INCLUDES
#include <pevirtualengine.h>
#include <StringLoader.h>
#include <MediatorDomainUIDs.h>
#include <videotelcontrolmediatorapi.h>

#include "cphonecallsetupandwaiting.h"
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
#include "cphonemediatorfactory.h"
#include "cphonemediatorsender.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneCallSetupAndWaiting::CPhoneCallSetupAndWaiting( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneGsmInCall( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneCallSetupAndWaiting::~CPhoneCallSetupAndWaiting()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneCallSetupAndWaiting::~CPhoneCallSetupAndWaiting()
    {
    }

// -----------------------------------------------------------
// CPhoneCallSetupAndWaiting::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneCallSetupAndWaiting::ConstructL()
    {
    CPhoneGsmInCall::ConstructL();
    }

// -----------------------------------------------------------
// CPhoneCallSetupAndWaiting::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneCallSetupAndWaiting* CPhoneCallSetupAndWaiting::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneCallSetupAndWaiting* self = new( ELeave ) CPhoneCallSetupAndWaiting( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneCallSetupAndWaiting::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneCallSetupAndWaiting::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneCallSetupAndWaiting::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;

        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;
            
        default:
            CPhoneGsmInCall::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneCallSetupAndWaiting::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneCallSetupAndWaiting::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneCallSetupAndWaiting::OpenMenuBarL()");
    TInt resourceId;

    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
        {
        resourceId = EPhoneDtmfDialerMenubar;
        }
    else if ( IsNumberEntryVisibleL() )
        {
        resourceId = EPhoneAlertingAndWaitingCallMenuBarWithNumberEntry;
        }
    else
        {
        resourceId = EPhoneAlertingAndWaitingCallMenuBar;
        }

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
    }

// -----------------------------------------------------------
// CPhoneCallSetupAndWaiting::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneCallSetupAndWaiting::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneCallSetupAndWaiting::HandleIdleL()");
        
    BeginUiUpdateLC();
            
    // Remove call 
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    // Find out do we have waiting or outgoing call left
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle->HandleCommandL( EPhoneViewGetCallIdByState, 
        &callStateData );
        
    if( callStateData.CallId() > KErrNotFound )
        {
        // Idle message came for callSetup

        if ( iOnScreenDialer && IsDTMFEditorVisibleL()  )
            {
            CloseDTMFEditorL();
            }
                
        // Display ringing bubble
        TPhoneCmdParamCallHeaderData callHeaderParam;
        callHeaderParam.SetCallState( EPEStateRinging );
         
        SetCallHeaderTextsForCallComingInL( callStateData.CallId(), EFalse, &callHeaderParam );
        
        iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, 
            callStateData.CallId(), 
            &callHeaderParam );
        
        // Show incoming call buttons
        SetTouchPaneButtons( EPhoneIncomingCallButtons ); 
        SetTouchPaneButtonEnabled( EPhoneCallComingCmdSilent );
        
        // Bring up callhandling view       
        BringIncomingToForegroundL();   
        
        // state changes to Incoming
        iCbaManager->UpdateIncomingCbaL( callStateData.CallId() );
        UpdateSilenceButtonDimming();
        SetRingingTonePlaybackL( callStateData.CallId() );
        iStateMachine->ChangeState( EPhoneStateIncoming );    
        }    
    
    else
        {
        // Show call setup buttons
        CPhoneState::SetTouchPaneButtons( EPhoneCallSetupButtons );
        // Waiting call was terminated
        UpdateCbaL( EPhoneCallHandlingInCallCBA );
        iStateMachine->ChangeState( EPhoneStateAlerting );            
        }
    
    EndUiUpdate();        
    }
        
// -----------------------------------------------------------
// CPhoneCallSetupAndWaiting::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneCallSetupAndWaiting::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, 
        "CPhoneCallSetupAndWaiting::UpdateInCallCbaL() ");
        
    UpdateCbaL( EPhoneCallHandlingIncomingRejectCBA );
    }

// -----------------------------------------------------------
// CPhoneCallSetupAndWaiting::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneCallSetupAndWaiting::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneCallSetupAndWaiting::HandleConnectedL() ");

    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
    
    CPhoneState::BeginUiUpdateLC();

    // Update bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
      
    // Update Touch buttons    
    CPhoneState::SetTouchPaneButtons( EPhoneWaitingCallButtons );
    SetToolbarDimming( EFalse );
    
    CPhoneState::EndUiUpdate();
 
    if ( CPhoneState::IsNumberEntryUsedL() )
        {
        // Show number entry
        TPhoneCmdParamBoolean booleanParam;
        booleanParam.SetBoolean( ETrue );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetNumberEntryVisible, &booleanParam );
        }
 
    // Go to Single And Waiting state 
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );   
    iStateMachine->ChangeState( EPhoneStateWaitingInSingle );        
    }
    
// End of File
