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
    // Reset flag 
    if ( iViewCommandHandle )
        {
        TPhoneCmdParamBoolean dtmfSendFlag;
        dtmfSendFlag.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetDtmfOptionsFlag, 
    	    &dtmfSendFlag );     	
        }
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
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateMaxConfMemberFlag );
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
	 		SendGlobalErrorNoteL( EPhoneNoteTextNotAllowed );  	 					
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
            iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateMaxConfMemberFlag );
            SetTouchPaneButtons( EPhoneConferenceAndHeldSingleButtons );
            break;
            
        case EPhoneInCallCmdJoin:
            iStateMachine->SendPhoneEngineMessage(
                CPEPhoneModelIF::EPEMessageAddConferenceMember );
            break;
        
        case EPhoneInCallCmdParticipants:
            commandStatus = CPhoneConference::HandleCommandL( aCommand );
            SetTouchPaneButtonDisabled( EPhoneInCallCmdPrivate );
            break;
        
        default:
            commandStatus = CPhoneConference::HandleCommandL( aCommand );
            break;
        }

    return commandStatus;
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::OpenMenuBarL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingle::OpenMenuBarL");
    
    // Set Conference And Single specific flag to view
    TPhoneCmdParamBoolean conferenceAndSingleFlag;
    conferenceAndSingleFlag.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetConferenceAndSingleFlag, 
    	&conferenceAndSingleFlag );
    
    // Set specific flag to view so that DTMF menu item available
    TPhoneCmdParamBoolean dtmfSendFlag;
    dtmfSendFlag.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetDtmfOptionsFlag, 
    	&dtmfSendFlag );    	
    
    TPhoneViewResponseId response;
    TPhoneCmdParamCallStateData callStateData;
    TPhoneCmdParamBoolean booleaParam;
    TInt heldCallId( KErrNotFound );
    TInt connectedCallId( KErrNotFound );
    
    // Fetch active call's id from view
    callStateData.SetCallState( EPEStateConnected );
    response = iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    if( response != EPhoneViewResponseSuccess )
        {
        // Unsuccesfull fetch - do nothing
        return;            
        }
    connectedCallId = callStateData.CallId();
    
    // Fetch held call's id from view
    callStateData.SetCallState( EPEStateHeld );
    response = iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    if( response != EPhoneViewResponseSuccess )
        {
        // Unsuccesfull fetch - do nothing
        return;            
        }
    heldCallId = callStateData.CallId();

    // Open the correct menubar according which call is active
    TInt resourceId;
    if( connectedCallId == KConferenceCallId )
        {
        if ( IsNumberEntryVisibleL() )
            {
            resourceId = EPhoneConfAndHeldCallMenubarWithNumberEntry;
            }
        else if ( IsConferenceBubbleInSelectionMode() )
            {
            resourceId = EPhoneConfCallParticipantsDropMenubar;    
            }
        else
            {
            resourceId = EPhoneConfAndHeldCallMenubar;
            }
        }
    else
        {
        if ( IsNumberEntryVisibleL() )
            {
            resourceId = EPhoneActiveAndHeldConfMenubarWithNumberEntry;
            }
        else if ( IsConferenceBubbleInSelectionMode() )
            {
            resourceId = EPhoneConfCallParticipantsDropMenubar;    
            }            
        else
            {
            resourceId = EPhoneActiveAndHeldConfMenubar;
            }            
        }
        
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateMaxConfMemberFlag );
    
    // Check that calls are the same type (f.ex. both are CS calls)
    booleaParam.SetBoolean( 
        heldCallId != KErrNotFound && connectedCallId != KErrNotFound &&
        iStateMachine->PhoneEngineInfo()->CallType( heldCallId ) != 
        iStateMachine->PhoneEngineInfo()->CallType( connectedCallId ));

    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetConfrenceOptionsFlag, 
        &booleaParam );

    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
        {
        resourceId = EPhoneDtmfDialerMenubar;
        }
    else if ( iOnScreenDialer && IsCustomizedDialerVisibleL() )
        {  
        resourceId = CustomizedDialerMenuResourceIdL();         
        }
            
    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
                CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( resourceId ) );
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::HandleConnectedConferenceL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleConnectedConferenceL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingle::HandleConnectedConferenceL");
    // Update call state
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );

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
    callHeaderParam.SetCallState( EPEStateHeld );
    
    TInt callLabelId;
    TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
    callLabelId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneCallOnHold );

    StringLoader::Load( 
        labelText, 
        callLabelId, 
        CCoeEnv::Static() );        
    callHeaderParam.SetLabelText( labelText );
         
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateMaxConfMemberFlag );

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
    
    UpdateInCallCbaL();
    
    
    // Display connected bubble
    TPhoneCmdParamCallHeaderData callHeaderParam;
    callHeaderParam.SetCallState( EPEStateConnected );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateBubble, aCallId, 
        &callHeaderParam );
    
    }

// -----------------------------------------------------------
// CPhoneConferenceAndSingle::HandleHeldL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleHeldL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingle::HandleHeldL");
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
// CPhoneConferenceAndSingle::HandleAddedConferenceMemberL
// -----------------------------------------------------------
//
void CPhoneConferenceAndSingle::HandleAddedConferenceMemberL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneConferenceAndSingle::HandleAddedConferenceMemberL");
    
    BeginUiUpdateLC();
        
    // Update conference bubble
    iViewCommandHandle->ExecuteCommandL( EPhoneViewAddToConference );
    
    UpdateConferenceSecurityStatusL( aCallId );

    SetTouchPaneButtons( EPhoneConferenceButtons );
    SetTouchPaneButtonEnabled( EPhoneInCallCmdPrivate );
    
    EndUiUpdate();

    TPhoneCmdParamBoolean conferenceAndSingleFlag;
    conferenceAndSingleFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetConferenceAndSingleFlag, 
    	&conferenceAndSingleFlag );

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
    
    // Get allow waiting call header param value.
    AllowShowingOfWaitingCallHeaderL( dialerParam );
    
    // Close conference list
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( 
    	EPhoneViewOpenConferenceList, &booleanParam );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewCloseFSW );
    
    // Set touch controls
    SetTouchPaneButtonDisabled( EPhoneCallComingCmdAnswer );

    // Check if HW Keys or Call UI should be disabled
    CheckDisableHWKeysAndCallUIL();

    SetTouchPaneButtons( EPhoneWaitingCallButtons );
        
    // Display incoming call
    DisplayIncomingCallL( aCallId, dialerParam  );

    EndUiUpdate();

    TPhoneCmdParamBoolean conferenceAndSingleFlag;
    conferenceAndSingleFlag.SetBoolean( EFalse );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetConferenceAndSingleFlag, 
    	&conferenceAndSingleFlag );
    
    // Go to Conference And Single And Waiting state
    UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
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
    
    // If dialler is not open then close menu bar.
    if ( !IsNumberEntryUsedL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
        }
    
    TPhoneCmdParamBoolean conferenceExistsForCallId;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetCallExistsInConference,
        aCallId, &conferenceExistsForCallId );
    
    if( conferenceExistsForCallId.Boolean() )
        {
        // Remove conference member from conference bubble
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveFromConference, 
            aCallId );                    

        iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateMaxConfMemberFlag );
        }
    else
        {       
        // Remove call
        BeginUiUpdateLC(); 
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );

	    TPhoneCmdParamBoolean conferenceAndSingleFlag;
	    conferenceAndSingleFlag.SetBoolean( EFalse );
	    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetConferenceAndSingleFlag, 
	    	&conferenceAndSingleFlag );
        
	    TPhoneCmdParamBoolean participantsVisibleFlag;
	    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetConferenceListVisibleFlag,
	            &participantsVisibleFlag );
	    
	    // if participants list is visible - do not change buttons or cba
	    if( !participantsVisibleFlag.Boolean() )
	        {
	        // Go to Conference state
	        SetTouchPaneButtons( EPhoneConferenceButtons );
	        SetTouchPaneButtonEnabled( EPhoneInCallCmdPrivate );  
	        
	        UpdateCbaL( EPhoneCallHandlingInCallCBA );
	        }
      

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
        // send-key
        case EKeyYes:
            if( IsNumberEntryVisibleL() )
				{
                HandleSendL();
                }
 			else
	 			{
                // Number entry is below so swap the call
                iStateMachine->SendPhoneEngineMessage(
                	CPEPhoneModelIF::EPEMessageSwap );        
	 			}
            break;
            
        default:
            // do base operation
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
    // Get the number entry contents
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
        // Send a manual control sequence by providing number 
        // information with dial command
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
