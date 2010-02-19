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
* Description: Implementation of CPhoneTwoSingles class.
*
*/


// INCLUDES
#include <eikmenub.h> 
#include <eikenv.h>
#include <StringLoader.h>
#include <pevirtualengine.h>
#include <mpeengineinfo.h>
#include <cpephonemodelif.h>
#include <featmgr.h>
#include "cphonetwosingles.h"
#include "mphonestatemachine.h"
#include "phoneviewcommanddefinitions.h"
#include "phoneui.hrh"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "phonestatedefinitions.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamappinfo.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamcallstatedata.h"
#include "phonestatedefinitionsgsm.h"
#include "phonelogger.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C CPhoneTwoSingles::CPhoneTwoSingles(
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneGsmInCall( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneTwoSingles::~CPhoneTwoSingles()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C CPhoneTwoSingles::~CPhoneTwoSingles()
    {
    }

// -----------------------------------------------------------
// CPhoneTwoSingles::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C void CPhoneTwoSingles::ConstructL()
    {
    CPhoneGsmInCall::ConstructL();
    
    // Re-enable global notes
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );
    }

// -----------------------------------------------------------
// CPhoneTwoSingles::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneTwoSingles* CPhoneTwoSingles::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneTwoSingles* self = new( ELeave ) CPhoneTwoSingles( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneTwoSingles::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneTwoSingles::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSingles::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageHeld:
            HandleHeldL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageConnectedConference:
            HandleConnectedConferenceL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageIncoming:
            HandleIncomingL( aCallId );
            break;

        default:
            CPhoneGsmInCall::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneTwoSingles::OpenMenuBarL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneTwoSingles::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSingles::OpenMenuBarL()");
    TInt resourceId;

    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
        {
        resourceId = EPhoneDtmfDialerMenubar;
        }
    if ( iOnScreenDialer && IsCustomizedDialerVisibleL() )
        {
        resourceId = CustomizedDialerMenuResourceIdL();
        }
    else if ( IsNumberEntryVisibleL() )
        {
        resourceId = EPhoneCallActiveAndHeldMenubarWithNumberEntry;
        }
    else
        {
        resourceId = EPhoneCallActiveAndHeldMenubar;
        }

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );

    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
    }
      
// -----------------------------------------------------------
// CPhoneTwoSingles:HandleIdleL
// -----------------------------------------------------------
//    
void CPhoneTwoSingles::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSingles::HandleIdleL()");
    BeginUiUpdateLC();

    // Remove call 
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

    TPhoneCmdParamInteger activeCallCount;
    iViewCommandHandle->ExecuteCommandL(
        EPhoneViewGetCountOfActiveCalls, &activeCallCount );

    switch( activeCallCount.Integer() )
        {
        case EOneActiveCall:
            {   
            if ( IsNumberEntryUsedL() )
                {
                // Show the number entry if it exists
                SetNumberEntryVisibilityL(ETrue);
                }
            else
                {
                // Close menu bar if number entry is not open.
                iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
                
                // Set incall CBAs
                UpdateCbaL( EPhoneCallHandlingInCallCBA );                
                }
            
            TPhoneCmdParamCallStateData callStateData;  
            callStateData.SetCallState( EPEStateHeld );
            iViewCommandHandle->HandleCommandL(
                EPhoneViewGetCallIdByState, &callStateData );
            TInt holdCallId = callStateData.CallId();
            
            // Set Hold flag to view
            TPhoneCmdParamBoolean holdFlag;
            
            if ( holdCallId < 0 )
                {
                holdFlag.SetBoolean( EFalse );
                }
            else
                {
                holdFlag.SetBoolean( ETrue );
                // Close the dtmf query if it exists
                CloseDtmfQueryL(); 
                }

            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    	    SetTouchPaneButtons( EPhoneIncallButtons );         
            // Go to single state
            // CBA updates in above if-else conditions
            iStateMachine->ChangeState( EPhoneStateSingle ); 
            }
            
        default:
            break;
        }
    EndUiUpdate(); 
    }

// -----------------------------------------------------------
// CPhoneTwoSingles::HandleKeyMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneTwoSingles::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSingles::HandleKeyMessageL()");
    switch ( aCode )
        {
        // send-key
        case EKeyYes:
            {
            // Check is number entry open
            if ( !IsNumberEntryVisibleL() )
                {
                iStateMachine->SendPhoneEngineMessage(
                    CPEPhoneModelIF::EPEMessageSwap );
                }
            else
                {
                CallFromNumberEntryL();
                }
            break;    
            }

        default:
            CPhoneGsmInCall::HandleKeyMessageL( aMessage, aCode );   
            break;
        }
    }
    
// -----------------------------------------------------------
// CPhoneTwoSingles::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneTwoSingles::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSingles::HandleConnectedL()");
    // Show bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
    }
    
// -----------------------------------------------------------
// CPhoneTwoSingles::HandleHeldL
// -----------------------------------------------------------
//
void CPhoneTwoSingles::HandleHeldL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSingles::HandleHeldL()");
    // Display hold bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateHeld );
    
    TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
    TInt callLabelId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCallOnHold );

    StringLoader::Load( 
        labelText, 
        callLabelId, 
        CCoeEnv::Static() );        
    callHeaderParam.SetLabelText( labelText );
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
    }
    
// -----------------------------------------------------------
// CPhoneTwoSingles::HandleNumberEntryClearedL()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneTwoSingles::HandleNumberEntryClearedL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSingles::HandleNumberEntryClearedL()");
    UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
    }

// -----------------------------------------------------------
// CPhoneTwoSingles::HandleConnectedConferenceL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneTwoSingles::HandleConnectedConferenceL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSingles::HandleConnectedConferenceL()");
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
    BeginUiUpdateLC();
    
    callHeaderParam.SetCiphering(
        iStateMachine->PhoneEngineInfo()->IsSecureCall( aCallId ) );
        
    callHeaderParam.SetCipheringIndicatorAllowed(
        iStateMachine->PhoneEngineInfo()->SecureSpecified() );
    
    // Service identifier must be given so that service specific settings
    // can be taken into account at phoneuiview.
    callHeaderParam.SetServiceId( 
        iStateMachine->PhoneEngineInfo()->ServiceId( aCallId ) );
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewCreateConference, aCallId,
        &callHeaderParam );

    // Set Hold flag to view
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );

    SetTouchPaneButtons( EPhoneConferenceButtons );
    SetTouchPaneButtonEnabled( EPhoneInCallCmdPrivate );
    EndUiUpdate();
    
    UpdateCbaL( EPhoneCallHandlingInCallCBA );
    
    // Go to conference state
    // No need for CBA update
    iStateMachine->ChangeState( EPhoneStateConference );
    }

// -----------------------------------------------------------
// CPhoneTwoSingles::HandleIncomingL
// -----------------------------------------------------------
//
void CPhoneTwoSingles::HandleIncomingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneTwoSingles::HandleIncomingL()");
    
    BeginUiUpdateLC();
    
    // Hide the number entry if it exists
    if ( IsNumberEntryUsedL() )
        {
        SetNumberEntryVisibilityL( EFalse );    
        }
    
    TPhoneCmdParamBoolean dialerParam;
    dialerParam.SetBoolean( ETrue );
    
    // Get allow waiting call header param value.
    AllowShowingOfWaitingCallHeaderL( dialerParam );
    
    // Close fast swap window if it's displayed
    CEikonEnv::Static()->DismissTaskList();

    // Display incoming call
    DisplayIncomingCallL( aCallId, dialerParam );
    
    // Set touch controls
    SetTouchPaneButtonDisabled( EPhoneCallComingCmdAnswer );
    
    // Check if HW Keys or Call UI should be disabled
    CheckDisableHWKeysAndCallUIL();
    
    SetTouchPaneButtons( EPhoneWaitingCallButtons );

    EndUiUpdate();

    // Go to incoming state
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
    iStateMachine->ChangeState( EPhoneStateTwoSinglesAndWaiting );        
    }

// -----------------------------------------------------------
// CPhoneTwoSingles::DisplayIncomingCallL
// -----------------------------------------------------------
//
void CPhoneTwoSingles::DisplayIncomingCallL( 
    TInt aCallId, 
    const TPhoneCmdParamBoolean aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates,
         "CPhoneTwoSingles::DisplayIncomingCallL()"); 
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    // Cannot delete active note, e.g. New call query, 
    // but show waiting note with or without caller name
    if ( IsAnyQueryActiveL() ||  
        ( !aCommandParam.Boolean() && iOnScreenDialer ) )
        {
        CallWaitingNoteL( aCallId );        
        }
    else
        {
        // Remove any phone dialogs if they are displayed
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
        }

    // Indicate that the Phone needs to be sent to the background if
    // an application other than the top application is in the foreground
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( !TopAppIsDisplayedL() );
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewSetNeedToSendToBackgroundStatus,
        &booleanParam );

    // Bring Phone app in the foreground
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );

    // Set Phone as the top application
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
        &uidParam );

    CPhoneState::DisplayHeaderForCallComingInL( aCallId, ETrue ); //waiting call 
    }
    
// -----------------------------------------------------------
// CPhoneTwoSingles::UpdateInCallCbaL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneTwoSingles::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneTwoSingles::UpdateInCallCbaL() ");
	UpdateCbaL( EPhoneCallHandlingNewCallSwapCBA );
    }

// End of File
