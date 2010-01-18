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
* Description:  Storage of data
*
*/


#include <eikappui.h>
#include <eikenv.h>
#include "cphonestorage.h"
#include "phonelogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhoneStorage::CPhoneStorage()
    : iIsScreenLocked( EFalse ),
    iNeedToEnableKeylock( EFalse )
    {
    __LOGMETHODSTARTEND(EPhoneUIUtils, "CPhoneStorage::CPhoneStorage()");
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhoneStorage::ConstructL()
    {
    __LOGMETHODSTARTEND(EPhoneUIUtils, "CPhoneStorage::ConstructL()");
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPhoneStorage* CPhoneStorage::NewL()
    {
    __LOGMETHODSTARTEND(EPhoneUIUtils, "CPhoneStorage::NewL()");
    CPhoneStorage* self = 
        new (ELeave) CPhoneStorage();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPhoneStorage::~CPhoneStorage()
    {
    __LOGMETHODSTARTEND(EPhoneUIUtils, "CPhoneStorage::~CPhoneStorage()");
    iBlockedKeysScanCode.Reset();
    iBlockedKeysScanCode.Close();
    }

// ---------------------------------------------------------------------------
// CPhoneStorage::AppendBlockedKeysListL
// ---------------------------------------------------------------------------
//
EXPORT_C void CPhoneStorage::AppendBlockedKeysListL( TStdScanCode aScanCode )
    {
    __LOGMETHODSTARTEND(EPhoneUIUtils, "CPhoneStorage::AppendBlockedKeysListL()");
    iBlockedKeysScanCode.AppendL( aScanCode );
    }

// -----------------------------------------------------------
// CPhoneStorage::GetBlockedKeyList
// -----------------------------------------------------------
//
EXPORT_C const RArray<TInt>& CPhoneStorage::GetBlockedKeyList() const
    {
    __LOGMETHODSTARTEND(EPhoneUIUtils, "CPhoneStorage::GetBlockedKeyList()");
    return iBlockedKeysScanCode;
    }

// -----------------------------------------------------------
// CPhoneStorage::ResetBlockedKeysList
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStorage::ResetBlockedKeysList()
    {
    __LOGMETHODSTARTEND(EPhoneUIUtils, "CPhoneStorage::ResetBlockedKeysList()");
    iBlockedKeysScanCode.Reset();
    }

// -----------------------------------------------------------
// CPhoneStorage::IsBlockedKeysListSet
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneStorage::IsBlockedKeysListEmpty()
    {
    __LOGMETHODSTARTEND(EPhoneUIUtils, "CPhoneStorage::IsBlockedKeysListSet()");
    TBool ret = ETrue;
    if( iBlockedKeysScanCode.Count() )
        {
        ret = EFalse;
        }
    return ret;
    }

// -----------------------------------------------------------
// CPhoneStorage::SetScreenLocked
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStorage::SetScreenLocked( TBool aLocked )
    {
    iIsScreenLocked = aLocked;
    }

// -----------------------------------------------------------
// CPhoneStorage::IsScreenLocked
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneStorage::IsScreenLocked()
    {
    return iIsScreenLocked;
    }

// -----------------------------------------------------------
// CPhoneStorage::NeedToEnableKeylock
// -----------------------------------------------------------
//
EXPORT_C TBool CPhoneStorage::NeedToEnableKeylock()
    {
    return iNeedToEnableKeylock;
    }


// -----------------------------------------------------------
// CPhoneStorage::NeedToEnableKeylock
// -----------------------------------------------------------
//
EXPORT_C void CPhoneStorage::SetNeedToEnableKeylock( TBool aNeedToEnableKeylock )
    {
    iNeedToEnableKeylock = aNeedToEnableKeylock;
    }

// END
