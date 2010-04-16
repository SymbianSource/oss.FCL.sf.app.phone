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

TEMPLATE = lib
TARGET = cptelephonyutils
CONFIG += hb
MOC_DIR = moc

INCLUDEPATH += inc
SOURCEPATH += src

# Input
HEADERS += inc/cpplugincommon.h inc/cpphonenotes.h
SOURCES += src/cpphonenotes.cpp
symbian: {
    SOURCES += src/cpplugincommon.cpp 
    }
else: {
    SOURCES += src/cpplugincommon_s.cpp
    }

TRANSLATIONS = telephone_cp.ts

DEFINES += BUILD_CPTELEPHONYUTILS

symbian: {
    load(data_caging_paths)
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
    
    LIBS += -lsssettings    \
            -lphonesettings \
            -lxqsysinfo \
            -lxqsettingsmanager

    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    TARGET.EPOCALLOWDLLDATA = 1 
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.UID3 = 0X20029F1F

    # For sis file
    dllfile.sources = $${TARGET}.dll
    dllfile.path = $$SHARED_LIB_DIR
    DEPLOYMENT += dllfile
} else: {
    INCLUDEPATH += ../../../../../mw/phonesrv/phonesrv_plat/phone_settings_api/inc
    INCLUDEPATH += ../../../../../mw/gsprofilesrv/controlpanel/controlpanel_plat/inc
    DESTDIR = c:\hb\lib
    DLLDESTDIR = c:\hb\bin
}