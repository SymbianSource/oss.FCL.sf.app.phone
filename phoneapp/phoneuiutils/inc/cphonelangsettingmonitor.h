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
* Description:  Monitor for input language setting
*
*/

#ifndef CPHONELANGSETTINGMONITOR_H
#define CPHONELANGSETTINGMONITOR_H

//  INCLUDES
#include    <e32base.h>
#include    "mphonecenrepobserver.h" 

// CLASS DECLARATION
// FORWARD DECLARATIONS
class MPhoneLangSettingObserver;

/**
*  Monitor for call theme settings.
*
*/
class CPhoneLangSettingMonitor : public CBase, private MPhoneCenRepObserver
    {
    public:

        /**
        * Two-phased constructor.
        */
        static CPhoneLangSettingMonitor* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CPhoneLangSettingMonitor();

    public: // New functions
        
        /**
        * Get input language id.
        * @since Series 60 3.1
        * @return Language id.
        */
        TInt InputLanguage() const;
        
        /**
        * Add observer.
        * @since Series 60 3.1
        * @param aObserver Observer.
        */
        void AddObserverL( MPhoneLangSettingObserver& aObserver );

        /**
        * Remove observer.
        * @since Series 60 3.1
        * @param aObserver Observer.
        */
        void RemoveObserver( MPhoneLangSettingObserver& aObserver );

    protected:  // Functions from base classes

        /**
        * Handle the change of the setting from Central Repository
        * @param aUid identifing the central repository UID.
        * @param aId central repository ID.
        */
        virtual void HandleCenRepChangeL( 
            const TUid& aUid,
            const TUint aId );
            
    private:

        /**
        * C++ default constructor.
        */
        CPhoneLangSettingMonitor();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data                
        // AI Setting Change Observer array.
        RPointerArray< MPhoneLangSettingObserver > iObserverArray;
        // Language Setting.
        TInt  iInputLanguageSetting;
    };

#endif      // CPHONELANGSETTINGMONITOR_H   
            
// End of File
