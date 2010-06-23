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

#include "cptelephonypluginview.h"
#include "cppluginlogging.h"
#include <QPluginLoader>
#include <hbdataformmodel.h>
#include <cpplugininterface.h>
#include <cpsettingformitemdata.h>
#include <cpitemdatahelper.h>
#include <cppluginutility.h>
#include <cppluginloader.h>


/*!
    CpTelephonyPluginView::CpTelephonyPluginView()
*/
CpTelephonyPluginView::CpTelephonyPluginView() : 
    CpBaseSettingView(0,0),
    m_helper(NULL)
{
    DPRINT << ": IN";
    
    HbDataForm *form = qobject_cast<HbDataForm*>(widget());
    if (form) {
        HbDataFormModel *model = new HbDataFormModel;
        form->setHeading(hbTrId("txt_phone_subhead_telephone"));
        // Create and initialize plugin's item data helper
        m_helper = initializeItemDataHelper(); 
        QList<CpSettingFormItemData*> items;
        
        // Load calls plugin
        DPRINT << ": Loading cpcallsplugin";
        items.append(groupItemFromPlugin("cpcallsplugin"));
        
        // Load diverts plugin
        DPRINT << ": Loading cpdivertsplugin";
        items.append(groupItemFromPlugin("cpdivertplugin"));
        
        // Load call mailboxes plugin
        DPRINT << ": Loading vmbxcpplugin";
        items.append(groupItemFromPlugin("vmbxcpplugin"));

        // Load barring plugin
        DPRINT << ": Loading cpbarringplugin";
        items.append(groupItemFromPlugin("cpbarringplugin"));
        
        // Insert items to form model
        foreach (CpSettingFormItemData* i, items) {
            model->appendDataFormItem(i);
        }
        
        form->setModel(model);
    }
    
    DPRINT << ": OUT";
}

/*!
    CpTelephonyPluginView::~CpTelephonyPluginView()
*/
CpTelephonyPluginView::~CpTelephonyPluginView()
{
    DPRINT << ": IN";
    
    delete m_helper;
    
    DPRINT << ": OUT";
}

/*!
    CpTelephonyPluginView::groupItemFromPlugin()
*/
QList<CpSettingFormItemData*> CpTelephonyPluginView::groupItemFromPlugin( const QString& plugin )
{
    DPRINT << ": IN";
 
    QList<CpSettingFormItemData*> items;
    CpPluginInterface *p(NULL);
    try {
        p = CpPluginLoader::loadCpPluginInterface(plugin);
        if (p && m_helper){
            items = p->createSettingFormItemData(*m_helper);
        }
    } catch(...) {
        DPRINT << "CATCH ERROR";
        delete p;
    }
    
    DPRINT << ": OUT";
    return items;
}

/*!
    CpTelephonyPluginView::initializeItemDataHelper()
*/
CpItemDataHelper* CpTelephonyPluginView::initializeItemDataHelper()
{
    DPRINT;
    return new CpItemDataHelper(qobject_cast<HbDataForm*>(widget()));
}

// End of File. 
