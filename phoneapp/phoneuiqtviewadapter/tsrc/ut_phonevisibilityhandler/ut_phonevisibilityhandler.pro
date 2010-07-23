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

TEMPLATE = app
TARGET = 
TARGET.UID3 = 0xEFF8FFF2
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += hb
CONFIG += qtestlib
CONFIG += mobility
MOBILITY = publishsubscribe

symbian {

    INCLUDEPATH += \epoc32\include\platform\mw \
                   \epoc32\include\platform \
                   \epoc32\include\platform\app \
	                 \epoc32\include\mw \
                   \sf\app\phone\inc \
                   ../../../phoneuiutils/inc/ \
                   ../../../phonemediatorcenter/inc/ \
                   ../../../phoneuiview2/inc \
                   ../../../../inc \
                   ../../../phoneringingtoneplayer/inc \
                   ../../../phoneui2/srcdata

    LIBS += -ltelephonyservice
        
    TARGET.CAPABILITY = ALL -TCB
    DEFINES += FT_SYMBIAN_INTEGRATION
}

# Input
HEADERS += ../../inc/phonevisibilityhandler.h
SOURCES += ../../src/phonevisibilityhandler.cpp

HEADERS += \sf\mw\phonesrv\phonesrv_plat\call_information_api\inc\ccallinfoiter.h
SOURCES += cphonepubsubproxy_stub.cpp
SOURCES += unit_tests.cpp
