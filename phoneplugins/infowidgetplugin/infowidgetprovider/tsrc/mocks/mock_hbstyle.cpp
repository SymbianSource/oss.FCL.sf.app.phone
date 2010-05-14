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
#include "hbstyle.h"

//needed for TSmcValue<HbStyleParameters &,100>Size()const
class HbStyleParameters {};
//needed for TSmcValue<HbWidget *,100>Size()const
class HbWidget {};

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbStyle::HbStyle
// -----------------------------------------------------------------------------
//
HbStyle::HbStyle( ):d_ptr(NULL)
       // QObject * parent )
    {
   // Q_UNUSED(parent);
    }


// -----------------------------------------------------------------------------
// HbStyle::~HbStyle
// -----------------------------------------------------------------------------
//
HbStyle::~HbStyle(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbStyle::createPrimitive
// -----------------------------------------------------------------------------
//
QGraphicsItem * HbStyle::createPrimitive( 
        HbStyle::Primitive primitive,
        QGraphicsItem * parent ) const
    {
    SMC_MOCK_METHOD2( QGraphicsItem *, HbStyle::Primitive, primitive, 
        QGraphicsItem *, parent )
    }


// -----------------------------------------------------------------------------
// HbStyle::updatePrimitive
// -----------------------------------------------------------------------------
//
void HbStyle::updatePrimitive( 
        QGraphicsItem * item,
        HbStyle::Primitive primitive,
        const QStyleOption * option ) const
    {
    SMC_MOCK_METHOD3( void, QGraphicsItem *, item, 
        HbStyle::Primitive, primitive, 
        const QStyleOption *, option )
    }


// -----------------------------------------------------------------------------
// HbStyle::registerPlugin
// -----------------------------------------------------------------------------
//
int HbStyle::registerPlugin( 
        const QString & pluginName )
    {
    SMC_MOCK_METHOD1( int, const QString &, pluginName )
    }


// -----------------------------------------------------------------------------
// HbStyle::unregisterPlugin
// -----------------------------------------------------------------------------
//
void HbStyle::unregisterPlugin( 
        const QString & pluginName )
    {
    SMC_MOCK_METHOD1( void, const QString &, pluginName )
    }


// -----------------------------------------------------------------------------
// HbStyle::setItemName
// -----------------------------------------------------------------------------
//
void HbStyle::setItemName( 
        QGraphicsItem * item,
        const QString & name )
    {
    SMC_MOCK_METHOD2( void, QGraphicsItem *, item, 
        const QString &, name )
    }


// -----------------------------------------------------------------------------
// HbStyle::itemName
// -----------------------------------------------------------------------------
//
QString HbStyle::itemName( 
        const QGraphicsItem * item )
    {
    SMC_MOCK_METHOD1( QString, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// HbStyle::parameter
// -----------------------------------------------------------------------------
//
bool HbStyle::parameter( 
        const QString & parameter,
        qreal & value,
        const HbDeviceProfile & profile ) const
    {
    SMC_MOCK_METHOD3( bool, const QString &, parameter, 
        qreal &, value, 
        const HbDeviceProfile &, profile )
    }


// -----------------------------------------------------------------------------
// HbStyle::parameters
// -----------------------------------------------------------------------------
//
void HbStyle::parameters( 
        HbStyleParameters & parameters,
        const HbDeviceProfile & profile ) const
    {
    SMC_MOCK_METHOD2( void, HbStyleParameters &, parameters, 
        const HbDeviceProfile &, profile )
    }


// -----------------------------------------------------------------------------
// HbStyle::polish
// -----------------------------------------------------------------------------
//
void HbStyle::polish( 
        HbWidget * widget,
        HbStyleParameters & params )
    {
    SMC_MOCK_METHOD2( void, HbWidget *, widget, 
        HbStyleParameters &, params )
    }


// -----------------------------------------------------------------------------
// HbStyle::updateThemedParams
// -----------------------------------------------------------------------------
//
void HbStyle::updateThemedParams( 
        HbWidget * widget )
    {
    SMC_MOCK_METHOD1( void, HbWidget *, widget )
    }


// -----------------------------------------------------------------------------
// HbStyle::hasOrientationSpecificStyleRules
// -----------------------------------------------------------------------------
//
bool HbStyle::hasOrientationSpecificStyleRules( 
        HbWidget * widget )
    {
    SMC_MOCK_METHOD1( bool, HbWidget *, widget )
    }


// -----------------------------------------------------------------------------
// HbStyle::HbStyle
// -----------------------------------------------------------------------------
//
HbStyle::HbStyle( 
        HbStylePrivate & dd,
        QStyle * parent ):d_ptr(NULL)
    //:
    //QObject( /*dd, parent*/ )
    {
    Q_UNUSED(dd);
    Q_UNUSED(parent);
    }


