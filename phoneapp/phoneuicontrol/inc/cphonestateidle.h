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
*     Idle state implementation.
*
*/


#ifndef CPHONESTATEIDLE
#define CPHONESTATEIDLE

// INCLUDES
#include "cphonestate.h"
#include "tphonecmdparamspeeddial.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Idle state
*/
class CPhoneStateIdle : public CPhoneState
    {
    public:  
        
    /** Dial initiation method. */
    enum TDialInitiationMethod
        {
        /** Dial initiation method unknown. */
        EDialMethodUnknown          = 0,
        /** Dial is initiated by explicit call type selection from menu.*/
        EDialMethodMenuSelection    = 1,
        /** Dial is initiated by send command. */
        EDialMethodSendCommand      = 2,
        /** Dial is initiated by one key dialing. */
        EDialMethodOneKeyDialing    = 3
        };
    
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPhoneStateIdle();

        /**
        * Creates the Idle state class
        * @param aStateMachine: a container of state objects
        * @param aViewCommandHandle: handle to the PhoneUIView
        * @return an instance of class CPhoneStateIdle
        */
        static CPhoneStateIdle* NewL( 
            MPhoneStateMachine* aStateMachine, 
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aPhoneCustomization );

        IMPORT_C virtual void HandleKeyEventL(
            const TKeyEvent& aKeyEvent,
            TEventCode aEventCode );

        IMPORT_C void HandleKeyMessageL(
            TPhoneKeyEventMessages aMessage,
            TKeyCode aCode );

        /**
        * A message handling function for Phone Engine messages
        * @param aMessage Message from Phone Engine
        * @param aCallId Call id the message concerns
        */
        IMPORT_C virtual void HandlePhoneEngineMessageL(
            const TInt aMessage, 
            TInt aCallId );
            
        IMPORT_C virtual TBool HandleCommandL( TInt aCommand );
        
        IMPORT_C virtual TBool ProcessCommandL( TInt aCommand );

        /**
        * Indicates when the Idle app is in the foreground.
        */
        IMPORT_C virtual void HandleIdleForegroundEventL();

        /**
        * Indicates when the Phone app is in the foreground.
        */
        IMPORT_C virtual void HandlePhoneForegroundEventL();

        /**
        * Indicates when the Phone app has lost focus.
        */
        IMPORT_C virtual void HandlePhoneFocusLostEventL();
        
        /**
        * This function is called from displaycallsetup and 
        * the purpose is to do state specific things for callsetup.
        */
        IMPORT_C virtual void DoStateSpecificCallSetUpDefinitionsL();
		
		/**
        * HandleError
        * Implements error handling framework
        * @param aErrorInfo: the error info
        */
        IMPORT_C virtual void HandleErrorL( 
                const TPEErrorInfo& aErrorInfo );
    protected:

        /** 
        * Handle state-specific behaviour when number entry is cleared
        */ 
        IMPORT_C virtual void HandleNumberEntryClearedL();

        IMPORT_C virtual TBool HandleRemConCommandL( 
            TRemConCoreApiOperationId aOperationId, 
            TRemConCoreApiButtonAction aButtonAct );

        /**
        * Display Incoming Call
        * @param aCallid call id
        */
        IMPORT_C void DisplayIncomingCallL( TInt aCallId );

        /**
        * By default EPOC constructor is private.
        */
        IMPORT_C CPhoneStateIdle( 
            MPhoneStateMachine* aStateMachine, 
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aPhoneCustomization );
        
        /**
        * ConstructL()
        */
        IMPORT_C virtual void ConstructL();

        /**
        * Dial the specified number. 
        * @param    aNumber            The number to call.
        * @param    aNumberType        A number type.
        * @param    aDialMethod        A dial initiation method.
        */
        IMPORT_C virtual void DialL( const TDesC& aNumber, 
            TPhoneNumberType aNumberType, TDialInitiationMethod aDialMethod );
        
        /**
        * Dial CS multimedia call
        * @param None
        */
        IMPORT_C void DialMultimediaCallL();

        /**
        * Dial CS voice call
        * @param None
        */
        IMPORT_C void DialVoiceCallL();

        /**
        * Handles situation when hash key has been kept down long
        * (long keypress) and the hash character is the only character
        * in number entry.
        */
        IMPORT_C virtual void OnlyHashInNumberEntryL();

        /**
        * A message handling function for EPEMessageDialling
        * @param aCallId: the call id of the call
        */
        IMPORT_C virtual void HandleDialingL( TInt aCallId );

        /**
        * This is called when Speed Dial dialog is cancelled or user didn't give
        * a valid number.
        * @param aDigit: entered digit
        */
        IMPORT_C void SpeedDialCanceledL( const TUint& aDigit );
        
        /**
        * Handle long key press of a number.
        */
        IMPORT_C void HandleNumberLongKeyPressL();
        
        /**
        * Tests whether given number is speed dial number.
        * @param    aNumber    A number to test.
        * @return   ETrue if given number is speed dial number.
        */
        IMPORT_C TBool IsSpeedDialNumber( const TDesC& aNumber ) const;
        
        /**
        * Handles speed dialing.
        * @param aDigit             Entered digit.
        * @param aDialMethod        Dial initiation method.
        */
        IMPORT_C void SpeedDialL( const TUint& aDigit, 
            TDialInitiationMethod aDialMethod );

        /**
        * Handles send command.
        */
        IMPORT_C virtual void HandleSendCommandL();
        
    protected:
        
        /**
        * Sets iCallInitialized value.
        */
        void SetCallInitialized( TBool aValue );
         
        /**
        * Gets iCallInitialized value.
        */
        TBool IsCallInitialized();
        
        /**
        * Removes NE and sets idle to background, for more detailed
        * information check comments from implementation.
        */
        void RemoveNumberEntryAndSetIdleToBackgroundIfNeededL();
        
    private: // New functions
        
        /**
        * A message handling function for EPEMessageIncoming
        * @param aCallId: the call id of the call
        */
        void HandleIncomingL( TInt aCallId );

        /**
        * A message handling function for EPEMessageHandleConnected
        * @param aCallId: the call id of the call
        */
        void HandleConnectedL( TInt aCallId );
        
        /**
        * A message handling function for EPEMessageIdle
        * @param aCallId: the call id of the call
        */
        void HandleIdleL( TInt aCallId );

        /**
        * Launches application.
        */
        void LaunchApplicationL();

        /**
        * Send Exit command
        */
        void SendExitCommandL();

        /**
        * Dials video call 
        */
        void DialVideoCallL();
        
        /**
        * Handles end key press that was catched in HandleKeyMessageL
        * @param aMessage - key event message
        */
        void HandleEndKeyPressL( TPhoneKeyEventMessages aMessage );
        
        /**
        * Changes manner mode.
        * Switches from any profile to the Silent profile or from the
        * Silent profile to the General profile.
        */
        void ChangeMannerModeL();
        
        /**
        * Checks if MC/PCSuite restore ongoing.
        * @param none.
        * @return ETrue if ongoing.
        */
        TBool RestoreOngoing();
        
        /**
        * Handles back command. 
        */
        void HandleBackCommandL();

        /**
         * Checks for application launching configurations from 
         * central repository.
         * @param aCode key code.
         * @return ETrue if configuration found.
         */        
         TBool CheckAppLaunchingL( const TKeyCode aCode );

         /**
          * Returns phone number for specified speed dial location.
          * @param  aDigit Speed dial digit.
          * @param  aDigit On return contains speed dial parameters.
          * @return Phone number
          */   
         HBufC* NumberForSpeedDialLocationL( const TUint& aDigit,
             TPhoneCmdParamSpeedDial& aSpeedDialParam ) const;

         void HandleVoiceCallCommandL( TBool aSendKey );

         /**
          * Returns options menu id when number entry is visible.
          * If easydialing is in focus, id is asked from it.
          * @return Menu resource id.
          */   
         TInt GetNumberAcqMenuIdL();
         
         /**
          * Changes state to aState and resets iBubbleInitialized
          * value to false.
          */
         void ChangeTo( TInt aState );
         
         /**
         * Opens or creates number entry and forwards key event to it.
         * @param aKeyEvent - key event
         * @param aCode - event code
         */
         void OpenNumberEntryWithKeyEventL( 
                 const TKeyEvent& aKeyEvent,
                 TEventCode aEventCode );
         
    private:
        
         // True if call initiliazed.
         TBool iCallInitialized;
    };

#endif // CPHONESTATEIDLE

// End of File
