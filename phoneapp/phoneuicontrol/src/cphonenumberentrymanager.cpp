/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  -
*
*/


#include <StringLoader.h>
#include <activeidle2domainpskeys.h>
#include <featmgr.h>
#include <eikenv.h>

#include "cphonenumberentrymanager.h"
#include "mpeengineinfo.h"
#include "mphoneviewcommandhandle.h"
#include "mphonestatemachine.h"
#include "mphonecustomization.h"

#include "cphonecbamanager.h"
#include "cphonepubsubproxy.h"
#include "cphonekeys.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamcallheaderdata.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "phoneui.pan"
#include "phoneviewcommanddefinitions.h"
#include "phoneappcommands.hrh"
#include "phonelogger.h"
#include "mphonesecuritymodeobserver.h"

const TInt  KPhoneKeyStart            = 33;
const TInt  KPhoneKeyEnd              = 127;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPhoneNumberEntryManager::CPhoneNumberEntryManager
// ---------------------------------------------------------------------------
//
CPhoneNumberEntryManager::CPhoneNumberEntryManager(
         CPhoneState* aState,
         MPhoneViewCommandHandle& aViewCommandHandle,
         MPhoneStateMachine& aStateMachine,
         MPhoneCustomization* aCustomization,
         CPhoneCbaManager& aCbaManager )
   : iState ( aState ),
     iViewCommandHandle ( aViewCommandHandle ),
     iStateMachine ( aStateMachine ),
     iCustomization ( aCustomization ),
     iCbaManager ( aCbaManager ),
     iEnv( *CEikonEnv::Static() )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::CPhoneNumberEntryManager() ");
    }

 // ---------------------------------------------------------------------------
 // CPhoneNumberEntryManager::NewL
 // ---------------------------------------------------------------------------
 //
CPhoneNumberEntryManager* CPhoneNumberEntryManager::NewL( 
        CPhoneState* aState,
        MPhoneViewCommandHandle& aViewCommandHandle,
        MPhoneStateMachine& aStateMachine,
        MPhoneCustomization* aCustomization,
        CPhoneCbaManager& aCbaManager )
     {
     __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::NewL() ");
     CPhoneNumberEntryManager* self = new (ELeave) CPhoneNumberEntryManager(
             aState,
             aViewCommandHandle,
             aStateMachine,
             aCustomization,
             aCbaManager );
     
     CleanupStack::PushL( self );
     self->ConstructL();
     CleanupStack::Pop( self );
     
     return self;
     }

// ---------------------------------------------------------------------------
// CPhoneNumberEntryManager::ConstructL
// ---------------------------------------------------------------------------
//
void CPhoneNumberEntryManager::ConstructL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::ConstructL() ");
    }

// ---------------------------------------------------------------------------
//  CPhoneNumberEntryManager::~CPhoneNumberEntryManager
// ---------------------------------------------------------------------------
//
CPhoneNumberEntryManager::~CPhoneNumberEntryManager()
     {
     __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::~CPhoneNumberEntryManager() ");
     delete iNumberEntryContent;  
     }
 
// ======== LOCAL FUNCTIONS ========

// -----------------------------------------------------------
// CPhoneNumberEntryManager::StoreNumberEntryContentL
// -----------------------------------------------------------
//
void CPhoneNumberEntryManager::StoreNumberEntryContentL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::StoreNumberEntryContentL( ) ");
    // Store the number entry content to cache
    if ( !iNumberEntryContent )
        {
        iNumberEntryContent = HBufC::NewL( KPhoneNumberEntryBufferSize );
        }    
    TPtr ptr( iNumberEntryContent->Des() );
    ptr.Zero();   
      
    TPhoneCmdParamInteger numberEntryCountParam;
    iViewCommandHandle.ExecuteCommandL( 
            EPhoneViewGetNumberEntryCount,
            &numberEntryCountParam );
    
    TInt neLength( numberEntryCountParam.Integer() );
    if ( neLength > 0 )
        {                    
        TPhoneCmdParamString stringParam;
        stringParam.SetString( &ptr );
        iViewCommandHandle.ExecuteCommandL(
                EPhoneViewGetNumberFromEntry,
                &stringParam );  
        }
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::RestoreNumberEntryContentL
// -----------------------------------------------------------
//
void CPhoneNumberEntryManager::RestoreNumberEntryContentL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::RestoreNumberEntryContentL( ) ");
    if ( iNumberEntryContent )
        {
        // Restore the number entry content from cache
        TPtr ptr( iNumberEntryContent->Des() );            
        iViewCommandHandle.ExecuteCommandL( EPhoneViewSetNumberEntryContent, 0, ptr );
        delete iNumberEntryContent;
        iNumberEntryContent = NULL;
        }
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::IsNumberEntryContentStored
// -----------------------------------------------------------
//
TBool CPhoneNumberEntryManager::IsNumberEntryContentStored()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::IsNumberEntryContentStored( ) ");
    return iNumberEntryContent ? ETrue : EFalse;
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::ClearNumberEntryContentCache
// -----------------------------------------------------------
//
void CPhoneNumberEntryManager::ClearNumberEntryContentCache()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::ClearNumberEntryContentCache( ) ");
    delete iNumberEntryContent;
    iNumberEntryContent = NULL;
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::CreateNumberEntryL
// -----------------------------------------------------------
void CPhoneNumberEntryManager::CreateNumberEntryL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::CreateNumberEntryL( ) ");
    iViewCommandHandle.ExecuteCommandL( EPhoneViewCreateNumberEntry );
    }
 
// -----------------------------------------------------------
// CPhoneNumberEntryManager::SetNumberEntryVisibilityL
// -----------------------------------------------------------
//
void CPhoneNumberEntryManager::SetNumberEntryVisibilityL( TPhoneCmdParamBoolean aVisible )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneNumberEntryManager::SetNumberEntryVisibilityL( ) ");
    __PHONELOG1( EBasic, EPhoneControl, 
            "CPhoneNumberEntryManager::SetNumberEntryVisibilityL : aVisible =%d", 
            aVisible.Boolean() );
    iViewCommandHandle.ExecuteCommandL( EPhoneViewSetNumberEntryVisible, &aVisible );
    if ( aVisible.Boolean() )
        {
        // Set Number Entry CBA
        iCbaManager.UpdateCbaL( EPhoneNumberAcqCBA );
        }
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::HandleCreateNumberEntryL
// add conditions here regarding when NE can be opened:
// - activeidle is not the top most application
// - any query is not visible
// - dialog is not visible ("new call")
// - information note is not visible ("IMEI code", "life timer")
// - options menu bar is not visible
// - "switch to ..." -operation  is not ongoing
// -----------------------------------------------------------
//
void CPhoneNumberEntryManager::HandleCreateNumberEntryL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::HandleCreateNumberEntryL() ");
    
    const TBool autoLockOn = iStateMachine.SecurityMode()->IsSecurityMode();
    const TBool idleVal = CPhonePubSubProxy::Instance()->Value( KPSUidAiInformation, KActiveIdleState );
    const TBool queryActive = iState->IsAnyQueryActiveL();
    const TBool menuBarVisible = iState->IsMenuBarVisibleL();
    
    if( autoLockOn || 
        ( idleVal == EPSAiForeground ) ||
        ( menuBarVisible || queryActive ) )
        {
        __PHONELOG( EBasic, EPhoneControl, 
             "CPhoneNumberEntryManager::HandleCreateNumberEntryL() NE NOT CREATED" );
        __PHONELOG2( EBasic, EPhoneControl, 
             "CPhoneNumberEntryManager::HandleCreateNumberEntryL() autoLcokOn(%d) idleVal(%d)", 
             autoLockOn, idleVal );
        __PHONELOG2( EBasic, EPhoneControl, 
             "CPhoneNumberEntryManager::HandleCreateNumberEntryL() queryActive(%d) menuBarVisible(%d)", 
             queryActive, menuBarVisible );
        }
    else
        {
        //creates NE
        if( ( !iStateMachine.PhoneEngineInfo()->IsSwitchToOperationOngoing () ) &&
            ( !IsNumberEntryUsedL() ) &&
            ( !queryActive || FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) ) )
            {
            __PHONELOG( EBasic, EPhoneControl,
                        "CPhoneNumberEntryManager::HandleCreateNumberEntryL() --create NE" );
            
            if ( iState->IsNoteDismissableL() )
                {
                iViewCommandHandle.ExecuteCommandL( EPhoneViewRemoveNote );
                }
            
            if( iStateMachine.SecurityMode()->IsSecurityMode() )
                {
                iState->HandleCommandL( EPhoneNumberAcqSecurityDialer );
                }
            else
                {
                // Effect is shown when dialer is created.
                iState->BeginTransEffectLC( ENumberEntryCreate );
                iViewCommandHandle.ExecuteCommandL( EPhoneViewCreateNumberEntry );
                iState->EndTransEffect();
                // Go to current state implementation
                iCbaManager.UpdateInCallCbaL();
                iState->UpdateInCallContextMenuL();
                iEnv.SyncNotifyFocusObserversOfChangeInFocus();
                }
            }

        //shows existing NE
        if ( ( !iStateMachine.PhoneEngineInfo()->IsSwitchToOperationOngoing () ) &&
             ( !IsNumberEntryVisibleL() ) &&
             ( !queryActive || FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) ) )
            {
            __PHONELOG( EBasic, EPhoneControl, 
                        "CPhoneNumberEntryManager::HandleCreateNumberEntryL() -- show NE" );
            
            if ( iState->IsNoteDismissableL() )
                {
                iViewCommandHandle.ExecuteCommandL( EPhoneViewRemoveNote );
                }
            
            TPhoneCmdParamBoolean booleanParam;
            booleanParam.SetBoolean( ETrue );
            // Show the number entry
            SetNumberEntryVisibilityL(booleanParam);
            }
        }
    
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::IsNumberEntryUsedL
// -----------------------------------------------------------
//
TBool CPhoneNumberEntryManager::IsNumberEntryUsedL() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneNumberEntryManager::IsNumberEntryUsedL( ) ");
    return iViewCommandHandle.HandleCommandL(
        EPhoneViewGetNumberEntryIsUsedStatus ) ==
        EPhoneViewResponseSuccess;
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::IsNumberEntryVisibleL
// -----------------------------------------------------------
//
TBool CPhoneNumberEntryManager::IsNumberEntryVisibleL() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneNumberEntryManager::IsNumberEntryVisibleL( ) ");
    return iViewCommandHandle.HandleCommandL(
        EPhoneViewGetNumberEntryIsVisibleStatus ) ==
        EPhoneViewResponseSuccess;
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::PhoneNumberFromEntryLC()
// -----------------------------------------------------------
//
HBufC* CPhoneNumberEntryManager::PhoneNumberFromEntryLC() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneNumberEntryManager::PhoneNumberFromEntryLC( ) ");
    HBufC* phoneNumber = HBufC::NewLC( KPhoneNumberEntryBufferSize );
    TPtr ptr( phoneNumber->Des() );
    TPhoneCmdParamString stringParam;
    stringParam.SetString( &ptr );
    iViewCommandHandle.ExecuteCommand(
        EPhoneViewGetNumberFromEntry,
        &stringParam );
    return phoneNumber;
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::HandleNumberEntryEdited
// -----------------------------------------------------------
//
void CPhoneNumberEntryManager::HandleNumberEntryEdited()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneNumberEntryManager::HandleNumberEntryEdited( ) ");
    TBool isNumberEntryVisible( EFalse );
    TRAP_IGNORE( isNumberEntryVisible = IsNumberEntryVisibleL() );

    if( isNumberEntryVisible )
        {
        HBufC* phoneNumber( NULL );
        TRAP_IGNORE( phoneNumber = HBufC::NewL( KPhoneNumberEntryBufferSize ) );
        if( phoneNumber )
            {
            phoneNumber->Des().Zero();
            TPtr ptr( phoneNumber->Des() );
            TPhoneCmdParamString stringParam;
            stringParam.SetString( &ptr );
            TRAP_IGNORE( iViewCommandHandle.ExecuteCommandL(
                    EPhoneViewGetNumberFromEntry,
                    &stringParam ) );
            // Save the phone number
            iStateMachine.PhoneEngineInfo()->SetPhoneNumber( ptr );

            iStateMachine.SendPhoneEngineMessage(
                    MPEPhoneModel::EPEMessagePhoneNumberEdited );

            // remove the phone number
            delete phoneNumber;

            // Set service code flag to view
            TPhoneCmdParamBoolean serviceCodeParam;
            serviceCodeParam.SetBoolean(
                iStateMachine.PhoneEngineInfo()->PhoneNumberIsServiceCode() );
            TRAP_IGNORE( iViewCommandHandle.ExecuteCommandL( 
                    EPhoneViewSetServiceCodeFlag,
                    &serviceCodeParam ) );
            }
        }
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::KeyEventForExistingNumberEntryL
// -----------------------------------------------------------
//
void CPhoneNumberEntryManager::KeyEventForExistingNumberEntryL( 
        const TKeyEvent& aKeyEvent,
        TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneNumberEntryManager::KeyEventForExistingNumberEntryL( ) ");

    TPhoneCmdParamKeyEvent keyEventParam;
    keyEventParam.SetKeyEvent( aKeyEvent );
    keyEventParam.SetEventCode( aEventCode );

    if ( IsValidAlphaNumericKey( aKeyEvent, aEventCode ) )
        {
        iViewCommandHandle.HandleCommandL(
                EPhoneViewSendKeyEventToNumberEntry, &keyEventParam );
        }
    else if ( aKeyEvent.iCode == EKeyBackspace )
        {
        TPhoneViewResponseId resp = iViewCommandHandle.HandleCommandL(
                    EPhoneViewSendKeyEventToNumberEntry, &keyEventParam );

        if ( resp == EPhoneViewResponseNumberEntryCleared )
            {
            // Remove number entry from screen
            iViewCommandHandle.ExecuteCommandL(
                EPhoneViewRemoveNumberEntry );

            // Stop DTMF tone playing just in case (user might be holding down a number
            // key when pressed 'clear' key).
            iStateMachine.SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );

            // Handle state-specific behaviour when number entry is
            // cleared
            HandleNumberEntryClearedL();
            }

        }
    else if( aKeyEvent.iCode == EKeyLeftArrow ||
           aKeyEvent.iCode == EKeyRightArrow ||
           aKeyEvent.iCode == EKeyUpArrow ||
           aKeyEvent.iCode == EKeyDownArrow )
           {
           iViewCommandHandle.HandleCommandL(
               EPhoneViewSendKeyEventToNumberEntry, &keyEventParam );
           }
    }

// -----------------------------------------------------------------------------
// CPhoneNumberEntryManager::IsValidAlphaNumericKey
// Checks is the key event a number, a special character
// or if VoIP is enabled some other character
// -----------------------------------------------------------------------------
//
TBool CPhoneNumberEntryManager::IsValidAlphaNumericKey( 
        const TKeyEvent& aKeyEvent,
        TEventCode aEventCode )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneNumberEntryManager::IsValidAlphaNumericKey( ) ");
    TBool ret(EFalse);
    const TBool numericKeyEntered( CPhoneKeys::IsNumericKey(
          aKeyEvent, aEventCode ) );

    // a numeric key (1,2,3,4,6,7,8,9,0,+,*,p,w )
    // or
    // a letter from fullscreen qwerty, miniqwerty or handwriting
    // when voip is enabled.
    if ( numericKeyEntered
        || IsAlphanumericSupportedAndCharInput( aKeyEvent ) )
        {
        ret= ETrue;
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// CPhoneNumberEntryManager::IsAlphanumericSupportedAndCharInput
// -----------------------------------------------------------------------------
//
TBool CPhoneNumberEntryManager::IsAlphanumericSupportedAndCharInput(
        const TKeyEvent& aKeyEvent ) const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneNumberEntryManager::IsAlphanumericSupportedAndCharInput( ) ");
    TBool ret = ( ( iCustomization &&
                 iCustomization->AllowAlphaNumericMode() ) &&
                 ( ( aKeyEvent.iScanCode >= KPhoneKeyStart &&
                     aKeyEvent.iScanCode <= KPhoneKeyEnd ) ||
                     aKeyEvent.iModifiers & EModifierSpecial ) );
    __PHONELOG1( EBasic, EPhoneControl, 
            "CPhoneNumberEntryManager::IsAlphanumericSupportedAndCharInput: %d", ret );
    return ret;
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::HandleNumberEntryClearedL
// -----------------------------------------------------------
//
void CPhoneNumberEntryManager::HandleNumberEntryClearedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneNumberEntryManager::HandleNumberEntryClearedL( ) ");
    }

// ---------------------------------------------------------
// CPhoneNumberEntryManager::NumberEntryClearL
// ---------------------------------------------------------
//
void CPhoneNumberEntryManager::NumberEntryClearL() const
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::NumberEntryClearL( ) ");
    iViewCommandHandle.ExecuteCommandL( EPhoneViewClearNumberEntryContent );
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::NumberEntryInNumericModeL
// -----------------------------------------------------------
//
TBool CPhoneNumberEntryManager::NumberEntryInNumericModeL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::NumberEntryInNumericModeL( ) ");
    return iViewCommandHandle.HandleCommandL(
        EPhoneViewIsNumberEntryNumericMode ) == EPhoneViewResponseSuccess;
    }

// -----------------------------------------------------------
// CPhoneNumberEntryManager::NumberEntryToggleAlphaNumericModeL
// -----------------------------------------------------------
//
TBool CPhoneNumberEntryManager::NumberEntryToggleAlphaNumericModeL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneNumberEntryManager::NumberEntryToggleAlphaNumericModeL( ) ");
    TPhoneCmdParamBoolean newModeAlpha;
    // Toggle number entry alpha/numeric mode
    iViewCommandHandle.ExecuteCommandL(
        EPhoneViewToggleNumberEntryAlphaNumericMode, &newModeAlpha );
    return newModeAlpha.Boolean();
    }


