/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Manager for contacts.
*
*/



#ifndef CPHONECONTACTCONTROLLER_H
#define CPHONECONTACTCONTROLLER_H

// INCLUDES
#include    <e32base.h>

// FORWARD DECLARATIONS
class CPhoneCntManagerBase;
class CPhCntFactory;
class CPhCntSingleItemFetch;
class CPhoneCntVoiceKeyImp;
class CPhoneCntSaveAddToName;
class CPhCntRfsHandler;
class CPhCntSpeedDialMonitor; 

// CONSTANTS
const TInt KConstructAll = -1;

// CLASS DECLARATION

/**
*  It is manager for contacts.
*/
class CPhoneContactController 
    : public CBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneContactController* NewL();
        
        /**
        * Destructor.
        */
        ~CPhoneContactController();

    public: // New functions

        /**
        * Perform steps in construction.
        *
        * @param aSteps steps to be performed, one by default
        * @return steps to be done.
        */
        TInt ContinueConstructL( TInt aSteps = 1 );

        /**
        * Returns amount of steps to be done.
        *
        * @return steps to be done, 0 if everything ok.
        */
        TInt ConstructionSteps() const;

        /**
        * Creates single item fetcher.
        * 
        * @return single item fetcher, ownership transferred.
        */
        CPhCntSingleItemFetch* CreateSingleItemFetchL();
      
        /**
        * Creates object from which pbk operations add
        * and save are possible
        *
        * @return CPhoneCntSaveAddToName, ownership transferred.
        */
        CPhoneCntSaveAddToName* CreateSaveAddToNameL();
        
        /**
        * Creates Restore Factory Settings handler.
        *
        * @return instance of restore factory settings handler
        */
        CPhCntRfsHandler* CreateRfsHandlerL();
        
        /**
        * Creates Speed dial monitor handler.
        *
        * @return instance of speed dial monitor handler
        */
        CPhCntSpeedDialMonitor* CreateSpeedDialMonitorL();
        
        /**
        * Creates phonebook services, stores etc.
        */
        void CreatePhoneBookServiceL();

    private:

        // It defines enumeration for steps in construction.
        enum TStep
            {
            EStepLoadLibrary,
            EStepCreateFactory,
            EStepDone
            };

        /**
        * C++ default constructor.
        */
        CPhoneContactController();
        
        /**
        * Perform a step.
        */
        void DoStepL();

        /**
        * Determine next step.
        *
        * @param aStep current step.
        * @return next step.
        */
        static TStep NextStep( TStep aStep );

        /**
        * Load library.
        */
        void ConstructLoadLibraryL();

        /**
        * Create factory.
        */
        void ConstructCreateFactoryL();

        // By default, prohibit copy constructor
        CPhoneContactController( const CPhoneContactController& );
        // Prohibit assigment operator
        CPhoneContactController& operator= ( const CPhoneContactController& );

    private:    // Data
        
        // Owned: library.
        RLibrary iLibrary;

        // Owned: contact factory.
        CPhCntFactory* iFactory;

        // Current situation.
        TStep iCurrentStep;
    };

#endif // CPhoneContactController_H

// End of File
