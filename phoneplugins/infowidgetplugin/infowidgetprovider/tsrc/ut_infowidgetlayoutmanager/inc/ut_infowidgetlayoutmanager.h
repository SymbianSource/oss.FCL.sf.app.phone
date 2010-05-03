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

#ifndef UT_INFOWIDGETLAYOUTMANAGER_H
#define UT_INFOWIDGETLAYOUTMANAGER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class InfoWidgetLayoutManager;
class InfoWidgetDocumentLoader;

class UT_InfoWidgetLayoutManager : public QObject, public MockService
{
    Q_OBJECT

public:  
    UT_InfoWidgetLayoutManager();
    ~UT_InfoWidgetLayoutManager();

private:     
    void fillDisplayContainers();
    void fillCurrentWidgetsContainer();
    void fillParams(const QString & fileName, bool * ok);
    
private slots:
    void init();
    void cleanup();
    
    void t_currentDisplayRole();
    void t_currentWidgetRoles();
    
    void t_layoutRows(); 
    void t_setLayoutRows();
    //void t_rowHeight();
    
    void t_layoutInfoDisplay();
    void t_layoutSettingsDisplay();
    
    void t_widgetRoles();
    void t_loadWidgets();
    void t_loadWidget();
    void t_getWidget();
    void t_hideWidget();

    void t_contentWidget();
    
    void t_showAll();
    void t_hideAll();
        
    void t_reloadWidgets();
    
    void t_removeWidget();
    
    // InfoWidgetDocumentLoader tests 
    void t_createObject();
    
private:    
    InfoWidgetLayoutManager *m_layoutManager;
    InfoWidgetDocumentLoader *m_documentLoader;
    
    QGraphicsWidget *m_checkBoxPtr; 
    QGraphicsWidget *m_iconPtr;
    QGraphicsWidget *m_marqueeItemPtr;
    QGraphicsWidget *m_labelPtr;
    QGraphicsWidget *m_graphicsWidgetPtr;
    
};

#endif // UT_INFOWIDGETLAYOUTMANAGER_H
