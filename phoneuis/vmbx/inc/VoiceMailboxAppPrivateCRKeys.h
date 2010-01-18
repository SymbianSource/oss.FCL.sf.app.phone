/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Private keys for Voice mailbox.
*
*/


#ifndef VOICEMAILBOXAPPPRIVATECRKEYS_H
#define VOICEMAILBOXAPPPRIVATECRKEYS_H
#include <e32std.h>

/** VoiceMailbox Settings API */
/** Provides access to the VoiceMailbox numbers and settings */

const TUid KCRUidVoiceMailbox = {0x101F874F};


/** Voice Message Date Stamp, a field in VMN 
*
* Text type
*
* Default value: ""
**/
const TUint32 KVmDateStamp                = 0x00000003;

/** 
* Voice message counter, a field in VMN to indicate how many messages are waiting on the network side 
*
* Integer type
*
* Default value: 0
**/
const TUint32  KVmMessageCounter            = 0x00000004;

/** 
* Voicemail Summary Info Set flag, a field in VMN 
*
* Integer type
*
* Possible values:
* 0 (not set)
* 1 (set)
*
* Default value: 0
**/
const TUint32  KVmSummaryInfoIsSet        = 0x00000005;

/** 
* Voice Message Time Stamp, a field in VMN 
*
* Text type
*
* Default value: ""
**/
const TUint32  KVmTimeStamp                = 0x00000006;

/**
* Voice Message Urgency Level, 
*
* Integer type
*
* Possible values:
* 0 (ENormal)
* 1 (EInteractive)
* 2 (EUrgent)
* 3 (EEmergency)
*
* Default value: 0
**/
const TUint32 KVmUrgencyLevel            = 0x00000007;

/** Voice Message User Data, VMS field 
*
* Text type
*
* Default value: ""
**/
const TUint32  KVmUserData                = 0x00000008;


/** Voice Message CDMA Operator Option to enable or disable UI for the user to
* view or modify his/her voicemail box number 
*
* Integer type
*
* Default value: 2
**/
const TUint32 KVmUIOpts                    = 0x00000010;

/** ring time index for i-Mode vmbx
*
* Integer type
*
* Default value: 2
*/
const TUint32 KVoiceMbxRingTimeIndex = 0x00000014;

#endif      // VOICEMAILBOXAPPPRIVATECRKEYS_H