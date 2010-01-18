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
* Description:  Implementation of connection termination plug-in.
*
*/



#ifndef CCOUTLIMPLEMENTATION_H
#define CCOUTLIMPLEMENTATION_H

//  INCLUDES
#include    <CCoUtlInterface.h>
#include    <ConeResLoader.h>

// FORWARD DECLARATION
class CCoUtlActive;

// CLASS DECLARATION

/**
*  CCoUtlInterface implementation.
*
*  @since 2.6
*/
class CCoUtlImplementation 
    : public CCoUtlInterface
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @return an instance of CCoUtlImplementation.
        */
        static CCoUtlImplementation* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CCoUtlImplementation();

    public: // Functions from base classes

        /**
        * @see CCoUtlInterface::CurrentState.
        */
        virtual TState CurrentState();

        /**
        * @see CCoUtlInterface::Terminate.
        */
        virtual void Terminate( TRequestStatus& aStatus );
    
        /**
        * @see CCoUtlInterface::Cancel.
        */
        virtual void Cancel();

    private:

        /**
        * C++ default constructor.
        */
        CCoUtlImplementation();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Owned active object to do the task.
        CCoUtlActive* iActive;

        // Owned resource loader.
        RConeResourceLoader iResourceLoader;

    };

#endif      // CCOUTLIMPLEMENTATION_H
            
// End of File
