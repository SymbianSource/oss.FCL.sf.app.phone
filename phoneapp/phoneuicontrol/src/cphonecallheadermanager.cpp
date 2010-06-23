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
* Description:  Bubble manager wrapper.
*
*/

#include <mpeengineinfo.h>
#include <StringLoader.h>
#include "cphonecallheadermanager.h"
#include "tphonecallheaderparam.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamcallheaderdata.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "phoneui.pan"
#include "phoneviewcommanddefinitions.h"
#include "phonelogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPhoneCallHeaderManager::CPhoneCallHeaderManager
// ---------------------------------------------------------------------------
//
CPhoneCallHeaderManager::CPhoneCallHeaderManager(
         MPhoneViewCommandHandle& aViewCommandHandle,
         MPhoneStateMachine& aStateMachine,
         MPhoneCustomization* aCustomization )
   : iViewCommandHandle ( aViewCommandHandle ),
     iStateMachine ( aStateMachine ),
     iCustomization ( aCustomization )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::CPhoneCallHeaderManager() ");
    }

 // ---------------------------------------------------------------------------
 // CPhoneCallHeaderManager::NewL
 // ---------------------------------------------------------------------------
 //
CPhoneCallHeaderManager* CPhoneCallHeaderManager::NewL( 
        MPhoneViewCommandHandle& aViewCommandHandle,
        MPhoneStateMachine& aStateMachine,
        MPhoneCustomization* aCustomization )
     {
     __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::NewL() ");
     CPhoneCallHeaderManager* self = new (ELeave) CPhoneCallHeaderManager(
             aViewCommandHandle,
             aStateMachine,
             aCustomization );
     
     CleanupStack::PushL( self );
     self->ConstructL();
     CleanupStack::Pop( self );
     
     return self;
     }

// ---------------------------------------------------------------------------
// CPhoneCallHeaderManager::ConstructL
// ---------------------------------------------------------------------------
//
void CPhoneCallHeaderManager::ConstructL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::ConstructL() ");
    if ( !iCallHeaderParam )
        {
        iCallHeaderParam = new (ELeave) TPhoneCallHeaderParam ( *this, iStateMachine );
        }
    }

// ---------------------------------------------------------------------------
//  CPhoneCallHeaderManager::~CPhoneCallHeaderManager
// ---------------------------------------------------------------------------
//
CPhoneCallHeaderManager::~CPhoneCallHeaderManager()
     {
     __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::~CPhoneCallHeaderManager() ");
     delete iCallHeaderParam;
     }
 
// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPhoneCallHeaderManager::LoadResource
// ---------------------------------------------------------------------------
//
void CPhoneCallHeaderManager::LoadResource( TDes& aData, const TInt aResource )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::LoadResource() ");
    __PHONELOG1( EBasic, EPhoneControl, "CPhoneCallHeaderManager::LoadResource - aResource: %d", aResource );
    StringLoader::Load(  
            aData, CPhoneMainResourceResolver::Instance()->ResolveResourceID( aResource ), 
            CCoeEnv::Static() );
    }

// ---------------------------------------------------------------------------
//  CPhoneCallHeaderManager::UpdateCallHeaderInfoL
// ---------------------------------------------------------------------------
//
TPhoneCmdParamCallHeaderData CPhoneCallHeaderManager::UpdateCallHeaderInfoL( 
        const TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::UpdateCallHeaderInfoL() ");
    __ASSERT_DEBUG( iStateMachine.PhoneEngineInfo(), Panic( EPhoneCtrlInvariant ) );
    TPhoneCmdParamCallHeaderData callHeaderParam;
    iCallHeaderParam->UpdateCallHeaderInfoL( 
                aCallId, 
                IsWaitingCallL( aCallId ), 
                IsVideoCall( aCallId ), 
                &callHeaderParam );

    callHeaderParam.SetCiphering(
        iStateMachine.PhoneEngineInfo()->IsSecureCall( aCallId ) );
    
    callHeaderParam.SetCipheringIndicatorAllowed(
        iStateMachine.PhoneEngineInfo()->SecureSpecified() );

    return callHeaderParam;    
    }

// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::GetRemoteInfoDataL
// ---------------------------------------------------------------------------
//
void CPhoneCallHeaderManager::GetRemoteInfoDataL( 
        const TInt aCallId, 
        TDes& aData ) const 
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::GetRemoteInfoDataL() ");
    iCallHeaderParam->GetRemoteInfoDataL( aCallId, aData);
    }

// ---------------------------------------------------------------------------
//  CPhoneCallHeaderManager::SetCallHeaderTextsForCallComingIn
// ---------------------------------------------------------------------------
//
void CPhoneCallHeaderManager::SetCallHeaderTextsForCallComingIn(
        const TInt aCallId, 
        const TBool aWaitingCall,
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneCallHeaderManager::SetCallHeaderTextsForCallComingIn() ");
    __ASSERT_DEBUG( iStateMachine.PhoneEngineInfo(), Panic( EPhoneCtrlInvariant ) );
     
    iCallHeaderParam->SetCallHeaderTexts( 
            aCallId, 
            aWaitingCall, 
            IsVideoCall( aCallId ), 
            aCallHeaderData );
    }

// ---------------------------------------------------------------------------
//  CPhoneCallHeaderManager::DisplayHeaderForCallComingInL
// ---------------------------------------------------------------------------
//
void CPhoneCallHeaderManager::DisplayHeaderForCallComingInL(
        const TInt aCallId, 
        const TBool aWaitingCall )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneCallHeaderManager::DisplayHeaderForCallComingInL() ");
    __ASSERT_DEBUG( iStateMachine.PhoneEngineInfo(), Panic( EPhoneCtrlInvariant ) );
    TPhoneCmdParamCallHeaderData callHeaderParam;
    
    iCallHeaderParam->SetIncomingCallHeaderParamsL( 
            aCallId, 
            aWaitingCall, 
            IsVideoCall( aCallId ), 
            &callHeaderParam );

    if( iCustomization )
        {
        // to do modifications to call header parameters
        // if VoIP call in question
        iCustomization->ModifyCallHeaderTexts( aCallId, &callHeaderParam,
            KNullDesC );
        }

    // Display call header
    iViewCommandHandle.ExecuteCommandL( 
            EPhoneViewCreateCallHeader, 
            aCallId, 
            &callHeaderParam );    
    }

// ---------------------------------------------------------------------------
//  CPhoneCallHeaderManager::DisplayHeaderForOutgoingCallL
// ---------------------------------------------------------------------------
//
void CPhoneCallHeaderManager::DisplayHeaderForOutgoingCallL( const TInt aCallId )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::DisplayHeaderForOutgoingCallL() ");
    __ASSERT_DEBUG( iStateMachine.PhoneEngineInfo(), Panic( EPhoneCtrlInvariant ) );
    TPhoneCmdParamCallHeaderData callHeaderParam;
    
    iCallHeaderParam->SetOutgoingCallHeaderParamsL( 
            aCallId,
            &callHeaderParam );
            
    if( iCustomization )
        {
        // to do modifications to call header parameters
        // if VoIP call in question
        iCustomization->ModifyCallHeaderTexts( aCallId, &callHeaderParam,
            KNullDesC );
        }
    
    // Display call header for outgoing call
    iViewCommandHandle.ExecuteCommandL( 
            EPhoneViewCreateCallHeader, 
            aCallId, 
            &callHeaderParam );
    }

// ---------------------------------------------------------------------------
//  CPhoneCallHeaderManager::IsWaitingCallL
// ---------------------------------------------------------------------------
//
TBool CPhoneCallHeaderManager::IsWaitingCallL( const TInt aCallId  ) const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneCallHeaderManager::IsWaitingCallL() ");
    TBool waiting( EFalse );
    if ( iStateMachine.PhoneEngineInfo()->CallState( aCallId ) == EPEStateRinging )
        {
        TPhoneCmdParamInteger activeCallCount;
        iViewCommandHandle.ExecuteCommandL( 
                EPhoneViewGetCountOfActiveCalls, 
                &activeCallCount );
        // if there is other calls then ringing call is waiting call.
        if( activeCallCount.Integer() > 0 )
            {
            waiting = ETrue;
            }
        }
    __PHONELOG1( EBasic, EPhoneControl, "CPhoneCallHeaderManager::IsWaitingCallL - waiting: %d", waiting );
    return waiting;
    }

// ---------------------------------------------------------------------------
//  CPhoneCallHeaderManager::IsVideoCall
// ---------------------------------------------------------------------------
//
TBool CPhoneCallHeaderManager::IsVideoCall( const TInt aCallId ) const
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::IsVideoCall() ");
    if( aCallId < 0 )
        {
        // Illegal call id, check call type command
        return ( iStateMachine.PhoneEngineInfo()->CallTypeCommand()
            == EPECallTypeVideo );  
        }
    return ( iStateMachine.PhoneEngineInfo()->CallType( aCallId )
            == EPECallTypeVideo );
    }

// ---------------------------------------------------------------------------
//  CPhoneCallHeaderManager::SetCallHeaderType
// ---------------------------------------------------------------------------
//
void CPhoneCallHeaderManager::SetCallHeaderType( 
    TInt aCallHeaderType )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::SetCallHeaderType() ");
    iCallHeaderParam->SetCallHeaderType( aCallHeaderType ); 
    }

// ---------------------------------------------------------------------------
//  CPhoneCallHeaderManager::~CPhoneCallHeaderManager
// ---------------------------------------------------------------------------
//
TInt CPhoneCallHeaderManager::CallHeaderType() const
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::CallHeaderType() ");
    return iCallHeaderParam->CallHeaderType();            
    }

// ---------------------------------------------------------------------------
//  CPhoneCallHeaderManager::SetDivertIndication
// ---------------------------------------------------------------------------
//
void CPhoneCallHeaderManager::SetDivertIndication( const TBool aDivertIndication )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCallHeaderManager::SetDivertIndication()");
    iCallHeaderParam->SetDivertIndication( aDivertIndication );           
    }

// ---------------------------------------------------------------------------
// CPhoneCallHeaderManager::SetPhoneNumberAvailabilityL
// ---------------------------------------------------------------------------
//
void CPhoneCallHeaderManager::SetPhoneNumberAvailabilityL( 
        const TInt aNumberLength,
        const TBool aContactInfoAvailable )
    {
    __LOGMETHODSTARTEND( 
            EPhoneControl, 
            "CPhoneCallHeaderManager::SetPhoneNumberAvailabilityL() ");
    TPhoneCmdParamBoolean phoneNumberAvailable;
    if( aNumberLength || aContactInfoAvailable )
        {
        // Phone number is available straight or via contact info
        // so switch to video/voice call is possible
        __PHONELOG( EBasic, 
                EPhoneControl, 
                "CPhoneCallHeaderManager::SetPhoneNumberAvailabilityL - Phonenumber is available" );
        phoneNumberAvailable.SetBoolean( ETrue );
        }
    else
        {
        __PHONELOG( EBasic, 
                EPhoneControl, 
                "CPhoneCallHeaderManager::SetPhoneNumberAvailabilityL - Phonenumber is not available" );
        phoneNumberAvailable.SetBoolean( EFalse );              
        }
    iViewCommandHandle.ExecuteCommandL( 
            EPhoneViewSetPhoneNumberAvailableInPhoneEngine, 
            &phoneNumberAvailable );        
    }

// -----------------------------------------------------------
// CPhoneCallHeaderManager::LoadCallHeaderTexts
// -----------------------------------------------------------
//
void CPhoneCallHeaderManager::LoadCallHeaderTexts(
    const TInt aLabelId, 
    const TInt aShortLabelId,
    TPhoneCmdParamCallHeaderData* aCallHeaderData ) 
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneCallHeaderManager::LoadCallHeaderTexts( ) ");
    if ( aLabelId != EPhoneStringList )
        {
        TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
        LoadResource( labelText, aLabelId );
        aCallHeaderData->SetLabelText( labelText );

        if ( aShortLabelId != EPhoneStringList )
            {
            TBuf<KPhoneCallHeaderLabelMaxLength> shortLabelText( KNullDesC );
            LoadResource( shortLabelText, aShortLabelId );       
            aCallHeaderData->SetShortLabelText( shortLabelText );        
            }
        }
    }

