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
#include <ScreensaverInternalPSKeys.h>
#include <mpeengineinfo.h>
#include <activeidle2domainpskeys.h>
#include <mpeclientinformation.h>
#include <bldvariant.hrh>
#include <MProEngEngine.h>
#include <ProEngFactory.h>

#include "cphonepubsubproxy.h"
#include "cphonecenrepproxy.h"
#include "cphonestateidle.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamspeeddial.h"
#include "tphonecmdparamboolean.h"
#include "mphonestatemachine.h"
#include "phonestatedefinitions.h"
#include "tphonecmdparamappinfo.h"
#include "cphonekeys.h"

#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamcallheaderdata.h"
#include "phoneviewcommanddefinitions.h"
#include "phoneui.hrh"
#include "phonerssbase.h"
#include "cphonemainresourceresolver.h"
#include "cphonecenrepproxy.h"
#include "cphonepubsubproxy.h"
#include "phonelogger.h"
#include "phoneui.pan"
#include "phoneconstants.h"
#include "mphonecustomization.h"
#include <easydialingcommands.hrh>
#include "mphonesecuritymodeobserver.h"

//CONSTANTS
const TInt  KMaxParamLength = 1024;

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C CPhoneStateIdle::CPhoneStateIdle( 
    MPhoneStateMachine* aStateMachine, 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneCustomization* aCustomization ) : 
    CPhoneState( aStateMachine, aViewCommandHandle, aCustomization ),
    iBubbleInitialized( EFalse )
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
    
    // Set send key activation on.
    TPhoneCmdParamBoolean boolean;
    boolean.SetBoolean( ETrue );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetSendKeyDialerActivationFlag, 
                &boolean );
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewCreatePhoneBookServices ); 
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
    
    // Number entry is blocked, if active Query
    if ( !IsAnyQueryActiveL() )
        {
        // Handle numeric keys when key events are received in idle state
        HandleNumericKeyEventL( aKeyEvent, aEventCode );  
        }
    else if ( iStateMachine->SecurityMode()->IsSecurityMode()
        && CPhoneKeys::IsNumericKey( aKeyEvent, aEventCode ) )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNote );
        // Handle numeric keys when key events are received in single state
        HandleNumericKeyEventL( aKeyEvent, aEventCode );  
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

    if( aMessage == EPhoneKeyLongPress && CheckAppLaunchingL( aCode ) )
        {
        return;
        }

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
            
            if ( iOnScreenDialer )
                {
                if ( IsNumberEntryUsedL() )
                    {
                   
                    TPhoneCmdParamInteger numberEntryCountParam;
                    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetNumberEntryCount,
                    &numberEntryCountParam );
                    TInt neLength( numberEntryCountParam.Integer() );
                    
                    // Query on top of dialer
                    if ( IsAnyQueryActiveL() )
                        {
                        return;
                        }
                    else if ( IsMenuBarVisibleL() )
                        {
                        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
                        return;
                        }
                    
                    else if ( neLength == 0 && !iStateMachine->SecurityMode()->IsSecurityMode() )
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
                   if ( iStateMachine->SecurityMode()->IsSecurityMode() )
                        {
                        iViewCommandHandle->HandleCommandL( EPhoneViewOpenNumberEntry );
                        }
                    }           
                }
            else // iOnScreenDialer false, non-touch.
                {
                HandleSendCommandL();
                }
            break;
            
        case EKeyApplication0:
            {
            // If dialer is not open but phone is in foreground and phone receives
            // applicaion-key event we have to open emergency dialer. 
            // Securitymode check because Applicationkey only open dialer in securitymode.
            if ( iStateMachine->SecurityMode()->IsSecurityMode() && !IsNumberEntryUsedL() )
                {
                iViewCommandHandle->HandleCommandL( EPhoneViewOpenNumberEntry );
                }           
            }
            break;
            
        // "O" key
        case KPhoneDtmf0Character:
            if ( aMessage == EPhoneKeyLongPress && IsSimOk() )
                {
                LaunchApplicationL();
                }
            break;
            
        // number key
        case KPhoneDtmf1Character:
        case KPhoneDtmf2Character:
        case KPhoneDtmf3Character:
        case KPhoneDtmf4Character:
        case KPhoneDtmf5Character:
        case KPhoneDtmf6Character:
        case KPhoneDtmf7Character:
        case KPhoneDtmf8Character:
        case KPhoneDtmf9Character:
            if ( aMessage == EPhoneKeyLongPress && IsSimOk() )
                {
                HandleNumberLongKeyPressL();
                }
            break;

#ifdef RD_INTELLIGENT_TEXT_INPUT   
        case EKeyEnter:
            if ( IsNumberEntryVisibleL() )
                {
                if ( IsDialingExtensionInFocusL() )
                    {
                    HandleCommandL( EEasyDialingEnterKeyAction );
                    }
                else
                    {
                    TPhoneCmdParamInteger numberEntryCountParam;
                    iViewCommandHandle->ExecuteCommandL( 
                            EPhoneViewGetNumberEntryCount,
                            &numberEntryCountParam );
                    
                    TInt neLength( numberEntryCountParam.Integer() );
                    if ( neLength > 0 )
                        {
                        HandleSendCommandL();
                        }
                    }               
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
        case MEngineMonitor::EPEMessageInitiatedMoCall:
           if( !IsBubbleInitialized() &&
              ( EPECallTypeCSVoice == 
                iStateMachine->PhoneEngineInfo()->CallType(aCallId) ) )
                {
                // Display call setup ASAP.
                DisplayInitializingCallL( aCallId );
                }
            break;
                    
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
            if ( !iOnScreenDialer )  
                {
                if ( IsNumberEntryUsedL() )
                    {
                    // Show the number entry if it exists
                    SetNumberEntryVisibilityL(ETrue);
                    }    
                else if ( NeedToReturnToForegroundAppL() )
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
                }
            break;
            
        case MEngineMonitor::EPEMessageIdle:
            HandleIdleL( aCallId );
            break;
            
        case MEngineMonitor::EPEMessageInValidEmergencyNumber:
            // if sim is ok then there is no need show this note.
            if ( !IsSimOk() )
                {
                SendGlobalWarningNoteL( EPhoneEmergencyCallsOnly );
                }
            break;
            
        case MEngineMonitor::EPEMessageValidEmergencyNumber:
            /* DisplayInitializingCallL calls EPEMessageCheckEmergencyNumber which
            means that engine will send EPEMessageInValidEmergencyNumber or
            EPEMessageValidEmergencyNumber response to phoneapp. If phoneengine
            sends EPEMessageValidEmergencyNumber then iBubbleInitialized should be
            set to EFalse this way we dont initializing call bubble behind attempting emergency 
            call dialog.*/
            if ( IsBubbleInitialized() )
                {
                SetBubbleInitialized( EFalse );
                }
            else
                {
                DialVoiceCallL();
                }
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
    
    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNaviPaneAudioVolume );

    SetRingingTonePlaybackL( aCallId );
    
    CPhonePubSubProxy::Instance()->ChangePropertyValue(
        KPSUidScreenSaver,
        KScreenSaverAllowScreenSaver,
        EPhoneScreensaverNotAllowed );
    
    BeginUiUpdateLC();
    
    // Hide the number entry if it exists
    if ( IsNumberEntryUsedL() )
        {
        SetNumberEntryVisibilityL(EFalse);    
        }
     
    // Close fast swap window if it's displayed
    CEikonEnv::Static()->DismissTaskList();

    // Show incoming call buttons
    SetTouchPaneButtons( EPhoneIncomingCallButtons );
    SetTouchPaneButtonEnabled( EPhoneCallComingCmdSilent );
    SetTouchPaneButtonEnabled( EPhoneInCallCmdHold );
    
    // Display incoming call
    DisplayIncomingCallL( aCallId );

    EndUiUpdate();
    
    // Go to incoming state
    iCbaManager->UpdateIncomingCbaL( aCallId );
    UpdateSilenceButtonDimming();
    UpdateIncomingContextMenuL( aCallId );
    //Dim toolbar items
    SetToolbarDimming( ETrue );
    //request that dimmed toolbar is visible.
    iViewCommandHandle->HandleCommandL( EPhoneViewShowToolbar );
    
    ChangeTo( EPhoneStateIncoming );
         
    // Reset divert indication
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
    if( IsBubbleInitialized() )
        {
        UpdateCallBubbleL( aCallId );
        }
    else
        {
        ShowCallHandlingViewL( aCallId );
        }
    ChangeTo( EPhoneStateCallSetup );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::ShowCallHandlingViewL
// -----------------------------------------------------------
//
void CPhoneStateIdle::ShowCallHandlingViewL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
            "CPhoneStateIdle::ShowCallHandlingViewL( ) ");
    BeginUiUpdateLC();
    SetNumberEntryVisibilityL(EFalse);

    // Show call setup buttons
    SetTouchPaneButtons( EPhoneCallSetupButtons );
    SetTouchPaneButtonDisabled( EPhoneInCallCmdHold );
    
    // Display call setup 
    DisplayCallSetupL( aCallId );
    EndUiUpdate();
    
    // Remove any phone dialogs if they are displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
    
    // Go to call setup state
    iCbaManager->UpdateCbaL( EPhoneCallHandlingCallSetupCBA );

    SetToolbarDimming( ETrue );
    SetToolbarButtonLoudspeakerEnabled();
    SetToolbarButtonHandsetEnabled();
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
            
    // Remove the number entry
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
 
    // Display call setup header
    DisplayCallSetupL( aCallId );

    // Update single active call
    UpdateSingleActiveCallL( aCallId );
    
    // Update touch buttons
    SetTouchPaneButtons( EPhoneIncallButtons );
    SetToolbarDimming( EFalse );
    
    EndUiUpdate();
  
    // Go to single state
    iCbaManager->UpdateCbaL( EPhoneCallHandlingInCallCBA );
    ChangeTo( EPhoneStateSingle );
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
            {
            // Open number entry menubar
            TPhoneCmdParamInteger integerParam;  
                      
            TInt menuId( GetNumberAcqMenuIdL() );
            integerParam.SetInteger( menuId );   

            iViewCommandHandle->ExecuteCommandL( 
                EPhoneViewMenuBarOpen, &integerParam );
            break;
            }
        
        case EPhoneNumberAcqCmdCall:
            HandleVoiceCallCommandL( EFalse );
            break;
        case EPhoneNumberAcqCmdSendCommand:
            HandleSendCommandL();
            break;
            
        case EPhoneNumberAcqCmdVideoCall:
            DialVideoCallL();
            break;
            
        case EPhoneCmdBack:
            BeginTransEffectLC( ENumberEntryClose );
            HandleBackCommandL();
            EndTransEffect();
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
           {
           // Open number entry OK menubar
           TPhoneCmdParamInteger integerParam;
           integerParam.SetInteger( 
               CPhoneMainResourceResolver::Instance()->
               ResolveResourceID( EPhoneNumberAcqOkMenubar ) );
           iViewCommandHandle->ExecuteCommandL( 
               EPhoneViewMenuBarOpen, &integerParam );
           commandStatus = ETrue;
           }
           break;       
           
       case EPhoneViewOpenNumberEntry:   
           {
           BeginTransEffectLC( ENumberEntryOpen );
           commandStatus = CPhoneState::ProcessCommandL( aCommand );
           EndTransEffect();
           }
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
// CPhoneStateIdle::HandleBackCommandL
// -----------------------------------------------------------
//
void CPhoneStateIdle::HandleBackCommandL()
    {
    TBool previousApp(EFalse);

    // If previous app activation is true then open
    // previous application.
    if ( iOnScreenDialer && 
        IsSimOk() )
        {
        TPhoneCmdParamBoolean booleanParam;
        iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewGetActivatePreviousApp, &booleanParam );
        

        if ( booleanParam.Boolean() )
            {
            // Open previous app.
            iViewCommandHandle->ExecuteCommandL( 
                EPhoneViewActivatePreviousApp );

            // Remove number entry from screen
            iViewCommandHandle->ExecuteCommandL( 
                EPhoneViewRemoveNumberEntry ); 

            iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateFSW ); 
            previousApp = ETrue;   
            }        
        }
        
    if ( !previousApp )
        {
        // Remove number entry from screen
        iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewRemoveNumberEntry );
         
        // Do state-specific operation when number entry is cleared
        HandleNumberEntryClearedL();
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
            if ( IsSpeedDialNumber( (*phoneNumber) ) )
                {
                // Handle speed dial
                SpeedDialL( (*phoneNumber)[0], EDialMethodMenuSelection );
                }
            else
                {
                // call the number
                iStateMachine->PhoneEngineInfo()->SetPhoneNumber( *phoneNumber );
                DialMultimediaCallL();
                }

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
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    // Remove any phone dialogs if they are displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );

    // Capture keys when there is an incoming call
    CaptureKeysDuringCallNotificationL( ETrue );

    // Indicate that the Foreground application needs to be sent to the foreground 
    // after call is ended.
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
    
    // Close menu bar, if it is displayed
    iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );

    // Capture keys when the phone is dialling
    CaptureKeysDuringCallNotificationL( ETrue );
    
    // Bring Phone app in the foreground
    TPhoneCmdParamInteger uidParam;
    uidParam.SetInteger( KUidPhoneApplication.iUid );
    iViewCommandHandle->ExecuteCommandL( EPhoneViewBringAppToForeground,
        &uidParam );

    // Set Phone as the top application
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetTopApplication,
        &uidParam ); 

    if( !IsBubbleInitialized() )
        {
        // Create and Display call setup header if update was not done.
        DisplayHeaderForOutgoingCallL( aCallId );
        }
    else 
        {
        /* Create and Display initializing call header, difference
        to DisplayHeaderForOutgoingCallL is that this doesnt load 
        CLI information to header.*/
        DisplayHeaderForInitializingCallL( aCallId );
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::UpdateCallBubbleL
// -----------------------------------------------------------
//
TBool CPhoneStateIdle::UpdateCallBubbleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
                "CPhoneStateIdle::UpdateCallBubbleL( )");
    TBool retValue(EFalse);
    if( IsBubbleInitialized() )
        {
        // Call bubble is already initialized so it is 
        // enough just to update existing header with 
        // contacts,number...etc info.
        TPhoneCmdParamCallHeaderData callHeaderParam = 
                UpdateCallHeaderInfoL( aCallId );
        iViewCommandHandle->ExecuteCommandL( 
                EPhoneViewUpdateCallHeaderRemoteInfoData, 
                aCallId, 
                &callHeaderParam );
        // reset flag.
        SetBubbleInitialized( EFalse );
        retValue = ETrue;
        }
    __PHONELOG1( EBasic, EPhoneControl, 
            "PhoneUIControl: CPhoneStateIdle::UpdateCallBubbleL - retValue =%d ",
            retValue );
    return retValue;
    }

// -----------------------------------------------------------
// CPhoneStateIdle::DisplayInitializingCallL
// -----------------------------------------------------------
//
void CPhoneStateIdle::DisplayInitializingCallL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::DisplayInitializingCallL( )");
    /*Set flag value to true before SendPhoneEngineMessage because if phone number
    is emergency number then EPEMessageValidEmergencyNumber message handling sets 
    iBubbleInitialized to false value and if value is false then initializing call 
    bubble shouldnt be shown.*/
    SetBubbleInitialized( ETrue );
    iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageCheckEmergencyNumber );
    if ( IsBubbleInitialized() )
        {
        ShowCallHandlingViewL( aCallId );
        }
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
        // If numberentry is used then we need to call EPhoneViewSetDialerControlVisible 
        // to ensure that numberentry/dialler is drawn to UI.
        TPhoneViewResponseId respond = 
            iViewCommandHandle->HandleCommandL( EPhoneViewSetDialerControlVisible );
        
        if ( respond && IsNumberEntryVisibleL() )
            {
            // Set Number Entry CBA
            iCbaManager->SetCbaL( EPhoneNumberAcqCBA );
            }
        }
    else if ( !IsAnyQueryActiveL() )
        {
        // Set idle as top application
        iViewCommandHandle->ExecuteCommandL( EPhoneViewBringIdleToForeground );
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandlePhoneForegroundEventL
// -----------------------------------------------------------

EXPORT_C void CPhoneStateIdle::HandlePhoneForegroundEventL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::HandlePhoneForegroundEventL( ) ");
    
    TBool activatePhone = CPhonePubSubProxy::Instance()->Value(
                    KPSUidAiInformation, KActiveIdleState ) == EPSAiNumberEntry;
    
    if ( activatePhone )
        {
        // If activatePhone is true then user has pressed some numeric
        // key and icon must be set to FSW. In this case number entry is not yet open so it
        // it is not enough to call EPhoneViewUpdateFSW.
        iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdatePhoneIconToFSW );
        }
    // If number entry is used update FSW accordingly.
    else if ( IsNumberEntryUsedL() )
        {
        // If numberentry is used then we need to call EPhoneViewSetDialerControlVisible 
        // to ensure that numberentry/dialler is drawn to UI.
        TPhoneViewResponseId respond = 
            iViewCommandHandle->HandleCommandL( EPhoneViewSetDialerControlVisible );
        
        if ( respond && IsNumberEntryVisibleL() )
            {
            // Set Number Entry CBA
            iCbaManager->SetCbaL( EPhoneNumberAcqCBA );
            }
        
        // If dialer is open add icon to FSW list.
        iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateFSW );
        }
    /*iBubbleInitialized has to be checked because if it true then phoneapp 
    is already showing call bubble and we cant bring idle to screen. */
    else if ( !IsNumberEntryUsedL() && 
              !iStateMachine->SecurityMode()->IsSecurityMode() &&
              !IsBubbleInitialized() )
        {
        // Bring Idle app to the foreground
        iViewCommandHandle->ExecuteCommandL( EPhoneViewBringIdleToForeground );
        }
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
         // If NE/dialer is open add icon to FSW list.
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
        
    // Remove all call headers
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveAllCallHeaders );
    
    // Close number entry
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );

    // Remove all notes and dialogs
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
    // Reset flag.
    SetBubbleInitialized( EFalse );
    }
 
// -----------------------------------------------------------
// CPhoneStateIdle::SpeedDialL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::SpeedDialL( const TUint& aDigit, 
        TDialInitiationMethod aDialMethod )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::SpeedDialL( ) ");
    iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );
    
    TPhoneCmdParamSpeedDial speedDialParam;
    HBufC* phoneNumber = NumberForSpeedDialLocationL( aDigit, speedDialParam );

    // Store serviceId received from vmbx handler
    iStateMachine->PhoneEngineInfo()->SetServiceIdCommand( speedDialParam.ServiceId() );
    CleanupStack::PushL( phoneNumber );
    
    if ( NULL != phoneNumber && KNullDesC() != *phoneNumber )
        {
        DialL( *phoneNumber, speedDialParam.NumberType(), aDialMethod );
        RemoveNumberEntryAndSetIdleToBackgroundIfNeededL();
        }
    else
        {
        // User cancelled dialog or didn't give a valid number
        SpeedDialCanceledL( aDigit );
        }
    CleanupStack::PopAndDestroy( phoneNumber );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::RemoveNumberEntryAndSetIdleToBackgroundIfNeededL
// -----------------------------------------------------------
//
void CPhoneStateIdle::RemoveNumberEntryAndSetIdleToBackgroundIfNeededL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
    "CPhoneStateIdle::RemoveNumberEntryAndSetIdleToBackgroundIfNeededL( ) ");
    if ( IsNumberEntryUsedL() )
        {
        /*NE should be removed because if speeddial dial is interupted during
        call setup phone should not return to NE/Dialler view.*/
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        
        /*It might be possible that some async operation was ongoing and 
        state has already in that case do not set idle to background. If 
        there is initialized call then do not set idle to background.*/
        if ( ( iStateMachine->State() == this ) && 
            ( !IsBubbleInitialized() ) )
            {
            SetupIdleScreenInBackgroundL();
            }
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle::SpeedDialCanceledL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::SpeedDialCanceledL( const TUint& aDigit )

    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::SpeedDialCanceledL( ) ");
    if ( IsNumberEntryUsedL() )
        {
        if ( aDigit == KPhoneDtmf1Character || 
                  ( aDigit == KPhoneDtmf2Character && 
                    iViewCommandHandle->HandleCommandL( 
                     EPhoneViewIsStraightCallToVideoMailBox ) == EPhoneViewResponseSuccess ) ) 
            {
            // Remove number entry and set idle background on non-touch products
            if ( !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
                {
                iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
                
                // Set Idle background, if still idle ( f.e not incoming call arrived )
                if ( iStateMachine->State() == this )
                    {
                    // idle screen in the background
                    SetupIdleScreenInBackgroundL();
                    }
                }
            }
        }        
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleNumberLongKeyPressL()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::HandleNumberLongKeyPressL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneStateIdle::HandleNumberLongKeyPressL( ) ");
    
    HBufC* phoneNumber = PhoneNumberFromEntryLC();
    
    if ( phoneNumber && IsSpeedDialNumber ( *phoneNumber ) )
        {
        SpeedDialL( (*phoneNumber)[0], EDialMethodOneKeyDialing );
        }
    
    CleanupStack::PopAndDestroy( phoneNumber );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::IsSpeedDialNumber()
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneStateIdle::IsSpeedDialNumber( 
        const TDesC& aNumber ) const
    {
    return ( aNumber.Length() == KPhoneDtmfSpeedDialPrefixLength 
            && aNumber[0] >= KPhoneDtmf1Character 
            && aNumber[0] <= KPhoneDtmf9Character );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleSendCommandL()
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::HandleSendCommandL()
    {
    __LOGMETHODSTARTEND(
        EPhoneControl, "CPhoneStateIdle::HandleSendCommandL()" );
    
    HandleVoiceCallCommandL( ETrue );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::LaunchApplicationL()
// -----------------------------------------------------------
//
void CPhoneStateIdle::LaunchApplicationL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::LaunchApplicationL( ) ");
    // Check if only one number exists in the number
    // entry
    TPhoneCmdParamInteger numberEntryCountParam;
    iViewCommandHandle->ExecuteCommandL( EPhoneViewGetNumberEntryCount,
        &numberEntryCountParam );

    if ( numberEntryCountParam.Integer() == 1 )
        {
        // Remove the number entry window
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        
        iStateMachine->SendPhoneEngineMessage( 
                MPEPhoneModel::EPEMessageEndDTMF );

        // Launch application.
        iViewCommandHandle->ExecuteCommandL(
            EPhoneViewLaunchApplication );

        // Continue displaying current app but set up the 
        // idle screen in the background
        SetupIdleScreenInBackgroundL();
        }
    }

// -----------------------------------------------------------
// CPhoneStateIdle:SendExitCommandL
// -----------------------------------------------------------
//
void CPhoneStateIdle::SendExitCommandL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneStateIdle::SendExitCommand( ) ");
    // Remove number entry from screen
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
        iViewCommandHandle->ExecuteCommandL( EPhoneViewExitApplications );
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
                // Close all connections
                iStateMachine->SendPhoneEngineMessage(
                        MPEPhoneModel::EPEMessageTerminateAllConnections );   

                if ( IsNumberEntryUsedL() )
                    {
                    BeginTransEffectLC( ENumberEntryClose );
                    // Remove number entry from screen
                    iViewCommandHandle->ExecuteCommandL( 
                        EPhoneViewRemoveNumberEntry );
                    EndTransEffect();
                    // Do state-specific operation when number entry is cleared
                    HandleNumberEntryClearedL();
                    
                    // If long end key event occures then all calls are terminated and
                    // dialer is closed, therefore tel.icon must be removed from FSW.
                    iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateFSW );
                    }
                if ( !TopAppIsDisplayedL() )
                    {
                    // Display idle screen
                    DisplayIdleScreenL();
                    }
                }
            else if ( IsNumberEntryUsedL() && TopAppIsDisplayedL() )
                {
                // Remove number entry from screen with effect
                BeginTransEffectLC( ENumberEntryClose );
                iViewCommandHandle->ExecuteCommandL( 
                    EPhoneViewRemoveNumberEntry );
                EndTransEffect();
                // Do state-specific operation when number entry is cleared
                HandleNumberEntryClearedL();
                }
            else if ( !TopAppIsDisplayedL() )
                {
                // Phone might not be the topmost app since it has
                // some dialog/query open therefore we need to remove dialog/phone.
            
                if ( !IsSimStateNotPresentWithSecurityModeEnabled() )
                    {
                    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemovePhoneDialogs );
                    }
                // Remove number entry from screen
                iViewCommandHandle->ExecuteCommandL( 
                        EPhoneViewRemoveNumberEntry );
                    
                // Bring Idle app to the foreground
                iViewCommandHandle->ExecuteCommandL( EPhoneViewBringIdleToForeground );
                
                // Updates Task list
                iViewCommandHandle->ExecuteCommandL( EPhoneViewUpdateFSW );

                }
            }
        else if ( IsNumberEntryUsedL() )
            {
            // Remove number entry from screen
            iViewCommandHandle->ExecuteCommandL( 
                EPhoneViewRemoveNumberEntry );
            StartShowSecurityNoteL();    
            }
        }
    }

// ---------------------------------------------------------
// CPhoneStateIdle::OnlyHashInNumberEntryL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::OnlyHashInNumberEntryL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneStateIdle::OnlyHashInNumberEntryL( ) ");
    
    if ( CPhoneCenRepProxy::Instance()->IsTelephonyFeatureSupported( 
        KTelephonyLVFlagMannerMode ) )
        {
        ChangeMannerModeL();
        }            
    else
        {
        StartAlsLineChangeTimerL();
        }

    // Check if any application launch functionality
    // is configured for pressing hash for a long time
    // the return value of CheckAppLaunchingL ignored.
    CheckAppLaunchingL( TKeyCode( KPhoneDtmfHashCharacter ) );
    }

// ---------------------------------------------------------
// CPhoneStateIdle::CanTransEffectTypeBeUsed
// ---------------------------------------------------------
//
EXPORT_C TBool CPhoneStateIdle::CanTransEffectTypeBeUsed( TStateTransEffectType aType )
    {
    TBool okToUse( EFalse );
    // These effects can be used for transitions between
    // phone and some other app.
    if ( aType == ENumberEntryOpen || 
         aType == ENumberEntryClose ||
         aType == ENumberEntryCreate )
        {
        okToUse = ETrue;
        }
    return okToUse;
    }

// ---------------------------------------------------------
// CPhoneStateIdle::ChangeMannerModeL
// ---------------------------------------------------------
//
void CPhoneStateIdle::ChangeMannerModeL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneStateIdle::ChangeMannerModeL( ) ");

    // Get the profile information
    MProEngEngine* profileEngine = ProEngFactory::NewEngineLC();
    const TInt profileId =
            profileEngine->ActiveProfileId();
    
    TInt newProfile;
    
    if ( profileId == EProfileSilentId )
        {
        newProfile = EProfileGeneralId;
        }
    else
        {
        newProfile = EProfileSilentId;    
        }
    
    profileEngine->SetActiveProfileL( newProfile );
    
    CleanupStack::Pop(); // profileEngine 
    profileEngine->Release();
    
    // Stop playing DTMF tone
    iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF ); 
    
    if ( !iOnScreenDialer )
        {
        // Remove the number entry
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );        
        }
    else
        {
        NumberEntryClearL();
        }
    
    iCbaManager->UpdateCbaL( EPhoneEmptyCBA );
 
     // Bring Idle app to the top app
    iViewCommandHandle->ExecuteCommandL( EPhoneViewSetIdleTopApplication );            
    }

// -----------------------------------------------------------
// CPhoneStateIdle::DialMultimediaCallL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::DialMultimediaCallL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneStateIdle::DialMultimediaCall() ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );
    
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

// -----------------------------------------------------------------------------
// CPhoneStateIdle::CheckAppLaunchingL
// -----------------------------------------------------------------------------
//
TBool CPhoneStateIdle::CheckAppLaunchingL( const TKeyCode aCode )
    {
    TBool valuesFetched( EFalse );
    
    if( !IsSimOk() )
        {
        return valuesFetched;
        }
    
    TPhoneCmdParamAppInfo appInfo;
    HBufC8* appParam = HBufC8::NewL( KMaxParamLength );
    CleanupStack::PushL( appParam );

    TBool isValidAppLaunchingKeyEvent = ETrue;
    
    // By-pass checking of number entry length in case of #-character.
    // #-character is handled differently from other keys, and number 
    // entry length has been checked earlier. Checking number entry length
    // for #-character is difficult here, because we cannot know if FEP
    // has removed the long-# from number entry or not.
    if ( TKeyCode( KPhoneDtmfHashCharacter ) != aCode )
        {
        TPhoneCmdParamInteger numberEntryCount;
        iViewCommandHandle->ExecuteCommandL( EPhoneViewGetNumberEntryCount,
            &numberEntryCount );
    
        isValidAppLaunchingKeyEvent = ( numberEntryCount.Integer() == 1 );
        }
    
    if ( isValidAppLaunchingKeyEvent )
        {
        TRAPD( err, 
            CPhoneCenRepProxy::Instance()->FetchValuesFromCenRepL( 
            appInfo, aCode, appParam, valuesFetched ) );
        
        if ( KErrNone == err && valuesFetched )
            {
            // Remove the number entry window
            iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );

            // Stop playing DTMF tone
            iStateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );

            TPhoneCommandParam* phoneCommandParam = 
                static_cast<TPhoneCommandParam*>( &appInfo );
            iViewCommandHandle->ExecuteCommandL( 
                        EPhoneViewActivateAppViewConventional,
                        phoneCommandParam ); 
            
            // Continue displaying current app but set up the 
            // idle screen in the background
            SetupIdleScreenInBackgroundL();
            }
        }
    CleanupStack::PopAndDestroy( appParam );
    return valuesFetched;
    }

// -----------------------------------------------------------
// CPhoneStateIdle::NumberForSpeedDialLocationL
// -----------------------------------------------------------
// 
HBufC* CPhoneStateIdle::NumberForSpeedDialLocationL( 
        const TUint& aDigit,
        TPhoneCmdParamSpeedDial& aSpeedDialParam ) const
    {
    // Set the speed dial location from the digit in buffer. Character
    // '1' means location 1, and so on.
    const TInt location = aDigit - KPhoneDtmf1Character + 1;
    aSpeedDialParam.SetLocation( location );
    
    // Get the number and type from the speed dial location.
    HBufC* phoneNumber = HBufC::NewLC( KPhoneNumberEntryBufferSize );
    
    TPtr ptr( phoneNumber->Des() );
    aSpeedDialParam.SetPhoneNumber( &ptr );
    iViewCommandHandle->HandleCommandL( 
        EPhoneViewGetNumberFromSpeedDialLocation, &aSpeedDialParam );

    CleanupStack::Pop( phoneNumber );
    return phoneNumber;
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleVoiceCallCommandL()
// -----------------------------------------------------------
//
void CPhoneStateIdle::HandleVoiceCallCommandL( TBool aSendKey )
    {
    __LOGMETHODSTARTEND(
        EPhoneControl, "CPhoneStateIdle::HandleVoiceCallCommandL()" );
    
    if ( IsNumberEntryUsedL() )
        {
        // Handle send key short press, get the number entry contents.
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
            if ( IsDialingExtensionInFocusL() )
                {
                CPhoneState::HandleCommandL( EEasyDialingVoiceCall );
                }
            
            else if ( IsSpeedDialNumber( *phoneNumber ) )
                {
                // Handle speed dial
                SpeedDialL( (*phoneNumber)[0], aSendKey ? EDialMethodSendCommand : EDialMethodMenuSelection );
                }
            else
                {
                // call the number
                iStateMachine->PhoneEngineInfo()->SetPhoneNumber( *phoneNumber );
                DialVoiceCallL();
                }
            
            CleanupStack::PopAndDestroy( phoneNumber );
            }
        }    
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleVoiceCallCommandL()
// -----------------------------------------------------------
//
TInt CPhoneStateIdle::GetNumberAcqMenuIdL()
    {
    TInt menuId = CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneNumberAcqMenubar );
    
    // Use easy dialing menu id, if easydialing is in focus.
    // Otherwise use number acquisition menu id.
    if ( IsDialingExtensionInFocusL() )
        {
        // Fetch easydialing menu id, check its validity and assign to menuId
        TPhoneCmdParamInteger integerParam;  
        iViewCommandHandle->ExecuteCommandL(
                EPhoneViewGetEasyDialingMenuId, &integerParam );
        if ( integerParam.Integer() ) 
            {
            menuId = integerParam.Integer();
            }
        }
    return menuId;
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleErrorL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStateIdle::HandleErrorL( const TPEErrorInfo& aErrorInfo )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, "CPhoneStateIdle::HandleErrorL() ");
    __PHONELOG1( EBasic, EPhoneControl, 
            "PhoneUIControl: CPhoneStateIdle::HandleErrorL - aErrorInfo.iErrorCode =%d ",
            aErrorInfo.iErrorCode );
    __PHONELOG1( EBasic, EPhoneControl,
            "PhoneUIControl: CPhoneStateIdle::HandleErrorL - aErrorInfo.iCallId =%d ",
            aErrorInfo.iCallId );
    if( ( KErrNotFound < aErrorInfo.iCallId ) && 
        ( ECCPErrorNone != aErrorInfo.iErrorCode ) &&
          IsBubbleInitialized() )
        {
        HandleInitializingCallErrorL( aErrorInfo.iCallId );
        SetBubbleInitialized( EFalse );
        }
    CPhoneState::HandleErrorL( aErrorInfo );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::HandleInitializingCallErrorL
// -----------------------------------------------------------
//
void CPhoneStateIdle::HandleInitializingCallErrorL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(
            EPhoneControl, 
            "CPhoneStateIdle::HandleInitializingCallErrorL()" );
    // Remove call 
    iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveCallHeader, aCallId );
    BeginUiUpdateLC();
    SetDefaultFlagsL();
    if ( IsNumberEntryUsedL() )
        {
        if ( NeedToReturnToForegroundAppL() )
            {
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSendToBackground );
            iViewCommandHandle->ExecuteCommandL( EPhoneViewSetControlAndVisibility );
            iCbaManager->SetCbaL( EPhoneNumberAcqCBA );
            }
        else
            {
            // Show the number entry if it exists and update cba's.
            SetNumberEntryVisibilityL( ETrue );
            if ( IsDTMFEditorVisibleL() )
                {
                CloseDTMFEditorL();
                DisplayIdleScreenL();
                }
            else if ( iOnScreenDialer && IsCustomizedDialerVisibleL() )
                {
                CloseCustomizedDialerL();
                DisplayIdleScreenL();
                } 
            }
        }
    else
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewMenuBarClose );
        if ( !TopAppIsDisplayedL() || IsAutoLockOn() || NeedToReturnToForegroundAppL() )
            {
            // Continue displaying current app but set up the 
            // idle screen in the background
            SetupIdleScreenInBackgroundL();
            }
        else
            {
            DisplayIdleScreenL();
            }
        }
    DeleteTouchPaneButtons();
    EndUiUpdate();
    
    // Display call termination note, if necessary
    DisplayCallTerminationNoteL();
    iViewCommandHandle->ExecuteCommandL( EPhoneViewHideToolbar );
    }

// -----------------------------------------------------------
// CPhoneStateIdle::SetBubbleInitialized
// -----------------------------------------------------------
//
void CPhoneStateIdle::SetBubbleInitialized( TBool aValue )
    {
    __PHONELOG1( EBasic, EPhoneControl, 
            "PhoneUIControl: CPhoneStateIdle::SetBubbleInitialized - aValue =%d ",
            aValue );
    iBubbleInitialized = aValue;
    }

// -----------------------------------------------------------
// CPhoneStateIdle::IsBubbleInitialized
// -----------------------------------------------------------
//
TBool CPhoneStateIdle::IsBubbleInitialized()
    {
    __PHONELOG1( EBasic, EPhoneControl, 
            "PhoneUIControl: CPhoneStateIdle::IsBubbleInitialized - iBubbleInitialized =%d ",
            iBubbleInitialized );
    return iBubbleInitialized;
    }

// -----------------------------------------------------------
// CPhoneStateIdle::ChangeTo
// -----------------------------------------------------------
//
void CPhoneStateIdle::ChangeTo( TInt aState )
    {
    __PHONELOG1( EBasic, EPhoneControl, 
           "PhoneUIControl: CPhoneStateIdle::ChangeTo - aState =%d ",
           aState );
    SetBubbleInitialized( EFalse );
    iStateMachine->ChangeState( aState );
    }

// End of File
