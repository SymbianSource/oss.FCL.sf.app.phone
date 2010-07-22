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
#include <QRectF>
#include <QPointF>
#include <QString>
#include <QFont>
#include <QFontMetricsF>
#include <hbmarqueeitem.h>
#include <hbstyle.h>
#define private public
#include "infowidgetlayoutmanager.h"
#include "qtestmains60.h"

const int KNumOfSettingsDisplayRoles = 5;
const int KNumOfInfoDisplayRoles = 7;
const QString KInfoWidgetDocmlFile = ":/resource/infowidget.docml";
const QString KSettingsDialogDocmlFile = ":/resource/settingsdialog.docml";
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
    m_graphicsWidgetPtr = new QGraphicsWidget;
}


/*!
  UT_InfoWidgetLayoutManager::~UT_InfoWidgetLayoutManager
 */
UT_InfoWidgetLayoutManager::~UT_InfoWidgetLayoutManager()
{
    delete m_layoutManager;
    delete m_graphicsWidgetPtr; 
}


/*!
  UT_InfoWidgetLayoutManager::init
 */
void UT_InfoWidgetLayoutManager::init()
{
    initialize();
    bool loadResultInfoWidgetDocml = true;  
    bool loadResultSettingsDialogDocml = true;
    
    QObjectList objectListInfoDisplayWidgets;
    QObjectList objectListSettingsDialogWidgets;
    
    EXPECT(HbDocumentLoader::load)
        .with(KInfoWidgetDocmlFile, loadResultInfoWidgetDocml)
        .returns(&objectListInfoDisplayWidgets);
    EXPECT(HbDocumentLoader::load)
        .with(KSettingsDialogDocmlFile, loadResultSettingsDialogDocml)
        .returns(&objectListSettingsDialogWidgets);
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
        m_layoutManager->m_infoDisplayWidgets.insert(currentRole, m_graphicsWidgetPtr.data());    
    }
     
    QList<InfoWidgetLayoutManager::LayoutItemRole> settingsDisplayWidgetRoles = 
            m_layoutManager->widgetRoles(InfoWidgetLayoutManager::SettingsDialog); 
    foreach (currentRole, settingsDisplayWidgetRoles) {
        m_layoutManager->m_settingsDialogWidgets.insert(currentRole, m_graphicsWidgetPtr.data());    
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
        m_layoutManager->m_widgets.insert(currentRole, m_graphicsWidgetPtr.data());    
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
            InfoWidgetLayoutManager::RoleSpnMarqueeItem));
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
    
    activeLayout= m_layoutManager->layoutSettingsDialog(); 
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
    m_layoutManager->layoutRowHeight();
    
    b = true;
    EXPECT(HbStyle::parameter).returns(b);
    m_layoutManager->layoutRowHeight();
    
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
  UT_InfoWidgetLayoutManager::t_layoutSettingsDialog
 */
void UT_InfoWidgetLayoutManager::t_layoutSettingsDialog()
{
    QVERIFY(!m_layoutManager->layoutSettingsDialog());
}

/*!
  UT_InfoWidgetLayoutManager::t_widgetRoles
 */
void UT_InfoWidgetLayoutManager::t_widgetRoles()
{
    const QList<InfoWidgetLayoutManager::LayoutItemRole> settingDisplayRoles =
        m_layoutManager->widgetRoles(InfoWidgetLayoutManager::SettingsDialog);
    
    QVERIFY(settingDisplayRoles.count() == KNumOfSettingsDisplayRoles);
    if (settingDisplayRoles.count() == KNumOfSettingsDisplayRoles) {
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSettingsDialog));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSpnCheckBox));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleMcnCheckBox));
        QVERIFY(0 <= settingDisplayRoles.indexOf(
            InfoWidgetLayoutManager::RoleSatTextCheckBox));
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
            InfoWidgetLayoutManager::RoleSpnMarqueeItem));
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
            m_layoutManager->widgetRoles(InfoWidgetLayoutManager::SettingsDialog);
    bool loadResult = m_layoutManager->loadWidgets(InfoWidgetLayoutManager::SettingsDialog, 
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
                InfoWidgetLayoutManager::SettingsDialog); 
    
    foreach (currentRole, settingsDisplayWidgetRoles) {
        currentWidget = m_layoutManager->loadWidget(*m_documentLoader,
                InfoWidgetLayoutManager::SettingsDialog,
                currentRole);
        QVERIFY(!currentWidget);
    }
    
    currentWidget = m_layoutManager->loadWidget(*m_documentLoader,
                (InfoWidgetLayoutManager::DisplayRole)2,
                        InfoWidgetLayoutManager::RoleLastEnum );
    
    QVERIFY(!currentWidget);
}

/*!
  UT_InfoWidgetLayoutManager::t_loadObject
 */
void UT_InfoWidgetLayoutManager::t_loadObject()
{
    QObject *currentObject(NULL); 
    InfoWidgetLayoutManager::LayoutItemRole currentRole; 
    
    QList<InfoWidgetLayoutManager::LayoutItemRole> objectRoles; 
    
    objectRoles.append(InfoWidgetLayoutManager::RoleCancelAction);
    QObject *okAction = new QObject; 
    
    objectRoles.append(InfoWidgetLayoutManager::RoleOkAction); 
    QObject *cancelAction = new QObject; 

    // Test: 
    EXPECT(HbDocumentLoader::findObject).with(QString("sd:cancelAction")).returns(cancelAction);
    EXPECT(HbDocumentLoader::findObject).with(QString("sd:okAction")).returns(okAction);
    
    foreach(currentRole, objectRoles) {
        currentObject = m_layoutManager->loadObject(*m_documentLoader,
                InfoWidgetLayoutManager::SettingsDialog, 
                currentRole);
        QVERIFY(currentObject);
    }    
    
   QVERIFY(verify());  
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
  UT_InfoWidgetLayoutManager::t_reloadWidgets
 */
void UT_InfoWidgetLayoutManager::t_reloadWidgets()
{
    bool loadResultIw = true;  
    bool loadResultSd = true;
    
    QObjectList objectListId;
    QObjectList objectListSd;
    
    EXPECT(HbDocumentLoader::load)
        .with(KInfoWidgetDocmlFile, loadResultIw)
        .returns(&objectListId);
    QVERIFY(!m_layoutManager->reloadWidgets(InfoWidgetLayoutManager::InfoDisplay));

    EXPECT(HbDocumentLoader::load)
        .with(KSettingsDialogDocmlFile, loadResultSd)
        .returns(&objectListSd);
    QVERIFY(!m_layoutManager->reloadWidgets(InfoWidgetLayoutManager::SettingsDialog));
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidgetLayoutManager::t_destroyWidgets
 */
void UT_InfoWidgetLayoutManager::t_destroyWidgets()
{
    m_layoutManager->destroyWidgets();
    
    QGraphicsWidget *widgetInfo = new QGraphicsWidget();
    QGraphicsWidget *widgetSettings = new QGraphicsWidget(); 
    m_layoutManager->m_infoDisplayWidgets.insert(InfoWidgetLayoutManager::RoleContent, widgetInfo);
    m_layoutManager->m_settingsDialogWidgets.insert(InfoWidgetLayoutManager::RoleContent, widgetSettings);
    m_layoutManager->destroyWidgets();
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidgetLayoutManager::t_textFitsToRect
 */
void UT_InfoWidgetLayoutManager::t_textFitsToRect()
{
    QString text;
    QFont font; 
    bool fits(false); 
    
    QRectF testRect; 
    
    // Currently not possible to use with .pro definition 
    // QFontMetrics to be properly mocked
    // QT -= gui
    // Test 1: use valid text and rect to which text fits 
    //text = "testtesttesttest";
    //testRect = QRectF(QPointF(0,0), QSizeF(200,100)); 
    //fits = m_layoutManager->textFitsToRect(text, font, testRect);
    //QVERIFY(fits); 
    
    QVERIFY(verify());
}



/*!
  UT_InfoWidgetLayoutManager::t_marqueeItems
 */
void UT_InfoWidgetLayoutManager::t_marqueeItems()
{
    QList<HbMarqueeItem *> items; 
    
    // Test 1: uninitialized
    items =  m_layoutManager->marqueeItems(); 
    QVERIFY(items.count() == 0); 
    
    // Test: 
    HbMarqueeItem item1; 
    HbMarqueeItem item2;
    HbMarqueeItem item3;
    
    m_layoutManager->m_widgets.insert(InfoWidgetLayoutManager::RoleSpnMarqueeItem, &item1); 
    m_layoutManager->m_widgets.insert(InfoWidgetLayoutManager::RoleMcnMarqueeItem, &item2);
    m_layoutManager->m_widgets.insert(InfoWidgetLayoutManager::RoleSatMarqueeItem, &item3);

    items =  m_layoutManager->marqueeItems(); 
    QVERIFY(items.count() == 3); 
    
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
