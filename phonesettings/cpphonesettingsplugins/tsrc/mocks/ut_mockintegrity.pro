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

INCLUDEPATH += .
INCLUDEPATH += ../../barringplugin/inc
INCLUDEPATH += ../../callsplugin/src
INCLUDEPATH += ../../cptelephonyutils/inc
INCLUDEPATH += ../../divertplugin/src
INCLUDEPATH += ../../networkplugin/src
INCLUDEPATH += ../../cpnetworkplugin/src
INCLUDEPATH += ../../telephonyplugin/src
INCLUDEPATH += /sf/mw/phonesrv/cellular/telephonysettings/xqbindings/psetwrapper/src

DEFINES += BUILD_CPTELEPHONYUTILS BUILD_PSETWRAPPER BUILD_SSSETTINGSWRAPPER
DEFINES += XQSYSINFO_LIBRARY XQSETTINGSMANAGER_LIBRARY

QT -= gui
QT += testlib

symbian: { 
    CONFIG += no_icon
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    LIBS += -lmocklib       \
            -lsymbianmock   \
            -lcpframework 
}

HEADERS += ut_mockintegrity.h
SOURCES += ut_mockintegrity.cpp

HEADERS += ../../callsplugin/src/cpcallsplugin.h                \
           ../../cptelephonyutils/inc/cpphonenotes.h            \
           ../../cptelephonyutils/inc/cpplugincommon.h          \
           /epoc32/include/platform/mw/psetwrapper.h            \
           /epoc32/include/platform/mw/psetcliwrapper.h         \
           /epoc32/include/platform/mw/sssettingswrapper.h      \
           /epoc32/include/platform/mw/psetcallwaitingwrapper.h \
           /epoc32/include/platform/mw/psetcalldivertingwrapper.h \
           /epoc32/include/platform/mw/psetnetworkwrapper.h     \
           /epoc32/include/mw/xqsettingskey.h                   \
           /epoc32/include/mw/xqsettingsmanager.h               \
           /epoc32/include/mw/xqsysinfo.h

SOURCES +=                                  \
        mock_cpbarringplugin.cpp            \
        mock_cpbarringplugingroup.cpp       \
        mock_cpcallsplugin.cpp              \
        mock_cpcallsplugingroup.cpp         \
        mock_cpnetworkplugin.cpp            \
        mock_cpnetworkpluginview.cpp        \
        mock_cpnetworkplugingroup.cpp       \
        mock_cpphonenotes.cpp               \
        mock_cpplugincommon.cpp             \
        mock_cptelephonypluginview.cpp      \
        mock_psetcalldivertingwrapper.cpp   \
        mock_psetcalldivertingwrapper_p.cpp \
        mock_psetcallwaitingwrapper.cpp     \
        mock_psetcallwaitingwrapper_p.cpp   \
        mock_psetcliwrapper.cpp             \
        mock_psetcliwrapper_p.cpp           \
        mock_psetnetworkwrapper.cpp         \
        mock_psetnetworkwrapper_p.cpp       \
        mock_psetwrapper.cpp                \
        mock_sssettingswrapper.cpp          \
        mock_xqsettingskey.cpp              \
        mock_xqsettingsmanager.cpp          \
        mock_xqsysinfo.cpp
  