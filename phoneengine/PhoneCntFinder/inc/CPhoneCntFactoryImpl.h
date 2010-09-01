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
* Description:  Factory class's implementation.
*
*/


#ifndef CPHCNTFACTORYIMPL_H
#define CPHCNTFACTORYIMPL_H

//  INCLUDES
#include    <e32base.h>
#include    <cphonecntfactory.h>
#include    <RPbkViewResourceFile.h>

class CPhCntPhonebookServices;
class CPhCntContactStores;
class CPhCntContactDataSelection;


// FORWARD DECLARATIONS
class CPhoneCntPbkOwner;
class MPhCntStoreLoader;

// CLASS DECLARATION

/**
*  Factory class's implementation. Not for external use.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCntFactoryImpl ) : public CPhCntFactory
    {
    public: // Constructors and destructors.

        /**
        * Two-phased constructor.
        *
        * @return new instance of the class.
        */
        static CPhCntFactoryImpl* NewL();

        /**
        * Destructor.
        */
        ~CPhCntFactoryImpl();

    public: // From base classes.

        /**
        * From CPhCntFactory
        */
        CPhCntSpeedDialMonitor* CreateSpeedDialMonitorL();

        /**
        * From CPhCntFactory
        */
        CPhCntThumbnailLoader* CreateThumbnailLoaderL(
            MPhCntThumbnailLoaderObserver& aObserver );

        /**
        * From CPhCntFactory
        */
        CPhCntSingleItemFetch* CreateSingleItemFetchL();

        /**
        * From CPhCntFactory
        */
        CPhCntRfsHandler* CreateRfsHandlerL();

        /**
        * From CPhCntFactory
        */
        CPhCntMmiSecurity* CreateMmiSecurityL();
      
        /**
        * From CPhCntFactory
        */
        CPhoneCntSaveAddToName* CreateSaveAddToNameL();

        /**
        * From CPhCntFactory
        */
        CPhCntMatcher* CreateContactMatcherL();

        /**
        * From CPhCntFactory
        */
        CPhCntProfileEngine* CreateProfileEngineL();

        /**
        * From CPhCntFactory
        */
        CBase* CreateProfileEngineNotifyHandlerL( 
            MProfileChangeObserver* aObserver );
            
        /**
         * Creates contact id instance.
         *
         * @since S60 v3.1
         * @return New instance.
         */
        CPhCntContactId* CreateContactIdL();
        
        /**
         * From CPhCntFactory
         * Creates an contact id instance.
         *
         * @since S60 v3.1
         * @return New instance of CPhCntContactId
         */
        CPhCntContactId* CreateContactIdL( TContactItemId aContactId );
        
        /**
         * From CPhCntFactory
         *
         * @since S60 v3.2
         * @see CPhCntFactory
         */
        CPhCntContactId* CreateContactIdL( const TDesC8& aContactLink );

        /**
         * From CPhCntFactory.
         * @since S60 v3.2
         * @see CPhCntFactory
         */
        CPhCntContactDataSelection* CreateContactDataSelectionL();

        /**
         * From CPhCntFactory.
         * @since S60 v9.1
         * @see CPhCntFactory
         */
        MPhCntStoreLoader* CreateContactStoreLoaderL();
        
        /**
         * From CPhCntFactory.
         * @since S60 v9.1
         * @see CPhCntFactory
         */
        void CreatePhonebookServicesL();
        
    private:

        /**
        * C++ constructor.
        */
        CPhCntFactoryImpl();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();
        
        void CreateContactStoreAndManagerL();

    private:
        // Phonebook view resources
        RPbkViewResourceFile iPbkResource;

        // Has the SecUi been initialized
        TBool iSecUiInit;

        // Phonebook owner
        CPhoneCntPbkOwner* iPbkOwner;

        /**
         * Phonebook services.
         * own.
         */
        CPhCntPhonebookServices* iPhonebookServices;
        
        /** 
         * Contact stores.
         * Own.
         */
        CPhCntContactStores* iContactStores;
    };


#endif      // CPHCNTFACTORYIMPL_H   

// End of File
