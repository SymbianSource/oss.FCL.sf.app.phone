/*
 * Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Implements the class CSPAudioHandler
 *
 */

#include "cspaudiohandler.h"
#include "tmshandler.h"
#include "csplogger.h"

// ---------------------------------------------------------------------------
// CSPAudioHandler::NewL.
// ---------------------------------------------------------------------------
//
CSPAudioHandler* CSPAudioHandler::NewL()
    {
    CSPLOGSTRING(CSPOBJECT, "CSPAudioHandler::NewL()" );
    CSPAudioHandler* self = new (ELeave) CSPAudioHandler();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Destructs the object by canceling first ongoing monitoring.
// ---------------------------------------------------------------------------
//
CSPAudioHandler::~CSPAudioHandler()
    {
    CSPLOGSTRING(CSPOBJECT, "CSPAudioHandler::~CSPAudioHandler()" );
    delete iTmsHandler;
    }

// ---------------------------------------------------------------------------
// CSPAudioHandler::Start
// ---------------------------------------------------------------------------
//
void CSPAudioHandler::Start()
    {
    iCallCount++;
    CSPLOGSTRING2(CSPINT, "CSPAudioHandler::Start callcount: %d", iCallCount);

    if (iTmsHandler && iCallCount == 1)
        {
        iTmsHandler->StartStreams();
        }
    }

// ---------------------------------------------------------------------------
// CSPAudioHandler::Stop
// ---------------------------------------------------------------------------
//
void CSPAudioHandler::Stop()
    {
    CSPLOGSTRING2(CSPINT, "CSPAudioHandler::Stop callcount: %d", iCallCount);
    if (iTmsHandler && iCallCount == 1)
        {
        iTmsHandler->StopStreams();
        iCallCount--;
        }
    else if (iCallCount > 1)
        {
        iCallCount--;
        }
    }

// ---------------------------------------------------------------------------
// Constructs the monitor.
// ---------------------------------------------------------------------------
//
CSPAudioHandler::CSPAudioHandler()
    {
    CSPLOGSTRING(CSPOBJECT, "CSPAudioHandler::CSPAudioHandler()");
    iCallCount = 0; // Active calls count
    }

// ---------------------------------------------------------------------------
// Second phase construction.
// ---------------------------------------------------------------------------
//
void CSPAudioHandler::ConstructL()
    {
    if (!iTmsHandler)
        {
        iTmsHandler = TmsHandler::NewL();
        }
    }

// End of file
