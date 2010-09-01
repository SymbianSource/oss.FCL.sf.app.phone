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
* Description:  It provides Profile information.
*
*/


#ifndef CPHCNTPROFILEENGINE_H
#define CPHCNTPROFILEENGINE_H

// INCLUDES
#include    <e32base.h>
#include    <MProfileEngine.h>

// CLASS DECLARATION

/**
* Profile interface.
*
* @lib PhoneCntFinder
* @since 2.0
*/
class CPhCntProfileEngine
    : public CBase
    {
    public: // New functions

        /**
        * Refresh contents of profile.
        */
        virtual void RefreshL() = 0;

        /**
        * Checks if profile information has been
        * retrieved.
        *
        * @return ETrue if it is allowed to call 
        * the Profile method.
        */
        virtual TBool HasProfile() const = 0;

        /**
        * Returns active profile. It is valid
        * after RefreshL has succeeded.
        *
        * @return active profile information.
        */
        virtual MProfile& Profile() = 0;

        /**
        * Sets active profile. It is valid
        * after RefreshL has succeeded.
        *
        * @param aProfileId Id of the profile to be set.
        */
        virtual void SetActiveProfileL( 
            const TInt aProfileId ) = 0;

        /**
         * Checks whether the given feature is supported by Profiles Engine.         
         * @param aFeatureId the feature to be checked.
         * @return ETrue if the given feature is supported, EFalse otherwise.
         * @since Series60_2.8
         */
        virtual TBool IsFeatureSupported(
            TProfileFeatureId aFeatureId ) const = 0;
    };

#endif      // CPHCNTPROFILEENGINE_H
            
// End of File
