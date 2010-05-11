/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  It is keyevent forwarder.
*
*/


#ifndef CPHONEKEYEVENTFORWARDER_H
#define CPHONEKEYEVENTFORWARDER_H

// INCLUDES
#include    <bldvariant.hrh> //Feature flags
#include    <coecntrl.h>
 
// FORWARD DECLARATIONS
class MPhoneStateMachine;
class MPhoneKeyEventHandler;
class MPhoneViewCommandHandle;
class CPhoneTimer;
class CPhoneQwertyHandler;
class MPhoneQwertyModeObserver;

// CLASS DECLARATION

/**
*  It is keyevent forwarder.
*
*/
class CPhoneKeyEventForwarder 
    : public CCoeControl 
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        *
        * @param aRect rectangle of this control.
        * @param aStateMachine state machine
        * @param aViewCommandHandle View command handle
        * @return new instance.
        */
        static CPhoneKeyEventForwarder* NewL( const TRect& aRect, 
                MPhoneStateMachine* aStateMachine,
                MPhoneViewCommandHandle* aViewCommandHandle  );
        
        /**
        * Destructor.
        */
        ~CPhoneKeyEventForwarder();     

    public: // New functions.
       
    public: // Functions from base classes
 
        /**
        * From CCoeControl, returns the amount of component controls.
        *
        * @return Returns the amount of component controls.
        */
        TInt CountComponentControls() const;              
        
        /**
        * From CCoeControl, returns a control by index.
        *    
        * @param aIndex It is the index of the control. 
        * @return Returns a control by index.
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * From CCoeControl, informs that the size is changed to update 
        * the sizes of component controls appropriately.
        */
        void SizeChanged();

        /**
        * From CCoeControl, draws the contents of the view.
        *
        * @param aRect It is the area to be redrawn.
        */
        void Draw( const TRect& aRect ) const;
    
        /**
        * From CCoeControl, offers a key event to the view. 
        *
        * @param aKeyEvent It contains information about the key event.
        * @param aType It is the type of the event.
        * @return Returns information which indicates whether the keypress
        *         was processed or not.
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
            TEventCode aType );
        
        /**
        * This function should be called from AppUi, after framework has
        * Looped through CCoeControlStack
        *
        * @param aKeyEvent It contains information about the key event.
        * @param aType It is the type of the event.
        * @return Returns information which indicates whether the keypress
        *         was processed or not.
        */
        TKeyResponse OfferKeyEventAfterControlStackL( 
                const TKeyEvent& aKeyEvent,
                TEventCode aType );

    private:

        /**
        * C++ default constructor.
        */
        CPhoneKeyEventForwarder( MPhoneStateMachine* aStateMachine,
                MPhoneViewCommandHandle* aViewCommandHandle  );

        /**
        * Symbian OS constructor.
        */
        void ConstructL( const TRect& aRect );

        /**
        * This function should be called before FEP or other components in 
        * CCoeControl stack
        *
        * @param aKeyEvent It contains information about the key event.
        * @param aType It is the type of the event.
        * @return Returns information which indicates whether the keypress
        *         was processed or not.
        */
        TKeyResponse OfferKeyEventBeforeControlStackL( 
                const TKeyEvent& aKeyEvent,
                TEventCode aType );
        
        /** 
        * Handle EEventKeyDown
        * @param aKeyEvent     Tells which key was pressed.
        * @return              tells whether the event was consumed or not.
        */
        TKeyResponse HandleEventKeyDownBeforeControlStackL( const TKeyEvent& aKeyEvent );
        
        /** 
        * Handle EEventKey
        * @param aKeyEvent     Tells which key was pressed.
        * @return              tells whether the event was consumed or not.
        */
        TKeyResponse HandleEventKeyBeforeControlStackL( const TKeyEvent& aKeyEvent );
        
        /** 
        * Handle EEventKeyUp
        * @param aKeyEvent     Tells which key was pressed.
        * @return              tells whether the event was consumed or not.
        */
        TKeyResponse HandleEventKeyUpBeforeControlStackL( const TKeyEvent& aKeyEvent );

      
        /**
        * Callback function for long press of a particular key
        */
        static TInt DoHandleLongPressKeyEventCallbackL( TAny* aAny );
        
        /**
        * Convert key code
        */   
        void ConvertKeyCode( TUint& aCode, const TKeyEvent& aKeyEvent ); 

        /**
        * Handle key long press
        */
        void HandleLongPressKeyEventL();
        
        /**
         * Checks from the statemachine is the given keyevent 
         * one which produces a legal character in the current mode.
         */
        TBool IsKeyAllowed( const TKeyEvent& aKeyEvent );
                
        /**
         * Checks is the given key special character (*, 0, # ) from half-qwerty
         * keyboard and conver key code if needed.
         */
        TBool ConvertHalfQwertySpecialChar( TUint& aCode, const TKeyEvent& aKeyEvent );
        
        /**
         * Checks if key should be blocked from phone.
         * 
         * @ return ETrue if alpha mode, EFalse otherwise.
         */
        TBool IsKeyBlocked( const TKeyEvent& aKeyEvent ) const;
        
        /**
         * Test if key has flags indicating that it has been simulated by the
         * touch dialer.
         */
        TBool IsKeySimulatedByTouchDialer( const TKeyEvent& aKeyEvent ) const;
        
        /**
         * Handles dialer key event. This includes handling multitapping of *-key. 
         * If multitap happens, function creates back space event 
         * to delete previous character, and replaces it with
         * the next one in the cyclig character list *+pw.
         */
        void HandleTouchDialerKeyEventL( const TKeyEvent& aKeyEvent );

    private:    // Data
        
        /**
        * State machine
        */
        MPhoneStateMachine* iStateMachine;
        
        /**
         * Interface to view command handle
         */
        MPhoneViewCommandHandle* iViewCommandHandle;
        
        /**
         * For long key press. Long press is handled within a 
         * specific duration.
         */
        CPhoneTimer* iLongPressKeyEventTimer; 

        /**
         * Key press time
         */
        TTime iKeyPressTime;

        /**
         * Key code
         */
        TUint iKeyPressedDown;
        
        /**
         * Key scan code from EEventKey Updated in HandleEventKeyDownL. 
         * Zeroed in HandleEventKeyUpL.
         */
        TInt iScanCode;

        /**
        * Has information of used qwerty keyboard.
        * Own.
        */
        CPhoneQwertyHandler* iQwertyHandler;
        
        /**
         * Time of the previous key press. Used for multitap behaviour.
         */
        TTime iPreviousKeyPressTime;
        
        /**
         * Keycode of the previous keypress. Used for multitap behaviour.
         */
        TInt iPreviousScanCode;
        
        /**
         * Points to next character in multitap character list.
         */
        TInt iMultitapIndex;
        
        /**
         * True if there has been a key event but not a closing key up event.
         * Needed to detect situations where key up event is handled before key event.
         */
        TBool iExpectKeyUpEvent;
    };

#endif      // CPHONEKEYEVENTFORWARDER_H
            
// End of File
