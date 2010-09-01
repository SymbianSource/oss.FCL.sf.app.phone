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
* Description: Implementation of CPhoneConferenceAndCallSetup class.
*
*/


// INCLUDES
#include "cphoneconferenceandcallsetup.h"
#include "mphonestatemachine.h"
#include "phoneviewcommanddefinitions.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamcallstatedata.h"
#include "phonestatedefinitionsgsm.h"
#include "phonelogger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "tphonecmdparamglobalnote.h"
#include "phoneui.hrh"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneConferenceAndCallSetup::CPhoneConferenceAndCallSetup( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneConference( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneConferenceAndCallSetup::~CPhoneConferenceAndCallSetup()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConferenceAndCallSetup::~CPhoneConferenceAndCallSetup()
    {
    // Need to check iViewCommandHandle validity here to not
    // trigger a high/can panic error in a Codescanner run.
    // coverity[var_compare_op]
    if ( iViewCommandHandle )
        {
        TPhoneCmdParamBoolean dtmfSendFlag;
        dtmfSendFlag.SetBoolean( EFalse );
        TRAP_IGNORE( iViewCommandHandle->ExecuteCommandL( EPhoneViewSetDtmfOptionsFlag, 
    	    &dtmfSendFlag ) );
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndCallSetup::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneConferenceAndCallSetup::ConstructL()
    {
    CPhoneConference::ConstructL();
  
    }

// -----------------------------------------------------------
// CPhoneConferenceAndCallSetup::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConferenceAndCallSetup* CPhoneConferenceAndCallSetup::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneConferenceAndCallSetup* self = new( ELeave ) CPhoneConferenceAndCallSetup( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneConferenceAndCallSetup::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneConferenceAndCallSetup::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneConferenceAndCallSetup::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageConnecting:
            {
            iAlerting = ETrue;
            HandleConnectingL( aCallId );
            }
            break;
            
        case MEngineMonitor::EPEMessageConnected:
            {
            HandleConnectedL( aCallId );
            }
            break;
            
        case MEngineMonitor::EPEMessageIdle:
            {
            HandleIdleL( aCallId );
            }
            break;
            
        case MEngineMonitor::EPEMessageConferenceIdle:
            {
            HandleConferenceIdleL();
            }
            break;
            
        default:
            {
            CPhoneConference::HandlePhoneEngineMessageL( aMessage, 
                aCallId );
            }
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndCallSetup::HandleCommandL
// -----------------------------------------------------------
//
TBool CPhoneConferenceAndCallSetup::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndCallSetup::HandleCommandL()");
    TBool commandStatus = ETrue;
    
    switch( aCommand )
        {   
        case EPhoneDtmfDialerCancel:
            {
            CloseDTMFEditorL();
            }
            break;    
    
        default:
            commandStatus = CPhoneConference::HandleCommandL( aCommand );
            break;
        }

    return commandStatus;
    }
    
// --------------------------------------------------------------
// CPhoneConferenceAndCallSetup::HandleKeyMessageL
// --------------------------------------------------------------
//
void CPhoneConferenceAndCallSetup::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndCallSetup::HandleKeyMessageL()");
    switch ( aCode )
        {
        // End-key
        case EKeyNo:
            if ( aMessage == EPhoneKeyLongPress )
                {
                iStateMachine->SendPhoneEngineMessage(
                        MPEPhoneModel::EPEMessageTerminateAllConnections );   
                }
            else
                {
                DisconnectOutgoingCallL();
                }
            break;
            
        default:
            // do base operation
            CPhoneConference::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }
// -----------------------------------------------------------
// CPhoneConferenceAndCallSetup::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneConferenceAndCallSetup::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneConferenceAndCallSetup::OpenMenuBarL()");
    TInt resourceId;
    
    // Set specific flag to view so that DTMF menu item available
    TPhoneCmdParamBoolean dtmfSendFlag;
    dtmfSendFlag.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetDtmfOptionsFlag, 
    	&dtmfSendFlag );
     
    if ( iOnScreenDialer &&  IsDTMFEditorVisibleL() )
        {
        resourceId = EPhoneDtmfDialerMenubar;
        }
    else if ( IsNumberEntryVisibleL() )
        {
        resourceId = EPhoneAlertingAndConfHeldCallMenuBarWithNumberEntry;
        }
    else if ( IsConferenceBubbleInSelectionMode() )
        {
        resourceId = EPhoneConfCallParticipantsDropMenubar;    
        }        
    else
	    {
	    resourceId = EPhoneAlertingAndConfHeldCallMenuBar;
	    }

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
                CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( resourceId ) );
	    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndCallSetup::HandleConnectingL
// -----------------------------------------------------------
//
void CPhoneConferenceAndCallSetup::HandleConnectingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneConferenceAndCallSetup::HandleConnectingL()");
    
    BeginUiUpdateLC();
        
    UpdateRemoteInfoDataL ( aCallId );
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveGlobalNote );
    
    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );

    // Stop capturing keys
    CaptureKeysDuringCallNotificationL( EFalse );

    // Remove the number entry if it isn't DTMF dialer
    if ( !iOnScreenDialer || !IsNumberEntryVisibleL() || ! IsDTMFEditorVisibleL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }
    
    // Set Hold flag to view EFalse that IHF is on RSK not unhold
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
    
    // Updated connecting cba's   
    UpdateCbaL( EPhoneCallHandlingInCallCBA );

    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnecting );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );       
        
    EndUiUpdate();        
    }
    
// -----------------------------------------------------------
// CPhoneConferenceAndCallSetup::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneConferenceAndCallSetup::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneConferenceAndCallSetup::HandleConnectedL()");
    
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
 
    BeginUiUpdateLC();
        
    // Show bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
    
    // Capturing keys and number entry must be removed because some
    // networks jump over connecting state directly to connected state.
    CaptureKeysDuringCallNotificationL( EFalse );
    
    // Remove the number entry if it isn't DTMF dialer
    if ( !iOnScreenDialer || !IsNumberEntryVisibleL() || ! IsDTMFEditorVisibleL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }
    
    SetTouchPaneButtons( EPhoneConferenceAndSingleButtons );
    SetTouchPaneButtonDisabled( EPhoneInCallCmdPrivate );
    EndUiUpdate(); 
    
    UpdateCbaL ( EPhoneCallHandlingNewCallSwapCBA );
   
    iStateMachine->ChangeState( EPhoneStateConferenceAndSingle );                     
    }

// -----------------------------------------------------------
// CPhoneConferenceAndCallSetup:HandleIdleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndCallSetup::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndCallSetup::HandleIdleL()");
    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );

    // Stop capturing keys
    CaptureKeysDuringCallNotificationL( EFalse );

    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    TPhoneCmdParamBoolean conferenceExistsForCallId;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetCallExistsInConference,
        aCallId, &conferenceExistsForCallId );
    
    if( conferenceExistsForCallId.Boolean() )
        {
		// Remove 'Conference on hold' global note
	    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveGlobalNote );
    
        // Remove conference member from conference bubble
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveFromConference, 
            aCallId );
        
        //Check if removed call was last one to be able to be in conference mode
        //If there is no confrence call, go to single and call setup state
        TPhoneCmdParamBoolean conferenceExists;
        iViewCommandHandle->ExecuteCommandL( EPhoneViewGetIsConference, 
            &conferenceExists );
        
        __PHONELOG1( EBasic, EPhoneControl,
            "CPhoneConferenceAndCallSetup::HandleIdleL - conferenceExists =%d ",
            conferenceExists.Boolean() );
                
        if( !conferenceExists.Boolean() )
            {
            if( iAlerting )
                {
                iStateMachine->ChangeState( EPhoneStateAlertingInSingle ); 
                }
            else
                {
                iStateMachine->ChangeState( EPhoneStateCallSetupInSingle );
                }
            }
        }
    else
        {
        // Remove  outgoing call 
        BeginTransEffectLC( ENumberEntryOpen );
        BeginUiUpdateLC();
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
        CheckIfRestoreNEContentAfterDtmfDialer();
        if ( IsNumberEntryUsedL() )
            {
            // Show the number entry if it exists
            SetNumberEntryVisibilityL(ETrue);
            }
            
        SetTouchPaneButtons( EPhoneConferenceButtons );
        EndUiUpdate();
        EndTransEffect(); 
        UpdateCbaL( EPhoneCallHandlingInCallCBA );
        iStateMachine->ChangeState( EPhoneStateConference );
        }
    }
    
// -----------------------------------------------------------
// CPhoneConferenceAndCallSetup::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneConferenceAndCallSetup::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneConferenceAndCallSetup::UpdateInCallCbaL() ");

    if ( iAlerting )
        {
        UpdateCbaL( EPhoneCallHandlingInCallCBA );
        }
    else
        {
        UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
        }
    } 

// -----------------------------------------------------------
// CPhoneConferenceAndCallSetup:HandleConferenceIdleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndCallSetup::HandleConferenceIdleL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndCallSetup::HandleConferenceIdleL()");
        
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveConferenceBubble );
        
    // Fetch alerting call's id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnecting );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );

    if ( callStateData.CallId() > KErrNotFound )
        {
        // Set Hold flag to view EFalse that dtmf menu item not delete
        TPhoneCmdParamBoolean holdFlag;
        holdFlag.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
        // Go to alerting and single state
        UpdateCbaL( EPhoneCallHandlingInCallCBA );
        iStateMachine->ChangeState( EPhoneStateAlertingInSingle );     
        }
    else
        {
        // Go to callsetup and single state
        UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
        iStateMachine->ChangeState( EPhoneStateCallSetupInSingle );
        }           
    }

// End of File
