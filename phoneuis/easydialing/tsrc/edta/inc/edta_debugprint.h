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
* Description:  Easy dialing test application.
*
*/


#ifndef __EDTADEBUGPRINT_H__
#define __EDTADEBUGPRINT_H__

// If you want file logging, uncomment the next define.

#ifdef _DEBUG
// #define USE_FILE_LOGGING
#endif // _DEBUG

#ifdef USE_FILE_LOGGING
#include <flogger.h>
_LIT( KLogDir,  "ed" );
_LIT( KLogFile, "edta.log" );
#endif


// From this point on, internal implementation

NONSHARABLE_CLASS(TOverflowTruncate) : public TDes16Overflow
	{
	public:
	    void Overflow(TDes16& /*aDes*/) {}
	};

inline void DoDebugPrintF(TRefByValue<const TDesC> aFmt, VA_LIST& aList)
    {
    TBuf<200> buf;
	TOverflowTruncate overFlow;
    buf.AppendFormatList( aFmt, aList, &overFlow );
    _LIT(KLogPrintFormat,"%S: %S");   // Log printing format 
    const TDesC& thname = RThread().Name();                 // FullName
    RDebug::Print( KLogPrintFormat, &thname, &buf );
#ifdef USE_FILE_LOGGING
    RFileLogger::Write(KLogDir, KLogFile, EFileLoggingModeAppend, buf);
#endif
    }

inline void DebugPrintF(const TRefByValue<const TDesC> aFmt,...)
    {
    VA_LIST list;
    VA_START(list,aFmt);
    DoDebugPrintF(aFmt, list);
    }


#ifdef _DEBUG

// These macros are to be used in debug prints
#define DEBUGPRINT(x)           DebugPrintF(x)
#define DEBUGPRINT_2(x,y)       DebugPrintF(x,y)
#define DEBUGPRINT_3(x,y,z)     DebugPrintF(x,y,z)
#define DEBUGPRINT_4(x,y,z,a)   DebugPrintF(x,y,z,a)

#else // _DEBUG

#define DEBUGPRINT(x)
#define DEBUGPRINT_2(x,y)
#define DEBUGPRINT_3(x,y,z)
#define DEBUGPRINT_4(x,y,z,a)

#endif // _DEBUG

#endif // __EDTADEBUGPRINT_H__

// End of file
