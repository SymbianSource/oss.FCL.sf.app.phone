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
* Description:  Handles phone ui commands.
*
*/

#ifndef PHONEUICOMMANDCONTROLLER_H
#define PHONEUICOMMANDCONTROLLER_H

#include <QObject>
#include <QString>
#include <hbmenu.h>
#include <spdefinitions.h>
#include "phonecommandextensionwrapper.h"
#include "phoneuiqtviewif.h"
#include "phoneaction.h"

// FORWARD DECLARATION
class HbAction;

class PhoneUiCommandController : public QObject
{
    Q_OBJECT
    
public:

    /*!
        \fn PhoneUiCommandController(
            PhoneUIQtViewIF &m_view, QObject *parent)
    */
    PhoneUiCommandController(PhoneUIQtViewIF &view, QObject *parent = 0);
        
    /*!
        Destructor
     */
    virtual ~PhoneUiCommandController();
    
    /*!
        \fn void showGlobalNote()
        
        Sets call menu actions.

    */
    void setCallMenuActions(QMap<int,int> callStates,
                            QMap<int,int> serviceIds,
                            int serviceId,
                            int expandedCallId);

    /*!
        \fn void setDialpadMenuActions ()
        
        Sets dialpad menu actions.

    */
    void setDialpadMenuActions();
    
    /*!
        \fn void pushButtonActionsForCall ()
        
        Returns push button actions actions for call.

    */
    QMap<PhoneAction::ActionType, PhoneAction *> pushButtonActionsForCall( 
            int callState,
            bool emergencyCall,
            QMap<int,int> callStates,
            QMap<int,int> serviceIds,
            int serviceId,
            int callId);
    
private:

    /*!
        \fn QList<int> menuCommands()
        
        Returns list of current menu commands.

    */
    QList<int> menuCommands(
            QMap<int,int> callStates, QMap<int,int> serviceIds );
    
    /*!
        \fn void addMenuItems()
        
        Adds menu commands to menu.
    */
    void addMenuItems(QList<int> menuCmdList);
    
    /*!
        \fn int mapCommand()
        
        Maps ui command extension commands to
        phone app commands.
    */
    int mapCommand(int command);
    
    /*!
        \fn void commandExtension()
        
        Returns command extension by service id (null if
        extension not available).
    */
    PhoneCommandExtensionWrapper *commandExtension(int serviceId);

    /*!
        \fn void ResolveImplementationUidL()
        
        Returns service's plugin uid by service id.
    */
    TUid ResolveImplementationUidL( 
            TUint32 aServiceId, TServicePropertyName aPropertyName );
    
    /*!
        \fn void MenuExtensionL()
        
        Returns command extension by service id (null if
        extension not available).
    */
    PhoneCommandExtensionWrapper *CommandExtensionL( TUint aServiceId );
    
    /*!
        \fn void releaseMenu()
        
        Releases extensions menu.
    */
    void releaseMenu();
       
    /*!
        \fn void areServicesSame()
        
        Checks are all calls made by same service (id).
        Returns true when same service used.
    */
    bool areServicesSame(QMap<int,int> callStates, QMap<int,int> serviceIds);
    
    /*!
        \fn void buttonCommandList()
        
        Returns push button command list.
    */    
    QList<int> buttonCommandList(int callState,
            bool emergencyCall,
            bool sameServices,
            QList<int> callStates);
    
    /*!
        \fn void buttonCommandList()
        
        Maps push button command to phone action.
    */
    PhoneAction *mapCommandToAction(int callState);
    
    
    
private:

    PhoneUIQtViewIF &m_view;
    QList<HbAction *> m_customMenuActions;
    QList<PhoneCommandExtensionWrapper *> m_commandExtensions;
    PhoneCommandExtensionWrapper *m_lastCommandExtension;
    
};

#endif // PHONEUICOMMANDCONTROLLER_H
