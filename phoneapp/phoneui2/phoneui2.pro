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
# Description: Project file for building Phoneui component
#
#


TEMPLATE = app
TARGET = phoneui
CONFIG += hb svg
RESOURCES = ./phoneui2.qrc

TRANSLATIONS = telephone.ts

symbian {
    TARGET.CAPABILITY = ALL -TCB
    TARGET.UID3 = 0x100058B3
    TARGET.VID = VID_DEFAULT
    TARGET.EPOCHEAPSIZE = 0x1000 0xA00000 
    DEFINES += FT_SYMBIAN_INTEGRATION

    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += ../../inc \
                   ../phoneui/inc \
                   ../phoneuicontrol/inc \
                   ../phoneuiutils/inc \
                   ../phoneuiqtviewadapter/inc \
                   ../phoneuiview2/inc

    LIBS += -lphoneengine \
            -lphoneuiutils \
            -lphoneuicontrol \
            -lphoneuiqtviewadapter \
            -lphoneuiqtview \
            -lFeatMgr      
      
      BLD_INF_RULES.prj_exports += \
       "$${LITERAL_HASH}include <platform_paths.hrh>" \
       "./rom/phoneui.iby    CORE_APP_LAYER_IBY_EXPORT_PATH(phoneui.iby)" \
       "./rom/phoneuiresources.iby    LANGUAGE_APP_LAYER_IBY_EXPORT_PATH(phoneuiresources.iby)"
       
       RSS_RULES += "hidden = KAppIsHidden;"
}

HEADERS += ./inc/hbphonemainwindow.h \
           ./inc/phoneuikeyeventadapter.h \
           ./inc/phoneuicommandadapter.h \
           ./inc/phoneuihousehold.h \
           ./inc/phoneuihousehold_p.h
       
SOURCES   += ./src/main.cpp \
             ./src/phoneuikeyeventadapter.cpp \
             ./src/phoneuicommandadapter.cpp \
             ./src/hbphonemainwindow.cpp \
             ./src/phoneuihousehold.cpp \
             ./src/phoneuihousehold_p.cpp

