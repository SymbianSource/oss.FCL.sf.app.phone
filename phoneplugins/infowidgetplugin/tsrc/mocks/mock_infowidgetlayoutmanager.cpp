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
#include <QDebug>
#include <QObject>
#include <smcmockclassincludes.h>
#include "infowidgetlayoutmanager.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::InfoWidgetLayoutManager
// -----------------------------------------------------------------------------
//
InfoWidgetLayoutManager::InfoWidgetLayoutManager( 
        QObject * parent )
    :QObject( parent )
    {
    
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::~InfoWidgetLayoutManager
// -----------------------------------------------------------------------------
//
InfoWidgetLayoutManager::~InfoWidgetLayoutManager(  )
    {
    
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::currentDisplayRole
// -----------------------------------------------------------------------------
//
InfoWidgetLayoutManager::DisplayRole InfoWidgetLayoutManager::currentDisplayRole(  )
    {
    SMC_MOCK_METHOD0( DisplayRole )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::currentWidgetRoles
// -----------------------------------------------------------------------------
//
QList <InfoWidgetLayoutManager::LayoutItemRole > 
        InfoWidgetLayoutManager::currentWidgetRoles(  )
    {
    SMC_MOCK_METHOD0( QList <LayoutItemRole > )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::layoutInfoDisplay
// -----------------------------------------------------------------------------
//
QGraphicsLayout * InfoWidgetLayoutManager::layoutInfoDisplay(  )
    {
    SMC_MOCK_METHOD0( QGraphicsLayout * )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::layoutSettingsDialog
// -----------------------------------------------------------------------------
//
QGraphicsLayout * InfoWidgetLayoutManager::layoutSettingsDialog(  )
    {
    SMC_MOCK_METHOD0( QGraphicsLayout * )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::widgetRoles
// -----------------------------------------------------------------------------
//
const QList <InfoWidgetLayoutManager::LayoutItemRole > 
        InfoWidgetLayoutManager::widgetRoles(DisplayRole displayRole ) const
    {
    SMC_MOCK_METHOD1( 
        const QList <InfoWidgetLayoutManager::LayoutItemRole >, 
        DisplayRole, displayRole )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::loadWidgets
// -----------------------------------------------------------------------------
//
bool InfoWidgetLayoutManager::loadWidgets(
        const DisplayRole displayRole, 
        const QList<LayoutItemRole> &displayWidgets,
        QMap<LayoutItemRole, QGraphicsWidget *> &widgetMap)
    {
    typedef QMap <LayoutItemRole, QGraphicsWidget *> & TYPE3;
    SMC_MOCK_METHOD3( bool, const DisplayRole, displayRole, 
        const QList<LayoutItemRole> &, displayWidgets, 
        TYPE3, widgetMap )
    }

// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::reloadWidgets
// -----------------------------------------------------------------------------
//
bool InfoWidgetLayoutManager::reloadWidgets( 
        const DisplayRole displayRole )
    {
    SMC_MOCK_METHOD1( bool, const DisplayRole, displayRole )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::loadWidget
// -----------------------------------------------------------------------------
//
QGraphicsWidget* InfoWidgetLayoutManager::loadWidget(
        InfoWidgetDocumentLoader &loader, 
        DisplayRole displayRole, 
        LayoutItemRole widgetRole)
    {
    SMC_MOCK_METHOD3( QGraphicsWidget *, InfoWidgetDocumentLoader &, loader, 
        DisplayRole, displayRole, 
        LayoutItemRole, widgetRole )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::getWidget
// -----------------------------------------------------------------------------
//
QGraphicsWidget * InfoWidgetLayoutManager::getWidget( 
        LayoutItemRole itemRole )
    {
    SMC_MOCK_METHOD1( QGraphicsWidget *, LayoutItemRole, itemRole )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::loadObject
// -----------------------------------------------------------------------------
//
QObject * InfoWidgetLayoutManager::loadObject( 
        InfoWidgetDocumentLoader & loader,
        DisplayRole displayRole,
        LayoutItemRole objectRole )
    {
    SMC_MOCK_METHOD3( QObject *, InfoWidgetDocumentLoader &, loader, 
        DisplayRole, displayRole, 
        LayoutItemRole, objectRole )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::getObject
// -----------------------------------------------------------------------------
//
QObject * InfoWidgetLayoutManager::getObject( 
        LayoutItemRole itemRole )
    {
    SMC_MOCK_METHOD1( QObject *, LayoutItemRole, itemRole )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::contentWidget
// -----------------------------------------------------------------------------
//
QGraphicsWidget * InfoWidgetLayoutManager::contentWidget(  )
    {
    SMC_MOCK_METHOD0( QGraphicsWidget * )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::marqueeItems
// -----------------------------------------------------------------------------
//
QList <HbMarqueeItem * > InfoWidgetLayoutManager::marqueeItems(  )
    {
    SMC_MOCK_METHOD0( QList <HbMarqueeItem * > )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::removeWidget
// -----------------------------------------------------------------------------
//
void InfoWidgetLayoutManager::removeWidget( 
        LayoutItemRole itemRole,
        bool deleteLater )
    {
    SMC_MOCK_METHOD2( void, LayoutItemRole, itemRole, 
        bool, deleteLater )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::destroyWidgets
// -----------------------------------------------------------------------------
//
void InfoWidgetLayoutManager::destroyWidgets(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::layoutRows
// -----------------------------------------------------------------------------
//
int InfoWidgetLayoutManager::layoutRows(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::setLayoutRows
// -----------------------------------------------------------------------------
//
void InfoWidgetLayoutManager::setLayoutRows( 
        int rows )
    {
    SMC_MOCK_METHOD1( void, int, rows )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::layoutRowHeight
// -----------------------------------------------------------------------------
//
qreal InfoWidgetLayoutManager::layoutRowHeight(  )
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// InfoWidgetLayoutManager::textFitsToRect
// -----------------------------------------------------------------------------
//
bool InfoWidgetLayoutManager::textFitsToRect( 
        QString text,
        QFont font,
        QRectF rect ) const
    {
    SMC_MOCK_METHOD3( bool, QString, text, 
        QFont, font, 
        QRectF, rect )
    }


// -----------------------------------------------------------------------------
// InfoWidgetDocumentLoader::createObject
// -----------------------------------------------------------------------------
//
QObject * InfoWidgetDocumentLoader::createObject( 
        const QString & type,
        const QString & name )
    {
    SMC_MOCK_METHOD2( QObject *, const QString &, type, 
        const QString &, name )
    }


