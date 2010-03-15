/*
* ============================================================================
*  Name     : phoneloggerviewcommands.cpp
*  Part of  : PhoneUI / phoneuiutils.dll
*
*  Description:
*     Macro definition file for logging.
*  Version:
*
*  Copyright � 2007 Nokia Corporation.
*  This material, including documentation and any related 
*  computer programs, is protected by copyright controlled by 
*  Nokia Corporation. All rights are reserved. Copying, 
*  including reproducing, storing,  adapting or translating, any 
*  or all of this material requires the prior written consent of 
*  Nokia Corporation. This material also contains confidential 
*  information which may not be disclosed to others without the 
*  prior written consent of Nokia Corporation.
* ============================================================================
*/
// Autogenerated from phoneloggerviewcommands.st by the stringtable tool - Do not edit
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
_STLIT8(K83, "EPhoneViewOpenConferenceList");
_STLIT8(K84, "EPhoneViewGetConferenceListVisibleFlag");
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
_STLIT8(K109, "EPhoneViewSetSecurityMode");
_STLIT8(K110, "EPhoneViewGetSecurityModeStatus");
_STLIT8(K111, "EPhoneViewSetStatusPaneVisible");
_STLIT8(K112, "EPhoneViewEnableKeyLock");
_STLIT8(K113, "EPhoneViewLaunchFaxModem");
_STLIT8(K114, "EPhoneViewUpdateFSW");
_STLIT8(K115, "EPhoneViewIsQuery");
_STLIT8(K116, "EPhoneViewBringIdleToForeground");
_STLIT8(K117, "EPhoneViewSetIdleTopApplication");
_STLIT8(K118, "EPhoneViewBeginUpdate");
_STLIT8(K119, "EPhoneViewEndUpdate");
_STLIT8(K120, "EPhoneViewSetConfrenceOptionsFlag");
_STLIT8(K121, "EPhoneViewRemoveConferenceBubble");
_STLIT8(K122, "EPhoneViewGetSoftRejectWindowGroupId");
_STLIT8(K123, "EPhoneViewSetTouchPaneButtons");
_STLIT8(K124, "EPhoneViewSetTouchPaneVisible");
_STLIT8(K125, "EPhoneViewEnableTouchButton");
_STLIT8(K126, "EPhoneViewDisableTouchButton");
_STLIT8(K127, "EPhoneResetTouchButtons");
_STLIT8(K128, "EPhoneViewGetIsConferenceInSelectionMode");
_STLIT8(K129, "EPhoneViewIsStraightCallToVideoMailBox");
_STLIT8(K130, "EPhoneViewSetDtmfDialerViewVisible");
_STLIT8(K131, "EPhoneViewSetRestrictedDialer");
_STLIT8(K132, "EPhoneViewIsConferenceInExpandedMode");
_STLIT8(K133, "EPhoneViewIsMenuBarVisible");
_STLIT8(K134, "EPhoneViewGetBlockingDialogStatus");
_STLIT8(K135, "EPhoneViewYesSingleItemFetch");
_STLIT8(K136, "EPhoneViewNoSingleItemFetch");
_STLIT8(K137, "EPhoneViewGetLocalizedNumberFromEntry");
_STLIT8(K138, "EPhoneViewIsNoteActive");
_STLIT8(K139, "EPhoneViewOpenNumberEntry");
_STLIT8(K140, "EPhoneViewOpenCallHandling");
_STLIT8(K141, "EPhoneViewGetConferenceAndSingleFlag");
_STLIT8(K142, "EPhoneViewSetConferenceAndSingleFlag");
_STLIT8(K143, "EPhoneViewSetVideoCallFlag");
_STLIT8(K144, "EPhoneViewSetDtmfOptionsFlag");
_STLIT8(K145, "EPhoneViewShowVTSetupFailedCreateCallToSameContactQuery");
_STLIT8(K146, "EPhoneViewGetAudioVolumeLevel");
_STLIT8(K147, "EPhoneViewIsIdleTopApp");
_STLIT8(K148, "EPhoneViewUpdateToolbar");
_STLIT8(K149, "EPhoneViewSetControlAndVisibility");
_STLIT8(K150, "EPhoneViewAllowWaitingCallHeader");
_STLIT8(K151, "EPhoneViewBeginTransEffect");
_STLIT8(K152, "EPhoneViewEndTransEffect");
_STLIT8(K153, "EPhoneViewSetDialerControlVisible");
_STLIT8(K154, "EPhoneViewCloseSingleItemFetchDialog");
_STLIT8(K155, "EPhoneViewGetActivatePreviousApp");
_STLIT8(K156, "EPhoneViewActivatePreviousApp");
_STLIT8(K157, "EPhoneViewSetPointerCapture");
_STLIT8(K158, "EPhoneViewSetSendKeyDialerActivationFlag");
_STLIT8(K159, "EPhoneViewPrepareIcons");
_STLIT8(K160, "EPhoneViewKeypadAudioEnabled");
_STLIT8(K161, "EPhoneViewKeypadAudioDisabled");
_STLIT8(K162, "EPhoneViewUpdateCallHeaderRemoteInfoDataAndLabel");
_STLIT8(K163, "EPhoneViewRemoveDtmfNote");
_STLIT8(K164, "EPhoneViewUpdatePhoneIconToFSW");
_STLIT8(K165, "EPhoneViewSetEikonNotifiersDisabled");
_STLIT8(K166, "EPhoneViewCancelAllNotications");
_STLIT8(K167, "EPhoneViewSetVideoCallDTMFVisibilityFlag");
_STLIT8(K168, "EPhoneViewLoadPlugins");
_STLIT8(K169, "PhoneViewServiceSelection");
_STLIT8(K170, "EPhoneViewOpenVirtualKeyBoard");
_STLIT8(K171, "EPhoneViewUpdateNoteSoftkeys");
_STLIT8(K172, "EPhoneViewShowCustomizedDialer");
_STLIT8(K173, "EPhoneViewHideCustomizedDialer");
_STLIT8(K174, "EPhoneViewGetCustomizedDialerMenuResourceId");
_STLIT8(K175, "EPhoneViewGetCustomizedDialerCbaResourceId");
_STLIT8(K176, "EPhoneViewCreatePhoneBookServices");
_STLIT8(K177, "EPhoneViewDisableKeyLockWithoutNote");
_STLIT8(K178, "EPhoneViewGetKeyLockStatus");
_STLIT8(K179, "EPhoneViewSetNumberEntryObserver");
_STLIT8(K180, "EPhoneViewGetSingleItemFetchType");
_STLIT8(K181, "EPhoneViewRemoveGlobalWaitNote");
_STLIT8(K182, "EPhoneViewSetPhoneCustomization");
_STLIT8(K183, "EPhoneViewSetViewCustomization");
_STLIT8(K184, "EPhoneViewSetButtonCustomization");
_STLIT8(K185, "EPhoneViewUpdateContactByString");
_STLIT8(K186, "EPhoneViewAddContactByString");
_STLIT8(K187, "EPhoneViewIsActiveNoteDissmissableByKeyEvent");
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
_STLIT8(K201, ", // Don't remove this, this should always be the last in list.");

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
	(const void*)&K201
	};

const TStringTable PhoneLoggerviewCommands::Table = {201, KStringPointers, EFalse};

