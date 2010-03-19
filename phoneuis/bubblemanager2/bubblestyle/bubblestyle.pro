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
TEMPLATE = lib
TARGET = bubblestyleplugin
CONFIG += plugin hb
INCLUDEPATH += . ../inc

win32 {
    INCLUDEPATH += c:/hb/include/hbcore c:/hb/include/hbwidgets
    DESTDIR = c:/hb/lib
    DLLDESTDIR = c:/hb/bin
}

symbian:TARGET.EPOCALLOWDLLDATA = 1
symbian:TARGET.CAPABILITY = CAP_GENERAL_DLL
symbian:TARGET.UID3 = 0x20029F89

# Input
include(src/bubblestyle.pri)
RESOURCES += bubblestyle.qrc
