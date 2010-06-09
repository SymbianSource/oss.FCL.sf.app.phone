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
* Description:  Key event forwarder.
*
*/


// INCLUDE FILES
#include <eikenv.h>
#include <eikappui.h>
#include <bldvariant.hrh>
#include <featmgr.h>
#include <PtiDefs.h>
#include <AknUtils.h>

#include "mphonestatemachine.h"
#include "mphonekeyeventhandler.h"
#include "cphonekeys.h"
#include "cphonekeyeventforwarder.h"
#include "cphonepubsubproxy.h"
#include "cphonetimer.h"
#include "phonelogger.h"
#include "phoneui.pan"
#include "mphoneviewcommandhandle.h"
#include "cphoneqwertyhandler.h"
#include "tphonecmdparampointer.h"
#include "mphoneqwertymodeobserver.h"
#include "cdialer.h"

// CONSTANTS

//Avkon P&S keys
const TUid KCRUidAvkon = { 0x101F876E };
const TUint32 KAknKeyBoardLayout = 0x0000000B;

// Characters resulting from multitapping *-key.
_LIT( KAsteriskMultitapCharacters, "*+pw" );

// Multitap delay parameter in micro seconds.
const TInt KMultitapDelay = 1000000;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::NewL
// -----------------------------------------------------------------------------
//
CPhoneKeyEventForwarder* CPhoneKeyEventForwarder::NewL(
        const TRect& aRect,
        MPhoneStateMachine* aStateMachine,
        MPhoneViewCommandHandle* aViewCommandHandle )
    {
    CPhoneKeyEventForwarder* self =
        new (ELeave) CPhoneKeyEventForwarder( aStateMachine, aViewCommandHandle );

    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::~CPhoneKeyEventForwarder
// -----------------------------------------------------------------------------
//
CPhoneKeyEventForwarder::~CPhoneKeyEventForwarder()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneKeyEventForwarder::~CPhoneKeyEventForwarder");

    if ( AknLayoutUtils::PenEnabled() ) 
        {
        iPeninputServer.RemovePenUiActivationHandler( this );
        }
    
    iPeninputServer.Close();
    
    if ( iEikonEnv )
        {
        iEikonEnv->EikAppUi()->RemoveFromStack( this );
        }
    delete iLongPressKeyEventTimer;
    
    delete iQwertyHandler;
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::CountComponentControls
// -----------------------------------------------------------------------------
//
TInt CPhoneKeyEventForwarder::CountComponentControls() const
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneKeyEventForwarder::CountComponentControls");
    return 0;
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::ComponentControl
// -----------------------------------------------------------------------------
//
CCoeControl* CPhoneKeyEventForwarder::ComponentControl(
        TInt /*aIndex*/ ) const
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneKeyEventForwarder::ComponentControl");
    return NULL;
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::SizeChanged
// -----------------------------------------------------------------------------
//
void CPhoneKeyEventForwarder::SizeChanged()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneKeyEventForwarder::SizeChanged");
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::Draw
// -----------------------------------------------------------------------------
//
void CPhoneKeyEventForwarder::Draw(
        const TRect& /*aRect*/ ) const
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneKeyEventForwarder::Draw");
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::OfferKeyEventL
// Checks is hte keyevent such that the number entry can be opened
// -----------------------------------------------------------------------------
//
TKeyResponse CPhoneKeyEventForwarder::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,
        TEventCode aType )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::OfferKeyEventL");

    TKeyResponse ret( EKeyWasNotConsumed );
    
    // After event key, expect to have key up event.
    iExpectKeyUpEvent = ( aType == EEventKey );

    ret = OfferKeyEventBeforeControlStackL( aKeyEvent, aType );

    if ( !aKeyEvent.iRepeats )
        {
        // Convert event. Use already converted iKeyPressedDown.
        TKeyEvent keyEvent = aKeyEvent;
        keyEvent.iCode = iKeyPressedDown;
        
        // Do not handle dtmf tone if the type is EEventKey but we are not
        // expecting key up event. This happens if the key up event has been
        // handled by some CActiveSchedulerWait object somewhere in the execution 
        // of function OfferKeyEventBeforeControlStackL.
        if ( iExpectKeyUpEvent || aType != EEventKey )
            {
            // Start and stop dtmf
            iStateMachine->State()->HandleDtmfKeyToneL( keyEvent, aType );
            }
        
        // Open number entry view if any allowed character key
        // is pressed on homescreen or in-call ui
        if ( aType != EEventKeyUp && IsKeyAllowed( keyEvent ) )
            {
            // Do not open number entry with up key
            iStateMachine->State()->HandleCreateNumberEntryL( keyEvent, aType );
            }
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::CPhoneKeyEventForwarder
// -----------------------------------------------------------------------------
//
CPhoneKeyEventForwarder::CPhoneKeyEventForwarder(
    MPhoneStateMachine* aStateMachine,
    MPhoneViewCommandHandle* aViewCommandHandle  )
    : iStateMachine( aStateMachine ),
    iViewCommandHandle( aViewCommandHandle )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::CPhoneKeyEventForwarder");
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneKeyEventForwarder::ConstructL( const TRect& aRect )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::ConstructL");

    if ( FeatureManager::FeatureSupported( KFeatureIdKeypadNoSlider ) )
        {
        RWindowGroup& groupWin = iCoeEnv->RootWin();
        }
    // Create invisible control.
    CreateWindowL();
    MakeVisible( EFalse );
    SetRect( aRect );

    if ( iEikonEnv )
        {
        iEikonEnv->EikAppUi()->AddToStackL(
            this,
            ECoeStackPriorityEnvironmentFilter,
            ECoeStackFlagRefusesFocus );
        }

    // Create the long press key event timer
    iLongPressKeyEventTimer = CPhoneTimer::NewL();
    
    // Create qwerty mode handler
    iQwertyHandler = CPhoneQwertyHandler::NewL();
    
    TPhoneCmdParamPointer ptrParam;
    iViewCommandHandle->ExecuteCommand( EPhoneViewGetQwertyModeObserver, &ptrParam );
    CDialer* qwertyObserver = 
            static_cast<CDialer*>( ptrParam.Pointer() );
            
    iQwertyHandler->AddQwertyModeObserverL( *qwertyObserver );
    
    if ( AknLayoutUtils::PenEnabled() )
        {
        User::LeaveIfError( iPeninputServer.Connect() );
        iPeninputServer.AddPenUiActivationHandler( this, EPluginInputModeAll );
        iVirtualKeyBoardOpen = iPeninputServer.IsVisible();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::IsAlphaNumericKey
// -----------------------------------------------------------------------------
//
TBool CPhoneKeyEventForwarder::IsKeyAllowed( const TKeyEvent& aKeyEvent )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::IsAlphaNumericKey");

    TKeyEvent keyEvent( aKeyEvent );
    
    // Check keyboard mode
    TBool isModeNumeric = iViewCommandHandle->HandleCommandL(
          EPhoneViewIsNumberEntryNumericMode ) == EPhoneViewResponseSuccess;

    // Check if key is a numeric key
    TBool isNumeric = CPhoneKeys::IsNumericKey( keyEvent, EEventKey );
    
    // Check if key is alpha numeric key and alphabet input is allowed
    TBool isAllowedAlphaNumeric =
       iStateMachine->State()->IsAlphanumericSupportedAndCharInput( keyEvent );

    return ( ( isModeNumeric && isNumeric ) || 
             ( !isModeNumeric && isAllowedAlphaNumeric ) );
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::ConvertHalfQwertySpecialChar
// -----------------------------------------------------------------------------
//
TBool CPhoneKeyEventForwarder::ConvertHalfQwertySpecialChar( TUint& aCode,
        const TKeyEvent& aKeyEvent )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::ConvertHalfQwertySpecialChar");

    TBool ret( EFalse );
    TInt keyboard( CPhonePubSubProxy::Instance()->Value(
          KCRUidAvkon,
          KAknKeyBoardLayout ) );

    if ( keyboard == EPtiKeyboardHalfQwerty )
        {
        switch ( aKeyEvent.iScanCode )
           {
           case EStdKeyLeftShift:
               ret = ETrue;
               aCode = KPhoneDtmfHashCharacter;
               break;
           case EStdKeyLeftFunc:
               ret = ETrue;
               aCode = KPhoneDtmfStarCharacter;
               break;
           case EStdKeySpace:
               ret = ETrue;
               aCode = KPhoneDtmf0Character;
               break;
           default:
               break;
           }
        }

    __PHONELOG1( EBasic, EPhoneControl,
        "CPhoneKeyEventForwarder::ConvertHalfQwertySpecialChar =%d ",
        ret );

    return ret;
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::IsKeySimulatedByTouchDialer
// 
// -----------------------------------------------------------------------------
//
TBool CPhoneKeyEventForwarder::IsKeySimulatedByTouchDialer( 
        const TKeyEvent& aKeyEvent ) const
    {
    return ( ( aKeyEvent.iModifiers & EModifierNumLock ) &&
             ( aKeyEvent.iModifiers & EModifierKeypad ) );
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::HandleTouchDialerKeyEventL
// Handle EEventKey type of event from touch dialer
// -----------------------------------------------------------------------------
//
void CPhoneKeyEventForwarder::HandleTouchDialerKeyEventL( const TKeyEvent& aKeyEvent )
    {
    TBool multitap = aKeyEvent.iScanCode == EStdKeyNkpAsterisk && 
            iPreviousScanCode == EStdKeyNkpAsterisk &&
            iKeyPressTime.MicroSecondsFrom( iPreviousKeyPressTime ) < KMultitapDelay;
    
    if ( multitap )
        {
        // Update multitap index
        iMultitapIndex = ( iMultitapIndex + 1 ) % KAsteriskMultitapCharacters().Length();
        
        // Delete the previously entered character by simulating a backspace character.
        TKeyEvent backSpaceEvent;
        backSpaceEvent.iModifiers = 0;
        backSpaceEvent.iRepeats = 0;
        backSpaceEvent.iCode = EKeyBackspace;
        backSpaceEvent.iScanCode = EStdKeyBackspace;
        iStateMachine->State()->HandleKeyEventL( backSpaceEvent, EEventKey );
        
        TKeyEvent keyEvent( aKeyEvent );
        
        // Modify the key event to contain the next character on multitap list.
        keyEvent.iCode = ( TInt ) KAsteriskMultitapCharacters()[ iMultitapIndex ];
        
        // Send character to number entry.
        iStateMachine->State()->HandleKeyEventL( keyEvent, EEventKey );
        }
    else 
        {
        iMultitapIndex = 0;
        iStateMachine->State()->HandleKeyEventL( aKeyEvent, EEventKey );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::OfferKeyEventBeforeControlStackL
// Let phone handle before other components in control stack
// -----------------------------------------------------------------------------
//
TKeyResponse CPhoneKeyEventForwarder::OfferKeyEventBeforeControlStackL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::OfferKeyEventBeforeControlStackL");

    TKeyResponse response( EKeyWasNotConsumed );

    // Handle key events before FEP
    // This is must becouse FEP consumes * key and when
    // editor is in alphanumeric mode FEP consumes all alphanumeric
    // keys
    switch( aType )
        {
         // EEventKeyDown
        case EEventKeyDown:
            response = HandleEventKeyDownBeforeControlStackL( aKeyEvent );
            break;
        // EEventKey
        case EEventKey:
            response = HandleEventKeyBeforeControlStackL( aKeyEvent );
            break;
        // EEventKeyUp
        case EEventKeyUp:
            response = HandleEventKeyUpBeforeControlStackL( aKeyEvent );
            break;
        default:
            break;
        }

    return response;
    }


// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::OfferKeyEventAfterControlStackL
// Let phone handle keys if no one has done it already
// -----------------------------------------------------------------------------
//
TKeyResponse CPhoneKeyEventForwarder::OfferKeyEventAfterControlStackL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::OfferKeyEventAfterControlStackL");
    
    // Send key to editor
    iStateMachine->State()->HandleKeyEventL( aKeyEvent, aType );
    
    if ( EEventKeyUp == aType && EKeyNull != iKeyPressedDown )
        {
        // Handle short key press
        iStateMachine->State()->HandleKeyMessageL( 
            MPhoneKeyEvents::EPhoneKeyShortPress, 
            TKeyCode( iKeyPressedDown ) );

        // Reset key code
        iScanCode = EStdKeyNull;
        iKeyPressedDown = EKeyNull;
        }

    return EKeyWasNotConsumed;
    }

// ---------------------------------------------------------------------------
// CPhoneKeyEventForwarder::OnPeninputUiDeactivated
//
// Gets called when the virtual keyboard editor is closed.
// ---------------------------------------------------------------------------
//
void CPhoneKeyEventForwarder::OnPeninputUiDeactivated()
    {
    __LOGMETHODSTARTEND( EPhoneControl,
            "CPhoneKeyEventForwarder::OnPeninputUiDeactivated");
    iVirtualKeyBoardOpen = EFalse;
    }

// ---------------------------------------------------------------------------
// CPhoneKeyEventForwarder::OnPeninputUiActivated
//
// Gets called when the virtual keyboard editor is opened.
// ---------------------------------------------------------------------------
//
void CPhoneKeyEventForwarder::OnPeninputUiActivated()
    {
    __LOGMETHODSTARTEND( EPhoneControl,
            "CPhoneKeyEventForwarder::OnPeninputUiActivated");
    iVirtualKeyBoardOpen = ETrue;
    }

// -----------------------------------------------------------
// CPhoneKeyEventForwarder::HandleEventKeyDownBeforeControlStackL
// -----------------------------------------------------------
//
TKeyResponse CPhoneKeyEventForwarder::HandleEventKeyDownBeforeControlStackL(
    const TKeyEvent& aKeyEvent )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::HandleEventKeyDownBeforeControlStackL");

    // Convert key code
    ConvertKeyCode( iKeyPressedDown, aKeyEvent );
    // Save key scan code
    iScanCode = aKeyEvent.iScanCode;
    
    // Store the previous keypress time.
    iPreviousKeyPressTime = iKeyPressTime;

    // Start the key press timer
    iKeyPressTime.UniversalTime();

    // Cancel the long press key event timer, if it is active
    if ( iLongPressKeyEventTimer->IsActive() )
        {
        iLongPressKeyEventTimer->CancelTimer();
        }
    
    // Don't initiate long tap timer if all these are true 
    // a) key event is not from virtual dialer
    // b) device is in qwerty mode
    // c) phone number editor is alpha mode, i.e. not in numeric mode
    // This is to prevent phone app's long tap functionality with qwerty 
    // long presses, and to have the normal editor long press behaviour 
    // instead.
    TBool preventLongTap = 
            !IsKeySimulatedByTouchDialer( aKeyEvent ) && 
            iQwertyHandler->IsQwertyInput() &&
            iViewCommandHandle->HandleCommandL( EPhoneViewIsNumberEntryNumericMode ) != EPhoneViewResponseSuccess;
    
    // Prevent long tap functionality for key events coming from virtual touch keyboard.
    // Virtual touch keyboard has own editing functionality for long presses, and telephony
    // should not add conflicting behaviour on top of that.
    preventLongTap = preventLongTap || iVirtualKeyBoardOpen;

    if ( !preventLongTap ) 
        {
        // Start long press timer
        iLongPressKeyEventTimer->After(
            KPhoneLongPressKeyEventDuration,
            TCallBack( DoHandleLongPressKeyEventCallbackL,
            this ) );
        }

    return ( EKeyWasNotConsumed );
    }

// -----------------------------------------------------------
// CPhoneKeyEventForwarder::HandleEventKeyBeforeControlStackL
// Both short key press event (iRepeats == 0) and
// long key press event (iRepeats == 1) are handled here
// -----------------------------------------------------------
//
TKeyResponse CPhoneKeyEventForwarder::HandleEventKeyBeforeControlStackL(
    const TKeyEvent& aKeyEvent )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::HandleEventKeyBeforeControlStackL");

    TKeyResponse response( EKeyWasNotConsumed );
    TKeyEvent keyEvent( aKeyEvent );

    // Special handling for virtual dialer keys events
    const TBool simulatedByDialer = IsKeySimulatedByTouchDialer( keyEvent );
    if ( simulatedByDialer )
        {
        // feed the event directly to number entry
        HandleTouchDialerKeyEventL( keyEvent );
        response = EKeyWasConsumed;
        }
    else 
        {
        // If not simulated by dialer, multitap related fields are reset.
        // Any key event not originating from dialer interrupts multitap
        // behaviour.
        iMultitapIndex = 0;
        iPreviousScanCode = 0;
        }

    // Special handling for QWERTY numeric mode key events
    if ( response == EKeyWasNotConsumed )
        {
        // FEP treats numeric QWERTY mode of Phone editor as a special case where most
        // key events flow through directly to the Phone app (but some don't).
        // To ensure consistent handling of numeric mode keys and QWERTY modifiers, 
        // handle those keys manually before FEP has a chance to mess things up.
        TBool numericMode = iViewCommandHandle->HandleCommandL(
              EPhoneViewIsNumberEntryNumericMode ) == EPhoneViewResponseSuccess;

        if ( iQwertyHandler->IsQwertyInput() && numericMode )
            {
            iQwertyHandler->ConvertToNumeric( keyEvent );
            // Send key to editor unless this is a repeat event for dtmf character
            if ( aKeyEvent.iRepeats == 0 ||
                 !CPhoneKeys::IsDtmfTone( keyEvent, EEventKey ) )
                {
                iStateMachine->State()->HandleKeyEventL( keyEvent, EEventKey );
                }
            response = EKeyWasConsumed;
            }
        }
    
    return response;
    }

// -----------------------------------------------------------
// CPhoneKeyEventForwarder::HandleEventKeyUpBeforeControlStackL
// -----------------------------------------------------------
//
TKeyResponse CPhoneKeyEventForwarder::HandleEventKeyUpBeforeControlStackL(
    const TKeyEvent& aKeyEvent )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::HandleEventKeyUpBeforeControlStackL");

    // Send a long press key event to the current state provided
    // that the key scan code is the same as the key that was pressed
    if ( iScanCode == aKeyEvent.iScanCode )
        {
        // Pass the key press duration to the current state
        TTime now;
        now.UniversalTime();
        TTimeIntervalMicroSeconds keyPressDuration = now.MicroSecondsFrom(
           iKeyPressTime );
        iStateMachine->State()->HandleKeyPressDurationL(
            TKeyCode( iKeyPressedDown ),
            keyPressDuration );

        // Cancel the long press timer, if it is active. Otherwise
        // ignore the key event since it was already handled as a long press
        // key event.
        if ( iLongPressKeyEventTimer->IsActive() )
            {
            iLongPressKeyEventTimer->CancelTimer();
            }
        }

    // Store the previous scan code
    iPreviousScanCode = iScanCode;

    return EKeyWasNotConsumed;
    }

// ---------------------------------------------------------
// CPhoneKeyEventForwarder::DoHandleLongPressKeyEventCallback
//
// This routine is called when the long press key event timer expires
// after KPhoneLongPressKeyEventDuration.
// ---------------------------------------------------------
//
TInt CPhoneKeyEventForwarder::DoHandleLongPressKeyEventCallbackL( TAny* aAny )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::DoHandleLongPressKeyEventCallbackL");

    __ASSERT_DEBUG( aAny,
        Panic( EPhoneCtrlUnknownPanic ) );

    reinterpret_cast< CPhoneKeyEventForwarder* >( aAny )->
        HandleLongPressKeyEventL();

    return KErrNone;
    }

// ---------------------------------------------------------
// CPhoneKeyEventForwarder::HandleLongPressKeyEventL
// Handle long press key event
// ---------------------------------------------------------
//
void CPhoneKeyEventForwarder::HandleLongPressKeyEventL()
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::HandleLongPressKeyEventL");

    // In alphanumeric mode EEventKey-event is not received so we must
    // compare also with scan code.
    if ( KPhoneDtmfHashCharacter == iKeyPressedDown )
        {
        // Separate handling for long hash key because there is so much
        // different functionality under it and it works differently
        // in different protocols.
        iStateMachine->State()->HandleLongHashL();
        }
    else if ( KPhoneDtmfStarCharacter == iKeyPressedDown )
        {
        // KPhoneDtmfStarCharacter value used in configuration file for
        // long key press application launching
        iStateMachine->State()->HandleKeyMessageL(
               MPhoneKeyEvents::EPhoneKeyLongPress,
               TKeyCode( KPhoneDtmfStarCharacter ) );
        }
    else
        {
        __PHONELOG1(
            EBasic, EPhoneControl,
            "CPhoneKeyEventHandler::HandleLongPressKeyEventL iCode(%d)",
            iKeyPressedDown );
        iStateMachine->State()->HandleKeyMessageL(
            MPhoneKeyEvents::EPhoneKeyLongPress,
            TKeyCode( iKeyPressedDown ) );
        }

    // Reset key code
    iScanCode = EStdKeyNull;
    iKeyPressedDown = EKeyNull;
    }

// -----------------------------------------------------------
// CPhoneKeyEventForwarder::ConvertKeyCode
// -----------------------------------------------------------
//
void CPhoneKeyEventForwarder::ConvertKeyCode( TUint& aCode,
        const TKeyEvent& aKeyEvent )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::ConvertKeyCode");

    // Handler for special device key mapping in case iScanCode
    // to iCode conversion hasn't been handled by CAknAppUi::GetAliasKeyCodeL
    __PHONELOG1( EBasic, EPhoneControl,
        "CPhoneKeyEventHandler::ConvertKeyCode scan code (%d)",
        aKeyEvent.iScanCode );

    if ( !ConvertHalfQwertySpecialChar( aCode, aKeyEvent ) )
        {
        TBool numMode = iViewCommandHandle->HandleCommandL(
              EPhoneViewIsNumberEntryNumericMode ) == EPhoneViewResponseSuccess;
        TBool simulatedByDialer = IsKeySimulatedByTouchDialer( aKeyEvent );
    
        if ( iQwertyHandler->IsQwertyInput() && numMode && !simulatedByDialer )
            {
            TUint numCode = iQwertyHandler->NumericKeyCode( aKeyEvent );
            if ( numCode )
                {
                aCode = numCode;
                }
            else
                {
                aCode = aKeyEvent.iScanCode;
                }
            }
        else 
            {
            switch ( aKeyEvent.iScanCode )
                {
                case EStdKeyEnter:
                case EStdKeyNkpEnter:
                    aCode = EKeyEnter;
                    break;
                case EStdKeyYes:
                    aCode = EKeyYes;
                    break;
                case EStdKeyNo:
                    aCode = EKeyNo;
                    break;
                case EStdKeyDeviceF:
                    aCode = EKeyDeviceF;  // EStdKeyDeviceF mapping for unlock switch.
                    break;
                case EStdKeyHash:
                    aCode = KPhoneDtmfHashCharacter;
                    break;
                case EStdKeyNkpAsterisk:
                    aCode = KPhoneDtmfStarCharacter;
                    break;
                case EStdKeyApplication0:
                    aCode = EKeyApplication0;
                    break;
                default:
                    aCode = aKeyEvent.iScanCode; // Use default code
                    break;
                }
            }
        }

    __PHONELOG1( EBasic, EPhoneControl,
        "CPhoneKeyEventHandler::ConvertKeyCode aCode (%d)", aCode );
    }

//  End of File

