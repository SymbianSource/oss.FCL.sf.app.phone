/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implements the class CSPAudioHandler
*
*/


#include "cspaudiohandler.h"

#include <telmicmutestatuspskeys.h>
#include <telincallvolcntrlcrkeys.h>
#include <PSVariables.h>
#include <AudioPreference.h>

#include "cspaudiostreams.h"
#include "csppubsublistener.h"
#include "cspcenreplistener.h"
#include "csplogger.h"


// ---------------------------------------------------------------------------
// CSPAudioHandler::NewL.
// ---------------------------------------------------------------------------
//
CSPAudioHandler* CSPAudioHandler::NewL(  )
    {
    CSPLOGSTRING(CSPOBJECT, 
        "CSPAudioHandler::NewL()" );
    CSPAudioHandler* self = new ( ELeave ) CSPAudioHandler(  );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;    
    }

// ---------------------------------------------------------------------------
// Destructs the object by canceling first ongoing monitoring.
// ---------------------------------------------------------------------------
//
CSPAudioHandler::~CSPAudioHandler( )
    {
    CSPLOGSTRING(CSPOBJECT, 
        "CSPAudioHandler::~CSPAudioHandler()" );
    delete iMuteListener;
    delete iIncallLoudspeakerVolumeListener;
    delete iIncallEarVolumeListener;
    delete iAudioStreams;
    }

// ---------------------------------------------------------------------------
// CSPAudioHandler::Start
// ---------------------------------------------------------------------------
//
void CSPAudioHandler::Start()
    {
    iCallCount++;
    CSPLOGSTRING2( CSPINT, "CSPAudioHandler::Start callcount: %d", iCallCount );

    if ( iCallCount == 1 )
        {
        iAudioStreams->StartStreams();
        }
    }

// ---------------------------------------------------------------------------
// CSPAudioHandler::Stop
// ---------------------------------------------------------------------------
//
void CSPAudioHandler::Stop()
    {
    CSPLOGSTRING2( CSPINT, "CSPAudioHandler::Stop callcount: %d", iCallCount );
    if ( iCallCount == 1 )
        {
        iAudioStreams->StopStreams();
        iCallCount--;
        }
    else if ( iCallCount > 1 )
        {
        iCallCount--;
        }
    }

// ---------------------------------------------------------------------------
// From MCSPPubSubObserver
// CSPAudioHandler::HandleNotifyPSL
// ---------------------------------------------------------------------------
//
void CSPAudioHandler::HandleNotifyPSL( const TUid /*aUid*/, const TInt& /*aKey*/, 
            const TRequestStatus& /*aStatus*/ )
    {
    TInt muteVal;
    TInt err = iMuteListener->Get( muteVal );
    if (  err == KErrNone && muteVal == EPSTelMicMuteOn )
        {
        CSPLOGSTRING2( CSPINT, 
            "CSPAudioHandler::HandleNotifyPSL_MUTE %d", muteVal);   
        #if !defined(__WINSCW__) 
        iAudioStreams->SetMuted();
        #endif //__WINSCW__
        }
    else if ( err == KErrNone )
        {
        CSPLOGSTRING2( CSPINT, 
            "CSPAudioHandler::HandleNotifyPSL_UNMUTE %d", muteVal);
        #if !defined(__WINSCW__) 
        // Change when gain is really changed
        iAudioStreams->SetUnmuted();
        #endif //__WINSCW__
        }
    }
    
// ---------------------------------------------------------------------------
// From MCSPCenRepObserver
// CSPAudioHandler::HandleNotifyCenRepL
// ---------------------------------------------------------------------------
//
void CSPAudioHandler::HandleNotifyCenRepL( const TUid /*aUid*/, 
                        const TUint32 aKey, 
                        TInt aVal )
    {
    if ( aKey == KTelIncallLoudspeakerVolume )
        {
        CSPLOGSTRING( CSPINT, "CSPAudioHandler::HandleNotifyCenRepL IncallLoudspeaker" );
        iAudioStreams->VolumeChangedLoudspeaker( aVal );
        }
    else if ( aKey == KTelIncallEarVolume )
        {
        CSPLOGSTRING( CSPINT, "CSPAudioHandler::HandleNotifyCenRepL IncallEar" );
        iAudioStreams->VolumeChangedEar( aVal );
        }
    else
        {
        CSPLOGSTRING( CSPINT, "CSPAudioHandler::HandleNotifyCenRepL  UNKNOWN" );
        }   
    }

// ---------------------------------------------------------
// CSPAudioHandler::InitializeComplete
//
// Handles DevSound initialisation callback
// ---------------------------------------------------------
//
void CSPAudioHandler::InitializeComplete( TInt aError )
    {
    if ( KErrNone != aError )
        {
        // If initialisation failed, volume can not be adjusted
        CSPLOGSTRING2( CSPERROR, 
           "CSPAudioHandler::InitializeComplete Error:%d", aError );
        }
    else
        {
        CSPLOGSTRING( CSPINT, 
           "CSPAudioHandler::InitializeComplete");        
        }
    }
    
// ---------------------------------------------------------
// CSPAudioHandler::BufferToBeFilled
// ---------------------------------------------------------
//
void CSPAudioHandler::BufferToBeFilled( CMMFBuffer* /*aBuffer*/ )
    {
    }
    
// ---------------------------------------------------------
// CSPAudioHandler::BufferToBeEmptied()
// Not supported
// ---------------------------------------------------------
//
void CSPAudioHandler::BufferToBeEmptied( CMMFBuffer* /*aBuffer*/ )
    {
    }

// ---------------------------------------------------------
// CSPAudioHandler::PlayError
// Not supported
// ---------------------------------------------------------
//    
void CSPAudioHandler::PlayError( TInt /*aError*/ )
    {       
    }
    
// ---------------------------------------------------------
// CSPAudioHandler::RecordError
// Not supported
// ---------------------------------------------------------
//
void CSPAudioHandler::RecordError( TInt /*aError*/ )
    {
    }
    
// ---------------------------------------------------------
// CSPAudioHandler::ConvertError
// Not supported
// ---------------------------------------------------------
// 
void CSPAudioHandler::ConvertError( TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------
// CSPAudioHandler::DeviceMessage
// Not supported
// ---------------------------------------------------------
//
void CSPAudioHandler::DeviceMessage( TUid /*aMessageType*/, const TDesC8& /*aMsg*/ )
    { 
    }
    
// ---------------------------------------------------------
// CSPAudioHandler::ToneFinished
// Not supported
// ---------------------------------------------------------
//   
void CSPAudioHandler::ToneFinished( TInt /*aError*/ ) 
    {
    }

// ---------------------------------------------------------------------------
// Constructs the monitor.
// ---------------------------------------------------------------------------
//
CSPAudioHandler::CSPAudioHandler( )
    {
    CSPLOGSTRING(CSPOBJECT, 
        "CSPAudioHandler::CSPAudioHandler()" );
    iCallCount = 0; // Active calls count
    }
    
// ---------------------------------------------------------------------------
// Second phase construction.
// ---------------------------------------------------------------------------
//
void CSPAudioHandler::ConstructL()
    {
    RProperty::TType type( RProperty::EInt );
    TSecurityPolicy readPolicy( ECapability_None );
    TSecurityPolicy writePolicy( ECapabilityWriteDeviceData );
    
    RProperty::Define( KPSUidTelMicrophoneMuteStatus, 
        KTelMicrophoneMuteState,
        type,
        readPolicy,
        writePolicy );
    
    iMuteListener = CSPPubSubListener::NewL( 
        KPSUidTelMicrophoneMuteStatus, 
        KTelMicrophoneMuteState, 
        this );
        
    iIncallLoudspeakerVolumeListener = CSPCenRepListener::NewL( KCRUidInCallVolume,
                         KTelIncallLoudspeakerVolume, this );
    
    iIncallEarVolumeListener = CSPCenRepListener::NewL( KCRUidInCallVolume,
                        KTelIncallEarVolume, this );
    
    iAudioStreams = CSPAudioStreams::NewL();

    // Initialize audio volumes
    TInt volEar;
    TInt volLoud;
    
    TInt volGetRes = iIncallEarVolumeListener->Get( volEar );
    CSPLOGSTRING3(CSPINT, "CSPAudioHandler::ConstructL() ear     %d %d", volGetRes, volEar );
    volGetRes = iIncallLoudspeakerVolumeListener->Get( volLoud );
    CSPLOGSTRING3(CSPINT, "CSPAudioHandler::ConstructL() loudspkr %d %d", volGetRes, volLoud );    
    
    iAudioStreams->StoreVolumes( volEar, volLoud );
    }

// End of file
