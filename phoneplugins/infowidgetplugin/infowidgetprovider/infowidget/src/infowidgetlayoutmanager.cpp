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
#include <hbmarqueeitem.h>
#include <hbiconitem.h> 
#include <hbpushbutton.h>
#include "infowidgetlogging.h"


const char INFOWIDGET_DOCML_FILE[] = ":/resource/infowidget.docml";

// Docml widget name mappings 
const char LAYOUT_PREFIX_INFO_DISPLAY[] = "id:";
const char LAYOUT_PREFIX_SETTINGS_DISPLAY[] = "sd:";
const char LAYOUT_NAME_CONTENT[] = "content";
const char LAYOUT_NAME_MCNMARQUEEITEM[] = "mcnMarqueeItem";
const char LAYOUT_NAME_MCNLABEL[] = "mcnLabel";
const char LAYOUT_NAME_HOMEZONELABEL[] = "homeZoneLabel";
const char LAYOUT_NAME_ACTIVELINELABEL[] = "activeLineLabel";
const char LAYOUT_NAME_SATTEXTLABEL[] = "satTextLabel";
const char LAYOUT_NAME_HOMEZONEICON[] = "homeZoneIcon";
const char LAYOUT_NAME_MCNICON[] = "mcnIcon";
const char LAYOUT_NAME_ACTIVELINEICON[] = "activeLineIcon";
const char LAYOUT_NAME_SATTEXTICON[] = "satTextIcon";
const char LAYOUT_NAME_HOMEZONECHECKBOX[] = "homeZoneCheckBox";
const char LAYOUT_NAME_MCNCHECKBOX[] = "mcnCheckBox";
const char LAYOUT_NAME_ACTIVELINECHECKBOX[] = "activeLineCheckBox";
const char LAYOUT_NAME_SATTEXTCHECKBOX[] = "satTextCheckBox";
const char LAYOUT_NAME_OKBUTTON[] = "okButton";
const char LAYOUT_NAME_CONTAINER[] = "container";
const char LAYOUT_NAME_SETTINGSCONTAINER[] = "settingsContainer";


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
    displayWidgetRoles = widgetRoles(SettingsDisplay);
  
    // Try to load all widgets in list by widget role 
    loadResult = loadWidgets(SettingsDisplay, 
            displayWidgetRoles,
            m_settingsDisplayWidgets); 
    DPRINT << ": settings display widget load result: " << loadResult;

    DPRINT << ": OUT";
}

/*!
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
 */
InfoWidgetLayoutManager::DisplayRole InfoWidgetLayoutManager::currentDisplayRole() 
{
    DPRINT; 
    return m_displayRole;
} 

/*!
 */
QList<InfoWidgetLayoutManager::LayoutItemRole> InfoWidgetLayoutManager::currentWidgetRoles() 
{
    DPRINT; 
    return m_widgets.keys(); 
} 

int InfoWidgetLayoutManager::layoutRows() const 
{
    DPRINT; 
    return m_layoutRows;
} 

void InfoWidgetLayoutManager::setLayoutRows(int rows) 
{
    DPRINT; 
    m_layoutRows = rows; 
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
   InfoWidgetLayoutManager::hideWidget();
*/
void InfoWidgetLayoutManager::hideWidget(LayoutItemRole itemRole)
{
    DPRINT << ": item role: " << itemRole;
    
    QGraphicsWidget *widget = m_widgets.value(itemRole); 
    if (widget) {
        DPRINT << ": hiding widget: " << widget;
        widget->hide(); 
    }
}

/*!
   InfoWidgetLayoutManager::hideAll();
*/
void InfoWidgetLayoutManager::hideAll()
{
    DPRINT << ": IN";
    
    QMapIterator<LayoutItemRole, QGraphicsWidget *> iterator(m_widgets);
    while (iterator.hasNext()) {
        iterator.next();
        QGraphicsWidget *widget = iterator.value(); 
        if (widget) {
            widget->hide(); 
        }
    }
    
    DPRINT << ": OUT";
}


/*!
   InfoWidgetLayoutManager::showAll();
*/
void InfoWidgetLayoutManager::showAll()
{
    DPRINT << ": IN";
    
    QMapIterator<LayoutItemRole, QGraphicsWidget *> iterator(m_widgets);
    while (iterator.hasNext()) {
        iterator.next();
        QGraphicsWidget *widget = iterator.value(); 
        if (widget) {
            widget->show(); 
        }
    }
    
    DPRINT << ": OUT";
}

/*!
    InfoWidgetLayoutManager::layoutInfoDisplay()
*/
QGraphicsLayout* InfoWidgetLayoutManager::layoutInfoDisplay()
{   
    DPRINT << ": IN";

    hideAll(); 

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
    InfoWidgetLayoutManager::layoutSettingsDisplay()
*/
QGraphicsLayout* InfoWidgetLayoutManager::layoutSettingsDisplay()
{   
    DPRINT << ": IN";
    
    hideAll(); 
    
    m_displayRole = SettingsDisplay;
    m_widgets = m_settingsDisplayWidgets; 
    
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
    
    bool loaded = false;
    m_documentLoader->load(INFOWIDGET_DOCML_FILE, &loaded);
        
    if (!loaded) {
        qWarning() << "Unable to load .docml:  " << INFOWIDGET_DOCML_FILE;
    }
    else {
        DPRINT << ": document " << INFOWIDGET_DOCML_FILE << " loaded successfully"; 
        
        QGraphicsWidget *widget(NULL);
        LayoutItemRole currentWidgetRole;
        bool allWidgetsLoaded(true); 
        
        foreach (currentWidgetRole, displayWidgets) {
            DPRINT << ": iterating displayWidgets, current role: " << static_cast<int>(currentWidgetRole);
            
            widget = loadWidget(*m_documentLoader, displayRole, currentWidgetRole);
               if (widget) {
                   DPRINT << ": widget found, inserting to widget map";
                   widgetMap.insert(currentWidgetRole, widget);
                   widget = NULL;
               } else {
                   allWidgetsLoaded = false; 
                   DPRINT << ": ERROR, widget not found!";
               }
        }
        
        DPRINT << ": allWidgetsLoaded: " << allWidgetsLoaded;
                
        int widgetCount = widgetMap.count(); 
        DPRINT << ": loaded widgets count: " << widgetCount;
        if (widgetCount == displayWidgets.count()) {
            DPRINT << ": all widgets loaded";
            loadResult = true;
            } else {
                DPRINT << ": error, all widgets were not loaded";
                loadResult = false;
            }        
    }
        
    DPRINT << ": OUT";
    return loadResult; 
}

/*!
    InfoWidgetLayoutManager::loadWidget()
    
    Initialize loader with corresponding document file before calling this single widget loader utility   
*/
QGraphicsWidget* InfoWidgetLayoutManager::loadWidget(InfoWidgetDocumentLoader &loader, 
        DisplayRole displayRole, 
        LayoutItemRole widgetRole)
{
    DPRINT << ": IN";
     
    QString widgetPrefix; 
    if (displayRole == InfoDisplay) {
        widgetPrefix = LAYOUT_PREFIX_INFO_DISPLAY;
    } else if (displayRole == SettingsDisplay) {
        widgetPrefix = LAYOUT_PREFIX_SETTINGS_DISPLAY;
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
        case RoleHomeZoneLabel: 
            widgetName.append(LAYOUT_NAME_HOMEZONELABEL);
        break;
        case RoleMcnLabel: 
            widgetName.append(LAYOUT_NAME_MCNLABEL);
        break;
        case RoleActiveLineLabel: 
            widgetName.append(LAYOUT_NAME_ACTIVELINELABEL);
        break; 
        case RoleSatTextLabel: 
            widgetName.append(LAYOUT_NAME_SATTEXTLABEL);
        break; 
        case RoleHomeZoneIcon: 
            widgetName.append(LAYOUT_NAME_HOMEZONEICON);
        break; 
        case RoleMcnIcon: 
            widgetName.append(LAYOUT_NAME_MCNICON);
        break; 
        case RoleActiveLineIcon: 
            widgetName.append(LAYOUT_NAME_ACTIVELINEICON);
        break; 
        case RoleSatTextIcon: 
            widgetName.append(LAYOUT_NAME_SATTEXTICON);
        break;
        case RoleHomeZoneCheckBox: 
            widgetName.append(LAYOUT_NAME_HOMEZONECHECKBOX);
        break; 
        case RoleMcnCheckBox: 
            widgetName.append(LAYOUT_NAME_MCNCHECKBOX);
        break; 
        case RoleActiveLineCheckBox: 
            widgetName.append(LAYOUT_NAME_ACTIVELINECHECKBOX);
        break; 
        case RoleSatTextCheckBox: 
            widgetName.append(LAYOUT_NAME_SATTEXTCHECKBOX);
        break; 
        case RoleOkButton: 
            widgetName.append(LAYOUT_NAME_OKBUTTON);
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
    InfoWidgetLayoutManager::widgetRoles()
    
    Returns supported widget roles for specific display
*/
const QList<InfoWidgetLayoutManager::LayoutItemRole> InfoWidgetLayoutManager::widgetRoles(
        DisplayRole displayRole) const
{
    DPRINT << ": IN";
    
    QList<LayoutItemRole> widgetRoles; 
    
    switch (displayRole) {
        case SettingsDisplay: 
            // Fill supported layout item roles for settings display
            widgetRoles.append(RoleContent); 
            widgetRoles.append(RoleHomeZoneLabel);
            widgetRoles.append(RoleMcnLabel); 
            widgetRoles.append(RoleActiveLineLabel);
            widgetRoles.append(RoleSatTextLabel);
            widgetRoles.append(RoleHomeZoneIcon);
            widgetRoles.append(RoleMcnIcon);
            widgetRoles.append(RoleActiveLineIcon);
            widgetRoles.append(RoleSatTextIcon);
            widgetRoles.append(RoleHomeZoneCheckBox);
            widgetRoles.append(RoleMcnCheckBox);
            widgetRoles.append(RoleActiveLineCheckBox);
            widgetRoles.append(RoleSatTextCheckBox);
            widgetRoles.append(RoleOkButton);
            widgetRoles.append(RoleSettingsContainer); 
            break;
        case InfoDisplay: 
            // Fill supported layout item roles for info display
            widgetRoles.append(RoleContent); 
            widgetRoles.append(RoleHomeZoneLabel);
            widgetRoles.append(RoleMcnMarqueeItem);
            widgetRoles.append(RoleActiveLineLabel);
            widgetRoles.append(RoleSatTextLabel);
            widgetRoles.append(RoleHomeZoneIcon);
            widgetRoles.append(RoleMcnIcon);
            widgetRoles.append(RoleActiveLineIcon);
            widgetRoles.append(RoleSatTextIcon);
            widgetRoles.append(RoleSettingsContainer); 
            break;
            
        default: 
            break; 
    }
    
    DPRINT << ": widgetRoles.count() : " << widgetRoles.count();
    DPRINT << ": OUT";
    return widgetRoles; 
}    
    
// End of File. 


