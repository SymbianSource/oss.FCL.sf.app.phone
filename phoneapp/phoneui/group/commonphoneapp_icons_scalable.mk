#
# Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
# Description: Make file for phoneui.
#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=\epoc32\release\$(PLATFORM)\$(CFG)\z
else
ZDIR=\epoc32\data\z
endif

# ----------------------------------------------------------------------------
# Configure these
# ----------------------------------------------------------------------------

TARGETDIR=$(ZDIR)\resource\apps
HEADERDIR=\epoc32\include
ICONTARGETFILENAME=$(TARGETDIR)\phoneui.mif
HEADERFILENAME=$(HEADERDIR)\phoneui.mbg

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

# ----------------------------------------------------------------------------
# Configure these.
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
		/c8,8 qgn_indi_button_bluetooth.svg \
		/c8,8 qgn_indi_button_conference.svg \
		/c8,8 qgn_indi_button_drop.svg \
		/c8,8 qgn_indi_button_end_active_call.svg \
		/c8,8 qgn_indi_button_end_all_calls.svg \
		/c8,8 qgn_indi_button_go_idle.svg \
		/c8,8 qgn_indi_button_handset.svg \
		/c8,8 qgn_indi_button_hold.svg \
		/c8,8 qgn_indi_button_join.svg \
		/c8,8 qgn_indi_button_loudspeaker.svg \
		/c8,8 qgn_indi_button_mute_mic.svg \
		/c8,8 qgn_indi_button_participants.svg \
		/c8,8 qgn_indi_button_private.svg \
		/c8,8 qgn_indi_button_swap.svg \
		/c8,8 qgn_indi_button_unhold.svg \
		/c8,8 qgn_indi_button_unmute_mic.svg \
		/c8,8 qgn_indi_button_video_to_voice.svg \
		/c8,8 qgn_indi_button_voice_to_video.svg \
		/c8,8 qgn_indi_button_answer.svg \
		/c8,8 qgn_indi_button_reject.svg \
		/c8,8 qgn_indi_tb_call.svg \
		/c8,8 qgn_indi_dialer_clear.svg \
		/c8,8 qgn_indi_tb_pb.svg \
		/c8,8 qgn_indi_dialer_add_contacts.svg \
		/c8,8 qgn_indi_dialer_logs.svg \
		/c8,8 qgn_indi_button_send_dtmf.svg \
		/c8,8 qgn_indi_button_silence.svg

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
