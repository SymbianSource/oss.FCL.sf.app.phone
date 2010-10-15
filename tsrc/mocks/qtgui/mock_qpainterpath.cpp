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
#include <qpainterpath.h>
#include <QTransform>
#include <QFont>

// NOTE! The following header include requires
//       INCLUDEPATH += /epoc32/include/mw/QtGui/private
//#include <qpainterpath_p.h>
class QPainterPathStrokerPrivate {};
class QPainterPathData : public QPainterPathPrivate {};

struct QPainterPathPrivateDeleter
{
    static inline void cleanup(QPainterPathPrivate *d)
    {
        // note - we must up-cast to QPainterPathData since QPainterPathPrivate
        // has a non-virtual destructor!
        if (d && !d->ref.deref())
            delete static_cast<QPainterPathData *>(d);
    }
};


// ============================ MEMBER FUNCTIONS ===============================


#ifndef QT_NO_DATASTREAM
// -----------------------------------------------------------------------------
// QPainterPathPrivate::operator<<
// -----------------------------------------------------------------------------
//
/*QDataStream & QPainterPathPrivate::operator<<( 
        QDataStream &,
        const QPainterPath & )
    {
    SMC_MOCK_METHOD2( QDataStream &, QDataStream, &, 
        const QPainterPath, & )
    }*/


// -----------------------------------------------------------------------------
// QPainterPathPrivate::operator>>
// -----------------------------------------------------------------------------
//
/*QDataStream & QPainterPathPrivate::operator>>( 
        QDataStream &,
        QPainterPath & )
    {
    SMC_MOCK_METHOD2( QDataStream &, QDataStream, &, 
        QPainterPath, & )
    }*/
#endif


// -----------------------------------------------------------------------------
// QPainterPathStroker::~QPainterPathStroker
// -----------------------------------------------------------------------------
//
QPainterPathStroker::~QPainterPathStroker(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::setWidth
// -----------------------------------------------------------------------------
//
void QPainterPathStroker::setWidth( 
        qreal width )
    {
    SMC_MOCK_METHOD1( void, qreal, width )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::width
// -----------------------------------------------------------------------------
//
qreal QPainterPathStroker::width(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::setCapStyle
// -----------------------------------------------------------------------------
//
void QPainterPathStroker::setCapStyle( 
        Qt::PenCapStyle style )
    {
    SMC_MOCK_METHOD1( void, Qt::PenCapStyle, style )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::capStyle
// -----------------------------------------------------------------------------
//
Qt::PenCapStyle QPainterPathStroker::capStyle(  ) const
    {
    SMC_MOCK_METHOD0( Qt::PenCapStyle )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::setJoinStyle
// -----------------------------------------------------------------------------
//
void QPainterPathStroker::setJoinStyle( 
        Qt::PenJoinStyle style )
    {
    SMC_MOCK_METHOD1( void, Qt::PenJoinStyle, style )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::joinStyle
// -----------------------------------------------------------------------------
//
Qt::PenJoinStyle QPainterPathStroker::joinStyle(  ) const
    {
    SMC_MOCK_METHOD0( Qt::PenJoinStyle )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::setMiterLimit
// -----------------------------------------------------------------------------
//
void QPainterPathStroker::setMiterLimit( 
        qreal length )
    {
    SMC_MOCK_METHOD1( void, qreal, length )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::miterLimit
// -----------------------------------------------------------------------------
//
qreal QPainterPathStroker::miterLimit(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::setCurveThreshold
// -----------------------------------------------------------------------------
//
void QPainterPathStroker::setCurveThreshold( 
        qreal threshold )
    {
    SMC_MOCK_METHOD1( void, qreal, threshold )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::curveThreshold
// -----------------------------------------------------------------------------
//
qreal QPainterPathStroker::curveThreshold(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::setDashPattern
// -----------------------------------------------------------------------------
//
void QPainterPathStroker::setDashPattern( 
        Qt::PenStyle style )
    {
    SMC_MOCK_METHOD1( void, Qt::PenStyle, style )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::setDashPattern
// -----------------------------------------------------------------------------
//
void QPainterPathStroker::setDashPattern( 
        const QVector<qreal> & dashPattern )
    {
    SMC_MOCK_METHOD1( void, const QVector<qreal> &, dashPattern )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::dashPattern
// -----------------------------------------------------------------------------
//
QVector <qreal > QPainterPathStroker::dashPattern(  ) const
    {
    SMC_MOCK_METHOD0( QVector <qreal > )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::setDashOffset
// -----------------------------------------------------------------------------
//
void QPainterPathStroker::setDashOffset( 
        qreal offset )
    {
    SMC_MOCK_METHOD1( void, qreal, offset )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::dashOffset
// -----------------------------------------------------------------------------
//
qreal QPainterPathStroker::dashOffset(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QPainterPathStroker::createStroke
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPathStroker::createStroke( 
        const QPainterPath & path ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, path )
    }


// -----------------------------------------------------------------------------
// QPainterPath::QPainterPath
// -----------------------------------------------------------------------------
//
QPainterPath::QPainterPath(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QPainterPath::QPainterPath
// -----------------------------------------------------------------------------
//
QPainterPath::QPainterPath( 
        const QPointF & startPoint )
    {
    
    }


// -----------------------------------------------------------------------------
// QPainterPath::QPainterPath
// -----------------------------------------------------------------------------
//
QPainterPath::QPainterPath( 
        const QPainterPath & other )
    {
    
    }


// -----------------------------------------------------------------------------
// QPainterPath::operator=
// -----------------------------------------------------------------------------
//
QPainterPath & QPainterPath::operator=( 
        const QPainterPath & other )
    {
    SMC_MOCK_METHOD1( QPainterPath &, const QPainterPath &, other )
    }


// -----------------------------------------------------------------------------
// QPainterPath::~QPainterPath
// -----------------------------------------------------------------------------
//
QPainterPath::~QPainterPath(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QPainterPath::closeSubpath
// -----------------------------------------------------------------------------
//
void QPainterPath::closeSubpath(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QPainterPath::moveTo
// -----------------------------------------------------------------------------
//
void QPainterPath::moveTo( 
        const QPointF & p )
    {
    SMC_MOCK_METHOD1( void, const QPointF &, p )
    }


// -----------------------------------------------------------------------------
// QPainterPath::lineTo
// -----------------------------------------------------------------------------
//
void QPainterPath::lineTo( 
        const QPointF & p )
    {
    SMC_MOCK_METHOD1( void, const QPointF &, p )
    }


// -----------------------------------------------------------------------------
// QPainterPath::arcMoveTo
// -----------------------------------------------------------------------------
//
void QPainterPath::arcMoveTo( 
        const QRectF & rect,
        qreal angle )
    {
    SMC_MOCK_METHOD2( void, const QRectF &, rect, 
        qreal, angle )
    }


// -----------------------------------------------------------------------------
// QPainterPath::arcTo
// -----------------------------------------------------------------------------
//
void QPainterPath::arcTo( 
        const QRectF & rect,
        qreal startAngle,
        qreal arcLength )
    {
    SMC_MOCK_METHOD3( void, const QRectF &, rect, 
        qreal, startAngle, 
        qreal, arcLength )
    }


// -----------------------------------------------------------------------------
// QPainterPath::cubicTo
// -----------------------------------------------------------------------------
//
void QPainterPath::cubicTo( 
        const QPointF & ctrlPt1,
        const QPointF & ctrlPt2,
        const QPointF & endPt )
    {
    SMC_MOCK_METHOD3( void, const QPointF &, ctrlPt1, 
        const QPointF &, ctrlPt2, 
        const QPointF &, endPt )
    }


// -----------------------------------------------------------------------------
// QPainterPath::quadTo
// -----------------------------------------------------------------------------
//
void QPainterPath::quadTo( 
        const QPointF & ctrlPt,
        const QPointF & endPt )
    {
    SMC_MOCK_METHOD2( void, const QPointF &, ctrlPt, 
        const QPointF &, endPt )
    }


// -----------------------------------------------------------------------------
// QPainterPath::currentPosition
// -----------------------------------------------------------------------------
//
QPointF QPainterPath::currentPosition(  ) const
    {
    SMC_MOCK_METHOD0( QPointF )
    }


// -----------------------------------------------------------------------------
// QPainterPath::addRect
// -----------------------------------------------------------------------------
//
void QPainterPath::addRect( 
        const QRectF & rect )
    {
    SMC_MOCK_METHOD1( void, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QPainterPath::addEllipse
// -----------------------------------------------------------------------------
//
void QPainterPath::addEllipse( 
        const QRectF & rect )
    {
    SMC_MOCK_METHOD1( void, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QPainterPath::addPolygon
// -----------------------------------------------------------------------------
//
void QPainterPath::addPolygon( 
        const QPolygonF & polygon )
    {
    SMC_MOCK_METHOD1( void, const QPolygonF &, polygon )
    }


// -----------------------------------------------------------------------------
// QPainterPath::addText
// -----------------------------------------------------------------------------
//
void QPainterPath::addText( 
        const QPointF & point,
        const QFont & f,
        const QString & text )
    {
    SMC_MOCK_METHOD3( void, const QPointF &, point, 
        const QFont &, f, 
        const QString &, text )
    }


// -----------------------------------------------------------------------------
// QPainterPath::addPath
// -----------------------------------------------------------------------------
//
void QPainterPath::addPath( 
        const QPainterPath & path )
    {
    SMC_MOCK_METHOD1( void, const QPainterPath &, path )
    }


// -----------------------------------------------------------------------------
// QPainterPath::addRegion
// -----------------------------------------------------------------------------
//
void QPainterPath::addRegion( 
        const QRegion & region )
    {
    SMC_MOCK_METHOD1( void, const QRegion &, region )
    }


// -----------------------------------------------------------------------------
// QPainterPath::addRoundedRect
// -----------------------------------------------------------------------------
//
void QPainterPath::addRoundedRect( 
        const QRectF & rect,
        qreal xRadius,
        qreal yRadius,
        Qt::SizeMode mode )
    {
    SMC_MOCK_METHOD4( void, const QRectF &, rect, 
        qreal, xRadius, 
        qreal, yRadius, 
        Qt::SizeMode, mode )
    }


// -----------------------------------------------------------------------------
// QPainterPath::addRoundRect
// -----------------------------------------------------------------------------
//
void QPainterPath::addRoundRect( 
        const QRectF & rect,
        int xRnd,
        int yRnd )
    {
    SMC_MOCK_METHOD3( void, const QRectF &, rect, 
        int, xRnd, 
        int, yRnd )
    }


// -----------------------------------------------------------------------------
// QPainterPath::connectPath
// -----------------------------------------------------------------------------
//
void QPainterPath::connectPath( 
        const QPainterPath & path )
    {
    SMC_MOCK_METHOD1( void, const QPainterPath &, path )
    }


// -----------------------------------------------------------------------------
// QPainterPath::contains
// -----------------------------------------------------------------------------
//
bool QPainterPath::contains( 
        const QPointF & pt ) const
    {
    SMC_MOCK_METHOD1( bool, const QPointF &, pt )
    }


// -----------------------------------------------------------------------------
// QPainterPath::contains
// -----------------------------------------------------------------------------
//
bool QPainterPath::contains( 
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD1( bool, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QPainterPath::intersects
// -----------------------------------------------------------------------------
//
bool QPainterPath::intersects( 
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD1( bool, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QPainterPath::translate
// -----------------------------------------------------------------------------
//
void QPainterPath::translate( 
        qreal dx,
        qreal dy )
    {
    SMC_MOCK_METHOD2( void, qreal, dx, 
        qreal, dy )
    }


// -----------------------------------------------------------------------------
// QPainterPath::translated
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPath::translated( 
        qreal dx,
        qreal dy ) const
    {
    SMC_MOCK_METHOD2( QPainterPath, qreal, dx, 
        qreal, dy )
    }


// -----------------------------------------------------------------------------
// QPainterPath::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QPainterPath::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QPainterPath::controlPointRect
// -----------------------------------------------------------------------------
//
QRectF QPainterPath::controlPointRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QPainterPath::fillRule
// -----------------------------------------------------------------------------
//
Qt::FillRule QPainterPath::fillRule(  ) const
    {
    SMC_MOCK_METHOD0( Qt::FillRule )
    }


// -----------------------------------------------------------------------------
// QPainterPath::setFillRule
// -----------------------------------------------------------------------------
//
void QPainterPath::setFillRule( 
        Qt::FillRule fillRule )
    {
    SMC_MOCK_METHOD1( void, Qt::FillRule, fillRule )
    }


// -----------------------------------------------------------------------------
// QPainterPath::toReversed
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPath::toReversed(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QPainterPath::toSubpathPolygons
// -----------------------------------------------------------------------------
//
QList <QPolygonF > QPainterPath::toSubpathPolygons( 
        const QMatrix & matrix ) const
    {
    SMC_MOCK_METHOD1( QList <QPolygonF >, const QMatrix &, matrix )
    }


// -----------------------------------------------------------------------------
// QPainterPath::toFillPolygons
// -----------------------------------------------------------------------------
//
QList <QPolygonF > QPainterPath::toFillPolygons( 
        const QMatrix & matrix ) const
    {
    SMC_MOCK_METHOD1( QList <QPolygonF >, const QMatrix &, matrix )
    }


// -----------------------------------------------------------------------------
// QPainterPath::toFillPolygon
// -----------------------------------------------------------------------------
//
QPolygonF QPainterPath::toFillPolygon( 
        const QMatrix & matrix ) const
    {
    SMC_MOCK_METHOD1( QPolygonF, const QMatrix &, matrix )
    }


// -----------------------------------------------------------------------------
// QPainterPath::toSubpathPolygons
// -----------------------------------------------------------------------------
//
QList <QPolygonF > QPainterPath::toSubpathPolygons( 
        const QTransform & matrix ) const
    {
    SMC_MOCK_METHOD1( QList <QPolygonF >, const QTransform &, matrix )
    }


// -----------------------------------------------------------------------------
// QPainterPath::toFillPolygons
// -----------------------------------------------------------------------------
//
QList <QPolygonF > QPainterPath::toFillPolygons( 
        const QTransform & matrix ) const
    {
    SMC_MOCK_METHOD1( QList <QPolygonF >, const QTransform &, matrix )
    }


// -----------------------------------------------------------------------------
// QPainterPath::toFillPolygon
// -----------------------------------------------------------------------------
//
QPolygonF QPainterPath::toFillPolygon( 
        const QTransform & matrix ) const
    {
    SMC_MOCK_METHOD1( QPolygonF, const QTransform &, matrix )
    }


// -----------------------------------------------------------------------------
// QPainterPath::length
// -----------------------------------------------------------------------------
//
qreal QPainterPath::length(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QPainterPath::percentAtLength
// -----------------------------------------------------------------------------
//
qreal QPainterPath::percentAtLength( 
        qreal t ) const
    {
    SMC_MOCK_METHOD1( qreal, qreal, t )
    }


// -----------------------------------------------------------------------------
// QPainterPath::pointAtPercent
// -----------------------------------------------------------------------------
//
QPointF QPainterPath::pointAtPercent( 
        qreal t ) const
    {
    SMC_MOCK_METHOD1( QPointF, qreal, t )
    }


// -----------------------------------------------------------------------------
// QPainterPath::angleAtPercent
// -----------------------------------------------------------------------------
//
qreal QPainterPath::angleAtPercent( 
        qreal t ) const
    {
    SMC_MOCK_METHOD1( qreal, qreal, t )
    }


// -----------------------------------------------------------------------------
// QPainterPath::slopeAtPercent
// -----------------------------------------------------------------------------
//
qreal QPainterPath::slopeAtPercent( 
        qreal t ) const
    {
    SMC_MOCK_METHOD1( qreal, qreal, t )
    }


// -----------------------------------------------------------------------------
// QPainterPath::intersects
// -----------------------------------------------------------------------------
//
bool QPainterPath::intersects( 
        const QPainterPath & p ) const
    {
    SMC_MOCK_METHOD1( bool, const QPainterPath &, p )
    }


// -----------------------------------------------------------------------------
// QPainterPath::contains
// -----------------------------------------------------------------------------
//
bool QPainterPath::contains( 
        const QPainterPath & p ) const
    {
    SMC_MOCK_METHOD1( bool, const QPainterPath &, p )
    }


// -----------------------------------------------------------------------------
// QPainterPath::united
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPath::united( 
        const QPainterPath & r ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, r )
    }


// -----------------------------------------------------------------------------
// QPainterPath::intersected
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPath::intersected( 
        const QPainterPath & r ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, r )
    }


// -----------------------------------------------------------------------------
// QPainterPath::subtracted
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPath::subtracted( 
        const QPainterPath & r ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, r )
    }


// -----------------------------------------------------------------------------
// QPainterPath::subtractedInverted
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPath::subtractedInverted( 
        const QPainterPath & r ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, r )
    }


// -----------------------------------------------------------------------------
// QPainterPath::simplified
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPath::simplified(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QPainterPath::operator==
// -----------------------------------------------------------------------------
//
bool QPainterPath::operator==( 
        const QPainterPath & other ) const
    {
    SMC_MOCK_METHOD1( bool, const QPainterPath &, other )
    }


// -----------------------------------------------------------------------------
// QPainterPath::operator!=
// -----------------------------------------------------------------------------
//
bool QPainterPath::operator!=( 
        const QPainterPath & other ) const
    {
    SMC_MOCK_METHOD1( bool, const QPainterPath &, other )
    }


// -----------------------------------------------------------------------------
// QPainterPath::operator&
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPath::operator&( 
        const QPainterPath & other ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, other )
    }


// -----------------------------------------------------------------------------
// QPainterPath::operator|
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPath::operator|( 
        const QPainterPath & other ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, other )
    }


// -----------------------------------------------------------------------------
// QPainterPath::operator+
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPath::operator+( 
        const QPainterPath & other ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, other )
    }


// -----------------------------------------------------------------------------
// QPainterPath::operator-
// -----------------------------------------------------------------------------
//
QPainterPath QPainterPath::operator-( 
        const QPainterPath & other ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, other )
    }


// -----------------------------------------------------------------------------
// QPainterPath::operator&=
// -----------------------------------------------------------------------------
//
QPainterPath & QPainterPath::operator&=( 
        const QPainterPath & other )
    {
    SMC_MOCK_METHOD1( QPainterPath &, const QPainterPath &, other )
    }


// -----------------------------------------------------------------------------
// QPainterPath::operator=
// -----------------------------------------------------------------------------
//
/*QPainterPath & QPainterPath::operator=( 
        const QPainterPath & other )
    {
    SMC_MOCK_METHOD1( QPainterPath &, const QPainterPath &, other )
    }*/


// -----------------------------------------------------------------------------
// QPainterPath::operator+=
// -----------------------------------------------------------------------------
//
QPainterPath & QPainterPath::operator+=( 
        const QPainterPath & other )
    {
    SMC_MOCK_METHOD1( QPainterPath &, const QPainterPath &, other )
    }


// -----------------------------------------------------------------------------
// QPainterPath::operator-=
// -----------------------------------------------------------------------------
//
QPainterPath & QPainterPath::operator-=( 
        const QPainterPath & other )
    {
    SMC_MOCK_METHOD1( QPainterPath &, const QPainterPath &, other )
    }


