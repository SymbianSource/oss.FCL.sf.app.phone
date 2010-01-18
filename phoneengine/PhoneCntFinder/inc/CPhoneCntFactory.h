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
* Description:  It's factory for creating components.
*
*/


#ifndef CPHCNTFACTORY_H
#define CPHCNTFACTORY_H

//  INCLUDES
#include    <e32base.h>
#include    <cntdef.h>

// FORWARD DECLARATIONS
class MPhCntThumbnailLoaderObserver;
class CEikStatusPane;
class CPhCntSpeedDialMonitor;
class CPhCntThumbnailLoader;
class CPhCntSingleItemFetch;
class CPhCntRfsHandler;
class CPhCntMmiSecurity;
class CPhoneCntSaveAddToName; 
class CPhoneCntVoiceKeyImp;
class CPhCntMatcher;
class CPhCntProfileEngine;
class MProfileChangeObserver;
class CPhCntContactId;
class CPhCntContactDataSelection;
class MPhCntStoreLoader;

// CLASS DECLARATION

/**
*  Factory class for creating phonebook related classes.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
class CPhCntFactory : public CBase
    {
    public:

        /**
        * Creates speed dial monitor.
        *
        * @param aStatusPane statuspane.
        * @return speed dial monitor, ownership transferred.
        */
        virtual CPhCntSpeedDialMonitor* CreateSpeedDialMonitorL() = 0;

        /**
        * Creates thumbnail loader.
        *
        * @param aObserver observer.
        * @return thumbnail loader, ownership transferred.
        */
        virtual CPhCntThumbnailLoader* CreateThumbnailLoaderL(
            MPhCntThumbnailLoaderObserver& aObserver ) = 0;
            
        /**
        * Creates single item fetcher.     
        * 
        * @return single item fetcher, ownership transferred.
        */
        virtual CPhCntSingleItemFetch* CreateSingleItemFetchL() = 0;

        /**
        * Creates rfs handler.
        *
        * @return rfs handler, ownership is transferred.
        */
        virtual CPhCntRfsHandler* CreateRfsHandlerL() = 0;

        /**
        * Creates mmi security.
        *
        * @return mmi security.
        */
        virtual CPhCntMmiSecurity* CreateMmiSecurityL() = 0;
       
        /**
        * Creates save and add to name-object.
        *
        * @return pointer to class, ownership is transferred.
        */
        virtual CPhoneCntSaveAddToName* CreateSaveAddToNameL() = 0;

        /** 
        * Creates an instance of Contact Matcher
        *
        * @return Contact Matcher object. Ownership is transferred.
        */
        virtual CPhCntMatcher* CreateContactMatcherL() = 0;

        /**
        * Creates an instance of profile engine.
        *
        * @return profile engine instance. Ownership is transferred.
        */
        virtual CPhCntProfileEngine* CreateProfileEngineL() = 0;

        /**
        * Creates an instance of profile engine notify handler.
        * @param aObserver observer to be notified of profile changes.
        * @return new instance.
        */
        virtual CBase* CreateProfileEngineNotifyHandlerL( 
            MProfileChangeObserver* aObserver ) = 0;
            
        /**
         * Creates contact id instance.
         *
         * @since S60 v3.1
         * @return New instance.
         */
        virtual CPhCntContactId* CreateContactIdL() = 0;
            
        /**
         * Creates an contact id instance.
         *
         * @since S60 v3.1
         * @return New instance of CPhCntContactId
         */
        virtual CPhCntContactId* CreateContactIdL( TContactItemId aContactId ) = 0;

        /**
         * Create and contact id instance from contact link.
         *
         * @since S60 v3.2
         */
        virtual CPhCntContactId* CreateContactIdL( const TDesC8& aContactId ) = 0;    

        /**
         * Creates service to get user to select data from contact.
         * 
         * @since S60 v3.2
         */
        virtual CPhCntContactDataSelection* CreateContactDataSelectionL() = 0;

        /**
         * Creates a service to load contact stores.
         * 
         * @since S60 v9.1
         */
        virtual MPhCntStoreLoader* CreateContactStoreLoaderL() = 0;
        
        /**
         * Creates a phonebook service and opens contact stores.
         * 
         * @since S60 v9.1
         */
        virtual void CreatePhonebookServicesL() = 0;
    };

/**
* Creates CPhCntFactory. Position 1 in DLL's lookup table.
* @return Reference to CPhCntFactory object. Ownership is transferred.
*/
IMPORT_C CPhCntFactory* CreateCntFactoryL();

#endif      // CPHCNTFACTORY_H
            
// End of File
