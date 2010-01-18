/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Qwerty Mode Monitor.
*
*/


#ifndef CPHONEQWERTYMODEMONITOR_H 
#define CPHONEQWERTYMODEMONITOR_H 

//  INCLUDES
#include    "mphonepubsubobserver.h" 
#include    "mphoneqwertymodeobserver.h"


// CLASS DECLARATION

/**
*  Qwerty Mode Monitor.
*/
class CPhoneQwertyModeMonitor 
    : public CBase, 
      private MPhonePubSubObserver
    {
    friend class T_CPhoneQwertyModeMonitorUT;
    
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        * @return New instance
        */
        static  CPhoneQwertyModeMonitor* NewL();
        /**
        * Destructor.
        */
        virtual ~CPhoneQwertyModeMonitor();

    protected: // Functions from base classes
        
        /**
        * Handle the change of the setting from P&S key.
        * @param aUid identifing the P&S gategory.
        * @param aId P&S key.
        * @param aValue Current value of the key.
        */
        virtual void HandlePropertyChangedL( 
            const TUid& aCategory,
            const TUint aKey,
            const TInt aValue );

    public: // New methods

        /**
        * Gets current qwerty mode.
        * @return Current qwerty mode.
        */
        TInt QwertyMode() const;

        /**
        * Gets current keyboard.
        * @return Current keyboard.
        */
        TInt Keyboard() const;
                
        /**
        * Adds qwerty mdoe observer.
        * @param aObserver Observer object.
        */
        void AddObserverL( MPhoneQwertyModeObserver& aObserver );

        /**
        * Removes qwerty mode observer.
        * @param aObserver Observer object.
        */
        void RemoveObserver( MPhoneQwertyModeObserver& aObserver );
        
    private: 

        /**
        * C++ default constructor.
        */
        CPhoneQwertyModeMonitor();

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();
        
        /**
        * Current qwerty mode.
        */
        TInt GetQwertyMode() const;

        /**
         * Current keyboard.
         */
        TInt GetKeyboard() const;

    private:    // Data

        // Call State Change Observer array.
        RPointerArray< MPhoneQwertyModeObserver > iObserverArray;

        // Current qwerty mode.
        TInt iQwertyMode;
        
        //Current keyboard.
        TInt iKeyboard;
    };

#endif      // CPHONEQWERTYMODEMONITOR_H 

// End of File












