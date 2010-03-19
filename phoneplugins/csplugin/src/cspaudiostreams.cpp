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
* Description:  Starts and stops audio streams.
*
*/


#include "cspaudiostreams.h"
#include "cspmicrophone.h"
#include "cspspeaker.h"
#include "csplogger.h"
#include "mcspaudiostreamobserver.h"
#include "csptimer.h"

#include <TelephonyAudioRouting.h>

/**
* Timeout initial value.
*/
const TInt KTimeoutInitial = 200000; // 0.2s

/**
* Double the timeout for every retry.
*/
const TInt KTimeoutMultiplier = 2;

// ======== MEMBER FUNCTIONS ========
   
// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CSPAudioStreams* CSPAudioStreams::NewL( )
    {
    CSPAudioStreams* self = 
        CSPAudioStreams::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CSPAudioStreams* CSPAudioStreams::NewLC(  )
    {
    CSPAudioStreams* self = new( ELeave ) CSPAudioStreams( );
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSPAudioStreams::~CSPAudioStreams()
    {
    if (iTimer)
        {
        iTimer->CancelNotify();    
        delete iTimer;
        }
    
    delete iSpeaker;
    delete iMic;
    delete iAudioRouting;
    }

// ---------------------------------------------------------------------------
// Sets observer for audio stream.
// ---------------------------------------------------------------------------
//
void CSPAudioStreams::SetAudioStreamObserver( 
    MCSPAudioStreamObserver& aObserver )
    {
    iStreamObserver = &aObserver;
    }
    
// ---------------------------------------------------------------------------
// Gives access to mic
// ---------------------------------------------------------------------------
//
CSPMicrophone& CSPAudioStreams::Mic() const
    {
    return *iMic;
    }

// ---------------------------------------------------------------------------
// Gives access to speaker
// ---------------------------------------------------------------------------
//
CSPSpeaker& CSPAudioStreams::Speaker() const
    {
    return *iSpeaker;
    }
    
// ---------------------------------------------------------------------------
// From class MCSPAudioStream
// Activates mic and speaker.
// ---------------------------------------------------------------------------
//
void CSPAudioStreams::StartStreams() 
    {
    CSPLOGSTRING(CSPINT, "CSPAudioStreams::StartStreams");
    
    if( !IsMicAndSpeakerStarted() )    
        {
        StartMicAndSpeaker(); 
        
        if( iStreamObserver )
            {
            iStreamObserver->AudioStreamsStarted();
            }
        }
    }

// ---------------------------------------------------------------------------
// From class MCSPAudioStream
// Deactivates mic and speaker if the streams are active or they are 
// activating. 
// ---------------------------------------------------------------------------
//
void CSPAudioStreams::StopStreams() 
    {
    CSPLOGSTRING(CSPINT, "CSPAudioStreams::StopStreams");
    
    if( IsMicAndSpeakerStarted() )
        {
            CSPLOGSTRING(CSPINT, "CSPAudioStreams::StopStreams Stopping");
        iTimer->CancelNotify(); 
        iTimeout = KTimeoutInitial;
        if( iStreamObserver ) 
            {
            iStreamObserver->AudioStreamsGoingToStop();
            }
        iMic->Deactivate();
        iSpeaker->Deactivate();
        }
    }

// ---------------------------------------------------------------------------
// CSPAudioStreams::VolumeChangedEar
// ---------------------------------------------------------------------------
//
void CSPAudioStreams::VolumeChangedEar(TInt aVolume)
    {
    CSPLOGSTRING2(CSPINT, "CSPAudioStreams::SetVolume %d", aVolume);
    iVolumeEar = aVolume;
    iSpeaker->SetVolume( aVolume );
    }

// ---------------------------------------------------------------------------
// CSPAudioStreams::VolumeChangedLoudspeaker
// ---------------------------------------------------------------------------
//
void CSPAudioStreams::VolumeChangedLoudspeaker(TInt aVolume)
    {
    CSPLOGSTRING2(CSPINT, "CSPAudioStreams::SetVolume %d", aVolume);
    iVolumeLoudspeaker = aVolume;
    iSpeaker->SetVolume( aVolume );
    }

// ---------------------------------------------------------------------------
// CSPAudioStreams::StoreVolumes
// ---------------------------------------------------------------------------
//
void CSPAudioStreams::StoreVolumes(TInt aVolumeEar, TInt aVolumeLoudspeaker )
    {
    iVolumeEar = aVolumeEar;
    iVolumeLoudspeaker = aVolumeLoudspeaker;
    }
    
// ---------------------------------------------------------------------------
// CSPAudioStreams::ApplyVolume
// ---------------------------------------------------------------------------
//
void CSPAudioStreams::ApplyVolume(TInt aVolumeEar, TInt aVolumeLoudspeaker )
    {
    CTelephonyAudioRouting::TAudioOutput current = iAudioRouting->Output();
    if ( current == CTelephonyAudioRouting::ELoudspeaker )
        {
        CSPLOGSTRING( CSPINT, "CSPAudioStreams::ApplyVolume Loudspeaker Active: SetVolume" );
        iSpeaker->SetVolume( aVolumeLoudspeaker );
        }
        // else: ear volume should be used
     else if ( current == CTelephonyAudioRouting::EHandset )
        {
        CSPLOGSTRING( CSPINT, "CSPAudioStreams::ApplyVolume Ear Active: SetVolume" );
        iSpeaker->SetVolume( aVolumeEar );
        }
    else
        {
        CSPLOGSTRING2( CSPINT, "CSPAudioStreams::ApplyVolume UNKNOWN AUDIO OUTPUT MODE %d", current );
        // No volume setting
        iSpeaker->SetVolume( aVolumeEar );
        }
    }

// ---------------------------------------------------------------------------
// CSPAudioStreams::SetMuted
// ---------------------------------------------------------------------------
//  
void CSPAudioStreams::SetMuted() 
    {
    iMic->SetMuted();
    }

// ---------------------------------------------------------------------------
// CSPAudioStreams::SetUnmuted
// ---------------------------------------------------------------------------
//  
void CSPAudioStreams::SetUnmuted() 
    {
    iMic->SetUnmuted();
    }

// ---------------------------------------------------------------------------
// From class MCSPAudioStreamObserver
// If speaker is already active then the streams are active.
// If speker is not active and it is not activating then speaker then
// activation has failed and start retry timer.
// ---------------------------------------------------------------------------
//   
void CSPAudioStreams::MicActivatedSuccessfully()
    {
    CSPLOGSTRING(CSPINT, "CSPAudioStreams::MicActivatedSuccessfully" );
    if( iSpeaker->IsActive() )
        {
        // Mic and speaker are active.
        AudioStreamsStarted();
        }
    else if( !iSpeaker->IsActivationOngoing() ) 
        {
        // Start retry timer for activating speaker again
        StartTimer();
        }
    }
    
// ---------------------------------------------------------------------------
// From class MCSPAudioStreamObserver
// If mic is already active then streams are active.
// If mic is not active and it is not activating then mic activation has
// failed and start retry timer.
// ---------------------------------------------------------------------------
//   
void CSPAudioStreams::SpeakerActivatedSuccessfully()
    {
    CSPLOGSTRING(CSPINT, "CSPAudioStreams::SpeakerActivatedSuccessfully" );
    if( iMic->IsActive() ) 
        {
        // Mic and speaker are active.
        AudioStreamsStarted();
        }
    else if( !iMic->IsActivationOngoing() ) 
        {
        // Start retry timer for activating mic again.
        StartTimer();
        }
    }

// ---------------------------------------------------------------------------
// From class MCSPAudioStreamObserver
// Starts timer for trying activation again.
// ---------------------------------------------------------------------------
//    
void CSPAudioStreams::MicActivationFailed()
    {
    CSPLOGSTRING(CSPINT, "CSPAudioStreams::MicActivationFailed" );
   
    // Dont start timer until speaker has stopped activation.
    if( !iSpeaker->IsActivationOngoing() ) 
        {
        StartTimer();
        }
    }
    
// ---------------------------------------------------------------------------
// From class MCSPAudioStreamObserver
// Starts timer for trying activation again.
// ---------------------------------------------------------------------------
// 
void CSPAudioStreams::SpeakerActivationFailed()
    {
    CSPLOGSTRING(CSPINT, "PE.AudioStreams::SpeakerActivationFailed" );
    
    // Dont start timer until mic has stopped activation.
    if( !iMic->IsActivationOngoing() )
        {
        StartTimer();
        }
    }
    
// ---------------------------------------------------------------------------
// From class MCSPTimerObserver
// Notify from CSPTimer that timeout passed. Try to start mic and
// speaker again.
// ---------------------------------------------------------------------------
// 
void CSPAudioStreams::TimerEvent()
    {
    CSPLOGSTRING(CSPINT, "CSPAudioStreams.TimerEvent" );
    iTimeout *= KTimeoutMultiplier;
    StartMicAndSpeaker();
    }

// -----------------------------------------------------------------------------
// CTSEAudioRouteObserver::AvailableOutputsChanged
// -----------------------------------------------------------------------------
//
void CSPAudioStreams::AvailableOutputsChanged( 
        CTelephonyAudioRouting& /*aTelephonyAudioRouting*/ )
    {
    CSPLOGSTRING(CSPINT, "CSPAudioStreams::AvailableOutputsChanged" );
    }
    
// -----------------------------------------------------------------------------
// CTSEAudioRouteObserver::OutputChanged
// -----------------------------------------------------------------------------
//
void CSPAudioStreams::OutputChanged( 
        CTelephonyAudioRouting& /*aTelephonyAudioRouting*/ )
    {
    CSPLOGSTRING(CSPINT, "CSPAudioStreams::OutputChanged" );
    ApplyVolume(iVolumeEar, iVolumeLoudspeaker);
    }

// -----------------------------------------------------------------------------
// CSPAudioStreams::SetOutputComplete
// -----------------------------------------------------------------------------
//
void CSPAudioStreams::SetOutputComplete( CTelephonyAudioRouting& /*aTelephonyAudioRouting*/,
                                         TInt /*aError*/ )
   {
   }  

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPAudioStreams::CSPAudioStreams( ): iTimeout(KTimeoutInitial)
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CSPAudioStreams::ConstructL( )
    {
    CSPLOGSTRING(CSPINT, "CSPAudioStreams::ConstructL");

    iTimer = CSPTimer::NewL();
    iMic = CSPMicrophone::NewL( *this );
    iSpeaker = CSPSpeaker::NewL( *this );
    
    #if !defined(__WINSCW__)
    iAudioRouting = CTelephonyAudioRouting::NewL( *this );
    #endif //__WINSCW__        
    }
    
// ---------------------------------------------------------------------------
// Resets timer
// ---------------------------------------------------------------------------
//
void CSPAudioStreams::AudioStreamsStarted()
    {
    CSPLOGSTRING(CSPINT, "CSPAudioStreams::AudioStreamsStarted" );
    iTimeout = KTimeoutInitial; 
    iTimer->CancelNotify();
    }

// ---------------------------------------------------------------------------
// Starts timer
// ---------------------------------------------------------------------------
//
void CSPAudioStreams::StartTimer() 
    {
    CSPLOGSTRING(CSPINT, "CSPAudioStreams::StartTimer" );
    iTimer->NotifyAfter( iTimeout, *this );
    }
    
// ---------------------------------------------------------------------------
// Starts mic and speaker
// ---------------------------------------------------------------------------
//
void CSPAudioStreams::StartMicAndSpeaker()
    {
    // if speaker and mic is active then activation does not
    // cause any actions.
    iSpeaker->Activate();
    iMic->Activate();   
    }

// ---------------------------------------------------------------------------
// Indicated if mic and speaker are started or starting up.
// ---------------------------------------------------------------------------
//    
TBool CSPAudioStreams::IsMicAndSpeakerStarted()
    {
    TBool areStreamsActive( iSpeaker->IsActive() && iMic->IsActive() );
    TBool areStreamsActivating( iMic->IsActivationOngoing() ||  
        iSpeaker->IsActivationOngoing() || iTimer->IsNotifyOngoing() );
        
    return areStreamsActive || areStreamsActivating;
    }

//  End of File  
