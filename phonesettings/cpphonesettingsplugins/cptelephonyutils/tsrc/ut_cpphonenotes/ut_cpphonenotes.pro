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


CONFIG += hb qtestlib
TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../src/
INCLUDEPATH += . ../../inc/
DEFINES += BUILD_CPTELEPHONYUTILS

QT -= gui
QT += testlib

symbian: {
  INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
  LIBS += -lmocklib -lsymbianmock
}

# test code
HEADERS += ut_cpphonenotes.h
SOURCES += ut_cpphonenotes.cpp

# code to be tested
HEADERS += ../../inc/cpphonenotes.h \
           ../../inc/cptelephonyutilsdefs.h \
           ../../inc/cpplugincommon.h
           
SOURCES += ../../src/cpphonenotes.cpp #    \
#           ../../src/cpphonenotes_p.cpp

# mocks needed for testing
SOURCES += ../../../tsrc/mocks/mock_cpplugincommon.cpp
