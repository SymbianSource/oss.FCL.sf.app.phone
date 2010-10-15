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
#include <smcmockclassincludes.h>
#include <hbview.h>

#include <HbIcon>
#include <HbMenu>
#include <HbToolBar>
#include <HbDockWidget>
#include <HbAction>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbView::HbView
// -----------------------------------------------------------------------------
//
HbView::HbView( 
        QGraphicsItem * parent )
    : HbWidget( parent )
    {
    
    }


// -----------------------------------------------------------------------------
// HbView::~HbView
// -----------------------------------------------------------------------------
//
HbView::~HbView(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbView::title
// -----------------------------------------------------------------------------
//
QString HbView::title(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbView::icon
// -----------------------------------------------------------------------------
//
HbIcon HbView::icon(  ) const
    {
    SMC_MOCK_METHOD0( HbIcon )
    }


// -----------------------------------------------------------------------------
// HbView::setIcon
// -----------------------------------------------------------------------------
//
void HbView::setIcon( 
        const HbIcon & icon )
    {
    SMC_MOCK_METHOD1( void, const HbIcon &, icon )
    }


// -----------------------------------------------------------------------------
// HbView::menu
// -----------------------------------------------------------------------------
//
HbMenu * HbView::menu(  ) const
    {
    SMC_MOCK_METHOD0( HbMenu * )
    }


// -----------------------------------------------------------------------------
// HbView::setMenu
// -----------------------------------------------------------------------------
//
void HbView::setMenu( 
        HbMenu * menu )
    {
    SMC_MOCK_METHOD1( void, HbMenu *, menu )
    }


// -----------------------------------------------------------------------------
// HbView::takeMenu
// -----------------------------------------------------------------------------
//
HbMenu * HbView::takeMenu(  )
    {
    SMC_MOCK_METHOD0( HbMenu * )
    }


// -----------------------------------------------------------------------------
// HbView::toolBar
// -----------------------------------------------------------------------------
//
HbToolBar * HbView::toolBar(  ) const
    {
    SMC_MOCK_METHOD0( HbToolBar * )
    }


// -----------------------------------------------------------------------------
// HbView::setToolBar
// -----------------------------------------------------------------------------
//
void HbView::setToolBar( 
        HbToolBar * toolBar )
    {
    SMC_MOCK_METHOD1( void, HbToolBar *, toolBar )
    }


// -----------------------------------------------------------------------------
// HbView::takeToolBar
// -----------------------------------------------------------------------------
//
HbToolBar * HbView::takeToolBar(  )
    {
    SMC_MOCK_METHOD0( HbToolBar * )
    }


// -----------------------------------------------------------------------------
// HbView::dockWidget
// -----------------------------------------------------------------------------
//
HbDockWidget * HbView::dockWidget(  ) const
    {
    SMC_MOCK_METHOD0( HbDockWidget * )
    }


// -----------------------------------------------------------------------------
// HbView::setDockWidget
// -----------------------------------------------------------------------------
//
void HbView::setDockWidget( 
        HbDockWidget * dockWidget )
    {
    SMC_MOCK_METHOD1( void, HbDockWidget *, dockWidget )
    }


// -----------------------------------------------------------------------------
// HbView::widget
// -----------------------------------------------------------------------------
//
QGraphicsWidget * HbView::widget(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsWidget * )
    }


// -----------------------------------------------------------------------------
// HbView::setWidget
// -----------------------------------------------------------------------------
//
void HbView::setWidget( 
        QGraphicsWidget * widget )
    {
    SMC_MOCK_METHOD1( void, QGraphicsWidget *, widget )
    }


// -----------------------------------------------------------------------------
// HbView::takeWidget
// -----------------------------------------------------------------------------
//
QGraphicsWidget * HbView::takeWidget(  )
    {
    SMC_MOCK_METHOD0( QGraphicsWidget * )
    }


// -----------------------------------------------------------------------------
// HbView::showItems
// -----------------------------------------------------------------------------
//
void HbView::showItems( 
        Hb::SceneItems /*items*/ )
    {
    //SMC_MOCK_METHOD1( void, Hb::SceneItems, items )
    }


// -----------------------------------------------------------------------------
// HbView::hideItems
// -----------------------------------------------------------------------------
//
void HbView::hideItems( 
        Hb::SceneItems /*items*/ )
    {
    //SMC_MOCK_METHOD1( void, Hb::SceneItems, items )
    }


// -----------------------------------------------------------------------------
// HbView::visibleItems
// -----------------------------------------------------------------------------
//
Hb::SceneItems HbView::visibleItems(  ) const
    {
    SMC_MOCK_METHOD0( Hb::SceneItems )
    }


// -----------------------------------------------------------------------------
// HbView::isItemVisible
// -----------------------------------------------------------------------------
//
bool HbView::isItemVisible( 
        Hb::SceneItem /*item*/ ) const
    {
    //SMC_MOCK_METHOD1( bool, Hb::SceneItem, item )
    }


// -----------------------------------------------------------------------------
// HbView::setItemVisible
// -----------------------------------------------------------------------------
//
void HbView::setItemVisible( 
        Hb::SceneItem /*item*/,
        bool /*visible*/ )
    {
    /*SMC_MOCK_METHOD2( void, Hb::SceneItem, item, 
        bool, visible )*/
    }


// -----------------------------------------------------------------------------
// HbView::isContentFullScreen
// -----------------------------------------------------------------------------
//
bool HbView::isContentFullScreen(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbView::viewFlags
// -----------------------------------------------------------------------------
//
HbView::HbViewFlags HbView::viewFlags(  ) const
    {
    SMC_MOCK_METHOD0( HbView::HbViewFlags )
    }


// -----------------------------------------------------------------------------
// HbView::setTitleBarVisible
// -----------------------------------------------------------------------------
//
void HbView::setTitleBarVisible( 
        bool visible )
    {
    SMC_MOCK_METHOD1( void, bool, visible )
    }


// -----------------------------------------------------------------------------
// HbView::setStatusBarVisible
// -----------------------------------------------------------------------------
//
void HbView::setStatusBarVisible( 
        bool visible )
    {
    SMC_MOCK_METHOD1( void, bool, visible )
    }



// -----------------------------------------------------------------------------
// HbView::addAction
// -----------------------------------------------------------------------------
//
void HbView::addAction( 
        HbAction * actione,
        ActionContainer preferredActionContainer )
    {
    SMC_MOCK_METHOD2( void, HbAction *, actione, 
        ActionContainer, preferredActionContainer )
    }


// -----------------------------------------------------------------------------
// HbView::navigationAction
// -----------------------------------------------------------------------------
//
HbAction * HbView::navigationAction(  ) const
    {
    SMC_MOCK_METHOD0( HbAction * )
    }


// -----------------------------------------------------------------------------
// HbView::setNavigationAction
// -----------------------------------------------------------------------------
//
void HbView::setNavigationAction( 
        HbAction * actione )
    {
    SMC_MOCK_METHOD1( void, HbAction *, actione )
    }


// -----------------------------------------------------------------------------
// HbView::setTitle
// -----------------------------------------------------------------------------
//
void HbView::setTitle( 
        const QString & title )
    {
    SMC_MOCK_METHOD1( void, const QString &, title )
    }


// -----------------------------------------------------------------------------
// HbView::setContentFullScreen
// -----------------------------------------------------------------------------
//
void HbView::setContentFullScreen( 
        bool enable )
    {
    SMC_MOCK_METHOD1( void, bool, enable )
    }


// -----------------------------------------------------------------------------
// HbView::setViewFlags
// -----------------------------------------------------------------------------
//
void HbView::setViewFlags( 
        HbViewFlags /*flags*/ )
    {
    //SMC_MOCK_METHOD1( void, HbViewFlags, flags )
    }


// -----------------------------------------------------------------------------
// HbView::titleChanged
// -----------------------------------------------------------------------------
//
void HbView::titleChanged( 
        const QString & title )
    {
    SMC_MOCK_METHOD1( void, const QString &, title )
    }


// -----------------------------------------------------------------------------
// HbView::iconChanged
// -----------------------------------------------------------------------------
//
void HbView::iconChanged( 
        const HbIcon & icon )
    {
    SMC_MOCK_METHOD1( void, const HbIcon &, icon )
    }


// -----------------------------------------------------------------------------
// HbView::toolBarChanged
// -----------------------------------------------------------------------------
//
void HbView::toolBarChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbView::visibleItemsChanged
// -----------------------------------------------------------------------------
//
void HbView::visibleItemsChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbView::contentFullScreenChanged
// -----------------------------------------------------------------------------
//
void HbView::contentFullScreenChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbView::dockWidgetChanged
// -----------------------------------------------------------------------------
//
void HbView::dockWidgetChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbView::HbView
// -----------------------------------------------------------------------------
//
HbView::HbView( 
        HbViewPrivate & dd,
        QGraphicsItem * parent )
    //:
    //HbWidget( /*dd, parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// HbView::event
// -----------------------------------------------------------------------------
//
bool HbView::event( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( bool, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbView::changeEvent
// -----------------------------------------------------------------------------
//
void HbView::changeEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


