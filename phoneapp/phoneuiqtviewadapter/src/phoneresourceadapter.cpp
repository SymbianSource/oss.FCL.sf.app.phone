/*!
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Converts Symbian resources to PhoneAction objects.
*/

#include "phoneresourceadapter.h"
#include "phoneresourceids.h"
#include "phoneui.hrh"
#include "phoneuiqtbuttonscontroller.h"
#include <phoneappvoipcommands.hrh>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QtDebug>
#include <QVariant>
#include <hbaction.h>

PhoneResourceAdapter *pinstance = 0;// initialize pointer

PhoneResourceAdapter* PhoneResourceAdapter::Instance (QObject *parent) 
{
    if (pinstance == 0)  // is it the first call?
    {  
        pinstance = new PhoneResourceAdapter (parent); // create sole instance
    }
    return pinstance; // address of sole instance
}
  
PhoneResourceAdapter::PhoneResourceAdapter (QObject *parent) : QObject (parent)
{
    m_buttonsCtrl = new PhoneUIQtButtonsController();
    
}

PhoneResourceAdapter::~PhoneResourceAdapter ()
{
    delete m_buttonsCtrl;
}

PhoneUIQtButtonsController* PhoneResourceAdapter::buttonsController ()
{
    PhoneUIQtButtonsController* retValue = 0;
    
    if (m_buttonsCtrl) 
        {
        retValue = m_buttonsCtrl;
        }
    
    return retValue;
}

QMap<PhoneAction::ActionType, PhoneAction *> PhoneResourceAdapter::convert (int symbianResourceId, ...)
{
    QMap<PhoneAction::ActionType, PhoneAction *> translatedActions;
    
    switch (symbianResourceId) {
    
    case R_PHONEUI_CALLHANDLING_INCOMINGCALL_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon("qtg_mono_send"));
        leftButton->setCommand(EPhoneCallComingCmdSoftReject);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;       
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon (HbIcon("qtg_mono_speaker_off"));
        rightButton->setCommand (EPhoneCallComingCmdSilent);
        translatedActions [PhoneAction::ToolbarButton2] = rightButton;            
        }
        break;

    case R_PHONEUI_CALLHANDLING_INCOMINGCALL_REJECT_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon("qtg_mono_send"));
        leftButton->setCommand(EPhoneCallComingCmdSoftReject);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;
               
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon (HbIcon("qtg_mono_reject_call"));
        rightButton->setCommand (EPhoneCallComingCmdReject);
        translatedActions [PhoneAction::ToolbarButton2] = rightButton;      
        }
        break;
        
    case R_PHONEUI_CALLHANDLING_INCOMINGCALL_SOFT_REJECT_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon("qtg_mono_send"));
        leftButton->setCommand(EPhoneCallComingCmdSoftReject);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;    
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon (HbIcon("qtg_mono_speaker_off"));
        rightButton->setDisabled(true);
        translatedActions [PhoneAction::ToolbarButton2] = rightButton;        
        }
        break;
        
    case R_PHONEUI_CALLHANDLING_CALLWAITING_CBA:        
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon("qtg_mono_send"));
        leftButton->setCommand(EPhoneCallComingCmdSoftReject);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;       
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon (HbIcon("qtg_mono_join_call"));
        rightButton->setCommand (EPhoneInCallCmdJoin);
        rightButton->setDisabled(true);
        translatedActions [PhoneAction::ToolbarButton2] = rightButton;            
        }
        break;        
        
    case R_PHONEUI_DIALER_CBA:
    case R_PHONEUI_INCALL_DIALER_CBA:
    case R_PHONEUI_CALLHANDLING_CALLSETUP_EMPTY_DTMFDIALER_CBA:
    case R_PHONEUI_CALLHANDLING_INCALL_UNHOLD_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon("qtg_mono_dialer"));
        leftButton->setCommand(EPhoneInCallCmdDialer);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;
        
        PhoneAction *centerButton = new PhoneAction;
        centerButton->setIcon (HbIcon("qtg_mono_mic_mute"));
        centerButton->setCommand (EPhoneInCallCmdMute);
        translatedActions [PhoneAction::ToolbarButton2] = centerButton;        

        PhoneAction *centerButton2 = new PhoneAction;
        if (!m_buttonsCtrl->getButtonFlags(PhoneUIQtButtonsController::Ihf))
            {
            centerButton2->setIcon (HbIcon("qtg_mono_speaker"));
            centerButton2->setCommand (EPhoneInCallCmdActivateIhf);
            }
        else
            {
            centerButton2->setIcon (HbIcon("qtg_mono_mobile"));
            centerButton2->setCommand (EPhoneInCallCmdDeactivateIhf);
            }
        translatedActions [PhoneAction::ToolbarButton3] = centerButton2; 
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon(HbIcon("qtg_mono_contacts"));
        rightButton->setDisabled( 
                (symbianResourceId==
                 R_PHONEUI_CALLHANDLING_CALLSETUP_EMPTY_DTMFDIALER_CBA));        
        rightButton->setCommand(EPhoneInCallCmdContacts);
        
        translatedActions[PhoneAction::ToolbarButton4] = rightButton;
        }
        break;
        
    case R_PHONEUI_CALLHANDLING_EMERGENCY_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon("qtg_mono_dialer"));
        leftButton->setCommand(EPhoneInCallCmdDialer);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;
        
        PhoneAction *rightButton = new PhoneAction;
        if (!m_buttonsCtrl->getButtonFlags(PhoneUIQtButtonsController::Ihf))
            {
            rightButton->setIcon (HbIcon("qtg_mono_speaker"));
            rightButton->setCommand (EPhoneInCallCmdActivateIhf);
            }
        else
            {
            rightButton->setIcon (HbIcon("qtg_mono_mobile"));
            rightButton->setCommand (EPhoneInCallCmdDeactivateIhf);
            }
        translatedActions [PhoneAction::ToolbarButton2] = rightButton;                    
        }
        break;
        
    case R_PHONEUI_CALLHANDLING_INCALL_UNMUTE_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon("qtg_mono_dialer"));
        leftButton->setCommand(EPhoneInCallCmdDialer);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;
        
        PhoneAction *centerButton = new PhoneAction;
        centerButton->setIcon (HbIcon("qtg_mono_mic_unmute"));
        centerButton->setCommand (EPhoneInCallCmdUnmute);
        translatedActions [PhoneAction::ToolbarButton2] = centerButton;        

        PhoneAction *centerButton2 = new PhoneAction;
        if (!m_buttonsCtrl->getButtonFlags(PhoneUIQtButtonsController::Ihf))
            {
            centerButton2->setIcon (HbIcon("qtg_mono_speaker"));
            centerButton2->setCommand (EPhoneInCallCmdActivateIhf);
            }
        else
            {
            centerButton2->setIcon (HbIcon("qtg_mono_mobile"));
            centerButton2->setCommand (EPhoneInCallCmdDeactivateIhf);
            }
        translatedActions [PhoneAction::ToolbarButton3] = centerButton2;     
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon(HbIcon("qtg_mono_contacts"));
        rightButton->setCommand(EPhoneInCallCmdContacts);
        translatedActions[PhoneAction::ToolbarButton4] = rightButton;   
        }
        break;
        
    case R_PHONEUI_CALLHANDLING_INCALL_HANDSET_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon("qtg_mono_dialer"));
        leftButton->setCommand(EPhoneInCallCmdDialer);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;
        
        PhoneAction *centerButton1 = new PhoneAction;
        if (!m_buttonsCtrl->getButtonFlags(PhoneUIQtButtonsController::Hold))
            {
            centerButton1->setIcon (HbIcon("qtg_mono_mic_mute"));
            centerButton1->setCommand (EPhoneInCallCmdMute);
            }
        else
            {
            centerButton1->setIcon (HbIcon("qtg_mono_mic_unmute"));
            centerButton1->setCommand (EPhoneInCallCmdUnmute);
            }
        translatedActions [PhoneAction::ToolbarButton2] = centerButton1;

        PhoneAction *centerButton2 = new PhoneAction;
        centerButton2->setIcon (HbIcon("qtg_mono_mobile"));
        centerButton2->setCommand (EPhoneInCallCmdDeactivateIhf);
        translatedActions [PhoneAction::ToolbarButton3] = centerButton2;
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon(HbIcon("qtg_mono_contacts"));
        rightButton->setCommand(EPhoneInCallCmdContacts);
        translatedActions[PhoneAction::ToolbarButton4] = rightButton;
        }
        break;

    case R_PHONEUI_MTCAL_INCOMING_CALL:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("txt_phone_other_incoming_call"));
        translatedActions [PhoneAction::Text] = text;
        }
        break;
        
    case R_PHONEUI_INCALL_CALL_DISCONNECTED:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("txt_phone_other_disconnected"));
        translatedActions [PhoneAction::Text] = text;
        }
        break;      
        
    case R_PHONEUI_INCALL_CALL_HELD:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("txt_phone_other_on_hold"));
        translatedActions [PhoneAction::Text] = text;    
        break;
        }

    case R_PHONEUI_MULTC_WAITING:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("txt_phone_other_waiting"));
        translatedActions [PhoneAction::Text] = text;
        }
        break; 

    case R_PHONEUI_INCALL_CALL_NUMBER:
        {
        PhoneAction *text = new PhoneAction;
        VA_LIST list;
        VA_START (list, symbianResourceId);
        int intValue = VA_ARG (list, int);
        text->setText(hbTrId("txt_phone_other_call_ln", intValue));
        translatedActions [PhoneAction::Text] = text;
        VA_END (list);
        }
        break;
        
    case R_PHONEUI_TIME_DURAT_LONG_WITH_ZERO:
        {
        PhoneAction *text = new PhoneAction;
        // Symbian modifier used here: %:0%H%:1%T%:2%S%:3
        // This is because this text is used by Symbian localization
        // for formatting later on.
        text->setText ("%:0%H%:1%T%:2%S%:3");
        translatedActions [PhoneAction::Text] = text;
        break;
        }
        
    case R_PHONEUI_OUT_GOING_CALL:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("txt_phone_other_calling"));
        translatedActions [PhoneAction::Text] = text;
        break;
        }
        
    case R_PHONEUI_MTCAL_CLI_WITHHELD:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("txt_phone_other_private_number"));
        translatedActions [PhoneAction::Text] = text;
        break;
        }
    case R_PHONEUI_MTCAL_CALL:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("txt_common_button_call")); //Call
        translatedActions [PhoneAction::Text] = text;
        break;
        }

    case R_PHONEUI_EMERGENCY_CALL_HEADER:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("txt_phone_other_emergency_call"));
        translatedActions [PhoneAction::Text] = text;            
        break;            
        }
        
    case R_PHONEUI_EMERGENCY_CALL_BUTTONS:
        {
        PhoneAction *button = new PhoneAction;
        button->setText (hbTrId("txt_phone_button_end_call"));
        button->setIcon (HbIcon("qtg_mono_end_call"));
        button->setCommand (EPhoneCmdEnd);
        translatedActions [PhoneAction::LeftButton] = button;
        }
        break;
        
    case R_PHONEUI_ATTEMPTING_EMERGENCY_CALL_TEXT:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("txt_phone_other_attempting"));
        translatedActions [PhoneAction::Text] = text;            
        }
        break;

    case R_PHONEUI_CONFERENCE_CALL:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("txt_phone_other_conference_call"));
        translatedActions [PhoneAction::Text] = text; 
        }
        break;
    case R_PHONEUI_SENDING_DTMF_WAIT_NOTE_TEXT:
        {
        PhoneAction *text = new PhoneAction;
        VA_LIST list;
        VA_START (list, symbianResourceId);
        const TDesC *string = (TDesC*)VA_ARG(list, TDesC*);
        text->setText (hbTrId("txt_phone_info_sending").arg(QString::fromUtf16(string->Ptr(), 
                string->Length())));
        translatedActions [PhoneAction::Text] = text;
        VA_END (list);
        }
        break;
    case R_PHONEUI_DTMF_WAIT_CHARACTER_CONFIRMATION_QUERY_TEXT:
        {
        PhoneAction *text = new PhoneAction;
        VA_LIST list;
        VA_START (list, symbianResourceId);
        const TDesC *string = (TDesC*)VA_ARG(list, TDesC*);
        text->setText (hbTrId("txt_phone_info_send_string").arg(QString::fromUtf16(string->Ptr(), 
                string->Length())));
        translatedActions [PhoneAction::Text] = text;
        VA_END (list);
        }
        break;
        
    default:
        break;
    }
    
    return translatedActions;
}

QString PhoneResourceAdapter::convertToString(int symbianResourceId) const
{
    QString ret;
    
    switch (symbianResourceId) {
        case R_PHONE_ERROR_CALL_NOT_ALLOWED:
            {
            ret = hbTrId("txt_phone_dpopinfo_not_allowed");      
            }
            break;
            
        case R_ERROR_NOTE_NOT_ALLOWED:
            {
            ret =  hbTrId("txt_phone_dpopinfo_not_allowed");  
            }
            break;
            
        case R_NOTETEXT_INVALID_PHONENUMBER:
            {
            ret = hbTrId("txt_phone_info_invalid_phone_number"); 
            }
            break;
            
        case R_NOTETEXT_NO_ANSWER:
            {
            ret = hbTrId("txt_phone_info_no_answer"); 
            }
            break;
        
        case R_PHONE_ERROR_NUMBER_BUSY:
            {
            ret = hbTrId("txt_phone_info_number_busy"); 
            }
            break;
            
        case R_PHONE_SS_NOTIFICATION_MOCAL_WAITING_TEXT:
            {
            ret = hbTrId("txt_phone_other_waiting");
            }
            break;
            
        case R_PHONE_SS_NOTIFICATION_INCAL_INFO_HOLD_TEXT:
            {
            ret = hbTrId("txt_phone_other_on_hold");
            }
            break;
            
        case R_INCAL_INFO_HOLD_TEXT:
            {
            ret = hbTrId("txt_phone_other_on_hold");
            }
            break;
            
        case R_INCAL_REMOTE_ACTIVE_TEXT:
            {
            ret = hbTrId("txt_phone_info_connected");
            }
            break;
            
        case R_INCAL_INFO_CONF_HOLD_TEXT:
            {
            ret = hbTrId("Conference on hold");
            }
            break;
        case R_PHONE_INCALL_INFO_NO_NETWORK_SUPPORT:
            {
            ret = hbTrId("txt_phone_info_no_network_support_for_video_call");
            }
            break;
        case R_PHONE_ERROR_EMERGENCY_CALLS_ONLY:
            {
            ret = hbTrId("txt_phone_info_emergency_calls_only");         
            }
            break;
        case R_NOTETEXT_NUMBER_NOT_IN_USE:
            {
            ret = hbTrId("txt_phone_info_number_not_in_use");         
            }
            break;
        case R_NOTETEXT_NETWORK_BUSY:
            {
            ret = hbTrId("txt_phone_info_network_busy");         
            }
            break;
        case R_NOTETEXT_ERROR_IN_CONNECTION:
            {
            ret = hbTrId("txt_phone_info_error_in_connection");         
            }
            break;
        case R_PHONEUI_ERROR_EMERGENCY_ATTEMPT_FAILED:
            {
            ret = hbTrId("txt_phone_info_emergency_call_failed");         
            }
            break;
        case R_PHONE_SS_NOTIFICATION_INCAL_TRANSFERRED_TEXT:
            {
            ret = hbTrId("txt_phone_info_transferred");         
            }
            break;
        case R_PHONE_ERROR_NO_SERVICE:
            {
            ret = hbTrId("txt_phone_dpopinfo_no_network_coverage");         
            }
            break;
        default:
            break;
    }

    return ret;
}

QString PhoneResourceAdapter::convertToString(
        int symbianResourceId, const QString &text) const
{
    QString ret;
    
    switch (symbianResourceId) {

        case R_PHONE_TEXT_COLP_CONNECTED:
            {
            ret = hbTrId("txt_phone_info_connected_to_1").arg(text);
            }
            break;
        default:
            break;
    }

    return ret;
}

int PhoneResourceAdapter::defaultToolbarResourceId() const
{
    return R_PHONEUI_CALLHANDLING_INCOMINGCALL_CBA;
}

QString PhoneResourceAdapter::convertCommandToString(
        int symbianCommandId ) const
{
    QString ret;
    
    switch(symbianCommandId) {
    case EPhoneInCallCmdEndAllCalls:
        ret = hbTrId("txt_phone_opt_end_all_calls");
        break;     
    case EPhoneInCallCmdTransfer:
        ret = hbTrId("txt_phone_opt_transfer");
        break;    
    case EPhoneInCallCmdSwitchToVideo:
        ret = hbTrId("Call back with video call");
        break;  
    case EPhoneCmdHandoverToWlan:
        ret = hbTrId("Handover to WLAN");
        break;  
    case EPhoneCmdHandoverToGsm:
        ret = hbTrId("Handover to GSM");
        break;  
        
    // TODO
    default:
        break;
    }

    return ret;
}

QList<HbAction*> PhoneResourceAdapter::convertToHbActions(
        int symbianResourceId ) const
{
    QList<HbAction*> ret;
    
    switch(symbianResourceId) {
        case R_PHONEUI_DTMF_WAIT_CHARACTER_CONFIRMATION_QUERY:
            {
            HbAction *leftButton = new HbAction(hbTrId("txt_common_button_send"));
            leftButton->setData(QVariant(EPhoneInCallCmdContinueSendingDtmfString));
            ret.append(leftButton);
    
            HbAction *rightButton = new HbAction(hbTrId("txt_common_button_cancel"));
            rightButton->setData(QVariant(EPhoneInCallCmdCancelSendingDtmfString));
            ret.append(rightButton);
            }
            break;
        case R_PHONEUI_SENDING_DTMF_WAIT_NOTE:
            {     
            HbAction *button = new HbAction(hbTrId("txt_common_button_cancel"));
            button->setData(QVariant(EPhoneInCallCmdCancelSendingDtmfString));
            ret.append(button); 
            }
            break;
        default:
            break;
    }

    return ret;
}

