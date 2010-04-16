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
# Release
#

r01.path = ../../../bin/release/hsresources/import/widgetregistry/20029F28
r01.files = ./release/*.dll \
            ./resource/*.manifest \
            ./resource/*.png

INSTALLS += r01

#
# Debug
#

d01.path = ../../../../bin/debug/hsresources/import/widgetregistry/20029F28
d01.files = ./debug/*.dll \
            ./resource/*.manifest \
            ./resource/*.png

INSTALLS += d01
