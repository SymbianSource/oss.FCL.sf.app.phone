/*
* ============================================================================
*  Name        : phoneringingtonetrace.h
*  Part of     : PhoneRingingtone
*  Description : Macros for tracing.
*  Version     : 
*
*  Copyright © 2010 Nokia.  All rights reserved.
*  This material, including documentation and any related computer
*  programs, is protected by copyright controlled by Nokia.  All
*  rights are reserved.  Copying, including reproducing, storing,
*  adapting or translating, any or all of this material requires the
*  prior written consent of Nokia.  This material also contains
*  confidential information which may not be disclosed to others
*  without the prior written consent of Nokia.
* ============================================================================
* Template version: 4.1
*/


#ifndef PHONERINGINGTONETRACE_H
#define PHONERINGINGTONETRACE_H




// INCLUDES
#include <e32svr.h>

#if ( defined (_DEBUG) && !defined (__WINS__) )

// CONSTANTS

const TInt KPhoneRingingtoneDebugBufferSize = 300;

// MACROS

typedef TBuf<KPhoneRingingtoneDebugBufferSize> TPhonePhoneRingingtoneDebBuf;

#define PHONERINGINGTONE_PRINT(x) { _LIT( KPhoneRingingtoneDebugPrintPrefix, "PhoneRingingtone: ");\
    TPhonePhoneRingingtoneDebBuf buf( KPhoneRingingtoneDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KPhoneRingingtoneDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf); }

#define PHONERINGINGTONE_PRINTF(x,y) { _LIT( KPhoneRingingtoneDebugPrintPrefix, "PhoneRingingtone: ");\
    TPhonePhoneRingingtoneDebBuf buf( KPhoneRingingtoneDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KPhoneRingingtoneDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf,y);}

#define PHONERINGINGTONE_PRINTF2(x,y,z) { _LIT( KPhoneRingingtoneDebugPrintPrefix, "PhoneRingingtone: ");\
    TPhonePhoneRingingtoneDebBuf buf( KPhoneRingingtoneDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KPhoneRingingtoneDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf,y,z); }

#define PHONERINGINGTONE_PRINTF3(v,x,y,z) { _LIT( KPhoneRingingtoneDebugPrintPrefix, "PhoneRingingtone: ");\
    TPhonePhoneRingingtoneDebBuf buf( KPhoneRingingtoneDebugPrintPrefix);\
    if ((_L(v).Length()) + buf.Length() <= KPhoneRingingtoneDebugBufferSize )\
        buf.Append(_L(v)); RDebug::Print(buf,x,y,z); }

#else // !_DEBUG

#define PHONERINGINGTONE_PRINT(x)
#define PHONERINGINGTONE_PRINTF(x,y)
#define PHONERINGINGTONE_PRINTF2(x,y,z)
#define PHONERINGINGTONE_PRINTF3(v,x,y,z)

#endif      // _DEBUG

#endif      // PHONERINGINGTONETRACE_H

// End of File
