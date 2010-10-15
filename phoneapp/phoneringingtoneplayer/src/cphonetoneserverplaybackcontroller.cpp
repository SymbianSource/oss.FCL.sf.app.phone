
/*
* Copyright (c) 2010 - 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Playback controller class for phone ringingtone server.
*
*/
#include <e32base.h>
#include <e32svr.h>
#include <utf.h>
#include <cntdef.h>
#include <apgcli.h>
#include <audiopreference.h>
#include <profile.hrh>

#include "cphonetoneserverplaybackcontroller.h"
#include "cphonetoneserver.h"
#include "cphonetoneserversession.h"
#include "mphonetoneserver.h"
#include "phoneconstants.h"
#include "phoneringingtonetrace.h"

const TInt KMinVolumeLevel = 1;
const TInt KMaxVolumeLevel = 10;

#if defined( __WINS__ )

_LIT( KFileListRngMimeType, "rng" );
_LIT8( KPhoneNoSoundSequence, "\x00\x11\x06\x05\xFC\x0A\x08\x40\x32\x0A\xF7\x40\x64\x06\x0B" );

#else

_LIT( KFileListRngMimeType, "application/vnd.nokia.ringing-tone" );

#endif

_LIT( KDefaultTone, "z:\\data\\sounds\\digital\\Nokia tune.aac" );

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::NewL
// -----------------------------------------------------------------------------
//
CPhoneToneServerPlaybackController* CPhoneToneServerPlaybackController::NewL()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::NewL <");
    CPhoneToneServerPlaybackController* self = 
            new( ELeave )CPhoneToneServerPlaybackController;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::NewL >");
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneToneServerPlaybackController::ConstructL()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::ConstructL <");
    CTimer::ConstructL();
    iRingingType = EProfileRingingTypeSilent;
    iSilentPlayer = CMdaAudioToneUtility::NewL( *this );
    iSilentPlayer->PrepareToPlayDesSequence( KPhoneNoSoundSequence() );
    iDefaultPlayer = CMdaAudioPlayerUtility::NewFilePlayerL( 
        KDefaultTone(),
        *this,
        KAudioPriorityPhoneCall,
        static_cast< TMdaPriorityPreference >( KAudioPrefIncomingCall ) );
    
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::ConstructL >");
    }
// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::CPhoneToneServerPlaybackController
// -----------------------------------------------------------------------------
//
CPhoneToneServerPlaybackController::CPhoneToneServerPlaybackController():
        CTimer(EPriorityHigh),
        iRingingType( EProfileRingingTypeSilent )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::CPhoneToneServerPlaybackController <");
    CActiveScheduler::Add(this);
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::CPhoneToneServerPlaybackController <");
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::~CPhoneToneServerPlaybackController
// -----------------------------------------------------------------------------
//
CPhoneToneServerPlaybackController::~CPhoneToneServerPlaybackController()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::~CPhoneToneServerPlaybackController <");
    delete iSilentPlayer;
    delete iDefaultPlayer;
    delete iProfileSamplePlayer;
    delete iProfileTonePlayer;
    Cancel();
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::~CPhoneToneServerPlaybackController <");
    }
// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::RunL
// -----------------------------------------------------------------------------
//
void CPhoneToneServerPlaybackController::RunL()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::RunL <");
    StopAndDeleteProfilePlayer();
    PlayDefaultTone();
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::RunL >");
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::ConvertVolume
// -----------------------------------------------------------------------------
//
TInt CPhoneToneServerPlaybackController::ConvertVolume( TInt aMaxVolume, TInt aVolume )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::ConvertVolume <");
    TInt result = aMaxVolume * aVolume / KMaxVolumeLevel;
    if ( aVolume == KMinVolumeLevel && result == 0 )
        {
        result = 1;
        }
    PHONERINGINGTONE_PRINTF("CPhoneToneServerPlaybackController::ConvertVolume - result(%d)", result);
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::ConvertVolume >");
    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::PlayProfileTone
// -----------------------------------------------------------------------------
//
void CPhoneToneServerPlaybackController::PlayProfileTone()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::PlayProfileTone <");
    if ( iProfileSamplePlayer )
        {
        iProfileSamplePlayer->SetVolume( 
            ConvertVolume( iProfileSamplePlayer->MaxVolume(), iVolume ) );
        iProfileSamplePlayer->SetRepeats(KPhoneMdaAudioToneRepeatForever,
            TTimeIntervalMicroSeconds( KPhoneRingingRepeatsTrailPause ));
        iProfileSamplePlayer->Play();
        }
    else if ( iProfileTonePlayer )
        {
        iProfileTonePlayer->SetVolume( 
        ConvertVolume( iProfileTonePlayer->MaxVolume(), iVolume ) );    
        iProfileTonePlayer->SetRepeats(KPhoneMdaAudioToneRepeatForever,
            TTimeIntervalMicroSeconds( KPhoneRingingRepeatsTrailPause ));
        iProfileTonePlayer->Play();
        }
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::PlayProfileTone >");
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::StopAndDeleteProfilePlayer
// -----------------------------------------------------------------------------
//
void CPhoneToneServerPlaybackController::StopAndDeleteProfilePlayer()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::DeleteProfilePlayer() <");
    if ( iProfileSamplePlayer )
        {
        iProfileSamplePlayer->Stop();
        iProfileSamplePlayer->Close();
        delete iProfileSamplePlayer;
        iProfileSamplePlayer = NULL;
        }
    else if ( iProfileTonePlayer )
        {
        if ( EMdaAudioToneUtilityPlaying == iProfileTonePlayer->State() )
            {
            iProfileTonePlayer->CancelPlay();
            }
        else if (EMdaAudioToneUtilityNotReady == iProfileTonePlayer->State() )
            {
            iProfileTonePlayer->CancelPrepare();
            } 
        delete iProfileTonePlayer;
        iProfileTonePlayer = NULL;
        }
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::DeleteProfilePlayer() >");
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::IsRngL
// -----------------------------------------------------------------------------
//
TBool CPhoneToneServerPlaybackController::IsRngL( const TDesC& aFile )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::IsRng <");
#if defined( __WINS__ )

    TInt index = aFile.Find( KFileListRngMimeType() );

    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::IsRng >");
    return ( KErrNotFound != index );

#else

    RApaLsSession apaLsSession;
    User::LeaveIfError( apaLsSession.Connect() );
    CleanupClosePushL( apaLsSession );
    TUid dummyUid = { 0 };
    TDataType dataType( dummyUid );
    User::LeaveIfError(
        apaLsSession.AppForDocument( aFile, dummyUid, dataType ) );
    CleanupStack::PopAndDestroy( &apaLsSession );
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::IsRng >");
    return ( dataType.Des().CompareF( KFileListRngMimeType ) == 0 );

#endif
    }
    

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::MapcInitComplete
// -----------------------------------------------------------------------------
//
void CPhoneToneServerPlaybackController::MapcInitComplete( TInt aError, 
        const TTimeIntervalMicroSeconds& /*aDuration*/ )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::MapcInitComplete <");
    PHONERINGINGTONE_PRINTF("CPhoneToneServerPlaybackController::MapcInitComplete - aError(%d)",aError);
    if ( !iDefaultPlayerInitialized && !aError )
        {
        // This player will be initialized first.
        iDefaultPlayerInitialized = ETrue;
        iDefaultPlayer->SetRepeats(KPhoneMdaAudioToneRepeatForever,
                TTimeIntervalMicroSeconds( KPhoneRingingRepeatsTrailPause ));
        return;
        }
    // Player is initialized at startup 
    if ( EProfileRingingTypeSilent == iRingingType )
        {
        return;
        }
    
    // Cancel backup player timer.
    Cancel();
   
    if ( !aError )
        {
        PlayProfileTone();
        }
    else if ( iDefaultPlayerInitialized )
        {
        PlayDefaultTone();
        }
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::MapcInitComplete >");
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::MatoPrepareComplete
// -----------------------------------------------------------------------------
//
void CPhoneToneServerPlaybackController::MatoPrepareComplete( TInt aError )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::MatoPrepareComplete <");
    PHONERINGINGTONE_PRINTF("CPhoneToneServerPlaybackController::MatoPrepareComplete - aError(%d)",aError);
    
    // Player is initialized at startup 
    if ( EProfileRingingTypeSilent == iRingingType  )
        {
        return;
        }
    
    // Cancel backup player timer.
    Cancel();
    
    if ( !aError )
        {
        PlayProfileTone();
        }
    else if ( iDefaultPlayerInitialized )
        {
        PlayDefaultTone();
        }
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::MatoPrepareComplete >");
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::MapcPlayComplete
// -----------------------------------------------------------------------------
//
void CPhoneToneServerPlaybackController::MapcPlayComplete( TInt aError )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::MapcPlayComplete <");
    PHONERINGINGTONE_PRINTF("CPhoneToneServerPlaybackController::MapcPlayComplete - aError(%d)",aError);
#if defined (__WINS__)
    
    // S^4 onwards MdaAudioPlayer returns -5 (Not Supported)
    // for audio files in emulator. 
    // Error code is ignored to avoid unwanted situation.
    if ( aError == KErrNotSupported )
        {
        aError = KErrNone;
        }
#endif     
    
    if ( aError && iDefaultPlayerInitialized )
        {
        PlayDefaultTone();
        }
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::MapcPlayComplete >");
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::MatoPlayComplete
// -----------------------------------------------------------------------------
//
void CPhoneToneServerPlaybackController::MatoPlayComplete( TInt aError )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::MatoPlayComplete <");
    PHONERINGINGTONE_PRINTF("CPhoneToneServerPlaybackController::MatoPlayComplete - aError(%d)",aError);
    if ( aError && iDefaultPlayerInitialized )
        {
        PlayDefaultTone();
        }
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::MatoPlayComplete >");
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::PlaySilentTone
// -----------------------------------------------------------------------------

void CPhoneToneServerPlaybackController::PlaySilentTone()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::PlaySilentTone <");
    iSilentPlayer->Play();
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::PlaySilentTone >");
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerPlaybackController::PlayNormalTone
// -----------------------------------------------------------------------------
//
void CPhoneToneServerPlaybackController::PlayNormalToneL( 
        const TFileName& aName, 
        TInt aVolume, 
        TProfileRingingType aType )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::PlayNormalTone <");
    PHONERINGINGTONE_PRINTF3("CPhoneToneServerPlaybackController::PlayNormalTone - aName(%S), aVolume(%d), aType(%d)",&aName, aVolume, aType );
    
    iVolume = aVolume;
    iRingingType = aType;
    
    StopAndDeleteProfilePlayer();
    Cancel();
    After( KPhoneMaxRingingWaiting );
    
    if ( !aName.Length() )
        {
        PlayDefaultTone();
        }
    // RNG file types have to be played with CMdaAudioToneUtility player.
    // Otherwise use sample player
    else if ( IsRngL( aName ) )
        {               
        PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::PlayNormalTone - toneplayer");
        iProfileTonePlayer = CMdaAudioToneUtility::NewL( *this );
        iProfileTonePlayer->PrepareToPlayFileSequence( aName );
        }
    else
        {
        PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::PlayNormalTone - sampleplayer");
        iProfileSamplePlayer = CMdaAudioPlayerUtility::NewFilePlayerL( 
            aName,  
            *this, 
            KAudioPriorityPhoneCall,
            static_cast< TMdaPriorityPreference >( KAudioPrefIncomingCall ));
        }
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::PlayNormalTone >");    
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerSession::PlayDefaultTone
// -----------------------------------------------------------------------------
//
void CPhoneToneServerPlaybackController::PlayDefaultTone()
    {
    Cancel();
    iDefaultPlayer->SetVolume( 
            ConvertVolume( iDefaultPlayer->MaxVolume(), iVolume ) );
    iDefaultPlayer->Play();
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerSession::StopTone
// -----------------------------------------------------------------------------
//
void CPhoneToneServerPlaybackController::StopTone()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::StopTone() <");

    Cancel();
    StopAndDeleteProfilePlayer();
    
    iDefaultPlayer->Stop();

    iSilentPlayer->CancelPlay();
    PHONERINGINGTONE_PRINT("CPhoneToneServerPlaybackController::StopTone() >");
    }

// End of file
