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
#include "ut_infowidgetlayoutmanager.h"

#include <QGraphicsWidget>

#define private public
#include "infowidgetlayoutmanager.h"
#include "qtestmains60.h"


const int KNumOfSettingsDisplayRoles = 15;
const int KNumOfInfoDisplayRoles = 10;
const QString KInfoWidgetDocmlFile = ":/resource/infowidget.docml";

/*!
  UT_InfoWidgetLayoutManager::UT_InfoWidgetLayoutManager
 */
UT_InfoWidgetLayoutManager::UT_InfoWidgetLayoutManager() 
    : 
    m_layoutManager(NULL),
    m_documentLoader(NULL)
{
    m_checkBoxPtr = new QGraphicsWidget;  
    m_iconPtr = new QGraphicsWidget; 
    m_marqueeItemPtr = new QGraphicsWidget; 
    m_labelPtr = new QGraphicsWidget; 
    m_graphicsWidgetPtr = new QGraphicsWidget;
}


/*!
  UT_InfoWidgetLayoutManager::~UT_InfoWidgetLayoutManager
 */
UT_InfoWidgetLayoutManager::~UT_InfoWidgetLayoutManager()
{
    delete m_layoutManager;
    delete m_checkBoxPtr;  
    delete m_iconPtr; 
    delete m_marqueeItemPtr; 
    delete m_labelPtr; 
    delete m_graphicsWidgetPtr; 
}


/*!
  UT_InfoWidgetLayoutManager::init
 */
void UT_InfoWidgetLayoutManager::init()
{
    initialize();
    
    QObjectList objectList;
    EXPECT(HbDocumentLoader::load)
        .with(KInfoWidgetDocmlFile, false)
        .returns(&objectList);
    EXPECT(HbDocumentLoader::load)
        .with(KInfoWidgetDocmlFile, false)
        .returns(&objectList);
    m_layoutManager = new InfoWidgetLayoutManager();
    
    m_documentLoader = new InfoWidgetDocumentLoader; 
    
    QVERIFY(verify());
}


/*!
  UT_InfoWidgetLayoutManager::cleanup
 */
void UT_InfoWidgetLayoutManager::cleanup()
{
    reset();
    
    delete m_layoutManager;
    m_layoutManager = NULL; 
    
    delete m_documentLoader; 
    m_documentLoader = NULL; 
}

/*!
  UT_InfoWidgetLayoutManager::fillDisplayContainers
 */
void UT_InfoWidgetLayoutManager::fillDisplayContainers()
{
    InfoWidgetLayoutManager::LayoutItemRole currentRole; 
    
    QList<InfoWidgetLayoutManager::LayoutItemRole> infoDisplayWidgetRoles = 
            m_layoutManager->widgetRoles(InfoWidgetLayoutManager::InfoDisplay); 
    foreach (currentRole, infoDisplayWidgetRoles) {
        m_layoutManager->m_infoDisplayWidgets.insert(currentRole, m_graphicsWidgetPtr);    
    }
     
    QList<InfoWidgetLayoutManager::LayoutItemRole> settingsDisplayWidgetRoles = 
            m_layoutManager->widgetRoles(InfoWidgetLayoutManager::SettingsDisplay); 
    foreach (currentRole, settingsDisplayWidgetRoles) {
        m_layoutManager->m_settingsDisplayWidgets.insert(currentRole, m_graphicsWidgetPtr);    
    }

}

/*!
  UT_InfoWidgetLayoutManager::fillCurrentWidgetsContainer
 */
void UT_InfoWidgetLayoutManager::fillCurrentWidgetsContainer()
{
    InfoWidgetLayoutManager::LayoutItemRole currentRole; 
    
    QList<InfoWidgetLayoutManager::LayoutItemRole> infoDisplayWidgetRoles = 
            m_layoutManager->widgetRoles(InfoWidgetLayoutManager::InfoDisplay); 
    foreach (currentRole, infoDisplayWidgetRoles) {
        m_layoutManager->m_widgets.insert(currentRole, m_graphicsWidgetPtr);    
    }
}

/*!
  UT_InfoWidgetLayoutManager::t_currentDisplayRole
 */
void UT_InfoWidgetLayoutManager::t_currentDisplayRole()
{
    InfoWidgetLayoutManager::DisplayRole role =
        m_layoutManager->currentDisplayRole();
    
    QVERIFY(InfoWidgetLayoutManager::InfoDisplay == role);
}


/*!
  UT_InfoWidgetLayoutManager::t_currentWidgetRoles
 */
void UT_InfoWidgetLayoutManager::t_currentWidgetRoles()
{
    // Initial state after layout manager construction 
    QList<InfoWidgetLayoutManager::LayoutItemRole> roles =
        m_layoutManager->currentWidgetRoles();
    QVERIFY(roles.count() == 0);
    
    // Fill display container data 
    fillDisplayContainers();
    
    QGraphicsLayout *activeLayout;    
    activeLayout= m_layoutManager->layoutInfoDisplay(); 
    QVERIFY(!activeLayout);
    
    roles = m_layoutManager->currentWidgetRoles();
            
    QVERIFY(roles.count() == KNumOfInfoDisplayRoles);
    if (roles.count() == KNumOfInfoDisplayRoles) {
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleContent));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleHomeZoneLabel));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleMcnMarqueeItem));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleActiveLineLabel));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleSatTextLabel));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleHomeZoneIcon));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleMcnIcon));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleActiveLineIcon));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleSatTextIcon));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleSettingsContainer));
    }
    
    activeLayout= m_layoutManager->layoutSettingsDisplay(); 
    QVERIFY(!activeLayout);

    roles = m_layoutManager->currentWidgetRoles();
    int dVar = roles.count();  
    QVERIFY(roles.count() == KNumOfSettingsDisplayRoles);

}


/*!
  UT_InfoWidgetLayoutManager::t_layoutRows
 */
void UT_InfoWidgetLayoutManager::t_layoutRows()
{
    int rows = m_layoutManager->layoutRows();
    QVERIFY(0 == rows);
}


/*!
  UT_InfoWidgetLayoutManager::t_setLayoutRows
 */
void UT_InfoWidgetLayoutManager::t_setLayoutRows()
{
    const int KLayoutRows = 1;
    m_layoutManager->setLayoutRows(KLayoutRows);
    QVERIFY(KLayoutRows == m_layoutManager->layoutRows());
}


/*!
  UT_InfoWidgetLayoutManager::t_layoutInfoDisplay
 */
void UT_InfoWidgetLayoutManager::t_layoutInfoDisplay()
{
    m_layoutManager->layoutInfoDisplay();
}


/*!
  UT_InfoWidgetLayoutManager::t_layoutSettingsDisplay
 */
void UT_InfoWidgetLayoutManager::t_layoutSettingsDisplay()
{
    m_layoutManager->layoutSettingsDisplay();
}

/*!
  UT_InfoWidgetLayoutManager::t_widgetRoles
 */
void UT_InfoWidgetLayoutManager::t_widgetRoles()
{
    const QList<InfoWidgetLayoutManager::LayoutItemRole> settingDisplayRoles =
        m_layoutManager->widgetRoles(InfoWidgetLayoutManager::SettingsDisplay);
    
    QVERIFY(settingDisplayRoles.count() == KNumOfSettingsDisplayRoles);
    if (settingDisplayRoles.count() == KNumOfSettingsDisplayRoles) {
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleContent));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleHomeZoneLabel));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleMcnLabel));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleActiveLineLabel));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSatTextLabel));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleHomeZoneIcon));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleMcnIcon));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleActiveLineIcon));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSatTextIcon));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleHomeZoneCheckBox));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleMcnCheckBox));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleActiveLineCheckBox));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSatTextCheckBox));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleOkButton));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSettingsContainer));
    }
    
    const QList<InfoWidgetLayoutManager::LayoutItemRole> infoDisplayRoles =
        m_layoutManager->widgetRoles(InfoWidgetLayoutManager::InfoDisplay);
    QVERIFY(infoDisplayRoles.count() == KNumOfInfoDisplayRoles);
    if (infoDisplayRoles.count() == KNumOfInfoDisplayRoles) {
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleContent));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleHomeZoneLabel));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleMcnMarqueeItem));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleActiveLineLabel));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSatTextLabel));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleHomeZoneIcon));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleMcnIcon));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleActiveLineIcon));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSatTextIcon));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSettingsContainer));
    }
}


/*!
  UT_InfoWidgetLayoutManager::t_loadWidgets
 */
void UT_InfoWidgetLayoutManager::t_loadWidgets()
{
    bool loadResult; 
    //m_layoutManager->loadWidgets();
}

/*!
  UT_InfoWidgetLayoutManager::t_loadWidget
 */
void UT_InfoWidgetLayoutManager::t_loadWidget()
{
    QGraphicsWidget *currentWidget; 
    InfoWidgetLayoutManager::LayoutItemRole currentRole; 
    
    QList<InfoWidgetLayoutManager::LayoutItemRole> 
        infoDisplayWidgetRoles = m_layoutManager->widgetRoles(
                InfoWidgetLayoutManager::InfoDisplay); 

    foreach (currentRole, infoDisplayWidgetRoles) {
        currentWidget = m_layoutManager->loadWidget(*m_documentLoader,
                InfoWidgetLayoutManager::InfoDisplay,
                currentRole);
    }
    
    QList<InfoWidgetLayoutManager::LayoutItemRole> 
        settingsDisplayWidgetRoles = m_layoutManager->widgetRoles(
                InfoWidgetLayoutManager::SettingsDisplay); 
    
    foreach (currentRole, settingsDisplayWidgetRoles) {
        currentWidget = m_layoutManager->loadWidget(*m_documentLoader,
                InfoWidgetLayoutManager::SettingsDisplay,
                currentRole);
    }

}

/*!
  UT_InfoWidgetLayoutManager::t_getWidget
 */
void UT_InfoWidgetLayoutManager::t_getWidget()
{
    fillCurrentWidgetsContainer();
    
    m_layoutManager->getWidget(InfoWidgetLayoutManager::RoleUndefined);
    m_layoutManager->getWidget(InfoWidgetLayoutManager::RoleContent);
    m_layoutManager->getWidget(InfoWidgetLayoutManager::RoleLastEnum);
}


/*!
  UT_InfoWidgetLayoutManager::t_contentWidget
 */
void UT_InfoWidgetLayoutManager::t_contentWidget()
{
    fillCurrentWidgetsContainer();
    
    m_layoutManager->contentWidget();
}


/*!
  UT_InfoWidgetLayoutManager::t_hideWidget
 */
void UT_InfoWidgetLayoutManager::t_hideWidget()
{
    fillCurrentWidgetsContainer();
    
    m_layoutManager->hideWidget(InfoWidgetLayoutManager::RoleUndefined);
    m_layoutManager->hideWidget(InfoWidgetLayoutManager::RoleContent);
    m_layoutManager->hideWidget(InfoWidgetLayoutManager::RoleLastEnum);
}


/*!
  UT_InfoWidgetLayoutManager::t_hideAll
 */
void UT_InfoWidgetLayoutManager::t_hideAll()
{
    fillCurrentWidgetsContainer();
    
    m_layoutManager->hideAll();
}


/*!
  UT_InfoWidgetLayoutManager::t_showAll
 */
void UT_InfoWidgetLayoutManager::t_showAll()
{
    fillCurrentWidgetsContainer();
    
    m_layoutManager->showAll();
}

void UT_InfoWidgetLayoutManager::t_createObject()
{
    m_documentLoader->createObject(QString(), QString());
}

QTEST_MAIN_S60(UT_InfoWidgetLayoutManager)
