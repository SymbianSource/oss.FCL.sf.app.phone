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
* Description:  Acessory and Bluetooth handler.
*
*/


// INCLUDE FILES
#include    <featmgr.h>
#include    <StringLoader.h>
#include    <btengdomainpskeys.h>
#include    <btengdomaincrkeys.h>
#include    <mpeengineinfo.h>
#include    <AknNotifyStd.h>

#include    "cphoneaccessorybthandler.h"
#include    "cphonecenrepproxy.h"
#include    "cphonemainresourceresolver.h"
#include    "mphoneneclearedhandler.h"
#include    "mphonestatemachine.h"
#include    "mphoneviewcommandhandle.h"
#include    "phoneconstants.h"
#include    "phonelogger.h"
#include    "phoneui.pan"
#include    "phonerssbase.h"
#include    "tphonecmdparamnote.h"
#include    "tphonecmdparamglobalnote.h"


// CONSTANTS

const TInt KPhoneBtAddressTextLength = 64;

// ============================ MEMBER FUNCTIONS ===============================

CPhoneAccessoryBTHandler::CPhoneAccessoryBTHandler( 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneStateMachine* aStateMachine,
    MPhoneNEClearedHandler* aNEClearedHandler ) :
    iViewCommandHandle( aViewCommandHandle ),
    iStateMachine( aStateMachine ),
    iNEClearedHandler( aNEClearedHandler )
    {
    }

// -----------------------------------------------------------
// CPhoneAccessoryBTHandler::NewLC()
// Constructor
// (other items were commented in a header).
// -----------------------------------------------------------
//
CPhoneAccessoryBTHandler* CPhoneAccessoryBTHandler::NewLC( 
    MPhoneViewCommandHandle* aViewCommandHandle,
    MPhoneStateMachine* aStateMachine,
    MPhoneNEClearedHandler* aNEClearedHandler )
    {
    CPhoneAccessoryBTHandler* self = new (ELeave) CPhoneAccessoryBTHandler( 
        aViewCommandHandle, aStateMachine, aNEClearedHandler );
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneAccessoryBTHandler::~CPhoneAccessoryBTHandler
// -----------------------------------------------------------------------------
//
CPhoneAccessoryBTHandler::~CPhoneAccessoryBTHandler()
    {
    }

// -----------------------------------------------------------
// CPhoneAccessoryBTHandler::SetBTDebugModeL
// Handling for message EPEMessageBTDebugMode.
// Sets BT debug mode PS key on.
// -----------------------------------------------------------
//
void CPhoneAccessoryBTHandler::SetBTDebugModeL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneAccessoryBTHandler::SetBTDebugModeL( ) ");
    if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) )
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewClearNumberEntryContent );                      
        }
    else
        {
        // Remove number entry from screen
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );   
        }
    iNEClearedHandler->HandleNumberEntryClearedL();
    if ( RProperty::Set( KPSUidBluetoothDutMode, KBTSspDebugmode, EBTSspDebugModeOn ))
        {
        __PHONELOG( EOnlyFatal, EPhoneControl, "CPhoneAccessoryBTHandler::SetBTDebugModeL PS key not defined" );
        }
    }

// ---------------------------------------------------------
// CPhoneAccessoryBTHandler::SetBTHandsfreeModeL
// ---------------------------------------------------------
//
TBool CPhoneAccessoryBTHandler::SetBTHandsfreeModeL( TBool aHandsfreeMode )
    {
    return DoSetHandsfreeModeL( aHandsfreeMode, aHandsfreeMode ? EPEBTAudioAccessory : EPEHandset );
    }

// ---------------------------------------------------------
// CPhoneAccessoryBTHandler::SetHandsfreeModeL
// ---------------------------------------------------------
//
TBool CPhoneAccessoryBTHandler::SetHandsfreeModeL( TBool aHandsfreeMode )
    {
    return DoSetHandsfreeModeL( aHandsfreeMode, aHandsfreeMode ? EPELoudspeaker : EPEHandset );
    }

// ---------------------------------------------------------
// CPhoneAccessoryBTHandler::ShowBTAddressL
// ---------------------------------------------------------
//
void CPhoneAccessoryBTHandler::ShowBTAddressL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneAccessoryBTHandler::ShowBTAddressL( ) ");
    if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ))
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewClearNumberEntryContent );                      
        }
    else
        {
        // Remove number entry from screen
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );   
        }
     
    // Get BT address
    TBuf<KPhoneBtAddressTextLength> addressBuffer;

    CPhoneCenRepProxy::Instance()->GetString(             
        KCRUidBluetoothLocalDeviceAddress,
        KBTLocalDeviceAddress,
        addressBuffer );
    
    // BT address was empty. BT is not turned on.
    if ( addressBuffer.Length() == 0 )
        {
        __PHONELOG( EBasic, EPhoneControl, "CPhoneAccessoryBTHandler::ShowBTAddressL.NoAddress" );
        }

    // So we got the address. Now we need the localised text:
    HBufC* buf = StringLoader::LoadLC( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhonePhoneBtDevAddress ) , 
        addressBuffer );
        
    TPhoneCmdParamNote noteParam;
    noteParam.SetType( EPhoneNoteCustom );
    noteParam.SetResourceId( CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneInformationWaitNote ) );
    noteParam.SetText( *buf );

    // Display note
    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNote, &noteParam );
    
    CleanupStack::PopAndDestroy( buf );               
    }    

// -----------------------------------------------------------
// CPhoneAccessoryBTHandler::ShowBTLoopbackL
// Handling for message EPEMessageShowBTLoopback.
// Shows BT loopback status note.
// -----------------------------------------------------------
//
void CPhoneAccessoryBTHandler::ShowBTLoopbackL()
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneAccessoryBTHandler::ShowBTLoopbackL( ) ");
    if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ))
        {
        iViewCommandHandle->ExecuteCommandL( EPhoneViewClearNumberEntryContent );                      
        }
    else
        {
        // Remove number entry from screen
        iViewCommandHandle->ExecuteCommandL( EPhoneViewRemoveNumberEntry );   
        }
    TInt btLoopback( EBTDutOff );

    // Get current status of BT loopback
    TInt err = RProperty::Get( KPSUidBluetoothDutMode, KBTDutEnabled, 
        btLoopback );

    // If loopback was disabled enable it. According to current knowledge
    // disabling BT test loop is not possible. If error getting the value
    // assume that it is disabled.
    if ( btLoopback == EBTDutOff || err != KErrNone )
        {
        // Safe to overwrite the previous error code since it is nolonger needed
        err = RProperty::Set( KPSUidBluetoothDutMode,
                  KBTDutEnabled,
                  EBTDutOn );
        if( !err )
            {
            btLoopback = EBTDutOn;
            }
        }

    HBufC* btLoopbackString( NULL );

    // Get localised text according to setting status
    btLoopbackString = StringLoader::LoadLC( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( btLoopback == EBTDutOn ? 
        EPhoneBtLoopbackEnabled : EPhoneBtLoopbackDisabled ) );

    TPhoneCmdParamNote noteParam;
    noteParam.SetType( EPhoneNoteCustom );
    noteParam.SetResourceId( CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneInformationWaitNote ) );
    noteParam.SetText( *btLoopbackString );
    noteParam.SetTone( CAknNoteDialog::EConfirmationTone );

    // Display note
    iViewCommandHandle->ExecuteCommandL( EPhoneViewShowNote, &noteParam );

    CleanupStack::PopAndDestroy( btLoopbackString );
    }

// -----------------------------------------------------------
// CPhoneAccessoryBTHandler::ShowBTActivatedL
// Shows BT activated note.
// -----------------------------------------------------------
//
void CPhoneAccessoryBTHandler::ShowBTActivatedL()
    {
 
    TPhoneCmdParamGlobalNote globalNoteParam;
    globalNoteParam.SetType( EAknGlobalInformationNote );
    globalNoteParam.SetTone( EAvkonSIDNoSound );

    globalNoteParam.SetTextResourceId( 
        CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneInfoBTAccActivated ) );
		
    // Display note
    iViewCommandHandle->ExecuteCommandL( 
        EPhoneViewShowGlobalNote, &globalNoteParam );   
    }

// ---------------------------------------------------------
// CPhoneAccessoryBTHandler::DoSetHandsfreeModeL
// ---------------------------------------------------------
//
TBool CPhoneAccessoryBTHandler::DoSetHandsfreeModeL( 
        TBool aHandsfreeMode, TPEAudioOutput aAudioOutput )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneAccessoryBTHandler::DoSetHandsfreeModeL( ) ");
    __ASSERT_DEBUG( iStateMachine->PhoneEngineInfo(), 
        Panic( EPhoneCtrlInvariant ));
 
    TBool allowed;
    // Only toggle the handsfree mode if there is no wired accessory connected
    if( iStateMachine->PhoneEngineInfo()->AudioOutput() != EPEWiredAudioAccessory )
        {
        TBool showNote = aHandsfreeMode || 
            !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling );

        iStateMachine->PhoneEngineInfo()->SetAudioOutputCommand( 
            aAudioOutput, showNote );
        iStateMachine->SendPhoneEngineMessage( 
            MPEPhoneModel::EPEMessageSetAudioOutput );
        allowed = ETrue; 
        }
    else
        {
        allowed = EFalse;   
        }
    return allowed;
    }

//  End of File  
