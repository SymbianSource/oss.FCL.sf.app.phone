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
#include <phoneui.rsg>
#include <telephonyvariant.hrh>

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
#include "cphonecenrepproxy.h"


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
EXPORT_C void CPhoneErrorMessagesHandler::SendGlobalInfoNoteL( TInt aResourceId )
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
        iViewCommandHandle->ExecuteCommandL( EPhoneViewSetGlobalNotifiersDisabled,    &globalNotifierParam );
            
        TPhoneCmdParamGlobalNote globalNoteParam;
      
        globalNoteParam.SetType( EAknGlobalInformationNote );
        
        if ( CPhoneCenRepProxy::Instance()->
                IsTelephonyFeatureSupported( KTelephonyLVFlagClearCodeCustomization ) )
            {
			// Show varied note if clear code customization is enabled
            TInt variedNote( KErrNotFound );
            //Get varied resource
            variedNote = ResourceVariation();
            if ( variedNote != KErrNotFound )
                {
                globalNoteParam.SetTextResourceId( variedNote );
                }
            else
                {
				// If varied resource is not found, show original note.
                globalNoteParam.SetTextResourceId( 
                    CPhoneMainResourceResolver::Instance()->
                    ResolveResourceID( aResourceId ) );
                }
            }
        else
            {
            globalNoteParam.SetTextResourceId( 
                CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( aResourceId ) );
            }
        globalNoteParam.SetTone( EAvkonSIDInformationTone );

        iViewCommandHandle->ExecuteCommandL( 
            EPhoneViewShowGlobalNote, &globalNoteParam );    
        }
  
    }
 
// ---------------------------------------------------------
//  CPhoneErrorMessagesHandler::SendGlobalErrorNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneErrorMessagesHandler::SendGlobalErrorNoteL( TInt aResourceId )
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
        
        if (  CPhoneCenRepProxy::Instance()->
                IsTelephonyFeatureSupported( KTelephonyLVFlagClearCodeCustomization ) )
            {
			// Show varied note if clear code customization is enabled
            TInt variedNote( KErrNotFound );
            //Get varied resource
            variedNote = ResourceVariation();
            if ( variedNote != KErrNotFound )
                {
                globalNoteParam.SetTextResourceId( variedNote );
                }
            else
                {
				// If varied resource is not found, show original note.
                globalNoteParam.SetTextResourceId( 
                    CPhoneMainResourceResolver::Instance()->
                    ResolveResourceID( aResourceId ) );
                }
            }
        else
            {
            globalNoteParam.SetTextResourceId( 
                CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( aResourceId ) );
            }
        globalNoteParam.SetTone( CAknNoteDialog::EErrorTone );

        iViewCommandHandle->ExecuteCommandL(  
            EPhoneViewShowGlobalNote, &globalNoteParam );
        }
  
    }

// ---------------------------------------------------------
//  CPhoneErrorMessagesHandler::SendGlobalWarningNoteL
// ---------------------------------------------------------
//
EXPORT_C void CPhoneErrorMessagesHandler::SendGlobalWarningNoteL( TInt aResourceId )
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
        
        if ( CPhoneCenRepProxy::Instance()->
                IsTelephonyFeatureSupported( KTelephonyLVFlagClearCodeCustomization ) )
            {
			// Show varied note if clear code customization is enabled
            TInt variedNote( KErrNotFound );
            //Get varied resource
            variedNote = ResourceVariation();
            if ( variedNote != KErrNotFound )
                {
                globalNoteParam.SetTextResourceId( variedNote );
                }
            else
                {
				// If varied resource is not found, show original note.
                globalNoteParam.SetTextResourceId( 
                    CPhoneMainResourceResolver::Instance()->
                    ResolveResourceID( aResourceId ) );
                }
            }
        else
            {
            globalNoteParam.SetTextResourceId( 
                CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( aResourceId ) );
            }
        globalNoteParam.SetTone( EAvkonSIDWarningTone );

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
    iCallId = aErrorInfo.iCallId;

    switch( aErrorInfo.iErrorCode )
        {
        case ECCPErrorRejected:
        case ECCPRequestFailure:
            SendGlobalErrorNoteL( EPhoneNoteTextRequestRejected );    
            break;
            
        case ECCPErrorInvalidPhoneNumber:
            SendGlobalErrorNoteL( EPhoneInvalidPhoneNumber );
            break;

        case ECCPErrorInvalidURI:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneInvalidPhoneNumber );
                }
            else
                {
                SendGlobalErrorNoteL( EPhoneInvalidPhoneNumber );
                }
            break;

        case ECCPErrorServiceSettingError:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNotePhoneOutOf3GCoverage );
                }
            else
                {
                SendGlobalInfoNoteL( EPhoneNoteTextCheckNetworkservices );
                }
            break;

        case ECCPErrorNotAllowedInOfflineMode:
        case ECCPErrorAuthenticationFailed:
            SendGlobalWarningNoteL( EPhoneEmergencyCallsOnly );
            break;

        case ECCPErrorNotReady:     
        case ECCPErrorGeneral:
        case ECCPErrorNotAllowed:
        case ECCPErrorNotFound:
        case ECCPErrorTimedOut:
        case ECCPErrorAccessDenied:        
            SendGlobalWarningNoteL( EPhoneNoteTextNotAllowed );                    
            break;
            
        case ECCPErrorAlreadyInUse:
            SendGlobalErrorNoteL( EPhoneNoteTextCallNotAllowed );                    
            break;

        case ECCPErrorInvalidFDN:
            SendGlobalWarningNoteL( EPhoneNoteTextCallNotAllowedFDN );                    
            break;
            
        case ECCPErrorNotReached:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNumberNotInUse );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneNumberNotInUse );    
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
                SendGlobalWarningNoteL( EPhoneErrorInConnection );
                }
            break;
            
        case ECCPErrorCCResourceNotAvailable:
            SendGlobalWarningNoteL( EPhoneErrorInConnection );
            break;
                        
        case ECCPErrorNumberBarred:
            SendGlobalWarningNoteL( EPhoneNumberBarred );
            break;
            
        case ECCPErrorCCUserAlertingNoAnswer:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoAnswer );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneNoAnswer );   
                }           
            break;
            
        case KErrPhoneEngineNoWcdmaNetwork:  // Videotel special case. Refactoring PE/CSPlugin needed
        case ECCPErrorVideoCallNotSupportedByNetwork:
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
            SendGlobalWarningNoteL( EPhoneNetworkBusy );
            break;
            
        case ECCPErrorNoService:
            SendGlobalWarningNoteL( EPhoneNoteNoService );   
            break;
            
        case ECCPErrorBusy:
            SendGlobalWarningNoteL( EPhoneNumberBusy );   
            break;
            
        case ECCPErrorUserNotInCug:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNumberNotInCUG );
                }
             else
                {
                SendGlobalWarningNoteL( EPhoneNumberNotInCUG );   
                }
            break;
            
        case ECCPErrorCCNoRouteToDestination:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNotePhoneOutOf3GCoverage );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneErrorInConnection );
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
                SendGlobalWarningNoteL( EPhoneNoAnswer );
                }
            break;
            
        case ECCPErrorCCCallRejected:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCalledNumberHasBarredIncomingCalls );
                }
            else if ( IsVoiceCall( aErrorInfo.iCallId ))
                {
                SendGlobalWarningNoteL( EPhoneNoteCallInfoCauseValue21 );
                }
            break;
            
        case ECCPErrorMovedPermanently:
            SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue22 );
            break;
            
        case ECCPErrorNoAnswerForVideo:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue16 );   
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneNoAnswer );
                }
            break;
            
        case ECCPErrorCCNoChannelAvailable:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue34 ); 
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneNetworkBusy );
                }
            break;
            
        case ECCPErrorNetworkOutOfOrder:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue38 );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneErrorInConnection );
                }
            break;
            
        case ECCPErrorCCRequestedFacilityNotSubscribed:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteVideoCallOnlyPossibleUnder3GCoverage );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneNoteTextNotAllowed );
                }
            break;

        case ECCPErrorCCIncomingCallsBarredInCug:
            SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue55 );
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
                SendGlobalWarningNoteL( EPhoneErrorInConnection );
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
                SendGlobalWarningNoteL( EPhoneErrorInConnection );
                }     
            break;

        case ECCPErrorCCRecoveryOnTimerExpiry:        
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoCauseValue16 );
                }
            break;
            
        case ECCPErrorCCFacilityRejected:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoNotSupported );
                }
            else
                {
                SendGlobalErrorNoteL( EPhoneNoteTextRequestRejected );
                }
            break;
            
        case ECCPTransferFailed:
            SendGlobalErrorNoteL( EPhoneNoteTextRequestRejected );
            break;
 
        case ECCPErrorCCServiceNotAvailable:
        case ECCPErrorCCBearerServiceNotImplemented:
        case ECCPErrorCCOnlyRestrictedDigitalInformationBCAvailable:
        case ECCPErrorCCServiceNotImplemented:
            if( IsVideoCall( aErrorInfo.iCallId ) )
                {
                SendGlobalInfoNoteL( EPhoneNoteCallInfoServiceNotAvailable );
                }
            else
                {
                SendGlobalWarningNoteL( EPhoneErrorInConnection );
                }
            break;
            
        case ECCPErrorCUGOutgoingCallsBarred:
            SendGlobalInfoNoteL( EPhoneOutgoingCallsBarredWithinCUG );
            break;
        
        case ECCPErrorCUGNotSelected:
            SendGlobalInfoNoteL( EPhoneNoCUGSelected );
            break;
        
        case ECCPErrorCUGIndexUnknown:
            SendGlobalInfoNoteL( EPhoneUnknownCUGIndex );
            break;
        
        case ECCPErrorCUGIndexIncompatible:
            SendGlobalInfoNoteL( EPhoneCUGIndexIncompatible );
            break;
        
        case ECCPErrorCUGCallsFailure:
            SendGlobalInfoNoteL( EPhoneCUGCallsFailure );
            break;
        
        case ECCPErrorCLIRNotSubscribed:
            SendGlobalInfoNoteL( EPhoneCLIRNotSubscribed );
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
    iCallId = KErrNotFound;
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
    
// -----------------------------------------------------------
// CPhoneErrorMessagesHandler::IsVoiceCall
// -----------------------------------------------------------
//
TBool CPhoneErrorMessagesHandler::IsVoiceCall( const TInt aCallId ) const
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneErrorMessagesHandler::IsVoiceCall() ");

    if( aCallId == KErrNotFound )
        {
       // Illegal call id, check call type command
        return ( iStateMachine->PhoneEngineInfo()->CallTypeCommand()
            == EPECallTypeCSVoice );  
        }
     
    return ( iStateMachine->PhoneEngineInfo()
            ->CallType( aCallId )== EPECallTypeCSVoice );
	}

// -----------------------------------------------------------------------------
// CPhoneErrorMessagesHandler::ResourceVariation
// -----------------------------------------------------------------------------
TInt CPhoneErrorMessagesHandler::ResourceVariation() const
    {
    TInt resource( KErrNotFound );
    
    TInt callId = (KErrNotFound == iCallId) ? 
        iStateMachine->PhoneEngineInfo()->CallId() :
        iCallId;

    //get exit code error from EngineInfo
    TInt callError = iStateMachine->PhoneEngineInfo()->ProtocolError( callId );
    
    switch( callError )
            {
            case KErrGsmCCUnassignedNumber:
                resource = R_NOTE_VAR_CAUSE_1;                
                break;
                
            case KErrGsmCCNoRouteToDestination:
                resource = R_NOTE_VAR_CAUSE_3;                
                break;
   
            case KErrGsmCCOperatorDeterminedBarring:
                resource = R_NOTE_VAR_CAUSE_8;                
                break;
                   
            case KErrGsmCCUserBusy:
                resource = R_NOTE_VAR_CAUSE_17;                
                break;
                
            case KErrGsmCCUserNotResponding:
                resource = R_NOTE_VAR_CAUSE_18;                
                break;
                
            case KErrGsmCCCallRejected:
                resource = R_NOTE_VAR_CAUSE_21;
                break;
                
            case KErrGsmCCNumberChanged:
                resource = R_NOTE_VAR_CAUSE_22;
                break;
                
            case KErrGsmCCDestinationOutOfOrder:
                resource = R_NOTE_VAR_CAUSE_27;
                break;

            case KErrGsmCCFacilityRejected:
                resource = R_NOTE_VAR_CAUSE_29;                
                break;
                
            case KErrGsmCCNetworkOutOfOrder:
                resource = R_NOTE_VAR_CAUSE_38;                
                break;
                
            case KErrGsmCCTemporaryFailure:
                resource = R_NOTE_VAR_CAUSE_41;                
                break;
                
            case KErrGsmCCSwitchingEquipmentCongestion:
                resource = R_NOTE_VAR_CAUSE_42;                
                break;
                
            case KErrGsmCCAccessInformationDiscarded:
                resource = R_NOTE_VAR_CAUSE_43;                
                break;
                
            case KErrGsmCCRequestedChannelNotAvailable:
                resource = R_NOTE_VAR_CAUSE_44;                
                break;
                
            case KErrGsmCCResourceNotAvailable:
                resource = R_NOTE_VAR_CAUSE_47;                
                break;
                
            case KErrGsmCCIncomingCallsBarredInCug:
                resource = R_NOTE_VAR_CAUSE_55;                
                break;
                
            case KErrGsmCCBearerCapabilityNotAuthorised:
                resource = R_NOTE_VAR_CAUSE_57;                
                break;
                
            case KErrGsmCCServiceNotAvailable:
                resource = R_NOTE_VAR_CAUSE_63;                
                break;
                
            case KErrGsmCCBearerServiceNotImplemented:
                resource = R_NOTE_VAR_CAUSE_65;                
                break;
                
            case KErrGsmCCRequestedFacilityNotImplemented:
                resource = R_NOTE_VAR_CAUSE_69;                
                break;
                
            case KErrGsmCCUserNotInCug:
                resource = R_NOTE_VAR_CAUSE_87;                
                break;
                
            case KErrGsmCCIncompatibleDestination:
                resource = R_NOTE_VAR_CAUSE_88;                
                break;
                
            case KErrGsmCCSemanticallyIncorrectMessage:
                resource = R_NOTE_VAR_CAUSE_95;                
                break;
                
            case KErrGsmCCUnspecifiedProtocolError:
                resource = R_NOTE_VAR_CAUSE_111;                
                break;
                
            case KErrGsmCCUnspecifiedInterworkingError:
                resource = R_NOTE_VAR_CAUSE_127;
                break;
            default:
                break;
            }  
    return resource;
    }
// End of File
