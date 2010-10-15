/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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
#include <QGraphicsSceneResizeEvent>
#include <HbAction>
#include <HbToolBarExtension>
#include "hbtoolbar.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbToolBar::HbToolBar
// -----------------------------------------------------------------------------
//
HbToolBar::HbToolBar( 
        QGraphicsItem * parent )
    : HbWidget( parent )
    {
    
    }


// -----------------------------------------------------------------------------
// HbToolBar::~HbToolBar
// -----------------------------------------------------------------------------
//
HbToolBar::~HbToolBar(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbToolBar::addAction
// -----------------------------------------------------------------------------
//
HbAction * HbToolBar::addAction( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( HbAction *, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbToolBar::addAction
// -----------------------------------------------------------------------------
//
HbAction * HbToolBar::addAction( 
        const HbIcon & icon,
        const QString & text )
    {
    SMC_MOCK_METHOD2( HbAction *, const HbIcon &, icon, 
        const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbToolBar::addAction
// -----------------------------------------------------------------------------
//
HbAction * HbToolBar::addAction( 
        const QString & text,
        const QObject * receiver,
        const char * member )
    {
    SMC_MOCK_METHOD3( HbAction *, const QString &, text, 
        const QObject *, receiver, 
        const char *, member )
    }


// -----------------------------------------------------------------------------
// HbToolBar::addAction
// -----------------------------------------------------------------------------
//
HbAction * HbToolBar::addAction( 
        const HbIcon & icon,
        const QString & text,
        const QObject * receiver,
        const char * member )
    {
    SMC_MOCK_METHOD4( HbAction *, const HbIcon &, icon, 
        const QString &, text, 
        const QObject *, receiver, 
        const char *, member )
    }


// -----------------------------------------------------------------------------
// HbToolBar::addExtension
// -----------------------------------------------------------------------------
//
HbAction * HbToolBar::addExtension( 
        HbToolBarExtension * extension )
    {
    SMC_MOCK_METHOD1( HbAction *, HbToolBarExtension *, extension )
    }


// -----------------------------------------------------------------------------
// HbToolBar::insertExtension
// -----------------------------------------------------------------------------
//
HbAction * HbToolBar::insertExtension( 
        HbAction * before,
        HbToolBarExtension * extension )
    {
    SMC_MOCK_METHOD2( HbAction *, HbAction *, before, 
        HbToolBarExtension *, extension )
    }


// -----------------------------------------------------------------------------
// HbToolBar::orientation
// -----------------------------------------------------------------------------
//
Qt::Orientation HbToolBar::orientation(  ) const
    {
    SMC_MOCK_METHOD0( Qt::Orientation )
    }


// -----------------------------------------------------------------------------
// HbToolBar::updatePrimitives
// -----------------------------------------------------------------------------
//
void HbToolBar::updatePrimitives(  )
    {
    SMC_MOCK_METHOD0( void )
    }

// -----------------------------------------------------------------------------
// HbToolBar::setOrientation
// -----------------------------------------------------------------------------
//
void HbToolBar::setOrientation( 
        Qt::Orientation orientation )
    {
    SMC_MOCK_METHOD1( void, Qt::Orientation, orientation )
    }


// -----------------------------------------------------------------------------
// HbToolBar::HbToolBar
// -----------------------------------------------------------------------------
//
HbToolBar::HbToolBar( 
        HbToolBarPrivate & dd,
        QGraphicsItem * parent )
    //:
    //HbWidget( /*dd, parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// HbToolBar::itemChange
// -----------------------------------------------------------------------------
//
QVariant HbToolBar::itemChange( 
        GraphicsItemChange change,
        const QVariant & value )
    {
    SMC_MOCK_METHOD2( QVariant, GraphicsItemChange, change, 
        const QVariant &, value )
    }


// -----------------------------------------------------------------------------
// HbToolBar::gestureEvent
// -----------------------------------------------------------------------------
//
void HbToolBar::gestureEvent( 
        QGestureEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGestureEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbToolBar::changeEvent
// -----------------------------------------------------------------------------
//
void HbToolBar::changeEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbToolBar::resizeEvent
// -----------------------------------------------------------------------------
//
void HbToolBar::resizeEvent( 
        QGraphicsSceneResizeEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneResizeEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbToolBar::hideEvent
// -----------------------------------------------------------------------------
//
void HbToolBar::hideEvent( 
        QHideEvent * event )
    {
    SMC_MOCK_METHOD1( void, QHideEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbToolBar::event
// -----------------------------------------------------------------------------
//
bool HbToolBar::event( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( bool, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbToolBar::polish
// -----------------------------------------------------------------------------
//
void HbToolBar::polish( 
        HbStyleParameters & params )
    {
    SMC_MOCK_METHOD1( void, HbStyleParameters &, params )
    }


