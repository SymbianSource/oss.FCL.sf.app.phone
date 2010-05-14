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
#include <hbtranslator.h>
#include <hbapplication.h>
#include <hbmainwindow.h>
#include <networkhandlingstarter.h>

/*!
    main.
 */
int main(int argc, char **argv)
{
    HbApplication app(argc, argv);
    
    // Load translators
    HbTranslator *commonTranslator = new HbTranslator("common");
    HbTranslator *control_panelTranslator = new HbTranslator("control_panel");
    HbTranslator *telephone_cpTranslator = new HbTranslator("telephone_cp"); 
    
    HbMainWindow mainWindow;
    NetworkHandlingStarter service;
    int err = app.exec();
    
    delete commonTranslator;
    delete control_panelTranslator;
    delete telephone_cpTranslator;
    
    return err;
}

