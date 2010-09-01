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
* Description:  It defines interface for speed dial for phone app.
*
*/


#ifndef CPHCNTSPEEDDIALMONITOR_H
#define CPHCNTSPEEDDIALMONITOR_H

//  INCLUDES
#include    <e32base.h>
#include <mphcntmatch.h>

// CONSTANTS
const TInt KPhCntContactItemNotFound = -1;
const TInt KPhCntInvalidIndex = -1;

// CLASS DECLARATION
class CSpdiaControl;
class CEikImage;
class MVPbkContactLink;
class CPhCntContactId;

/**
*  It defines interface for speed dial for phone app.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCntSpeedDialMonitor )
    : public CBase
    {
    public: // New functions

        /**
        * Structure holding speed dial information.
        */
        struct TSpdDialFieldInfo
            {
            MPhCntMatch::TNumberType iNumberType;
            };
        
        /**
        * Gets the speed dial number.
        *
        * @param aSpeedDialPosition a speed dial position
        * @return aPhoneNumber phone number
        * @return error code, KErrNone if success
        */
        virtual TInt GetSpeedDialFieldL( 
            TInt aSpeedDialPosition, 
            TDes& aPhoneNumber ) = 0;

        /**
        * Gets the speed dial number.
        *
        * @param aSpeedDialPosition a speed dial position
        * @return aPhoneNumber phone number
        * @return aFieldInfo speed dial contact information
        * @return error code, KErrNone if success
        */
        virtual TInt GetSpeedDialFieldL(
            TInt aSpeedDialPosition, 
            TDes& aPhoneNumber,
            TSpdDialFieldInfo& aFieldInfo ) = 0;

        /**
        * Assigns phone number to speed dial location
        *
        * @param aSpeedDialPosition the speed dial position
        * @return aPhoneNumber If the number is assigned successfully, phone
        *         return the phone number
        * @return KErrNone if successfully assigned, KErrNotFound if user
        *         cancel the assignation
        */
        virtual TInt AssignSpeedDialFieldL(
            TInt aSpeedDialPosition,
            TDes& aPhoneNumber ) = 0;

        /**
        * Assigns phone number to speed dial location
        *
        * @param aSpeedDialPosition the speed dial position
        * @return aPhoneNumber If the number is assigned successfully, phone
        *         return the phone number
        * @return aFieldInfo speed dial contact information
        * @return KErrNone if successfully assigned, KErrNotFound if user
        *         cancel the assignation
        */
        virtual TInt AssignSpeedDialFieldL(
            TInt aSpeedDialPosition,
            TDes& aPhoneNumber,
            TSpdDialFieldInfo& aFieldInfo ) = 0;

        /**
        * Cancel speed dialing assignment process
        */
        virtual void Cancel() = 0;

        /**
        * Create number type icon from phone book view
        * @param aNumberType the number type
        * @return returns a object of CEikImage
        */
        virtual CEikImage* CreateNumberTypeIconLC( TInt aNumberType ) = 0;

    };

#endif      // CPHCNTSPEEDDIALMONITOR_H
            
// End of File
