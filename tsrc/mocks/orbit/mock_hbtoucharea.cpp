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
#include <hbtoucharea.h>

#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbTouchArea::HbTouchArea
// -----------------------------------------------------------------------------
//
HbTouchArea::HbTouchArea( 
        QGraphicsItem * /*parent*/ )
    //:
    //HbWidgetBase( /*parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// HbTouchArea::~HbTouchArea
// -----------------------------------------------------------------------------
//
HbTouchArea::~HbTouchArea(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbTouchArea::setSize
// -----------------------------------------------------------------------------
//
void HbTouchArea::setSize( 
        const QSizeF & size )
    {
    SMC_MOCK_METHOD1( void, const QSizeF &, size )
    }


// -----------------------------------------------------------------------------
// HbTouchArea::setGeometry
// -----------------------------------------------------------------------------
//
void HbTouchArea::setGeometry( 
        const QRectF & rect )
    {
    SMC_MOCK_METHOD1( void, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// HbTouchArea::updateGeometry
// -----------------------------------------------------------------------------
//
void HbTouchArea::updateGeometry(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbTouchArea::paint
// -----------------------------------------------------------------------------
//
void HbTouchArea::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// HbTouchArea::type
// -----------------------------------------------------------------------------
//
/*int HbTouchArea::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }*/


// -----------------------------------------------------------------------------
// HbTouchArea::HbTouchArea
// -----------------------------------------------------------------------------
//
HbTouchArea::HbTouchArea( 
        HbTouchAreaPrivate & /*dd*/,
        QGraphicsItem * /*parent*/ )
    //:
    //HbWidgetBase( /*dd, parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// HbTouchArea::sizeHint
// -----------------------------------------------------------------------------
//
QSizeF HbTouchArea::sizeHint( 
        Qt::SizeHint which,
        const QSizeF & constraint ) const
    {
    SMC_MOCK_METHOD2( QSizeF, Qt::SizeHint, which, 
        const QSizeF &, constraint )
    }


// -----------------------------------------------------------------------------
// HbTouchArea::gestureEvent
// -----------------------------------------------------------------------------
//
void HbTouchArea::gestureEvent( 
        QGestureEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGestureEvent *, event )
    }


