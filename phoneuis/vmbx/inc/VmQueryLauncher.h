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
* Description:  Active object for asking vmbx number (if it's not defined) after
*                the vmbx application has been fully constructed.
*
*/


#ifndef VM_QUERY_LAUNCHER_H
#define VM_QUERY_LAUNCHER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CVmContainer;

/**
*  MVmQueryLauncher
*
*  @lib vmbx.lib
*
**/
NONSHARABLE_CLASS ( MVmQueryLauncher )
    {
    public:
        /**
        * query number completed
        **/
        virtual void NumberQueryComplete() = 0;

    };


// CLASS DECLARATION

/**
*  CVmQueryLauncher
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
class CVmQueryLauncher : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aContainer VmContainer
        * @param aObserver observer for launching number
        * @param aPriority  Priority of CVmQueryLauncher
        */
        static CVmQueryLauncher* NewL( CVmContainer& aContainer, 
                                        MVmQueryLauncher& aObserver, 
                                TInt aPriority = CActive::EPriorityStandard );
      
        /**
         * Destructor.
         */
        ~CVmQueryLauncher();
 
     protected:  // From CActive

        void RunL();
        void DoCancel();
     
     private:
        /**
        * C++ default constructors.
        *
        * @param aContainer VmContainer
        * @param aObserver observer for launching number
        * @param aPriority  Priority of CVmQueryLauncher
        */
        CVmQueryLauncher( CVmContainer& aContainer,
                         MVmQueryLauncher& aObserver, TInt aPriority );
     
     private:  // Data
        CVmContainer& iContainer;

        // Observer for AppUi to query number
        MVmQueryLauncher& iObserver;
    };

#endif  // VM_QUERY_LAUNCHER_H

// End of File  
