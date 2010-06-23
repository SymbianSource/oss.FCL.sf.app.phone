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

#include "infowidgetlayoutmanager.h"
#include <QtGlobal>
#include <QObject>
#include <QGraphicsWidget>
#include <hbdocumentloader.h>
#include <hblabel.h>
#include <hbaction.h>
#include <hbmarqueeitem.h>
#include <hbiconitem.h> 
#include <hbpushbutton.h>
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
  InfoWidgetDocumentLoader::InfoWidgetDocumentLoader()
 */
InfoWidgetDocumentLoader::InfoWidgetDocumentLoader()
{
}

/*!
  InfoWidgetDocumentLoader::createObject()
 */
QObject *InfoWidgetDocumentLoader::createObject(
    const QString &type,
    const QString &name)
{
    DPRINT << ": IN";
    
    DPRINT << ": type: "<< type;
    DPRINT << ": name: "<< name;
    
    if ( type == HbMarqueeItem::staticMetaObject.className() ) {
        DPRINT << ": HbMarqueeItem";
        QObject *object = new HbMarqueeItem;
        object->setObjectName(name);
        DPRINT << ": HbMarqueeitem found, OUT";
        return object;
    }
    
    DPRINT << ": OUT";
    return HbDocumentLoader::createObject(type, name);
}

/*!
   InfoWidgetLayoutManager::InfoWidgetLayoutManager()
*/
InfoWidgetLayoutManager::InfoWidgetLayoutManager(QObject *parent) 
: QObject(parent), 
  m_documentLoader(NULL), 
  m_displayRole(InfoDisplay),
  m_layoutRows(0)
{
    DPRINT << ": IN"; 
    
    // Fill supported layout item roles for info display
    QList<LayoutItemRole> displayWidgetRoles = widgetRoles(InfoDisplay);
    
    // Try to load all widgets in list by widget role 
    bool loadResult = loadWidgets(InfoDisplay, 
            displayWidgetRoles,
            m_infoDisplayWidgets); 
    DPRINT << ": info display widget load result: " << loadResult;
    
    // Fill supported layout item roles for settings display
    displayWidgetRoles = widgetRoles(SettingsDialog);
  
    // Try to load all widgets in list by widget role 
    loadResult = loadWidgets(SettingsDialog, 
            displayWidgetRoles,
            m_settingsDialogWidgets); 
    DPRINT << ": settings dialog widget load result: " << loadResult;
    
    DPRINT << ": OUT";
}

/*!
   InfoWidgetLayoutManager::~InfoWidgetLayoutManager()
*/
InfoWidgetLayoutManager::~InfoWidgetLayoutManager()
{
    DPRINT << ": IN";
    
    if (m_documentLoader) { 
        delete m_documentLoader;
    }
    
    DPRINT << ": OUT";
}

/*!
   InfoWidgetLayoutManager::destroyWidgets()
*/
void InfoWidgetLayoutManager::destroyWidgets()
{
    DPRINT << ": IN";
    
    // Destroy parent items 
    removeWidget(RoleContent); 
    removeWidget(RoleSettingsDialog); 

    DPRINT << ": OUT";    
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
   InfoWidgetLayoutManager::layoutRows()
*/
int InfoWidgetLayoutManager::layoutRows() const 
{
    DPRINT; 
    return m_layoutRows;
} 

/*!
   InfoWidgetLayoutManager::setLayoutRows()
*/
void InfoWidgetLayoutManager::setLayoutRows(int rows) 
{
    DPRINT; 
    m_layoutRows = rows; 
} 

/*!
   InfoWidgetLayoutManager::layoutRowHeight()
   
   Read size from style, currently graphics icon size used
   as it defines row height in layout. Real font height 
   and layout spacing could be used instead. 
*/
qreal InfoWidgetLayoutManager::layoutRowHeight() const
{
    DPRINT; 
    HbStyle style; 
    qreal rowHeight;

    bool ok = style.parameter("hb-param-graphic-size-primary-small", 
            rowHeight);
    if (!ok) {
        DWARNING << ": Paremeters reading failed!! Using default";
        rowHeight = 26.8;
    }
    
    DPRINT << ": rowHeight: " << rowHeight;
    return rowHeight; 
}

/*!
   InfoWidgetLayoutManager::textFitsToRect()
   
   Check if text fits to given rect width. Return true also if 
   text width is null, or text width AND rect width is null.   
*/
bool InfoWidgetLayoutManager::textFitsToRect(QString text, 
        QFont font, QRectF rect)
{
    bool fits(true);
    if (!rect.isEmpty()) { 
        QFontMetricsF metrics(font);
        qreal width = metrics.boundingRect(text).width();
        if (width > rect.width() ) {
            fits = false; 
        }
    }
    
    DPRINT << ": fits: " << fits;
    return fits; 
}

/*!
   InfoWidgetLayoutManager::contentWidget()
*/
QGraphicsWidget* InfoWidgetLayoutManager::contentWidget()
{
    DPRINT; 
    return getWidget(RoleContent); 
}

/*!
   InfoWidgetLayoutManager::marqueeItems()
*/
QList<HbMarqueeItem *> InfoWidgetLayoutManager::marqueeItems() 
{
    DPRINT;
    QList<HbMarqueeItem *> items;
    
    QList<LayoutItemRole> marqueeItemRoles; 
    marqueeItemRoles.append(RoleSpnMarqueeItem);
    marqueeItemRoles.append(RoleMcnMarqueeItem);
    marqueeItemRoles.append(RoleSatMarqueeItem);
    
    LayoutItemRole role;
    HbMarqueeItem *item(NULL);
    QGraphicsWidget *widget(NULL); 
    foreach (role, marqueeItemRoles) {
        widget = getWidget(role); 
        if (widget) {
            item = qobject_cast<HbMarqueeItem*>(widget); 
            if (item) {
                items.append(item); 
                item = NULL; 
            }
        }
    }
    DPRINT << ": count of marquee items: " << items.count(); 
    return items; 
}

/*!
   InfoWidgetLayoutManager::getWidget();
*/
QGraphicsWidget* InfoWidgetLayoutManager::getWidget(LayoutItemRole itemRole)
{
    DPRINT << ": item role: " << itemRole;
    
    QGraphicsWidget *widget = m_widgets.value(itemRole); 
    if (widget) {
        DPRINT << ": widget: " << widget;
    }
    
    return widget; 
}

/*!
   InfoWidgetLayoutManager::getObject();
*/
QObject* InfoWidgetLayoutManager::getObject(LayoutItemRole itemRole)
{
    DPRINT << ": item role: " << itemRole;
    
    QObject *object = m_objects.value(itemRole); 
    if (object) {
        DPRINT << ": object: " << object;
    }
    
    return object; 
}

/*!
   InfoWidgetLayoutManager::removeWidget();
*/
void InfoWidgetLayoutManager::removeWidget(LayoutItemRole itemRole, 
        bool deleteLater)
{
    DPRINT << ": item role: " << itemRole;
    
    QGraphicsWidget *widget = m_widgets.value(itemRole); 
    if (widget) {
        DPRINT << ": removing widget: " << widget;        
        m_widgets.remove(itemRole);
        if (!deleteLater) {
            delete widget;
        } else {
            widget->deleteLater(); 
        }
    }
    
    widget = m_infoDisplayWidgets.value(itemRole); 
    if (widget) {
        DPRINT << ": removing widget from m_infoDisplayWidgets";        
        m_infoDisplayWidgets.remove(itemRole);
    }
    
    widget = m_settingsDialogWidgets.value(itemRole); 
    if (widget) {
        DPRINT << ": removing widget from m_settingsDialogWidgets";         
        m_settingsDialogWidgets.remove(itemRole);
    }
}

/*!
    InfoWidgetLayoutManager::layoutInfoDisplay()
*/
QGraphicsLayout* InfoWidgetLayoutManager::layoutInfoDisplay()
{   
    DPRINT << ": IN";

    m_displayRole = InfoDisplay;
    m_widgets = m_infoDisplayWidgets; 
           
    QGraphicsLayout *activeLayout(NULL); 
    DPRINT << ": getting content item and using its layout for activeLayout";
    QGraphicsWidget *content = getWidget(RoleContent); 
    if (content) {
        DPRINT << ": content found, getting layout";
        activeLayout = content->layout(); 
    }

    DPRINT  << ": OUT";
    return activeLayout; 
}

/*!
    InfoWidgetLayoutManager::layoutSettingsDialog()
*/
QGraphicsLayout* InfoWidgetLayoutManager::layoutSettingsDialog()
{   
    DPRINT << ": IN";
    
    m_displayRole = SettingsDialog;
    m_widgets = m_settingsDialogWidgets; 
    
    QGraphicsLayout *activeLayout(NULL); 
    DPRINT << ": getting settingsDialog item";
    QGraphicsWidget *dialog = getWidget(RoleSettingsDialog); 
    if (dialog) {
        DPRINT << ": dialog found, getting layout";
        activeLayout = dialog->layout(); 
        
        HbAction *okAction = qobject_cast<HbAction *>(getObject(RoleOkAction));
        if (okAction) {
            dialog->addAction(okAction); 
        }
        
        HbAction *cancelAction = qobject_cast<HbAction *>(getObject(RoleCancelAction));
        if (cancelAction) {
            dialog->addAction(cancelAction);
        }
    }

    DPRINT  << ": OUT";
    return activeLayout;    
}

/*!
    InfoWidgetLayoutManager::loadWidgets()
*/
bool InfoWidgetLayoutManager::loadWidgets(const DisplayRole displayRole, 
        const QList<LayoutItemRole> &displayWidgets,
        QMap<LayoutItemRole, QGraphicsWidget *> &widgetMap)
{
    DPRINT << ": IN";
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
    
    QGraphicsWidget *widget(NULL);
    LayoutItemRole currentWidgetRole;
    bool allWidgetsLoaded(true); 
    
    foreach (currentWidgetRole, displayWidgets) {
        widget = loadWidget(*m_documentLoader, displayRole, currentWidgetRole);
           if (widget) {
               widgetMap.insert(currentWidgetRole, widget);
               widget = NULL;
           } else {
               allWidgetsLoaded = false; 
               DWARNING << ": widget not found!";
           }
    }
    
    DPRINT << ": allWidgetsLoaded: " << allWidgetsLoaded;
            
    int widgetCount = widgetMap.count(); 
    if (widgetCount == displayWidgets.count()) {
        loadResult = true;
        } else {
            DWARNING << ": all widgets were not loaded!";
            loadResult = false;
        }        
    
    if (displayRole == SettingsDialog) {
        m_objects.clear();
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
    
    DPRINT << ": OUT";
    return loadResult; 
}

/*!
    InfoWidgetLayoutManager::reloadWidgets()
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
    InfoWidgetLayoutManager::loadWidget()
    
    Initialize loader with corresponding document file 
    before calling this single widget loader utility   
*/
QGraphicsWidget* InfoWidgetLayoutManager::loadWidget(InfoWidgetDocumentLoader &loader, 
        DisplayRole displayRole, 
        LayoutItemRole widgetRole)
{
    DPRINT << ": IN";
     
    QString widgetPrefix; 
    if (displayRole == InfoDisplay) {
        widgetPrefix = LAYOUT_PREFIX_INFO_DISPLAY;
    } else if (displayRole == SettingsDialog) {
        widgetPrefix = LAYOUT_PREFIX_SETTINGS_DIALOG;
    }
        
    QGraphicsWidget *widget(NULL);
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
    
    DPRINT << ": widget name: " << widgetName;
    widget = qobject_cast<QGraphicsWidget *>(loader.findWidget(widgetName));
    
    if (widget) {
        DPRINT << ": widget found: " << widgetName;
    } else {
        DPRINT << ": ERROR, widget not found!";
    }
           
    DPRINT << ": OUT";
    return widget; 
}

/*!
    InfoWidgetLayoutManager::loadObject()
    
    Initialize loader with corresponding document 
    file before calling this single object loader utility   
*/
QObject* InfoWidgetLayoutManager::loadObject(InfoWidgetDocumentLoader &loader, 
        DisplayRole displayRole, 
        LayoutItemRole objectRole)
{
    DPRINT << ": IN";
     
    QString objectPrefix; 
    if (displayRole == InfoDisplay) {
        objectPrefix = LAYOUT_PREFIX_INFO_DISPLAY;
    } else if (displayRole == SettingsDialog) {
        objectPrefix = LAYOUT_PREFIX_SETTINGS_DIALOG;
    }
        
    QObject *object(NULL);
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
    
    DPRINT << ": object name: " << objectName;
    object = qobject_cast<QObject *>(loader.findObject(objectName));
    
    if (object) {
        DPRINT << ": object found: " << objectName;
    } else {
        DPRINT << ": ERROR, object not found!";
    }
           
    DPRINT << ": OUT";
    return object; 
}


/*!
    InfoWidgetLayoutManager::widgetRoles()
    
    Returns supported widget roles for specific display
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
    
    DPRINT << ": widgetRoles.count() : " << widgetRoles.count();
    return widgetRoles; 
}    
    
// End of File. 


