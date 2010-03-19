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

#ifndef UT_INFOWIDGET_H
#define UT_INFOWIDGET_H

#include <QtTest/QtTest>
#include <mockservice.h>
#include "infowidgetengine.h"

#define private public
#include "infowidget.h"

class InfoWidget;

class UT_InfoWidget : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_InfoWidget();
    ~UT_InfoWidget();
    
private slots:

    void init();
    void cleanup();
    
    void t_setHomeZoneDisplay(); 
    void t_setSatDisplay(); 
    void t_setMcnDisplay();
    void t_setActiveLineDisplay(); 
    void t_readModel();
    void t_handleModelError();
    void t_onInitialize(); 
    void t_onUninitialize(); 
    void t_onShow(); 
    void t_onHide();
    void t_homeZoneDisplaySettingChanged();
    void t_mcnDisplaySettingChanged();
    void t_activeLineDisplaySettingChanged();
    void t_satDisplaySettingChanged();
    void t_settingsEditingFinished();

    
private:
    
    InfoWidget *m_infoWidget;
    InfoWidgetEngine::ModelData m_modelData;

};

#endif // UT_INFOWIDGET_H
