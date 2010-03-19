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
#include <hblabel.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbLabel::Q_PROPERTY
// -----------------------------------------------------------------------------
/*
void HbLabel::Q_PROPERTY( 
        QString plainText READ plainText WRITE setPlainText ) Q_PROPERTY ( QString html READ html WRITE setHtml ) Q_PROPERTY ( Qt::Alignment alignment READ alignment WRITE setAlignment ) Q_PROPERTY ( Qt::TextElideMode elideMode READ elideMode WRITE setElideMode ) Q_PROPERTY ( Hb::TextWrapping textWrapping READ textWrapping WRITE setTextWrapping ) Q_PROPERTY ( QColor textColor READ textColor WRITE setTextColor ) Q_PROPERTY ( HbIcon icon READ icon WRITE setIcon ) Q_PROPERTY ( Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode WRITE setAspectRatioMode ) Q_ENUMS ( QIcon::Mode QIcon::State ) Q_PROPERTY ( QString text READ text WRITE setText NOTIFY textChanged USER true ) Q_PROPERTY ( QColor color READ color WRITE setColor ) Q_PROPERTY ( int textFlags READ textFlags WRITE setTextFlags ) Q_PROPERTY ( Hb::LogicalFontName logicalFont READ logicalFont WRITE setLogicalFont ) Q_PROPERTY ( QIcon::Mode mode READ mode WRITE setMode ) Q_PROPERTY ( QIcon::State state READ state WRITE setState ) Q_PROPERTY ( Qt::TextFormat textFormat READ textFormat WRITE setTextFormat ):HbLabel ( QGraphicsItem * parent )
    {
    SMC_MOCK_METHOD1( void, QString plainText READ plainText WRITE setPlainText ) Q_PROPERTY ( QString html READ html WRITE setHtml ) Q_PROPERTY ( Qt::Alignment alignment READ alignment WRITE setAlignment ) Q_PROPERTY ( Qt::TextElideMode elideMode READ elideMode WRITE setElideMode ) Q_PROPERTY ( Hb::TextWrapping textWrapping READ textWrapping WRITE setTextWrapping ) Q_PROPERTY ( QColor textColor READ textColor WRITE setTextColor ) Q_PROPERTY ( HbIcon icon READ icon WRITE setIcon ) Q_PROPERTY ( Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode WRITE setAspectRatioMode ) Q_ENUMS ( QIcon::Mode QIcon::State ) Q_PROPERTY ( QString text READ text WRITE setText NOTIFY textChanged USER true ) Q_PROPERTY ( QColor color READ color WRITE setColor ) Q_PROPERTY ( int textFlags READ textFlags WRITE setTextFlags ) Q_PROPERTY ( Hb::LogicalFontName logicalFont READ logicalFont WRITE setLogicalFont ) Q_PROPERTY ( QIcon::Mode mode READ mode WRITE setMode ) Q_PROPERTY ( QIcon::State state READ state WRITE setState ) Q_PROPERTY ( Qt::TextFormat textFormat READ textFormat WRITE setTextFormat ):HbLabel ( QGraphicsItem *, parent )
    }
*/

// -----------------------------------------------------------------------------
// HbLabel::HbLabel
// -----------------------------------------------------------------------------
//
HbLabel::HbLabel( 
        QGraphicsItem * parent )
    {

    }
    
// -----------------------------------------------------------------------------
// HbLabel::HbLabel
// -----------------------------------------------------------------------------
//
HbLabel::HbLabel( 
        const QString & displayText,
        QGraphicsItem * parent )
    {

    }


// -----------------------------------------------------------------------------
// HbLabel::~HbLabel
// -----------------------------------------------------------------------------
//
HbLabel::~HbLabel(  )
    {

    }


// -----------------------------------------------------------------------------
// HbLabel::plainText
// -----------------------------------------------------------------------------
//
QString HbLabel::plainText(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbLabel::html
// -----------------------------------------------------------------------------
//
QString HbLabel::html(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbLabel::setElideMode
// -----------------------------------------------------------------------------
//
void HbLabel::setElideMode( 
        Qt::TextElideMode elideMode )
    {
   // SMC_MOCK_METHOD1( void, Qt::TextElideMode, elideMode )
    }


// -----------------------------------------------------------------------------
// HbLabel::elideMode
// -----------------------------------------------------------------------------
//
Qt::TextElideMode HbLabel::elideMode(  ) const
    {
   // SMC_MOCK_METHOD0( Qt::TextElideMode )
    }


// -----------------------------------------------------------------------------
// HbLabel::setTextWrapping
// -----------------------------------------------------------------------------
//
void HbLabel::setTextWrapping( 
        Hb::TextWrapping mode )
    {
  //  SMC_MOCK_METHOD1( void, Hb::TextWrapping, mode )
    }


// -----------------------------------------------------------------------------
// HbLabel::textWrapping
// -----------------------------------------------------------------------------
//
Hb::TextWrapping HbLabel::textWrapping(  ) const
    {
   // SMC_MOCK_METHOD0( Hb::TextWrapping )
    }


// -----------------------------------------------------------------------------
// HbLabel::setIcon
// -----------------------------------------------------------------------------
//
void HbLabel::setIcon( 
        const HbIcon & icon )
    {
  //  SMC_MOCK_METHOD1( void, const HbIcon &, icon )
    }


// -----------------------------------------------------------------------------
// HbLabel::icon
// -----------------------------------------------------------------------------
//
HbIcon HbLabel::icon(  ) const
    {
   // SMC_MOCK_METHOD0( HbIcon )
    }


// -----------------------------------------------------------------------------
// HbLabel::setAspectRatioMode
// -----------------------------------------------------------------------------
//
void HbLabel::setAspectRatioMode( 
        Qt::AspectRatioMode mode )
    {
    //SMC_MOCK_METHOD1( void, Qt::AspectRatioMode, mode )
    }


// -----------------------------------------------------------------------------
// HbLabel::aspectRatioMode
// -----------------------------------------------------------------------------
//
Qt::AspectRatioMode HbLabel::aspectRatioMode(  ) const
    {
   // SMC_MOCK_METHOD0( Qt::AspectRatioMode )
    }


// -----------------------------------------------------------------------------
// HbLabel::setAlignment
// -----------------------------------------------------------------------------
//
void HbLabel::setAlignment( 
        Qt::Alignment alignment )
    {
   // SMC_MOCK_METHOD1( void, Qt::Alignment, alignment )
    }


// -----------------------------------------------------------------------------
// HbLabel::alignment
// -----------------------------------------------------------------------------
//
Qt::Alignment HbLabel::alignment(  ) const
    {
   // SMC_MOCK_METHOD0( Qt::Alignment )
    }


// -----------------------------------------------------------------------------
// HbLabel::setTextColor
// -----------------------------------------------------------------------------
//
void HbLabel::setTextColor( 
        const QColor & textColor )
    {
   // SMC_MOCK_METHOD1( void, const QColor &, textColor )
    }


// -----------------------------------------------------------------------------
// HbLabel::textColor
// -----------------------------------------------------------------------------
//
QColor HbLabel::textColor(  ) const
    {
  //  SMC_MOCK_METHOD0( QColor )
    }


// -----------------------------------------------------------------------------
// HbLabel::isEmpty
// -----------------------------------------------------------------------------
//
bool HbLabel::isEmpty(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbLabel::setGeometry
// -----------------------------------------------------------------------------
//
void HbLabel::setGeometry( 
        const QRectF & rect )
    {
  //  SMC_MOCK_METHOD1( void, const QRectF &, rect )
    }


// -----------------------------------------------------------------------------
// HbLabel::primitive
// -----------------------------------------------------------------------------
//
QGraphicsItem * HbLabel::primitive( 
        HbStyle::Primitive primitive ) const
    {
 //   SMC_MOCK_METHOD1( QGraphicsItem *, HbStyle::Primitive, primitive )
    }


// -----------------------------------------------------------------------------
// HbLabel::type
// -----------------------------------------------------------------------------
//
/*
int HbLabel::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }

*/

// -----------------------------------------------------------------------------
// HbLabel::setColor
// -----------------------------------------------------------------------------
//
void HbLabel::setColor( 
        const QColor & textColor )
    {
  //  SMC_MOCK_METHOD1( void, const QColor &, textColor )
    }


// -----------------------------------------------------------------------------
// HbLabel::color
// -----------------------------------------------------------------------------
//
QColor HbLabel::color(  ) const
    {
   // SMC_MOCK_METHOD0( QColor )
    }


// -----------------------------------------------------------------------------
// HbLabel::setLogicalFont
// -----------------------------------------------------------------------------
//
void HbLabel::setLogicalFont( 
        Hb::LogicalFontName logicalFont )
    {
 //   SMC_MOCK_METHOD1( void, Hb::LogicalFontName, logicalFont )
    }


// -----------------------------------------------------------------------------
// HbLabel::logicalFont
// -----------------------------------------------------------------------------
//
Hb::LogicalFontName HbLabel::logicalFont(  ) const
    {
   // SMC_MOCK_METHOD0( Hb::LogicalFontName )
    }


// -----------------------------------------------------------------------------
// HbLabel::text
// -----------------------------------------------------------------------------
//
QString HbLabel::text(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbLabel::setTextFlags
// -----------------------------------------------------------------------------
//
void HbLabel::setTextFlags( 
        int flags )
    {
    SMC_MOCK_METHOD1( void, int, flags )
    }


// -----------------------------------------------------------------------------
// HbLabel::textFlags
// -----------------------------------------------------------------------------
//
int HbLabel::textFlags(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbLabel::effectiveFont
// -----------------------------------------------------------------------------
//
QFont HbLabel::effectiveFont(  ) const
    {
  //  SMC_MOCK_METHOD0( QFont )
    }


// -----------------------------------------------------------------------------
// HbLabel::setMode
// -----------------------------------------------------------------------------
//
void HbLabel::setMode( 
        QIcon::Mode mode )
    {
   // SMC_MOCK_METHOD1( void, QIcon::Mode, mode )
    }


// -----------------------------------------------------------------------------
// HbLabel::mode
// -----------------------------------------------------------------------------
//
QIcon::Mode HbLabel::mode(  ) const
    {
   // SMC_MOCK_METHOD0( QIcon::Mode )
    }


// -----------------------------------------------------------------------------
// HbLabel::setState
// -----------------------------------------------------------------------------
//
void HbLabel::setState( 
        QIcon::State state )
    {
  //  SMC_MOCK_METHOD1( void, QIcon::State, state )
    }


// -----------------------------------------------------------------------------
// HbLabel::state
// -----------------------------------------------------------------------------
//
QIcon::State HbLabel::state(  ) const
    {
  //  SMC_MOCK_METHOD0( QIcon::State )
    }


// -----------------------------------------------------------------------------
// HbLabel::setTextFormat
// -----------------------------------------------------------------------------
//
void HbLabel::setTextFormat( 
        Qt::TextFormat textFormat )
    {
 //   SMC_MOCK_METHOD1( void, Qt::TextFormat, textFormat )
    }


// -----------------------------------------------------------------------------
// HbLabel::textFormat
// -----------------------------------------------------------------------------
//
Qt::TextFormat HbLabel::textFormat(  ) const
    {
 //   SMC_MOCK_METHOD0( Qt::TextFormat )
    }


// -----------------------------------------------------------------------------
// HbLabel::setMirroringMode
// -----------------------------------------------------------------------------
//
void HbLabel::setMirroringMode( 
        HbIcon::MirroringMode mirroringMode )
    {
 //   SMC_MOCK_METHOD1( void, HbIcon::MirroringMode, mirroringMode )
    }


// -----------------------------------------------------------------------------
// HbLabel::setIconFlags
// -----------------------------------------------------------------------------
//
void HbLabel::setIconFlags( 
        HbIcon::Flags flags )
    {
 //   SMC_MOCK_METHOD1( void, HbIcon::Flags, flags )
    }


// -----------------------------------------------------------------------------
// HbLabel::isNull
// -----------------------------------------------------------------------------
//
bool HbLabel::isNull(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbLabel::setPlainText
// -----------------------------------------------------------------------------
//
void HbLabel::setPlainText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbLabel::setHtml
// -----------------------------------------------------------------------------
//
void HbLabel::setHtml( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbLabel::setNumber
// -----------------------------------------------------------------------------
//
void HbLabel::setNumber( 
        int number )
    {
    SMC_MOCK_METHOD1( void, int, number )
    }


// -----------------------------------------------------------------------------
// HbLabel::setNumber
// -----------------------------------------------------------------------------
//
void HbLabel::setNumber( 
        qreal number )
    {
    SMC_MOCK_METHOD1( void, qreal, number )
    }


// -----------------------------------------------------------------------------
// HbLabel::updatePrimitives
// -----------------------------------------------------------------------------
//
void HbLabel::updatePrimitives(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbLabel::clear
// -----------------------------------------------------------------------------
//
void HbLabel::clear(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbLabel::setText
// -----------------------------------------------------------------------------
//
void HbLabel::setText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbLabel::setNum
// -----------------------------------------------------------------------------
//
void HbLabel::setNum( 
        int num )
    {
    SMC_MOCK_METHOD1( void, int, num )
    }


// -----------------------------------------------------------------------------
// HbLabel::setNum
// -----------------------------------------------------------------------------
//
void HbLabel::setNum( 
        qreal num )
    {
    SMC_MOCK_METHOD1( void, qreal, num )
    }


// -----------------------------------------------------------------------------
// HbLabel::toggleState
// -----------------------------------------------------------------------------
//
void HbLabel::toggleState(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbLabel::HbLabel
// -----------------------------------------------------------------------------
//
HbLabel::HbLabel( 
        HbLabelPrivate & dd,
        QGraphicsItem * parent )
    {
 //   SMC_MOCK_METHOD2( void, HbLabelPrivate &, dd, 
 //       QGraphicsItem *, parent )
    }


// -----------------------------------------------------------------------------
// HbLabel::resizeEvent
// -----------------------------------------------------------------------------
//
void HbLabel::resizeEvent( 
        QGraphicsSceneResizeEvent * event )
    {
  //  SMC_MOCK_METHOD1( void, QGraphicsSceneResizeEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbLabel::changeEvent
// -----------------------------------------------------------------------------
//
void HbLabel::changeEvent( 
        QEvent * event )
    {
 //   SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbLabel::initStyleOption
// -----------------------------------------------------------------------------
//
void HbLabel::initStyleOption( 
        HbStyleOptionLabel * option ) const
    {
//    SMC_MOCK_METHOD1( void, HbStyleOptionLabel *, option )
    }


