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
* Description:  Contact utilitiest class
*
*/


#ifndef PHCNTCONTACTUTIL_H
#define PHCNTCONTACTUTIL_H

#include "CPhCntContact.h"

/**
 *  Contact utilities.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v5.1
 */
NONSHARABLE_CLASS( TPhCntContactUtil ) 
    {
public:
    
    /**
     * Checks are name fields same in both contacts.
     * Checked fields are First name, Last name and Company name.
     *
     * @since S60 v5.1
     * @param aFirstContact First Contact to be checked.
     * @param aSecondContact Second Contact to be checked.
     * @return ETrue if name fields are same in both contacts.
     */
    static TBool AreFirstAndLastAndCompanyNameSame( 
            const CPhCntContact& aFirstContact,
            const CPhCntContact& aSecondContact );
    
    };

#endif // PHCNTCONTACTUTIL_H

