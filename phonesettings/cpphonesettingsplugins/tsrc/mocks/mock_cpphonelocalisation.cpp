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
#include "cpphonelocalisation.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CpPhoneLocalisation::CpPhoneLocalisation
// -----------------------------------------------------------------------------
//
CpPhoneLocalisation::CpPhoneLocalisation( 
        QObject * /*parent*/ )
    //:
    //QObject( /*parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CpPhoneLocalisation::~CpPhoneLocalisation
// -----------------------------------------------------------------------------
//
CpPhoneLocalisation::~CpPhoneLocalisation(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CpPhoneLocalisation::installTranslator
// -----------------------------------------------------------------------------
//
bool CpPhoneLocalisation::installTranslator( 
        TranslationFileId translationFileId )
    {
    SMC_MOCK_METHOD1( bool, TranslationFileId, translationFileId )
    }


// -----------------------------------------------------------------------------
// CpPhoneLocalisation::removeTranslators
// -----------------------------------------------------------------------------
//
void CpPhoneLocalisation::removeTranslators(  )
    {
    SMC_MOCK_METHOD0( void )
    }


