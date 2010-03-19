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
#include "cpplugincommon.h"
#include "cppluginlogging.h"
#include <QPluginLoader>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <hbdataformmodel.h>
#include <cppluginplatinterface.h>
#include <cpsettingformitemdata.h>
#include <cpitemdatahelper.h>

CpTelephonyPluginView::CpTelephonyPluginView() : 
    CpBaseSettingView(),
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
    
    HbDataForm *form = settingForm();
    if (form) {
        HbDataFormModel *model = new HbDataFormModel;
        form->setHeading(hbTrId("Telephone settings"));
        // Create and initialize plugin's item data helper
        m_helper = initializeItemDataHelper(); 
        HbDataFormModelItem *item(NULL);
        
        DPRINT << ": Loading cpcallsplugin";
        // Load calls plugin
        item = groupItemFromPlugin("cpcallsplugin");
        if (item) {
            model->appendDataFormItem(item);
        }
        
        // Load diverts plugin
        DPRINT << ": Loading cpdivertsplugin";
        item = groupItemFromPlugin("cpdivertplugin");
        if (item) {
            model->appendDataFormItem(item);
        }

        // Load call mailboxes plugin
        DPRINT << ": Loading vmbxcpplugin";
        item = groupItemFromPlugin("vmbxcpplugin");
        if (item) {
            model->appendDataFormItem(item);
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

HbDataFormModelItem* CpTelephonyPluginView::groupItemFromPlugin( const QString& plugin )
{
    DPRINT << ": IN";
 
    CpSettingFormItemData *item(NULL);
    CpPluginPlatInterface *p(NULL);
    try {
        p = Tools::loadCpPlugin(plugin);
        if (p && m_helper){
            item = p->createSettingFormItemData(*m_helper);
            if (item){
                item->setType(HbDataFormModelItem::GroupItem);
                QObject::connect(
                        settingForm(), SIGNAL(itemShown(QModelIndex)),
                        item, SLOT(itemShown(QModelIndex)));
                DPRINT << "plugin:" << plugin;
            }
        }
    } catch(...) {
        DPRINT << "CATCH ERROR";
        delete p;
    }
    
    DPRINT << ": OUT";
    return item;
}

void CpTelephonyPluginView::onConnectionAdded(HbDataFormModelItem *item,
                                   const QString &signal,
                                   QObject *receiver,
                                   const QString &method)
{
    DPRINT << ": IN";
    
    if (HbDataForm *form = settingForm()) {
        form->addConnection(item, signal.toAscii(), receiver, method.toAscii());
    }
    
    DPRINT << ": OUT";
}

void CpTelephonyPluginView::onConnectionRemoved(HbDataFormModelItem *item,
                                     const QString &signal,
                                     QObject *receiver,
                                     const QString &method)
{
    DPRINT << ": IN";
    
    if (HbDataForm *form = settingForm()) {
        form->removeConnection(item, signal.toAscii(), receiver, method.toAscii());
    }
    
    DPRINT << ": OUT";
}

void CpTelephonyPluginView::onPrototypeAdded(HbAbstractViewItem *prototype)
{
    DPRINT << ": IN";
    
    if (HbDataForm *form = settingForm()) {
        QList<HbAbstractViewItem *> prototypes = form->itemPrototypes();
        prototypes.append(prototype);
        form->setItemPrototypes(prototypes);
    }
    
    DPRINT << ": OUT";
}

CpItemDataHelper* CpTelephonyPluginView::initializeItemDataHelper()
{
    DPRINT << ": IN";
    
    CpItemDataHelper *itemDataHelper = new CpItemDataHelper;
    DPRINT << "itemDataHelper:" << reinterpret_cast<int>(itemDataHelper);
    itemDataHelper->bindToForm(settingForm());
    connect(itemDataHelper, 
        SIGNAL(connectionAdded(HbDataFormModelItem*, QString, QObject *, QString)),
        this,
        SLOT(onConnectionAdded(HbDataFormModelItem*, QString, QObject *, QString)));
    connect(itemDataHelper,
        SIGNAL(connectionRemoved(HbDataFormModelItem*, QString, QObject *, QString)),
        this,
        SLOT(onConnectionRemoved(HbDataFormModelItem*, QString, QObject *, QString)));
    connect(itemDataHelper,
        SIGNAL(prototypeAdded(HbAbstractViewItem *)),
        this,
        SLOT(onPrototypeAdded(HbAbstractViewItem *)));
    
    DPRINT << ": OUT : helper signals connected";
    return itemDataHelper;
}

// End of File. 
