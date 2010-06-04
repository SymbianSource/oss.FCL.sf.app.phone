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
#include <hbicon.h>

class HbIconPrivate
{
public:
    HbIconPrivate(){};
    ~HbIconPrivate(){};
    
    QAtomicInt ref;
};

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbIcon::HbIcon
// -----------------------------------------------------------------------------
//
HbIcon::HbIcon(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbIcon::HbIcon
// -----------------------------------------------------------------------------
//
HbIcon::HbIcon( 
        const QString & iconName )
    {
    
    }


// -----------------------------------------------------------------------------
// HbIcon::HbIcon
// -----------------------------------------------------------------------------
//
HbIcon::HbIcon( 
        const QIcon & icon )
    {
    
    }


// -----------------------------------------------------------------------------
// HbIcon::HbIcon
// -----------------------------------------------------------------------------
//
HbIcon::HbIcon( 
        const HbIcon & other )
    {
    
    }

// -----------------------------------------------------------------------------
// HbIcon::~HbIcon
// -----------------------------------------------------------------------------
//
HbIcon::~HbIcon(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbIcon::isNull
// -----------------------------------------------------------------------------
//
bool HbIcon::isNull(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbIcon::clear
// -----------------------------------------------------------------------------
//
void HbIcon::clear(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbIcon::pixmap
// -----------------------------------------------------------------------------
//
QPixmap HbIcon::pixmap(  )
    {
//    SMC_MOCK_METHOD0( QPixmap )
    }


// -----------------------------------------------------------------------------
// HbIcon::setColor
// -----------------------------------------------------------------------------
//
void HbIcon::setColor( 
        const QColor & color )
    {
//    SMC_MOCK_METHOD1( void, const QColor &, color )
    }


// -----------------------------------------------------------------------------
// HbIcon::color
// -----------------------------------------------------------------------------
//
QColor HbIcon::color(  ) const
    {
//    SMC_MOCK_METHOD0( QColor )
    }


// -----------------------------------------------------------------------------
// HbIcon::iconName
// -----------------------------------------------------------------------------
//
QString HbIcon::iconName(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbIcon::setIconName
// -----------------------------------------------------------------------------
//
void HbIcon::setIconName( 
        const QString & iconName )
    {
    SMC_MOCK_METHOD1( void, const QString &, iconName )
    }


// -----------------------------------------------------------------------------
// HbIcon::iconName
// -----------------------------------------------------------------------------
//
QString HbIcon::iconName( 
        QIcon::Mode mode,
        QIcon::State state ) const
    {
//    SMC_MOCK_METHOD2( QString, QIcon::Mode, mode, 
//        QIcon::State, state )
    }


// -----------------------------------------------------------------------------
// HbIcon::setIconName
// -----------------------------------------------------------------------------
//
void HbIcon::setIconName( 
        const QString & iconName,
        QIcon::Mode mode,
        QIcon::State state )
    {
//    SMC_MOCK_METHOD3( void, const QString &, iconName, 
//        QIcon::Mode, mode, 
//        QIcon::State, state )
    }


// -----------------------------------------------------------------------------
// HbIcon::defaultSize
// -----------------------------------------------------------------------------
//
QSizeF HbIcon::defaultSize(  ) const
    {
//    SMC_MOCK_METHOD0( QSizeF )
    }


// -----------------------------------------------------------------------------
// HbIcon::size
// -----------------------------------------------------------------------------
//
QSizeF HbIcon::size(  ) const
    {
//    SMC_MOCK_METHOD0( QSizeF )
    }


// -----------------------------------------------------------------------------
// HbIcon::setSize
// -----------------------------------------------------------------------------
//
void HbIcon::setSize( 
        const QSizeF & size )
    {
//    SMC_MOCK_METHOD1( void, const QSizeF &, size )
    }


// -----------------------------------------------------------------------------
// HbIcon::setHeight
// -----------------------------------------------------------------------------
//
void HbIcon::setHeight( 
        qreal height )
    {
//    SMC_MOCK_METHOD1( void, qreal, height )
    }


// -----------------------------------------------------------------------------
// HbIcon::setWidth
// -----------------------------------------------------------------------------
//
void HbIcon::setWidth( 
        qreal width )
    {
//    SMC_MOCK_METHOD1( void, qreal, width )
    }


// -----------------------------------------------------------------------------
// HbIcon::width
// -----------------------------------------------------------------------------
//
qreal HbIcon::width(  ) const
    {
//    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// HbIcon::height
// -----------------------------------------------------------------------------
//
qreal HbIcon::height(  ) const
    {
//    SMC_MOCK_METHOD0( qreal )
    }

// -----------------------------------------------------------------------------
// HbIcon::setMirroringMode
// -----------------------------------------------------------------------------
//
void HbIcon::setMirroringMode( 
        MirroringMode mode )
    {
//    SMC_MOCK_METHOD1( void, MirroringMode, mode )
    }

// -----------------------------------------------------------------------------
// HbIcon::setFlags
// -----------------------------------------------------------------------------
//
void HbIcon::setFlags( 
        Flags flags )
    {
//    SMC_MOCK_METHOD1( void, Flags, flags )
    }


// -----------------------------------------------------------------------------
// HbIcon::paint
// -----------------------------------------------------------------------------
//
void HbIcon::paint( 
        QPainter * painter,
        const QRectF & rect,
        Qt::AspectRatioMode aspectRatioMode,
        Qt::Alignment alignment,
        QIcon::Mode mode,
        QIcon::State state ) const
    {
/*    SMC_MOCK_METHOD6( void, QPainter *, painter, 
        const QRectF &, rect, 
        Qt::AspectRatioMode, aspectRatioMode, 
        Qt::Alignment, alignment, 
        QIcon::Mode, mode, 
        QIcon::State, state )*/
    }


// -----------------------------------------------------------------------------
// HbIcon::qicon
// -----------------------------------------------------------------------------
//
QIcon & HbIcon::qicon(  ) const
    {
 //   SMC_MOCK_METHOD0( QIcon & )
    }


