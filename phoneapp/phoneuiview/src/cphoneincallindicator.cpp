/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Incall indicator.
*
*/


// INCLUDE FILES
#include <ctsydomainpskeys.h>
#include <AknIncallBubbleNotify.h>
#include <avkon.hrh>

#include "cphoneincallindicator.h"
#include "cphonerecoverysystem.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamincallindicatordata.h"
#include "phonelogger.h"
#include "phonerssbase.h"
#include "cphonepubsubproxy.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------
// CPhoneIncallIndicator::CPhoneIncallIndicator
// ---------------------------------------------------------
//
CPhoneIncallIndicator::CPhoneIncallIndicator()
    {
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::~CPhoneIncallIndicator
// ---------------------------------------------------------
//
CPhoneIncallIndicator::~CPhoneIncallIndicator()
    {
    CPhoneRecoverySystem::Remove( iUpdateRecoveryId );

    delete iBubble;
    delete iUpdater;
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::NewL
// ---------------------------------------------------------
//
CPhoneIncallIndicator* CPhoneIncallIndicator::NewL()
    {
    CPhoneIncallIndicator* self = new ( ELeave ) CPhoneIncallIndicator();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::ConstructL
// ---------------------------------------------------------
//
void CPhoneIncallIndicator::ConstructL()
    {
    iBubble = CAknIncallBubble::NewL();
    iUpdater = CIdle::NewL( CActive::EPriorityHigh );
    iUpdater->Start( TCallBack( DoUpdate, this ) );

    iUpdateRecoveryId = CPhoneRecoverySystem::Instance()->AddL( 
        TCallBack( DoRecoverUpdateL, this ),
        CTeleRecoverySystem::EPhonePriorityStandard,
        CTeleRecoverySystem::EPhoneStateIdle );   
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::Update
//
// Shows the incall indicator according to current call state.
// ---------------------------------------------------------
//
void CPhoneIncallIndicator::Update(
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamIncallIndicatorData* incallIndicatorParam =
        static_cast<TPhoneCmdParamIncallIndicatorData*>( aCommandParam );

    iCallState = incallIndicatorParam->CallState();
    iMode = incallIndicatorParam->Mode();    
    iMuted = incallIndicatorParam->Mute();
    iCipheringOff = !incallIndicatorParam->Ciphering();
    iCipheringIndicatorAllowed = 
        incallIndicatorParam->CipheringIndicatorAllowed();
    iEmergency = incallIndicatorParam->Emergency();
    iLine2 = incallIndicatorParam->Line2();
    iVisible = incallIndicatorParam->LittleBubbleVisible();

    Update();
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::HandleMuteChange
// ---------------------------------------------------------
//
void CPhoneIncallIndicator::HandleMuteChange( 
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamBoolean* booleanParam =
        static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

    iMuted = booleanParam->Boolean();

    Update();
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::HandleCipheringChange
// ---------------------------------------------------------
//
void CPhoneIncallIndicator::HandleCipheringChange(  
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamBoolean* booleanParam =
        static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

    iCipheringOff = booleanParam->Boolean();

    Update();
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::Update
//
// Updates incall indicator according to current call state.
// ---------------------------------------------------------
//
void CPhoneIncallIndicator::Update()
    {
    TInt state = KPhoneIncallIndicatorDefaultValue;
    TBool sync = ETrue;

    // Put base value appropriate to the state.
    switch ( iCallState )
        {
        case EPSCTsyCallStateDialling:            
            state = EAknStatusBubbleActive;
            break;
			
        case EPSCTsyCallStateRinging:
        case EPSCTsyCallStateAlerting:
        case EPSCTsyCallStateAnswering:
            state = EAknStatusBubbleAlerting;
            sync = EFalse;
            break;

        case EPSCTsyCallStateConnected:
            state = EAknStatusBubbleActive;
            if ( iEmergency )
                {
                state |= EAknStatusBubbleEmergency;
                }
            break;

        case EPSCTsyCallStateHold: // Held
            state = EAknStatusBubbleOnHold;
            break;

        case EPSCTsyCallStateDisconnecting:
            state = EAknStatusBubbleDisconnected;
            break;

        case EPSCTsyCallStateUninitialized:
        case EPSCTsyCallStateNone:
            iVisible = EFalse;
            break;

        default:
            break;
        }

    // Put call mode flag if necessary.
    switch ( iMode )
        {
        case EPSCTsyCallTypeFax:
            state |= EAknStatusBubbleFax;
            break;

        case EPSCTsyCallTypeData:
        case EPSCTsyCallTypeHSCSD:
            state |= EAknStatusBubbleData;
            break;
        case EPSCTsyCallTypeH324Multimedia:
            state |= EAknStatusBubbleVideo;
            break;
        case EPSCTsyCallTypeVoIP:
            state |= EAknStatusBubbleVoIP;
            break;
        case EPSCTsyCallTypeNone:
        case EPSCTsyCallTypeCSVoice: 
        default:
            break;
        }
    
    if ( iMuted )
        {
        state |= EAknStatusBubbleMuted;
        }

    if ( iCipheringOff && iCipheringIndicatorAllowed )
        {
        state |= EAknStatusBubbleNoCiphering;
        }
        
    if ( iLine2 )
        {
        state |= EAknStatusBubbleLine2;   
        }

    if ( iVisible )
        {
        state |= EAknStatusBubbleVisible;
        }

    SetState( state, sync );
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::SetState
//
// Modify only if needed.
// ---------------------------------------------------------
//
void CPhoneIncallIndicator::SetState( 
    TInt aState,
    TBool aSync )
    {
    if ( !aSync )
        {
        if ( aState != iState )
            {
            iState = aState;

            if ( !iUpdater->IsActive() )
                {
                iUpdater->Start( TCallBack( DoUpdate, this ) );
                }   
            }
        }
    else
        {
        if ( aState != iState || iUpdater->IsActive() )
            {
            iState = aState;
            UpdateWithRecovery();
            iUpdater->Cancel();
            }
        }
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::UpdateWithRecovery
// ---------------------------------------------------------
//
void CPhoneIncallIndicator::UpdateWithRecovery()
    {
    CPhoneRecoverySystem::Instance()->RecoverNow( 
        iUpdateRecoveryId, 
        CTeleRecoverySystem::EPhonePriorityStandard );
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::DoRecoverUpdateL
//
// Callback function.
// ---------------------------------------------------------
//
TInt CPhoneIncallIndicator::DoRecoverUpdateL( TAny* aAny )
    {
    CPhoneIncallIndicator* ind = static_cast<CPhoneIncallIndicator*>( aAny );

    ind->iBubble->SetIncallBubbleFlagsL( ind->iState );

    return KErrNone;
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::DoUpdate
//
// Callback function.
// ---------------------------------------------------------
//
TInt CPhoneIncallIndicator::DoUpdate( TAny* aAny )
    {
    static_cast<CPhoneIncallIndicator*>( aAny )->UpdateWithRecovery();
    
    return KErrNone;
    }

// ---------------------------------------------------------
// CPhoneIncallIndicator::IsEmergencyCall
// ---------------------------------------------------------
//
TBool CPhoneIncallIndicator::IsEmergencyCall()
    {
    return iEmergency;
    }

//  End of File  

