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
* Description: 
*     Helper class for converting vanity dialing numbers
*
*/


#ifndef PHONEVANITYDIALINGUTILS_H
#define PHONEVANITYDIALINGUTILS_H


//  INCLUDES
#include <e32std.h>


// CLASS DECLARATION

/**
* Defines helper function for vanity dialing number conversion
*/
class PhoneVanityDialingUtils
    {
    public:
        /**
        * Converts vanity dialing number to normal phone number
        * @param aDes Phone number to be converted
        */   
        IMPORT_C static void DoVanityNumberConversion( TDes& aDes );
    };


#endif //PHONEVANITYDIALINGUTILS_H
