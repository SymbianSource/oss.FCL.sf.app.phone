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
    EPhoneViewMenuBarOpen, //20
    EPhoneViewMenuBarClose,
    EPhoneViewDimCbaCommand,
    EPhoneViewUpdateCba,
    EPhoneViewUpdateBubble,
    EPhoneViewMenuPane,
    EPhoneViewMenuBar,
    EPhoneViewSetTitlePaneContent,
    EPhoneViewSetNaviPaneContent,
    EPhoneViewSetTitlePanePicture,
    EPhoneViewGetTitlePaneIsVisibleStatus, //30
    EPhoneViewActivateAudioPathUIChanges,
    EPhoneViewActivateAudioAvailabilityUIChanges,
    EPhoneViewActivateMuteUIChanges,
    EPhoneViewShowNote,
    EPhoneViewShowGlobalNote,
    EPhoneViewShowProgressNote,
    EPhoneViewRemoveNote,
    EPhoneViewRemoveProgressNote,
    EPhoneViewRemoveGlobalNote,
    EPhoneViewGetIsNoteVisible, // 40
    EPhoneViewGetIsProgressNoteVisible,
    EPhoneViewShowQuery,
    EPhoneViewSetListQueryString,
    EPhoneViewRemoveQuery,
    EPhoneViewGetTextQueryContent,
    EPhoneViewSetTopApplication,
    EPhoneViewBringAppToForeground,
    EPhoneViewSendToBackground,
    EPhoneViewSetNeedToSendToBackgroundStatus,
    EPhoneViewGetNeedToSendToBackgroundStatus, // 50
    EPhoneViewGetTopApplicationIsDisplayedStatus,
    EPhoneViewGetForegroundApplication,
    EPhoneViewSetBackgroundImageBitmap,
    EPhoneViewGetBackgroundImageBitmap,
    EPhoneViewActivateApp,
    EPhoneViewActivateAppView,
    EPhoneViewActivateAppViewWithCustomMessage,
    EPhoneViewActivateAppViewConventional,
    EPhoneViewLaunchApplication,
    EPhoneViewLaunchHelpApplication,  // 60
    EPhoneViewOpenMessageEditor,
    EPhoneViewExitApplications,
    EPhoneViewUpdateIncallIndicator,
    EPhoneViewStartCapturingKey,
    EPhoneViewStopCapturingKey,
    EPhoneViewSetGlobalNotifiersDisabled,
    EPhoneViewSetSpeedDialLocation, 
    EPhoneViewGetNumberFromSpeedDialLocation,
    EPhoneViewAssignSpeedDialLocation,
    EPhoneViewGetHoldFlag,
    EPhoneViewSetHoldFlag,
    EPhoneViewSetSoftRejectFlag,
    EPhoneViewGetSoftRejectFlag,
    EPhoneViewSetServiceCodeFlag,
    EPhoneViewUpdateContact, // 80
    EPhoneViewAddContact,
    EPhoneViewOpenSingleItemFetchDialog,
    EPhoneViewRemovePhoneDialogs,
    EPhoneViewCreateConference,
    EPhoneViewAddToConference,
    EPhoneViewRemoveFromConference,
    EPhoneViewPrivateFromConference,
    EPhoneViewSelectedConfMember,
    EPhoneViewGetConferenceListVisibleFlag, 
    EPhoneViewOpenConferenceList, // 90
    EPhoneViewGetCallState,
    EPhoneViewGetCallIdByState,
    EPhoneViewMoveHighLightInList,
    EPhoneViewGetCountOfActiveCalls,
    EPhoneViewSetNaviPaneAudioVolume,
    EPhoneViewShowNaviPaneAudioVolume,
    EPhoneViewHideNaviPaneAudioVolume,
    EPhoneViewGetIsConference,
    EPhoneViewGetCallExistsInConference,
    EPhoneViewRemoveAllCallHeaders,
    EPhoneViewPlayRingTone,
    EPhoneViewMuteRingTone,
    EPhoneViewStopRingTone,
    EPhoneViewMuteRingToneOnAnswer,
    EPhoneViewCipheringInfoChange,
    EPhoneViewUpdateMaxConfMemberFlag, // 110
    EPhoneViewCloseFSW, 
    EPhoneViewLaunchRfsDeep,
    EPhoneViewLaunchRfsNormal,
    EPhoneViewUpdateView,
    EPhoneViewOpenSoftRejectEditor,
    EPhoneViewSendMessage,
    EPhoneViewSetBlockingDialogStatus,
    EPhoneViewSetPhoneNumberAvailableInPhoneEngine,
    EPhoneViewSetSecurityMode,
    EPhoneViewGetSecurityModeStatus, // 120
    EPhoneViewSetStatusPaneVisible, 
    EPhoneViewEnableKeyLock,
    EPhoneViewEnableKeyLockWithoutNote,
    EPhoneViewLaunchFaxModem,
    EPhoneViewUpdateFSW,
    EPhoneViewIsQuery,
    EPhoneViewBringIdleToForeground, 
    EPhoneViewSetIdleTopApplication,  
    EPhoneViewBeginUpdate,
    EPhoneViewEndUpdate,   // 130
    EPhoneViewSetConfrenceOptionsFlag,
    EPhoneViewRemoveConferenceBubble,
    EPhoneViewGetSoftRejectWindowGroupId,
    EPhoneViewSetTouchPaneButtons,
    EPhoneViewSetTouchPaneVisible,
    EPhoneViewEnableTouchButton,
    EPhoneViewDisableTouchButton, 
    EPhoneResetTouchButtons, 
    EPhoneViewGetIsConferenceInSelectionMode,
    EPhoneViewIsStraightCallToVideoMailBox, // 140
    EPhoneViewSetDtmfDialerViewVisible,
    EPhoneViewSetRetrictedDialer,
    EPhoneViewIsConferenceInExpandedMode,
    EPhoneViewIsMenuBarVisible,
    EPhoneViewGetBlockingDialogStatus,
    EPhoneViewYesSingleItemFetch,
    EPhoneViewNoSingleItemFetch, 
    EPhoneViewGetLocalizedNumberFromEntry,
    EPhoneViewIsNoteActive,
    EPhoneViewOpenNumberEntry, //150
    EPhoneViewOpenCallHandling,
    EPhoneViewGetConferenceAndSingleFlag,
    EPhoneViewSetConferenceAndSingleFlag,
    EPhoneViewSetVideoCallFlag,
    EPhoneViewSetDtmfOptionsFlag,
    EPhoneViewShowVTSetupFailedCreateCallToSameContactQuery,
    EPhoneViewGetAudioVolumeLevel, 
    EPhoneViewIsIdleTopApp,
    EPhoneViewUpdateToolbar,
    EPhoneViewSetControlAndVisibility, //160
    EPhoneViewAllowWaitingCallHeader,
    EPhoneViewBeginTransEffect,
    EPhoneViewEndTransEffect,
    EPhoneViewSetDialerControlVisible,
    EPhoneViewCloseSingleItemFetchDialog,
    EPhoneViewGetActivatePreviousApp,
    EPhoneViewActivatePreviousApp, 
    EPhoneViewSetPointerCapture,
    EPhoneViewSetSendKeyDialerActivationFlag,
    EPhoneViewPrepareIcons, //170
    EPhoneViewKeypadAudioEnabled,
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
    EPhoneViewDisableKeyLockWithoutNote,
    EPhoneViewGetKeyLockStatus,
    EPhoneViewSetNumberEntryObserver,
    EPhoneViewGetSingleItemFetchType,
    EPhoneViewRemoveGlobalWaitNote,
    EPhoneViewSetPhoneCustomization,
    EPhoneViewSetViewCustomization,
    EPhoneViewSetButtonCustomization,
    EPhoneViewUpdateContactByString,
    EPhoneViewAddContactByString,
    EPhoneViewIsActiveNoteDissmissableByKeyEvent,
    EPhoneViewUpdateContextMenu,
    EPhoneViewSetNoConnectedCalls,
    EPhoneViewIsDTMFEditorVisible,
    EPhoneViewSetToolbarDimming,
    EPhoneViewEnableToolbarButton,
    EPhoneViewHideToolbar,
    EPhoneViewShowToolbar,
    EPhoneViewGetExpandedBubbleCallId,
    EPhoneViewOpenDialer,
    EPhoneSetConferenceExpanded,
    EPhoneAppShutDown,
    EPhoneViewBackButtonActive,
    EPhoneViewHsToForegroundAfterCall,
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
