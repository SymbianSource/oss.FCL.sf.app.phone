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
* Description: 
*     GSM-specific incoming state implementation.
*
*/


#ifndef CPHONEINCOMING_H
#define CPHONEINCOMING_H

// INCLUDES
#include "cphonestateincoming.h"

// FORWARD DECLARATIONS
class CPhoneState;
class TPhoneCmdParamBoolean;

// CLASS DECLARATION
/**
*  GSM-specific incoming state implementation.
*/
class CPhoneIncoming : public CPhoneStateIncoming
    {
    public:  

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPhoneIncoming();
        
        /**
        * Creates the Incoming state class
        * @param aStateMachine: a state machine
        * @param aViewCommandHandle: handle to the PhoneUIView
        * @return an instance of class CPhoneEmergency
        */
        static CPhoneIncoming* NewL( 
            MPhoneStateMachine* aStateMachine, 
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aPhoneCustomization );

        /**
        * A message handling function for Phone Engine messages
        * @param aMessage Message from Phone Engine
        * @param aCallId Call id the message concerns
        */
        IMPORT_C void HandlePhoneEngineMessageL(
            const TInt aMessage, 
            TInt aCallId );    

        /**
        * HandleError
        * Implements error handling framework
        * @param aErrorInfo: the error info
        */
        IMPORT_C virtual void HandleErrorL( 
            const TPEErrorInfo& aErrorInfo ); 
    protected:

        /**
        * By default EPOC constructor is private.
        */
        IMPORT_C CPhoneIncoming( 
            MPhoneStateMachine* aStateMachine, 
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aPhoneCustomization );
                    
        /**
        * ConstructL()
        */
        IMPORT_C virtual void ConstructL();
        
        /**
        * Handles long hash key
        */
        virtual void HandleLongHashL();

        /**
        * Passes create number entry command forward if NE can be created.
        */
        IMPORT_C void HandleCreateNumberEntryL(
                const TKeyEvent& aKeyEvent,
                TEventCode aEventCode );

    private:
        
        /**
        * A message handling function for EPEMessageIncoming
        * @param aCallId: the call id of the call
        */
        void HandleIncomingL( TInt aCallId );
        
        /**
        * Display Incoming Call
        * @param aCallid call id
        * @param aCommandParam
        */
        void DisplayIncomingCallL( 
            TInt aCallId, 
            const TPhoneCmdParamBoolean aCommandParam );
        
        /**
        * Get allow waiting call header value.
        * @param aCommandParam
        */
        void AllowShowingOfWaitingCallHeaderL( 
            TPhoneCmdParamBoolean& aCommandParam );
    private:
        
        // call id of new waiting call
        TInt iWaitingCallId;
    };

#endif // CPHONEINCOMING_H

// End of File
