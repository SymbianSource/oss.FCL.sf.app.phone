/*
* ============================================================================
*  Name        : phoneuiviewtrace.h
*  Part of     : PhoneUI / PhoneUIView
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


#ifndef PHONEUIVIEWTRACE_H
#define PHONEUIVIEWTRACE_H

//#define __RINGINGTONE_LOGGING
#ifdef __RINGINGTONE_LOGGING

// INCLUDES
#include <e32svr.h>

// CONSTANTS

const TInt KPhoneUiViewDebugBufferSize = 100;

// MACROS

typedef TBuf<KPhoneUiViewDebugBufferSize> TPhoneUiViewDebBuf;

#define PHONEUIVIEW_PRINT(x) { _LIT( KPhoneUiViewDebugPrintPrefix, "PhoneUiView: ");\
    TPhoneUiViewDebBuf buf( KPhoneUiViewDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KPhoneUiViewDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf); }

#define PHONEUIVIEW_PRINTF(x,y) { _LIT( KPhoneUiViewDebugPrintPrefix, "PhoneUiView: ");\
    TPhoneUiViewDebBuf buf( KPhoneUiViewDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KPhoneUiViewDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf,y);}

#define PHONEUIVIEW_PRINTF2(x,y,z) { _LIT( KPhoneUiViewDebugPrintPrefix, "PhoneUiView: ");\
    TPhoneUiViewDebBuf buf( KPhoneUiViewDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KPhoneUiViewDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf,y,z); }

#define PHONEUIVIEW_PRINTF3(v,x,y,z) { _LIT( KPhoneUiViewDebugPrintPrefix, "PhoneUiView: ");\
    TPhoneUiViewDebBuf buf( KPhoneUiViewDebugPrintPrefix);\
    if ((_L(v).Length()) + buf.Length() <= KPhoneUiViewDebugBufferSize )\
        buf.Append(_L(v)); RDebug::Print(buf,x,y,z); }

#else // !_DEBUG

#define PHONEUIVIEW_PRINT(x)
#define PHONEUIVIEW_PRINTF(x,y)
#define PHONEUIVIEW_PRINTF2(x,y,z)
#define PHONEUIVIEW_PRINTF3(v,x,y,z)

#endif      // _DEBUG

#endif      // PHONEUIVIEWTRACE_H

// End of File
