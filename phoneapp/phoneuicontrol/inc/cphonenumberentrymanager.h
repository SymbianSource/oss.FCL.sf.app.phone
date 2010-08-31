/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Manager number entry for state
*
*/


#ifndef CPONENUMBERENTRYMANAGER_H
#define CPONENUMBERENTRYMANAGER_H

//INCLUDES
#include <e32base.h>

#include "cphonestate.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamkeyevent.h"

class CPhoneCbaManager;
class MPhoneViewCommandHandle;
class MPhoneStateMachine;
class MPhoneCustomization;
class CPhoneState;
class CEikonEnv;

/**
 * Class contains helper functions for NE content
 *
 * @lib phoneuicontrol
 * @since S60 v9.1
 */
NONSHARABLE_CLASS( CPhoneNumberEntryManager ) :  public CBase
                                             
    {
    public:
        
        /**
        * Creates new instance of CPhoneNumberEntryManager
        * @param aState: Handle to cphonestate.
        * @param aStateMachine: a state machine
        * @param aViewCommandHandle: Handle to the PhoneUIView.
        * @param aCustomization: Handle to customization.
        * @param aCbaManager: Handle to cphonecbamanager.
        * @return CPhoneNumberEntryManager* object  
        */
        static CPhoneNumberEntryManager* NewL(
                    CPhoneState* aState,
                    MPhoneViewCommandHandle& aViewCommandHandle,
                    MPhoneStateMachine& aStateMachine,
                    MPhoneCustomization* aCustomization,
                    CPhoneCbaManager& aCbaManager );
        /**
        * Destructor.
        */
        virtual ~CPhoneNumberEntryManager();    
        
    public:

        /**
        * Stores the number entry content to the cache
        */
        void StoreNumberEntryContentL();
        

        /**
        * Check if number entry content is stored
        * @param None
        * @return boolean value indicating that number entry content is stored
        */
        TBool IsNumberEntryContentStored();
        
        /**
        * Clears the number entry content cache
        */        
        void ClearNumberEntryContentCache();
        
        /**
        * Set Number Entry visibility.
        * @param aVisible ETrue if numberentry is wanted to be shown
        *                 (Note ETrue will set NE CBA's)
        *                 EFalse if numberentry isnt wanted to be shown
        *                 (Note EFalse doesnt affect to CBA's)
        */
        void SetNumberEntryVisibilityL( TPhoneCmdParamBoolean aVisible );
        
        /**
        * Check if number entry is used
        * @return boolean value indicating that number entry is used
        */
        TBool IsNumberEntryUsedL() const;
        
        /**
        * Check if number entry is visible
        * @return boolean value indicating that number entry is visible
        */
        TBool IsNumberEntryVisibleL() const;
        
        /**
         * Returns phone number from the phone number entry.
         * @return  Phone number
         */
        HBufC* PhoneNumberFromEntryLC() const;
        
        
        /**
        * Returns ETrue if alphanumeric characters are supported.
        * @param aKeyEvent Key event.
        * @return ETrue if alphanumeric chars are supported.
        */
        TBool IsAlphanumericSupportedAndCharInput(
                const TKeyEvent& aKeyEvent ) const;
        
        /**
        * Handle state-specific behaviour when number entry is cleared
        */
        void HandleNumberEntryClearedL();
        
        /**
         * Internal number entry handling methods.
         */
        void NumberEntryClearL() const;
        
    private:
        
        /**
        * Checks is given key contains numeric charaters or if customization is used
        * alphanumeir letters
        */
        TBool IsValidAlphaNumericKey( const TKeyEvent& aKeyEvent,
                TEventCode aEventCode );
    
    private:
    
        /**
        * C++ default constructor.
        */
        CPhoneNumberEntryManager(
                CPhoneState* aState,
                MPhoneViewCommandHandle& aViewCommandHandle,
                MPhoneStateMachine& aStateMachine,
                MPhoneCustomization* aCustomization,
                CPhoneCbaManager& aCbaManager );
        
        /**
        * Symbian constructor
        */
        void ConstructL();
        
    private:
        
        CPhoneState* iState;
        MPhoneViewCommandHandle& iViewCommandHandle;
        MPhoneStateMachine& iStateMachine;
        MPhoneCustomization* iCustomization;
        CPhoneCbaManager& iCbaManager;
        
                
        /**
        * Cache for the number entry content
        */
        HBufC* iNumberEntryContent;
        
        /**
        * Internal variable for EikonEnv to avoid
        * use of static system calls
        */
        CEikonEnv& iEnv;

    };


#endif // CPONENUMBERENTRYMANAGER_H
