/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides abstract ECOM interface for connection utility.
*
*/



#ifndef CCOUTLINTERFACE_H
#define CCOUTLINTERFACE_H

//  INCLUDES
#include    <e32base.h>

// CLASS DECLARATION

/**
*  Abstract ECOM interface for connection termination.
*
*  @since 2.6
*/
class CCoUtlInterface 
    : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        inline static CCoUtlInterface* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CCoUtlInterface();

    public: // New functions
        
        /**
        * Enumerates states.
        *
        * EIdle - idle, 
        * EConfirm - asking for user decision,
        * ETerminate - terminating
        */
        enum TState
            {
            EIdle,
            EConfirm,
            ETerminate
            };

        /**
        * Returns current state.
        * @return current state.
        */
        virtual TState CurrentState() = 0;

        /**
        * Terminates connections.
        * @param aStatus request status to be completed once
        *                finished.
        */
        virtual void Terminate( TRequestStatus& aStatus ) = 0;
    
        /**
        * Interrupts termination. 
        */
        virtual void Cancel() = 0;

    private:    // Data
        
        // Uid required by ECOM.
        TUid iDtor_ID_Key;

    };

#include    <ccoutlinterface.inl>

#endif      // CCOUTLINTERFACE_H
            
// End of File
