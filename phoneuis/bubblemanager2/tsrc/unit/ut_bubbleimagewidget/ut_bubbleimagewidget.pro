#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
#
#

TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../../bubblecore/src
INCLUDEPATH += . ../../../bubblecore/src ../../../inc ../shared
CONFIG += hb qtestlib

symbian {
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB


    BLD_INF_RULES.prj_exports += \
        "./data/pixmap.png  /epoc32/winscw/c/data/images/pixmap.png"
    BLD_INF_RULES.prj_exports += \
        "./data/pixmap_wide.png  /epoc32/winscw/c/data/images/pixmap_wide.png"

} else:win32 {
    DESTDIR = ./
}

#
HEADERS += ../../../bubblecore/src/bubbleimagewidget.h \
           ../../../bubblecore/src/bubbleimagemanager.h \
           ../../../bubblecore/src/bubbleimagemanager_p.h
SOURCES += ut_bubbleimagewidget.cpp \
           ../../../bubblecore/src/bubbleimagewidget.cpp \
           ../../../bubblecore/src/bubblestyleoption.cpp \
           ../../../bubblecore/src/bubbleimagemanager.cpp

symbian {
    SOURCES += ../../../bubblecore/src/bubbleimagemanager_p.cpp
    LIBS += -lthumbnailmanagerqt
} else:win32 {
    SOURCES += ../../../bubblecore/src/bubbleimagemanager_p_stub.cpp
}

RESOURCES += ut_bubbleimagewidget.qrc

