/*
* Copyright (c) 2007 - 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CDialerKeyPadContainer class provides number keypad fuctionality 
*
*/



#ifndef CDIALERKEYPADCONTAINER_H
#define CDIALERKEYPADCONTAINER_H

//  INCLUDES
#include    <coecntrl.h>
#include    <coecobs.h>    // MCoeControlObserver
#include    <coemain.h>

#include    "cdialercontainerbase.h"
#include    "cdialerkeypadlabelmanager.h"

// CONSTANTS

// FORWARD DECLARATIONS
class CDialerKeyPadButton;


// CLASS DECLARATION
 
/**
*  CDialerKeyPadContainer container class
*
*  @lib dialer.lib
*  @since S60 v5.0
*/
NONSHARABLE_CLASS(CDialerKeyPadContainer) : public CDialerContainerBase, 
                                            public MCoeForegroundObserver,
                                            public MDialerKeyPadLabelManagerCallback
    {
    public:  // Constructors and destructor
        
        /**
        * Two phase constructor
        * @param aContainer Parent container
        * @param aOperatingMode 
        * @return New instance
        */
        static CDialerKeyPadContainer* NewL( 
            const CCoeControl& aContainer,
            TDialerOperationMode aOperatingMode
             );
        
        /**
        * Destructor.
        */
        virtual ~CDialerKeyPadContainer();      
        
    public:
        
        /**
        * Enables or disables tactile feedback audio for keypad buttons.
        */
        
        void EnableTactileFeedback( const TBool aEnable );
        
        // moved from private to public
        /**
        * @see CCoeControl
        */        
        void MakeVisible( TBool aVisible );
        
        /**
         * Sets the operation mode. New mode takes effect once the
         * size of the component is reset.
         * @param   aOperatingMode  New operation mode.
         */
        void SetOperationMode( TDialerOperationMode aOperatingMode );
        
    private:  // Functions from base classes
        
        /**
        * From CCoeControl
        */
        TInt CountComponentControls() const;
        
        /**
        * From CCoeControl
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
        /**
         * From CoeControl.
         */
        void HandlePointerEventL( const TPointerEvent& aPointerEvent );
        
        /**
        * @see CCoeControl
        */
        void HandleResourceChange( TInt aType );   
        
        /**
        * @see MCoeControlObserver
        */        
        void HandleControlEventL( CCoeControl* aControl,TCoeEvent aEventType );      
        
        /**
        * @see MCoeForegroundObserver
        */ 
        void HandleGainingForeground();
        
        /**
        * @see MCoeForegroundObserver
        */         
        void HandleLosingForeground();      
        
        /**
         * @see MDialerKeyPadLabelManagerCallback
         */
        void KeyLabelsChanged();
        
    private: // From CDialerContainerBase
    
        /**
        * @see CDialerContainerBase
        */
        void SetVariety();
        
        /**
        * @see CDialerContainerBase
        */
        void SetLayout();

    private:
    
		CDialerKeyPadContainer( const CCoeControl& aContainer, 
            TDialerOperationMode aOperatingMode );
            	
        /**
        * 2nd phase constructor.
        */
        void ConstructL( );

        /**
        * Set layout in Phone Dialer mode
        */
        void SetPhoneLayout();

        /**
         * Set layout in Easy Dialing mode
         */
        void SetEasyDialingLayout();
        
        /**
        * Set layout in Video DTMF mode
        */
        void SetVideoLayout();
        
        /**
        * Create keypad buttons.
        */
        void CreateButtonsL();
        
    private:    // Data
    
        TDialerOperationMode iOperatingMode;
        
        /**
         * Keypad buttons.
         * Owned.
         */
        RPointerArray<CDialerKeyPadButton> iButtons;
        
        /**
         * True, when EEventStateChanged event reported. 
         */
        TBool iButtonPressedDown;
        
        /**
         * Stores last pointer event. 
         */
        TPointerEvent iPointerEvent;
        
        /**
         * Helper to manage keypad button labels
         */
        CDialerKeyPadLabelManager* iKeyLabelManager;
        
        /**
         * True if key up event has been simulated in order to cancel
         * long tap action due to dragging pointer outside of
         * pressed button.
         */
        TBool iKeyUpSimulatedDueToDragging;
    };

#endif      // CDIALERKEYPADCONTAINER_H

// End of File
