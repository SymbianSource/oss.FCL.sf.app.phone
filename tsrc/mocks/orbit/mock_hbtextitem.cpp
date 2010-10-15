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
#include "hbtextitem.h"

/*#include <hbnamespace.h>
#include <hbwidgetbase.h>*/

#include <qwidget.h>
#include <qgraphicssceneevent.h>
#include <qstyleoption.h>
#include <qpainter.h>
#include <QGraphicsSceneResizeEvent>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbTextItem::HbTextItem
// -----------------------------------------------------------------------------
//
HbTextItem::HbTextItem( 
        QGraphicsItem * parent )
    //:
    //HbWidgetBase( /*parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// HbTextItem::HbTextItem
// -----------------------------------------------------------------------------
//
HbTextItem::HbTextItem( 
        const QString & text,
        QGraphicsItem * parent )
    //:
    //HbWidgetBase( /*text, parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// HbTextItem::~HbTextItem
// -----------------------------------------------------------------------------
//
HbTextItem::~HbTextItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbTextItem::setText
// -----------------------------------------------------------------------------
//
void HbTextItem::setText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbTextItem::text
// -----------------------------------------------------------------------------
//
QString HbTextItem::text(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbTextItem::setTextColor
// -----------------------------------------------------------------------------
//
void HbTextItem::setTextColor( 
        const QColor & color )
    {
    SMC_MOCK_METHOD1( void, const QColor &, color )
    }


// -----------------------------------------------------------------------------
// HbTextItem::textColor
// -----------------------------------------------------------------------------
//
QColor HbTextItem::textColor(  ) const
    {
    SMC_MOCK_METHOD0( QColor )
    }


// -----------------------------------------------------------------------------
// HbTextItem::setAlignment
// -----------------------------------------------------------------------------
//
void HbTextItem::setAlignment( 
        Qt::Alignment alignment )
    {
    int align = alignment;
    SMC_MOCK_METHOD1( void, int, align )
    }


// -----------------------------------------------------------------------------
// HbTextItem::alignment
// -----------------------------------------------------------------------------
//
Qt::Alignment HbTextItem::alignment(  ) const
    {
    //SMC_MOCK_METHOD0( Qt::Alignment )
    }


// -----------------------------------------------------------------------------
// HbTextItem::setElideMode
// -----------------------------------------------------------------------------
//
void HbTextItem::setElideMode( 
        Qt::TextElideMode elideMode )
    {
    SMC_MOCK_METHOD1( void, Qt::TextElideMode, elideMode )
    }


// -----------------------------------------------------------------------------
// HbTextItem::elideMode
// -----------------------------------------------------------------------------
//
Qt::TextElideMode HbTextItem::elideMode(  ) const
    {
    SMC_MOCK_METHOD0( Qt::TextElideMode )
    }


// -----------------------------------------------------------------------------
// HbTextItem::setTextWrapping
// -----------------------------------------------------------------------------
//
void HbTextItem::setTextWrapping( 
        Hb::TextWrapping mode )
    {
    SMC_MOCK_METHOD1( void, Hb::TextWrapping, mode )
    }


// -----------------------------------------------------------------------------
// HbTextItem::textWrapping
// -----------------------------------------------------------------------------
//
Hb::TextWrapping HbTextItem::textWrapping(  ) const
    {
    SMC_MOCK_METHOD0( Hb::TextWrapping )
    }


// -----------------------------------------------------------------------------
// HbTextItem::setTextVisible
// -----------------------------------------------------------------------------
//
void HbTextItem::setTextVisible( 
        bool isVisible )
    {
    SMC_MOCK_METHOD1( void, bool, isVisible )
    }


// -----------------------------------------------------------------------------
// HbTextItem::isTextVisible
// -----------------------------------------------------------------------------
//
bool HbTextItem::isTextVisible(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbTextItem::setTextClip
// -----------------------------------------------------------------------------
//
void HbTextItem::setTextClip( 
        bool cliping )
    {
    SMC_MOCK_METHOD1( void, bool, cliping )
    }


// -----------------------------------------------------------------------------
// HbTextItem::isTextClip
// -----------------------------------------------------------------------------
//
bool HbTextItem::isTextClip(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbTextItem::type
// -----------------------------------------------------------------------------
//
/*int HbTextItem::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }*/


// -----------------------------------------------------------------------------
// HbTextItem::setGeometry
// -----------------------------------------------------------------------------
//
void HbTextItem::setGeometry( 
        const QRectF & rect )
    {
    SMC_MOCK_METHOD1( void, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// HbTextItem::boundingRect
// -----------------------------------------------------------------------------
//
QRectF HbTextItem::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// HbTextItem::setMinimumLines
// -----------------------------------------------------------------------------
//
void HbTextItem::setMinimumLines( 
        int minLines )
    {
    SMC_MOCK_METHOD1( void, int, minLines )
    }


// -----------------------------------------------------------------------------
// HbTextItem::setMaximumLines
// -----------------------------------------------------------------------------
//
void HbTextItem::setMaximumLines( 
        int maxLines )
    {
    SMC_MOCK_METHOD1( void, int, maxLines )
    }


// -----------------------------------------------------------------------------
// HbTextItem::minimumLines
// -----------------------------------------------------------------------------
//
int HbTextItem::minimumLines(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbTextItem::maximumLines
// -----------------------------------------------------------------------------
//
int HbTextItem::maximumLines(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbTextItem::paint
// -----------------------------------------------------------------------------
//
void HbTextItem::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// HbTextItem::fadeLengths
// -----------------------------------------------------------------------------
//
QPointF HbTextItem::fadeLengths(  ) const
    {
    SMC_MOCK_METHOD0( QPointF )
    }


// -----------------------------------------------------------------------------
// HbTextItem::setFadeLength
// -----------------------------------------------------------------------------
//
void HbTextItem::setFadeLength( 
        qreal length )
    {
    SMC_MOCK_METHOD1( void, qreal, length )
    }


// -----------------------------------------------------------------------------
// HbTextItem::setFadeLengths
// -----------------------------------------------------------------------------
//
void HbTextItem::setFadeLengths( 
        qreal xLength,
        qreal yLength )
    {
    SMC_MOCK_METHOD2( void, qreal, xLength, 
        qreal, yLength )
    }


// -----------------------------------------------------------------------------
// HbTextItem::setFadeLengths
// -----------------------------------------------------------------------------
//
void HbTextItem::setFadeLengths( 
        const QPointF & lengths )
    {
    SMC_MOCK_METHOD1( void, const QPointF &, lengths )
    }


// -----------------------------------------------------------------------------
// HbTextItem::sizeHint
// -----------------------------------------------------------------------------
//
QSizeF HbTextItem::sizeHint( 
        Qt::SizeHint which,
        const QSizeF & constraint ) const
    {
    SMC_MOCK_METHOD2( QSizeF, Qt::SizeHint, which, 
        const QSizeF &, constraint )
    }


// -----------------------------------------------------------------------------
// HbTextItem::resizeEvent
// -----------------------------------------------------------------------------
//
void HbTextItem::resizeEvent( 
        QGraphicsSceneResizeEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneResizeEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbTextItem::changeEvent
// -----------------------------------------------------------------------------
//
void HbTextItem::changeEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbTextItem::updateGeometry
// -----------------------------------------------------------------------------
//
void HbTextItem::updateGeometry(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbTextItem::HbTextItem
// -----------------------------------------------------------------------------
//
HbTextItem::HbTextItem( 
        HbTextItemPrivate & dd,
        QGraphicsItem * parent )
    //:
    //HbWidgetBase( /*dd, parent*/ )
    {
    
    }


