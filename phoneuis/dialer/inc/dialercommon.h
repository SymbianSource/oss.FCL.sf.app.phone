/*
* Copyright (c) 2007 - 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of common constants of the dialer.
*
*/


#ifndef DIALERCOMMON_H
#define DIALERCOMMON_H

#include <e32def.h>
#include <e32keys.h>

#ifndef __DIALER_MBG
#define __DIALER_MBG
#include "dialer.mbg"
#endif // __DIALER_MBG

#include "dialer.hrh"

const TInt KDialerPhoneNumberEntryBufferSize = 100;
const TUid KPhoneUidAppPhone = { 0x100058B3 };

// ROM drive.
_LIT( KDriveZ, "z:" );

// Application name literal for panic macro
_LIT( KModuleName, "Dialer" );

// Panic macro
inline void DialerPanic( TInt aReason )
    {
    User::Panic( KModuleName, aReason );
    }

// Panic codes
enum TDialerPanicCode 
    {
    EDialerPanicDrawingError = 100,
    EDialerPanicNoApplicationInstance,
    EDialerPanicUnknownScript,
    EDialerPanicChineseScriptError,
    EDialerPanicLabelNotFound,
    EDialerPanicToolbarButtonNotFound,
    EDialerPanicNullController
    };

#endif // DIALERCOMMON_H

// End of file
