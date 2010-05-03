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
TARGET = parserrecognizer

symbian {
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA =1
    TARGET.UID3 = 0x20029F87
    DEFINES += BUILD_PARSERRECOGNIZER
    
    INCLUDEPATH += ../../inc \
                   ../phonemodel/inc
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
    
    parserDefFiles = \
        "$${LITERAL_HASH}ifdef WINSCW" \
        "DEFFILE bwins/parserrecognizer.def" \
        "$${LITERAL_HASH}else" \
        "DEFFILE eabi/parserrecognizer.def" \
        "$${LITERAL_HASH}endif"
        
    MMP_RULES += parserDefFiles

    LIBS += -lxqservice \
            -lxqserviceutil
}

# Input
#include(\ext\mw\qthighway\xqservicebase.pri)
include(./parserrecognizer.pri)

headers.sources = $$PARSERRECOGNIZER_API_HEADERS
headers.path = |../../inc
# This is for new exporting system coming in garden
for(header, headers.sources):BLD_INF_RULES.prj_exports += "$$header $$headers.path/$$basename(header)"	   