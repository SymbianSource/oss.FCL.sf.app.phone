/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Audio player implementation.
*
*/


// INCLUDE FILES
#include <e32std.h>
#include <eikenv.h>
#include <apgcli.h>
#include <apmrec.h>
#include <AudioPreference.h>
#include <utf.h>    // For converting data for TTS
#include <c3dringingtoneinterface.h>
#include <AudioOutput.h>

#include "cphoneaudioplayer.h"
#include "cphoneringingtone.h"
#include "mphoneaudioplayerobserver.h"
#include "phoneconstants.h"
#include "phoneuiviewtrace.h"
#include "phoneui.pan"


// CONSTANTS


_LIT( KFileListRngMimeType, "application/vnd.nokia.ringing-tone" );
//Min volume level
const TInt KMinVolumeLevel = 1;
//Max volume level
const TInt KMaxVolumeLevel = 10;
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::CPhoneAudioPlayer
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer::CPhoneAudioPlayer( TAudioDataFormat aDataFormat,
    MPhoneAudioPlayerObserver& aObserver, 
    TInt aPriority, 
    TUint aPreference,
    TInt aId )
    : iFormat( aDataFormat ),
    iObserver( aObserver ),
    iPriority( aPriority ),
    iPreference( aPreference ),
    iId( aId )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::ConstructL(
    const TDesC& aFileName,
    CMdaServer* aMdaServer )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::ConstructL() <" );
    if ( iFormat == EFormatTone )
        {
        iTonePlayer = CMdaAudioToneUtility::NewL( *this, aMdaServer );
        iTonePlayer->PrepareToPlayFileSequence( aFileName );
        iPlayerStatus = EToneLoading;
        }
    else
        {
        //audio sample player
        PHONEUIVIEW_PRINTF3(
            "CPhoneAudioPlayer::ConstructL - aFileName(%S), iPriority(%d), iPreference(%d)",
            &aFileName, 
            iPriority, 
            iPreference );

        iSamplePlayer = CMdaAudioPlayerUtility::NewFilePlayerL( 
            aFileName,
            *this, 
            iPriority,
            static_cast< TMdaPriorityPreference >( iPreference ),
            aMdaServer );
        iPlayerStatus = EToneLoading;

        }
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::ConstructL() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::ConstructSeqL
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::ConstructSeqL( 
    const TDesC8& aSequence,
        CMdaServer* aMdaServer )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::ConstructSeqL() <" );
    iTonePlayer = CMdaAudioToneUtility::NewL( *this, aMdaServer );
    iSequence = aSequence.AllocL();
    iTonePlayer->PrepareToPlayDesSequence( *iSequence );
    iPlayerStatus = EToneLoading;
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::ConstructSeqL() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::ConstructTtsL
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::ConstructTtsL( 
    const TDesC& aTtsText, 
    TInt aPriority,
    TUint aPreference )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::ConstructTtsL() <" );
    delete iTtsPlayer; // if any
    iTtsPlayer = NULL;
    iTtsPlayer = 
        CMdaAudioPlayerUtility::NewL( 
            *this, aPriority, 
            static_cast< TMdaPriorityPreference >( aPreference ) );

    delete iTtsText;  // if any
    iTtsText = NULL;
    // UTF-8 strings can take up to 4 bytes per character
    iTtsText = HBufC8::NewL( aTtsText.Length() << KTimesToMultiply ); 

    TPtr8 refText = iTtsText->Des(); 
    User::LeaveIfError( 
        CnvUtfConverter::ConvertFromUnicodeToUtf8( refText, aTtsText ) );

    // UTF-8 chars can be up to 4 bytes long, but usually 
    // take 1-2 bytes, 3 for asian chars.
    HBufC8* oldText = iTtsText;
    iTtsText = iTtsText->ReAlloc( iTtsText->Length() );
    if ( !iTtsText )
        {
        // ReAlloc failed, set back to original.
        iTtsText = oldText;
        }

    iTtsPlayer->OpenDesL( *iTtsText );
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::ConstructTtsL() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::New
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer* CPhoneAudioPlayer::New( 
    const CPhoneRingingTone& aRingingTone, 
    TInt aPriority, 
    TUint aPreference,
    MPhoneAudioPlayerObserver& aObserver, 
    TInt aId,
    CMdaServer* aMdaServer,
    TBool aExtSecNeeded )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::New() <" );
    CPhoneAudioPlayer* self = NULL;
    TRAPD( err, self = CPhoneAudioPlayer::NewL( 
        aRingingTone,
        aPriority,
        aPreference,
        aObserver,
        aId,
        aMdaServer,
        aExtSecNeeded ) );
    if ( err != KErrNone )
        {
        return NULL;
        }
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::New() >" );
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::NewL
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer* CPhoneAudioPlayer::NewL( 
    const CPhoneRingingTone& aRingingTone, 
    TInt aPriority,
    TUint aPreference,
    MPhoneAudioPlayerObserver& aObserver,
    TInt aId,
    CMdaServer* aMdaServer,
    TBool aExtSecNeeded )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::NewL() <" );
    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::NewL - aExtSecNeeded(%d)",
        aExtSecNeeded );

    // Check the file DRM property if extend secure is needed.
    if ( aExtSecNeeded )
        {
        if ( !aRingingTone.IsFileInRom() &&
             !aRingingTone.IsFileDrmProtected() )
            {
            PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::NewL - DRM extend security check permission denied" );
            User::Leave( KErrPermissionDenied );
            }
        PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::NewL - DRM extend security check ok" );
        }
    // RNG file types have to be played with CMdaAudioToneUtility player.
    // Otherwise use sample player
    TAudioDataFormat format = 
    ( aRingingTone.MimeType().CompareF( KFileListRngMimeType ) == KErrNone ) 
    ? EFormatTone :EFormatSample;

    CPhoneAudioPlayer* self = 
        new (ELeave) CPhoneAudioPlayer( 
            format,
            aObserver, 
            aPriority,
            aPreference,
            aId );

    CleanupStack::PushL( self );
    self->ConstructL( aRingingTone.FileName(), aMdaServer );
    CleanupStack::Pop( self );
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::NewL() >" );
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::NewL
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer* CPhoneAudioPlayer::NewL( 
    const TDesC& aFileName, 
    TInt aPriority,
    TUint aPreference,
    MPhoneAudioPlayerObserver& aObserver,
    TInt aId,
    CMdaServer* aMdaServer,
    TBool aExtSecNeeded )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::NewL() <" );
    CPhoneRingingTone* tone = CPhoneRingingTone::NewL( aFileName );
    CleanupStack::PushL( tone );

    CPhoneAudioPlayer* player = CPhoneAudioPlayer::NewL( *tone,
                                                         aPriority,
                                                         aPreference,
                                                         aObserver,
                                                         aId,
                                                         aMdaServer,
                                                         aExtSecNeeded );

    CleanupStack::PopAndDestroy( tone );
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::NewL() >");
    return player;
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::NewSeqL
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer* CPhoneAudioPlayer::NewSeqL(
    const TDesC8& aSequence,
    TInt aPriority,
    TUint aPreference,
    MPhoneAudioPlayerObserver& aObserver, 
    TInt aId,
    CMdaServer* aMdaServer )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::NewSeqL() <" );
    CPhoneAudioPlayer* self = 
        new (ELeave) CPhoneAudioPlayer( 
            EFormatTone, 
            aObserver, 
            aPriority,
            aPreference,
            aId );

    CleanupStack::PushL( self );
    self->ConstructSeqL( aSequence, aMdaServer  );
    CleanupStack::Pop( self );

    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::NewSeqL() >" );
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::NewTtsL
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer* CPhoneAudioPlayer::NewTtsL(
    const TDesC& aText,
    TInt aPriority,
    TUint aPreference,
    MPhoneAudioPlayerObserver& aObserver, 
    TInt aId )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::NewTtsL() <" );
    CPhoneAudioPlayer* self = 
        new (ELeave) CPhoneAudioPlayer( 
            EFormatTts, 
            aObserver, 
            aPriority,
            aPreference,
            aId );

    CleanupStack::PushL( self );
    self->ConstructTtsL( aText, aPriority, aPreference );
    CleanupStack::Pop( self );
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::NewTtsL() >" );
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::~CPhoneAudioPlayer
// -----------------------------------------------------------------------------
//
CPhoneAudioPlayer::~CPhoneAudioPlayer()
    {

    if ( iAudioOutput )
        {
        delete iAudioOutput;
        }
    if ( iTonePlayer )
        {
        delete iTonePlayer;
        } 
    if ( i3DPlugin )
       {
        i3DPlugin->Stop();
        delete i3DPlugin;
        }
    // ECom cleanup
    REComSession::FinalClose();
    if ( iSamplePlayer )
        {
        delete iSamplePlayer; 
        }
    if ( iTtsPlayer )
        {
        delete iTtsPlayer; 
        }
    if ( iSequence )
        {
        delete iSequence; 
        }
    if ( iTtsText )
        {
        delete iTtsText;
        } 
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::Play
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::Play( TRingingType aRingType, TInt aVolume, TInt aTTsToBePlayed )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::Play() <" );
    PHONEUIVIEW_PRINTF2( 
        "CPhoneAudioPlayer::Play - iFormat(%d), iPlayerStatus(%d)",
        iFormat, 
        iPlayerStatus );
    PHONEUIVIEW_PRINTF3( 
        "CPhoneAudioPlayer::Play - aRingType(%d), aVolume(%d), aTTsToBePlayed(%d)",
        aRingType, 
        aVolume,
        aTTsToBePlayed );
    iRingType = aRingType;
    iVolume = aVolume;
    iTTsToBePlayed = aTTsToBePlayed;

    if ( iFormat == EFormatTone )
        {
        switch ( iPlayerStatus )
            {
            case ETonePlaying:
                iTonePlayer->CancelPlay();
                DoPlay();
                break;

            case EToneReady:
                iPlayerStatus = ETonePlaying;
                DoPlay();
                break;

            case EToneLoading:
                iToBePlaying = ETrue;
                break;

            default:
                __ASSERT_DEBUG( EFalse, 
                       Panic( EPhoneViewCaseNotHandled ) );
            }
        }
    else if ( iFormat == EFormatTts )
        {
        switch ( iPlayerStatus )
            {
            case ETonePlaying:
                //stop previous playing
                iTtsPlayer->Stop();
                DoPlay();
                break;

            case EToneReady:
                iPlayerStatus = ETonePlaying;
                DoPlay();
                break;

            case EToneLoading:
                iToBePlaying = ETrue;
                break;

            default:
                __ASSERT_DEBUG( EFalse, 
                        Panic( EPhoneViewCaseNotHandled ) );
            }
        }
    else
        {
        switch ( iPlayerStatus )
            {
            case ETonePlaying:
                //stop previous playing
                StopPlaying();
                iPlayerStatus = ETonePlaying;
                DoPlay();
                break;

            case EToneReady:
                iPlayerStatus = ETonePlaying;
                DoPlay();
                break;

            case EToneLoading:
                iToBePlaying = ETrue;
                break;

            default:
                __ASSERT_DEBUG( EFalse, 
                        Panic( EPhoneViewCaseNotHandled ) );
            }
        }
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::Play() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::ReStartPlaying
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::ReStartPlaying()
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::ReStartPlaying() <" );
    PHONEUIVIEW_PRINTF3( 
        "CPhoneAudioPlayer::ReStartPlaying - iFormat(%d), iRingType(%d), iVolume(%d)",
        iFormat, 
        iRingType, 
        iVolume );

    if ( iFormat == EFormatTone )
        {
        iTonePlayer->Play();
        iPlayerStatus = ETonePlaying;
        }
    else if ( iFormat == EFormatTts )
        {
        iTtsPlayer->Play();
        iPlayerStatus = ETonePlaying;
        }
    else
        {
        iSamplePlayer->Play();
        iPlayerStatus = ETonePlaying;
        }
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::ReStartPlaying() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::StopPlaying
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::StopPlaying()
    {
   PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::StopPlaying() <" );

    iToBePlaying = EFalse;

    if ( iFormat == EFormatTone )
        {
        if ( EMdaAudioToneUtilityPrepared == iTonePlayer->State() )
            {
            iTonePlayer->CancelPrepare();
            }
        else if ( EMdaAudioToneUtilityPlaying == iTonePlayer->State() )
            {
            iTonePlayer->CancelPlay();
            }
        }
    else if ( iFormat == EFormatTts )
        {
        iTtsPlayer->Stop();
        }
    else
        {
        if ( i3DPlugin )
            {
            i3DPlugin->Stop();
            delete i3DPlugin;
            i3DPlugin = NULL;
            }
        else
            {
            iSamplePlayer->Stop();
            }
        }

    iPlayerStatus = EToneReady;
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::StopPlaying() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::MapcInitComplete
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::MapcInitComplete(
    TInt aError, 
    const TTimeIntervalMicroSeconds& /*aDuration*/ )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::MapcInitComplete() <" );
    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::MapcInitComplete - aError(%d)",
        aError );
    __ASSERT_DEBUG( ( ( iFormat == EFormatSample ) || ( iFormat == EFormatTts ) ) && 
        ( iPlayerStatus == EToneLoading ), Panic( EPhoneViewGeneralError ) );
    if ( aError == KErrNone )
        {
        if ( iToBePlaying )
            {
            iPlayerStatus = ETonePlaying;
            iToBePlaying = EFalse;
            DoPlay();
            }
        else
            {
            iPlayerStatus = EToneReady;
            }
        iObserver.HandleAudioPlayerInitComplete( iId );
        }
    else
        {
        iObserver.HandleAudioPlayerError( 
            MPhoneAudioPlayerObserver::EAudioPlayerInitializingFailure, 
            aError, iId );
        }
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::MapcInitComplete() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::MapcPlayComplete
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::MapcPlayComplete( TInt aError )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::MapcPlayComplete() <" );
    PHONEUIVIEW_PRINTF2( "CPhoneAudioPlayer::MapcPlayComplete - iFormat(%d), aError(%d)",
        iFormat, aError );
    iPlayerStatus = EToneReady;

    //Destruct 3D plugin before player itself gets destructed.
    //In case of continuous ringing type, stopplaying() handles
    //3d plugin destruction
    if ( i3DPlugin )
        {
        i3DPlugin->Stop();
        delete i3DPlugin;
        i3DPlugin = NULL;
        }

    if ( aError != KErrNone )
        {
        iObserver.HandleAudioPlayerError( 
            MPhoneAudioPlayerObserver::EAudioPlayerPlayingFailure, 
            aError, iId );
        }
    else
        {
        iObserver.HandlePlayingComplete( iId );
        }
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::MapcPlayComplete() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::MatoPrepareComplete
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::MatoPrepareComplete( TInt aError )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::MatoPrepareComplete() <" );
    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::MatoPrepareComplete - aError(%d)",
        aError );

    __ASSERT_DEBUG( ( iFormat == EFormatTone ) && ( iPlayerStatus == EToneLoading ), 
        Panic( EPhoneViewGeneralError ) );
    if ( aError == KErrNone )
        {
        if ( iToBePlaying )
            {
            iPlayerStatus = ETonePlaying;
            iToBePlaying = EFalse;
            DoPlay();
            }
        else
            {
            iPlayerStatus = EToneReady;
            }
        iObserver.HandleAudioPlayerInitComplete( iId );
        }
    else
        {
        iObserver.HandleAudioPlayerError( 
            MPhoneAudioPlayerObserver::EAudioPlayerInitializingFailure, 
            aError, iId );
        }
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::MatoPrepareComplete() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::MatoPlayComplete
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::MatoPlayComplete(TInt aError)
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::MatoPlayComplete() <" );
    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::MatoPlayComplete - aError(%d)",
        aError );
    if ( aError != KErrNone )
        {
        iObserver.HandleAudioPlayerError( 
            MPhoneAudioPlayerObserver::EAudioPlayerPlayingFailure, 
            aError, iId );
        }
    else
        {
        iObserver.HandlePlayingComplete( iId );
        }
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::MatoPlayComplete() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::DoPlay
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::DoPlay()
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::DoPlay() <" );
    __ASSERT_DEBUG( iPlayerStatus == ETonePlaying, 
        Panic( EPhoneViewGeneralError ) );

    SetRingingTypeProperties();

    TInt err(KErrNone);
    if ( iFormat == EFormatTone )
        {
        iTonePlayer->Play();
        }
    else if ( iFormat == EFormatTts )
        {
        iTtsPlayer->Play();
        }
    else
        {
        TRAP( err,i3DPlugin = C3DRingingToneInterface::NewL( KNullUid ) );
        if ( !err && i3DPlugin )
            {
            i3DPlugin->SetAttr( E3DRTIAttrAudioPlayerUtility, iSamplePlayer );
            TRAP( err,i3DPlugin->PlayL() )
            }
        if ( err || !i3DPlugin )
            {
            if ( !iAudioOutput )
                 {
                 PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::DoPlay c aud ");
                 TRAP (err, iAudioOutput = CAudioOutput::NewL( *iSamplePlayer ) );
                if ( err )
                    {
                    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::DoPlay iAudioOutput.err ", err );
                    }
               }
            if ( !err && iAudioOutput )
                 {
                 PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::DoPlay EALL ");
                 TRAP (err, iAudioOutput->SetAudioOutputL( CAudioOutput::EAll ) );
                 if ( err )
                     {
                     PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::DoPlay SetAudioOutputL.err ", err ); 
                     }

                 }
            iSamplePlayer->Play();
            }
        }
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::DoPlay() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::SetRingingTypeProperties
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::SetRingingTypeProperties()
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::SetRingingTypeProperties() <" );
    PHONEUIVIEW_PRINTF2( "CPhoneAudioPlayer::SetRingingTypeProperties - iFormat(%d), iRingType(%d)",
        iFormat, iRingType );
    TInt rampTime( 0 ); 
    if ( iFormat == EFormatTone )
        {
        switch ( iRingType )
            {
            case ETypeRinging:
                //If we have TTS activated and ringingtype is ringing:
                //We need to play TTS sequence again when ringingtone restarts.
                //Thats why we need to set ringingtype to ETypeRingingOnce because
                //it is the only way of knowing when ringingtone is completed. 
                //Then we can restart it with new TTS iterations.
                if ( iTTsToBePlayed )
                    {
                     //play only once
                    iTonePlayer->SetRepeats( 0, 
                        TTimeIntervalMicroSeconds(
                        KPhoneRingingRepeatsTrailPause ) ); 
                    iTonePlayer->SetVolume( ConvertVolume( iVolume ) );
                    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::SetRingingTypeProperties - Ringing tone with TTS, Set volume(%d) and ring once",
                        iVolume );
                    }
                else
                    {
                    iTonePlayer->SetRepeats( KPhoneMdaAudioToneRepeatForever,
                        TTimeIntervalMicroSeconds( 
                         KPhoneRingingRepeatsTrailPause ) );
                    iTonePlayer->SetVolume( ConvertVolume( iVolume ) );
                    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::SetRingingTypeProperties - Ringing tone, Set volume(%d)",
                        iVolume );
                    }
                break;

            case ETypeRingingOnce:
                //play only once
                iTonePlayer->SetRepeats( 0, 
                    TTimeIntervalMicroSeconds(
                    KPhoneRingingRepeatsTrailPause ) ); 
                iTonePlayer->SetVolume( ConvertVolume( iVolume ) );
                PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::SetRingingTypeProperties - Ring once tone, set volume(%d)",
                    iVolume );
                break;

            case ETypeAscending:
                iTonePlayer->SetRepeats( KPhoneMdaAudioToneRepeatForever,
                    TTimeIntervalMicroSeconds(
                    KPhoneRingingRepeatsTrailPause ) );

                //Special case for TTs + ascending profile. Volume 
                //and ramptime needs to be different as usual
                if ( iTTsToBePlayed )
                    {
                    //rampTime in this case is only 3 seconds and volume is 1.
                    rampTime = KPhoneAudioAscendingRampDuration;
                    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::SetRingingTypeProperties - Ascending tone with TTS, Start from lowest volume, rampTime(%d)",
                        rampTime );
                    iTonePlayer->SetVolumeRamp( 
                        TTimeIntervalMicroSeconds( rampTime ) ); 

                    iCurrentVolume = 1; //we start from the lowest volume
                    iTonePlayer->SetVolume( ConvertVolume( iCurrentVolume ) );
                    }
                else
                    {
                    //rampTime: time for one step * volume level
                    rampTime = KPhoneAudioAscendingRampDuration*iVolume;
                    PHONEUIVIEW_PRINTF2( "CPhoneAudioPlayer::SetRingingTypeProperties - Ascending tone, Set volume(%d), rampTime(%d)",
                        iVolume, rampTime );
                    iTonePlayer->SetVolumeRamp( 
                        TTimeIntervalMicroSeconds( rampTime ) );

                    iCurrentVolume = 1; //we start from the lowest volume
                    iTonePlayer->SetVolume( ConvertVolume( iVolume ) ); 
                    }
                break;

            default:
                Panic( EPhoneViewGeneralError );
                break;
            }

        PHONEUIVIEW_PRINTF3( "CPhoneAudioPlayer::SetRingingTypeProperties - Tone, iPriority(%d), iPreference(%d), iVolume(%d)",
            iPriority, iPreference, iVolume );

        //Set priority and preference
        iTonePlayer->SetPriority( iPriority, 
            static_cast< TMdaPriorityPreference >( iPreference ) );
        }
    else if ( iFormat == EFormatTts )
        {
        switch ( iRingType )
            {
            case ETypeRinging:
            case ETypeAscending:
                // Not possible iRingType for TTS.
                break;

            case ETypeRingingOnce:
                iTtsPlayer->SetRepeats( 0, // play only once
                    TTimeIntervalMicroSeconds( 
                    KPhoneRingingRepeatsTrailPause ) );
                iTtsPlayer->SetVolume( ConvertVolume( iVolume ) );
                PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::SetRingingTypeProperties - TTS, Set volume(%d)",
                    iVolume );
                break;

            default:
                __ASSERT_DEBUG( EFalse, 
                        Panic( EPhoneViewCaseNotHandled ) );
            }
        PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::SetRingingTypeProperties - TTS" );
        }
    else
        {
        switch ( iRingType )
            {
            case ETypeRinging:
                //If we have TTS activated and ringingtype is ringing:
                //We need to play TTS sequence again when ringingtone restarts.
                //Thats why we need to set ringingtype to ETypeRingingOnce because
                //it is theonly way of knowing when ringingtone is completed. 
                //Then we can restartit with new TTS iterations.
                if ( iTTsToBePlayed )
                    {
                     //play only once
                    iSamplePlayer->SetRepeats( 0, 
                        TTimeIntervalMicroSeconds(
                        KPhoneRingingRepeatsTrailPause ) ); 
                    iSamplePlayer->SetVolume( ConvertVolume( iVolume ) );
                    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::SetRingingTypeProperties - Ringing sample with TTS, Ring only once, Set volume(%d)",
                        iVolume );
                    }
                else
                    {
                    iSamplePlayer->SetRepeats( KPhoneMdaAudioToneRepeatForever,
                        TTimeIntervalMicroSeconds( 
                        KPhoneRingingRepeatsTrailPause ) );
                    iSamplePlayer->SetVolume( ConvertVolume( iVolume ) );
                    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::SetRingingTypeProperties - Ringing sample, Set volume(%d)",
                        iVolume );
                    }
                break;

            case ETypeRingingOnce:
                iSamplePlayer->SetRepeats( 0,//play only once
                    TTimeIntervalMicroSeconds( 
                    KPhoneRingingRepeatsTrailPause ) ); 
                iSamplePlayer->SetVolume( ConvertVolume( iVolume ) );
                PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::SetRingingTypeProperties - Ring once sample, set volume(%d)",
                    iVolume );
                break;

            case ETypeAscending:
                iSamplePlayer->SetRepeats( KPhoneMdaAudioToneRepeatForever,
                    TTimeIntervalMicroSeconds(
                    KPhoneRingingRepeatsTrailPause ) );

                //Special case for TTs + ascending profile. Volume 
                //and ramptime needs to be different as usual
                if ( iTTsToBePlayed )
                    {
                    //rampTime in this case is only 3 seconds and volume is 1.
                    rampTime = KPhoneAudioAscendingRampDuration;
                    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::SetRingingTypeProperties - Ascending sample with TTS, Start from lowest volume, rampTime(%d)",
                        rampTime );
                    iSamplePlayer->SetVolumeRamp( 
                        TTimeIntervalMicroSeconds( rampTime ) ); 

                    iCurrentVolume = 1; //we start from the lowest volume
                    iSamplePlayer->SetVolume( ConvertVolume( iCurrentVolume ) );
                    }
                else
                    {
                    //rampTime: time for one step * volume level
                    rampTime = KPhoneAudioAscendingRampDuration*iVolume;
                    PHONEUIVIEW_PRINTF2( "CPhoneAudioPlayer::SetRingingTypeProperties - Ascending sample, Set volume(%d), rampTime(%d)",
                        iVolume, rampTime );
                    iSamplePlayer->SetVolumeRamp( 
                        TTimeIntervalMicroSeconds( rampTime ) );

                    iCurrentVolume = 1; //we start from the lowest volume
                    iSamplePlayer->SetVolume( ConvertVolume( iVolume ) );
                    }
                break;

            default:
                Panic( EPhoneViewGeneralError );
                break;
            }

        PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::SetRingingTypeProperties - Sample" );
        }
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::SetRingingTypeProperties() >" );
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::SetNewVolumeAndRamptime
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::SetNewVolumeAndRamptime( TInt aVolume, TInt aRamptime )
    {
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::SetNewVolumeAndRamptime() <" );
 
    //Check that volume is in valid range.
    TInt volume = aVolume<1 ? 1:aVolume;
    PHONEUIVIEW_PRINTF( "CPhoneAudioPlayer::SetNewVolumeAndRamptime - aVolume(%d)",
        aVolume ); 

    if ( iFormat == EFormatTone && iTonePlayer )
        {
        iTonePlayer->SetVolumeRamp(TTimeIntervalMicroSeconds( aRamptime ) );
        iTonePlayer->SetVolume( ConvertVolume( volume ) );
        PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::SetNewVolumeAndRamptime - Tone player" );
        }
    else if ( iFormat == EFormatTts && iTtsPlayer )
        {
        iTtsPlayer->SetVolumeRamp(TTimeIntervalMicroSeconds( aRamptime ) );
        iTtsPlayer->SetVolume( ConvertVolume( volume ) );
        PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::SetNewVolumeAndRamptime - TTS player" );
        }
    else 
        {
        iSamplePlayer->SetVolumeRamp(TTimeIntervalMicroSeconds( aRamptime ) );
        iSamplePlayer->SetVolume( ConvertVolume( volume ) );
        PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::SetNewVolumeAndRamptime - Sample player" );
        }
 
    PHONEUIVIEW_PRINT( "CPhoneAudioPlayer::SetNewVolumeAndRamptime() >" );
    }
 

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::ConvertVolume
// -----------------------------------------------------------------------------
//
TInt CPhoneAudioPlayer::ConvertVolume( TInt aVolume )
    {
    TInt result( 0 );

    if ( iFormat == EFormatTone && iTonePlayer )
        {
        result = iTonePlayer->MaxVolume() * aVolume / KMaxVolumeLevel;
        }
    else if ( iFormat == EFormatTts && iTtsPlayer )
        {
        result = iTtsPlayer->MaxVolume() * aVolume / KMaxVolumeLevel;
        }
    else
        {
        result = iSamplePlayer->MaxVolume() * aVolume / KMaxVolumeLevel; 
        }

    // if user has selected minimum volume level
    // set HW volume 1
    if ( aVolume == KMinVolumeLevel && result == 0 )
        {
        result = 1; 
        }
 
    //_DDPRINT( 5, "P.AudPlayer.ConvertVolume.result", result ); 
    return result;
    }

 
// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::SetTTsToBePlayed
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::SetTTsToBePlayed( TBool aTTsToBePlayed )
    {
    iTTsToBePlayed = aTTsToBePlayed;
    }

// -----------------------------------------------------------------------------
// CPhoneAudioPlayer::MutePlaying
// -----------------------------------------------------------------------------
//
void CPhoneAudioPlayer::MutePlaying()
    {
    //_DPRINT( 5, "P.Aud.Mute" );
    if ( iPlayerStatus == ETonePlaying )
        {
        if ( iFormat == EFormatTone && iTonePlayer )
            {
            iTonePlayer->SetVolume(0);
            }
        else if ( iFormat == EFormatTts && iTtsPlayer )
            {
            iTtsPlayer->SetVolume(0);
            }
        else // EFormatSample
            {
            iSamplePlayer->SetVolume(0);
            }
        }
    else
        {
        // Mute called during EToneLoading state.
        iToBePlaying = EFalse;
        } 
    }
 
//  End of File
