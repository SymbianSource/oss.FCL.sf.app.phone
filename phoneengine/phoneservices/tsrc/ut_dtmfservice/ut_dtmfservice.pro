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
TARGET.UID3 = 0xEEE39B2F
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += hb
CONFIG += qtestlib

symbian {

    INCLUDEPATH += \epoc32\include\platform \
                   \epoc32\include\platform\mw \
                   \epoc32\include\mw \
                   ../../../phoneuiutils/inc \
                   ../../../phoneuicontrol/inc \
                   ../../../phoneuiqtviewadapter/src \
                   ../../../../inc

    LIBS += -lxqservice \
            -lxqserviceutil \
            -lphoneuicontrol

    TARGET.CAPABILITY = ALL -TCB
}

# Input
HEADERS += ../../inc/dtmfservice.h
SOURCES += unit_tests.cpp
SOURCES += ../../src/dtmfservice.cpp
