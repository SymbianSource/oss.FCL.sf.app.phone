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
* Description:  Easy dialing logging functions.
*
*/


#ifndef __EASYDIALINGLOGGER_H__
#define __EASYDIALINGLOGGER_H__

// Note! These macros are to be used in trace prints (legacy support).
//       To be used only in error situations and major events only !
//       Otherwise use OST - Open System Trace

#ifdef _DEBUG

// #define USE_FILE_LOGGING

// From this point on, internal implementation

#include <e32svr.h>     // for RDebug::Print

#ifdef USE_FILE_LOGGING
#include <flogger.h>
_LIT( KLogDir,  "ed" );
_LIT( KLogFile, "easydialing.log" );
#endif


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
    _LIT(KLogPrintFormat,"%S: %S");                         // Log printing format
    const TDesC& thname = RThread().Name();                 // With FullName more info
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


// Macros

#define LOGSTRING( TEXT ) DebugPrintF( _L( TEXT ) )
#define LOGSTRING1( TEXT, VAR1 ) DebugPrintF( _L( TEXT ), VAR1 )
#define LOGSTRING2( TEXT, VAR1, VAR2 ) DebugPrintF( _L( TEXT ), VAR1, VAR2 )

#define PERF_MEASURE_START \
        TTime startTime, endTime; \
        startTime.HomeTime();\
        DebugPrintF( _L("EasyDialingPlugin: Measure start..") );


#define PERF_MEASURE_STOP \
        endTime.HomeTime(); \
        TTimeIntervalMicroSeconds microsecs = endTime.MicroSecondsFrom( startTime ); \
        TReal timeSecs=(TReal)((TReal)(microsecs.Int64())/1000); \
        DebugPrintF( _L("EasyDialingPlugin: Measure stop, time taken %5.3f msecs"), timeSecs );

#else

#define LOGSTRING( TEXT )
#define LOGSTRING1( TEXT, VAR1 )
#define LOGSTRING2( TEXT, VAR1, VAR2 )

#define PERF_MEASURE_START
#define PERF_MEASURE_STOP

#endif

#endif

