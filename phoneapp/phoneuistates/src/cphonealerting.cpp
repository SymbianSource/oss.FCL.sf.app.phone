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
* Description: Implementation of CPhoneAlerting class.
*
*/


// INCLUDES
#include <pevirtualengine.h>
#include <mpeengineinfo.h>
#include <mpeclientinformation.h>
#include <MediatorDomainUIDs.h>
#include <videotelcontrolmediatorapi.h>
#include "cphonealerting.h"
#include "mphonestatemachine.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamcallstatedata.h"
#include "phoneviewcommanddefinitions.h"
#include "phoneui.hrh"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "phonestatedefinitionsgsm.h"
#include "phonelogger.h"
#include "phoneconstants.h"
#include "cphonemediatorfactory.h"
#include "cphonemediatorsender.h"


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C CPhoneAlerting::CPhoneAlerting( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneGsmInCall( aStateMachine, aViewCommandHandle, aPhoneCustomization ),
    iIncomingCallId( KErrNotFound )
    {
    }

// -----------------------------------------------------------
// CPhoneAlerting::~CPhoneAlerting()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C CPhoneAlerting::~CPhoneAlerting()
    {
    }

// -----------------------------------------------------------
// CPhoneAlerting::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C void CPhoneAlerting::ConstructL()
    {
    CPhoneGsmInCall::ConstructL();
    }

// -----------------------------------------------------------
// CPhoneAlerting::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneAlerting* CPhoneAlerting::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneAlerting* self = new( ELeave ) CPhoneAlerting( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// --------------------------------------------------------------
// CPhoneAlerting::HandleKeyMessageL
// --------------------------------------------------------------
//
EXPORT_C void CPhoneAlerting::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneAlerting::HandleKeyMessageL()");
    switch ( aCode )
        {
        // send-key
        case EKeyYes:
            if ( IsNumberEntryUsedL() )
                {
                // send a manual control sequence
                CallFromNumberEntryL();    
                }
            else
                {
                // Show not allowed note
                SendGlobalErrorNoteL( 
                    EPhoneNoteTextNotAllowed );
                }
            break;

        // end-key
        case EKeyNo:
            // handle long press
            if ( aMessage == EPhoneKeyLongPress )
                {
                // Close all connections
                iStateMachine->SendPhoneEngineMessage(
                    MPEPhoneModel::EPEMessageTerminateAllConnections ); 

                if ( IsNumberEntryUsedL() )
                    {
                    CloseClearNumberEntryAndLoadEffectL( EPhoneTransEffectPhoneUiClose );
                    }
                if ( !TopAppIsDisplayedL() )
                    {
                    // Bring app to foreground
                    TPhoneCmdParamInteger uidParam;
                    uidParam.SetInteger( KUidPhoneApplication.iUid );
                    iViewCommandHandle->ExecuteCommandL(
                        EPhoneViewBringAppToForeground, &uidParam );
                    }
                }
            else
                {
                // handle end key
                DisconnectOutgoingCallL();
                }
            break;
            
        default:
            // do base operation
            CPhoneGsmInCall::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneAlerting::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneAlerting::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneAlerting::HandlePhoneEngineMessageL()");
    
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageConnected:
            {
            HandleConnectedL( aCallId );
            }
            break;
        
        case MEngineMonitor::EPEMessageDisconnecting:
            {
            HandleDisconnectingL( aCallId );
            }
            break;
        
        case MEngineMonitor::EPEMessageRemoteTerminated:
            {
            iViewCommandHandle->ExecuteCommandL( 
                EPhoneViewHideNaviPaneAudioVolume );            
            }
            break;
        
        case MEngineMonitor::EPEMessageIncoming:
            {
            // If incoming message is received in alerting state, teardown of
            // alerting call must be ongoing. Handling of the incoming call is
            // buffered until first call teardown is completed.
            iIncomingCallId = aCallId;
            }
            break;
        
        case MEngineMonitor::EPEMessageIdle:
            {
            if ( KErrNotFound < iIncomingCallId )
                {
                TInt incomingCallId = iIncomingCallId;
                iIncomingCallId = KErrNotFound;
                
                if ( aCallId != incomingCallId )
                    {
                    CPhoneGsmInCall::HandlePhoneEngineMessageL( 
                        aMessage, aCallId );
                    
                    // Simulate incoming event which was received while 
                    // teardown of first call was ongoing.
                    iStateMachine->State()->HandlePhoneEngineMessageL( 
                        MEngineMonitor::EPEMessageIncoming, incomingCallId );
                    }
                
                return;
                }
            }
            break;
                   
        
        default:
            break;
        }

    CPhoneGsmInCall::HandlePhoneEngineMessageL( aMessage, aCallId );
    }

// -----------------------------------------------------------
// CPhoneAlerting::HandleConnectedL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneAlerting::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
        "CPhoneAlerting::HandleConnectedL()");

    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    TransitionHandlerL().BeginUiUpdateLC();

    // Update the single call
    UpdateSingleActiveCallL( aCallId );

    SetTouchPaneButtons( EPhoneIncallButtons );

    SetToolbarDimming( EFalse );

    TransitionHandlerL().EndUiUpdate();
    
    // Go to single state
    UpdateCbaL( EPhoneCallHandlingInCallCBA );    
    iStateMachine->ChangeState( EPhoneStateSingle );
    }

// -----------------------------------------------------------
// CPhoneAlerting::OpenMenuBarL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneAlerting::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneAlerting::OpenMenuBarL()");
    TInt resourceId;

    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
        {
        resourceId = EPhoneDtmfDialerMenubar;
        }
    else if ( IsNumberEntryVisibleL() )
        {
        resourceId = EPhoneAlertingCallMenubarWithNumberEntry;
        }
    // Use different resources for alerting data, video and cs call
    else if( IsVideoCallAlertingL() )
        {
        resourceId = EPhoneAlertingVideoCallMenubar;
        }
    else
        {
        resourceId = EPhoneAlertingCallMenubar;
        }            
 
    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
    }

// -----------------------------------------------------------
// CPhoneAlerting::HandleCommandL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneAlerting::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,  
        "CPhoneAlerting::HandleCommandL()" );
    TBool commandStatus = ETrue;

    switch( aCommand )
        {
        case EPhoneInCallCmdHelp:
            {
            TPtrC contextName;
            if ( IsVideoCallAlertingL() )
                {
                contextName.Set( KINCAL_HLP_VIDEOCALL() );    
                }
            else
                {
                contextName.Set( KINCAL_HLP_CALL_HANDLING() );
                }
            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewLaunchHelpApplication, 0, contextName );
            }
            break;
        default:
            commandStatus = CPhoneGsmInCall::HandleCommandL( aCommand );
            break;
        }
    return commandStatus;
    }

// -----------------------------------------------------------
// CPhoneAlerting::IsVideoCallAlertingL
// -----------------------------------------------------------
//
TBool CPhoneAlerting::IsVideoCallAlertingL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,  
        "CPhoneAlerting::IsVideoCallAlerting()" );
    TBool retVal = EFalse;
    // Fetch active call's id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnecting );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
        
    if ( callStateData.CallId() > KErrNotFound )
        {
        retVal = IsVideoCall( callStateData.CallId() );
        }
        
    return retVal;
    }
    
// -----------------------------------------------------------
// CPhoneAlerting::CheckIfShowTerminationNote
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneAlerting::CheckIfShowCallTerminationNote( )
    {
    // Alerting state does not need call termination since
    // call has not begun.
    return EFalse;
    }

// -----------------------------------------------------------
// CPhoneAlerting::HandleDisconnectingL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneAlerting::HandleDisconnectingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,  
            "CPhoneAlerting::HandleDisconnectingL()" );
    
    if ( iStateMachine->PhoneEngineInfo()->CallOrigin( aCallId ) == EPECallOriginSAT )
        {
        // User has hangup alerting SAT call, complete sat request
        CompleteSatRequestL( aCallId );
        }
    
    CPhoneGsmInCall::HandleDisconnectingL( aCallId );
    }



// End of File
