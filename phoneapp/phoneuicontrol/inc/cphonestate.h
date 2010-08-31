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
* Description:
*     A base class for an object implementing the state.
*
*/


#ifndef CPHONESTATE_H
#define CPHONESTATE_H

// INCLUDES
#include <w32std.h>
#include <remconcoreapitargetobserver.h>
#include <MProfileEngine.h>
#include "mphonestate.h"
#include "mphoneviewcommandhandle.h"
#include "cphonecbamanager.h"

// FORWARD DECLARATIONS
class MPhoneState;
class MPhoneStateMachine;
class CPhoneCallHeaderManager;
class CPhoneTimer;
class TPhoneCmdParamCallHeaderData;
class MPhoneCustomization;
class CPhoneNumberEntryManager;

enum TStateTransEffectType
    {
    ENoneType,
    ENumberEntryOpen,
    ENumberEntryClose,
    ENumberEntryCreate
    };

// CLASS DECLARATION

/**
*  An abstract class for an object implementing the state.
*
*/
class CPhoneState :
    public CBase,
    public MPhoneState
    {
    public:

        /**
        * Constructor
        */
        IMPORT_C CPhoneState(
            MPhoneStateMachine* aStateMachine,
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aCustomization);

        /**
        * Creates CbaManager instance.
        */
        IMPORT_C void BaseConstructL();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPhoneState();

        /**
        * A message handling function for Phone Engine messages
        * @param aMessage Message from Phone Engine
        * @param aCallId Call id the message concerns
        */
        IMPORT_C virtual void HandlePhoneEngineMessageL(
            const TInt aMessage,
            TInt aCallId );

        /**
        * HandleError
        * Implements error handling framework
        * @param aErrorInfo: the error info
        */
        IMPORT_C virtual void HandleErrorL(
            const TPEErrorInfo& aErrorInfo );

        /**
        * Handle processed (short, long) key messages from the key event
        * handler
        * @param aMessage a key message
        * @param aCode key event code
        */
        IMPORT_C virtual void HandleKeyMessageL(
            TPhoneKeyEventMessages aMessage,
            TKeyCode aCode );

        /**
        * Handles raw key events from the key event handler
        * @param aKeyEvent a key event
        * @param aEventCode key event code
        */
        IMPORT_C virtual void HandleKeyEventL(
            const TKeyEvent& aKeyEvent,
            TEventCode aEventCode );

        /**
        * Handles key press duration events from the key event handler
        * @param aCode key event code
        * @param aKeyPressDuration key press duration
        */
        IMPORT_C virtual void HandleKeyPressDurationL(
            TKeyCode aCode,
            TTimeIntervalMicroSeconds aKeyPressDuration );

        /**
        * From CEikAppUi. For Idle indicator
        */
        IMPORT_C virtual void HandleSystemEventL(
            const TWsEvent& aEvent );

        /**
        * From CAknAppUi, indicates when app ui is on the foreground.
        * @param aForeground It is true if app is on the foreground.
        */
        IMPORT_C virtual void HandleForegroundEventL( TBool aForeground );

        /**
        * Indicates when the Phone app is in the foreground.
        */
        IMPORT_C virtual void HandlePhoneForegroundEventL();

        /**
        * Indicates when the Phone app is losing focus.
        */
        IMPORT_C virtual void HandlePhoneFocusLostEventL();

        /**
        * Indicates when the Idle app is in the foreground.
        */
        IMPORT_C virtual void HandleIdleForegroundEventL();

        /**
        * Indicates when the keylock events
        */
        IMPORT_C virtual void HandleKeyLockEnabled( TBool aKeylockEnabled );
        
        /**
        * Handle environment changes.
        * @param aChanges environment changes which may be reported by
        *  a change notifier through the RChangeNotifier interface.
        */
        IMPORT_C void HandleEnvironmentChangeL( const TInt aChanges );

        /**
        * Handles startup of the phone application
        */
        IMPORT_C virtual void HandlePhoneStartupL();

        /**
        * This function is called when there is property value change.
        * @param aCategory Category of the property
        * @param aKey Property key that is changed
        * @param aValue New property value
        */
        IMPORT_C virtual void HandlePropertyChangedL(
            const TUid& aCategory,
            const TUint aKey,
            const TInt aValue );

        /**
        * Handle the change of the setting from Central Repository
        * @param aUid identifing the central repository UID.
        * @param aId central repository ID.
        */
        IMPORT_C virtual void HandleCenRepChangeL(
            const TUid& aUid,
            const TUint aId );

        /**
        * Handles commands.
        * @param aCommand It is the code of the command to be handled.
        * @returns boolean value was the command handled by the state
        *          (ETrue) or not (EFalse)
        */
        IMPORT_C virtual TBool HandleCommandL( TInt aCommand );

        /**
        * Processes command.
        * @param aCommand It is the code of the command to be handled.
        * @returns boolean value was the command handled by the state
        *          (ETrue) or not (EFalse)
        */
        IMPORT_C virtual TBool ProcessCommandL( TInt aCommand );

        /**
        * Handles commands from the Remote Control framework.
        * @param aOperationId The operation ID of the command.
        * @param aButtonAct The button action associated with the command.
        * @return ETrue if the event was handled, EFalse otherwise.
        */
        IMPORT_C virtual TBool HandleRemConCommandL(
            TRemConCoreApiOperationId aOperationId,
            TRemConCoreApiButtonAction aButtonAct);

        /**
        * Setter for divert indication showing in bubble.
        * @param aDivertIndication ETrue to show divert indication,
        *          EFalse to not. Usually setting EFalse isn't necessary
        *         as it's a default value in bubble creation.
        */
        IMPORT_C virtual void SetDivertIndication(
            const TBool aDivertIndication );

        /**
        * Handles Long hash key press
        */
        IMPORT_C void HandleLongHashL();

        /**
        * Checks whether customized dialer view is active,
        * @return ETrue if customized dialer is active
        */
        IMPORT_C TBool IsCustomizedDialerVisibleL() const;

        /**
         * Plays DTMF tone for key event
         * */
        IMPORT_C void HandleDtmfKeyToneL( const TKeyEvent& aKeyEvent,
                TEventCode aEventCode );

        /**
        * Show global InfoNote
        * @param aResourceId resource id to be resolved
        * @param aNotificationDialog True if notification
        *        dialog should be used.
        */
        IMPORT_C void SendGlobalInfoNoteL( TInt aResourceId,
                TBool aNotificationDialog = EFalse );

        /**
        * Show global ErrorNote
        * @param aResourceId resource id to be resolved
        * @param aNotificationDialog True if notification
        *        dialog should be used.
        */
        IMPORT_C void SendGlobalErrorNoteL( TInt aResourceId,
                TBool aNotificationDialog = EFalse );

        /**
         * Get blocked key list
         * @returns RArray<TInt>& lsit of blocked keys 
         */
        IMPORT_C const RArray<TInt>& GetBlockedKeyList() const;

        /**
         * Disable HW Keys if needed
         */
        IMPORT_C void DisableHWKeysL();

        /**
         * Disable Call UI
         */
        IMPORT_C void DisableCallUIL();

        /**
         * Enable Call UI
         */
        IMPORT_C void EnableCallUIL();

        /**
         * Disable HW Keys and Call UI if needed
         */
        IMPORT_C void CheckDisableHWKeysAndCallUIL();

        /**
         * Handle hold switch key event when there is an incoming or waiting call
         */
        IMPORT_C void HandleHoldSwitchL();
        
        /**
         * Enable HW Keys and Call UI if needed
         */
        IMPORT_C void CheckEnableHWKeysAndCallUIL();
        
        /**
        * Fetches autolock information - is it set on or not
        * @return is autolock set on (ETrue) or off (EFalse)
        */
        IMPORT_C TBool IsAutoLockOn() const;

        /**
        * Return is sim state ok
        * @return ETrue if is sim ok, otherwise EFalse
        **/
        IMPORT_C TBool IsSimOk();

        /**
        * Checks Svivel state.
        * @return ETrue if swivel is closed.
        */
        IMPORT_C TBool IsSwivelClosed() const;

        /**
        * Check if note, query or blockingDialog is visible
        * @return boolean value indicating that note, query or
        * blocking dialog is visible
        */
        IMPORT_C TBool IsAnyQueryActiveL();

        /*
        * If KFeatureIdFfSimlessOfflineSupport is undefined and
        * UI shows note which will be closed by key event then
        * method return ETrue otherwise EFalse.
        */
        TBool IsNoteDismissableL();

        IMPORT_C void RetainPreviousKeylockStateL();
        
    public: // NumberEntry functions.

        /**
        * Check if number entry is used
        * @return boolean value indicating that number entry is used
        */
        IMPORT_C virtual TBool IsNumberEntryUsedL();

        /**
        * Check if number entry is visible
        * @return boolean value indicating that number entry is visible
        */
        IMPORT_C TBool IsNumberEntryVisibleL();

    protected:

        /**
        * Handle EPEMessageAudioMuteChanged
        */
        IMPORT_C void HandleAudioMuteChangedL();

        /**
        * Handle EPEMessageAudioOutputChanged
        */
        IMPORT_C void HandleAudioOutputChangedL();

        /**
        * Handle SideVolumeKeyInc
        */
        IMPORT_C void IncreaseAudioVolumeL();

        /**
        * Handle SideVolumeKeyDec
        */
        IMPORT_C void DecreaseAudioVolumeL();

        /**
        * Dial CS multimedia call
        */
        IMPORT_C void DialMultimediaCallL();

        /**
        * Dial CS voice call
        */
        IMPORT_C void DialVoiceCallL();

        /**
        * Disconnect call
        */
        IMPORT_C TBool DisconnectCallL();

        /**
        * Display idle screen
        */
        IMPORT_C void DisplayIdleScreenL();

        /**
        * Set up the Idle screen in the background for the cases
        * where the phone is no longer the top application (eg. end
        * call when an external app is being displayed)
        */
        IMPORT_C void SetupIdleScreenInBackgroundL();

        /**
        * Display call header for call coming in ( the remote info data
        * and picture is displayed immediately )
        * @param aCallid call id
        * @param aWaitingCall waiting call indication
        */
        IMPORT_C void DisplayHeaderForCallComingInL(
            TInt aCallId,
            TInt aWaitingCall );

        /**
        * Sets call header texts for call coming in
        * @param aCallid call id
        * @param aWaitingCall waiting call indication
        * @param aCallHeaderData - Call header parameter into which the text
        *                          will be set.
        */
        IMPORT_C void SetCallHeaderTextsForCallComingInL(
            TInt aCallId,
            TBool aWaitingCall,
            TPhoneCmdParamCallHeaderData* aCallHeaderData );

        /**
        * Display call header for outgoing call (the phone number is initially
        * displayed)
        * @param aCallid call id
        */
        IMPORT_C void DisplayHeaderForOutgoingCallL( TInt aCallId );

        /**
        * Update Single Active Call
        * @param aCallid call id
        */
        IMPORT_C void UpdateSingleActiveCallL( TInt aCallId );

        /**
        * Update remote information data in the call header
        * @param aCallid call id
        */
        IMPORT_C void UpdateRemoteInfoDataL( TInt aCallId );

        /**
        * Return remote info data
        * @param aCallid call id
        * @param aData the returned remote info data
        */
        IMPORT_C void GetRemoteInfoDataL( TInt aCallId, TDes& aData );

        /**
        * Show note
        * @param aResourceId resource id to be resolved
        */
        IMPORT_C void ShowNoteL( TInt aResourceId );

        /**
        * Show query
        * @param aResourceId resource id to be resolved
        */
        IMPORT_C void ShowQueryL( TInt aResourceId );

        /**
        * Show text query
        * @param aDialogResourceId dialog's resource id
        * @param aDefaultCbaResourceId default CBA's resource id
        * @param aContentCbaResourceId content CBA's resource id
        * @param aDataText pointer to data text
        * @param aSendKeyEnabled send key enabled status for the query
        */
        IMPORT_C void ShowTextQueryL(
            TInt aDialogResourceId,
            TInt aDefaultCbaResourceId,
            TInt aContentCbaResourceId,
            TDes* aDataText,
            TBool aSendKeyEnabled = EFalse );

        /**
        * Handle numeric key event
        */
        IMPORT_C virtual void HandleNumericKeyEventL(
            const TKeyEvent& aKeyEvent,
            TEventCode aEventCode );

        /**
        * Check if the application needs to be sent to the background
        * @return boolean value indicating that application needs to be
        *  sent to the background
        */
        IMPORT_C TBool NeedToSendToBackgroundL() const;

        /**
        * Check if the top application is currently displayed in the foreground
        * @return boolean value indicating that top app is displayed
        */
        IMPORT_C TBool TopAppIsDisplayedL() const;

        /**
        * Capture keys during call notifications (dialing, incoming, waiting)
        * @param aCaptured ETrue if keys are to be captured
        */
        IMPORT_C void CaptureKeysDuringCallNotificationL( TBool aCaptured );

        /**
        * Displays the call termination note, if required
        */
        IMPORT_C void DisplayCallTerminationNoteL();

        /**
        * Set the handsfree mode
        * @param aHandsfreeMode ETrue to activate IHF, EFalse to deactivate
        */
        IMPORT_C void SetHandsfreeModeL( TBool aHandsfreeMode );

        /**
        * Set the BT handsfree mode
        * @param aHandsfreeMode ETrue to activate BT, EFalse to deactivate
        */
        IMPORT_C void SetBTHandsfreeModeL( TBool aHandsfreeMode );

         /**
        * Show global WarningNote
        * @param aResourceId resource id to be resolved
        * @param aNotificationDialog True if notification dialog
        *                            should be used.
        */
        IMPORT_C void SendGlobalWarningNoteL( TInt aResourceId,
                TBool aNotificationDialog = EFalse );

        /**
        * Checks is the call id a video call.
        * @param aCallId - call id to be checked
        * @return ETrue if video call id, otherwise EFalse
        */
        IMPORT_C TBool IsVideoCall( const TInt aCallId );

        /**
        * Return SimState.
        */
        IMPORT_C TPESimState SimState() const;

        /**
        * Starts ALS line change timer.
        * This should be called after receiving long keypress
        * which is 0.8 seconds. ALS line change timer ticks
        * 2.2 seconds before callback is called.
        */
        IMPORT_C void StartAlsLineChangeTimerL();

        /**
        * Start show security note
        * This should be called in startup and idle state
        * if sim state not ok
        */
        IMPORT_C void StartShowSecurityNoteL();

        /**
        * Check if the sim card state is EPESimNotPresent with
        * the security mode is enabled.
        * @return ETrue if the sim card state is EPESimNotPresent and
        * the security mode is enabled, otherwise EFalse
        **/
        IMPORT_C TBool IsSimStateNotPresentWithSecurityModeEnabled();

        /**
        * Handle audio output availability change
        */
        void HandleAudioAvailableOutputChangedL();

        /**
        * Shows 'Number busy' note.
        */
        IMPORT_C void ShowNumberBusyNoteL();

        /**
        * Fetches keylock information - is it set on or not
        * @return is keylock set on (ETrue) or off (EFalse)
        */
        IMPORT_C TBool IsKeyLockOn() const;

        /**
        * Informs Phone Engine Sat request completion
        * @param aCallId a call id
        */
        IMPORT_C void CompleteSatRequestL( const TInt aCallId );

        /**
        * Sets the call header type used in the call bubble.
        */
        IMPORT_C void SetCallHeaderType( TInt aCallHeaderType );

        /**
        * Handles situation when hash key has been kept down long
        * (long keypress) and the hash character is the only character
        * in number entry.
        */
        IMPORT_C virtual void OnlyHashInNumberEntryL();

        /**
        * Informs view that UI is being updated (call bubble or number editor).
        * EndUiUpdate() must be called when update is done.
        */
        IMPORT_C void BeginUiUpdateLC();

        /**
        * Informs view that UI update is completed.
        */
        IMPORT_C void EndUiUpdate();

        /*
        * Checks if necessary to show call termination note
        *
        * @param aCallId call id
        * @return true if note should be shown
        */
        IMPORT_C virtual TBool CheckIfShowCallTerminationNote( );

        /*
        * Sets used touchpane buttons.
        *
        * @param touchpane resource id.
        */
        IMPORT_C void SetTouchPaneButtons( TInt aResourceId );

        /*
        * Destroys touchpane buttons.
        */
        IMPORT_C void DeleteTouchPaneButtons();

        /**
        * Set default flags values.
        * No active call
        */
        IMPORT_C void SetDefaultFlagsL();

        /**
        * Check if contact info available and
        * shown waiting note with or without caller name
        */
        IMPORT_C void CallWaitingNoteL( TInt aCallId );

        /*
        * Sets ringtone playback.
        * @param aCallId call id
        */
        IMPORT_C void SetRingingTonePlaybackL( TInt aCallId );

        /**
        * A message handling function for message EPEMessageDisconnecting
        * @param aCallId: the call id of the call
        */
        IMPORT_C void HandleDisconnectingL( TInt aCallId );
        
        /**
        * Opens soft reject message editor.
        */
        IMPORT_C virtual void OpenSoftRejectMessageEditorL();

    protected: // NumberEntry functions.

        /**
        * Set Number Entry visibility.
        * @param aVisible ETrue if numberentry is wanted to be shown
        *                 (Note ETrue will set NE CBA's)
        *                 EFalse if numberentry isnt wanted to be shown
        *                 (Note EFalse doesnt affact to CBA's)
        */
        IMPORT_C void SetNumberEntryVisibilityL( const TBool aVisible );

        /**
        * Create call if in numberentry more that 2 number and Send key
        * Send manual control sequence
        * if 1-2 number in numberentry and Send key
        */
        IMPORT_C void CallFromNumberEntryL();

        /**
         * Returns phone number from the phone number entry.
         * @return  Phone number
         */
        IMPORT_C HBufC* PhoneNumberFromEntryLC();

        /**
        * Check if number entry content is stored
        * @return boolean value indicating that number entry content is stored
        */
        IMPORT_C TBool IsNumberEntryContentStored();

        /**
        * Stores the number entry content to the cache
        */
        IMPORT_C void StoreNumberEntryContentL();

        /**
        * Clears the number entry content cache
        */
        IMPORT_C void ClearNumberEntryContentCache();

        /**
        * Restores number entry content after dtmf dialer
        */
        IMPORT_C void CheckIfRestoreNEContentAfterDtmfDialer();

        /**
         * Checks if on screen dialer feature is supported.
         */
        IMPORT_C TBool IsOnScreenDialerSupported() const;

        /**
         * Returns ETrue if alphanumeric characters are supported.
         * @param aKeyEvent Key event.
         * @return ETrue if alphanumeric chars are supported.
         */
         IMPORT_C TBool IsAlphanumericSupportedAndCharInput(
                     const TKeyEvent& aKeyEvent );

         /**
         * Handle state-specific behaviour when number entry is cleared
         */
         IMPORT_C virtual void HandleNumberEntryClearedL();

         /**
         * Internal number entry handling methods.
         */
         void NumberEntryClearL();

         /**
         * Sets toolbar loudspeaker button enabled.
         */
         IMPORT_C void SetToolbarButtonLoudspeakerEnabled();
         
         /**
         * Sets state of TitleBar Back button 
         */
         IMPORT_C void SetBackButtonActive( TBool aActive );

    protected:

       /**
       * Returns updated remote information data.
       * NOTE: This metohed is used when state receives
       * EPEMessageRemotePartyInfoChanged from PhoneEngine.
       * @param aCallId - Call Id.
       */
       TPhoneCmdParamCallHeaderData UpdateCallHeaderInfoL( TInt aCallId );

       /**
       * Checks if call is waiting, returns ETrue if call is waiting
       * otherwise EFalse.
       * @param aCallId, call id.
       */
       TBool IsWaitingCallL( const TInt aCallId );

       /**
       * Checks status of KFeatureIdSideVolumeKeys and KTelephonyLVFlagScrollVolumeKeys
       * keys. If KTelephonyLVFlagScrollVolumeKeys is false and KFeatureIdSideVolumeKeys is true
       * then side volume keys are only supported and method returns true, otherwise false.
       */
       TBool OnlySideVolumeKeySupported();

       /**
       * Checks if device has connected call ongoing.
       * List of connected call states: EPEStateConnected
       *                                EPEStateConnectedConference
       *                                EPEStateConnecting
       */
       TBool IsAnyConnectedCalls();

    private:

        /**
        * Handle EPEMessageChangedCallDuration
        * @param aCallId call id of the message
        */
        void HandleChangedCallDurationL( TInt aCallId );

        /**
        * Update profile display
        */
        void UpdateProfileDisplayL();

        /**
        * Sends key down event to the phone engine
        * @param aKeyEvent a key event
        * @param aEventCode key event code
        */
        void SendKeyPressL(
            const TKeyEvent& aKeyEvent,
            TEventCode aEventCode );

        /**
        * Sends key events to the phone engine
        * @param aKeyEvent a key event
        * @param aEventCode key event code
        */
        void SendKeyEventL(
            const TKeyEvent& aKeyEvent,
            TEventCode aEventCode );

        /**
        * Change Audio volume level
        * @param aLevel New volume level.
        * @param aUpdateControl Set ETrue if volume ui control
        *                       needs to be updated.
        */
        void ChangeAudioVolumeL( TInt aLevel,
                                 TBool aUpdateControl = ETrue );

        /**
        * Handles EPEMessageInitiatedEmergency call message from Phone Engine.
        * @param aCallId - Caller id.
        */
        void HandleInitiatedEmergencyCallL( TInt aCallId );

        /**
         *  Handles EPEMessageInitiatedEmergencyWhileActiveVideo message from
         * Phone Engine.
         */
        void HandleInitiatedEmergencyWhileActiveVideoL();

        /**
        * Handles network ciphering info changes
        */
        void HandleCallSecureStatusChangeL( TInt aCallId );

        /**
        * Handles change als line command
        */
        void ChangeAlsLineL();

        /**
        * Active call id
        */
        TInt GetActiveCallIdL();

        /**
        * Callback function for launching ALS line change dialog
        * This is called when Als line change timer timeout is
        * reached which means user has pushed the #-key long enough.
        * @param aAny - not used
        */
        static TInt AlsLineChangeTimerCallbackL( TAny* aAny );

        /**
        * Shows WLAN MAC address note
        */
        void ShowWlanMacAddressL();

        /**
        * Check if ALS line change is possible
        */
        TBool IsAlsLineChangePossible();

        /**
        * A message handling function for message EPEMessageRemoteBusy
        * @param aCallId a call id
        */
        void HandleRemoteBusyL( const TInt aCallId );

        /**
        * Creates caller information. Sets CNAP, phone number, thumbnail image
        * etc. into the call header parameter.
        * @param aCallId - Call Id.
        * @param aCallHeaderData - Call header parameter where modifications
        *                          are made.
        */
        void CreateCallerInfoL( const TInt aCallId,
            TPhoneCmdParamCallHeaderData* aCallHeaderData );

        /**
        * TCleanupOperation to call EndUiUpdate(), if leave occurs
        * after BeginUiUpdate().
        */
        static void UiUpdateCleanup(TAny* aThis );

        /**
        * Gets volume level from ui control.
        */
        TInt GetVolumeLevel();

        /**
        * Shows video call specific out of memory note.
        */
        void ShowVideoCallOutOfMemoryNoteL();

        /**
        * Returns sim security status:
        * Rejected or UnAccepted.
        */
        TInt SimSecurityStatus() const;

        /**
        * Show CUG in use note
        */
        void HandleCugInUseNoteL();

        /**
         * Handles changes in the SIM card state.
         */
        void HandleSimStateChangedL();

        /**
         * Checks if key events are allowed to be redirected for further handling
         */
        TBool IsKeyEventFurtherProcessedL( const TKeyEvent& aKeyEvent ) const;

        /*
         * Checks is given key contains numeric charaters or if customization is used
         * alphanumeir letters
        */
        TBool IsValidAlphaNumericKey( const TKeyEvent& aKeyEvent,
                TEventCode aEventCode );

        /*
         * Updates CBA using either customization or resource resolver
         */
        void CustomizeCbaForPhoneNumberL();

        /*
         * Checks if Touch dialer is on in DTMF mode.
         */
        TBool IsTouchDTmfDialerOn() const;

        /*
         * Checks if keyevent is from dtmf key and sends it to phone-engine
         */
        void SendDtmfKeyEventL( const TKeyEvent& aKeyEvent,
                TEventCode aEventCode  );

        /*
         * Creates call header manager if needed.
        */
        CPhoneCallHeaderManager* CallheaderManagerL();

        /*
        * Creates number entry content if needed.
        */
        CPhoneNumberEntryManager* NumberEntryManagerL();

        /*
        * Updated Cba when Swivel state changes.
        */
        void UpdateCbaSwivelStateChangedL();

        /**
        * Loads data commonengine stringloader.
        * @param aData - The returned data including needed text.
        * @param aResourceId - Resource id for text.
        */
        void LoadResource( TDes& aData, const TInt aResource ) const;


    protected:  // Data

        /**
        * Reference to State machine
        */
        MPhoneStateMachine* iStateMachine;

        /**
        * Reference to the Phone UI View command handle
        */
        MPhoneViewCommandHandle* iViewCommandHandle;

    protected:

        //Indicates whether onscreen dialer is defined
        TBool iOnScreenDialer;

        /**
        * Customization of functionality according to e.g.
        * call type specific needs.
        * Not own.
        */
        MPhoneCustomization* iCustomization;

        // CBA key manager. Own.
        CPhoneCbaManager* iCbaManager;

    private:

        /**
         * Previously handled SIM card state.
         */
        TPESimState iPreviousSimState;

        // Bitmap redraw counter
        TInt iBitmapRedrawCounter;

        /**
        * Timer for ALS line change.
        */
        CPhoneTimer* iAlsLineChangeKeyPressTimer;

        // Internal variable for EikonEnv to avoid
        // use of static system calls
        CEikonEnv& iEnv;

        // Call header manager.
        CPhoneCallHeaderManager* iCallHeaderManager;

        // Number entry manager
        CPhoneNumberEntryManager* iNumberEntryManager;

        // RingtoneSilenced flag.
        // This is used to determine if the ringtone
        // for incoming call is silenced. The information
        // is used while updating softkeys in method
        // UpdateIncomingCbaL only
        TBool iRingtoneSilenced;

        /**
         * Handle to the Operator logo resource.
         */
        TInt iLogoHandle;
        };

#endif // CPHONESTATE_H

// End of File
