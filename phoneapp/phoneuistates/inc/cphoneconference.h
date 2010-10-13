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
*     Conference call state implementation.
*
*/


#ifndef CPHONECONFERENCE_H
#define CPHONECONFERENCE_H

// INCLUDES
#include "cphonegsmincall.h"
#include "tphonecmdparamboolean.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
*  Conference call state implementation.
*/
class CPhoneConference : public CPhoneGsmInCall
    {
    public:  

        /**
        * Destructor.
        */
        virtual ~CPhoneConference();

        /**
        * Creates the Conference Call state class
        * @param aStateMachine: a container of state objects
        * @param aViewCommandHandle: handle to the PhoneUIView
        * @return an instance of class CPhoneStateIncoming
        */
        static CPhoneConference* NewL( 
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

        virtual TBool HandleCommandL( TInt aCommand );  
        
        virtual void HandleKeyMessageL(
            TPhoneKeyEventMessages aMessage,
            TKeyCode aCode );
        
        /**
        * This function is called from displaycallsetup and 
        * the purpose is to do state specific things for callsetup.
        */
        IMPORT_C virtual void DoStateSpecificCallSetUpDefinitionsL();
            
    protected:

        /**
        * By default EPOC constructor is private.
        */
        CPhoneConference( 
            MPhoneStateMachine* aStateMachine, 
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aPhoneCustomization );
        
        /**
        * ConstructL()
        */
        virtual void ConstructL();
        
        virtual void HandleKeyEventL( 
            const TKeyEvent& aKeyEvent, 
            TEventCode aEventCode );
            
        virtual void HandleNumberEntryClearedL();
        
        virtual void OpenMenuBarL();

        virtual void HandleIdleL( TInt aCallId );
        
        virtual void MakeStateTransitionToSingleL();

        virtual void HandleIncomingL( TInt aCallId );

        virtual void DisplayIncomingCallL( 
            TInt aCallId, 
            const TPhoneCmdParamBoolean aCommandParam );
        
        virtual void HandleConferenceIdleL();
                        
        virtual void MakeStateTransitionToTwoSinglesL();
        
        virtual TBool IsConferenceBubbleInSelectionMode() const;
        
        virtual void UpdateInCallCbaL();
        
        void CloseSelectionListL();
        
        virtual void SetHoldFlagL();
        
        virtual void DefineAndSetHoldFlagL();
  
        virtual void HandleCreateNumberEntryL( 
			const TKeyEvent& aKeyEvent,
            TEventCode aEventCode );

        void UpdateConferenceSecurityStatusL( TInt aCallId );
		
        virtual void DisconnectCallL();
        
     private:
              
        void OpenParticipantsListL();
     
        void HandleHeldConferenceL( TInt aCallId );
         
        void HandleConnectedConferenceL();

        void ToggleHoldL();

        void OpenDropParticipantSelectionL();
        
        void DropSelectedParticipantL();
        
        void OpenPrivateSelectionL();
        
        void PrivateSelectedParticipantL();
        
        void MakeStateTransitionToIdleL();
        
        void HandleDialingL( TInt aCallId );
              
        void CallFromNewCallQueryL();
        
        void HandleWentOneToOneL( TInt aCallId );
        
    };

#endif // CPHONECONFERENCE_H

// End of File
