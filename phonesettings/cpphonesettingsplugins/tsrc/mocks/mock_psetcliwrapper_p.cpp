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
#include <QObject>
#include <rsssettings.h>
#include <psetcontainer.h>
#include <psetcli.h>
#include <psuiconstants.h>
#include <smcmockclassincludes.h>
#include "psetcliwrapper_p.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PSetCliWrapperPrivate::PSetCliWrapperPrivate
// -----------------------------------------------------------------------------
//
PSetCliWrapperPrivate::PSetCliWrapperPrivate( 
        PSetCliWrapper & owner )
    : m_Owner(owner)
    {
    
    }


// -----------------------------------------------------------------------------
// PSetCliWrapperPrivate::~PSetCliWrapperPrivate
// -----------------------------------------------------------------------------
//
PSetCliWrapperPrivate::~PSetCliWrapperPrivate(  )
    {
    
    }


// -----------------------------------------------------------------------------
// PSetCliWrapperPrivate::SetEngineContact
// -----------------------------------------------------------------------------
//
void PSetCliWrapperPrivate::SetEngineContact( 
        MPsetCli * aEngine )
    {
    SMC_MOCK_METHOD1( void, MPsetCli *, aEngine )
    }


// -----------------------------------------------------------------------------
// PSetCliWrapperPrivate::HandleCliRequestingL
// -----------------------------------------------------------------------------
//
void PSetCliWrapperPrivate::HandleCliRequestingL( 
        TBool aOngoing,
        TBool aInterrupted )
    {
    SMC_MOCK_METHOD2( void, TBool, aOngoing, 
        TBool, aInterrupted )
    }


// -----------------------------------------------------------------------------
// PSetCliWrapperPrivate::CliInformationL
// -----------------------------------------------------------------------------
//
void PSetCliWrapperPrivate::CliInformationL( 
        TPsuiCli aType )
    {
    SMC_MOCK_METHOD1( void, TPsuiCli, aType )
    }


// -----------------------------------------------------------------------------
// PSetCliWrapperPrivate::HandleCliStatusL
// -----------------------------------------------------------------------------
//
void PSetCliWrapperPrivate::HandleCliStatusL( 
        TUint8 aBsc[KPSetNumberOfBsc],
        TPsuiCli aMode )
    {
    // TODO: make compile
    Q_UNUSED(aBsc)
    Q_UNUSED(aMode)
    //SMC_MOCK_METHOD2( void, TUint8[KPSetNumberOfBsc], aBsc, TPsuiCli, aMode )
    }


// -----------------------------------------------------------------------------
// PSetCliWrapperPrivate::HandleCnapStatusL
// -----------------------------------------------------------------------------
//
void PSetCliWrapperPrivate::HandleCnapStatusL( 
        TInt aStatus )
    {
    SMC_MOCK_METHOD1( void, TInt, aStatus )
    }


// -----------------------------------------------------------------------------
// PSetCliWrapperPrivate::HandleCliErrorL
// -----------------------------------------------------------------------------
//
void PSetCliWrapperPrivate::HandleCliErrorL( 
        TInt aError )
    {
    SMC_MOCK_METHOD1( void, TInt, aError )
    }


