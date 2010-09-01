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
* Description:  Implementation of mmi security.
*
*/


#ifndef CPHCNTMMISECURITYIMPL_H
#define CPHCNTMMISECURITYIMPL_H

// INCLUDES
#include    "CPhCntMmiSecurity.h"

// FORWARD DECLARATIONS
class CManualSecuritySettings;

// CLASS DECLARATION

/**
*  It is implementation of mmi security.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCntMmiSecurityImpl )
    : public CPhCntMmiSecurity
    {
    public:  // Constructors and destructor      
                
        /**
        * Two-phased constructor.
        * @return New instance
        */
        static CPhCntMmiSecurityImpl* NewL();
        
        /**
        * Destructor.
        */
        ~CPhCntMmiSecurityImpl();

    public: // From base classes

        /**
        * From CPhCntMmiSecurity
        */
        TBool ChangePinL(
            TPinCommand aCommand,
            const TDesC& aCode,
            const TDesC& aNewPin,
            const TDesC& aVerifyNew );

        /**
        * From CPhCntMmiSecurity
        */
        TBool ChangeSimLockL(
            TSimCommand aCommand,
            const TDesC& aCode,
            const TDesC& aType );

        /**
        * From CPhCntMmiSecurity
        */
        void Cancel();

    private:

        /**
        * Creates model.
        *
        * @return model.
        */
        CManualSecuritySettings& CreateModelL();

        /**
        * Destroyes model.
        */
        void DestroyModel();

        /**
        * Panics (in debug builds).
        */
        static void Panic();

    private:    // Data
        // Security model
        CManualSecuritySettings* iModel;

    };

#endif      // CPHCNTMMISECURITYIMPL_H
            
// End of File
