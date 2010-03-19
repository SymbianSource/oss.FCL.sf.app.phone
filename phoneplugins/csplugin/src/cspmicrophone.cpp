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
* Description:  Mic stream to DevSound.
*
*/


#include "cspmicrophone.h"
#include "csplogger.h"
#include "mcspdevsoundobserver.h"

#include <AudioPreference.h>

// Mute value
const TInt KSetMuteToDevSound = 0;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Static constructor.
// ---------------------------------------------------------------------------
//
CSPMicrophone* CSPMicrophone::NewL( 
    MCSPDevSoundObserver& aObserver )
    {
    CSPMicrophone* self = new( ELeave ) CSPMicrophone( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSPMicrophone::~CSPMicrophone()
    {
    }

// ---------------------------------------------------------------------------
// Gives mic mute state
// ---------------------------------------------------------------------------
//
TBool CSPMicrophone::IsMuted()
    {
    const TInt gain( iDevSound->Gain() );
    TBool isMuted = EFalse;
    if ( !gain )
        {
        // Mute is on
        isMuted = ETrue;
        }
    CSPLOGSTRING( CSPINT, "CSPMicrophone::IsMuted" );
    return isMuted;
    }
    
// ---------------------------------------------------------------------------
// Set mic muted.
// ---------------------------------------------------------------------------
//
void CSPMicrophone::SetMuted()
    {
    CSPLOGSTRING( CSPINT, "CSPMicrophone::SetMicMuted" );
    iDevSound->SetGain( KSetMuteToDevSound );
    }
    
// ---------------------------------------------------------------------------
// Set mic unmuted
// ---------------------------------------------------------------------------
//
void CSPMicrophone::SetUnmuted()
    {
    CSPLOGSTRING( CSPINT, "CSPMicrophone::SetUnmuted" );
    iDevSound->SetGain( iDevSound->MaxGain() );
    }

// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Activation was successfull. 
// ---------------------------------------------------------------------------
//
void CSPMicrophone::BufferToBeEmptied( CMMFBuffer* /*aBuffer*/ )
    {
    CSPLOGSTRING( CSPINT, "CSPMicrophone::BufferToBeEmptied" );
    
    // We dont react to devsound messages unless we are activating.
    if( IsActivationOngoing() ) 
        {
        iActive = ETrue;
        iActivationOngoing = EFalse;
        iObserver.MicActivatedSuccessfully();
        }
    }

// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Activation feiled
// ---------------------------------------------------------------------------
//    
void CSPMicrophone::RecordError( TInt aError )
    {
    CSPLOGSTRING( CSPINT, "CSPMicrophone::RecordError" );
    
    // We dont react to devsound messages unless we are activating.
    if( IsActivationOngoing() )
        {
        if( aError == KErrAccessDenied ) 
            {
            iActivationOngoing = EFalse;
            iObserver.MicActivationFailed();    
            }
        }   
    }
    
// ---------------------------------------------------------------------------
// From class CSPDevSound
// Tries to activate mic stream. Stream becomes active when BufferToBeFilled
// gets called.
// ---------------------------------------------------------------------------
//
void CSPMicrophone::DoActivateL() 
    {
    iDevSound->RecordInitL();
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPMicrophone::CSPMicrophone(
    MCSPDevSoundObserver& aObserver ) : 
    CSPDevSound( aObserver )
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CSPMicrophone::ConstructL()
    {
    CSPDevSound::ConstructL( 
        EMMFStateRecording, 
        KAudioPrefCSCallUplink, 
        KAudioPriorityCSCallUplink );
    }

//  End of File  
