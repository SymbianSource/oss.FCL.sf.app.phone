/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneErrorMessagesHandler class.
*
*/


// INCLUDES
#include <mpeengineinfo.h>
#include <telephonydomainpskeys.h>
#include <UikonInternalPSKeys.h>
#include <ccpdefs.h>

#include "phoneui.pan"
#include "phonerssbase.h"
#include "cphoneerrormessageshandler.h"
#include "cphonemainerrormessageshandler.h"
#include "phonelogger.h"
#include "cphonestatehandle.h"
#include "mphoneviewcommandhandle.h"
#include "cphonepubsubproxy.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamnote.h"
#include "cphonemainresourceresolver.h"


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C CPhoneErrorMessagesHandler::CPhoneErrorMessagesHandler(
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneStateMachine* aStateMachine ) :
    iViewCommandHandle( aViewCommandHandle),
    iStateMachine (aStateMachine )
    {
    }

// -----------------------------------------------------------
// CPhoneErrorMessagesHandler::~CPhoneErrorMessagesHandler()
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------

EXPORT_C CPhoneErrorMessagesHandler::~CPhoneErrorMessagesHandler()
    {
    }

// -----------------------------------------------------------------------------
// ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneErrorMessagesHandler::ConstructL()
    {    
    CPhoneMainErrorMessagesHandler::Instance()->RegisterErrorMessagesHandler( this );
    }

// -----------------------------------------------------------
// CPhoneVoIPErrorMessagesHandler::NewL()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneErrorMessagesHandler* CPhoneErrorMessagesHandler::NewL( 
                    MPhoneViewCommandHandle* aViewCommandHandle,
                    MPhoneStateMachine* aStateMachine )
    {
    CPhoneErrorMessagesHandler* self = 
        new( ELeave ) CPhoneErrorMessagesHandler( 
                    aViewCommandHandle,
                    aStateMachine );
    CleanupStack::PushL( self );    
    self->ConstructL();
    CleanupStack::Pop( self );    
    return self;
    }

// ---------------------------------------------------------
// CPhoneErrorMessagesHandler::SendGlobalInfoNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneErrorMessagesHandler::SendGlobalInfoNoteL( 
        TInt aResourceId, TBool aNotificationDialog )
    {
    __LOGMETHODSTARTEND( EPhoneControl, 
        "CPhoneErrorMessagesHandler::SendGlobalInfoNoteL()" );

    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );
    if ( CPhonePubSubProxy::Instance()->Value( 
            KPSUidUikon, KUikGlobalNotesAllowed ) == 1 )
        {
        // Re-enable global notes
        TPhoneCmdParamBoolean globalNotifierParam;
        globalNotifierParam.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( 
                EPhoneViewSetGlobalNotifiersDisabled,    
                &globalNotifierParam );
            
        TPhoneCmdParamGlobalNote globalNoteParam;
      
        globalNoteParam.SetType( EAknGlobalInformationNote );
        globalNoteParam.SetTextResourceId( 
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( aResourceId ) );
        globalNoteParam.SetTone( EAvkonSIDInformationTone );
        globalNoteParam.SetNotificationDialog( aNotificationDialog );

        iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewShowGlobalNote, &globalNoteParam );    
        }
  
    }
 
// ---------------------------------------------------------
//  CPhoneErrorMessagesHandler::SendGlobalErrorNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneErrorMessagesHandler::SendGlobalErrorNoteL( 
        TInt aResourceId, TBool aNotificationDialog )
    {
    __LOGMETHODSTARTEND( EPhoneControl, 
        "CPhoneErrorMessagesHandler::SendGlobalErrorNoteL()" );
    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );

    if ( CPhonePubSubProxy::Instance()->Value( 
            KPSUidUikon, KUikGlobalNotesAllowed ) == 1 )
        {
        // Re-enable global notes
        TPhoneCmdParamBoolean globalNotifierParam;
        globalNotifierParam.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL( 
             EPhoneViewSetGlobalNotifiersDisabled,
            &globalNotifierParam );
            
        TPhoneCmdParamGlobalNote globalNoteParam;
        globalNoteParam.SetType( EAknGlobalErrorNote );
        globalNoteParam.SetTextResourceId( 
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( aResourceId ) );
        globalNoteParam.SetTone( CAknNoteDialog::EErrorTone );
        globalNoteParam.SetNotificationDialog( aNotificationDialog );
        
        iViewCommandHandle->ExecuteCommandL(  
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
  
    }

// ---------------------------------------------------------
//  CPhoneErrorMessagesHandler::SendGlobalWarningNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneErrorMessagesHandler::SendGlobalWarningNoteL( 
        TInt aResourceId, TBool aNotificationDialog )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneErrorMessagesHandler::SendGlobalWarningNoteL( ) ");
    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );

    if ( CPhonePubSubProxy::Instance()->Value( 
            KPSUidUikon, KUikGlobalNotesAllowed ) == 1 || 
            SimState() == EPESimReadable )
        {    
        // Re-enable global notes
        TPhoneCmdParamBoolean globalNotifierParam;
        globalNotifierParam.SetBoolean( EFalse );
        iViewCommandHandle->ExecuteCommandL(  
            EPhoneViewSetGlobalNotifiersDisabled,
            &globalNotifierParam );
            
        TPhoneCmdParamGlobalNote globalNoteParam;
        globalNoteParam.SetType( EAknGlobalWarningNote );
        globalNoteParam.SetTextResourceId( 
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( aResourceId ) );
        globalNoteParam.SetTone( EAvkonSIDWarningTone );
        globalNoteParam.SetNotificationDialog( aNotificationDialog );

        iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
    }

// ---------------------------------------------------------
// CPhoneErrorMessagesHandler::SimState()
// ---------------------------------------------------------
//
TPESimState CPhoneErrorMessagesHandler::SimState() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneErrorMessagesHandler::SimState()");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(),
        Panic( EPhoneCtrlInvariant ) );
    return iStateMachine->PhoneEngineInfo()->SimState();
    }
            
// -----------------------------------------------------------
// CPhoneErrorMessagesHandler::ShowErrorSpecificNoteL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneErrorMessagesHandler::ShowErrorSpecificNoteL( const TPEErrorInfo& aErrorInfo )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneErrorMessagesHandler::ShowErrorSpecificNoteL()");
    
    __PHONELOG1( EBasic, EPhoneControl,
            "PhoneUIControl: CPhoneErrorMessagesHandler::ShowErrorSpecificNoteL - aErrorInfo.iErrorCode =%d ",
            aErrorInfo.iErrorCode);

    switch( aErrorInfo.iErrorCode )
        {
        case ECCPErrorRejected:
        case ECCPRequestFailure:
            SendGlobalErrorNoteL( EPhoneNoteTextRequestRejected, ETrue );    
            break;
            
        case ECCPErrorInvalidPhoneNumber:
            SendGlobalErrorNoteL( EPhoneInvalidPhoneNumber, ETrue );
            break;

        case ECCPErrorInvalidURI:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneInvalidPhoneNumber, ETrue );
                }
            else
                {
                SendGlobalErrorNoteL( EPhoneInvalidPhoneNumber, ETrue );
                }
            break;

        case ECCPErrorServiceSettingError:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNotePhoneOutOf3GCoverage );
                }
            else
                {
                SendGlobalInfoNoteL( EPhoneNoteTextCheckNetworkservices, ETrue );
                }
            break;

        case ECCPErrorNotAllowedInOfflineMode:
        case ECCPErrorAuthenticationFailed:
            SendGlobalWarningNoteL( EPhoneEmergencyCallsOnly, ETrue );
            break;

        case ECCPErrorNotReady:     
        case ECCPErrorGeneral:
        case ECCPErrorNotAllowed:
        case ECCPErrorNotFound:
        case ECCPErrorTimedOut:
        case ECCPErrorAccessDenied:        
            SendGlobalWarningNoteL( EPhoneNoteTextNotAllowed, ETrue );                    
            break;
            
        case ECCPErrorAlreadyInUse:
            SendGlobalErrorNoteL( EPhoneNoteTextCallNotAllowed, ETrue );                    
            break;

        case ECCPErrorInvalidFDN:
            SendGlobalWarningNoteL( EPhoneNoteTextCallNotAllowedFDN, ETrue );                    
            break;
            
        case ECCPErrorNotReached:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNumberNotInUse, ETrue );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneNumberNotInUse, ETrue );    
                }
                
            break;

        case ECCPErrorUnacceptableChannel:
        case ECCPErrorCCDestinationOutOfOrder:
        case ECCPErrorAccessInformationDiscarded:
        case ECCPErrorQualityOfServiceNotAvailable:        
        case ECCPErrorInvalidCallReferenceValue:
        case ECCPErrorCCInvalidTransitNetworkSelection:        
        case ECCPErrorConnectionError:        
        case ECCPErrorCCIncompatibleMessageInCallState:        
            if( IsVideoCall( aErrorInfo.iCallId ) &&
                iStateMachine->PhoneEngineInfo()->ProfileId() != EProfileOffLineId )
                {
                SendGlobalInfoNoteL( EPhoneNoteVideoCallNotPossible );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneErrorInConnection, ETrue );
                }
            break;
            
        case ECCPErrorCCResourceNotAvailable:
            SendGlobalWarningNoteL( EPhoneErrorInConnection, ETrue );
            break;
                        
        case ECCPErrorNumberBarred:
            SendGlobalWarningNoteL( EPhoneNumberBarred, ETrue );
            break;
            
        case ECCPErrorCCUserAlertingNoAnswer:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoAnswer, ETrue );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneNoAnswer, ETrue );   
                }           
            break;
            
        case KErrPhoneEngineNoWcdmaNetwork:  // Videotel special case. Refactoring PE/CSPlugin needed
        case ECCPErrorVideoCallNotSupportedByNetwork: ////
            SendGlobalInfoNoteL( EPhoneInformationNoNetworkSupportForVideoCallNote );
            break;
                
        case KErrPhoneEngineVideoCallNotAllowedDuringRestore: // Videotel special case. Refactoring PE/CSPlugin needed
        case ECCPErrorVideoCallNotAllowedDuringRestore:
            SendGlobalInfoNoteL( EPhoneInformationVideoCallNotAllowedDuringRestoreNote );
            break;
 
        case KErrPhoneEngineVideoCallSetupFailed: // Videotel special case. Refactoring PE/CSPlugin needed
        case ECCPErrorVideoCallSetupFailed:
            SendGlobalInfoNoteL( EPhoneNoteVideoCallSetupFailed );
            break;

        case ECCPErrorNetworkBusy:
            SendGlobalWarningNoteL( EPhoneNetworkBusy, ETrue );
            break;
            
        case ECCPErrorNoService:
            SendGlobalWarningNoteL( EPhoneNoteNoService, ETrue );   
            break;
            
        case ECCPErrorBusy:
            SendGlobalWarningNoteL( EPhoneNumberBusy, ETrue );   
            break;
            
        case ECCPErrorUserNotInCug:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNumberNotInCUG, ETrue );
                }
             else
                {
                SendGlobalWarningNoteL( EPhoneNumberNotInCUG, ETrue );   
                }
            break;
            
        case ECCPErrorCCNoRouteToDestination:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNotePhoneOutOf3GCoverage );
                }
             break;
            
        case ECCPErrorCCNormalCallClearing:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue16 );
                }
            break;
                    
        case ECCPErrorCCUserNotResponding:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue18 ); 
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneNoAnswer, ETrue );
                }
            break;
            
        case ECCPErrorCCCallRejected:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCalledNumberHasBarredIncomingCalls, ETrue );
                }
            break;
            
        case ECCPErrorMovedPermanently:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue22 );
                }
            break;
            
        case ECCPErrorNoAnswerForVideo:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNotePhoneOutOf3GCoverage );   
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneNoAnswer, ETrue );
                }
            break;
            
        case ECCPErrorCCNoChannelAvailable:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue34 ); 
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneNetworkBusy, ETrue );
                }
            break;
            
        case ECCPErrorNetworkOutOfOrder:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue38 );
                }
            break;
            
        case ECCPErrorCCRequestedFacilityNotSubscribed:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteVideoCallOnlyPossibleUnder3GCoverage );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneNoteTextNotAllowed, ETrue );
                }
            break;

        case ECCPErrorCCIncomingCallsBarredInCug:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue55 );
                }
            break;
            
        case ECCPErrorCCIncompatibleDestination:
        case ECCPErrorCCBearerCapabilityNotAuthorised:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteUnableToMakeVideoCallNotSupportedByOtherPhone );
                }
            break;
        
        case ECCPErrorCCBearerCapabilityNotCurrentlyAvailable:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue58 );
                }
            break;
            
        case ECCPErrorBadRequest:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteUnableToMakeVideoCallNotSupportedByOtherPhone );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneErrorInConnection, ETrue );
                } 
            break;
            
        case ECCPErrorCCUnspecifiedInterworkingError:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue127 );
                }
            break;
            
        case ECCPErrorCCPreemption:        
        case ECCPErrorCCResponseToStatusEnquiry:        
        case ECCPErrorCCInvalidMandatoryInformation:        
        case ECCPErrorCCNonExistentMessageType:        
        case ECCPErrorCCIncompatibleMessageInProtocolState:        
        case ECCPErrorCCNonExistentInformationElement:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoNotSupported );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneErrorInConnection, ETrue );
                }     
            break;

        case ECCPErrorCCRecoveryOnTimerExpiry:        
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoNotSupported );                
                }
            break;
            
        case ECCPErrorCCFacilityRejected:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoNotSupported );
                }
            else
                {
                SendGlobalErrorNoteL( EPhoneNoteTextRequestRejected, ETrue );
                }
            break;
            
        case ECCPTransferFailed:
            SendGlobalErrorNoteL( EPhoneNoteTextRequestRejected, ETrue );
            break;
 
        case ECCPErrorCCServiceNotAvailable:
        case ECCPErrorCCBearerServiceNotImplemented:
        case ECCPErrorCCOnlyRestrictedDigitalInformationBCAvailable:
        case ECCPErrorCCServiceNotImplemented:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoServiceNotAvailable );
                }
            break;
            
        case ECCPErrorCUGOutgoingCallsBarred:
            SendGlobalInfoNoteL( EPhoneOutgoingCallsBarredWithinCUG, ETrue );
            break;
        
        case ECCPErrorCUGNotSelected:
            SendGlobalInfoNoteL( EPhoneNoCUGSelected, ETrue );
            break;
        
        case ECCPErrorCUGIndexUnknown:
            SendGlobalInfoNoteL( EPhoneUnknownCUGIndex, ETrue );
            break;
        
        case ECCPErrorCUGIndexIncompatible:
            SendGlobalInfoNoteL( EPhoneCUGIndexIncompatible, ETrue );
            break;
        
        case ECCPErrorCUGCallsFailure:
            SendGlobalInfoNoteL( EPhoneCUGCallsFailure, ETrue );
            break;
        
        case ECCPErrorCLIRNotSubscribed:
            SendGlobalInfoNoteL( EPhoneCLIRNotSubscribed, ETrue );
            break;
            
        case ECCPErrorCCBSPossible:
        case ECCPErrorCCBSNotPossible:
        case ECCPErrorSatControl:
            break;
                
        default:
            __PHONELOG1(
                EOnlyFatal, 
                EPhoneControl,
                "PHONEUI_ERROR: CPhoneErrorMessagesHandler::HandleErrorL - Error received (err=%d)",
                aErrorInfo.iErrorCode);
            break;
        } 
    }    

// -----------------------------------------------------------
// CPhoneErrorMessagesHandler::IsVideoCall
// -----------------------------------------------------------
//
TBool CPhoneErrorMessagesHandler::IsVideoCall( const TInt aCallId ) const
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneErrorMessagesHandler::IsVideoCall() ");

    if( aCallId == KErrNotFound )
        {
        // Illegal call id, check call type command
        return ( iStateMachine->PhoneEngineInfo()->CallTypeCommand()
            == EPECallTypeVideo );  
        }
        
    return ( iStateMachine->PhoneEngineInfo()
        ->CallType( aCallId )== EPECallTypeVideo );
    }
    
// End of File
