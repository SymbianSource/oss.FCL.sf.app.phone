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
DEPENDPATH  += . ../../infowidget/src ../../infowidget/inc
INCLUDEPATH += .
INCLUDEPATH += ../common
INCLUDEPATH += ../../inc/
INCLUDEPATH += ../../infowidget/inc/
DEFINES += QT_NO_DEBUG_STREAM

MOC_DIR = moc

QT -= gui
QT += testlib

symbian: {
  CONFIG += no_icon
  INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
  LIBS += -lmocklib 		\
          -lsymbianmock 	\
          -lhswidgetmodel
}

# test code
HEADERS += ./inc/ut_infowidget.h
SOURCES += ./src/ut_infowidget.cpp

# code to be tested
HEADERS += ../../infowidget/inc/infowidget.h
SOURCES += ../../infowidget/src/infowidget.cpp

# mocks needed for testing
#HEADERS += hbdocumentloader.h
#SOURCES += ../mocks/mock_hbdocumentloader.cpp

HEADERS += ../../infowidget/inc/infowidgetpreferences.h
SOURCES += ../mocks/mock_infowidgetpreferences.cpp
HEADERS += ../../infowidget/inc/infowidgetengine.h
SOURCES += ../mocks/mock_infowidgetengine.cpp
HEADERS += ../../infowidget/inc/infowidgetlayoutmanager.h
SOURCES += ../mocks/mock_infowidgetlayoutmanager.cpp
HEADERS += ../../infowidget/inc/infowidgetnetworkhandler.h
SOURCES += ../mocks/mock_infowidgetnetworkhandler.cpp

SOURCES += ../mocks/mock_hbcolorscheme.cpp
SOURCES += ../mocks/mock_hblabel.cpp
SOURCES += ../mocks/mock_hbstyleoptionlabel.cpp
SOURCES += ../mocks/mock_hbmarqueeitem.cpp
SOURCES += ../mocks/mock_hbwidget.cpp

