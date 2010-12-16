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
* Description:  AppUI for PhoneUI
*
*/


#ifndef CPHONEAPPUI_H
#define CPHONEAPPUI_H

//  INCLUDES
#include <aknappui.h>
#include <eikbtgpc.h>
#include <eikmenub.h> 
#include "mphonepubsubobserver.h"
#include "cphonerecoverysystem.h"

// FORWARD DECLARATIONS
class CPhoneViewController;
class CPhoneUIController;
class CPhoneQwertyHandler;
class CEnvironmentChangeNotifier;

// CLASS DECLARATION

/**
*  AppUI for the PhoneUI.
*
*  @since 4.0
*
*/
class CPhoneAppUI :
    public CAknAppUi,
    public MCoeViewActivationObserver,
    public MAknAliasKeyCodeResolver,
    private MPhonePubSubObserver 
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPhoneAppUI();

        /**
        * Destructor.
        */
        ~CPhoneAppUI();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

    public: // Functions from base classes

        /**
        * From CAknAppUi, handles window server events.
        * @param aEvent event.
        * @param aDestination destination control.
        */
        virtual void HandleWsEventL( 
            const TWsEvent& aEvent,
            CCoeControl* aDestination );

        /**
        * From CAknAppUi, handle key events.
        * @param aKeyEvent a key event
        * @param aType a key event type
        */
        TKeyResponse HandleKeyEventL( 
            const TKeyEvent& aKeyEvent, 
            TEventCode aType );

        /**
        * From CAknAppUi, handles command.
        *
        * This method is called first when command is to be performed. Then
        * control flow goes to HandleCommandL.
        *
        * @param aCommand command to be handled.
        */
        void ProcessCommandL( TInt aCommand );

        /**
        * From CAknAppUi, handles commands.
        *
        * @param aCommand It is the code of the command to be handled.
        */
        void HandleCommandL( TInt aCommand );

        /**
        * From CAknAppUi, initialise a menupane (dynamic).
        *
        * @param aResourceId It is the resource id for the pane.
        * @param aMenuPane It is the menu pane corresponding to the resource.
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

        /**
        * From CAknAppUi, initialise a menubar (dynamic).
        *
        * @param aResourceId It is the resource id for the bar.
        * @param aMenuBar It is the menu bar corresponding to the resource.
        */
        void DynInitMenuBarL( TInt aResourceId, CEikMenuBar* aMenuBar );

        /**
        * From CAknAppUi, indicates when app ui is on the foreground.
        * @param aForeground It is true if app is on the foreground.
        */
        void HandleForegroundEventL( TBool aForeground );

        /**
        * From CAknAppUi, handles status pane size change.
        */
        void HandleStatusPaneSizeChange();

        /**
        * From CAknAppUi. For idle state monitor
        */
        void HandleApplicationSpecificEventL( 
            TInt aType,
            const TWsEvent& aEvent );

        /**
        * From CAknAppUi, handles resouce changes.
        */
        void HandleResourceChangeL( TInt aType );

        /**
        * From MCoeViewActivationObserver
        */
        void HandleViewActivation(
            const TVwsViewId& aNewlyActivatedViewId,
            const TVwsViewId& aViewIdToBeDeactivated );

        /**
        * From CAknAppUi
        * Handles view deactivation.
        * @param aViewIdToBeDeactivated:Id of view to be deactivated.
        * @param aNewlyActivatedViewId: Id of view to be activated.        
        */
        void HandleViewDeactivation(
            const TVwsViewId& aViewIdToBeDeactivated,
            const TVwsViewId& aNewlyActivatedViewId );

        /** 
        * From CCoeAppUi
        * Returns help context
        */
        virtual CArrayFix<TCoeHelpContext>* HelpContextL() const; 

        // STARTUP SIGNALLING

        /**
        * Performs startup signalling. 
        *
        * Calls either idle or security signalling.
        */
        static TInt DoStartupSignalL( TAny* aAny );

        /**
        * Idle signalling.
        */
        TInt DoStartupSignalIdleL();

        /**
        * Security signalling. 
        */
        TInt DoStartupSignalSecurityL();

    public: // From MPhonePubSubObserver

        /**
        * This function is called when there is property value change.
        * @param aCategory Category of the property
        * @param aKey Property key that is changed
        * @param aValue New property value
        */
        IMPORT_C virtual void HandlePropertyChangedL( 
            const TUid& aCategory,
            const TUint aKey,
            const TInt aValue );

	public: // From MAknAliasKeyCodeResolver
	
		/**
		* @see MAknAliasKeyCodeResolver
		*/        
		TInt GetAliasKeyCode( TUint& aCode, 
							  const TKeyEvent& aKeyEvent,
							  TEventCode aType);  
		
	private:

        /**
        * Handle enviroment changes
        */
        void HandleEnvironmentChangeL( const TInt aChanges ); 
        
        /**
        * Process Window server events.
        */
        void ProcessWsEventL( 
            const TWsEvent& aEvent, 
            CCoeControl* aDestination );
            
        void LayoutMainView();
        
        /**
        * Called in callback
        */
        static TInt HandleCallBackL( TAny* aAny );

        /**
        * Callback pointed to specific instance
        */
        void DoHandleCallBackL();
        
        /**
        * NoKey specil handling
        */
        void NoKeySpecialHandlingL( 
                    const TKeyEvent& aKeyEvent,
					TEventCode aType);  

    private: // Data types

        // Startup states.
        // EPhoneStartupInitial - Initial state.
        // EPhoneStartedUp - Phone application is ready.
        // EPhoneIdleStartedUp - Idle application is ready.
        enum
            {
            EPhoneStartupInitial = 0x00000001,
            EPhoneStartedUp = 0x00000002,
            EPhoneIdleStartedUp = 0x00000004
            };

    private: // Data

        CPhoneViewController *iPhoneViewController;
        
        // Phone UI controller
        CPhoneUIController* iPhoneUIController;    

        //      Owned: Eikon server window group.
        TInt iEikonServerWgId;

        // STARTUP SIGNALLING

        // Application (Phone and Idle) startup status.
        TInt iAppsReady;
        
        //      Recovery id for startup signalling.
        TRecoveryId iStartupSignalRecoveryId;

        //      Boolean flag. ETrue iff we have reached light idle
        TBool iLightIdleReached;

        // Id of system event client - Idle started up.
        TInt iIdleReadyEventId;
        
        // Id of system event client - topmost application.
        TInt iTopmostAppId;

        // Id of system events from Starter states.
        TInt iStarterEventId;
        
        // System evenviroment notifier
        CEnvironmentChangeNotifier* iEnvChangeNotifier;
    };

#endif      // CPHONEAPPUI_H   
            
// End of File
