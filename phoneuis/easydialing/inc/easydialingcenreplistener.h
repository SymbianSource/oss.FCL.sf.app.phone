/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Grid for easy dialing action menu.
*
*/


#ifndef __EASYDIALINGCENREPLISTENER_H__
#define __EASYDIALINGCENREPLISTENER_H__

//  INCLUDES
#include <e32base.h>
#include <cenrepnotifyhandler.h> // link against commonengine.lib

// FORWARD DECLARATIONS
class CRepository;
class CenRepNotifyHandler;
class MEasyDialingCenrepListenerObserver;

// CLASS DECLARATION

/**
*  Central Repository event monitor
*/
class CEasyDialingCenrepListener
    : public CBase, public MCenRepNotifyHandlerCallback
{
    public:  // Constructors and destructor

        /**
        * Static constructor. Construct a CEasyDialingCenrepListener
        * using two phase construction, and return a pointer to the created object.
        * @param    aObserver   Pointer to observer.
        * @return   Created object.
        */
        static CEasyDialingCenrepListener* NewL(MEasyDialingCenrepListenerObserver* aObserver);
        
        /**
        * Destructor. Destroy the object and release all memory objects
        */
        virtual ~CEasyDialingCenrepListener();

    public:

        /**
        * Gets the easydialing on/off cenrep key value
        * @return   The key value
        */
        TInt Value() const;
        
        /**
         * Sets the easydialing on/off cenrep key value
         * @param aValue Value to be set.
         */
        void SetEasyDialingSettingsValue( const TInt aValue );
        
        /**
         * Handles cenrep notifications.
         * @param   Id of the changed key.
         */
       void HandleNotifyGeneric( TUint32 aId );
              
    private:

        /**
        * Perform the first phase of two phase construction
        * @param    aObserver   Pointer to observer
        */
        CEasyDialingCenrepListener(MEasyDialingCenrepListenerObserver* aObserver);

        /**
        * Second phase constructor
        */
        void ConstructL();

    private:       

        /** The central repository handle. Owned. */
        CRepository* iClient;

        /** Observer to this class. Not owned. */
        MEasyDialingCenrepListenerObserver* iObserver;
        
        /** The cenrep value for easydialing setting. */
        TInt iEasyDialingSettingValue;
        
        /** The cenrep value for contact thumbnail setting (shown / not shown). */
        TInt iContactThumbnailsSettingsValue;
     
        /** Cenrep notify handler. Owned. */
        CCenRepNotifyHandler* iNotifyHandler;
};

#endif      // __EASYDIALINGCENREPLISTENER_H__
            
// End of File
