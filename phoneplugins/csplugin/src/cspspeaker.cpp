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
* Description:  Provides speaker functionality
*
*/


#include <AudioPreference.h>

#include "cspspeaker.h"
#include "csplogger.h"
#include "mcspdevsoundobserver.h"

//Audio levels
//Max volume level
const TInt KMaxVolumeLevel = 10;

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CSPSpeaker* CSPSpeaker::NewL(
    MCSPDevSoundObserver& aObserver )
    {
    CSPSpeaker* self = new( ELeave ) CSPSpeaker( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CSPSpeaker::~CSPSpeaker()
    {
    }
   
// ---------------------------------------------------------------------------
// Sets volume
// Volume needs to be converted from 0-10 scale to the volume scale used by 
// DevSound 0 - MaxVolume().
// No need to check aVolume value, because devsound takes care
// than illegal value are converted to legal, ie <0 are 0 and >MaxVolume
// are set to MaxVolume.
// ---------------------------------------------------------------------------
//    
void CSPSpeaker::SetVolume( TInt aVolume )
    {
    TInt vol( 0 );    
    vol = iDevSound->MaxVolume() * aVolume / KMaxVolumeLevel;
    iDevSound->SetVolume( vol );
    CSPLOGSTRING2(CSPINT, "CSPSpeaker::SetVolume() \
                vol=%d", vol ); 
    CSPLOGSTRING2(CSPINT, "CSPSpeaker::SetVolume() \
                maxVol=%d", iDevSound->MaxVolume() );
    }

// ---------------------------------------------------------------------------
// Gives volume
// ---------------------------------------------------------------------------
//    
TInt CSPSpeaker::Volume()
    {
    TInt devVol = iDevSound->Volume();
    CSPLOGSTRING2(CSPINT, "CSPSpeaker::Volume \
                    vol from DevSound =%d", devVol );
    TInt vol = devVol * KMaxVolumeLevel / iDevSound->MaxVolume();
    
    CSPLOGSTRING2(CSPINT, "CSPSpeaker::Volume() \
                       vol=%d", vol );
       
    return vol;
    }

// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Speaker stream has been activated successfully.
// ---------------------------------------------------------------------------
//    
void CSPSpeaker::BufferToBeFilled( CMMFBuffer* /*aBuffer*/ )
    {
    CSPLOGSTRING( CSPINT, "CSPSpeaker::BufferToBeFilled activated" );
    // We dont react to devsound messages unless we are activating.
    if( IsActivationOngoing() )
        {
        iActive = ETrue;
        iActivationOngoing = EFalse;
        iObserver.SpeakerActivatedSuccessfully();
        }
    }
    
// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Speaker stream activation failed
// ---------------------------------------------------------------------------
//    
void CSPSpeaker::PlayError( TInt aError )   
    {
    CSPLOGSTRING( CSPINT, "CSPSpeaker::PlayError" );
    
    // We dont react to devsound messages unless we are activating.
    if( IsActivationOngoing() )
        {
        CSPLOGSTRING( CSPINT, "CSPSpeaker::PlayError activation failed" );
        if( aError == KErrAccessDenied ) 
            {
            iActivationOngoing = EFalse;
            iObserver.SpeakerActivationFailed();
            }
        }
    }

// ---------------------------------------------------------------------------
// From class CSPDevsound
// Tries to activate speaker stream.
// ---------------------------------------------------------------------------
//
void CSPSpeaker::DoActivateL()
    {
    iDevSound->PlayInitL();
    }  
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPSpeaker::CSPSpeaker( 
    MCSPDevSoundObserver& aObserver ) :
    CSPDevSound( aObserver )
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CSPSpeaker::ConstructL()
    {
    CSPDevSound::ConstructL( 
        EMMFStatePlaying, 
        KAudioPrefCSCallDownlink,
        KAudioPriorityCSCallDownlink );
    }

//  End of File  
