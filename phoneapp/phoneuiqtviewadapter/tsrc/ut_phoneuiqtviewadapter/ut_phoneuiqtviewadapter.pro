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
TARGET.UID3 = 0xEFF8FEA2
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += hb
CONFIG += qtestlib

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


    LIBS += -lphoneuiutils
    LIBS += -lPhoneRingingtonePlayer
    LIBS += -lphoneuiqtview
    LIBS += -lxqservice
    LIBS += -lxqserviceutil
    LIBS += -lxqplugins
    LIBS += -lserviceprovidersettings
    LIBS += -lakncapserverclient
    LIBS += -lxqsettingsmanager
    LIBS += -llogsengine
    
    TARGET.CAPABILITY = ALL -TCB
    DEFINES += FT_SYMBIAN_INTEGRATION
    DEFINES += BUILD_PHONEUIQTVIEWADAPTER
}

# Input
HEADERS += ../../inc/phoneuiqtviewadapter.h
SOURCES += ../../src/phoneuiqtviewadapter.cpp
HEADERS += ../../inc/phoneresourceadapter.h
SOURCES += ../../src/phoneresourceadapter.cpp
HEADERS += ../../inc/phoneuiqtbuttonscontroller.h
HEADERS += ../../inc/phonenotecontroller.h
HEADERS += ../../inc/telephonyservice.h
SOURCES += ../../src/telephonyservice.cpp
HEADERS += ../../inc/phoneuicommandcontroller.h
SOURCES += ../../src/phoneuicommandcontroller.cpp
HEADERS += ../../inc/phonecommandextensionwrapper.h
SOURCES += ../../src/phonecommandextensionwrapper.cpp
HEADERS += ../../inc/phonemessagecontroller.h
SOURCES += ../../src/phonemessagecontroller.cpp
HEADERS += ../../inc/phoneindicatorcontroller.h
SOURCES += ../../src/phoneindicatorcontroller.cpp
SOURCES += phoneuiqtbuttonscontroller_stub.cpp
HEADERS += phonebubblewrapper.h
SOURCES += phonebubblewrapper_stub.cpp
SOURCES += cphoneringingtonecontroller_stub.cpp
SOURCES += cphonemediatorfactory_stub.cpp
SOURCES += cphonemediatorsender_stub.cpp
SOURCES += phonenotecontroller_stub.cpp
SOURCES += unit_tests.cpp

