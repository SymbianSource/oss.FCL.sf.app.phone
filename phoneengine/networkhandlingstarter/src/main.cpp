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
#include <QLocale>
#include <QTranslator>
#include <hbapplication.h>
#include <hbmainwindow.h>
#include <networkhandlingstarter.h>

/*!
    main.
 */
int main(int argc, char **argv)
{
    HbApplication app(argc, argv);
    
    // Load telephone_cp translator
    QTranslator translator; 
    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";
    bool translatorLoaded = translator.load(path + "telephone_cp_" + lang);
    if (translatorLoaded) {
        app.installTranslator(&translator);
    }
    // Load common translator
    QTranslator commontranslator;
    translatorLoaded = commontranslator.load( path + "common_" + lang);
    if (translatorLoaded) {
        app.installTranslator(&commontranslator);
    }
    
    HbMainWindow mainWindow;
    NetworkHandlingStarter service;
    return app.exec();
}

