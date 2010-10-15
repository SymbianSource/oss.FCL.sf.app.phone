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
# Description: Project file for building unit test component
#
#

include(common.pri)

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../../src
INCLUDEPATH += ../../../../inc


# Input
HEADERS += ../../inc/phoneuihousehold.h
HEADERS += ../../inc/phonemainwindow.h

SOURCES += phoneuihousehold_stub.cpp
SOURCES += hbmainwindow_stub.cpp
SOURCES += ut_phonemainwindow.cpp
SOURCES += ../../src/phonemainwindow.cpp


symbian {
    TARGET.CAPABILITY = ALL -TCB
    MMP_RULES += SMPSAFE
}
