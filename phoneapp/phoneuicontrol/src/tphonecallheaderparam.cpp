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
* Description:  A parameter class for setting call header information
*
*/


#include <mpeclientinformation.h>
#include <StringLoader.h>
#include <bautils.h>

#include "tphonecallheaderparam.h"
#include "tphonecmdparamboolean.h"
#include "mphonestatemachine.h"
#include "mphonecallheadermanagerutility.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "phoneui.pan"
#include "cphonecenrepproxy.h"
#include "telephonyvariant.hrh"
#include "phoneviewcommanddefinitions.h"
#include "phonelogger.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// TPhoneCallHeaderParam::TPhoneCallHeaderParam
// -----------------------------------------------------------------------------
//  
TPhoneCallHeaderParam::TPhoneCallHeaderParam(
        MPhoneCallHeaderManagerUtility& aManagerUtility,
        MPhoneStateMachine& aStateMachine ) 
        : iManagerUtility ( aManagerUtility ),
          iStateMachine ( aStateMachine ),
          iCallHeaderType ( EPECallTypeUninitialized ),
          iSetDivertIndication ( EFalse )
    {
    }

// -----------------------------------------------------------
// TPhoneCallHeaderParam::SetCallHeaderTexts
// -----------------------------------------------------------
//
void TPhoneCallHeaderParam::SetCallHeaderTexts(
    const TInt aCallId, 
    const TBool aWaitingCall,
    const TBool aVideoCall,
    TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::SetCallHeaderTexts( ) ");
    
    TInt labelId(KPhoneRssCommonFirst);
    TInt shortLabelId( KPhoneRssCommonFirst );
    
    // Fetch engine info parameters.
    const TBool auxLine( iStateMachine.PhoneEngineInfo()->CallALSLine( aCallId ) == CCCECallParameters::ECCELineTypeAux );
    const TBool cli( iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ).Length());
    const TBool cnap( iStateMachine.PhoneEngineInfo()->RemotePartyName( aCallId ).Length());
    const TInt numberType( iStateMachine.PhoneEngineInfo()->RemotePhoneNumberType( aCallId ));
    
    __PHONELOG2( EBasic, EPhoneControl, "TPhoneCallHeaderParam::SetCallHeaderTexts - NumberType(%d), CLI(%d)", numberType, cli );
    __PHONELOG2( EBasic, EPhoneControl, "TPhoneCallHeaderParam::SetCallHeaderTexts - CNAP(%d), AuxLine(%d)", cnap, auxLine );
    
    if ( !cli && !cnap && numberType != EPEPrivateNumber && numberType != EPEUnknownNumber )
        {
        if ( auxLine )
            {
            if ( aWaitingCall )
                {
                labelId = EPhoneIncomingLine2WaitingText; // waiting, line 2
                shortLabelId = EPhoneIncomingLine2Text; // on line 2
                }
            else
                {
                labelId = EPhoneIncomingLine2Text; // on line 2
                }
            }
        // If CLIR, but also network limitation(e.g. EPEUnknownNumber), then second line 
        // should be empty in call bubble.
        else
            {
            labelId = KPhoneRssCommonFirst; // No second line in call bubble
            }
        }
    else  // Voice or video call with CLI or with CNAP.
        {
        if ( aWaitingCall )
            {
            if ( auxLine  )
                {
                labelId = EPhoneIncomingLine2WaitingText; // waiting, line 2
                shortLabelId = EPhoneIncomingLine2Text; // on line 2
                }
            else
                {
                labelId = EPhoneCallWaitingLabel; // waiting
                shortLabelId = EPhoneCallWaitingLabelShort; // waiting
                }
            }
        else // Mo other calls
            {
            if ( auxLine )
                {
                labelId = EPhoneIncomingLine2CallingText; // calling, line 2
                shortLabelId = EPhoneIncomingLine2Text; // on line 2
                }
            else
                {
                // If CLIR, but not network limitation, then second line 
                // (calling or video call) should be shown in call bubble.
                if ( aVideoCall )
                    {
                    labelId = EPhoneVideoCallIncoming; // video call
                    shortLabelId = EPhoneVideoCallIncomingShort; // video call
                    }
                else
                    {
                    labelId = EPhoneIncomingCallLabel; // calling
                    shortLabelId = EPhoneIncomingCallLabelShort; // calling
                    }
                }
            }
        }
    __PHONELOG2( EBasic, EPhoneControl, 
            "TPhoneCallHeaderParam::SetCallHeaderTexts - labelId(%d) , shortLabelId(%d)",
            labelId, shortLabelId );
    iManagerUtility.LoadCallHeaderTexts( labelId, shortLabelId, aCallHeaderData );
    }
    
// -----------------------------------------------------------------------------
// TPhoneCallHeaderParam::SetCliParamatersL
// -----------------------------------------------------------------------------
// 
void TPhoneCallHeaderParam::SetCliParamatersL(
        const TInt aCallId, 
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::SetCliParamatersL( ) ");
    
    // Set call header number type
    aCallHeaderData->SetNumberType( iStateMachine.PhoneEngineInfo()->RemotePhoneNumberType( aCallId ) );
    
    if ( ( iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ).Length() ) && 
         ( !ContactInfoAvailable( aCallId ) ) )
        {
        // Set phonenumber/URI as the CLI text for the call header      
       aCallHeaderData->SetCLIText( iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ),
                 TPhoneCmdParamCallHeaderData::ELeft );
            
        // No contact name, use phonenumber when available.
        aCallHeaderData->SetParticipantListCLI(
                TPhoneCmdParamCallHeaderData::EPhoneParticipantCNAPText );
        }
    else
        {
        TPhoneCmdParamCallHeaderData::TPhoneTextClippingDirection cnapClippingDirection = TPhoneCmdParamCallHeaderData::ERight;
        TBuf<KCntMaxTextFieldLength> remoteInfoText( KNullDesC );
 
        TBool secondaryCli = GetRemoteInfoDataL( aCallId, remoteInfoText );
        cnapClippingDirection = TPhoneCmdParamCallHeaderData::ELeft;

        aCallHeaderData->SetCLIText( remoteInfoText,  TPhoneCmdParamCallHeaderData::ERight );
        
        if (secondaryCli)
            {
            aCallHeaderData->SetCNAPText( iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ), 
                cnapClippingDirection );
            }
        }
    
    SetCallerImage( aCallId, aCallHeaderData );

    // Set the Caller text
    if ( iStateMachine.PhoneEngineInfo()->CallerText( aCallId ).Length() > 0 )
        {
        aCallHeaderData->SetCallerText( iStateMachine.PhoneEngineInfo()->CallerText( aCallId ) );
        }
    }

// -----------------------------------------------------------------------------
// TPhoneCallHeaderParam::SetCallerImage
// -----------------------------------------------------------------------------
//     
void TPhoneCallHeaderParam::SetCallerImage( 
        const TInt aCallId, 
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::SetCallerImage( ) ");
    // Set the call header picture data if it is available
    if ( ( iStateMachine.PhoneEngineInfo()->CallerImage( aCallId ).Length() > 0 )  && 
         ( BaflUtils::FileExists( CCoeEnv::Static()->FsSession(), 
                                  iStateMachine.PhoneEngineInfo()->CallerImage( aCallId ) ) ) )
        {
        aCallHeaderData->SetPicture( iStateMachine.PhoneEngineInfo()->CallerImage( aCallId ) );
        }
    else
        {
        // Set the thumbnail picture data if it is available
        aCallHeaderData->SetHasThumbnail( iStateMachine.PhoneEngineInfo()->HasCallerThumbnail( aCallId ) );
        CFbsBitmap* picture = iStateMachine.PhoneEngineInfo()->CallerThumbnail( aCallId );
        if ( picture )
            {
            aCallHeaderData->SetThumbnail( picture );
            }
        }
    }

// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::SetBasicCallHeaderParamsL
// ---------------------------------------------------------------------------
//
void TPhoneCallHeaderParam::SetBasicCallHeaderParamsL(
        const TInt aCallId, 
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::SetBasicCallHeaderParamsL( ) ");
    // Set call header call state
    aCallHeaderData->SetCallState( 
        iStateMachine.PhoneEngineInfo()->CallState( aCallId ) );

    // Set call header type            
    aCallHeaderData->SetCallType( GetCallType( aCallId, aCallHeaderData ) );
    
    // Set call header voice privacy status
    aCallHeaderData->SetCiphering( 
        iStateMachine.PhoneEngineInfo()->IsSecureCall( aCallId ) );
    aCallHeaderData->SetCipheringIndicatorAllowed( 
        iStateMachine.PhoneEngineInfo()->SecureSpecified() );
    
    //see service provider settings API
    aCallHeaderData->SetServiceId(
       iStateMachine.PhoneEngineInfo()->ServiceId( aCallId ) );
       
    // Set contact link, see virtual phonebook API
    aCallHeaderData->SetContactLink(
       iStateMachine.PhoneEngineInfo()->ContactLink( aCallId ) );
       
    // Set remote phone number
    aCallHeaderData->SetRemotePhoneNumber(
       iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ) );
    }

// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::GetCallType
// ---------------------------------------------------------------------------
//
TPECallType TPhoneCallHeaderParam::GetCallType( 
        const TInt aCallId,
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::GetCallType( ) ");
    // Set call header type.
    TPECallType callType = 
        iStateMachine.PhoneEngineInfo()->CallType( aCallId );
    SetCallHeaderType( callType );
    
    if ( iStateMachine.PhoneEngineInfo()->CallALSLine( aCallId ) 
         == CCCECallParameters::ECCELineTypeAux )
        {
        aCallHeaderData->SetLine2( ETrue );
        }

    __PHONELOG1( EBasic, EPhoneControl, 
                "TPhoneCallHeaderParam::GetCallType() - callType: %d ", 
                callType )
    return callType;
    }

// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::SetCallHeaderType
// ---------------------------------------------------------------------------
//
void TPhoneCallHeaderParam::SetCallHeaderType( 
    TInt aCallHeaderType )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::SetCallHeaderType( ) ");
    iCallHeaderType = aCallHeaderType;
    __PHONELOG1( EBasic, EPhoneControl, 
                    "TPhoneCallHeaderParam::SetCallHeaderType() - iCallHeaderType: %d ", 
                    iCallHeaderType )
    }

// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::CallHeaderType
// ---------------------------------------------------------------------------
//
TInt TPhoneCallHeaderParam::CallHeaderType() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::CallHeaderType( ) ");
    __PHONELOG1( EBasic, EPhoneControl, 
                        "TPhoneCallHeaderParam::CallHeaderType() - iCallHeaderType: %d ", 
                        iCallHeaderType )
    return iCallHeaderType;            
    }

// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::ContactInfoAvailable
// ---------------------------------------------------------------------------
//
TBool TPhoneCallHeaderParam::ContactInfoAvailable( const TInt aCallId ) const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::ContactInfoAvailable( ) ");
    TBool contactAvailable = EFalse;
    if ( ( iStateMachine.PhoneEngineInfo()->RemoteName( aCallId ).Length() ) || 
         ( iStateMachine.PhoneEngineInfo()->RemoteCompanyName( aCallId ).Length() ) )
        {
        contactAvailable = ETrue;
        }
    return contactAvailable;
    }

// -----------------------------------------------------------------------------
// TPhoneCallHeaderParam::SetCliAndCnapParamatersL
// -----------------------------------------------------------------------------
// 
void TPhoneCallHeaderParam::SetCliAndCnapParamatersL(
        const TInt aCallId, 
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::SetCliAndCnapParamatersL( ) ");
    TBuf<KCntMaxTextFieldLength> cnapText( KNullDesC );

    // Set call header number type
    aCallHeaderData->SetNumberType( iStateMachine.PhoneEngineInfo()->RemotePhoneNumberType( aCallId ) );
        
    const MPEClientInformation& info = 
        iStateMachine.PhoneEngineInfo()->CallClientInformation( aCallId );

    if ( ( iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ).Length() ) && 
         ( !ContactInfoAvailable( aCallId ) ) && 
         ( !info.ShowNumber() ) )
        {
        // No contact info data available; use the phone number
        aCallHeaderData->SetCLIText(
                iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ),
                TPhoneCmdParamCallHeaderData::ELeft);
        
        // No contact name, use phonenumber when available.
        aCallHeaderData->SetParticipantListCLI( 
                TPhoneCmdParamCallHeaderData::EPhoneParticipantCNAPText );
        }
    else
        {
        TBuf<KCntMaxTextFieldLength> remoteInfoText( KNullDesC );
        
        GetRemoteInfoDataL( aCallId, remoteInfoText );
        aCallHeaderData->SetCLIText( remoteInfoText, TPhoneCmdParamCallHeaderData::ERight );
        }

    // Fetch CNAP text and clipping direction
    TPhoneCmdParamCallHeaderData::TPhoneTextClippingDirection cnapClippingDirection;
    GetCNAPText( aCallId, cnapText, cnapClippingDirection );
    
    // Set CNAP data 
    aCallHeaderData->SetCNAPText( cnapText, cnapClippingDirection );
    
    // Set caller image 
    SetCallerImage( aCallId, aCallHeaderData );

    // Set the Caller text
    if ( iStateMachine.PhoneEngineInfo()->CallerText( aCallId ).Length() > 0 )
        {
        aCallHeaderData->SetCallerText( iStateMachine.PhoneEngineInfo()->CallerText( aCallId ) );
        }        
    
    // Set the call header CNAP data ( Contains possible CNAP name or received skype identification ).
    if ( IsFeatureSupported( KTelephonyLVFlagUUS, aCallId ) )
        {
        aCallHeaderData->SetCNAPText( iStateMachine.PhoneEngineInfo()->RemotePartyName( aCallId ), 
                TPhoneCmdParamCallHeaderData::ERight );
        }
    }

// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::GetCNAPText
// ---------------------------------------------------------------------------
//
void TPhoneCallHeaderParam::GetCNAPText( 
    const TInt aCallId,
       TDes& aData, 
       TPhoneCmdParamCallHeaderData::TPhoneTextClippingDirection& aDirection ) const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::GetCNAPText( ) ");
    
    // Set clipping direction  
    aDirection = TPhoneCmdParamCallHeaderData::ERight;
    
    // If it's not a private number show further info
    if ( iStateMachine.PhoneEngineInfo()->RemotePhoneNumberType( aCallId ) != 
        EPEPrivateNumber )
        {
        if ( ( iStateMachine.PhoneEngineInfo()->RemoteName( aCallId ).Length() ||
            iStateMachine.PhoneEngineInfo()->RemotePartyName( aCallId ).Length() || 
            iStateMachine.PhoneEngineInfo()->RemoteCompanyName( aCallId ).Length() ) &&
            iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ).Length() )
            {
            // Use the phone number for the CNAP display
            aData.Copy( iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ) );
            
            // Clipping direction for non-private number
            aDirection = TPhoneCmdParamCallHeaderData::ELeft;
            }
        }
    }

// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::IsFeatureSupported
// ---------------------------------------------------------------------------
//
TBool TPhoneCallHeaderParam::IsFeatureSupported( 
        const TInt aFeatureKey, 
        const TInt aCallId ) const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::IsFeatureSupported( ) ");
    TBool featureSupport(EFalse);
    switch( aFeatureKey )
        {
        case KTelephonyLVFlagUUS:
            {
            if( ( CPhoneCenRepProxy::Instance()->IsTelephonyFeatureSupported( aFeatureKey ) ) &&
                ( iStateMachine.PhoneEngineInfo()->RemotePartyName( aCallId ).Length() ) )
                {
                featureSupport = ETrue;
                }
            }
            break;
        default:
            //Do nothing.
            break;
        }
    __PHONELOG1( EBasic, EPhoneControl, 
            "TPhoneCallHeaderParam::IsFeatureSupported() - featureSupport: %d ", 
            featureSupport )
    
    return featureSupport;
    }

// ---------------------------------------------------------------------------
// TPhoneCallHeaderParam::SetDivertIndicatorToCallHeader
// ---------------------------------------------------------------------------
//
void TPhoneCallHeaderParam::SetDivertIndicatorToCallHeader( 
    const TInt aCallId, 
    TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::SetDivertIndicatorToCallHeader( ) ");
    if( iSetDivertIndication )
        {
        aCallHeaderData->SetDiverted( ETrue );
        }
    
    if ( iStateMachine.PhoneEngineInfo()->CallALSLine( aCallId ) == CCCECallParameters::ECCELineTypeAux )
        {
        __PHONELOG( EBasic, EPhoneControl, 
                "TPhoneCallHeaderParam::SetDivertIndicatorToCallHeader - CallALSLine() == CCCECallParameters::ECCELineTypeAux");
        aCallHeaderData->SetLine2( ETrue );    
        }        
    }

// ---------------------------------------------------------------------------
// TPhoneCallHeaderParam::SetDivertIndication
// ---------------------------------------------------------------------------
//
void TPhoneCallHeaderParam::SetDivertIndication( const TBool aDivertIndication )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::SetDivertIndication( ) ");
    iSetDivertIndication = aDivertIndication;           
    __PHONELOG1( EBasic, EPhoneControl, 
                "TPhoneCallHeaderParam::SetDivertIndication() - iSetDivertIndication: %d ", 
                iSetDivertIndication )
    }

// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::SetIncomingCallHeaderParamsL
// ---------------------------------------------------------------------------
//
void TPhoneCallHeaderParam::SetIncomingCallHeaderParamsL(
        const TInt aCallId, 
        const TBool aWaitingCall,
        const TBool aVideoCall,
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::SetIncomingCallHeaderParamsL( ) ");
    // Set basic params must be called before update is called.
    SetBasicCallHeaderParamsL( aCallId, aCallHeaderData );
    
    // Set call header labels
    SetCallHeaderTexts( 
            aCallId, 
            aWaitingCall, 
            aVideoCall, 
            aCallHeaderData );
    
    SetCliAndCnapParamatersL( aCallId, aCallHeaderData );
    
    // Set divert indication to call header if needed.
    SetDivertIndicatorToCallHeader( aCallId, aCallHeaderData );
    }

// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::SetOutgoingCallHeaderParamsL
// ---------------------------------------------------------------------------
//
void TPhoneCallHeaderParam::SetOutgoingCallHeaderParamsL(
        const TInt aCallId,
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::SetOutgoingCallHeaderParamsL( ) ");
    // Set basic params must be called before update is called.
    SetBasicCallHeaderParamsL( aCallId, aCallHeaderData );
    
    // Set call header labels
    if ( aCallHeaderData->CallType() == EPECallTypeVideo )
        {
        iManagerUtility.LoadCallHeaderTexts( 
                EPhoneOutgoingVideoCallLabel, 
                EPhoneOutgoingVideoCallLabelShort, 
                aCallHeaderData );
        }
    else
        {
        iManagerUtility.LoadCallHeaderTexts( 
                EPhoneOutgoingCallLabel, 
                EPhoneOutgoingCallLabelShort, 
                aCallHeaderData );
        }
    
    SetCliParamatersL( aCallId, aCallHeaderData );
    }

// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::UpdateCallHeaderInfoL
// ---------------------------------------------------------------------------
//
void TPhoneCallHeaderParam::UpdateCallHeaderInfoL( 
        const TInt aCallId,
        const TBool aWaitingCall,
        const TBool aVideoCall,
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "TPhoneCallHeaderParam::UpdateCallHeaderInfoL( ) ");
    __ASSERT_DEBUG( iStateMachine.PhoneEngineInfo(), Panic( EPhoneCtrlInvariant ) );
    TBuf<KCntMaxTextFieldLength> remoteInfoText( KNullDesC );
    
    // Set call header type
    GetCallType( aCallId, aCallHeaderData );
    
    // Set CLI text for the call header
    TBool secondaryCli = GetRemoteInfoDataL( aCallId, remoteInfoText );
    if ( remoteInfoText != KNullDesC )
        {
        aCallHeaderData->SetCLIText( remoteInfoText, TPhoneCmdParamCallHeaderData::ERight );
        if ( secondaryCli )
            {
            aCallHeaderData->SetCNAPText( iStateMachine.PhoneEngineInfo()->
                RemotePhoneNumber( aCallId ), TPhoneCmdParamCallHeaderData::ELeft );       
            }
        }
    else
        {
        aCallHeaderData->SetCLIText( 
            iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ),
            TPhoneCmdParamCallHeaderData::ELeft );
        }

    // If KTelephonyLVFlagUUS is enabled it will over write RemotePartyName setting.
    // Contains possible CNAP name or received skype identification
    if ( IsFeatureSupported( KTelephonyLVFlagUUS, aCallId ) )
        {
        TBuf<KCntMaxTextFieldLength> remotePartyName( KNullDesC );
        remotePartyName.Copy( iStateMachine.PhoneEngineInfo()->RemotePartyName( aCallId ) );
       
        if ( iStateMachine.PhoneEngineInfo()->CallState( aCallId ) == EPEStateRinging )
            {
            // Set CNAP text  
            aCallHeaderData->SetCNAPText( remotePartyName, TPhoneCmdParamCallHeaderData::ERight );
            }
        else
            {
            aCallHeaderData->SetCLIText( remotePartyName, TPhoneCmdParamCallHeaderData::ERight );
            }
        }   
    
    // Set call header labels
    SetCallHeaderTexts( 
            aCallId, 
            aWaitingCall, 
            aVideoCall, 
            aCallHeaderData );
            
    // Update caller image
    SetCallerImage( 
            aCallId, 
            aCallHeaderData ); 
    }



// ---------------------------------------------------------------------------
//  TPhoneCallHeaderParam::GetRemoteInfoDataL
// ---------------------------------------------------------------------------
//
TBool TPhoneCallHeaderParam::GetRemoteInfoDataL( 
        const TInt aCallId, 
        TDes& aData ) const 
    {
    __LOGMETHODSTARTEND( EPhoneControl, "TPhoneCallHeaderParam::GetRemoteInfoDataL() ");
    __PHONELOG1( EBasic, EPhoneControl, "TPhoneCallHeaderParam::GetRemoteInfoDataL() - call id =%d ", aCallId);
    __ASSERT_DEBUG( iStateMachine.PhoneEngineInfo(), Panic( EPhoneCtrlInvariant ) );
    
    TBool secondaryCli(EFalse);
        
    if ( aCallId == KEmergencyCallId )
        {
        // Set emergency label text
        iManagerUtility.LoadResource( aData, EPhoneEmergencyCallHeader );
        }
    else
        {
        const RMobileCall::TMobileCallRemoteIdentityStatus identity = iStateMachine.PhoneEngineInfo()->RemoteIdentity( aCallId );
        // Note next if-statements are in priority order so be careful if you change order
        // or add new if-statements.
        if ( iStateMachine.PhoneEngineInfo()->RemoteName( aCallId ).Length() )
            {
            // Display the contact name if it is available
            aData.Copy( iStateMachine.PhoneEngineInfo()->RemoteName( aCallId ) );
            secondaryCli = ETrue;
            }
        else if ( iStateMachine.PhoneEngineInfo()->RemotePartyName( aCallId ).Length() )
            {
            // Display the CNAP or UUS info if it is available.
            aData.Copy( iStateMachine.PhoneEngineInfo()->RemotePartyName( aCallId ) );
            secondaryCli = ETrue;
            }
        else if ( iStateMachine.PhoneEngineInfo()->RemoteCompanyName( aCallId ).Length() )
            {
            // Display the company name if it is available
            aData.Copy( iStateMachine.PhoneEngineInfo()->RemoteCompanyName( aCallId ) );
            }
        else if ( iStateMachine.PhoneEngineInfo()->CallDirection( aCallId ) == RMobileCall::EMobileTerminated )
            {
            if ( EPEPrivateNumber == iStateMachine.PhoneEngineInfo()->RemotePhoneNumberType( aCallId ) )
                {
                if ( EPECallTypeVoIP == CallHeaderType() )
                    {
                    iManagerUtility.LoadResource( aData, iManagerUtility.Customization()->CustomizeCallHeaderText() );
                    }
                else
                    {
                    // private number
                    iManagerUtility.LoadResource( aData, EPhoneCLIWithheld );
                    }
                }
            else if ( identity == RMobileCall::ERemoteIdentityUnavailableNoCliCoinOrPayphone || 
                    identity == RMobileCall::ERemoteIdentityAvailableNoCliCoinOrPayphone )
                {
                __PHONELOG( EBasic, EPhoneControl, "GetRemoteInfoDataL : payphone" );
                // Display "Payphone".
                iManagerUtility.LoadResource( aData, EPhoneCLIPayphone );
                }
            else if ( identity == RMobileCall::ERemoteIdentityUnknown )
                {
                __PHONELOG( EBasic, EPhoneControl, "GetRemoteInfoDataL : unknown number" );
                // Display "Unknown Number".
                iManagerUtility.LoadResource( aData, EPhoneCallCLIUnknown );
                }
            else if ( iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ).Length() )
                {
                // Display the number if it is available
                aData.Copy( iStateMachine.PhoneEngineInfo()->RemotePhoneNumber( aCallId ) );
                }            
            }

        }
    return secondaryCli;
    }

