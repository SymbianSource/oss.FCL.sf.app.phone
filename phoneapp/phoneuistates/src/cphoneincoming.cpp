/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneIncoming class.
*
*/


// INCLUDES
#include <featmgr.h>
#include "cphoneincoming.h"
#include "phonerssbase.h"
#include "phonelogger.h"
#include "cphonegeneralgsmmessageshandler.h"
#include "phonestatedefinitionsgsm.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "phoneui.hrh"
#include "mphonestatemachine.h"
#include "mphonesecuritymodeobserver.h"
#include "tphonecmdparamcallstatedata.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C CPhoneIncoming::CPhoneIncoming( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneStateIncoming( aStateMachine, aViewCommandHandle, aPhoneCustomization ),
    iWaitingCallId( KErrNotFound )
    {
    }

// -----------------------------------------------------------
// CPhoneIncoming::~CPhoneIncoming()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C CPhoneIncoming::~CPhoneIncoming()
    {
    }

// -----------------------------------------------------------
// CPhoneIncoming::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C void CPhoneIncoming::ConstructL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneIncoming::ConstructL()");
    CPhoneStateIncoming::ConstructL();
    }

// -----------------------------------------------------------
// CPhoneIncoming::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneIncoming* CPhoneIncoming::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneIncoming* self = new (ELeave) 
        CPhoneIncoming( aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneIncoming::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneIncoming::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, 
        "CPhoneIncoming::HandlePhoneEngineMessageL() ");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageIncoming:
            iWaitingCallId = aCallId;
            break;

        case MEngineMonitor::EPEMessageConnected:
            if( iWaitingCallId > KErrNotFound )
                {
                HandleIncomingL( iWaitingCallId );
                }
            else
                {
                // There is no another incoming call
                CPhoneStateIncoming::HandlePhoneEngineMessageL( 
                    aMessage, aCallId );
                }
            break;
            
        case MEngineMonitor::EPEMessageRemoteHeld:
                SendGlobalInfoNoteL( EPhoneInformationRemotePutOnHoldNote );
            break;
        
        case MEngineMonitor::EPEMessageRemoteResumed:
                SendGlobalInfoNoteL( EPhoneInformationConnectedNote );
            break;
		
		case MEngineMonitor::EPEMessageShowVersion:
			{
			if ( iStateMachine->SecurityMode()->IsSecurityMode() )
				{
				// Do nothing if security mode is enabled.
				return;
				}
			}
		// Fall through

        // fall through.
        case MEngineMonitor::EPEMessageIssuingSSRequest:
        case MEngineMonitor::EPEMessageCallBarred:
        case MEngineMonitor::EPEMessageIssuedSSRequest:
        case MEngineMonitor::EPEMessageTempClirActivationUnsuccessful:
        case MEngineMonitor::EPEMessageIncCallIsForw:
        case MEngineMonitor::EPEMessageIncCallForwToC:
        case MEngineMonitor::EPEMessageOutCallForwToC:
        case MEngineMonitor::EPEMessageForwardUnconditionalModeActive:
        case MEngineMonitor::EPEMessageForwardConditionallyModeActive:
            {
            CPhoneGeneralGsmMessagesHandler* gsmMsgHandler =
                CPhoneGeneralGsmMessagesHandler::NewL( *iStateMachine,
                                                       *iViewCommandHandle,
                                                       *this );
            CleanupStack::PushL( gsmMsgHandler );
            gsmMsgHandler->HandlePhoneEngineMessageL( aMessage, aCallId );
            CleanupStack::PopAndDestroy( gsmMsgHandler );
            }
            break;
            
        case MEngineMonitor::EPEMessageIdle:
           {
           // Check if there is another incoming call while the first one is
           // still disconnecting
           if ( iWaitingCallId > KErrNotFound && aCallId != iWaitingCallId )
               {
               HandleIncomingL( iWaitingCallId );
               
               // Prevent execution of CPhoneStateIncoming::HandleIdleL, only remove
               // the call bubble
               BeginUiUpdateLC();
               iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
               EndUiUpdate();

               iWaitingCallId = KErrNotFound;
               }
           else
               {
               CPhoneStateIncoming::HandlePhoneEngineMessageL( 
                   aMessage, aCallId );
               }
           }
           break;

        default:
            CPhoneStateIncoming::HandlePhoneEngineMessageL( 
                aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
//  CPhoneIncoming::HandleIncomingL
// -----------------------------------------------------------
void CPhoneIncoming::HandleIncomingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneIncoming::HandleIncomingL()");
    
    TPhoneCmdParamCallStateData callState;
    callState.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetCallIdByState, &callState );
    TInt connectedCall = callState.CallId(); 
    
    IsNumberEntryUsedL() ? 
        BeginTransEffectLC( ECallUiAppear ) :
        BeginTransEffectLC( ENumberEntryOpen );
    BeginUiUpdateLC();
    
    // Hide the number entry if it exists
    if ( IsNumberEntryUsedL() )
        {
        SetNumberEntryVisibilityL( EFalse );    
        }
    
    TPhoneCmdParamBoolean dialerParam;
    dialerParam.SetBoolean( ETrue );
    
    AllowShowingOfWaitingCallHeaderL( dialerParam );
      
    // Close fast swap window if it's displayed
    CEikonEnv::Static()->DismissTaskList();
    
    // If the 1st incoming call became Connected, this is waiting call
    // If the 1st incoming call went just Idle, this is a normal call
    if ( connectedCall > KErrNotFound )
        {
        SetTouchPaneButtons( EPhoneWaitingCallButtons );
        }
    else
        {
        SetTouchPaneButtons( EPhoneIncomingCallButtons );
        }
    
    if ( KErrNotFound == connectedCall )
        {
        dialerParam.SetBoolean( EFalse );
        }
    
    SetToolbarDimming( EFalse );
    // Display incoming call
    DisplayIncomingCallL( aCallId, dialerParam );

    EndUiUpdate();
    EndTransEffect();

    if ( connectedCall > KErrNotFound )
        {
        iCbaManager->UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
        iStateMachine->ChangeState( EPhoneStateWaitingInSingle );   
        }
    }
    
// -----------------------------------------------------------
// CPhoneIncoming::DisplayIncomingCallL
// -----------------------------------------------------------
//
void CPhoneIncoming::DisplayIncomingCallL( 
    TInt aCallId, 
    const TPhoneCmdParamBoolean aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneIncoming::DisplayIncomingCallL()");
 
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    // Remove any phone dialogs if they are displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );

    // if keys have been locked, disable keylock without information note
    if ( IsKeyLockOn() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewDisableKeyLockWithoutNote );
        }
    
    // Indicate that the Phone needs to be sent to the background if
    // an application other than the top application is in the foreground
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( !TopAppIsDisplayedL() );
    iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewSetNeedToReturnToForegroundAppStatus,
        &booleanParam );

    // Bring Phone app in the foreground
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );

    // Set Phone as the top application
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
        &uidParam );

    DisplayHeaderForCallComingInL( aCallId, aCommandParam.Boolean() ); //waiting call 
    }    

// -----------------------------------------------------------
// CPhoneIncoming::AllowShowingOfWaitingCallHeaderL
// -----------------------------------------------------------
//
void CPhoneIncoming::AllowShowingOfWaitingCallHeaderL( 
    TPhoneCmdParamBoolean& aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIStates, "CPhoneIncoming::AllowShowingOfWaitingCallHeaderL() ");

    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewAllowWaitingCallHeader, 
        &aCommandParam );
    
    // Non-touch :Hide number entry if it exists on 
    // Touch : an internal operation ongoing 
    // -> do not hide dialer
    if ( !iOnScreenDialer )
        {   
        SetNumberEntryVisibilityL(EFalse);
        }
    // If param is true and number entry is open only then
    // hide number entry.
    else if ( aCommandParam.Boolean() && IsNumberEntryUsedL() )
        {
        SetNumberEntryVisibilityL(EFalse);
        }
    }

// -----------------------------------------------------------
// CPhoneIncoming::HandleLongHashL
// -----------------------------------------------------------
//
 void CPhoneIncoming::HandleLongHashL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneIncoming::HandleLongHashL( ) ");
    }

 // -----------------------------------------------------------
 // CPhoneIncoming::HandleErrorL
 // -----------------------------------------------------------
 //
 EXPORT_C void CPhoneIncoming::HandleErrorL( 
         const TPEErrorInfo& aErrorInfo )
     {
     __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneIncoming::HandleErrorL()");
     
     if( aErrorInfo.iErrorCode == ECCPErrorCCUserAlertingNoAnswer )
         {
         // Should not shown "No Answer" note
         __PHONELOG1( EBasic, EPhoneUIStates,
        "PhoneUIControl: CPhoneIncoming::HandleErrorL - aErrorInfo.iErrorCode =%d ",
             aErrorInfo.iErrorCode);
         }
     else
         {
         CPhoneState::HandleErrorL( aErrorInfo );
         }
     }
 
// End of File
