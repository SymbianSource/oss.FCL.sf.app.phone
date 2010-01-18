/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef CPHONESTORAGE_H
#define CPHONESTORAGE_H

#include "mphonestorage.h"
#include <e32base.h>


// CLASS DECLARATION

/**
 *  Data storage.
 *
 *  @lib PhoneUIUtils.lib
 *  @since S60 v5.0
 */
class CPhoneStorage : 
    public CBase, public MPhoneStorage
    {
public:

    IMPORT_C static CPhoneStorage* NewL();

    IMPORT_C virtual ~CPhoneStorage();
    
    /**
     * AppendBlockedKeysListL.
     *
     * @since S60 v5.0
     */
    IMPORT_C void AppendBlockedKeysListL( TStdScanCode aScanCode );
    
    /**
     * GetBlockedKeyList.
     *
     * @since S60 v5.0
     */
    IMPORT_C const RArray<TInt>& GetBlockedKeyList() const;

    /**
     * ResetBlockedKeysList.
     *
     * @since S60 v5.0
     */
    IMPORT_C void ResetBlockedKeysList();

    /**
     * IsBlockedKeysListEmpty.
     */
    IMPORT_C TBool IsBlockedKeysListEmpty();

    /**
     * SetScreenLocked.
     */
    IMPORT_C void SetScreenLocked( TBool aLocked );
    
    /**
     * IsScreenLocked.
     */
    IMPORT_C TBool IsScreenLocked();
    
    /**
     * SetNeedToEnableKeylock.
     */
    IMPORT_C void SetNeedToEnableKeylock( TBool aNeedToEnableKeylock );

    /**
     * NeedToEnableKeylock.
     */
    IMPORT_C TBool NeedToEnableKeylock();
    
private:

    CPhoneStorage();
    
    void ConstructL();
    
private: // data

     // Blocked key list
     RArray<TInt> iBlockedKeysScanCode;
     
     // Is touch screen locked there is an incoming call
     TBool iIsScreenLocked;
     
     // ETrue, if keylock must enable when call ends
     TBool iNeedToEnableKeylock;
    };
#endif // CPHONESTORAGE_H
