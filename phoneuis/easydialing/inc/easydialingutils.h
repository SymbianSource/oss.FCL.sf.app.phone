/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Helper class for easydialing. 
*
*/

#ifndef __EASYDIALINGUTILS_H__
#define __EASYDIALINGUTILS_H__

#include <CPbkContactEngine.h>  // for TPbkNameOrder

class EasyDialingUtils
    {
    public:

        /*
         * Creates contact string from first name, last name and company name.
         * F.ex. "Firstname Lastname\tCompany"
         * @param aFirstName First name
         * @param aFirstName Last name
         * @param aFirstName Comapany name
         * @param aNameOrder The name ordering to be used
         * @return Contact string (in cleanupstack)
         */
        static HBufC* CreateContactStringLC(
            const TDesC& aFirstName,
            const TDesC& aLastName,
            const TDesC& aCompanyName,
            CPbkContactEngine::TPbkNameOrder aNameOrder );
    };

#endif //__EASYDIALINGUTILS_H__


