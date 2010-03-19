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
#include <QDebug>
#include <smcmockclassincludes.h>
#include "cpnetworkplugingroup.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CpNetworkPluginGroup::CpNetworkPluginGroup
// -----------------------------------------------------------------------------
//
CpNetworkPluginGroup::CpNetworkPluginGroup( 
        CpItemDataHelper & helper )
    :
    m_helper(helper)
    {
    
    }


// -----------------------------------------------------------------------------
// CpNetworkPluginGroup::~CpNetworkPluginGroup
// -----------------------------------------------------------------------------
//
CpNetworkPluginGroup::~CpNetworkPluginGroup(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CpNetworkPluginGroup::networkModeStateChanged
// -----------------------------------------------------------------------------
//
void CpNetworkPluginGroup::networkModeStateChanged( 
        int index )
    {
    SMC_MOCK_METHOD1( void, int, index )
    }


// -----------------------------------------------------------------------------
// CpNetworkPluginGroup::operatorSelectionStateChanged
// -----------------------------------------------------------------------------
//
void CpNetworkPluginGroup::operatorSelectionStateChanged( 
        int index )
    {
    SMC_MOCK_METHOD1( void, int, index )
    }


// -----------------------------------------------------------------------------
// CpNetworkPluginGroup::networkAccessModeGot
// -----------------------------------------------------------------------------
//
void CpNetworkPluginGroup::networkAccessModeGot( 
        int mode )
    {
    SMC_MOCK_METHOD1( void, int, mode )
    }


// -----------------------------------------------------------------------------
// CpNetworkPluginGroup::availableNetworksGot
// -----------------------------------------------------------------------------
//
void CpNetworkPluginGroup::availableNetworksGot( 
        QList<PSetNetworkWrapper::NetworkInfo *> & m_networkInfoList )
    {
    SMC_MOCK_METHOD1( void, QList<PSetNetworkWrapper::NetworkInfo *> &, m_networkInfoList )
    }


// -----------------------------------------------------------------------------
// CpNetworkPluginGroup::networkReqestFailed
// -----------------------------------------------------------------------------
//
void CpNetworkPluginGroup::networkReqestFailed( 
        PSetNetworkWrapper::ErrorCode error,
        PSetNetworkWrapper::RequestType type )
    {
    SMC_MOCK_METHOD2( void, PSetNetworkWrapper::ErrorCode, error, 
        PSetNetworkWrapper::RequestType, type )
    }


// -----------------------------------------------------------------------------
// CpNetworkPluginGroup::userCancel
// -----------------------------------------------------------------------------
//
void CpNetworkPluginGroup::userCancel(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CpNetworkPluginGroup::handleSearchingNetworks
// -----------------------------------------------------------------------------
//
void CpNetworkPluginGroup::handleSearchingNetworks( 
        PSetNetworkWrapper::RequestType & type )
    {
    SMC_MOCK_METHOD1( void, PSetNetworkWrapper::RequestType &, type )
    }


// -----------------------------------------------------------------------------
// CpNetworkPluginGroup::handleRequestingSelectedNetwork
// -----------------------------------------------------------------------------
//
void CpNetworkPluginGroup::handleRequestingSelectedNetwork( 
        bool ongoing )
    {
    SMC_MOCK_METHOD1( void, bool, ongoing )
    }


// -----------------------------------------------------------------------------
// CpNetworkPluginGroup::handleNetworkChanged
// -----------------------------------------------------------------------------
//
void CpNetworkPluginGroup::handleNetworkChanged( 
        PSetNetworkWrapper::NetworkInfo & currentInfo,
        PSetNetworkWrapper::RegistrationStatus & status )
    {
    SMC_MOCK_METHOD2( void, PSetNetworkWrapper::NetworkInfo &, currentInfo, 
        PSetNetworkWrapper::RegistrationStatus &, status )
    }


