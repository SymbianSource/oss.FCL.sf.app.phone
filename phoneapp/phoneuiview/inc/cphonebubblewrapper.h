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
* Description:  PhoneUI Bubble Wrapper
*
*/

#ifndef CPHONEBUBBLEWRAPPER_H
#define CPHONEBUBBLEWRAPPER_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <coecntrl.h>
#include <eikimage.h>
#include <pevirtualengine.h>
#include <bmbubblemanager.h>
#include "mphonevideoplayer.h"
#include "bmvideoplaybackobserver.h"
#include "mphoneviewcommandhandle.h"
#include "cphonecallthememonitor.h"
#include "mphonenumberentrychangedhandler.h"

// FORWARD DECLARATIONS
class CBubbleManager;
class CPhoneBubbleMapping;
class CPhoneNumberEntry;
class TPhoneCommandParam;
class TPhoneCmdParamCallHeaderData;
class CPhoneCallHeaderTextAnimation;

class MNumberEntryObserver;
class MNumberEntry;

class CPhoneBubbleExtensionManager;
class MPhoneCustomization;

class CPhoneBubbleWrapper :
    public CBase,
    public MPhoneVideoPlayer,
    private MBubbleVideoPlaybackObserver,
    public MPhoneCallThemeSettingsObserver,
    public MPhoneNumberEntryChangedHandler,
    public MCoeControlObserver
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneBubbleWrapper* NewL(
            CCoeControl* aControl,
            const TRect& aRect );

        /**
        * Destructor.
        */
        virtual ~CPhoneBubbleWrapper();

    public: // Call Header functions

        void RemoveCallHeaderL( TInt aCallId );

        CBubbleManager::TBubbleId CreateCallHeaderL(
            TInt aCallId,
            TPhoneCommandParam* aCommandParam );

        CBubbleManager::TBubbleId CreateEmergencyCallHeaderL(
            TInt aCallId,
            TPhoneCommandParam* aCommandParam );

        void UpdateCallHeaderDisplay(
            TInt aCallId,
            TPhoneCommandParam* aCommandParam );

        void UpdateCallHeaderAndLabel(
            TInt aCallId,
            TPhoneCommandParam* aCommandParam );

    public: // Thumbnail functions

    public: // Number Entry functions

        void CreateNumberEntry();

        void SetNumberEntryObserver( MNumberEntryObserver& aObserver  );

        void SetNumberEntryPromptText( const TDesC& aPromptText );

        void RemoveNumberEntry();

        void SetNumberEntryContent( const TDesC& aContent );

        void GetNumberEntryContent( TPhoneCommandParam* aCommandParam );

        void GetLocalizedNumberEntryContent( TPhoneCommandParam* aCommandParam );

        void SetNumberEntryVisible( TPhoneCommandParam* aCommandParam );

        void GetNumberEntryCount( TPhoneCommandParam* aCommandParam );

        TBool IsNumberEntryUsed();

        TBool IsNumberEntryVisible();

        /**
        * Count amount of characters in number entry.
        * @return amount of chracters.
        */
        TInt CountNumberEntryCharacters();

        // from base class MPhoneNumberEntryChangedHandler
        
        /**
         * From MPhoneNumberEntryChangedHandler
         * 
         * Contains implementation about what is done when Phone Number Entry
         *  has informed that it's state has changed  
         *  
         * @since S60 5.0
         */
        void HandleNumberEntryChanged( );
        
        /**
         * From MPhoneNumberEntryChangedHandler
         * 
         * Sets callback function to observer that want to know about the 
         * number entry changes. 
         *
         * @since S60 5.0 
         */
        void SetNumberEntryChangedCallBack( TPhoneCommandParam* aCommandParam );
        
        /**
         * From MCoeControlObserver
         * 
         * Bubble wrapper receives events from bubble number editor
         */
        void HandleControlEventL( CCoeControl* aControl,TCoeEvent aEventType );
        
        
    public: // New functions

        /**
        * Handles key event.
        *
        * @param aCommandParam a command parameter
        * @return Indicates if event was handled.
        */
        TKeyResponse HandleKeyEventL( TPhoneCommandParam* aCommandParam );

        /**
        * Set the phone muted icon.
        * @param aCommandParam a command parameter
        */
        void SetPhoneMuted( TPhoneCommandParam* aCommandParam );

        /**
        * Prepare the call state
        * @param aCallId call id
        * @param aState call state
        * @param aStateLabelText label associated with the call state
        * @param aStateShortLabelText short label associated with the call state
        */
        void PrepareCallStateL(
            TInt aCallId,
            TPEState aState,
            const TDesC& aStateLabelText,
            const TDesC& aStateShortLabelText );

        /**
        * Update the call duration
        * @param aCallId call id
        */
        void UpdateCallDuration( TInt aCallId, const TDesC& aDurationText );

        /**
        * Give selected conf member callId
        * @param  aCommandParam
        */
        TBool SelectedConfMemberCallIdL( TPhoneCommandParam* aCommandParam );

        /**
        * Give selected conf member callId
        * @param  aCommandParam
        */
        void OpenConferenceList( TPhoneCommandParam* aCommandParam );

        /**
        * Move Highlight in conference up/down
        * @param  aCommandParam
        */
        void MoveHighlightInConference( TPhoneCommandParam* aCommandParam );

        /**
        * Give callState by callId
        * @param  aCommandParam
        */
        TInt GetCallState( TPhoneCommandParam* aCommandParam );

        /**
        * Give callId by callState
        * @param  aCommandParam
        */
        TInt GetCallIdByState( TPhoneCommandParam* aCommandParam );

        /**
        * Give number of active calls
        * @param  aCommandParam
        */
        void GetActiveCallsCount( TPhoneCommandParam* aCommandParam );

        /**
        * Give is conference active or not
        * @param  aCommandParam
        */
        void GetIsConference( TPhoneCommandParam* aCommandParam );

        /**
        * Give is this callId member of conference or not
        * @param    aCallId, callId
        *           aCommandParam
        */
        void CallExistsInConference(
            TInt aCallId,
            TPhoneCommandParam* aCommandParam );
        /**
        * Removes all callHeaders
        */
        void RemoveAllCallHeadersL();

        /**
        * Updates ciphering indicator in the bubble if necessary.
        * @param aCallId Call id.
        * @param aCommandParam Command parameter.
        */
        void UpdateCipheringIndicator(
            TInt aCallId,
            TPhoneCommandParam* aCommandParam );

        /**
        * Give is max members in conference
        */
        TBool IsMaxMembersInConf() const;

        /**
        * Toggle number entry mode
        * @param aCommandparam ETrue if new mode is alpha
        */
        void ToggleNumberEntryAlphaNumericMode( TPhoneCommandParam* aCommandParam );

        /**
        * Opens VKB
        * 
        */
        void OpenVkbL();
        
        /**
        * Get number entry cursor position
        * @param aCommandparam cursor position
        */
        void GetNumberEntryCursorPosition( TPhoneCommandParam* aCommandParam );

        /**
        * Set number entry cursor position
        * @param aCommandparam cursor position
        */
        void SetNumberEntryCursorPositionL( TPhoneCommandParam* aCommandParam );
           
        /**
        * Get number entry mode
        */
        TBool IsNENumericMode() const;

        /**
        * Before making any changes to bubble headers, call this function.
        */
        void StartChanges();

        /**
        * After the changes to bubble headers call this function.
        */
        void EndChanges();

        /**
        * Gives access to button pane.
        */
        MBubbleTouchPaneInterface& TouchPane() const;

        /**
        * Sets touchpane visible.
        * @param aVisible ETrue if touch is to be shown
        *        otherwise EFalse.
        */
        void SetTouchPaneVisible( TBool aVisible );

        /**
        * Gives access to bubblemanager's interface.
        */
        CBubbleManager& BubbleManager();

        /**
        * Returns BubbleManager's CCoeControl interface.
        */
        CCoeControl* Control();

        /**
        * Checks whether conference is in expanded mode
        */
        TBool IsConferenceInExpandedMode() const;

        /**
         * Loads the extension plugins.
         */
        void LoadPlugins();

        /**
         * Sets phone customization interface
         */
        void SetPhoneCustomization( MPhoneCustomization* aCustomization );
    public: // Dialer specific methods

        /**
        * Sets number entry
        */
        void SetNumberEntry( MNumberEntry* aNumberEntry );

        /**
        * Forward commands to Dialer
        * @param aCommand Dialer command
        */
        void HandleCommandL( TInt aCommand);

        /**
        * Keypad audio enabled
        *
        */
        void KeypadAudioEnabled();

        /**
        * Keypad audio disabled
        *
        */
        void KeypadAudioDisabled();

    public: // DO NOT CALL THE FOLLOWING BEFORE STARTCHANGES HAS BEEN CALLED.

        /**
        * Creates new bubble for call. If bubble for the
        * call already exists, then it is returned.
        *
        * @param aCallId callId.
        * @return bubble id.
        */
        CBubbleManager::TBubbleId CreateBubbleL(
            TInt aCallId );

        /**
        * Changes CLI of the bubble.
        *
        * @param aId bubble id.
        * @param aText text, new CLI.
        * @param aDirection clipping direction.
        */
        void SetCLI(
            CBubbleManager::TBubbleId aId,
            const TDesC& aText,
            CBubbleManager::TPhoneClippingDirection aDirection );

        /**
        * Changes Cnap of the bubble, clipping done from the beginning
        * Related to __SERIES60_PHONE_CNAP flag
        *
        * @param aId bubble id.
        * @param aCnapText text, new Cnap
        * @param aDirection clipping direction.
        */
        void SetCNAP( 
            CBubbleManager::TBubbleId aId,
            const TDesC& aCnapText,
            CBubbleManager::TPhoneClippingDirection aDirection );

        /**
        * Set the icon that represents the call number type in the current
        * call bubble.
        * @param aBubbleId bubble id.
        * @param aNumberType number type.
        */
        void SetNumberTypeIndicator(
            CBubbleManager::TBubbleId aId,
            TPEPhoneNumberIdType aNumberType  );

        /**
        * Creates a new conference call.
        * @param TInt aCallId .
        */
        void CreateConferenceL(
            TInt aCallId,
            TPhoneCommandParam *aCommandParam );

        /**
        * Removes conference call
        */
        void RemoveConferenceL();

        /**
        * Removes conference call bubble
        */
        void RemoveConferenceBubbleL();

        /**
        * Removes party from conference call.
        * @param TInt aCallId .
        */
        void RemoveFromConferenceL(TInt aCallId);

        /**
        * Finds a participant position in conference call participant list.
        *
        * @param aCallId call id to search for
        * @return position in participant list or KErrNotFound
        */
        TInt FindParticipantPosition( TInt aCallId );

        /**
        * Set conference call header expanded
        */
        void SetConferenceExpand( TBool aStatus );

        /**
        * Set conference call header highlighted
        */
        void SetConferenceHighlight( TBool aStatus );

        /**
        * Handles flag changes.
        */
        void HandleFlagsUpdated( TInt aNewFlags );

        /**
        * Drop party from conference call
        * to private call.
        * @param TInt aCallId .
        */
        void ConferenceMemberToPrivateL( TInt aCalld );

        /**
        * Adds party to conference call.
        */
        void AddToConferenceL();


    protected:  // Constructors

        /**
        * C++ default constructor.
        */
        CPhoneBubbleWrapper(  );

    protected:

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL( CCoeControl* aControl, const TRect& aRect );

        /**
        * convert TPEState to TPhoneCallState
        */
        CBubbleManager::TPhoneCallState GetBubbleState( TPEState aState );

        /**
        * convert TPhoneCallState to TPEState
        */
        TPEState ConvertCallState(
            CBubbleManager::TPhoneCallState aBubbleState );

        /**
        * Count active call bubbles (Active + Hold + Disconnecting )
        */
        TInt CountNumberOfActiveCalls() const;

        /**
        * Adds party to conference call.
        */
        void AddToConferenceL( TInt aCallId );


    private:

        void StartMOVideoCallAnimationHeaderL(
            CBubbleManager::TBubbleId aBubbleId,
            const TDesC& aStateLabelText,
            const TDesC& aStateShortLabelText );

        /**
        * From MPhoneVideoPlayer
        */
        void PlayVideoRingTone( const TDesC& aFileName,
                                TPlayMode aPlayType,
                                TInt aVolumeLevel,
                                TBool aArbitraryScaling,
                                MPhoneVideoPlayerObserver* aObserver );

        /**
        * From MPhoneVideoPlayer
        */
        void StopVideoRingTone();

        /**
        * From MPhoneVideoPlayer
        */
        void MuteVideoRingTone();

        /**
        * From MPhoneVideoPlayer
        */
        void CancelVideoRingTone();

        /**
        * From MBubbleVideoPlaybackObserver
        */
        void HandleBubbleVideoPlaybackEvent(
            TBubbleVideoPlaybackEvent aEvent );

        /**
        * From MBubbleVideoPlaybackObserver
        */
        void HandleBubbleVideoPlaybackError(
            TBubbleVideoPlaybackError aErrorType, TInt aErrorCode );

        /**
        * Get number entry mode from bubblemanager
        */
        TBool IsNumberEntryNumericMode() const;

        /**
        * From MPhoneCallThemeSettingsObserver
        */
        void CallThemeSettingsChanged();

        /**
        * Sets call header parameters.
        */
        void SetCallHeaderParameters(
            TInt aBubble,
            TPhoneCmdParamCallHeaderData* aCallHeaderParam );
        
        /**
         * Returns ETrue if simplified conference call bubble must be used
         * in a conference created using the given service.
         * 
         * @param   aServiceId  Service identifier.
         * @return  ETrue if simplified conference call bubble setting is 
         *          enabled, EFalse otherwise.
         */
        TBool IsSimplifiedConferenceCallHeaderEnabledL( TUint32 aServiceId ) const;
        
        /**
         * Sets simplified conference call header status flag on/off.
         * 
         * @param   aOnOff      ETrue switches setting on, EFalse off.
         */
        void SetSimplifiedConferenceCallHeaderStatus( TBool aOnOff );
        
    private:

        // Real bubble manager
        CBubbleManager* iBubbleManager;

        // Bubble mapping of call and bubble
        CPhoneBubbleMapping* iMapping;

        // is number entry visible
        TBool iNumberEntryVisible;

        // is conference call on or not
        TBool iConference;

        // Stores call id's of conference participants.
        typedef CArrayFixFlat<const TInt> CParticipantArray;
        CParticipantArray* iParticipants;



        // ETrue if ciphering indicator is allowed
        TBool iCipheringIndicatorAllowed;

        enum TFlags
            {
            // Conference is expanded.
            EFlagConferenceExpanded = 1,
            // Conference is required to expanded, with highlight
            EFlagConferenceReqExpanded = 2,
            // Number entry exists
            EFlagNumberEntryExists = 4,
            // Number entry visible
            EFlagNumberEntryVisible = 8,
            // Overrides EFlagConferenceExpanded setting
            EFlagSimplifiedConferenceCallHeader = 16
            };
        // Sum of subset of TFlags
        TInt iFlags;

        // Pointer to video call's text animation bubble header
        CPhoneCallHeaderTextAnimation* iVideoCallAnimationHeader;

        // Is Image/text call feature supported
        TBool iCallImageTextSupported;

        /** 
         * Pointer to call theme monitor
         * Own.
         */
        CPhoneCallThemeMonitor* iCallTheme;

        // Observer for video playback events
        MPhoneVideoPlayerObserver* iVideoPlayerObserver;

        /**
         * Container for Phone number entry related handling.
         * Own.
         */
        CPhoneNumberEntry* iPhoneNumberEntry;

        /**
         * Interface to Dialer's number entry functionality
         * Not own.
        */
        MNumberEntry* iDialerNumberEntry;

        // Is dialler supported.
        TBool iUseDialer;

        // Is number entry mode numeric/alpha
        TBool iIsNumberEntryModeNumeric;

        /**
         * Bubble extension plugin manager.
         * Own.
         */
        CPhoneBubbleExtensionManager* iBubbleExtensionManager;
       
        /**
         * Interface to phone customization
         * Not own.
         */
        MPhoneCustomization* iPhoneCustomization;
    };

#endif // CPHONEBUBBLEWRAPPER_H

// End of File
