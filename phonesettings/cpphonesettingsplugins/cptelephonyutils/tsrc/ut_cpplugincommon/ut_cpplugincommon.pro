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

CONFIG += qtestlib
CONFIG += hb
TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../src/
INCLUDEPATH += . ../../inc/
DEFINES += BUILD_CPTELEPHONYUTILS \
    XQSETTINGSMANAGER_NO_LIBRARY \
    XQSYSINFO_NO_LIBRARY

QT -= gui
QT += testlib

symbian: {
  CONFIG += no_icon
  INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
  INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
  LIBS += -lmocklib -lsymbianmock
}

# test code
HEADERS += ut_cpplugincommon.h
SOURCES += ut_cpplugincommon.cpp

# code to be tested
HEADERS += ../../inc/cpplugincommon.h \
           /epoc32/include/platform/mw/xqsettingskey.h \
           /epoc32/include/platform/mw/xqsettingsmanager.h \
           /epoc32/include/platform/mw/xqsysinfo.h \
           ../../../../../inc/telephonyvariant.hrh \
           ../../../../../inc/telinternalcrkeys.h
           
SOURCES += ../../src/cpplugincommon.cpp

# mocks needed for testing
SOURCES += ../../../tsrc/mocks/mock_xqsettingskey.cpp \
           ../../../tsrc/mocks/mock_xqsettingsmanager.cpp \
           ../../../tsrc/mocks/mock_xqsysinfo.cpp
