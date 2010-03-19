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
* Description:  Wrapper for CMMFDevSound
*
*/


#include "cspdevsound.h"
#include "csplogger.h"
#include "mcspdevsoundobserver.h"

#include <AudioPreference.h>
#include <S60FourCC.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSPDevSound
// ---------------------------------------------------------------------------
//
CSPDevSound::CSPDevSound( MCSPDevSoundObserver& aObserver ) : 
    iObserver( aObserver )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSPDevSound::ConstructL( 
    TMMFState aMode, 
    TUint aAudioPreference,
    TUint aAudioPriority )
    {
    CSPLOGSTRING( CSPINT, "CSPDevSound::ConstructL; Entry" );

    TFourCC modemFourCC;
    modemFourCC.Set(KS60FourCCCodeModem);

    iDevSound = CMMFDevSound::NewL();
#ifndef __WINSCW__
    iDevSound->InitializeL( *this, modemFourCC, aMode  );   
#endif
    iPrioritySettings.iPriority = aAudioPriority;
    iPrioritySettings.iPref = 
        static_cast<TMdaPriorityPreference>( aAudioPreference );
    iPrioritySettings.iState = aMode;    
    CSPLOGSTRING( CSPINT, "CSPDevSound::ConstructL; Exit" );
    }
    
// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Not implemented
// ---------------------------------------------------------------------------
//
CSPDevSound::~CSPDevSound()
    {
    delete iDevSound;
    }
    
// ---------------------------------------------------------------------------
// Tries to activate the audio stream if not active or activating
// ---------------------------------------------------------------------------
//
void CSPDevSound::Activate() 
    {
    CSPLOGSTRING( CSPINT, "CSPDevSound::Activate" );
    if( !IsActive() && !IsActivationOngoing() )
        {
        iActivationOngoing = ETrue;
        TInt err = KErrNone; 
        TRAP( err, DoActivateL() );
        CSPLOGSTRING2( CSPINT, "CSPDevSound::Activate %d", err );
        }
    }
    
// ---------------------------------------------------------------------------
// Deactivates the audio device.
// ---------------------------------------------------------------------------
//
void CSPDevSound::Deactivate()
    {
    CSPLOGSTRING( CSPINT, "CSPDevSound::Deactivate" );
    if( IsActive() || IsActivationOngoing() )
        {
        CSPLOGSTRING( CSPINT, "CSPDevSound::Deactivate Stopping" );
        iDevSound->Stop();
        iActive = EFalse;
        iActivationOngoing = EFalse;
        }
    }
    
// ---------------------------------------------------------------------------
// ActivationOngoing
// ---------------------------------------------------------------------------
//
TBool CSPDevSound::IsActivationOngoing() const 
    {
    return iActivationOngoing;
    }
    
// ---------------------------------------------------------------------------
// IsActive
// ---------------------------------------------------------------------------
//
TBool CSPDevSound::IsActive() const
    {
    return iActive;
    }
    
CMMFDevSound& CSPDevSound::DevSound()
    {
    return *iDevSound;
    }

// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Not implemented
// ---------------------------------------------------------------------------
//
void CSPDevSound::InitializeComplete( TInt aError ) 
    {
    CSPLOGSTRING( CSPINT, "CSPDevSound::InitializeComplete" );
    if( aError == KErrNone )
        {
        iDevSound->SetPrioritySettings( iPrioritySettings );   
            
        }
        
    }

// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Not implemented
// ---------------------------------------------------------------------------
//
void CSPDevSound::BufferToBeFilled( CMMFBuffer* /*aBuffer*/ )
    {
    }

// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Not implemented
// ---------------------------------------------------------------------------
//
void CSPDevSound::PlayError( TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Not implemented
// ---------------------------------------------------------------------------
//
void CSPDevSound::ToneFinished( TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Not implemented
// ---------------------------------------------------------------------------
//
void CSPDevSound::BufferToBeEmptied( CMMFBuffer* /*aBuffer*/ )
    {
    }

// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Not implemented
// ---------------------------------------------------------------------------
//
void CSPDevSound::RecordError( TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Not implemented
// ---------------------------------------------------------------------------
//
void CSPDevSound::ConvertError( TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// From class MDevSoundObserver
// Not implemented
// ---------------------------------------------------------------------------
//
void CSPDevSound::DeviceMessage( 
    TUid /*aMessageType*/, 
    const TDesC8& /*aMsg*/ )
    {
    }
 
//  End of File

