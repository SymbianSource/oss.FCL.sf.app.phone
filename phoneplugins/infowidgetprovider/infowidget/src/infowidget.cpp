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

#include "infowidget.h"

#include <hbanchorlayout.h>
#include <hbiconitem.h>
#include <hbmarqueeitem.h>
#include <hbfontspec.h>
#include <hbdialog.h>
#include <hblabel.h>
#include <hbaction.h>
#include <hbcheckbox.h>
#include <hbpushbutton.h>
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QGraphicsLinearLayout>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "infowidgetlogging.h"
#include "infowidgetengine.h"
#include "infowidgetlayoutmanager.h"
#include "infowidgetpreferences.h"

const int INFOWIDGET_LINE_WIDTH = 0; 
const int INFOWIDGET_MARGIN = 5;
const int INFOWIDGET_ROUNDING = 15;


/*!
    InfoWidget::InfoWidget() 
*/
InfoWidget::InfoWidget(QGraphicsItem* parent, Qt::WindowFlags flags)
    : HsWidget(parent, flags),
    m_engine(NULL), 
    m_preferences(NULL),
    m_layoutManager(NULL),
    m_layout(NULL),
    m_layoutChanging(false),
    m_isDragEvent(false)
{
    INSTALL_TRACE_MSG_HANDLER; 
    
    DPRINT << ": IN";
    
    // Localization file loading
    QTranslator translator; 
    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";
    DPRINT << ": loading translation: " << QString(path + "operator_widget_" + lang);
    bool translatorLoaded = translator.load(path + "operator_widget_" + lang);
    DPRINT << ": translator loaded: " << translatorLoaded; 
    if (translatorLoaded) {
        qApp->installTranslator(&translator);
        DPRINT << ": translator installed"; 
    }
    
    m_layoutManager = new InfoWidgetLayoutManager(this);
    Q_ASSERT(m_layoutManager); 
    
    // Create network engine  
    m_engine = new InfoWidgetEngine(this); 

    // Create widget preference handler 
    m_preferences = new InfoWidgetPreferences(this);

    DPRINT << ": reading preferences from meta-object properties";
    initializePreferences();

    // Create widget main layout 
    m_layout = new QGraphicsLinearLayout;    
    m_layout->setSpacing(0); 
    m_layout->setContentsMargins(0,0,0,0); 
    
    // Layout info display
    layoutInfoDisplay();

    setLayout(m_layout);

    // Read initial data from model
    updateInfoDisplay(); 
    
    QObject::connect(m_engine, SIGNAL(modelChanged()), 
            this, SLOT(readModel())); 

    DPRINT << ": OUT";
}

/*!
    InfoWidget::~InfoWidget() 
*/
InfoWidget::~InfoWidget()
{
    DPRINT << ": IN"; 
    
    // Clean layout container, 
    // layout manager handles layout item cleanup
    for (int i=0; i < m_layout->count(); i++) {
        DPRINT << ": item(" << i << ") removed from layout";
        m_layout->removeAt(i);
        } 
    
    DPRINT << ": OUT"; 
    UNINSTALL_TRACE_MSG_HANDLER;
}

/*!
    InfoWidget::boundingRect() const
*/
QRectF InfoWidget::boundingRect() const
{
    DPRINT;
    
    QRectF rectF = rect();
    rectF.adjust(-INFOWIDGET_MARGIN, -INFOWIDGET_MARGIN, 
            INFOWIDGET_MARGIN, INFOWIDGET_MARGIN);
    
    return rectF;
}

/*!
    InfoWidget::shape() const
    Return shape
*/
QPainterPath InfoWidget::shape() const
{
    DPRINT;    
    
    QPainterPath path;
    path.addRoundRect(boundingRect(), INFOWIDGET_ROUNDING, INFOWIDGET_ROUNDING);
    return path;
}

/*!
    InfoWidget::sizeHint() 
*/
QSizeF InfoWidget::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const   
{
    Q_UNUSED(which);
    Q_UNUSED(constraint); 
    
    QSizeF requiredSize(70,160);
    
    // Try to get size hint from docml content, if not found use default  
    // size preference 
    if (m_layoutManager->currentDisplayRole() == 
                InfoWidgetLayoutManager::InfoDisplay) {
        if (m_layoutManager->contentWidget()) {
            requiredSize = m_layoutManager->contentWidget()->minimumSize();
            }
    }
    else if (m_layoutManager->currentDisplayRole() == 
            InfoWidgetLayoutManager::SettingsDisplay) {
        requiredSize = QSizeF(250,250);
        if (m_layoutManager->contentWidget()) {
            requiredSize = m_layoutManager->contentWidget()->preferredSize();
            }
    } 

    DPRINT << ": returning size: " << requiredSize;
    return requiredSize; 
}

/*!
    InfoWidget::sizePolicy() 
*/
QSizePolicy InfoWidget::sizePolicy () const 
{
    DPRINT;

    // Size tells the exact size for the widget    
    return QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); 
}

/*!
    InfoWidget::onInitialize() 
*/
void InfoWidget::onInitialize()
{
    DPRINT << ": IN";
    
    // Initialize preferences from meta-object data 
    // set by homescreen framework. Homescreen framework 
    // has restored the properties before calling onInitialize  
    DPRINT << ": reading preferences from meta-object properties";
    initializePreferences();
    
    DPRINT << ": OUT";
}

/*!
    InfoWidget::onUninitialize() 
*/
void InfoWidget::onUninitialize()
{
    DPRINT;
}

/*!
    InfoWidget::onShow() 
*/
void InfoWidget::onShow()
{
    DPRINT;
}

/*!
    InfoWidget::onHide() 
*/
void InfoWidget::onHide()
{
    DPRINT;
}

/*!
    InfoWidget::onStart() 
*/
HsWidget::StartResult InfoWidget::onStart()
{
    DPRINT; 
    return HsWidget::StartResultRunning; 
}

/*!
    InfoWidget::onStop() 
*/
HsWidget::StopResult InfoWidget::onStop()
{
    DPRINT; 
    return HsWidget::StopResultFinished; 
}

/*!
    InfoWidget::onSuspend() 
*/
HsWidget::SuspendResult InfoWidget::onSuspend()
{
    DPRINT; 
    return HsWidget::SuspendResultSuspended; 
}

/*!
    InfoWidget::onResume() 
*/
HsWidget::ResumeResult InfoWidget::onResume()
{
    DPRINT; 
    return HsWidget::ResumeResultRunning; 
}

/*!
    InfoWidget::updateItemsVisibility() 
*/
void InfoWidget::updateItemsVisibility()
{
    DPRINT <<": IN"; 
    int layoutRows = 0; 
    QList<QGraphicsWidget *> widgetsToHide; 
    
    // Update layout according to item visibility settings
    // 1. read visible items
    if (m_preferences->preference(InfoWidgetPreferences::DisplayHomeZone).compare(
            DISPLAY_SETTING_ON) == 0) {
        layoutRows++;
    } else if (m_layoutManager->currentDisplayRole() != InfoWidgetLayoutManager::SettingsDisplay) {
        m_layoutManager->hideWidget(InfoWidgetLayoutManager::RoleHomeZoneLabel); 
        m_layoutManager->hideWidget(InfoWidgetLayoutManager::RoleHomeZoneIcon); 
    }
    
    if (m_preferences->preference(InfoWidgetPreferences::DisplayMcn).compare(
            DISPLAY_SETTING_ON) == 0) {
        layoutRows++;
    } else if (m_layoutManager->currentDisplayRole() != InfoWidgetLayoutManager::SettingsDisplay) {
        m_layoutManager->hideWidget(InfoWidgetLayoutManager::RoleMcnMarqueeItem); 
        m_layoutManager->hideWidget(InfoWidgetLayoutManager::RoleMcnIcon); 
    }

    if (m_preferences->preference(InfoWidgetPreferences::DisplayActiveLine).compare(
            DISPLAY_SETTING_ON) == 0) {
        layoutRows++;
    } else if (m_layoutManager->currentDisplayRole() != InfoWidgetLayoutManager::SettingsDisplay) {
        m_layoutManager->hideWidget(InfoWidgetLayoutManager::RoleActiveLineLabel); 
        m_layoutManager->hideWidget(InfoWidgetLayoutManager::RoleActiveLineIcon); 
    }
    
    if (m_preferences->preference(InfoWidgetPreferences::DisplaySatText).compare(
            DISPLAY_SETTING_ON) == 0) {
        layoutRows++;
    } else if (m_layoutManager->currentDisplayRole() != InfoWidgetLayoutManager::SettingsDisplay) {
        m_layoutManager->hideWidget(InfoWidgetLayoutManager::RoleSatTextLabel); 
        m_layoutManager->hideWidget(InfoWidgetLayoutManager::RoleSatTextIcon); 
    }
    
    DPRINT << ": visible layout rows count: " << layoutRows;
    m_layoutManager->setLayoutRows(layoutRows);
}

/*!
    InfoWidget::layoutInfoDisplay()
    
    Layout info display    
*/
void InfoWidget::layoutInfoDisplay()
{  
    startChanges(); 
    
    QGraphicsLayout *infoDisplayLayout = 
        m_layoutManager->layoutInfoDisplay(); 
    
    DPRINT << ": IN";
    if (infoDisplayLayout) {
        DPRINT << ": infoDisplayLayout has been returned from layout manager";

        QGraphicsWidget *contentWidget = m_layoutManager->contentWidget();
        if (contentWidget) {
            DPRINT << ": contentWidget has been returned from layout manager";
            
            // Remove old data from layout. 
            for (int i=0; i < m_layout->count(); i++) {
                DPRINT << ": item(" << i << ") removed from layout";
                m_layout->removeAt(i);
                } 
            
            // Add content widget to main layout 
            m_layout->addItem(contentWidget);
            
            resize(contentWidget->preferredSize()); 
        }       
    }
    
    m_layoutManager->showAll(); 
    updateItemsVisibility(); 

    endChanges(); 
    
    DPRINT << ": OUT";
}

/*!
    InfoWidget::layoutSettingsDisplay()
    
    Switch to settings display layout   
*/
void InfoWidget::layoutSettingsDisplay()
{  
    startChanges(); 
    
    QGraphicsLayout *settingDisplayLayout = 
        m_layoutManager->layoutSettingsDisplay(); 
    
    DPRINT << ": IN";
    if (settingDisplayLayout) {
        DPRINT << ": settingDisplayLayout has been returned from layout manager";

        QGraphicsWidget *contentWidget = m_layoutManager->contentWidget();
        if (contentWidget) {
            DPRINT << ": contentWidget has been returned from layout manager";
                
            // Remove old data from layout. 
            for (int i=0; i < m_layout->count(); i++) {
                DPRINT << ": item(" << i << ") removed from layout";
                m_layout->removeAt(i);
                } 
            
            // Add content widget to main layout 
            m_layout->addItem(contentWidget); 

            resize(contentWidget->preferredSize()); 
        }
        
        // Connect settings display widget signals 
        initializeSettingsDisplayItems(); 
    }
     
    m_layoutManager->showAll(); 

    endChanges(); 
    
    DPRINT << ": OUT";
}

/*!
    InfoWidget::initializeInfoDisplayItems()
*/
void InfoWidget::initializeInfoDisplayItems()
{  
    DPRINT;
}

/*!
    InfoWidget::initializeSettingsDisplayItems()
    
    Set up initial check box states, lock check boxes 
    if needed and connect signals to local slots  
*/
void InfoWidget::initializeSettingsDisplayItems()
{  
    DPRINT << ": IN";
    
    HbPushButton *okButton = qobject_cast<HbPushButton *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleOkButton));
    if (okButton) {
        DPRINT << ": okButton has been returned from layout manager, connecting signal";
        QObject::connect(okButton, SIGNAL(clicked()), 
                this, SLOT(settingsEditingFinished())); 
    }

    // Connect display setting check boxes
    HbCheckBox *homeZoneBox = qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleHomeZoneCheckBox));
    if (homeZoneBox) {
        DPRINT << ": homeZoneBox has been returned from layout manager, initializing";
        // Make checkable when home zone display is supported
        homeZoneBox->setCheckable(false); 
        QObject::connect(homeZoneBox, SIGNAL(stateChanged(int)), 
                this, SLOT(homeZoneDisplaySettingChanged(int))); 
    }
    
    HbCheckBox *mcnCheckBox = qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleMcnCheckBox));
    if (mcnCheckBox) {
        mcnCheckBox->setChecked(true);
        
        DPRINT << ": mcnCheckBox has been returned from layout manager, connecting signal";
        QObject::connect(mcnCheckBox, SIGNAL(stateChanged(int)), 
                this, SLOT(mcnDisplaySettingChanged(int))); 
    }
    
    HbCheckBox *activeLineCheckBox = qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleActiveLineCheckBox));
    if (activeLineCheckBox) {
        // Make checkable when active line display is supported
        activeLineCheckBox->setCheckable(false); 

        DPRINT << ": activeLineCheckBox has been returned from layout manager, connecting signal";
        QObject::connect(activeLineCheckBox, SIGNAL(stateChanged(int)), 
                this, SLOT(activeLineDisplaySettingChanged(int))); 
    }
    
    HbCheckBox *satTextCheckBox = qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleSatTextCheckBox));
    if (satTextCheckBox) {
        // Make checkable when sat text display is supported
        satTextCheckBox->setCheckable(false); 

        DPRINT << ": satTextCheckBox has been returned from layout manager, connecting signal";
        QObject::connect(satTextCheckBox, SIGNAL(stateChanged(int)), 
                this, SLOT(satDisplaySettingChanged(int))); 
    }
    
    DPRINT << ": OUT";
}

/*!
    InfoWidget::updateInfoDisplay() 
*/
void InfoWidget::updateInfoDisplay()
{
    DPRINT; 
    
    QString text;
    InfoWidgetEngine::ModelData modelData = m_engine->modelData(); 

    HbLabel *homeZoneLabel = qobject_cast<HbLabel *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleHomeZoneLabel));
    if (homeZoneLabel && 
            m_layoutManager->currentDisplayRole() == InfoWidgetLayoutManager::InfoDisplay) {
        text = modelData.homeZoneTextTag(); 
        homeZoneLabel->setPlainText(text);
    }        
    
    HbMarqueeItem *mcnMarqueeItem = qobject_cast<HbMarqueeItem *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleMcnMarqueeItem));
    if (mcnMarqueeItem && 
            m_layoutManager->currentDisplayRole() == InfoWidgetLayoutManager::InfoDisplay) {
        text = modelData.mcnName(); 
        if (text.length()) {
            mcnMarqueeItem->setText(text);
                
            // Set marquee animation looping mode to infinite
            mcnMarqueeItem->setLoopCount(-1); 
            
            // Finally, start marquee animation
            DPRINT << ": mcnMarqueeItem->isAnimating()"; 
            if (!mcnMarqueeItem->isAnimating()) {
                DPRINT << ": mcnMarqueeItem->startAnimation()";   
                mcnMarqueeItem->startAnimation();
            }
        }
    }

    HbLabel *activeLineLabel = qobject_cast<HbLabel *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleActiveLineLabel));
    if (activeLineLabel && 
            m_layoutManager->currentDisplayRole() == InfoWidgetLayoutManager::InfoDisplay) {
        text.setNum(modelData.activeLine());
        text.insert(0, hbTrId("Line: "));
    
        if (text.length()) {
            activeLineLabel->setPlainText(text);
        }
    }    

    HbLabel *satTextLabel = qobject_cast<HbLabel *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleSatTextLabel));
    if (satTextLabel && 
            m_layoutManager->currentDisplayRole() == InfoWidgetLayoutManager::InfoDisplay) {

        text = modelData.satDisplayText(); 

        if (text.length()) {
            satTextLabel->setPlainText(text);
        }
    }    

}

/*!
    InfoWidget::readModel() 
    
    Read model data. Model's modelChanged - signal is connected to this slot.  
*/
void InfoWidget::readModel()
{
    DPRINT << ": IN"; 

    if (m_layoutManager->currentDisplayRole() == InfoWidgetLayoutManager::InfoDisplay) { 
        updateInfoDisplay(); 
    }
    DPRINT << ": OUT";
}

/*!
    InfoWidget::handleModelError() 
    
    Model error signal is connected to this slot 
*/
void InfoWidget::handleModelError(int operation,int errorCode)
{
    Q_UNUSED(operation); 
    Q_UNUSED(errorCode); 
    DPRINT;
}

/*!
    InfoWidget::paint() 
*/
void InfoWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); 
    Q_UNUSED(widget);
    DPRINT;

    if (!m_layoutChanging) {

        QBrush brush(Qt::white); 
        brush.setStyle(Qt::SolidPattern); 
        painter->setBackground(brush); 
            
        QRectF drawRect = boundingRect(); 
        drawRect.adjust( INFOWIDGET_LINE_WIDTH, INFOWIDGET_LINE_WIDTH, 
                         -INFOWIDGET_LINE_WIDTH, -INFOWIDGET_LINE_WIDTH );
        
        QPainterPath path;
        path.addRoundRect(drawRect, INFOWIDGET_ROUNDING, INFOWIDGET_ROUNDING);
    
        painter->save();
        
        painter->fillPath(path, Qt::white);
        painter->restore();
    }
}

/*!
    InfoWidget::mousePressEvent() 
*/

void InfoWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    DPRINT; 
    
    // Initialize flag 
    m_isDragEvent = false; 
    
    DPRINT; 
}

/*!
    InfoWidget::mouseReleaseEvent() 
*/
void InfoWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    // If in info display and widget wasn't dragged 
    // change to settings display
    if ((!m_isDragEvent) && 
          m_layoutManager->currentDisplayRole() == 
                  InfoWidgetLayoutManager::InfoDisplay) {
        DPRINT << ": layout settings display";
        layoutSettingsDisplay(); 
    }
    
    m_isDragEvent = false; 
}

/*!
    InfoWidget::mouseMoveEvent() 
*/
void InfoWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    
    // Mouse is moving after mouse press event
    m_isDragEvent = true; 
}

/*!
    InfoWidget::homeZoneDisplaySettingChanged() 
*/
void InfoWidget::homeZoneDisplaySettingChanged(int state)
{
    DPRINT << ": state: " << state;
    if (state == Qt::Checked){
        m_preferences->setPreference(InfoWidgetPreferences::DisplayHomeZone, DISPLAY_SETTING_ON);
    } else {
        m_preferences->setPreference(InfoWidgetPreferences::DisplayHomeZone, DISPLAY_SETTING_OFF);
    }
}

/*!
    InfoWidget::mcnDisplaySettingChanged() 
*/
void InfoWidget::mcnDisplaySettingChanged(int state)
{
    DPRINT << ": state: " << state; 
    if (state == Qt::Checked){
        m_preferences->setPreference(InfoWidgetPreferences::DisplayMcn, DISPLAY_SETTING_ON);
    } else {
        m_preferences->setPreference(InfoWidgetPreferences::DisplayMcn, DISPLAY_SETTING_OFF);
    }
}

/*!
    InfoWidget::activeLineDisplaySettingChanged() 
*/
void InfoWidget::activeLineDisplaySettingChanged(int state)
{
    DPRINT << ": state: " << state; 
    if (state == Qt::Checked){
        m_preferences->setPreference(InfoWidgetPreferences::DisplayActiveLine, DISPLAY_SETTING_ON);
    } else {
        m_preferences->setPreference(InfoWidgetPreferences::DisplayActiveLine, DISPLAY_SETTING_OFF);
    }
}

/*!
    InfoWidget::satDisplaySettingChanged() 
*/
void InfoWidget::satDisplaySettingChanged(int state)
{
    DPRINT << ": state: " << state; 
    if (state == Qt::Checked){
        m_preferences->setPreference(InfoWidgetPreferences::DisplaySatText, DISPLAY_SETTING_ON);
    } else {
        m_preferences->setPreference(InfoWidgetPreferences::DisplaySatText, DISPLAY_SETTING_OFF);
    }
}

/*!
    InfoWidget::mcnDisplay() 
    
    Getter function for Meta-object property "mcnDisplay"
*/
QString InfoWidget::mcnDisplay()
{
    DPRINT; 
    return m_preferences->preference(InfoWidgetPreferences::DisplayMcn); 
}

/*!
    InfoWidget::setMcnDisplay() 
    
    Setter function for Meta-object property "mcnDisplay"
*/
void InfoWidget::setMcnDisplay(QString value)
{
    DPRINT;
    m_preferences->setPreference(InfoWidgetPreferences::DisplayMcn, value);
    }

/*!
    InfoWidget::homeZoneDisplay() 
    
    Getter function for Meta-object property "homeZoneDisplay"
*/
QString InfoWidget::homeZoneDisplay()
{
    DPRINT; 
    return m_preferences->preference(InfoWidgetPreferences::DisplayHomeZone); 
}

/*!
    InfoWidget::setHomeZoneDisplay()
    
    Setter function for Meta-object property "homeZoneDisplay" 
*/
void InfoWidget::setHomeZoneDisplay(QString value)
{
    DPRINT; 
    m_preferences->setPreference(InfoWidgetPreferences::DisplayHomeZone, value);
}

/*!
    InfoWidget::activeLineDisplay() 
    
    Getter function for Meta-object property "activeLineDisplay"
*/
QString InfoWidget::activeLineDisplay()
{
    DPRINT; 
    return m_preferences->preference(InfoWidgetPreferences::DisplayActiveLine);
}

/*!
    InfoWidget::setActiveLineDisplay() 
    
    Setter function for Meta-object property "activeLineDisplay"
*/
void InfoWidget::setActiveLineDisplay(QString value)
{
    DPRINT; 
    m_preferences->setPreference(InfoWidgetPreferences::DisplayActiveLine, value);
}

/*!
    InfoWidget::satDisplay()
    
    Getter function for Meta-object property "satDisplay" 
*/
QString InfoWidget::satDisplay()
{
    DPRINT; 
    return m_preferences->preference(InfoWidgetPreferences::DisplaySatText);
}

/*!
    InfoWidget::setSatDisplay()
    
    Setter function for Meta-object property "satDisplay" 
*/
void InfoWidget::setSatDisplay(QString value)
{
    DPRINT;
    m_preferences->setPreference(InfoWidgetPreferences::DisplaySatText, value);
}

/*!
    InfoWidget::initializePreferences()
    
    Read initial Meta-object properties and store to preference handler. 
    Restores preferences from previous session.   
*/
void InfoWidget::initializePreferences()
{
    DPRINT << ": IN";
    QString propertyValue;
    
    propertyValue = QObject::property("homeZoneDisplay").toString();
    m_preferences->setPreference(InfoWidgetPreferences::DisplayHomeZone, 
            propertyValue);
    
    propertyValue = QObject::property("mcnDisplay").toString();
    m_preferences->setPreference(InfoWidgetPreferences::DisplayMcn, 
            propertyValue);
    
    propertyValue = QObject::property("activeLineDisplay").toString();
    m_preferences->setPreference(InfoWidgetPreferences::DisplayActiveLine, 
            propertyValue);
    
    propertyValue = QObject::property("satTextDisplay").toString();
    m_preferences->setPreference(InfoWidgetPreferences::DisplaySatText, 
            propertyValue);

    // Check that at least one item is set visible. If not 
    // set default item(s) visible. TBD which items and how selected.   
    if (m_preferences->visibleItemCount() == 0) {
        DPRINT << ": no visible items initially, setting MCN on by default"; 
        m_preferences->setPreference(InfoWidgetPreferences::DisplayMcn, 
                DISPLAY_SETTING_ON);
    }
        
    DPRINT << ": OUT"; 
}

/*!
    InfoWidget::settingsEditingFinished()
*/
void InfoWidget::settingsEditingFinished()
{
    DPRINT << ": IN";
    
    layoutInfoDisplay();
    
    updateInfoDisplay(); 
    
    DPRINT << ": OUT";
}

void InfoWidget::startChanges()
{
    DPRINT << ": IN";
    
    m_layoutChanging = true;; 
    
    DPRINT << ": OUT";
}

void InfoWidget::endChanges()
{
    DPRINT << ": IN";
    
    m_layoutChanging = false;; 
    
    DPRINT << ": OUT";
}


// End of File. 

