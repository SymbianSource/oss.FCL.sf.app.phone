/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneGeneralGsmMessagesHandler class.
*
*/


// INCLUDES
#ifdef __SYNCML_DM
#include <apgcli.h>
#endif
#include <mpeengineinfo.h>
#include <UikonInternalPSKeys.h>
#include <mccessobserver.h>
#include <featmgr.h>
#include <StringLoader.h>

#include "phoneconstants.h"
#include "phonerssbase.h"
#include "cphonegeneralgsmmessageshandler.h"
#include "phonelogger.h"
#include "mphonestatemachine.h"
#include "mphoneviewcommandhandle.h"
#include "phoneui.pan"
#include "cphonepubsubproxy.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamnote.h"
#include "tphonecmdparamappinfo.h"
#include "cphonemainresourceresolver.h"
#include "tphonecmdparaminteger.h"


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneGeneralGsmMessagesHandler::CPhoneGeneralGsmMessagesHandler( 
    MPhoneStateMachine& aStateMachine, 
    MPhoneViewCommandHandle& aViewCommandHandle,
    MPhoneState& aActiveState ) : 
    iStateMachine( aStateMachine ),
    iViewCommandHandle( aViewCommandHandle ),
    iActiveState( aActiveState )
    {
    }

// -----------------------------------------------------------
// CPhoneGeneralGsmMessagesHandler::~CPhoneGeneralGsmMessagesHandler()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------

CPhoneGeneralGsmMessagesHandler::~CPhoneGeneralGsmMessagesHandler()
    {
    }

// -----------------------------------------------------------
// CPhoneGeneralGsmMessagesHandler::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneGeneralGsmMessagesHandler* CPhoneGeneralGsmMessagesHandler::NewL( 
    MPhoneStateMachine& aStateMachine, 
    MPhoneViewCommandHandle& aViewCommandHandle,
    MPhoneState& aActiveState )
    {
    CPhoneGeneralGsmMessagesHandler* self = new( ELeave ) 
        CPhoneGeneralGsmMessagesHandler( aStateMachine, 
                                         aViewCommandHandle,
                                         aActiveState );
    
    return self;
    }

// -----------------------------------------------------------
// CPhoneGeneralGsmMessagesHandler::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
void CPhoneGeneralGsmMessagesHandler::HandlePhoneEngineMessageL( 
    const TInt aMessage, 
    TInt /*aCallId*/ )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneGeneralGsmMessagesHandler::HandlePhoneEngineMessageL()" );

    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageCallBarred:
            SendGlobalInfoNoteL( EPhoneActiveBarrings, ETrue );
            break;
        
        case MEngineMonitor::EPEMessageIncCallIsForw:
            HandleIncomingCallForwardedL();
            break;
            
        case MEngineMonitor::EPEMessageIncCallForwToC:
            SendGlobalInfoNoteL( EPhoneMtCallDiverting, ETrue );
            break;
            
        case MEngineMonitor::EPEMessageOutCallForwToC:
            SendGlobalInfoNoteL( EPhoneDiverting, ETrue );
            break;

        case MEngineMonitor::EPEMessageIssuedSSRequest:
            {
            __PHONELOG(
                EBasic, 
                EPhoneUIStates,
                "CPhoneGeneralGsmMessagesHandler::EPEMessageIssuedSSRequest" );
            break;
            }
            
        case MEngineMonitor::EPEMessageIssuingSSRequest:
            {
            __PHONELOG(
                EBasic, 
                EPhoneUIStates,
                "CPhoneGeneralGsmMessagesHandler::EPEMessageIssuingSSRequest" );
                    
            iStateMachine.SendPhoneEngineMessage( MPEPhoneModel::EPEMessageEndDTMF );
             
            // Remove number entry from screen
            if ( !FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) &&
                 iViewCommandHandle.HandleCommandL( 
                     EPhoneViewGetNumberEntryIsVisibleStatus ) == 
                     EPhoneViewResponseSuccess )  
                {            
                iViewCommandHandle.ExecuteCommandL( 
                    EPhoneViewRemoveNumberEntry );
                }
            else if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) )
                {
                iViewCommandHandle.ExecuteCommandL( 
                    EPhoneViewClearNumberEntryContent );
                }

            // Enable global notes
            TPhoneCmdParamBoolean globalNotifierParam;
            globalNotifierParam.SetBoolean( EFalse );
            iViewCommandHandle.ExecuteCommandL( 
                EPhoneViewSetGlobalNotifiersDisabled,
                &globalNotifierParam );
            
            // Get active call count
            TPhoneCmdParamInteger activeCallCount;
            iViewCommandHandle.ExecuteCommandL(
                EPhoneViewGetCountOfActiveCalls, &activeCallCount );
            
            // Remove phoneumber query
            iViewCommandHandle.ExecuteCommandL( EPhoneViewRemoveQuery );
            break;
            }

        case MEngineMonitor::EPEMessageTempClirActivationUnsuccessful:
            SendGlobalErrorNoteL( EPhoneSSNotifCLIRSupprReject, ETrue );
            break;
            
        case MEngineMonitor::EPEMessageForwardUnconditionalModeActive:
            SendGlobalInfoNoteL( EPhoneAllIncomingCallsDiverted, ETrue );
            break;
            
        case MEngineMonitor::EPEMessageForwardConditionallyModeActive:
            SendGlobalInfoNoteL( EPhoneActiveDiverts, ETrue );
            break;

        default:
            break;
        }
    }

// ---------------------------------------------------------
// CPhoneGeneralGsmMessagesHandler::SendGlobalInfoNoteL
// ---------------------------------------------------------
//
void CPhoneGeneralGsmMessagesHandler::SendGlobalInfoNoteL( 
        TInt aResourceId, TBool aNotificationDialog )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneGeneralGsmMessagesHandler::SendGlobalInfoNoteL()" );
    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );
    if ( CPhonePubSubProxy::Instance()->Value( 
            KPSUidUikon, KUikGlobalNotesAllowed ) == 1 )
        {
        // Re-enable global notes
        TPhoneCmdParamBoolean globalNotifierParam;
        globalNotifierParam.SetBoolean( EFalse );
        iViewCommandHandle.ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
            &globalNotifierParam );
            
        TPhoneCmdParamGlobalNote globalNoteParam;
      
        globalNoteParam.SetType( EAknGlobalInformationNote );
        globalNoteParam.SetTextResourceId( 
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( aResourceId ) );
        globalNoteParam.SetTone( EAvkonSIDInformationTone );
        globalNoteParam.SetNotificationDialog( aNotificationDialog );

        iViewCommandHandle.ExecuteCommandL( 
            EPhoneViewShowGlobalNote, &globalNoteParam );    
        }
    }

// -----------------------------------------------------------
// CPhoneGeneralGsmMessagesHandler::HandleIncomingCallForwardedL
// -----------------------------------------------------------
//
void CPhoneGeneralGsmMessagesHandler::HandleIncomingCallForwardedL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneGeneralGsmMessagesHandler::HandleIncomingCallForwardedL()" );
    
    iActiveState.SetDivertIndication( ETrue );
    
    }

// ---------------------------------------------------------
//  CPhoneGeneralGsmMessagesHandler::SendGlobalErrorNoteL
// ---------------------------------------------------------
//
void CPhoneGeneralGsmMessagesHandler::SendGlobalErrorNoteL( 
        TInt aResourceId, TBool aNotificationDialog )
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneGeneralGsmMessagesHandler::SendGlobalErrorNoteL()" );
    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );

    if ( CPhonePubSubProxy::Instance()->Value( 
            KPSUidUikon, KUikGlobalNotesAllowed ) == 1 )
        {
        // Re-enable global notes
        TPhoneCmdParamBoolean globalNotifierParam;
        globalNotifierParam.SetBoolean( EFalse );
        iViewCommandHandle.ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,
            &globalNotifierParam );
            
        TPhoneCmdParamGlobalNote globalNoteParam;
        globalNoteParam.SetType( EAknGlobalErrorNote );
        globalNoteParam.SetTextResourceId( 
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( aResourceId ) );
        globalNoteParam.SetTone( CAknNoteDialog::EErrorTone );
        globalNoteParam.SetNotificationDialog( aNotificationDialog );
        
        iViewCommandHandle.ExecuteCommandL( 
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    }

// End of File
