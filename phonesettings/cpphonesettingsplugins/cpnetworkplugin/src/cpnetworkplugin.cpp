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
#include <cpsettingformentryitemdataimpl.h>
#include "cpnetworkplugin.h"
#include "cpnetworkpluginview.h"
#include "cppluginlogging.h"

/*!
  CpNetworkPlugin::CpNetworkPlugin
 */
CpNetworkPlugin::CpNetworkPlugin() : QObject(0)
{
    INSTALL_TRACE_MSG_HANDLER;
    
    DPRINT;
}

/*!
  CpNetworkPlugin::~CpNetworkPlugin
 */
CpNetworkPlugin::~CpNetworkPlugin()
{
    UNINSTALL_TRACE_MSG_HANDLER;
}

/*!
  CpNetworkPlugin::createSettingFormItemData
 */
CpSettingFormItemData *CpNetworkPlugin::createSettingFormItemData(
    CpItemDataHelper &itemDataHelper) const
{
    DPRINT;
    
    return new CpSettingFormEntryItemDataImpl<CpNetworkPluginView>(
        itemDataHelper,
        hbTrId("Mobile network"), 
        hbTrId("Mobile network settings"));
}

Q_EXPORT_PLUGIN2(CpNetworkPlugin, CpNetworkPlugin);
