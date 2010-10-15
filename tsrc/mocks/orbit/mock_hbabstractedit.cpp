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
#include <smcmockclassincludes.h>
#include <QTextBlock>
#include <QPainter>
#include <HbMenu>
#include <QGraphicsSceneResizeEvent>
#include <HbValidator>
#include "hbabstractedit.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbAbstractEdit::HbAbstractEdit
// -----------------------------------------------------------------------------
//
HbAbstractEdit::HbAbstractEdit( 
        QGraphicsItem * parent ) :
        HbWidget(parent)
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::~HbAbstractEdit
// -----------------------------------------------------------------------------
//
HbAbstractEdit::~HbAbstractEdit(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::isReadOnly
// -----------------------------------------------------------------------------
//
bool HbAbstractEdit::isReadOnly(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setReadOnly
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setReadOnly( 
        bool value )
    {
    SMC_MOCK_METHOD1( void, bool, value )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::cursorPosition
// -----------------------------------------------------------------------------
//
int HbAbstractEdit::cursorPosition(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::cursorVisibility
// -----------------------------------------------------------------------------
//
Hb::TextCursorVisibility HbAbstractEdit::cursorVisibility(  ) const
    {
    SMC_MOCK_METHOD0( Hb::TextCursorVisibility )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setCursorVisibility
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setCursorVisibility( 
        Hb::TextCursorVisibility value )
    {
    SMC_MOCK_METHOD1( void, Hb::TextCursorVisibility, value )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::validator
// -----------------------------------------------------------------------------
//
HbValidator * HbAbstractEdit::validator(  ) const
    {
    SMC_MOCK_METHOD0( HbValidator * )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setValidator
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setValidator( 
        HbValidator * validator )
    {
    SMC_MOCK_METHOD1( void, HbValidator *, validator )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::hasAcceptableInput
// -----------------------------------------------------------------------------
//
bool HbAbstractEdit::hasAcceptableInput(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::primitive
// -----------------------------------------------------------------------------
//
QGraphicsItem * HbAbstractEdit::primitive( 
        HbStyle::Primitive primitive ) const
    {
    SMC_MOCK_METHOD1( QGraphicsItem *, HbStyle::Primitive, primitive )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::event
// -----------------------------------------------------------------------------
//
bool HbAbstractEdit::event( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( bool, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::type
// -----------------------------------------------------------------------------
//
int HbAbstractEdit::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setAlignment
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setAlignment( 
        Qt::Alignment alignment )
    {
    SMC_MOCK_METHOD1( void, /*Qt::Alignment*/int, alignment )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::alignment
// -----------------------------------------------------------------------------
//
Qt::Alignment HbAbstractEdit::alignment(  ) const
    {
    SMC_MOCK_METHOD0( Qt::Alignment )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::contextMenuFlags
// -----------------------------------------------------------------------------
//
Hb::TextContextMenuFlags HbAbstractEdit::contextMenuFlags(  ) const
    {
    SMC_MOCK_METHOD0( Hb::TextContextMenuFlags )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setContextMenuFlags
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setContextMenuFlags( 
        Hb::TextContextMenuFlags flags )
    {
    SMC_MOCK_METHOD1( void, /*Hb::TextContextMenuFlags*/int, flags )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setContextMenuFlag
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setContextMenuFlag( 
        Hb::TextContextMenuFlag flag )
    {
    SMC_MOCK_METHOD1( void, Hb::TextContextMenuFlag, flag )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::clearContextMenuFlag
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::clearContextMenuFlag( 
        Hb::TextContextMenuFlag flag )
    {
    SMC_MOCK_METHOD1( void, Hb::TextContextMenuFlag, flag )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::placeholderText
// -----------------------------------------------------------------------------
//
QString HbAbstractEdit::placeholderText(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setPlaceholderText
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setPlaceholderText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::anchorAt
// -----------------------------------------------------------------------------
//
QString HbAbstractEdit::anchorAt( 
        const QPointF & pos ) const
    {
    SMC_MOCK_METHOD1( QString, const QPointF &, pos )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::anchorAt
// -----------------------------------------------------------------------------
//
QString HbAbstractEdit::anchorAt( 
        int pos ) const
    {
    SMC_MOCK_METHOD1( QString, int, pos )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setSmileyTheme
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setSmileyTheme( 
        const HbSmileyTheme & theme )
    {
    SMC_MOCK_METHOD1( void, const HbSmileyTheme &, theme )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::smileyTheme
// -----------------------------------------------------------------------------
//
HbSmileyTheme HbAbstractEdit::smileyTheme(  ) const
    {
    SMC_MOCK_METHOD0( HbSmileyTheme )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::defaultSmileyTheme
// -----------------------------------------------------------------------------
//
HbSmileyTheme HbAbstractEdit::defaultSmileyTheme(  ) const
    {
    SMC_MOCK_METHOD0( HbSmileyTheme )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::resetSmileyTheme
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::resetSmileyTheme(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::isSmileysEnabled
// -----------------------------------------------------------------------------
//
bool HbAbstractEdit::isSmileysEnabled(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setSmileysEnabled
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setSmileysEnabled( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setFormatDialog
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setFormatDialog( 
        HbFormatDialogPointer dialog )
    {
    SMC_MOCK_METHOD1( void, HbFormatDialogPointer, dialog )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::characterAt
// -----------------------------------------------------------------------------
//
QChar HbAbstractEdit::characterAt( 
        int pos ) const
    {
    SMC_MOCK_METHOD1( QChar, int, pos )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::contentsChanged
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::contentsChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::cursorPositionChanged
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::cursorPositionChanged( 
        int oldPos,
        int newPos )
    {
    SMC_MOCK_METHOD2( void, int, oldPos, 
        int, newPos )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::selectionChanged
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::selectionChanged( 
        const QTextCursor & oldCursor,
        const QTextCursor & newCursor )
    {
    SMC_MOCK_METHOD2( void, const QTextCursor &, oldCursor, 
        const QTextCursor &, newCursor )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::aboutToShowContextMenu
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::aboutToShowContextMenu( 
        HbMenu * contextMenu,
        const QPointF & pos )
    {
    SMC_MOCK_METHOD2( void, HbMenu *, contextMenu, 
        const QPointF &, pos )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::anchorTapped
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::anchorTapped( 
        const QString & anchor )
    {
    SMC_MOCK_METHOD1( void, const QString &, anchor )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::updatePrimitives
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::updatePrimitives(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setCursorPosition
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setCursorPosition( 
        int pos,
        QTextCursor::MoveMode mode )
    {
    SMC_MOCK_METHOD2( void, int, pos, 
        QTextCursor::MoveMode, mode )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::cut
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::cut(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::copy
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::copy(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::paste
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::paste(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::selectClickedWord
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::selectClickedWord(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::selectAll
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::selectAll(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::deselect
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::deselect(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::format
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::format(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::HbAbstractEdit
// -----------------------------------------------------------------------------
//
HbAbstractEdit::HbAbstractEdit( 
        HbAbstractEditPrivate & dd,
        QGraphicsItem * parent ) :
        HbWidget(parent)
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::drawContents
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::drawContents( 
        QPainter * painter,
        const QStyleOptionGraphicsItem & option )
    {
    SMC_MOCK_METHOD2( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem &, option )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::resizeEvent
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::resizeEvent( 
        QGraphicsSceneResizeEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneResizeEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::inputMethodEvent
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::inputMethodEvent( 
        QInputMethodEvent * event )
    {
    SMC_MOCK_METHOD1( void, QInputMethodEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::inputMethodQuery
// -----------------------------------------------------------------------------
//
QVariant HbAbstractEdit::inputMethodQuery( 
        Qt::InputMethodQuery query ) const
    {
    SMC_MOCK_METHOD1( QVariant, Qt::InputMethodQuery, query )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::keyPressEvent
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::keyPressEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::keyReleaseEvent
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::keyReleaseEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::focusInEvent
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::focusInEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::focusOutEvent
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::focusOutEvent( 
        QFocusEvent * event )
    {
    SMC_MOCK_METHOD1( void, QFocusEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::changeEvent
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::changeEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::hideEvent
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::hideEvent( 
        QHideEvent * event )
    {
    SMC_MOCK_METHOD1( void, QHideEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setDocument
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setDocument( 
        QTextDocument * document )
    {
    SMC_MOCK_METHOD1( void, QTextDocument *, document )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::document
// -----------------------------------------------------------------------------
//
QTextDocument * HbAbstractEdit::document(  ) const
    {
    SMC_MOCK_METHOD0( QTextDocument * )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setTextCursor
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setTextCursor( 
        const QTextCursor & cursor )
    {
    SMC_MOCK_METHOD1( void, const QTextCursor &, cursor )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::textCursor
// -----------------------------------------------------------------------------
//
QTextCursor HbAbstractEdit::textCursor(  ) const
    {
    SMC_MOCK_METHOD0( QTextCursor )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::toPlainText
// -----------------------------------------------------------------------------
//
QString HbAbstractEdit::toPlainText(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::toHtml
// -----------------------------------------------------------------------------
//
QString HbAbstractEdit::toHtml(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::moveCursor
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::moveCursor( 
        QTextCursor::MoveOperation op,
        QTextCursor::MoveMode mode )
    {
    SMC_MOCK_METHOD2( void, QTextCursor::MoveOperation, op, 
        QTextCursor::MoveMode, mode )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::blockBoundingRect
// -----------------------------------------------------------------------------
//
QRectF HbAbstractEdit::blockBoundingRect( 
        const QTextBlock & block ) const
    {
    SMC_MOCK_METHOD1( QRectF, const QTextBlock &, block )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::isScrollable
// -----------------------------------------------------------------------------
//
bool HbAbstractEdit::isScrollable(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setScrollable
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setScrollable( 
        bool value )
    {
    SMC_MOCK_METHOD1( void, bool, value )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::scrollArea
// -----------------------------------------------------------------------------
//
HbScrollArea * HbAbstractEdit::scrollArea(  ) const
    {
    SMC_MOCK_METHOD0( HbScrollArea * )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::createContextMenu
// -----------------------------------------------------------------------------
//
HbMenu * HbAbstractEdit::createContextMenu(  )
    {
    SMC_MOCK_METHOD0( HbMenu * )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::showContextMenu
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::showContextMenu( 
        QPointF position )
    {
    SMC_MOCK_METHOD1( void, QPointF, position )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::createMimeDataFromSelection
// -----------------------------------------------------------------------------
//
QMimeData * HbAbstractEdit::createMimeDataFromSelection(  ) const
    {
    SMC_MOCK_METHOD0( QMimeData * )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::filterInputText
// -----------------------------------------------------------------------------
//
QString HbAbstractEdit::filterInputText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( QString, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::canInsertFromMimeData
// -----------------------------------------------------------------------------
//
bool HbAbstractEdit::canInsertFromMimeData( 
        const QMimeData * source ) const
    {
    SMC_MOCK_METHOD1( bool, const QMimeData *, source )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::insertFromMimeData
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::insertFromMimeData( 
        const QMimeData * source )
    {
    SMC_MOCK_METHOD1( void, const QMimeData *, source )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::rectForPosition
// -----------------------------------------------------------------------------
//
QRectF HbAbstractEdit::rectForPosition( 
        int position,
        QTextLine::Edge edge ) const
    {
    SMC_MOCK_METHOD2( QRectF, int, position, 
        QTextLine::Edge, edge )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::updateGeometry
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::updateGeometry(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::polish
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::polish( 
        HbStyleParameters & params )
    {
    SMC_MOCK_METHOD1( void, HbStyleParameters &, params )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::itemChange
// -----------------------------------------------------------------------------
//
QVariant HbAbstractEdit::itemChange( 
        GraphicsItemChange change,
        const QVariant & value )
    {
    SMC_MOCK_METHOD2( QVariant, GraphicsItemChange, change, 
        const QVariant &, value )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::gestureEvent
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::gestureEvent( 
        QGestureEvent * e )
    {
    SMC_MOCK_METHOD1( void, QGestureEvent *, e )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setPlainText
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setPlainText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::setHtml
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::setHtml( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::documentLayoutChanged
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::documentLayoutChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::documentSizeChanged
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::documentSizeChanged( 
        const QSizeF & size )
    {
    SMC_MOCK_METHOD1( void, const QSizeF &, size )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::blockCountChanged
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::blockCountChanged( 
        int newBlockCount )
    {
    SMC_MOCK_METHOD1( void, int, newBlockCount )
    }


// -----------------------------------------------------------------------------
// HbAbstractEdit::timerEvent
// -----------------------------------------------------------------------------
//
void HbAbstractEdit::timerEvent( 
        QTimerEvent * e )
    {
    SMC_MOCK_METHOD1( void, QTimerEvent *, e )
    }


