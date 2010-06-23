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
#include <QString>
#include <QFont>
#include <QMap>
#include <hbdocumentloader.h>

class QGraphicsLayout; 
class QGraphicsWidget;
class HbLabel;
class HbIconItem;
class HbPushButton;
class HbMarqueeItem;
class HbCheckBox; 

// Class declaration
class InfoWidgetDocumentLoader : public HbDocumentLoader
{
public:
    InfoWidgetDocumentLoader();
    virtual QObject *createObject(const QString& type,
                                  const QString &name);
};

// Class declaration
class InfoWidgetLayoutManager : public QObject
{
    Q_OBJECT
    
public: 
    
    enum LayoutItemRole {
        RoleUndefined = 0,
        // Content widget, the main widget from document 
        RoleContent,
        // Marquee items
        RoleSpnMarqueeItem,
        RoleMcnMarqueeItem,
        RoleSatMarqueeItem,
        // Icons
        RoleSpnIcon, 
        RoleMcnIcon, 
        RoleSatTextIcon, 
        // Check boxes 
        RoleSpnCheckBox,
        RoleMcnCheckBox, 
        RoleSatTextCheckBox,
        // Dialogs
        RoleSettingsDialog,         
        // Actions
        RoleOkAction,
        RoleCancelAction,
        // List widgets 
        RoleSettingsList,
        // Layout containers
        RoleContainer, 
        RoleSettingsContainer, 
        // Must be the last enumeration,
        RoleLastEnum
        }; 
    
    enum DisplayRole {
        InfoDisplay,
        SettingsDialog
        }; 
    
    InfoWidgetLayoutManager(QObject *parent = 0);
    ~InfoWidgetLayoutManager();
    
    DisplayRole currentDisplayRole(); 
    QList<LayoutItemRole> currentWidgetRoles(); 
    
    QGraphicsLayout* layoutInfoDisplay();
    QGraphicsLayout* layoutSettingsDialog();
    
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

    QObject* loadObject(InfoWidgetDocumentLoader &loader, 
            DisplayRole displayRole, 
            LayoutItemRole objectRole);
    QObject* getObject(LayoutItemRole itemRole);
    
    QGraphicsWidget* contentWidget();
    QList<HbMarqueeItem *> marqueeItems(); 
    
    void removeWidget(LayoutItemRole itemRole, 
            bool deleteLater = false);
    void destroyWidgets();
    
    int layoutRows() const; 
    void setLayoutRows(int rows);
    
    qreal layoutRowHeight() const;
    bool textFitsToRect(QString text, QFont font, QRectF rect); 
    
private:
    QMap<LayoutItemRole, QGraphicsWidget *> m_widgets;
    QMap<LayoutItemRole, QGraphicsWidget *> m_infoDisplayWidgets; 
    QMap<LayoutItemRole, QGraphicsWidget *> m_settingsDialogWidgets;
    QMap<LayoutItemRole, QObject *> m_objects;

    InfoWidgetDocumentLoader *m_documentLoader;
    DisplayRole m_displayRole; 
    int m_layoutRows;
    
};


#endif // INFOWIDGETLAYOUTMANAGER_H

