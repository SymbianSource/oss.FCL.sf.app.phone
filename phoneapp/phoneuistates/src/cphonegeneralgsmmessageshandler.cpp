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
            SendGlobalInfoNoteL( EPhoneActiveBarrings );
            break;
        
        case MEngineMonitor::EPEMessageIncCallIsForw:
            HandleIncomingCallForwardedL();
            break;
            
        case MEngineMonitor::EPEMessageIncCallForwToC:
            SendGlobalInfoNoteL( EPhoneMtCallDiverting );
            break;
            
        case MEngineMonitor::EPEMessageOutCallForwToC:
            SendGlobalInfoNoteL( EPhoneDiverting );
            break;

        case MEngineMonitor::EPEMessageShowVersion:
            HandleShowVersionL();
            break;
            
        case MEngineMonitor::EPEMessageIssuedSSRequest:
            {
            __PHONELOG(
                EBasic, 
                EPhoneUIStates,
                "CPhoneGeneralGsmMessagesHandler::EPEMessageIssuedSSRequest" );

            TPhoneCmdParamBoolean booleanParam;
            booleanParam.SetBoolean( EFalse );
            iViewCommandHandle.ExecuteCommandL( 
                EPhoneViewSetBlockingDialogStatus, 
                &booleanParam );
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
            
            TPhoneCmdParamBoolean booleanParam;
            booleanParam.SetBoolean( ETrue );
            iViewCommandHandle.ExecuteCommandL( 
                EPhoneViewSetBlockingDialogStatus, 
                &booleanParam );
            
            // Get active call count
            TPhoneCmdParamInteger activeCallCount;
            iViewCommandHandle.ExecuteCommandL(
                EPhoneViewGetCountOfActiveCalls, &activeCallCount );
            
            if( !activeCallCount.Integer() )
                {
                __PHONELOG(
                    EBasic, 
                    EPhoneUIStates,
                    "CPhoneGeneralGsmMessagesHandler::EPEMessageIssuingSSRequest no active call" );
                // Ensure that the dialer is activated to display local notes and dialogs properly.
                TPhoneCmdParamAppInfo param;
                param.SetAppUid( KUidPhoneApplication );
                param.SetViewUid( KUidViewId );
                param.SetCustomMessageId( TUid::Uid( KTouchDiallerViewCommand ) );
                iViewCommandHandle.ExecuteCommandL( 
                    EPhoneViewActivateAppViewWithCustomMessage, 
                    &param );
                }
            // Remove phoneumber query
            iViewCommandHandle.ExecuteCommandL( EPhoneViewRemoveQuery );
            break;
            }

        case MEngineMonitor::EPEMessageTempClirActivationUnsuccessful:
            SendGlobalErrorNoteL( EPhoneSSNotifCLIRSupprReject );
            break;
            
        case MEngineMonitor::EPEMessageForwardUnconditionalModeActive:
            SendGlobalInfoNoteL( EPhoneAllIncomingCallsDiverted );
            break;
            
        case MEngineMonitor::EPEMessageForwardConditionallyModeActive:
            SendGlobalInfoNoteL( EPhoneActiveDiverts );
            break;

        default:
            break;
        }
    }

// ---------------------------------------------------------
// CPhoneGeneralGsmMessagesHandler::SendGlobalInfoNoteL
// ---------------------------------------------------------
//
void CPhoneGeneralGsmMessagesHandler::SendGlobalInfoNoteL( TInt aResourceId )
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
    }

// -----------------------------------------------------------
// CPhoneGeneralGsmMessagesHandler::HandleShowVersionL
// -----------------------------------------------------------
//
void CPhoneGeneralGsmMessagesHandler::HandleShowVersionL()
    {
    __LOGMETHODSTARTEND( EPhoneUIStates, 
        "CPhoneGeneralGsmMessagesHandler::HandleShowVersionL()" );
    if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer )  )  
        {
        iViewCommandHandle.ExecuteCommandL( EPhoneViewClearNumberEntryContent );       
        }
    else
        {
        // Remove number entry from screen
        iViewCommandHandle.ExecuteCommandL( EPhoneViewRemoveNumberEntry );        
        } 


#ifdef __SYNCML_DM
    // Launch DM UI 
    RWsSession sess = CCoeEnv::Static()->WsSession();
    RApaLsSession apaLsSession;        
  
    TApaTaskList appList( sess );
    TApaTask bring = appList.FindApp( KDeviceManagerUid );

    if ( bring.Exists() )
        {
        bring.BringToForeground();
        }
    else
        {
        if( !apaLsSession.Handle() )
            {
            User::LeaveIfError(apaLsSession.Connect());
            }
        CleanupClosePushL( apaLsSession );
        TThreadId thread;
        User::LeaveIfError( apaLsSession.StartDocument(KNullDesC, KDeviceManagerUid, thread) );
        CleanupStack::PopAndDestroy( &apaLsSession );  
        }

    // Stop dtmf tone. Long key press case key up event go to 
    // device manager application.
    iStateMachine.SendPhoneEngineMessage(
        MPEPhoneModel::EPEMessageEndDTMF );
    
#else    
    // Fetch version number
    TPEPhoneIdentityParameters phoneIdentityParameters = iStateMachine.
        PhoneEngineInfo()->PhoneIdentityParameters();

    // Add it to the resource string
    HBufC* buf = HBufC::NewLC( KSysUtilVersionTextLength );
    buf->Des().Format( phoneIdentityParameters.iRevision );
        
    TPhoneCmdParamNote noteParam;
    noteParam.SetType( EPhoneNoteCustom );
    noteParam.SetResourceId( CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneInformationWaitNote ) );
    noteParam.SetText( *buf );
    
    // Display note
    iViewCommandHandle.ExecuteCommandL( EPhoneViewShowNote, &noteParam );
    
    CleanupStack::PopAndDestroy( buf );
#endif
    }

// ---------------------------------------------------------
//  CPhoneGeneralGsmMessagesHandler::SendGlobalErrorNoteL
// ---------------------------------------------------------
//
void CPhoneGeneralGsmMessagesHandler::SendGlobalErrorNoteL( TInt aResourceId )
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

        iViewCommandHandle.ExecuteCommandL( 
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    }

// End of File
