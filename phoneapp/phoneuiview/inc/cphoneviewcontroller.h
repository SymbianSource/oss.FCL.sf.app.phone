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
* Description:  Controller for the PhoneUI View component
*
*/

#ifndef CPHONEVIEWCONTROLLER_H
#define CPHONEVIEWCONTROLLER_H

//  INCLUDES
#include <apgcli.h>
#include <eikenv.h>
#include <e32base.h>
#include <cntdef.h>
#include <cphcntspeeddialmonitor.h>
#include <AknsSrvClient.h>
#include <aknappui.h>

#include "mphoneviewcommandhandle.h"
#include "cphonerecoverysystem.h"
#include "mphonestatuspaneobserver.h"
#include "tphonecmdparamspeeddial.h"
#include "aknkeylock.h"
#include "tphonecmdparamtranseffect.h"

#include "mphoneviewblockingdialogobserver.h"
#include "mnumberentry.h"
#include "mphonesecuritymodechangeobserver.h"

#include "mphoneviewcontrollerobserver.h"

// FORWARD DECLARATIONS
class CPhoneButtonsController;
class CLogsClient;
class CPhoneView;
class CPhoneDialerView;
class TPhoneViewId;
class TPhoneCommandParam;
class CEikButtonGroupContainer;
class CPhoneMenuController;
class CPhoneNoteController;
class CPhoneQueryController;
class CPhoneKeyCaptureController;
class CSpdiaControl;
class CEikStatusPane;
class CAknNavigationControlContainer;
class CPhoneBubbleWrapper;
class CCoeControl;
class CPhoneStatusPane;
class TPhoneNoteType;
class CPhoneNoteUtilities;
class CPhoneIncallIndicator;
class CPhoneTimer;
class CPhoneContactController;
class CPhoneCntSaveAddToName;
class CPhCntSingleItemFetch;
class CPhoneApplicationExit;
class CPhoneAudioController;
class CPhoneRingingTonePlayerAO;
class RAknUiServer;
class CPhoneSingleItemFetch;
class CPhoneDialerController;
class CAknIndicatorContainer;
class CPhCntRfsHandler;
class CPhoneToolbarController;

class CEikonEnv;
class CPhoneVmbxHandler;
class CAknAppUi;
class CDialer;
class MPhoneNumberEntryChangedHandler;
class MPhoneViewCustomization;
class CPhoneEasyDialingController;
class CPhoneDtmfDialerController;
class CAknIncallBubble;

class CPhoneViewController :
    public CBase,
    public MPhoneViewCommandHandle,
    public MPhoneStatusPaneObserver,
    public MPhoneViewBlockingDialogObserver,
    public MNumberEntryObserver,
    public MPhoneSecurityModeChangeObserver,
    public MPhoneViewControllerObserver,
    public MAknsSkinChangeObserver
    {
    public:  // Constructors and destructor

        IMPORT_C static CPhoneViewController* NewL( TRect aRect );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPhoneViewController();

        IMPORT_C CPhoneView* PhoneView();

        /**
        * From MPhoneViewCommandHandle
        */
        IMPORT_C virtual void ExecuteCommandL(
            TPhoneViewCommandId aCmdId );

        /**
        * From MPhoneViewCommandHandle
        */
        IMPORT_C virtual void ExecuteCommandL(
            TPhoneViewCommandId aCmdId,
            TInt aCallId );

        /**
        * From MPhoneViewCommandHandle
        */
        IMPORT_C virtual void ExecuteCommandL(
            TPhoneViewCommandId aCmdId,
            TPhoneCommandParam* aCommandParam );

        /**
        * From MPhoneViewCommandHandle
        */
        IMPORT_C virtual void ExecuteCommandL(
            TPhoneViewCommandId aCmdId,
            TInt aCallId,
            TPhoneCommandParam* aCommandParam );

        /**
        * From MPhoneViewCommandHandle
        */
        IMPORT_C virtual void ExecuteCommandL(
            TPhoneViewCommandId aCmdId,
            TInt aCallId,
            TDesC& aMessage );

        /**
        * From MPhoneViewCommandHandle
        */
        IMPORT_C virtual void ExecuteCommand(
            TPhoneViewCommandId aCmdId );

        /**
        * From MPhoneViewCommandHandle
        */
        IMPORT_C virtual void ExecuteCommand(
            TPhoneViewCommandId aCmdId,
            TPhoneCommandParam* aCommandParam );

        /**
        * From MPhoneViewCommandHandle
        */
        IMPORT_C virtual TPhoneViewResponseId HandleCommandL(
            TPhoneViewCommandId aCmdId );

        /**
        * From MPhoneViewCommandHandle
        */
        IMPORT_C virtual TPhoneViewResponseId HandleCommandL(
            TPhoneViewCommandId aCmdId,
            TPhoneCommandParam* aCommandParam );

        /**
        * From MPhoneViewCommandHandle
        */
        IMPORT_C virtual const TDesC& FetchContent();

        /**
        * @see MPhoneStatusPaneObserver.
        */
        void HandleTitlePaneActiveL( TBool aActive = ETrue );

        /**
        * Returns window group id of the phone application.
        */
        IMPORT_C TInt ApplicationWindowGroupId();

        /**
        * Returns window group id of the Idle application.
        */
        IMPORT_C TInt IdleWindowGroupId();

        /**
        * Returns window group id of the application that is currently
        * in the foreground.
        */
        IMPORT_C TInt ForegroundApplicationWindowGroupId();

        /**
        * Fetches foreground application's window group id.
        */
        IMPORT_C static TInt DoFetchForegroundApplicationWindowGroupIdL(
            CEikonEnv& aEnv );

        /**
        * Hides / makes visible Phone icon in fast swap menu.
        * @since 2.8
        * @param aHidden is application hidden or shown.
        */
        IMPORT_C void SetHiddenL( const TBool aHidden );

        /**
        * Get the help context
        * @return Saved help context
        */
        IMPORT_C const TDesC& HelpContext();

        /**
        * From CCoeControl; notifier for resource changes
        * @param aType : Type of resource change
        */
        IMPORT_C void HandleResourceChangeL( TInt aType );

        /**
        * From CCoeControl; notifier for layout changes
        * @param aRect : screen size
        */
        IMPORT_C void HandleLayoutChange( TRect aRect );

        /**
        * Indicates that a blocking dialog is currently being displayed
        * This is required by the AppUI to determine if
        * the End key should be processed in HandleWsEventL()
        */
        IMPORT_C TBool BlockingDialogIsDisplayed() const;

        /**
        * Indicates that a status panel is currently being displayed
        * This is required by the AppUI
        */
        IMPORT_C TBool StatusPaneDisplayed();

        /**
        * Set single item fetch content
        */
        void SetFetchContent( TDesC& aFetchContent );

        /**
        * Sets single item fetch type
        * @aType   single item fetch type
        */
        void SetSingleItemFetchType( TInt aType );

         /**
        * From MPhoneViewBlockingDialogObserver
        * Sets the is blocking dialogs in screen.
        * @param aBlockingDialog ETrue if blocking dialog is
        *                        displayed.
        */
        IMPORT_C void SetBlockingDialogIsDisplayed( TBool aBlockingDialog );

        /**
         * From base class MNumberEntryObserver
         * Indication that numberentry state changed
         */
        void NumberEntryStateChanged( TBool aEntryHasText );

// From MPhoneSecurityModeChangeObserver

        IMPORT_C void HandleSecurityModeChanged( TBool aIsEnabled );

        /**
        * Sets incall small bubble´s visibility in special cases.
        */
        void AllowInCallBubbleInSpecialCases();

        /**
        * Sets value for incall bubble visibility.
        */
        void SetIncallBubbleVisibility( TBool aIncallBubbleVisible );

    private:     // New functions

        /**
         * Set security mode
         *
         * @param aMode ETrue if mode should be set enabled.
         */
        void SetSecurityMode( TBool aMode );

        /**
        * read IdleApplicationUid from PubSub
        */
        TInt IdleAppUid();

        /**
        * Indicates whether application needs to return to the
        * background after being brought to the foreground using
        * BringToForeground()
        */
        TBool GetNeedToReturnToForegroundAppAfterCallL();

        /**
        * Gets the blocking dialogs status
        */
        void GetBlockingDialogIsDisplayed( TPhoneCommandParam* aCommandParam );

        /**
        * Sets the required background status of the application
        */
        void SetNeedToReturnToForegroundAppAfterCall( TPhoneCommandParam* aCommandParam );

        /**
        * Sends application to background.
        */
        void SendToBackgroundL();

        /**
        * Brings foreground given application.
        * @param aCommandParam command parameter.
        */
        void BringAppToForeground( TPhoneCommandParam* aCommandParam );

        /**
        * Brings phone application to foreground.
        * @param aCommandParam command parameter.
        */
        void BringPhoneAppToForeground( TPhoneCommandParam* aCommandParam );

        /**
        * Sets the top application
        * @param aCommandParam command parameter
        */
        void SetTopApplicationL( TPhoneCommandParam* aCommandParam );

        /**
        * Gets the foreground application
        * @param aCommandParam command parameter
        */
        void GetForegroundApplication( TPhoneCommandParam* aCommandParam );

        /**
        * Sets phone application windowgroup position.
        */
        void SetWindowGroupPosition();

        /**
        * Activate other application.
        * @param aCommandParam command parameter
        */
        void ActivateAppL( TPhoneCommandParam* aCommandParam );

        /**
        * Activate other application with specific view.
        * @param aCommandParam command parameter
        */
        void ActivateAppViewL( TPhoneCommandParam* aCommandParam );

        /**
        * Activate other application with specific view and custom message.
        * @param aCommandParam command parameter
        */
        void ActivateAppViewWithCustomMessageL(
            TPhoneCommandParam* aCommandParam );

        /**
        * Activate other application with specific view with conventional way
        * @param aCommandParam command parameter
        */
        void ActivateAppViewConventionalL( TPhoneCommandParam* aCommandParam );

        /**
        * Launches multimedia sharing (live video sharing)
        */
        static void LaunchMultimediaSharingL();

        /**
        * Launches application.
        */
        void LaunchApplicationL();

        /**
        * Launches help application
        * @param aHelpCommand help command string
        */
        void LaunchHelpApplicationL( const TDesC& aHelpCommand );

        /**
        * Continues construction of contact manager.
        */
        static TInt DoConstructContactControllerL( TAny* aAny );

        void SetCbaL( TPhoneCommandParam* aCommandParam );

        void ShowInfoNote( TPhoneCommandParam* aCommandParam );

        /**
        * Creates connection.
        */
        void CreateConnectionL();

        /**
        * Activate another instance of the application
        */
        void ActivateAppInstanceL( TUid aUid );

        /**
        * Set the global Avkon notifier disabler status
        * @param aCommandParam a command parameter
        */
        void SetGlobalNotifiersDisabledL( TPhoneCommandParam* aCommandParam );

        /* Set the Eikon notifier disable status
        * @param aCommandParam a command parameter
        */
        void SetEikonNotifiersDisabled( TPhoneCommandParam* aCommandParam );

        /**
        * Cancel all notification from screen.
        */
        void CancelAllNotications();

        /**
        * Handles bubble updating
        */
        void HandleUpdateBubbleL(
            TInt aCallId,
            TPhoneCommandParam* aCommandParam );

        /**
        * Remove Phone dialogs
        */
        void RemovePhoneDialogsL();

        /**
        * Remove add to contact dialog.
        */
        void RemoveAddToContactDialog();

        /**
        * Update menu and CBAs based on current audio path
        * @param aCommandParam a command parameter
        */
        void UpdateAudioPathOptions( TPhoneCommandParam* aCommandParam );

        /**
        * Update menu and CBAs based on current audio availability
        * @param aCommandParam a command parameter
        */
        void UpdateAudioAvailabilityOptions( TPhoneCommandParam* aCommandParam );

        /**
        /**
        * Soft Reject feature enable status
        */
        TBool SoftRejectIsEnabled() const;

        /**
        * Set Speed Dial location
        */
        void SetSpeedDialLocation( TPhoneCommandParam* aCommandParam );

        /**
        * Get number from Speed Dial location
        * @return   EFalse if user cancel or failed.
        */
        TBool GetNumberFromSpeedDialLocationL(
            TPhoneCommandParam* aCommandParam );

        /**
        * Assign Speed Dial location
        * @param aCommandParam a command parameter
        * @return ETrue if speed dial location is assigned
        */
        TBool AssignSpeedDialLocation( TPhoneCommandParam* aCommandParam );

        /**
        * Assign Speed Dial location
        * @param aLocation a speed dial location
        * @return ETrue if speed dial location is assigned
        */
        TBool AssignSpeedDialLocationL( TInt aLocation );

        /**
        * Opens Contact and adds address to contacts.
        * @param aCommand a contact command
        * @param aString a string to be added to contacts, if KNullDesC string
        *                will be added from the number entry.
        */
        void OpenAddToContactL( TInt aCommand,
                                const TDesC& aString = KNullDesC );

        /**
        * Launches Restore Factory Settings
        * @param aDeepRfs - ETrue if deep rfs, EFalse if normal
        */
        void LaunchRfsL( TBool aDeepRfs );

        /**
        * Open the SMS or MMS message editor
        */
        void OpenMessageEditorL( TPhoneCommandParam* aCommandParam );

        /**
        * Open the soft reject message editor
        */
        void OpenSoftRejectMessageEditorL(TPhoneCommandParam* aCommandParam );

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL( TRect aRect );

        /**
        * C++ default constructor.
        */
        CPhoneViewController();

        /**
        * Fetches phone number's type for aSpeedDialLocation located number.
        * @param aSpeedDialLocation - speed dial location of number.
        * @return Phone number's type.
        */
        TPhoneNumberType SpeedDialNumberTypeL( TInt aSpeedDialLocation );

        /**
        * Convert number type from Speed Dial field info
        * @param  aFieldInfo - field info
        * @return Phone number's type
        */
        TPhoneNumberType ConvertNumberTypeFromSpeedDialFieldInfo(
            const CPhCntSpeedDialMonitor::TSpdDialFieldInfo& aFieldInfo  );

        /**
        * Open the Unified message editor and get
        * number from number entry
        */
        void SendMessageL();

        /**
        * Set security mode to view and statuspane
        */
        void SetStatusPaneVisible( TPhoneCommandParam* aCommandParam );

        /**
        * Reads information is security code query displayed on the screen.
        * @return ETrue if is displayed, otherwise EFalse
        */
        TBool IsSecurityCodeQuery() const;

        /**
        * Set update of FSW
        */
        void UpdateFSW();

        /**

        /**
        * Creates Speed Dial Controller if it doesn't exist already.
        */
        void CreateSpeedDialControllerL();

        /**
        * Checks replace option.
        * @return ETrue if active and waiting calls are both video or voice
        */
        TBool IsReplaceAllowed();

        /**
        * Open single item fecth dialod for Dialer.
        */
        void SingleItemFetchL();

        /**
        * Get Soft Reject Window Group Id.
        */
        void GetSoftRejectWindowGroupId( TPhoneCommandParam* aCommandParam );

        /**
        * Switch flat status pane.
        */
        void SwitchLayoutToFlatStatusPaneL( TBool aSwitch );

        /**
        * Swap empty indicator pane.
        */
        void SwapEmptyIndicatorPaneL( const TBool aSwapEmpty );

        /**
        * Handles transition effect.
        */
        void HandleTransitionEffect( TPhoneTransEffectType aType,
                const TUid& aAppUidEffectParam = KUidPhoneApplication );

        /**
        * Sets control to dialer.
        */
        void SetControltoDialerL();

        /**
        * Sets control to call handling.
        */
        void SetControltoCallHandlingL();

        /**
        * Sets control to dialer and makes control
        * visible if needed.
        */
        void SetDialerControlAndVisibilityL();

        /**
        * Sets pointer capture.
        * This is needed when statuspane must be inactive.
        */
        void CapturePointerEvents( TPhoneCommandParam* aCommandParam );

        /**
        * Returns allow waiting call header value.
        */
        void AllowWaitingCallHeaderL( TPhoneCommandParam* aCommandParam );

        /**
        * Checks if given window group id still exists in
        * window group list.
        *
        * @aAppWgID     window group id to check
        * @return TInt  KErrNotFound if app not exists anymore in the list
        *                   otherwise app index in the list.
        */
        TInt FindAppByWgIDL( TInt aAppWgID ) const;

       /**
        * Returns single item fetch type
        * @return single item fetch type
        */
        TInt SingleItemFetchType() const;

        /**
         * Checks if DTMF dialer mode is currently active
         */
        TBool IsDtmfDialerActive() const;

        /**
         * Checks if custom dialer mode is currently active
         */
        TBool IsCustomDialerActive() const;

        /**
        * Checks if the effect could and should be triggered.
        */
        TBool CanTransEffectBeUsed( TPhoneTransEffectType aType );

        /**
        * Triggers application start effect for the application
        * that is going to be lauched from dialer.
        * @param aAppUid app that is launched
        * EndTransEffect() must be called after app is launched.
        */
        void BeginTransEffectForAppStartFromDialerLC( const TUid& aAppUid );

        /**
        * Complete transition effect.
        */
        void EndTransEffect();

        /**
         * TCleanupOperation to call EndTransEffect, if leave occurs
         * after BeginTransEffectForAppStartFromDialerLC.
         */
        static void EffectCleanup(TAny* aThis );

        /**
         * Checks if emergency call is ongoing.
         */
        TBool IsEmergencyCallOngoing();
        
        /**
        * Is it ok to aType effect inside phoneapp.
        */
        TBool IsOkToUseThisTypeOfEffectInsidePhoneApp( 
                TPhoneTransEffectType aType );

    private:
        /**
         * Called by the skin server when skin content is changed and the
         * connected client wants to be informed.
         *
         * @see MAknsSkinChangeObserver
         */
         void SkinContentChanged();

         /**
         * Called by the skin server to indicate that the current skin
         * configuration has changed.
         *
         * @see MAknsSkinChangeObserver
         */
         void SkinConfigurationChanged(
             const TAknsSkinStatusConfigurationChangeReason aReason );

         /**
         * Called by the skin server to indicate that the available
         * skin packages have changed.
         *
         * @see MAknsSkinChangeObserver
         */
         void SkinPackageChanged(
             const TAknsSkinStatusPackageChangeReason aReason );

         /**
         * Sets orientation.
         */
         void SetOrientationL();

         void CreateDialerAndSetItActiveControlL();
         
         TInt GetDialerCBAResource() const;

         /**
         * Returns orientation for call ui view.
         */
         CAknAppUi::TAppUiOrientation CallUiOrientation();
		 
		 TBool IsPhoneAppInForeground();
         
         TBool IsDialerActiveControlAndInForeground();
         
         void SetDialerAsActiveControlInPhoneAppL();
         
    private:    // Data
         
        CPhoneView* iPhoneView;

        CPhoneDialerView* iDialerView;

        // iCba
        CEikButtonGroupContainer* iCba;

        CPhoneStatusPane* iStatusPane;

        // For accessing bubble
        CPhoneBubbleWrapper* iBubbleWrapper;

        // Controls the touch buttons
        CPhoneButtonsController* iButtonsController;

        // Controls the touch buttons
        CPhoneDialerController* iDialerController;

        // Controls touch dialer in DTMF mode
        CPhoneDtmfDialerController* iDtmfDialerController;

        // Controls the toolbar
        CPhoneToolbarController* iToolbarController;

        // Handles easydialing commands. Owned.
        CPhoneEasyDialingController* iEasyDialingController;

        // For accessing incall indicator
        CPhoneIncallIndicator* iIncallIndicator;

        CPhoneMenuController* iMenuController;

        CPhoneNoteController* iNoteController;

        CPhoneQueryController* iQueryController;

        CPhoneKeyCaptureController* iKeyCaptureController;

        CPhoneApplicationExit* iApplicationExit;

        CPhoneAudioController* iAudioController;

        CPhoneRingingTonePlayerAO* iRingingTonePlayerAO;

        // EikEnv for CPhoneKeyCaptureController, CPhoneNoteController,
        // CPhoneMenuController and CPhoneQueryController
        CEikonEnv& iEikEnv;

        // Instance of keylock.
        RAknKeyLock iKeyLock;

        CPhoneContactController* iContactController;

        // Window group of Idle application.
        TInt iIdleWg;

        // Boolean flag. ETrue if the application needs to return
        // to the background
        TBool iNeedToSendToBackground;

        // Owned server session.
        RApaLsSession iApaLsSession;

        // Saved help context name retrieved indirectly from AppUI when
        // the help application is launched
        TPtrC iHelpCommand;

        // Recovery id for contact manager construction.
        TRecoveryId iContactControllerId;

        // Boolean flag. ETrue if a blocking dialog is currently being
        // displayed.
        TBool iBlockingDialogIsDisplayed;

        //Pointer to Akn UI server, not owned.
        RAknUiServer* iAknUiServerClient;

        //Uid of idle application
        TInt iIdleUid;

        // Voice mailbox handler
        CPhoneVmbxHandler* iVmbxHandler;

        // Reference to speed dialling monitor.
        CPhCntSpeedDialMonitor* iSpeedDialController;


        // Dialer - owned
        CDialer* iDialer;

        // Pointer to the CPhoneSingleItemFetch
        CPhoneSingleItemFetch* iSingleItemFetch;

        // Content of the SingleItemFetch
        TBuf<KPhoneNumberEntryBufferSize> iFetchContent;

        // Type of the SingleItemFetch
        TInt iSingleItemFetchType;

        // Pointer to CPhoneCntSaveAddToName.
        CPhoneCntSaveAddToName* iContactSaveAddToName;

        // Previous application Window group id.
        TInt iPrevForegroundAppWg;

        CAknAppUi* iAppui;

        // Pointer to view customization.
        MPhoneViewCustomization* iCustomization;

        // Status if there is transition effect ongoing
        TBool iEffectOngoing;

        // Send back flag indicates send back need if it is
        // EFalse phone was foregound and there is no need to send it
        // background. Otherwise ETrue.
        TBool iSendBack;

        // Dialer status flag.
        TBool iDialerActive;

        CCoeControl* iPreviousIndicatorControl;

        // Empty indicator pane - Owned.
        CAknIndicatorContainer* iIndiContainer;

        // Owned
        CPhCntRfsHandler* iRfsHandler;

        // Dialer send key flag.
        TBool iDialerSendKeyActivation;

        TInt iPrevious;

        TBool iPriotityChanged;

        /**
         * Internal flag to define if security mode is enabled.
         */
        TBool iSecurityMode;

        // Boolean flag. ETrue if the application needs to return
        // to the foreground after call ended
        TBool iNeedToReturnToForegroundAppAfterCall;

        CAknIncallBubble* iIncallBubble;

        // Server wich sends events about background image changes
        RAknsSrvSession iSkinServerSession;
        
        // Qwerty mode flag
        TBool iQwertyMode;
        
        // Call Ui orietation
        CAknAppUi::TAppUiOrientation iCallUiOrientation;
        
        // Call Ui orietation flag
        TBool iLockCallUiInCurrentOrientation;
    };

#endif // CPHONEVIEWCONTROLLER_H
