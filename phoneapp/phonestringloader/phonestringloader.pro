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
# Description: Project file for building stringloader component
#
#

TEMPLATE = lib
TARGET = phonestringloader
CONFIG += hb

symbian {
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA =1
    TARGET.UID3 = 0x20029F85
    DEFINES += FT_SYMBIAN_INTEGRATION
    
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE

    INCLUDEPATH += ../../inc \  
                   ../phoneuiutils/inc \
                   ../phoneuiview2/inc \
                   ../phoneuiqtviewadapter/inc
                   
    LIBS += -lphoneuiqtviewadapter
    LIBS += -lphoneuiqtview
    
    defFiles = \
        "$${LITERAL_HASH}ifdef WINS" \
            "DEFFILE bwins/phonestringloader.def" \
        "$${LITERAL_HASH}else" \
            "DEFFILE eabi/phonestringloader.def" \
        "$${LITERAL_HASH}endif"
    
    MMP_RULES += defFiles
}

# Input
include(phonestringloader.pri)
