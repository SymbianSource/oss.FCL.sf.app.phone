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
* Description:  Provides interface to mmi initiated security operations.
*
*/


#ifndef CPHCNTMMISECURITY_H
#define CPHCNTMMISECURITY_H

// INCLUDES
#include    <e32base.h>

// CLASS DECLARATION

/**
*  It provides interface to perform mmi initiated security operations.
*
*  The class shouldn't be destroyed during operation.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
class CPhCntMmiSecurity
    : public CBase
    {
    public: // New functions       

        /**
        * Enumerates different pin related commands.
        *
        * ECmdChangePin - change pin.
        * ECmdChangePin2 - change pin2.
        * ECmdUnblockPin - unblock pin.
        * ECmdUnblockPin2 - unblock pin2.
        */
        enum TPinCommand
            {
            ECmdChangePin,
            ECmdChangePin2,
            ECmdUnblockPin,
            ECmdUnblockPin2
            };

        /**
        * Change pin code.
        *
        * @param aCommand command. 
        * @param aCode old code. 
        *       if command is change, then old pin.
        *       if command is unblock, then puk.
        * @param aNewPin new pin. 
        * @param aVerifyNew verification of pin.
        */
        virtual TBool ChangePinL(
            TPinCommand aCommand,
            const TDesC& aCode,
            const TDesC& aNewPin,
            const TDesC& aVerifyNew ) = 0;

        /**
        * Enumerates different sim commands.
        *
        * ECmdOpenLock - opens lock.
        * ECmdCloseLock - closes lock.
        */
        enum TSimCommand
            {
            ECmdOpenLock,
            ECmdCloseLock
            };

        /**
        * Change sim lock status.
        *
        * @param aCommand command. 
        * @param aCode password.
        * @param aType type of lock.
        */
        virtual TBool ChangeSimLockL(
            TSimCommand aCommand,
            const TDesC& aCode,
            const TDesC& aType ) = 0;

        /**
        * Cancels whatever it is doing.
        *
        * If not doing anything, then does nothing.
        */
        virtual void Cancel() = 0;
        
    };

#endif      // CPHCNTMMISECURITY_H
            
// End of File
