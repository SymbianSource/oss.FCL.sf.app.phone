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
#include "hbwidget.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbWidget::HbWidget
// -----------------------------------------------------------------------------
//
HbWidget::HbWidget( 
        QGraphicsItem * parent,
        Qt::WindowFlags wFlags )
    : HbWidgetBase( parent, wFlags )
    {
    
    }


// -----------------------------------------------------------------------------
// HbWidget::~HbWidget
// -----------------------------------------------------------------------------
//
HbWidget::~HbWidget(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbWidget::event
// -----------------------------------------------------------------------------
//
bool HbWidget::event( 
        QEvent * e )
    {
    SMC_MOCK_METHOD1( bool, QEvent *, e )
    }


// -----------------------------------------------------------------------------
// HbWidget::setStyle
// -----------------------------------------------------------------------------
//
void HbWidget::setStyle( 
        HbStyle * style )
    {
    SMC_MOCK_METHOD1( void, HbStyle *, style )
    }


// -----------------------------------------------------------------------------
// HbWidget::style
// -----------------------------------------------------------------------------
//
HbStyle * HbWidget::style(  ) const
    {
    SMC_MOCK_METHOD0( HbStyle * )
    }


// -----------------------------------------------------------------------------
// HbWidget::clearActions
// -----------------------------------------------------------------------------
//
void HbWidget::clearActions(  )
    {
    SMC_MOCK_METHOD0( void )
    }

// -----------------------------------------------------------------------------
// HbWidget::mainWindow
// -----------------------------------------------------------------------------
//
HbMainWindow * HbWidget::mainWindow(  ) const
    {
    SMC_MOCK_METHOD0( HbMainWindow * )
    }


// -----------------------------------------------------------------------------
// HbWidget::primitive
// -----------------------------------------------------------------------------
//
QGraphicsItem * HbWidget::primitive( 
        HbStyle::Primitive primitive ) const
    {
    SMC_MOCK_METHOD1( QGraphicsItem *, HbStyle::Primitive, primitive )
    }


// -----------------------------------------------------------------------------
// HbWidget::primitive
// -----------------------------------------------------------------------------
//
QGraphicsItem * HbWidget::primitive( 
        const QString & itemName ) const
    {
    SMC_MOCK_METHOD1( QGraphicsItem *, const QString &, itemName )
    }


// -----------------------------------------------------------------------------
// HbWidget::layoutPrimitive
// -----------------------------------------------------------------------------
//
QGraphicsLayoutItem * HbWidget::layoutPrimitive( 
        const QString & itemName ) const
    {
    SMC_MOCK_METHOD1( QGraphicsLayoutItem *, const QString &, itemName )
    }


// -----------------------------------------------------------------------------
// HbWidget::setFocusOrientation
// -----------------------------------------------------------------------------
//
void HbWidget::setFocusOrientation( 
        Qt::Orientations ,
        Qt::Orientations  )
    {
    //SMC_MOCK_METHOD2( void, Qt::Orientations, previous, 
    //    Qt::Orientations, next )
    }


// -----------------------------------------------------------------------------
// HbWidget::keyForFocusNextChild
// -----------------------------------------------------------------------------
//
QKeySequence HbWidget::keyForFocusNextChild(  ) const
    {
    SMC_MOCK_METHOD0( QKeySequence )
    }


// -----------------------------------------------------------------------------
// HbWidget::keyForFocusPreviousChild
// -----------------------------------------------------------------------------
//
QKeySequence HbWidget::keyForFocusPreviousChild(  ) const
    {
    SMC_MOCK_METHOD0( QKeySequence )
    }


// -----------------------------------------------------------------------------
// HbWidget::setKeyFocusMode
// -----------------------------------------------------------------------------
//
void HbWidget::setKeyFocusMode( 
        const QKeySequence & key )
    {
    SMC_MOCK_METHOD1( void, const QKeySequence &, key )
    }


// -----------------------------------------------------------------------------
// HbWidget::keyFocusMode
// -----------------------------------------------------------------------------
//
QKeySequence HbWidget::keyFocusMode(  ) const
    {
    SMC_MOCK_METHOD0( QKeySequence )
    }


// -----------------------------------------------------------------------------
// HbWidget::lastFocusedChild
// -----------------------------------------------------------------------------
//
QGraphicsItem * HbWidget::lastFocusedChild(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsItem * )
    }


// -----------------------------------------------------------------------------
// HbWidget::setFocusOrder
// -----------------------------------------------------------------------------
//
void HbWidget::setFocusOrder( 
        QGraphicsItem * first,
        QGraphicsItem * second )
    {
    SMC_MOCK_METHOD2( void, QGraphicsItem *, first, 
        QGraphicsItem *, second )
    }


// -----------------------------------------------------------------------------
// HbWidget::setFocusDelegation
// -----------------------------------------------------------------------------
//
void HbWidget::setFocusDelegation( 
        HbWidget::FocusDelegation focusDelegation )
    {
    SMC_MOCK_METHOD1( void, HbWidget::FocusDelegation, focusDelegation )
    }


// -----------------------------------------------------------------------------
// HbWidget::focusDelegation
// -----------------------------------------------------------------------------
//
HbWidget::FocusDelegation HbWidget::focusDelegation(  ) const
    {
    SMC_MOCK_METHOD0( HbWidget::FocusDelegation )
    }


// -----------------------------------------------------------------------------
// HbWidget::setFocusLooping
// -----------------------------------------------------------------------------
//
void HbWidget::setFocusLooping( 
        bool enable )
    {
    SMC_MOCK_METHOD1( void, bool, enable )
    }


// -----------------------------------------------------------------------------
// HbWidget::hasFocusLooping
// -----------------------------------------------------------------------------
//
bool HbWidget::hasFocusLooping(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbWidget::setFocusMode
// -----------------------------------------------------------------------------
//
void HbWidget::setFocusMode( 
        HbWidget::FocusMode mode)
    {
    SMC_MOCK_METHOD1( void, HbWidget::FocusMode, mode )
    }


// -----------------------------------------------------------------------------
// HbWidget::focusMode
// -----------------------------------------------------------------------------
//
HbWidget::FocusMode HbWidget::focusMode(  ) const
    {
    SMC_MOCK_METHOD0( HbWidget::FocusMode )
    }


// -----------------------------------------------------------------------------
// HbWidget::setFocusHighlight
// -----------------------------------------------------------------------------
//
void HbWidget::setFocusHighlight( 
        HbStyle::Primitive type,
        HbWidget::FocusHighlight focusHighlight )
    {
    SMC_MOCK_METHOD2( void, HbStyle::Primitive, type, 
        HbWidget::FocusHighlight, focusHighlight )
    }


// -----------------------------------------------------------------------------
// HbWidget::focusHighlight
// -----------------------------------------------------------------------------
//
HbStyle::Primitive HbWidget::focusHighlight( 
        HbWidget::FocusHighlight focusHighlight )
    {
    SMC_MOCK_METHOD1( HbStyle::Primitive, HbWidget::FocusHighlight, focusHighlight )
    }


// -----------------------------------------------------------------------------
// HbWidget::setBackgroundItem
// -----------------------------------------------------------------------------
//
void HbWidget::setBackgroundItem( 
        QGraphicsItem * item,
        int zValue )
    {
    SMC_MOCK_METHOD2( void, QGraphicsItem *, item, 
        int, zValue )
    }


// -----------------------------------------------------------------------------
// HbWidget::backgroundItem
// -----------------------------------------------------------------------------
//
QGraphicsItem * HbWidget::backgroundItem(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsItem * )
    }


// -----------------------------------------------------------------------------
// HbWidget::recreatePrimitives
// -----------------------------------------------------------------------------
//
void HbWidget::recreatePrimitives(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbWidget::updatePrimitives
// -----------------------------------------------------------------------------
//
void HbWidget::updatePrimitives(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbWidget::focusInEvent
// -----------------------------------------------------------------------------
//
void HbWidget::focusInEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbWidget::focusOutEvent
// -----------------------------------------------------------------------------
//
void HbWidget::focusOutEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbWidget::resizeEvent
// -----------------------------------------------------------------------------
//
void HbWidget::resizeEvent( 
        QGraphicsSceneResizeEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneResizeEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbWidget::changeEvent
// -----------------------------------------------------------------------------
//
void HbWidget::changeEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbWidget::itemChange
// -----------------------------------------------------------------------------
//
QVariant HbWidget::itemChange( 
        GraphicsItemChange change,
        const QVariant & value )
    {
    SMC_MOCK_METHOD2( QVariant, GraphicsItemChange, change, 
        const QVariant &, value )
    }


// -----------------------------------------------------------------------------
// HbWidget::polishEvent
// -----------------------------------------------------------------------------
//
void HbWidget::polishEvent(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbWidget::polish
// -----------------------------------------------------------------------------
//
void HbWidget::polish( 
        HbStyleParameters & params )
    {
    SMC_MOCK_METHOD1( void, HbStyleParameters &, params )
    }


// -----------------------------------------------------------------------------
// HbWidget::repolish
// -----------------------------------------------------------------------------
//
void HbWidget::repolish(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbWidget::sceneEventFilter
// -----------------------------------------------------------------------------
//
bool HbWidget::sceneEventFilter( 
        QGraphicsItem * watched,
        QEvent * event )
    {
    SMC_MOCK_METHOD2( bool, QGraphicsItem *, watched, 
        QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbWidget::HbWidget
// -----------------------------------------------------------------------------
//
HbWidget::HbWidget( 
        HbWidgetPrivate & dd,
        QGraphicsItem * parent,
        Qt::WindowFlags wFlags )
    //:
    //HbWidgetBase( /*dd, parent, wFlags*/ )
    {
    
    }


