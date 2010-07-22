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

#include <QtGlobal>
#include <QObject>
#include <QGraphicsWidget>
#include <hbdocumentloader.h>
#include <hblabel.h>
#include <hbaction.h>
#include <hbmarqueeitem.h>
#include <hbiconitem.h> 
#include <hbpushbutton.h>
#include <hbinstance.h>
#include "infowidgetlayoutmanager.h"
#include "infowidgetlogging.h"

/*!
  \class InfoWidgetDocumentLoader
  \brief Custom document loader for Operator info widget  

   Derived from HbDocumentLoader.
    
*/

/*!
  \class InfoWidgetLayoutManager
  \brief Layout manager class for Operator info widget.   

   Handles layout document loading and accessing the loaded 
   widgets.   
    
*/

// Local constants 
const char INFOWIDGET_DOCML_FILE[] = ":/resource/infowidget.docml";
const char SETTINGS_DIALOG_DOCML_FILE[] = ":/resource/settingsdialog.docml";

const char LAYOUT_PREFIX_INFO_DISPLAY[] = "id:";
const char LAYOUT_PREFIX_SETTINGS_DIALOG[] = "sd:";
const char LAYOUT_NAME_CONTENT[] = "content";
const char LAYOUT_NAME_MCNMARQUEEITEM[] = "mcnMarqueeItem";
const char LAYOUT_NAME_SPNMARQUEEITEM[] = "spnMarqueeItem";
const char LAYOUT_NAME_SATMARQUEEITEM[] = "satMarqueeItem";
const char LAYOUT_NAME_SPNICON[] = "spnIcon";
const char LAYOUT_NAME_MCNICON[] = "mcnIcon";
const char LAYOUT_NAME_SATTEXTICON[] = "satTextIcon";
const char LAYOUT_NAME_SPNCHECKBOX[] = "spnCheckBox";
const char LAYOUT_NAME_MCNCHECKBOX[] = "mcnCheckBox";
const char LAYOUT_NAME_SATTEXTCHECKBOX[] = "satTextCheckBox";
const char LAYOUT_NAME_OKACTION[] = "okAction";
const char LAYOUT_NAME_CANCELACTION[] = "cancelAction";
const char LAYOUT_NAME_SETTINGSDIALOG[] = "settingsDialog"; 
const char LAYOUT_NAME_SETTINGSCONTAINER[] = "settingsContainer";
const char LAYOUT_NAME_CONTAINER[] = "container";

/*!
  Create object from document. 
 */
QObject *InfoWidgetDocumentLoader::createObject(
    const QString &type,
    const QString &name)
{
    DPRINT;
    if ( type == HbMarqueeItem::staticMetaObject.className() ) {
        QObject *object = new HbMarqueeItem;
        object->setObjectName(name);
        return object;
    }
    return HbDocumentLoader::createObject(type, name);
}

/*!
   Constructor. 
*/
InfoWidgetLayoutManager::InfoWidgetLayoutManager(QObject *parent) 
: QObject(parent), 
  m_documentLoader(NULL), 
  m_displayRole(InfoDisplay),
  m_layoutRows(0),
  m_cachedLayoutRowHeight(0.0)
{
    DPRINT; 
    // Fill supported layout item roles for info display
    QList<LayoutItemRole> displayWidgetRoles = widgetRoles(InfoDisplay);
    
    // Try to load all widgets in list by widget role 
    bool loadResult = loadWidgets(InfoDisplay, 
            displayWidgetRoles,
            m_infoDisplayWidgets); 
    
    // Fill supported layout item roles for settings display
    displayWidgetRoles = widgetRoles(SettingsDialog);

    // Try to load all widgets in list by widget role 
    loadResult = loadWidgets(SettingsDialog, 
            displayWidgetRoles,
            m_settingsDialogWidgets); 
}

/*!
   Destructor. 
*/
InfoWidgetLayoutManager::~InfoWidgetLayoutManager()
{
    DPRINT;
    if (m_documentLoader) { 
        delete m_documentLoader;
    }
}

/*!
   Destroy all widgets. 
   Deletes parent widgets of each display 
   causing deletion of child items. 
*/
void InfoWidgetLayoutManager::destroyWidgets()
{
    DPRINT;
    // Destroy parent items 
    removeWidget(RoleContent); 
    removeWidget(RoleSettingsDialog); 
}

/*!
   InfoWidgetLayoutManager::currentDisplayRole()
*/
InfoWidgetLayoutManager::DisplayRole InfoWidgetLayoutManager::currentDisplayRole() 
{
    DPRINT; 
    return m_displayRole;
} 

/*!
   InfoWidgetLayoutManager::currentWidgetRoles()
*/
QList<InfoWidgetLayoutManager::LayoutItemRole> InfoWidgetLayoutManager::currentWidgetRoles() 
{
    DPRINT; 
    return m_widgets.keys(); 
} 

/*!
   Returns count of layout rows. 
*/
int InfoWidgetLayoutManager::layoutRows() const 
{
    DPRINT; 
    return m_layoutRows;
} 

/*!
   Set count of layout rows.  
*/
void InfoWidgetLayoutManager::setLayoutRows(int rows) 
{
    DPRINT; 
    m_layoutRows = rows; 
} 

/*!
   Read row height from style. 
*/
qreal InfoWidgetLayoutManager::layoutRowHeight()
{
    DPRINT;
    // Read from style only if not already initialized
    if (m_cachedLayoutRowHeight == 0.0) { 
        bool ok = hbInstance->style()->parameter("hb-param-graphic-size-primary-small", 
                m_cachedLayoutRowHeight);
            DPRINT << ": row height from style: " << m_cachedLayoutRowHeight;
        if (!ok) {
            DWARNING << ": Error, paremeters reading failed!!";
        }
    }
    return m_cachedLayoutRowHeight;
}

/*!
   Check if text fits to given rect width.  
*/
bool InfoWidgetLayoutManager::textFitsToRect(QString text, 
        QFont font, QRectF rect) const 
{
    bool fits(true);
    if (!rect.isEmpty()) { 
        QFontMetricsF metrics(font);
        qreal width = metrics.boundingRect(text).width();
        if (width > rect.width() ) {
            fits = false; 
        }
    }
    return fits; 
}

/*!
   Returns content widget.  
   The content widget is layout main widget and parent for 
   sub-widgets in current display.     
*/
QGraphicsWidget* InfoWidgetLayoutManager::contentWidget()
{
    DPRINT; 
    return getWidget(RoleContent); 
}

/*!
   Returns list of marquee items.  
*/
QList<HbMarqueeItem *> InfoWidgetLayoutManager::marqueeItems() 
{
    DPRINT;
    QList<HbMarqueeItem *> items;
    
    QList<LayoutItemRole> marqueeItemRoles; 
    marqueeItemRoles.append(RoleSpnMarqueeItem);
    marqueeItemRoles.append(RoleMcnMarqueeItem);
    marqueeItemRoles.append(RoleSatMarqueeItem);
    
    foreach (LayoutItemRole role, marqueeItemRoles) {
        QGraphicsWidget *widget = getWidget(role); 
        if (widget) {
            HbMarqueeItem *item = 
                    qobject_cast<HbMarqueeItem*>(widget); 
            if (item) {
                items.append(item); 
                item = NULL; 
            }
        }
    }
    return items; 
}

/*!
   Get widget with given item role. 
*/
QGraphicsWidget* InfoWidgetLayoutManager::getWidget(LayoutItemRole itemRole)
{
    QGraphicsWidget *widget = m_widgets.value(itemRole); 
    return widget; 
}

/*!
   Get object with given item role. 
*/
QObject* InfoWidgetLayoutManager::getObject(LayoutItemRole itemRole)
{
    QObject *object = m_objects.value(itemRole); 
    return object; 
}

/*!
   Remove widget with given item role. 
*/
void InfoWidgetLayoutManager::removeWidget(LayoutItemRole itemRole, 
        bool deleteLater)
{
    DPRINT;
    QGraphicsWidget *widget = m_widgets.value(itemRole); 
    if (widget) {
        if (!deleteLater) {
            delete widget;
        } else {
            widget->deleteLater(); 
        }
    }
    
    m_widgets.remove(itemRole);
    m_infoDisplayWidgets.remove(itemRole);
    m_settingsDialogWidgets.remove(itemRole);
}

/*!
    Returns info display layout.  
*/
QGraphicsLayout* InfoWidgetLayoutManager::layoutInfoDisplay()
{   
    DPRINT;
    m_displayRole = InfoDisplay;
    m_widgets = m_infoDisplayWidgets; 
           
    QGraphicsLayout *activeLayout(NULL); 
    QGraphicsWidget *content = getWidget(RoleContent); 
    if (content) {
        DPRINT << ": content found, getting layout";
        activeLayout = content->layout(); 
    }
    return activeLayout; 
}

/*!
    Returns settings dialog layout. 
*/
QGraphicsLayout* InfoWidgetLayoutManager::layoutSettingsDialog()
{   
    DPRINT;
    m_displayRole = SettingsDialog;
    m_widgets = m_settingsDialogWidgets; 
    
    QGraphicsLayout *activeLayout(NULL); 
    QGraphicsWidget *dialog = getWidget(RoleSettingsDialog); 
    if (dialog) {
        activeLayout = dialog->layout(); 

        HbAction *okAction = qobject_cast<HbAction *>(
                getObject(RoleOkAction));
        if (okAction) {
            dialog->addAction(okAction); 
        }
        
        HbAction *cancelAction = qobject_cast<HbAction *>(
                getObject(RoleCancelAction));
        if (cancelAction) {
            dialog->addAction(cancelAction);
        }
    }
    return activeLayout;    
}

/*!
    Load widgets from document for given display role.
*/
bool InfoWidgetLayoutManager::loadWidgets(const DisplayRole displayRole, 
        const QList<LayoutItemRole> &displayWidgets,
        QMap<LayoutItemRole, QGraphicsWidget *> &widgetMap)
{
    DPRINT;
    bool loadResult(true); 

    // Cleanup previously loaded content in case of any data  
    widgetMap.clear(); 
    
    if (!m_documentLoader) {
        m_documentLoader = new InfoWidgetDocumentLoader;
    }
    Q_ASSERT(m_documentLoader); 
    
    bool loaded = true;
    if (displayRole != SettingsDialog) {
        m_documentLoader->load(INFOWIDGET_DOCML_FILE, &loaded);
    } else {
        m_documentLoader->load(SETTINGS_DIALOG_DOCML_FILE, &loaded);
    }
    
    Q_ASSERT_X(loaded, 
            "InfoWidgetLayoutManager", 
            "Invalid docml file");    
    
    
    foreach (LayoutItemRole role, displayWidgets) {
        QGraphicsWidget *widget = 
                loadWidget(*m_documentLoader, displayRole, role);
           if (widget) {
               widgetMap.insert(role, widget);
               widget = NULL;
           } 
    }
    
    if (widgetMap.count() == displayWidgets.count()) {
        loadResult = true;
        } else {
            DWARNING << ": all widgets were not loaded!";
            loadResult = false;
        }        

    m_objects.clear();
    if (displayRole == SettingsDialog) {
        QObject *okAction = 
                loadObject(*m_documentLoader, 
                        displayRole, 
                        RoleOkAction);
                m_objects.insert(RoleOkAction, okAction); 
        QObject *cancelAction = 
                loadObject(*m_documentLoader, 
                        displayRole, 
                        RoleCancelAction); 
                m_objects.insert(RoleCancelAction, cancelAction); 
    } 

    return loadResult; 
}

/*!
    Restores widgets from layout document. 
    Called when layout items have been deleted  
    and items should be shown again. 
*/
bool InfoWidgetLayoutManager::reloadWidgets(const DisplayRole displayRole)
{
    QList<LayoutItemRole> displayWidgetRoles = widgetRoles(displayRole); 
    bool loadResult(false); 
    
    switch (displayRole) {
        case InfoDisplay:
            loadResult = loadWidgets(displayRole, 
                        displayWidgetRoles,
                        m_infoDisplayWidgets); 
            break; 
        case SettingsDialog:  
            loadResult = loadWidgets(displayRole, 
                        displayWidgetRoles,
                        m_settingsDialogWidgets); 
            break;
        default: 
            break; 
    }
    return loadResult; 
}

/*!
    Loads widget by given widget role id.
*/
QGraphicsWidget* InfoWidgetLayoutManager::loadWidget(InfoWidgetDocumentLoader &loader, 
        DisplayRole displayRole, 
        LayoutItemRole widgetRole)
{
    DPRINT;
    QString widgetPrefix; 
    if (displayRole == InfoDisplay) {
        widgetPrefix = LAYOUT_PREFIX_INFO_DISPLAY;
    } else if (displayRole == SettingsDialog) {
        widgetPrefix = LAYOUT_PREFIX_SETTINGS_DIALOG;
    }
        
    QString widgetName = widgetPrefix;
    switch (widgetRole) 
        {
        case RoleContent: 
            widgetName.append(LAYOUT_NAME_CONTENT);
        break;     
        case RoleMcnMarqueeItem: 
            widgetName.append(LAYOUT_NAME_MCNMARQUEEITEM);
        break; 
        case RoleSpnMarqueeItem: 
            widgetName.append(LAYOUT_NAME_SPNMARQUEEITEM);
        break;
        case RoleSatMarqueeItem: 
            widgetName.append(LAYOUT_NAME_SATMARQUEEITEM);
        break; 
        case RoleSpnIcon: 
            widgetName.append(LAYOUT_NAME_SPNICON);
        break; 
        case RoleMcnIcon: 
            widgetName.append(LAYOUT_NAME_MCNICON);
        break; 
        case RoleSatTextIcon: 
            widgetName.append(LAYOUT_NAME_SATTEXTICON);
        break;
        case RoleSpnCheckBox: 
            widgetName.append(LAYOUT_NAME_SPNCHECKBOX);
        break; 
        case RoleMcnCheckBox: 
            widgetName.append(LAYOUT_NAME_MCNCHECKBOX);
        break; 
        case RoleSatTextCheckBox: 
            widgetName.append(LAYOUT_NAME_SATTEXTCHECKBOX);
        break; 
        case RoleSettingsDialog: 
            widgetName.append(LAYOUT_NAME_SETTINGSDIALOG);
        break;
        case RoleContainer: 
            widgetName.append(LAYOUT_NAME_CONTAINER);
        break;        
        case RoleSettingsContainer: 
            widgetName.append(LAYOUT_NAME_SETTINGSCONTAINER);
        break; 
        
        case RoleUndefined: // Fall through 
        default: 
            break; 
        }
    
    QGraphicsWidget *widget = qobject_cast<QGraphicsWidget *>(
            loader.findWidget(widgetName));
    return widget; 
}

/*!
    Loads object by given object role id. 
*/
QObject* InfoWidgetLayoutManager::loadObject(InfoWidgetDocumentLoader &loader, 
        DisplayRole displayRole, 
        LayoutItemRole objectRole)
{
    DPRINT;
    QString objectPrefix; 
    if (displayRole == InfoDisplay) {
        objectPrefix = LAYOUT_PREFIX_INFO_DISPLAY;
    } else if (displayRole == SettingsDialog) {
        objectPrefix = LAYOUT_PREFIX_SETTINGS_DIALOG;
    }
        
    QString objectName = objectPrefix;
    switch (objectRole) 
        {
        case RoleOkAction: 
            objectName.append(LAYOUT_NAME_OKACTION);
        break;
        case RoleCancelAction: 
            objectName.append(LAYOUT_NAME_CANCELACTION);
        break;

        case RoleUndefined: // Fall through 
        default: 
            break; 
        }

    QObject *object = qobject_cast<QObject *>(loader.findObject(objectName));
    if (!object) {
        DWARNING << ": ERROR, object not found!";
    }
           
    return object; 
}

/*!
    Returns supported widget roles for specific display.
*/
const QList<InfoWidgetLayoutManager::LayoutItemRole> InfoWidgetLayoutManager::widgetRoles(
        DisplayRole displayRole) const
{
    QList<LayoutItemRole> widgetRoles; 
    switch (displayRole) {
        case InfoDisplay: 
            // Fill supported layout item roles for info display
            widgetRoles.append(RoleContent);
            widgetRoles.append(RoleSpnIcon);
            widgetRoles.append(RoleSpnMarqueeItem);
            widgetRoles.append(RoleMcnIcon);
            widgetRoles.append(RoleMcnMarqueeItem);
            widgetRoles.append(RoleSatTextIcon);
            widgetRoles.append(RoleSatMarqueeItem);
            break;
        case SettingsDialog: 
            // Fill supported layout item roles for settings display
            widgetRoles.append(RoleSettingsDialog);
            widgetRoles.append(RoleSettingsContainer);
            widgetRoles.append(RoleSpnCheckBox);
            widgetRoles.append(RoleMcnCheckBox);
            widgetRoles.append(RoleSatTextCheckBox);
            break;
            
        default: 
            break; 
    }
    
    return widgetRoles; 
}    
    
// End of File. 


