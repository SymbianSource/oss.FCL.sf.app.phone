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
* Description:  It provides interface to retrieve phonebook instance in 
*                the PhoneCntFinder subsystem.
*
*/


#ifndef CPHONECNTPBKOWNER_H
#define CPHONECNTPBKOWNER_H

//  INCLUDES
#include "MPhoneCntPbkOwner.h"

class CPhCntContactManager;

// CLASS DECLARATION

/**
*  It's the phonebook owner.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhoneCntPbkOwner )
    : public MPhoneCntPbkOwner
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @return New instance
        */
        static CPhoneCntPbkOwner* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CPhoneCntPbkOwner();

    public: // Functions from base classes

        /**
        * From MPhoneCntPbkOwner
        */
        CPhCntContactManager* ContactManager() const;

    private:

        /**
        * C++ constructor.
        */
        CPhoneCntPbkOwner();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

    private:    // Data
    
        // Contact manager.
        CPhCntContactManager* iContactManager;
    };

#endif      // CPHONECNTPBKOWNER_H  
            
// End of File
