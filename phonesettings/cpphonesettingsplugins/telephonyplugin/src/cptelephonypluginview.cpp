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
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <hbdataformmodel.h>
#include <cpplugininterface.h>
#include <cpsettingformitemdata.h>
#include <cpitemdatahelper.h>
#include <cppluginutility.h>
#include <cppluginloader.h>

CpTelephonyPluginView::CpTelephonyPluginView() : 
    CpBaseSettingView(0,0),
    m_helper(NULL)
{
    DPRINT << ": IN";
    
    // Localization file loading
    QTranslator translator; 
    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";
    DPRINT << ": loading translation: " << QString(path + "telephone_cp_" + lang);
    bool translatorLoaded = translator.load(path + "telephone_cp_" + lang);
    DPRINT << ": translator loaded: " << translatorLoaded; 
    if (translatorLoaded) {
        qApp->installTranslator(&translator);
        DPRINT << ": translator installed"; 
    }
    
    HbDataForm *form = qobject_cast<HbDataForm*>(widget());
    if (form) {
        HbDataFormModel *model = new HbDataFormModel;
        form->setHeading(hbTrId("Telephone settings"));
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

        // Insert items to form model
        foreach (CpSettingFormItemData* i, items) {
            model->appendDataFormItem(i);
        }
        
        form->setModel(model);
    }
    
    DPRINT << ": OUT";
}

CpTelephonyPluginView::~CpTelephonyPluginView()
{
    DPRINT << ": IN";
    
    delete m_helper;
    
    DPRINT << ": OUT";
}

QList<CpSettingFormItemData*> CpTelephonyPluginView::groupItemFromPlugin( const QString& plugin )
{
    DPRINT << ": IN";
 
    QList<CpSettingFormItemData*> items;
    CpPluginInterface *p(NULL);
    try {
        p = CpPluginLoader::loadCpPlugin(plugin);
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

CpItemDataHelper* CpTelephonyPluginView::initializeItemDataHelper()
{
    DPRINT;
    return new CpItemDataHelper(qobject_cast<HbDataForm*>(widget()));
}

// End of File. 
