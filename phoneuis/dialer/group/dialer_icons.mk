#
# Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
# Description: Make icons for the dialer.
#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=\epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=\epoc32\data\z
endif

# ----------------------------------------------------------------------------
# Configure these
# ----------------------------------------------------------------------------

TARGETDIR=$(ZDIR)\Resource\Apps
HEADERDIR=\epoc32\include
ICONTARGETFILENAME=$(TARGETDIR)\dialer.mif
HEADERFILENAME=$(HEADERDIR)\dialer.mbg

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

# ----------------------------------------------------------------------------
# TODO: Configure these.
#
# NOTE 1: DO NOT DEFINE MASK FILE NAMES! They are included automatically by
# MifConv if the mask detph is defined.
#
# NOTE 2: Usually, source paths should not be included in the bitmap
# definitions. MifConv searches for the icons in all icon directories in a
# predefined order, which is currently \s60\icons, \s60\bitmaps2.
# The directory \s60\icons is included in the search only if the feature flag
# __SCALABLE_ICONS is defined.
# ----------------------------------------------------------------------------
RESOURCE :
	mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) \
/c8,8  qgn_indi_dialer_eight.svg \
/c8,8  qgn_indi_dialer_five.svg \
/c8,8  qgn_indi_dialer_four.svg \
/c8,8  qgn_indi_dialer_hash.svg \
/c8,8  qgn_indi_dialer_nine.svg \
/c8,8  qgn_indi_dialer_one.svg \
/c8,8  qgn_indi_dialer_prefix.svg \
/c8,8  qgn_indi_dialer_seven.svg \
/c8,8  qgn_indi_dialer_six.svg \
/c8,8  qgn_indi_dialer_three.svg \
/c8,8  qgn_indi_dialer_two.svg \
/c8,8  qgn_indi_dialer_zero.svg \

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(HEADERFILENAME)&& \
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
