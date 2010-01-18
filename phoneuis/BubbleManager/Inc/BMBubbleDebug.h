/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Macro definitions for tracing and debugging purposes.
*
*/


#ifndef BM_DEBUG_H
#define BM_DEBUG_H

//#ifdef _DEBUG 

// INCLUDES
#include <e32svr.h>

// CONSTANTS
_LIT( KComponent, "[BUBBLEMANAGER]" );
_LIT( KThisFile,  "[BUBBLEMANAGER] - Trace this file: %s, line: %d, compiled: %s %s" );
_LIT( KAssertion, "[BUBBLEMANAGER] - Assertion failed: File: %s, line: %d, compiled: %s %s" );
_LIT( KPanic,     "[BUBBLEMANAGER] - Panic occurred: File: %s, line: %d, compiled: %s %s" );
_LIT( KMystic,    "[BUBBLEMANAGER] - Mystic failure: File: %s, line: %d, compiled: %s %s" );
_LIT8( KDATE, __DATE__ );
_LIT8( KTIME, __TIME__ );

const TInt KTraceMaxSize = 256;

// DATA TYPES
enum TTraceType
    {
    ETraceInit,
    ETraceAssert,
    ETracePanic
    };

// INLINE FUNCTIONS
 
 

    #define BM_TRACE_( AAA ) do\
        { _LIT( logStr, AAA ); RDebug::Print( logStr ); } while ( 0 )
    
    #define BM_TRACE_1( AAA, BBB ) do\
         { _LIT( logStr, AAA ); RDebug::Print( logStr, BBB ); } while ( 0 )
    
    #define BM_TRACE_2( AAA, BBB, CCC ) do\
         { _LIT( logStr, AAA ); RDebug::Print( logStr, BBB, CCC ); } while ( 0 )
    
    #define BM_TRACE_3( AAA, BBB, CCC, DDD ) do\
         { _LIT( logStr, AAA ); RDebug::Print( logStr, BBB, CCC, DDD ); } while ( 0 )
    
    #define BM_TRACE_4( AAA, BBB, CCC, DDD, EEE ) do\
         { _LIT( logStr, AAA ); RDebug::Print( logStr, BBB, CCC, DDD, EEE ); } while ( 0 )
    
    #define BM_TRACE_RAW_( AAA ) do\
        { RDebug::RawPrint( AAA ); } while ( 0 )
        
    #define BM_TRACE_RAW_1( AAA, BBB ) do\
         { _LIT( logStr, AAA ); TBuf<KTraceMaxSize> buffer; buffer.Append( logStr ); buffer.Append( BBB );\
         RDebug::RawPrint( buffer ); } while ( 0 )
       

    

    
/*
#else // _DEBUG

    #define TRACE_ASSERT( a )
    #define TRACE_ASSERT_RETURN( a ) a
    #define TRACE_ASSERT_ALWAYS

    #define BM_TRACE_( AAA )
    #define BM_TRACE_1( AAA, BBB )
    #define BM_TRACE_2( AAA, BBB, CCC )
    #define BM_TRACE_3( AAA, BBB, CCC, DDD )
    #define BM_TRACE_4( AAA, BBB, CCC, DDD, EEE )
    #define BM_TRACE_RAW_( AAA )
    #define BM_TRACE_RAW_1( AAA, BBB )

    #define API_TRACE_( AAA )
    #define API_TRACE_1( AAA, BBB )
    #define API_TRACE_2( AAA, BBB, CCC )
    #define API_TRACE_3( AAA, BBB, CCC, DDD )
    #define API_TRACE_4( AAA, BBB, CCC, DDD, EEE )
    #define API_TRACE_RAW_1( AAA, BBB )

    #define COMPONENT_TRACE_THIS_FILE

    #define PANIC_IF_FALSE( a )
    #define PANIC_IF_TRUE( a )
    #define PANIC_ALWAYS

#endif // _DEBUG
*/
#endif // BM_DEBUG_H

// End of File
