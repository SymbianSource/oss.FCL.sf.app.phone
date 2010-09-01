/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Telephony information Publish and Subscribe keys.
*
*/


#ifndef TELINFORMATIONPSKEYS_H
#define TELINFORMATIONPSKEYS_H

#include <e32std.h>

// CONSTANTS

/*****************************************************************************
* Telephony Information API UID
*****************************************************************************/
const TUid KPSUidTelInformation = {0x101F8789};

/**
* Identifier of Phone application.
* @type RProperty::EInt
*/
const TUint32 KTelPhoneUid = 0x00000001; 

/**
* Operator information display data.
* @see TTelTitleDisplay in TelephonyDomainPSTypes.h
* @type RProperty::EByteArray
*/

const TUint32 KTelDisplayInfo =  0x00000002; 

#endif      // TELINFORMATIONPSKEYS_H

// End of file
