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
*
*
*/

// INCLUDE FILES
#include <e32base.h>
#include <cphcltussdint.h>

#include "ussdcomms.h"
#include "tflogger.h"

// The maximum length of editor lines.
const TInt KUssdEditorMaxLength = 182;

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CUssdComms::CUssdComms
// Constructor.
// -----------------------------------------------------------------------------
//
CUssdComms::CUssdComms(QObject *parent)
    : QObject(parent),
      mUssd(NULL), 
      mErrorCode(static_cast<int>(EPhCltExitReasonUnknown))
{
    // Default value - ETrue
    TRAP_IGNORE(mUssd = CPhCltUssdInt::NewL());
    TFLOGSTRING("USSDEDITOR: CUssdComms::CUssdComms")
}

// -----------------------------------------------------------------------------
// CUssdComms::~CUssdComms
// ~CUssdComms.
// -----------------------------------------------------------------------------
//
CUssdComms::~CUssdComms()
{
    if (mUssd){
        mErrorCode = mUssd->AppTerminating(
            static_cast<TPhCltUssdAppExitReason>(mErrorCode));
        delete mUssd;
        mUssd = NULL;
        TFLOGSTRING2("USSDEDITOR: CUssdComms::~CUssdComms %d", mErrorCode)
    }
    TFLOGSTRING("USSDEDITOR: CUssdComms::~CUssdComms")
}

// -----------------------------------------------------------------------------
// CUssdComms::appStarting
// appStarting.
// -----------------------------------------------------------------------------
//
void CUssdComms::appStarting()
{
    mErrorCode = mUssd->AppStarting();
    TFLOGSTRING("USSDEDITOR: CUssdComms::appStarting")
}

// -----------------------------------------------------------------------------
// CUssdComms::appTerminating
// appTerminating.
// -----------------------------------------------------------------------------
//
void CUssdComms::appTerminating()
{
    mErrorCode = mUssd->AppTerminating(EPhCltUserExit);
    TFLOGSTRING("USSDEDITOR: CUssdComms::appTerminating")
}

// -----------------------------------------------------------------------------
// CUssdComms::appToBackground
// appToBackground.
// -----------------------------------------------------------------------------
//
void CUssdComms::appToBackground()
{
    mErrorCode = mUssd->AppToBackground();
    TFLOGSTRING("USSDEDITOR: CUssdComms::appToBackground")
}

// -----------------------------------------------------------------------------
// CUssdComms::appToForeground
// appToForeground.
// -----------------------------------------------------------------------------
//
void CUssdComms::appToForeground()
{
    mErrorCode = mUssd->AppToForeground();
    TFLOGSTRING("USSDEDITOR: CUssdComms::appToForeground")
}

// -----------------------------------------------------------------------------
// CUssdComms::send
// send.
// -----------------------------------------------------------------------------
//
int CUssdComms::send( const QString& message )
{
    TBuf<KUssdEditorMaxLength> buf16;
    buf16.Copy((const TUint16*)message.constData(), message.length());
    mErrorCode = mUssd->SendUssd(buf16);
    TFLOGSTRING("USSDEDITOR: CUssdComms::send")
    return mErrorCode;
}

// -----------------------------------------------------------------------------
// CUssdComms::cancel
// cancel.
// -----------------------------------------------------------------------------
//
void CUssdComms::cancel()
{
    mUssd->SendUssdCancel();
    TFLOGSTRING("USSDEDITOR: CUssdComms::cancel")
}

// -----------------------------------------------------------------------------
// CUssdComms::errorCode
// errorCode.
// -----------------------------------------------------------------------------
//
int CUssdComms::errorCode()
{
    TFLOGSTRING("USSDEDITOR: CUssdComms::send")
    return mErrorCode;
}

// -----------------------------------------------------------------------------
// CUssdComms::informExitReason
// informExitReason.
// -----------------------------------------------------------------------------
//
void CUssdComms::informExitReason(int aExitReason)
{
    TFLOGSTRING2("USSDEDITOR: CUssdComms::informExitReason \
        %d", aExitReason) 
    mErrorCode = aExitReason;
}

// End of file
