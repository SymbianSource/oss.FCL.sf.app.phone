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
* Description: Implementation of CPhoneTwoSinglesAndWaiting class.
*
*/


// INCLUDES
#include <pevirtualengine.h>
#include <StringLoader.h>
#include <mpeengineinfo.h>
#include <featmgr.h>
#include "cphonetwosinglesandwaiting.h"
#include "tphonecmdparamboolean.h"
#include "mphonestatemachine.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamstring.h"
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
CPhoneTwoSinglesAndWaiting::CPhoneTwoSinglesAndWaiting( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneTwoSingles( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::~CPhoneTwoSinglesAndWaiting()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneTwoSinglesAndWaiting::~CPhoneTwoSinglesAndWaiting()
    {
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneTwoSinglesAndWaiting::ConstructL()
    {
    CPhoneTwoSingles::ConstructL();
    
    // Fetch ringing call's id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    iRingingCallId = callStateData.CallId();
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneTwoSinglesAndWaiting* CPhoneTwoSinglesAndWaiting::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneTwoSinglesAndWaiting* self = new( ELeave ) CPhoneTwoSinglesAndWaiting( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::HandleKeyMessageL
// -----------------------------------------------------------
//
void CPhoneTwoSinglesAndWaiting::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSinglesAndWaiting::HandleKeyMessageL()");
    switch ( aCode )
        {
        // send-key
        case EKeyYes:
			{
            if( IsNumberEntryVisibleL() )
                {
                CallFromNumberEntryL();    	 					
                }
            else
                {
                // Waiting call bubble is over number entry
                // so display not allowed note because we
                // can't answer the call
                SendGlobalWarningNoteL( EPhoneNoteTextCallNotAllowed );		 				
                }
            break;
			}

        default:
            CPhoneTwoSingles::HandleKeyMessageL( aMessage, aCode );   
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneTwoSinglesAndWaiting::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSinglesAndWaiting::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;
            
        default:
            CPhoneTwoSingles::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::HandleErrorL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneTwoSinglesAndWaiting::HandleErrorL( 
        const TPEErrorInfo& aErrorInfo )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneTwoSinglesAndWaiting::HandleErrorL()");
    
    if( aErrorInfo.iErrorCode == ECCPErrorCCUserAlertingNoAnswer )
        {
        // Should not shown "No Answer" note
        __PHONELOG1( EBasic, EPhoneUIStates,
       "PhoneUIControl: CPhoneTwoSinglesAndWaiting::HandleErrorL - aErrorInfo.iErrorCode =%d ",
            aErrorInfo.iErrorCode);
        }
    else
        {
        CPhoneState::HandleErrorL( aErrorInfo );
        }
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneTwoSinglesAndWaiting::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSinglesAndWaiting::OpenMenuBarL()");

    TInt resourceId ( EPhoneCallTwoSinglesWaitingLockMenubar );
    
    if ( !IsAutoLockOn() )
        {
        if ( IsNumberEntryVisibleL() )
            {
            resourceId = EPhoneCallActiveHeldAndWaitingMenubarWithNumberEntry;
            }
        else
            {
            resourceId = EPhoneCallActiveHeldAndWaitingMenubar;
            }
        }   

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneTwoSinglesAndWaiting::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSinglesAndWaiting::HandleIdleL()");
        
    // Effect is shown when dialer exist.
    
    TBool effectStarted ( EFalse );
    if ( !NeedToReturnToForegroundAppL() )
        {
        BeginTransEffectLC( ENumberEntryOpen );
        effectStarted = ETrue;   
        }
 
    BeginUiUpdateLC();    
         
    if ( !IsNumberEntryUsedL() )
        {
        // Close menu bar, if number entry isnt open.
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );       
        }
    
    // Remove call 
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
    
    if ( iRingingCallId == aCallId )
        {
        StateTransitionToTwoSinglesL();    
        }
    else
        {
        StateTransitionToSingleAndWaitingL();
        }

    EndUiUpdate();
    if ( effectStarted )
        {
        EndTransEffect();
        }
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::StateTransitionToTwoSinglesL
// -----------------------------------------------------------
//
void CPhoneTwoSinglesAndWaiting::StateTransitionToTwoSinglesL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSinglesAndWaiting::StateTransitionToTwoSinglesL()");
    if ( IsNumberEntryUsedL() )
        {
        // Go to background if necessary
        if ( NeedToReturnToForegroundAppL() )
            {
            // Return phone to the background if menu application is needed to foreground.
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );

            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetControlAndVisibility );
            
            UpdateCbaL( EPhoneCallHandlingInCallCBA );
            }
        else
            {
            // Show the number entry if it exists.
            SetNumberEntryVisibilityL(ETrue);
            }
        } 
    else
        {
        // Set Two singles softkeys
        UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
	    
        // If numberentry is not open just check NeedToReturnToForegroundAppL and 
        // sendbackround if needed.
        if ( NeedToReturnToForegroundAppL() )
            {
            // Return phone to the background if send to background is needed.
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
            }
        }

    // Go to two singles state
    SetTouchPaneButtonEnabled( EPhoneCallComingCmdAnswer );
    SetTouchPaneButtons( EPhoneTwoSinglesButtons );
    // CBA updates in above if-else conditions
    iStateMachine->ChangeState( EPhoneStateTwoSingles );
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::StateTransitionToSingleAndWaitingL
// -----------------------------------------------------------
//
void CPhoneTwoSinglesAndWaiting::StateTransitionToSingleAndWaitingL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSinglesAndWaiting::StateTransitionToSingleAndWaitingL()");
    // Go to two singles state
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
  
    // Set touch controls
    SetTouchPaneButtonEnabled( EPhoneCallComingCmdAnswer );
    
    SetTouchPaneButtons( EPhoneWaitingCallButtons );
  
    iStateMachine->ChangeState( EPhoneStateWaitingInSingle );                             
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::HandleConnectedConferenceL
// -----------------------------------------------------------
//
void CPhoneTwoSinglesAndWaiting::HandleConnectedConferenceL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSinglesAndWaiting::HandleConnectedConferenceL()");
    BeginUiUpdateLC();
    // Update call state
    TPhoneCmdParamCallHeaderData callHeaderParam;

    TInt callLabelId;
    TBuf<KPhoneCallHeaderLabelMaxLength> conferenceText( KNullDesC );
    callLabelId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCLIConferenceCall );

    StringLoader::Load( 
        conferenceText, 
        callLabelId, 
        CCoeEnv::Static() );        
    callHeaderParam.SetCLIText( conferenceText, CBubbleManager::ERight );
    
    callHeaderParam.SetCiphering(
        iStateMachine->PhoneEngineInfo()->IsSecureCall( aCallId ) );
        
    callHeaderParam.SetCipheringIndicatorAllowed(
        iStateMachine->PhoneEngineInfo()->SecureSpecified() );

    // Service identifier must be given so that service specific settings
    // can be taken into account at phoneuiview.
    callHeaderParam.SetServiceId( 
        iStateMachine->PhoneEngineInfo()->ServiceId( aCallId ) );
    
    // Set touch controls
    SetTouchPaneButtonEnabled( EPhoneCallComingCmdAnswer );

    SetTouchPaneButtons( EPhoneWaitingCallButtons );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewCreateConference, aCallId,
        &callHeaderParam );
        
    // Clear the flag
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( 
         EPhoneViewSetNeedToReturnToForegroundAppStatus,
         &booleanParam );
        
    EndUiUpdate();
    // Go to Conference And Waiting state
    // No need for CBA update
    iStateMachine->ChangeState( EPhoneStateConferenceAndWaiting );                
    }
    
// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneTwoSinglesAndWaiting::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSinglesAndWaiting::UpdateInCallCbaL()");
	UpdateCbaL ( EPhoneCallHandlingCallWaitingCBA );   
    }

// -----------------------------------------------------------
// CPhoneTwoSinglesAndWaiting::HandleNumberEntryClearedL()
// -----------------------------------------------------------
//
void CPhoneTwoSinglesAndWaiting::HandleNumberEntryClearedL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSinglesAndWaiting::HandleNumberEntryClearedL()");
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
    }
    
// End of File
