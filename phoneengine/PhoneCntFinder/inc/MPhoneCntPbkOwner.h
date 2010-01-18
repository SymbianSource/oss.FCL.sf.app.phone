/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for Phonebook Owner implementation
*
*/


#ifndef MPHONECNTPBKOWNER_H
#define MPHONECNTPBKOWNER_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CPhCntContactManager;

// CLASS DECLARATION

/**
*  Interface for Phonebook Owner implementation.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
class MPhoneCntPbkOwner
    {
    public:
        /**
        * Gives intance of contact manager.
        */
        virtual CPhCntContactManager* ContactManager() const = 0;
    };

#endif      // MPHONECNTPBKOWNER_H
            
// End of File
