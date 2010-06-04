#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
TARGET.UID3 = 0xE31F4ECF
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += qtestlib

QT -= gui

symbian {

    INCLUDEPATH += \epoc32\include\platform \
                   \epoc32\include\platform\mw \
                   \epoc32\include\mw \
                   ../../../../inc

    LIBS += -lxqservice \
            -lxqserviceutil

    TARGET.CAPABILITY = ALL -TCB
}

# Input
HEADERS += ../../inc/keysequencerecognitionservice.h
SOURCES += ../../src/keysequencerecognitionservice.cpp
SOURCES += unit_tests.cpp
