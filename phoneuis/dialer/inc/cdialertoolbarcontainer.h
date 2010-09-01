/*
* Copyright (c) 2009, 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CDialerToolbarContainer replaces the standard S60 toolbar
*               in the Dialer layout in the Easy dialing mode. It's 
*               responsible of the toolbar layout and logic, but not of the 
*               contents which are provided by an instance of 
*               MPhoneDialerController.
*
*/

#ifndef CDIALERTOOLBAR_H_
#define CDIALERTOOLBAR_H_

// INCLUDES
#include <mphonedialercontroller.h>
#include "cdialercontainerbase.h"

// FORWARD DECLARATIONS
class CAknButton;


// CLASS DECLARATION
NONSHARABLE_CLASS(CDialerToolbarContainer) : 
    public CDialerContainerBase, 
    private MCoeForegroundObserver
    {
public: // constructor and destructor
    static CDialerToolbarContainer* NewL( 
            CCoeControl& aParent, 
            MPhoneDialerController* aContentProvider );
    virtual ~CDialerToolbarContainer();

private: // constructors
    CDialerToolbarContainer( 
            CCoeControl& aParent,
            MPhoneDialerController* aContentProvider );
    void ConstructL();

public: // new methods
    /**
     * Set provider of toolbar button functions
     */
    void SetContentProviderL( MPhoneDialerController* aProvider );
    
    /**
     * Update states of all toolbar buttons.
     */
    void UpdateButtonStates();

protected: // from CDialerContainerBase
    virtual void SetVariety();
    virtual void SetLayout();
    
protected: // from CCoeControl
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl( TInt aIndex ) const;
    void HandlePointerEventL( const TPointerEvent& aPointerEvent );

private: // from MCoeControlObserver
    /**
     * Callback for handling button presses.
     */
    void HandleControlEventL( CCoeControl* aControl, TCoeEvent aEventType );

private: // from MCoeForegroundObserver
    void HandleGainingForeground();
    void HandleLosingForeground();
    
private: // new methods
    
    /**
     * Create toolbar button instances and fill them with states
     * according the data given by the content provider.
     */
    void CreateButtonsL();
    
    /**
     * Delete all buttons and free the memory reserved for button data.
     */
    void DeleteButtons();
    
    /**
     * Create and add new button state.
     */
    void AddButtonStateL( TInt aButtonIndex, MPhoneDialerController::CButtonData& aButtonData );
    
    /**
     * Get command ID for the given button in it's current state.
     */
    TInt ButtonCommand( const CAknButton& aButton ) const;
    
    /**
     * Set layout for one button.
     * @param   aButtonIdx  Index of the button to be laid out.
     * @param   aPos        Position (top-left corned) where button is to be placed.
     * @return  Position of the next button.
     */
    TPoint LayoutButton( TInt aButtonIdx, TPoint aPos );
    
    /**
     * Simulate backspace press including key down event, key event, and key up event
     */
    void SimulateBackspace();
    
private: // data
    
    /** Provides toolbar button data. Not owned. */
    MPhoneDialerController* iContentProvider;
    
    /** Button objects. Owned. */
    RPointerArray< CAknButton > iButtons;
    
    /** 
     * Maps buttons states to command IDs. 
     * First index is the button index, second index is the state index.
     */
    RArray< RArray<TInt> > iButtonCommands;
        
    /** The last received pointer event. */
    TPointerEvent iLatestPointerEvent;
    };

#endif /* CDIALERTOOLBAR_H_ */
