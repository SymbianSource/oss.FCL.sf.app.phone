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
CpNetworkPlugin::CpNetworkPlugin() : 
    QObject(0),
    m_localisation(0)
{
    INSTALL_TRACE_MSG_HANDLER;
    DPRINT << ": IN";
    
    // Set scoped pointer 
    m_localisation.reset(new CpPhoneLocalisation); 
    
    // Install required translations
    m_localisation->installTranslator(
            CpPhoneLocalisation::
            TranslationFileCommon);
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPlugin::~CpNetworkPlugin
 */
CpNetworkPlugin::~CpNetworkPlugin()
{
    DPRINT;
    UNINSTALL_TRACE_MSG_HANDLER;
}


/*!
  CpNetworkPlugin::createSettingFormItemData
 */
QList<CpSettingFormItemData*> CpNetworkPlugin::createSettingFormItemData(
        CpItemDataHelper &itemDataHelper) const
{
    DPRINT;

    QList<CpSettingFormItemData*> ret;
    ret.append(new CpSettingFormEntryItemDataImpl<CpNetworkPluginView>(
            itemDataHelper,
            hbTrId("txt_cp_dblist_mobile_network"),
            hbTrId("")));
    return ret;
}


Q_EXPORT_PLUGIN2(CpNetworkPlugin, CpNetworkPlugin);
