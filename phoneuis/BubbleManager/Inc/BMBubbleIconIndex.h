/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Icon index table
*
*/

#include    <e32base.h>
#include    <bubblemanager.mbg>
#include    <avkon.mbg>
#include    <callstatus.mbg>

struct BMIconIndex
    {
    TInt iconIndex;
    TInt fileIndex;
    };

enum TBMIconsfile
    {   
    ECallstatusIconfile = 0,
    EBubblemanagerIconfile,
    EAvkonIconfile
    };

const BMIconIndex KBMIconIndex[] = 
    {
        { EMbmCallstatusQgn_indi_call_active, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_active_mask, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_active_2, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_active_2_mask, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_disconn, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_disconn_mask, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_held, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_held_mask, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_video_callsta_1, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_video_callsta_1_mask, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_video_callsta_2, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_video_callsta_2_mask, ECallstatusIconfile },
        { EMbmBubblemanagerQgn_indi_call_video_callsta_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_video_callsta_disconn_mask, EBubblemanagerIconfile },
        { EMbmCallstatusQgn_indi_call_voip_active, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_voip_active_mask, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_voip_active_2, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_voip_active_2_mask, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_voip_held, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_voip_held_mask, ECallstatusIconfile },
        { EMbmBubblemanagerQgn_indi_call_voip_callsta_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_voip_callsta_disconn_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_1, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_1_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_2, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_2_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_disconn_mask, EBubblemanagerIconfile },
        { EMbmCallstatusQgn_indi_call_video_1, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_video_1, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_video_2, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_video_2_mask, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_video_disconn, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_video_disconn_mask, ECallstatusIconfile },
        { EMbmBubblemanagerQgn_indi_call_voip_waiting_1, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_voip_waiting_1_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_voip_waiting_2, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_voip_waiting_2_mask, EBubblemanagerIconfile },
        { EMbmCallstatusQgn_indi_call_voip_disconn, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_voip_disconn_mask, ECallstatusIconfile },   
        { EMbmBubblemanagerQgn_graf_call_rec_big, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_big_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_big_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_big_disconn_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_one_active, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_one_active_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_one_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_one_disconn_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_one_held, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_one_held_mask, EBubblemanagerIconfile },
        { EMbmCallstatusQgn_indi_call_active_cyph_off, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_active_cyph_off_mask, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_disconn_cyph_off, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_disconn_cyph_off_mask, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_held_cyph_off, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_held_cyph_off_mask, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_cyphering_off, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_cyphering_off, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_line2, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_line2, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_active_emergency, ECallstatusIconfile },
        { EMbmCallstatusQgn_indi_call_active_emergency_mask, ECallstatusIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_cyph_off_1, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_cyph_off_1_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_disconn_cyph_off, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_disconn_cyph_off_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_one_active_emergency, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_one_active_emergency_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_two_active_emergency, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_two_active_emergency_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_big_right, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_big_right_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_big_right_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_big_right_disconn_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_conf_five, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_conf_five_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_conf_four, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_conf_four_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_conf_three, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_conf_three_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_conf_two, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_conf_two_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_three_active, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_three_active_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_three_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_three_disconn_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_three_held, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_three_held_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_two_active, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_two_active_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_two_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_two_disconn_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_two_held, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_two_held_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_wait_active, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_wait_active_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_wait_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_first_wait_disconn_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_hidden_held, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_hidden_held_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_big_left, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_big_left_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_big_left_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_big_left_disconn_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_small_left, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_small_left_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_small_right, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_small_right_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_small_right_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_small_right_disconn_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_bigger, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_rec_bigger_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_three_active, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_three_active_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_three_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_three_disconn_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_three_held, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_three_held_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_two_active, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_two_active_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_two_disconn, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_two_disconn_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_two_held, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_second_two_held_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_active_conf, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_active_conf_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_disconn_conf, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_disconn_conf_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_diverted, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_diverted_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_held_conf, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_held_conf_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_muted, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_muted_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_prop_nrtyp_mobile_div, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_prop_nrtyp_mobile_div_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_prop_nrtyp_phone_div, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_prop_nrtyp_phone_div_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_active_conf_cyph_off, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_active_conf_cyph_off_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_active_cyph_off_2, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_active_cyph_off_2_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_disconn_conf_cyph_off, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_disconn_conf_cyph_off_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_held_conf_cyph_off, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_held_conf_cyph_off_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_cyph_off_2, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call_waiting_cyph_off_2_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_prop_nrtyp_home_div, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_prop_nrtyp_home_div_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_prop_nrtyp_work_div, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_prop_nrtyp_work_div_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_image_1, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_graf_call_image_1_mask, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call4_image_mask_prt, EBubblemanagerIconfile },
        { EMbmBubblemanagerQgn_indi_call4_image_mask_lsc, EBubblemanagerIconfile },
        { EMbmAvkonQgn_prop_nrtyp_fax, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_fax_mask, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_home, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_home_mask, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_mobile, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_mobile_mask, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_pager, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_pager_mask, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_phone, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_phone_mask, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_work, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_work_mask, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_voip, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_voip_mask, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_car, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_car_mask, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_assistant, EAvkonIconfile },
        { EMbmAvkonQgn_prop_nrtyp_assistant_mask, EAvkonIconfile }
    };

// End of File