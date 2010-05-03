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

#include "cpnetworkpluginview.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "cpnetworkpluginform.h"
#include "cppluginlogging.h"

/*!
  CpNetworkPluginView::CpNetworkPluginView
 */
CpNetworkPluginView::CpNetworkPluginView(QGraphicsItem *parent) :
    CpBaseSettingView(0, parent),
    m_networkSettingsForm(0)
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
    
    m_networkSettingsForm = new CpNetworkPluginForm();
    // base class takes ownership of the form
    this->setSettingForm(m_networkSettingsForm);
    
    DPRINT << ": OUT";
    }

/*!
  CpNetworkPluginView::~CpNetworkPluginView
 */
CpNetworkPluginView::~CpNetworkPluginView()
{
    DPRINT << ": IN";
    
    DPRINT << ": OUT";
}

/*!
  CpNetworkPluginView::SearchAvailableNetworks
 */
void CpNetworkPluginView::SearchAvailableNetworks()
{
    DPRINT << ": IN";
    
    m_networkSettingsForm->SearchAvailableNetworks();
    
    DPRINT << ": OUT";
}
    
// End of File. 
