/*
* Copyright (c) 2005 - 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The control class of phone ui view in PhoneUI
*
*/


// INCLUDE FILES
#include "cphoneviewcontroller.h"

#include "phoneui.pan"
#include "cphonetoolbarcontroller.h"
#include "cphonebuttonscontroller.h"
#include "cphonedialercontroller.h"
#include "cphoneeasydialingcontroller.h"
#include "cphonedtmfdialercontroller.h"
#include "cphonemenucontroller.h"
#include "cphonenotecontroller.h"
#include "cphonequerycontroller.h"
#include "cphonekeycapturecontroller.h"
#include "cphoneview.h"
#include "cphonedialerview.h"
#include "tphonecommandparam.h"
#include "cphonebubblewrapper.h"
#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamKeycapture.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamappinfo.h"
#include "tphonecmdparammessageeditor.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamsingleitemfetch.h"
#include "tphonecmdparamaudiooutput.h"
#include "tphonecmdparamaudioavailability.h"
#include "tphonecmdparamquery.h"
#include "tphonecmdparamsfidata.h"
#include "tphonecmdparamtranseffect.h"
#include "phoneconstants.h"
#include "cphonestatuspane.h"
#include "cphonepubsubproxy.h"
#include "cphoneincallindicator.h"
#include "phonelogger.h"
#include "cphonetimer.h"
#include "cphonecenrepproxy.h"
#include "cphonerecoverysystem.h"
#include "cphonecontactcontroller.h"
#include "cphoneapplicationexit.h"
#include "cphoneaudiocontroller.h"
#include "cphoneringingtoneplayerao.h"
#include "cphonevmbxhandler.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "telprivatecrkeys.h"
#include "cphonesingleitemfetch.h"
#include "mnumberentry.h"
#include "cdialer.h"
#include "dialer.hrh"
#include "cphonemediatorfactory.h"
#include "cphonemediatorsender.h"
#include "mphoneviewcustomization.h"
#include "tphonecmdparamcustomdialer.h"
#include "tphonecmdparampointer.h"
#include "dialer.hrh"
#include "phoneui.hrh"
#include "musmanager.h"
#include <apgtask.h>
#include <apgwgnam.h>
#include <apacmdln.h>
#include <coedef.h>  // TKeyResponse
#include <eikenv.h>
#include <eikappui.h>
#include <eikapp.h>
#include <eikmenub.h>
#include <eikspane.h>
#include <aknkeylock.h>
#include <SpdiaControl.h>
#include <hlplch.h>
#include <aknnavide.h>
#include <aknnavilabel.h>
#include <AknNotifierController.h>
#include <CMessageData.h> // CMessageData
#include <sendui.h> // CSendUi
#include <SenduiMtmUids.h> // KSenduiMtmIrUid
#include <cphonecntsaveaddtoname.h>
#include <PbkDataSaveAppUi.hrh>
#include <PhCltUtils.h>
#include <telephonyvariant.hrh>
#include <ConeResLoader.h>
#include <bldvariant.hrh>
#include <data_caging_path_literals.hrh>
#include <AknDlgShut.h> // Avkon dialog shutter.
#include <cphcntrfshandler.h>
#include <bautils.h>
#include <CPbkContactEngine.h>
#include <CPbkContactItem.h>
#include <CPbkFieldInfo.h>
#include <PbkFields.hrh>
#include <cphcntcontactid.h>
#include <AknUtils.h>
#include <AknSgcc.h>
#include <settingsinternalcrkeys.h>
#include <UikonInternalPSKeys.h>
#include <startupdomainpskeys.h>
#include <utf.h>
#include <activeidle2domainpskeys.h>
#include <telinternalpskeys.h>
#include <featmgr.h>
#include <layoutmetadata.cdl.h>
#include <akntoolbar.h>
#include <LogsUiCmdStarter.h>
#include <AknIndicatorContainer.h>

#include <SoftReject.rsg>
#include <ConeResLoader.h>
#include <StringLoader.h>
#include <txtrich.h>

#include <vmnumber.h>
#include <coreapplicationuisdomainpskeys.h>
#include <gfxtranseffect/gfxtranseffect.h>
#include <akntranseffect.h>

#include <gslauncher.h> // Call Settings launch.

#include <dialingextensioninterface.h>
#include <easydialingcommands.hrh>
#include <AknIncallBubbleNotify.h>

// Kastor effect IDs, aknskincontent/101f84b9.sel
// These effects cannot be used for internal transitions (Call UI<->Dialer)
const TInt KTouchPhoneUiOpenEffect  = 3;
const TInt KTouchPhoneUiCloseEffect = 5;

const TInt KTouchCallUiOpenEffect  = 1000;
const TInt KTouchCallUiCloseEffect = 1001;

const TInt KDialerInputMaxChars( 100 );

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CPhoneViewController::CPhoneViewController
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CPhoneViewController::CPhoneViewController() :
    iEikEnv( *CEikonEnv::Static() ),
    iHelpCommand( KINCAL_HLP_CALL_HANDLING ),
    iBlockingDialogIsDisplayed( EFalse ),
    iIdleUid( KErrNotFound ),
    iFetchContent( KNullDesC ),
    iEffectOngoing( EFalse ),
    iSendBack( ETrue ),
    iDialerActive( EFalse ),
    iPriotityChanged( EFalse ),
    iSecurityMode( ETrue ),
    iNeedToReturnToForegroundAppAfterCall( EFalse )
    {
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CPhoneViewController::ConstructL( TRect aRect )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneViewController::ConstructL()");
    // Create the status pane singleton
    iStatusPane = CPhoneStatusPane::Instance();
    // Reserve the title pane
    iStatusPane->AddTitlePaneHandlerL( *this );
    iStatusPane->ReserveTitlePane( *this );
    // Set status pane dimming
    iStatusPane->StatusPane().SetDimmed( iSecurityMode );

    iAppui = (CAknAppUi*)iEikEnv.EikAppUi();
    iCba = iEikEnv.AppUiFactory()->Cba();

    iPhoneView = CPhoneView::NewL( aRect, *iCba, this );

    iBubbleWrapper = CPhoneBubbleWrapper::NewL( iPhoneView, aRect  );

    iPhoneView->SetControl( iBubbleWrapper->Control() );
    iPhoneView->ActivateL(); // We're ready to activate view after the bubble is initialized

    iIncallIndicator = CPhoneIncallIndicator::NewL();
    iMenuController = CPhoneMenuController::NewL( iEikEnv );
    iNoteController = CPhoneNoteController::NewL( iEikEnv );
    iQueryController = CPhoneQueryController::NewL( iEikEnv );
    iAudioController = CPhoneAudioController::NewL();

    iRingingTonePlayerAO = CPhoneRingingTonePlayerAO::NewL();
    // Set display provider for ringing tone player.
    // Display access is needed for visual ringing tones.
    iRingingTonePlayerAO->SetVideoPlayer( iBubbleWrapper );

    // Contact
    CPhoneContactController* contactManager =
        CPhoneContactController::NewL();
    iContactController = contactManager;

    iContactControllerId =
        CPhoneRecoverySystem::Instance()->AddL(
            TCallBack( DoConstructContactControllerL, this ),
            CTeleRecoverySystem::EPhonePriorityHigh,
            CTeleRecoverySystem::EPhoneStateStarting );

    iKeyCaptureController = CPhoneKeyCaptureController::NewL( iEikEnv );

    iApplicationExit = CPhoneApplicationExit::NewL(
        this,
        iEikEnv.WsSession(),
        iEikEnv.WsSession().FindWindowGroupIdentifier( 0, KPhoneEikonServer ) );

    CCoeEnv* coeEnv = CCoeEnv::Static();

    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        iButtonsController = CPhoneButtonsController::NewL( iBubbleWrapper->TouchPane() );
        iToolbarController = CPhoneToolbarController::NewL( *coeEnv );
        }

    if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) )
        {
        iDialerActive = EFalse;
        // Create dialer controller
        iDialerController = CPhoneDialerController::NewL( iBubbleWrapper,
                                                          *coeEnv );
        // Create dialer view
        iDialerView = CPhoneDialerView::NewL( aRect );
        iDialerView->DrawableWindow()->SetOrdinalPosition( -1 );
        iDialerView->MakeVisible( EFalse );
        // Create dialer
        iDialer = CDialer::NewL( *iDialerView, aRect, iDialerController );
        // Set NE for bubbleWrapper
        iBubbleWrapper->SetNumberEntry( iDialer->NumberEntry() );
        iBubbleWrapper->SetNumberEntryObserver( *this );
        // Finalise dialer view
        iDialerView->SetControl( iDialer );
        iDialerView->ActivateL();

        // Create controller for Easy Dialing
        iEasyDialingController = CPhoneEasyDialingController::NewL( *iDialer );
        iDialerController->SetEasyDialingController( iEasyDialingController );

        // Create controller for DTMF mode of the dialer
        iDtmfDialerController = CPhoneDtmfDialerController::NewL( iBubbleWrapper,
                                                                  *coeEnv );
        }

    User::LeaveIfError( iKeyLock.Connect() );

    // if you are using sgc-client's session to capserver,
    //you are not allowed to close the session.
    iAknUiServerClient = CAknSgcClient::AknSrv();

    iIncallBubble = CAknIncallBubble::NewL();

    User::LeaveIfError( iSkinServerSession.Connect( this ) );
    }

// -----------------------------------------------------------------------------
// CPhoneViewController::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPhoneViewController* CPhoneViewController::NewL( TRect aRect )
    {
    __PHONELOG( EBasic, EPhoneUIView,
            "CPhoneViewController::NewL()" );
    CPhoneViewController* self = new (ELeave) CPhoneViewController;

    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );

    return self;
    }

// Destructor
CPhoneViewController::~CPhoneViewController()
    {
    __LOGMETHODSTARTEND( EPhoneUIView,
                        "CPhoneViewController::~CPhoneViewController()" );
    iSkinServerSession.Close();

    delete iButtonsController;
    delete iDialerController;
    delete iToolbarController;
    iKeyLock.Close();
    delete iApplicationExit;
    delete iSpeedDialController;
    if ( iVmbxHandler )
        {
        delete iVmbxHandler;
        iVmbxHandler = NULL;
        }
    delete iKeyCaptureController;
    CPhoneRecoverySystem::Remove( iContactControllerId );
    delete iContactController;
    delete iRingingTonePlayerAO;
    delete iAudioController;
    delete iQueryController;
    delete iNoteController;
    delete iMenuController;
    delete iIncallIndicator;
    delete iBubbleWrapper;
    delete iPhoneView;
    delete iIndiContainer;
    if ( iDialerView )
        {
        if(iAppui) //for codescanner
            {
            iAppui->RemoveFromStack( iDialerView );
            }
        delete iDialerView;
        }

    delete iEasyDialingController;
    delete iDtmfDialerController;

    delete iDialer;
    if ( iSingleItemFetch )
        {
        delete iSingleItemFetch;
        iSingleItemFetch = NULL;
        }
    delete iRfsHandler;
    RemoveAddToContactDialog();

//    delete iStatusPane; <-- CCoeStatic are destroyed outside application

    iApaLsSession.Close();

    delete iIncallBubble;
    }

// ---------------------------------------------------------
// CPhoneViewController::PhoneView
// ---------------------------------------------------------
//
EXPORT_C CPhoneView* CPhoneViewController::PhoneView()
    {
    return iPhoneView;
    }

// ---------------------------------------------------------
// CPhoneViewController::ExecuteCommandL( aCmdId )
// ---------------------------------------------------------
//
EXPORT_C void CPhoneViewController::ExecuteCommandL(
    TPhoneViewCommandId aCmdId )
    {
    __PHONELOGVIEWMSG( aCmdId, 0 );

    switch ( aCmdId )
        {
        case EPhoneViewLaunchApplication:
            LaunchApplicationL();
            break;

        case EPhoneViewMenuBarClose:
             iMenuController->CloseMenuBar();
            break;

        case EPhoneViewRemoveNumberEntry:
            if ( iDialer && iDialerActive )
                {
                SetControltoCallHandlingL();
                iBubbleWrapper->RemoveNumberEntry();
                iPhoneView->SetActivatePreviousApp( EFalse );

                TPhoneCmdParamCallStateData callStateData;
                callStateData.SetCallState( EPEStateConnected );
                iBubbleWrapper->GetCallIdByState( &callStateData );
                TInt connected = callStateData.CallId();

                // If video call
                if( connected == KVideoTelephonyCallId1 ||
                        connected == KVideoTelephonyCallId2 )
                    {
                    TPhoneCmdParamInteger uidParam;
                    // Bring video telephone app in the foreground
                    uidParam.SetInteger( KVtUiAppUidValue.iUid );
                    BringAppToForeground( &uidParam );
                    }
                }
            else
                {
                iBubbleWrapper->RemoveNumberEntry();
                }
            break;

        case EPhoneViewRemoveNote:
            iNoteController->DestroyNote();
            break;

        case EPhoneViewRemoveDtmfNote:
            iNoteController->DestroyDTMFNote();
            break;

        case EPhoneViewRemoveGlobalNote:
            iNoteController->DestroyGlobalNoteL();
            break;

        case EPhoneViewRemoveQuery:
            iQueryController->DestroyQuery();
            iQueryController->DestroyGlobalQuery();
            break;

        case EPhoneViewRemoveGlobalWaitNote:
            iQueryController->DestroyGlobalWaitNote();
            break;
        case EPhoneViewRemoveProgressNote:
            iNoteController->DestroyProgressNote();
            break;

        case EPhoneViewSendToBackground:
            SendToBackgroundL();
            break;

        case EPhoneViewExitApplications:
            iApplicationExit->ExitApplication();
            break;

        case EPhoneViewHideNaviPaneAudioVolume:
            iAudioController->DeactivateVolumeControl();
            break;

        case EPhoneViewShowNaviPaneAudioVolume:
            iAudioController->ActivateVolumeControlL();
            break;

        case EPhoneViewRemovePhoneDialogs:
            RemovePhoneDialogsL();
            break;

        case EPhoneViewRemoveAllCallHeaders:
            iBubbleWrapper->RemoveAllCallHeadersL();
            break;

        case EPhoneViewMuteRingToneOnAnswer:
            iRingingTonePlayerAO->MuteRingingToneOnAnswer();
            break;

        case EPhoneViewStopRingTone:
            iRingingTonePlayerAO->StopPlaying();
            break;

        case EPhoneViewMuteRingTone:
            iRingingTonePlayerAO->MuteRingingTone();
            break;

        case EPhoneViewAddToConference:
            iBubbleWrapper->AddToConferenceL();
            break;

        case EPhoneViewCreateNumberEntry:
            // Keep menu controller in synch with number entry mode
            iMenuController->SetAlphaModeFlag( EFalse );
            iBubbleWrapper->CreateNumberEntry();
            if ( iDialer && !iDialerActive )
                {
                SetControltoDialerL();
                }
            break;

        case EPhoneViewUpdateMaxConfMemberFlag:
            iMenuController->SetMaxConfMembersFlag(
                iBubbleWrapper->IsMaxMembersInConf() );
            if ( iButtonsController )
                {
                TPhoneCmdParamBoolean joinParam;
                joinParam.SetBoolean( iBubbleWrapper->IsMaxMembersInConf() );
                iButtonsController->SetInCallJoinEnabled( &joinParam );
                }
            break;

        case EPhoneViewLaunchRfsDeep:
            LaunchRfsL( ETrue );
            break;

        case EPhoneViewLaunchRfsNormal:
            LaunchRfsL( EFalse );
            break;

        case EPhoneViewUpdateView:
            iPhoneView->DrawDeferred();
            break;

        case EPhoneViewUpdateContact:
            OpenAddToContactL( EPbkCmdDataSaveAddToExisting );
            break;

        case EPhoneViewAddContact:
            OpenAddToContactL( EPbkCmdDataSaveCreateNew );
            break;

        case EPhoneViewSendMessage:
            SendMessageL();
            break;

        case EPhoneViewEnableKeyLock:
            iKeyLock.EnableKeyLock();
            break;

        case EPhoneViewDisableKeyLockWithoutNote:
            iKeyLock.DisableWithoutNote();
            break;

        case EPhoneViewUpdateFSW:
            SetHiddenL( !iBubbleWrapper->IsNumberEntryUsed() );
            UpdateFSW();
            break;

        case EPhoneViewUpdatePhoneIconToFSW:
            SetHiddenL( EFalse );
            UpdateFSW();
            break;

        case EPhoneViewCloseFSW:
            iEikEnv.DismissTaskList();
            break;

        case EPhoneViewBringIdleToForeground:
            {
            // bringe idle application to fore.
            TPhoneCmdParamInteger uidParam;
            uidParam.SetInteger( IdleAppUid() );
            BringAppToForeground( &uidParam );
            if ( iSingleItemFetch )
                {
                // Cancel the Single Item Fetch
                iSingleItemFetch->Delete();
                }
            }
            break;

        case EPhoneViewSetIdleTopApplication:
            {
            TPhoneCmdParamInteger uidParam;
            uidParam.SetInteger( IdleAppUid() );
            SetTopApplicationL( &uidParam );
            }
            break;

        case EPhoneViewRemoveConferenceBubble:
            {
            iBubbleWrapper->RemoveConferenceL();
            }
            break;

        case EPhoneViewClearNumberEntryContent:
            {
            iBubbleWrapper->SetNumberEntryContent( KNullDesC );
            }
            break;

        case EPhoneViewUpdateToolbar:
            {
            if ( iDialer )
                {
                iDialer->UpdateToolbar();
                }
            }
            break;

        case EPhoneViewSetControlAndVisibility:
            if ( iDialer )
                {
                SetDialerControlAndVisibilityL();
                }
            else
                {
                // If iDialer is false then set numberentry
                // visible.
                TPhoneCmdParamBoolean booleanParam;
                booleanParam.SetBoolean( ETrue );
                iBubbleWrapper->SetNumberEntryVisible( &booleanParam );
                }
            break;

        case EPhoneViewActivatePreviousApp:
            {
            TApaTaskList appList( iEikEnv.WsSession() );
            TApaTask bring = appList.FindApp( KUidPhoneApplication );
            // If CheckSendBackRoundEventNeed returns false then the
            // active/foreground application is phone&singleitemfetch and
            // there is no need to make SendToBackRound event or update
            // window group position.
            if ( bring.Exists() )
                {
                CAknSgcClient::MoveApp( bring.WgId(),
                                        ESgcMoveAppToBackground );
                // Position needs to be updated to ensure that exit works
                // from previous view.
                SetWindowGroupPosition();
                }
            }
            break;

        case EPhoneViewKeypadAudioEnabled:
            {
            iBubbleWrapper->KeypadAudioEnabled();
            }
            break;

        case EPhoneViewKeypadAudioDisabled:
            {
            iBubbleWrapper->KeypadAudioDisabled();
            }
            break;

        case EPhoneViewCancelAllNotications:
            {
            CancelAllNotications();
            }
            break;
        case EPhoneViewOpenVirtualKeyBoard:
            {
            iBubbleWrapper->OpenVkbL();
            }
            break;
        case EPhoneViewCreatePhoneBookServices:
            {
            // Open and create phonebook services(phonebook stores etc).
            iContactController->CreatePhoneBookServiceL();
            break;
            }
        case EPhoneViewHideToolbar:
            {
            iToolbarController->HideToolbar();
            }
            break;

        case EPhoneViewLaunchMultimediaSharing:
            {
            LaunchMultimediaSharingL();
            }
            break;

        case EPhoneViewSetIncallBubbleTrue:
            {
            // Allow small call bubble, if call handling view is in background and allow for dialer also
            SetIncallBubbleVisibility( ETrue );
            }
            break;

        case EPhoneViewSetIncallBubbleFalse:
            {
            // Don't allow small call bubble, if call handling view is not in foreground
            SetIncallBubbleVisibility( EFalse );
            }
            break;

        default:
            __PHONELOG( EBasic, EPhonePhoneapp,
                "CPhoneViewController::ExecuteCommandL -> UnKnownMessage !");
            break;
        }
    CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ExecuteCommandL( aCmdId, aCallId )
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneViewController::ExecuteCommandL(
    TPhoneViewCommandId aCmdId,
    TInt aCallId )
    {
    __PHONELOGVIEWMSG( aCmdId, aCallId );
    __ASSERT_DEBUG( aCallId > KErrNotFound,
                    Panic( EPhoneViewParameterNotInitialized ) );

    switch ( aCmdId )
        {
        case EPhoneViewRemoveCallHeader:
            iBubbleWrapper->RemoveCallHeaderL( aCallId );
            break;

        case EPhoneViewRemoveFromConference:
            iBubbleWrapper->RemoveFromConferenceL( aCallId );
            break;

        case EPhoneViewPrivateFromConference:
            iBubbleWrapper->ConferenceMemberToPrivateL( aCallId );
            break;

        default:
            __PHONELOG( EBasic, EPhonePhoneapp,
             "CPhoneViewController::ExecuteCommandL -> UnKnownMessage !!! ");
            break;
        }
    CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId, aCallId );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ExecuteCommandL( aCmdId, aCommandParam )
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneViewController::ExecuteCommandL(
    TPhoneViewCommandId aCmdId,
    TPhoneCommandParam* aCommandParam )
    {
    __PHONELOGVIEWMSG( aCmdId, 0 );

    switch ( aCmdId )
        {
        case EPhoneViewSetSendKeyDialerActivationFlag:
            {
            TPhoneCmdParamBoolean* booleanParam =
                static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            iPhoneView->SetDialerActivation( booleanParam->Boolean() );
            }
            break;

        case EPhoneViewAllowWaitingCallHeader:
            AllowWaitingCallHeaderL( aCommandParam );
            break;

        case EPhoneViewCreateNumberEntry:
            iBubbleWrapper->CreateNumberEntry();
            if ( iDialer && !iDialerActive )
                {
                SetControltoDialerL();
                }
            // Pass the key event to number entry
            iBubbleWrapper->HandleKeyEventL( aCommandParam );
            break;

        case EPhoneViewUpdateContextMenu:
            {
            iMenuController->SetContextMenu( aCommandParam );
            }
            break;

        case EPhoneViewGetLocalizedNumberFromEntry:
            iBubbleWrapper->GetLocalizedNumberEntryContent( aCommandParam );
            break;

        case EPhoneViewSetNumberEntryVisible:
            if ( iDialer )
                {
                TPhoneCmdParamBoolean* booleanParam =
                    static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

                if ( booleanParam->Boolean() )
                    {
                    SetControltoDialerL();
                    }
                else
                    {
                    // Set activate previous to false since dialer was set to
                    // hidden status.
                    iPhoneView->SetActivatePreviousApp( EFalse );
                    SetControltoCallHandlingL();
                    }
                }
            else
                {
                iBubbleWrapper->SetNumberEntryVisible( aCommandParam );
                }
            break;

        case EPhoneViewGetNumberEntryCount:
            iBubbleWrapper->GetNumberEntryCount( aCommandParam );
            break;

        case EPhoneViewMenuBarOpen:
            iMenuController->SetNEVisibleFlag(
                iBubbleWrapper->IsNumberEntryVisible() );
            iMenuController->OpenMenuBarL( aCommandParam );
            break;

        case EPhoneViewUpdateCba:
            SetCbaL( aCommandParam );
            break;

        case EPhoneViewMenuPane:
            // First offer initialization to easydialing, and if no
            // initialization done, give it to the menu controller.
            if ( !iEasyDialingController->InitializeEasyDialingMenuL( aCommandParam ) )
                {
                iMenuController->SetReplaceFlag( IsReplaceAllowed() );
                iMenuController->DynInitMenuPaneL( aCommandParam );
                }
            break;

        case EPhoneViewMenuBar:
            iMenuController->DynInitMenuBar( aCommandParam );
            break;

        case EPhoneViewShowNote:
            iNoteController->CreateNoteL( aCommandParam );
            break;

        case EPhoneViewShowGlobalNote:
            iNoteController->CreateGlobalNoteL( aCommandParam );
            break;

        case EPhoneViewShowQuery:
            iQueryController->CreateQueryL( aCommandParam );
            break;

        case EPhoneViewSetListQueryString:
            iQueryController->SetListQueryStringL( aCommandParam );
            break;

        case EPhoneViewUpdateNoteSoftkeys:
            iNoteController->UpdateNoteSoftkeysL( aCommandParam );
            break;

        case EPhoneViewShowProgressNote:
            iNoteController->CreateProgressNoteL( aCommandParam );
            break;

        case EPhoneViewActivateAudioPathUIChanges:
            UpdateAudioPathOptions( aCommandParam );
            break;

        case EPhoneViewActivateAudioAvailabilityUIChanges:
            UpdateAudioAvailabilityOptions( aCommandParam );
            break;

        case EPhoneViewActivateMuteUIChanges:
            iMenuController->SetMuteFlag( aCommandParam );
            iBubbleWrapper->SetPhoneMuted( aCommandParam );
            iIncallIndicator->HandleMuteChange( aCommandParam );
            iToolbarController->SetMuteFlag( aCommandParam );
            if ( iButtonsController )
                {
                iButtonsController->SetMuteFlag( aCommandParam );
                }
            break;

        case EPhoneViewBringAppToForeground:
            {
            TPhoneCmdParamInteger* integerParam =
                   static_cast<TPhoneCmdParamInteger*>( aCommandParam );
            // If uid is phone app then call phone app specific foreground
            // method.
            if ( KUidPhoneApplication.iUid == integerParam->Integer() )
                {
                BringPhoneAppToForeground( aCommandParam );
                }
            else
                {
                BringAppToForeground( aCommandParam );
                }
            }
            break;

        case EPhoneViewSetNeedToReturnToForegroundAppStatus:
            SetNeedToReturnToForegroundAppAfterCall( aCommandParam );
            break;

       case EPhoneViewSetBlockingDialogStatus:
            {
            TPhoneCmdParamBoolean* booleanParam =
                static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            SetBlockingDialogIsDisplayed( booleanParam->Boolean() );
            }
            break;

       case EPhoneViewGetBlockingDialogStatus:
            GetBlockingDialogIsDisplayed( aCommandParam );
            break;

        case EPhoneViewGetForegroundApplication:
            GetForegroundApplication( aCommandParam );
            break;

        case EPhoneViewSetTopApplication:
            SetTopApplicationL( aCommandParam );
            break;

        case EPhoneViewActivateApp:
            ActivateAppL( aCommandParam );
            break;

        case EPhoneViewActivateAppView:
            ActivateAppViewL( aCommandParam );
            break;

        case EPhoneViewActivateAppViewWithCustomMessage:
            ActivateAppViewWithCustomMessageL( aCommandParam );
            break;

        case EPhoneViewActivateAppViewConventional:
            ActivateAppViewConventionalL( aCommandParam );
            break;

        case EPhoneViewUpdateIncallIndicator:
            iMenuController->CloseMenuBar();
            iIncallIndicator->Update( aCommandParam );
            iMenuController->SetCallInProgressFlag( aCommandParam );
            iToolbarController->SetCallInProgressFlag( aCommandParam );
            break;

        case EPhoneViewStartCapturingKey:
            iKeyCaptureController->StartCapturingKey( aCommandParam );
            break;

        case EPhoneViewStopCapturingKey:
            iKeyCaptureController->StopCapturingKey( aCommandParam );
            break;

        case EPhoneViewSetPointerCapture:
            CapturePointerEvents( aCommandParam );
            break;
        case EPhoneViewSetGlobalNotifiersDisabled:
            SetGlobalNotifiersDisabledL( aCommandParam );
            break;
        case EPhoneViewSetEikonNotifiersDisabled:
            SetEikonNotifiersDisabled( aCommandParam );
            break;

        case EPhoneViewGetHoldFlag:
            iMenuController->GetHoldFlag( aCommandParam );
            break;

        case EPhoneViewSetHoldFlag:
            iMenuController->SetHoldFlag( aCommandParam );
            if ( iButtonsController )
                {
                iButtonsController->SetHoldFlag( aCommandParam );
                }
            break;

        case EPhoneViewSetVideoCallFlag:
            if ( iButtonsController )
                {
                iButtonsController->SetVideoCallFlag( aCommandParam );
                }
            break;

        case EPhoneViewGetConferenceAndSingleFlag:
            iMenuController->GetConferenceAndSingleFlag( aCommandParam );
            break;

        case EPhoneViewSetConferenceAndSingleFlag:
            iMenuController->SetConferenceAndSingleFlag( aCommandParam );
            break;

        case EPhoneViewGetSoftRejectFlag:
            iMenuController->GetSoftRejectFlag( aCommandParam );
            break;

        case EPhoneViewSetSoftRejectFlag:
            iMenuController->SetSoftRejectFlag( aCommandParam );
            break;

        case EPhoneViewSetServiceCodeFlag:
            iMenuController->SetServiceCodeFlag( aCommandParam );

            if ( iDialer )
                {
                iDialerController->SetServiceCodeFlag( aCommandParam );
                }
            break;

        case EPhoneViewGetTextQueryContent:
            iQueryController->GetTextQueryContentL( aCommandParam );
            break;

        case EPhoneViewOpenSoftRejectEditor:
            OpenSoftRejectMessageEditorL( aCommandParam );
            break;

        case EPhoneViewOpenMessageEditor:
            OpenMessageEditorL( aCommandParam );
            break;

        case EPhoneViewOpenConferenceList:
            iBubbleWrapper->OpenConferenceList( aCommandParam );
            iMenuController->SetConferenceParticipantsListVisibleFlag(
                                                              aCommandParam );
            break;

        case EPhoneViewGetConferenceListVisibleFlag:
            iMenuController->GetConferenceParticipantsListVisibleFlag(
                                                              aCommandParam );
            break;

        case EPhoneViewMoveHighLightInList:
            iBubbleWrapper->MoveHighlightInConference( aCommandParam );
            break;

        case EPhoneViewGetCountOfActiveCalls:
            iBubbleWrapper->GetActiveCallsCount( aCommandParam );
            break;

        case EPhoneViewSetNaviPaneAudioVolume:
            iAudioController->HandleVolumeChangeL( aCommandParam );
            break;

        case EPhoneViewSetTitlePanePicture:
            iStatusPane->SetTitlePanePictureL( aCommandParam );
            break;

       case EPhoneViewSetStatusPaneVisible:
            SetStatusPaneVisible( aCommandParam );
            break;

        case EPhoneViewSetBackgroundImageBitmap:
            iPhoneView->SetBitmap( aCommandParam );
            break;

        case EPhoneViewGetBackgroundImageBitmap:
            iPhoneView->GetBitmap( aCommandParam );
            break;

        case EPhoneViewGetIsConference:
            iBubbleWrapper->GetIsConference( aCommandParam );
            break;

        case EPhoneViewPlayRingTone:
            iRingingTonePlayerAO->PlayRingToneL( aCommandParam );
            break;

        case EPhoneViewGetIsNoteVisible:
            iNoteController->IsNoteVisible( aCommandParam );
            break;

        case EPhoneViewIsNoteActive:
            iNoteController->IsNoteActive( aCommandParam );
            break;

        case EPhoneViewIsActiveNoteDissmissableByKeyEvent:
            iNoteController->IsActiveNoteDissmissableByKeyEvent( aCommandParam );
            break;

        case EPhoneViewGetIsProgressNoteVisible:
            iNoteController->IsProgressNoteVisible( aCommandParam );
            break;

        case EPhoneViewSetPhoneNumberAvailableInPhoneEngine:
            iMenuController->SetPhoneNumberAvailableInPhoneEngine(
            aCommandParam );
            break;

        case EPhoneViewSendAiwCommand:
            iMenuController->SendAiwCommandL( aCommandParam );
            break;

        case EPhoneViewIsQuery:
            iQueryController->IsQueryActive( aCommandParam );
            break;

        case EPhoneViewToggleNumberEntryAlphaNumericMode:
            {
            iBubbleWrapper->ToggleNumberEntryAlphaNumericMode( aCommandParam );
            TPhoneCmdParamBoolean*  modeAlpha =
                static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            iMenuController->SetAlphaModeFlag( modeAlpha->Boolean() );
            }
            break;

        case EPhoneViewSetConfrenceOptionsFlag:
            {
            TPhoneCmdParamBoolean* param =
                            static_cast<TPhoneCmdParamBoolean*>(aCommandParam);
            iMenuController->SetRemoveConfrenceOptionsFlag( param->Boolean() );
            }
            break;

        case EPhoneViewGetSoftRejectWindowGroupId:
            GetSoftRejectWindowGroupId( aCommandParam );
            break;

        case EPhoneViewSetTouchPaneButtons:
            iButtonsController->SetButtonSet( aCommandParam );
            break;

        case EPhoneViewSetTouchPaneVisible:
            {
            TPhoneCmdParamBoolean* booleanParam =
                static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            iBubbleWrapper->SetTouchPaneVisible( booleanParam->Boolean() );
            }
            break;

        case EPhoneViewSetDtmfDialerViewVisible:
            {
            if ( iDialer )
                {
                TPhoneCmdParamBoolean* booleanParam =
                    static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
                if ( booleanParam->Boolean() )
                    {
                    iDialer->SetControllerL( iDtmfDialerController );
                    }
                else
                    {
                    iDialer->SetControllerL( iDialerController );
                    }
                 }
            break;
            }

         case EPhoneViewSetDtmfOptionsFlag:
            {
            TPhoneCmdParamBoolean*  booleanParam =
                static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            iMenuController->SetAllowDtmfOptionsFlag(
                                                booleanParam->Boolean() );
            break;
            }
		 
		 case EPhoneViewSetVoipCallDTMFVisibilityFlag:
            {
            TPhoneCmdParamBoolean*  booleanParam =
                static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            iMenuController->SetHideVoipCallDTMFVisibilityFlag(
                                                    booleanParam->Boolean() );
            break;
            }
        case EPhoneViewSetVideoCallDTMFVisibilityFlag:
            {
            TPhoneCmdParamBoolean*  booleanParam =
                static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            iMenuController->SetHideVideoCallDTMFVisibilityFlag(
                                                    booleanParam->Boolean() );
            break;
            }

        case EPhoneViewShowVTSetupFailedCreateCallToSameContactQuery:
            iQueryController->CreateQueryL( aCommandParam );
            break;

        case EPhoneViewGetActivatePreviousApp:
            {
            TPhoneCmdParamBoolean*  booleanParam =
                static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            booleanParam->SetBoolean( iPhoneView->GetActivatePreviousApp() );
            }
            break;

        case EPhoneViewShowCustomizedDialer:
            {
            if ( iDialer )
                {
                TPhoneCmdParamCustomDialer* customDialerParam =
                    static_cast<TPhoneCmdParamCustomDialer*>( aCommandParam );
                iDialer->SetControllerL( customDialerParam->DialerController() );
                }
            }
            break;
        case EPhoneViewGetNumberFromEntry:
            {
            iBubbleWrapper->GetNumberEntryContent( aCommandParam );
            break;
            }
        case EPhoneViewUpdateContactByString:
            {
            TPhoneCmdParamString* paramString =
                    static_cast<TPhoneCmdParamString*>( aCommandParam );

            OpenAddToContactL( EPbkCmdDataSaveAddToExisting,
                               *paramString->String() );
            }
            break;

        case EPhoneViewAddContactByString:
            {
            TPhoneCmdParamString* paramString =
                    static_cast<TPhoneCmdParamString*>( aCommandParam );

            OpenAddToContactL( EPbkCmdDataSaveCreateNew,
                               *paramString->String() );
            }
            break;
        case EPhoneViewSetNoConnectedCalls:
            {
            iQueryController->SetNoConnectedCalls( aCommandParam );
            }
            break;
        case EPhoneViewSetToolbarDimming:
            {
            TPhoneCmdParamBoolean*  booleanParam =
                static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            iToolbarController->DimToolbar( booleanParam->Boolean());
            break;
            }

        case EPhoneViewSetInvalidCsPhoneNumberFlag:
            iMenuController->SetInvalidCsPhoneNumberFlag( aCommandParam );
            break;

        case EPhoneViewGetInvalidCsPhoneNumberFlag:
            iMenuController->GetInvalidCsPhoneNumberFlag( aCommandParam );
            break;

        case EPhoneViewGetEasyDialingMenuId:
        case EPhoneViewGetEasyDialingCbaId:
                {
                iEasyDialingController->ExecuteCommandL( aCmdId, aCommandParam );
                }
            break;
        case EPhoneViewSetConferenceAndWaitingVideo:
            {
            TPhoneCmdParamBoolean*  booleanParam =
                            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            iMenuController->SetConferenceAndWaitingVideoFlag( booleanParam->Boolean() );
            }
            break;
        default:
            __PHONELOG( EBasic, EPhonePhoneapp,
             "CPhoneViewController::ExecuteCommandL -> UnKnownMessage !!! " );
            break;
        }

    CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId,
                                                            *aCommandParam );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ExecuteCommandL( aCmdId, aCallId, aCommandParam )
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneViewController::ExecuteCommandL(
    TPhoneViewCommandId aCmdId,
    TInt aCallId,
    TPhoneCommandParam* aCommandParam )
    {
    __PHONELOGVIEWMSG( aCmdId, aCallId );

    switch ( aCmdId )
        {
        case EPhoneViewUpdateBubble:
            HandleUpdateBubbleL( aCallId, aCommandParam );
            break;

        case EPhoneViewCreateCallHeader:
            iBubbleWrapper->CreateCallHeaderL( aCallId, aCommandParam );
            break;

        case EPhoneViewUpdateCallHeaderRemoteInfoData:
            iBubbleWrapper->UpdateCallHeaderDisplay( aCallId, aCommandParam );
            break;

        case EPhoneViewUpdateCallHeaderRemoteInfoDataAndLabel:
            iBubbleWrapper->UpdateCallHeaderAndLabel( aCallId, aCommandParam );
            break;

        case EPhoneViewCreateEmergencyCallHeader:
            iBubbleWrapper->CreateEmergencyCallHeaderL( aCallId,
                                                        aCommandParam );
            break;

        case EPhoneViewCreateConference:
            iBubbleWrapper->CreateConferenceL( aCallId, aCommandParam );
            break;

        case EPhoneViewGetCallExistsInConference:
            iBubbleWrapper->CallExistsInConference( aCallId, aCommandParam );
            break;

        case EPhoneViewCipheringInfoChange:
            {
            iBubbleWrapper->UpdateCipheringIndicator( aCallId, aCommandParam );
            }
            break;

        default:
            __PHONELOG( EBasic, EPhonePhoneapp,
             "CPhoneViewController::ExecuteCommandL -> UnKnownMessage !!! " );
            break;
        }
    CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId, aCallId,
                                                            *aCommandParam );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ExecuteCommandL( aCmdId, aCallId, aMessage )
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneViewController::ExecuteCommandL(
    TPhoneViewCommandId aCmdId,
    TInt aCallId,
    TDesC& aMessage )
    {
    __PHONELOGVIEWMSG( aCmdId, aCallId );

    switch( aCmdId )
        {
        case EPhoneViewSetNumberEntryContent:
            iBubbleWrapper->SetNumberEntryContent( aMessage );
            break;

        case EPhoneViewSetTitlePaneContent:
            iStatusPane->SetTitlePaneContentL( aMessage );
            break;

        case EPhoneViewSetNaviPaneContent:
            {
            if ( !iDialer )
                {
                CAknNaviLabel* naviLabel = static_cast<CAknNaviLabel*>(
                    iStatusPane->NaviDecorator().DecoratedControl() );
                naviLabel->SetTextL( aMessage );
                iStatusPane->NaviDecorator().DrawDeferred();
                }

            }
            break;

        case EPhoneViewLaunchHelpApplication:
            LaunchHelpApplicationL( aMessage );
            break;

        case EPhoneViewUpdateCallHeaderCallDuration:
            iBubbleWrapper->UpdateCallDuration( aCallId, aMessage );
            break;

        default:
            __PHONELOG (EBasic, EPhonePhoneapp, " CPhoneViewController::ExecuteCommandL -> UnHandledMessage !!! ");
            break;
        }
    CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId, aCallId, &aMessage );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ExecuteCommand( aCmdId )
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneViewController::ExecuteCommand(
    TPhoneViewCommandId aCmdId )
    {
    switch( aCmdId )
        {
        case EPhoneResetTouchButtons:
            if ( iButtonsController )
                {
                iButtonsController->SetToggleButtons();
                }
            break;

        case EPhoneViewBeginUpdate:
            {
            iBubbleWrapper->StartChanges();
            }
            break;

        case EPhoneViewEndUpdate:
            {
            iBubbleWrapper->EndChanges();
            }
            break;
        case EPhoneViewCloseSingleItemFetchDialog:
            {
            if ( iSingleItemFetch )
                {
                // Cancel the Single Item Fetch
                iSingleItemFetch->Delete();
                iSingleItemFetch = NULL;
                }
            }
          break;

        case EPhoneViewPrepareIcons:
          iBubbleWrapper->BubbleManager().PrepareIcons();
          break;

        case EPhoneViewLoadPlugins:
          iBubbleWrapper->LoadPlugins();
          break;

        case EPhoneViewCipheringInfoChangePlayTone:
            {
            iRingingTonePlayerAO->PlayUnsecureVoIPTone();
            }
            break;

        default:
            __PHONELOG (EBasic, EPhonePhoneapp, " CPhoneViewController::ExecuteCommand -> UnHandledMessage !!! ");
            break;
        }
    CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ExecuteCommand( aCmdId, aCommandParam )
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneViewController::ExecuteCommand(
    TPhoneViewCommandId aCmdId,
    TPhoneCommandParam* aCommandParam )
    {
    __PHONELOGVIEWMSG( aCmdId, 0 );

    switch ( aCmdId )
        {
        case EPhoneViewGetIsConferenceInSelectionMode:
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam )->SetBoolean(
                iBubbleWrapper->BubbleManager().SelectionIdInConference() !=
                KBubbleInvalidId );
            break;

        case EPhoneViewEnableTouchButton:
            if ( iButtonsController )
                {
                iButtonsController->SetButtonEnabled( aCommandParam );
                }
            break;

        case EPhoneViewDisableTouchButton:
            if ( iButtonsController )
                {
                iButtonsController->SetButtonDisabled( aCommandParam );
                }
            break;

        case EPhoneViewGetAudioVolumeLevel:
            {
            TInt level = iAudioController->VolumeLevelFromControl();
            static_cast<TPhoneCmdParamInteger*>( aCommandParam )->SetInteger(
                level );
            }
           break;

        case EPhoneViewBeginTransEffect:
            {
            if ( iDialer )
                {
                TPhoneTransEffectType type =
                    static_cast<TPhoneCmdParamTransEffect*>( aCommandParam )->Type();
                if ( CanTransEffectBeUsed( type ) )
                    {
                    HandleTransitionEffect( type,
                        static_cast<TPhoneCmdParamTransEffect*>( aCommandParam )->AppUid());
                    }
                }
            break;
            }

        case EPhoneViewEndTransEffect:
            {
            if ( iDialer )
                {
                HandleTransitionEffect(
                    static_cast<TPhoneCmdParamTransEffect*>( aCommandParam )->Type() );
                }
            break;
            }
        case EPhoneViewGetNumberFromEntry:
            {
            iBubbleWrapper->GetNumberEntryContent( aCommandParam );
            break;
            }

        case EPhoneViewGetKeyLockStatus:
            {
            TPhoneCmdParamBoolean*  booleanParam = static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            booleanParam->SetBoolean( iKeyLock.IsKeyLockEnabled() );
            }
            break;

        case EPhoneViewSetNumberEntryObserver:
            {
            iBubbleWrapper->SetNumberEntryChangedCallBack( aCommandParam );
            break;
            }
        case EPhoneViewGetSingleItemFetchType:
            {
            static_cast<TPhoneCmdParamInteger*>( aCommandParam )->SetInteger( SingleItemFetchType() );
            break;
            }
        case EPhoneViewSetPhoneCustomization:
            {
            TPhoneCmdParamPointer* pointerParam =
                static_cast<TPhoneCmdParamPointer*>( aCommandParam );
            MPhoneCustomization* phoneCustomization =
                static_cast<MPhoneCustomization*>( pointerParam->Pointer() );
            iBubbleWrapper->SetPhoneCustomization( phoneCustomization );
            }
            break;
     case EPhoneViewSetViewCustomization:
            {
            TPhoneCmdParamPointer* pointerParam =
                static_cast<TPhoneCmdParamPointer*>( aCommandParam );
            iCustomization =
                static_cast<MPhoneViewCustomization*>( pointerParam->Pointer() );
            }
            break;
        case EPhoneViewSetButtonCustomization:
            {
            if ( iButtonsController )
                {
                TPhoneCmdParamPointer* pointerParam =
                    static_cast<TPhoneCmdParamPointer*>( aCommandParam );
                MPhoneButtonCustomization* customization =
                    static_cast<MPhoneButtonCustomization*>( pointerParam->Pointer() );
                iButtonsController->SetCustomization( customization );
                }
            }
            break;

        case EPhoneViewEnableToolbarButton:
            {
            TPhoneCmdParamInteger*  integerParam =
                static_cast<TPhoneCmdParamInteger*>( aCommandParam );
            iToolbarController->SetToolbarButtonDimmed( integerParam->Integer(), EFalse );
            }
            break;

        case EPhoneViewGetQwertyModeObserver:
            {
            TPhoneCmdParamPointer* ptrParam = static_cast<TPhoneCmdParamPointer*>( aCommandParam );
            ptrParam->SetPointer( static_cast<TAny*>( iDialer ) );
            }
            break;

        default:
            __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneViewController::ExecuteCommand -> UnHandledMessage !!! " );
            break;
        }
    CPhoneMediatorFactory::Instance()->Sender()->SendEvent( aCmdId, *aCommandParam );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::HandleCommandL( aCmdId )
// ---------------------------------------------------------------------------
//
EXPORT_C TPhoneViewResponseId CPhoneViewController::HandleCommandL(
    TPhoneViewCommandId aCmdId )
    {
    __PHONELOGVIEWMSG( aCmdId, 0 );

    TPhoneViewResponseId viewResponse = EPhoneViewResponseSuccess;

    switch ( aCmdId )
        {
        case EPhoneViewGetNumberEntryIsUsedStatus:
            viewResponse = iBubbleWrapper->IsNumberEntryUsed() ?
                EPhoneViewResponseSuccess :
                EPhoneViewResponseFailed;
            break;

        case EPhoneViewGetNumberEntryIsVisibleStatus:
            if ( iDialer )
                {
                viewResponse = iDialerActive ?
                    EPhoneViewResponseSuccess :
                    EPhoneViewResponseFailed;
                }
            else
                {
                viewResponse = iBubbleWrapper->IsNumberEntryVisible() ?
                EPhoneViewResponseSuccess :
                EPhoneViewResponseFailed;
                }
             break;

         case EPhoneViewGetEasyDialingInFocusStatus:
         case EEasyDialingVoiceCall:
         case EEasyDialingVideoCall:
         case EEasyDialingSendMessage:
         case EEasyDialingOpenContact:
         case EEasyDialingCallHandlingActivated:
         case EEasyDialingEnterKeyAction:
         case EEasyDialingOn:
         case EEasyDialingOff:
             if ( iDialer )
                 {
                 viewResponse = iEasyDialingController->HandleCommandL( aCmdId );
                 }
             else
                 {
                 viewResponse = EPhoneViewResponseFailed;
                 }
             break;

         case EPhoneViewIsNumberEntryNumericMode:
            viewResponse = iBubbleWrapper->IsNENumericMode() ?
                EPhoneViewResponseSuccess :
                EPhoneViewResponseFailed;
            break;

        case EPhoneViewGetNeedToReturnToForegroundAppStatus:
            viewResponse = GetNeedToReturnToForegroundAppAfterCallL() ?
                EPhoneViewResponseSuccess :
                EPhoneViewResponseFailed;
            break;

        case EPhoneViewGetTopApplicationIsDisplayedStatus:
            {
            const TInt foregroundWg = ForegroundApplicationWindowGroupId();
            const TBool topAppDisplayed = ( IdleWindowGroupId() == foregroundWg ) ||
                ( ApplicationWindowGroupId() == foregroundWg );
            viewResponse = topAppDisplayed ?
                EPhoneViewResponseSuccess :
                EPhoneViewResponseFailed;
            }
            break;

        case EPhoneViewGetTitlePaneIsVisibleStatus:
            viewResponse = iStatusPane->IsTitlePaneVisible( *this ) ?
                EPhoneViewResponseSuccess :
                EPhoneViewResponseFailed;
            break;

        case EPhoneViewIsMenuBarVisible:
            viewResponse = iMenuController->IsMenuBarVisible() ?
                EPhoneViewResponseSuccess :
                EPhoneViewResponseFailed;
            break;

        case EPhoneViewIsStraightCallToVideoMailBox:
          {
          if( !iVmbxHandler )
              {
              iVmbxHandler = CPhoneVmbxHandler::NewL();
              }

          viewResponse = iVmbxHandler->IsStraightCallToVideoMailBox() ?
              EPhoneViewResponseSuccess :
              EPhoneViewResponseFailed;
          }
          break;

    // Dialer specific commands start

    // Dialer handles these commands.
    // ** TODO Here we need just to flag if functionality creates blocking dialog.
        case EPhoneDialerCmdLog:
            {
            BeginTransEffectForAppStartFromDialerLC( LogsUiCmdStarterConsts::KLogsUID3 );
            LogsUiCmdStarter::CmdStartL( LogsUiCmdStarterConsts::KDialledView());
            EndTransEffect();
            }
            break;

        case EPhoneDialerCmdContacts:
            {
            // Launch Phonebook application
            TPhoneCmdParamAppInfo appInfoParam;
            appInfoParam.SetAppUid( KPhoneUidAppPhonebook );
            BeginTransEffectForAppStartFromDialerLC( appInfoParam.AppUid() );
            ExecuteCommandL( EPhoneViewActivateApp, &appInfoParam );
            EndTransEffect();
            }
            break;

        case EPhoneDialerCmdSpeedDial:
            {
            // Launch speeddial application.
            TPhoneCmdParamAppInfo appInfoParam;
            appInfoParam.SetAppUid( KUidSpdia );
            ExecuteCommandL( EPhoneViewActivateApp, &appInfoParam );
            break;
            }

        case EPhoneDialerCallSettings:
            {
            CGSLauncher* launcher = CGSLauncher::NewLC();
            launcher->LaunchGSViewL( TUid::Uid( 0x102824A2),
                                    TUid::Uid( 0x100058B3),
                                    KNullDesC8 );
            CleanupStack::PopAndDestroy( launcher );
            break;
            }

        case EPhoneViewIsDTMFEditorVisible:
            if ( iDialer )
                {
                viewResponse = ( IsDtmfDialerActive() ?
                    EPhoneViewResponseSuccess :
                    EPhoneViewResponseFailed );
                }
            else // non-touch.
                {
                viewResponse = iQueryController->IsDTMFQueryVisible() ?
                    EPhoneViewResponseSuccess :
                    EPhoneViewResponseFailed;
                }
            break;

        case EPhoneViewIsConferenceInExpandedMode:
            viewResponse = iBubbleWrapper->IsConferenceInExpandedMode() ?
                EPhoneViewResponseSuccess :
                EPhoneViewResponseFailed;
            break;

        case EPhoneViewOpenNumberEntry:
            if ( iDialer )
                {
                TPhoneCmdParamBoolean booleanParam;
                booleanParam.SetBoolean( ETrue );

                if ( iBubbleWrapper->IsNumberEntryUsed() )
                    {
                    // Launch dialer.
                    ExecuteCommandL( EPhoneViewSetNumberEntryVisible, &booleanParam );
                    }
                else // dialer doesnt exist.
                    {
                     // Create and launch dialer.
                    ExecuteCommandL( EPhoneViewCreateNumberEntry, &booleanParam );
                    }
                // Set Number Entry CBA
                TPhoneCmdParamInteger integerParam;
                if ( iSecurityMode )
                    {
                    iNoteController->DestroyNote();
                    integerParam.SetInteger(
                            CPhoneMainResourceResolver::Instance()->
                                ResolveResourceID( EPhoneEmergencyModeNoteCBA ) );
                    }
                else
                    {
                    integerParam.SetInteger(
                            CPhoneMainResourceResolver::Instance()->
                                ResolveResourceID( EPhoneNumberAcqCBA ) );
                    }
                ExecuteCommandL( EPhoneViewUpdateCba, &integerParam );
                // Set flag to false because dialler is set to open status.
                iPhoneView->SetPhoneAppViewToDialer( EFalse );
              }
          break;

        case EPhoneViewOpenCallHandling:
            if ( iDialer )
                {
                if ( iBlockingDialogIsDisplayed )
                    {
                    RemovePhoneDialogsL();
                    }
                // Remove DTMF dialer when exist
                if ( IsDtmfDialerActive() )
                    {
                    static_cast<MEikCommandObserver*>( CEikonEnv::Static()->EikAppUi() )
                    ->ProcessCommandL( EPhoneDtmfDialerExit );
                    }
                else if ( iCustomization &&
                          IsCustomDialerActive() )
                    {
                    iCustomization->HandleCommandL(
                       *this, EPhoneViewOpenCallHandling, NULL );
                    }
                else
                    {
                    SetControltoCallHandlingL();
                    }
                }
          break;

        case EPhoneViewIsIdleTopApp:
            {
            viewResponse = CPhonePubSubProxy::Instance()->Value(
                    KPSUidUikon, KUikVideoCallTopApp ) == IdleAppUid() ?
                EPhoneViewResponseSuccess :
                EPhoneViewResponseFailed;
            }
            break;

        case EPhoneViewSetDialerControlVisible:
            {
            // If dialer was not active and needtosendback is false
            // then check if dialer view is to be opened.
            if ( !iDialerActive &&
                 !iNeedToReturnToForegroundAppAfterCall &&
                 iPhoneView->PhoneAppViewToDialer() )
                {
                SetControltoDialerL();
                // Reset flag.
                iPhoneView->SetPhoneAppViewToDialer( EFalse );
                viewResponse = EPhoneViewResponseSuccess;
                }
            else
                {
                // Reset flag.
                iPhoneView->SetPhoneAppViewToDialer( EFalse );
                viewResponse = EPhoneViewResponseFailed;
                }
            }
            break;
// Dialer specific commands end.

        case EPhoneIsCustomizedDialerVisible:
            {
            viewResponse = IsCustomDialerActive() ?
                EPhoneViewResponseSuccess :
                EPhoneViewResponseFailed;
            }
            break;

        case EPhoneViewHideCustomizedDialer:
            {
            if ( iDialer )
                {
                iDialer->SetControllerL( iDialerController );
                viewResponse = EPhoneViewResponseSuccess;
                }
            }
            break;
        case EPhoneViewShowToolbar:
            {
            iToolbarController->ShowToolbar();
            }
            break;
            
        case EPhoneViewIsDisplayingMenuOrDialog:
            {
            viewResponse = iEikEnv.EikAppUi()->IsDisplayingMenuOrDialog() ?
                EPhoneViewResponseSuccess :
                EPhoneViewResponseFailed;
            }
            break;
        default:
            viewResponse = EPhoneViewResponseFailed;
            __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneViewController::HandleCommandL -> UnKnownMessage !!! ");
            break;
        }

    return viewResponse;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::HandleCommand( aCmdId, aCommandParam )
// ---------------------------------------------------------------------------
//
EXPORT_C TPhoneViewResponseId CPhoneViewController::HandleCommandL(
    TPhoneViewCommandId aCmdId,
    TPhoneCommandParam* aCommandParam )
    {
    __PHONELOGVIEWMSG( aCmdId, 0 );
    TPhoneViewResponseId viewResponse = EPhoneViewResponseSuccess;
    TInt err;

    switch ( aCmdId )
        {
        case EPhoneViewSendKeyEventToNumberEntry:
            {
            // Pass the key event to the Bubble Manager (ignore key
            // reponse)
            iBubbleWrapper->HandleKeyEventL( aCommandParam );

            // Indicate whether the number entry has been cleared
            if ( iBubbleWrapper->CountNumberEntryCharacters() == 0 && !iDialer )
                {
                viewResponse = EPhoneViewResponseNumberEntryCleared;
                }
            else
                {
                viewResponse = EPhoneViewResponseSuccess;
                }
            break;
            }

        case EPhoneViewAssignSpeedDialLocation:
            {
            if ( AssignSpeedDialLocation( aCommandParam ) )
                {
                viewResponse = EPhoneViewResponseSuccess;
                }
            else
                {
                viewResponse = EPhoneViewResponseFailed;
                }
            break;
            }

        case EPhoneViewSelectedConfMember:
            {
            if ( iBubbleWrapper->SelectedConfMemberCallIdL( aCommandParam ) )
                {
                viewResponse = EPhoneViewResponseSuccess;
                }
            else
                {
                viewResponse = EPhoneViewResponseFailed;
                }
            break;
            }

        case EPhoneViewGetCallState:
            {
            if ( iBubbleWrapper->GetCallState( aCommandParam ) )
                {
                viewResponse = EPhoneViewResponseSuccess;
                }
            else
                {
                viewResponse = EPhoneViewResponseFailed;
                }
            break;
            }

        case EPhoneViewGetCallIdByState:
            {
            if ( iBubbleWrapper->GetCallIdByState( aCommandParam ) )
                {
                viewResponse = EPhoneViewResponseFailed;
                }
            else
                {
                viewResponse = EPhoneViewResponseSuccess;
                }
            break;
            }

        case EPhoneViewOpenSingleItemFetchDialog:
            {
            // Create CPhoneSingleItemFetch, if not exist
            if ( !iSingleItemFetch )
                {
                iSingleItemFetch = CPhoneSingleItemFetch::NewL(
                    iEikEnv, *this, *iContactController, *iStatusPane );
                }

            iBlockingDialogIsDisplayed = ETrue;

            TRAP( err, iSingleItemFetch->OpenSingleItemFetchDialogL( aCommandParam ))
            if ( err )
                {
                iBlockingDialogIsDisplayed = EFalse;
                User::Leave( err );
                }
            break;
            }

        case EPhoneViewGetNumberFromSpeedDialLocation:
            if ( iBlockingDialogIsDisplayed )
                {
                viewResponse = EPhoneViewResponseFailed;
                }
            else
                {
                TBool status( EFalse );
                TRAP( err, status = GetNumberFromSpeedDialLocationL( aCommandParam ))
                if ( err )
                    {
                    viewResponse = EPhoneViewResponseFailed;
                    // Set and reset in GetNumberFromSpeedDialLocationL when not leave
                    iBlockingDialogIsDisplayed = EFalse;
                    }
                else if ( status )
                    {
                    viewResponse = EPhoneViewResponseSuccess;
                    }
                else
                    {
                    viewResponse = EPhoneViewResponseFailed;
                    }
                }
            break;

        case EPhoneViewGetCustomizedDialerMenuResourceId:
            {
            TInt resId = iDialer->Controller()->MenuResourceId(); // easydialing change
            static_cast<TPhoneCmdParamInteger*>( aCommandParam )->SetInteger( resId );
            viewResponse = EPhoneViewResponseSuccess;
            }
            break;
        case EPhoneViewGetCustomizedDialerCbaResourceId:
            {
            TInt resId = iDialer->Controller()->CbaResourceId(); // easydialing change
            static_cast<TPhoneCmdParamInteger*>( aCommandParam )->SetInteger( resId );
            viewResponse = EPhoneViewResponseSuccess;
            }
            break;

        default:
            if ( iCustomization )
                {
                // Let customization handle view command
                if ( iCustomization->HandleCommandL(
                        *this, aCmdId, aCommandParam ) )
                    {
                    viewResponse = EPhoneViewResponseSuccess;
                    }
                else
                    {
                    viewResponse = EPhoneViewResponseFailed;
                    }
                }
            else
                {
                __PHONELOG( EBasic, EPhonePhoneapp, "CPhoneViewController::HandleCommandL -> UnKnownMessage !!! ");
                }
            break;
        }

    return viewResponse;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::HandleTitlePaneActiveL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::HandleTitlePaneActiveL( TBool aActive )
    {
    if ( aActive )
        {
        iStatusPane->UpdateTitlePane();
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ApplicationWindowGroupId
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPhoneViewController::ApplicationWindowGroupId()
    {
    TInt phoneAppWg = iEikEnv.RootWin().Identifier();
    __PHONELOG1( EBasic, EPhoneUIView,
      "CPhoneViewController::ApplicationWindowGroupId - PhoneApplicationWindowGroupId = (%d)",
      phoneAppWg );
    return phoneAppWg;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::IdleWindowGroupId
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPhoneViewController::IdleWindowGroupId()
    {
    if ( !iIdleWg )
        {
        // Save Idle window group.
        TApaTaskList appList( iEikEnv.WsSession() );
        TApaTask bring = appList.FindApp( TUid::Uid( IdleAppUid() ) );
        if ( bring.Exists() )
            {
            iIdleWg = bring.WgId();
            __PHONELOG1( EBasic, EPhoneUIView,
              "CPhoneViewController::ConstructL -> iIdleWindowGroupId = (%d)",
              iIdleWg );
            }
        }

    return iIdleWg;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::GetSoftRejectWindowGroupId
// ---------------------------------------------------------------------------
//
void CPhoneViewController::GetSoftRejectWindowGroupId(
                                    TPhoneCommandParam* aCommandParam )
    {
    TInt softRejectWg( KErrNotFound );
    TPhoneCmdParamInteger* integerParam =
        static_cast<TPhoneCmdParamInteger*>( aCommandParam );

    TApaTaskList appList( iEikEnv.WsSession() );
    TApaTask bring = appList.FindApp( TUid::Uid( KSenduiUniEditorUidValue ) );
    if ( bring.Exists() )
        {
        softRejectWg = bring.WgId();
        __PHONELOG1( EBasic, EPhoneUIView,
            "CPhoneViewController::SoftRejectWindowGroupId = (%d)",
            softRejectWg );
        }
    integerParam->SetInteger( softRejectWg );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ForegroundApplicationWindowGroupId
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPhoneViewController::ForegroundApplicationWindowGroupId()
    {
    TInt result(0);
    TRAP_IGNORE( result = DoFetchForegroundApplicationWindowGroupIdL(
                          iEikEnv ) );
    __PHONELOG1( EBasic,
            EPhoneUIView,
            "CPhoneViewController::ForegroundApplicationWindowGroupId() GroupId %d",
            result );
    return result;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::DoFetchForegroundApplicationWindowGroupIdL
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPhoneViewController::DoFetchForegroundApplicationWindowGroupIdL(
    CEikonEnv& aEnv )
    {
    TInt result = 0;
    RWsSession& wsSession = aEnv.WsSession();
    const TInt numWg = wsSession.NumWindowGroups(
            KPhoneApplicationWindowGroupPriority );

    CArrayFixFlat<TInt>* wgList =
        new (ELeave) CArrayFixFlat<TInt>( numWg );
    CleanupStack::PushL( wgList );
    wgList->SetReserveL( numWg );

    // Get list of window groups; all applications should be at priority 0.
    //
    if ( wsSession.WindowGroupList(
        KPhoneApplicationWindowGroupPriority,
        wgList ) == KErrNone )
        {
        if ( wgList->Count() )
            {
            result = wgList->At( 0 ); // The very first window group id
            }
        }

    CleanupStack::PopAndDestroy( wgList );
    return result;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetHiddenL
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneViewController::SetHiddenL( const TBool aHidden )
    {
    __PHONELOG( EBasic, EPhoneUIView,"CPhoneViewController::SetHiddenL()" )
    CApaWindowGroupName* windowGroupName =
        CApaWindowGroupName::NewLC(
            iEikEnv.WsSession(),
            iEikEnv.RootWin().Identifier() );
    windowGroupName->SetHidden( aHidden );
    User::LeaveIfError(
        windowGroupName->SetWindowGroupName(
            iEikEnv.RootWin() ) );
    CleanupStack::PopAndDestroy( windowGroupName );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::HandleResourceChangeL
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneViewController::HandleResourceChangeL( TInt aType )
    {
    __PHONELOG( EBasic, EPhoneUIView,
                        "CPhoneViewController::HandleResourceChangeL()" )
    // Notify the bubble that there has been a resource change
    iBubbleWrapper->Control()->HandleResourceChange( aType );

    // Notify dialer and switch statuspane layout.
    if ( iDialer )
        {
        if ( aType == KEikDynamicLayoutVariantSwitch )
            {
            SwitchLayoutToFlatStatusPaneL( iDialerActive );
            }
        static_cast<CCoeControl*>(iDialer)->HandleResourceChange( aType );
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::BlockingDialogIsDisplayed
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneViewController::BlockingDialogIsDisplayed() const
    {
    return iBlockingDialogIsDisplayed;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::HandleLayoutChange
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneViewController::HandleLayoutChange( TRect aRect )
    {
    __PHONELOG( EBasic, EPhoneUIView,"CPhoneViewController::HandleLayoutChange()" )
    iPhoneView->SetRect( aRect );
    if ( iDialerView )
        {
        iDialerView->SetRect( aRect );
        iStatusPane->UpdateTitlePane();
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::StatusPaneDisplayed
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneViewController::StatusPaneDisplayed()
    {
    return  CPhoneStatusPane::Instance()->IsVisible();
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::IsEmergencyCallOngoing()
// ---------------------------------------------------------------------------
//
TBool CPhoneViewController::IsEmergencyCallOngoing()
    {
    __LOGMETHODSTARTEND( EPhoneUIView,
                         "CPhoneViewController::IsEmergencyCallOngoing()");

    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnected );
    iBubbleWrapper->GetCallIdByState( &callStateData );
    TInt connected = callStateData.CallId();

    callStateData.SetCallState( EPEStateConnecting );
    iBubbleWrapper->GetCallIdByState( &callStateData );
    TInt connecting = callStateData.CallId();

    return ( ( connected == KEmergencyCallId ) ||
             ( connecting == KEmergencyCallId ) );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::HandleSecurityModeChanged
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneViewController::HandleSecurityModeChanged( TBool aIsEnabled )
    {
    __PHONELOG1( EBasic, EPhoneUIView,
            "CPhoneViewController::HandleSecurityModeChanged Mode = (%d)", aIsEnabled );

    TBool emergencyCallOngoing = IsEmergencyCallOngoing();

    // Don't undim toolbar if emergency call is ongoing 
    if ( !emergencyCallOngoing )
        {
        iToolbarController->DimToolbar( aIsEnabled );
        }
    iPhoneView->SetSecurityMode( aIsEnabled );
    iMenuController->SetSecurityMode( aIsEnabled );

    TBool securityModeChanged = ( iSecurityMode != aIsEnabled );

    iSecurityMode = aIsEnabled;

    if ( iDialer )
        {
        iDialerController->SetRestrictedDialer( aIsEnabled );

        if ( iDialerActive && securityModeChanged )
            {
            CDialingExtensionInterface* easyDialing = iDialer->GetEasyDialingInterface();
            if ( easyDialing )
                {
                if ( aIsEnabled )
                    {
                    // Reset the Easy Dialing just in case, this clears the existing matches
                    // when restricted mode is activated
                    easyDialing->Reset();
                    }
                else
                    {
                    if ( easyDialing->IsEnabled() )
                        {
                        // If Easy Dialing is enabled, set the input from the numeric entry
                        // field for updating the search result when restricted mode is
                        // deactivated.
                        TBuf<KDialerInputMaxChars> buf;
                        iDialer->NumberEntry()->GetTextFromNumberEntry( buf );
                        TRAP_IGNORE( easyDialing->SetInputL( buf ) );
                        }
                    }
                }
            iDialer->RelayoutAndDraw();

            // Don't update cba here if emergency call is ongoing 
            if ( !emergencyCallOngoing )
                {
                // also update cba if security mode changes while dialer is open
                TPhoneCmdParamInteger integerParam;
                if ( aIsEnabled )
                    {
                    iNoteController->DestroyNote();
                    integerParam.SetInteger( CPhoneMainResourceResolver::Instance()->
                            ResolveResourceID( EPhoneEmergencyModeNoteCBA ) );
                    }
                else
                    {
                    integerParam.SetInteger( CPhoneMainResourceResolver::Instance()->
                            ResolveResourceID( EPhoneNumberAcqCBA ) );
                    }
                TRAP_IGNORE( ExecuteCommandL( EPhoneViewUpdateCba, &integerParam ) );
                }
            if ( !aIsEnabled )
                {
                TRAP_IGNORE( SwitchLayoutToFlatStatusPaneL( ETrue ) );
                }
            }
        }
    if ( securityModeChanged )
        {
        // Update cba if security mode changes while emergency call is ongoing
        if ( emergencyCallOngoing )
            {
            TRAP_IGNORE( static_cast<MEikCommandObserver*>( CEikonEnv::Static()->EikAppUi() )
                                ->ProcessCommandL( EPhoneCmdUpdateEmergencyCba ) );
            }

        // Update status pane dimming
        iStatusPane->StatusPane().SetDimmed( aIsEnabled );

        // Uncapture the App key for sure
        TPhoneCmdParamKeyCapture appKeyCaptureParam;
        appKeyCaptureParam.SetKey( EStdKeyApplication0 );
        appKeyCaptureParam.SetKeyCode( EKeyApplication0 );
        appKeyCaptureParam.SetCaptureType( EPhoneKeyAllEvents );

        iKeyCaptureController->StopCapturingKey( &appKeyCaptureParam );
        
        // Uncapture the Camera key for sure, if it exists
        if ( !iSecurityMode && FeatureManager::FeatureSupported( KFeatureIdCamera ) )
            {
            TPhoneCmdParamKeyCapture cameraKeyCaptureParam;
            cameraKeyCaptureParam.SetKey( EStdKeyDevice7 );
            cameraKeyCaptureParam.SetKeyCode( EKeyDevice7 );
            cameraKeyCaptureParam.SetCaptureType( EPhoneKeyAllEvents );
            iKeyCaptureController->StopCapturingKey( &cameraKeyCaptureParam );
            }
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::IdleAppUid
// ---------------------------------------------------------------------------
//
TInt CPhoneViewController::IdleAppUid()
    {
    if ( iIdleUid == KErrNotFound )
        {
        // Get Idle's UID from PubSub.
        iIdleUid = CPhonePubSubProxy::Instance()->Value(
            KPSUidAiInformation,
            KActiveIdleUid );

        __PHONELOG1( EBasic, EPhoneUIView,
            "CPhoneViewController::ConstructL -> iIdleUid = (%d)", iIdleUid );
        }
    return iIdleUid;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetBlockingDialogIsDisplayed
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneViewController::SetBlockingDialogIsDisplayed(
        TBool aBlockingDialog )
    {
    iBlockingDialogIsDisplayed = aBlockingDialog;
    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::SetBlockingDialogIsDisplayed(%d)",
        iBlockingDialogIsDisplayed );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::NumberEntryStateChanged
// ---------------------------------------------------------------------------
//
void CPhoneViewController::NumberEntryStateChanged( TBool aEntryHasText )
    {
    iMenuController->SetNumberEntryEmptyFlag( !aEntryHasText );
    iBubbleWrapper->HandleNumberEntryChanged();
    TRAP_IGNORE( ExecuteCommandL( EPhoneViewUpdateToolbar ) );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::GetBlockingDialogIsDisplayed
// ---------------------------------------------------------------------------
//
void CPhoneViewController::GetBlockingDialogIsDisplayed(
    TPhoneCommandParam* aCommandParam )
    {
    if( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanValue =
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
        booleanValue->SetBoolean( iBlockingDialogIsDisplayed );
        }
    }



// ---------------------------------------------------------------------------
// CPhoneViewController::SendToBackgroundL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SendToBackgroundL()
    {
    __PHONELOG( EBasic, EPhoneUIView,
        "CPhoneViewController::SendToBackgroundL" );

    TApaTaskList appList( iEikEnv.WsSession() );
    TApaTask phoneApp = appList.FindApp( KUidPhoneApplication );
    iSendBack = EFalse;

    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::SendToBackgroundL - phoneApp.WgId() (%d)",  phoneApp.WgId() );

    // no need for sending phone background if it is not front
    if ( ( phoneApp.Exists() ) && phoneApp.WgId() ==
            ForegroundApplicationWindowGroupId() )
        {
        // make sure that previous application window group id is not
        //telephony's
        if( iPrevForegroundAppWg > 0 &&
            iPrevForegroundAppWg != phoneApp.WgId() )
            {
            // check that previous app still exists. It might be for example
            // add to contact - dialog that is allready destroyed.
            TInt previousApp = FindAppByWgIDL( iPrevForegroundAppWg );

            if( previousApp != KErrNotFound )
                {
                __PHONELOG1( EBasic, EPhoneUIView,
                 "CPhoneViewController::SendToBackgroundL - Bring previous app to foreground (%d)"
                        ,  iPrevForegroundAppWg );

                CAknSgcClient::MoveApp( iPrevForegroundAppWg, ESgcMoveAppToForeground );
                iSendBack = ETrue;
                SetWindowGroupPosition();
                }
            }
        else
            {
            __PHONELOG( EBasic, EPhoneUIView,
                "CPhoneViewController::SendToBackgroundL - send telephony to background" );

            iSendBack = ETrue;
            CAknSgcClient::MoveApp( phoneApp.WgId(),
                                    ESgcMoveAppToBackground );
            }
        }

    // Clear the send to background flag
    iNeedToReturnToForegroundAppAfterCall = EFalse;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::FindAppByWgIDL
// ---------------------------------------------------------------------------
//
TInt CPhoneViewController::FindAppByWgIDL( TInt aAppWgID ) const
    {
    __LOGMETHODSTARTEND( EPhoneUIView,
        "CPhoneViewController::FindAppByWgIDL() ")
    TInt appExists( KErrNotFound );
    RWsSession& wsSession = iEikEnv.WsSession();
    const TInt numWg = wsSession.NumWindowGroups(
            KPhoneApplicationWindowGroupPriority );

    CArrayFixFlat<TInt>* wgList =
        new (ELeave) CArrayFixFlat<TInt>( numWg );
    CleanupStack::PushL( wgList );
    wgList->SetReserveL( numWg );

    // Get list of window groups; all applications should be at priority 0.
    if ( wsSession.WindowGroupList(
        KPhoneApplicationWindowGroupPriority,
        wgList ) == KErrNone )
        {
        for( TInt i = 0; i < wgList->Count(); i++ )
            {
            if( aAppWgID == wgList->At( i ) )
                {
                appExists = i;
                break;
                }
            }
        }
    CleanupStack::PopAndDestroy( wgList );
    return appExists;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetWindowGroupPosition
// This is used when dialer has opened some other
// app(Logs) and MT call arrives, when MT call is
// answered phone is put to backround and it isnt anymore next
// active app which means that when previous app is opened
// after answer and user presses exit we need to update
// ordinal position so that dialer is activated.
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SetWindowGroupPosition()
    {
    iEikEnv.WsSession().SetWindowGroupOrdinalPosition(
        ApplicationWindowGroupId(),
        KPhoneWindowGroupPositionBackground );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::BringAppToForeground
// ---------------------------------------------------------------------------
//
void CPhoneViewController::BringAppToForeground(
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamInteger* integerParam =
            static_cast<TPhoneCmdParamInteger*>( aCommandParam );

   TApaTaskList appList( iEikEnv.WsSession() );
   TApaTask application = appList.FindApp(
                                   TUid::Uid( integerParam->Integer() ) );

   __PHONELOG1( EBasic, EPhoneUIView,
                 "CPhoneViewController::BringAppToForeground - App WG ID: %d",
                 application.WgId() );
   if( application.Exists() )
       {
       iPrevForegroundAppWg = ForegroundApplicationWindowGroupId();
       CAknSgcClient::MoveApp( application.WgId(), ESgcMoveAppToForeground );
       __PHONELOG1( EBasic, EPhoneUIView,
                     "CPhoneViewController::BringAppToForeground - Previous App WG ID: %d",
                     iPrevForegroundAppWg );
       }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::BringPhoneAppToForeground
// ---------------------------------------------------------------------------
//
void CPhoneViewController::BringPhoneAppToForeground(
    TPhoneCommandParam* aCommandParam )
    {
    __PHONELOG( EBasic, EPhoneUIView,
        "CPhoneViewController::BringPhoneAppToForeground()" );

    TPhoneCmdParamInteger* integerParam =
        static_cast<TPhoneCmdParamInteger*>( aCommandParam );

    TApaTaskList appList( iEikEnv.WsSession() );
    TApaTask phoneApp = appList.FindApp(
                                    TUid::Uid( integerParam->Integer() ) );

    // Saves previous window group id. Test if app is allready in front.
    if( phoneApp.Exists() &&
        ForegroundApplicationWindowGroupId() !=  phoneApp.WgId() )
        {
        iPrevForegroundAppWg = ForegroundApplicationWindowGroupId();
        CAknSgcClient::MoveApp( phoneApp.WgId(), ESgcMoveAppToForeground );
        __PHONELOG1( EBasic, EPhoneUIView,
                      "CPhoneViewController::BringPhoneAppToForeground - Previous App WG ID: %d",
                      iPrevForegroundAppWg );
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetTopApplicationL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SetTopApplicationL(
    TPhoneCommandParam* aCommandParam )
    {
    // Set the top application
    TPhoneCmdParamInteger* integerParam =
        static_cast<TPhoneCmdParamInteger*>( aCommandParam );
    CPhonePubSubProxy::Instance()->ChangePropertyValue(
        KPSUidUikon,
        KUikVideoCallTopApp,
        integerParam->Integer() );

    // Hide the Phone icon if it is not the top application
    SetHiddenL( integerParam->Integer() != KUidPhoneApplication.iUid );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::GetForegroundApplication
// ---------------------------------------------------------------------------
//
void CPhoneViewController::GetForegroundApplication(
    TPhoneCommandParam* aCommandParam )
    {
    // Get the foreground application window group id
    TPhoneCmdParamInteger* integerParam =
        static_cast<TPhoneCmdParamInteger*>( aCommandParam );
    integerParam->SetInteger( ForegroundApplicationWindowGroupId() );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ActivateAppViewConventionalL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::ActivateAppViewConventionalL(
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamAppInfo* appInfoParam =
        static_cast<TPhoneCmdParamAppInfo*>( aCommandParam );

    TApaTaskList taskList( iEikEnv.WsSession() );
    TApaTask task = taskList.FindApp( appInfoParam->AppUid() );
    if ( task.Exists() )
        {
        CAknSgcClient::MoveApp( task.WgId(), ESgcMoveAppToForeground );
        }
    else
        {
        CreateConnectionL();
        HBufC* param = HBufC::NewLC( appInfoParam->Param().Length() );
        param->Des().Copy( appInfoParam->Param() );
        TThreadId id;
        User::LeaveIfError(
            iApaLsSession.StartDocument( *param,
            appInfoParam->AppUid(), id ) );
        CleanupStack::PopAndDestroy( param );
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ActivateAppL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::ActivateAppL(
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamAppInfo* appInfoParam =
        static_cast<TPhoneCmdParamAppInfo*>( aCommandParam );

    __PHONELOG( EBasic, EPhoneUIView,"CPhoneViewController::ActivateAppL()" );

    CreateConnectionL();

    if ( appInfoParam->MultipleInstances() )
        {
        ActivateAppInstanceL( appInfoParam->AppUid() );
        }
    else
        {
        TApaTaskList apaTaskList( iEikEnv.WsSession() );
        TApaTask apaTask = apaTaskList.FindApp( appInfoParam->AppUid() );
        if ( apaTask.Exists() )
            {
            __PHONELOG( EBasic, EPhoneUIView,
                "CPhoneViewController::ActivateAppL() bring to foreground" );
            CAknSgcClient::MoveApp( apaTask.WgId(), ESgcMoveAppToForeground );
            }
        else
            {
            ActivateAppInstanceL( appInfoParam->AppUid() );
            }
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ActivateAppInstanceL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::ActivateAppInstanceL( TUid aUid )
    {
    __PHONELOG( EBasic, EPhoneUIView,
        "CPhoneViewController::ActivateAppInstanceL()" );
    CreateConnectionL();

    TApaAppInfo appInfo;
    User::LeaveIfError( iApaLsSession.GetAppInfo( appInfo, aUid ) );

#ifdef SYMBIAN_SUPPORT_UI_FRAMEWORKS_V1
    CApaCommandLine* apaCommandLine = CApaCommandLine::NewLC(
        appInfo.iFullName );
#else
    CApaCommandLine* apaCommandLine = CApaCommandLine::NewLC();
    apaCommandLine->SetExecutableNameL( appInfo.iFullName );
    apaCommandLine->SetCommandL( EApaCommandRun );
#endif // SYMBIAN_SUPPORT_UI_FRAMEWORKS_V1

    User::LeaveIfError ( iApaLsSession.StartApp( *apaCommandLine ) );
    CleanupStack::PopAndDestroy( apaCommandLine );
    __PHONELOG( EBasic, EPhoneUIView,
        "CPhoneViewController::ActivateAppInstanceL() done" );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ActivateAppViewL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::ActivateAppViewL(
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamAppInfo* appInfoParam =
        static_cast<TPhoneCmdParamAppInfo*>( aCommandParam );

    __PHONELOG( EBasic, EPhoneUIView,
                "CPhoneViewController::ActivateAppViewL()" );

    TVwsViewId view( appInfoParam->AppUid(), appInfoParam->ViewUid() );
    iEikEnv.EikAppUi()->ActivateViewL( view );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ActivateAppViewL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::ActivateAppViewWithCustomMessageL(
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamAppInfo* appInfoParam =
        static_cast<TPhoneCmdParamAppInfo*>( aCommandParam );

    __PHONELOG( EBasic, EPhoneUIView,
        "CPhoneViewController::ActivateAppViewWithCustomMessageL()" );

    TVwsViewId view( appInfoParam->AppUid(), appInfoParam->ViewUid() );
    iEikEnv.EikAppUi()->ActivateViewL(
        view,
        appInfoParam->CustomMessageId(),
        appInfoParam->CustomMessage() );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::CreateConnectionL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::CreateConnectionL()
    {
    if ( !iApaLsSession.Handle() )
        {
        User::LeaveIfError( iApaLsSession.Connect() );
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::LaunchMultimediaSharingL (static)
// ---------------------------------------------------------------------------
//
void CPhoneViewController::LaunchMultimediaSharingL()
    {
    CMusManager* manager = CMusManager::NewLC();
    manager->StartApplicationL( MultimediaSharing::EMusLiveVideo );
    CleanupStack::PopAndDestroy( manager );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::LaunchApplicationL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::LaunchApplicationL()
    {
    //long 0 launches application defined in KTelAppLaunch key.
    TInt appId(0);
    TUid appUid;
    User::LeaveIfError( CPhoneCenRepProxy::Instance()->GetInt(
        KCRUidTelPrivateVariation,
        KTelAppLaunch,
        appId ) );
    appUid.iUid = appId;

    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::LaunchApplicationL appId: %d",
        appId );

    HBufC* params = HBufC::NewLC( KPhoneMaxParamLength );
    TPtr ptr = params->Des();
    User::LeaveIfError( CPhoneCenRepProxy::Instance()->GetString(
        KCRUidTelPrivateVariation,
        KTelAppLaunchParam,
        ptr ) );

    TApaTaskList taskList( iEikEnv.WsSession() );
    TApaTask task = taskList.FindApp( appUid );
    if ( task.Exists() )
        {
        // Uid is not used.
        HBufC8* param = CnvUtfConverter::ConvertFromUnicodeToUtf8L( *params );
        User::LeaveIfError( task.SendMessage( TUid::Uid( 0 ), *param ) );
        delete param;
        }
    else
        {
        CreateConnectionL();
        TThreadId thread;
        User::LeaveIfError(
            iApaLsSession.StartDocument(
                *params,
                appUid,
                thread ) );
        }
    CleanupStack::PopAndDestroy( params ); // params
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::LaunchHelpApplicationL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::LaunchHelpApplicationL(
    const TDesC& aHelpCommand )
    {
    // Save the help command
    iHelpCommand.Set( aHelpCommand );

    // Activate Help application
    HlpLauncher::LaunchHelpApplicationL(
       CCoeEnv::Static()->WsSession(),
       iEikEnv.EikAppUi()->AppHelpContextL() );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::HelpContext
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CPhoneViewController::HelpContext()
    {
    return iHelpCommand;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetCbaL
// Set the CBA to the specified setting
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SetCbaL( TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamInteger* cbaResource =
        static_cast<TPhoneCmdParamInteger*>( aCommandParam );

    TInt resourceId = cbaResource->Integer();

    iCba->SetCommandSetL( resourceId );

    iCba->DrawDeferred();

    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetEikonNotifiersDisabled
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SetEikonNotifiersDisabled(
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamBoolean* booleanParam =
        static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

    if ( booleanParam->Boolean() )
        {
        if ( !iPriotityChanged )
            {
            //Store current ordinal priority.
            iPrevious = CEikonEnv::Static()->RootWin().OrdinalPriority();
            iPriotityChanged = ETrue;
            //Rise window priority in order to get phoneview on top of eikon
            //notifiers. e.g. Select USB mode notifier is the one which using
            //eikon notiers.
            CEikonEnv::Static()->RootWin().SetOrdinalPosition(
                    0,
                    ECoeWinPriorityAlwaysAtFront-1 );
            __PHONELOG( EBasic, EPhoneUIView,
            "CPhoneViewController::SetEikonNotifiersDisabled()_priority_rise" );
            }
        }
    // enable notifiers, sametime reset previous rootwin priority
    else if ( iPriotityChanged )
        {
        TInt ordinalPos = CEikonEnv::Static()->RootWin().OrdinalPosition();
        CEikonEnv::Static()->RootWin().SetOrdinalPosition(
                ordinalPos,
                iPrevious );
        iPriotityChanged = EFalse;
        __PHONELOG( EBasic, EPhoneUIView,
        "CPhoneViewController::SetEikonNotifiersDisabled()_priority_reset" );
        }
    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::SetEikonNotifiersDisabled(%d)",
        booleanParam->Boolean() );
    }
// ---------------------------------------------------------------------------
// CPhoneViewController::SetGlobalNotifiersDisabledL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SetGlobalNotifiersDisabledL(
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamBoolean* booleanParam =
        static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

    // Global note always removed if exists when notifiers disabled
    if ( booleanParam->Boolean() )
        {
        ExecuteCommandL( EPhoneViewRemoveGlobalNote );
        }
    iKeyLock.EnableSoftNotifications( !booleanParam->Boolean() );
    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::SetGlobalNotifiersDisabledL(%d)",
        booleanParam->Boolean() );
    }

// CPhoneViewController::CancelAllNotications
// ---------------------------------------------------------------------------
//
void CPhoneViewController::CancelAllNotications()
    {
    iKeyLock.CancelAllNotifications();
    __PHONELOG( EBasic, EPhoneUIView,
        "CPhoneViewController::CancelAllNotications()");
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::HandleUpdateBubbleL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::HandleUpdateBubbleL(
    TInt aCallId,
    TPhoneCommandParam* aCommandParam )
    {
     // Check is the given parameter valid
    if ( aCommandParam->ParamId() ==
        TPhoneCommandParam::EPhoneParamIdCallHeaderData )
        {
        __PHONELOG( EBasic, EPhoneUIView,
            "CPhoneViewController::HandleUpdateBubble()" );

        TPhoneCmdParamCallHeaderData* callHeaderParam =
            static_cast<TPhoneCmdParamCallHeaderData*>( aCommandParam );
        iBubbleWrapper->PrepareCallStateL(
            aCallId,
            callHeaderParam->CallState(),
            callHeaderParam->LabelText(),
            callHeaderParam->ShortLabelText() );
        }
    }

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// CPhoneViewController::SoftRejectIsEnabled
// ---------------------------------------------------------------------------
//
TBool CPhoneViewController::SoftRejectIsEnabled() const
    {
    // Check if phone is locked
    const TBool phoneIsLocked =
        CPhonePubSubProxy::Instance()->Value(
        KPSUidCoreApplicationUIs,
        KCoreAppUIsAutolockStatus ) > EAutolockOff;

    if ( phoneIsLocked || IsSecurityCodeQuery() )
        {
        // Soft Reject is enabled if the phone is not locked.
        // Security Code query is checked because in the boot
        // autolock status value is OFF even when it's actually
        // ON so we need to do this double checking.
        return EFalse;
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::IsSecurityCodeQuery
// ---------------------------------------------------------------------------
//
TBool CPhoneViewController::IsSecurityCodeQuery() const
  {
  return CPhonePubSubProxy::Instance()->Value(
    KPSUidStartup,
    KStartupSecurityCodeQueryStatus ) == ESecurityQueryActive;
  }

// ---------------------------------------------------------------------------
// CPhoneViewController::GetNumberFromSpeedDialLocationL
// ---------------------------------------------------------------------------
//
TBool CPhoneViewController::GetNumberFromSpeedDialLocationL(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::GetNumberFromSpeedDialLocationL() ");
    TPhoneCmdParamSpeedDial* speedDialContent =
        static_cast<TPhoneCmdParamSpeedDial*>( aCommandParam );
    TBool retValue( ETrue );

    if ( speedDialContent->Location() != KErrNotFound )
        {
        TBuf<KPhoneNumberEntryBufferSize> speedDialNumber;
        speedDialNumber.Zero(); // empty the string

        // Indicate that the speed dial controller is blocking key
        // events from the Phone
        iBlockingDialogIsDisplayed = ETrue;

        CreateSpeedDialControllerL();

        TInt err = iVmbxHandler->GetStraightSpeedDialNumber(
                        speedDialContent->Location(),
                        speedDialNumber );

        // Straight number founded
        if ( err == KErrNone && speedDialNumber.Length() != 0)
            {
            if ( iVmbxHandler->GetMailboxType() == EVmbxVideo )
                {
                speedDialContent->SetNumberType( EPhoneNumberTypeVideo );
                }
            else
                {
                speedDialContent->SetNumberType( EPhoneNumberTypeCS );
                }
            }
        else if ( iVmbxHandler->IsStraightCallToVideoMailBox() &&
              ( speedDialContent->Location() == KPhoneVideoMbxLocation ||
              speedDialContent->Location() == KPhoneVoiceMailBoxLocation ))
            {
            // Mbx number field of given type was empty and assigning cancelled
            retValue = EFalse;
            }

        else if ( speedDialContent->Location() == KPhoneVoiceMailBoxLocation )
            {
            // Get the voice mail box type and service ID
            TUint serviceId( 0 );
            TInt mailboxType = iVmbxHandler->SelectMailboxTypeL( serviceId );
            speedDialContent->SetServiceId( serviceId );

            // Get the voice mail box number
            if ( mailboxType == EVmbxNone )
              {
              // Nothing selected from the mailbox selection
              retValue = EFalse;
              }
            else if ( iVmbxHandler->LaunchMbx( mailboxType, speedDialNumber )
                      == KErrNone )
                {
                if ( mailboxType == EVmbxVideo )
                    {
                    speedDialContent->SetNumberType( EPhoneNumberTypeVideo );
                    }
                else if ( mailboxType == EVmbxIP )
                    {
                    speedDialContent->SetNumberType( EPhoneNumberTypeVoip );
                    }
                else
                    {
                    speedDialContent->SetNumberType( EPhoneNumberTypeCS );
                    }
                }
            else
              {
              // Mbx number field of given type was empty and assigning cancelled
              retValue = EFalse;
              }
            }
        else
            {
            // Get the number from the speed dial location
            iSpeedDialController->GetSpeedDialFieldL(
                speedDialContent->Location(), speedDialNumber );
            if (speedDialNumber.Length() == 0 )
                {
                AssignSpeedDialLocationL( speedDialContent->Location() );
                // Get the number from the speed dial location
                iSpeedDialController->GetSpeedDialFieldL(
                    speedDialContent->Location(), speedDialNumber );
                }

            // If assigned
            if ( speedDialNumber.Length() )
                {
                speedDialContent->SetNumberType(
                    SpeedDialNumberTypeL( speedDialContent->Location() ) );
                }
            else
                {
                retValue = EFalse;
                }
            }

        // Reset the flag
        iBlockingDialogIsDisplayed = EFalse;

        TPtr* phoneNumber = speedDialContent->PhoneNumber();
        phoneNumber->Append( speedDialNumber );

        }
    return retValue;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SpeedDialNumberTypeL
// ---------------------------------------------------------------------------
//
TPhoneNumberType CPhoneViewController::SpeedDialNumberTypeL(
        TInt aSpeedDialLocation )
    {
    __LOGMETHODSTARTEND( EPhoneUIView,
            "CPhoneViewController::SpeedDialNumberTypeL() ");
    CPhCntSpeedDialMonitor::TSpdDialFieldInfo fieldInfo;

    TBuf<KPhoneNumberEntryBufferSize> number;
    TContactItemId itemId = iSpeedDialController->GetSpeedDialFieldL(
                                aSpeedDialLocation,
                                number,
                                fieldInfo );

    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::SpeedDialNumberTypeL - itemId = %d ", itemId );

    return ConvertNumberTypeFromSpeedDialFieldInfo( fieldInfo );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::ConvertNumberTypeFromSpeedDialFieldInfo
// ---------------------------------------------------------------------------
//
TPhoneNumberType CPhoneViewController::ConvertNumberTypeFromSpeedDialFieldInfo(
    const CPhCntSpeedDialMonitor::TSpdDialFieldInfo& aFieldInfo )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::ConvertNumberTypeFromSpeedDialFieldInfo() ");
    TPhoneNumberType retValue;
    switch( aFieldInfo.iNumberType )
        {
        case MPhCntMatch::EVideoNumber:
            retValue =  EPhoneNumberTypeVideo;
            break;
        case MPhCntMatch::EVoipNumber:
            retValue =  EPhoneNumberTypeVoip;
            break;
        case KErrNotFound:
            retValue = EPhoneNumberTypeNotFound;
            break;
        default:
            retValue = EPhoneNumberTypeCS;
            break;
        }
    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::ConvertNumberTypeFromSpeedDialFieldInfo - number type = %d ",
        retValue );

    return retValue;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::AssignSpeedDialLocation
// ---------------------------------------------------------------------------
//
TBool CPhoneViewController::AssignSpeedDialLocation(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneViewController::AssignSpeedDialLocation( ) ")

    TPhoneCmdParamInteger* locationParam =
        static_cast<TPhoneCmdParamInteger*>( aCommandParam );

    // Indicate that the contact operation is blocking key
    // events from the Phone
    iBlockingDialogIsDisplayed = ETrue;
    TBool numberAssigned = EFalse;
    TRAP_IGNORE( numberAssigned = AssignSpeedDialLocationL(locationParam->Integer()) );

    // Reset the flag
    iBlockingDialogIsDisplayed = EFalse;

    return numberAssigned;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::AssignSpeedDialLocationL
// ---------------------------------------------------------------------------
//
TBool CPhoneViewController::AssignSpeedDialLocationL(
    TInt aLocation )
    {
    CreateSpeedDialControllerL();

    // Assign speed dial location
    TBuf<KPhoneNumberEntryBufferSize> number;

    const TBool numberAssigned =
        iSpeedDialController->AssignSpeedDialFieldL( aLocation, number );

    return numberAssigned;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::OpenMessageEditorL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::OpenMessageEditorL(
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamMessageEditor* messageEditorParam =
        static_cast<TPhoneCmdParamMessageEditor*>( aCommandParam );

    //Launch message editor standalone
    TUid mtmUid = { KSenduiMtmUniMessageUidValue };

    CSendUi* messageEditor = CSendUi::NewL();
    CleanupStack::PushL( messageEditor );

    // Inform the SendUi to open the message editor. This is not a blocking
    // dialog so the flag does not need to be set.
    messageEditor->CreateAndSendMessageL(
        mtmUid,
        messageEditorParam->MessageData(),
        KNullUid,
        EFalse );

    CleanupStack::PopAndDestroy( messageEditor );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::OpenContactL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::OpenAddToContactL(
    TInt aCommand,
    const TDesC& aString )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneViewController::OpenAddToContactL( ) ")

    TPhoneCmdParamString phoneNumberString;

    HBufC *phoneNumber = HBufC::NewL( KPhoneNumberEntryBufferSize );

    if ( KErrNone == aString.Compare( KNullDesC ) )
        {
        // get the number entry contents
        TPtr ptr( phoneNumber->Des() );
        phoneNumberString.SetString( &ptr );
        iBubbleWrapper->GetNumberEntryContent( &phoneNumberString );
        }
    else
        {
        phoneNumber->Des().Copy( aString );
        }

    // Indicate that the contact operation is blocking key
    // events from the Phone
    iBlockingDialogIsDisplayed = ETrue;

    TRAPD( err,
        {
        // Create the contact dialog
        iContactSaveAddToName = iContactController->CreateSaveAddToNameL();

        // The contact operation may be cancelled if the END key
        // is pressed. If so, the blocking dialog will be removed.
        if ( iBlockingDialogIsDisplayed )
            {
            // Inform the Contact to open the contacts
            iContactSaveAddToName->HandlePbkOperationL(
                *phoneNumber, // TelNum
                aCommand );//CreateNew/Update
            }
        }) //TRAPD

    RemoveAddToContactDialog();
    delete phoneNumber;

    // Reset the flag
    iBlockingDialogIsDisplayed = EFalse;

    User::LeaveIfError( err );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::RemovePhoneDialogsL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::RemovePhoneDialogsL()
    {
    __PHONELOG( EBasic, EPhoneUIView,
        "CPhoneViewController::RemovePhoneDialogsL()" );
    // Remove the note, if necessary
    iNoteController->DestroyNote();

    // Remove the query, if necessary.
    // Global query not allowed to destroy in this phase.
    iQueryController->DestroyQuery();

    // Remove contact dialog.
    RemoveAddToContactDialog();

    // Dismiss queries from customization, if necessary
   if ( iCustomization )
        {
        iCustomization->RemoveDialog();
        }

    // Remove speeddial dialogs.
    if ( iSpeedDialController )
        {
        iSpeedDialController->Cancel();
        }

    if ( iSingleItemFetch )
        {
        // Cancel the Single Item Fetch
        iSingleItemFetch->Delete();
        }

    if ( iVmbxHandler )
        {
        iVmbxHandler->CancelVmbx();
        }

    if ( iRfsHandler )
        {
        __PHONELOG( EBasic, EPhoneUIView,
            "CPhoneViewController::RemovePhoneDialogsL() CPhCntRfsHandler::ERfsCmdCancel" );
        iRfsHandler->ProcessCommandL( CPhCntRfsHandler::ERfsCmdCancel );
        delete iRfsHandler;
        iRfsHandler = NULL;
        }

    // Remove all blocking dialogs
    if ( iBlockingDialogIsDisplayed )
        {
        // Reset the flag
        iBlockingDialogIsDisplayed = EFalse;

        __PHONELOG( EBasic, EPhoneUIView,
            "CPhoneViewController::RemovePhoneDialogsL() AknDialogShutter::ShutDialogsL" );

        TRAP_IGNORE(
            AknDialogShutter::ShutDialogsL( iEikEnv ) );
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::RemoveAddToContactDialog
// ---------------------------------------------------------------------------
//
void CPhoneViewController::RemoveAddToContactDialog()
    {
    if ( iContactSaveAddToName )
        {
        delete iContactSaveAddToName;
        iContactSaveAddToName = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::DoConstructContactControllerL
//
// Callback function.
// ---------------------------------------------------------------------------
//
TInt CPhoneViewController::DoConstructContactControllerL( TAny* aAny )
    {
    CPhoneViewController* viewController =
        static_cast< CPhoneViewController* >( aAny );

    return viewController->iContactController->ContinueConstructL();
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::UpdateAudioPathOptions
// ---------------------------------------------------------------------------
//
void CPhoneViewController::UpdateAudioPathOptions(
    TPhoneCommandParam* aCommandParam )
    {
    __PHONELOG( EBasic, EPhoneUIView,
        "CPhoneViewController::UpdateAudioPathOptions()" );
    TPhoneCmdParamBoolean ihfParam;
    TPhoneCmdParamBoolean wiredParam;
    TPhoneCmdParamBoolean btaaParam;

    TPhoneCmdParamAudioOutput* outputParam =
        static_cast<TPhoneCmdParamAudioOutput*>( aCommandParam );

    ihfParam.SetBoolean(
        outputParam->AudioOutput() == EPELoudspeaker );
    wiredParam.SetBoolean(
       ( outputParam->AudioOutput() == EPEWiredAudioAccessory ) ||
       ( outputParam->AudioOutput() == EPETTY ) );

    btaaParam.SetBoolean(
        outputParam->AudioOutput() == EPEBTAudioAccessory );

    // Wired options
    iMenuController->SetWiredAccFlag( &wiredParam );
    iToolbarController->SetWiredAccFlag( &wiredParam );
    iMenuController->SetIhfFlag( &ihfParam );
    iToolbarController->SetIhfFlag( &ihfParam );

    // btaa menu options
    iMenuController->SetBTAccFlag( &btaaParam );
	iToolbarController->SetBTAccFlag( &btaaParam );
    iAudioController->HandleIhfChange( &ihfParam );

    if ( iButtonsController )
        {
        iButtonsController->SetIhfFlag( &ihfParam );
        iButtonsController->SetWiredAccFlag( &wiredParam );
        iButtonsController->SetBTAccFlag( &btaaParam );
        iButtonsController->ReplaceAudioButton();
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::UpdateAudioAvailabilityOptions
// ---------------------------------------------------------------------------
//
void CPhoneViewController::UpdateAudioAvailabilityOptions(
    TPhoneCommandParam* aCommandParam )
    {
    __PHONELOG( EBasic, EPhoneUIView,
        "CPhoneViewController::UpdateAudioAvailabilityOptions()" );

    TPhoneCmdParamBoolean wiredAvailableParam;
    TPhoneCmdParamBoolean btAvailableParam;

    TPhoneCmdParamAudioAvailability* outputParam =
        static_cast<TPhoneCmdParamAudioAvailability*>( aCommandParam );

    wiredAvailableParam.SetBoolean(
        outputParam->IsWiredAccAvailable() );

    btAvailableParam.SetBoolean(
        outputParam->IsBTAccAvailable());

    // Wired options
    iMenuController->SetWiredAccAvailableFlag( &wiredAvailableParam );
    iToolbarController->SetWiredAccFlag( &wiredAvailableParam );

    // btaa menu options
    iMenuController->SetBTAccAvailableFlag( &btAvailableParam );
	iToolbarController->SetBTAccAvailableFlag( &btAvailableParam );
	
    if ( iButtonsController )
        {
        iButtonsController->SetBTAccAvailableFlag( &btAvailableParam );
        iButtonsController->ReplaceAudioButton();
        }

    __PHONELOG2(
        EBasic,
        EPhoneUIView,
        "CPhoneViewController::UpdateAudioAvailabilityOptions( BT: (%d), Wired: (%d) ) )",
        btAvailableParam.Boolean(),
        wiredAvailableParam.Boolean() );
    }


// ---------------------------------------------------------------------------
// CPhoneViewController::LaunchRfsL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::LaunchRfsL( TBool aDeepRfs )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::LaunchRfsL()" );
    CPhCntRfsHandler::TRfsCommand command;
    if ( !iRfsHandler )
        {
        iRfsHandler = iContactController->CreateRfsHandlerL();
        }

    if( aDeepRfs )
        {
        command = CPhCntRfsHandler::ERfsCmdActivateDeep;
        }
    else
        {
        command = CPhCntRfsHandler::ERfsCmdActivateNormal;
        }

    // Indicate that the rfs dialog is blocking key
    // events from the Phone
    iBlockingDialogIsDisplayed = ETrue;

    TRAPD( err,
        {
        if ( iDialer )
            {
            ExecuteCommandL( EPhoneViewClearNumberEntryContent );
            }
        else
            {
            // Remove number entry
            ExecuteCommandL( EPhoneViewRemoveNumberEntry );
            }

        iRfsHandler->ProcessCommandL( command );
        })
    delete iRfsHandler;
    iRfsHandler = NULL;

    iBlockingDialogIsDisplayed = EFalse;
    User::LeaveIfError( err );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::OpenSoftRejectMessageEditorL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::OpenSoftRejectMessageEditorL(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::OpenSoftRejectMessageEditorL ()" );

    TPhoneCmdParamSfiData* sfiParam =
        static_cast<TPhoneCmdParamSfiData*>( aCommandParam );

    CMessageData* messageData = CMessageData::NewL();
    CleanupStack::PushL( messageData );

    // Get message body
    HBufC* softRejectTxt = HBufC::NewLC( KPhoneSMSLength );
    TPtr string( softRejectTxt->Des() );

    TInt softRejectDefaultInUseValue = 0;
    const TInt err = CPhoneCenRepProxy::Instance()->GetInt(
        KCRUidTelephonySettings,
        KSettingsSoftRejectDefaultInUse,
        softRejectDefaultInUseValue );

    if ( err == KErrNone && softRejectDefaultInUseValue )
        {
        // Default txt used or not
        CPhoneCenRepProxy::Instance()->GetString(
            KCRUidTelephonySettings,
            KSettingsSoftRejectText,
            string );
        }
    else
        {

        TParse fp;
        fp.Set( KPhoneSoftRejectResource, &KDC_RESOURCE_FILES_DIR, NULL );
        TFileName name( fp.FullName() );
        // Open soft reject resource file
        RConeResourceLoader coneResourceLoader( iEikEnv );
        TInt softRejectResourceError = coneResourceLoader.Open( name );
        if ( softRejectResourceError != KErrNone )
            {
            // Indicate that the soft reject feature cannot be enabled
            Panic( EPhoneViewErrorInAddingResourceFile );
            }

        CleanupStack::PopAndDestroy( softRejectTxt );
        softRejectTxt = StringLoader::LoadLC( R_SOFT_REJECT_DEFAULT_TXT );
        string.Set( softRejectTxt->Des() );
        coneResourceLoader.Close();
        }

    CRichText* text = CRichText::NewL(
        iEikEnv.SystemParaFormatLayerL(),
        iEikEnv.SystemCharFormatLayerL());
    CleanupStack::PushL( text );
    text->Reset();
    text->InsertL( 0, string );

    // Set body text
    messageData->SetBodyTextL( text );

    // store the remote caller's info
    if ( sfiParam->Name().Length() )
        {
        // store both the name and the number
        messageData->AppendToAddressL(
            sfiParam->Number(), sfiParam->Name() );
        }
    else
        {
        // store the number
        messageData->AppendToAddressL(
            sfiParam->Number() );
        }

    // Inform SysAp not to re-activate keyguard even if the call ends.
    CPhonePubSubProxy::Instance()->ChangePropertyValue(
        KPSUidCoreApplicationUIs,
        KCoreAppUIsSoftReject,
        ECoreAppUIsSoftReject );

    // Launch the Soft Reject message Editor.
    TPhoneCmdParamMessageEditor messageEditorParam;
    messageEditorParam.SetMessageData( messageData );
    OpenMessageEditorL( &messageEditorParam );

    CleanupStack::PopAndDestroy( text );
    CleanupStack::PopAndDestroy( softRejectTxt );
    CleanupStack::PopAndDestroy( messageData );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SendMessageL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SendMessageL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::SendMessageL()" );
    // get the number entry contents
    HBufC *phoneNumber = HBufC::NewLC( KPhoneNumberEntryBufferSize );
    TPtr ptr( phoneNumber->Des() );
    TPhoneCmdParamString stringParam;
    stringParam.SetString( &ptr );
    iBubbleWrapper->GetNumberEntryContent( &stringParam );

    // Launch the message Editor.
    TPhoneCmdParamMessageEditor messageEditorParam;

    // create the message data
    CMessageData* messageData = CMessageData::NewLC();
    messageData->AppendToAddressL( ptr );
    messageEditorParam.SetMessageData( messageData );

    if ( !iDialer )
        {
        // Remove number entry from screen
        iPhoneView->SetControl( iBubbleWrapper->Control() );
        ExecuteCommandL( EPhoneViewRemoveNumberEntry );
        }

    // Open the message editor
    OpenMessageEditorL( &messageEditorParam );

    CleanupStack::PopAndDestroy( messageData );
    CleanupStack::PopAndDestroy( phoneNumber );
    }

// -----------------------------------------------------------
// CPhoneViewController::SetStatusPaneVisible
// -----------------------------------------------------------
//
void CPhoneViewController::SetStatusPaneVisible(
                                        TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::SetStatusPaneVisible()" );
    TPhoneCmdParamBoolean* booleanParam = static_cast<TPhoneCmdParamBoolean*>(
        aCommandParam );
    iStatusPane->StatusPane().MakeVisible( booleanParam->Boolean() );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::UpdateFSW()
// ---------------------------------------------------------------------------
//
void CPhoneViewController::UpdateFSW()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneViewController::UpdateFSW()" );
    iAknUiServerClient->UpdateTaskList();
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::CreateSpeedDialControllerL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::CreateSpeedDialControllerL()
  {
    __LOGMETHODSTARTEND( EPhoneUIView,
        "CPhoneViewController::CreateSpeedDialControllerL()" );
    if( !iSpeedDialController )
        {
        iSpeedDialController = iContactController->CreateSpeedDialMonitorL();
        }

    if( !iVmbxHandler )
        {
        iVmbxHandler = CPhoneVmbxHandler::NewL();
        }
  }

// ---------------------------------------------------------------------------
// CPhoneViewController::IsReplaceAllowed()
// ---------------------------------------------------------------------------
//
TBool CPhoneViewController::IsReplaceAllowed()
    {
    __LOGMETHODSTARTEND( EPhoneUIView,
                         "CPhoneViewController::IsReplaceAllowed()");

    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnected );
    iBubbleWrapper->GetCallIdByState( &callStateData );
    TInt connected = callStateData.CallId();

    callStateData.SetCallState( EPEStateRinging );
    iBubbleWrapper->GetCallIdByState( &callStateData );
    TInt waiting = callStateData.CallId();

    return // both video or
        (( connected == KVideoTelephonyCallId1 ||
           connected == KVideoTelephonyCallId2 ) &&

         ( waiting == KVideoTelephonyCallId1 ||
           waiting == KVideoTelephonyCallId2 )) ||

           // both voice ( including active conference )
        ((( connected >= KVoiceCallIdFirst &&
            connected < KEmergencyCallId ) ||
            connected == KConferenceCallId ) &&

         ( waiting >= KVoiceCallIdFirst &&
           waiting < KEmergencyCallId ));
    }


// ---------------------------------------------------------------------------
// CPhoneViewController::SingleItemFetchL()
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SingleItemFetchL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::SingleItemFetchL()" );

    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( EPhoneNewCallFetchTitle );

    // Make sure the dialog is not ended prematurely
    HandleCommandL(
        EPhoneViewOpenSingleItemFetchDialog,
        &integerParam );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetFetchContent
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SetFetchContent( TDesC& aFetchContent )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::SetFetchContent()" );
    iFetchContent = aFetchContent;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::FetchContent
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CPhoneViewController::FetchContent()
   {
   __LOGMETHODSTARTEND(EPhoneUIView,
       "CPhoneViewController::FetchContent()" );
   return iFetchContent;
   }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetSingleItemFetchType
// -----------------------------------------------------------
//
void CPhoneViewController::SetSingleItemFetchType( TInt aType )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::SetSingleItemFetchType()" );
    iSingleItemFetchType = aType;
    }

// -----------------------------------------------------------
// CPhoneViewController::SingleItemFetchType
// -----------------------------------------------------------
//
TInt CPhoneViewController::SingleItemFetchType() const
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
       "CPhoneViewController::SingleItemFetchType()" );

    return iSingleItemFetchType;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SwitchLayoutToFlatStatusPaneL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SwitchLayoutToFlatStatusPaneL( TBool aSwitch )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::SwitchLayoutToFlatStatusPaneL()" );

    if ( aSwitch )
        {
        SwapEmptyIndicatorPaneL( ETrue );
        if ( !Layout_Meta_Data::IsLandscapeOrientation() )
            {
            iStatusPane->StatusPane().SwitchLayoutL
                    ( R_AVKON_STATUS_PANE_LAYOUT_USUAL_FLAT );
            }
        else
            {
            iStatusPane->StatusPane().SwitchLayoutL
                    ( R_AVKON_WIDESCREEN_PANE_LAYOUT_USUAL_FLAT );
            }
        }
    else
        {
        SwapEmptyIndicatorPaneL( EFalse );
        if ( !Layout_Meta_Data::IsLandscapeOrientation() )
            {
            iStatusPane->StatusPane().SwitchLayoutL
                    ( R_AVKON_STATUS_PANE_LAYOUT_IDLE );
            }
        else
            {
            iStatusPane->StatusPane().SwitchLayoutL
                    ( R_AVKON_WIDESCREEN_PANE_LAYOUT_USUAL_FLAT );
            }
        }
    // ApplyCurrentSettingsL is called whenever statuspane
    // visibility status or pane layout changes.
    iStatusPane->StatusPane().ApplyCurrentSettingsL();
    iStatusPane->StatusPane().DrawNow();
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SwapEmptyIndicatorPaneL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SwapEmptyIndicatorPaneL(
                                    const TBool aSwapEmpty )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::SwapEmptyIndicatorPaneL()" );
    if ( iSecurityMode )
        {
        if ( aSwapEmpty )
            {
            if ( !iIndiContainer )
                {
                iIndiContainer =
                iStatusPane->CreateEmptyIndicatorContainerL();
                }
            //Save previous indicator control
            iPreviousIndicatorControl =
                                    iStatusPane->StatusPane().SwapControlL(
            TUid::Uid( EEikStatusPaneUidIndic ), iIndiContainer );
            }
        else if ( iPreviousIndicatorControl )
            {
            //Restore previous indicator control
            iStatusPane->StatusPane().SwapControlL(
            TUid::Uid( EEikStatusPaneUidIndic ), iPreviousIndicatorControl );
            }
        }
    else if ( iPreviousIndicatorControl 
              && iIndiContainer
              && ( iIndiContainer ==
                      iStatusPane->StatusPane().ControlL(
                              TUid::Uid( EEikStatusPaneUidIndic ) ) ) )
        {
        //Restore previous indicator control if security mode is not on anymore 
        // and empty indicator pane is still in status pane.
        iStatusPane->StatusPane().SwapControlL(
        TUid::Uid( EEikStatusPaneUidIndic ), iPreviousIndicatorControl );
        }
    }


// ---------------------------------------------------------------------------
// CPhoneViewController::HandleTransitionEffect
// ---------------------------------------------------------------------------
//
void CPhoneViewController::HandleTransitionEffect(
    TPhoneTransEffectType aType, const TUid& aAppUidEffectParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::HandleTransitionEffect()" );
    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::HandleTransitionEffect aType:(%d)", aType );
    TInt useEffect(KErrNotFound);
    switch ( aType )
        {
        case EPhoneTransEffectDialerCreate:
        case EPhoneTransEffectPhoneUiOpen:
            useEffect = KTouchPhoneUiOpenEffect;
            break;
        case EPhoneTransEffectPhoneUiClose:
            useEffect = KTouchPhoneUiCloseEffect;
            break;
        case EPhoneTransEffectAppStartFromDialer:
            useEffect = AknTransEffect::EApplicationStart;
            break;
        case EPhoneTransEffectCallUiAppear:
            useEffect = KTouchCallUiOpenEffect;
            break;
        case EPhoneTransEffectCallUiDisappear:
            useEffect = KTouchCallUiCloseEffect;
            break;
        case EPhoneTransEffectStop:
            {
            if ( iEffectOngoing )
                {
                GfxTransEffect::EndFullScreen();
                iEffectOngoing = EFalse;
                }
            break;
            }
        default:
            break;
        }
    
    if ( KErrNotFound < useEffect )
        {
        GfxTransEffect::BeginFullScreen(
            useEffect,
            TRect(),
            AknTransEffect::EParameterType,
            AknTransEffect::GfxTransParam( aAppUidEffectParam,
                    AknTransEffect::TParameter::EActivateExplicitCancel ) );
        iEffectOngoing = ETrue;
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetControltoDialerL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SetControltoDialerL()
    {
    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::SetControltoDialerL iDialerActive (%d)", iDialerActive );

    SetIncallBubbleVisibility( ETrue );

    if ( !iDialerActive )
        {
        iDialerActive = ETrue;

        // Ensure that toolbar and menu commands are up-to-date
        TBuf<1> temp; // we are only interested if the text is empty or not
        iDialer->GetTextFromNumberEntry( temp );
        NumberEntryStateChanged( temp.Length() > 0 );

        // Make dialer view top most
        iDialerView->DrawableWindow()->SetOrdinalPosition(
            iPhoneView->DrawableWindow()->OrdinalPosition() );

        // Hide in-call view toolbar
        iToolbarController->HideToolbar();

        SwitchLayoutToFlatStatusPaneL( ETrue );

        // Don't make dialer view visible before status pane is updated.
        // This prevents unnecessary resizings.
        // But do it right away after it. Otherwice a black toolbar area is shortly shown.
        iDialerView->MakeVisible( ETrue );
        // Number entry is emptied when dialer is hidden but drawing doesn't
        // succeed at that point as dialer is hidden first. So must draw
        // dialer as soon as it becomes visible to prevent the flashing of
        // number entry (DrawDeferred() isn't fast enough here).
        iDialerView->DrawNow();

        iPhoneView->DrawableWindow()->SetOrdinalPosition( -1 );
        iPhoneView->MakeVisible( EFalse );

        // Update control stack
        iAppui->AddToStackL( iDialerView );
        iAppui->RemoveFromStack( iPhoneView );
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetControltoCallHandlingL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SetControltoCallHandlingL()
    {
    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::SetControltoCallHandlingL iDialerActive (%d)", iDialerActive );

    SetIncallBubbleVisibility( EFalse );
    iToolbarController->ShowToolbar();
    if ( iDialerActive )
        {
        iDialerActive = EFalse;
        // Hide dialer view. Do this before resizing status pane to prevent unnecessary
        // resizing of dialer components. Hiding dialer view already before showing
        // phone view might cause screen flickering but tests have proven it doesn't happen.
        iDialerView->MakeVisible( EFalse );

        SwitchLayoutToFlatStatusPaneL( EFalse );

        // Make call handling view top most
        iPhoneView->DrawableWindow()->SetOrdinalPosition(
            iDialerView->DrawableWindow()->OrdinalPosition() );
        iPhoneView->MakeVisible( ETrue );
        iPhoneView->DrawNow();

        iDialerView->DrawableWindow()->SetOrdinalPosition( -1 );

        // Update control stack
        iAppui->AddToStackL( iPhoneView );
        iAppui->RemoveFromStack( iDialerView );

        iEasyDialingController->HandleCommandL( EEasyDialingCallHandlingActivated );
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetDialerControlAndVisibilityL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SetDialerControlAndVisibilityL()
    {
    __PHONELOG( EBasic, EPhoneUIView,
        "CPhoneViewController::SetDialerControlAndVisibilityL" );

    if ( iBubbleWrapper->IsNumberEntryUsed() )
        {
        if ( !iSendBack )
            {
            SetControltoDialerL();
            // Reset flag to default value.
            iSendBack = ETrue;
            }
        // next active phone app view is dialer.
        iPhoneView->SetPhoneAppViewToDialer( ETrue );
        }
    else
        {
        // Reset flag.
        iPhoneView->SetPhoneAppViewToDialer( EFalse );
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::CapturePointerEvents
// ---------------------------------------------------------------------------
//
void CPhoneViewController::CapturePointerEvents(
                                    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::CapturePointerEvents()" );
    TPhoneCmdParamBoolean* booleanParam = static_cast<TPhoneCmdParamBoolean*>(
        aCommandParam );
    iPhoneView->CapturePointerEvents( booleanParam->Boolean() );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::AllowWaitingCallHeaderL
// ---------------------------------------------------------------------------
//
void CPhoneViewController::AllowWaitingCallHeaderL(
                                           TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneViewController::AllowWaitingCallHeaderL()" );

    TPhoneCmdParamBoolean* booleanParam =
        static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

    // Phone in foreground and control is in dialer ->
    //do not show waiting call header.
    if ( ( ( ApplicationWindowGroupId() ==
             ForegroundApplicationWindowGroupId() ) &&
         ( iDialerActive ) ) )
        {
        booleanParam->SetBoolean( EFalse );
        }
    // Phone isnt in foreground but since control is in dialer and save add to
    // name service is active -> do not show waiting call header.
    else if ( ( ( ApplicationWindowGroupId() !=
                  ForegroundApplicationWindowGroupId() ) &&
              ( iDialerActive ) && iContactSaveAddToName ) )
        {
        booleanParam->SetBoolean( EFalse );
        }
    // If Phone is in foreground and control is in bubble we need to check
    // query status -> if query is active do not show waiting call header, if
    // query is not active then show waiting call header.
    else if ( ( ( ApplicationWindowGroupId() ==
                  ForegroundApplicationWindowGroupId() ) &&
              ( !iDialerActive ) ) )
        {
        TPhoneCmdParamBoolean isQueryDisplayed;
        // Check is query displayed
        ExecuteCommandL( EPhoneViewIsQuery, &isQueryDisplayed );

        if ( isQueryDisplayed.Boolean() )
            {
            booleanParam->SetBoolean( EFalse );
            }
        else
            {
            booleanParam->SetBoolean( ETrue );
            }
        }
    // Phone isnt in foreground but since control is in bubble and single item
    // service is active(note! iBlockingDialogIsDisplayed must be true if it
    // not then iSingleItemFetch is not active anymore) -> do not show waiting
    // call header.
    else if ( ( ( ApplicationWindowGroupId() !=
                  ForegroundApplicationWindowGroupId() ) &&
              ( !iDialerActive ) &&
              ( iSingleItemFetch && iBlockingDialogIsDisplayed ) ) )
        {
        booleanParam->SetBoolean( EFalse );
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::IsDtmfDialerActive
// ---------------------------------------------------------------------------
//
TBool CPhoneViewController::IsDtmfDialerActive() const
    {
    TBool ret =
            ( iDialer && iDtmfDialerController &&
              iDialer->Controller() == iDtmfDialerController );
    return ret;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::IsCustomDialerActive
// ---------------------------------------------------------------------------
//
TBool CPhoneViewController::IsCustomDialerActive() const
    {
    TBool ret = EFalse;
    if ( iDialer )
        {
        MPhoneDialerController* curController = iDialer->Controller();
        ret = ( curController &&
                curController != iDialerController &&
                curController != iDtmfDialerController );
        }
    return ret;
    }

// -----------------------------------------------------------
// CPhoneViewController::CanTransEffectBeUsed
// -----------------------------------------------------------
//
TBool CPhoneViewController::CanTransEffectBeUsed(
    TPhoneTransEffectType aType )
    {
    TBool okToUseEffect( EFalse );
    TBool isPhoneForeground = iAppui->IsForeground();
    TBool isIdleInForeground = 
            ForegroundApplicationWindowGroupId() == IdleWindowGroupId() ?
            ETrue : EFalse;
    if ( isPhoneForeground && 
         IsOkToUseThisTypeOfEffectInsidePhoneApp( aType ) )
        {
        okToUseEffect = ETrue;
        }
    /* In case transition is from idle to dialer or to incoming call 
    show transition effects as well. */
    else if ( isIdleInForeground && 
              ( aType == EPhoneTransEffectPhoneUiOpen ||
                aType == EPhoneTransEffectCallUiAppear ) )
        {
        okToUseEffect = ETrue;
        }
    /* isPhoneForeground/isIdleInForeground are false
    when call is created from some other then phone app for example logs or from phonebook*/
    else if ( !isPhoneForeground && 
              !isIdleInForeground && 
              ( aType == EPhoneTransEffectPhoneUiOpen ||
                aType == EPhoneTransEffectCallUiAppear ) )
        {
        okToUseEffect = ETrue;
        }
    return okToUseEffect;
    }

// -----------------------------------------------------------
// CPhoneViewController::IsOkToUseThisTypeOfEffectInsidePhoneApp
// -----------------------------------------------------------
//
TBool CPhoneViewController::IsOkToUseThisTypeOfEffectInsidePhoneApp(
        TPhoneTransEffectType aType )
    {
    TBool okToUse(EFalse);
    switch ( aType )
        {
        case EPhoneTransEffectDialerCreate:
        case EPhoneTransEffectPhoneUiOpen:
            okToUse = !iDialerActive;
            break;
        case EPhoneTransEffectAppStartFromDialer:
            okToUse = iDialerActive;
            break;
        /* No need to check dialer activity below because effects are 
        common to callhandling/dialer */
        case EPhoneTransEffectPhoneUiClose:
        case EPhoneTransEffectCallUiAppear:
        case EPhoneTransEffectCallUiDisappear:
            okToUse = ETrue;
            break;
        default:
            break;
        }
    __PHONELOG1( EBasic, EPhoneUIView,
                "CPhoneViewController::IsOkToUseThisTypeOfEffectInsidePhoneApp(%d)",
                okToUse );
    return okToUse;
    }

// -----------------------------------------------------------
// CPhoneViewController::BeginTransEffectForAppStartFromDialerLC
// -----------------------------------------------------------
//
void CPhoneViewController::BeginTransEffectForAppStartFromDialerLC(
    const TUid& aAppUid )
    {
    TPhoneCmdParamTransEffect effectParam;
    effectParam.SetType( EPhoneTransEffectAppStartFromDialer );
    effectParam.SetAppUid( aAppUid );

    ExecuteCommand( EPhoneViewBeginTransEffect,  &effectParam );

    TCleanupItem operation( EffectCleanup, this );
    CleanupStack::PushL( operation );
    }

// -----------------------------------------------------------
// CPhoneViewController::EndTransEffect
// -----------------------------------------------------------
//
void CPhoneViewController::EndTransEffect()
    {
    CleanupStack::PopAndDestroy(); // Call EffectCleanup
    }

// -----------------------------------------------------------------------------
// CPhoneViewController::EffectCleanup
// -----------------------------------------------------------------------------
//
void CPhoneViewController::EffectCleanup(TAny* aThis )
    {
    TPhoneCmdParamTransEffect effectParam;
    effectParam.SetType( EPhoneTransEffectStop );

    static_cast<CPhoneViewController*>( aThis )->ExecuteCommand(
        EPhoneViewEndTransEffect, &effectParam );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetNeedToReturnToForegroundAppAfterCall
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SetNeedToReturnToForegroundAppAfterCall(
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamBoolean* booleanParam = static_cast<TPhoneCmdParamBoolean*>(
        aCommandParam );

    iNeedToReturnToForegroundAppAfterCall = booleanParam->Boolean();
    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::SetNeedToReturnToForegroundAppAfterCall(%d)",
        iNeedToReturnToForegroundAppAfterCall );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::GetNeedToReturnToForegroundAppAfterCallL
// ---------------------------------------------------------------------------
//
TBool CPhoneViewController::GetNeedToReturnToForegroundAppAfterCallL()
    {
    // Check that previous app still exists. It might be for example
    // add to contact - dialog that is allready destroyed or user has closed
    // app using task swapper.
    if( KErrNotFound == FindAppByWgIDL( iPrevForegroundAppWg ) )
        {
        iNeedToReturnToForegroundAppAfterCall = EFalse;
        }
    return iNeedToReturnToForegroundAppAfterCall;
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::AllowInCallBubbleInSpecialCases
// ---------------------------------------------------------------------------
//
void CPhoneViewController::AllowInCallBubbleInSpecialCases()
    {
    // this method can be used for special cases like dialer

    if( iDialerActive )
        {
        SetIncallBubbleVisibility( ETrue );
        }
    else
        {
        SetIncallBubbleVisibility( EFalse );
        }
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SetIncallBubbleVisibility
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SetIncallBubbleVisibility( TBool aVisible )
    {
    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneViewController::SetIncallBubbleVisibility(%d)",
        aVisible );

    TRAP_IGNORE( iIncallBubble->SetIncallBubbleAllowedInUsualL( aVisible ) );
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SkinContentChanged
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SkinContentChanged()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneViewController::SkinContentChanged()" );
    iPhoneView->DrawNow();
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SkinConfigurationChanged
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SkinConfigurationChanged( const TAknsSkinStatusConfigurationChangeReason /*aReason*/ )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneViewController::SkinConfigurationChanged()" );
    iPhoneView->DrawNow();
    }

// ---------------------------------------------------------------------------
// CPhoneViewController::SkinPackageChanged
// ---------------------------------------------------------------------------
//
void CPhoneViewController::SkinPackageChanged( const TAknsSkinStatusPackageChangeReason /*aReason*/ )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneViewController::SkinPackageChanged()" );
    iPhoneView->DrawNow();
    }
// End of File
