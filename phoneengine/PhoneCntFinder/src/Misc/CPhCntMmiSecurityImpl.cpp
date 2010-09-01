/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of security operations
*
*/


// INCLUDE FILES
#include    "CPhCntMmiSecurityImpl.h"
#include    <secuimanualsecuritysettings.h>

// CONSTANTS
#ifdef _DEBUG
_LIT( KPhCntMmiSecurity, "PhCntMmi" );
#endif // _DEBUG

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCntMmiSecurityImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhCntMmiSecurityImpl* CPhCntMmiSecurityImpl::NewL()
    {
    CPhCntMmiSecurityImpl* self = 
        new (ELeave) CPhCntMmiSecurityImpl;
    
    return self;
    }
   
// Destructor
CPhCntMmiSecurityImpl::~CPhCntMmiSecurityImpl()
    {
    DestroyModel();
    }

// ---------------------------------------------------------
// CPhCntMmiSecurityImpl::ChangePinL
// ---------------------------------------------------------
//
TBool CPhCntMmiSecurityImpl::ChangePinL(
        TPinCommand aCommand,
        const TDesC& aCode,
        const TDesC& aNewPin,
        const TDesC& aVerifyNew )
    {
    CManualSecuritySettings& model = CreateModelL();
    
    // Initially, setting is pin2.
    CManualSecuritySettings::TPin pin = 
        CManualSecuritySettings::EPin2;

    TBool result = EFalse;

    switch ( aCommand )
        {
        case ECmdChangePin:
            
            // Change setting.
            //
            pin = CManualSecuritySettings::EPin1;
            
            // Flow through..
            //
        case ECmdChangePin2:
            
            result = model.ChangePinL( 
                pin,
                aCode,
                aNewPin,
                aVerifyNew );
            break;

        case ECmdUnblockPin:
            
            // Change setting
            //
            pin = CManualSecuritySettings::EPin1;

            // Flow through..
            //
        case ECmdUnblockPin2:
            
            result = model.UnblockPinL( 
                pin,
                aCode,
                aNewPin,
                aVerifyNew );
            break;

        default:
            // Case not handled, panic.
            Panic();
            break;
        }

    return result;
    }

// ---------------------------------------------------------
// CPhCntMmiSecurityImpl::ChangeSimLockL
// ---------------------------------------------------------
//
TBool CPhCntMmiSecurityImpl::ChangeSimLockL(
        TSimCommand aCommand,
        const TDesC& aCode,
        const TDesC& aType )
    {   
    CManualSecuritySettings& model = CreateModelL();
    TBool result = EFalse;

    switch ( aCommand )
        {
        case ECmdOpenLock:
            result = 
                model.UnlockSimL( aCode, aType );
            break;
        case ECmdCloseLock:
            result =
                model.LockSimL( aCode, aType );
            break;
        default:
            // Case not handled, panic.
            Panic();
            break;
        }

    return result;
    }

// ---------------------------------------------------------
// CPhCntMmiSecurityImpl::Cancel
// ---------------------------------------------------------
//
void CPhCntMmiSecurityImpl::Cancel()
    {
    if ( iModel )
        {
        iModel->CancelChangePin();
        iModel->CancelUnblockPin();
        iModel->CancelLockSim();
        iModel->CancelUnlockSim();

        DestroyModel();
        }
    }

// ---------------------------------------------------------
// CPhCntMmiSecurityImpl::CreateModelL
// ---------------------------------------------------------
//
CManualSecuritySettings& 
    CPhCntMmiSecurityImpl::CreateModelL()
    {
    if ( !iModel )
        {
        iModel = CManualSecuritySettings::NewL();
        }

    return *iModel;
    }

// ---------------------------------------------------------
// CPhCntMmiSecurityImpl::DestroyModel
// ---------------------------------------------------------
//
void CPhCntMmiSecurityImpl::DestroyModel()
    {
    delete iModel; 
    iModel = NULL;
    }

// ---------------------------------------------------------
// CPhCntMmiSecurityImpl::Panic
// ---------------------------------------------------------
//
void CPhCntMmiSecurityImpl::Panic()
    {
    // We only panic with debug builds.
    __ASSERT_DEBUG( 
        EFalse, 
        User::Panic( KPhCntMmiSecurity, 0 ) );
    }

//  End of File  

