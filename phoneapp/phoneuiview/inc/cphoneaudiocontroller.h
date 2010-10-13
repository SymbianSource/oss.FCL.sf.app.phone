/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  It manages incall indicator.
*
*/


#ifndef CPHONEAUDIOCONTROLLER_H
#define CPHONEAUDIOCONTROLLER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CPhoneStatusPane;
class TPhoneCommandParam;
class CPhoneTimer;

// CLASS DECLARATION

/**
*  It manages navi pane audio indicators
*
*  @since 1.0
*/
class CPhoneAudioController : public CBase, public MCoeControlObserver,
    public MCoeForegroundObserver
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        * @return new instance.
        */
        static CPhoneAudioController* NewL();
        
        /**
        * Destructor.
        */
        ~CPhoneAudioController();

    public: // New functions

        /**
        * Activates voice volume control.
        */
        void ActivateVolumeControlL();

        /**
        * Deactivate voice volume control.
        */
        void DeactivateVolumeControl();
        
        /**
        * Update control using the specified data
        * @param aCommandParam command parameter
        */
        void HandleVolumeChangeL( TPhoneCommandParam* aCommandParam );
        
        /**
        * Update control using the specified data
        * @param aCommandParam command parameter
        */
        void HandleIhfChange( TPhoneCommandParam* aCommandParam );
        
        /**
        * Update control using the specified data
        * @param aCommandParam command parameter
        */
        void HandleMuteChange( TPhoneCommandParam* aCommandParam );
        
        /**
        * Gets volume level from navi pane control.
        * @return Volume level.
        */
        TInt VolumeLevelFromControl();  
        
    private: // MCoeControlObserver
    
        void HandleControlEventL( CCoeControl* aControl, 
                                  TCoeEvent aEventType ); 
        
    private: // MCoeForegroundObserver
        
        /**
         * Handles the application coming to the foreground.
         */
        void HandleGainingForeground();
        
        /**
         * Handles the application going into the background.
         */
        void HandleLosingForeground();

    private:

        /**
        * C++ constructor.
        */
        CPhoneAudioController();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        /**
        * Update indicator.
        */
        void ActivateL();

        /**
        * Recovery system callback
        */
        static TInt DoRecoverActivateL( TAny* aAny );
        
        /**
        * Pushes new decorator to navi pane replacing the
        * previous one if needed.
        * @param aNew new to replace the old one.
        */
        void PushL(CAknNavigationDecorator& aNew);

        /**
        * Callback function. Called when navi pane should be updated 
        * back to default.
        * @param aAny pointer to instance of this class.
        * @return KErrNone always.
        */
        static TInt DoUpdateNaviPaneToDefault( TAny* aAny );

        /**
        * Get current volume decorator (Ihf or Ear) 
        * @return Current volume decorator
        */
        CAknNavigationDecorator& VolumeDecorator();
        
        /**
        * Get current volume control (Ihf or Ear) 
        * @return Current volume control (Ihf or Ear)         
        */
        CAknVolumeControl& VolumeControl();    
                
        /**
        * Get current volume level (Ihf or Ear)  
        * @return Current Ihf or Ear volume level         
        */        
        TInt VolumeLevel(); 
        
        /**
        * Select decorator based on current audio status
        * @return Selected decorator (Ihf, Ear or Muted).         
        */
        CAknNavigationDecorator& SelectDecoratorL();         

        /**
        * Set volume level for given volume control
        * @param aVolumeControl Volume control
        * @param aVolumeLevel New volume level
        */
        void SetVolumeLevel(CAknVolumeControl& aVolumeControl, TInt aVolumeLevel);
    
    private:    // Data

        // Recovery id for activating volume control
        TInt iActivateRecoveryId;    

        CPhoneStatusPane* iStatusPane;
        
        // Count the volume control activations
        TInt iVolumeControlCount;

        // Owned IHF volume control
        CAknNavigationDecorator* iIhfVolumeControl;

        // IHF volume, from 1 to 10
        TInt iIhfVolume;
        
        // ETrue IHF is active
        TBool iIhfMode;

        // ETrue if transfer to or from IHF pending
        TBool iIhfTransferPending; 
        
        // Owned non-IHF volume control.
        CAknNavigationDecorator* iEarVolumeControl;
        
        // Non-IHF volume, from 1 to 10
        TInt iEarVolume; 

        // Owned muted indicator
        CAknNavigationDecorator* iMutedControl;
        
        // ETrue phone is muted
        TBool iMuted;

        // ETrue if timed control transfer to muted is pending
        TBool iTimedMuteTransferPending; 

        // Pointer the current control that is in navi pane
        CAknNavigationDecorator* iOldControl;
        
        // Owned timer to update navi pane
        CPhoneTimer* iNaviPaneUpdateTimer;
        
        /**
         * ETrue if Telephony is the foreground application,
         * otherwise EFalse.
         */
        TBool iPhoneInForeground;

    };

#endif      // CPHONEAUDIOCONTROLLER_H
            
// End of File
