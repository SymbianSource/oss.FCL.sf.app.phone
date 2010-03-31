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
*     Describes the interface for setting and getting active state.
*
*/


#ifndef MPHONESTATEMACHINE_H
#define MPHONESTATEMACHINE_H

// INCLUDES
#include "mphonestate.h"
#include "mphoneenginemessagesender.h"


// CLASS DECLARATION
class MPhoneState;
class MPEPhoneModel;
class MPEEngineInfo;
class CSpdiaControl;
class MPhoneSecurityMessageHandler;
class MPhoneSecurityModeObserver;

/**
*  Describes the interface for setting and getting active state
*/

class MPhoneStateMachine : public MPhoneEngineMessageSender
    {
    public:

        /**
        * Destructor.
        */
        virtual ~MPhoneStateMachine() {};
    
        /**
        * Sets active state
        */
        virtual void ChangeState( TInt aState ) = 0;

        /**
        * Returns active state
        */
        virtual MPhoneState* State() = 0;
        
        /**
        * Sets pointer to Phone Engine
        * @param aPhoneEngine pointer to Phone Engine instance
        */
        virtual void SetPhoneEngine( MPEPhoneModel* aPhoneEngine ) = 0;

        /**
        * Gets pointer to Phone Engine
        * @returns pointer to Phone Engine
        */
        virtual MPEPhoneModel* PhoneEngine() = 0;

        /**
        * Gets pointer to Phone Engine Info
        * @return pointer to Phone Engine Info
        */
        virtual MPEEngineInfo* PhoneEngineInfo() = 0;
        
        /**
        * Instantiates protocol specific Phone Engine
        * @param reference to Engine Monitor
        * @return Phone Engine instance
        */
        virtual MPEPhoneModel* CreatePhoneEngineL( 
            MEngineMonitor& aEngineMonitor ) = 0;
        
        /**
        * Sets callId to Phone Engine Info
        * @param aCallId
        */    
        virtual void SetCallId( TInt aCallId ) = 0;
    
        /**
         * Getter for security mode observer interface.
         * 
         * @return Pointer to security mode interface.
         */
        virtual MPhoneSecurityModeObserver* SecurityMode() = 0;
        
		/**
		 * Sets security mode observer. Doesn't transfer ownership.
		 *
		 * @param aObserver Pointer to observer instance.
		 */
		virtual void SetSecurityModeObserver( MPhoneSecurityModeObserver* aObserver ) = 0;
		
		/**
		 * Set security message handler for security message observations.
		 * 
		 * @param aHandler Pointer to handler instance. 
		 */
		virtual void SetSecurityMessageHandler( MPhoneSecurityMessageHandler* aHandler ) = 0;
		
        /**
         * Handle phone engine message.
         * 
         * @param aMessage Message
         * @param aCallId Call id
         */
        virtual void HandlePhoneEngineMessageL(const TInt aMessage, 
        	TInt aCallId ) = 0;
        
        /**
         * This function is called when there is property value change.
         * @param aCategory Category of the property
         * @param aKey Property key that is changed
         * @param aValue New property value
         */
        virtual void HandlePropertyChangedL(const TUid& aCategory,
        	const TUint aKey,
        	const TInt aValue ) = 0;

    };

#endif // MPHONESTATEMACHINE_H

// End of File
