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
    const TBool numericKeyEntered( 
            CPhoneKeys::IsNumericKey( aKeyEvent, aEventCode ) );
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

