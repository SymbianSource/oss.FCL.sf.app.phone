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
* Description:  Shows number keypad and generates keypress event when 
*                 buttons are pressed.
*
*/


// INCLUDE FILES
#include <e32event.h>
#include <gulicon.h>
#include <eikapp.h>
#include <aknbutton.h>
#include <AknControl.h>
#include <AknsUtils.h>
#include <AknsSkinInstance.h>
#include <AknsDrawUtils.h>
#include <AknsConstants.h>
#include <AknLayout2Def.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>
#include <data_caging_path_literals.hrh> // for KDC_APP_RESOURCE_DIR
#include <touchfeedback.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <aknsframebackgroundcontrolcontext.h>

#include "cdialerkeypadcontainer.h"
#include "dialercommon.h"
#include "dialertrace.h"

#include "cdialerkeypadbutton.h"

// Number of buttons in this container
const TInt KNumberOfButtons = 12; 

struct TDialerButton
    {
    TMifDialer iBitmap;
    TMifDialer iBitmapMask;
    TInt iScanCode;
    TInt iKeyCode;
    };  

const TDialerButton KKeyPadButtons[ KNumberOfButtons ] =

    {
        { EMbmDialerQgn_indi_dialer_voicemail,
          EMbmDialerQgn_indi_dialer_voicemail_mask, 
          '1',
          '1'
        },
        { KDialerNoIcon, KDialerNoIcon, 
          '2', '2' },
        { KDialerNoIcon, KDialerNoIcon,  
          '3', '3' },
        { KDialerNoIcon, KDialerNoIcon,  
          '4', '4' },
        { KDialerNoIcon, KDialerNoIcon,  
          '5', '5' },
        { KDialerNoIcon, KDialerNoIcon,  
          '6', '6' },
        { KDialerNoIcon, KDialerNoIcon, 
          '7', '7' },
        { KDialerNoIcon, KDialerNoIcon,  
          '8', '8' },
        { KDialerNoIcon, KDialerNoIcon,  
          '9', '9' },
        { KDialerNoIcon, KDialerNoIcon,  
          EStdKeyNkpAsterisk, '*'  },
        { KDialerNoIcon, KDialerNoIcon,  
          '0', '0' },
        { KDialerNoIcon, KDialerNoIcon,      
          EStdKeyHash, '#' }
    };



// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
//
CDialerKeyPadContainer* CDialerKeyPadContainer::NewL( 
    const CCoeControl& aContainer,
    TDialerOperationMode aOperatingMode
     )
    {
    CDialerKeyPadContainer* self = 
        new( ELeave )CDialerKeyPadContainer ( aContainer, aOperatingMode );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// ---------------------------------------------------------------------------
// Symbian OS two phased constructor
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::ConstructL()
    {    
    DIALER_PRINT("KeyPadContainer::ConstructL<");
    BaseConstructL();
    iKeyLabelManager = CDialerKeyPadLabelManager::NewL( *iCoeEnv, *this );
    CreateButtonsL();
    iCoeEnv->AddForegroundObserverL( *this );
    DIALER_PRINT("KeyPadContainer::ConstructL>");
    }

// Constructor
CDialerKeyPadContainer::CDialerKeyPadContainer( 
    const CCoeControl& aContainer, 
    TDialerOperationMode aOperatingMode )
    : CDialerContainerBase ( 
        const_cast<CCoeControl&>(aContainer) ),
      iOperatingMode( aOperatingMode ),
      iButtonCount( KNumberOfButtons )
    {
    }
    
// Destructor
CDialerKeyPadContainer::~CDialerKeyPadContainer()
    {
    for ( TInt i = 0; i < iButtonCount; i++ )
        {   
        delete iButtons[i];
        }
    iButtons.Close();
    iCoeEnv->RemoveForegroundObserver( *this );
    delete iKeyLabelManager;
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::CountComponentControls
//
// ---------------------------------------------------------------------------
//
TInt CDialerKeyPadContainer::CountComponentControls() const
    {
    return KNumberOfButtons;
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::ComponentControl
//
// ---------------------------------------------------------------------------
//
CCoeControl* CDialerKeyPadContainer::ComponentControl( TInt aIndex ) const
    {
    __ASSERT_DEBUG( aIndex < KNumberOfButtons, 
    _L("CDialerKeyPadContainer::ComponentControl, index out of range."));
    
    return iButtons[aIndex];
    }
    
// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::HandlePointerEventL
//
// ---------------------------------------------------------------------------
//  
void CDialerKeyPadContainer::HandlePointerEventL(
    const TPointerEvent& aPointerEvent )
    {
    DIALER_PRINT("KeyPadContainer::HandlePointerEventL<");    
    
    CCoeControl::HandlePointerEventL( aPointerEvent );    

    if ( aPointerEvent.iType == TPointerEvent::EButton1Down ) 
        {
        iPointerEvent = aPointerEvent;    
        }
    DIALER_PRINT("KeyPadContainer::HandlePointerEventL>");     
    }
    
// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::SetVariety
//
// Set variety according to current state.
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::SetVariety()
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        iVariety = EVideoVarietyLandscape;
        }
    else
        {
        iVariety = EVideoVarietyPortrait;
        }
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::SetLayout
//
// Set layout for video dtmf or phone dialer 
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::SetLayout()
    {
    if ( iOperatingMode == EModeDialer ||
         iOperatingMode == EModeEasyDialing )
        {
        // Same renewed layout is used regardless of the status
        // of Easy Dialing
        SetEasyDialingLayout();
        }
    else
        {
        SetVideoLayout();
        }
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::SetPhoneLayout
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::SetPhoneLayout()
    {
    // Set Number entry layout.
    /*

    // LAF Table : grid_dialer2_keypad_pane
    inline TAknLayoutScalableParameterLimits cell_dialer2_keypad_pane_ParamLimits()
    inline TAknWindowComponentLayout cell_dialer2_keypad_pane()

    // LAF Table : cell_dialer2_keypad_pane
    inline TAknLayoutScalableParameterLimits bg_button_pane_pane_cp04_ParamLimits()
    inline TAknWindowComponentLayout bg_button_pane_pane_cp04()
    inline TAknLayoutScalableParameterLimits cell_dialer2_keypad_pane_g1_ParamLimits()
    */
    
    TRect parentRect( Rect() );

    // Layout buttons
    TAknLayoutScalableParameterLimits limits = 
        AknLayoutScalable_Apps::cell_dialer2_keypad_pane_ParamLimits( iVariety );
    
    TInt i = 0;
    for ( TInt row = limits.FirstRow(); row <= limits.LastRow(); row++ )
        {
        for ( TInt col = limits.FirstColumn(); 
              col <= limits.LastColumn(); col++ )
            {
            iButtons[i]->SetVariety( iVariety );
            iButtons[i]->SetOperationMode( iOperatingMode );
    
            AknLayoutUtils::LayoutControl( iButtons[i], parentRect, TAknWindowComponentLayout::Compose( 
                    AknLayoutScalable_Apps::cell_dialer2_keypad_pane( iVariety, col, row ), 
                    AknLayoutScalable_Apps::bg_button_pane_pane_cp04( iVariety ) ) );
            i++;
            }
        }
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::SetEasyDialingLayout
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::SetEasyDialingLayout()
    {
    TRect parentRect( Rect() );
    
    // Layout buttons
    TAknLayoutScalableParameterLimits limits = 
        AknLayoutScalable_Apps::cell_dialer2_keypad_pane_ParamLimits( iVariety );
    
    // Rectangle of the first button
    TAknLayoutRect buttonLayoutRect;
    buttonLayoutRect.LayoutRect( parentRect, 
            AknLayoutScalable_Apps::cell_dia3_key_num_pane( iVariety ) );
    TRect firstButtonRect = buttonLayoutRect.Rect();
    // Move the first button rect to top-left-corner of the keypad area, 
    // the default place is wrong in mirrored layout.
    firstButtonRect.SetRect( parentRect.iTl, firstButtonRect.Size() );
    
    TInt colCount = limits.LastColumn() - limits.FirstColumn() + 1;

    for ( TInt row = limits.FirstRow() ; row <= limits.LastRow() ; row++ )
        {
        for ( TInt col = limits.FirstColumn() ; col <= limits.LastColumn() ; col++ )
            {
            // Calculate corresponding index in iButtons array
            TInt idx = col + row * colCount;
            
            // Set mode and variety so that correct button internal layout will be used
            iButtons[idx]->SetVariety( iVariety );
            iButtons[idx]->SetOperationMode( iOperatingMode );
            
            // Layout button
            TRect buttonRect = firstButtonRect;
            buttonRect.Move( col * buttonRect.Width(), row * buttonRect.Height() );
            buttonRect.Shrink( 1, 1 ); // to create small gap between buttons
            iButtons[idx]->SetRect( buttonRect );
            }
        }
    
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::SetVideoLayout
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::SetVideoLayout()
    {
    TRect parentRect( Rect() );
    
    TAknLayoutScalableParameterLimits limits = 
        AknLayoutScalable_Apps::cell_video_dialer_keypad_pane_ParamLimits( iVariety ) ;
    
    TInt i = 0;
    for ( TInt row = limits.FirstRow(); row <= limits.LastRow(); row++ )
        {
        for ( TInt col = limits.FirstColumn(); 
              col <= limits.LastColumn(); 
              col++ )
            {
            iButtons[i]->SetVariety( iVariety );
            iButtons[i]->SetOperationMode( iOperatingMode );
        
            // Layout button
            AknLayoutUtils::LayoutControl(
                iButtons[i], parentRect, 
                TAknWindowComponentLayout::Compose( 
                    AknLayoutScalable_Apps::cell_video_dialer_keypad_pane( 
                        iVariety, col, row ),             
                    AknLayoutScalable_Apps::bg_button_pane_cp08( iVariety ) ) );
            i++;
            }
        }
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::CreateButtonsL
//
// Create buttons for this container.
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::CreateButtonsL()
    {
    DIALER_PRINT("KeyPadContainer::CreateButtonsL<");
    
    TInt flags ( KAknButtonReportOnLongPress|
                 KAknButtonReportOnKeyDown  |
                 KAknButtonRequestExitOnButtonUpEvent );
    
    for ( TInt i = 0; i < KNumberOfButtons; i++ )
        {
        const TPtrC numLabel = iKeyLabelManager->ButtonNumLabel( i );

        const TPtrC alphaLabel = iKeyLabelManager->ButtonFirstAlphaLabel( i );

        const TPtrC secondAlphaLabel = iKeyLabelManager->ButtonSecondAlphaLabel( i );
        
        CDialerKeyPadButton* button = CDialerKeyPadButton::NewLC( 
            numLabel,
            alphaLabel,
            secondAlphaLabel,
            KKeyPadButtons[i].iScanCode,
            KKeyPadButtons[i].iKeyCode,
            KKeyPadButtons[i].iBitmap,
            KKeyPadButtons[i].iBitmapMask,
            flags );
        iButtons.AppendL( button );
        CleanupStack::Pop( button );
        
        button->SetMopParent( this );
        button->SetParent( this );
        button->SetContainerWindowL( *this );
        button->SetObserver( this );
        button->ActivateL();
        }     
    DIALER_PRINT("KeyPadContainer::CreateButtonsL>");
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::HandleControlEventL
//
// Handles an event from an observed button item.
// Finds the pressed button and generates key press event, which
// is handled by parent control.
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::HandleControlEventL( CCoeControl* aControl,
                                                TCoeEvent aEventType )
    {
    DIALER_PRINTF("KeyPadContainer::HandleControlEventL.EventType=",
                 (TInt)aEventType);
    
   if ( aEventType == EEventStateChanged   || 
        aEventType == EEventRequestCancel ||
        aEventType == EEventRequestExit)
        
        {
        // Find tapped control 

        CDialerKeyPadButton* tappedButton = NULL;
        for ( TInt i=0; i < iButtons.Count(); i++ )
            {
            if ( iButtons[i] == aControl )
                {
                tappedButton = iButtons[i];
                break;
                }    
            }
        
        __ASSERT_ALWAYS( tappedButton, 
        _L("CDialerKeyPadContainer::HandleControlEventL, invalid button handle"));
        
        // Send key event to phone.
        TKeyEvent keyEvent;
        keyEvent.iScanCode = tappedButton->ScanCode();
        keyEvent.iModifiers = 0;
        keyEvent.iRepeats = 0;  
             
        switch ( aEventType )
            {
            case EEventRequestExit:
            case EEventRequestCancel:
                {
                DIALER_PRINT("HandleControlEventL.EEventRequestExit");
                iButtonPressedDown = EFalse;
                keyEvent.iCode = 0;
                ControlEnv()->SimulateKeyEventL( keyEvent, EEventKeyUp );
                }
                break;
            case EEventStateChanged:
                {    
                DIALER_PRINT("HandleControlEventL.EEventStateChanged");
                iButtonPressedDown = ETrue;
                keyEvent.iCode = tappedButton->KeyCode();
                iParentControl.PrepareForFocusGainL();

                ControlEnv()->SimulateKeyEventL( keyEvent, EEventKeyDown );    

                if( iButtonPressedDown )
                    {
                    // Send event key if key havent be lifted up already
                    ControlEnv()->SimulateKeyEventL( keyEvent, EEventKey );
                    }
                }
                break;
            default:
            break;
            }
        }
    DIALER_PRINT("KeyPadContainer::HandleControlEventL>");        
    }
    
// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::HandleResourceChange
//
// Forwards skin change event to buttons
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::HandleResourceChange( TInt aType )
    {
    if ( aType == KAknsMessageSkinChange )
        {    
        for ( TInt i=0; i < iButtons.Count(); i++ )
            {
            iButtons[i]->HandleResourceChange( aType );
            }
        }
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::MakeVisible
//
// Called when dialer control becomes visible or invisible
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::MakeVisible( TBool aVisible )
    {
    // This is needed when dialer control disappeares during keypress. 
    // Last pressed pointer event must be forwarted to framework 
    // as pointer up event. Thus button is set to unpressed state etc. 
    // 
    if ( !aVisible && iButtonPressedDown )
        {    
        iPointerEvent.iType = TPointerEvent::EButton1Up;
        TRAP_IGNORE( HandlePointerEventL( iPointerEvent ) );
        }    
    CCoeControl::MakeVisible( aVisible );        
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::SetOperationMode
//
// Set new operation mode for the keypad
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::SetOperationMode( TDialerOperationMode aOperatingMode )
    {
    iOperatingMode = aOperatingMode;
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::HandleGainingForeground
//
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::HandleGainingForeground()
    {
    
    }
    
// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::HandleLosingForeground
//
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::HandleLosingForeground()
    {
    // This is needed when dialer control disappeares during keypress. 
    // Last pressed pointer event must be forwarted to framework 
    // as pointer up event. Thus button is set to unpressed state etc. 
    // 
    if ( iButtonPressedDown )
        {    
        iPointerEvent.iType = TPointerEvent::EButton1Up;
        TRAP_IGNORE( HandlePointerEventL( iPointerEvent ) );
        }    
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::KeyLabelsChanged
//
// Reset button labels and update the layout
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::KeyLabelsChanged()
    {
    for ( TInt i = 0 ; i < iButtons.Count() ; i++ )
        {
        const TPtrC numLabel = iKeyLabelManager->ButtonNumLabel( i );
        const TPtrC alphaLabel = iKeyLabelManager->ButtonFirstAlphaLabel( i );
        const TPtrC secondAlphaLabel = iKeyLabelManager->ButtonSecondAlphaLabel( i );
        
        iButtons[i]->SetNumLabel( numLabel );
        iButtons[i]->SetPrimaryAlphaLabel( alphaLabel );
        iButtons[i]->SetSecondaryAlphaLabel( secondAlphaLabel );
        
        // reset layout
        iButtons[i]->SetSize( iButtons[i]->Size() );
        }
    DrawDeferred();
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::EnableTactileFeedback
//
//
// ---------------------------------------------------------------------------
//
void CDialerKeyPadContainer::EnableTactileFeedback( const TBool aEnable )
    {
    for ( TInt i=0; i < iButtons.Count(); i++ )
        {
        (( CDialerKeyPadButton* )iButtons[i])->EnableAudioFeedback( aEnable );
        }

    }
// End of File
