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
#include <hbevent.h>
#include <hbcolorscheme.h>
#include <hbmessagebox.h>
#include <hbframedrawer.h>
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

const int INFOWIDGET_ROUNDING = 15;
const int INFOWIDGET_DEFAULT_HEIGHT = 100;
const int INFOWIDGET_DEFAULT_WIDTH = 100;
const char *TS_FILE_OPERATOR_WIDGET = "operator_widget"; 
const char *TS_FILE_COMMON = "common";
const char *BACKGROUND_COLOR_GROUP_ID = "qtc_default_popup_normal"; 
const char *BACKGROUND_FRAME_NAME = "qtg_fr_hswidget_normal"; 

/*!
  \class InfoWidget
  \brief Operator info widget main class. 

   Implements HomeScreen specific slots and 
   graphical representation of the 
   Operator Info widget. 

   Derived from HbWidget.
    
*/

/*!
    InfoWidget::InfoWidget() 
*/
InfoWidget::InfoWidget(QGraphicsItem* parent, Qt::WindowFlags flags)
    : HbWidget(parent, flags),
    m_engine(NULL), 
    m_preferences(NULL),
    m_layoutManager(NULL),
    m_layout(NULL),
    m_frameDrawer(NULL),
    m_layoutChanging(false),
    m_dragEvent(false), 
    m_initialized(false)
{
    INSTALL_TRACE_MSG_HANDLER; 
    
    DPRINT << ": IN";
    
    // Localization file loading
    installTranslator(TS_FILE_OPERATOR_WIDGET);
    installTranslator(TS_FILE_COMMON);
    
    m_layoutManager = new InfoWidgetLayoutManager(this);
    Q_ASSERT(m_layoutManager); 
    
    m_engine = new InfoWidgetEngine(this); 
    Q_ASSERT(m_engine);
    
    m_preferences = new InfoWidgetPreferences(this);
    Q_ASSERT(m_preferences);
    
    QObject::connect( m_preferences, SIGNAL(prefChanged(int,int)),
                    m_engine, SLOT(preferenceChanged(int,int)));
    
    // Setup widget main layout 
    m_layout = new QGraphicsLinearLayout;    
    m_layout->setSpacing(0); 
    m_layout->setContentsMargins(0,0,0,0); 
    setLayout(m_layout);

    // Read color definitions 
    m_backGroundColor = HbColorScheme::color(
            BACKGROUND_COLOR_GROUP_ID);
    if (!m_backGroundColor.isValid()) {
        m_backGroundColor = Qt::black; 
    }
    
    // Create background frame drawer 
    m_frameDrawer = new HbFrameDrawer(
            BACKGROUND_FRAME_NAME, 
            HbFrameDrawer::NinePieces);
    Q_ASSERT(m_frameDrawer); 
    
    // Set widget initial size
    resize(INFOWIDGET_DEFAULT_WIDTH,
           INFOWIDGET_DEFAULT_HEIGHT); 
    
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
    
    if (m_frameDrawer) {
        delete m_frameDrawer;
        m_frameDrawer = NULL; 
    }
    
    // Remove and delete language translators 
    removeTranslators(); 
    
    DPRINT << ": OUT"; 
    UNINSTALL_TRACE_MSG_HANDLER;
}

/*!
    InfoWidget::onInitialize()
    
    Called by HS framework, saved preference data
    is available when onInitialize() is called and 
    meta-object data reading should be done here      
*/
void InfoWidget::onInitialize()
{
    DPRINT << ": IN";
    
    m_initialized = true; 
    
    // Initialize preferences from meta-object data
    if (!readPersistentPreferences()) {

        // Reading failed, initialize default values  
        m_preferences->setPreference(InfoWidgetPreferences::DisplaySpn, 
                DISPLAY_SETTING_ON);
        m_preferences->setPreference(InfoWidgetPreferences::DisplayMcn, 
                DISPLAY_SETTING_ON);
        m_preferences->setPreference(InfoWidgetPreferences::DisplaySatText, 
                DISPLAY_SETTING_ON);
    } 
    m_preferences->storePreferences(); 
    
    // Layout components 
    layoutInfoDisplay();
    m_layout->activate(); 
    
    // Read initial data from model
    updateInfoDisplay(); 
    
    // Listen for model changes 
    QObject::connect(m_engine, SIGNAL(modelChanged()), 
            this, SLOT(readModel())); 

    DPRINT << ": OUT";
}

/*!
    InfoWidget::onUninitialize() 
*/
void InfoWidget::onUninitialize()
{
    DPRINT;
    m_initialized = false; 
    m_engine->suspend(); 
}

/*!
    InfoWidget::onShow() 
*/
void InfoWidget::onShow()
{
    DPRINT;
    m_engine->resume(); 
}

/*!
    InfoWidget::onHide() 
*/
void InfoWidget::onHide()
{
    DPRINT;
    m_engine->suspend(); 
}

/*!
    InfoWidget::installTranslator() const
*/
bool InfoWidget::installTranslator(QString translationFile)
{
    DPRINT << ": IN";

    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";
    bool translatorLoaded(false);  
    
    QTranslator* widgetTranslator = new QTranslator;
    translatorLoaded = widgetTranslator->load(
            path + translationFile + "_" + lang);
    if (translatorLoaded) {
        qApp->installTranslator(widgetTranslator);
        m_translators.append(widgetTranslator); 
        DPRINT << ": translator installed: " << translationFile; 
    } else {
        delete widgetTranslator; 
        widgetTranslator = NULL; 
    }
    
    DPRINT << ": OUT";
    return translatorLoaded;
}

/*!
    InfoWidget::removeTranslators() const
*/
void InfoWidget::removeTranslators()
{
    DPRINT << ": IN";

    foreach (QTranslator *translator, m_translators) {
        qApp->removeTranslator(translator);
    }    
    qDeleteAll(m_translators);
    m_translators.clear();
    
    DPRINT << ": OUT";
}

/*!
    InfoWidget::boundingRect() const
*/
QRectF InfoWidget::boundingRect() const
{   
    return rect();
}

/*!
    InfoWidget::shape() const
    
    Return Operator widget's shape 
    according to currect display 
*/
QPainterPath InfoWidget::shape() const
{
    DPRINT;    
    
    QPainterPath path;
    if (m_layoutManager->currentDisplayRole() == 
                    InfoWidgetLayoutManager::InfoDisplay) {
        path.addRoundRect(boundingRect(), 
                INFOWIDGET_ROUNDING, 
                INFOWIDGET_ROUNDING);
    } else {
        path.addRect(boundingRect()); 
    }
    return path;
}

/*!
    InfoWidget::sizeHint() 
*/
QSizeF InfoWidget::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const   
{
    Q_UNUSED(which);
    Q_UNUSED(constraint); 
    
    QSizeF requiredSize(
            INFOWIDGET_DEFAULT_WIDTH,
            INFOWIDGET_DEFAULT_HEIGHT);
    
    if (m_initialized) { 
        // Read size hint from docml content
        if (m_layoutManager->currentDisplayRole() == 
                    InfoWidgetLayoutManager::InfoDisplay) {
            if (m_layoutManager->contentWidget()) {
                requiredSize = m_layoutManager->contentWidget()->minimumSize();
                // Height according number of rows, if 0 or 1 row use minimum size
                int rowCount = m_preferences->visibleItemCount();
                if (1 < rowCount) {
                        requiredSize.rheight() += (rowCount-1)*
                                m_layoutManager->rowHeight();
                }
            }
        }
        else if (m_layoutManager->currentDisplayRole() == 
                InfoWidgetLayoutManager::SettingsDisplay) {
            if (m_layoutManager->contentWidget()) {
                requiredSize= m_layoutManager->contentWidget()->size();
                }
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
    return QSizePolicy(
            QSizePolicy::Fixed, 
            QSizePolicy::Fixed); 
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
    if (m_preferences->preference(InfoWidgetPreferences::DisplaySpn).compare(
            DISPLAY_SETTING_ON) == 0) {
        layoutRows++;
    } else if (m_layoutManager->currentDisplayRole() != InfoWidgetLayoutManager::SettingsDisplay) {
        m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleSpnLabel); 
        m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleSpnIcon); 
    }

    if (m_preferences->preference(InfoWidgetPreferences::DisplayMcn).compare(
            DISPLAY_SETTING_ON) == 0) {
        layoutRows++;
    } else if (m_layoutManager->currentDisplayRole() != InfoWidgetLayoutManager::SettingsDisplay) {
        m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleMcnMarqueeItem); 
        m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleMcnIcon); 
    }
    
    if (m_preferences->preference(InfoWidgetPreferences::DisplaySatText).compare(
            DISPLAY_SETTING_ON) == 0) {
        layoutRows++;
    } else if (m_layoutManager->currentDisplayRole() != InfoWidgetLayoutManager::SettingsDisplay) {
        m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleSatMarqueeItem); 
        m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleSatTextIcon); 
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
        }       
    }
    
    m_layoutManager->showAll(); 
    updateItemsVisibility(); 

    m_layout->invalidate();
    m_layout->activate(); 
    
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
        QObject::connect(okButton, SIGNAL(clicked()), 
                this, SLOT(settingsEditingFinished()), Qt::UniqueConnection); 
    }

    // Connect display setting check boxes
    HbCheckBox *spnCheckBox = qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleSpnCheckBox));
    if (spnCheckBox) {
        spnCheckBox->setChecked(m_preferences->isPreferenceSet(
                InfoWidgetPreferences::DisplaySpn));
        
        QObject::connect(spnCheckBox, SIGNAL(stateChanged(int)), 
                this, SLOT(spnDisplaySettingChanged(int)), Qt::UniqueConnection); 
    }
    
    HbCheckBox *mcnCheckBox = qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleMcnCheckBox));
    if (mcnCheckBox) {
        mcnCheckBox->setChecked(m_preferences->isPreferenceSet(
                InfoWidgetPreferences::DisplayMcn));
        
        QObject::connect(mcnCheckBox, SIGNAL(stateChanged(int)), 
                this, SLOT(mcnDisplaySettingChanged(int)), Qt::UniqueConnection); 
    }
    
    HbCheckBox *satTextCheckBox = qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleSatTextCheckBox));
    if (satTextCheckBox) {
        satTextCheckBox->setChecked(m_preferences->isPreferenceSet(
                InfoWidgetPreferences::DisplaySatText));
        
        QObject::connect(satTextCheckBox, SIGNAL(stateChanged(int)), 
                this, SLOT(satDisplaySettingChanged(int)), Qt::UniqueConnection); 
    }
    
    DPRINT << ": OUT";
}

/*!
    InfoWidget::updateInfoDisplay() 
*/
void InfoWidget::updateInfoDisplay()
{
    DPRINT << ": IN"; 
    
    if (m_layoutManager->currentDisplayRole() == 
            InfoWidgetLayoutManager::InfoDisplay )
        {
        QString text;
        InfoWidgetEngine::ModelData modelData = m_engine->modelData(); 
    
        HbLabel *spnLabel = qobject_cast<HbLabel *>(m_layoutManager->getWidget(
                InfoWidgetLayoutManager::RoleSpnLabel));
        if (spnLabel) {
            if (m_engine->modelData().serviceProviderNameDisplayRequired()) {
                text = modelData.serviceProviderName();  
                spnLabel->setPlainText(text);
            }
        }        
        
        HbMarqueeItem *mcnMarqueeItem = qobject_cast<HbMarqueeItem *>(m_layoutManager->getWidget(
                InfoWidgetLayoutManager::RoleMcnMarqueeItem));
        if (mcnMarqueeItem) {
            text = modelData.mcnName(); 
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
    
        HbMarqueeItem *satMarqueeItem = qobject_cast<HbMarqueeItem *>(m_layoutManager->getWidget(
                InfoWidgetLayoutManager::RoleSatMarqueeItem));
        if (satMarqueeItem) {
            text = modelData.satDisplayText(); 
    
            satMarqueeItem->setText(text);
            
            // Set marquee animation looping mode to infinite
            satMarqueeItem->setLoopCount(-1); 
                        
            // Finally, start marquee animation
            DPRINT << ": satMarqueeItem->isAnimating()"; 
            if (!satMarqueeItem->isAnimating()) {
                DPRINT << ": mcnMarqueeItem->startAnimation()";   
                satMarqueeItem->startAnimation();
            }
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

    if (m_layoutManager->currentDisplayRole() == 
            InfoWidgetLayoutManager::InfoDisplay) { 
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
void InfoWidget::paint(QPainter *painter, 
        const QStyleOptionGraphicsItem *option, 
        QWidget *widget)
{
    Q_UNUSED(option); 
    Q_UNUSED(widget);
    DPRINT;

    if (!m_layoutChanging) {
        painter->save();
        
        if (m_layoutManager->currentDisplayRole() == 
                InfoWidgetLayoutManager::InfoDisplay) { 
            if (m_frameDrawer) {
                m_frameDrawer->paint(painter,boundingRect());
            }
        } else {            
            QBrush brush(Qt::black); 
            QPainterPath path;
            path.addRoundRect(boundingRect(), 
                    INFOWIDGET_ROUNDING, 
                    INFOWIDGET_ROUNDING);
            painter->fillPath(path, brush);
        }
        
        painter->restore();
    }
}

/*!
    InfoWidget::mousePressEvent() 
*/
void InfoWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    
    // Clear flag 
    m_dragEvent = false; 
}

/*!
    InfoWidget::mouseReleaseEvent() 
*/
void InfoWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    // If in info display and widget wasn't dragged 
    // change to settings display
    if ((!m_dragEvent) && 
          m_layoutManager->currentDisplayRole() == 
                  InfoWidgetLayoutManager::InfoDisplay) {
        DPRINT << ": layout settings display";
        layoutSettingsDisplay(); 
    }
    
    // Clear flag 
    m_dragEvent = false; 
}

/*!
    InfoWidget::mouseMoveEvent() 
*/
void InfoWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    
    // Mouse is moving 
    // after mouse press event
    m_dragEvent = true; 
}

/*!
    InfoWidget::spnDisplaySettingChanged() 
*/
void InfoWidget::spnDisplaySettingChanged(int state)
{
    DPRINT << ": state: " << state;
    if (state == Qt::Checked){
        m_preferences->setPreference(InfoWidgetPreferences::DisplaySpn, DISPLAY_SETTING_ON);
    } else {
        m_preferences->setPreference(InfoWidgetPreferences::DisplaySpn, DISPLAY_SETTING_OFF);
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
    InfoWidget::spnDisplay()
    
    Getter function for Meta-object property "spnDisplay" 
*/
QString InfoWidget::spnDisplay()
{
    DPRINT; 
    return m_preferences->preference(InfoWidgetPreferences::DisplaySpn);
}

/*!
    InfoWidget::setSpnDisplay()
    
    Setter function for Meta-object property "spnDisplay" 
*/
void InfoWidget::setSpnDisplay(QString value)
{
    DPRINT;
    m_preferences->setPreference(InfoWidgetPreferences::DisplaySpn, value);
}

/*!
    InfoWidget::readPersistentPreferences()
    
    Read Meta-object properties and store to preference handler. 
    Restores preferences from previous session.   
*/
bool InfoWidget::readPersistentPreferences()
{
    DPRINT << ": IN";
    bool changed(false); 
    
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
    
    propertyValue = QObject::property("satDisplay").toString();
    m_preferences->setPreference(InfoWidgetPreferences::DisplaySatText, 
            propertyValue);

    propertyValue = QObject::property("spnDisplay").toString();
    m_preferences->setPreference(InfoWidgetPreferences::DisplaySpn, 
            propertyValue);

    // Check that at least one item is set visible and  
    // store preferences if true 
    if (m_preferences->validate()) {
        changed = m_preferences->storePreferences(); 
    } 
        
    DPRINT << ": OUT";
    return changed; 
}

/*!
    InfoWidget::initializeCheckBoxStates()
*/
void InfoWidget::initializeCheckBoxStates()
{
    DPRINT << ": IN";

    HbCheckBox *spnCheckBox = qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleSpnCheckBox));
    if (spnCheckBox) {
    spnCheckBox->setChecked(m_preferences->isPreferenceSet(
                InfoWidgetPreferences::DisplaySpn));
    }
    
    HbCheckBox *mcnCheckBox = qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleMcnCheckBox));
    if (mcnCheckBox) {
        mcnCheckBox->setChecked(m_preferences->isPreferenceSet(
                InfoWidgetPreferences::DisplayMcn));
    }
    
    HbCheckBox *satTextCheckBox = qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
            InfoWidgetLayoutManager::RoleSatTextCheckBox));
    if (satTextCheckBox) {
        satTextCheckBox->setChecked(m_preferences->isPreferenceSet(
                InfoWidgetPreferences::DisplaySatText));
    }
        
    DPRINT << ": OUT"; 
}

/*!
    InfoWidget::settingsEditingFinished()
*/
void InfoWidget::settingsEditingFinished()
{
    DPRINT << ": IN";
    
    // Save settings data if validation succeeds 
    if (m_preferences->validate()) {
        DPRINT << ": switching to info display";
        
        // Store preferences if changed 
        if (m_preferences->storePreferences()) {
            // Signal Homescreen FW   
            emit setPreferences(
                    m_preferences->preferenceNames());
            }
        
        // ToDo: do only if settings have really changed 
        m_layoutManager->reloadWidgets(
                InfoWidgetLayoutManager::InfoDisplay);

        // Switch to info display 
        layoutInfoDisplay();
        updateInfoDisplay();
        
    } else {
        DPRINT << ": staying in settings display";    
        // Display warning note
        settingsValidationFailed(); 
        
        // Restore check box states 
        initializeCheckBoxStates(); 
    }
    
    DPRINT << ": OUT";
}

/*!
    InfoWidget::startChanges()
*/
void InfoWidget::startChanges()
{
    DPRINT;
    m_layoutChanging = true; 
}

/*!
    InfoWidget::endChanges()
*/
void InfoWidget::endChanges()
{
    DPRINT;
    m_layoutChanging = false; 
}

/*!
   \reimp
*/
void InfoWidget::changeEvent(QEvent *event)
{
   DPRINT << ": IN";
   
   if (event->type() == HbEvent::ThemeChanged) {
       DPRINT << ": HbEvent::ThemeChanged";
       m_backGroundColor = HbColorScheme::color(
               BACKGROUND_COLOR_GROUP_ID);
       if (!m_backGroundColor.isValid()) {
           m_backGroundColor = Qt::black; 
       }
   }
   HbWidget::changeEvent(event);
   
   DPRINT << ": OUT";
}

/*!
   InfoWidget::settingsValidationFailed()
*/
void InfoWidget::settingsValidationFailed()
{
   DPRINT << ": IN";
   
   if (m_layoutManager->currentDisplayRole() == 
                  InfoWidgetLayoutManager::SettingsDisplay) {
       HbMessageBox::warning(
               hbTrId("txt_operatorwidget_info_select_one"));
   }

   DPRINT << ": OUT";
}


// End of File. 

