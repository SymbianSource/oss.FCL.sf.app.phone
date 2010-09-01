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
* Description:  Publish & Subscribe property change observer
*
*/



#ifndef VMDETDLGPROPERTYOBSERVER_H
#define VMDETDLGPROPERTYOBSERVER_H

//  INCLUDES
#include <e32base.h>        // CActive
#include <e32property.h>    // RProperty

// FORWARD DECLARATIONS
class CVmDetailsDialog;

// CLASS DECLARATION

/**
*  Implementation of the Publish & Subscribe property change observer
*
*  @lib
*  @since 2.6
*/
class CVmDetDlgPropertyObserver : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aObserver Observer to be informed when observed property
        *                  changes
        */
        static CVmDetDlgPropertyObserver* NewL( CVmDetailsDialog& aDetDialog );

        /**
        * Destructor.
        */
        virtual ~CVmDetDlgPropertyObserver();

    public:     // New functions

        /**
        * Starts observing the given key for any changes
        * Notifies the observer when the property changes
        * @since 2.6
        * @param aCategory, Category of the observed property
        * @param aKey, Observed property
        */
        void ObservePropertyChangeL( TUid aCategory, TUint aKey );

    protected:  // Functions from base classes

        /**
        * From CActive
        */
        void RunL();

        /**
        * From CActive
        */
        void DoCancel();

        /**
        * From CActive
        */
        TInt RunError( TInt aError );

    private:

        /**
        * C++ default constructor.
        */
        CVmDetDlgPropertyObserver( CVmDetailsDialog& aDetDialog );

        /**
        * Symbian 2nd phase constructor..
        */
        void ConstructL();

    private:    // Data

        // Owns: Observed property
        RProperty iProperty;

        // Observer to be informed when property changes
        CVmDetailsDialog& iDetDialog;
        
        // Observed property
        TUint iKey;
    };

#endif      // VMDETDLGPROPERTYOBSERVER_H

// End of File
