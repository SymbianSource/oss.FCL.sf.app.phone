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
DEPENDPATH += . ../../../bubblestyle/src
INCLUDEPATH += . ../../../bubblestyle/src ../../../inc ../shared
CONFIG += hb qtestlib

symbian {
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
} else:win32 {
    DESTDIR = ./
}

#
HEADERS += ../../../bubblestyle/src/bubblestyleplugin.h \
           ../../../bubblestyle/src/bubbleanimiconitem.h
SOURCES += ut_bubblestyleplugin.cpp \
           ../../../bubblestyle/src/bubblestyleplugin.cpp \
           ../../../bubblestyle/src/bubbleanimiconitem.cpp \
           ../../../bubblecore/src/bubblestyleoption.cpp

