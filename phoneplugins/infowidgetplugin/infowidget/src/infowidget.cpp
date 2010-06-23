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
#include <hbiconitem.h>
#include <hbmarqueeitem.h>
#include <hbfontspec.h>
#include <hbaction.h>
#include <hbcheckbox.h>
#include <hbevent.h>
#include <hbcolorscheme.h>
#include <hbdialog.h>
#include <hbmessagebox.h>
#include <hbframedrawer.h>
#include <hbframeitem.h>
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QGraphicsLinearLayout>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QScopedPointer>
#include "infowidgetlogging.h"
#include "infowidgetengine.h"
#include "infowidgetlayoutmanager.h"
#include "infowidgetpreferences.h"

/*!
  \class InfoWidget
  \brief Operator info widget main class. 

   Implements HomeScreen specific slots and 
   graphical representation of the 
   Operator Info widget. 

   Derived from HbWidget.
    
*/

// Local constants 
const int INFOWIDGET_DEFAULT_HEIGHT = 100;
const int INFOWIDGET_DEFAULT_WIDTH = 200;
const char *TS_FILE_OPERATOR_WIDGET = "operator_widget"; 
const char *TS_FILE_COMMON = "common";
const char *BACKGROUND_FRAME_NAME = "qtg_fr_hswidget_normal"; 

/*!
    InfoWidget::InfoWidget() 
*/
InfoWidget::InfoWidget(QGraphicsItem* parent, Qt::WindowFlags flags)
    : HbWidget(parent, flags),
    m_animationState(AnimationIdle), 
    m_engine(NULL), 
    m_preferences(NULL),
    m_layoutManager(NULL),
    m_layout(NULL),
    m_backgroundFrameItem(NULL),
    m_timerId(0),
    m_layoutChanging(false),
    m_dragEvent(false), 
    m_initialized(false)
{
    INSTALL_TRACE_MSG_HANDLER; 
    
    DPRINT << ": IN";
    
    // Localization file loading
    installTranslator(TS_FILE_OPERATOR_WIDGET);
    installTranslator(TS_FILE_COMMON);

    // Create layout & child-widget manager 
    m_layoutManager.reset(new InfoWidgetLayoutManager);
    
    // Create widget engine 
    m_engine.reset(new InfoWidgetEngine);
    
    // Create preference store and start listening signal(s) 
    m_preferences.reset(new InfoWidgetPreferences);
    QObject::connect(m_preferences.data(), 
            SIGNAL(preferencesChanged(InfoWidgetPreferences::Options)),
                m_engine.data(), 
            SLOT(handlePreferencesChanged(
                InfoWidgetPreferences::Options)));

    // Setup widget main layout 
    m_layout = new QGraphicsLinearLayout;    
    m_layout->setSpacing(0); 
    m_layout->setContentsMargins(0,0,0,0); 
    setLayout(m_layout);
  
    // Create and set background frame drawer
    QScopedPointer<HbFrameDrawer> backgroundFrameDrawer(
            new HbFrameDrawer(
                    BACKGROUND_FRAME_NAME, 
                    HbFrameDrawer::NinePieces));
    Q_ASSERT(!backgroundFrameDrawer.isNull()); 
    
    // Set widget initial size
    resize(INFOWIDGET_DEFAULT_WIDTH,
           INFOWIDGET_DEFAULT_HEIGHT); 

    // Ownership of frame drawer is 
    // transferred for frame item
    m_backgroundFrameItem = new HbFrameItem(
            backgroundFrameDrawer.take(), this);  
    
    setBackgroundItem(m_backgroundFrameItem); 

    DPRINT << ": OUT";
}

/*!
    InfoWidget::~InfoWidget() 
*/
InfoWidget::~InfoWidget()
{
    DPRINT << ": IN";
    
    // Force layout manager to delete widgets 
    // before InfoWidget is destroyed   
    m_layoutManager->destroyWidgets(); 
    
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
    
    // Update background frame size  
    m_backgroundFrameItem->resize(size());
    
    // Listen for model changes 
    QObject::connect(m_engine.data(), SIGNAL(modelChanged()), 
            this, SLOT(readModel()), Qt::UniqueConnection); 
    
    DPRINT << ": OUT";
}

/*!
    InfoWidget::onUninitialize() 
*/
void InfoWidget::onUninitialize()
{
    DPRINT << ": IN";
    stopMarquees();
    m_initialized = false; 
    m_engine->suspend();
    DPRINT << ": OUT";
}

/*!
    InfoWidget::onShow() 
*/
void InfoWidget::onShow()
{
    DPRINT;
    if (m_initialized) { 
        m_engine->resume();
        updateInfoDisplay(); 
    }
}

/*!
    InfoWidget::onHide() 
*/
void InfoWidget::onHide()
{
    DPRINT;
    if (m_initialized) { 
        m_engine->suspend();
        stopMarquees(); 
    }
}

/*!
    InfoWidget::timerEvent() 
*/
void InfoWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event); 
    
    if (m_animationState == AnimationStarting) {
        // Execute delayed start of marquee animation 
        if (m_animatingItem) {
            m_animationState = AnimationOngoing;
            m_animatingItem->startAnimation(); 
        } 
    }
    
    if (m_timerId) {
        killTimer(m_timerId);
        m_timerId = 0;
    }
}

/*!
    InfoWidget::installTranslator() 
*/
bool InfoWidget::installTranslator(QString translationFile)
{
    DPRINT << ": IN";

    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";
    bool translatorLoaded(false);  
    
    QScopedPointer<QTranslator> widgetTranslator; 
    widgetTranslator.reset(new QTranslator);
    translatorLoaded = widgetTranslator->load(
            path + translationFile + "_" + lang);
    if (translatorLoaded) {
        qApp->installTranslator(widgetTranslator.data());
        m_translators.append(widgetTranslator.take()); 
        DPRINT << ": translator installed: " << translationFile; 
    }
    
    DPRINT << ": OUT";
    return translatorLoaded;
}

/*!
    InfoWidget::removeTranslators()
    
    Remove translators from qApp and delete objects 
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
    InfoWidget::boundingRect() 
*/
QRectF InfoWidget::boundingRect() const
{   
    return rect();
}

/*!
    InfoWidget::sizeHint() 
    
    Calculate size hint based on visible rows count 
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
        requiredSize = m_layoutManager->contentWidget()->minimumSize();
        // Height according number of rows, if 0 or 1 row use minimum size
        int rowCount = m_preferences->visibleItemCount();
        if (1 < rowCount) {
                requiredSize.rheight() += (rowCount-1)*
                        m_layoutManager->layoutRowHeight();
        }
        
        // Update background frame size 
        // if widget size is changing
        if (size() != requiredSize) {
            m_backgroundFrameItem->resize(requiredSize);
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
    } else {
        m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleSpnMarqueeItem); 
        m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleSpnIcon); 
    }

    if (m_preferences->preference(InfoWidgetPreferences::DisplayMcn).compare(
            DISPLAY_SETTING_ON) == 0) {
        layoutRows++;
    } else {
        m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleMcnMarqueeItem); 
        m_layoutManager->removeWidget(InfoWidgetLayoutManager::RoleMcnIcon); 
    }
    
    if (m_preferences->preference(InfoWidgetPreferences::DisplaySatText).compare(
            DISPLAY_SETTING_ON) == 0) {
        layoutRows++;
    } else {
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
    DPRINT << ": IN";
    
    QGraphicsLayout *infoDisplayLayout = 
        m_layoutManager->layoutInfoDisplay(); 
    
    if (!m_layout->count()) {
        QGraphicsWidget *contentWidget = 
                m_layoutManager->contentWidget();
        if (contentWidget) {
            // Add content widget to main layout 
            m_layout->addItem(contentWidget);
        }
    }

    updateItemsVisibility(); 
    
    endChanges();
    DPRINT << ": OUT";
}

/*!
    InfoWidget::layoutSettingsDialog()
    
    Layout and display settings dialog    
*/
void InfoWidget::layoutSettingsDialog()
{  
    DPRINT << ": IN";
    startChanges();
    
    m_layoutManager->reloadWidgets(InfoWidgetLayoutManager::SettingsDialog); 
    QGraphicsLayout *settingDialogLayout =
            m_layoutManager->layoutSettingsDialog(); 
    
    if (settingDialogLayout) {
        HbDialog *settingsDialog = qobject_cast<HbDialog *>(
                m_layoutManager->getWidget(InfoWidgetLayoutManager::
                RoleSettingsDialog)); 

        if (settingsDialog) {
            DPRINT << ": settingsDialog has been returned from layout manager";
            initializeSettingsDialogItems();
            
            settingsDialog->setDismissPolicy(HbDialog::NoDismiss); 
            settingsDialog->setTimeout(HbDialog::NoTimeout);
            settingsDialog->open(this, 
                    SLOT(settingsDialogClosed(HbAction *))); 
            }
    }    
    DPRINT << ": OUT";
}

/*!
    InfoWidget::initializeSettingsDialogItems()
    
    Set up initial check box states 
    and connect signals to local slots  
*/
void InfoWidget::initializeSettingsDialogItems()
{  
    DPRINT << ": IN";

    // Connect display setting check boxes
    HbCheckBox *spnCheckBox = 
            qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
                    InfoWidgetLayoutManager::RoleSpnCheckBox));
    if (spnCheckBox) {
        spnCheckBox->setChecked(m_preferences->isPreferenceSet(
                InfoWidgetPreferences::DisplaySpn));
        
        QObject::connect(spnCheckBox, SIGNAL(stateChanged(int)), 
                this, SLOT(spnDisplaySettingChanged(int)), 
                Qt::UniqueConnection); 
    }
    
    HbCheckBox *mcnCheckBox = 
            qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
                    InfoWidgetLayoutManager::RoleMcnCheckBox));
    if (mcnCheckBox) {
        mcnCheckBox->setChecked(m_preferences->isPreferenceSet(
                InfoWidgetPreferences::DisplayMcn));
        
        QObject::connect(mcnCheckBox, SIGNAL(stateChanged(int)), 
                this, SLOT(mcnDisplaySettingChanged(int)), 
                Qt::UniqueConnection); 
    }
    
    HbCheckBox *satTextCheckBox = 
            qobject_cast<HbCheckBox *>(m_layoutManager->getWidget(
                    InfoWidgetLayoutManager::RoleSatTextCheckBox));
    if (satTextCheckBox) {
        satTextCheckBox->setChecked(m_preferences->isPreferenceSet(
                InfoWidgetPreferences::DisplaySatText));
        
        QObject::connect(satTextCheckBox, SIGNAL(stateChanged(int)), 
                this, SLOT(satDisplaySettingChanged(int)), 
                Qt::UniqueConnection); 
    }
    
    DPRINT << ": OUT";
}

/*!
    InfoWidget::updateInfoDisplayItem() 
    
    Fetch widget based on item role and update 
    item specific data. 
*/
void InfoWidget::updateInfoDisplayItem(
        InfoWidgetLayoutManager::LayoutItemRole itemRole, 
        QString text)
{
    DPRINT; 
    HbMarqueeItem *marqueeItem = qobject_cast<HbMarqueeItem *>(
            m_layoutManager->getWidget(itemRole));
    
    if (marqueeItem) {
        marqueeItem->setText(text);
        marqueeItem->setTextColor( HbColorScheme::color(
                "qtc_hs_list_item_title_normal"));
        
        // Update widget effective size if not already set  
        marqueeItem->adjustSize(); 
        if (!m_layoutManager->textFitsToRect(
                text,
                marqueeItem->font(), 
                marqueeItem->rect())) {
            DPRINT << ": enable marquee animation";
            m_animatingItems.append(marqueeItem);
        }
    }
}

/*!
    InfoWidget::updateInfoDisplay() 
    
    Model or visibility data has changed, 
    update info display widgets accordingly. 
*/
void InfoWidget::updateInfoDisplay()
{
    DPRINT << ": IN"; 
    
    if (m_initialized) {
        stopMarquees();
        
        if (m_layoutManager->currentDisplayRole() == 
                InfoWidgetLayoutManager::InfoDisplay )
            {
            QString text;
            InfoWidgetEngine::ModelData modelData = m_engine->modelData(); 
        
            // Update service provider name item
            text = modelData.serviceProviderName();
            updateInfoDisplayItem(InfoWidgetLayoutManager::RoleSpnMarqueeItem, text); 

            // Update MCN name item
            text = modelData.mcnName();
            updateInfoDisplayItem(InfoWidgetLayoutManager::RoleMcnMarqueeItem, text); 
            
            // Update SAT display text item
            text = modelData.satDisplayText();
            updateInfoDisplayItem(InfoWidgetLayoutManager::RoleSatMarqueeItem, text); 
        }
        
        if (m_animatingItems.count() > 0) {
            startMarquees(StartDelayed); 
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
    DWARNING << ": operation: " << operation << " error: " << errorCode; 
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
    // layout and open settings dialog
    if ((!m_dragEvent) && 
          m_layoutManager->currentDisplayRole() == 
                  InfoWidgetLayoutManager::InfoDisplay) {
        DPRINT << ": layout and display settings dialog";
        layoutSettingsDialog();
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
        m_preferences->setPreference(
                InfoWidgetPreferences::DisplaySpn, DISPLAY_SETTING_ON);
    } else {
        m_preferences->setPreference(
                InfoWidgetPreferences::DisplaySpn, DISPLAY_SETTING_OFF);
    }
}

/*!
    InfoWidget::mcnDisplaySettingChanged() 
*/
void InfoWidget::mcnDisplaySettingChanged(int state)
{
    DPRINT << ": state: " << state; 
    if (state == Qt::Checked){
        m_preferences->setPreference(
                InfoWidgetPreferences::DisplayMcn, DISPLAY_SETTING_ON);
    } else {
        m_preferences->setPreference(
                InfoWidgetPreferences::DisplayMcn, DISPLAY_SETTING_OFF);
    }
}

/*!
    InfoWidget::satDisplaySettingChanged() 
*/
void InfoWidget::satDisplaySettingChanged(int state)
{
    DPRINT << ": state: " << state; 
    if (state == Qt::Checked){
        m_preferences->setPreference(
                InfoWidgetPreferences::DisplaySatText, DISPLAY_SETTING_ON);
    } else {
        m_preferences->setPreference(
                InfoWidgetPreferences::DisplaySatText, DISPLAY_SETTING_OFF);
    }
}

/*!
    InfoWidget::mcnDisplay() 
    
    Getter function for Meta-object property "mcnDisplay"
*/
QString InfoWidget::mcnDisplay()
{
    DPRINT; 
    return m_preferences->preference(
            InfoWidgetPreferences::DisplayMcn); 
}

/*!
    InfoWidget::setMcnDisplay() 
    
    Setter function for Meta-object property "mcnDisplay"
*/
void InfoWidget::setMcnDisplay(QString value)
{
    DPRINT;
    m_preferences->setPreference(
            InfoWidgetPreferences::DisplayMcn, value);
    }

/*!
    InfoWidget::homeZoneDisplay() 
    
    Getter function for Meta-object property "homeZoneDisplay"
*/
QString InfoWidget::homeZoneDisplay()
{
    DPRINT; 
    return m_preferences->preference(
            InfoWidgetPreferences::DisplayHomeZone); 
}

/*!
    InfoWidget::setHomeZoneDisplay()
    
    Setter function for Meta-object property "homeZoneDisplay" 
*/
void InfoWidget::setHomeZoneDisplay(QString value)
{
    DPRINT; 
    m_preferences->setPreference(
            InfoWidgetPreferences::DisplayHomeZone, value);
}

/*!
    InfoWidget::activeLineDisplay() 
    
    Getter function for Meta-object property "activeLineDisplay"
*/
QString InfoWidget::activeLineDisplay()
{
    DPRINT; 
    return m_preferences->preference(
            InfoWidgetPreferences::DisplayActiveLine);
}

/*!
    InfoWidget::setActiveLineDisplay() 
    
    Setter function for Meta-object property "activeLineDisplay"
*/
void InfoWidget::setActiveLineDisplay(QString value)
{
    DPRINT; 
    m_preferences->setPreference(
            InfoWidgetPreferences::DisplayActiveLine, value);
}

/*!
    InfoWidget::satDisplay()
    
    Getter function for Meta-object property "satDisplay" 
*/
QString InfoWidget::satDisplay()
{
    DPRINT; 
    return m_preferences->preference(
            InfoWidgetPreferences::DisplaySatText);
}

/*!
    InfoWidget::setSatDisplay()
    
    Setter function for Meta-object property "satDisplay" 
*/
void InfoWidget::setSatDisplay(QString value)
{
    DPRINT;
    m_preferences->setPreference(
            InfoWidgetPreferences::DisplaySatText, value);
}

/*!
    InfoWidget::spnDisplay()
    
    Getter function for Meta-object property "spnDisplay" 
*/
QString InfoWidget::spnDisplay()
{
    DPRINT; 
    return m_preferences->preference(
            InfoWidgetPreferences::DisplaySpn);
}

/*!
    InfoWidget::setSpnDisplay()
    
    Setter function for Meta-object property "spnDisplay" 
*/
void InfoWidget::setSpnDisplay(QString value)
{
    DPRINT;
    m_preferences->setPreference(
            InfoWidgetPreferences::DisplaySpn, value);
}

/*!
    InfoWidget::readPersistentPreferences()
    
    Read Meta-object properties and store to preference handler. 
    Restores preferences from previous session.   
*/
bool InfoWidget::readPersistentPreferences()
{
    DPRINT;
    bool changed(false);

    QString propertyValue = QObject::property("homeZoneDisplay").toString();
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
        
    return changed; 
}

/*!
    InfoWidget::initializeCheckBoxStates()
    
    Read display settings from preference store 
    and set check box initial states accordingly. 
*/
void InfoWidget::initializeCheckBoxStates()
{
    DPRINT;
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
        
        // Signal HS framework to store Meta-object 
        // preferences if changed 
        if (m_preferences->storePreferences()) {
            emit setPreferences(
                    m_preferences->preferenceNames());
            }
        
        // Visible item configuration changed, reload 
        // widgets. Restores deleted items.  
        m_layoutManager->reloadWidgets(
                InfoWidgetLayoutManager::InfoDisplay);
        m_layoutManager->removeWidget(
                InfoWidgetLayoutManager::RoleSettingsDialog,
                true);
        
    } else {
        DPRINT << ": settings validation failed";
        // Cancel edit mode 
        settingsEditingCancelled();
        
        // Display warning note
        settingsValidationFailed();
    }
    
    DPRINT << ": OUT";
}

/*!
    InfoWidget::settingsEditingCancelled()
    
    Slot to be called when settings editing 
    shouldn't cause change set of visible items. 
    Restores previous state.  
*/
void InfoWidget::settingsEditingCancelled()
{
    DPRINT;
    m_preferences->restorePreferences(); 
    
    m_layoutManager->reloadWidgets(
            InfoWidgetLayoutManager::InfoDisplay);
    m_layoutManager->removeWidget(
            InfoWidgetLayoutManager::RoleSettingsDialog,
            true); 
}

/*!
    InfoWidget::settingsDialogClosed()
    
    Slot to be called when settings dialog is about to close
*/
void InfoWidget::settingsDialogClosed(HbAction* action)
{
    DPRINT << ": IN";
    if (action) {
        if (action->text() == hbTrId("txt_common_button_ok")) {
            settingsEditingFinished(); 
        } else if (action->text() == hbTrId("txt_common_button_cancel") ) {
            settingsEditingCancelled(); 
        }       
    } else {
        DPRINT << ": null action";
        settingsEditingCancelled(); 
    }
     
    // Switch to info display 
    layoutInfoDisplay();
    DPRINT << ": OUT";
}

/*!
    InfoWidget::startChanges()
*/
void InfoWidget::startChanges()
{
    DPRINT;
    m_layoutChanging = true;
    if (m_animationState != AnimationIdle) {
            stopMarquees(); 
    }
}

/*!
    InfoWidget::endChanges()
*/
void InfoWidget::endChanges()
{
    DPRINT;
    updateGeometry();
    updateInfoDisplay();
    
    m_layoutChanging = false;
}

/*!
   \reimp
*/
void InfoWidget::changeEvent(QEvent *event)
{
   DPRINT;
   if (event->type() == HbEvent::ThemeChanged) {
       DPRINT << ": HbEvent::ThemeChanged";
       updateInfoDisplay(); 
   }
   HbWidget::changeEvent(event);
}

/*!
   InfoWidget::settingsValidationFailed()
   
   Slot to be called when preference validation has failed. 
   Displays warning message box
*/
void InfoWidget::settingsValidationFailed()
{
   DPRINT;
   if (m_initialized) {
       HbMessageBox::warning(
               hbTrId("txt_operatorwidget_info_select_one"));
   }
}

/*!
   InfoWidget::startMarquees()
   
   Start marquee animations. 
   First find existing marquee items and 
   enable marquee sequence
*/
bool InfoWidget::startMarquees(AnimationStartDelay delay)
{  
    DPRINT;
    bool started(true); 
    
    if (m_animationState == AnimationOngoing || 
        m_animationState == AnimationStarting) {
        return false; 
        }
      
    int animatingItemsCount = m_animatingItems.count();  
    if (animatingItemsCount > 0) {
        HbMarqueeItem *marqueeItem(NULL);  
        foreach (marqueeItem, m_animatingItems) {
            if (animatingItemsCount > 1) {
                // Multiple items, connect to marqueeNext() 
                // sequence logic
                QObject::connect(
                        marqueeItem,SIGNAL(animationStopped()),
                        this, SLOT(marqueeNext()), 
                        Qt::UniqueConnection);
                marqueeItem->setLoopCount(1); 
            } else if (animatingItemsCount ==1 ){
                // Single item, set continuous marquee mode 
                marqueeItem->setLoopCount(-1); 
            }
        }
        
        // Store marquee sequence start item 
        m_animatingItem = m_animatingItems.first();
        
        if (delay == StartNoDelay) {
            m_animationState = AnimationOngoing; 
            m_animatingItem->startAnimation();
        } else if (delay == StartDelayed && !m_timerId) {
            m_animationState = AnimationStarting;
            m_timerId = startTimer(100);
        } 
        
    } else {
        // No animating items, not started
        DWARNING << ": not done, no animating items";
        m_animatingItem = NULL; 
        started = false;
    }
    
    return started; 
}

/*!
   InfoWidget::stopMarquees()
   
   Stop all marquee animations and reset 
   animation state
*/
void InfoWidget::stopMarquees()
{  
    DPRINT;
    if (m_animationState != AnimationIdle && 
        m_animatingItems.count() > 0) {
        HbMarqueeItem *marqueeItem(NULL);
        
        foreach (marqueeItem, m_animatingItems) {
            
            // Disconnect if more than one item, 
            // single animator doesn't connect to animationStopped() 
            if (m_animatingItems.count() > 1) {
                QObject::disconnect(
                    marqueeItem, SIGNAL(animationStopped()),
                    this, SLOT(marqueeNext()));
            }
            
            if (marqueeItem->isAnimating()) {
                marqueeItem->stopAnimation();
            }
        }
    }
    
    // Stop timer 
    if (m_timerId) {
        killTimer(m_timerId); 
        m_timerId = 0; 
    }
    
    m_animationState = AnimationIdle;
    m_animatingItems.clear();
    m_animatingItem = NULL; 
}

/*!
   InfoWidget::marqueeNext()
   
   Starts marquee animation for 
   next item in sequence.  
*/
void InfoWidget::marqueeNext()
{  
    DPRINT;
    if (m_animationState == AnimationOngoing) {
        QListIterator<HbMarqueeItem *> i(m_animatingItems);
        if (i.findNext(m_animatingItem)) {
            if (i.hasNext()) {
                m_animatingItem = i.peekNext();
            } else {
                // Was last item, loop back to first item
                i.toFront();
                m_animatingItem = i.peekNext();  
            }
        } else {
            DWARNING << ": animating item not found from list"; 
            m_animatingItem = NULL; 
        }
          
        if (m_animatingItem) {
            if (!m_animatingItem->isAnimating()) {
                m_animatingItem->setLoopCount(1); 
                m_animatingItem->startAnimation();
            }
        }
    }
    
}

// End of File. 

