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
* Description:  Macro definition file for logging.
*
*/

// Autogenerated from .\phoneloggerviewcommands.st by the stringtable tool - Do not edit
#include <e32std.h>
#include <stringpool.h>
#include <stringtablesupport.h>
#include "phoneloggerviewcommands.h"
#ifdef _DEBUG
#undef _DEBUG
#endif

_STLIT8(K1, "EPhoneViewStartValue");
_STLIT8(K2, "EPhoneViewCreateNumberEntry");
_STLIT8(K3, "EPhoneViewSetNumberEntryContent");
_STLIT8(K4, "EPhoneViewSendKeyEventToNumberEntry");
_STLIT8(K5, "EPhoneViewRemoveNumberEntry");
_STLIT8(K6, "EPhoneViewSetNumberEntryVisible");
_STLIT8(K7, "EPhoneViewGetNumberFromEntry");
_STLIT8(K8, "EPhoneViewGetNumberEntryCount");
_STLIT8(K9, "EPhoneViewGetNumberEntryIsUsedStatus");
_STLIT8(K10, "EPhoneViewGetNumberEntryIsVisibleStatus");
_STLIT8(K11, "EPhoneViewToggleNumberEntryAlphaNumericMode");
_STLIT8(K12, "EPhoneViewIsNumberEntryNumericMode");
_STLIT8(K13, "EPhoneViewClearNumberEntryContent");
_STLIT8(K14, "EPhoneViewCreateCallHeader");
_STLIT8(K15, "EPhoneViewCreateEmergencyCallHeader");
_STLIT8(K16, "EPhoneViewUpdateCallHeaderRemoteInfoData");
_STLIT8(K17, "EPhoneViewUpdateCallHeaderCallDuration");
_STLIT8(K18, "EPhoneViewRemoveCallHeader");
_STLIT8(K19, "EPhoneViewMenuBarOpen");
_STLIT8(K20, "EPhoneViewMenuBarClose");
_STLIT8(K21, "EPhoneViewDimCbaCommand");
_STLIT8(K22, "EPhoneViewUpdateCba");
_STLIT8(K23, "EPhoneViewUpdateBubble");
_STLIT8(K24, "EPhoneViewMenuPane");
_STLIT8(K25, "EPhoneViewMenuBar");
_STLIT8(K26, "EPhoneViewSetTitlePaneContent");
_STLIT8(K27, "EPhoneViewSetNaviPaneContent");
_STLIT8(K28, "EPhoneViewSetTitlePanePicture");
_STLIT8(K29, "EPhoneViewGetTitlePaneIsVisibleStatus");
_STLIT8(K30, "EPhoneViewActivateAudioPathUIChanges");
_STLIT8(K31, "EPhoneViewActivateAudioAvailabilityUIChanges");
_STLIT8(K32, "EPhoneViewActivateMuteUIChanges");
_STLIT8(K33, "EPhoneViewShowNote");
_STLIT8(K34, "EPhoneViewShowGlobalNote");
_STLIT8(K35, "EPhoneViewShowProgressNote");
_STLIT8(K36, "EPhoneViewRemoveNote");
_STLIT8(K37, "EPhoneViewRemoveProgressNote");
_STLIT8(K38, "EPhoneViewRemoveGlobalNote");
_STLIT8(K39, "EPhoneViewGetIsNoteVisible");
_STLIT8(K40, "EPhoneViewGetIsProgressNoteVisible");
_STLIT8(K41, "EPhoneViewShowQuery");
_STLIT8(K42, "EPhoneViewSetListQueryString");
_STLIT8(K43, "EPhoneViewRemoveQuery");
_STLIT8(K44, "EPhoneViewGetTextQueryContents");
_STLIT8(K45, "EPhoneViewSetTopApplication");
_STLIT8(K46, "EPhoneViewBringAppToForeground");
_STLIT8(K47, "EPhoneViewSendToBackground");
_STLIT8(K48, "EPhoneViewSetNeedToSendToBackgroundStatus");
_STLIT8(K49, "EPhoneViewGetNeedToSendToBackgroundStatus");
_STLIT8(K50, "EPhoneViewGetTopApplicationIsDisplayedStatus");
_STLIT8(K51, "EPhoneViewGetForegroundApplication");
_STLIT8(K52, "EPhoneViewSetBackgroundImageBitmap");
_STLIT8(K53, "EPhoneViewGetBackgroundImageBitmap");
_STLIT8(K54, "EPhoneViewActivateApp");
_STLIT8(K55, "EPhoneViewActivateAppView");
_STLIT8(K56, "EPhoneViewActivateAppViewWithCustomMessage");
_STLIT8(K57, "EPhoneViewActivateAppViewConventional");
_STLIT8(K58, "EPhoneViewLaunchApplication");
_STLIT8(K59, "EPhoneViewLaunchHelpApplication");
_STLIT8(K60, "EPhoneViewOpenMessageEditor");
_STLIT8(K61, "EPhoneViewExitApplications");
_STLIT8(K62, "EPhoneViewUpdateIncallIndicator");
_STLIT8(K63, "EPhoneViewStartCapturingKey");
_STLIT8(K64, "EPhoneViewStopCapturingKey");
_STLIT8(K65, "EPhoneViewSetGlobalNotifiersDisabled");
_STLIT8(K66, "EPhoneViewSetSpeedDialLocation");
_STLIT8(K67, "EPhoneViewGetNumberFromSpeedDialLocation");
_STLIT8(K68, "EPhoneViewAssignSpeedDialLocation");
_STLIT8(K69, "EPhoneViewGetHoldFlag");
_STLIT8(K70, "EPhoneViewSetHoldFlag");
_STLIT8(K71, "EPhoneViewSetSoftRejectFlag");
_STLIT8(K72, "EPhoneViewGetSoftRejectFlag");
_STLIT8(K73, "EPhoneViewSetServiceCodeFlag");
_STLIT8(K74, "EPhoneViewUpdateContact");
_STLIT8(K75, "EPhoneViewAddContact");
_STLIT8(K76, "EPhoneViewOpenSingleItemFetchDialog");
_STLIT8(K77, "EPhoneViewRemovePhoneDialogs");
_STLIT8(K78, "EPhoneViewCreateConference");
_STLIT8(K79, "EPhoneViewAddToConference");
_STLIT8(K80, "EPhoneViewRemoveFromConference");
_STLIT8(K81, "EPhoneViewPrivateFromConference");
_STLIT8(K82, "EPhoneViewSelectedConfMember");
_STLIT8(K83, "EPhoneViewGetConferenceListVisibleFlag");
_STLIT8(K84, "EPhoneViewOpenConferenceList");
_STLIT8(K85, "EPhoneViewGetCallState");
_STLIT8(K86, "EPhoneViewGetCallIdByState");
_STLIT8(K87, "EPhoneViewMoveHighLightInList");
_STLIT8(K88, "EPhoneViewGetCountOfActiveCalls");
_STLIT8(K89, "EPhoneViewSetNaviPaneAudioVolume");
_STLIT8(K90, "EPhoneViewShowNaviPaneAudioVolume");
_STLIT8(K91, "EPhoneViewHideNaviPaneAudioVolume");
_STLIT8(K92, "EPhoneViewGetIsConference");
_STLIT8(K93, "EPhoneViewGetCallExistsInConference");
_STLIT8(K94, "EPhoneViewRemoveAllCallHeaders");
_STLIT8(K95, "EPhoneViewPlayRingTone");
_STLIT8(K96, "EPhoneViewMuteRingTone");
_STLIT8(K97, "EPhoneViewStopRingTone");
_STLIT8(K98, "EPhoneViewMuteRingToneOnAnswer");
_STLIT8(K99, "EPhoneViewCipheringInfoChange");
_STLIT8(K100, "EPhoneViewUpdateMaxConfMemberFlag");
_STLIT8(K101, "EPhoneViewCloseFSW");
_STLIT8(K102, "EPhoneViewLaunchRfsDeep");
_STLIT8(K103, "EPhoneViewLaunchRfsNormal");
_STLIT8(K104, "EPhoneViewUpdateView");
_STLIT8(K105, "EPhoneViewOpenSoftRejectEditor");
_STLIT8(K106, "EPhoneViewSendMessage");
_STLIT8(K107, "EPhoneViewSetBlockingDialogStatus");
_STLIT8(K108, "EPhoneViewSetPhoneNumberAvailableInPhoneEngine");
_STLIT8(K109, "EPhoneViewSetStatusPaneVisible");
_STLIT8(K110, "EPhoneViewEnableKeyLock");
_STLIT8(K111, "EPhoneViewLaunchFaxModem");
_STLIT8(K112, "EPhoneViewUpdateFSW");
_STLIT8(K113, "EPhoneViewIsQuery");
_STLIT8(K114, "EPhoneViewBringIdleToForeground");
_STLIT8(K115, "EPhoneViewSetIdleTopApplication");
_STLIT8(K116, "EPhoneViewBeginUpdate");
_STLIT8(K117, "EPhoneViewEndUpdate");
_STLIT8(K118, "EPhoneViewSetConfrenceOptionsFlag");
_STLIT8(K119, "EPhoneViewRemoveConferenceBubble");
_STLIT8(K120, "EPhoneViewGetSoftRejectWindowGroupId");
_STLIT8(K121, "EPhoneViewSetTouchPaneButtons");
_STLIT8(K122, "EPhoneViewSetTouchPaneVisible");
_STLIT8(K123, "EPhoneViewEnableTouchButton");
_STLIT8(K124, "EPhoneViewDisableTouchButton");
_STLIT8(K125, "EPhoneResetTouchButtons");
_STLIT8(K126, "EPhoneViewGetIsConferenceInSelectionMode");
_STLIT8(K127, "EPhoneViewIsStraightCallToVideoMailBox");
_STLIT8(K128, "EPhoneViewSetDtmfDialerViewVisible");
_STLIT8(K129, "EPhoneViewSetRestrictedDialer");
_STLIT8(K130, "EPhoneViewIsConferenceInExpandedMode");
_STLIT8(K131, "EPhoneViewIsMenuBarVisible");
_STLIT8(K132, "EPhoneViewGetBlockingDialogStatus");
_STLIT8(K133, "EPhoneViewYesSingleItemFetch");
_STLIT8(K134, "EPhoneViewNoSingleItemFetch");
_STLIT8(K135, "EPhoneViewGetLocalizedNumberFromEntry");
_STLIT8(K136, "EPhoneViewIsNoteActive");
_STLIT8(K137, "EPhoneViewOpenNumberEntry");
_STLIT8(K138, "EPhoneViewOpenCallHandling");
_STLIT8(K139, "EPhoneViewGetConferenceAndSingleFlag");
_STLIT8(K140, "EPhoneViewSetConferenceAndSingleFlag");
_STLIT8(K141, "EPhoneViewSetVideoCallFlag");
_STLIT8(K142, "EPhoneViewSetDtmfOptionsFlag");
_STLIT8(K143, "EPhoneViewShowVTSetupFailedCreateCallToSameContactQuery");
_STLIT8(K144, "EPhoneViewGetAudioVolumeLevel");
_STLIT8(K145, "EPhoneViewIsIdleTopApp");
_STLIT8(K146, "EPhoneViewUpdateToolbar");
_STLIT8(K147, "EPhoneViewSetControlAndVisibility");
_STLIT8(K148, "EPhoneViewAllowWaitingCallHeader");
_STLIT8(K149, "EPhoneViewBeginTransEffect");
_STLIT8(K150, "EPhoneViewEndTransEffect");
_STLIT8(K151, "EPhoneViewSetDialerControlVisible");
_STLIT8(K152, "EPhoneViewCloseSingleItemFetchDialog");
_STLIT8(K153, "EPhoneViewGetActivatePreviousApp");
_STLIT8(K154, "EPhoneViewActivatePreviousApp");
_STLIT8(K155, "EPhoneViewSetPointerCapture");
_STLIT8(K156, "EPhoneViewSetSendKeyDialerActivationFlag");
_STLIT8(K157, "EPhoneViewPrepareIcons");
_STLIT8(K158, "EPhoneViewKeypadAudioEnabled");
_STLIT8(K159, "EPhoneViewKeypadAudioDisabled");
_STLIT8(K160, "EPhoneViewUpdateCallHeaderRemoteInfoDataAndLabel");
_STLIT8(K161, "EPhoneViewRemoveDtmfNote");
_STLIT8(K162, "EPhoneViewUpdatePhoneIconToFSW");
_STLIT8(K163, "EPhoneViewSetEikonNotifiersDisabled");
_STLIT8(K164, "EPhoneViewCancelAllNotications");
_STLIT8(K165, "EPhoneViewSetVideoCallDTMFVisibilityFlag");
_STLIT8(K166, "EPhoneViewLoadPlugins");
_STLIT8(K167, "EPhoneViewOpenVirtualKeyBoard");
_STLIT8(K168, "EPhoneViewUpdateNoteSoftkeys");
_STLIT8(K169, "EPhoneViewSendAiwCommand");
_STLIT8(K170, "EPhoneViewShowCustomizedDialer");
_STLIT8(K171, "EPhoneViewHideCustomizedDialer");
_STLIT8(K172, "EPhoneViewGetCustomizedDialerMenuResourceId");
_STLIT8(K173, "EPhoneViewGetCustomizedDialerCbaResourceId");
_STLIT8(K174, "EPhoneViewCipheringInfoChangePlayTone");
_STLIT8(K175, "EPhoneViewCreatePhoneBookServices");
_STLIT8(K176, "EPhoneViewDisableKeyLockWithoutNote");
_STLIT8(K177, "EPhoneViewGetKeyLockStatus");
_STLIT8(K178, "EPhoneViewSetNumberEntryObserver");
_STLIT8(K179, "EPhoneViewGetSingleItemFetchType");
_STLIT8(K180, "EPhoneViewRemoveGlobalWaitNote");
_STLIT8(K181, "EPhoneViewSetPhoneCustomization");
_STLIT8(K182, "EPhoneViewSetViewCustomization");
_STLIT8(K183, "EPhoneViewSetButtonCustomization");
_STLIT8(K184, "EPhoneViewUpdateContactByString");
_STLIT8(K185, "EPhoneViewAddContactByString");
_STLIT8(K186, "EPhoneViewIsActiveNoteDissmissableByKeyEvent");
_STLIT8(K187, "EPhoneViewUpdateContextMenu");
_STLIT8(K188, "EPhoneViewSetNoConnectedCalls");
_STLIT8(K189, "EPhoneViewIsDTMFEditorVisible");
_STLIT8(K190, "EPhoneViewSetToolbarDimming");
_STLIT8(K191, "EPhoneViewEnableToolbarButton");
_STLIT8(K192, "EPhoneViewHideToolbar");
_STLIT8(K193, "EPhoneViewShowToolbar");
_STLIT8(K194, "EPhoneViewGetEasyDialingInFocusStatus");
_STLIT8(K195, "EPhoneViewSetInvalidCsPhoneNumberFlag");
_STLIT8(K196, "EPhoneViewGetInvalidCsPhoneNumberFlag");
_STLIT8(K197, "EPhoneViewGetEasyDialingMenuId");
_STLIT8(K198, "EPhoneViewGetEasyDialingCbaId");
_STLIT8(K199, "EPhoneViewGetNeedToReturnToForegroundAppStatus");
_STLIT8(K200, "EPhoneViewSetNeedToReturnToForegroundAppStatus");
_STLIT8(K201, "EPhoneViewSetConferenceAndWaitingVideo");
_STLIT8(K202, "EPhoneViewLaunchMultimediaSharing");
_STLIT8(K203, "EPhoneViewGetQwertyModeObserver");
_STLIT8(K204, "EPhoneViewSetIncallBubbleTrue");
_STLIT8(K205, "EPhoneViewSetIncallBubbleFalse");
_STLIT8(K206, "EPhoneViewIsDisplayingMenuOrDialog");
_STLIT8(K207, "EPhoneViewSetQwertyModeAndOrientation");
_STLIT8(K208, "EPhoneViewIsCallWaiting");
_STLIT8(K209, "EPhoneViewLockCallUiOrientationIfNeeded");
_STLIT8(K210, ", // Don't remove this, this should always be the last in list.");

// Intermediate
const void * const KStringPointers[] =
	{
	(const void*)&K1,
	(const void*)&K2,
	(const void*)&K3,
	(const void*)&K4,
	(const void*)&K5,
	(const void*)&K6,
	(const void*)&K7,
	(const void*)&K8,
	(const void*)&K9,
	(const void*)&K10,
	(const void*)&K11,
	(const void*)&K12,
	(const void*)&K13,
	(const void*)&K14,
	(const void*)&K15,
	(const void*)&K16,
	(const void*)&K17,
	(const void*)&K18,
	(const void*)&K19,
	(const void*)&K20,
	(const void*)&K21,
	(const void*)&K22,
	(const void*)&K23,
	(const void*)&K24,
	(const void*)&K25,
	(const void*)&K26,
	(const void*)&K27,
	(const void*)&K28,
	(const void*)&K29,
	(const void*)&K30,
	(const void*)&K31,
	(const void*)&K32,
	(const void*)&K33,
	(const void*)&K34,
	(const void*)&K35,
	(const void*)&K36,
	(const void*)&K37,
	(const void*)&K38,
	(const void*)&K39,
	(const void*)&K40,
	(const void*)&K41,
	(const void*)&K42,
	(const void*)&K43,
	(const void*)&K44,
	(const void*)&K45,
	(const void*)&K46,
	(const void*)&K47,
	(const void*)&K48,
	(const void*)&K49,
	(const void*)&K50,
	(const void*)&K51,
	(const void*)&K52,
	(const void*)&K53,
	(const void*)&K54,
	(const void*)&K55,
	(const void*)&K56,
	(const void*)&K57,
	(const void*)&K58,
	(const void*)&K59,
	(const void*)&K60,
	(const void*)&K61,
	(const void*)&K62,
	(const void*)&K63,
	(const void*)&K64,
	(const void*)&K65,
	(const void*)&K66,
	(const void*)&K67,
	(const void*)&K68,
	(const void*)&K69,
	(const void*)&K70,
	(const void*)&K71,
	(const void*)&K72,
	(const void*)&K73,
	(const void*)&K74,
	(const void*)&K75,
	(const void*)&K76,
	(const void*)&K77,
	(const void*)&K78,
	(const void*)&K79,
	(const void*)&K80,
	(const void*)&K81,
	(const void*)&K82,
	(const void*)&K83,
	(const void*)&K84,
	(const void*)&K85,
	(const void*)&K86,
	(const void*)&K87,
	(const void*)&K88,
	(const void*)&K89,
	(const void*)&K90,
	(const void*)&K91,
	(const void*)&K92,
	(const void*)&K93,
	(const void*)&K94,
	(const void*)&K95,
	(const void*)&K96,
	(const void*)&K97,
	(const void*)&K98,
	(const void*)&K99,
	(const void*)&K100,
	(const void*)&K101,
	(const void*)&K102,
	(const void*)&K103,
	(const void*)&K104,
	(const void*)&K105,
	(const void*)&K106,
	(const void*)&K107,
	(const void*)&K108,
	(const void*)&K109,
	(const void*)&K110,
	(const void*)&K111,
	(const void*)&K112,
	(const void*)&K113,
	(const void*)&K114,
	(const void*)&K115,
	(const void*)&K116,
	(const void*)&K117,
	(const void*)&K118,
	(const void*)&K119,
	(const void*)&K120,
	(const void*)&K121,
	(const void*)&K122,
	(const void*)&K123,
	(const void*)&K124,
	(const void*)&K125,
	(const void*)&K126,
	(const void*)&K127,
	(const void*)&K128,
	(const void*)&K129,
	(const void*)&K130,
	(const void*)&K131,
	(const void*)&K132,
	(const void*)&K133,
	(const void*)&K134,
	(const void*)&K135,
	(const void*)&K136,
	(const void*)&K137,
	(const void*)&K138,
	(const void*)&K139,
	(const void*)&K140,
	(const void*)&K141,
	(const void*)&K142,
	(const void*)&K143,
	(const void*)&K144,
	(const void*)&K145,
	(const void*)&K146,
	(const void*)&K147,
	(const void*)&K148,
	(const void*)&K149,
	(const void*)&K150,
	(const void*)&K151,
	(const void*)&K152,
	(const void*)&K153,
	(const void*)&K154,
	(const void*)&K155,
	(const void*)&K156,
	(const void*)&K157,
	(const void*)&K158,
	(const void*)&K159,
	(const void*)&K160,
	(const void*)&K161,
	(const void*)&K162,
	(const void*)&K163,
	(const void*)&K164,
	(const void*)&K165,
	(const void*)&K166,
	(const void*)&K167,
	(const void*)&K168,
	(const void*)&K169,
	(const void*)&K170,
	(const void*)&K171,
	(const void*)&K172,
	(const void*)&K173,
	(const void*)&K174,
	(const void*)&K175,
	(const void*)&K176,
	(const void*)&K177,
	(const void*)&K178,
	(const void*)&K179,
	(const void*)&K180,
	(const void*)&K181,
	(const void*)&K182,
	(const void*)&K183,
	(const void*)&K184,
	(const void*)&K185,
	(const void*)&K186,
	(const void*)&K187,
	(const void*)&K188,
	(const void*)&K189,
	(const void*)&K190,
	(const void*)&K191,
	(const void*)&K192,
	(const void*)&K193,
	(const void*)&K194,
	(const void*)&K195,
	(const void*)&K196,
	(const void*)&K197,
	(const void*)&K198,
	(const void*)&K199,
	(const void*)&K200,
	(const void*)&K201,
	(const void*)&K202,
	(const void*)&K203,
	(const void*)&K204,
	(const void*)&K205,
	(const void*)&K206,
	(const void*)&K207,
	(const void*)&K208,
	(const void*)&K209,
	(const void*)&K210
	};

const TStringTable PhoneLoggerviewCommands::Table = {210, KStringPointers, EFalse};

