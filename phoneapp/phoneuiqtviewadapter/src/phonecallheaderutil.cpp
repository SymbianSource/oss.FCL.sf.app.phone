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
* Description:  A parameter class for setting call header information
*
*/


#include <mpeclientinformation.h>
#include <StringLoader.h>
#include <bautils.h>

#include "phonecallheaderutil.h"
#include "tphonecmdparamboolean.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "phoneui.pan"
#include "cphonecenrepproxy.h"
#include "telephonyvariant.hrh"
#include "phoneviewcommanddefinitions.h"
#include "phonelogger.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::PhoneCallHeaderUtil
// -----------------------------------------------------------------------------
//  
PhoneCallHeaderUtil::PhoneCallHeaderUtil(
        MPEEngineInfo& engineInfo ) 
        : m_engineInfo( engineInfo),
          iCallHeaderType( EPECallTypeUninitialized ),
          iSetDivertIndication( EFalse ),
          iLabelText( NULL ),
          iEmergencyHeaderText( NULL ),
          iAttemptingEmergencyText( NULL )
    {
    }

// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::~PhoneCallHeaderUtil
// -----------------------------------------------------------------------------
//  
PhoneCallHeaderUtil::~PhoneCallHeaderUtil()
    {
    delete iLabelText;
    
    delete iEmergencyHeaderText;
    
    delete iAttemptingEmergencyText;
    }

// -----------------------------------------------------------
// PhoneCallHeaderUtil::SetCallHeaderTexts
// -----------------------------------------------------------
//
void PhoneCallHeaderUtil::SetCallHeaderTexts(
    const TInt aCallId, 
    const TBool aWaitingCall,
    const TBool aVideoCall,
    TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::SetCallHeaderTexts( ) ");
    
    TInt labelId(KPhoneRssCommonFirst);
    
    // Fetch engine info parameters.
    const TBool auxLine( m_engineInfo.CallALSLine( aCallId ) == CCCECallParameters::ECCELineTypeAux );
    const TBool cli( m_engineInfo.RemotePhoneNumber( aCallId ).Length());
    const TBool cnap( m_engineInfo.RemotePartyName( aCallId ).Length());
    const TInt numberType( m_engineInfo.RemotePhoneNumberType( aCallId ));
    
    __PHONELOG2( EBasic, EPhoneControl, "PhoneCallHeaderUtil::SetCallHeaderTexts - NumberType(%d), CLI(%d)", numberType, cli );
    __PHONELOG2( EBasic, EPhoneControl, "PhoneCallHeaderUtil::SetCallHeaderTexts - CNAP(%d), AuxLine(%d)", cnap, auxLine );
    
    if ( !cli && !cnap && numberType != EPEPrivateNumber && numberType != EPEUnknownNumber )
        {
        if ( auxLine )
            {
            if ( aWaitingCall )
                {
                labelId = EPhoneIncomingLine2WaitingText; // waiting, line 2
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
                }
            else
                {
                labelId = EPhoneCallWaitingLabel; // waiting
                }
            }
        else // Mo other calls
            {
            if ( auxLine )
                {
                labelId = EPhoneIncomingLine2CallingText; // calling, line 2
                }
            else
                {
                // If CLIR, but not network limitation, then second line 
                // (calling or video call) should be shown in call bubble.
                if ( aVideoCall )
                    {
                    labelId = EPhoneVideoCallIncoming; // video call
                    }
                else
                    {
                    labelId = EPhoneIncomingCallLabel; // calling
                    }
                }
            }
        }
    __PHONELOG1( EBasic, EPhoneControl, "PhoneCallHeaderUtil::SetCallHeaderTexts - labelId(%d)", labelId );
    LoadCallHeaderTexts( labelId, aCallHeaderData );
    }
 
// -----------------------------------------------------------
// PhoneCallHeaderUtil::LabelText
// -----------------------------------------------------------
//
const TDesC& PhoneCallHeaderUtil::LabelText( TInt aCallId )
    {

    if (iLabelText)
        {
        delete iLabelText;
        iLabelText = NULL;
        }
    
    switch (m_engineInfo.CallState(aCallId))
        {
        case EPEStateDisconnecting:
            {
            iLabelText = LoadResource(EPhoneInCallDisconnected);
            break;
            }
        case EPEStateHeld:
        case EPEStateHeldConference:
            {
            iLabelText = LoadResource(EPhoneCallOnHold);
            break;
            }
        case EPEStateConnectedConference:
            {
            iLabelText = LoadResource(EPhoneCLIConferenceCall);
            break;
            }
        case EPEStateRinging:
            {
            TPhoneCmdParamCallHeaderData callHeaderData;
            SetCallHeaderTexts(
                aCallId, 
                EFalse,
                IsVideoCall( aCallId ),
                &callHeaderData );           

            if (callHeaderData.LabelText().Length())
                {
                iLabelText = callHeaderData.LabelText().Alloc();
                }
            break;
            }
        default:
            break;
        }
    
    return iLabelText ? *iLabelText : KNullDesC();
    }

// -----------------------------------------------------------
// PhoneCallHeaderUtil::EmergencyHeaderText
// -----------------------------------------------------------
//
const TDesC& PhoneCallHeaderUtil::EmergencyHeaderText()
    {
    if (!iEmergencyHeaderText)
        {
        iEmergencyHeaderText = LoadResource(EPhoneEmergencyCallHeader);
        }
     
    return iEmergencyHeaderText ? *iEmergencyHeaderText : KNullDesC();
    }

// -----------------------------------------------------------
// PhoneCallHeaderUtil::AttemptingEmergencyText
// -----------------------------------------------------------
//
const TDesC& PhoneCallHeaderUtil::AttemptingEmergencyText()
    {
    if (!iAttemptingEmergencyText)
        {
        iAttemptingEmergencyText = LoadResource(EPhoneAttemptingEmergencyText);
        }
     
    return iAttemptingEmergencyText ? *iAttemptingEmergencyText : KNullDesC();
    }

// -----------------------------------------------------------
// PhoneCallHeaderUtil::CallState
// -----------------------------------------------------------
//
TInt PhoneCallHeaderUtil::CallState( TInt aCallId ) const
    {
    return m_engineInfo.CallState(aCallId);
    }

// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::SetCliParamatersL
// -----------------------------------------------------------------------------
// 
void PhoneCallHeaderUtil::SetCliParamaters(
        const TInt aCallId, 
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::SetCliParamatersL( ) ");
    
    // Set call header number type
    aCallHeaderData->SetNumberType( m_engineInfo.RemotePhoneNumberType( aCallId ) );
    
    if ( ( m_engineInfo.RemotePhoneNumber( aCallId ).Length() ) && 
         ( !ContactInfoAvailable( aCallId ) ) )
        {
        // Set phonenumber/URI as the CLI text for the call header      
       aCallHeaderData->SetCLIText( m_engineInfo.RemotePhoneNumber( aCallId ),
                 TPhoneCmdParamCallHeaderData::ELeft );
            
        // No contact name, use phonenumber when available.
        aCallHeaderData->SetParticipantListCLI(
                TPhoneCmdParamCallHeaderData::EPhoneParticipantCNAPText );
        }
    else
        {
        TPhoneCmdParamCallHeaderData::TPhoneTextClippingDirection cnapClippingDirection = TPhoneCmdParamCallHeaderData::ERight;
        TBuf<KCntMaxTextFieldLength> remoteInfoText( KNullDesC );
 
        TBool secondaryCli = GetRemoteInfoData( aCallId, remoteInfoText );
        cnapClippingDirection = TPhoneCmdParamCallHeaderData::ELeft;

        aCallHeaderData->SetCLIText( remoteInfoText,  TPhoneCmdParamCallHeaderData::ERight );
        
        if (secondaryCli)
            {
            aCallHeaderData->SetCNAPText( m_engineInfo.RemotePhoneNumber( aCallId ), 
                cnapClippingDirection );
            }
        }
    
    SetCallerImage( aCallId, aCallHeaderData );

    // Set the Caller text
    if ( m_engineInfo.CallerText( aCallId ).Length() > 0 )
        {
        aCallHeaderData->SetCallerText( m_engineInfo.CallerText( aCallId ) );
        }
    }

// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::SetCallerImage
// -----------------------------------------------------------------------------
//     
void PhoneCallHeaderUtil::SetCallerImage( 
        const TInt aCallId, 
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::SetCallerImage( ) ");
    // Set the call header picture data if it is available
    if ( ( m_engineInfo.CallerImage( aCallId ).Length() > 0 )  && 
         ( BaflUtils::FileExists( CCoeEnv::Static()->FsSession(), 
                 m_engineInfo.CallerImage( aCallId ) ) ) )
        {
        aCallHeaderData->SetPicture( m_engineInfo.CallerImage( aCallId ) );
        }
    else
        {
        // Set the thumbnail picture data if it is available
        aCallHeaderData->SetHasThumbnail( m_engineInfo.HasCallerThumbnail( aCallId ) );
        CFbsBitmap* picture = m_engineInfo.CallerThumbnail( aCallId );
        if ( picture )
            {
            aCallHeaderData->SetThumbnail( picture );
            }
        }
    }

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::SetBasicCallHeaderParamsL
// ---------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::SetBasicCallHeaderParams(
        const TInt aCallId, 
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::SetBasicCallHeaderParamsL( ) ");
    // Set call header call state
    aCallHeaderData->SetCallState( 
            m_engineInfo.CallState( aCallId ) );

    // Set call header type            
    aCallHeaderData->SetCallType( GetCallType( aCallId, aCallHeaderData ) );
    
    // Set call header voice privacy status
    aCallHeaderData->SetCiphering( 
            m_engineInfo.IsSecureCall( aCallId ) );
    aCallHeaderData->SetCipheringIndicatorAllowed( 
            m_engineInfo.SecureSpecified() );
    
    //see service provider settings API
    aCallHeaderData->SetServiceId(
            m_engineInfo.ServiceId( aCallId ) );
       
    // Set contact link, see virtual phonebook API
    aCallHeaderData->SetContactLink(
            m_engineInfo.ContactLink( aCallId ) );
       
    // Set remote phone number
    aCallHeaderData->SetRemotePhoneNumber(
            m_engineInfo.RemotePhoneNumber( aCallId ) );
    }

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::GetCallType
// ---------------------------------------------------------------------------
//
TPECallType PhoneCallHeaderUtil::GetCallType( 
        const TInt aCallId,
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::GetCallType( ) ");
    // Set call header type.
    TPECallType callType = 
            m_engineInfo.CallType( aCallId );
    SetCallHeaderType( callType );
    
    if ( m_engineInfo.CallALSLine( aCallId ) 
         == CCCECallParameters::ECCELineTypeAux )
        {
        aCallHeaderData->SetLine2( ETrue );
        }

    __PHONELOG1( EBasic, EPhoneControl, 
                "PhoneCallHeaderUtil::GetCallType() - callType: %d ", 
                callType )
    return callType;
    }

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::SetCallHeaderType
// ---------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::SetCallHeaderType( 
    TInt aCallHeaderType )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::SetCallHeaderType( ) ");
    iCallHeaderType = aCallHeaderType;
    __PHONELOG1( EBasic, EPhoneControl, 
                    "PhoneCallHeaderUtil::SetCallHeaderType() - iCallHeaderType: %d ", 
                    iCallHeaderType )
    }

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::CallHeaderType
// ---------------------------------------------------------------------------
//
TInt PhoneCallHeaderUtil::CallHeaderType() const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::CallHeaderType( ) ");
    __PHONELOG1( EBasic, EPhoneControl, 
                        "PhoneCallHeaderUtil::CallHeaderType() - iCallHeaderType: %d ", 
                        iCallHeaderType )
    return iCallHeaderType;            
    }

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::ContactInfoAvailable
// ---------------------------------------------------------------------------
//
TBool PhoneCallHeaderUtil::ContactInfoAvailable( const TInt aCallId ) const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::ContactInfoAvailable( ) ");
    TBool contactAvailable = EFalse;
    if ( ( m_engineInfo.RemoteName( aCallId ).Length() ) || 
         ( m_engineInfo.RemoteCompanyName( aCallId ).Length() ) )
        {
        contactAvailable = ETrue;
        }
    return contactAvailable;
    }

// -----------------------------------------------------------------------------
// PhoneCallHeaderUtil::SetCliAndCnapParamatersL
// -----------------------------------------------------------------------------
// 
void PhoneCallHeaderUtil::SetCliAndCnapParamaters(
        const TInt aCallId, 
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::SetCliAndCnapParamatersL( ) ");
    TBuf<KCntMaxTextFieldLength> cnapText( KNullDesC );

    // Set call header number type
    aCallHeaderData->SetNumberType( m_engineInfo.RemotePhoneNumberType( aCallId ) );
        
    const MPEClientInformation& info = 
            m_engineInfo.CallClientInformation( aCallId );

    if ( ( m_engineInfo.RemotePhoneNumber( aCallId ).Length() ) && 
         ( !ContactInfoAvailable( aCallId ) ) && 
         ( !info.ShowNumber() ) )
        {
        // No contact info data available; use the phone number
        aCallHeaderData->SetCLIText(
                m_engineInfo.RemotePhoneNumber( aCallId ),
                TPhoneCmdParamCallHeaderData::ELeft);
        
        // No contact name, use phonenumber when available.
        aCallHeaderData->SetParticipantListCLI( 
                TPhoneCmdParamCallHeaderData::EPhoneParticipantCNAPText );
        }
    else
        {
        TBuf<KCntMaxTextFieldLength> remoteInfoText( KNullDesC );
        
        GetRemoteInfoData( aCallId, remoteInfoText );
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
    if ( m_engineInfo.CallerText( aCallId ).Length() > 0 )
        {
        aCallHeaderData->SetCallerText( m_engineInfo.CallerText( aCallId ) );
        }        
    
    // Set the call header CNAP data ( Contains possible CNAP name or received skype identification ).
    if ( IsFeatureSupported( KTelephonyLVFlagUUS, aCallId ) )
        {
        aCallHeaderData->SetCNAPText( m_engineInfo.RemotePartyName( aCallId ), 
                TPhoneCmdParamCallHeaderData::ERight );
        }
    }

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::GetCNAPText
// ---------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::GetCNAPText( 
    const TInt aCallId,
       TDes& aData, 
       TPhoneCmdParamCallHeaderData::TPhoneTextClippingDirection& aDirection ) const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::GetCNAPText( ) ");
    
    // Set clipping direction  
    aDirection = TPhoneCmdParamCallHeaderData::ERight;
    
    // If it's not a private number show further info
    if ( m_engineInfo.RemotePhoneNumberType( aCallId ) != 
        EPEPrivateNumber )
        {
        if ( ( m_engineInfo.RemoteName( aCallId ).Length() ||
               m_engineInfo.RemotePartyName( aCallId ).Length() || 
               m_engineInfo.RemoteCompanyName( aCallId ).Length() ) &&
               m_engineInfo.RemotePhoneNumber( aCallId ).Length() )
            {
            // Use the phone number for the CNAP display
            aData.Copy( m_engineInfo.RemotePhoneNumber( aCallId ) );
            
            // Clipping direction for non-private number
            aDirection = TPhoneCmdParamCallHeaderData::ELeft;
            }
        }
    }

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::IsFeatureSupported
// ---------------------------------------------------------------------------
//
TBool PhoneCallHeaderUtil::IsFeatureSupported( 
        const TInt aFeatureKey, 
        const TInt aCallId ) const
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::IsFeatureSupported( ) ");
    TBool featureSupport(EFalse);
    switch( aFeatureKey )
        {
        case KTelephonyLVFlagUUS:
            {
            if( ( CPhoneCenRepProxy::Instance()->IsTelephonyFeatureSupported( aFeatureKey ) ) &&
                ( m_engineInfo.RemotePartyName( aCallId ).Length() ) )
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
            "PhoneCallHeaderUtil::IsFeatureSupported() - featureSupport: %d ", 
            featureSupport )
    
    return featureSupport;
    }

// ---------------------------------------------------------------------------
// PhoneCallHeaderUtil::SetDivertIndicatorToCallHeader
// ---------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::SetDivertIndicatorToCallHeader( 
    const TInt aCallId, 
    TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::SetDivertIndicatorToCallHeader( ) ");
    if( m_engineInfo.IncomingCallForwarded( aCallId ) )
        {
        aCallHeaderData->SetDiverted( ETrue );
        }
    
    if ( m_engineInfo.CallALSLine( aCallId ) == CCCECallParameters::ECCELineTypeAux )
        {
        __PHONELOG( EBasic, EPhoneControl, 
                "PhoneCallHeaderUtil::SetDivertIndicatorToCallHeader - CallALSLine() == CCCECallParameters::ECCELineTypeAux");
        aCallHeaderData->SetLine2( ETrue );    
        }        
    }

// ---------------------------------------------------------------------------
// PhoneCallHeaderUtil::SetDivertIndication
// ---------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::SetDivertIndication( const TBool aDivertIndication )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::SetDivertIndication( ) ");
    iSetDivertIndication = aDivertIndication;           
    __PHONELOG1( EBasic, EPhoneControl, 
                "PhoneCallHeaderUtil::SetDivertIndication() - iSetDivertIndication: %d ", 
                iSetDivertIndication )
    }

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::SetIncomingCallHeaderParams
// ---------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::SetIncomingCallHeaderParams(
        const TInt aCallId, 
        const TBool aWaitingCall,
        const TBool aVideoCall,
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::SetIncomingCallHeaderParamsL( ) ");
    // Set basic params must be called before update is called.
    SetBasicCallHeaderParams( aCallId, aCallHeaderData );
    
    // Set call header labels
    SetCallHeaderTexts( 
            aCallId, 
            aWaitingCall, 
            aVideoCall, 
            aCallHeaderData );
    
    SetCliAndCnapParamaters( aCallId, aCallHeaderData );
    
    // Set divert indication to call header if needed.
    SetDivertIndicatorToCallHeader( aCallId, aCallHeaderData );
    }

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::SetOutgoingCallHeaderParams
// ---------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::SetOutgoingCallHeaderParams(
        const TInt aCallId,
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::SetOutgoingCallHeaderParams( ) ");
    // Set basic params must be called before update is called.
    SetBasicCallHeaderParams( aCallId, aCallHeaderData );
    
    // Set call header labels
    if ( aCallHeaderData->CallType() == EPECallTypeVideo )
        {
        LoadCallHeaderTexts( 
                EPhoneOutgoingVideoCallLabel, 
                aCallHeaderData );
        }
    else
        {
        LoadCallHeaderTexts( 
                EPhoneOutgoingCallLabel, 
                aCallHeaderData );
        }
    
    SetCliParamaters( aCallId, aCallHeaderData );
    }

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::UpdateCallHeaderInfo
// ---------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::UpdateCallHeaderInfo( 
        const TInt aCallId,
        const TBool aWaitingCall,
        const TBool aVideoCall,
        TPhoneCmdParamCallHeaderData* aCallHeaderData )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "PhoneCallHeaderUtil::UpdateCallHeaderInfoL( ) ");
    TBuf<KCntMaxTextFieldLength> remoteInfoText( KNullDesC );
    
    // Set call header type
    GetCallType( aCallId, aCallHeaderData );
    
    // Set CLI text for the call header
    TBool secondaryCli = GetRemoteInfoData( aCallId, remoteInfoText );
    if ( remoteInfoText != KNullDesC )
        {
        aCallHeaderData->SetCLIText( remoteInfoText, TPhoneCmdParamCallHeaderData::ERight );
        if ( secondaryCli )
            {
            aCallHeaderData->SetCNAPText( m_engineInfo.
                RemotePhoneNumber( aCallId ), TPhoneCmdParamCallHeaderData::ELeft );       
            }
        }
    else
        {
        aCallHeaderData->SetCLIText( 
            m_engineInfo.RemotePhoneNumber( aCallId ),
                TPhoneCmdParamCallHeaderData::ELeft );
        }

    // If KTelephonyLVFlagUUS is enabled it will over write RemotePartyName setting.
    // Contains possible CNAP name or received skype identification
    if ( IsFeatureSupported( KTelephonyLVFlagUUS, aCallId ) )
        {
        TBuf<KCntMaxTextFieldLength> remotePartyName( KNullDesC );
        remotePartyName.Copy( m_engineInfo.RemotePartyName( aCallId ) );
       
        if ( m_engineInfo.CallState( aCallId ) == EPEStateRinging )
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
//  PhoneCallHeaderUtil::GetRemoteInfoDataL
// ---------------------------------------------------------------------------
//
TBool PhoneCallHeaderUtil::GetRemoteInfoData( 
        const TInt aCallId, 
        TDes& aData ) const 
    {
    __LOGMETHODSTARTEND( EPhoneControl, "PhoneCallHeaderUtil::GetRemoteInfoDataL() ");
    __PHONELOG1( EBasic, EPhoneControl, "PhoneCallHeaderUtil::GetRemoteInfoDataL() - call id =%d ", aCallId);
    
    TBool secondaryCli(EFalse);
        
    if ( aCallId == KEmergencyCallId )
        {
        // Set emergency label text
        LoadResource( aData, EPhoneEmergencyCallHeader );
        }
    else
        {
        const RMobileCall::TMobileCallRemoteIdentityStatus identity = 
                m_engineInfo.RemoteIdentity( aCallId );
        // Note next if-statements are in priority order so be careful if you change order
        // or add new if-statements.
        if ( m_engineInfo.RemoteName( aCallId ).Length() )
            {
            // Display the contact name if it is available
            aData.Copy( m_engineInfo.RemoteName( aCallId ) );
            secondaryCli = ETrue;
            }
        else if ( m_engineInfo.RemotePartyName( aCallId ).Length() )
            {
            // Display the CNAP or UUS info if it is available.
            aData.Copy( m_engineInfo.RemotePartyName( aCallId ) );
            secondaryCli = ETrue;
            }
        else if ( m_engineInfo.RemoteCompanyName( aCallId ).Length() )
            {
            // Display the company name if it is available
            aData.Copy( m_engineInfo.RemoteCompanyName( aCallId ) );
            }
        else if ( m_engineInfo.CallDirection( aCallId ) == RMobileCall::EMobileTerminated )
            {
            if ( EPEPrivateNumber == m_engineInfo.RemotePhoneNumberType( aCallId ) )
                {
                if ( EPECallTypeVoIP == CallHeaderType() )
                    {
                    // TODO LoadResource( aData, iManagerUtility.Customization()->CustomizeCallHeaderText() );
                    }
                else
                    {
                    // private number
                    LoadResource( aData, EPhoneCLIWithheld );
                    }
                }
            else if ( identity == RMobileCall::ERemoteIdentityUnavailableNoCliCoinOrPayphone || 
                    identity == RMobileCall::ERemoteIdentityAvailableNoCliCoinOrPayphone )
                {
                // Display "Payphone".
                LoadResource( aData, EPhoneCLIPayphone );
                }
            else if ( identity == RMobileCall::ERemoteIdentityUnknown )
                {
                __PHONELOG( EBasic, EPhoneControl, "GetRemoteInfoDataL : unknown number" );
                // Display "Unknown Number".
                LoadResource( aData, EPhoneCallCLIUnknown );
                }
            else if ( m_engineInfo.RemotePhoneNumber( aCallId ).Length() )
                {
                // Display the number if it is available
                aData.Copy( m_engineInfo.RemotePhoneNumber( aCallId ) );
                }            
            }

        }
    return secondaryCli;
    }

// -----------------------------------------------------------
// PhoneCallHeaderUtil::LoadCallHeaderTexts
// -----------------------------------------------------------
//
void PhoneCallHeaderUtil::LoadCallHeaderTexts(
    const TInt aLabelId, 
    TPhoneCmdParamCallHeaderData* aCallHeaderData ) 
    {
    if ( aLabelId != EPhoneStringList )
        {
        TBuf<KPhoneCallHeaderLabelMaxLength> labelText( KNullDesC );
        LoadResource( labelText, aLabelId );
        aCallHeaderData->SetLabelText( labelText );
        }
    }

// ---------------------------------------------------------------------------
// PhoneCallHeaderUtil::LoadResource
// ---------------------------------------------------------------------------
//
void PhoneCallHeaderUtil::LoadResource( TDes& aData, const TInt aResource ) const
    {
    StringLoader::Load(  
            aData, CPhoneMainResourceResolver::Instance()->ResolveResourceID( aResource ), 
            CCoeEnv::Static() );
    }

// ---------------------------------------------------------------------------
// PhoneCallHeaderUtil::LoadResource
// ---------------------------------------------------------------------------
//
HBufC* PhoneCallHeaderUtil::LoadResource( const TInt aResource ) const
    {
    HBufC* ret = NULL;
    
    TRAP_IGNORE( ret = StringLoader::LoadL(  
            CPhoneMainResourceResolver::Instance()->ResolveResourceID( aResource ), 
            CCoeEnv::Static() ) );
    
    return ret;
    }

// ---------------------------------------------------------------------------
//  PhoneCallHeaderUtil::IsVideoCall
// ---------------------------------------------------------------------------
//
TBool PhoneCallHeaderUtil::IsVideoCall(int aCallId ) const
    {
    if( aCallId < 0 )
        {
        return ( m_engineInfo.CallTypeCommand()
            == EPECallTypeVideo );  
        }
    return ( m_engineInfo.CallType( aCallId )
            == EPECallTypeVideo );
    }
