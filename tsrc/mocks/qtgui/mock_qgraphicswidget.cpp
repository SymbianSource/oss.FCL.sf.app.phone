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
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QAction>
#include <QGraphicsLayout>
#include "qgraphicswidget.h"

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// QGraphicsWidget::QGraphicsWidget
// -----------------------------------------------------------------------------
//
QGraphicsWidget::QGraphicsWidget(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QGraphicsObject( parent )
{

}

// -----------------------------------------------------------------------------
// QGraphicsWidget::QGraphicsWidget
// -----------------------------------------------------------------------------
//
QGraphicsWidget::QGraphicsWidget( 
        QGraphicsWidgetPrivate &,
        QGraphicsItem * parent,
        QGraphicsScene *,
        Qt::WindowFlags  )
    : QGraphicsObject( parent )
    {
    
    }

// -----------------------------------------------------------------------------
// QGraphicsWidget::~QGraphicsWidget
// -----------------------------------------------------------------------------
//
QGraphicsWidget::~QGraphicsWidget(  )
    {
    
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::layout
// -----------------------------------------------------------------------------
//
QGraphicsLayout * QGraphicsWidget::layout(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsLayout * )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setLayout
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setLayout( 
        QGraphicsLayout * layout )
    {
    SMC_MOCK_METHOD1( void, QGraphicsLayout *, layout )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::adjustSize
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::adjustSize(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::layoutDirection
// -----------------------------------------------------------------------------
//
Qt::LayoutDirection QGraphicsWidget::layoutDirection(  ) const
    {
    SMC_MOCK_METHOD0( Qt::LayoutDirection )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setLayoutDirection
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setLayoutDirection( 
        Qt::LayoutDirection direction )
    {
    SMC_MOCK_METHOD1( void, Qt::LayoutDirection, direction )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::unsetLayoutDirection
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::unsetLayoutDirection(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::style
// -----------------------------------------------------------------------------
//
QStyle * QGraphicsWidget::style(  ) const
    {
    SMC_MOCK_METHOD0( QStyle * )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setStyle
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setStyle( 
        QStyle * style )
    {
    SMC_MOCK_METHOD1( void, QStyle *, style )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::font
// -----------------------------------------------------------------------------
//
QFont QGraphicsWidget::font(  ) const
    {
    SMC_MOCK_METHOD0( QFont )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setFont
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setFont( 
        const QFont & font )
    {
    SMC_MOCK_METHOD1( void, const QFont &, font )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::palette
// -----------------------------------------------------------------------------
//
QPalette QGraphicsWidget::palette(  ) const
    {
    SMC_MOCK_METHOD0( QPalette )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setPalette
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setPalette( 
        const QPalette & palette )
    {
    SMC_MOCK_METHOD1( void, const QPalette &, palette )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::autoFillBackground
// -----------------------------------------------------------------------------
//
bool QGraphicsWidget::autoFillBackground(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setAutoFillBackground
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setAutoFillBackground( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::resize
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::resize( 
        const QSizeF & size )
    {
    SMC_MOCK_METHOD1( void, const QSizeF &, size )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::size
// -----------------------------------------------------------------------------
//
QSizeF QGraphicsWidget::size(  ) const
    {
    SMC_MOCK_METHOD0( QSizeF )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setGeometry
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setGeometry( 
        const QRectF & rect )
    {
    SMC_MOCK_METHOD1( void, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setContentsMargins
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setContentsMargins( 
        qreal left,
        qreal top,
        qreal right,
        qreal bottom )
    {
    SMC_MOCK_METHOD4( void, qreal, left, 
        qreal, top, 
        qreal, right, 
        qreal, bottom )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::getContentsMargins
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::getContentsMargins( 
        qreal * left,
        qreal * top,
        qreal * right,
        qreal * bottom ) const
    {
    SMC_MOCK_METHOD4( void, qreal *, left, 
        qreal *, top, 
        qreal *, right, 
        qreal *, bottom )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setWindowFrameMargins
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setWindowFrameMargins( 
        qreal left,
        qreal top,
        qreal right,
        qreal bottom )
    {
    SMC_MOCK_METHOD4( void, qreal, left, 
        qreal, top, 
        qreal, right, 
        qreal, bottom )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::getWindowFrameMargins
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::getWindowFrameMargins( 
        qreal * left,
        qreal * top,
        qreal * right,
        qreal * bottom ) const
    {
    SMC_MOCK_METHOD4( void, qreal *, left, 
        qreal *, top, 
        qreal *, right, 
        qreal *, bottom )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::unsetWindowFrameMargins
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::unsetWindowFrameMargins(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::windowFrameGeometry
// -----------------------------------------------------------------------------
//
QRectF QGraphicsWidget::windowFrameGeometry(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::windowFrameRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsWidget::windowFrameRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::windowFlags
// -----------------------------------------------------------------------------
//
Qt::WindowFlags QGraphicsWidget::windowFlags(  ) const
    {
    SMC_MOCK_METHOD0( Qt::WindowFlags )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::windowType
// -----------------------------------------------------------------------------
//
Qt::WindowType QGraphicsWidget::windowType(  ) const
    {
    SMC_MOCK_METHOD0( Qt::WindowType )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setWindowFlags
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setWindowFlags( 
        Qt::WindowFlags wFlags )
    {
    SMC_MOCK_METHOD1( void, /*Qt::WindowFlags*/int, wFlags )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::isActiveWindow
// -----------------------------------------------------------------------------
//
bool QGraphicsWidget::isActiveWindow(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setWindowTitle
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setWindowTitle( 
        const QString & title )
    {
    SMC_MOCK_METHOD1( void, const QString &, title )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::windowTitle
// -----------------------------------------------------------------------------
//
QString QGraphicsWidget::windowTitle(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::focusPolicy
// -----------------------------------------------------------------------------
//
Qt::FocusPolicy QGraphicsWidget::focusPolicy(  ) const
    {
    SMC_MOCK_METHOD0( Qt::FocusPolicy )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setFocusPolicy
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setFocusPolicy( 
        Qt::FocusPolicy policy )
    {
    SMC_MOCK_METHOD1( void, Qt::FocusPolicy, policy )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setTabOrder
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setTabOrder( 
        QGraphicsWidget * first,
        QGraphicsWidget * second )
    {
    SMC_MOCK_METHOD2( void, QGraphicsWidget *, first, 
        QGraphicsWidget *, second )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::focusWidget
// -----------------------------------------------------------------------------
//
QGraphicsWidget * QGraphicsWidget::focusWidget(  ) const
    {
    SMC_MOCK_METHOD0( QGraphicsWidget * )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::grabShortcut
// -----------------------------------------------------------------------------
//
int QGraphicsWidget::grabShortcut( 
        const QKeySequence & s,
        Qt::ShortcutContext c )
    {
    SMC_MOCK_METHOD2( int, const QKeySequence &, s, 
        Qt::ShortcutContext, c )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::releaseShortcut
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::releaseShortcut( 
        int id )
    {
    SMC_MOCK_METHOD1( void, int, id )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setShortcutEnabled
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setShortcutEnabled( 
        int id,
        bool enabled )
    {
    SMC_MOCK_METHOD2( void, int, id, 
        bool, enabled )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setShortcutAutoRepeat
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setShortcutAutoRepeat( 
        int id,
        bool enabled )
    {
    SMC_MOCK_METHOD2( void, int, id, 
        bool, enabled )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::addAction
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::addAction( 
        QAction * a )
    {
    SMC_MOCK_METHOD1( void, QAction *, a )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::addActions
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::addActions( 
        QList<QAction *> actions )
    {
    SMC_MOCK_METHOD1( void, QList<QAction *>, actions )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::insertAction
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::insertAction( 
        QAction * before,
        QAction * a )
    {
    SMC_MOCK_METHOD2( void, QAction *, before, 
        QAction *, a )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::insertActions
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::insertActions( 
        QAction * before,
        QList<QAction *> actions )
    {
    SMC_MOCK_METHOD2( void, QAction *, before, 
        QList<QAction *>, actions )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::removeAction
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::removeAction( 
        QAction * a )
    {
    SMC_MOCK_METHOD1( void, QAction *, a )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::actions
// -----------------------------------------------------------------------------
//
QList <QAction * > QGraphicsWidget::actions(  ) const
    {
    SMC_MOCK_METHOD0( QList <QAction * > )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::setAttribute
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::setAttribute( 
        Qt::WidgetAttribute attribute,
        bool on )
    {
    SMC_MOCK_METHOD2( void, Qt::WidgetAttribute, attribute, 
        bool, on )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::testAttribute
// -----------------------------------------------------------------------------
//
bool QGraphicsWidget::testAttribute( 
        Qt::WidgetAttribute attribute ) const
    {
    SMC_MOCK_METHOD1( bool, Qt::WidgetAttribute, attribute )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::type
// -----------------------------------------------------------------------------
//
int QGraphicsWidget::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::paint
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::paintWindowFrame
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::paintWindowFrame( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::boundingRect
// -----------------------------------------------------------------------------
//
QRectF QGraphicsWidget::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::shape
// -----------------------------------------------------------------------------
//
QPainterPath QGraphicsWidget::shape(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }



// -----------------------------------------------------------------------------
// QGraphicsWidget::layoutChanged
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::layoutChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::close
// -----------------------------------------------------------------------------
//
bool QGraphicsWidget::close(  )
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::initStyleOption
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::initStyleOption( 
        QStyleOption * option ) const
    {
    SMC_MOCK_METHOD1( void, QStyleOption *, option )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::sizeHint
// -----------------------------------------------------------------------------
//
QSizeF QGraphicsWidget::sizeHint( 
        Qt::SizeHint which,
        const QSizeF & constraint ) const
    {
    SMC_MOCK_METHOD2( QSizeF, Qt::SizeHint, which, 
        const QSizeF &, constraint )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::updateGeometry
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::updateGeometry(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::itemChange
// -----------------------------------------------------------------------------
//
QVariant QGraphicsWidget::itemChange( 
        GraphicsItemChange change,
        const QVariant & value )
    {
    SMC_MOCK_METHOD2( QVariant, GraphicsItemChange, change, 
        const QVariant &, value )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::propertyChange
// -----------------------------------------------------------------------------
//
QVariant QGraphicsWidget::propertyChange( 
        const QString & propertyName,
        const QVariant & value )
    {
    SMC_MOCK_METHOD2( QVariant, const QString &, propertyName, 
        const QVariant &, value )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::sceneEvent
// -----------------------------------------------------------------------------
//
bool QGraphicsWidget::sceneEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( bool, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::windowFrameEvent
// -----------------------------------------------------------------------------
//
bool QGraphicsWidget::windowFrameEvent( 
        QEvent * e )
    {
    SMC_MOCK_METHOD1( bool, QEvent *, e )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::windowFrameSectionAt
// -----------------------------------------------------------------------------
//
Qt::WindowFrameSection QGraphicsWidget::windowFrameSectionAt( 
        const QPointF & pos ) const
    {
    SMC_MOCK_METHOD1( Qt::WindowFrameSection, const QPointF &, pos )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::event
// -----------------------------------------------------------------------------
//
bool QGraphicsWidget::event( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( bool, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::changeEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::changeEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::closeEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::closeEvent( 
        QCloseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QCloseEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::focusInEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::focusInEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::focusNextPrevChild
// -----------------------------------------------------------------------------
//
bool QGraphicsWidget::focusNextPrevChild( 
        bool next )
    {
    SMC_MOCK_METHOD1( bool, bool, next )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::focusOutEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::focusOutEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::hideEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::hideEvent( 
        QHideEvent * event )
    {
    SMC_MOCK_METHOD1( void, QHideEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::moveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::moveEvent( 
        QGraphicsSceneMoveEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneMoveEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::polishEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::polishEvent(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::resizeEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::resizeEvent( 
        QGraphicsSceneResizeEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneResizeEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::showEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::showEvent( 
        QShowEvent * event )
    {
    SMC_MOCK_METHOD1( void, QShowEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::hoverMoveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::hoverMoveEvent( 
        QGraphicsSceneHoverEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneHoverEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::hoverLeaveEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::hoverLeaveEvent( 
        QGraphicsSceneHoverEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneHoverEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::grabMouseEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::grabMouseEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::ungrabMouseEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::ungrabMouseEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::grabKeyboardEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::grabKeyboardEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// QGraphicsWidget::ungrabKeyboardEvent
// -----------------------------------------------------------------------------
//
void QGraphicsWidget::ungrabKeyboardEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


