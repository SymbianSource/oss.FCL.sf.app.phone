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
* Description:  Concrete implementation of CPhCntProfileEngine interface.
*
*/


#ifndef CPHCNTPROFILEENGINEIMPL_H
#define CPHCNTPROFILEENGINEIMPL_H

// INCLUDES
#include    "CPhCntProfileEngine.h"

// FORWARD DECLARATIONS
class MProfile;
class MProfileEngine;

// CLASS DECLARATION

/**
* Profile interface.
*
* @lib PhoneCntFinder
* @since 2.0
*/
NONSHARABLE_CLASS( CPhCntProfileEngineImpl )
    : public CPhCntProfileEngine
    {
    public: // Constructors and destructors

        /**
        * Two-phased constructor.
        * @return New instance
        */
        static CPhCntProfileEngineImpl* NewL();

        /**
        * Destructor.
        */
        ~CPhCntProfileEngineImpl();

    public: // From base classes

        /**
        * From CPhCntProfileEngine, refresh contents 
        * of profile.
        */
        virtual void RefreshL();

        /**
        * From CPhCntProfileEngine, checks if profile 
        * information has been retrieved.
        *
        * @return ETrue if it is allowed to call 
        * the Profile method.
        */
        virtual TBool HasProfile() const;

        /**
        * From CPhCntProfileEngine, returns active 
        * profile. It is valid after RefreshL has 
        * succeeded.
        *
        * @return active profile information.
        */
        virtual MProfile& Profile();

        /**
        * Sets active profile. It is valid
        * after RefreshL has succeeded.
        *
        * @param aProfileId Id of the profile to be set.
        */
        virtual void SetActiveProfileL( 
            const TInt aProfileId );

        /**
         * Checks whether the given feature is supported by Profiles Engine.         
         * @param aFeatureId the feature to be checked.
         * @return ETrue if the given feature is supported, EFalse otherwise.
         * @since Series60_2.8
         */
        virtual TBool IsFeatureSupported(
            TProfileFeatureId aFeatureId ) const;

    private:
        // Profile engine
        MProfileEngine* iEngine;
        // Profile
        MProfile* iProfile;

    };

#endif      // CPHCNTPROFILEENGINEIMPL_H
            
// End of File
