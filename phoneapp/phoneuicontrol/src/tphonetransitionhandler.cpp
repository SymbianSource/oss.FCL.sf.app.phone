/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class handles phoneapp internal/external transitions.
*
*/

#include "tphonetransitionhandler.h"
#include "cphonestate.h"
#include "phonelogger.h"

// ---------------------------------------------------------------------------
// TPhoneTransitionHandler::TPhoneTransitionHandler
// ---------------------------------------------------------------------------
//
TPhoneTransitionHandler::TPhoneTransitionHandler(
        CPhoneState& aState,
        MPhoneViewCommandHandle& aViewCommandHandle )
   : iState ( aState ), 
     iViewCommandHandle ( aViewCommandHandle )
    {
    __LOGMETHODSTARTEND( EPhoneControl, 
            "TPhoneTransitionHandler::TPhoneTransitionHandler( ) ");
    }

// -----------------------------------------------------------------------------
// TPhoneTransitionHandler::BeginUiUpdateLC
// -----------------------------------------------------------------------------
//
EXPORT_C void TPhoneTransitionHandler::BeginUiUpdateLC()
    {
    __LOGMETHODSTARTEND( EPhoneControl, 
            "TPhoneTransitionHandler::BeginUiUpdateLC( ) ");
    iViewCommandHandle.ExecuteCommand( EPhoneViewBeginUpdate );
    TCleanupItem operation( UiUpdateCleanup, this );
    CleanupStack::PushL( operation );
    }

// -----------------------------------------------------------------------------
// TPhoneTransitionHandler::EndUiUpdate
// -----------------------------------------------------------------------------
//
EXPORT_C void TPhoneTransitionHandler::EndUiUpdate()
    {
    __LOGMETHODSTARTEND( EPhoneControl, 
            "TPhoneTransitionHandler::EndUiUpdate( ) ");
    CleanupStack::PopAndDestroy(); // Call UiUpdateCleanup
    }

// -----------------------------------------------------------------------------
// TPhoneTransitionHandler::IncomingCallUiUpdateLC
// -----------------------------------------------------------------------------
//
EXPORT_C void TPhoneTransitionHandler::IncomingCallUiUpdateLC()
    {
    __LOGMETHODSTARTEND( EPhoneControl, 
            "TPhoneTransitionHandler::IncomingCallUiUpdateLC( ) ");
    iState.IsNumberEntryUsedL() ?
        BeginTransEffectLC( EPhoneTransEffectCallUiAppear ) :
        BeginTransEffectLC( EPhoneTransEffectPhoneUiOpen );
    BeginUiUpdateLC();
    }

// -----------------------------------------------------------------------------
// TPhoneTransitionHandler::IdleCallUiUpdateLC
// -----------------------------------------------------------------------------
//
EXPORT_C void TPhoneTransitionHandler::IdleCallUiUpdateLC()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "TPhoneTransitionHandler::IdleCallUiUpdateLC( ) ");
    iState.IsNumberEntryUsedL() ?
        BeginTransEffectLC( EPhoneTransEffectCallUiDisappear ) :
        BeginTransEffectLC( EPhoneTransEffectPhoneUiClose );
    BeginUiUpdateLC();
    }

// -----------------------------------------------------------------------------
// TPhoneTransitionHandler::EndUiUpdateAndEffect
// -----------------------------------------------------------------------------
//
EXPORT_C void TPhoneTransitionHandler::EndUiUpdateAndEffect()
    {
    __LOGMETHODSTARTEND( EPhoneControl, 
            "TPhoneTransitionHandler::EndUiUpdateAndEffect( ) ");
    EndUiUpdate();
    EndTransEffect();
    }

// -----------------------------------------------------------------------------
// TPhoneTransitionHandler::BeginTransEffectLC
// -----------------------------------------------------------------------------
//
EXPORT_C void TPhoneTransitionHandler::BeginTransEffectLC( TPhoneTransEffectType aType )
    {
    __LOGMETHODSTARTEND( EPhoneControl, 
            "TPhoneTransitionHandler::BeginTransEffectLC( ) ");
    TPhoneCmdParamTransEffect effectParam;
    effectParam.SetType( aType );
    effectParam.SetAppUid( KUidPhoneApplication );
    iViewCommandHandle.ExecuteCommand( 
            EPhoneViewBeginTransEffect, 
            &effectParam );
    // Always put the cleanup item into stack as expected by the caller.
    TCleanupItem operation( EffectCleanup, this );
    CleanupStack::PushL( operation );
    }

// -----------------------------------------------------------------------------
// TPhoneTransitionHandler::EndTransEffect
// -----------------------------------------------------------------------------
//
EXPORT_C void TPhoneTransitionHandler::EndTransEffect()
    {
    __LOGMETHODSTARTEND( EPhoneControl,
            "TPhoneTransitionHandler::EndTransEffect( ) ");
    CleanupStack::PopAndDestroy(); // Call EffectCleanup
    }

// -----------------------------------------------------------------------------
// TPhoneTransitionHandler::UiUpdateCleanup
// -----------------------------------------------------------------------------
//
void TPhoneTransitionHandler::UiUpdateCleanup(TAny* aThis )
    {
    __LOGMETHODSTARTEND( EPhoneControl, 
            "TPhoneTransitionHandler::UiUpdateCleanup( ) ");
    static_cast<TPhoneTransitionHandler*>( aThis )->
            iViewCommandHandle.ExecuteCommand( EPhoneViewEndUpdate );
    }

// -----------------------------------------------------------------------------
// TPhoneTransitionHandler::EffectCleanup
// -----------------------------------------------------------------------------
//
void TPhoneTransitionHandler::EffectCleanup(TAny* aThis )
    {
    __LOGMETHODSTARTEND( EPhoneControl, 
            "TPhoneTransitionHandler::EffectCleanup( ) ");
    TPhoneCmdParamTransEffect effectParam;
    effectParam.SetType( EPhoneTransEffectStop );
    // won't do anything if effect wasn't started
    static_cast<TPhoneTransitionHandler*>( aThis )->
            iViewCommandHandle.ExecuteCommand( 
                    EPhoneViewEndTransEffect, &effectParam );
    }

