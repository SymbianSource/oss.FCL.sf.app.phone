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
    EPhoneViewRemoveNumberEntry = KPhoneViewCommandCommonFirst,
    EPhoneViewSetNumberEntryVisible,
    EPhoneViewGetNumberFromEntry,
    EPhoneViewGetNumberEntryCount,
    EPhoneViewGetNumberEntryIsUsedStatus,
    EPhoneViewGetNumberEntryIsVisibleStatus,
    EPhoneViewClearNumberEntryContent,
    EPhoneViewCreateCallHeader,
    EPhoneViewCreateEmergencyCallHeader,
    EPhoneViewUpdateCallHeaderRemoteInfoData,
    EPhoneViewUpdateCallHeaderCallDuration,
    EPhoneViewRemoveCallHeader,
    EPhoneViewUpdateCba,
    EPhoneViewUpdateBubble,
    EPhoneViewActivateAudioPathUIChanges,
    EPhoneViewActivateAudioAvailabilityUIChanges,
    EPhoneViewActivateMuteUIChanges,
    EPhoneViewShowNote,
    EPhoneViewShowGlobalNote,
    EPhoneViewShowProgressNote,
    EPhoneViewRemoveNote,
    EPhoneViewRemoveProgressNote,
    EPhoneViewRemoveGlobalNote,
    EPhoneViewShowQuery,
    EPhoneViewSetListQueryString, 
    EPhoneViewRemoveQuery,
    EPhoneViewSetTopApplication,
    EPhoneViewBringAppToForeground,
    EPhoneViewSendToBackground,
    EPhoneViewSetNeedToSendToBackgroundStatus, 
    EPhoneViewGetNeedToSendToBackgroundStatus,
    EPhoneViewGetTopApplicationIsDisplayedStatus, 
    EPhoneViewGetForegroundApplication, 
    EPhoneViewStartCapturingKey,
    EPhoneViewStopCapturingKey,
    EPhoneViewSetGlobalNotifiersDisabled,
    EPhoneViewGetHoldFlag,
    EPhoneViewSetHoldFlag,
    EPhoneViewRemovePhoneDialogs,
    EPhoneViewCreateConference,
    EPhoneViewAddToConference,
    EPhoneViewRemoveFromConference,
    EPhoneViewPrivateFromConference,
    EPhoneViewSelectedConfMember,
    EPhoneViewGetCallIdByState,
    EPhoneViewGetCountOfActiveCalls,
    EPhoneViewSetNaviPaneAudioVolume,
    EPhoneViewHideNaviPaneAudioVolume,
    EPhoneViewGetIsConference,
    EPhoneViewGetCallExistsInConference,
    EPhoneViewRemoveAllCallHeaders,
    EPhoneViewPlayRingTone,
    EPhoneViewMuteRingTone,
    EPhoneViewStopRingTone,
    EPhoneViewMuteRingToneOnAnswer,
    EPhoneViewCipheringInfoChange,
    EPhoneViewCloseFSW, 
    EPhoneViewLaunchRfsDeep, 
    EPhoneViewLaunchRfsNormal, 
    EPhoneViewOpenSoftRejectEditor,
    EPhoneViewSetSecurityMode, 
    EPhoneViewGetSecurityModeStatus, 
    EPhoneViewEnableKeyLock, 
    EPhoneViewEnableKeyLockWithoutNote, 
    EPhoneViewUpdateFSW,
    EPhoneViewIsQuery, 
    EPhoneViewBringIdleToForeground, 
    EPhoneViewSetIdleTopApplication,  
    EPhoneViewBeginUpdate,
    EPhoneViewEndUpdate,
    EPhoneViewRemoveConferenceBubble,
    EPhoneViewSetTouchPaneButtons, 
    EPhoneViewGetBlockingDialogStatus, 
    EPhoneViewIsNoteActive, 
    EPhoneViewOpenCallHandling, 
    EPhoneViewGetAudioVolumeLevel,
    EPhoneViewUpdateCallHeaderRemoteInfoDataAndLabel,
    EPhoneViewRemoveDtmfNote,
    EPhoneViewSetEikonNotifiersDisabled, 
    EPhoneViewCancelAllNotications,   
    EPhoneViewDisableKeyLock, 
    EPhoneViewSendAiwCommand, 
    
    EPhoneViewCipheringInfoChangePlayTone, 
    EPhoneViewDisableKeyLockWithoutNote, 
    
    EPhoneViewGetKeyLockStatus, 
    
    EPhoneViewRemoveGlobalWaitNote,
    EPhoneViewSetPhoneCustomization, 
    EPhoneViewSetViewCustomization, 
    EPhoneViewSetButtonCustomization, 
    
    EPhoneViewUpdateContactByString, 
    EPhoneViewAddContactByString, 
    
    EPhoneViewIsActiveNoteDissmissableByKeyEvent, 
    
    EPhoneViewEnableToolbarButton, 
    EPhoneViewHideToolbar, 
    EPhoneViewShowToolbar, 
    EPhoneViewGetExpandedBubbleCallId,
    EPhoneViewOpenDialer,
    EPhoneSetConferenceExpanded,
    EPhoneAppShutDown,
    EPhoneViewBackButtonActive,
    EPhoneViewHsToForegroundAfterCall,
    EPhoneViewOpenContacts,
    EPhoneViewSetIhfFlag,
    EPhoneViewSetMuteFlag,
    EPhoneViewLaunchLogs,
    EPhoneViewSetBlueToothFlag,
    EPhoneViewSetBluetoothAvailableFlag,
    EPhoneViewSetSoftRejectDimmed,
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
