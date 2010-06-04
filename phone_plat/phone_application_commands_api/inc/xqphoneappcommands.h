/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*     This file contains declarations for call commands of Phone Application.
*     
*
*/


#ifndef XQPHONEAPPCOMMANDS_H
#define XQPHONEAPPCOMMANDS_H

//  MENU COMMANDS
enum PhoneMenuCommands
    {
    // Commands for incall menu
    PhoneInCallCmdEndAllCalls,
    PhoneInCallCmdTransfer,
    PhoneInCallCmdSendMessage,
    PhoneInCallShareVideo,
    PhoneInCallCmdSwitchToVideo,
    PhoneInCallCmdHandoverToWlan,
    PhoneInCallCmdHandoverToGsm,
    
    PhoneMenuCmdLast 
    };

//  PUSH BUTTON COMMANDS
enum PhonePushButtonCommands
    {
    // Push button commands for incall
    PhoneInCallCmdJoinToConference = PhoneMenuCmdLast,
    PhoneInCallCmdCreateConference,
    PhoneInCallCmdAnswer,
    PhoneInCallCmdReject,
    PhoneInCallCmdHold,
    PhoneInCallCmdUnhold,
    PhoneInCallCmdEndActive,
    PhoneInCallCmdEndOutgoingCall,
    PhoneInCallCmdReplace,
    PhoneInCallCmdSwap,
    
    PhonePushButtonCmdLast
    };
    
#endif

// End of File
