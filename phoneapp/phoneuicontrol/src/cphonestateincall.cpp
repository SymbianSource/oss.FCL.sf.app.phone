/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneStateInCall class.
*
*/


// INCLUDES
#include <featmgr.h>
#include <StringLoader.h>
#include <PSVariables.h>
#include <hwrmdomainpskeys.h>
#include <UikonInternalPSKeys.h>
#include <mpeengineinfo.h>
#include <videotelcontrolmediatorapi.h>
#include <MediatorDomainUIDs.h>
#include <bldvariant.hrh>
#include <coreapplicationuisdomainpskeys.h>

#include "cphonestateincall.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamnote.h"
#include "tphonecmdparamquery.h"
#include "tphonecmdparamsingleitemfetch.h"
#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamsfidata.h"
#include "mphonestatemachine.h"
#include "mphonesecuritymodeobserver.h"
#include "phonestatedefinitions.h"
#include "phoneviewcommanddefinitions.h"
#include "phoneui.hrh"
#include "phonerssbase.h"
#include "cphonemainresourceresolver.h"
#include "phonelogger.h"
#include "phoneui.pan"
#include "cphonepubsubproxy.h"
#include "cphonedtmfwaitchartimer.h"
#include "tphonecmdparamcallheaderdata.h"
#include "cphonemediatorfactory.h"
#include "cphonemediatorsender.h"
#include "cphoneterminateallconnectionscommand.h"
#include "mphonecustomization.h"
#include "easydialingcommands.hrh"


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C CPhoneStateInCall::CPhoneStateInCall( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aCustomization ) : 
    CPhoneState( aStateMachine, aViewCommandHandle, aCustomization ),
    iDtmfWaitCharTimer( NULL )
    {
    }

// -----------------------------------------------------------
// CPhoneStateInCall::~CPhoneStateInCall()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C CPhoneStateInCall::~CPhoneStateInCall()
    {
    delete iDtmfWaitCharTimer;            
    }

// -----------------------------------------------------------
// CPhoneStateInCall::ConstructL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::ConstructL()
    {
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );
    CPhoneState::BaseConstructL();
    // Enable the volume display
    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNaviPaneAudioVolume );
    }

// -----------------------------------------------------------
// CPhoneStateInCall::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneStateInCall* CPhoneStateInCall::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneStateInCall* self = new (ELeave) CPhoneStateInCall( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneStateInCall::HandleKeyMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::HandleKeyMessageL()" ); 
    switch ( aCode )
        {
        // end-key
        case EKeyNo:
            HandleEndKeyPressL( aMessage );
            break;

        // Voice key
        case EKeyDevice6:
            HandleVoiceKeyPressL( aMessage ); 
            break;
            
#ifdef RD_INTELLIGENT_TEXT_INPUT
        case EKeyEnter:
            if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
                {
                SendDtmfL();    
                }
            else if ( IsNumberEntryVisibleL() )
                {
                if ( IsDialingExtensionInFocusL() )
                    {
                    iViewCommandHandle->HandleCommandL(
                                       EEasyDialingEnterKeyAction );
                    }
                else
                    {
                    HandleCommandL( EPhoneCmdOptions );
                    }
                }
            break;
#endif            
          
        default:
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneStateInCall::HandleNumberEntryClearedL()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::HandleNumberEntryClearedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::HandleNumberEntryClearedL()" );
    // Update single call CBA when number entry is cleared
     UpdateInCallCbaL();
    }

// -----------------------------------------------------------
// CPhoneStateInCall::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::HandlePhoneEngineMessageL()" );
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;

        case MEngineMonitor::EPEMessageAudioMuteChanged:
            HandleAudioMuteChangedL();
            break;
            
        case MEngineMonitor::EPEMessageAudioOutputChanged:
            HandleAudioOutputChangedL();
            break;
        
        case MEngineMonitor::EPEMessageSendingDTMF:
        case MEngineMonitor::EPEMessageContinuingDTMFSending:
            HandleSendingDTMFL();
            break;

        case MEngineMonitor::EPEMessageStoppedDTMF:
            HandleStoppedDTMFL();
            break;

        case MEngineMonitor::EPEMessageSentDTMF:
        case MEngineMonitor::EPEMessageDTMFSendingAborted:
            CancelDTMFSendingL();
            break;

        case MEngineMonitor::EPEMessagePromptSpeedDial:
            HandleDTMFPromptSpeedDialL();
            break;

        case MEngineMonitor::EPEMessageSpeedDialNotAssigned:
            CPhoneState::SendGlobalErrorNoteL( 
                EPhoneDtmfSpeedDialNotAssigned );
            break;

        case MEngineMonitor::EPEMessageInvalidSpeedDial:
            CPhoneState::SendGlobalErrorNoteL( EPhoneDtmfInvalidSpeedDial );
            break;
            
        case MEngineMonitor::EPEMessageAudioVolumeChanged:
            HandleAudioVolumeChangedL();
            break;
            
        case MEngineMonitor::EPEMessageDisconnecting:
            CancelDTMFSendingL();
            if ( !CPhoneState::IsAnyConnectedCalls() )
                {
                CloseDtmfQueryL();
                }            
            CPhoneState::HandlePhoneEngineMessageL( aMessage, aCallId );
            break; 
            
        case MEngineMonitor::EPEMessageInValidEmergencyNumber:
            SendGlobalErrorNoteL( EPhoneNoteTextNotAllowed );
            break;
            
        case MEngineMonitor::EPEMessageValidEmergencyNumber:
            DialVoiceCallL();
            break;    
        
        case MEngineMonitor::EPEMessageRemotePartyInfoChanged:
            // If there is a waiting call then update header and label
            // else forward message to CPhoneState.
            if ( IsWaitingCallL( aCallId ) )
                {
                // Update remote info data and label.
                UpdateRemoteInfoDataAndLabelL( aCallId, UpdateCallHeaderInfoL( aCallId ) );
                }
            else
                {
                CPhoneState::HandlePhoneEngineMessageL( aMessage, aCallId );
                }
            break;
            
        default:
            CPhoneState::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneStateInCall::UpdateRemoteInfoDataAndLabelL
// -----------------------------------------------------------
//
void CPhoneStateInCall::UpdateRemoteInfoDataAndLabelL( 
        TInt aCallId, 
        TPhoneCmdParamCallHeaderData aCallHeaderParam )
    {
    __LOGMETHODSTARTEND(EPhoneControl,
            "CPhoneStateInCall::UpdateRemoteInfoDataAndLabelL ()" );
    // Update the remote info data in the call header
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewUpdateCallHeaderRemoteInfoDataAndLabel, 
        aCallId,
        &aCallHeaderParam );
    }

// -----------------------------------------------------------
// CPhoneStateInCall::HandleIdleL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::HandleIdleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::HandleIdleL()" );
    __ASSERT_DEBUG( aCallId >= 0, Panic( EPhoneCtrlParameterNotInitialized ) );
    // Remove call 
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
  
    //Make sure that toolbar is not shown
    iViewCommandHandle->ExecuteCommandL( EPhoneViewHideToolbar );
    BeginUiUpdateLC();
    SetDefaultFlagsL();
    if ( IsNumberEntryUsedL() )
        {
        if ( NeedToReturnToForegroundAppL() )
            {
            // Return phone to the background if menu application is needed to foreground.
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
    
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetControlAndVisibility );
    
            // Set Number Entry CBA
            iCbaManager->SetCbaL( EPhoneNumberAcqCBA );
            }
        else
            {
            // Show the number entry if it exists and update cba's.
            SetNumberEntryVisibilityL( ETrue );
            
            // Close dtmf dialer when call is disconnected.
            if ( IsDTMFEditorVisibleL() )
                {      
                CloseDTMFEditorL();
                
                // Display idle screen and update CBA's
                DisplayIdleScreenL();
                }
            else if ( iOnScreenDialer && IsCustomizedDialerVisibleL() )
                {            
                CloseCustomizedDialerL();
                // Display idle screen and update CBA's
                DisplayIdleScreenL();
                } 
            }
        }
    else
        {
        // Close menu bar, if it is displayed
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
        
        if ( !TopAppIsDisplayedL() || IsAutoLockOn() || NeedToReturnToForegroundAppL() )
            {        
            // Continue displaying current app but set up the 
            // idle screen in the background
            SetupIdleScreenInBackgroundL();
            }
        else
            {
            // Display idle screen and update CBAs
            DisplayIdleScreenL();
            }
        }
        
    DeleteTouchPaneButtons();        
    EndUiUpdate();
    
    // Display call termination note, if necessary
    DisplayCallTerminationNoteL();

    // Go to idle state
    iStateMachine->ChangeState( EPhoneStateIdle );
    }

// -----------------------------------------------------------
// CPhoneStateInCall::UpdateInCallCbaL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateInCall::UpdateInCallCbaL() ");
    UpdateCbaL( EPhoneCallHandlingInCallCBA );
    }

// -----------------------------------------------------------
// CPhoneStateInCall::UpdateCbaL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::UpdateCbaL( TInt aResource )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateInCall::UpdateCbaL() ");
    if ( iOnScreenDialer && IsNumberEntryVisibleL() && IsDTMFEditorVisibleL() )
        {
        iCbaManager->SetCbaL( EPhoneDtmfDialerCBA );        
        }
    else if ( iOnScreenDialer && IsNumberEntryVisibleL() && IsCustomizedDialerVisibleL() )
        {
        iCbaManager->SetCbaL( CustomizedDialerCbaResourceIdL() );
        }
    else
        {
        iCbaManager->UpdateCbaL( aResource );   
        }
    }

// -----------------------------------------------------------
// CPhoneStateInCall::HandleAudioMuteChangedL
// -----------------------------------------------------------
//
void CPhoneStateInCall::HandleAudioMuteChangedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::HandleAudioMuteChangedL()" );
    CPhoneState::HandleAudioMuteChangedL();

    // Update the single call CBA only if the number entry is not
    // used
    if ( !IsNumberEntryUsedL() )
        {
        // Go to current state implementation
        UpdateInCallCbaL();
        }
    }

// -----------------------------------------------------------
// CPhoneStateInCall::HandleAudioOutputChangedL
// -----------------------------------------------------------
//
void CPhoneStateInCall::HandleAudioOutputChangedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::HandleAudioOutputChangedL()" );
    // Handle the handsfree mode change
    CPhoneState::HandleAudioOutputChangedL();

    // Update the single call CBA only if the number entry is not
    // used
    if ( !IsNumberEntryVisibleL() )
        {
        // Go to current state implementation
        UpdateInCallCbaL();
        }
    }

// -----------------------------------------------------------
// CPhoneStateInCall::HandleAudioVolumeChangedL
// -----------------------------------------------------------
//
void CPhoneStateInCall::HandleAudioVolumeChangedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::HandleAudioVolumeChangedL()" );
    // Update the volume display
    TInt audioVolume = iStateMachine->PhoneEngineInfo()->AudioVolume();
    TPhoneCmdParamInteger volumeParam;
    volumeParam.SetInteger( audioVolume );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetNaviPaneAudioVolume,
        &volumeParam );
    }
    
// -----------------------------------------------------------
// CPhoneStateInCall::HandleSendingDTMFL
// -----------------------------------------------------------
//
void CPhoneStateInCall::HandleSendingDTMFL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::HandleSendingDTMFL()" );
    TPhoneCmdParamNote noteParam;
    noteParam.SetType( EPhoneNoteDtmfSending );
    noteParam.SetResourceId( CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneSendingDtmfWaitNote ) );

    // Show the "Sending..." Wait Note, and pre-populate it 
    // with the PhoneEngine's current DTMF String.
    // Set the text for the wait-note.
    TPEDtmfString dtmfString = iStateMachine->PhoneEngineInfo()->DtmfString();
    HBufC* noteText = StringLoader::LoadLC( 
        CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneSendingDtmfWaitNoteText ), 
        dtmfString );
    noteParam.SetText( *noteText );

    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNote, &noteParam );

    CleanupStack::PopAndDestroy( noteText );
    }

// -----------------------------------------------------------
// CPhoneStateInCall::HandleStoppedDTMFL
// -----------------------------------------------------------
//
void CPhoneStateInCall::HandleStoppedDTMFL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::HandleStoppedDTMFL()" );
    // Remove the Sending... note
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );

    // Fetch the remaining (unparsed) portion of the DTMF String 
    // from PhoneEngine
    TPEDtmfString dtmfString = iStateMachine->PhoneEngineInfo()->DtmfString();

    if ( dtmfString.Length() )
        {
        // Show the DTMF Wait Character confirmation query
        TPhoneCmdParamQuery queryParam;
        queryParam.SetQueryType( EPhoneQueryDialog );
        queryParam.SetQueryResourceId( 
            CPhoneMainResourceResolver::Instance()->ResolveResourceID( 
                EPhoneDtmfWaitCharacterConfirmationQuery ) );

        // Append the PhoneEngine's current DTMF String to the query's prompt
        HBufC* queryPrompt = StringLoader::LoadLC( 
            CPhoneMainResourceResolver::Instance()->ResolveResourceID( 
                EPhoneDtmfWaitCharacterConfirmationQueryText ), 
            dtmfString );
        queryParam.SetQueryPrompt( *queryPrompt );

        iViewCommandHandle->ExecuteCommandL( EPhoneViewShowQuery, 
            &queryParam );

        CleanupStack::PopAndDestroy( queryPrompt );
        }
    else
        {
        // Stop the asynchronous sending operation the 
        // PhoneEngine may be performing.
        iStateMachine->SendPhoneEngineMessage( 
            MPEPhoneModel::EPEMessageStopDTMFSending );

        // Prompt for more characters by showing DTMF query with empty string
        HBufC* emptyString = HBufC::NewLC( KPhoneNumberEntryBufferSize );
        TPtr ptr( emptyString->Des() );

        ShowDtmfTextQueryL( 
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneDtmfNumberQuery ),
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneSendDtmfEmptyEditBoxCBA ),
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneSendDtmfNormalEditBoxCBA ),
            &ptr );

        CleanupStack::PopAndDestroy( emptyString );
        }
    }

// -----------------------------------------------------------
// CPhoneStateInCall::CancelDTMFSendingL
// -----------------------------------------------------------
//
void CPhoneStateInCall::CancelDTMFSendingL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::CancelDTMFSendingL()" );
    // Remove the Sending... note
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveDtmfNote );
    }

// -----------------------------------------------------------
// CPhoneStateInCall::HandleDtmfPromptSpeedDialL
// -----------------------------------------------------------
//
void CPhoneStateInCall::HandleDTMFPromptSpeedDialL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::HandleDTMFPromptSpeedDialL()" );
    // Remove the Sending... note
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );

    // Prompt for speed dial number
    HBufC* emptyString = HBufC::NewLC( KPhoneNumberEntryBufferSize );
    TPtr ptr( emptyString->Des() );

    // If the Search softkey is selected before a speed dial number is entered,
    // revert back to the DTMF query   
    ShowDtmfTextQueryL( 
        CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneDtmfSpeedDialNumberQuery ),
        CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneSendDtmfEmptyEditBoxCBA ),
        CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneDtmfSpeedDialNormalEditBoxCBA ),
        &ptr );

    CleanupStack::PopAndDestroy( emptyString );
    }

// -----------------------------------------------------------
// CPhoneStateInCall::HandleCommandL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneStateInCall::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneControl,  
        "CPhoneStateInCall::HandleCommandL()" );
    __PHONELOG1 ( EBasic, EPhoneControl,
        "CPhoneStateInCall::HandleCommandL() - aCommand  = %d ", aCommand );
    TBool commandStatus = ETrue;

    switch( aCommand )
        {
        case EPhoneInCallCmdDialer:
            if ( !IsNumberEntryUsedL() )
                {
                CreateNumberEntryL();
                }
            SetNumberEntryVisibilityL(ETrue ); 
          break;
          
        case EPhoneCmdOptions:
            OpenMenuBarL();
            break;
            
        case EPhoneDtmfDialerCancel:
            {
            CloseDTMFEditorL();
            
            // Activate DTMF list query when cancel is pressed.
            // Forwards command to the framework 
            if ( !iStateMachine->SecurityMode()->IsSecurityMode() )
                {
                HandleCommandL( EPhoneInCallCmdDtmfListQuery );
                }
            }
            break;  
			
        case EPhoneDtmfDialerExit:
            {
            CloseDTMFEditorL();   
            }
            break;
            
        case EPhoneInCallCmdEndThisActiveCall:
            CPhoneState::DisconnectCallL();
            break;
            
        case EPhoneCallComingCmdReject: // fall through
        case EPhoneInCallCmdReject:
            iStateMachine->SendPhoneEngineMessage( 
                MPEPhoneModel::EPEMessageReject );
            break;
                     
        case EPhoneInCallCmdMute: // fall through
        case EPhoneInCallCmdUnmute:
            iStateMachine->PhoneEngineInfo()->SetAudioMuteCommand( 
                ( aCommand == EPhoneInCallCmdMute ) );
            iStateMachine->SendPhoneEngineMessage( 
                MPEPhoneModel::EPEMessageSetAudioMute );
            break;
        //DTMF list query - dialog   
        case EPhoneInCallCmdDtmfListQuery:
            if ( iStateMachine->SecurityMode()->IsSecurityMode() )
                {
                HandleCommandL( EPhoneInCallCmdDtmfManualQuery );
                }
             else
                {
                LaunchDtmfListQueryL();
                }
            break;
            
        //DTMF list query - Search   
        case EPhoneInCallCmdDtmfListViewSearch:
            LaunchDtmfListViewSearchDialogL();
            break;
            
        //DTMF manual entry 
        case EPhoneInCallCmdDtmfManualQuery:
            if ( iOnScreenDialer )
                {
                ShowDtmfDialerL();
                }
            else
                {
                LaunchDtmfManualQueryL();
                }
            break;
            
        // DTMF manual entry - Search          
        case EPhoneCmdDtmfSearch:
            LaunchDtmfSearchDialogL();
            break;
            
        // DTMF entry - Ok          
        case EPhoneCmdDtmfOk:
            SendDtmfL();
            break;

        // DTMF Speed entry - Ok
        case EPhoneCmdDtmfSpeedDialOk:
            SendDtmfSpeedDialNumberL();
            break;

        // DTMF sending - Cancel
        case EPhoneInCallCmdCancelSendingDtmfString:
            // Stop the asynchronous sending operation the 
            // PhoneEngine may be performing.
            iStateMachine->SendPhoneEngineMessage( 
                MPEPhoneModel::EPEMessageStopDTMFSending );
            // Remove DTMF sending from screen
            iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );
            break;    
            
        // DTMF wait - Ok
        case EPhoneInCallCmdContinueSendingDtmfString:
            // Continue sending the DTMF string
            if( !iDtmfWaitCharTimer )
                {
                iDtmfWaitCharTimer = CPhoneDtmfWaitCharTimer::NewL( 
                    iStateMachine );                    
                }
            iDtmfWaitCharTimer->ButtonPressedL();
            break;
            
        case  EPhoneDialerCmdHelpDtmf:
            {
            if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {                
                TPtrC contextName( KDATA_DIALER_HLP_SEND_DTMF );
                iViewCommandHandle->ExecuteCommandL( 
                    EPhoneViewLaunchHelpApplication,
                    0,
                    contextName );
                }
            }
            break;

        // New Call - Search
        case EPhoneInCallCmdNewCallFind:
            LaunchNewCallSearchDialogL();
            break;
            
        // New Call - Dialog     
        case EPhoneInCallCmdNewCall:
            LaunchNewCallQueryL();
            break;
            
        case EPhoneViewYesSingleItemFetch:       
            HandleSuccessFetchedNumberL();                          
            break;
            
        case EPhoneViewNoSingleItemFetch:
            if ( !iOnScreenDialer )
                {   
                HandleFailedFetchedNumberL();
                }
            break;
            
        case EPhoneNumberAcqCmdCall:
        case EPhoneNumberAcqCmdSendCommand:
            if ( IsVideoCallActiveL() )
                {
                // Get the number entry contents
                HBufC *phoneNumber = HBufC::NewLC( KPhoneNumberEntryBufferSize );
                TPtr ptr( phoneNumber->Des() );
                TPhoneCmdParamString stringParam;
                stringParam.SetString( &ptr );
                iViewCommandHandle->ExecuteCommand(
                    EPhoneViewGetNumberFromEntry,
                    &stringParam );
                iStateMachine->PhoneEngineInfo()->SetPhoneNumber( ptr );
                                        
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
            else
                {
                // Provide number information with dial command
                CallFromNumberEntryL();
                }
            break;

        case EPhoneInCallCmdHelp:
            if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {                
                TPtrC contextName;
                if ( IsVideoCallActiveL() )
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

        case EPhoneCmdEnd:
            CloseDtmfQueryL();
            DisconnectCallL();
            break;
            
        // 'End all calls' from menu
        case EPhoneInCallCmdEndAllCalls:
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageReleaseAll );
            break;
            
        case EPhoneInCallCmdEndThisOutgoingCall:
            DisconnectOutgoingCallL();
            break;

        case EPhoneInCallCmdGoToIdle:
            {
            // Bring Idle app to foreground
            iViewCommandHandle->ExecuteCommandL( EPhoneViewBringIdleToForeground );    
            break;                   
            }
            
        case EPhoneNumberAcqCmdVideoCall:
            {// Create normal voice call, if number is emergency number.
            // Get the number entry contents
            HBufC* phoneNumber = PhoneNumberFromEntryLC();
            iStateMachine->PhoneEngineInfo()->SetPhoneNumber( *phoneNumber );
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageCheckEmergencyNumber );
            CleanupStack::PopAndDestroy( phoneNumber );}
            break;
            
        case EPhoneInCallCmdLockKeypad:
        case EPhoneInCallCmdLockScreen:
            LockKeypadL();          
            break;

       case EPhoneViewOpenCallHandling:
            if ( iOnScreenDialer && IsNumberEntryUsedL() )
                {
                // Remove number entry from screen
                iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
                HandleNumberEntryClearedL();
                }
            commandStatus = CPhoneState::HandleCommandL( aCommand );        
            break;
 
        case EPhoneInCallCmdShareLiveVideo:
            iViewCommandHandle->ExecuteCommandL( EPhoneViewLaunchMultimediaSharing );
            break;

       default:
            commandStatus = CPhoneState::HandleCommandL( aCommand );
            break;
        }

    return commandStatus;
    }
    
// -----------------------------------------------------------
// CPhoneStateInCall::CreateNumberEntryL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::CreateNumberEntryL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneStateInCall::CreateNumberEntryL( ) ");
    iViewCommandHandle->ExecuteCommandL( EPhoneViewCreateNumberEntry );
    }

// -----------------------------------------------------------
// CPhoneStateInCall::OpenMenuBarL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::OpenMenuBarL()" );
    TInt resourceId;
    
    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
        {
        resourceId = EPhoneDtmfDialerMenubar;
        }
    else if ( iOnScreenDialer && IsCustomizedDialerVisibleL() )
        {
        resourceId = CustomizedDialerMenuResourceIdL();
        }
    else if ( IsNumberEntryVisibleL() )
        {
        resourceId = EPhoneCallHandlingMenubarWithNumberEntry;
        }
    else
        {
        resourceId = EPhoneCallHandlingMenubar;
        }

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( resourceId ) );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarOpen, 
        &integerParam );
    }
    
// -----------------------------------------------------------
// CPhoneStateInCall::LaunchNewCallQueryL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::LaunchNewCallQueryL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::LaunchNewCallQueryL()" );
    
    if ( iOnScreenDialer )
        {
        //In touch, just activate dialer
        if ( IsNumberEntryUsedL() )
            {
            SetNumberEntryVisibilityL(ETrue);   
            }
        else
            {
            CreateNumberEntryL();
            SetNumberEntryVisibilityL(ETrue); 
            }
        }
    else
        {
        HBufC *text = HBufC::NewLC( KPhoneNumberEntryBufferSize );
        TPtr ptr( text->Des() );

        // Pre-populate the query with the number entry contents,
        // if it exists
        if ( IsNumberEntryUsedL() )
            {
            // get the number entry contents
            TPhoneCmdParamString stringParam;
            stringParam.SetString( &ptr );
            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewGetLocalizedNumberFromEntry,
                &stringParam );
            }

        // Show text query with send key enabled
        const TBool enableSendKey = ETrue;

        CPhoneState::ShowTextQueryL( 
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneNewCallPhoneNumberEditor ),
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneCallHandlingNewCallFindCBA ),
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneCallHandlingNewCallCallCBA ),
            &ptr,
            enableSendKey );

        CleanupStack::PopAndDestroy( text );        
        }

    }

// -----------------------------------------------------------
// CPhoneStateInCall::CallFetchedNumberL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::CallFetchedNumberL( 
    const TDesC& aFetchedNumber )
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::CallFetchedNumberL()" );
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );    
        
    // Store the phone number
    iStateMachine->PhoneEngineInfo()->SetPhoneNumber( aFetchedNumber );

    DialVoiceCallL();
    }

// -----------------------------------------------------------
// CPhoneStateInCall::SendDtmfL
// -----------------------------------------------------------
//
void CPhoneStateInCall::SendDtmfL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::SendDtmfL()" );
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );    

    // First get the DTMF sequence from dialog
    TPhoneCmdParamString dtmfSequence;
    HBufC *content = HBufC::NewLC( KPEDtmfMaxLength );
    TPtr ptr( content->Des() );
    dtmfSequence.SetString( &ptr );
    
    if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
        {
        iViewCommandHandle->ExecuteCommand(
            EPhoneViewGetNumberFromEntry,
            &dtmfSequence );  
                    
        CloseDTMFEditorL();   
        }
    else
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewGetTextQueryContent,
                                             &dtmfSequence );
        }        

    // Send the DTMF
    if ( ptr.Length() )
        {
        iStateMachine->PhoneEngineInfo()->SetDtmfStringCommand( ptr );
        iStateMachine->SendPhoneEngineMessage(
            MPEPhoneModel::EPEMessageSendDTMF );    
        }
    
    CleanupStack::PopAndDestroy( content );
    }
    
// -----------------------------------------------------------
// CPhoneStateInCall::SendDtmfSpeedDialNumberL
// -----------------------------------------------------------
//
void CPhoneStateInCall::SendDtmfSpeedDialNumberL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::SendDtmfSpeedDialNumberL()" );
    // First get the DTMF sequence from dialog
    TPhoneCmdParamString dtmfSequence;
    HBufC *content = HBufC::NewLC( KPEDtmfMaxLength );
    TPtr ptr( content->Des() );
    dtmfSequence.SetString( &ptr );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetTextQueryContent,
        &dtmfSequence );

    // Prefix the query content string with the speed dial character '+'
    _LIT( KSpeedDialPrefix, "+" );
    ptr.Insert( 0, KSpeedDialPrefix );

    // Send the DTMF
    iStateMachine->PhoneEngineInfo()->SetDtmfStringCommand( ptr );
    iStateMachine->SendPhoneEngineMessage(
        MPEPhoneModel::EPEMessageSendDTMF );    

    CleanupStack::PopAndDestroy( content );
    }

// -----------------------------------------------------------
// CPhoneStateInCall::LaunchDtmfManualQueryL
// -----------------------------------------------------------
//
void CPhoneStateInCall::LaunchDtmfManualQueryL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateInCall::LaunchDtmfManualQueryL()" );
    
    HBufC *text = HBufC::NewLC( KPhoneNumberEntryBufferSize );
    TPtr ptr( text->Des() );
    // Pre-populate the query with the number entry contents, if it exists
    if ( IsNumberEntryUsedL() )
        {
        // get the number entry contents
        TPhoneCmdParamString stringParam;
        stringParam.SetString( &ptr );
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewGetLocalizedNumberFromEntry,
            &stringParam );
        }

    ShowDtmfTextQueryL( 
        CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneDtmfNumberQuery ),
        CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneSendDtmfEmptyEditBoxCBA ),
        CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneSendDtmfNormalEditBoxCBA ),
        &ptr );
    CleanupStack::PopAndDestroy( text );
    }

// -----------------------------------------------------------
// CPhoneStateInCall::LaunchDtmfListQueryL
// -----------------------------------------------------------
//
void CPhoneStateInCall::LaunchDtmfListQueryL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::LaunchDtmfListQueryL()" );   
         
    // Fetch active call's id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnected );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
   
    if ( callStateData.CallId() == KErrNotFound )
        {
        callStateData.SetCallState( EPEStateConnecting );
        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );    
        }
        
    if ( callStateData.CallId() > KErrNotFound )
        {
        //Get Array of DTMF strings from PhoneEngine           
        const CDesCArray& dtmfArray = 
        iStateMachine->PhoneEngineInfo()->
        RemotePredefinedDtmfStrings( callStateData.CallId() );              
        
        TInt itemCount = dtmfArray.Count();  
        if ( itemCount )
            {
     
            TPhoneCmdParamString stringParam;                 
            //Go through the array and send each dtmf string
            //to view.
            for ( TInt i=0; i<itemCount; i++ )
                {
                TPtrC16 ptrC16 = dtmfArray.MdcaPoint( i );
                HBufC* string = ptrC16.AllocL();
                CleanupStack::PushL( string );
                TPtr ptr( string->Des() );
                
                stringParam.SetString( &ptr ); 
                iViewCommandHandle->ExecuteCommandL(
                    EPhoneViewSetListQueryString,
                    &stringParam );     
                CleanupStack::PopAndDestroy( string );
                string = NULL;
                }           
            }
        }
    else
        {
        __PHONELOG( EOnlyFatal, EPhoneControl, 
            "CPhoneStateInCall::LaunchDtmfListQueryL() No found valid call id" );    
        }  
    
    // if there is a connected call only then open DtmfListQuery.
    if ( IsAnyConnectedCalls() )
       {
       TPhoneCmdParamQuery queryDialogParam;
       queryDialogParam.SetQueryType( EPhoneDtmfListQueryDialog );
       queryDialogParam.SetQueryResourceId(
               CPhoneMainResourceResolver::Instance()->
               ResolveResourceID( EPhoneDtmfNumberListQuery )  );
       
       // Display dialog        
       iViewCommandHandle->ExecuteCommandL( EPhoneViewShowQuery,
           &queryDialogParam );
       }
    }

// -----------------------------------------------------------
// CPhoneStateInCall::LaunchDtmfSearchDialogL
// -----------------------------------------------------------
//
void CPhoneStateInCall::LaunchDtmfSearchDialogL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::LaunchDtmfSearchDialogL()" );
    
    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( EPhoneDtmfNumberQuery ); 
    iViewCommandHandle->HandleCommandL( EPhoneViewOpenSingleItemFetchDialog, &integerParam );
    }
    
// -----------------------------------------------------------
// CPhoneStateInCall::LaunchDtmfListViewSearchDialogL
// -----------------------------------------------------------
//
void CPhoneStateInCall::LaunchDtmfListViewSearchDialogL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::LaunchDtmfListViewSearchDialogL()" );

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( EPhoneDtmfFetchTitle ); 
    iViewCommandHandle->HandleCommandL( EPhoneViewOpenSingleItemFetchDialog, &integerParam );
    }    

// -----------------------------------------------------------
// CPhoneStateInCall::LaunchNewCallSearchDialogL
// -----------------------------------------------------------
//
void CPhoneStateInCall::LaunchNewCallSearchDialogL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::LaunchNewCallSearchDialogL()" );
    
    TPhoneCmdParamBoolean blockingDialogStatus;
            iViewCommandHandle->ExecuteCommandL( EPhoneViewGetBlockingDialogStatus, 
            &blockingDialogStatus ); 
    // Prevents unlegal use of singleitem fetch       
    if ( !blockingDialogStatus.Boolean() )     
        {             
                    
        TPhoneCmdParamInteger integerParam;
        integerParam.SetInteger( EPhoneNewCallFetchTitle ); 
        iViewCommandHandle->HandleCommandL( EPhoneViewOpenSingleItemFetchDialog, &integerParam );
        }
    }
    
// ---------------------------------------------------------
// CPhoneStateInCall::IsVideoCallActiveL
// ---------------------------------------------------------
//
EXPORT_C TBool CPhoneStateInCall::IsVideoCallActiveL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::IsVideoCallActive()" );
    TBool retVal = EFalse;
    // Fetch active call's id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnected );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
        
    if ( callStateData.CallId() > KErrNotFound )
        {
        retVal = IsVideoCall( callStateData.CallId() );
        }
        
    return retVal;
    }
    
// ---------------------------------------------------------
// CPhoneStateInCall::IsVideoCallRingingL
// ---------------------------------------------------------
//

EXPORT_C TBool CPhoneStateInCall::IsVideoCallRingingL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::IsVideoCallRingingL()" );
    TBool retVal = EFalse;
    // Fetch ringing call id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
        
    if ( callStateData.CallId() > KErrNotFound )
        {
        retVal = IsVideoCall( callStateData.CallId() );
        }
        
    return retVal;
    }

// ---------------------------------------------------------
// CPhoneStateInCall::GetRingingCallL
// ---------------------------------------------------------
//

EXPORT_C TInt CPhoneStateInCall::GetRingingCallL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,  
        "CPhoneStateInCall::GetRingingCallL()" );
    
    // Fetch ringing call id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
         
    return callStateData.CallId();
    }

// -----------------------------------------------------------
// CPhoneStateInCall::DisconnectOutgoingCallL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::DisconnectOutgoingCallL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneStateInCall::DisconnectOutgoingCallL()");
    // Fetch alerting call's id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnecting );
    iViewCommandHandle->HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
        
    if( callStateData.CallId() == KErrNotFound )
        {
        // No connecting call, find the dialing call
        callStateData.SetCallState( EPEStateDialing );
        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );
        
        if( callStateData.CallId() == KErrNotFound )
            {
            // No dialing call, find the disconnectinging call
            callStateData.SetCallState( EPEStateDisconnecting );
            iViewCommandHandle->HandleCommandL(
                EPhoneViewGetCallIdByState, &callStateData );
            }
        else
            {
            if ( IsNumberEntryUsedL() )
                {
                SetNumberEntryVisibilityL(ETrue);   
                }
            }
        }

    if( callStateData.CallId() > KErrNotFound )
        {
        // Release the call
        iStateMachine->SetCallId( callStateData.CallId() );
        iStateMachine->SendPhoneEngineMessage( 
            MPEPhoneModel::EPEMessageRelease );
        }
     else
        {
        __PHONELOG( EOnlyFatal, EPhoneControl, 
            "CPhoneStateInCall::DisconnectOutgoingCallL() has negative call id!" );   
        __ASSERT_DEBUG( EFalse, Panic( EPhoneCtrlIndexOutOfBounds ) );
        }       
    }

// -----------------------------------------------------------------------------
// CPhoneStateInCall::LockKeypadL
// -----------------------------------------------------------------------------
//
void CPhoneStateInCall::LockKeypadL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneStateInCall::LockKeypadL()");

    if ( !FeatureManager::FeatureSupported( KFeatureIdKeypadNoSlider ) )
        {
        TInt state = KErrNone; 
        TInt err = RProperty::Get(
            KPSUidHWRM,
            KHWRMGripStatus,
            state );
        if ( state == EPSHWRMGripOpen )
            {
            SendGlobalErrorNoteL( EPhoneNoteTextNotAllowed );
            }
        else
            {
            iViewCommandHandle->ExecuteCommandL(
                EPhoneViewEnableKeyLock );    
            }
        }
    else
        {
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewEnableKeyLock ); 
        }
    }    
     
// -----------------------------------------------------------------------------
// CPhoneStateInCall::HandleSuccessFetchedNumberL
// -----------------------------------------------------------------------------
//    
void CPhoneStateInCall::HandleSuccessFetchedNumberL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneStateInCall::HandleSuccessFetchedNumberL()");
    TBuf<KPhoneNumberEntryBufferSize> fetchContent;    
    fetchContent = iViewCommandHandle->FetchContent();

    TPhoneCmdParamInteger fetchTypeParam;
    iViewCommandHandle->ExecuteCommand( EPhoneViewGetSingleItemFetchType,
        &fetchTypeParam );
    TInt fetchType( fetchTypeParam.Integer() );
    
    if ( fetchType == EPhoneNewCallFetchTitle )
        {
        if ( iOnScreenDialer )
            {
            iViewCommandHandle->ExecuteCommandL(
                            EPhoneViewSetNumberEntryContent,
                            0,
                            fetchContent );              
            }
        // Call the fetched number
        CallFetchedNumberL( fetchContent );
        }
    else if ( fetchType == EPhoneDtmfFetchTitle )
        {
        // Send the DTMF
        iStateMachine->PhoneEngineInfo()->SetDtmfStringCommand( fetchContent );
        iStateMachine->SendPhoneEngineMessage(
            MPEPhoneModel::EPEMessageSendDTMF ); 
        }
    else if( iCustomization && fetchType > EPhoneRssBaseLast )
        {
        // Handle unattended transfer contact fetch
        iCustomization->HandleCommandL( fetchType );
        }
    else // EPhoneCmdDtmfSearch
        {
        // Send the DTMF
        iStateMachine->PhoneEngineInfo()->SetDtmfStringCommand( fetchContent );
        iStateMachine->SendPhoneEngineMessage(
            MPEPhoneModel::EPEMessageSendDTMF ); 
        }
    }
    
// -----------------------------------------------------------------------------
// CPhoneStateInCall::HandleFailedFetchedNumberL
// -----------------------------------------------------------------------------
//    
void CPhoneStateInCall::HandleFailedFetchedNumberL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneStateInCall::HandleFailedFetchedNumberL()");
    
    TPhoneCmdParamInteger fetchTypeParam;
    iViewCommandHandle->ExecuteCommand( EPhoneViewGetSingleItemFetchType,
        &fetchTypeParam );
    TInt fetchType( fetchTypeParam.Integer() );
    
    if ( fetchType == EPhoneNewCallFetchTitle )
        {
        HandleCommandL( EPhoneInCallCmdNewCall );
        }
    else if ( fetchType ==  EPhoneDtmfFetchTitle )
        {
        HandleCommandL( EPhoneInCallCmdDtmfListQuery );
        }
    else // EPhoneCmdDtmfSearch
        {
        HandleCommandL( EPhoneDtmfFetchTitle ); 
        }
    }  


// -----------------------------------------------------------------------------
// CPhoneStateInCall::CloseDtmfQueryL
// -----------------------------------------------------------------------------
//    
EXPORT_C void CPhoneStateInCall::CloseDtmfQueryL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneStateInCall::CloseDtmfQueryL()");
    TPhoneCmdParamInteger fetchTypeParam;
        iViewCommandHandle->ExecuteCommand( EPhoneViewGetSingleItemFetchType,
            &fetchTypeParam );
        TInt fetchType( fetchTypeParam.Integer() );
     
    // If DTMF fetch is active then remove numberentry because it is considered
    // to be a part of DTMF fetch operation.
    if ( fetchType ==  EPhoneDtmfFetchTitle )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        iViewCommandHandle->ExecuteCommand( EPhoneViewCloseSingleItemFetchDialog ); 
        }
    
    // Close dtmf dialer or dtmf query if exist.
    if ( IsDTMFEditorVisibleL() )
        {
        CloseDTMFEditorL();
        }
    }  

// -----------------------------------------------------------
// CPhoneStateInCall::PhoneNumberAvailableInPhoneEngineL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateInCall::PhoneNumberAvailableInPhoneEngineL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneStateInCall::PhoneNumberAvailableInPhoneEngineL() ");
    TPhoneCmdParamBoolean phoneNumberAvailable;
    
    const TBool contactInfoAvailable = 
        iStateMachine->PhoneEngineInfo()->RemoteName( aCallId ).Length() ||
        iStateMachine->PhoneEngineInfo()->RemoteCompanyName( aCallId ).Length();
    
    if( iStateMachine->PhoneEngineInfo()->RemotePhoneNumber( aCallId ).Length() ||
        contactInfoAvailable )
        {
        // Phone number is available straight or via contact info
        // so switch to video/voice call is possible
        __PHONELOG( EBasic, EPhoneControl, "CPhoneStateInCall::PhoneNumberAvailableInPhoneEngineL - Phonenumber is available" );
        phoneNumberAvailable.SetBoolean( ETrue );
        }
    else
        {
        __PHONELOG( EBasic, EPhoneControl, "CPhoneStateInCall::PhoneNumberAvailableInPhoneEngineL - Phonenumber is not available" );
        phoneNumberAvailable.SetBoolean( EFalse );
        }

    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetPhoneNumberAvailableInPhoneEngine,
        &phoneNumberAvailable );
    }


// -----------------------------------------------------------------------------
// CPhoneStateInCall::HandleEndKeyPressL
// -----------------------------------------------------------------------------
//    
void CPhoneStateInCall::HandleEndKeyPressL( TPhoneKeyEventMessages aMessage )
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneStateInCall::HandleEndKeyPressL()");        
    // handle long press of end key
    if ( aMessage == EPhoneKeyLongPress )
        {
        // Fetch active call's id from view
        TPhoneCmdParamCallStateData callStateData;
        callStateData.SetCallState( EPEStateConnected );
        iViewCommandHandle->HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );

        if( IsVideoCall( callStateData.CallId() ) )
            {
            // Video call can be released only after we get response to VT Shutdown Command
            CPhoneMediatorFactory::Instance()->Sender()->IssueCommand( 
                KMediatorVideoTelephonyDomain,
                KCatPhoneToVideotelCommands, 
                EVtCmdReleaseDataport,
                TVersion( KPhoneToVideotelCmdVersionMajor,
                    KPhoneToVideotelCmdVersionMinor, 
                    KPhoneToVideotelCmdVersionBuild ),
                KNullDesC8,
                CPhoneTerminateAllConnectionsCommand::NewL( *iStateMachine ) );
            }
        else
            {
            // Close all connections
            iStateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageTerminateAllConnections );
            
            if ( IsNumberEntryContentStored() )
                {
                ClearNumberEntryContentCache();
                }
            }

        if ( IsNumberEntryUsedL() )
            {
            if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
                {
                CloseDTMFEditorL();
                }
            else
                {
                // Remove number entry from screen
                iViewCommandHandle->ExecuteCommandL( 
                    EPhoneViewRemoveNumberEntry );
                // Do state-specific operation when number entry is cleared
                HandleNumberEntryClearedL();         
                }
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
        if ( IsNumberEntryUsedL() )
            {
            if ( iOnScreenDialer && IsDTMFEditorVisibleL() )
                {
                CloseDTMFEditorL();
                }
            }
      
        // handle short end key
        CPhoneState::DisconnectCallL();    
        }
    }

// -----------------------------------------------------------------------------
// CPhoneStateInCall::HandleVoiceKeyPressL
// -----------------------------------------------------------------------------
//    
void CPhoneStateInCall::HandleVoiceKeyPressL( TPhoneKeyEventMessages aMessage )
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneStateInCall::HandleVoiceKeyPressL()");
    if ( aMessage == EPhoneKeyShortPress )
        {
        // Toggle the handsfree mode
        const TBool handsfreeMode = 
            iStateMachine->PhoneEngineInfo()->AudioOutput() ==
                EPELoudspeaker;
        SetHandsfreeModeL( !handsfreeMode );
        }
    else // aMessage == EPhoneKeyLongPress
        {
        // Display call in progress information note
        SendGlobalInfoNoteL( EPhoneCallInProgress );
        }        
    }

// -----------------------------------------------------------
// CPhoneStateInCall::ShowDtmfTextQueryL
// -----------------------------------------------------------
//
void CPhoneStateInCall::ShowDtmfTextQueryL(
    TInt aDialogResourceId,
    TInt aDefaultCbaResourceId,
    TInt aContentCbaResourceId,
    TDes* aDataText,
    TBool aSendKeyEnabled )
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneStateInCall::ShowDtmfTextQueryL()");

    __ASSERT_DEBUG( aDialogResourceId &&
        aDefaultCbaResourceId &&
        aContentCbaResourceId &&
        aDataText, 
        Panic( EPhoneCtrlParameterNotInitialized ) );
    TPhoneCmdParamQuery queryDialogParam;
    queryDialogParam.SetQueryType( EPhoneDtmfTextQuery );
    queryDialogParam.SetQueryResourceId( aDialogResourceId );
    queryDialogParam.SetDefaultCba( aDefaultCbaResourceId );
    queryDialogParam.SetContentCba( aContentCbaResourceId );
    queryDialogParam.SetDataText( aDataText );
    queryDialogParam.SetSendKeyEnabled( aSendKeyEnabled );
    
    // Display dialog        
    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowQuery,
        &queryDialogParam );    
    }
  
// -----------------------------------------------------------------------------
// CPhoneStateInCall::HandlePropertyChangedL
// from CPhoneState
// -----------------------------------------------------------------------------
//  
EXPORT_C void CPhoneStateInCall::HandlePropertyChangedL(
        const TUid& aCategory, const TUint aKey, const TInt aValue)
    {
    if ( aCategory == KPSUidCoreApplicationUIs &&
         aKey == KCoreAppUIsVideoSharingIndicator )
        {
        if ( aValue == ECoreAppUIsVideoSharingIndicatorOn )
            {
            SetTouchPaneButtons( EPhoneIncallVideoShareButtons );
            }
        else
            {
            SetTouchPaneButtons( EPhoneIncallButtons );
            }
        }
    else
        {
        CPhoneState::HandlePropertyChangedL( aCategory, aKey, aValue );
        }
    }

// End of File
