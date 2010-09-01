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
* Description:  Implementation of restore factory settings.
*
*/


#ifndef CPHCNTRFSHANDLERIMPL_H
#define CPHCNTRFSHANDLERIMPL_H

// INCLUDES
#include    "CPhCntRfsHandler.h"

// FORWARD DECLARATIONS
class CRfsHandler;

// CLASS DECLARATION

/**
*  It is implementation of restore factory settings interface.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCntRfsHandlerImpl )
    : public CPhCntRfsHandler
    {
    public:  // Constructors and destructor      
                
        /**
        * Two-phased constructor.
        * @return New instance
        */
        static CPhCntRfsHandlerImpl* NewL();
        
        /**
        * Destructor.
        */
        ~CPhCntRfsHandlerImpl();

    public: // From base classes

        /**
        * From CPhCntRfsHandlerImpl
        */
        void ProcessCommandL( TRfsCommand aCommand );

    private:    // Data
        // Restory factory settings component
        CRfsHandler* iRfsHandler;
    };

#endif      // CPHCNTRFSHANDLERIMPL_H
            
// End of File
