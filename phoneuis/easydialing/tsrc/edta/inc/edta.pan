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
* Description:  Easy dialing test application.
*
*/

#ifndef __EDTA_PAN__
#define __EDTA_PAN__

#include "edta_debugprint.h"

/** Edta application panic codes */
enum TEdtaPanics
    {
    EEdtaUi = 1,
    EEdtaSkinChangePanic = 2,
    EEdtaCalculatePanic = 3,
    EEdtaFieldNotInitialized = 4
    // add further panics here
    };

_LIT(KEdtaApplicationName,"Edta");

inline void Panic(TEdtaPanics aReason)
    {
    DebugPrintF(_L("Panic reason: %d"), (TInt) aReason);
    User::Panic(KEdtaApplicationName, aReason);
    }

inline void Panic(TEdtaPanics aReason, TDesC aText)
    {
    DebugPrintF(_L("Panic reason: %d, '%S'"), (TInt) aReason, &aText );
    User::Panic(KEdtaApplicationName, aReason);
    }

#endif // __EDTA_PAN__
