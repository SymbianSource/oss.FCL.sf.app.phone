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
#include <QString>
#include <QPainter>
#include <QEvent>
#include <HbLabel>
#include <HbStyleOptionLabel>
#include <HbMarqueeItem>
#include <HbPushButton>
#include <HbCheckBox>
#include <HbEvent>
#include <HbColorScheme>
#include <hbframedrawer.h>
#include "ut_infowidget.h"
#include "qtestmains60.h"

#define private friend class UT_InfoWidget;private
#include "infowidget.h"

// mocked dependencies
#include "infowidgetpreferences.h"

const QString KPreferenceOn("On"); 
const QString KPreferenceOff("Off"); 
const QString KMcnName("tnw1: 3233255435"); 

/*!
  UT_InfoWidget::UT_InfoWidget
 */
UT_InfoWidget::UT_InfoWidget() 
    : m_infoWidget(0)
{
    m_modelData.setActiveLine(1);
    m_modelData.setHomeZoneIndicatorType(1);
    m_modelData.setMcnName(KMcnName);
        
    SmcDefaultValue<const InfoWidgetEngine::ModelData &>::SetL(m_modelData);
    SmcDefaultValue<QColor>::SetL(m_qcolor);
    SmcDefaultValue<QGraphicsWidget*>::SetL(&m_qgraphicswidget);
}

/*!
  UT_InfoWidget::~UT_InfoWidget
 */
UT_InfoWidget::~UT_InfoWidget()
{
    delete m_infoWidget;
}

/*!
  UT_InfoWidget::init
 */
void UT_InfoWidget::init()
{
    initialize();
    
    SmcDefaultValue<QString>::SetL("On");
    SmcDefaultValue<InfoWidgetLayoutManager::DisplayRole>::SetL(
        InfoWidgetLayoutManager::InfoDisplay);
    m_infoWidget = new InfoWidget();
    SmcDefaultValue<QString>::SetL("");
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::cleanup
 */
void UT_InfoWidget::cleanup()
{
    reset();
    
    delete m_infoWidget;
    m_infoWidget = 0;
}

/*!
  UT_InfoWidget::t_boundingRect
 */
void UT_InfoWidget::t_boundingRect()
{
    m_infoWidget->boundingRect();
}

/*!
  UT_InfoWidget::t_shape
 */
void UT_InfoWidget::t_shape()
{
    m_infoWidget->shape();
}


/*!
  UT_InfoWidget::t_sizePolicy
 */
void UT_InfoWidget::t_sizePolicy()
{
    QVERIFY(m_infoWidget->sizePolicy() == 
        QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}

/*!
  UT_InfoWidget::t_setHomeZoneDisplay
 */
void UT_InfoWidget::t_setHomeZoneDisplay()
{
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayHomeZone, KPreferenceOn);
    
    m_infoWidget->setHomeZoneDisplay(KPreferenceOn); 
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::t_setMcnDisplay
 */
void UT_InfoWidget::t_setMcnDisplay()
{
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayMcn, KPreferenceOn);
    
    m_infoWidget->setMcnDisplay(KPreferenceOn);
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::t_setActiveLineDisplay
 */
void UT_InfoWidget::t_setActiveLineDisplay()
{
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayActiveLine, KPreferenceOn);
    
    m_infoWidget->setActiveLineDisplay(KPreferenceOn);
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::t_setSatDisplay
 */
void UT_InfoWidget::t_setSatDisplay()
{
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplaySatText, KPreferenceOn);
    
    m_infoWidget->setSatDisplay(KPreferenceOn);
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::t_paint
 */
void UT_InfoWidget::t_paint()
{
    QScopedPointer<QPainter> painter(new QPainter());
    QStyleOptionGraphicsItem *option = NULL;
    QWidget *widget = NULL;
    
    m_infoWidget->paint(painter.data(), option, widget);
    
    // layout change ongoing while paint requested
    m_infoWidget->startChanges();
    m_infoWidget->paint(painter.data(), option, widget);
}

/*!
  UT_InfoWidget::t_mousePressEvent
 */
void UT_InfoWidget::t_mousePressEvent()
{
    QGraphicsSceneMouseEvent event;
    m_infoWidget->mousePressEvent(&event);
}

/*!
  UT_InfoWidget::t_mouseReleaseEvent
 */
void UT_InfoWidget::t_mouseReleaseEvent()
{
    QGraphicsSceneMouseEvent event;
    
    // widget clicked while showing info display => 
    // switch to settings display expected
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);
    EXPECT(InfoWidgetLayoutManager::layoutSettingsDisplay);
    m_infoWidget->mousePressEvent(&event);
    m_infoWidget->mouseReleaseEvent(&event);
    
    // widget clicked while showing settings display => 
    // no action expected
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::SettingsDisplay);
    EXPECT(InfoWidgetLayoutManager::layoutSettingsDisplay).times(0);
    m_infoWidget->mousePressEvent(&event);
    m_infoWidget->mouseReleaseEvent(&event);
    
    // release event received after dragging widget => 
    // no action expected
    EXPECT(InfoWidgetLayoutManager::layoutSettingsDisplay).times(0);
    m_infoWidget->mouseMoveEvent(&event);
    m_infoWidget->mouseReleaseEvent(&event);
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::t_mouseMoveEvent
 */
void UT_InfoWidget::t_mouseMoveEvent()
{
    
}

/*!
  UT_InfoWidget::t_updateInfoDisplay
 */
void UT_InfoWidget::t_updateInfoDisplay()
{
    HbLabel hbLabel;
    QGraphicsWidget* pointerHbLabel(&hbLabel);

    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);

    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleSpnLabel)
        .returns(pointerHbLabel);
    
    HbMarqueeItem mcnMarqueeItem; 
    QGraphicsWidget* pointerMcnMarqueeItem(&mcnMarqueeItem);
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleMcnMarqueeItem)
        .returns(pointerMcnMarqueeItem);
    EXPECT(HbMarqueeItem::isAnimating).returns(true);
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleSatMarqueeItem)
        .returns(pointerMcnMarqueeItem);
    
    m_infoWidget->updateInfoDisplay();
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::t_readPersistentPreferences
 */
void UT_InfoWidget::t_readPersistentPreferences()
{    
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayHomeZone, QString(""));

    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayMcn, QString(""));
    
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayActiveLine, QString(""));
    
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplaySatText, QString(""));

    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplaySpn, QString(""));
    
    EXPECT(InfoWidgetPreferences::validate);

    m_infoWidget->readPersistentPreferences();
    
    QVERIFY(verify());
    
    // one visible item initially
    EXPECT(InfoWidgetPreferences::validate).returns(true);
    EXPECT(InfoWidgetPreferences::storePreferences);
    
    m_infoWidget->readPersistentPreferences();
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::t_onInitialize
 */
void UT_InfoWidget::t_onInitialize()
{    
    // Test onInitialize sequence where 
    // meta-object properties have been 
    // initialized 
    EXPECT(InfoWidgetPreferences::validate)
        .returns(true);
    EXPECT(InfoWidgetPreferences::storePreferences)
        .returns(true);
    m_infoWidget->onInitialize();
    
    // Test onInitialize sequence where 
    // meta-object properties are 
    // uninitialized 
    EXPECT(InfoWidgetPreferences::validate)
        .returns(false);
    m_infoWidget->onInitialize();
}

/*!
  UT_InfoWidget::t_onUninitialize
 */
void UT_InfoWidget::t_onUninitialize()
{
    m_infoWidget->onUninitialize(); 
}

/*!
  UT_InfoWidget::t_onShow
 */
void UT_InfoWidget::t_onShow()
{
    m_infoWidget->onShow(); 
}

/*!
  UT_InfoWidget::t_onHide
 */
void UT_InfoWidget::t_onHide()
{
    m_infoWidget->onHide(); 
}

/*!
  UT_InfoWidget::t_readModel
 */
void UT_InfoWidget::t_readModel()
{
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);
    m_infoWidget->readModel();
    
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::SettingsDisplay);
    m_infoWidget->readModel();
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::t_handleModelError
 */
void UT_InfoWidget::t_handleModelError()
{
    int operation(0);
    int errorCode(0); 
    m_infoWidget->handleModelError(operation, errorCode); 
}

/*!
  UT_InfoWidget::t_mcnDisplaySettingChanged
 */
void UT_InfoWidget::t_mcnDisplaySettingChanged()
{
    // user enables MCN setting
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayMcn, KPreferenceOn);
    int setting(Qt::Checked);
    m_infoWidget->mcnDisplaySettingChanged(setting);
    
    // user disables MCN setting
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayMcn, KPreferenceOff);
    setting = Qt::Unchecked;
    m_infoWidget->mcnDisplaySettingChanged(setting);
    
    QVERIFY(verify());
}

void UT_InfoWidget::t_satDisplaySettingChanged()
{
    // user enables SAT setting
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplaySatText, KPreferenceOn);
    int setting(Qt::Checked);
    m_infoWidget->satDisplaySettingChanged(setting);
    
    // user disables SAT setting
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplaySatText, KPreferenceOff);
    setting = Qt::Unchecked;
    m_infoWidget->satDisplaySettingChanged(setting);
    
    QVERIFY(verify());
}

void UT_InfoWidget::t_settingsEditingFinished()
{
    m_infoWidget->settingsEditingFinished(); 
}

void UT_InfoWidget::t_updateItemsVisibility()
{
    // All items visible
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplaySpn)
        .returns(KPreferenceOn);
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplayMcn)
        .returns(KPreferenceOn);
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplaySatText)
        .returns(KPreferenceOn);
    EXPECT(InfoWidgetLayoutManager::setLayoutRows)
        .with(3);
    
    m_infoWidget->updateItemsVisibility();
    QVERIFY(verify());
    
    // none of items visible
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);
    
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplaySpn)
        .returns(KPreferenceOff);
    EXPECT(InfoWidgetLayoutManager::removeWidget)
        .with(InfoWidgetLayoutManager::RoleSpnLabel);
    EXPECT(InfoWidgetLayoutManager::removeWidget)
        .with(InfoWidgetLayoutManager::RoleSpnIcon);

    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplayMcn)
        .returns(KPreferenceOff);
    EXPECT(InfoWidgetLayoutManager::removeWidget)
        .with(InfoWidgetLayoutManager::RoleMcnMarqueeItem);
    EXPECT(InfoWidgetLayoutManager::removeWidget)
        .with(InfoWidgetLayoutManager::RoleMcnIcon);
    
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplaySatText)
        .returns(KPreferenceOff);
    EXPECT(InfoWidgetLayoutManager::removeWidget)
        .with(InfoWidgetLayoutManager::RoleSatMarqueeItem);
    EXPECT(InfoWidgetLayoutManager::removeWidget)
        .with(InfoWidgetLayoutManager::RoleSatTextIcon);
    
    EXPECT(InfoWidgetLayoutManager::setLayoutRows)
        .with(0);
    
    m_infoWidget->updateItemsVisibility();
    
    QVERIFY(verify());
}

void UT_InfoWidget::t_layoutInfoDisplay()
{
    EXPECT(InfoWidgetLayoutManager::showAll);
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::SettingsDisplay);
    
    m_infoWidget->layoutInfoDisplay();
    
    QVERIFY(verify());
}

void UT_InfoWidget::t_layoutSettingsDisplay()
{
    EXPECT(InfoWidgetLayoutManager::showAll); 
    
    m_infoWidget->layoutSettingsDisplay();
    
    QVERIFY(verify());
}

void UT_InfoWidget::t_initializeInfoDisplayItems()
{
    m_infoWidget->initializeInfoDisplayItems();
}

void UT_InfoWidget::t_initializeSettingsDisplayItems()
{
    QGraphicsWidget okButton;
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleOkButton)
        .returns(&okButton);
    
    QGraphicsWidget mcnCheckBox;
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleMcnCheckBox)
        .returns(&mcnCheckBox);
    
    QGraphicsWidget satTextCheckBox;
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleSatTextCheckBox)
        .returns(&satTextCheckBox);
    
    m_infoWidget->initializeSettingsDisplayItems();
    
    QVERIFY(verify());
}

void UT_InfoWidget::t_startChanges()
{
    m_infoWidget->startChanges();
}

void UT_InfoWidget::t_endChanges()
{
    m_infoWidget->endChanges();
}

/*!
  UT_InfoWidget::t_changeEvent
 */
void UT_InfoWidget::t_changeEvent()
{
    /*
     * Not work from commmand prompt.
     */
    HbEvent event(HbEvent::ThemeChanged);
    EXPECT(HbColorScheme::color);
    m_infoWidget->changeEvent(&event);
    QVERIFY(verify());
    
    HbEvent event2(HbEvent::ChildFocusIn);
    m_infoWidget->changeEvent(&event2);
    QVERIFY(verify());
}


/*!
  UT_InfoWidget::t_sizeHint
 */
void UT_InfoWidget::t_sizeHint()
{   
    const QSizeF KMinimumSize(33.5, 160);
    const QSizeF KPreferredSize(134, 160);
    const QSizeF KDefaultSizeInfoDisplay(100, 100);
    const QSizeF KDefaultSizeSettingsDisplay(230, 220);

    // Test: initial size after construction  
    Qt::SizeHint sizeHint = Qt::PreferredSize;
    QSizeF constraint = QSizeF();
    QVERIFY(KDefaultSizeInfoDisplay == m_infoWidget->sizeHint(
            sizeHint, constraint));
    
    // Test: size after onInitialize is called 
    // and current display is InfoDisplay 
    m_infoWidget->onInitialize();
    
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole).
            returns(InfoWidgetLayoutManager::InfoDisplay);
    QGraphicsWidget *contentWidget = new QGraphicsWidget;
    contentWidget->setMinimumSize(KMinimumSize); 
    EXPECT(InfoWidgetLayoutManager::contentWidget).
            returns(contentWidget);
    EXPECT(InfoWidgetPreferences::visibleItemCount).
            returns(1);
    
    // No strict verification, approximate values.
    // Data in docml is in "un" format and strict comparison to pixels won't work  
    //QVERIFY(abs(KMinimumSize.width()-m_infoWidget->sizeHint(sizeHint, constraint).width()) < 1);
    //QVERIFY(abs(KMinimumSize.height()-m_infoWidget->sizeHint(sizeHint, constraint).height()) < 1);
    QVERIFY(KDefaultSizeInfoDisplay != m_infoWidget->sizeHint(sizeHint, constraint));
    
    // Test: size after onInitialize is called 
    // and current display is SettingsDisplay 

    delete contentWidget;
    contentWidget = NULL;
    
    QVERIFY(verify());
}

QTEST_MAIN_S60(UT_InfoWidget)
