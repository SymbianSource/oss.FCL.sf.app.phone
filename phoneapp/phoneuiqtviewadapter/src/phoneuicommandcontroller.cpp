/*!
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles phone ui commands.
*/

#include "phoneuicommandcontroller.h"
#include "phoneresourceadapter.h"
#include "phoneuiqtbuttonscontroller.h"
#include "qtphonelog.h"
#include <hbaction.h>
#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>
#include <xqphoneappcommands.h>
#include <phoneappcommands.hrh>
#include <phoneappvoipcommands.hrh>
#include <pevirtualengine.h>
#include <xqteluicommandextension.h>

  
PhoneUiCommandController::PhoneUiCommandController(
        PhoneUIQtViewIF &view, QObject *parent) 
: QObject (parent), m_view(view), m_lastCommandExtension(0)
{
    PHONE_DEBUG("PhoneMenuController::PhoneMenuController");
}

PhoneUiCommandController::~PhoneUiCommandController()
{
    PHONE_DEBUG("PhoneMenuController::~PhoneMenuController");
    for (int i=0;i<m_commandExtensions.count();++i) {
        m_commandExtensions.at(i)->release();
    }
}

void PhoneUiCommandController::setCallMenuActions(
        QMap<int,int> callStates, QMap<int,int> serviceIds, 
        int serviceId, int expandedCallId )
{
    PHONE_DEBUG("PhoneMenuController::setCallMenuActions");
    releaseMenu();
    
    PhoneCommandExtensionWrapper *extension = commandExtension(serviceId);
    QList<int> menuCmdList = menuCommands(callStates, serviceIds);
    
    if (extension) {
        QList<XQTelUiCommandExtension::CallInfo> callInfo;
        extension->getCallInfoList(
                callInfo,callStates,serviceIds,expandedCallId);
        
        //Modify command list by extension
        extension->modifyMenuCommandList(callInfo,menuCmdList);
        addMenuItems(menuCmdList);
        
        extension->addMenuActions(callInfo,m_customMenuActions);        
        for (int i=0;i<m_customMenuActions.count();++i) {
            m_view.menuReference().addAction(m_customMenuActions.at(i));
        }
    } else {
        addMenuItems(menuCmdList);
    }
        
    
}

void PhoneUiCommandController::setDialpadMenuActions()
{
    PHONE_DEBUG("PhoneMenuController::setDialpadMenuActions");
    releaseMenu();
    QList<PhoneAction*> values;
    m_view.setMenuActions(values);
    qDeleteAll(values);
}

QMap<PhoneAction::ActionType, PhoneAction *> 
    PhoneUiCommandController::pushButtonActionsForCall( 
        int callState,
        bool emergencyCall,
        QMap<int,int> callStates,
        QMap<int,int> serviceIds,
        int serviceId,
        int callId)
{
    QMap<PhoneAction::ActionType, PhoneAction *> translatedActions;
    bool sameServices = areServicesSame(callStates, serviceIds);
    QList<int> commands = buttonCommandList(
            callState, emergencyCall, sameServices, callStates.values());

    PhoneCommandExtensionWrapper *extension = commandExtension(serviceId);
    
    if (extension) {
        QList<XQTelUiCommandExtension::CallInfo> callInfo;
        extension->getCallInfoList(
                callInfo,callStates,serviceIds,callId);
        
        //Modify command list by extension
        extension->modifyPushButtonCommandList(callInfo,commands);
    }
    
    if (1 == commands.count()) {
        PhoneAction *action = mapCommandToAction(commands.at(0));
        if (action) {
            translatedActions[PhoneAction::LeftButton] = action;
        }
    } else if (1 < commands.count()){
        PhoneAction *action = mapCommandToAction(commands.at(0));
        PhoneAction *action2 = mapCommandToAction(commands.at(1));
        if (action) {
            translatedActions[PhoneAction::LeftButton] = action;
        }
        if (action2) {
            translatedActions[PhoneAction::RightButton] = action2;
        }
    }
    
    return translatedActions;
}

QList<int> PhoneUiCommandController::menuCommands(
        QMap<int,int> callStates, QMap<int,int> serviceIds )
{
    PHONE_DEBUG("PhoneMenuController::menuCommands");
    QList<int> commands;
    bool sameServices = areServicesSame(callStates,serviceIds);

    switch(callStates.values().count()) {
    case 1: {
        // No logical string for switch to video option	
        /*if (callStates.values().contains(EPEStateConnected)) {
            commands.append(PhoneInCallCmdSwitchToVideo);
        }*/
    }
    break;
    case 2: {
        if (!callStates.values().contains(EPEStateRinging) &&
            !callStates.values().contains(EPEStateDisconnecting)) {
            commands.append(PhoneInCallCmdEndAllCalls);
        }
        if (callStates.values().contains(EPEStateConnected) &&
            callStates.values().contains(EPEStateHeld)) {
            if (sameServices) {
                commands.append(PhoneInCallCmdTransfer);
            }
        }
    }
    break;
    case 3: {
        commands.append(PhoneInCallCmdEndAllCalls);    
    }
    break;
    default:
        break;
    }
    
    return commands;
}

void PhoneUiCommandController::addMenuItems(QList<int> menuCmdList)
{
    QList<PhoneAction*> values;
    
    for (int i=0;i<menuCmdList.count();++i) {
        int command = mapCommand(menuCmdList.at(i));
        if (-1 != command) {
            PhoneAction* phoneAction = new PhoneAction();
            phoneAction->setCommand(command);
            phoneAction->setText(
                    PhoneResourceAdapter::Instance()->
                        convertCommandToString(command));
            
            values.append(phoneAction);
        }
    }
    
    m_view.setMenuActions(values);
    qDeleteAll(values);
}

int PhoneUiCommandController::mapCommand(int command)
{
    int ret(-1);
    
    switch(command) {
    case PhoneInCallCmdEndAllCalls:
        ret = EPhoneInCallCmdEndAllCalls;
        break;     
    case PhoneInCallCmdTransfer:
        ret = EPhoneInCallCmdTransfer;
        break;    
    case PhoneInCallCmdSwitchToVideo:
        ret = EPhoneInCallCmdSwitchToVideo;
        break;  
    case PhoneInCallCmdSendMessage:
        //TODO
        break;    
    case PhoneInCallShareVideo:
        //TODO
        break;
    case PhoneInCallCmdHandoverToWlan:
        ret = EPhoneCmdHandoverToWlan;
        break;
    case PhoneInCallCmdHandoverToGsm:
        ret = EPhoneCmdHandoverToGsm;
        break;
    default:
        break;
    }
    
    return ret;
}

PhoneCommandExtensionWrapper *PhoneUiCommandController::commandExtension(
        int serviceId)
{
    PhoneCommandExtensionWrapper *extension = 0;
    TRAP_IGNORE( extension = CommandExtensionL(serviceId) );
    return extension;
}

TUid PhoneUiCommandController::ResolveImplementationUidL( 
        TUint32 aServiceId, TServicePropertyName aPropertyName )
    {
    TUid implUid = { 0 };
    CSPEntry* entry = CSPEntry::NewLC();

    CSPSettings* spSettings = CSPSettings::NewLC(); 
    
    User::LeaveIfError(
        spSettings->FindEntryL( aServiceId, *entry ) ); 

    CleanupStack::PopAndDestroy( spSettings ); 

    const CSPProperty* property = NULL;
    User::LeaveIfError( entry->GetProperty( property, aPropertyName ) );
    
    if ( property ) 
        {
        TInt temp(0);
        User::LeaveIfError( property->GetValue( temp ) );
        implUid.iUid = temp;
        }

    CleanupStack::PopAndDestroy( entry );
    return implUid;   
    }

PhoneCommandExtensionWrapper 
    *PhoneUiCommandController::CommandExtensionL( TUint aServiceId )
{
    TUid pluginUid = ResolveImplementationUidL( aServiceId, 
        EPropertyCallMenuHandlerPluginId );
    
    int pluginInd(-1);
    PhoneCommandExtensionWrapper* wrapper = NULL;
    
    for (int i=0; i<m_commandExtensions.count();++i) {
        if (pluginUid.iUid==m_commandExtensions.at(i)->pluginUid()) {
            pluginInd = i;
            break;
        }
    }
    
    if ( -1 == pluginInd )
        {
        wrapper = new PhoneCommandExtensionWrapper( pluginUid.iUid );
        if (wrapper)
            m_commandExtensions.append( wrapper );
        }
    else
        {
        wrapper = m_commandExtensions[pluginInd];
        }
    
    m_lastCommandExtension = wrapper;
    return wrapper;
}

void PhoneUiCommandController::releaseMenu()
{
    PHONE_DEBUG("PhoneMenuController::setDialpadMenuActions");
    for (int i=0;i<m_customMenuActions.count();i++) {
        m_view.menuReference().removeAction(m_customMenuActions.at(i));
    }
    m_customMenuActions.clear();
    
    if (m_lastCommandExtension) {
        m_lastCommandExtension->releaseMenu();
        m_lastCommandExtension = 0;
    }
}

bool PhoneUiCommandController::areServicesSame(
        QMap<int,int> callStates, QMap<int,int> serviceIds)
{
    bool ret(true);
    int serviceId(-1);
    for(int i=0;i<callStates.count()&&ret;++i) {
        if (-1==serviceId) {
            serviceId = serviceIds.value(callStates.keys().at(i));
        } else if (serviceId!=serviceIds.value(callStates.keys().at(i))) {
            ret = false;
        }
    }
    
    return ret;
}

QList<int> PhoneUiCommandController::buttonCommandList(
        int callState,
        bool emergencyCall,
        bool sameServices,
        QList<int> callStates)
{
    QList<int> ret;
    
    switch( callState ) {
    case EPEStateIdle:
    case EPEStateDisconnecting:
    case EPEStateConferenceIdle:    
    break;
    
    case EPEStateDialing:
    case EPEStateConnecting: {
        ret.append(PhoneInCallCmdEndOutgoingCall);  
    }
    break;
    case EPEStateRinging: {
        if (callStates.size()<3){
            ret.append(PhoneInCallCmdAnswer);
            ret.append(PhoneInCallCmdReject); 
        } else {
            ret.append(PhoneInCallCmdReplace); 
        }
    }
    break;
    
    case EPEStateConnected:
    case EPEStateConnectedConference: {
        if (!emergencyCall) {
            if ( 1 == callStates.size() || 
                 (2 == callStates.size() && 
                  callStates.contains(EPEStateRinging)) ) {
                ret.append(PhoneInCallCmdHold);
            } else {
                
                if (EPEStateConnectedConference == callState) {
                    ret.append(PhoneInCallCmdSwap);
                } else if (sameServices && 
                    false == PhoneResourceAdapter::Instance()->buttonsController()->
                        getButtonFlags(PhoneUIQtButtonsController::DisableJoin)) {
                    if ( callStates.contains(EPEStateHeldConference) ) {
                        ret.append(PhoneInCallCmdJoinToConference);                  
                    } else {
                        ret.append(PhoneInCallCmdCreateConference);
                    }
                }
            }            
        }
        
        ret.append(PhoneInCallCmdEndActive); 
    }
    break;
    
    case EPEStateHeld:
    case EPEStateHeldConference: {
        if (1 == callStates.size() || 
            (2 == callStates.size() && 
             callStates.contains(EPEStateRinging))) {
             
            ret.append(PhoneInCallCmdUnhold);
        } else {
            if (EPEStateHeldConference == callState) {
                ret.append(PhoneInCallCmdSwap);
            } else if (sameServices && 
                    false == PhoneResourceAdapter::Instance()->buttonsController()->
                        getButtonFlags(PhoneUIQtButtonsController::DisableJoin)) {
                if ( callStates.contains(EPEStateConnectedConference)) {
                    ret.append(PhoneInCallCmdJoinToConference);
                } else {
                    ret.append(PhoneInCallCmdCreateConference);
                }
            }
        }

        ret.append(PhoneInCallCmdEndActive);
    }
    break;
    
    case EPEStateUnknown:
    break;
    
    default:
    break;    
    }
    
    return ret;
}

PhoneAction *PhoneUiCommandController::mapCommandToAction(int command)
{
    PhoneAction *action=0;
    
    switch( command ) {
    case PhoneInCallCmdJoinToConference: {
        action = new PhoneAction;
        action->setIcon(HbIcon("qtg_mono_join_call"));
        action->setCommand(EPhoneInCallCmdJoin);
    }
    break;

    case PhoneInCallCmdCreateConference: {
        action = new PhoneAction;
        action->setIcon(HbIcon("qtg_mono_join_call"));
        action->setCommand(EPhoneInCallCmdCreateConference);
    }
    break;
        
    case PhoneInCallCmdAnswer: {
        action = new PhoneAction;
        action->setIcon(HbIcon("qtg_mono_call"));
        action->setCommand (EPhoneCallComingCmdAnswer);
        action->setActionRole(PhoneAction::Accept);
    }
    break;
    
    case PhoneInCallCmdReject: {
        action = new PhoneAction;
        action->setIcon(HbIcon("qtg_mono_reject_call"));
        action->setCommand (EPhoneCallComingCmdReject);
        action->setActionRole(PhoneAction::Decline);
    }
    break;
    
    case PhoneInCallCmdHold: {
        action = new PhoneAction;
        action->setIcon(HbIcon("qtg_mono_hold_call"));
        action->setCommand(EPhoneInCallCmdHold);
    }
    break;
    
    case PhoneInCallCmdUnhold: {
        action = new PhoneAction;
        action->setIcon(HbIcon("qtg_mono_call"));
        action->setCommand(EPhoneInCallCmdUnhold);
    }
    break;
    
    case PhoneInCallCmdEndActive: {
        action = new PhoneAction;
        action->setIcon(HbIcon("qtg_mono_end_call"));
        action->setCommand(EPhoneInCallCmdEndThisActiveCall);
        action->setActionRole(PhoneAction::Decline);
    }
    break;
    
    case PhoneInCallCmdEndOutgoingCall: {
        action = new PhoneAction;
        action->setIcon(HbIcon("qtg_mono_end_call"));
        action->setCommand(EPhoneInCallCmdEndThisOutgoingCall);
        action->setActionRole(PhoneAction::Decline);
    }
    break;

    case PhoneInCallCmdReplace: {
        action = new PhoneAction;
        action->setIcon(HbIcon("qtg_mono_replace_call"));
        action->setCommand(EPhoneInCallCmdReplace);
        action->setActionRole(PhoneAction::Accept);
    }
    break;
    
    case PhoneInCallCmdSwap: {
        action = new PhoneAction;
        action->setIcon(HbIcon("qtg_mono_swap"));
        action->setCommand(EPhoneInCallCmdSwap); 
    }
    break;
    
    default:
    break;
    }
            
    return action;
}
