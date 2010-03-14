/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
*   Common phone view command definitions.
*
*/


#ifndef PHONEVIEWCOMMANDDEFINITIONS_H
#define PHONEVIEWCOMMANDDEFINITIONS_H

// INCLUDES

#include "phoneconstants.h"

// DATA TYPES

typedef TInt TPhoneViewCommandId;

enum
{
    EPhoneViewCreateNumberEntry = KPhoneViewCommandCommonFirst,
    EPhoneViewSetNumberEntryContent,
    EPhoneViewSendKeyEventToNumberEntry,
    EPhoneViewRemoveNumberEntry,
    EPhoneViewSetNumberEntryVisible,
    EPhoneViewGetNumberFromEntry,
    EPhoneViewGetNumberEntryCount,
    EPhoneViewGetNumberEntryIsUsedStatus,
    EPhoneViewGetNumberEntryIsVisibleStatus,
    EPhoneViewToggleNumberEntryAlphaNumericMode, // 10
    EPhoneViewIsNumberEntryNumericMode,
    EPhoneViewClearNumberEntryContent,
    EPhoneViewCreateCallHeader,
    EPhoneViewCreateEmergencyCallHeader,
    EPhoneViewUpdateCallHeaderRemoteInfoData,
    EPhoneViewUpdateCallHeaderCallDuration,
    EPhoneViewRemoveCallHeader,
    EPhoneViewMenuBarOpen,
    EPhoneViewMenuBarClose,
    EPhoneViewDimCbaCommand, // 20
    EPhoneViewUpdateCba,
    EPhoneViewUpdateBubble,
    EPhoneViewMenuPane,
    EPhoneViewMenuBar,
    EPhoneViewSetTitlePaneContent,
    EPhoneViewSetNaviPaneContent,
    EPhoneViewSetTitlePanePicture,
    EPhoneViewGetTitlePaneIsVisibleStatus,
    EPhoneViewActivateAudioPathUIChanges,
    EPhoneViewActivateAudioAvailabilityUIChanges, //30
    EPhoneViewActivateMuteUIChanges,
    EPhoneViewShowNote,
    EPhoneViewShowGlobalNote,
    EPhoneViewShowProgressNote,
    EPhoneViewRemoveNote,
    EPhoneViewRemoveProgressNote,
    EPhoneViewRemoveGlobalNote,
    EPhoneViewGetIsNoteVisible, 
    EPhoneViewGetIsProgressNoteVisible,
    EPhoneViewShowQuery, // 40
    EPhoneViewSetListQueryString,
    EPhoneViewRemoveQuery,
    EPhoneViewGetTextQueryContent,
    EPhoneViewSetTopApplication,
    EPhoneViewBringAppToForeground,
    EPhoneViewSendToBackground,
    EPhoneViewSetNeedToSendToBackgroundStatus,
    EPhoneViewGetNeedToSendToBackgroundStatus, 
    EPhoneViewGetTopApplicationIsDisplayedStatus,
    EPhoneViewGetForegroundApplication, // 50
    EPhoneViewSetBackgroundImageBitmap,
    EPhoneViewGetBackgroundImageBitmap,
    EPhoneViewActivateApp,
    EPhoneViewActivateAppView,
    EPhoneViewActivateAppViewWithCustomMessage,
    EPhoneViewActivateAppViewConventional,
    EPhoneViewLaunchApplication,
    EPhoneViewLaunchHelpApplication, 
    EPhoneViewOpenMessageEditor,
    EPhoneViewExitApplications,  // 60
    EPhoneViewUpdateIncallIndicator,
    EPhoneViewStartCapturingKey,
    EPhoneViewStopCapturingKey,
    EPhoneViewSetGlobalNotifiersDisabled,
    EPhoneViewSetSpeedDialLocation, 
    EPhoneViewGetNumberFromSpeedDialLocation,
    EPhoneViewAssignSpeedDialLocation,
    EPhoneViewGetHoldFlag,
    EPhoneViewSetHoldFlag,
    EPhoneViewSetSoftRejectFlag, // 70
    EPhoneViewGetSoftRejectFlag,
    EPhoneViewSetServiceCodeFlag,
    EPhoneViewUpdateContact,
    EPhoneViewAddContact,
    EPhoneViewOpenSingleItemFetchDialog,
    EPhoneViewRemovePhoneDialogs,
    EPhoneViewCreateConference,
    EPhoneViewAddToConference,
    EPhoneViewRemoveFromConference,
    EPhoneViewPrivateFromConference, // 80
    EPhoneViewSelectedConfMember,
    EPhoneViewGetConferenceListVisibleFlag, 
    EPhoneViewOpenConferenceList,
    EPhoneViewGetCallState,
    EPhoneViewGetCallIdByState,
    EPhoneViewMoveHighLightInList,
    EPhoneViewGetCountOfActiveCalls,
    EPhoneViewSetNaviPaneAudioVolume,
    EPhoneViewShowNaviPaneAudioVolume,
    EPhoneViewHideNaviPaneAudioVolume, // 90 
    EPhoneViewGetIsConference,
    EPhoneViewGetCallExistsInConference,
    EPhoneViewRemoveAllCallHeaders,
    EPhoneViewPlayRingTone,
    EPhoneViewMuteRingTone,
    EPhoneViewStopRingTone,
    EPhoneViewMuteRingToneOnAnswer,
    EPhoneViewCipheringInfoChange,
    EPhoneViewUpdateMaxConfMemberFlag,
    EPhoneViewCloseFSW, // 100
    EPhoneViewLaunchRfsDeep,
    EPhoneViewLaunchRfsNormal,
    EPhoneViewUpdateView,
    EPhoneViewOpenSoftRejectEditor,
    EPhoneViewSendMessage,
    EPhoneViewSetBlockingDialogStatus,
    EPhoneViewSetPhoneNumberAvailableInPhoneEngine,
    EPhoneViewSetSecurityMode,
    EPhoneViewGetSecurityModeStatus,
    EPhoneViewSetStatusPaneVisible, // 110
    EPhoneViewEnableKeyLock,
    EPhoneViewEnableKeyLockWithoutNote,
    EPhoneViewLaunchFaxModem,
    EPhoneViewUpdateFSW,
    EPhoneViewIsQuery,
    EPhoneViewBringIdleToForeground, 
    EPhoneViewSetIdleTopApplication,  
    EPhoneViewBeginUpdate,
    EPhoneViewEndUpdate,
    EPhoneViewSetConfrenceOptionsFlag, // 120 
    EPhoneViewRemoveConferenceBubble, 
    EPhoneViewGetSoftRejectWindowGroupId,
    EPhoneViewSetTouchPaneButtons,
    EPhoneViewSetTouchPaneVisible,
    EPhoneViewEnableTouchButton,
    EPhoneViewDisableTouchButton, 
    EPhoneResetTouchButtons, 
    EPhoneViewGetIsConferenceInSelectionMode,
    EPhoneViewIsStraightCallToVideoMailBox,
    EPhoneViewSetDtmfDialerViewVisible, // 130
    EPhoneViewSetRestrictedDialer, 
    EPhoneViewIsConferenceInExpandedMode,
    EPhoneViewIsMenuBarVisible,
    EPhoneViewGetBlockingDialogStatus,
    EPhoneViewYesSingleItemFetch,
    EPhoneViewNoSingleItemFetch, 
    EPhoneViewGetLocalizedNumberFromEntry,
    EPhoneViewIsNoteActive,
    EPhoneViewOpenNumberEntry,
    EPhoneViewOpenCallHandling, // 140
    EPhoneViewGetConferenceAndSingleFlag,
    EPhoneViewSetConferenceAndSingleFlag,
    EPhoneViewSetVideoCallFlag,
    EPhoneViewSetDtmfOptionsFlag,
    EPhoneViewShowVTSetupFailedCreateCallToSameContactQuery,
    EPhoneViewGetAudioVolumeLevel, 
    EPhoneViewIsIdleTopApp,
    EPhoneViewUpdateToolbar,
    EPhoneViewSetControlAndVisibility,
    EPhoneViewAllowWaitingCallHeader, // 150
    EPhoneViewBeginTransEffect,
    EPhoneViewEndTransEffect,
    EPhoneViewSetDialerControlVisible,
    EPhoneViewCloseSingleItemFetchDialog,
    EPhoneViewGetActivatePreviousApp,
    EPhoneViewActivatePreviousApp, 
    EPhoneViewSetPointerCapture,
    EPhoneViewSetSendKeyDialerActivationFlag,
    EPhoneViewPrepareIcons,
    EPhoneViewKeypadAudioEnabled, // 160
    EPhoneViewKeypadAudioDisabled,
    EPhoneViewUpdateCallHeaderRemoteInfoDataAndLabel,
    EPhoneViewRemoveDtmfNote,
    EPhoneViewUpdatePhoneIconToFSW,
    EPhoneViewSetEikonNotifiersDisabled, 
    EPhoneViewCancelAllNotications, 
    EPhoneViewSetVideoCallDTMFVisibilityFlag,
    EPhoneViewIsKeyLockEnabled,
    EPhoneViewDisableKeyLock,
    EPhoneViewLoadPlugins, // 170
    EPhoneViewOpenVirtualKeyBoard,
    EPhoneViewUpdateNoteSoftkeys,

    EPhoneViewSendAiwCommand,
    EPhoneViewShowCustomizedDialer,
    EPhoneViewHideCustomizedDialer,
    EPhoneViewGetCustomizedDialerMenuResourceId,
    EPhoneViewGetCustomizedDialerCbaResourceId,
    EPhoneViewCipheringInfoChangePlayTone,
	EPhoneViewCreatePhoneBookServices,
	EPhoneViewDisableKeyLockWithoutNote, // 180
	EPhoneViewGetKeyLockStatus,
    EPhoneViewSetNumberEntryObserver,
    EPhoneViewGetSingleItemFetchType,
    EPhoneViewRemoveGlobalWaitNote,
    EPhoneViewSetPhoneCustomization,
    EPhoneViewSetViewCustomization,
    EPhoneViewSetButtonCustomization,
    EPhoneViewUpdateContactByString,
    EPhoneViewAddContactByString,
    EPhoneViewIsActiveNoteDissmissableByKeyEvent, // 190
    EPhoneViewUpdateContextMenu,
    EPhoneViewSetNoConnectedCalls,
    EPhoneViewIsDTMFEditorVisible,
    EPhoneViewSetToolbarDimming,
	EPhoneViewEnableToolbarButton,
	EPhoneViewHideToolbar,
	EPhoneViewShowToolbar,    
    EPhoneViewGetEasyDialingInFocusStatus,
    EPhoneViewSetInvalidCsPhoneNumberFlag,
    EPhoneViewGetInvalidCsPhoneNumberFlag,
    EPhoneViewGetEasyDialingMenuId,
    EPhoneViewGetEasyDialingCbaId,

    EPhoneAmountOfCommands, // Don't remove this, this should always be the last in list.
};


typedef TInt TPhoneViewResponseId;

enum
{
    EPhoneViewResponseFailed = KPhoneViewResponseCommonFirst,
    EPhoneViewResponseSuccess,
    EPhoneViewResponseNumberEntryCleared

};

#endif // PHONEVIEWCOMMANDDEFINITIONS_H

// End of File
