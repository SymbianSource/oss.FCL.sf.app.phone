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
#include <MPsetDivertObs.h>
#include <MPhCltEmergencyCallObserver.h>
#include <mmlist.h>
#include <smcmockclassincludes.h>
#include "psetcalldivertingwrapper_p.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::PSetCallDivertingWrapperPrivate
// -----------------------------------------------------------------------------
//
PSetCallDivertingWrapperPrivate::PSetCallDivertingWrapperPrivate( 
        PSetCallDivertingWrapper & owner )
    : m_Owner(owner)
    {
    
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::~PSetCallDivertingWrapperPrivate
// -----------------------------------------------------------------------------
//
PSetCallDivertingWrapperPrivate::~PSetCallDivertingWrapperPrivate(  )
    {
    
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::HandleDivertingChangedL
// -----------------------------------------------------------------------------
//
void PSetCallDivertingWrapperPrivate::HandleDivertingChangedL( 
        const TCallDivertSetting & aSetting,
        TBool aPlural )
    {
    SMC_MOCK_METHOD2( void, const TCallDivertSetting &, aSetting, 
        TBool, aPlural )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::HandleDivertingStatusL
// -----------------------------------------------------------------------------
//
void PSetCallDivertingWrapperPrivate::HandleDivertingStatusL( 
        CMobilePhoneCFList & aSetting,
        TBool aPlural )
    {
    SMC_MOCK_METHOD2( void, CMobilePhoneCFList &, aSetting, 
        TBool, aPlural )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::HandleDivertingErrorL
// -----------------------------------------------------------------------------
//
void PSetCallDivertingWrapperPrivate::HandleDivertingErrorL( 
        TInt aReason )
    {
    SMC_MOCK_METHOD1( void, TInt, aReason )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::HandleCFRequestingL
// -----------------------------------------------------------------------------
//
void PSetCallDivertingWrapperPrivate::HandleCFRequestingL( 
        TBool aOngoing,
        TBool aInterrupted )
    {
    SMC_MOCK_METHOD2( void, TBool, aOngoing, 
        TBool, aInterrupted )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::SetEngineContact
// -----------------------------------------------------------------------------
//
void PSetCallDivertingWrapperPrivate::SetEngineContact( 
        MPsetCallDiverting * aDivertEngine )
    {
    SMC_MOCK_METHOD1( void, MPsetCallDiverting *, aDivertEngine )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::HandleEmergencyDialL
// -----------------------------------------------------------------------------
//
void PSetCallDivertingWrapperPrivate::HandleEmergencyDialL( 
        const TInt aStatus )
    {
    SMC_MOCK_METHOD1( void, const TInt, aStatus )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::convert
// -----------------------------------------------------------------------------
//
TCallDivertingCondition PSetCallDivertingWrapperPrivate::convert( 
        PsCallDivertingCondition type )
    {
    SMC_MOCK_METHOD1( TCallDivertingCondition, PsCallDivertingCondition, type )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::convert
// -----------------------------------------------------------------------------
//
TDivertingStatus PSetCallDivertingWrapperPrivate::convert( 
        PsCallDivertingStatus type )
    {
    SMC_MOCK_METHOD1( TDivertingStatus, PsCallDivertingStatus, type )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::convert
// -----------------------------------------------------------------------------
//
TServiceGroup PSetCallDivertingWrapperPrivate::convert( 
        PsServiceGroup type )
    {
    // TODO: QFlags does not compile with SymbianMock
    // SMC_MOCK_METHOD1( TServiceGroup, PsServiceGroup, type )
    Q_UNUSED(type)
    SMC_MOCK_METHOD0( TServiceGroup )
    }


// -----------------------------------------------------------------------------
// PSetCallDivertingWrapperPrivate::convert
// -----------------------------------------------------------------------------
//
TDivertingSetting PSetCallDivertingWrapperPrivate::convert( 
        PsCallDivertingSetting type )
    {
    SMC_MOCK_METHOD1( TDivertingSetting, PsCallDivertingSetting, type )
    }


