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
#include "aknkeylock.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RAknKeylock2::Connect
// -----------------------------------------------------------------------------
//
TInt RAknKeylock2::Connect(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RAknKeyLock::EnableKeyLock
// -----------------------------------------------------------------------------
//
void RAknKeyLock::EnableKeyLock(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RAknKeyLock::DisableKeyLock
// -----------------------------------------------------------------------------
//
void RAknKeyLock::DisableKeyLock(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RAknKeyLock::IsKeyLockEnabled
// -----------------------------------------------------------------------------
//
TBool RAknKeyLock::IsKeyLockEnabled(  )
    {
    SMC_MOCK_METHOD0( TBool )
    }


// -----------------------------------------------------------------------------
// RAknKeyLock::EnableSoftNotifications
// -----------------------------------------------------------------------------
//
void RAknKeyLock::EnableSoftNotifications( 
        TBool aEnable )
    {
    SMC_MOCK_METHOD1( void, TBool, aEnable )
    }


// -----------------------------------------------------------------------------
// RAknKeyLock::OfferKeyLock
// -----------------------------------------------------------------------------
//
void RAknKeyLock::OfferKeyLock(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RAknKeyLock::CancelAllNotifications
// -----------------------------------------------------------------------------
//
void RAknKeyLock::CancelAllNotifications(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RAknKeyLock::EnableAutoLockEmulation
// -----------------------------------------------------------------------------
//
void RAknKeyLock::EnableAutoLockEmulation(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RAknKeyLock::DisableWithoutNote
// -----------------------------------------------------------------------------
//
void RAknKeyLock::DisableWithoutNote(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RAknKeyLock::EnableWithoutNote
// -----------------------------------------------------------------------------
//
void RAknKeyLock::EnableWithoutNote(  )
    {
    SMC_MOCK_METHOD0( void )
    }


