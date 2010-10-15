/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/
#include <QDebug>
#include <smcmockclassincludes.h>
#include "dialpadkeyhandler.h"

// ============================ MEMBER FUNCTIONS ===============================
class DialpadVoiceMailboxEventFilter {};
class DialpadVideoMailboxEventFilter {};
class DialpadBluetoothEventFilter {};
class DialpadKeySequenceEventFilter {};
class DialpadEmergencyCallEventFilter {};
class DialpadHashEventFilter {};
#ifdef _DEBUG
#ifdef __WINSCW__
class DialpadInternalEventFilter {};
#endif
#endif
// -----------------------------------------------------------------------------
// DialpadKeyHandler::DialpadKeyHandler
// -----------------------------------------------------------------------------
//
DialpadKeyHandler::DialpadKeyHandler( 
        Dialpad * dialPad,
        HbMainWindow & mainWindow,
        QObject * parent )
    : QObject( parent ), mMainWindow(mainWindow)
    {
    
    }


// -----------------------------------------------------------------------------
// DialpadKeyHandler::DialpadKeyHandler
// -----------------------------------------------------------------------------
//
DialpadKeyHandler::DialpadKeyHandler( 
        Dialpad * dialPad,
        DialpadKeyHandler::DialpadKeyEventFilters filters,
        QObject * parent )
    : QObject( parent ), mMainWindow(*(HbMainWindow *)(0))
    {
    
    }


// -----------------------------------------------------------------------------
// DialpadKeyHandler::~DialpadKeyHandler
// -----------------------------------------------------------------------------
//
DialpadKeyHandler::~DialpadKeyHandler(  )
    {
    
    }


