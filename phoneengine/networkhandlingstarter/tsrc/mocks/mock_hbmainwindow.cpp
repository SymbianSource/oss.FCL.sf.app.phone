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
#include <hbmainwindow.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbMainWindow::HbMainWindow
// -----------------------------------------------------------------------------
//
HbMainWindow::HbMainWindow( 
        QWidget * parent,
        Hb::WindowFlags windowFlags )
    : d_ptr(NULL)
    //QGraphicsView( /*parent, windowFlags*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// HbMainWindow::~HbMainWindow
// -----------------------------------------------------------------------------
//
HbMainWindow::~HbMainWindow(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbMainWindow::insertView
// -----------------------------------------------------------------------------
//
HbView * HbMainWindow::insertView( 
        int index,
        QGraphicsWidget * widget )
    {
    //SMC_MOCK_METHOD2( HbView *, int, index, 
      //  QGraphicsWidget *, widget )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::removeView
// -----------------------------------------------------------------------------
//
QGraphicsWidget * HbMainWindow::removeView( 
        int index )
    {
    SMC_MOCK_METHOD1( QGraphicsWidget *, int, index )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::currentViewIndex
// -----------------------------------------------------------------------------
//
int HbMainWindow::currentViewIndex(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::viewCount
// -----------------------------------------------------------------------------
//
int HbMainWindow::viewCount(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::indexOfView
// -----------------------------------------------------------------------------
//
int HbMainWindow::indexOfView( 
        HbView * view ) const
    {
    //SMC_MOCK_METHOD1( int, HbView *, view )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::viewAt
// -----------------------------------------------------------------------------
//
HbView * HbMainWindow::viewAt( 
        int index ) const
    {
    SMC_MOCK_METHOD1( HbView *, int, index )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::addView
// -----------------------------------------------------------------------------
//
HbView * HbMainWindow::addView( 
        QGraphicsWidget * widget )
    {
    //SMC_MOCK_METHOD1( HbView *, QGraphicsWidget *, widget )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::removeView
// -----------------------------------------------------------------------------
//
void HbMainWindow::removeView( 
        QGraphicsWidget * widget )
    {
    //SMC_MOCK_METHOD1( void, QGraphicsWidget *, widget )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::views
// -----------------------------------------------------------------------------
//
QList <HbView * > HbMainWindow::views(  ) const
    {
    //SMC_MOCK_METHOD0( QList <HbView * > )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::currentView
// -----------------------------------------------------------------------------
//
HbView * HbMainWindow::currentView(  ) const
    {
    SMC_MOCK_METHOD0( HbView * )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::setCurrentView
// -----------------------------------------------------------------------------
//
void HbMainWindow::setCurrentView( 
        HbView * view,
        bool animate,
        Hb::ViewSwitchFlags flags )
    {
    //SMC_MOCK_METHOD3( void, HbView *, view, 
    //    bool, animate, 
    //    Hb::ViewSwitchFlags, flags )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::element
// -----------------------------------------------------------------------------
//
QGraphicsWidget * HbMainWindow::element( 
        HbMainWindow::Element element ) const
    {
    //SMC_MOCK_METHOD1( QGraphicsWidget *, HbMainWindow::Element, element )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::setViewSwitchingEnabled
// -----------------------------------------------------------------------------
//
void HbMainWindow::setViewSwitchingEnabled( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::isViewSwitchingEnabled
// -----------------------------------------------------------------------------
//
bool HbMainWindow::isViewSwitchingEnabled(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::softKeyAction
// -----------------------------------------------------------------------------
//
HbAction * HbMainWindow::softKeyAction( 
        Hb::SoftKeyId key ) const
    {
    //SMC_MOCK_METHOD1( HbAction *, Hb::SoftKeyId, key )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::addSoftKeyAction
// -----------------------------------------------------------------------------
//
void HbMainWindow::addSoftKeyAction( 
        Hb::SoftKeyId key,
        HbAction * action )
    {
    //SMC_MOCK_METHOD2( void, Hb::SoftKeyId, key, 
    //    HbAction *, action )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::removeSoftKeyAction
// -----------------------------------------------------------------------------
//
void HbMainWindow::removeSoftKeyAction( 
        Hb::SoftKeyId key,
        HbAction * action )
    {
    //SMC_MOCK_METHOD2( void, Hb::SoftKeyId, key, 
    //    HbAction *, action )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::orientation
// -----------------------------------------------------------------------------
//
Qt::Orientation HbMainWindow::orientation(  ) const
    {
    SMC_MOCK_METHOD0( Qt::Orientation )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::setOrientation
// -----------------------------------------------------------------------------
//
void HbMainWindow::setOrientation( 
        Qt::Orientation orientation,
        bool animate )
    {
    //SMC_MOCK_METHOD2( void, Qt::Orientation, orientation, 
    //    bool, animate )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::unsetOrientation
// -----------------------------------------------------------------------------
//
void HbMainWindow::unsetOrientation( 
        bool animate )
    {
    SMC_MOCK_METHOD1( void, bool, animate )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::showItems
// -----------------------------------------------------------------------------
//
void HbMainWindow::showItems( 
        Hb::SceneItems items )
    {
    //SMC_MOCK_METHOD1( void, Hb::SceneItems, items )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::hideItems
// -----------------------------------------------------------------------------
//
void HbMainWindow::hideItems( 
        Hb::SceneItems items )
    {
    //SMC_MOCK_METHOD1( void, Hb::SceneItems, items )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::visibleItems
// -----------------------------------------------------------------------------
//
Hb::SceneItems HbMainWindow::visibleItems(  ) const
    {
    SMC_MOCK_METHOD0( Hb::SceneItems )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::isItemVisible
// -----------------------------------------------------------------------------
//
bool HbMainWindow::isItemVisible( 
        Hb::SceneItem item ) const
    {
    //SMC_MOCK_METHOD1( bool, Hb::SceneItem, item )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::setItemVisible
// -----------------------------------------------------------------------------
//
void HbMainWindow::setItemVisible( 
        Hb::SceneItem item,
        bool visible )
    {
    //SMC_MOCK_METHOD2( void, Hb::SceneItem, item, 
    //    bool, visible )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::nativeBackgroundWindow
// -----------------------------------------------------------------------------
//
WId HbMainWindow::nativeBackgroundWindow(  )
    {
    SMC_MOCK_METHOD0( WId )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::resetNativeBackgroundWindow
// -----------------------------------------------------------------------------
//
void HbMainWindow::resetNativeBackgroundWindow(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::layoutRect
// -----------------------------------------------------------------------------
//
QRectF HbMainWindow::layoutRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::setCurrentViewIndex
// -----------------------------------------------------------------------------
//
void HbMainWindow::setCurrentViewIndex( 
        int index )
    {
    SMC_MOCK_METHOD1( void, int, index )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::nextView
// -----------------------------------------------------------------------------
//
void HbMainWindow::nextView(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::previousView
// -----------------------------------------------------------------------------
//
void HbMainWindow::previousView(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::broadcastEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::broadcastEvent( 
        int eventType )
    {
    SMC_MOCK_METHOD1( void, int, eventType )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::toggleOrientation
// -----------------------------------------------------------------------------
//
void HbMainWindow::toggleOrientation(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::currentViewIndexChanged
// -----------------------------------------------------------------------------
//
void HbMainWindow::currentViewIndexChanged( 
        int index )
    {
    SMC_MOCK_METHOD1( void, int, index )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::viewReady
// -----------------------------------------------------------------------------
//
void HbMainWindow::viewReady(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::currentViewChanged
// -----------------------------------------------------------------------------
//
void HbMainWindow::currentViewChanged( 
        HbView * view )
    {
    //SMC_MOCK_METHOD1( void, HbView *, view )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::aboutToChangeOrientation
// -----------------------------------------------------------------------------
//
void HbMainWindow::aboutToChangeOrientation(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::aboutToChangeOrientation
// -----------------------------------------------------------------------------
//
void HbMainWindow::aboutToChangeOrientation( 
        Qt::Orientation newOrientation,
        bool animated )
    {
    //SMC_MOCK_METHOD2( void, Qt::Orientation, newOrientation, 
    //    bool, animated )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::orientationChanged
// -----------------------------------------------------------------------------
//
void HbMainWindow::orientationChanged( 
        Qt::Orientation orientation )
    {
    //SMC_MOCK_METHOD1( void, Qt::Orientation, orientation )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::changeEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::changeEvent( 
        QEvent * event )
    {
    //SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::closeEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::closeEvent( 
        QCloseEvent * event )
    {
    //SMC_MOCK_METHOD1( void, QCloseEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::keyPressEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::keyPressEvent( 
        QKeyEvent * event )
    {
    //SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::keyReleaseEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::keyReleaseEvent( 
        QKeyEvent * event )
    {
    //SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::resizeEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::resizeEvent( 
        QResizeEvent * event )
    {
    //SMC_MOCK_METHOD1( void, QResizeEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::customEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::customEvent( 
        QEvent * event )
    {
    //SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::scrollContentsBy
// -----------------------------------------------------------------------------
//
void HbMainWindow::scrollContentsBy( 
        int dx,
        int dy )
    {
    SMC_MOCK_METHOD2( void, int, dx, 
        int, dy )
    }


// -----------------------------------------------------------------------------
// HbMainWindow::paintEvent
// -----------------------------------------------------------------------------
//
void HbMainWindow::paintEvent( 
        QPaintEvent * event )
    {
    //SMC_MOCK_METHOD1( void, QPaintEvent *, event )
    }


