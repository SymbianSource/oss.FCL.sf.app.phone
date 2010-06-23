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

#include "mphonestatemachine.h"
#include "mphonekeyeventhandler.h"
#include "cphonekeys.h"
#include "cphonekeyeventforwarder.h"
#include "cphonepubsubproxy.h"
#include "cphonetimer.h"
#include "phonelogger.h"
#include "phoneui.pan"

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

    CEikonEnv* env = static_cast<CEikonEnv*>(ControlEnv());
    if( env )
        {
        env->EikAppUi()->RemoveFromStack( this );
        }
    delete iLongPressKeyEventTimer;
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
    TKeyEvent keyEvent = aKeyEvent;

    ret = OfferKeyEventBeforeControlStackL( aKeyEvent, aType );

    // Convert event. Use already converted iKeyPressedDown.
    // Do not convert other than DTMF tones
    if( CPhoneKeys::IsDtmfTone( aKeyEvent, aType ) )
        {
        keyEvent.iCode = iKeyPressedDown;
        }

    if ( !keyEvent.iRepeats )
        {
        // Start and stop dtmf
        iStateMachine->State()->HandleDtmfKeyToneL( keyEvent, aType );
        if( aType != EEventKeyUp &&
            ( CPhoneKeys::IsNumericKey( keyEvent, aType ) ||
              IsAlphaNumericKey( keyEvent ) )
          )
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

    CEikonEnv* env = static_cast<CEikonEnv*>(ControlEnv());
    if( env )
        {
        env->EikAppUi()->AddToStackL(
            this,
            ECoeStackPriorityEnvironmentFilter,
            ECoeStackFlagRefusesFocus );
        }

    // Create the long press key event timer
    iLongPressKeyEventTimer = CPhoneTimer::NewL();
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::IsAlphaNumericKey
// -----------------------------------------------------------------------------
//
TBool CPhoneKeyEventForwarder::IsAlphaNumericKey( const TKeyEvent& aKeyEvent )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneKeyEventForwarder::IsAlphaNumericKey");

    TBool alphaNumeric =
       iStateMachine->State()->IsAlphanumericSupportedAndCharInput( aKeyEvent );

    return alphaNumeric;
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
    if( !IsKeyBlocked( aKeyEvent ) )
        {
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
    
    if( !IsKeyBlocked( aKeyEvent ) )
        {
        // Send key to editor
        iStateMachine->State()->HandleKeyEventL( aKeyEvent, aType );
        
        if( EEventKeyUp == aType 
                && EKeyNull != iKeyPressedDown )
            {
            // Handle short key press
            iStateMachine->State()->HandleKeyMessageL( 
                MPhoneKeyEvents::EPhoneKeyShortPress, 
                TKeyCode( iKeyPressedDown ) );

            // Reset key code
            iScanCode = EStdKeyNull;
            iKeyPressedDown = EKeyNull;
            }
        }

    return EKeyWasNotConsumed;
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

    // Start the key press timer
    iKeyPressTime.UniversalTime();

    // Cancel the long press key event timer, if it is active
    if ( iLongPressKeyEventTimer->IsActive() )
        {
        iLongPressKeyEventTimer->CancelTimer();
        }

    // Re-start the timer
    iLongPressKeyEventTimer->After(
        KPhoneLongPressKeyEventDuration,
        TCallBack( DoHandleLongPressKeyEventCallbackL,
        this ) );

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

    // Convert event.
    TKeyEvent keyEvent = aKeyEvent;
    keyEvent.iCode = iKeyPressedDown;

    if(  aKeyEvent.iRepeats > 0 &&
        CPhoneKeys::IsDtmfTone( keyEvent, EEventKey ) )
        {
        // Do not repeat dtmf characters
        response = EKeyWasConsumed;
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

        switch ( aKeyEvent.iScanCode )
            {
            case EStdKeyEnter:
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
        	} //switch

    __PHONELOG1( EBasic, EPhoneControl,
        "CPhoneKeyEventHandler::ConvertKeyCode aCode (%d)", aCode );
    }

// -----------------------------------------------------------------------------
// CPhoneKeyEventForwarder::IsKeyBlocked
// -----------------------------------------------------------------------------
//
TBool CPhoneKeyEventForwarder::IsKeyBlocked( const TKeyEvent& aKeyEvent ) const
    {
    TBool ret = EFalse;
    
    // Check blocked keys
    const RArray<TInt>& keyScanCodeList = iStateMachine->State()->GetBlockedKeyList();
    TInt count = keyScanCodeList.Count();
    if( count )
        {
        for( TInt i = 0; i < count; i++ )
            {
            if( keyScanCodeList[i] == aKeyEvent.iScanCode )
                {
                ret = ETrue;
                }
            }
        }
    __PHONELOG1( EBasic, EPhoneControl,
        "CPhoneKeyEventHandler::IsKeyBlocked return (%d)", ret );
    
    return ret;
    }

//  End of File

