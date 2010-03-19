/*
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
* Description:
*
*/

#ifndef UT_NETWORKPLUGINFORM_H
#define UT_NETWORKPLUGINFORM_H

#include <QtTest/QtTest>
#include <mockservice.h>
#include <psetnetworkwrapper.h>

class CpNetworkPluginForm;
//class PSetNetworkWrapper;

class UT_CpNetworkPluginForm : public QObject, MockService
{
    Q_OBJECT

public:    
    UT_CpNetworkPluginForm();
    ~UT_CpNetworkPluginForm();
    
    void updateNetworkSelectionMode(
        PSetNetworkWrapper::NetworkSelectionMode& mode);
    
private slots:

    void init();

    void t_networkModeStateChanged();
    void t_operatorSelectionStateChanged();
    void t_networkAccessModeGot();
    
    void t_availableNetworksGot();
    void t_networkReqestFailed();
    void t_userCancel();
    void t_handleSearchingNetworks();
    void t_handleRequestingSelectedNetwork();
    void t_handleNetworkChanged();

    void cleanup();
    
    void t_memleak();
private:
    CpNetworkPluginForm *m_networkPluginForm;
    PSetNetworkWrapper *m_networkWrapper;
    PSetNetworkWrapper::NetworkSelectionMode m_NetworkSelectionMode;
    int i;

};

#endif // UT_NETWORKPLUGINFORM_H
