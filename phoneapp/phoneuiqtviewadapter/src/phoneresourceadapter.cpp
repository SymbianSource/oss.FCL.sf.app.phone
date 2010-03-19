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
    case R_PHONEUI_INCOMING_CALL_BUTTONS:
    case R_PHONEUI_WAITING_CALL_BUTTONS:        
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setText (hbTrId("txt_common_button_answer"));
        leftButton->setIcon (HbIcon(":/pri_large_button_answer.svg"));
        leftButton->setCommand (EPhoneCallComingCmdAnswer);
        translatedActions [PhoneAction::LeftButton] = leftButton;
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setText (hbTrId("txt_common_button_reject"));
        rightButton->setIcon (HbIcon(":/qtg_large_button_reject.svg"));
        rightButton->setCommand (EPhoneCallComingCmdReject);
        translatedActions [PhoneAction::RightButton] = rightButton;
        }
        break;
        
    case R_PHONEUI_CALL_SETUP_BUTTONS:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setText (hbTrId("txt_phone_button_end_call"));
        leftButton->setIcon (HbIcon(":/pri_large_button_endcall.svg"));
        leftButton->setCommand (EPhoneInCallCmdEndThisOutgoingCall);
        translatedActions [PhoneAction::LeftButton] = leftButton;
        }
        break;

    case R_PHONEUI_INCALL_BUTTONS:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setText (hbTrId("txt_phone_button_hold"));
        leftButton->setIcon (HbIcon(":/qtg_large_button_hold.svg"));
        leftButton->setCommand (EPhoneInCallCmdHold);
        translatedActions [PhoneAction::LeftButton] = leftButton;

        PhoneAction *rightButton = new PhoneAction;
        rightButton->setText (hbTrId("txt_phone_button_end_call"));
        rightButton->setIcon (HbIcon(":/pri_large_button_endcall.svg"));
        rightButton->setCommand (EPhoneInCallCmdEndThisActiveCall);
        translatedActions [PhoneAction::RightButton] = rightButton;
        }
        break;
        
    case R_PHONEUI_INCALL_HELD_BUTTONS:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setText (hbTrId("txt_phone_button_unhold"));
        leftButton->setIcon (HbIcon(":/qtg_large_button_onhold.svg"));
        leftButton->setCommand (EPhoneInCallCmdUnhold);
        translatedActions [PhoneAction::LeftButton] = leftButton;

        PhoneAction *rightButton = new PhoneAction;
        rightButton->setText (hbTrId("txt_phone_button_end_call"));
        rightButton->setIcon (HbIcon(":/pri_large_button_endcall.svg"));
        rightButton->setCommand (EPhoneInCallCmdEndThisActiveCall);
        translatedActions [PhoneAction::RightButton] = rightButton;
        }
        break;

    case R_PHONEUI_TWO_SINGLES_BUTTONS:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setText (hbTrId("txt_phone_button_swap"));
        leftButton->setIcon (HbIcon(":/qgn_indi_button_swap.svg"));
        leftButton->setCommand (EPhoneInCallCmdSwap);
        translatedActions [PhoneAction::LeftButton] = leftButton;

        PhoneAction *rightButton = new PhoneAction;
        rightButton->setText (hbTrId("txt_phone_button_end_call"));
        rightButton->setIcon (HbIcon(":/pri_large_button_endcall.svg"));
        rightButton->setCommand (EPhoneInCallCmdEndThisActiveCall);
        translatedActions [PhoneAction::RightButton] = rightButton;                        
        }
        break;
    
    case R_PHONEUI_CALLHANDLING_INCOMINGCALL_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon(":/qtg_large_comp_message.svg"));
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;       
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon (HbIcon(":/qtg_large_tb_silence.svg"));
        rightButton->setCommand (EPhoneCallComingCmdSilent);
        translatedActions [PhoneAction::ToolbarButton2] = rightButton;            
        }
        break;

    case R_PHONEUI_CALLHANDLING_INCOMINGCALL_REJECT_CBA:
    case R_PHONEUI_CALLHANDLING_INCOMINGCALL_SOFT_REJECT_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon(":/qtg_large_comp_message.svg"));
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon (HbIcon(":/qtg_large_tb_silence.svg"));
        rightButton->setDisabled(true);
        translatedActions [PhoneAction::ToolbarButton2] = rightButton;        
        }
        break;
        
    case R_PHONEUI_CALLHANDLING_CALLWAITING_CBA:        
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon(":/qtg_large_comp_message.svg"));
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;       
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon (HbIcon(":/qtg_large_tb_join.svg"));
        rightButton->setCommand (EPhoneInCallCmdJoin);
        translatedActions [PhoneAction::ToolbarButton2] = rightButton;            
        }
        break;        
        
    case R_PHONEUI_DIALER_CBA:
    case R_PHONEUI_INCALL_DIALER_CBA:
    case R_PHONEUI_CALLHANDLING_CALLSETUP_EMPTY_DTMFDIALER_CBA:
    case R_PHONEUI_CALLHANDLING_INCALL_UNHOLD_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon(":/qtg_large_tb_dialler.svg"));
        leftButton->setCommand(EPhoneInCallCmdDialer);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;
        
        PhoneAction *centerButton = new PhoneAction;
        centerButton->setIcon (HbIcon(":/qtg_large_tb_mute.svg"));
        centerButton->setCommand (EPhoneInCallCmdMute);
        translatedActions [PhoneAction::ToolbarButton2] = centerButton;        

        PhoneAction *centerButton2 = new PhoneAction;
        if (!m_buttonsCtrl->getButtonFlags(PhoneUIQtButtonsController::Ihf))
            {
            centerButton2->setIcon (HbIcon(":/qtg_large_tb_loudsp_unmute.svg"));
            centerButton2->setCommand (EPhoneInCallCmdActivateIhf);
            }
        else
            {
            centerButton2->setIcon (HbIcon(":/qtg_large_tb_loudsp_mute.svg"));
            centerButton2->setCommand (EPhoneInCallCmdDeactivateIhf);
            }
        translatedActions [PhoneAction::ToolbarButton3] = centerButton2; 
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon(HbIcon(":/qtg_large_tb_contacts.svg"));
        rightButton->setDisabled( 
                (symbianResourceId==
                 R_PHONEUI_CALLHANDLING_CALLSETUP_EMPTY_DTMFDIALER_CBA));
        
        translatedActions[PhoneAction::ToolbarButton4] = rightButton;
        }
        break;
        
    case R_PHONEUI_CALLHANDLING_EMERGENCY_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon(":/qtg_large_tb_dialler.svg"));
        leftButton->setCommand(EPhoneInCallCmdDialer);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;
        
        PhoneAction *rightButton = new PhoneAction;
        if (!m_buttonsCtrl->getButtonFlags(PhoneUIQtButtonsController::Ihf))
            {
            rightButton->setIcon (HbIcon(":/qtg_large_tb_loudsp_unmute.svg"));
            rightButton->setCommand (EPhoneInCallCmdActivateIhf);
            }
        else
            {
            rightButton->setIcon (HbIcon(":/qtg_large_tb_loudsp_mute.svg"));
            rightButton->setCommand (EPhoneInCallCmdDeactivateIhf);
            }
        translatedActions [PhoneAction::ToolbarButton2] = rightButton;                    
        }
        break;
        
    case R_PHONEUI_CALLHANDLING_INCALL_UNMUTE_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon(":/qtg_large_tb_dialler.svg"));
        leftButton->setCommand(EPhoneInCallCmdDialer);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;
        
        PhoneAction *centerButton = new PhoneAction;
        centerButton->setIcon (HbIcon(":/qtg_large_tb_unmute.svg"));
        centerButton->setCommand (EPhoneInCallCmdUnmute);
        translatedActions [PhoneAction::ToolbarButton2] = centerButton;        

        PhoneAction *centerButton2 = new PhoneAction;
        if (!m_buttonsCtrl->getButtonFlags(PhoneUIQtButtonsController::Ihf))
            {
            centerButton2->setIcon (HbIcon(":/qtg_large_tb_loudsp_unmute.svg"));
            centerButton2->setCommand (EPhoneInCallCmdActivateIhf);
            }
        else
            {
            centerButton2->setIcon (HbIcon(":/qtg_large_tb_loudsp_mute.svg"));
            centerButton2->setCommand (EPhoneInCallCmdDeactivateIhf);
            }
        translatedActions [PhoneAction::ToolbarButton3] = centerButton2;     
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon(HbIcon(":/qtg_large_tb_contacts.svg"));
        translatedActions[PhoneAction::ToolbarButton4] = rightButton;   
        }
        break;
        
    case R_PHONEUI_CALLHANDLING_INCALL_HANDSET_CBA:
        {
        PhoneAction *leftButton = new PhoneAction;
        leftButton->setIcon (HbIcon(":/qtg_large_tb_dialler.svg"));
        leftButton->setCommand(EPhoneInCallCmdDialer);
        translatedActions [PhoneAction::ToolbarButton1] = leftButton;
        
        PhoneAction *centerButton1 = new PhoneAction;
        if (!m_buttonsCtrl->getButtonFlags(PhoneUIQtButtonsController::Hold))
            {
            centerButton1->setIcon (HbIcon(":/qtg_large_tb_mute.svg"));
            centerButton1->setCommand (EPhoneInCallCmdMute);
            }
        else
            {
            centerButton1->setIcon (HbIcon(":/qtg_large_tb_unmute.svg"));
            centerButton1->setCommand (EPhoneInCallCmdUnmute);
            }
        translatedActions [PhoneAction::ToolbarButton2] = centerButton1;

        PhoneAction *centerButton2 = new PhoneAction;
        centerButton2->setIcon (HbIcon(":/qtg_large_tb_loudsp_mute.svg"));
        centerButton2->setCommand (EPhoneInCallCmdDeactivateIhf);
        translatedActions [PhoneAction::ToolbarButton3] = centerButton2;
        
        PhoneAction *rightButton = new PhoneAction;
        rightButton->setIcon(HbIcon(":/qtg_large_tb_contacts.svg"));
        translatedActions[PhoneAction::ToolbarButton4] = rightButton;
        }
        break;

    case R_PHONEUI_MTCAL_INCOMING_CALL:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("calling"));
        translatedActions [PhoneAction::Text] = text;
        }
        break;
        
    case R_PHONEUI_INCALL_CALL_DISCONNECTED:
        {
        PhoneAction *text = new PhoneAction;
        text->setText (hbTrId("disconnected"));
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
        text->setText (hbTrId("txt_phone_other_call_ln").arg (intValue));
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
        button->setIcon (HbIcon(":/pri_large_button_endcall.svg"));
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
        text->setText (hbTrId("Sending:\n%1").arg(QString::fromUtf16(string->Ptr(), 
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
        text->setText (hbTrId("Send string:\n%1").arg(QString::fromUtf16(string->Ptr(), 
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
            ret = hbTrId("Call not allowed");      
            }
            break;
            
        case R_ERROR_NOTE_NOT_ALLOWED:
            {
            ret =  hbTrId("Not allowed");  
            }
            break;
            
        case R_NOTETEXT_INVALID_PHONENUMBER:
            {
            ret = hbTrId("Invalid phone number"); 
            }
            break;
            
        case R_NOTETEXT_NO_ANSWER:
            {
            ret = hbTrId("No answer"); 
            }
            break;
        
        case R_PHONE_ERROR_NUMBER_BUSY:
            {
            ret = hbTrId("Number busy"); 
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
            ret = hbTrId("Connected");
            }
            break;
            
        case R_INCAL_INFO_CONF_HOLD_TEXT:
            {
            ret = hbTrId("Conference on hold");
            }
            break;
        case R_PHONE_INCALL_INFO_NO_NETWORK_SUPPORT:
            {
            ret = hbTrId("No network support for video call");
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
            ret = hbTrId("Connected to %1").arg(text);
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

