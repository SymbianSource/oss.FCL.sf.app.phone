#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
# Description: Homescreen Infowidget installer application
#

TEMPLATE = app

QT = core
CONFIG += no_icon

CONFIG += mobility
MOBILITY = serviceframework

SOURCES += infowidgetproviderinstaller.cpp

symbian {    
    load(data_caging_paths)
    TARGET.CAPABILITY = ALL -TCB
    TARGET.UID3 = 0x2002EA1E
    BLD_INF_RULES.prj_exports +=  \
     "$${LITERAL_HASH}include <platform_paths.hrh>" \
     "./rom/infowidgetproviderinstaller.iby    CORE_APP_LAYER_IBY_EXPORT_PATH(infowidgetproviderinstaller.iby)" 
}
