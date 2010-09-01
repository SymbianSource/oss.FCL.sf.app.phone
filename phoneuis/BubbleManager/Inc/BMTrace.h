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



#ifndef BUBBLETRACE_H
#define BUBBLETRACE_H

#ifdef _DEBUG

// INCLUDES
#include <e32svr.h>

// CONSTANTS

const TInt KBubbleDebugBufferSize = 60;

// MACROS

typedef TBuf<KBubbleDebugBufferSize> TBubbleDebBuf;

#define BUBBLE_PRINT(x) { _LIT( KBubbleDebugPrintPrefix, "BM: ");\
    TBubbleDebBuf buf( KBubbleDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KBubbleDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf); }

#define BUBBLE_PRINTF(x,y) { _LIT( KBubbleDebugPrintPrefix, "BM: ");\
    TBubbleDebBuf buf( KBubbleDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KBubbleDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf,y);}

#define BUBBLE_PRINTF2(x,y,z) { _LIT( KBubbleDebugPrintPrefix, "BM: ");\
    TBubbleDebBuf buf( KBubbleDebugPrintPrefix);\
    if ((_L(x).Length()) + buf.Length() <= KBubbleDebugBufferSize )\
        buf.Append(_L(x)); RDebug::Print(buf,y,z); }

#else // !_DEBUG

#define BUBBLE_PRINT(x)
#define BUBBLE_PRINTF(x,y)
#define BUBBLE_PRINTF2(x,y,z)

#endif      // _DEBUG

#endif      // BUBBLETRACE_H

// End of File
