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

#include <aknbutton.h>
#include <aknappui.h>
#include <AknsUtils.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>
#include <phoneui.rsg>
#include <phoneappcommands.hrh>
#include <StringLoader.h>
#include <data_caging_path_literals.hrh>

#include "dialercommon.h"
#include "cdialertoolbarcontainer.h"


// CONSTANTS
const TInt KDialerToolbarButtonKeyRepeatDelay = 300;
const TInt KDialerToolbarKeyRepeatInterval = 75;



// CLASS DEFINITION

// ---------------------------------------------------------------------------
// NewL
//
// ---------------------------------------------------------------------------
//
CDialerToolbarContainer* CDialerToolbarContainer::NewL( CCoeControl& aParent,
        MPhoneDialerController* aContentProvider )
    {
    CDialerToolbarContainer* self = 
            new (ELeave) CDialerToolbarContainer( aParent, aContentProvider );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CDialerToolbarContainer
//
// ---------------------------------------------------------------------------
//
CDialerToolbarContainer::CDialerToolbarContainer( CCoeControl& aParent,
        MPhoneDialerController* aContentProvider ) : 
    CDialerContainerBase( aParent ), 
    iContentProvider( aContentProvider )
    {
    }

// ---------------------------------------------------------------------------
// ~CDialerToolbarContainer
//
// ---------------------------------------------------------------------------
//
CDialerToolbarContainer::~CDialerToolbarContainer()
    {
    iCoeEnv->RemoveForegroundObserver( *this );
    DeleteButtons();
    }

// ---------------------------------------------------------------------------
// ConstructL
//
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::ConstructL()
    {
    BaseConstructL();
    
    CreateButtonsL();

    iCoeEnv->AddForegroundObserverL( *this );
    }

// ---------------------------------------------------------------------------
// SetContentProviderL
//
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::SetContentProviderL( MPhoneDialerController* aProvider )
    {
    if ( iContentProvider != aProvider )
        {
        iContentProvider = aProvider;
        
        // Recreate buttons to reset available button states
        DeleteButtons();
        CreateButtonsL();
        }
    }

// ---------------------------------------------------------------------------
// UpdateButtonStates
//
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::UpdateButtonStates()
    {
    // safety check
    if ( iButtons.Count() < MPhoneDialerController::ENumberOfButtons ||
         !iContentProvider )
        {
        return;
        }
    
    for ( TInt i = 0 ; i < MPhoneDialerController::ENumberOfButtons ; ++i )
        {
        MPhoneDialerController::TButtonIndex idx =
            static_cast< MPhoneDialerController::TButtonIndex >( i );
        TInt state = iContentProvider->ButtonState( idx );
        TBool dimmed = iContentProvider->ButtonDimmed( idx );
        if ( state >= 0 )
            {
            iButtons[i]->SetCurrentState( state, EFalse );
            }
        iButtons[i]->SetDimmed( dimmed );
        }
    }

// ---------------------------------------------------------------------------
// SetVariety
//
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::SetVariety()
    {
    iVariety = ( Layout_Meta_Data::IsLandscapeOrientation() ?
        EDialerVarietyLandscape : EDialerVarietyPortrait );
    }

// ---------------------------------------------------------------------------
// SetLayout
//
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::SetLayout()
    {
    if ( iButtons.Count() < MPhoneDialerController::ENumberOfButtons )
        {
        return;
        }
    
    // Position for the first button
    TPoint buttonPos = Rect().iTl;
    
    if ( iVariety == EDialerVarietyPortrait )
        {
        // In portrait, buttons are laid out from top to bottom
        for ( TInt i = 0 ; i < iButtons.Count() ; ++i )
            {
            buttonPos = LayoutButton( i, buttonPos );
            }
        }
    else
        {
        // In landscape, buttons are laid out from right to left.
        // This is achieved by looping button array backwards and laying
        // those out from left to right.
        for ( TInt i = iButtons.Count()-1 ; i >= 0  ; --i )
            {
            buttonPos = LayoutButton( i, buttonPos );
            }
        }
    }

// ---------------------------------------------------------------------------
// CountComponentControls
//
// ---------------------------------------------------------------------------
//
TInt CDialerToolbarContainer::CountComponentControls() const
    {
    return iButtons.Count();
    }

// ---------------------------------------------------------------------------
// ComponentControl
//
// ---------------------------------------------------------------------------
//
CCoeControl* CDialerToolbarContainer::ComponentControl( TInt aIndex ) const
    {
    CCoeControl* control = NULL;
    if ( aIndex >= 0 && aIndex < iButtons.Count() )
        {
        control = iButtons[ aIndex ];
        }
    return control;
    }

// ---------------------------------------------------------------------------
// HandleControlEventL
//
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::HandleControlEventL( CCoeControl* aControl, TCoeEvent aEventType )
    {
    if ( aEventType == MCoeControlObserver::EEventStateChanged )
        {
        // Handle button press. 

        // Multistate buttons change state automatically when pressed. 
        // Revert this change as we do not want to use states
        // for "toggle button" type of functionality but to give one button
        // different functions depending on the situation.
        // Must initiate button redraw so that the state change done here 
        // is drawn too; the automatically changed state is drawn within 
        // AknButton just before EEventStateChanged is reported 
        // (in emulator button icon flashing can be seen).
        UpdateButtonStates();
        aControl->DrawNow();

        // Handle the command attached to the button
        CAknButton* button = static_cast<CAknButton*>( aControl );
        TInt commandId = ButtonCommand( *button );
        
        if ( commandId == EPhoneDialerCmdClear )
            {
            // simulate backspace
            TKeyEvent keyEvent;
            keyEvent.iModifiers = 0;
            keyEvent.iRepeats = 0;
            keyEvent.iCode = EKeyBackspace;
            keyEvent.iScanCode = EStdKeyBackspace;
            iCoeEnv->SimulateKeyEventL( keyEvent, EEventKey );
            }
        else
            {
            // Forward pressed toolbar button to the framework 
            // so that we can handle it in active state
            CAknAppUi* appUi = static_cast<CAknAppUi*>( iCoeEnv->AppUi() );
            appUi->ProcessCommandL( commandId );  
            }
        }
    }

// ---------------------------------------------------------------------------
// HandlePointerEventL
//
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    iLatestPointerEvent = aPointerEvent;
    CCoeControl::HandlePointerEventL( aPointerEvent );
    }

// ---------------------------------------------------------------------------
// HandleGainingForeground
//
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::HandleGainingForeground()
    {
    // no implementation needed
    }
    
// ---------------------------------------------------------------------------
// HandleLosingForeground
//
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::HandleLosingForeground()
    {
    // This is needed when dialer control disappeares during keypress. 
    // Last pressed pointer event must be forwarted to framework 
    // as pointer up event. Thus button is set to unpressed state etc. 
    // 
    if ( iLatestPointerEvent.iType != TPointerEvent::EButton1Up )
        {    
        iLatestPointerEvent.iType = TPointerEvent::EButton1Up;
        TRAP_IGNORE( HandlePointerEventL( iLatestPointerEvent ) );
        }    
    }

// ---------------------------------------------------------------------------
// CreateButtonsL
//
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::CreateButtonsL()
    {
    // Create the avkon button objects
    for ( TInt i = 0 ; i < MPhoneDialerController::ENumberOfButtons ; ++i )
        {
        CAknButton* button = CAknButton::NewLC();
        button->SetContainerWindowL( *this ); // this sets also parent and mopParent
        button->SetObserver( this );
        button->SetIconScaleMode( EAspectRatioPreserved );
        AknsUtils::RegisterControlPosition( button );
        button->ActivateL();
        iButtons.AppendL( button );
        CleanupStack::Pop( button );
        iButtonCommands.AppendL( RArray<TInt>() );
        }

    // Backspace button has auto-repeat feature
    iButtons[MPhoneDialerController::EClearButton]->SetButtonFlags( KAknButtonKeyRepeat );
    iButtons[MPhoneDialerController::EClearButton]->SetKeyRepeatInterval( 
        KDialerToolbarButtonKeyRepeatDelay, KDialerToolbarKeyRepeatInterval );
    
    // Call button doesn't use default button background but a green one 
    // (same as used by in-call view for answer button)
    iButtons[MPhoneDialerController::ECallButton]->SetFrameAndCenterIds( 
            KAknsIIDQsnFrButtonNormalAnswer, KAknsIIDQsnFrButtonNormalAnswerCenter,
            KAknsIIDQsnFrButtonPressedAnswer, KAknsIIDQsnFrButtonPressedAnswerCenter, 
            KAknsIIDQsnFrButtonInactiveAnswer, KAknsIIDQsnFrButtonInactiveAnswerCenter, 
            KAknsIIDQsnFrButtonPressedAnswer, KAknsIIDQsnFrButtonPressedAnswerCenter, 
            KAknsIIDQsnFrButtonInactiveAnswer, KAknsIIDQsnFrButtonInactiveAnswerCenter
            );
        
    // Add button states
    if ( iContentProvider )
        {
        for ( TInt i = 0 ; i < MPhoneDialerController::ENumberOfButtons ; ++i )
            {
            MPhoneDialerController::TButtonIndex btnIdx =
                static_cast< MPhoneDialerController::TButtonIndex >( i );
            RPointerArray< MPhoneDialerController::CButtonData > states;
            CleanupClosePushL( states );
            iContentProvider->GetButtonData( btnIdx, states );
            for ( TInt j = 0 ; j < states.Count() ; ++j )
                {
                TRAP_IGNORE( AddButtonStateL( i, *states[j] ) );
                delete states[j];
                states[j] = NULL;
                }
            CleanupStack::PopAndDestroy( &states );
            }
        }
    }

// ---------------------------------------------------------------------------
// DeleteButtons
// Reset internal arrays holding the button data
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::DeleteButtons()
    {
    for ( TInt i = 0 ; i < iButtons.Count() ; ++i )
        {
        AknsUtils::DeregisterControlPosition( iButtons[i] );
        }
    iButtons.ResetAndDestroy();
    
    for ( TInt i = 0 ; i < iButtonCommands.Count() ; ++i )
        {
        iButtonCommands[i].Reset();
        }
    iButtonCommands.Reset();
    }

// ---------------------------------------------------------------------------
// AddButtonStateL
//
// ---------------------------------------------------------------------------
//
void CDialerToolbarContainer::AddButtonStateL( TInt aButtonIndex, 
        MPhoneDialerController::CButtonData& aButtonData )
    {
    iButtons[ aButtonIndex ]->AddStateL(
            aButtonData.iIcon,      //CGulIcon* aIcon, 
            NULL,                   //CGulIcon* aDimmedIcon, 
            NULL,                   //CGulIcon* aPressedIcon,
            NULL,                   //CGulIcon* aHoverIcon,
            KNullDesC,              //const TDesC& aText, 
            *aButtonData.iTooltip,  //const TDesC& aHelpText, 
            0 );                    //const TInt aStateFlags
    aButtonData.iIcon = NULL; // ownership transferred
    iButtonCommands[ aButtonIndex ].AppendL( aButtonData.iCommandId );
    }

// ---------------------------------------------------------------------------
// ButtonCommand
//
// ---------------------------------------------------------------------------
//
TInt CDialerToolbarContainer::ButtonCommand( const CAknButton& aButton ) const
    {
    TInt commandId = KErrNotFound;
    
    // Find out index of the button in iButtons array
    TInt btnIndex = KErrNotFound;
    for ( TInt i = 0 ; i < iButtons.Count() ; ++i )
        {
        if ( iButtons[i] == &aButton )
            {
            btnIndex = i;
            i = iButtons.Count(); // to break out of the loop
            }
        }
    __ASSERT_DEBUG( btnIndex >= 0, DialerPanic( EDialerPanicToolbarButtonNotFound ) );
    
    // Find the corresponding command from command array
    TInt btnStateIdx = aButton.StateIndex();
    if ( btnIndex < iButtonCommands.Count() &&
         btnStateIdx < iButtonCommands[btnIndex].Count() )
        {
        commandId = iButtonCommands[btnIndex][btnStateIdx];
        }
    
    __ASSERT_DEBUG( commandId != KErrNotFound, DialerPanic( EDialerPanicToolbarButtonNotFound ) );
    return commandId;
    }

// ---------------------------------------------------------------------------
// LayoutButton
// Set layout for one button.
// ---------------------------------------------------------------------------
//
TPoint CDialerToolbarContainer::LayoutButton( TInt aButtonIdx, TPoint aPos )
    {
    TRect parentRect = Rect();

    TAknWindowComponentLayout buttonLayout = 
        ( aButtonIdx == MPhoneDialerController::ECallButton ? 
          AknLayoutScalable_Apps::cell_dia3_key1_fun_pane( iVariety ) :
          AknLayoutScalable_Apps::cell_dia3_key0_fun_pane( iVariety ) );
        
    TAknLayoutRect layoutRect;
    layoutRect.LayoutRect( parentRect, buttonLayout );
    TRect outerRect = layoutRect.Rect();
    outerRect.SetRect( aPos, outerRect.Size() );

    TRect innerRect = outerRect;
    innerRect.Shrink( 1, 1 );
    iButtons[ aButtonIdx ]->SetRect( innerRect );
    AknsUtils::RegisterControlPosition( iButtons[ aButtonIdx ] );
    
    // Calculate position for the next button
    TPoint nextPos = aPos;
    if ( iVariety == EDialerVarietyPortrait )
        {
        nextPos.iY += outerRect.Height();
        }
    else
        {
        nextPos.iX += outerRect.Width();
        }
    
    return nextPos;
    }

// end of file
