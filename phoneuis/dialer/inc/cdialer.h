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
* Description:  Definition of CDialer class.
*
*/



#ifndef CDIALER_H
#define CDIALER_H

//  INCLUDES
#include <coecntrl.h>
#include <coemain.h>

#include "mnumberentry.h"
#include "mphoneqwertymodeobserver.h"

// CONSTANTS

// FORWARD DECLARATIONS
class CEikButtonGroupContainer;
class CDialerKeyPadContainer;
class CDialerNumberEntry;
class MNumberEntry;
class CEikMenuPane;
class CEikonEnv;
class CAknEdwinState;
class MNumberEntryObserver;
class CDialingExtensionInterface;
class CDialingExtensionObserver;
class CDialerToolbarContainer;
class MPhoneDialerController;

// CLASS DECLARATION

/**
*  CDialer class.
*
*  @lib dialer.lib
*  @since Series60_5.0
*
*  Description:
*   Dialer is used in two applications, telephony and video telephony.
*
*   In Telephony dialer handles number (number/sip address) input from 
*   user via touch screen, VKB or hw keyboard (ITU-T and QWERTY).
*
*   From host application dialer requires:
*   - View/container as CCoeControl
*
* 
*  Usage:
*

    // Hide status pane.
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        static_cast<CAknAppUi*>( CEikonEnv::Static()->EikAppUi())->
            StatusPane()->MakeVisible( ETrue);
        }
    else
        {
        static_cast<CAknAppUi*>( CEikonEnv::Static()->EikAppUi())->
            StatusPane()->MakeVisible( EFalse );
        }

    // Calculate dialer area:
    TRect screenRect( 
        TPoint( 0, 0 ), 
        iCoeEnv->ScreenDevice()->SizeInPixels() );   

    TAknLayoutRect appRect;
    appRect.LayoutRect( 
        screenRect, AknLayout::application_window( screenRect );

    TAknLayoutRect mainRect;
    mainRect.LayoutRect( 
        appRect.Rect(), 
        AknLayout::main_pane( appRect.Rect(), 0, 1, 0 ) );

    TRect mainPane( mainRect.Rect() );

    // Reduce the space that CBA need from the rect
    CEikButtonGroupContainer cba = iEikEnv.AppUiFactory()->Cba();
    TRect mainPane( aRect );    
    cba->ReduceRect( mainPane );

    // Create dialer from container (CCoeControl) that created
    // dialer window (CreateWindowL).


*  In telephony dialer:

    // Create dialer.
    CDialer* dialer = CDialer::NewL( *this, aDialerArea );

    // Open dialer
    // Open and show dialer
    iDialer->CreateNumberEntry();

    // methods in MNumberEntry are available to contol dialer and
    // get/set text in number entry.
    
    // Close dialer
    iDialer->RemoveNumberEntry( );
    // Delete dialer
    delete dialer;
*
*/
NONSHARABLE_CLASS(CDialer) : 
	public CCoeControl, 
	public MNumberEntry, 
	public MCoeControlObserver,
	public MPhoneQwertyModeObserver
    {
    public:  // Constructors and destructor

        /**
        * Two phase constructor
        * @param aContainer Parent container
        * @param aRect Area to use for dialer
        * @param aController Dialer controller providing state data for dialer
        * @return New instance of Dialer
        */
        IMPORT_C static CDialer* NewL( const CCoeControl& aContainer,
                                       const TRect& aRect,
                                       MPhoneDialerController* aController );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CDialer();

    public: // New functions

        /**
        * Return MNumberEntry API of Dialer
        * @return Pointer to MNumberEntry implementation
        */
        IMPORT_C MNumberEntry* NumberEntry();

        /**
        * Set number entry observer.
        */
        IMPORT_C void SetNumberEntryObserver( MNumberEntryObserver& aObserver );

        /**
        * Sets controller of the dialer. Controller is responsible of providing
        * toolbar functions, text prompt, etc, as appropriate for the curret dialer state.
        */
        IMPORT_C void SetControllerL( MPhoneDialerController* aController );

        /**
        * Gets current controller of the dialer.
        */
        IMPORT_C MPhoneDialerController* Controller();
        
        /**
         * Returns pointer to easydialing interface. This can be NULL if
         * easydialing is not present in device, so value must be checked
         * before using the interface.
         * 
         * @return  Pointer to easydialing interface
         */
        IMPORT_C CDialingExtensionInterface* GetEasyDialingInterface() const;
        
        /**
         * Toolbar of the easydialing layout is owned by the Dialer.
         * Calling this methods updates status of its buttons.
         */
        IMPORT_C void UpdateToolbar();
        
        /**
         * Updates number entry editor to correct state. State depends on
         * qwerty availability and easydialing and voip settings. 
         */
        void UpdateNumberEntryConfiguration();
        
		
		/**
		 * Relayout and draw control. Also updates toolbar.
		 */
		IMPORT_C void RelayoutAndDraw();

    public: // from MNumberEntry 

        /**
        * Sets number entry active.
        */
        void CreateNumberEntry();
        
        /**
        * Return pointer to number entry
        * @return The NE.
        */
        CCoeControl* GetNumberEntry( ) const;
        
        /**
        * Query, is number entry used.
        * @return ETrue is NE is used.
        */
        TBool IsNumberEntryUsed( ) const;
        
        /**
        * This disables NE drawing but doesn't empty the component.
        * @param aVisibility NE visible or not.
        */
        void SetNumberEntryVisible( 
            const TBool& aVisibility = ETrue );
        
        /**
        * Set Number Entry text.
        * @param aDesC The text.
        */
        void SetTextToNumberEntry( const TDesC& aDesC );
        
        /**
        * Get Number Entry Text.
        * @param aDes The text.
        */
        void GetTextFromNumberEntry( TDes& aDes );
        
        /**
        * Removes NE and empties the buffer.
        */
        void RemoveNumberEntry( );
        
        /**
        * Set number entry's editor mode.
        * @param aMode for number editor input mode.
        * @return Input mode of the editor.
        */
        TInt ChangeEditorMode( TBool aDefaultMode = EFalse );
        
        /**
        * Open VKB
        */
        void OpenVkbL();
        
        /**
        * Get number entry's editor mode.
        * @return Input mode of the editor.
        */
        TInt GetEditorMode() const;

        /**
        * Resets number entry editor to default values.
        */
        void ResetEditorToDefaultValues();
        
        /**
        * Set number entry's prompt text.
        * @param aPromptText.
        */        
        void SetNumberEntryPromptText( const TDesC& aPromptText );
        
        /**
        * Enable or disable tactile feedback. 
        * @param aEnable.
        */             
        void EnableTactileFeedback( const TBool aEnable );

        /**
        * Handle Qwerty mode change.
        * @param aMode 0 = off, 1 = on
        */
        IMPORT_C void HandleQwertyModeChange( TInt aMode );

        /**
        * @see MIdleQwertyModeObserver.
        */
        IMPORT_C void HandleKeyboardLayoutChange();

    private: // Functions from MCoeControlObserver
        
        void HandleControlEventL( CCoeControl* aControl, TCoeEvent aEventType );
        
    private:  // Functions from CCoeControl
        
        /**
        * @see CCoeControl
        */
        void SizeChanged();
        
        /**
        * @see CCoeControl
        */
        void PositionChanged();

        /**
        * @see CCoeControl
        */
        TInt CountComponentControls() const;
        
        /**
        * @see CCoeControl
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
        /**
        * @see CCoeControl
        */
        void Draw( const TRect& aRect ) const;
        
        /**
        * @see CCoeControl
        */
        void FocusChanged( TDrawNow aDrawNow );
                
        /**
        * @see CCoeControl
        */        
        void MakeVisible( TBool aVisible );  
        
        /**
        * @see CCoeControl
        */ 
        void PrepareForFocusGainL();
        
        /**
        * @see CCoeControl
        */
        void HandleResourceChange( TInt aType );
        
        /**
        * @see CCoeControl
        */
        void HandlePointerEventL( const TPointerEvent& aPointerEvent );
              
        
    private:

        /**
        * Constructor
        */
        CDialer();

        /**
        * 2nd phase constructor.
        * @param aContainer Parent container
        * @param aRect Area to use for dialer
        * @param aController Dialer controller providing state data for dialer 
        */
        void ConstructL( const CCoeControl& aContainer, 
            const TRect& aRect,
            MPhoneDialerController* aController );
        
        /**
        * Load resource file.
        */
        void LoadResourceL();
        
        /**
        * Unload resource file.
        */
        void UnLoadResources();
        
        /**
        * @see CCoeControl::ComponentControl()
        * @param aIndex control index
        */
        CCoeControl* ComponentControlForDialerMode( const TInt aIndex ) const;
        
        /**
         * Returns edwin state
         * @return Pointer to CAknEdwinState
         */
        CAknEdwinState* EdwinState() const;
        
        /** Editor types. */
        enum TEditorType
            {
            ENumericEditor,
            EAlphanumericEditor,
            EVirtualKeyboardEditor
            };
                
        /**
         * Updates editor flags for virtual
         * keyboard.
         */
        void UpdateEdwinState( TEditorType aType );

        /**
         * Loads easydialing plugin. If loading fails (for instance when
         * easydialing feature flag is not enabled in the device),
         * iEasyDialer will be NULL after this function returns.
         */
        void LoadEasyDialingPlugin();
        
        /**
         * Checks if Easy dialing is available, allowed, and currently enabled.
         * 
         * @return  ETrue if Easy Dialing is enabked 
         */
        TBool EasyDialingEnabled() const;
        
        /**
         * Layout number entry component. The used layout depends on
         * the availability and state of Easy dialing. 
         */
        void LayoutNumberEntry( const TRect& aParent, TInt aVariety );
        
    private:    // Data
          
        // Keypad container  - owned
        CDialerKeyPadContainer* iKeypadArea;
             
        // Number entry container - owned
        CDialerNumberEntry* iNumberEntry;
        
        // Is Number entry (e.g. dialer) being used at the moment
        TBool iIsUsed;
        
        // Resource 
        TInt iResourceOffset;
        
        // Is virtual key board open
        TBool iVirtualKeyBoardOpen;

        /**
         * Easydialing plugin. This can be NULL if easydialing feature flag
         * is not enabled in the device.
         * Own.
         */
        CDialingExtensionInterface* iEasyDialer;
        
        /**
         * Observer for Easydialing.
         * Own.
         */
        CDialingExtensionObserver* iDialingExtensionObserver;
        
        /**
         * Toolbar used in Easy dialing layout.
         * Own.
         */
        CDialerToolbarContainer* iToolbar;
        
        /**
         * Controller rules the mode of the Dialer (e.g. normal, DTMF, restricted...)
         * Not owned.
         */
        MPhoneDialerController* iController;
        
        /*
         * Is qwerty mode on.
         */
        TBool iQwertyMode;
        
        /**
         * Current type of the editor field. The field is numeric unless
         * the device is in QWERTY mode and there is some consumer (like VoIP, EasyDialing)
         * for the alphabetic characters.
         */
        TEditorType iEditorType;
    };

#endif      // CDIALER_H

// End of File
