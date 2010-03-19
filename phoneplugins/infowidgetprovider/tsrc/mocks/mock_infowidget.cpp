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
#include "infowidget.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// InfoWidget::InfoWidget
// -----------------------------------------------------------------------------
//
InfoWidget::InfoWidget( 
        QGraphicsItem * parent,
        Qt::WindowFlags flags )
    //:
    //HsWidget( /*parent, flags*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// InfoWidget::~InfoWidget
// -----------------------------------------------------------------------------
//
InfoWidget::~InfoWidget(  )
    {
    
    }


// -----------------------------------------------------------------------------
// InfoWidget::boundingRect
// -----------------------------------------------------------------------------
//
QRectF InfoWidget::boundingRect(  ) const
    {
    SMC_MOCK_METHOD0( QRectF )
    }


// -----------------------------------------------------------------------------
// InfoWidget::shape
// -----------------------------------------------------------------------------
//
QPainterPath InfoWidget::shape(  ) const
    {
    SMC_MOCK_METHOD0( QPainterPath )
    }


// -----------------------------------------------------------------------------
// InfoWidget::sizeHint
// -----------------------------------------------------------------------------
//
QSizeF InfoWidget::sizeHint( 
        Qt::SizeHint which,
        const QSizeF & constraint ) const
    {
    SMC_MOCK_METHOD2( QSizeF, Qt::SizeHint, which, 
        const QSizeF &, constraint )
    }


// -----------------------------------------------------------------------------
// InfoWidget::sizePolicy
// -----------------------------------------------------------------------------
//
QSizePolicy InfoWidget::sizePolicy(  ) const
    {
    SMC_MOCK_METHOD0( QSizePolicy )
    }


// -----------------------------------------------------------------------------
// InfoWidget::Q_PROPERTY
// -----------------------------------------------------------------------------
//
void InfoWidget::Q_PROPERTY( 
        QString homeZoneDisplay READ homeZoneDisplay WRITE setHomeZoneDisplay ) Q_PROPERTY ( QString mcnDisplay READ mcnDisplay WRITE setMcnDisplay ) Q_PROPERTY ( QString activeLineDisplay READ activeLineDisplay WRITE setActiveLineDisplay ) Q_PROPERTY ( QString satDisplay READ satDisplay WRITE setSatDisplay ) QString homeZoneDisplay ( )
    {
    SMC_MOCK_METHOD1( void, QString homeZoneDisplay READ homeZoneDisplay WRITE setHomeZoneDisplay ) Q_PROPERTY ( QString mcnDisplay READ mcnDisplay WRITE setMcnDisplay ) Q_PROPERTY ( QString activeLineDisplay READ activeLineDisplay WRITE setActiveLineDisplay ) Q_PROPERTY ( QString satDisplay READ satDisplay WRITE setSatDisplay ) QString homeZoneDisplay, ( )
    }


// -----------------------------------------------------------------------------
// InfoWidget::setHomeZoneDisplay
// -----------------------------------------------------------------------------
//
void InfoWidget::setHomeZoneDisplay( 
        QString value )
    {
    SMC_MOCK_METHOD1( void, QString, value )
    }


// -----------------------------------------------------------------------------
// InfoWidget::mcnDisplay
// -----------------------------------------------------------------------------
//
QString InfoWidget::mcnDisplay(  )
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// InfoWidget::setMcnDisplay
// -----------------------------------------------------------------------------
//
void InfoWidget::setMcnDisplay( 
        QString value )
    {
    SMC_MOCK_METHOD1( void, QString, value )
    }


// -----------------------------------------------------------------------------
// InfoWidget::activeLineDisplay
// -----------------------------------------------------------------------------
//
QString InfoWidget::activeLineDisplay(  )
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// InfoWidget::setActiveLineDisplay
// -----------------------------------------------------------------------------
//
void InfoWidget::setActiveLineDisplay( 
        QString value )
    {
    SMC_MOCK_METHOD1( void, QString, value )
    }


// -----------------------------------------------------------------------------
// InfoWidget::satDisplay
// -----------------------------------------------------------------------------
//
QString InfoWidget::satDisplay(  )
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// InfoWidget::setSatDisplay
// -----------------------------------------------------------------------------
//
void InfoWidget::setSatDisplay( 
        QString value )
    {
    SMC_MOCK_METHOD1( void, QString, value )
    }


// -----------------------------------------------------------------------------
// InfoWidget::onInitialize
// -----------------------------------------------------------------------------
//
void InfoWidget::onInitialize(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// InfoWidget::onUninitialize
// -----------------------------------------------------------------------------
//
void InfoWidget::onUninitialize(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// InfoWidget::onShow
// -----------------------------------------------------------------------------
//
void InfoWidget::onShow(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// InfoWidget::onHide
// -----------------------------------------------------------------------------
//
void InfoWidget::onHide(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// InfoWidget::readModel
// -----------------------------------------------------------------------------
//
void InfoWidget::readModel(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// InfoWidget::handleModelError
// -----------------------------------------------------------------------------
//
void InfoWidget::handleModelError( 
        int operation,
        int errorCode )
    {
    SMC_MOCK_METHOD2( void, int, operation, 
        int, errorCode )
    }


// -----------------------------------------------------------------------------
// InfoWidget::homeZoneDisplaySettingChanged
// -----------------------------------------------------------------------------
//
void InfoWidget::homeZoneDisplaySettingChanged( 
        int state )
    {
    SMC_MOCK_METHOD1( void, int, state )
    }


// -----------------------------------------------------------------------------
// InfoWidget::mcnDisplaySettingChanged
// -----------------------------------------------------------------------------
//
void InfoWidget::mcnDisplaySettingChanged( 
        int state )
    {
    SMC_MOCK_METHOD1( void, int, state )
    }


// -----------------------------------------------------------------------------
// InfoWidget::activeLineDisplaySettingChanged
// -----------------------------------------------------------------------------
//
void InfoWidget::activeLineDisplaySettingChanged( 
        int state )
    {
    SMC_MOCK_METHOD1( void, int, state )
    }


// -----------------------------------------------------------------------------
// InfoWidget::satDisplaySettingChanged
// -----------------------------------------------------------------------------
//
void InfoWidget::satDisplaySettingChanged( 
        int state )
    {
    SMC_MOCK_METHOD1( void, int, state )
    }


// -----------------------------------------------------------------------------
// InfoWidget::settingsEditingFinished
// -----------------------------------------------------------------------------
//
void InfoWidget::settingsEditingFinished(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// InfoWidget::setPreferences
// -----------------------------------------------------------------------------
//
void InfoWidget::setPreferences( 
        const QStringList & names )
    {
    SMC_MOCK_METHOD1( void, const QStringList &, names )
    }


// -----------------------------------------------------------------------------
// InfoWidget::onStart
// -----------------------------------------------------------------------------
//
HsWidget::StartResult InfoWidget::onStart(  )
    {
    SMC_MOCK_METHOD0( HsWidget::StartResult )
    }


// -----------------------------------------------------------------------------
// InfoWidget::onStop
// -----------------------------------------------------------------------------
//
HsWidget::StopResult InfoWidget::onStop(  )
    {
    SMC_MOCK_METHOD0( HsWidget::StopResult )
    }


// -----------------------------------------------------------------------------
// InfoWidget::onSuspend
// -----------------------------------------------------------------------------
//
HsWidget::SuspendResult InfoWidget::onSuspend(  )
    {
    SMC_MOCK_METHOD0( HsWidget::SuspendResult )
    }


// -----------------------------------------------------------------------------
// InfoWidget::onResume
// -----------------------------------------------------------------------------
//
HsWidget::ResumeResult InfoWidget::onResume(  )
    {
    SMC_MOCK_METHOD0( HsWidget::ResumeResult )
    }


// -----------------------------------------------------------------------------
// InfoWidget::paint
// -----------------------------------------------------------------------------
//
void InfoWidget::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// InfoWidget::mouseDoubleClickEvent
// -----------------------------------------------------------------------------
//
void InfoWidget::mouseDoubleClickEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneMouseEvent *, event )
    }


// -----------------------------------------------------------------------------
// InfoWidget::mousePressEvent
// -----------------------------------------------------------------------------
//
void InfoWidget::mousePressEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGraphicsSceneMouseEvent *, event )
    }


// -----------------------------------------------------------------------------
// InfoWidget::updateInfoDisplay
// -----------------------------------------------------------------------------
//
void InfoWidget::updateInfoDisplay(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// InfoWidget::initializePreferences
// -----------------------------------------------------------------------------
//
void InfoWidget::initializePreferences(  )
    {
    SMC_MOCK_METHOD0( void )
    }


