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

CONFIG += qtestlib hb
TEMPLATE = app
TARGET = 
MOC_DIR = moc

INCLUDEPATH += . \
        /epoc32/include/mw/hb/hbfeedback \
        ../../phoneapp/phoneuiqtviewadapter/inc \
        ../../inc \
        ../../phoneapp/phoneuiutils/inc \
        ../../phoneapp/phoneuiview2/inc \
        ../../phoneapp/phoneringingtoneplayer/inc

DEFINES += XQSYSINFO_LIBRARY XQSETTINGSMANAGER_LIBRARY
DEFINES += QT_BUILD_SYSINFO_LIB QT_MAKEDLL BUILD_HB_CORE

QT -= gui
QT += testlib

symbian: { 
    CONFIG += no_icon
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    LIBS += -lmocklib       \
            -lsymbianmock
}

HEADERS += 

SOURCES += ./mw/*.cpp \
        ./orbit/*.cpp \
        ./phoneuiqtviewadapter/*.cpp \
        ./qtgui/*.cpp \
        ./qthighway/*.cpp \
        ./qtmobility/*.cpp \
        ./bubblemanager/*.cpp \
        ./phonesrv/*.cpp
        

symbian:MMP_RULES += SMPSAFE
