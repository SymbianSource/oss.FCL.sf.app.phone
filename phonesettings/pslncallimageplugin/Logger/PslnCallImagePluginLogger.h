/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Macro definition file for logging.
*
*
*/



#ifndef PSLNCALLIMAGEPLUGINLOGGER_H
#define PSLNCALLIMAGEPLUGINLOGGER_H

#include "PslnCallImagePluginLoggingConf.h"
#include <e32std.h>
#include <flogger.h>
#include <e32svr.h>

_LIT( KLogsDir,                  "CALLIMAGE");
_LIT( KLogFileName,              "CALLIMAGE.log");

/*
 We have 3 possible logging methods:

 set CALL_LOGGING_METHOD to 0 to get no logging at all
 set CALL_LOGGING_METHOD to 1 to get logging via RDebug::Print
 set CALL_LOGGING_METHOD to 2 to get logging to log file

 Switching on/off is done in the Configuration file (PslnCallImagePluginLoggingConf.h)
 */

#if ( defined (_CALLLOGGING) )
    #if ( defined (_CALL_LOGGING_TO_FILE))
        #define CALL_LOGGING_METHOD 2 
    #else
        #define CALL_LOGGING_METHOD 1
    #endif
#else
    #define CALL_LOGGING_METHOD 0
#endif


#if (CALL_LOGGING_METHOD==0)
    #define __CALLLOGSTRING(C)
    #define __CALLLOGSTRING1(C, X)
    #define __CALLLOGSTRING2(C, X, Y)
    #define __CALLLOGSTRING3(C, X, Y, Z)
#endif
#if (CALL_LOGGING_METHOD==1)
    #define __CALLLOGSTRING(C)            RDebug::Print(_L(C));
    #define __CALLLOGSTRING1(C, X)        RDebug::Print(_L(C),X);
    #define __CALLLOGSTRING2(C, X, Y)     RDebug::Print(_L(C),X, Y);
    #define __CALLLOGSTRING3(C, X, Y, Z)  RDebug::Print(_L(C),X, Y, Z);
#endif
#if (CALL_LOGGING_METHOD==2)
    #define __CALLLOGSTRING(C)            {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, tempLogDes());}
    #define __CALLLOGSTRING1(C, X)        {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X);}
    #define __CALLLOGSTRING2(C, X, Y)     {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X,Y);}
    #define __CALLLOGSTRING3(C, X, Y, Z)  {_LIT(tempLogDes,C);RFileLogger::WriteFormat(KLogsDir(),KLogFileName(), EFileLoggingModeAppend, TRefByValue<const TDesC>(tempLogDes()),X,Y,Z);}
#endif

#endif // PSLNCALLIMAGEPLUGINLOGGER_H
