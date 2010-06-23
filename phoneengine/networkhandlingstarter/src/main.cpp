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
#include <hbview.h>
#include <hbmainwindow.h>
#include <networkhandlingstarter.h>
#include <xqserviceutil.h>

/*!
    main.
 */
int main(int argc, char **argv)
{
    HbApplication app(argc, argv, Hb::NoSplash);
    
    // Load translators
    HbTranslator commonTranslator("common");
    HbTranslator control_panelTranslator("control_panel");
    HbTranslator telephone_cpTranslator("telephone_cp"); 
    
    HbView view;
    HbMainWindow mainWindow;
    mainWindow.addView(&view);
    NetworkHandlingStarter service;
    XQServiceUtil::toBackground( true );
    int err = app.exec();
    
    return err;
}

