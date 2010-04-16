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
#include "infowidgetpreferences.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// InfoWidgetPreferences::InfoWidgetPreferences
// -----------------------------------------------------------------------------
//
InfoWidgetPreferences::InfoWidgetPreferences( 
        QObject * parent ):
    QObject( parent )
    {
    
    }


// -----------------------------------------------------------------------------
// InfoWidgetPreferences::~InfoWidgetPreferences
// -----------------------------------------------------------------------------
//
InfoWidgetPreferences::~InfoWidgetPreferences(  )
    {
    
    }


// -----------------------------------------------------------------------------
// InfoWidgetPreferences::loadPreferences
// -----------------------------------------------------------------------------
//
void InfoWidgetPreferences::loadPreferences(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// InfoWidgetPreferences::storePreferences
// -----------------------------------------------------------------------------
//
void InfoWidgetPreferences::storePreferences(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// InfoWidgetPreferences::preference
// -----------------------------------------------------------------------------
//
QString InfoWidgetPreferences::preference( 
        Option preferenceId ) const
    {
    SMC_MOCK_METHOD1( QString, Option, preferenceId )
    }


// -----------------------------------------------------------------------------
// InfoWidgetPreferences::setPreference
// -----------------------------------------------------------------------------
//
void InfoWidgetPreferences::setPreference( 
        Option preferenceId,
        const QString & preferenceString )
    {
    SMC_MOCK_METHOD2( void, Option, preferenceId, 
        const QString &, preferenceString )
    }


// -----------------------------------------------------------------------------
// InfoWidgetPreferences::visibleItemCount
// -----------------------------------------------------------------------------
//
int InfoWidgetPreferences::visibleItemCount(  )
    {
    SMC_MOCK_METHOD0( int )
    }


