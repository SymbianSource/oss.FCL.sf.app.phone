#
# Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
# Description: Make file for vmbx.
#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=\epoc32\release\$(PLATFORM)\$(CFG)\z
else
ZDIR=\epoc32\data\z
endif

TARGETDIR1=$(ZDIR)\resource\apps

HEADERDIR=\epoc32\include
ICONTARGETFILENAME=$(TARGETDIR1)\vm.mif

HEADERFILENAME =$(HEADERDIR)\vm.mbg


do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE :

	mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) \
	/c8,8 qgn_menu_smsvo_cxt.svg \
	/c8,8 qgn_prop_nrtyp_sim_contact.svg \
	/c8,8 qgn_prop_nrtyp_mobile.svg \
	/c8,8 qgn_prop_nrtyp_video.svg \
	/c8,8 qgn_prop_nrtyp_voip.svg \
	/c8,8 qgn_prop_empty.svg
	


		
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
