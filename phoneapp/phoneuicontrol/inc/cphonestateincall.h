/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*     In call state implementation.
*
*/


#ifndef CPHONESTATEINCALL
#define CPHONESTATEINCALL

// INCLUDES
#include "cphonestate.h"

// FORWARD DECLARATIONS
class CPhoneDtmfWaitCharTimer;

// CLASS DECLARATION
/**
*  In call state
*/
class CPhoneStateInCall : public CPhoneState
    {
    public:  

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPhoneStateInCall();

        /**
        * Creates the InCall state class
        * @param aStateMachine: a state machine
        * @param aViewCommandHandle: handle to the PhoneUIView
        * @return an instance of class CPhoneStateInCall
        */
        static CPhoneStateInCall* NewL( 
            MPhoneStateMachine* aStateMachine, 
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aCustomization );

        IMPORT_C virtual void HandleKeyMessageL(
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

    protected:

        /** 
        * Handle state-specific behaviour when number entry is cleared
        */ 
        IMPORT_C virtual void HandleNumberEntryClearedL();

        /**
        * By default EPOC constructor is private.
        */
        IMPORT_C CPhoneStateInCall( 
            MPhoneStateMachine* aStateMachine, 
            MPhoneViewCommandHandle* aViewCommandHandle,
            MPhoneCustomization* aPhoneCustomization );
        
        /**
        * ConstructL()
        */
        IMPORT_C virtual void ConstructL();

        /**
        * Open menu bar
        */
        IMPORT_C virtual void OpenMenuBarL();

        /**
        * Launch the New call phone number query
        */
        IMPORT_C void LaunchNewCallQueryL();

        /**
        * Call number fetched from New call dialog
        * @param aFetchedNumber number fetched from the diaog
        */
        IMPORT_C virtual void CallFetchedNumberL( 
            const TDesC& aFetchedNumber );

        /**
        * Handle EPEMessageIdle
        */
        IMPORT_C void HandleIdleL( TInt aCallId );
        
        /**
        * Method for quering is video call active.
        * @return ETrue if active otherwise EFalse
        */
        IMPORT_C virtual TBool IsVideoCallActiveL();
         
        /**
        * Method for quering is video call ringing.
        * @return ETrue if ringing otherwise EFalse
        */
        IMPORT_C virtual TBool IsVideoCallRingingL();
        
        /**
        * Method for quering ringing call .
        * @return call id otherwise KErrNotFound
        */
        IMPORT_C virtual TBool GetRingingCallL();
        /**
        * Search and release outgoingcall
        */
        IMPORT_C void DisconnectOutgoingCallL();
        
        /**
        * Upadated CBAs Incall case
        */
        IMPORT_C virtual void UpdateInCallCbaL();
        

        /**
        * Upadated CBAs
        */
        IMPORT_C virtual void UpdateCbaL( TInt aResource );     

        /**
        * Close all dtmf dialogs
        */
        IMPORT_C void CloseDtmfQueryL();
    
        /**
        * Creates number entry.
        */
        IMPORT_C void CreateNumberEntryL();
        
        
        /**
          * Concludes does the engine have information of the
          * phone number. This information is then send to the view.
          * @param aCallId - Call Id.
        */
        IMPORT_C void PhoneNumberAvailableInPhoneEngineL( TInt aCallId );
    
    private: // New functions

        /**
        * Handle EPEMessageAudioMuteChanged
        */
        void HandleAudioMuteChangedL();

        /**
        * Handle EPEMessageAudioOutputChanged
        */
        void HandleAudioOutputChangedL();
        
        /**
        * Handle EPEMessageAudioVolumeChanged
        */
        void HandleAudioVolumeChangedL();
        
        /**
        * Handle EPEMessageSendingDTMF
        */
        void HandleSendingDTMFL();
        
        /**
        * Handle EPEMessageStoppedDTMF
        */
        void HandleStoppedDTMFL();

        /**
        * Handles EPEMessageDTMFSent
        *         EPEMessageDTMFSendingAborted
        */
        void CancelDTMFSendingL();

        /**
        * Handle EPEMessagePromptSpeedDial
        */
        void HandleDTMFPromptSpeedDialL();

        /**
        * Sends DTMF sequence to Phone Engine
        */
        void SendDtmfL();
        
        /**
        * Sends DTMF speed dial number sequence to Phone Engine
        */
        void SendDtmfSpeedDialNumberL();
        
        /**
        * Sends command to view for launching manual DTMF entry
        */
        void LaunchDtmfManualQueryL();

        /**
        * Sends command to view for launching DTMF list query
        */
        void LaunchDtmfListQueryL();
        
        /**
        * Launch DTMF Single Item fetch dialog
        */
        void LaunchDtmfSearchDialogL();
        
        /**
        * Launch DTMF Single Item fetch for DTMF List query usage
        */
        void LaunchDtmfListViewSearchDialogL();

        /**
        * Launch New call fetch dialog
        */
        void LaunchNewCallSearchDialogL();
              
        /**
        * Sends command to view for 'Lock keypad'
        */
        void LockKeypadL();
        
        /**
        * Handle successful single item fetch.
        */
        void HandleSuccessFetchedNumberL();
        
        /**
        * Handle failed single item fetch.
        */
        void HandleFailedFetchedNumberL();
        
        /**
        * Update remote data and label to the call header.
        * @param aCallId - Call Id.
        * @param aCallHeaderData - Call header parameters where modifications
        *                          are made.
        */
        void UpdateRemoteInfoDataAndLabelL( 
                TInt aCallId, 
                TPhoneCmdParamCallHeaderData aCallHeaderParam );
        
        /**
        * Show dtmf text query
        * @param aDialogResourceId dialog's resource id
        * @param aDefaultCbaResourceId default CBA's resource id
        * @param aContentCbaResourceId content CBA's resource id
        * @param aDataText pointer to data text
        * @param aSendKeyEnabled send key enabled status for the query
        */
        void ShowDtmfTextQueryL(
            TInt aDialogResourceId,
            TInt aDefaultCbaResourceId,
            TInt aContentCbaResourceId,
            TDes* aDataText,
            TBool aSendKeyEnabled = EFalse );
                        
        /**
        * Handles End key pressing (long/short).
        * @param aMessage - Key event from AVKON
        */
        void HandleEndKeyPressL( TPhoneKeyEventMessages aMessage );

        /**
        * Handles Voice key pressing.
        * @param aMessage - Key event from AVKON
        */
        void HandleVoiceKeyPressL( TPhoneKeyEventMessages aMessage );
        
    private:
    
        /**
        * Special timer for preventing recursive calls
        * which would otherwise occur with multiple
        * wait characters in a DTMF string.
        */
        CPhoneDtmfWaitCharTimer* iDtmfWaitCharTimer;
  
    
    };

#endif // CPHONESTATEINCALL

// End of File
