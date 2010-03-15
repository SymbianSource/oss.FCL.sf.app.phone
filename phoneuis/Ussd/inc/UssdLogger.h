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
* Description:  Logger headerfile for CbsServer
*
*/

#ifndef USSDLOGGER_H
#define USSDLOGGER_H

/* 
-----------------------------------------------------------------------------
    LOGGIN MODE SELECTION
-----------------------------------------------------------------------------
*/
#ifndef _DEBUG
    // UREL BUILD:
    #define USSDUI_LOGGING_METHOD 0   // No logging in UREL builds
#else
    // UDEB BUILD:
    // 0 = No logging, 
    // 1 = Flogger, 
    // 2 = RDebug
    #define USSDUI_LOGGING_METHOD 2   
#endif //_DEBUG

/* 
-----------------------------------------------------------------------------
    FILE LOGGING SETTINGS
-----------------------------------------------------------------------------
*/
#if USSDUI_LOGGING_METHOD == 1      // Flogger
    #include <flogger.h>
    _LIT(KCbsLogFolder, "ussdui");
    _LIT(KCbsLogFile, "ussdui.txt");

#elif USSDUI_LOGGING_METHOD == 2    // RDebug

    #include <e32svr.h>

#endif //USSDUI_LOGGING_METHOD

/*
-----------------------------------------------------------------------------
    IMPLEMENTATION OF THE LOGGING MACROs
    USE THESE MACROS IN YOUR CODE
----------------------------------------------------------------------------
*/


#if USSDUI_LOGGING_METHOD == 1      // Flogger

#define _LOGTEXT(AAA) /*lint -save -e960 */ { RFileLogger::Write(KCbsLogFolder(),KCbsLogFile(),EFileLoggingModeAppend,AAA); } /*lint -restore */
#define _LOGSTRING(AAA) /*lint -save -e960 */ { _LIT(tempLogDes,AAA); RFileLogger::Write(KCbsLogFolder(),KCbsLogFile(),EFileLoggingModeAppend,tempLogDes()); } /*lint -restore */
#define _LOGSTRING2(AAA,BBB) /*lint -save -e960 -e437 */ { _LIT(tempLogDes,AAA); RFileLogger::WriteFormat(KCbsLogFolder(),KCbsLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(tempLogDes()),BBB); } /*lint -restore */
#define _LOGSTRING3(AAA,BBB,CCC) /*lint -save -e960 */ { _LIT(tempLogDes,AAA); RFileLogger::WriteFormat(KCbsLogFolder(),KCbsLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(tempLogDes()),BBB,CCC); } /*lint -restore */

#elif USSDUI_LOGGING_METHOD == 2    // RDebug

#define _LOGTEXT(AAA)              RDebug::Print(AAA);
#define _LOGSTRING(AAA)            RDebug::Print(_L(AAA));
#define _LOGSTRING2(AAA,BBB)       RDebug::Print(_L(AAA),BBB);
#define _LOGSTRING3(AAA,BBB,CCC)   RDebug::Print(_L(AAA),BBB,CCC);

#else    // USSDUI_LOGGING_METHOD == 0 or invalid

// Example: _LOGTEXT(own_desc);
#define _LOGTEXT(AAA)              
// Example: _LOGSTRING("Test");
#define _LOGSTRING(AAA)            
// Example: _LOGSTRING("Test %i", aValue);
#define _LOGSTRING2(AAA,BBB)       
// Example: _LOGSTRING("Test %i %i", aValue1, aValue2);
#define _LOGSTRING3(AAA,BBB,CCC)   

#endif // USSDUI_LOGGING_METHOD

#endif // USSDLOGGER_H
            
// End of File
