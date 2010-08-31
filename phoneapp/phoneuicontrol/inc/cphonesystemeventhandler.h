/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*     Base class for handling key events.
*
*/


#ifndef __CPHONESYSTEMEVENTHANDLER_H
#define __CPHONESYSTEMEVENTHANDLER_H

//  INCLUDES
#include <e32base.h>
#include "mphonesystemeventhandler.h"
#include "mphonepubsubobserver.h"
#include "mphonecenrepobserver.h"

// FORWARD DECLARATIONS
class MPhoneStateMachine;

/**
* Transforms system events to be sent to the states
*/
class CPhoneSystemEventHandler : 
    public CBase, 
    public MPhoneSystemEventHandler,
    public MPhonePubSubObserver,
    public MPhoneCenRepObserver
    {
    public:

        /**
        * Two-phased constructor.
        */
        static CPhoneSystemEventHandler* NewL( 
            MPhoneStateMachine* aStateMachine );

        /**
        * Destructor.
        */
        virtual ~CPhoneSystemEventHandler();

    public: // From MPhoneSystemEvents

        void HandleSystemEventL( const TWsEvent& aEvent );

        void HandleForegroundEventL( TBool aForeground );

        void HandlePhoneForegroundEventL();

        void HandleIdleForegroundEventL();

        void HandleEnvironmentChangeL( const TInt aChanges );

        void HandlePhoneStartupL();
        
        void HandlePhoneFocusLostEventL();

        void HandleKeyLockEnabled( TBool aKeylockEnabled );
        
    public: // From MPhonePubSubObserver

        /**
        * This function is called when there is property value change.
        * @param aCategory Category of the property
        * @param aKey Property key that is changed
        * @param aValue New property value
        */
        virtual void HandlePropertyChangedL( 
            const TUid& aCategory,
            const TUint aKey,
            const TInt aValue );

    public: // From MPhoneCenRepObserver

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
        * By default EPOC constructor is private.
        */
        void ConstructL();

        /**
        * C++ default constructor.
        */
        CPhoneSystemEventHandler( MPhoneStateMachine* aStateMachine );

    private:  // Data

        /**
        * State machine
        */
        MPhoneStateMachine* iStateMachine;

    };

#endif      // __CPHONESYSTEMEVENTHANDLER_H   
            
// End of File
