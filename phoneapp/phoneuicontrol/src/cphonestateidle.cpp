/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneStateIdle class.
*
*/


// INCLUDES
#include <eikenv.h>
#include <e32property.h>
#include <connect/sbdefs.h>
#include <w32std.h>
#include <StringLoader.h>
#include <aknnotedialog.h> // For CAknNoteDialog::EErrorTone
#include <apgcli.h>
#include <telephonyvariant.hrh>
#include <featmgr.h>
#include <settingsinternalcrkeys.h>
#include <mpeengineinfo.h>
#include <mpeclientinformation.h>
#include <bldvariant.hrh>

#include "cphonepubsubproxy.h"
#include "cphonecenrepproxy.h"
#include "cphonestateidle.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamkeycapture.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamboolean.h"
#include "mphonestatemachine.h"
#include "phonestatedefinitions.h"
#include "tphonecmdparamappinfo.h"
#include "cphonekeys.h"

#include "tphonecmdparamcallstatedata.h"
#include "phoneviewcommanddefinitions.h"
#include "phoneui.hrh"
#include "phonerssbase.h"
#include "cphonemainresourceresolver.h"
#include "phonelogger.h"
#include "phoneui.pan"
#include "mphonecustomization.h"


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C CPhoneStateIdle::CPhoneStateIdle( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aCustomization ) : 
    CPhoneState( aStateMachine, aViewCommandHandle, aCustomization )
    {
    }

// Destructor
EXPORT_C CPhoneStateIdle::~CPhoneStateIdle()
    {
    }

// -----------------------------------------------------------
// CPhoneStateIdle::ConstructL()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::ConstructL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::ConstructL() ");
    CPhoneState::BaseConstructL();
    }

// -----------------------------------------------------------
// CPhoneStateIdle::NewL()
// -----------------------------------------------------------
//
CPhoneStateIdle* CPhoneStateIdle::NewL( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aPhoneCustomization )
    {
    CPhoneStateIdle* self = new (ELeave) CPhoneStateIdle( 
        aStateMachine, aViewCommandHandle, aPhoneCustomization );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleKeyEventL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::HandleKeyEventL( 
    const TKeyEvent& aKeyEvent, 
    TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandleKeyEventL( ) ");
    TPhoneCmdParamBoolean isSecurityMode;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetSecurityModeStatus, &isSecurityMode );
    if ( isSecurityMode.Boolean() && CPhoneKeys::IsNumericKey( 
        aKeyEvent, aEventCode ) )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );
        }
    else if ( aEventCode == EEventKeyUp )
        {
        // Provide up event to engine. 
        // Otherwise DTMF tone keeps playing.    
           iStateMachine->SendPhoneEngineMessage( 
                MPEPhoneModel::EPEMessageEndDTMF );
         __PHONELOG( EBasic, EPhoneControl,
                "PhoneUIControl: CPhoneStateIdle::HandleKeyEventL" );
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleNumberEntryClearedL()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::HandleNumberEntryClearedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandleNumberEntryClearedL( ) ");
    if ( IsSimOk() && !IsSimStateNotPresentWithSecurityModeEnabled() )
        {
        //  if still idle ( f.e not incoming call arrived when rfs )
        if ( iStateMachine->State() == this )
            {
            DisplayIdleScreenL();
            }
        }
   else
        {
        StartShowSecurityNoteL(); 
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleKeyMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::HandleKeyMessageL( 
    TPhoneKeyEventMessages aMessage,
    TKeyCode aCode )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandleKeyMessageL( ) ");
    switch ( aCode )
        {
        case EKeyEnd:
            // This key even occur only in idle state, behaviour for this should
            // be same as long end-key press. (Bug in window server)
            HandleEndKeyPressL( EPhoneKeyLongPress );
            break;
            
        // end-key
        case EKeyNo:
            HandleEndKeyPressL( aMessage );
            break;
        // send-key
        case EKeyYes:
            if ( IsNumberEntryUsedL() )
                {
                TPhoneCmdParamBoolean isSecurityMode;
                iViewCommandHandle->ExecuteCommandL( EPhoneViewGetSecurityModeStatus, &isSecurityMode );
                
                TPhoneCmdParamInteger numberEntryCountParam;
                iViewCommandHandle->ExecuteCommandL( EPhoneViewGetNumberEntryCount,
                &numberEntryCountParam );
                
                TInt neLength( numberEntryCountParam.Integer() );
                if ( IsAnyQueryActiveL() )
                    {
                    return;
                    }
                else if ( neLength == 0 && !isSecurityMode.Boolean())
                    {
                    // start logs
                    iViewCommandHandle->HandleCommandL( 
                    EPhoneDialerCmdLog );
                    return;
                    }
                HandleSendCommandL();
                }
            // If dialer is not open but phone is in foreground and phone receives
            // send-key event we have to check if security mode is true and if it
            // is then open emergency dialer.
            else if ( !IsNumberEntryUsedL() )
                {
                TPhoneCmdParamBoolean isSecurityMode;
                iViewCommandHandle->ExecuteCommandL( EPhoneViewGetSecurityModeStatus, &isSecurityMode );
                if ( isSecurityMode.Boolean())
                    {
                    // launch dialer.
                    HandleCommandL(EPhoneNumberAcqSecurityDialer);
                    }
                }
            break;
            
        case EKeyApplication0:
            {
            // If dialer is not open but phone is in foreground and phone receives
            // applicaion-key event we have to open emergency dialer. No need own
            // securitymode check because Applicationkey only captured in securitymode.
            if ( !IsNumberEntryUsedL() )
                {
                HandleCommandL(EPhoneNumberAcqSecurityDialer);
                }
            }
            break;
#ifdef RD_INTELLIGENT_TEXT_INPUT
        case EKeyEnter:
            if ( IsNumberEntryVisibleL() )
                {
                HandleCommandL( EPhoneCmdOptions );
                }
            break;
#endif
        default:
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandlePhoneEngineMessageL( ) ");
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageIncoming:
            HandleIncomingL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageDialing:
            HandleDialingL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageConnected:
            HandleConnectedL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageIssuedUSSDRequest:
            // Note that after the sending of SS 
            // strings the view stays in Dialer.
            if ( IsNumberEntryUsedL() )
                {
                // Show the number entry if it exists
                SetNumberEntryVisibilityL(ETrue);    
                }    
            else if ( NeedToSendToBackgroundL() )
                {
                // Continue displaying current app but set up the 
                // idle screen in the background
                SetupIdleScreenInBackgroundL();
                }
            else
                {
                 // Display idle screen
                DisplayIdleScreenL();
                }
            break;
            
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageInValidEmergencyNumber:
            SendGlobalWarningNoteL( EPhoneEmergencyCallsOnly, ETrue );
            break;
            
        case MEngineMonitor::EPEMessageValidEmergencyNumber:
            DialVoiceCallL();
            break; 
                        
        default:
            CPhoneState::HandlePhoneEngineMessageL( aMessage, aCallId );
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleIncomingL( TInt aCallId )
// Default handling for incoming message
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneStateIdle::HandleIncomingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandleIncomingL( ) ");
    HandleAudioAvailableOutputChangedL();
    SetRingingTonePlaybackL( aCallId );
    BeginUiUpdateLC();
    
    if ( IsNumberEntryUsedL() )
        {
        SetNumberEntryVisibilityL(EFalse);
        }
     
    DisplayIncomingCallL( aCallId );
    SetTouchPaneButtons( EPhoneIncomingCallButtons );
    CheckDisableHWKeysAndCallUIL();
    SetBackButtonActive(EFalse);
    EndUiUpdate();
    
    iCbaManager->UpdateIncomingCbaL( aCallId );
    iViewCommandHandle->HandleCommandL( EPhoneViewShowToolbar );
    iStateMachine->ChangeState( EPhoneStateIncoming );
    SetDivertIndication( EFalse );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleDiallingL
// Default handling for dialling message
// (other items were commented in a header).
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::HandleDialingL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandleDialingL( ) ");
    BeginUiUpdateLC();
    DisplayCallSetupL( aCallId );
    SetTouchPaneButtons( EPhoneCallSetupButtons );
    EndUiUpdate();
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
    iCbaManager->UpdateCbaL( EPhoneCallHandlingCallSetupCBA );
    iStateMachine->ChangeState( EPhoneStateCallSetup );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleConnectedL
// Message Handling function for message EPEMessageConnected
// This could happen during transaction from Emergency mode
// to Normal mode where a network status change is received
// from PE after PE initiates an MO call. Since PE messages
// are asynchronous, this sequence can occur in any order.
// (other items were commented in a header).
// -----------------------------------------------------------
//
void CPhoneStateIdle::HandleConnectedL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandleConnectedL( ) ");
    // Keep Phone app in the foreground if it is external
    if ( !TopAppIsDisplayedL() )
        {
        TPhoneCmdParamInteger uidParam;
        uidParam.SetInteger( KUidPhoneApplication.iUid );
        iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground, 
            &uidParam );
        }
    BeginUiUpdateLC();
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
    DisplayCallSetupL( aCallId );
    UpdateSingleActiveCallL( aCallId );
    SetTouchPaneButtons( EPhoneIncallButtons );
    EndUiUpdate();
    
    iCbaManager->UpdateCbaL( EPhoneCallHandlingInCallCBA );
    iStateMachine->ChangeState( EPhoneStateSingle );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleCommandL
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneStateIdle::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandleCommandL( ) ");
    TBool commandStatus = ETrue;
    switch( aCommand )
        {
        case EPhoneCmdOptions:
        case EPhoneCmdBack:
            break;
        
        case EPhoneNumberAcqCmdCall:
            HandleVoiceCallCommandL();
            break;
            
        case EPhoneNumberAcqCmdSendCommand:
            HandleSendCommandL();
            break;
            
        case EPhoneNumberAcqCmdVideoCall:
            DialVideoCallL();
            break;
            
        default:
            commandStatus = CPhoneState::HandleCommandL( aCommand );
            break;
        }
    return commandStatus;
    }

// -----------------------------------------------------------------------------
// CPhoneStateIdle::ProcessCommandL
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneStateIdle::ProcessCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::ProcessCommandL( ) ");
    TBool commandStatus = EFalse;
    switch( aCommand )
       {
       case EAknSoftkeyContextOptions:
           break;
           
       default:
           commandStatus = CPhoneState::ProcessCommandL( aCommand );
           break;
       }
    return commandStatus;
    }

// -----------------------------------------------------------------------------
// CPhoneStateIdle::HandleRemConCommandL
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneStateIdle::HandleRemConCommandL(
    TRemConCoreApiOperationId /*aOperationId */, 
    TRemConCoreApiButtonAction /*aButtonAct*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------
// CPhoneStateIdle::DialL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::DialL( 
        const TDesC& aNumber, 
        TPhoneNumberType aNumberType,
        TDialInitiationMethod /*aDialMethod*/ )
    {
    iStateMachine->PhoneEngineInfo()->SetPhoneNumber( aNumber );
    if ( aNumberType == EPhoneNumberTypeVideo &&
        FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
        {
        DialMultimediaCallL();
        }
    else if ( EPhoneNumberTypeVoip == aNumberType &&
              iCustomization )
        {
        iCustomization->HandleDialL( aNumber );
        }
    else
        {
        DialVoiceCallL();
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::DialVideoCallL
// -----------------------------------------------------------
//
void CPhoneStateIdle::DialVideoCallL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::DialVideoCallL( ) ");
    if ( IsNumberEntryUsedL() )
        {
        // get the number entry contents
        HBufC* phoneNumber = PhoneNumberFromEntryLC();
#ifdef _DEBUG
        if ( (*phoneNumber).Compare( KExit ) == 0 )
            {
            CleanupStack::PopAndDestroy( phoneNumber );
            SendExitCommandL();
            }
        else
#endif // _DEBUG
            {
            // call the number
            iStateMachine->PhoneEngineInfo()->SetPhoneNumber( *phoneNumber );
            DialMultimediaCallL();
            CleanupStack::PopAndDestroy( phoneNumber );
            }
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::DisplayIncomingCallL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::DisplayIncomingCallL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::DisplayIncomingCallL( ) ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo() && aCallId >= 0,
        Panic( EPhoneCtrlInvariant ) );
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
    TPhoneCmdParamKeyCapture captureParam;
    captureParam.SetKeyCode( EKeyNo );
    iViewCommandHandle->ExecuteCommand( EPhoneViewStartCapturingKey, &captureParam );

    // Indicate that the Phone needs to be sent to the background if
    // an application other than the top application is in the foreground
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( !TopAppIsDisplayedL() );
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewSetNeedToSendToBackgroundStatus,
        &booleanParam );

    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
        &uidParam );
        
    // Disable global notes when there is an incoming call
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetEikonNotifiersDisabled,
        &globalNotifierParam );
    DisplayHeaderForCallComingInL( aCallId, EFalse ); //not waiting
    }

// -----------------------------------------------------------
// CPhoneStateIdle::DisplayCallSetupL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::DisplayCallSetupL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::DisplayCallSetupL( ) ");
    __ASSERT_DEBUG( aCallId > KErrNotFound, Panic( EPhoneCtrlParameterNotInitialized ) );

    TPhoneCmdParamKeyCapture captureParam;
    captureParam.SetKeyCode( EKeyNo );
    iViewCommandHandle->ExecuteCommand( EPhoneViewStartCapturingKey, &captureParam );

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
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
        &uidParam ); 
    
    DisplayHeaderForOutgoingCallL( aCallId );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleIdleForegroundEventL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::HandleIdleForegroundEventL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandleIdleForegroundEventL( ) ");
    if ( IsNumberEntryUsedL() )
        {
        if ( IsNumberEntryVisibleL() )
            {
            iCbaManager->SetCbaL( EPhoneNumberAcqCBA );
            }
        }
    else if ( !IsAnyQueryActiveL() )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewBringIdleToForeground );
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandlePhoneForegroundEventL
// -----------------------------------------------------------

EXPORT_C void CPhoneStateIdle::HandlePhoneForegroundEventL()
    {
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandlePhoneFocusLostEventL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::HandlePhoneFocusLostEventL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandlePhoneFocusLostEventL( ) ");
    if ( !IsNumberEntryUsedL() )
         {
         // If dialer is open add icon to FSW list.
         iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateFSW );
         }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleIdleL
// -----------------------------------------------------------
//
void CPhoneStateIdle::HandleIdleL( TInt /*aCallId*/ )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandleIdleL()" );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveAllCallHeaders );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleSendCommandL()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::HandleSendCommandL()
    {
    __LOGMETHODSTARTEND(
        EPhoneControl, "CPhoneStateIdle::HandleSendCommandL()" );
    HandleVoiceCallCommandL();
    }

// -----------------------------------------------------------
// CPhoneStateIdle:SendExitCommandL
// -----------------------------------------------------------
//
void CPhoneStateIdle::SendExitCommandL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::SendExitCommand( ) ");
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );

    // Simulate exit command
    CEikonEnv& eikEnv = *CEikonEnv::Static();
    CCoeEnv& coeEnv = eikEnv;
    RWsSession& wsSession=coeEnv.WsSession();
    TKeyEvent keyEvent;
    keyEvent.iCode = EEikCmdExit;
    keyEvent.iScanCode = EEikCmdExit;
    keyEvent.iModifiers = 0;
    keyEvent.iRepeats = 0;
    wsSession.SimulateKeyEvent( keyEvent );
    wsSession.Flush();  

    iViewCommandHandle->ExecuteCommandL( EPhoneAppShutDown );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleEndKeyPressL
// -----------------------------------------------------------
//
void CPhoneStateIdle::HandleEndKeyPressL( TPhoneKeyEventMessages aMessage )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneStateIdle::HandleEndKeyPressL( ) ");
    // End key not handled also in phone startup when pressed during country/time/date query
    RWsSession sess = CCoeEnv::Static()->WsSession();
    TApaTaskList appList( sess );
    TApaTask startup = appList.FindApp( KUidStartupApplication );
    if ( !IsAutoLockOn() && !IsKeyLockOn() && !startup.Exists() )
        {
        //Cancels e.g. USB notifier and Audio notifier.
        iViewCommandHandle->ExecuteCommandL( EPhoneViewCancelAllNotications );
        // If KFeatureIdFfSimlessOfflineSupport is undef and security note is shown then do not
        // remove security note from screen.
        if ( !( IsSimStateNotPresentWithSecurityModeEnabled() && 
                !FeatureManager::FeatureSupported( KFeatureIdFfSimlessOfflineSupport ) ) )
            {
            iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
            }
        
        if ( IsSimOk() )
            {
            if ( aMessage == EPhoneKeyLongPress )
                {
                iStateMachine->SendPhoneEngineMessage(
                        MPEPhoneModel::EPEMessageTerminateAllConnections );
                if ( IsNumberEntryUsedL() )
                    {
                    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
                    // Do state-specific operation when number entry is cleared
                    HandleNumberEntryClearedL();
                    // If long end key event occures then all calls are terminated and
                    // dialer is closed, therefore tel.icon must be removed from FSW.
                    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateFSW );
                    }
                if ( !TopAppIsDisplayedL() )
                    {
                    DisplayIdleScreenL();
                    }
                }
            else if ( IsNumberEntryUsedL() && TopAppIsDisplayedL() )
                {
                iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
                // Do state-specific operation when number entry is cleared
                HandleNumberEntryClearedL();
                }
            else if ( !TopAppIsDisplayedL() )
                {
                // Phone might not be the topmost app since it has
                // some dialog/query open therefore we need to check this
                // and remove dialog/phone.
                if ( IsAnyQueryActiveL()  )
                    {
                    if ( !IsSimStateNotPresentWithSecurityModeEnabled() )
                        {
                        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
                        }
                    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
                    }
                iViewCommandHandle->ExecuteCommandL( EPhoneViewBringIdleToForeground );
                }
            }
        else if ( IsNumberEntryUsedL() )
            {
            iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
            StartShowSecurityNoteL();
            }
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::DialMultimediaCallL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::DialMultimediaCallL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneStateIdle::DialMultimediaCall() ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(), Panic( EPhoneCtrlInvariant ) );
    if ( RestoreOngoing() )
        {
        CPhoneState::SendGlobalInfoNoteL( EPhoneInformationVideoCallNotAllowedDuringRestoreNote );
        }
    else
        {
        CPhoneState::DialMultimediaCallL();
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::DialVoiceCallL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::DialVoiceCallL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::DialVoiceCallL() ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(), Panic( EPhoneCtrlInvariant ) );
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
        &globalNotifierParam );
    CPhoneState::DialVoiceCallL();
    }

// -----------------------------------------------------------------------------
// CPhoneStateIdle::RestoreOngoing
// -----------------------------------------------------------------------------
//
TBool CPhoneStateIdle::RestoreOngoing()
    {
    TInt restoreValue;
    RProperty::Get( KUidSystemCategory, 
                    conn::KUidBackupRestoreKey,
                    restoreValue );
    return ( restoreValue & ( conn::EBURRestoreFull | conn::EBURRestorePartial ));
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleVoiceCallCommandL()
// -----------------------------------------------------------
//
void CPhoneStateIdle::HandleVoiceCallCommandL()
    {
    __LOGMETHODSTARTEND(
        EPhoneControl, "CPhoneStateIdle::HandleVoiceCallCommandL()" );
    if ( IsNumberEntryUsedL() )
        {
        HBufC* phoneNumber = PhoneNumberFromEntryLC();
        if ( !IsSimOk() )
            {
            iStateMachine->PhoneEngineInfo()->SetPhoneNumber( *phoneNumber ) ;
            iStateMachine->SendPhoneEngineMessage( 
            MPEPhoneModel::EPEMessageCheckEmergencyNumber );
            CleanupStack::PopAndDestroy( phoneNumber );
            return;
            }
            
#ifdef _DEBUG
        if ( (*phoneNumber).Compare( KExit ) == 0 )
            {
            CleanupStack::PopAndDestroy( phoneNumber );
            SendExitCommandL();
            }
        else
#endif // _DEBUG
            {
            iStateMachine->PhoneEngineInfo()->SetPhoneNumber( *phoneNumber );
            DialVoiceCallL();
            CleanupStack::PopAndDestroy( phoneNumber );
            }
        }    
    }

// End of File
