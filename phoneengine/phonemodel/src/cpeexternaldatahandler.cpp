/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of 
*                CPEExternalDataHandler class 
*
*/


// INCLUDE FILES
#include "cpeexternaldatahandler.h"
#include "mpephonemodelinternal.h"
#include <accessoriescrkeys.h>
#include <centralrepository.h>
#include <CoreApplicationUIsSDKCRKeys.h>
#include <ctsydomainpskeys.h>
#include <mpedatastore.h>
#include <pepanic.pan>
#include <settingsinternalcrkeys.h>
#include <talogger.h>
#include <telephonyvariant.hrh>
#include <telinternalcrkeys.h>


// CONSTANTS
// <-- QT PHONE START -->
//const TInt KPrefixLength = 10;
// <-- QT PHONE END -->

CPEExternalDataHandler* CPEExternalDataHandler::NewL( MPEPhoneModelInternal& aModel )
    {
    CPEExternalDataHandler* self = new ( ELeave ) CPEExternalDataHandler( *aModel.DataStore() );
    CleanupStack::PushL( self );
    self->ConstructL( aModel );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CPEExternalDataHandler::CPEExternalDataHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPEExternalDataHandler::CPEExternalDataHandler(
        MPEDataStore& aDataStore
        ) : iDataStore( aDataStore )
    {
    }

// Destructor
CPEExternalDataHandler::~CPEExternalDataHandler()
    {
    TEFLOGSTRING( KTAOBJECT, "PE CPEExternalDataHandler::~CPEExternalDataHandler" );    

    delete iTelephonyVariationRepository;
    delete iTelephonySettingsRepository;
    delete iProfileSettings;
    delete iNetworkRegistrationStatus;
    delete iLoudspeakerVolumeSetting;
    delete iEarVolumeSetting;
    delete iCallDurationDisplay;
    delete iAudioOutputPreferenceMonitor;
    delete iAccessorySettingsRepository;
    delete iAccessoryModeMonitor;
    delete iCoreApplicationRepository;
    }

// -----------------------------------------------------------------------------
// CPEExternalDataHandler::BaseConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPEExternalDataHandler::ConstructL(
        MPEPhoneModelInternal& aModel )
    {
    TEFLOGSTRING( KTAOBJECT, "PE CPEExternalDataHandler::BaseConstructL" );

    // Instantiate monitor objects
    iAudioOutputPreferenceMonitor = CPEAudioOutputPreferenceMonitor::NewL( aModel );
    // <-- QT PHONE START -->
    iEarVolumeSetting = CPEIncallEarVolumeSettingMonitor::NewL( aModel );
    iLoudspeakerVolumeSetting = CPEIncallLoudspeakerVolumeSettingMonitor::NewL( aModel );
    iCallDurationDisplay = CPECallDurationDisplaySettingMonitor::NewL(); 
    iProfileSettings = CPEProfileSettingMonitor::NewL( aModel );
    
    //TODO
    /*
    iNetworkRegistrationStatus = CPENetworkRegistrationStatusMonitor::NewL( aModel );
    iAccessoryModeMonitor = CPEAccessoryModeMonitor::NewL(); 

    // Instantiate repository objects for later use
    iAccessorySettingsRepository = CRepository::NewL( KCRUidAccessorySettings );*/
    //iTelephonySettingsRepository = CRepository::NewL( KCRUidTelephonySettings );
    iTelephonyVariationRepository = CRepository::NewL( KCRUidTelVariation );
    //iCoreApplicationRepository = CRepository::NewL( KCRUidCoreApplicationUIs ); 
    TEFLOGSTRING( KTAOBJECT, "PE CPEExternalDataHandler::BaseConstructL 2" );
    // <-- QT PHONE END -->
    }

// -----------------------------------------------------------------------------
// CPEExternalDataHandler::Get
// Gets the current value of the specified setting, if the operation was successful
// aValue contains the current valid value of the setting
// -----------------------------------------------------------------------------
//
TInt CPEExternalDataHandler::Get(
        const TPEExternalDataId aSetting,
        TInt& aValue ) const
    {
    TInt errorCode( KErrNotFound );

    // <-- QT PHONE START -->
    //TODO

    if ( EPETelephonyVariationFlags == aSetting )
        {
        errorCode = iTelephonyVariationRepository->Get( KTelVariationFlags, aValue );
        TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPETelephonyVariationFlags, error code: %d", errorCode );
        }
    else if ( EPEAudioOutputPreference == aSetting )
        {
        errorCode = iAudioOutputPreferenceMonitor->Get( aValue );
        TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEAudioOutputPreference, error code: %d", errorCode );
        }
    else if ( EPEIncallEarVolumeSetting == aSetting )
        {
        errorCode = iEarVolumeSetting->Get( aValue );
        TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEIncallEarVolumeSetting, error code: %d", errorCode );
        }
    else if ( EPEIncallLoudspeakerVolumeSetting == aSetting )
        {
        errorCode = iLoudspeakerVolumeSetting->Get( aValue );
        TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEIncallLoudspeakerVolumeSetting, error code: %d", errorCode );
        }
    else if ( EPECallDurationDisplaySetting == aSetting )
        {
        errorCode = iCallDurationDisplay->Get( aValue );
        TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPECallDurationDisplaySetting, error code: %d", errorCode );
        }
    else
        {
        errorCode = KErrNone;
        aValue = 0;        
        }
    
    // Process Common id
    /*switch ( aSetting )
        {
        case EPEAutomaticAnswerHeadsetSetting:
            {
            errorCode = GetAutomaticAnswer( KSettingsHeadsetAutomaticAnswer, aValue );
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEAutomaticAnswerHeadsetSetting, error code: %d",errorCode );
            break;
            }        
        case EPEAutomaticAnswerLoopsetSetting:
            {
            errorCode = GetAutomaticAnswer( KSettingsLoopsetAutomaticAnswer, aValue );
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEAutomaticAnswerLoopsetSetting, error code: %d", errorCode );
            break;
            }        
        case EPEAutomaticAnswerCarkitSetting:
            {
            errorCode = GetAutomaticAnswer( KSettingsCarKitAutomaticAnswer, aValue );
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEAutomaticAnswerCarkitSetting, error code: %d", errorCode );
            break;
            }        
        case EPEAutomaticAnswerMusicStandSetting:
            {
            errorCode = GetAutomaticAnswer( KSettingsMusicStandAutomaticAnswer, aValue );
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEAutomaticAnswerMusicStandSetting, error code: %d", errorCode );
            break;
            }        
        case EPEAutomaticAnswerWirelessCarkitSetting:
            {
            errorCode = GetAutomaticAnswer( KSettingsWirelessCarkitAutomaticAnswer, aValue );
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEAutomaticAnswerWirelessCarkitSetting, error code: %d", errorCode );
            break;
            }
        case EPEAudioOutputPreference:
            {
            errorCode = iAudioOutputPreferenceMonitor->Get( aValue );
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEAudioOutputPreference, error code: %d", errorCode );
            break;
            }
        case EPEDialPrefixChangeSetting:
            {
            errorCode = iTelephonySettingsRepository->Get( KSettingsDialPrefixChangeMode, aValue );
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEDialPrefixChangeSetting, error code: %d", errorCode );
            break;
            }
        case EPEIncallEarVolumeSetting:
            {
            errorCode = iEarVolumeSetting->Get( aValue );
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEIncallEarVolumeSetting, error code: %d", errorCode );
            break;
            }
        case EPEIncallLoudspeakerVolumeSetting:
            {
            errorCode = iLoudspeakerVolumeSetting->Get( aValue );
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEIncallLoudspeakerVolumeSetting, error code: %d", errorCode );
            break;
            }
        case EPENetworkRegistrationStatus:
            {
            errorCode = iNetworkRegistrationStatus->Get( aValue );
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPENetworkRegistrationStatus, error code: %d", errorCode );
            break;
            }
        case EPETelephonyVariationFlags:
            {
            errorCode = iTelephonyVariationRepository->Get( KTelVariationFlags, aValue );
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPETelephonyVariationFlags, error code: %d", errorCode );
            break;
            }
        case EPEAccessoryMode:            
            {
            errorCode = KErrNone;
            aValue = iAccessoryModeMonitor->AccessoryMode();
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::Get EPEAccessoryMode, error code: %d", errorCode );
            break;
            }
        case EPENetworkConnectionAllowedSetting:
            {
            // Fetches setting that indicates if network connection is allowed,
            // i.e. is the phone in off-line mode. This method is used while
            // attempting an emergency call, so no memory may be consumed.
            errorCode = iCoreApplicationRepository
                ->Get( KCoreAppUIsNetworkConnectionAllowed, aValue );
            TEFLOGSTRING2( KTAINT, "CPEGsmExternalDataHandler::Get EPENetworkConnectionAllowedSetting, error code: %d",errorCode );
            break;
            }
        default:
            {
            TEFLOGSTRING2( KTAERROR, 
                "PE CPEEXTERNALDATAHANDLER::GET UNKNOWN SETTING ID ! %d", aSetting );
            break;
            }
        }*/
    // <-- QT PHONE END -->
    return errorCode;
    }

// -----------------------------------------------------------------------------
// CPEExternalDataHandler::Get
// Gets the current value of the specified setting, if the operation was successful
// aValue contains the current valid value of the setting
// -----------------------------------------------------------------------------
//
TInt CPEExternalDataHandler::GetText(
        const TPEExternalDataId /*aSetting*/,
        TDesC& aValue ) const
    {
    TInt errorCode( KErrNotFound );
    // <-- QT PHONE START -->
    //TODO
    aValue = KNullDesC;
    errorCode = KErrNone;
    // Process Common id
    /*switch ( aSetting )
        {
        case EPEDialPrefixTextSetting:
            {
            TBuf<KPrefixLength> prefixTextSetting;
            
            errorCode = iTelephonySettingsRepository->Get( 
                KSettingsDialPrefixText, 
                prefixTextSetting );
                
            aValue = prefixTextSetting;
            
            TEFLOGSTRING2( KTAINT, "CPEExternalDataHandler::GetText EPEDialPrefixTextSetting, error code: %d", errorCode );
            break;
            }
        default:
            {
            TEFLOGSTRING2( KTAERROR, 
                "PE CPEEXTERNALDATAHANDLER::GETTEXT UNKNOWN SETTING ID ! %d", aSetting );
            break;
            }
        }*/
    // <-- QT PHONE END -->
    return errorCode;
    }

// -----------------------------------------------------------------------------
// CPEExternalDataHandler::GetAutomaticAnswer
// Check if automatic answer is defined for connected accessory.
// -----------------------------------------------------------------------------
//
TInt CPEExternalDataHandler::GetAutomaticAnswer(
        TUint32 /*aSetting*/,
        TInt& aValue ) const
    {
    TInt errorCode( KErrNotFound );
    
    TEFLOGSTRING( KTAINT, "CPEExternalDataHandler::GetAutomaticAnswer" );
    // <-- QT PHONE START -->
    //errorCode = iAccessorySettingsRepository->Get( aSetting, aValue );
    errorCode = KErrNone;
    aValue = 0;
    // <-- QT PHONE END -->
    return errorCode;
    }

// -----------------------------------------------------------------------------
// CPEExternalDataHandler::Set
// Sets the value of the specified setting
// -----------------------------------------------------------------------------
//
TInt CPEExternalDataHandler::Set(
        const TPEExternalDataId aSetting,
        const TInt aValue )
    {
    TInt errorCode( KErrNotFound );

    switch ( aSetting )
        {
        // <-- QT PHONE START -->
        case EPEIncallEarVolumeSetting:
            {
            errorCode = iEarVolumeSetting->Set( aValue );
            break;
            }
        case EPEIncallLoudspeakerVolumeSetting:
            {
            errorCode = iLoudspeakerVolumeSetting->Set( aValue );
            break;
            }
        // <-- QT PHONE END -->
        case EPEEmergencyCallInfo:
            {
            errorCode = RProperty::Set( KPSUidCtsyEmergencyCallInfo, 
                                        KCTSYEmergencyCallInfo, aValue );
            TEFLOGSTRING2( KTAERROR, 
            "PE CPEExternalDataHandler::Set KPSUidCtsyEmergencyCallInfo: %d", aValue );
            break;            
            }
        default:
            {
            TEFLOGSTRING2( KTAERROR, "PE CPEEXTERNALDATAHANDLER::SET UNKNOWN SETTING ID ! %d", aSetting );
            break;
            }
        }
    return errorCode;
    }

// End of file
