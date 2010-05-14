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
#include <hbmarqueeitem>
#include <hbstyle>
#define private public
#include "infowidgetlayoutmanager.h"
#include "qtestmains60.h"



const int KNumOfSettingsDisplayRoles = 6;
const int KNumOfInfoDisplayRoles = 7;
const QString KInfoWidgetDocmlFile = ":/resource/infowidget.docml";
const QString KMargueeItemClassName = HbMarqueeItem::staticMetaObject.className();

//class HbStyle;
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
  UT_InfoWidgetLayoutManager::fillParams
 */
void UT_InfoWidgetLayoutManager::fillParams(const QString & fileName,
        bool * ok){
    Q_UNUSED(fileName)
    *ok = true;
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
            InfoWidgetLayoutManager::RoleSpnLabel));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleMcnMarqueeItem));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleSatMarqueeItem));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleSpnIcon));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleMcnIcon));
        QVERIFY(0 <= roles.indexOf(
            InfoWidgetLayoutManager::RoleSatTextIcon));
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
  UT_InfoWidgetLayoutManager::t_setLayoutRows
 */
void UT_InfoWidgetLayoutManager::t_rowHeight()
{
    bool b = false;
    EXPECT(HbStyle::parameter).returns(b);
    m_layoutManager->rowHeight();
    
    b = true;
    EXPECT(HbStyle::parameter).returns(b);
    m_layoutManager->rowHeight();
    
    QVERIFY(verify()); 
}

/*!
  UT_InfoWidgetLayoutManager::t_layoutInfoDisplay
 */
void UT_InfoWidgetLayoutManager::t_layoutInfoDisplay()
{
    QVERIFY(!m_layoutManager->layoutInfoDisplay());
}


/*!
  UT_InfoWidgetLayoutManager::t_layoutSettingsDisplay
 */
void UT_InfoWidgetLayoutManager::t_layoutSettingsDisplay()
{
    QVERIFY(!m_layoutManager->layoutSettingsDisplay());
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
            InfoWidgetLayoutManager::RoleSpnCheckBox));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleMcnCheckBox));
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
            InfoWidgetLayoutManager::RoleSpnLabel));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleMcnMarqueeItem));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSatMarqueeItem));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSpnIcon));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleMcnIcon));
        QVERIFY(0 <= infoDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSatTextIcon));
    }
}

/*!
  UT_InfoWidgetLayoutManager::t_loadWidgets
 */
void UT_InfoWidgetLayoutManager::t_loadWidgets()
{
    QObjectList list = QObjectList();
    EXPECT(HbDocumentLoader::load)
        .willOnce(invoke(this, &fillParams)).returns(list);
    
    const QList<InfoWidgetLayoutManager::LayoutItemRole> settingDisplayRoles =
            m_layoutManager->widgetRoles(InfoWidgetLayoutManager::SettingsDisplay);
    bool loadResult = m_layoutManager->loadWidgets(InfoWidgetLayoutManager::SettingsDisplay, 
            settingDisplayRoles,
            m_layoutManager->m_infoDisplayWidgets); 
    
    QVERIFY(!loadResult);
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
    
    infoDisplayWidgetRoles.append(InfoWidgetLayoutManager::RoleUndefined); 
    infoDisplayWidgetRoles.append(InfoWidgetLayoutManager::RoleContainer);
    

    foreach (currentRole, infoDisplayWidgetRoles) {
        currentWidget = m_layoutManager->loadWidget(*m_documentLoader,
                InfoWidgetLayoutManager::InfoDisplay,
                currentRole);
        QVERIFY(!currentWidget);
    }
    
    QList<InfoWidgetLayoutManager::LayoutItemRole> 
        settingsDisplayWidgetRoles = m_layoutManager->widgetRoles(
                InfoWidgetLayoutManager::SettingsDisplay); 
    
    foreach (currentRole, settingsDisplayWidgetRoles) {
        currentWidget = m_layoutManager->loadWidget(*m_documentLoader,
                InfoWidgetLayoutManager::SettingsDisplay,
                currentRole);
        QVERIFY(!currentWidget);
    }
    
    currentWidget = m_layoutManager->loadWidget(*m_documentLoader,
                (InfoWidgetLayoutManager::DisplayRole)2,
                        InfoWidgetLayoutManager::RoleLastEnum );
    
    QVERIFY(!currentWidget);
}

/*!
  UT_InfoWidgetLayoutManager::t_getWidget
 */
void UT_InfoWidgetLayoutManager::t_getWidget()
{
    fillCurrentWidgetsContainer();
    
    QVERIFY(!m_layoutManager->getWidget(InfoWidgetLayoutManager::RoleUndefined));
    QVERIFY(m_layoutManager->getWidget(InfoWidgetLayoutManager::RoleContent));
    QVERIFY(!m_layoutManager->getWidget(InfoWidgetLayoutManager::RoleLastEnum));
}

/*!
  UT_InfoWidgetLayoutManager::t_removeWidget
 */
void UT_InfoWidgetLayoutManager::t_removeWidget()
{
    fillCurrentWidgetsContainer();
        
    m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleUndefined);
    m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleContent);
    m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleLastEnum);
    
    QVERIFY(verify());
}


/*!
  UT_InfoWidgetLayoutManager::t_contentWidget
 */
void UT_InfoWidgetLayoutManager::t_contentWidget()
{
    fillCurrentWidgetsContainer();
    
    QVERIFY(m_layoutManager->contentWidget());
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
    
    QVERIFY(verify());
}


/*!
  UT_InfoWidgetLayoutManager::t_hideAll
 */
void UT_InfoWidgetLayoutManager::t_hideAll()
{
    m_layoutManager->hideAll();
    
    QVERIFY(verify());
}


/*!
  UT_InfoWidgetLayoutManager::t_showAll
 */
void UT_InfoWidgetLayoutManager::t_showAll()
{
    fillCurrentWidgetsContainer();//times 2 HbDocumentLoader::load
    m_layoutManager->showAll();
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidgetLayoutManager::t_reloadWidgets
 */
void UT_InfoWidgetLayoutManager::t_reloadWidgets()
{
    QVERIFY(!m_layoutManager->reloadWidgets(InfoWidgetLayoutManager::SettingsDisplay));
    QVERIFY(!m_layoutManager->reloadWidgets((InfoWidgetLayoutManager::DisplayRole)101));
}

/*!
  UT_InfoWidgetLayoutManager::t_destroyObjects
 */
void UT_InfoWidgetLayoutManager::t_destroyObjects()
{
    m_layoutManager->destroyObjects();
    
    QGraphicsWidget *widgetInfo = new QGraphicsWidget();
    QGraphicsWidget *widgetSettings = new QGraphicsWidget(); 
    m_layoutManager->m_infoDisplayWidgets.insert(InfoWidgetLayoutManager::RoleContent, widgetInfo);
    m_layoutManager->m_settingsDisplayWidgets.insert(InfoWidgetLayoutManager::RoleContent, widgetSettings);
    m_layoutManager->destroyObjects();
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidgetLayoutManager::t_createObject
 */
void UT_InfoWidgetLayoutManager::t_createObject()
{
    QObject *object = m_documentLoader->createObject(QString(), QString());
    QVERIFY(!object);
    object = m_documentLoader->createObject(KMargueeItemClassName, QString());
    QVERIFY(object);
    delete object;
    
    //For destructor coverage
    delete m_layoutManager->m_documentLoader;
    m_layoutManager->m_documentLoader = NULL;
}

QTEST_MAIN_S60(UT_InfoWidgetLayoutManager)
