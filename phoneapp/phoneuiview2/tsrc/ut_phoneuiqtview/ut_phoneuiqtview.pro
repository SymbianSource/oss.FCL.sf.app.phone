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

TEMPLATE = app
TARGET = 
TARGET.UID3 = 0xEA9BEF3F
DEPENDPATH += . ../../src/
INCLUDEPATH += .
CONFIG += hb qtestlib mobility
MOBILITY += systeminfo

symbian {

    INCLUDEPATH += /epoc32/include/platform/mw \
                   /epoc32/include/platform/app \
                   /epoc32/include/mw \
                   ../../../phoneuiutils/inc/ \
                   ../../inc/ \
                   ../../../../inc \
                   ./

    LIBS += -lbubblemanager2 \
            -lxqserviceutil \
            -ldialpad \
            -lxqkeycapture \
            -lmocklib \
            -lsymbianmock

} else:win32 {
    INCLUDEPATH += c:/hb/include/hbcore \
        c:/hb/include/hbwidgets \
        ../../inc/ \
        ../../../../inc \
        .

    LIBS += -lbubblemanager2
}

DEFINES += BUILD_PHONEUIQTVIEW

# Input
HEADERS += ../../inc/phoneuiqtview.h \
           ../../inc/phoneuiqtviewif.h \
           ../../inc/phoneuiqtview_p.h
           
SOURCES += ../../src/phoneuiqtview.cpp \
           ../../src/phoneuiqtview_p.cpp \
           unit_tests.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbaction.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbicon.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbmainwindow.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbview.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbwidget.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbwidgetbase.cpp \
           ../../../../tsrc/mocks/orbit/mock_hblineedit.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbabstractedit.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbtoolbar.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbvolumesliderpopup.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbsliderpopup.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbdialog.cpp \
           ../../../../tsrc/mocks/orbit/mock_hbpopup.cpp \
           ../../../../tsrc/mocks/qtgui/mock_qgraphicsview.cpp \
           ../../../../tsrc/mocks/qtgui/mock_qgraphicswidget.cpp \
           ../../../../tsrc/mocks/qtgui/mock_qgraphicsitem.cpp \
           ../../../../tsrc/mocks/bubblemanager/mock_bubblemanager2.cpp \
           ../../../../tsrc/mocks/qtmobility/mock_qsysteminfo.cpp \
           ../../../../tsrc/mocks/qtmobility/mock_xqkeycapture.cpp \
           ../../../../tsrc/mocks/phonesrv/mock_dialpad.cpp \
           ../../../../tsrc/mocks/phonesrv/mock_dialpadkeyhandler.cpp
           


symbian:MMP_RULES += SMPSAFE
