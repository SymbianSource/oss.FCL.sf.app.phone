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
} else:win32 {
    DESTDIR = ./
}

#
HEADERS += ../../../bubblecore/src/bubbleparticipantlistitem.h \
           ../../../bubblecore/src/bubblebutton.h

SOURCES += ut_bubbleparticipantlistitem.cpp \
           ../../../bubblecore/src/bubbleparticipantlistitem.cpp \
           ../../../bubblecore/src/bubbleutils.cpp \
           ../../../bubblecore/src/bubbleheader.cpp \
           ../../../bubblecore/src/bubbleconferenceheader.cpp \
           ../../../bubblecore/src/bubblebutton.cpp



symbian:MMP_RULES += SMPSAFE
