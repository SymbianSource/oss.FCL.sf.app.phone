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

#include <hbdataformmodelitem.h>
#include <QTranslator>
#include <QLocale>
#include <QApplication>
#include <cpitemdatahelper.h>
#include "cpbarringplugingroup.h"
#include "cpplugincommon.h"
#include "cpphonenotes.h"
#include "cppluginlogging.h"

/*!
  CpBarringPluginGroup::CpBarringPluginGroup.
 */
CpBarringPluginGroup::CpBarringPluginGroup(CpItemDataHelper &helper)
     :CpSettingFormItemData(HbDataFormModelItem::GroupItem, hbTrId("Call barring"),0),
       m_helper(helper)
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
    
    DPRINT << ": OUT";
}

/*!
  CpBarringPluginGroup::~CpBarringPluginGroup.
 */
CpBarringPluginGroup::~CpBarringPluginGroup()
{
    DPRINT << ": IN";
    DPRINT << ": OUT";
}

// End of File. 
