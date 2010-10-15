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
* Description:  Acessory and Bluetooth handler.
*
*/


// INCLUDE FILES
#include    "phonecallutil.h"
#include    "mphonestatemachine.h"
#include    "mphoneviewcommandhandle.h"
#include    "cphonestatehandle.h"
#include    "tphonecmdparamboolean.h"
#include    "tphonecmdparaminteger.h"
#include    "cphonemediatorfactory.h"
#include    "cphonemediatorsender.h"
#include    "cphonereleasecommand.h"
#include    "tphonecmdparamcallstatedata.h" 

#include    <videotelcontrolmediatorapi.h>
#include    <MediatorDomainUIDs.h>
#include    <pevirtualengine.h>
#include    <mpeengineinfo.h>

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------
// PhoneCallUtil::DialMultimediaCall
// ---------------------------------------------------------
//
EXPORT_C void PhoneCallUtil::DialMultimediaCall()
    {    
    MPhoneStateMachine* stateMachine = StateMachine();
    stateMachine->PhoneEngineInfo()->SetCallTypeCommand( EPECallTypeVideo );
    stateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageDial );
    }

// ---------------------------------------------------------
// PhoneCallUtil::DialVoiceCallL
// ---------------------------------------------------------
//
EXPORT_C void PhoneCallUtil::DialVoiceCallL()
    {
    MPhoneStateMachine* stateMachine = StateMachine();
    
    TPhoneCmdParamBoolean globalNotifierParam;
    globalNotifierParam.SetBoolean( ETrue );
    ViewCommandHandle()->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
            &globalNotifierParam );
    stateMachine->PhoneEngineInfo()->SetCallTypeCommand( EPECallTypeCSVoice );
    stateMachine->SendPhoneEngineMessage( MPEPhoneModel::EPEMessageDial );
    }

// -----------------------------------------------------------
// PhoneCallUtil::DisconnectCallL
// -----------------------------------------------------------
//
EXPORT_C TBool PhoneCallUtil::DisconnectCallL()
    {
    MPhoneStateMachine* stateMachine = StateMachine();
    TPhoneCmdParamInteger callIdParam;
    
    ViewCommandHandle()->ExecuteCommand( 
            EPhoneViewGetExpandedBubbleCallId, &callIdParam );
    TBool ret = EFalse;
    if( callIdParam.Integer() > KErrNotFound )
        {
        stateMachine->SetCallId( callIdParam.Integer() );
        if( IsVideoCall( callIdParam.Integer() ) )
            {
            // Video call can be released only after we get response to VT Shutdown Command
            CPhoneMediatorFactory::Instance()->Sender()->IssueCommand( 
                    KMediatorVideoTelephonyDomain,
                             KCatPhoneToVideotelCommands,
                             EVtCmdReleaseDataport,
                   TVersion( KPhoneToVideotelCmdVersionMajor,
                             KPhoneToVideotelCmdVersionMinor,
                             KPhoneToVideotelCmdVersionBuild ),
                   KNullDesC8,
                   CPhoneReleaseCommand::NewL( *stateMachine ) );
            }
        else
            {
            stateMachine->SendPhoneEngineMessage(
                MPEPhoneModel::EPEMessageRelease );
            }
        
        ret = ETrue;
        }

    return ret;
    }


// ---------------------------------------------------------
// PhoneCallUtil::IsVideoCall
// ---------------------------------------------------------
//
EXPORT_C TBool PhoneCallUtil::IsVideoCall( const TInt aCallId )
    {
    if ( aCallId < 0 )
        {
        return EFalse;
        }

    return EPECallTypeVideo == StateMachine()->PhoneEngineInfo()->
                CallType( aCallId );
    }

// ---------------------------------------------------------
// PhoneCallUtil::CallIdByState
// ---------------------------------------------------------
//
EXPORT_C TInt PhoneCallUtil::CallIdByState( const TPEState aCallState )
    {
    TPhoneCmdParamCallStateData callStateData;
    callStateData.SetCallState( aCallState );

    ViewCommandHandle()->ExecuteCommand(
        EPhoneViewGetCallIdByState, &callStateData );

    return callStateData.CallId();
    }

// ---------------------------------------------------------
// PhoneCallUtil::CheckIfCallExists
// ---------------------------------------------------------
//
EXPORT_C TBool PhoneCallUtil::CheckIfCallExists( 
        const TPEState aCallState, const TPECallType aCallType )
    {
    TBool retVal = EFalse;
    TInt callId = CallIdByState( aCallState );

    if ( callId > 0 && 
         aCallType == StateMachine()->PhoneEngineInfo()->
            CallType( callId ) )
        {
        retVal = ETrue;
        }
    
    return retVal;
    }

// -----------------------------------------------------------
// PhoneCallUtil::IsAnyConnectedCalls
// -----------------------------------------------------------
//
EXPORT_C TBool PhoneCallUtil::IsAnyConnectedCalls()
    {
    TBool connectedCall(EFalse);
    MPhoneStateMachine* stateMachine = StateMachine();
    
    if ( stateMachine->PhoneEngineInfo()->CheckIfCallStateExists( 
            EPEStateConnected ) ||
         stateMachine->PhoneEngineInfo()->CheckIfCallStateExists( 
            EPEStateConnecting ) ||
         stateMachine->PhoneEngineInfo()->CheckIfCallStateExists( 
            EPEStateConnectedConference ) )
        {
        connectedCall = ETrue;
        }

    return connectedCall;
    }

// ---------------------------------------------------------
//  PhoneCallUtil::ViewCommandHandle
// ---------------------------------------------------------
//
MPhoneViewCommandHandle* PhoneCallUtil::ViewCommandHandle()
    {   
    return CPhoneStateHandle::Instance()->ViewCommandHandle();
    }

// ---------------------------------------------------------
//  PhoneCallUtil::StateMachine
// ---------------------------------------------------------
//
MPhoneStateMachine* PhoneCallUtil::StateMachine()
    {   
    return CPhoneStateHandle::Instance()->StateMachine();
    }

//  End of File  
