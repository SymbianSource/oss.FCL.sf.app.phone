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
* Description:  An abstract class for get blocked key list.
*
*/


#ifndef MPHONEBLOCKEDKEYS_H
#define MPHONEBLOCKEDKEYS_H

// INCLUDES
#include <e32cmn.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  An abstract class get blocked key list
*
*/
class MPhoneBlockedKeys
    {
    public:

        /**
        * Get blocked key list
        * @returns RArray<TInt>& lsit of blocked keys 
        */
        virtual const RArray<TInt>& GetBlockedKeyList() const = 0;
        
    };

#endif // MPHONEBLOCKEDKEYS_H
            
// End of File
