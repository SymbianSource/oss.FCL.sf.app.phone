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

#ifndef UT_INFOWIDGETPROVIDER_H
#define UT_INFOWIDGETPROVIDER_H

#include <QtTest/QtTest>
#include <mockservice.h>
#include <qserviceplugininterface.h>

QTM_USE_NAMESPACE

class InfoWidgetProvider;

class UT_InfoWidgetProvider : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_InfoWidgetProvider();
    ~UT_InfoWidgetProvider();
    
private slots:

    void init();
    void cleanup();
    void t_createInstance(); 
    
private:
    
    InfoWidgetProvider *m_infoWidgetProvider;

};

#endif // UT_INFOWIDGET_H
