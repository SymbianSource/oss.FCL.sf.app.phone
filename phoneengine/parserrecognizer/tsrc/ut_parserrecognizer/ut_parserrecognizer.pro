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
TARGET.UID3 = 0xE31F4ECF
DEPENDPATH += .
CONFIG += qtestlib
QT -= gui

symbian {
	INCLUDEPATH +=./
	INCLUDEPATH +=../../../../inc
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
	TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB
    DEFINES += BUILD_PARSERRECOGNIZER
}

# Input
HEADERS += xqservicerequest.h
HEADERS += ../../inc/parserrecognizer.h
SOURCES += unit_tests.cpp
SOURCES += ../../src/parserrecognizer.cpp
