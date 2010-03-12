/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of CPhoneSystemEventHandler class.
*
*/


// INCLUDE FILES
#include <telinformationpskeys.h>
#include <telephonyvariant.hrh>
#include <UikonInternalPSKeys.h>
#include <startupdomainpskeys.h>
#include <hwrmdomainpskeys.h>
#include <oem/SimLockUiKeys.h>
#include <featmgr.h>
#include <coreapplicationuisdomainpskeys.h>

#include "cphonesystemeventhandler.h"
#include "mphonestate.h"
#include "mphonestatemachine.h"
#include "cphonecenrepproxy.h"
#include "cphonepubsubproxy.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPhoneSystemEventHandler::CPhoneSystemEventHandler
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPhoneSystemEventHandler::CPhoneSystemEventHandler(
    MPhoneStateMachine* aStateMachine ) :
    iStateMachine( aStateMachine )
    {
    }

// -----------------------------------------------------------
// CPhoneSystemEventHandler::NewL()
// Two-phased constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneSystemEventHandler* CPhoneSystemEventHandler::NewL(
    MPhoneStateMachine* aStateMachine )
    {
    CPhoneSystemEventHandler* self =
        new (ELeave) CPhoneSystemEventHandler( aStateMachine );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------
// CPhoneSystemEventHandler::ConstructL()
// EPOC default constructor can leave.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPhoneSystemEventHandler::ConstructL()
    {
    // Set up notifications for call state values
    CPhonePubSubProxy::Instance()->NotifyChangeL(
        KPSUidCtsyCallInformation,
        KCTsyCallState,
        this );

    // Set up notifications for displaying Telephony info on the title pane
    CPhonePubSubProxy::Instance()->NotifyChangeL(
        KPSUidTelInformation,
        KTelDisplayInfo,
        this );

    // Set up notifications for Sim security state values.
    CPhonePubSubProxy::Instance()->NotifyChangeL(
        KPSUidStartup,
        KStartupSimSecurityStatus,
        this );

    if ( CPhoneCenRepProxy::Instance()->IsTelephonyFeatureSupported(
            KTelephonyLVFlagSwivelInDevice ))
        {
        // Set up notifications for Swivel state.
        CPhonePubSubProxy::Instance()->NotifyChangeL(
            KPSUidHWRM,
            KHWRMGripStatus,
            this );
        }
    
    if ( FeatureManager::FeatureSupported( KFeatureIdFfSimlockUi ) )
       {
        //Set up notifications for Sim Lock Dialogs values.
        CPhonePubSubProxy::Instance()->NotifyChangeL(
            KSimLockProperty,
            ESimLockActiveStatus,
            this );
       }

    if ( FeatureManager::FeatureSupported( KFeatureIdFfEntryPointForVideoShare ) )
        {
        // Set up notifications for Video Share (un)availability
        CPhonePubSubProxy::Instance()->NotifyChangeL(
            KPSUidCoreApplicationUIs,
            KCoreAppUIsVideoSharingIndicator,
            this );
        }
    }

// ---------------------------------------------------------
// CPhoneSystemEventHandler::CPhoneSystemEventHandler
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPhoneSystemEventHandler::~CPhoneSystemEventHandler()
    {
    }

// ---------------------------------------------------------
// CPhoneSystemEventHandler::DynInitMenuPaneL
// ---------------------------------------------------------
//
void CPhoneSystemEventHandler::DynInitMenuPaneL(
    TInt aResourceId,
    CEikMenuPane* aMenuPane )
    {
    iStateMachine->State()->DynInitMenuPaneL( aResourceId, aMenuPane );
    }

// ---------------------------------------------------------
// CPhoneSystemEventHandler::DynInitMenuBarL
// ---------------------------------------------------------
//
void CPhoneSystemEventHandler::DynInitMenuBarL(
    TInt aResourceId,
    CEikMenuBar* aMenuBar )
    {
    iStateMachine->State()->DynInitMenuBarL( aResourceId, aMenuBar );
    }

// ---------------------------------------------------------
// CPhoneSystemEventHandler::HandleSystemEventL
// ---------------------------------------------------------
//
void CPhoneSystemEventHandler::HandleSystemEventL( const TWsEvent& aEvent )
    {
    iStateMachine->State()->HandleSystemEventL( aEvent );
    }

// ---------------------------------------------------------
// CPhoneSystemEventHandler::HandleForegroundEventL
// ---------------------------------------------------------
//
void CPhoneSystemEventHandler::HandleForegroundEventL( TBool aForeground )
    {
    iStateMachine->State()->HandleForegroundEventL( aForeground );
    }

// ---------------------------------------------------------
// CPhoneSystemEventHandler::HandlePhoneForegroundEventL
// ---------------------------------------------------------
//
void CPhoneSystemEventHandler::HandlePhoneForegroundEventL()
    {
    iStateMachine->State()->HandlePhoneForegroundEventL();
    }

 // ---------------------------------------------------------
// CPhoneSystemEventHandler::HandlePhoneFocusLostEventL
// ---------------------------------------------------------
//
void CPhoneSystemEventHandler::HandlePhoneFocusLostEventL()
    {
    iStateMachine->State()->HandlePhoneFocusLostEventL();
    }
// ---------------------------------------------------------
// CPhoneSystemEventHandler::HandleIdleForegroundEventL
// ---------------------------------------------------------
//
void CPhoneSystemEventHandler::HandleIdleForegroundEventL()
    {
    iStateMachine->State()->HandleIdleForegroundEventL();
    }

// ---------------------------------------------------------
// CPhoneSystemEventHandler::HandleKeyLockEnabled
// ---------------------------------------------------------
//
void CPhoneSystemEventHandler::HandleKeyLockEnabled( TBool aKeylockEnabled )
    {
    iStateMachine->State()->HandleKeyLockEnabled( aKeylockEnabled );
    }

// -----------------------------------------------------------
// CPhoneSystemEventHandler::HandleEnvironmentChangeL
// -----------------------------------------------------------
//
void CPhoneSystemEventHandler::HandleEnvironmentChangeL( const TInt aChanges )
    {
    iStateMachine->State()->HandleEnvironmentChangeL( aChanges );
    }

// ---------------------------------------------------------
// CPhoneSystemEventHandler::HandlePhoneStartupL
// ---------------------------------------------------------
//
void CPhoneSystemEventHandler::HandlePhoneStartupL()
    {
    iStateMachine->State()->HandlePhoneStartupL();
    }

// -----------------------------------------------------------
// CPhoneSystemEventHandler::HandlePropertyChangedL
// -----------------------------------------------------------
//
void CPhoneSystemEventHandler::HandlePropertyChangedL(
    const TUid& aCategory,
    const TUint aKey,
    const TInt aValue )
    {
    iStateMachine->State()->HandlePropertyChangedL( aCategory, aKey, aValue );
    }

// -----------------------------------------------------------
// CPhoneSystemEventHandler::HandleCenRepChangeL
// -----------------------------------------------------------
//
void CPhoneSystemEventHandler::HandleCenRepChangeL(
    const TUid& aUid,
    const TUint aId )
    {
    iStateMachine->State()->HandleCenRepChangeL( aUid, aId );
    }

//  End of File
