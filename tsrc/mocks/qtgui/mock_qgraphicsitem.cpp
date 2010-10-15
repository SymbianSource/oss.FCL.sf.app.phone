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
#include <qgraphicsitem.h>

#include <QtCore/qglobal.h>
#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>
#include <QtCore/qrect.h>
//#include <QtCore/qscopedpointer.h>
#include <QtGui/qpainterpath.h>
#include <QtGui/qpixmap.h>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QTextCursor>
#include <QCursor>
#include <QTextDocument>
#include <QTransform>

#include <QtGui/private/qgraphicsitem_p.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// QGraphicsItemGroup::QGraphicsItemGroup
// -----------------------------------------------------------------------------
//
QGraphicsItemGroup::QGraphicsItemGroup( 
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QGraphicsItem( /*parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsItemGroup::~QGraphicsItemGroup
// -----------------------------------------------------------------------------
//
QGraphicsItemGroup::~QGraphicsItemGroup(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsItemGroup::addToGroup
// -----------------------------------------------------------------------------
//
void QGraphicsItemGroup::addToGroup( 
        QGraphicsItem * item )
    {
    SMC_MOCK_METHOD1( void, QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsItemGroup::removeFromGroup
// -----------------------------------------------------------------------------
//
void QGraphicsItemGroup::removeFromGroup( 
        QGraphicsItem * item )
    {
    SMC_MOCK_METHOD1( void, QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsItemGroup::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsItemGroup::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsItemGroup::paint
// -----------------------------------------------------------------------------
//
void QGraphicsItemGroup::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsItemGroup::isObscuredBy
// -----------------------------------------------------------------------------
//
bool QGraphicsItemGroup::isObscuredBy( 
        const QGraphicsItem * item ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsItemGroup::opaqueArea
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsItemGroup::opaqueArea(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsItemGroup::type
// -----------------------------------------------------------------------------
//
int QGraphicsItemGroup::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QAbstractGraphicsShapeItem::QAbstractGraphicsShapeItem
// -----------------------------------------------------------------------------
//
QAbstractGraphicsShapeItem::QAbstractGraphicsShapeItem( 
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QGraphicsItem( /*parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QAbstractGraphicsShapeItem::~QAbstractGraphicsShapeItem
// -----------------------------------------------------------------------------
//
QAbstractGraphicsShapeItem::~QAbstractGraphicsShapeItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QAbstractGraphicsShapeItem::pen
// -----------------------------------------------------------------------------
//
QPen QAbstractGraphicsShapeItem::pen(  ) const
    {
    SMC_MOCK_METHOD0( QPen )
    }


// -----------------------------------------------------------------------------
// QAbstractGraphicsShapeItem::setPen
// -----------------------------------------------------------------------------
//
void QAbstractGraphicsShapeItem::setPen( 
        const QPen & pen )
    {
    SMC_MOCK_METHOD1( void, const QPen &, pen )
    }


// -----------------------------------------------------------------------------
// QAbstractGraphicsShapeItem::brush
// -----------------------------------------------------------------------------
//
QBrush QAbstractGraphicsShapeItem::brush(  ) const
    {
    SMC_MOCK_METHOD0( QBrush )
    }


// -----------------------------------------------------------------------------
// QAbstractGraphicsShapeItem::setBrush
// -----------------------------------------------------------------------------
//
void QAbstractGraphicsShapeItem::setBrush( 
        const QBrush & brush )
    {
    SMC_MOCK_METHOD1( void, const QBrush &, brush )
    }


// -----------------------------------------------------------------------------
// QAbstractGraphicsShapeItem::isObscuredBy
// -----------------------------------------------------------------------------
//
bool QAbstractGraphicsShapeItem::isObscuredBy( 
        const QGraphicsItem * item ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QAbstractGraphicsShapeItem::opaqueArea
// -----------------------------------------------------------------------------
//
QPainterPath QAbstractGraphicsShapeItem::opaqueArea(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QAbstractGraphicsShapeItem::QAbstractGraphicsShapeItem
// -----------------------------------------------------------------------------
//
QAbstractGraphicsShapeItem::QAbstractGraphicsShapeItem( 
        QAbstractGraphicsShapeItemPrivate & dd,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QGraphicsItem( /*dd, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::QGraphicsLineItem
// -----------------------------------------------------------------------------
//
QGraphicsLineItem::QGraphicsLineItem( 
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QGraphicsItem( /*parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::QGraphicsLineItem
// -----------------------------------------------------------------------------
//
QGraphicsLineItem::QGraphicsLineItem( 
        const QLineF & line,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QGraphicsItem( /*line, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::QGraphicsLineItem
// -----------------------------------------------------------------------------
//
QGraphicsLineItem::QGraphicsLineItem( 
        qreal x1,
        qreal y1,
        qreal x2,
        qreal y2,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QGraphicsItem( /*x1, y1, x2, y2, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::~QGraphicsLineItem
// -----------------------------------------------------------------------------
//
QGraphicsLineItem::~QGraphicsLineItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::pen
// -----------------------------------------------------------------------------
//
QPen QGraphicsLineItem::pen(  ) const
    {
    SMC_MOCK_METHOD0( QPen )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::setPen
// -----------------------------------------------------------------------------
//
void QGraphicsLineItem::setPen( 
        const QPen & pen )
    {
    SMC_MOCK_METHOD1( void, const QPen &, pen )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::line
// -----------------------------------------------------------------------------
//
QLineF QGraphicsLineItem::line(  ) const
    {
    SMC_MOCK_METHOD0( QLineF )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::setLine
// -----------------------------------------------------------------------------
//
void QGraphicsLineItem::setLine( 
        const QLineF & line )
    {
    SMC_MOCK_METHOD1( void, const QLineF &, line )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsLineItem::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::shape
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsLineItem::shape(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::contains
// -----------------------------------------------------------------------------
//
bool QGraphicsLineItem::contains( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( bool, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::paint
// -----------------------------------------------------------------------------
//
void QGraphicsLineItem::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::isObscuredBy
// -----------------------------------------------------------------------------
//
bool QGraphicsLineItem::isObscuredBy( 
        const QGraphicsItem * item ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::opaqueArea
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsLineItem::opaqueArea(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::type
// -----------------------------------------------------------------------------
//
int QGraphicsLineItem::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::supportsExtension
// -----------------------------------------------------------------------------
//
bool QGraphicsLineItem::supportsExtension( 
        Extension extension ) const
    {
    SMC_MOCK_METHOD1( bool, Extension, extension )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::setExtension
// -----------------------------------------------------------------------------
//
void QGraphicsLineItem::setExtension( 
        Extension extension,
        const QVariant & variant )
    {
    SMC_MOCK_METHOD2( void, Extension, extension, 
        const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsLineItem::extension
// -----------------------------------------------------------------------------
//
QVariant QGraphicsLineItem::extension( 
        const QVariant & variant ) const
    {
    SMC_MOCK_METHOD1( QVariant, const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::QGraphicsTextItem
// -----------------------------------------------------------------------------
//
QGraphicsTextItem::QGraphicsTextItem( 
        const QString & text,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QGraphicsObject( /*text, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::~QGraphicsTextItem
// -----------------------------------------------------------------------------
//
QGraphicsTextItem::~QGraphicsTextItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::toHtml
// -----------------------------------------------------------------------------
//
QString QGraphicsTextItem::toHtml(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::setHtml
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::setHtml( 
        const QString & html )
    {
    SMC_MOCK_METHOD1( void, const QString &, html )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::toPlainText
// -----------------------------------------------------------------------------
//
QString QGraphicsTextItem::toPlainText(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::setPlainText
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::setPlainText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::font
// -----------------------------------------------------------------------------
//
QFont QGraphicsTextItem::font(  ) const
    {
    SMC_MOCK_METHOD0( QFont )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::setFont
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::setFont( 
        const QFont & font )
    {
    SMC_MOCK_METHOD1( void, const QFont &, font )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::setDefaultTextColor
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::setDefaultTextColor( 
        const QColor & c )
    {
    SMC_MOCK_METHOD1( void, const QColor &, c )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::defaultTextColor
// -----------------------------------------------------------------------------
//
QColor QGraphicsTextItem::defaultTextColor(  ) const
    {
    SMC_MOCK_METHOD0( QColor )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsTextItem::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::shape
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsTextItem::shape(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::contains
// -----------------------------------------------------------------------------
//
bool QGraphicsTextItem::contains( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( bool, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::paint
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::isObscuredBy
// -----------------------------------------------------------------------------
//
bool QGraphicsTextItem::isObscuredBy( 
        const QGraphicsItem * item ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::opaqueArea
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsTextItem::opaqueArea(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::type
// -----------------------------------------------------------------------------
//
int QGraphicsTextItem::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::setTextWidth
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::setTextWidth( 
        qreal width )
    {
    SMC_MOCK_METHOD1( void, qreal, width )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::textWidth
// -----------------------------------------------------------------------------
//
qreal QGraphicsTextItem::textWidth(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::adjustSize
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::adjustSize(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::setDocument
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::setDocument( 
        QTextDocument * document )
    {
    SMC_MOCK_METHOD1( void, QTextDocument *, document )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::document
// -----------------------------------------------------------------------------
//
QTextDocument * QGraphicsTextItem::document(  ) const
    {
    SMC_MOCK_METHOD0( QTextDocument * )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::setTextInteractionFlags
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::setTextInteractionFlags( 
        Qt::TextInteractionFlags flags )
    {
    //SMC_MOCK_METHOD1( void, tiFlag, flags )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::textInteractionFlags
// -----------------------------------------------------------------------------
//
Qt::TextInteractionFlags QGraphicsTextItem::textInteractionFlags(  ) const
    {
    typedef QFlags<Qt::TextInteractionFlag> tiFlag;
    SMC_MOCK_METHOD0( tiFlag )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::setTabChangesFocus
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::setTabChangesFocus( 
        bool b )
    {
    SMC_MOCK_METHOD1( void, bool, b )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::tabChangesFocus
// -----------------------------------------------------------------------------
//
bool QGraphicsTextItem::tabChangesFocus(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::setOpenExternalLinks
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::setOpenExternalLinks( 
        bool open )
    {
    SMC_MOCK_METHOD1( void, bool, open )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::openExternalLinks
// -----------------------------------------------------------------------------
//
bool QGraphicsTextItem::openExternalLinks(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::setTextCursor
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::setTextCursor( 
        const QTextCursor & cursor )
    {
    SMC_MOCK_METHOD1( void, const QTextCursor &, cursor )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::textCursor
// -----------------------------------------------------------------------------
//
QTextCursor QGraphicsTextItem::textCursor(  ) const
    {
    SMC_MOCK_METHOD0( QTextCursor )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::linkActivated
// -----------------------------------------------------------------------------
//
/*void QGraphicsTextItem::linkActivated( 
        const QString & )
    {
    SMC_MOCK_METHOD1( void, const QString, & )
    }*/


// -----------------------------------------------------------------------------
// QGraphicsTextItem::linkHovered
// -----------------------------------------------------------------------------
//
/*void QGraphicsTextItem::linkHovered( 
        const QString & )
    {
    SMC_MOCK_METHOD1( void, const QString, & )
    }*/


// -----------------------------------------------------------------------------
// QGraphicsTextItem::sceneEvent
// -----------------------------------------------------------------------------
//
bool QGraphicsTextItem::sceneEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( bool, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::mousePressEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::mousePressEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneMouseEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::mouseMoveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::mouseMoveEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneMouseEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::mouseReleaseEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::mouseReleaseEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneMouseEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::mouseDoubleClickEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::mouseDoubleClickEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneMouseEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::contextMenuEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::contextMenuEvent( 
        QGraphicsSceneContextMenuEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneContextMenuEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::keyPressEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::keyPressEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::keyReleaseEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::keyReleaseEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::focusInEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::focusInEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::focusOutEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::focusOutEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::dragEnterEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::dragEnterEvent( 
        QGraphicsSceneDragDropEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneDragDropEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::dragLeaveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::dragLeaveEvent( 
        QGraphicsSceneDragDropEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneDragDropEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::dragMoveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::dragMoveEvent( 
        QGraphicsSceneDragDropEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneDragDropEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::dropEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::dropEvent( 
        QGraphicsSceneDragDropEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneDragDropEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::inputMethodEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::inputMethodEvent( 
        QInputMethodEvent * event )
    {
    SMC_MOCK_METHOD1( void, QInputMethodEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::hoverEnterEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::hoverEnterEvent( 
        QGraphicsSceneHoverEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneHoverEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::hoverMoveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::hoverMoveEvent( 
        QGraphicsSceneHoverEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneHoverEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::hoverLeaveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::hoverLeaveEvent( 
        QGraphicsSceneHoverEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneHoverEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::inputMethodQuery
// -----------------------------------------------------------------------------
//
QVariant QGraphicsTextItem::inputMethodQuery( 
        Qt::InputMethodQuery query ) const
    {
    SMC_MOCK_METHOD1( QVariant, Qt::InputMethodQuery, query )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::supportsExtension
// -----------------------------------------------------------------------------
//
bool QGraphicsTextItem::supportsExtension( 
        Extension extension ) const
    {
    SMC_MOCK_METHOD1( bool, Extension, extension )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::setExtension
// -----------------------------------------------------------------------------
//
void QGraphicsTextItem::setExtension( 
        Extension extension,
        const QVariant & variant )
    {
    SMC_MOCK_METHOD2( void, Extension, extension, 
        const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsTextItem::extension
// -----------------------------------------------------------------------------
//
QVariant QGraphicsTextItem::extension( 
        const QVariant & variant ) const
    {
    SMC_MOCK_METHOD1( QVariant, const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::QGraphicsItem
// -----------------------------------------------------------------------------
//
QGraphicsItem::QGraphicsItem( 
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::~QGraphicsItem
// -----------------------------------------------------------------------------
//
QGraphicsItem::~QGraphicsItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::scene
// -----------------------------------------------------------------------------
//
QGraphicsScene * QGraphicsItem::scene(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsScene * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::parentItem
// -----------------------------------------------------------------------------
//
QGraphicsItem * QGraphicsItem::parentItem(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsItem * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::topLevelItem
// -----------------------------------------------------------------------------
//
QGraphicsItem * QGraphicsItem::topLevelItem(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsItem * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::parentObject
// -----------------------------------------------------------------------------
//
QGraphicsObject * QGraphicsItem::parentObject(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsObject * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::parentWidget
// -----------------------------------------------------------------------------
//
QGraphicsWidget * QGraphicsItem::parentWidget(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsWidget * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::topLevelWidget
// -----------------------------------------------------------------------------
//
QGraphicsWidget * QGraphicsItem::topLevelWidget(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsWidget * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::window
// -----------------------------------------------------------------------------
//
QGraphicsWidget * QGraphicsItem::window(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsWidget * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::panel
// -----------------------------------------------------------------------------
//
QGraphicsItem * QGraphicsItem::panel(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsItem * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setParentItem
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setParentItem( 
        QGraphicsItem * parent )
    {
    SMC_MOCK_METHOD1( void, QGraphicsItem *, parent )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::children
// -----------------------------------------------------------------------------
//
QList <QGraphicsItem * > QGraphicsItem::children(  ) const
    {
    SMC_MOCK_METHOD0( QList <QGraphicsItem * > )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::childItems
// -----------------------------------------------------------------------------
//
QList <QGraphicsItem * > QGraphicsItem::childItems(  ) const
    {
    SMC_MOCK_METHOD0( QList <QGraphicsItem * > )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isWidget
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isWidget(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isWindow
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isWindow(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isPanel
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isPanel(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::toGraphicsObject
// -----------------------------------------------------------------------------
//
QGraphicsObject * QGraphicsItem::toGraphicsObject(  )
    {
    SMC_MOCK_METHOD0( QGraphicsObject * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::toGraphicsObject
// -----------------------------------------------------------------------------
//
const QGraphicsObject * QGraphicsItem::toGraphicsObject(  ) const
    {
    SMC_MOCK_METHOD0( const QGraphicsObject * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::group
// -----------------------------------------------------------------------------
//
QGraphicsItemGroup * QGraphicsItem::group(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsItemGroup * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setGroup
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setGroup( 
        QGraphicsItemGroup * group )
    {
    SMC_MOCK_METHOD1( void, QGraphicsItemGroup *, group )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::flags
// -----------------------------------------------------------------------------
//
QGraphicsItem::GraphicsItemFlags QGraphicsItem::flags(  ) const
    {
    //SMC_MOCK_METHOD0( QGraphicsItem::GraphicsItemFlags )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setFlag
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setFlag( 
        GraphicsItemFlag flag,
        bool enabled )
    {
    SMC_MOCK_METHOD2( void, GraphicsItemFlag, flag, 
        bool, enabled )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setFlags
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setFlags( 
        QGraphicsItem::GraphicsItemFlags flags )
    {
    //SMC_MOCK_METHOD1( void, GraphicsItemFlags, flags )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::cacheMode
// -----------------------------------------------------------------------------
//
QGraphicsItem::CacheMode QGraphicsItem::cacheMode(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsItem::CacheMode )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setCacheMode
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setCacheMode( 
        QGraphicsItem::CacheMode mode,
        const QSize & cacheSize )
    {
    SMC_MOCK_METHOD2( void, QGraphicsItem::CacheMode, mode, 
        const QSize &, cacheSize )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::panelModality
// -----------------------------------------------------------------------------
//
QGraphicsItem::PanelModality QGraphicsItem::panelModality(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsItem::PanelModality )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setPanelModality
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setPanelModality( 
        QGraphicsItem::PanelModality panelModality )
    {
    SMC_MOCK_METHOD1( void, QGraphicsItem::PanelModality, panelModality )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isBlockedByModalPanel
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isBlockedByModalPanel( 
        QGraphicsItem * * blockingPanel ) const
    {
    SMC_MOCK_METHOD1( bool, QGraphicsItem * *, blockingPanel )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::toolTip
// -----------------------------------------------------------------------------
//
QString QGraphicsItem::toolTip(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setToolTip
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setToolTip( 
        const QString & toolTip )
    {
    SMC_MOCK_METHOD1( void, const QString &, toolTip )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::cursor
// -----------------------------------------------------------------------------
//
QCursor QGraphicsItem::cursor(  ) const
    {
    SMC_MOCK_METHOD0( QCursor )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setCursor
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setCursor( 
        const QCursor & cursor )
    {
    SMC_MOCK_METHOD1( void, const QCursor &, cursor )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::hasCursor
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::hasCursor(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::unsetCursor
// -----------------------------------------------------------------------------
//
void QGraphicsItem::unsetCursor(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isVisible
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isVisible(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isVisibleTo
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isVisibleTo( 
        const QGraphicsItem * parent ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, parent )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setVisible
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setVisible( 
        bool visible )
    {
    SMC_MOCK_METHOD1( void, bool, visible )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isEnabled
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isEnabled(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setEnabled
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setEnabled( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isSelected
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isSelected(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setSelected
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setSelected( 
        bool selected )
    {
    SMC_MOCK_METHOD1( void, bool, selected )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::acceptDrops
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::acceptDrops(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setAcceptDrops
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setAcceptDrops( 
        bool on )
    {
    SMC_MOCK_METHOD1( void, bool, on )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::opacity
// -----------------------------------------------------------------------------
//
qreal QGraphicsItem::opacity(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::effectiveOpacity
// -----------------------------------------------------------------------------
//
qreal QGraphicsItem::effectiveOpacity(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setOpacity
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setOpacity( 
        qreal opacity )
    {
    SMC_MOCK_METHOD1( void, qreal, opacity )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::graphicsEffect
// -----------------------------------------------------------------------------
//
QGraphicsEffect * QGraphicsItem::graphicsEffect(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsEffect * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setGraphicsEffect
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setGraphicsEffect( 
        QGraphicsEffect * effect )
    {
    SMC_MOCK_METHOD1( void, QGraphicsEffect *, effect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::acceptedMouseButtons
// -----------------------------------------------------------------------------
//
Qt::MouseButtons QGraphicsItem::acceptedMouseButtons(  ) const
    {
    //SMC_MOCK_METHOD0( Qt::MouseButtons )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setAcceptedMouseButtons
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setAcceptedMouseButtons( 
        Qt::MouseButtons buttons )
    {
    //SMC_MOCK_METHOD1( void, Qt::MouseButtons, buttons )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::acceptsHoverEvents
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::acceptsHoverEvents(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setAcceptsHoverEvents
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setAcceptsHoverEvents( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::acceptHoverEvents
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::acceptHoverEvents(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setAcceptHoverEvents
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setAcceptHoverEvents( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::acceptTouchEvents
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::acceptTouchEvents(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setAcceptTouchEvents
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setAcceptTouchEvents( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::filtersChildEvents
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::filtersChildEvents(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setFiltersChildEvents
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setFiltersChildEvents( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::handlesChildEvents
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::handlesChildEvents(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setHandlesChildEvents
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setHandlesChildEvents( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isActive
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isActive(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setActive
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setActive( 
        bool active )
    {
    SMC_MOCK_METHOD1( void, bool, active )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::hasFocus
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::hasFocus(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setFocus
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setFocus( 
        Qt::FocusReason focusReason )
    {
    SMC_MOCK_METHOD1( void, Qt::FocusReason, focusReason )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::clearFocus
// -----------------------------------------------------------------------------
//
void QGraphicsItem::clearFocus(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::focusProxy
// -----------------------------------------------------------------------------
//
QGraphicsItem * QGraphicsItem::focusProxy(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsItem * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setFocusProxy
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setFocusProxy( 
        QGraphicsItem * item )
    {
    SMC_MOCK_METHOD1( void, QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::focusItem
// -----------------------------------------------------------------------------
//
QGraphicsItem * QGraphicsItem::focusItem(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsItem * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::focusScopeItem
// -----------------------------------------------------------------------------
//
QGraphicsItem * QGraphicsItem::focusScopeItem(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsItem * )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::grabMouse
// -----------------------------------------------------------------------------
//
void QGraphicsItem::grabMouse(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::ungrabMouse
// -----------------------------------------------------------------------------
//
void QGraphicsItem::ungrabMouse(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::grabKeyboard
// -----------------------------------------------------------------------------
//
void QGraphicsItem::grabKeyboard(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::ungrabKeyboard
// -----------------------------------------------------------------------------
//
void QGraphicsItem::ungrabKeyboard(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::pos
// -----------------------------------------------------------------------------
//
QPointF QGraphicsItem::pos(  ) const
    {
    SMC_MOCK_METHOD0( QPointF )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setX
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setX( 
        qreal x )
    {
    SMC_MOCK_METHOD1( void, qreal, x )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setY
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setY( 
        qreal y )
    {
    SMC_MOCK_METHOD1( void, qreal, y )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::scenePos
// -----------------------------------------------------------------------------
//
QPointF QGraphicsItem::scenePos(  ) const
    {
    SMC_MOCK_METHOD0( QPointF )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setPos
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setPos( 
        const QPointF & pos )
    {
    SMC_MOCK_METHOD1( void, const QPointF &, pos )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::ensureVisible
// -----------------------------------------------------------------------------
//
void QGraphicsItem::ensureVisible( 
        const QRectF & rect,
        int xmargin,
        int ymargin )
    {
    SMC_MOCK_METHOD3( void, const QRectF &, rect, 
        int, xmargin, 
        int, ymargin )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::matrix
// -----------------------------------------------------------------------------
//
QMatrix QGraphicsItem::matrix(  ) const
    {
    SMC_MOCK_METHOD0( QMatrix )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::sceneMatrix
// -----------------------------------------------------------------------------
//
QMatrix QGraphicsItem::sceneMatrix(  ) const
    {
    SMC_MOCK_METHOD0( QMatrix )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setMatrix
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setMatrix( 
        const QMatrix & matrix,
        bool combine )
    {
    SMC_MOCK_METHOD2( void, const QMatrix &, matrix, 
        bool, combine )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::resetMatrix
// -----------------------------------------------------------------------------
//
void QGraphicsItem::resetMatrix(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::transform
// -----------------------------------------------------------------------------
//
QTransform QGraphicsItem::transform(  ) const
    {
    SMC_MOCK_METHOD0( QTransform )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::sceneTransform
// -----------------------------------------------------------------------------
//
QTransform QGraphicsItem::sceneTransform(  ) const
    {
    SMC_MOCK_METHOD0( QTransform )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::deviceTransform
// -----------------------------------------------------------------------------
//
QTransform QGraphicsItem::deviceTransform( 
        const QTransform & viewportTransform ) const
    {
    SMC_MOCK_METHOD1( QTransform, const QTransform &, viewportTransform )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::itemTransform
// -----------------------------------------------------------------------------
//
QTransform QGraphicsItem::itemTransform( 
        const QGraphicsItem * other,
        bool * ok ) const
    {
    SMC_MOCK_METHOD2( QTransform, const QGraphicsItem *, other, 
        bool *, ok )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setTransform
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setTransform( 
        const QTransform & matrix,
        bool combine )
    {
    SMC_MOCK_METHOD2( void, const QTransform &, matrix, 
        bool, combine )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::resetTransform
// -----------------------------------------------------------------------------
//
void QGraphicsItem::resetTransform(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::rotate
// -----------------------------------------------------------------------------
//
void QGraphicsItem::rotate( 
        qreal angle )
    {
    SMC_MOCK_METHOD1( void, qreal, angle )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::scale
// -----------------------------------------------------------------------------
//
void QGraphicsItem::scale( 
        qreal sx,
        qreal sy )
    {
    SMC_MOCK_METHOD2( void, qreal, sx, 
        qreal, sy )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::shear
// -----------------------------------------------------------------------------
//
void QGraphicsItem::shear( 
        qreal sh,
        qreal sv )
    {
    SMC_MOCK_METHOD2( void, qreal, sh, 
        qreal, sv )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::translate
// -----------------------------------------------------------------------------
//
void QGraphicsItem::translate( 
        qreal dx,
        qreal dy )
    {
    SMC_MOCK_METHOD2( void, qreal, dx, 
        qreal, dy )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setRotation
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setRotation( 
        qreal angle )
    {
    SMC_MOCK_METHOD1( void, qreal, angle )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::rotation
// -----------------------------------------------------------------------------
//
qreal QGraphicsItem::rotation(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setScale
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setScale( 
        qreal scale )
    {
    SMC_MOCK_METHOD1( void, qreal, scale )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::scale
// -----------------------------------------------------------------------------
//
qreal QGraphicsItem::scale(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::transformations
// -----------------------------------------------------------------------------
//
QList <QGraphicsTransform * > QGraphicsItem::transformations(  ) const
    {
    SMC_MOCK_METHOD0( QList <QGraphicsTransform * > )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setTransformations
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setTransformations( 
        const QList<QGraphicsTransform *> & transformations )
    {
    SMC_MOCK_METHOD1( void, const QList<QGraphicsTransform *> &, transformations )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::transformOriginPoint
// -----------------------------------------------------------------------------
//
QPointF QGraphicsItem::transformOriginPoint(  ) const
    {
    SMC_MOCK_METHOD0( QPointF )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setTransformOriginPoint
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setTransformOriginPoint( 
        const QPointF & origin )
    {
    SMC_MOCK_METHOD1( void, const QPointF &, origin )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::advance
// -----------------------------------------------------------------------------
//
void QGraphicsItem::advance( 
        int phase )
    {
    SMC_MOCK_METHOD1( void, int, phase )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::zValue
// -----------------------------------------------------------------------------
//
qreal QGraphicsItem::zValue(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setZValue
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setZValue( 
        qreal z )
    {
    SMC_MOCK_METHOD1( void, qreal, z )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::stackBefore
// -----------------------------------------------------------------------------
//
void QGraphicsItem::stackBefore( 
        const QGraphicsItem * sibling )
    {
    SMC_MOCK_METHOD1( void, const QGraphicsItem *, sibling )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsItem::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::childrenBoundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsItem::childrenBoundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::sceneBoundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsItem::sceneBoundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::shape
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsItem::shape(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isClipped
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isClipped(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::clipPath
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsItem::clipPath(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::contains
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::contains( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( bool, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::collidesWithItem
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::collidesWithItem( 
        const QGraphicsItem * other,
        Qt::ItemSelectionMode mode ) const
    {
    SMC_MOCK_METHOD2( bool, const QGraphicsItem *, other, 
        Qt::ItemSelectionMode, mode )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::collidesWithPath
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::collidesWithPath( 
        const QPainterPath & path,
        Qt::ItemSelectionMode mode ) const
    {
    SMC_MOCK_METHOD2( bool, const QPainterPath &, path, 
        Qt::ItemSelectionMode, mode )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::collidingItems
// -----------------------------------------------------------------------------
//
QList <QGraphicsItem * > QGraphicsItem::collidingItems( 
        Qt::ItemSelectionMode mode ) const
    {
    SMC_MOCK_METHOD1( QList <QGraphicsItem * >, Qt::ItemSelectionMode, mode )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isObscured
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isObscured(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isObscured
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isObscured( 
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD1( bool, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isObscuredBy
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isObscuredBy( 
        const QGraphicsItem * item ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::opaqueArea
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsItem::opaqueArea(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::boundingRegion
// -----------------------------------------------------------------------------
//
QRegion QGraphicsItem::boundingRegion( 
        const QTransform & itemToDeviceTransform ) const
    {
    SMC_MOCK_METHOD1( QRegion, const QTransform &, itemToDeviceTransform )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::boundingRegionGranularity
// -----------------------------------------------------------------------------
//
qreal QGraphicsItem::boundingRegionGranularity(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setBoundingRegionGranularity
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setBoundingRegionGranularity( 
        qreal granularity )
    {
    SMC_MOCK_METHOD1( void, qreal, granularity )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::paint
// -----------------------------------------------------------------------------
//
void QGraphicsItem::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::update
// -----------------------------------------------------------------------------
//
void QGraphicsItem::update( 
        const QRectF & rect )
    {
    SMC_MOCK_METHOD1( void, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::scroll
// -----------------------------------------------------------------------------
//
void QGraphicsItem::scroll( 
        qreal dx,
        qreal dy,
        const QRectF & rect )
    {
    SMC_MOCK_METHOD3( void, qreal, dx, 
        qreal, dy, 
        const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToItem
// -----------------------------------------------------------------------------
//
QPointF QGraphicsItem::mapToItem( 
        const QGraphicsItem * item,
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD2( QPointF, const QGraphicsItem *, item, 
        const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToParent
// -----------------------------------------------------------------------------
//
QPointF QGraphicsItem::mapToParent( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( QPointF, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToScene
// -----------------------------------------------------------------------------
//
QPointF QGraphicsItem::mapToScene( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( QPointF, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToItem
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapToItem( 
        const QGraphicsItem * item,
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD2( QPolygonF, const QGraphicsItem *, item, 
        const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToParent
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapToParent( 
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD1( QPolygonF, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToScene
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapToScene( 
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD1( QPolygonF, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapRectToItem
// -----------------------------------------------------------------------------
//
QRectF QGraphicsItem::mapRectToItem( 
        const QGraphicsItem * item,
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD2( QRectF, const QGraphicsItem *, item, 
        const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapRectToParent
// -----------------------------------------------------------------------------
//
QRectF QGraphicsItem::mapRectToParent( 
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD1( QRectF, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapRectToScene
// -----------------------------------------------------------------------------
//
QRectF QGraphicsItem::mapRectToScene( 
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD1( QRectF, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToItem
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapToItem( 
        const QGraphicsItem * item,
        const QPolygonF & polygon ) const
    {
    SMC_MOCK_METHOD2( QPolygonF, const QGraphicsItem *, item, 
        const QPolygonF &, polygon )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToParent
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapToParent( 
        const QPolygonF & polygon ) const
    {
    SMC_MOCK_METHOD1( QPolygonF, const QPolygonF &, polygon )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToScene
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapToScene( 
        const QPolygonF & polygon ) const
    {
    SMC_MOCK_METHOD1( QPolygonF, const QPolygonF &, polygon )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToItem
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsItem::mapToItem( 
        const QGraphicsItem * item,
        const QPainterPath & path ) const
    {
    SMC_MOCK_METHOD2( QPainterPath, const QGraphicsItem *, item, 
        const QPainterPath &, path )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToParent
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsItem::mapToParent( 
        const QPainterPath & path ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, path )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapToScene
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsItem::mapToScene( 
        const QPainterPath & path ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, path )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromItem
// -----------------------------------------------------------------------------
//
QPointF QGraphicsItem::mapFromItem( 
        const QGraphicsItem * item,
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD2( QPointF, const QGraphicsItem *, item, 
        const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromParent
// -----------------------------------------------------------------------------
//
QPointF QGraphicsItem::mapFromParent( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( QPointF, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromScene
// -----------------------------------------------------------------------------
//
QPointF QGraphicsItem::mapFromScene( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( QPointF, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromItem
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapFromItem( 
        const QGraphicsItem * item,
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD2( QPolygonF, const QGraphicsItem *, item, 
        const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromParent
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapFromParent( 
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD1( QPolygonF, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromScene
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapFromScene( 
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD1( QPolygonF, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapRectFromItem
// -----------------------------------------------------------------------------
//
QRectF QGraphicsItem::mapRectFromItem( 
        const QGraphicsItem * item,
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD2( QRectF, const QGraphicsItem *, item, 
        const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapRectFromParent
// -----------------------------------------------------------------------------
//
QRectF QGraphicsItem::mapRectFromParent( 
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD1( QRectF, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapRectFromScene
// -----------------------------------------------------------------------------
//
QRectF QGraphicsItem::mapRectFromScene( 
        const QRectF & rect ) const
    {
    SMC_MOCK_METHOD1( QRectF, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromItem
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapFromItem( 
        const QGraphicsItem * item,
        const QPolygonF & polygon ) const
    {
    SMC_MOCK_METHOD2( QPolygonF, const QGraphicsItem *, item, 
        const QPolygonF &, polygon )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromParent
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapFromParent( 
        const QPolygonF & polygon ) const
    {
    SMC_MOCK_METHOD1( QPolygonF, const QPolygonF &, polygon )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromScene
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsItem::mapFromScene( 
        const QPolygonF & polygon ) const
    {
    SMC_MOCK_METHOD1( QPolygonF, const QPolygonF &, polygon )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromItem
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsItem::mapFromItem( 
        const QGraphicsItem * item,
        const QPainterPath & path ) const
    {
    SMC_MOCK_METHOD2( QPainterPath, const QGraphicsItem *, item, 
        const QPainterPath &, path )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromParent
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsItem::mapFromParent( 
        const QPainterPath & path ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, path )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mapFromScene
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsItem::mapFromScene( 
        const QPainterPath & path ) const
    {
    SMC_MOCK_METHOD1( QPainterPath, const QPainterPath &, path )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isAncestorOf
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isAncestorOf( 
        const QGraphicsItem * child ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, child )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::commonAncestorItem
// -----------------------------------------------------------------------------
//
QGraphicsItem * QGraphicsItem::commonAncestorItem( 
        const QGraphicsItem * other ) const
    {
    SMC_MOCK_METHOD1( QGraphicsItem *, const QGraphicsItem *, other )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::isUnderMouse
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::isUnderMouse(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::data
// -----------------------------------------------------------------------------
//
QVariant QGraphicsItem::data( 
        int key ) const
    {
    SMC_MOCK_METHOD1( QVariant, int, key )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setData
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setData( 
        int key,
        const QVariant & value )
    {
    SMC_MOCK_METHOD2( void, int, key, 
        const QVariant &, value )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::inputMethodHints
// -----------------------------------------------------------------------------
//
Qt::InputMethodHints QGraphicsItem::inputMethodHints(  ) const
    {
    //SMC_MOCK_METHOD0( Qt::InputMethodHints )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setInputMethodHints
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setInputMethodHints( 
        Qt::InputMethodHints hints )
    {
    //SMC_MOCK_METHOD1( void, Qt::InputMethodHints, hints )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::type
// -----------------------------------------------------------------------------
//
int QGraphicsItem::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::installSceneEventFilter
// -----------------------------------------------------------------------------
//
void QGraphicsItem::installSceneEventFilter( 
        QGraphicsItem * filterItem )
    {
    SMC_MOCK_METHOD1( void, QGraphicsItem *, filterItem )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::removeSceneEventFilter
// -----------------------------------------------------------------------------
//
void QGraphicsItem::removeSceneEventFilter( 
        QGraphicsItem * filterItem )
    {
    SMC_MOCK_METHOD1( void, QGraphicsItem *, filterItem )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::updateMicroFocus
// -----------------------------------------------------------------------------
//
void QGraphicsItem::updateMicroFocus(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::sceneEventFilter
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::sceneEventFilter( 
        QGraphicsItem * watched,
        QEvent * event )
    {
    SMC_MOCK_METHOD2( bool, QGraphicsItem *, watched, 
        QEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::sceneEvent
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::sceneEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( bool, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::contextMenuEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::contextMenuEvent( 
        QGraphicsSceneContextMenuEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneContextMenuEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::dragEnterEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::dragEnterEvent( 
        QGraphicsSceneDragDropEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneDragDropEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::dragLeaveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::dragLeaveEvent( 
        QGraphicsSceneDragDropEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneDragDropEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::dragMoveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::dragMoveEvent( 
        QGraphicsSceneDragDropEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneDragDropEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::dropEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::dropEvent( 
        QGraphicsSceneDragDropEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneDragDropEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::focusInEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::focusInEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::focusOutEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::focusOutEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::hoverEnterEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::hoverEnterEvent( 
        QGraphicsSceneHoverEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneHoverEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::hoverMoveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::hoverMoveEvent( 
        QGraphicsSceneHoverEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneHoverEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::hoverLeaveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::hoverLeaveEvent( 
        QGraphicsSceneHoverEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneHoverEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::keyPressEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::keyPressEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::keyReleaseEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::keyReleaseEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mousePressEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::mousePressEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneMouseEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mouseMoveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::mouseMoveEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneMouseEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mouseReleaseEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::mouseReleaseEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneMouseEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::mouseDoubleClickEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::mouseDoubleClickEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneMouseEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::wheelEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::wheelEvent( 
        QGraphicsSceneWheelEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneWheelEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::inputMethodEvent
// -----------------------------------------------------------------------------
//
void QGraphicsItem::inputMethodEvent( 
        QInputMethodEvent * event )
    {
    SMC_MOCK_METHOD1( void, QInputMethodEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::inputMethodQuery
// -----------------------------------------------------------------------------
//
QVariant QGraphicsItem::inputMethodQuery( 
        Qt::InputMethodQuery query ) const
    {
    SMC_MOCK_METHOD1( QVariant, Qt::InputMethodQuery, query )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::itemChange
// -----------------------------------------------------------------------------
//
QVariant QGraphicsItem::itemChange( 
        GraphicsItemChange change,
        const QVariant & value )
    {
    SMC_MOCK_METHOD2( QVariant, GraphicsItemChange, change, 
        const QVariant &, value )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::supportsExtension
// -----------------------------------------------------------------------------
//
bool QGraphicsItem::supportsExtension( 
        Extension extension ) const
    {
    SMC_MOCK_METHOD1( bool, Extension, extension )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::setExtension
// -----------------------------------------------------------------------------
//
void QGraphicsItem::setExtension( 
        Extension extension,
        const QVariant & variant )
    {
    SMC_MOCK_METHOD2( void, Extension, extension, 
        const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::extension
// -----------------------------------------------------------------------------
//
QVariant QGraphicsItem::extension( 
        const QVariant & variant ) const
    {
    SMC_MOCK_METHOD1( QVariant, const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::QGraphicsItem
// -----------------------------------------------------------------------------
//
QGraphicsItem::QGraphicsItem( 
        QGraphicsItemPrivate & dd,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::addToIndex
// -----------------------------------------------------------------------------
//
void QGraphicsItem::addToIndex(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::removeFromIndex
// -----------------------------------------------------------------------------
//
void QGraphicsItem::removeFromIndex(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsItem::prepareGeometryChange
// -----------------------------------------------------------------------------
//
void QGraphicsItem::prepareGeometryChange(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::QGraphicsEllipseItem
// -----------------------------------------------------------------------------
//
QGraphicsEllipseItem::QGraphicsEllipseItem( 
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::QGraphicsEllipseItem
// -----------------------------------------------------------------------------
//
QGraphicsEllipseItem::QGraphicsEllipseItem( 
        const QRectF & rect,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*rect, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::QGraphicsEllipseItem
// -----------------------------------------------------------------------------
//
QGraphicsEllipseItem::QGraphicsEllipseItem( 
        qreal x,
        qreal y,
        qreal w,
        qreal h,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*x, y, w, h, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::~QGraphicsEllipseItem
// -----------------------------------------------------------------------------
//
QGraphicsEllipseItem::~QGraphicsEllipseItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::rect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsEllipseItem::rect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::setRect
// -----------------------------------------------------------------------------
//
void QGraphicsEllipseItem::setRect( 
        const QRectF & rect )
    {
    SMC_MOCK_METHOD1( void, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::startAngle
// -----------------------------------------------------------------------------
//
int QGraphicsEllipseItem::startAngle(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::setStartAngle
// -----------------------------------------------------------------------------
//
void QGraphicsEllipseItem::setStartAngle( 
        int angle )
    {
    SMC_MOCK_METHOD1( void, int, angle )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::spanAngle
// -----------------------------------------------------------------------------
//
int QGraphicsEllipseItem::spanAngle(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::setSpanAngle
// -----------------------------------------------------------------------------
//
void QGraphicsEllipseItem::setSpanAngle( 
        int angle )
    {
    SMC_MOCK_METHOD1( void, int, angle )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsEllipseItem::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::shape
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsEllipseItem::shape(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::contains
// -----------------------------------------------------------------------------
//
bool QGraphicsEllipseItem::contains( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( bool, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::paint
// -----------------------------------------------------------------------------
//
void QGraphicsEllipseItem::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::isObscuredBy
// -----------------------------------------------------------------------------
//
bool QGraphicsEllipseItem::isObscuredBy( 
        const QGraphicsItem * item ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::opaqueArea
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsEllipseItem::opaqueArea(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::type
// -----------------------------------------------------------------------------
//
int QGraphicsEllipseItem::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::supportsExtension
// -----------------------------------------------------------------------------
//
bool QGraphicsEllipseItem::supportsExtension( 
        Extension extension ) const
    {
    SMC_MOCK_METHOD1( bool, Extension, extension )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::setExtension
// -----------------------------------------------------------------------------
//
void QGraphicsEllipseItem::setExtension( 
        Extension extension,
        const QVariant & variant )
    {
    SMC_MOCK_METHOD2( void, Extension, extension, 
        const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsEllipseItem::extension
// -----------------------------------------------------------------------------
//
QVariant QGraphicsEllipseItem::extension( 
        const QVariant & variant ) const
    {
    SMC_MOCK_METHOD1( QVariant, const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::QGraphicsPixmapItem
// -----------------------------------------------------------------------------
//
QGraphicsPixmapItem::QGraphicsPixmapItem( 
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QGraphicsItem( /*parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::QGraphicsPixmapItem
// -----------------------------------------------------------------------------
//
QGraphicsPixmapItem::QGraphicsPixmapItem( 
        const QPixmap & /*pixmap*/,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QGraphicsItem( /*pixmap, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::~QGraphicsPixmapItem
// -----------------------------------------------------------------------------
//
QGraphicsPixmapItem::~QGraphicsPixmapItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::pixmap
// -----------------------------------------------------------------------------
//
QPixmap QGraphicsPixmapItem::pixmap(  ) const
    {
    SMC_MOCK_METHOD0( QPixmap )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::setPixmap
// -----------------------------------------------------------------------------
//
void QGraphicsPixmapItem::setPixmap( 
        const QPixmap & pixmap )
    {
    SMC_MOCK_METHOD1( void, const QPixmap &, pixmap )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::transformationMode
// -----------------------------------------------------------------------------
//
Qt::TransformationMode QGraphicsPixmapItem::transformationMode(  ) const
    {
    SMC_MOCK_METHOD0( Qt::TransformationMode )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::setTransformationMode
// -----------------------------------------------------------------------------
//
void QGraphicsPixmapItem::setTransformationMode( 
        Qt::TransformationMode mode )
    {
    SMC_MOCK_METHOD1( void, Qt::TransformationMode, mode )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::offset
// -----------------------------------------------------------------------------
//
QPointF QGraphicsPixmapItem::offset(  ) const
    {
    SMC_MOCK_METHOD0( QPointF )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::setOffset
// -----------------------------------------------------------------------------
//
void QGraphicsPixmapItem::setOffset( 
        const QPointF & offset )
    {
    SMC_MOCK_METHOD1( void, const QPointF &, offset )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsPixmapItem::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::shape
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsPixmapItem::shape(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::contains
// -----------------------------------------------------------------------------
//
bool QGraphicsPixmapItem::contains( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( bool, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::paint
// -----------------------------------------------------------------------------
//
void QGraphicsPixmapItem::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::isObscuredBy
// -----------------------------------------------------------------------------
//
bool QGraphicsPixmapItem::isObscuredBy( 
        const QGraphicsItem * item ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::opaqueArea
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsPixmapItem::opaqueArea(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::type
// -----------------------------------------------------------------------------
//
int QGraphicsPixmapItem::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::shapeMode
// -----------------------------------------------------------------------------
//
QGraphicsPixmapItem::ShapeMode QGraphicsPixmapItem::shapeMode(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsPixmapItem::ShapeMode )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::setShapeMode
// -----------------------------------------------------------------------------
//
void QGraphicsPixmapItem::setShapeMode( 
        QGraphicsPixmapItem::ShapeMode mode )
    {
    SMC_MOCK_METHOD1( void, QGraphicsPixmapItem::ShapeMode, mode )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::supportsExtension
// -----------------------------------------------------------------------------
//
bool QGraphicsPixmapItem::supportsExtension( 
        QGraphicsItem::Extension extension ) const
    {
    SMC_MOCK_METHOD1( bool, QGraphicsItem::Extension, extension )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::setExtension
// -----------------------------------------------------------------------------
//
void QGraphicsPixmapItem::setExtension( 
        QGraphicsItem::Extension extension,
        const QVariant & variant )
    {
    SMC_MOCK_METHOD2( void, QGraphicsItem::Extension, extension, 
        const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsPixmapItem::extension
// -----------------------------------------------------------------------------
//
QVariant QGraphicsPixmapItem::extension( 
        const QVariant & variant ) const
    {
    SMC_MOCK_METHOD1( QVariant, const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::QGraphicsRectItem
// -----------------------------------------------------------------------------
//
QGraphicsRectItem::QGraphicsRectItem( 
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::QGraphicsRectItem
// -----------------------------------------------------------------------------
//
QGraphicsRectItem::QGraphicsRectItem( 
        const QRectF & /*rect*/,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*rect, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::QGraphicsRectItem
// -----------------------------------------------------------------------------
//
QGraphicsRectItem::QGraphicsRectItem( 
        qreal /*x*/,
        qreal /*y*/,
        qreal /*w*/,
        qreal /*h*/,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*x, y, w, h, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::~QGraphicsRectItem
// -----------------------------------------------------------------------------
//
QGraphicsRectItem::~QGraphicsRectItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::rect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsRectItem::rect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::setRect
// -----------------------------------------------------------------------------
//
void QGraphicsRectItem::setRect( 
        const QRectF & rect )
    {
    SMC_MOCK_METHOD1( void, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsRectItem::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::shape
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsRectItem::shape(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::contains
// -----------------------------------------------------------------------------
//
bool QGraphicsRectItem::contains( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( bool, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::paint
// -----------------------------------------------------------------------------
//
void QGraphicsRectItem::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::isObscuredBy
// -----------------------------------------------------------------------------
//
bool QGraphicsRectItem::isObscuredBy( 
        const QGraphicsItem * item ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::opaqueArea
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsRectItem::opaqueArea(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::type
// -----------------------------------------------------------------------------
//
int QGraphicsRectItem::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::supportsExtension
// -----------------------------------------------------------------------------
//
bool QGraphicsRectItem::supportsExtension( 
        Extension extension ) const
    {
    SMC_MOCK_METHOD1( bool, Extension, extension )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::setExtension
// -----------------------------------------------------------------------------
//
void QGraphicsRectItem::setExtension( 
        Extension extension,
        const QVariant & variant )
    {
    SMC_MOCK_METHOD2( void, Extension, extension, 
        const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsRectItem::extension
// -----------------------------------------------------------------------------
//
QVariant QGraphicsRectItem::extension( 
        const QVariant & variant ) const
    {
    SMC_MOCK_METHOD1( QVariant, const QVariant &, variant )
    }

// -----------------------------------------------------------------------------
// QGraphicsObject::QGraphicsObject
// -----------------------------------------------------------------------------
//
/*QGraphicsObject::QGraphicsObject(QGraphicsItem *parent)
        : QGraphicsItem(parent)
{
}
*/

// -----------------------------------------------------------------------------
// QGraphicsObject::QGraphicsObject
// -----------------------------------------------------------------------------
//
QGraphicsObject::QGraphicsObject( 
        QGraphicsItemPrivate & /*dd*/,
        QGraphicsItem * parent,
        QGraphicsScene * s )
    : QObject( s ), QGraphicsItem(parent)
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::children
// -----------------------------------------------------------------------------
//
/*const QObjectList & QGraphicsObject::children(  ) const
    {
    SMC_MOCK_METHOD0( const QObjectList & )
    }*/


// -----------------------------------------------------------------------------
// QGraphicsObject::grabGesture
// -----------------------------------------------------------------------------
//
void QGraphicsObject::grabGesture( 
        Qt::GestureType type,
        Qt::GestureFlags flags )
    {
    /*SMC_MOCK_METHOD2( void, Qt::GestureType, type, 
        QFlags<Qt::GestureFlag>, theFlags )*/
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::ungrabGesture
// -----------------------------------------------------------------------------
//
void QGraphicsObject::ungrabGesture( 
        Qt::GestureType type )
    {
    SMC_MOCK_METHOD1( void, Qt::GestureType, type )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::updateMicroFocus
// -----------------------------------------------------------------------------
//
void QGraphicsObject::updateMicroFocus(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::parentChanged
// -----------------------------------------------------------------------------
//
/*void QGraphicsObject::parentChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::opacityChanged
// -----------------------------------------------------------------------------
//
void QGraphicsObject::opacityChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::visibleChanged
// -----------------------------------------------------------------------------
//
void QGraphicsObject::visibleChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::enabledChanged
// -----------------------------------------------------------------------------
//
void QGraphicsObject::enabledChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::xChanged
// -----------------------------------------------------------------------------
//
void QGraphicsObject::xChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::yChanged
// -----------------------------------------------------------------------------
//
void QGraphicsObject::yChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::zChanged
// -----------------------------------------------------------------------------
//
void QGraphicsObject::zChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::rotationChanged
// -----------------------------------------------------------------------------
//
void QGraphicsObject::rotationChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::scaleChanged
// -----------------------------------------------------------------------------
//
void QGraphicsObject::scaleChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::childrenChanged
// -----------------------------------------------------------------------------
//
void QGraphicsObject::childrenChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::widthChanged
// -----------------------------------------------------------------------------
//
void QGraphicsObject::widthChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsObject::heightChanged
// -----------------------------------------------------------------------------
//
void QGraphicsObject::heightChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }*/


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::QGraphicsPolygonItem
// -----------------------------------------------------------------------------
//
QGraphicsPolygonItem::QGraphicsPolygonItem( 
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::QGraphicsPolygonItem
// -----------------------------------------------------------------------------
//
QGraphicsPolygonItem::QGraphicsPolygonItem( 
        const QPolygonF & /*polygon*/,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*polygon, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::~QGraphicsPolygonItem
// -----------------------------------------------------------------------------
//
QGraphicsPolygonItem::~QGraphicsPolygonItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::polygon
// -----------------------------------------------------------------------------
//
QPolygonF QGraphicsPolygonItem::polygon(  ) const
    {
    SMC_MOCK_METHOD0( QPolygonF )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::setPolygon
// -----------------------------------------------------------------------------
//
void QGraphicsPolygonItem::setPolygon( 
        const QPolygonF & polygon )
    {
    SMC_MOCK_METHOD1( void, const QPolygonF &, polygon )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::fillRule
// -----------------------------------------------------------------------------
//
Qt::FillRule QGraphicsPolygonItem::fillRule(  ) const
    {
    SMC_MOCK_METHOD0( Qt::FillRule )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::setFillRule
// -----------------------------------------------------------------------------
//
void QGraphicsPolygonItem::setFillRule( 
        Qt::FillRule rule )
    {
    SMC_MOCK_METHOD1( void, Qt::FillRule, rule )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsPolygonItem::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::shape
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsPolygonItem::shape(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::contains
// -----------------------------------------------------------------------------
//
bool QGraphicsPolygonItem::contains( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( bool, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::paint
// -----------------------------------------------------------------------------
//
void QGraphicsPolygonItem::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::isObscuredBy
// -----------------------------------------------------------------------------
//
bool QGraphicsPolygonItem::isObscuredBy( 
        const QGraphicsItem * item ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::opaqueArea
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsPolygonItem::opaqueArea(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::type
// -----------------------------------------------------------------------------
//
int QGraphicsPolygonItem::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::supportsExtension
// -----------------------------------------------------------------------------
//
bool QGraphicsPolygonItem::supportsExtension( 
        Extension extension ) const
    {
    SMC_MOCK_METHOD1( bool, Extension, extension )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::setExtension
// -----------------------------------------------------------------------------
//
void QGraphicsPolygonItem::setExtension( 
        Extension extension,
        const QVariant & variant )
    {
    SMC_MOCK_METHOD2( void, Extension, extension, 
        const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsPolygonItem::extension
// -----------------------------------------------------------------------------
//
QVariant QGraphicsPolygonItem::extension( 
        const QVariant & variant ) const
    {
    SMC_MOCK_METHOD1( QVariant, const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::QGraphicsPathItem
// -----------------------------------------------------------------------------
//
QGraphicsPathItem::QGraphicsPathItem( 
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::QGraphicsPathItem
// -----------------------------------------------------------------------------
//
QGraphicsPathItem::QGraphicsPathItem( 
        const QPainterPath & /*path*/,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*path, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::~QGraphicsPathItem
// -----------------------------------------------------------------------------
//
QGraphicsPathItem::~QGraphicsPathItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::path
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsPathItem::path(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::setPath
// -----------------------------------------------------------------------------
//
void QGraphicsPathItem::setPath( 
        const QPainterPath & path )
    {
    SMC_MOCK_METHOD1( void, const QPainterPath &, path )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsPathItem::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::shape
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsPathItem::shape(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::contains
// -----------------------------------------------------------------------------
//
bool QGraphicsPathItem::contains( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( bool, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::paint
// -----------------------------------------------------------------------------
//
void QGraphicsPathItem::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::isObscuredBy
// -----------------------------------------------------------------------------
//
bool QGraphicsPathItem::isObscuredBy( 
        const QGraphicsItem * item ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::opaqueArea
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsPathItem::opaqueArea(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::type
// -----------------------------------------------------------------------------
//
int QGraphicsPathItem::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::supportsExtension
// -----------------------------------------------------------------------------
//
bool QGraphicsPathItem::supportsExtension( 
        Extension extension ) const
    {
    SMC_MOCK_METHOD1( bool, Extension, extension )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::setExtension
// -----------------------------------------------------------------------------
//
void QGraphicsPathItem::setExtension( 
        Extension extension,
        const QVariant & variant )
    {
    SMC_MOCK_METHOD2( void, Extension, extension, 
        const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsPathItem::extension
// -----------------------------------------------------------------------------
//
QVariant QGraphicsPathItem::extension( 
        const QVariant & variant ) const
    {
    SMC_MOCK_METHOD1( QVariant, const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::QGraphicsSimpleTextItem
// -----------------------------------------------------------------------------
//
QGraphicsSimpleTextItem::QGraphicsSimpleTextItem( 
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::QGraphicsSimpleTextItem
// -----------------------------------------------------------------------------
//
QGraphicsSimpleTextItem::QGraphicsSimpleTextItem( 
        const QString & /*text*/,
        QGraphicsItem * /*parent*/,
        QGraphicsScene * /*scene*/ )
    //:
    //QAbstractGraphicsShapeItem( /*text, parent, scene*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::~QGraphicsSimpleTextItem
// -----------------------------------------------------------------------------
//
QGraphicsSimpleTextItem::~QGraphicsSimpleTextItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::setText
// -----------------------------------------------------------------------------
//
void QGraphicsSimpleTextItem::setText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::text
// -----------------------------------------------------------------------------
//
QString QGraphicsSimpleTextItem::text(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::setFont
// -----------------------------------------------------------------------------
//
void QGraphicsSimpleTextItem::setFont( 
        const QFont & font )
    {
    SMC_MOCK_METHOD1( void, const QFont &, font )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::font
// -----------------------------------------------------------------------------
//
QFont QGraphicsSimpleTextItem::font(  ) const
    {
    SMC_MOCK_METHOD0( QFont )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsSimpleTextItem::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::shape
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsSimpleTextItem::shape(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::contains
// -----------------------------------------------------------------------------
//
bool QGraphicsSimpleTextItem::contains( 
        const QPointF & point ) const
    {
    SMC_MOCK_METHOD1( bool, const QPointF &, point )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::paint
// -----------------------------------------------------------------------------
//
void QGraphicsSimpleTextItem::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::isObscuredBy
// -----------------------------------------------------------------------------
//
bool QGraphicsSimpleTextItem::isObscuredBy( 
        const QGraphicsItem * item ) const
    {
    SMC_MOCK_METHOD1( bool, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::opaqueArea
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsSimpleTextItem::opaqueArea(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::type
// -----------------------------------------------------------------------------
//
int QGraphicsSimpleTextItem::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::supportsExtension
// -----------------------------------------------------------------------------
//
bool QGraphicsSimpleTextItem::supportsExtension( 
        Extension extension ) const
    {
    SMC_MOCK_METHOD1( bool, Extension, extension )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::setExtension
// -----------------------------------------------------------------------------
//
void QGraphicsSimpleTextItem::setExtension( 
        Extension extension,
        const QVariant & variant )
    {
    SMC_MOCK_METHOD2( void, Extension, extension, 
        const QVariant &, variant )
    }


// -----------------------------------------------------------------------------
// QGraphicsSimpleTextItem::extension
// -----------------------------------------------------------------------------
//
QVariant QGraphicsSimpleTextItem::extension( 
        const QVariant & variant ) const
    {
    SMC_MOCK_METHOD1( QVariant, const QVariant &, variant )
    }


