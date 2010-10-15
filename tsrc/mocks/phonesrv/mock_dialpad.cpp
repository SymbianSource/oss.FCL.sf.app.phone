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
#include <QPainter>
#include "dialpad.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Dialpad::Dialpad
// -----------------------------------------------------------------------------
//
Dialpad::Dialpad(  ) : mMainWindow(*(const HbMainWindow *)(0))
    {
    
    }


// -----------------------------------------------------------------------------
// Dialpad::Dialpad
// -----------------------------------------------------------------------------
//
Dialpad::Dialpad( 
        const HbMainWindow & mainWindow ) : mMainWindow(mainWindow)
    {
    
    }


// -----------------------------------------------------------------------------
// Dialpad::~Dialpad
// -----------------------------------------------------------------------------
//
Dialpad::~Dialpad(  )
    {
    
    }


// -----------------------------------------------------------------------------
// Dialpad::editor
// -----------------------------------------------------------------------------
//
HbLineEdit & Dialpad::editor(  ) const
    {
    SMC_MOCK_METHOD0( HbLineEdit & )
    }


// -----------------------------------------------------------------------------
// Dialpad::isOpen
// -----------------------------------------------------------------------------
//
bool Dialpad::isOpen(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// Dialpad::isCallButtonEnabled
// -----------------------------------------------------------------------------
//
bool Dialpad::isCallButtonEnabled(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// Dialpad::openDialpad
// -----------------------------------------------------------------------------
//
void Dialpad::openDialpad(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// Dialpad::closeDialpad
// -----------------------------------------------------------------------------
//
void Dialpad::closeDialpad(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// Dialpad::setCallButtonEnabled
// -----------------------------------------------------------------------------
//
void Dialpad::setCallButtonEnabled( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// Dialpad::setTapOutsideDismiss
// -----------------------------------------------------------------------------
//
void Dialpad::setTapOutsideDismiss( 
        bool dismiss )
    {
    SMC_MOCK_METHOD1( void, bool, dismiss )
    }


// -----------------------------------------------------------------------------
// Dialpad::paint
// -----------------------------------------------------------------------------
//
void Dialpad::paint( 
        QPainter * painter,
        const QStyleOptionGraphicsItem * option,
        QWidget * widget )
    {
    SMC_MOCK_METHOD3( void, QPainter *, painter, 
        const QStyleOptionGraphicsItem *, option, 
        QWidget *, widget )
    }


// -----------------------------------------------------------------------------
// Dialpad::changeEvent
// -----------------------------------------------------------------------------
//
void Dialpad::changeEvent( 
        QEvent * event )
    {
    SMC_MOCK_METHOD1( void, QEvent *, event )
    }


// -----------------------------------------------------------------------------
// Dialpad::showEvent
// -----------------------------------------------------------------------------
//
void Dialpad::showEvent( 
        QShowEvent * event )
    {
    SMC_MOCK_METHOD1( void, QShowEvent *, event )
    }


// -----------------------------------------------------------------------------
// Dialpad::hideEvent
// -----------------------------------------------------------------------------
//
void Dialpad::hideEvent( 
        QHideEvent * event )
    {
    SMC_MOCK_METHOD1( void, QHideEvent *, event )
    }


// -----------------------------------------------------------------------------
// Dialpad::closeEvent
// -----------------------------------------------------------------------------
//
void Dialpad::closeEvent( 
        QCloseEvent * event )
    {
    SMC_MOCK_METHOD1( void, QCloseEvent *, event )
    }


// -----------------------------------------------------------------------------
// Dialpad::gestureEvent
// -----------------------------------------------------------------------------
//
void Dialpad::gestureEvent( 
        QGestureEvent * event )
    {
    SMC_MOCK_METHOD1( void, QGestureEvent *, event )
    }


// -----------------------------------------------------------------------------
// Dialpad::closeAnimValueChanged
// -----------------------------------------------------------------------------
//
void Dialpad::closeAnimValueChanged( 
        qreal value )
    {
    SMC_MOCK_METHOD1( void, qreal, value )
    }


// -----------------------------------------------------------------------------
// Dialpad::closeAnimFinished
// -----------------------------------------------------------------------------
//
void Dialpad::closeAnimFinished(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// Dialpad::openAnimValueChanged
// -----------------------------------------------------------------------------
//
void Dialpad::openAnimValueChanged( 
        qreal value )
    {
    SMC_MOCK_METHOD1( void, qreal, value )
    }


// -----------------------------------------------------------------------------
// Dialpad::openAnimFinished
// -----------------------------------------------------------------------------
//
void Dialpad::openAnimFinished(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// Dialpad::orientationChangeStarted
// -----------------------------------------------------------------------------
//
void Dialpad::orientationChangeStarted(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// Dialpad::orientationChangeFinished
// -----------------------------------------------------------------------------
//
void Dialpad::orientationChangeFinished( 
        Qt::Orientation current )
    {
    SMC_MOCK_METHOD1( void, Qt::Orientation, current )
    }


