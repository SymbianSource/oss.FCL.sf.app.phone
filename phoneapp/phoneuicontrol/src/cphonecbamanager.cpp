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
* Description: Class takes care of CBA's.
*
*/

#include "cphonecbamanager.h"

#include "mpeengineinfo.h"
#include "mphoneviewcommandhandle.h"
#include "mphonestatemachine.h"

#include <featmgr.h>

#include "phonerssbase.h"
#include "phoneui.pan"
#include "phonelogger.h"

#include "cphonestate.h"
#include "cphonecenrepproxy.h"
#include "cphonemainresourceresolver.h"

#include "telephonyvariant.hrh"
#include "phoneui.hrh"

#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamboolean.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPhoneCbaManager::CPhoneCbaManager
// ---------------------------------------------------------------------------
//
CPhoneCbaManager::CPhoneCbaManager(
         CPhoneState* aState,
         MPhoneViewCommandHandle& aViewCommandHandle,
         MPhoneStateMachine& aStateMachine )
   : iState ( aState ), 
     iViewCommandHandle ( aViewCommandHandle ),
     iStateMachine ( aStateMachine ),
     iSoftRejectStatus ( ETrue )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCbaManager::CPhoneCbaManager() ");
    }

 // ---------------------------------------------------------------------------
 // CPhoneCbaManager::NewL
 // ---------------------------------------------------------------------------
 //
CPhoneCbaManager* CPhoneCbaManager::NewL( 
        CPhoneState* aState,
        MPhoneViewCommandHandle& aViewCommandHandle,
        MPhoneStateMachine& aStateMachine )
     {
     __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCbaManager::NewL() ");
     return new (ELeave) CPhoneCbaManager( aState, aViewCommandHandle, aStateMachine );
     }

// ---------------------------------------------------------------------------
//  CPhoneCbaManager::~CPhoneCbaManager
// ---------------------------------------------------------------------------
//
CPhoneCbaManager::~CPhoneCbaManager()
     {
     __LOGMETHODSTARTEND( EPhoneControl, "CPhoneCbaManager::~CPhoneCbaManager() ");
     }
 
// ======== LOCAL FUNCTIONS ========

// -----------------------------------------------------------
// CPhoneCbaManager::UpdateInCallCbaL
// -----------------------------------------------------------
//
void CPhoneCbaManager::UpdateInCallCbaL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneCbaManager::UpdateInCallCbaL() ");
    UpdateCbaL( EPhoneCallHandlingInCallCBA );
    }

// -----------------------------------------------------------
// CPhoneCbaManager::UpdateCbaL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneCbaManager::UpdateCbaL( TInt aResource )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneCbaManager::UpdateCbaL() ");
    __ASSERT_DEBUG( iStateMachine.PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );
    TInt resourceId = aResource;
    TPhoneCmdParamBoolean hold;
    iViewCommandHandle.ExecuteCommandL( EPhoneViewGetHoldFlag, &hold );

    const TBool coverHideSendEndKey =
        CPhoneCenRepProxy::Instance()->IsTelephonyFeatureSupported(
            KTelephonyLVFlagCoverHideSendEndKey );

    const TPEAudioOutput audioOutput =
        iStateMachine.PhoneEngineInfo()->AudioOutput();

    TBool btAvailable = iStateMachine.PhoneEngineInfo()->AudioOutputAvailable(
        EPEBTAudioAccessory );
    
    TPhoneCmdParamBoolean btParam;
    btParam.SetBoolean( audioOutput == EPEBTAudioAccessory );        
    iViewCommandHandle.ExecuteCommand(EPhoneViewSetBlueToothFlag,&btParam);

    SetSoftRejectDimmedFlag();
    
    TPhoneCmdParamBoolean btAvailableParam;
    btAvailableParam.SetBoolean( btAvailable );        
    iViewCommandHandle.ExecuteCommand(
            EPhoneViewSetBluetoothAvailableFlag,&btAvailableParam);

    // Check is Audio muted
    if ( iStateMachine.PhoneEngineInfo()->AudioMute() )
        {
        resourceId = EPhoneCallHandlingInCallUnmuteCBA;
        }
    // Multicall swap
    else if ( EPhoneCallHandlingNewCallSwapCBA == aResource )
        {
        resourceId = EPhoneCallHandlingNewCallSwapCBA;
        }
    // Call waiting cases
    else if ( EPhoneCallHandlingCallWaitingCBA == aResource )
        {
        resourceId = EPhoneCallHandlingCallWaitingCBA;
        if ( iState->IsSwivelClosed() )
            {
            TPhoneCmdParamInteger activeCallCount;
            iViewCommandHandle.ExecuteCommandL(
                    EPhoneViewGetCountOfActiveCalls, &activeCallCount );
            if( activeCallCount.Integer() == EOneActiveCall )
                {
                resourceId = EPhoneCallHandlingWaitingAnswerSwivelClosedCBA;
                }
            else if( activeCallCount.Integer() == ETwoActiveCalls )
                {
                resourceId = EPhoneCallHandlingWaitingReplaceSwivelClosedCBA;
                }
            }
        }
    // Check is call in hold
    else if ( hold.Boolean() )
        {
        resourceId = EPhoneCallHandlingInCallUnholdCBA;
        }
    // Is BT accessory connected
    else if ( audioOutput == EPEBTAudioAccessory &&
              IsVoiceCallAlertingOrActiveL() &&
              !coverHideSendEndKey )
        {
        resourceId = EPhoneCallHandlingInCallHandsetCBA;
        }
    else if ( audioOutput == EPEWiredAudioAccessory ||
              coverHideSendEndKey || iState->IsSwivelClosed() )
        {
        resourceId = EPhoneCallHandlingInCallNoIhfCBA;
        }
    // Check BT availability
    else if ( audioOutput == EPELoudspeaker )
        {
        if ( btAvailable )
            {
            resourceId = EPhoneCallHandlingInCallBtaaCBA;
            }
        else
            {
            resourceId = EPhoneCallHandlingInCallHandsetCBA;
            }
        }
    else if ( audioOutput == EPEHandset &&  btAvailable )
        {
        resourceId = EPhoneCallHandlingInCallBtaaCBA;
        }
    SetCbaL( resourceId );
    }

// -----------------------------------------------------------
// CPhoneCbaManager::UpdateIncomingCbaL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneCbaManager::UpdateIncomingCbaL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneControl,"CPhoneCbaManager::UpdateIncomingCbaL()" );
    const TBool coverHideSendEndKey =
        CPhoneCenRepProxy::Instance()->IsTelephonyFeatureSupported(
            KTelephonyLVFlagCoverHideSendEndKey );

    const TProfileRingingType ringingType =
        iStateMachine.PhoneEngineInfo()->RingingType();

    const TBool callIsAlerting =
            !(( ringingType == EProfileRingingTypeSilent ) ||
              ( ringingType == EProfileRingingTypeBeepOnce ));

    // Update soft reject flag
    iSoftRejectStatus = ETrue;
    // VoIP calls do not support sms sending
    if ( iStateMachine.PhoneEngineInfo()->CallType( aCallId ) == EPECallTypeVoIP 
        || iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ).Length() == 0 )
        {
        iSoftRejectStatus = EFalse;
        }
    
    TPhoneCmdParamBoolean dimSoftRejectParam;
    dimSoftRejectParam.SetBoolean( !iSoftRejectStatus );        
    iViewCommandHandle.ExecuteCommand(
            EPhoneViewSetSoftRejectDimmed, &dimSoftRejectParam );
    
    TInt incomingCbaResourceId;

    if ( iState->IsSwivelClosed() )
        {
        if ( !callIsAlerting || iRingtoneSilenced )
            {
            incomingCbaResourceId = EPhoneCallHandlingIncomingSilentSwivelClosedCBA;
            }
        else
            {
            incomingCbaResourceId = EPhoneCallHandlingIncomingSwivelClosedCBA;
            }
        }
    else if ( coverHideSendEndKey )
        {
        if ( callIsAlerting )
            {
            // Set CBA to Options..Silence
            incomingCbaResourceId = EPhoneCallHandlingIncomingSliderCBA;
            }
        else if ( iSoftRejectStatus )
            {
            // Set CBA for soft reject
            incomingCbaResourceId = EPhoneCallHandlingIncomingSoftRejectSliderCBA;
            }
        else
            {
            // Set CBA to Options..Reject
            incomingCbaResourceId = EPhoneCallHandlingIncomingSilentSliderCBA;
            }
        }
    else
        {
        if ( callIsAlerting )
            {
            incomingCbaResourceId = GetIncomingCallSilenceCBA( iSoftRejectStatus );
            }
        else if ( iSoftRejectStatus )
            {
            // Set CBA for soft reject
            incomingCbaResourceId = EPhoneCallHandlingIncomingSoftRejectCBA;
            }
        else
            {
            incomingCbaResourceId = EPhoneCallHandlingIncomingRejectCBA;
            }
        }
    SetCbaL( incomingCbaResourceId );
    }

// -----------------------------------------------------------
// CPhoneCbaManager::SetCbaL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneCbaManager::SetCbaL( TInt aResource )
    {
    __LOGMETHODSTARTEND(EPhoneControl,"CPhoneCbaManager::SetCbaL()" );
    __PHONELOG1( EBasic, EPhoneControl,
            "CPhoneCbaManager::SetCbaL : %d",aResource );
    if ( EPhoneCallHandlingIncomingRejectCBA == aResource ||
         EPhoneCallHandlingIncomingSoftRejectCBA == aResource )
        {
        SetSoftRejectDimmedFlag();
        }
    
    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger(
        CPhoneMainResourceResolver::Instance()->ResolveResourceID(
        aResource ) );
    iViewCommandHandle.ExecuteCommandL( EPhoneViewUpdateCba,
        &integerParam );
    }

// -----------------------------------------------------------
// CPhoneCbaManager::UpdateCbaSwivelStateChangedL
// -----------------------------------------------------------
//
void CPhoneCbaManager::UpdateCbaSwivelStateChangedL()
    {
    __LOGMETHODSTARTEND(EPhoneControl,"CPhoneCbaManager::UpdateCbaSwivelStateChangedL()" );
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle.HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    TInt incomingCall = callStateData.CallId();

    TPhoneCmdParamInteger activeCallCount;
    iViewCommandHandle.ExecuteCommandL(
            EPhoneViewGetCountOfActiveCalls, &activeCallCount );

    if ( incomingCall > KErrNotFound )
        {
       if( activeCallCount.Integer() == ENoActiveCalls )
            {
            UpdateIncomingCbaL( incomingCall );
            }
        else
            {
            UpdateCbaL( EPhoneCallHandlingCallWaitingCBA );
            }
        }
    else if ( activeCallCount.Integer() != ENoActiveCalls )
        {
        UpdateInCallCbaL();
        }
    }

// -----------------------------------------------------------
// CPhoneCbaManager::IsVoiceCallAlertingOrActiveL
// -----------------------------------------------------------
//
TBool CPhoneCbaManager::IsVoiceCallAlertingOrActiveL()
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneCbaManager::IsVoiceCallAlertingOrActiveL()" );
    TBool retVal = EFalse;
    // Fetch alerting call's id from view
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateConnecting );
    iViewCommandHandle.HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );

    if ( callStateData.CallId() > KErrNotFound )
        {
        TPECallType type = iStateMachine.PhoneEngineInfo()->CallType(
            callStateData.CallId() );
        if ( EPECallTypeCSVoice == type || EPECallTypeVoIP == type )
            {
            retVal = ETrue;
            }
        }
    else
        {
        // Fetch active call's id from view
        TPhoneCmdParamCallStateData callStateData;
        callStateData.SetCallState( EPEStateConnected );
        iViewCommandHandle.HandleCommandL(
            EPhoneViewGetCallIdByState, &callStateData );
        if ( callStateData.CallId() > KErrNotFound )
            {
            TPECallType type = iStateMachine.PhoneEngineInfo()->CallType(
                 callStateData.CallId() );

            if ( EPECallTypeCSVoice == type || EPECallTypeVoIP == type )
                {
                retVal = ETrue;
                }
            }
        }
    return retVal;
    }

// -----------------------------------------------------------
// CPhoneCbaManager::SetRingtoneSilencedStatus
// -----------------------------------------------------------
//
void CPhoneCbaManager::SetRingtoneSilencedStatus( const TBool aSilencedStatus )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneCbaManager::SetRingtoneSilencedStatus ()" );
    // Set the internal TBool variable
    iRingtoneSilenced = aSilencedStatus;
    __PHONELOG1( 
            EBasic, 
            EPhoneControl, 
            "CPhoneCbaManager::SetRingtoneSilencedStatus - iRingtoneSilenced =%d", 
            iRingtoneSilenced  );
    }

// -----------------------------------------------------------
// CPhoneCbaManager::SetSoftRejectStatus
// -----------------------------------------------------------
//
void CPhoneCbaManager::SetSoftRejectStatus( TBool aSoftRejectStatus )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneCbaManager::SetSoftRejectStatus ()" );
    // Set the internal TBool variable
    iSoftRejectStatus = aSoftRejectStatus;
    __PHONELOG1( 
            EBasic, 
            EPhoneControl, 
            "CPhoneCbaManager::SetSoftRejectStatus - iSoftRejectStatus =%d", 
            iSoftRejectStatus  );
    }

// -----------------------------------------------------------
// CPhoneCbaManager::SoftRejectStatus
// -----------------------------------------------------------
//
TBool CPhoneCbaManager::SoftRejectStatus()
    {
    return iSoftRejectStatus;
    }

// -----------------------------------------------------------
// CPhoneCbaManager::GetIncomingCallSilenceCBA
// -----------------------------------------------------------
//
TInt CPhoneCbaManager::GetIncomingCallSilenceCBA( 
        const TBool /*aSoftRejectActivated*/ )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneCbaManager::GetIncomingCallSilenceCBA ()" );
    return EPhoneCallHandlingIncomingCBA;
    }

// -----------------------------------------------------------
// CPhoneCbaManager::SetSoftRejectDimmedFlag
// -----------------------------------------------------------
//
void CPhoneCbaManager::SetSoftRejectDimmedFlag()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneCbaManager::GetIncomingCallSilenceCBA ()" );
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( EPEStateRinging );
    iViewCommandHandle.HandleCommandL(
        EPhoneViewGetCallIdByState, &callStateData );
    TPhoneCmdParamBoolean dimSoftRejectParam;
    if ( callStateData.CallId() > KErrNotFound )
        {
        TBool privateNumber = iStateMachine.PhoneEngineInfo()->
                RemotePhoneNumber( callStateData.CallId() ).Length() == 0;
        
        dimSoftRejectParam.SetBoolean( privateNumber || !iSoftRejectStatus );        
        }
    else
        {
        iSoftRejectStatus = ETrue;
        dimSoftRejectParam.SetBoolean( EFalse );
        }
    iViewCommandHandle.ExecuteCommand(
            EPhoneViewSetSoftRejectDimmed, &dimSoftRejectParam );
    }


