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
DEPENDPATH += . ../../src/
INCLUDEPATH += .
INCLUDEPATH +=../../../cptelephonyutils/inc
DEFINES += BUILD_CPTELEPHONYUTILS BUILD_PSETWRAPPER BUILD_SSSETTINGSWRAPPER 

QT -= gui
QT += testlib

symbian: {
  #CONFIG += no_icon
  INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
  
  LIBS += -lmocklib \
        -lsymbianmock \
        -lcpframework
}

# test code
HEADERS += ut_cpdivertplugin.h
SOURCES += ut_cpdivertplugin.cpp

# code to be tested
HEADERS += ../../src/cpdivertplugin.h \
           ../../src/cpdivertplugingroup.h \
           ../../src/cpdivertitemdata.h \
           ../../../cptelephonyutils/inc/cpphonenotes.h \
           ../../../cptelephonyutils/inc/cpplugincommon.h \
           /epoc32/include/platform/mw/psetwrapper.h \
           /epoc32/include/platform/mw/psetcliwrapper.h \
           /epoc32/include/platform/mw/psetcallwaitingwrapper.h \
           /epoc32/include/platform/mw/psetcalldivertingwrapper.h \
           /epoc32/include/platform/mw/psetnetworkwrapper.h \
           /epoc32/include/platform/mw/sssettingswrapper.h 
           
SOURCES += ../../src/cpdivertplugin.cpp \
           ../../src/cpdivertplugingroup.cpp \
           ../../src/cpdivertitemdata.cpp

# mocks needed for testing
SOURCES += ../../../tsrc/mocks/mock_psetcalldivertingwrapper.cpp \
           ../../../tsrc/mocks/mock_sssettingswrapper.cpp \
           ../../../tsrc/mocks/mock_cpplugincommon.cpp \
           ../../../tsrc/mocks/mock_cpphonenotes.cpp \
           ../../../tsrc/mocks/mock_psetwrapper.cpp 
