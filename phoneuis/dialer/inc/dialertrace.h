/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Macros for tracing.
*
*/



#ifndef DIALERTRACE_H
#define DIALERTRACE_H

#ifdef _DEBUG

// INCLUDES
#include <e32svr.h>

// CONSTANTS

const TInt KDialerDebugBufferSize = 60;

// MACROS

typedef TBuf<KDialerDebugBufferSize> TDialerDebBuf;

#define DIALER_PRINT(x) { _LIT( KDialerDebugPrintPrefix, "Dialer: ");\
    TDialerDebBuf buf( KDialerDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KDialerDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf); }

#define DIALER_PRINTF(x,y) { _LIT( KDialerDebugPrintPrefix, "Dialer: ");\
    TDialerDebBuf buf( KDialerDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KDialerDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf,y);}

#define DIALER_PRINTF2(x,y,z) { _LIT( KDialerDebugPrintPrefix, "Dialer: ");\
    TDialerDebBuf buf( KDialerDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KDialerDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf,y,z); }

#else // !_DEBUG

#define DIALER_PRINT(x)
#define DIALER_PRINTF(x,y)
#define DIALER_PRINTF2(x,y,z)

#endif      // _DEBUG

#endif      // DIALERTRACE_H

// End of File
