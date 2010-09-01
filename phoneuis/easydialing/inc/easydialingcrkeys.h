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
* Description:  Easy dialing central repository keys.
*
*/


#ifndef __EASYDIALINGCRKEYS_H__
#define __EASYDIALINGCRKEYS_H__

#include <e32cmn.h>

/**
* Easydialing settings CR UID (same as easydialing DLL uid)
*/
const TUid KCRUidEasyDialSettings = {0x200212A0};

/**
* Easydialing on/off
*
* Possible values are:
* 0 = Easydialing is off
* 1 = Easydialing is on
*/
const TUint32 KEasyDialing = 0x00000001;

/**
* Easydialing Contact Thumbnails show/hide
* Possible values are:
* 0 = Thumbnails are hidden
* 1 = Thumbnails are shown
*/
const TUint32 KEasyDialingContactThumbnails = 0x00000002;

#endif //__EASYDIALINGCRKEYS_H__

