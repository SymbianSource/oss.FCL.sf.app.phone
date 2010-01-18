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
                                            public MCoeForegroundObserver
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
        * @see CCoeControl
        */        
        void MakeVisible( TBool aVisible );     
        
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
         * Number of buttons.
         */
        TInt iButtonCount;
         
        /**
         * True, when EEventStateChanged event reported. 
         */        
        TBool iButtonPressedDown;
        
        /**
         * Stores last pointer event. 
         */        
        TPointerEvent iPointerEvent;
    };

#endif      // CDIALERKEYPADCONTAINER_H

// End of File
