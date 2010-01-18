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
* Description:  GSM-specific in-call handling.
*
*/


#ifndef CPHONEGSMINCALL_H
#define CPHONEGSMINCALL_H

// INCLUDES
#include "cphonestateincall.h"
#include "tphonecmdparamboolean.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
*  GSM-specific in-call handling.
*/
class CPhoneGsmInCall : public CPhoneStateInCall
    {
    public:  

        /**
        * Destructor.
        */
        virtual ~CPhoneGsmInCall();
        
        /**
        * Creates the GSM in-call state class
        * @param aStateMachine: a state machine
        * @param aViewCommandHandle: handle to the PhoneUIView
        * @return an instance of class CPhoneEmergency
        */
        static CPhoneGsmInCall* NewL( 
            MPhoneStateMachine* aStateMachine, 
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aPhoneCustomization );

        /**
        * A message handling function for Phone Engine messages
        * @param aMessage Message from Phone Engine
        * @param aCallId Call id the message concerns
        */
        void HandlePhoneEngineMessageL(
            const TInt aMessage, 
            TInt aCallId );

    protected:  

        /**
        * By default EPOC constructor is private.
        */
        CPhoneGsmInCall( 
            MPhoneStateMachine* aStateMachine, 
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aPhoneCustomization );

        /**
        * Handles commands.
        * @param aCommand It is the code of the command to be handled.
        * @returns boolean value was the command handled by the state 
        *          (ETrue) or not (EFalse)
        */    
        IMPORT_C TBool HandleCommandL( TInt aCommand );    

        /**
        * ConstructL()
        */
        virtual void ConstructL();
        
        /**
        * Show Colp note if necessary
        */
        void HandleColpNoteL( TInt aCallId );
        
        /**
        * Bring call handling to foreground when moved to incoming state
        **/        
        void BringIncomingToForegroundL();
       
        /**
        * Get allow waiting call header value.
        */
        void AllowShowingOfWaitingCallHeaderL( 
            TPhoneCmdParamBoolean& aCommandParam );
        
        /**
        * Indicates when the Phone app is in the foreground.
        */    
        IMPORT_C void HandlePhoneForegroundEventL();
        
        /**
        * Show hold/connected note 
        */
        void HandleHoldNoteL( TInt aCallId, TBool aHold );
        
        
        /**
        * Setter for divert indication showing in bubble.
        * @param aDivertIndication ETrue to show divert indication,
        *        EFalse to not. Usually setting EFalse isn't necessary
        *        as it's a default value in bubble creation.
        */
        IMPORT_C void SetDivertIndication(
            const TBool aDivertIndication );
        
    private:
    
    	void ReplaceCallL();
    	/**
        * Handles divert indication 
        */
    	void HandeDivertIndicationL();
    };

#endif // CPHONEGSMINCALL_H

// End of File
