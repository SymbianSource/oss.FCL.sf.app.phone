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

CONFIG += qtestlib hb
TEMPLATE = app
TARGET = 

INCLUDEPATH += ./inc
INCLUDEPATH += ../../inc
INCLUDEPATH += ../common
DEFINES += QT_NO_DEBUG_STREAM BUILD_NETWORKHANDLINGSTARTER

QT -= gui
QT += testlib

symbian: {
  CONFIG += no_icon
  INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
  LIBS += -lmocklib \
          -lsymbianmock
}

# test code
HEADERS += ./inc/ut_cnetworklistener.h
SOURCES += ./src/ut_cnetworklistener.cpp

# code to be tested
HEADERS += ../../inc/cnetworklistener.h
SOURCES += ../../src/cnetworklistener.cpp

# mocks needed for testing
SOURCES += ../mocks/mock_cnwsession.cpp
SOURCES += ../mocks/mock_centralrepository.cpp

