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

#ifndef INFOWIDGETLAYOUTMANAGER_H
#define INFOWIDGETLAYOUTMANAGER_H

#include <QObject>
#include <QList>
#include <QSizeF>
#include <QRectF>
#include <QMap>
#include <hbdocumentloader.h>

class QGraphicsLayout; 
class QGraphicsWidget;
class HbLabel;
class HbIconItem;
class HbPushButton;
class HbMarqueeItem;
class HbCheckBox; 

/*!
 */
class InfoWidgetDocumentLoader : public HbDocumentLoader
{
public:
    InfoWidgetDocumentLoader();
    virtual QObject *createObject(const QString& type,
                                  const QString &name);
};

class InfoWidgetLayoutManager : public QObject
{
    Q_OBJECT
    
public: 
    
    enum LayoutItemRole {
        RoleUndefined = 0,
        // Content widget, the main widget from document 
        RoleContent,
        // Marquee items 
        RoleMcnMarqueeItem,
        RoleSatMarqueeItem,
        // Labels 
        RoleSpnLabel,
        RoleMcnLabel, 
        RoleSatTextLabel,
        // Icons
        RoleSpnIcon, 
        RoleMcnIcon, 
        RoleSatTextIcon, 
        // Check boxes 
        RoleSpnCheckBox,
        RoleMcnCheckBox, 
        RoleSatTextCheckBox,
        // Buttons 
        RoleOkButton,
        // Layout containers
        RoleContainer, 
        RoleSettingsContainer, 
        // Must be the last enumeration        
        RoleLastEnum
        }; 
    
    enum DisplayRole {
        InfoDisplay,
        SettingsDisplay
        }; 
    
    InfoWidgetLayoutManager(QObject *parent = 0);
    ~InfoWidgetLayoutManager();
    
    void destroyObjects(); 
    
    DisplayRole currentDisplayRole(); 
    QList<LayoutItemRole> currentWidgetRoles(); 
    
    int layoutRows() const; 
    void setLayoutRows(int rows);
    qreal rowHeight() const; 
    
    QGraphicsLayout* layoutInfoDisplay();
    QGraphicsLayout* layoutSettingsDisplay(); 
    
    const QList<InfoWidgetLayoutManager::LayoutItemRole> widgetRoles(
            DisplayRole displayRole) const; 

    bool loadWidgets(const DisplayRole displayRole, 
            const QList<LayoutItemRole> &displayWidgets,
            QMap<LayoutItemRole, QGraphicsWidget *> &widgetMap);
    bool reloadWidgets(const DisplayRole displayRole);
    
    QGraphicsWidget* loadWidget(InfoWidgetDocumentLoader &loader, 
            DisplayRole displayRole, 
            LayoutItemRole widgetRole);
    
    QGraphicsWidget* getWidget(LayoutItemRole itemRole);
    QGraphicsWidget* contentWidget();
    
    void removeWidget(LayoutItemRole itemRole);
    void hideWidget(LayoutItemRole itemRole);
    void hideAll();
    void showAll();
    
    
private:
    QMap<LayoutItemRole, QGraphicsWidget *> m_widgets; 
    QMap<LayoutItemRole, QGraphicsWidget *> m_infoDisplayWidgets; 
    QMap<LayoutItemRole, QGraphicsWidget *> m_settingsDisplayWidgets; 

    InfoWidgetDocumentLoader *m_documentLoader;
    DisplayRole m_displayRole; 
    int m_layoutRows;
    
};


#endif // INFOWIDGETLAYOUTMANAGER_H

