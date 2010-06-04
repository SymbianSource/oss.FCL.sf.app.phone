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

#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <hbwidget.h>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QFlags>
#include "infowidgetlayoutmanager.h"

class QLabel;
class QGraphicsLinearLayout;
class QTranslator; 
class HbDialog;
class HbIconItem; 
class HbMarqueeItem;
class HbLabel; 
class HbCheckBox; 
class HbPushButton;
class HbFrameDrawer;
class InfoWidgetEngine; 
class InfoWidgetPreferences;
 

class InfoWidget : public HbWidget
{
    Q_OBJECT

public:    
    InfoWidget(QGraphicsItem *parent = 0, Qt::WindowFlags flags = 0);
    ~InfoWidget();

public: // From QGraphicsItem.
    QRectF boundingRect() const;
    QPainterPath shape() const; 
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint) const;  
    QSizePolicy sizePolicy () const;  
    
public: // Property definitions
    Q_PROPERTY(QString homeZoneDisplay READ homeZoneDisplay WRITE setHomeZoneDisplay)
    Q_PROPERTY(QString mcnDisplay READ mcnDisplay WRITE setMcnDisplay)
    Q_PROPERTY(QString activeLineDisplay READ activeLineDisplay WRITE setActiveLineDisplay)
    Q_PROPERTY(QString satDisplay READ satDisplay WRITE setSatDisplay)
    Q_PROPERTY(QString spnDisplay READ spnDisplay WRITE setSpnDisplay)
    QString homeZoneDisplay();
    void setHomeZoneDisplay(QString value);
    QString mcnDisplay();
    void setMcnDisplay(QString value);
    QString activeLineDisplay();
    void setActiveLineDisplay(QString value);
    QString satDisplay();
    void setSatDisplay(QString value);
    QString spnDisplay();
    void setSpnDisplay(QString value);
    
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0); 
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event); 
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event); 
    
    void updateInfoDisplay();
    bool readPersistentPreferences();
    void initializeCheckBoxStates(); 
    void changeEvent(QEvent *event);
    bool installTranslator(QString translationFile); 
    void removeTranslators(); 
    
private: 
    void updateItemsVisibility();
    void layoutInfoDisplay(); 
    void layoutSettingsDisplay(); 
    void initializeInfoDisplayItems(); 
    void initializeSettingsDisplayItems();
    void startChanges(); 
    void endChanges(); 

public slots: 
    // Slots from HsWidget
    void onInitialize();
    void onUninitialize();
    void onShow();
    void onHide();

    // Info widget specific slots 
    void readModel(); 
    void handleModelError(int operation,int errorCode);  
    void spnDisplaySettingChanged(int state);
    void mcnDisplaySettingChanged(int state);
    void satDisplaySettingChanged(int state);
    void settingsEditingFinished();
    void settingsValidationFailed(); 
    
signals: 
    void setPreferences(const QStringList &names);

private:
    Q_DISABLE_COPY(InfoWidget)
    
    // Engine for widget      
    InfoWidgetEngine *m_engine;
    
    // Widget preference store  
    InfoWidgetPreferences *m_preferences;

    // Layout manager for widget
    InfoWidgetLayoutManager *m_layoutManager;
    
    QGraphicsLinearLayout *m_layout; 
    QList<QTranslator *> m_translators;
    QColor m_backGroundColor; 
    HbFrameDrawer *m_frameDrawer;
    bool m_layoutChanging;
    bool m_dragEvent; 
    bool m_initialized;
};


#endif // INFOWIDGET_H

