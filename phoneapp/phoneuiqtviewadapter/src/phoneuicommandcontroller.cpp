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
#include "phoneconstants.h"
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
    PHONE_TRACE
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
    QList<HbAction *> values;
    m_view.setMenuActions(values);
}

QList<HbAction *> PhoneUiCommandController::pushButtonActionsForCall( 
        int callState,
        bool emergencyCall,
        QMap<int,int> callStates,
        QMap<int,int> serviceIds,
        int serviceId,
        int callId)
{
    PHONE_TRACE
    QList<HbAction *> translatedActions;
    QList<int> commands = buttonCommandList(
            callState, emergencyCall, callStates.values());

    PhoneCommandExtensionWrapper *extension = commandExtension(serviceId);
    
    if (extension) {
        QList<XQTelUiCommandExtension::CallInfo> callInfo;
        extension->getCallInfoList(
                callInfo,callStates,serviceIds,callId);
        
        //Modify command list by extension
        extension->modifyPushButtonCommandList(callInfo,commands);
    }
    
    if (1 == commands.count()) {
        HbAction *action = createAction(commands.at(0));
        if (action) {
            translatedActions.append(action);
        }
    } else if (1 < commands.count()){
        HbAction *action = createAction(commands.at(0));
        HbAction *action2 = createAction(commands.at(1));
        if (action) {
            translatedActions.append(action);
        }
        if (action2) {
            translatedActions.append(action2);
        }
    }
    
    return translatedActions;
}

QList<HbAction *> PhoneUiCommandController::toolBarActions( 
        int resourceId,
        QMap<int,int> callStates,
        QMap<int,int> serviceIds,
        int serviceId,
        int callId)
{
    PHONE_TRACE
    
    //Set tool bar button flags
    setJoinFlag(callStates, serviceIds);
    setConferenceFlag(callStates.values());
    setHoldFlag(callStates.value(callId));
    setMultiCallFlag(callStates.values());
    setOutgoingFlag(callStates.values());
    
    //Get tool bar item list by resource id.
    QList<XQTelUiCommandExtension::ToolBarCommand> toolBarCmdList = 
            PhoneResourceAdapter::Instance()->convertToToolBarCommandList(resourceId);

    if (serviceId != -1) {
        PhoneCommandExtensionWrapper *extension = commandExtension(serviceId);
        
        if (extension) {
            QList<XQTelUiCommandExtension::CallInfo> callInfo;
            extension->getCallInfoList(
                    callInfo,callStates,serviceIds,callId);

            //Modify tool bar command list by extension
            extension->modifyToolBarCommandList(callInfo,toolBarCmdList);

        }
    }
    
    QList<HbAction *> actions;
    for ( int i=0; i < toolBarCmdList.count(); ++i) {
        HbAction *action = createAction(
                        toolBarCmdList[i].mCommandId,
                        toolBarCmdList[i].mIsEnabled);
        if (action) {
            actions.append(action);
        }
    }
    
    return actions;
}

QList<int> PhoneUiCommandController::menuCommands(
        QMap<int,int> callStates, QMap<int,int> serviceIds ) const
{
    PHONE_TRACE
    QList<int> commands;
    bool sameServices = areServicesSame(callStates,serviceIds);
    bool isEmergencyCall = emergencyCall(callStates);
    
    // No menu items if an emergency call ongoing.
    if (false == isEmergencyCall) {
        switch(callStates.values().count()) {
        case 1: {            
        }
        break;
        case 2: {
            if (!callStates.values().contains(EPEStateRinging) &&
                !callStates.values().contains(EPEStateDisconnecting) &&
                !callStates.values().contains(EPEStateDialing)) {
                commands.append(PhoneInCallCmdEndAllCalls);
            }
            if ((callStates.values().contains(EPEStateConnected) &&
                 callStates.values().contains(EPEStateHeld)) ||
                (callStates.values().contains(EPEStateConnecting) &&
                 callStates.values().contains(EPEStateHeld))) {
                if (sameServices) {
                    commands.append(PhoneInCallCmdTransfer);
                }
            }
        }
        break;
        case 3: {
            commands.append(PhoneInCallCmdEndAllCalls);
            
            if (callStates.values().contains(EPEStateConnected) &&
                callStates.values().contains(EPEStateHeld) &&
                callStates.values().contains(EPEStateRinging)) {
                if (sameServices) {
                    commands.append(PhoneInCallCmdTransfer);
                }
            }
        }
        break;
        default:
            break;
        }
    }
    
    return commands;
}

void PhoneUiCommandController::addMenuItems(QList<int> menuCmdList)
{
    QList<HbAction *> values;
    
    for (int i=0;i<menuCmdList.count();++i) {
        int command = mapCommand(menuCmdList.at(i));
        if (-1 != command) {
            QScopedPointer<HbAction> action(new HbAction());
            action->setProperty("command", command);
            action->setText(
                    PhoneResourceAdapter::Instance()->
                        convertCommandToString(command));
            
            values.append(action.take());
        }
    }
    
    m_view.setMenuActions(values);
}

int PhoneUiCommandController::mapCommand(int extensionCommand) const
{
    int ret(-1);
    
    switch(extensionCommand) {
    case PhoneInCallCmdEndAllCalls:
        ret = EPhoneInCallCmdEndAllCalls;
        break;     
    case PhoneInCallCmdTransfer:
        ret = EPhoneInCallCmdTransfer;
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
        TUint32 aServiceId, TServicePropertyName aPropertyName ) const
    {
    PHONE_TRACE
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
    PHONE_TRACE
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
    
    if ( -1 == pluginInd ) {
        wrapper = new PhoneCommandExtensionWrapper( pluginUid.iUid );
        if (wrapper) {
            m_commandExtensions.append( wrapper );
        }
    } else {
        wrapper = m_commandExtensions[pluginInd];
    }
    
    m_lastCommandExtension = wrapper;
    return wrapper;
}

void PhoneUiCommandController::releaseMenu()
{
    PHONE_TRACE
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
        QMap<int,int> callStates, QMap<int,int> serviceIds) const
{
    bool ret(true);
    int serviceId(-1);
    for(int i=0;i<callStates.count()&&ret;++i) {
        if (-1==serviceId) {
            serviceId = serviceIds.value(callStates.keys().at(i));
        } else if (serviceId != serviceIds.value(callStates.keys().at(i))) {
            ret = false;
        }
    }
    
    return ret;
}

void PhoneUiCommandController::setJoinFlag(
        QMap<int,int> callStates, QMap<int,int> serviceIds) const
{
    bool disable(PhoneResourceAdapter::Instance()->buttonsController()->
                    getButtonFlags(PhoneUIQtButtonsController::FullConference));
    
    if (!disable) {
        disable = !areServicesSame(callStates, serviceIds);
    }

    PhoneResourceAdapter::Instance()->buttonsController()->
            setButtonFlags(PhoneUIQtButtonsController::DisableJoin, disable);
}

void PhoneUiCommandController::setHoldFlag(int callState) const
{
    bool hold = (callState == EPEStateHeldConference ||
                 callState == EPEStateHeld );

    PhoneResourceAdapter::Instance()->buttonsController()->
            setButtonFlags(PhoneUIQtButtonsController::Hold,hold);
}

void PhoneUiCommandController::setOutgoingFlag(
        QList<int> callStates) const
{
    bool outgoing(callStates.contains(EPEStateDialing) ||
                  callStates.contains(EPEStateConnecting));
    
    PhoneResourceAdapter::Instance()->buttonsController()->
            setButtonFlags(PhoneUIQtButtonsController::Outgoing, outgoing);
}

void PhoneUiCommandController::setConferenceFlag(
        QList<int> callStates) const
{
    bool conference(callStates.contains(EPEStateConnectedConference) ||
                    callStates.contains(EPEStateHeldConference));
    
    PhoneResourceAdapter::Instance()->buttonsController()->
            setButtonFlags(PhoneUIQtButtonsController::Conference, conference);
}

void PhoneUiCommandController::setMultiCallFlag(
        QList<int> callStates) const
{
    bool multicall(false);
    
    if (1<callStates.count()) {  
        if (callStates.count() == 2 && callStates.contains(EPEStateRinging)) {
            multicall = false;
        } else {
            multicall = true;   
        }        
    }
    
    PhoneResourceAdapter::Instance()->buttonsController()->
            setButtonFlags(PhoneUIQtButtonsController::MultiCall, multicall);
}

QList<int> PhoneUiCommandController::buttonCommandList(
        int callState,
        bool emergencyCall,
        QList<int> callStates) const
{
    PHONE_TRACE
    QList<int> ret;
    
    switch( callState ) {
    case EPEStateIdle:
    case EPEStateDisconnecting:
    case EPEStateConferenceIdle:    
    break;
    
    case EPEStateDialing:
    case EPEStateConnecting: {
        if (!emergencyCall) {
            if (PhoneResourceAdapter::Instance()->buttonsController()->
                   getButtonFlags(
                           PhoneUIQtButtonsController::IhfAsPushButton)) {
                if (PhoneResourceAdapter::Instance()->buttonsController()->getButtonFlags(
                        PhoneUIQtButtonsController::BluetoothAvailable)) {
                    if (PhoneResourceAdapter::Instance()->buttonsController()->getButtonFlags(
                            PhoneUIQtButtonsController::Btaa)) {
                        ret.append(PhoneInCallCmdHandset);
                    } else {
                        ret.append(PhoneInCallCmdBtHandsfree);
                    } 
                } else {
                    if (!PhoneResourceAdapter::Instance()->buttonsController()->
                            getButtonFlags(PhoneUIQtButtonsController::Ihf)) {
                        ret.append(PhoneInCallCmdActivateIhf);
                    } else {
                        ret.append(PhoneInCallCmdDeactivateIhf);
                    }           
                }
            } else {
                if (PhoneResourceAdapter::Instance()->buttonsController()->
                        getButtonFlags(PhoneUIQtButtonsController::Mute)) {
                    ret.append(PhoneInCallCmdUnmute);
                } else {
                    ret.append(PhoneInCallCmdMute);
                }
            }
        }
        
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
    case EPEStateConnectedConference:     
    case EPEStateHeld:
    case EPEStateHeldConference: {
        if (!emergencyCall) {
            if (PhoneResourceAdapter::Instance()->buttonsController()->
                   getButtonFlags(
                           PhoneUIQtButtonsController::IhfAsPushButton)) {
                if (PhoneResourceAdapter::Instance()->buttonsController()->getButtonFlags(
                        PhoneUIQtButtonsController::BluetoothAvailable)) {
                    if (PhoneResourceAdapter::Instance()->buttonsController()->getButtonFlags(
                            PhoneUIQtButtonsController::Btaa)) {
                        ret.append(PhoneInCallCmdHandset);
                    } else {
                        ret.append(PhoneInCallCmdBtHandsfree);
                    } 
                } else {
                    if (!PhoneResourceAdapter::Instance()->buttonsController()->
                            getButtonFlags(PhoneUIQtButtonsController::Ihf)) {
                        ret.append(PhoneInCallCmdActivateIhf);
                    } else {
                        ret.append(PhoneInCallCmdDeactivateIhf);
                    }           
                }
            } else {
                if (PhoneResourceAdapter::Instance()->buttonsController()->
                        getButtonFlags(PhoneUIQtButtonsController::Mute)) {
                    ret.append(PhoneInCallCmdUnmute);
                } else {
                    ret.append(PhoneInCallCmdMute);
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



HbAction *PhoneUiCommandController::createAction(
        int extensionCommand, bool enabled) const
{
    QScopedPointer<HbAction> action(new HbAction);
    action->setEnabled(enabled);
    
    switch (extensionCommand) {
    case PhoneInCallCmdJoinToConference: {
        action->setIcon(HbIcon("qtg_mono_join_call"));
        action->setProperty("command", EPhoneInCallCmdJoin);
    }
    break;

    case PhoneInCallCmdCreateConference: {
        action->setIcon(HbIcon("qtg_mono_join_call"));
        action->setProperty("command", EPhoneInCallCmdCreateConference);
    }
    break;
        
    case PhoneInCallCmdAnswer: {
        action->setIcon(HbIcon("qtg_mono_call"));
        action->setProperty("command", EPhoneCallComingCmdAnswer);        
        action->setSoftKeyRole(QAction::PositiveSoftKey);
    }
    break;
    
    case PhoneInCallCmdReject: {
        action->setIcon(HbIcon("qtg_mono_reject_call"));
        action->setProperty("command", EPhoneCallComingCmdReject);
        action->setSoftKeyRole(QAction::NegativeSoftKey);
    }
    break;
    
    case PhoneInCallCmdHold: {
        action->setIcon(HbIcon("qtg_mono_hold_call"));
        action->setProperty("command", EPhoneInCallCmdHold);
    }
    break;
    
    case PhoneInCallCmdUnhold: {
        action->setIcon(HbIcon("qtg_mono_call"));
        action->setProperty("command", EPhoneInCallCmdUnhold);
    }
    break;
    
    case PhoneInCallCmdEndActive: {
        action->setIcon(HbIcon("qtg_mono_end_call"));
        action->setProperty("command", EPhoneInCallCmdEndThisActiveCall);
        action->setSoftKeyRole(QAction::NegativeSoftKey);
    }
    break;
    
    case PhoneInCallCmdEndOutgoingCall: {
        action->setIcon(HbIcon("qtg_mono_end_call"));
        action->setProperty("command", EPhoneInCallCmdEndThisOutgoingCall);
        action->setSoftKeyRole(QAction::NegativeSoftKey);
    }
    break;

    case PhoneInCallCmdReplace: {
        action->setIcon(HbIcon("qtg_mono_replace_call"));
        action->setProperty("command", EPhoneInCallCmdReplace);
        action->setSoftKeyRole(QAction::PositiveSoftKey);
    }
    break;
    
    case PhoneInCallCmdSwap: {
        action->setIcon(HbIcon("qtg_mono_swap_call"));
        action->setProperty("command", EPhoneInCallCmdSwap); 
    }
    break;
    case PhoneCallComingCmdSoftReject: {
        action->setIcon(HbIcon("qtg_mono_send"));
        action->setProperty("command", EPhoneCallComingCmdSoftReject); 
    }
    break;
    case PhoneCallComingCmdSilent: {
        action->setIcon(HbIcon("qtg_mono_speaker_off"));
        action->setProperty("command", EPhoneCallComingCmdSilent); 
    }
    break;
    case PhoneInCallCmdOpenDialer: {
        action->setIcon(HbIcon("qtg_mono_dialer"));
        action->setProperty("command", EPhoneInCallCmdDialer); 
    }
    break;
    case PhoneInCallCmdMute: {
        action->setIcon(HbIcon("qtg_mono_mic_mute"));
        action->setProperty("command", EPhoneInCallCmdMute); 
    }
    break;
    case PhoneInCallCmdUnmute: {
        action->setIcon(HbIcon("qtg_mono_mic_unmute"));
        action->setProperty("command", EPhoneInCallCmdUnmute); 
    }
    break;
    case PhoneInCallCmdActivateIhf: {
        action->setIcon(HbIcon("qtg_mono_speaker"));
        action->setProperty("command", EPhoneInCallCmdActivateIhf); 
    }
    break;
    case PhoneInCallCmdDeactivateIhf: {
        action->setIcon(HbIcon("qtg_mono_mobile"));
        action->setProperty("command", EPhoneInCallCmdDeactivateIhf); 
    }
    break;
    case PhoneInCallCmdOpenContacts: {
        action->setIcon(HbIcon("qtg_mono_contacts"));
        action->setProperty("command", EPhoneInCallCmdContacts); 
    }
    break;
    case PhoneInCallCmdBtHandsfree: {
        action->setIcon(HbIcon("qtg_mono_bluetooth_headset"));
        action->setProperty("command", EPhoneInCallCmdBtHandsfree); 
    }
    break;
    case PhoneInCallCmdHandset: {
        action->setIcon(HbIcon("qtg_mono_mobile"));
        action->setProperty("command", EPhoneInCallCmdHandset); 
    }
    break;
    default: {
        delete action.take();
    }
    break;
    }
            
    return action.take();
}

bool PhoneUiCommandController::emergencyCall(QMap<int,int> callStates) const
{   
    return callStates.keys().contains(KEmergencyCallId);
}

