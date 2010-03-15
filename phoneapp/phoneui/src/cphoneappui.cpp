/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Application UI class implementation. 
*
*/


// INCLUDE FILES
#include <eikenv.h>
#include <coemain.h>
#include <coehelp.h>
#include <bacntf.h>
#include <starterclient.h>
#include <telinternalpskeys.h>
#include <activeidle2domainpskeys.h>
#include <telremotepartyinformationpskeys.h>
#include <telinformationpskeys.h>
#include <UikonInternalPSKeys.h>
#include <startupdomainpskeys.h>
#include <apgtask.h>
#include <apgwgnam.h>
#include <barsread.h>
#include <coemain.h>
#include <AknsConstants.h>   // KAknsMessageSkinChange
#include <bautils.h>
#include <StringLoader.h>
#include <aknsoundsystem.h>
#include <aknnotewrappers.h> // for Notification for startup...
#include <eikbtgpc.h>
#include <eiksoftkeyimage.h> // EikSoftkeyImage
#include <featmgr.h>         // Feature Manager
#include <bldvariant.hrh>    // System variant definition
#include <phoneui.rsg>
#include "phoneui.pan"
#include "cphoneappui.h"
#include "cphoneapplication.h"
#include "cphonedocument.h"
#include "cphonekeys.h"
#include "cphoneview.h" // from PhoneUIView
#include "cphoneviewcontroller.h"
#include "cphoneuicontroller.h"
#include "cphonepubsubproxy.h"
#include "phonelogger.h"
#include "phoneconstants.h"
#include "cphoneclearblacklist.h"
#include "cphonestatuspane.h"
#include "cphoneqwertyhandler.h"
#include "tphonecmdparaminteger.h"
#ifndef __WINS__
#include <f32file.h>
#endif


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPhoneAppUI::CPhoneAppUI()
// Default constructor
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPhoneAppUI::CPhoneAppUI() : 
    iAppsReady( EPhoneStartupInitial )
    {            
    }

// ---------------------------------------------------------
// CPhoneAppUI::~CPhoneAppUI()
// Destructor
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPhoneAppUI::~CPhoneAppUI()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneAppUI::~CPhoneAppUI() ");
    if ( iPhoneViewController )
        {
        RemoveFromStack( iPhoneViewController->PhoneView() );
        delete iPhoneViewController;
        }
    delete iPhoneUIController;
    delete iQwertyHandler;
    
    CPhoneRecoverySystem::Remove( iStartupSignalRecoveryId );

    delete iEnvChangeNotifier;
    iEnvChangeNotifier = NULL;
    }

// -----------------------------------------------------------------------------
// CPhoneAppUI::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::ConstructL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneAppUI::ConstructL() ");
    // Requires the availability of CBA and Menubar resources
    BaseConstructL( EAknEnableSkin | EAknEnableMSK | EAknSingleClickCompatible );
    SetKeyEventFlags( CAknAppUiBase::EDisableSendKeyShort | CAknAppUiBase::EDisableSendKeyLong );
    
    // Item specific soft key IS DISABLED in order to make SK1 visible with easy dialing.
    CEikCba* cba = static_cast<CEikCba*>( CEikButtonGroupContainer::Current()->ButtonGroup() );
    cba->EnableItemSpecificSoftkey( EFalse );
    
    SetAliasKeyCodeResolverL( this );
    
    // Set phone application to be system application so it can't be closed
    iEikonEnv->SetSystem( ETrue );

    // Disable priority changes of window server and set
    // process priority (phone app might be on the background
    // and so it's priority might be low).
    iEikonEnv->WsSession().ComputeMode( 
        RWsSession::EPriorityControlDisabled ); 
    RThread().SetProcessPriority( EPriorityHigh  );

    // System enviroment change notifier.
    iEnvChangeNotifier = CEnvironmentChangeNotifier::NewL( 
        CActive::EPriorityStandard, TCallBack( HandleCallBackL, this ) );
        
    iEnvChangeNotifier->Start();

    // Push key sounds to sound system.
    CAknKeySoundSystem* keySounds = KeySounds();
    keySounds->PushContextL( R_PHONEUI_DEFAULT_SKEY_LIST );
   
    iPhoneViewController = CPhoneViewController::NewL( ClientRect() );
    iPhoneUIController = CPhoneUIController::NewL( iPhoneViewController );
    iQwertyHandler = CPhoneQwertyHandler::NewL();        

    // Blacklist singleton is initialized here
    // return value is ignored since we do not require its services in this class
    CPhoneClearBlacklist::CreateL( 
        iEikonEnv->WsSession(), 
        *iPhoneViewController->PhoneView()->DrawableWindow() );

    RegisterViewL( *iPhoneViewController->PhoneView() );
    AddToStackL( iPhoneViewController->PhoneView(), ECoeStackPriorityDefault );

    const_cast<CEikAppUi*>( 
        iEikonEnv->EikAppUi() )->AddViewActivationObserverL( this );

    // Eikon server window group
    iEikonServerWgId = 
        iEikonEnv->WsSession().FindWindowGroupIdentifier( 0, KPhoneEikonServer );
    __ASSERT_ALWAYS( iEikonServerWgId != KErrNotFound, Panic( EPhoneUiNoWindowGroup ) );
    iEikonEnv->RootWin().EnableFocusChangeEvents(); 
    iEikonEnv->RootWin().EnableGroupChangeEvents();

    // CLI Name.
    TInt err = RProperty::Define( 
        KPSUidTelRemotePartyInformation,
        KTelCLIName,
        RProperty::EText,
        KPhoneReadPolicy,
        KPhoneWritePolicy );

    __PHONELOG1( 
        EBasic, 
        EPhonePhoneapp, 
        "CPhoneAppUI::ConstructL() - Define CLIName property (status=%d)", 
        err );
      
    // CLI Number.
    err = RProperty::Define( 
        KPSUidTelRemotePartyInformation,
        KTelCLINumber,
        RProperty::EText,
        KPhoneReadPolicy,
        KPhoneWritePolicy );

    __PHONELOG1( 
        EBasic, 
        EPhonePhoneapp, 
        "CPhoneAppUI::ConstructL() - Define CLINumber property (status=%d)", 
        err );

    // Define the top application system property
    err = RProperty::Define( 
        KPSUidUikon,
        KUikVideoCallTopApp,
        RProperty::EInt,
        KPhoneReadPolicy,
        KPhoneWritePolicy );

    __PHONELOG1( 
        EBasic, 
        EPhonePhoneapp, 
        "CPhoneAppUI::ConstructL() Define top application property (status=%d)", 
        err );

    // Startup event signalling
    // Define the telephony application system property 
 
     err = RProperty::Define( 
        KPSUidTelInformation,
        KTelPhoneUid,
        RProperty::EInt,
        KPhoneReadPolicy,
        KPhoneWritePolicy );  

// DefineNewPSKeys:

    // UID: KPSUidTelInternalInformation:
    err = RProperty::Define( 
        KPSUidTelInternalInformation,
        KTelRemotePartyContactInfo,
        RProperty::EByteArray,
        KPhoneReadPolicy,
        KPhoneWritePolicy );    
               
    // VT UID.
    err = RProperty::Define( 
        KPSUidTelInternalInformation,
        KTelVideoCallUid,
        RProperty::EInt,
        KPhoneReadPolicy,
        KPhoneWritePolicy );

    // TELINTERNALPSKEY CHANGE 
    // Move to AudioHandling
    // UID: KPSUidTelAudioPreference
    // Call Audio Routing
    err = RProperty::Define( 
        KPSUidTelAudioPreference, 
        KTelAudioOutput, 
        RProperty::EInt,
        KPhoneReadPolicyAlwaysPass,
        KPhoneWritePolicy );

    // UID: KPSUidNEInformation
    err = RProperty::Define( 
        KPSUidNEInformation, 
        KTelNumberEntryInfo, 
        RProperty::EInt,
        KPhoneReadPolicyAlwaysPass,
        KPhoneWritePolicy );

    __PHONELOG1( 
        EBasic, 
        EPhonePhoneapp, 
        "CPhoneAppUI::ConstructL() Define phone property (status=%d)", 
        err );
           
    // Add DoStartupSignalL to the recovery system 
    iStartupSignalRecoveryId = 
        CPhoneRecoverySystem::Instance()->AddL( 
            TCallBack( DoStartupSignalL, this ),
            CTeleRecoverySystem::EPhonePriorityHigh,
            CTeleRecoverySystem::EPhoneStateIdle );

    // Call DoStartupSignalL within recovery system
    const TInt startupState = CPhonePubSubProxy::Instance()->Value(
        KPSUidStartup,
        KPSGlobalSystemState );

    __PHONELOG1( 
        EBasic, 
        EPhonePhoneapp, 
        "CPhoneAppUI::ConstructL() StartupState value", 
        startupState );
        
    if ( startupState == ESwStateCriticalPhaseOK ||
        startupState == ESwStateEmergencyCallsOnly ||
        startupState == ESwStateNormalRfOn ||
        startupState == ESwStateNormalRfOff ||
        startupState == ESwStateNormalBTSap )
        {
        __PHONELOG( 
            EBasic, 
            EPhonePhoneapp, 
            "CPhoneAppUI::ConstructL() Notify Starter that phone is ready..." );
            
        CPhoneRecoverySystem::Instance()->RecoverNow(
            iStartupSignalRecoveryId, 
            CTeleRecoverySystem::EPhonePriorityHigh );
        }
    else
        {        
        __PHONELOG( 
            EBasic, 
            EPhonePhoneapp, 
            "CPhoneAppUI::ConstructL() Startup not ready yet. Start listening..." );            
        CPhonePubSubProxy::Instance()->NotifyChangeL(
            KPSUidStartup,
            KPSGlobalSystemState,
            this );
        }


    // THE VERY LAST STEP: DROP PRIORITY.
    RThread().SetPriority( EPriorityNormal );

    if ( err == KErrNone || err == KErrAlreadyExists )
        {
        // Indicate to the system that the Phone has been started
        CPhonePubSubProxy::Instance()->ChangePropertyValue(
            KPSUidTelInformation,
            KTelPhoneUid,
            KUidPhoneApplication.iUid );
        }

    // Update the Apps ready flag
    iAppsReady += EPhoneStartedUp;
    
    if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) )     
        {
        iOnScreenDialer = ETrue;   
        }
    __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUI::ConstructL() Phone started..." );
    __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUI::ConstructL() done..." );
    }
   
// -----------------------------------------------------------------------------
// CPhoneAppUI::DoStartupSignalL
//
// Callback function.
// -----------------------------------------------------------------------------
//
TInt CPhoneAppUI::DoStartupSignalL( TAny* aAny )
    {
    CPhoneAppUI* appUi =
        static_cast< CPhoneAppUI* >( aAny );
    
    return
        !appUi->iLightIdleReached ?
            appUi->DoStartupSignalIdleL() :
            appUi->DoStartupSignalSecurityL();
    }

// -----------------------------------------------------------------------------
// CPhoneAppUI::DoStartupSignalIdleL
// -----------------------------------------------------------------------------
//
TInt CPhoneAppUI::DoStartupSignalIdleL()
    {
    TBool idleReached = EFalse;
    
    // Check if Idle application has been started.
    const TInt idleUid = CPhonePubSubProxy::Instance()->Value(
            KPSUidAiInformation,
            KActiveIdleUid );

    if ( idleUid != 0 && idleUid != KErrNotFound )
        {
        // Idle application has already started
        if ( !( iAppsReady & EPhoneIdleStartedUp ) )
            {
            __PHONELOG1( EBasic, EPhonePhoneapp, "CPhoneAppUI::ConstructL() Idle App started (uid=%d)!", idleUid );
            iAppsReady += EPhoneIdleStartedUp;
            
            // Remove Phone application from Fast Swap Window.
            iPhoneViewController->SetHiddenL( ETrue );
            
            // Set Idle's UID to PubSub.
            CPhonePubSubProxy::Instance()->ChangePropertyValue(
                KPSUidUikon,
                KUikVideoCallTopApp,
                idleUid );
            }
        }
    else
        {
        CPhonePubSubProxy::Instance()->NotifyChangeL(
            KPSUidAiInformation,
            KActiveIdleUid,
            this );   
        }

    const TInt current = CPhonePubSubProxy::Instance()->Value(
        KPSUidStartup,
        KPSGlobalSystemState );

    __PHONELOG1( EBasic, EPhonePhoneapp, "CPhoneAppUI::DoStartupSignalIdleL() System state=%d", current );
    if ( current == ESwStateSelfTestOK ||
        current == ESwStateCriticalPhaseOK )
        {                       
        // Check that both the Phone and Idle applications are ready
        idleReached = ( iAppsReady & EPhoneStartedUp ) &&
             ( iAppsReady & EPhoneIdleStartedUp );

        if ( idleReached )
            {
            __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUI::DoStartupSignalIdleL() Phone and Idle apps have both been started" );

            CPhonePubSubProxy::Instance()->ChangePropertyValue(
                KPSUidStartup,
                KPSPhonePhase1Ok,
                EPhonePhase1Ok );
            }
        }
    else 
        {
        idleReached = ( iAppsReady & EPhoneIdleStartedUp ) && 
            ( current == ESwStateEmergencyCallsOnly ||
            current == ESwStateNormalRfOn ||
            current == ESwStateNormalRfOff ||
            current == ESwStateNormalBTSap );

        __PHONELOG1( EBasic, EPhonePhoneapp, "CPhoneAppUI::DoStartupSignalIdleL() System state is not at SelfTestOk. Idle reached anyway?=%d", idleReached );
        }

    if ( idleReached )
        {
        __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUI::DoStartupSignalIdleL() Idle reached!" );
        // Now err == KErrNone and it means that
        // we have succeeded in signalling.
        CPhoneRecoverySystem::Instance()->EnablePreconditionL();

        // Update the Phone light idle flag
        iLightIdleReached = ETrue;

        // Notify the UI controller that the phone is ready
        iPhoneUIController->HandlePhoneStartupL();
        }
    else 
        {
        __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUI::DoStartupSignalIdleL() Idle is not reached yet!" );
        // Idle has not been reached yet. Indicate to the recovery
        // system that this iteration has failed so that it will
        // try again at a later time.
        User::Leave( KErrNotReady );
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPhoneAppUI::DoStartupSignalSecurityL
// -----------------------------------------------------------------------------
//
TInt CPhoneAppUI::DoStartupSignalSecurityL()
    {
    TInt err = KErrNone;    
    return err;
    }

// -----------------------------------------------------------
// CPhoneAppUI::HandlePropertyChangedL
// -----------------------------------------------------------
//
void CPhoneAppUI::HandlePropertyChangedL( 
    const TUid& aCategory,
    const TUint aKey,
    const TInt aValue )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneAppUI::HandlePropertyChangedL( ) ");

    if ( aCategory == KPSUidAiInformation )
        {
        // Call state event
        if ( aKey == KActiveIdleUid )
            {
            __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUi::HandleChangeL() Idle event" );
            TInt idleUid = aValue;
            
            // If idle startup is ok.
            if ( idleUid != 0 )
                {
                // If idle startup is ok for the first time, then recover
                // the Start signal processing
                if ( !( iAppsReady & EPhoneIdleStartedUp ) )
                    {            
                    __PHONELOG( EBasic, EPhonePhoneapp, 
                        "CPhoneAppUI::HandlePropertyChangedL() Idle App started!" );
                    iAppsReady += EPhoneIdleStartedUp;
                    // Try to update the startup signal again
                    CPhoneRecoverySystem::Instance()->RecoverNow(
                        iStartupSignalRecoveryId, 
                        CTeleRecoverySystem::EPhonePriorityHigh );    
                    // Remove Phone application from Fast Swap Window.
                    iPhoneViewController->SetHiddenL( ETrue );
                    // Set Idle's UID to PubSub.
                    CPhonePubSubProxy::Instance()->ChangePropertyValue(
                        KPSUidUikon,
                        KUikVideoCallTopApp,
                        idleUid );
                    }
                }
            }
        }

    if ( aCategory == KPSUidStartup )
        {
        __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUi::HandleChangeL() Starter event" );
        const TInt startupState = CPhonePubSubProxy::Instance()->Value(
            KPSUidStartup,
            KPSGlobalSystemState );

        __PHONELOG1( EBasic, EPhonePhoneapp, "CPhoneAppUI::HandleChangeL() StartupState value", startupState );
        if ( startupState == ESwStateCriticalPhaseOK ||
            startupState == ESwStateEmergencyCallsOnly ||
            startupState == ESwStateNormalRfOn ||
            startupState == ESwStateNormalRfOff ||
            startupState == ESwStateNormalBTSap )
            {
            // Try to update the startup signal again
            CPhoneRecoverySystem::Instance()->RecoverNow(
                iStartupSignalRecoveryId, 
                CTeleRecoverySystem::EPhonePriorityHigh );    
            }
        }
    }


// -----------------------------------------------------------------------------
// CPhoneAppUI::HandleEnvironmentChangeL
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::HandleEnvironmentChangeL( const TInt aChanges )
    {
    // Pass environment changes to the UI controller
    iPhoneUIController->HandleEnvironmentChangeL( aChanges );
    }

// -----------------------------------------------------------------------------
// CPhoneAppUI::HandleWsEventL
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::HandleWsEventL( 
    const TWsEvent& aEvent,
    CCoeControl* aDestination )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneAppUI::HandleWsEventL" );
    
    // Only handle window server events if startup is finished
    if ( iLightIdleReached )
        {
        __PHONELOG3( EBasic, EPhonePhoneapp, "CPhoneAppUI::HandleWsEvent( key(%d), iCode(%d), type(%d) )",
            aEvent.Key()->iScanCode, aEvent.Key()->iCode, aEvent.Type() );

        CEikonEnv& eikEnv = *iEikonEnv;
        CCoeEnv& coeEnv = eikEnv; 

        const TInt type = aEvent.Type();
        switch ( type )
            {
            case EEventFocusGained:
                {
                const TInt foregroundApp = 
                    iPhoneViewController->ForegroundApplicationWindowGroupId();
                const TInt thisApplicationWgId = 
                    iPhoneViewController->ApplicationWindowGroupId();
                
                if ( thisApplicationWgId != foregroundApp )
                    {
                    // Focus gained but phone is not in fore then phone need
                    // to bring idle to fore if needed.
                    iPhoneUIController->HandleIdleForegroundEventL();
                    }
                else
                    {
                    iPhoneUIController->HandlePhoneForegroundEventL();
                    }
                }
                break;
            
            case EEventFocusLost:
                {
                const TInt windowGroupId = 
                    coeEnv.WsSession().GetFocusWindowGroup();
                const TInt foregroundAppWgId = 
                    iPhoneViewController->ForegroundApplicationWindowGroupId();
                const TInt phoneApplicationWgId = 
                    iPhoneViewController->ApplicationWindowGroupId();
                // If phone app is not in fore and we receive focuslost then we
                // need to send focus lost event to state which then updates
                // FSW to correct state. 
                if ( ( foregroundAppWgId != phoneApplicationWgId ) && 
                        ( windowGroupId != phoneApplicationWgId ) && 
                        ( windowGroupId != iEikonServerWgId ) )
                    {
                    iPhoneUIController->HandlePhoneFocusLostEventL();
                    }
                }
                break;

            case EEventKey:
                {
                TKeyEvent* keyEvent = aEvent.Key();
                
                if ( keyEvent->iScanCode == EStdKeyNo )
                    {
                    NoKeySpecialHandlingL( *keyEvent, EEventKey );    
                    }
                    
                 //will be called if exit number is typed (*#1234#)
                if ( keyEvent->iCode == EEikCmdExit )
                    {
                    Exit();
                    return;     
                    }
                           
                // Check if key press is simulated by FEP or by touch dialer
                TBool fepSimulated = keyEvent->iModifiers & EModifierSpecial;               
                TInt modifierMask = ( EModifierNumLock | EModifierKeypad );
                TBool dialerSimulated = ( ( keyEvent->iModifiers & modifierMask ) == modifierMask );
                        
                if ( !fepSimulated && !dialerSimulated && iQwertyHandler->IsQwertyInput() )
                    {
                    TInt code = iQwertyHandler->NumericKeyCode( *keyEvent );                    
                    if ( code == EKeyNull && CPhoneKeys::IsNumericKey( *keyEvent, ( TEventCode ) aEvent.Type() ) )
                        {
                        code = keyEvent->iCode;
                        }
                    
                    if ( code != EKeyNull ) 
                        {
                        // Modify event according to keyboard layout
                        TWsEvent newWsEvent = aEvent;    
                        TKeyEvent* newKeyEvent = newWsEvent.Key();
                        newKeyEvent->iCode = code;
                        TInt shiftMask = EModifierLeftShift  | 
                                         EModifierRightShift | 
                                         EModifierShift;
                        shiftMask = ~shiftMask;
                        newKeyEvent->iModifiers = newKeyEvent->iModifiers & shiftMask;
                        return CAknAppUi::HandleWsEventL( newWsEvent, aDestination );
                        }
                    }
                }
                break;
                
            case EEventKeyUp:
                {
                TKeyEvent* keyEvent = aEvent.Key();
                
                if ( keyEvent->iScanCode == EStdKeyNo )
                    {
                    NoKeySpecialHandlingL( *keyEvent, EEventKeyUp );    
                    }    
                }
                break;
                
            case EEventKeyDown:
                {
                TKeyEvent* keyEvent = aEvent.Key();
                
                if ( keyEvent->iScanCode == EStdKeyNo )
                    {
                    NoKeySpecialHandlingL( *keyEvent, EEventKeyDown );    
                    }    
                }
                break;


            default:
                break;
            }            
        }
    // All events are sent to base class.
    CAknAppUi::HandleWsEventL( aEvent, aDestination );
    }


// ---------------------------------------------------------
// CPhoneAppUI::NoKeySpecialHandlingL
// Handle no key specific cases.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPhoneAppUI::NoKeySpecialHandlingL( const TKeyEvent& aEvent,
                                         TEventCode aType  )
    {
    CEikonEnv& eikEnv = *iEikonEnv;
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneAppUI::NoKeySpecialHandling" );
    
    // Handle case where menu bar is opened or a blocking dialog is 
    // displayed and the end key is pressed. HandleKeyEventL will not be 
    // called in this case so it has to be handled here.
    if ( eikEnv.AppUiFactory()->MenuBar()->IsDisplayed() ||
        iPhoneViewController->BlockingDialogIsDisplayed() )
        {
        // Send key event to the UI controller
        iPhoneUIController->HandleKeyEventL( aEvent,  aType );
        }    
    }

// ---------------------------------------------------------
// CPhoneAppUI::HandleKeyEventL
// Handle key events.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TKeyResponse CPhoneAppUI::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneAppUI::HandleKeyEventL" );
    // Let key event handler handle event. 
    return iPhoneUIController->HandleKeyEventL( aKeyEvent, aType );
    }

// -----------------------------------------------------------------------------
// CPhoneAppUI::ProcessCommandL
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::ProcessCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneAppUI::ProcessCommandL()" );

    if( !iPhoneUIController->ProcessCommandL( aCommand ) )
        {
		// State didn't handle the command so pass it up
        CAknAppUi::ProcessCommandL( aCommand );
        }
    
    }

// -----------------------------------------------------------------------------
// CPhoneAppUI::HandleCommandL
// 
// Forwards the request to the main view.
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneAppUI::HandleCommandL()" );
 
    switch (aCommand)
        {
        case EEikCmdExit:
            {
            __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUI::HandleCommandL -- EEikCmdExit" );
            Exit();
            }
            break;
        default:
            {
            if( !iPhoneUIController->HandleCommandL( aCommand ) )
                {
                // State didn't handle the command so pass it up
                CAknAppUi::HandleCommandL( aCommand );
                }
            }
            break;
       }
    }

// -----------------------------------------------------------------------------
// CPhoneAppUI::DynInitMenuPaneL
// 
// Forwards the request to the main view.
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane )
    {
    iPhoneUIController->DynInitMenuPaneL( aResourceId, aMenuPane );
    }

// ---------------------------------------------------------
// CPAAppUIS60Base::DynInitMenuBarL
// ---------------------------------------------------------
//
void CPhoneAppUI::DynInitMenuBarL(
    TInt aResourceId, CEikMenuBar* aMenuBar)
    {
    iPhoneUIController->DynInitMenuBarL( aResourceId, aMenuBar );
    }   

// -----------------------------------------------------------------------------
// CPhoneAppUI::HandleForegroundEventL
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::HandleForegroundEventL( TBool aForeground )
    {
    __PHONELOG1(
        EBasic,
        EPhonePhoneapp,
        "CPhoneAppUI::HandleForegroundEventL( InForeground=%d )",
        aForeground );
        
    iPhoneUIController->HandleForegroundEventL( aForeground );

    CAknAppUi::HandleForegroundEventL( aForeground );
    }

// -----------------------------------------------------------------------------
// CPhoneAppUI::HandleStatusPaneSizeChange
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::HandleStatusPaneSizeChange()
    {
    __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUI::HandleStatusPaneSizeChange" );
    CAknAppUi::HandleStatusPaneSizeChange();

    LayoutMainView();
    }

// -----------------------------------------------------------------------------
// CPhoneAppUI::HandleViewDeactivation
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::HandleViewDeactivation(
    const TVwsViewId& aViewIdToBeDeactivated, 
    const TVwsViewId& aNewlyActivatedViewId )
    {
    __PHONELOG2(
        EBasic,
        EPhonePhoneapp,
        "CPhoneAppUI::HandleViewDeactivation( aViewIdToBeDeactivated(%d), aNewlyActivatedViewId(%d) )",
        aViewIdToBeDeactivated.iViewUid, 
        aNewlyActivatedViewId.iViewUid );
        
    CAknAppUi::HandleViewDeactivation( 
        aViewIdToBeDeactivated, 
        aNewlyActivatedViewId );
    }

// -----------------------------------------------------------------------------
// CPhoneAppUI::HandleApplicationSpecificEventL
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::HandleApplicationSpecificEventL(
    TInt aType,
    const TWsEvent& aEvent )
    {
    __LOGMETHODSTARTEND( EPhonePhoneapp, "CPhoneAppUI::HandleApplicationSpecificEventL" );
    
    CAknAppUi::HandleApplicationSpecificEventL( aType, aEvent );
    }

// -----------------------------------------------------------------------------
// CPhoneAppUI::HandleResourceChangeL
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::HandleResourceChangeL( TInt aType )
    {
    __LOGMETHODSTARTEND( EPhonePhoneapp, "CPhoneAppUI::HandleResourceChangeL" );
    
    CAknAppUi::HandleResourceChangeL( aType );
            
    if ( aType == KAknsMessageSkinChange || 
         aType == KEikDynamicLayoutVariantSwitch )
        {
        LayoutMainView();
        iPhoneViewController->HandleResourceChangeL( aType );
        }
    }

// ---------------------------------------------------------
// CPhoneAppUI::HandleViewActivation
//  Handles the activation of the view aNewlyActivatedViewId 
//  before the old view 
//  aViewIdToBeDeactivated is to be deactivated.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPhoneAppUI::HandleViewActivation( 
    const TVwsViewId& /*aNewlyActivatedViewId*/, 
    const TVwsViewId& /*aViewIdToBeDeactivated*/ )
    {
     __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUI::HandleViewActivation" );        
    }

// ---------------------------------------------------------
// CPhoneAppUI::HelpContextL
// This routine will be called for returning help context
// when AppHelpContextL() is called in 
// CPhoneViewController::LaunchHelpApplicationL()
// ---------------------------------------------------------
//
CArrayFix<TCoeHelpContext>* CPhoneAppUI::HelpContextL() const
    {
    CArrayFix<TCoeHelpContext>* result = 
        new(ELeave) CArrayFixFlat<TCoeHelpContext>( 1 );
    
    CleanupStack::PushL( result );
    // Get the help context name from the View controller
    TCoeHelpContext helpContext( KUidPhoneApplication, 
        iPhoneViewController->HelpContext() ); 
    result->AppendL( helpContext );
    CleanupStack::Pop( result );

    return result; 
    }

// -----------------------------------------------------------------------------
// CPhoneAppUi::LayoutMainView
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::LayoutMainView()
    {
    __LOGMETHODSTARTEND( EPhonePhoneapp, "CPhoneAppUI::LayoutMainView" );
    iPhoneViewController->HandleLayoutChange( ClientRect() );
    }

// -----------------------------------------------------------------------------
// CPhoneAppUi::GetAliasKeyCode
// -----------------------------------------------------------------------------
//        
TInt CPhoneAppUI::GetAliasKeyCode( TUint& aCode, 
                                   const TKeyEvent& aKeyEvent,
                                   TEventCode /*aType*/)
    {    
    aCode = aKeyEvent.iCode;    
    return KErrNone;
    } 


// -----------------------------------------------------------------------------
// CPhoneAppUI::HandleCallBackL
// Called when the observed value changes
//
// Callback function.
// -----------------------------------------------------------------------------
//
TInt CPhoneAppUI::HandleCallBackL( TAny* aAny )
    {
    __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUI::HandleCallBackL" );
    CPhoneAppUI* appUi =
        static_cast< CPhoneAppUI* >( aAny );
    appUi->DoHandleCallBackL();
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CPhoneAppUI::DoHandleCallBackL
// Call back to a specific instance.
// -----------------------------------------------------------------------------
//
void CPhoneAppUI::DoHandleCallBackL()
    {
    __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneAppUI::DoHandleCallBackL" );
    const TInt change = iEnvChangeNotifier->Change();
    HandleEnvironmentChangeL( change );
    }
        
//  End of File  
