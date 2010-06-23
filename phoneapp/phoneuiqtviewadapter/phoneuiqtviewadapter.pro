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
# Description: Project file for building qt view adapter component
#
#

TEMPLATE = lib
TARGET = phoneuiqtviewadapter
CONFIG += hb mobility
MOBILITY = publishsubscribe

symbian {
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x20029F86
    DEFINES += FT_SYMBIAN_INTEGRATION
    DEFINES += BUILD_PHONEUIQTVIEWADAPTER
    
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += ../../inc \
                   ../phoneuiutils/inc \
                   ../phoneuiview2/inc \
                   ../phoneui2/srcdata \
                   ../phoneringingtoneplayer/inc \
                   ../phonemediatorcenter/inc

    LIBS += -lphoneuiutils \
        -lbubblemanager2 \
        -lphoneringingtoneplayer \
        -lphoneuiqtview \
        -lphonemediatorcenter \
        -lxqservice \
        -lxqserviceutil \
        -lserviceprovidersettings \
        -lxqsettingsmanager \
        -llogsengine \
        -lxqplugins \
        -lakncapserverclient \
        -ltelephonyservice \
        -lapgrfx \
        -lavkon \
        -lws32 \
        -lcone
        
    defFiles = "$${LITERAL_HASH}ifdef WINS" \
        "DEFFILE bwins/phoneuiqtviewadapter.def" \
        "$${LITERAL_HASH}else" \
            "DEFFILE eabi/phoneuiqtviewadapter.def" \
        "$${LITERAL_HASH}endif"
    
    MMP_RULES += defFiles

    #qcrml export
    BLD_INF_RULES.prj_exports += "./conf/carmode.qcrml  c:/resource/qt/crml/carmode.qcrml "
    crml.sources = ./conf/carmode.qcrml
    crml.path = c:/resource/qt/crml
    DEPLOYMENT += crml

}

# Input
include(phoneuiqtviewadapter.pri)
