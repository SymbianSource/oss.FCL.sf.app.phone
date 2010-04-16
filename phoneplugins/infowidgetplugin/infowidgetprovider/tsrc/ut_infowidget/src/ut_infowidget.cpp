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
  UT_InfoWidget::t_sizeHint
 */
void UT_InfoWidget::t_sizeHint()
{   
    const QSizeF KMinimumSize(50, 50);
    const QSizeF KPreferredSize(70, 70);
    const QSizeF KDefaultSizeInfoDisplay(70, 160);
    const QSizeF KDefaultSizeSettingsDisplay(250, 250);
    
    // current display is info display
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);
    EXPECT(InfoWidgetLayoutManager::contentWidget);
    m_qgraphicswidget.setMinimumSize(KMinimumSize);
    m_qgraphicswidget.setPreferredSize(KPreferredSize);
    Qt::SizeHint sizeHint = Qt::MinimumSize;
    QSizeF constraint = QSizeF();
    QVERIFY(KMinimumSize == m_infoWidget->sizeHint(sizeHint, constraint));
    
    // current display is settings display
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::SettingsDisplay);
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::SettingsDisplay);
    EXPECT(InfoWidgetLayoutManager::contentWidget);
    QVERIFY(KPreferredSize == m_infoWidget->sizeHint(sizeHint, constraint));
    
    QGraphicsWidget* nullPoiter(NULL);
    // cannot fetch info display size from docml, default size to be used
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);
    EXPECT(InfoWidgetLayoutManager::contentWidget).returns(nullPoiter);
    QVERIFY(KDefaultSizeInfoDisplay == 
        m_infoWidget->sizeHint(sizeHint, constraint));
    
    // cannot fetch settings display size from docml, default size to be used
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::SettingsDisplay);
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::SettingsDisplay);
    EXPECT(InfoWidgetLayoutManager::contentWidget).returns(nullPoiter);
    QVERIFY(KDefaultSizeSettingsDisplay == 
        m_infoWidget->sizeHint(sizeHint, constraint)); 
    
    QVERIFY(verify());
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
    
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleHomeZoneLabel)
        .returns(pointerHbLabel);
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);
    
    HbMarqueeItem mcnMarqueeItem; 
    QGraphicsWidget* pointerMcnMarqueeItem(&mcnMarqueeItem);
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleMcnMarqueeItem)
        .returns(pointerMcnMarqueeItem);
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);
    EXPECT(HbMarqueeItem::isAnimating).returns(true);
    
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleActiveLineLabel)
        .returns(pointerHbLabel);
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);
    
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleSatTextLabel)
        .returns(pointerHbLabel);
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);
    
    m_infoWidget->updateInfoDisplay();
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::t_initializePreferences
 */
void UT_InfoWidget::t_initializePreferences()
{    
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplayHomeZone)
        .returns(KPreferenceOff);
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayHomeZone, KPreferenceOff);

    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplayMcn)
        .returns(KPreferenceOff);
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayMcn, KPreferenceOff);
    
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplayActiveLine)
        .returns(KPreferenceOff);
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayActiveLine, KPreferenceOff);
    
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplaySatText)
        .returns(KPreferenceOff);
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplaySatText, KPreferenceOff);
    
    EXPECT(InfoWidgetPreferences::visibleItemCount).returns(0);
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayMcn, KPreferenceOn);
    
    m_infoWidget->initializePreferences();
    
    QVERIFY(verify());
    
    // one visible item initially
    EXPECT(InfoWidgetPreferences::visibleItemCount).returns(1);
    
    m_infoWidget->initializePreferences();
    
    QVERIFY(verify());
}

/*!
  UT_InfoWidget::t_onInitialize
 */
void UT_InfoWidget::t_onInitialize()
{
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

void UT_InfoWidget::t_homeZoneDisplaySettingChanged()
{
    // user enables home zone setting
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayHomeZone, KPreferenceOn);
    int setting(Qt::Checked);
    m_infoWidget->homeZoneDisplaySettingChanged(setting);
    
    // user disables home zone setting
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayHomeZone, KPreferenceOff);
    setting = Qt::Unchecked;
    m_infoWidget->homeZoneDisplaySettingChanged(setting);
    
    QVERIFY(verify());
}

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

void UT_InfoWidget::t_activeLineDisplaySettingChanged()
{
    // user enables active line setting
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayActiveLine, KPreferenceOn);
    int setting(Qt::Checked);
    m_infoWidget->activeLineDisplaySettingChanged(setting);
    
    // user disables active line setting
    EXPECT(InfoWidgetPreferences::setPreference)
        .with(InfoWidgetPreferences::DisplayActiveLine, KPreferenceOff);
    setting = Qt::Unchecked;
    m_infoWidget->activeLineDisplaySettingChanged(setting);
    
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
    // all items visible
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplayHomeZone)
        .returns(KPreferenceOn);
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplayMcn)
        .returns(KPreferenceOn);
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplayActiveLine)
        .returns(KPreferenceOn);
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplaySatText)
        .returns(KPreferenceOn);
    EXPECT(InfoWidgetLayoutManager::setLayoutRows)
        .with(4);
    
    m_infoWidget->updateItemsVisibility();

    // none of items visible
    EXPECT(InfoWidgetLayoutManager::currentDisplayRole)
        .returns(InfoWidgetLayoutManager::InfoDisplay);
    
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplayHomeZone)
        .returns(KPreferenceOff);
    EXPECT(InfoWidgetLayoutManager::hideWidget)
        .with(InfoWidgetLayoutManager::RoleHomeZoneLabel);
    EXPECT(InfoWidgetLayoutManager::hideWidget)
        .with(InfoWidgetLayoutManager::RoleHomeZoneIcon);
    
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplayMcn)
        .returns(KPreferenceOff);
    EXPECT(InfoWidgetLayoutManager::hideWidget)
        .with(InfoWidgetLayoutManager::RoleMcnMarqueeItem);
    EXPECT(InfoWidgetLayoutManager::hideWidget)
        .with(InfoWidgetLayoutManager::RoleMcnIcon);
    
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplayActiveLine)
        .returns(KPreferenceOff);
    EXPECT(InfoWidgetLayoutManager::hideWidget)
        .with(InfoWidgetLayoutManager::RoleActiveLineLabel);
    EXPECT(InfoWidgetLayoutManager::hideWidget)
        .with(InfoWidgetLayoutManager::RoleActiveLineIcon);
    
    EXPECT(InfoWidgetPreferences::preference)
        .with(InfoWidgetPreferences::DisplaySatText)
        .returns(KPreferenceOff);
    EXPECT(InfoWidgetLayoutManager::hideWidget)
        .with(InfoWidgetLayoutManager::RoleSatTextLabel);
    EXPECT(InfoWidgetLayoutManager::hideWidget)
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
    
    QGraphicsWidget homeZoneBox;
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleHomeZoneCheckBox)
        .returns(&homeZoneBox);

    QGraphicsWidget mcnCheckBox;
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleMcnCheckBox)
        .returns(&mcnCheckBox);
    
    QGraphicsWidget activeLineCheckBox;
    EXPECT(InfoWidgetLayoutManager::getWidget)
        .with(InfoWidgetLayoutManager::RoleActiveLineCheckBox)
        .returns(&activeLineCheckBox);

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

QTEST_MAIN_S60(UT_InfoWidget)
