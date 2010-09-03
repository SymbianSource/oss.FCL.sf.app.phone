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
* Description: Implementation of CPhoneConferenceAndSingle class.
*
*/


// INCLUDES
#include <StringLoader.h>
#include <cpephonemodelif.h>
#include <mpeengineinfo.h>
#include <featmgr.h>
#include "cphoneconferenceandsingle.h"
#include "mphonestatemachine.h"
#include "phoneviewcommanddefinitions.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamstring.h"
#include "phonestatedefinitionsgsm.h"
#include "phonelogger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamcallstatedata.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "tphonecmdparamglobalnote.h"
#include "phoneui.hrh"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneConferenceAndSingle::CPhoneConferenceAndSingle( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization ) : 
    CPhoneConference( aStateMachine, aViewCommandHandle, aPhoneCustomization )
    {
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::~CPhoneConferenceAndSingle()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConferenceAndSingle::~CPhoneConferenceAndSingle()
    {
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::ConstructL()
    {
    CPhoneConference::ConstructL();
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneConferenceAndSingle* CPhoneConferenceAndSingle::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneConferenceAndSingle* self = new( ELeave ) CPhoneConferenceAndSingle( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneConferenceAndSingle::HandlePhoneEngineMessageL()");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageConnectedConference:
            HandleConnectedConferenceL();
            break;
            
        case MEngineMonitor::EPEMessageHeldConference:
            HandleHeldConferenceL();
            break;
            
        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageHeld:
            HandleHeldL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageInValidEmergencyNumber:
            SendGlobalErrorNoteL( EPhoneNoteTextNotAllowed, ETrue );
            break;
            
        case MEngineMonitor::EPEMessageValidEmergencyNumber:
            DialVoiceCallL();
            break;                          
           
        case MEngineMonitor::EPEMessageAddedConferenceMember:
            HandleAddedConferenceMemberL( aCallId );
            break;
            
        default:
            CPhoneConference::HandlePhoneEngineMessageL( aMessage, 
                aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle:HandleCommandL
// -----------------------------------------------------------
//
TBool CPhoneConferenceAndSingle::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingle::HandleCommandL");
    TBool commandStatus = ETrue;
    switch( aCommand )
        {
        case EPhoneNumberAcqCmdCall:
        case EPhoneNumberAcqCmdSendCommand:
            HandleSendL();
            break;
                    
        case EAknSoftkeyCancel:
            commandStatus = CPhoneConference::HandleCommandL( aCommand );
            SetTouchPaneButtons( EPhoneConferenceAndHeldSingleButtons );
            break;
            
        case EPhoneInCallCmdJoin:
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessageAddConferenceMember );
            break;
        
        case EPhoneInCallCmdParticipants:
            break;
        
        default:
            commandStatus = CPhoneConference::HandleCommandL( aCommand );
            break;
        }
    return commandStatus;
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::HandleConnectedConferenceL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleConnectedConferenceL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingle::HandleConnectedConferenceL");
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnectedConference );

    TBuf<KPhoneCallHeaderLabelMaxLength> conferenceText( KNullDesC );
    TInt callLabelId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCLIConferenceCall );

    StringLoader::Load( 
        conferenceText, 
        callLabelId, 
        CCoeEnv::Static() );        
    callHeaderParam.SetLabelText( conferenceText );
            
    // Conference call is no longer on hold
    TPhoneCmdParamBoolean holdFlag;
    holdFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetHoldFlag, &holdFlag );
    
    BeginUiUpdateLC();
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewUpdateBubble, KConferenceCallId, &callHeaderParam );
    
    SetTouchPaneButtons( EPhoneConferenceAndHeldSingleButtons );
    UpdateInCallCbaL();
    EndUiUpdate();
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::HandleHeldConferenceL
// 
// one of the calls is on hold all the time, conference or single call
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleHeldConferenceL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingle::HandleHeldConferenceL");
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateHeldConference );
    TInt callLabelId;
    TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
    callLabelId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCallOnHold );
    StringLoader::Load( 
        labelText, 
        callLabelId, 
        CCoeEnv::Static() );        
    callHeaderParam.SetLabelText( labelText );
    BeginUiUpdateLC();
    
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewUpdateBubble, KConferenceCallId, &callHeaderParam );
    SetTouchPaneButtons( EPhoneConferenceAndSingleButtons );
    
    EndUiUpdate();
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::HandleConnectedL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingle::HandleConnectedL");
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
    UpdateInCallCbaL();
    SetTouchPaneButtons( EPhoneConferenceButtons );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::HandleHeldL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleHeldL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingle::HandleHeldL");
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
    SetTouchPaneButtons( EPhoneConferenceButtons );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::HandleAddedConferenceMemberL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleAddedConferenceMemberL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingle::HandleAddedConferenceMemberL");
    BeginUiUpdateLC();
    iViewCommandHandle->ExecuteCommandL( EPhoneViewAddToConference );
    UpdateConferenceSecurityStatusL( aCallId );
    SetTouchPaneButtons( EPhoneConferenceButtons );
    EndUiUpdate();
    UpdateCbaL( EPhoneCallHandlingInCallCBA );
    iStateMachine->ChangeState( EPhoneStateConference );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneConferenceAndSingle::UpdateInCallCbaL() ");
    UpdateCbaL ( EPhoneCallHandlingNewCallSwapCBA );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::HandleIncomingL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleIncomingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndSingle::HandleIncomingL");
    BeginUiUpdateLC(); 
    
    TPhoneCmdParamBoolean dialerParam;
    dialerParam.SetBoolean( ETrue );
    AllowShowingOfWaitingCallHeaderL( dialerParam );
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewCloseFSW );
    
    CheckDisableHWKeysAndCallUIL();
    DisplayIncomingCallL( aCallId, dialerParam  );
    SetTouchPaneButtons( EPhoneWaitingCallButtons );
    
    EndUiUpdate();
    
    UpdateCbaL( EPhoneCallHandlingIncomingRejectCBA );
    iStateMachine->ChangeState( EPhoneStateConferenceAndSingleAndWaiting );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndSingle::HandleIdleL");
    TPhoneCmdParamBoolean conferenceExistsForCallId;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetCallExistsInConference,
        aCallId, &conferenceExistsForCallId );
    
    if( conferenceExistsForCallId.Boolean() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveFromConference, 
            aCallId );
        }
    else
        {
        BeginUiUpdateLC(); 
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
        SetTouchPaneButtons( EPhoneConferenceButtons );
        UpdateCbaL( EPhoneCallHandlingInCallCBA );
        EndUiUpdate();
        iStateMachine->ChangeState( EPhoneStateConference );
        }
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::HandleConferenceIdleL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleConferenceIdleL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingle::HandleConferenceIdleL");
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveConferenceBubble );
    TPhoneCmdParamInteger intParam;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetCountOfActiveCalls,
        &intParam );
        
    switch( intParam.Integer() )
        {
        case EOneActiveCall:
            MakeStateTransitionToSingleL();
            break;
            
        case ETwoActiveCalls: // Fall through
        default:
            MakeStateTransitionToTwoSinglesL();
            break;
        }
    }

// --------------------------------------------------------------
// CPhoneConferenceAndSingle::HandleKeyMessageL
// --------------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneConferenceAndSingle::HandleKeyMessageL()");
    switch ( aCode )
        {
        case EKeyYes: // send-key
            if( IsNumberEntryVisibleL() )
                {
                HandleSendL();
                }
            else
                {
                iStateMachine->SendPhoneEngineMessage(
                    CPEPhoneModelIF::EPEMessageSwap );
                }
            break;
            
        default:
            CPhoneConference::HandleKeyMessageL( aMessage, aCode );
            break;
        }
    }

// --------------------------------------------------------------
// CPhoneConferenceAndSingle::HandleSendL 
// --------------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleSendL()
    {
    HBufC *phoneNumber = HBufC::NewLC( KPhoneNumberEntryBufferSize );
    TPtr ptr( phoneNumber->Des() );
    TPhoneCmdParamString stringParam;
    stringParam.SetString( &ptr );
    iViewCommandHandle->ExecuteCommand(
        EPhoneViewGetNumberFromEntry,
        &stringParam );
    iStateMachine->PhoneEngineInfo()->SetPhoneNumber( ptr ) ;
    
    if ( iStateMachine->PhoneEngineInfo()->PhoneNumberIsServiceCode() || 
         phoneNumber->Des().Length() < KPhoneValidPhoneNumberLength )
        {
        CallFromNumberEntryL();
        }
    else
        {
        iStateMachine->SendPhoneEngineMessage( 
            MPEPhoneModel::EPEMessageCheckEmergencyNumber );
        }   
    CleanupStack::PopAndDestroy( phoneNumber );
    }

// End of File
