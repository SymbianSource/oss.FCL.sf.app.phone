/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Conference and waiting call state implementation.
*
*/


#ifndef CPHONECONFERENCEANDWAITING_H
#define CPHONECONFERENCEANDWAITING_H

// INCLUDES
#include "cphoneconference.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
*  Conference and waiting call state implementation.
*/
class CPhoneConferenceAndWaiting : public CPhoneConference
    {
    public:  

        /**
        * Destructor.
        */
        virtual ~CPhoneConferenceAndWaiting();

        /**
        * Creates the Conference And Waiting state class
        * @param aStateMachine: a container of state objects
        * @param aViewCommandHandle: handle to the PhoneUIView
        * @return an instance of class CPhoneStateIncoming
        */
        static CPhoneConferenceAndWaiting* NewL( 
            MPhoneStateMachine* aStateMachine, 
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aPhoneCustomization );

        /**
        * A message handling function for Phone Engine messages
        * @param aMessage Message from Phone Engine
        * @param aCallId Call id the message concerns
        */
        virtual void HandlePhoneEngineMessageL(
            const TInt aMessage, 
            TInt aCallId );   
        
        virtual void HandleKeyMessageL(
            TPhoneKeyEventMessages aMessage,
            TKeyCode aCode );
     
        /**
        * HandleError
        * Implements error handling framework
        * @param aErrorInfo: the error info
        */
        IMPORT_C virtual void HandleErrorL( 
            const TPEErrorInfo& aErrorInfo ); 
        
        /**
        * This function is called from displaycallsetup and 
        * the purpose is to do state specific things for callsetup.
        */
        IMPORT_C virtual void DoStateSpecificCallSetUpDefinitionsL();
        
    protected:

        /**
        * By default EPOC constructor is private.
        */
        CPhoneConferenceAndWaiting( 
            MPhoneStateMachine* aStateMachine, 
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aPhoneCustomization );
        
        /**
        * ConstructL()
        */
        virtual void ConstructL();
        
        virtual void OpenMenuBarL();
        
        virtual void UpdateInCallCbaL();
        
        /**
        * Passes create number entry command forward if NE can be created.
        */
        void HandleCreateNumberEntryL(
                const TKeyEvent& aKeyEvent,
                TEventCode aEventCode );

     private:
     
         void HandleConnectedL( TInt aCallId );
         
         void HandleIdleL( TInt aCallId );
         
         void HandleConferenceIdleL();
                  
         void MakeStateTransitionToConferenceAndSingleL( TInt aCallId );
         
         void HandleDialingL( TInt aCallId );
         
         void HandleWentOneToOneL( TInt aCallId );
      
     private:
        TInt iRingingCallId;
         
    };

#endif // CPHONECONFERENCEANDWAITING_H

// End of File
