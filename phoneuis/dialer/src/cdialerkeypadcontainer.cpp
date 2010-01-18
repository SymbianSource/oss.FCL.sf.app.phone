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
* Description:  Shows number keypad and generates keypress event when 
*                 buttons are pressed.
*
*/


// INCLUDE FILES
#include <e32event.h>
#include <gulicon.h>
#include <eikapp.h>
#include <AknUtils.h>
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

#include "cdialerkeypadcontainer.h"
#include "dialercommon.h"
#include "dialertrace.h"

#include "cdialerkeypadbutton.h"
_LIT( KDialerMifFileName, "dialer.mif" );

// Number of buttons in this container
const TInt KNumberOfButtons = 12; 

// Keypad button images
const TMifDialer KKeyPadButtons[] =
    {
    EMbmDialerQgn_indi_dialer_one,
    EMbmDialerQgn_indi_dialer_two,
    EMbmDialerQgn_indi_dialer_three,
    EMbmDialerQgn_indi_dialer_four,
    EMbmDialerQgn_indi_dialer_five,
    EMbmDialerQgn_indi_dialer_six,
    EMbmDialerQgn_indi_dialer_seven,
    EMbmDialerQgn_indi_dialer_eight,
    EMbmDialerQgn_indi_dialer_nine,
    EMbmDialerQgn_indi_dialer_prefix,
    EMbmDialerQgn_indi_dialer_zero,
    EMbmDialerQgn_indi_dialer_hash
    };

// Keypad button image masks
const TMifDialer KKeyPadButtonsMasks[] =
    {
    EMbmDialerQgn_indi_dialer_one_mask,
    EMbmDialerQgn_indi_dialer_two_mask,
    EMbmDialerQgn_indi_dialer_three_mask,
    EMbmDialerQgn_indi_dialer_four_mask,
    EMbmDialerQgn_indi_dialer_five_mask,
    EMbmDialerQgn_indi_dialer_six_mask,
    EMbmDialerQgn_indi_dialer_seven_mask,
    EMbmDialerQgn_indi_dialer_eight_mask,
    EMbmDialerQgn_indi_dialer_nine_mask,
    EMbmDialerQgn_indi_dialer_prefix_mask,
    EMbmDialerQgn_indi_dialer_zero_mask,
    EMbmDialerQgn_indi_dialer_hash_mask
    };

// Match keypresses to keyevents send to the phone
//
const TInt TDialerButtonToKeypadMap[12][2] = 
        {  
            { 49, // ScanCode
              49  // KeyCode
            },
            { 50,
              50
            },
            { 51,
              51
            },
            { 52,
              52
            },    
            { 53,
              53
            },
            { 54,
              54
            },
            { 55,
              55
            },
            { 56,
              56
            },
            { 57,
              57
            },                        
            { EStdKeyNkpAsterisk,
              42
            },
            { '0',
              '0'
            }, 
            { EStdKeyHash,
              35
            }                                                                                                       
        };   

// Class declaration for CDialerKeyPadButton separated to cdialerkeypadbutton.h.

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CDialerKeyPadButton::CDialerKeyPadButton()
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CDialerKeyPadButton::~CDialerKeyPadButton()
    {
    MTouchFeedback* feedback = MTouchFeedback::Instance();
    if ( feedback )
        {
        feedback->RemoveFeedbackForControl( iButton );
        }
    delete iButton;   
    }
        
// ---------------------------------------------------------------------------
// Creates number keypad button
// ---------------------------------------------------------------------------
//
void CDialerKeyPadButton::CreateButtonL( const TInt aScanCode, 
                                         const TInt aKeyCode,
                                         const TMifDialer aButtonIcon, 
                                         const TMifDialer aButtonIconMask )
    {
    iScanCode = aScanCode;
    iKeyCode  = aKeyCode;
    iButtonIcon = aButtonIcon;
    iButtonIconMask = aButtonIconMask;
    
    TFileName mifPath( KDriveZ );
    mifPath.Append( KDC_APP_BITMAP_DIR );
    mifPath.Append( KDialerMifFileName );

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    CFbsBitmap* bitmap (NULL);
    CFbsBitmap* mask (NULL);

    // Get icon ids.
    TAknsItemID skinItemId( KAknsIIDNone );  
    
    // Create button image.
    MapDialerIconToSkinIcon( aButtonIcon ,skinItemId );
    AknsUtils::CreateColorIconLC( 
        skin, 
        skinItemId,
        KAknsIIDQsnIconColors, 
        EAknsCIQsnIconColorsCG30,
        bitmap, 
        mask, 
        mifPath, 
        aButtonIcon, 
        aButtonIconMask,
        KRgbBlack
        );

    CGulIcon* icon = CGulIcon::NewL( bitmap, mask );
    CleanupStack::Pop( 2 );
    CleanupStack::PushL( icon );
    iButton = CAknButton::NewL(
        icon , // ownership taken
        NULL, NULL, NULL, _L(""), _L(""), 0, 0 );
    iButton->SetButtonFlags( KAknButtonReportOnLongPress|
                            KAknButtonReportOnKeyDown  |
                            KAknButtonRequestExitOnButtonUpEvent );
    iButton->SetIconScaleMode( EAspectRatioPreserved );
    iButton->SetMargins( TMargins8(0,0,0,0) );    
    CleanupStack::Pop( icon ); 
    }
    
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//    
TInt CDialerKeyPadButton::ScanCode() const
    {
    return iScanCode;
    }
    
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//    
TInt CDialerKeyPadButton::KeyCode() const
    {
    return iKeyCode;
    }    
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CDialerKeyPadButton::MapDialerIconToSkinIcon( const TInt aDialerIcon, 
                                         TAknsItemID& aItemId ) const
    {    
    switch ( aDialerIcon )
        {
        case EMbmDialerQgn_indi_dialer_one:
            aItemId = KAknsIIDQgnIndiDialerOne;
            break;
        case EMbmDialerQgn_indi_dialer_two:
            aItemId = KAknsIIDQgnIndiDialerTwo;
            break;
        case EMbmDialerQgn_indi_dialer_three:
            aItemId = KAknsIIDQgnIndiDialerThree;
            break;
        case EMbmDialerQgn_indi_dialer_four:
            aItemId = KAknsIIDQgnIndiDialerFour;
            break;
        case EMbmDialerQgn_indi_dialer_five:
            aItemId = KAknsIIDQgnIndiDialerFive;
            break;
        case EMbmDialerQgn_indi_dialer_six:
            aItemId = KAknsIIDQgnIndiDialerSix;
            break;
        case EMbmDialerQgn_indi_dialer_seven:
            aItemId = KAknsIIDQgnIndiDialerSeven;
            break;    
        case EMbmDialerQgn_indi_dialer_eight:
            aItemId = KAknsIIDQgnIndiDialerEight;
            break;
        case EMbmDialerQgn_indi_dialer_nine:
            aItemId = KAknsIIDQgnIndiDialerNine;
            break;
        case EMbmDialerQgn_indi_dialer_zero:
            aItemId = KAknsIIDQgnIndiDialerZero;
            break;
        case EMbmDialerQgn_indi_dialer_prefix:
            aItemId = KAknsIIDQgnIndiDialerPrefix;
            break;
        case EMbmDialerQgn_indi_dialer_hash:
            aItemId = KAknsIIDQgnIndiDialerHash;
            break;                        
        default:
            break;
        }
    }   

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//   
void CDialerKeyPadButton::SetIconSize( TSize& aSize )
    {
    iButton->SetIconSize( aSize );
    }  
    
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//   
void CDialerKeyPadButton::UpdateIconL()
    {
    TFileName mifPath( KDriveZ );
    mifPath.Append( KDC_APP_BITMAP_DIR );
    mifPath.Append( KDialerMifFileName );
    
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    CFbsBitmap* bitmap (NULL);
    CFbsBitmap* mask (NULL);
        
    // Get icon id.
    TAknsItemID skinItemId( KAknsIIDNone );  
    MapDialerIconToSkinIcon( iButtonIcon, skinItemId );
    
    AknsUtils::CreateColorIconLC( 
            skin, 
            skinItemId,
            KAknsIIDQsnIconColors, 
            EAknsCIQsnIconColorsCG30,
            bitmap, 
            mask, 
            mifPath, 
            iButtonIcon, 
            iButtonIconMask,
            KRgbBlack
            );
    
    CGulIcon* icon = CGulIcon::NewL( bitmap, mask );
    CleanupStack::Pop( 2 );
    
    iButton->State()->SetIcon( icon ); // icon ownership transfered
    
    }  

// ---------------------------------------------------------------------------
// 
// Enable or disable audio but keep vibra feedback 
// ---------------------------------------------------------------------------
//   
void CDialerKeyPadButton::EnableAudioFeedback( const TBool aEnable )
    {
    MTouchFeedback* feedback = MTouchFeedback::Instance();
    if ( feedback )
        {
        feedback->EnableFeedbackForControl( iButton, ETrue, aEnable );
        }
    }
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CDialerKeyPadButton::HandleResourceChange( TInt aType )
    {
    if ( aType == KAknsMessageSkinChange )
        {
        TRAP_IGNORE( UpdateIconL() );
        iButton->HandleResourceChange( aType );
        }    
    }

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
    CreateButtonsL();
    CCoeEnv* env = CCoeEnv::Static();
    env->AddForegroundObserverL( *this );
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
        AknsUtils::DeregisterControlPosition( 
                   ((CDialerKeyPadButton*)iButtons[i])->Control());   
        delete iButtons[i];
        }
    iButtons.Close();
    CCoeEnv* env = CCoeEnv::Static();
    env->RemoveForegroundObserver( *this );
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
    
    return ((CDialerKeyPadButton*)iButtons[aIndex])->Control();
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
    if ( iOperatingMode == EModeDialer )
        {
        SetPhoneLayout();
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

    // Icon size - all in same size
    TAknLayoutRect functionGraphics;  
             
    functionGraphics.LayoutRect( 
        parentRect,
        TAknWindowComponentLayout::Compose(
            AknLayoutScalable_Apps::cell_dialer2_keypad_pane( iVariety ),
            AknLayoutScalable_Apps::cell_dialer2_keypad_pane_g1( 
                                                        iVariety ) ) );
    TSize iconSize = functionGraphics.Rect().Size();
    
    // Layout buttons
    TAknLayoutScalableParameterLimits limits = 
    AknLayoutScalable_Apps::cell_dialer2_keypad_pane_ParamLimits( iVariety );
    
    TInt i = 0;
    for ( TInt row = limits.FirstRow(); row <= limits.LastRow(); row++ )
        {
        for ( TInt col = limits.FirstColumn(); 
              col <= limits.LastColumn(); col++ )
            {
            // Layout button
            AknLayoutUtils::LayoutControl(
                ((CDialerKeyPadButton*)iButtons[i])->Control(), parentRect, 
                TAknWindowComponentLayout::Compose( 
                    AknLayoutScalable_Apps::cell_dialer2_keypad_pane( iVariety,
                                                col, row ), 
                    AknLayoutScalable_Apps::bg_button_pane_pane_cp04( 
                                                iVariety ) ) );
            
            // Set icon size
            ((CDialerKeyPadButton*)iButtons[i++])->SetIconSize( iconSize );
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
    
    // Icon size - all in same size
    TAknLayoutRect functionGraphics;  
             
    functionGraphics.LayoutRect( 
        parentRect,
        TAknWindowComponentLayout::Compose(
            AknLayoutScalable_Apps::cell_video_dialer_keypad_pane( iVariety, 
                                        0, 0 ),
            AknLayoutScalable_Apps::cell_video_dialer_keypad_pane_g1( 
                                        iVariety ) ) );
    TSize iconSize = functionGraphics.Rect().Size();
    
    TAknLayoutScalableParameterLimits limits = 
        AknLayoutScalable_Apps::cell_video_dialer_keypad_pane_ParamLimits( 
                                        iVariety ) ;
    
    TInt i = 0;
    for ( TInt row = limits.FirstRow(); row <= limits.LastRow(); row++ )
        {
        for ( TInt col = limits.FirstColumn(); 
              col <= limits.LastColumn(); 
              col++ )
            {
            // Layout button
            AknLayoutUtils::LayoutControl(
                ((CDialerKeyPadButton*)iButtons[i])->Control(), parentRect, 
                TAknWindowComponentLayout::Compose( 
                    AknLayoutScalable_Apps::cell_video_dialer_keypad_pane( 
                        iVariety, col, row ), 
                    AknLayoutScalable_Apps::bg_button_pane_cp08( 
                                        iVariety ) ) );
            
            // Set icon size
            iButtons[i++]->SetIconSize( iconSize );
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
    
    for ( TInt i=0; i < KNumberOfButtons; i++ )
        {
        CDialerKeyPadButton* button = new (ELeave) CDialerKeyPadButton();
        CleanupStack::PushL( button );
        button->CreateButtonL( TDialerButtonToKeypadMap[i][0],
                               TDialerButtonToKeypadMap[i][1],         
                               KKeyPadButtons[i],
                               KKeyPadButtonsMasks[i] );
        CleanupStack::Pop( button );
        iButtons.Append( button );                      
        button->Control()->SetMopParent( this );
        button->Control()->SetParent( this );    
        button->Control()->SetContainerWindowL( *this );
        button->Control()->SetObserver( this );    
        button->Control()->ActivateL();
        AknsUtils::RegisterControlPosition( button->Control() );   
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
            if ( iButtons[i]->Control() == aControl )
                {
                tappedButton = iButtons[i];
                break;
                }    
            }
            
        // Send key event to phone.
        TKeyEvent keyEvent;
        keyEvent.iScanCode = tappedButton->ScanCode();
        keyEvent.iCode = tappedButton->KeyCode();
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
            (( CDialerKeyPadButton* )iButtons[i])->HandleResourceChange( 
                                                                    aType );
            }
        }
    }

// ---------------------------------------------------------------------------
// CDialerKeyPadContainer::MakeVisible
//
// Called when dialer control come visible or unvisible
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
