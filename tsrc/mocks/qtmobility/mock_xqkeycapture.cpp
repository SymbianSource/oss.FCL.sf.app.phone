/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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
*/
#include <QDebug>
#include <smcmockclassincludes.h>
#include "xqkeycapture.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// XQKeyCapture::XQKeyCapture
// -----------------------------------------------------------------------------
//
XQKeyCapture::XQKeyCapture(  )
    {
    
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::~XQKeyCapture
// -----------------------------------------------------------------------------
//
XQKeyCapture::~XQKeyCapture(  )
    {
    
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureKey( 
        Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, Qt::Key, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureKey( 
        TUint aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, TUint, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureLongKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureLongKey( 
        Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType )
    {
    SMC_MOCK_METHOD4( bool, Qt::Key, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier, 
        XQKeyCapture::LongFlags, aLongType )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureLongKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureLongKey( 
        TUint aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType )
    {
    SMC_MOCK_METHOD4( bool, TUint, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier, 
        XQKeyCapture::LongFlags, aLongType )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureKeyUpAndDowns
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureKeyUpAndDowns( 
        Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, Qt::Key, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureKeyUpAndDowns
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureKeyUpAndDowns( 
        TUint aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, TUint, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureKey( 
        XQKeyCaptureKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, XQKeyCaptureKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureKey( 
        XQKeyCaptureNativeKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, XQKeyCaptureNativeKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureLongKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureLongKey( 
        XQKeyCaptureKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType )
    {
    SMC_MOCK_METHOD4( bool, XQKeyCaptureKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier, 
        XQKeyCapture::LongFlags, aLongType )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureLongKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureLongKey( 
        XQKeyCaptureNativeKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType )
    {
    SMC_MOCK_METHOD4( bool, XQKeyCaptureNativeKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier, 
        XQKeyCapture::LongFlags, aLongType )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureKeyUpAndDowns
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureKeyUpAndDowns( 
        XQKeyCaptureKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, XQKeyCaptureKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureKeyUpAndDowns
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureKeyUpAndDowns( 
        XQKeyCaptureNativeKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, XQKeyCaptureNativeKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureKey( 
        Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, Qt::Key, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureKey( 
        TUint aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, TUint, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureLongKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureLongKey( 
        Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType )
    {
    SMC_MOCK_METHOD4( bool, Qt::Key, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier, 
        XQKeyCapture::LongFlags, aLongType )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureLongKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureLongKey( 
        TUint aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType )
    {
    SMC_MOCK_METHOD4( bool, TUint, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier, 
        XQKeyCapture::LongFlags, aLongType )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureKeyUpAndDowns
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureKeyUpAndDowns( 
        Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, Qt::Key, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureKeyUpAndDowns
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureKeyUpAndDowns( 
        TUint aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, TUint, aKey, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureKey( 
        XQKeyCaptureKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, XQKeyCaptureKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureKey( 
        XQKeyCaptureNativeKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, XQKeyCaptureNativeKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureLongKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureLongKey( 
        XQKeyCaptureKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType )
    {
    SMC_MOCK_METHOD4( bool, XQKeyCaptureKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier, 
        XQKeyCapture::LongFlags, aLongType )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureLongKey
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureLongKey( 
        XQKeyCaptureNativeKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType )
    {
    SMC_MOCK_METHOD4( bool, XQKeyCaptureNativeKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier, 
        XQKeyCapture::LongFlags, aLongType )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureKeyUpAndDowns
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureKeyUpAndDowns( 
        XQKeyCaptureKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, XQKeyCaptureKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureKeyUpAndDowns
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureKeyUpAndDowns( 
        XQKeyCaptureNativeKeyList list,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier )
    {
    SMC_MOCK_METHOD3( bool, XQKeyCaptureNativeKeyList, list, 
        /*Qt::KeyboardModifiers*/int, aModifiersMask, 
        /*Qt::KeyboardModifiers*/int, aModifier )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::captureRemoteKeys
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::captureRemoteKeys( 
        CapturingFlags flags )
    {
    SMC_MOCK_METHOD1( bool, /*CapturingFlags*/int, flags )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::cancelCaptureRemoteKeys
// -----------------------------------------------------------------------------
//
bool XQKeyCapture::cancelCaptureRemoteKeys( 
        CapturingFlags flags )
    {
    SMC_MOCK_METHOD1( bool, /*CapturingFlags*/int, flags )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::remoteEventType_KeyPress
// -----------------------------------------------------------------------------
//
QEvent::Type XQKeyCapture::remoteEventType_KeyPress(  )
    {
    SMC_MOCK_METHOD0( QEvent::Type )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::remoteEventType_KeyRelease
// -----------------------------------------------------------------------------
//
QEvent::Type XQKeyCapture::remoteEventType_KeyRelease(  )
    {
    SMC_MOCK_METHOD0( QEvent::Type )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::errorString
// -----------------------------------------------------------------------------
//
QString XQKeyCapture::errorString(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// XQKeyCapture::errorId
// -----------------------------------------------------------------------------
//
int XQKeyCapture::errorId(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


