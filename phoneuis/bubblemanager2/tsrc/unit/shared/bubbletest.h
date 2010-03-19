/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/

#ifndef BUBBLETEST_H
#define BUBBLETEST_H

#include <QtCore>

#define BUBBLE_TEST_MAIN(TestObject) \
int main(int argc, char *argv[]) \
{ \
    HbApplication app(argc, argv); \
    TestObject tc; \
    QResource::registerResource("../hbcore.rcc"); \
    int ret = QTest::qExec(&tc, argc, argv); \
    /* Core dump if HbIconLoader instance is not destroyed before the application instance. */ \
    /* HbIconLoader uses QCoreApplication::aboutToQuit() signal to destroy itself. */ \
    /* app.exec() where the signal is normally emitted is not called here. */ \
    /* So, invoking the signal explicitly. */ \
    QMetaObject::invokeMethod(&app, "aboutToQuit", Qt::DirectConnection); \
    return ret; \
}

// style plugin name
#if defined(Q_OS_SYMBIAN)
    const char* BUBBLE_STYLE_PLUGIN = \
        "z:/resource/qt/plugins/phone/bubblestyleplugin.dll";
#elif defined(Q_OS_WIN)
    const char* BUBBLE_STYLE_PLUGIN = \
        "c:/hb/bin/bubblestyleplugin.dll";
#else // Q_OS_UNIX
    const char* BUBBLE_STYLE_PLUGIN = \
        "/home/lib/libbubblestyleplugin.so";
#endif

#endif // BUBBLETEST_H
