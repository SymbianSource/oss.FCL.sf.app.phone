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
    QObject(NULL),
    m_localisation(NULL),
    m_networkStatus(NULL)
{
    INSTALL_TRACE_MSG_HANDLER;
    DPRINT << ": IN";
    
    // Create guarded objects
    m_localisation.reset(new CpPhoneLocalisation); 
    m_networkStatus.reset(new CpNetworkStatus); 
    
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
    DPRINT << ": IN";
    QList<CpSettingFormItemData*> ret;
    ret.append(new CpSettingFormEntryItemDataImpl<CpNetworkPluginView>(
            itemDataHelper,
            hbTrId("txt_cp_dblist_mobile_network"),
            m_networkStatus->statusText(),
            m_networkStatus->statusIcon()));
    
    m_networkStatus->setSettingFormItemData(ret.first());
        
    DPRINT << ": OUT";
    return ret;
}

/*!
  CpNetworkPlugin::createSettingView
 */
CpBaseSettingView *CpNetworkPlugin::createSettingView(const QVariant &hint) const 
    {
    Q_UNUSED(hint)
    DPRINT << ": IN";
    
    CpItemDataHelper *itemDataHelper(NULL);
    QScopedPointer<CpSettingFormEntryItemDataImpl<CpNetworkPluginView> > 
        data( new CpSettingFormEntryItemDataImpl<CpNetworkPluginView>(
                    *itemDataHelper,
                    hbTrId("txt_cp_dblist_mobile_network"),
                    m_networkStatus->statusText()));
    CpBaseSettingView *view = data->createSettingView();
    
    DPRINT << ": OUT";
    return view;
    }
    
Q_EXPORT_PLUGIN2(CpNetworkPlugin, CpNetworkPlugin);

