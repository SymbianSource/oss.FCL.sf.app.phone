/*
* Copyright (c) 2002-2005,2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Ringing tone player.
*
*/


// INCLUDE FILES
#include <cntdef.h>
#include <f32file.h>
#include <featmgr.h>
#include <utility.h>
#include <AudioPreference.h>
#include <data_caging_path_literals.hrh>
#include <MusicPlayerInternalCRKeys.h>
#include <ProfileEngineDomainCRKeys.h>
#include <telephonyvariant.hrh>
#include <UikonInternalPSKeys.h>
#include <DRMHelperServerInternalCRKeys.h>
#include <DRMHelper.h>
#include <activeidle2domainpskeys.h>

#include "cphoneringingtoneplayerao.h"
#include "phoneui.pan"
#include "phoneui.hrh"
#include "mphoneringingtoneobserver.h"
#include "cphonetimer.h"
#include "cphonecenrepproxy.h"
#include "phoneconstants.h"
#include "phonelogger.h"
#include "cphoneringingtone.h"
#include "tphonecmdparamringtone.h"
#include "cphonepubsubproxy.h"
#include "cphonecenrepproxy.h"

// CONSTANTS

// Minimum acceptable length of parsed DRM message.
const TInt KPhoneMinDRMTextLength = 3;
// Value field of parsed DRM string.

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::NewL
// -----------------------------------------------------------------------------
//
CPhoneRingingTonePlayerAO* CPhoneRingingTonePlayerAO::NewL()
    {
    CPhoneRingingTonePlayerAO* self = 
        new ( ELeave ) CPhoneRingingTonePlayerAO();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::CPhoneRingingTonePlayerAO
// -----------------------------------------------------------------------------
//
CPhoneRingingTonePlayerAO::CPhoneRingingTonePlayerAO() :
    CActive( CActive::EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::ConstructL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ConstructL()" );

    // Connect to the tone server
    User::LeaveIfError( iToneServer.Connect() );

    // Construct timer.
    iTimer = CPhoneTimer::NewL();

    // Checks if DRM extend security is needed
    TInt extSecNeeded = KErrNone;
    TInt err = CPhoneCenRepProxy::Instance()->GetInt(
        KCRUidMusicPlayerFeatures,
        KRequireDRMInPlayback,
        extSecNeeded );

    iExtSecNeeded = ( ( err == KErrNone ) && extSecNeeded ) ? ETrue : EFalse;
    iToneServer.SetExtendedSecurityNeeded( iExtSecNeeded );

    CPhoneCenRepProxy::Instance()->NotifyChangeL(
        KCRUidDRMHelperServer, 
        KDRMHelperServerNotificationPassive,
        this );

    // Check video player configuration.
    iArbitraryVideoScaling = 
    CPhoneCenRepProxy::Instance()->IsTelephonyFeatureSupported( 
            KTelephonyLVFlagArbitraryVideoScaling );

    // Construct the default ringing tone
    TBuf<KMaxFileName> defaultTone;
    err = CPhoneCenRepProxy::Instance()->GetString(
        KCRUidProfileEngine,
        KProEngDefaultRingingTone,
        defaultTone );

    if ( err == KErrNone )
        {
        iToneServer.SetDefaultRingingToneL( defaultTone );
        }

    // Get maximum tone file size
    GetMaxToneFileSizeL( iToneFileSizeLimitKB );

    // And set the tone size limit to the tone server too
    iToneServer.SetToneFileSizeLimit( iToneFileSizeLimitKB );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::~CPhoneRingingTonePlayerAO
// -----------------------------------------------------------------------------
//
CPhoneRingingTonePlayerAO::~CPhoneRingingTonePlayerAO()
    {
    Cancel();
    iToneServer.Close();

    delete iTimer;
    delete iAudioVideoRingingTone;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayRingToneL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayRingToneL( 
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayRingToneL()" );

    // Check that the parameter is a ringing tone
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdRingTone )
        {
        // Cast to get the ringing tone parameters
        TPhoneCmdParamRingTone* ringToneParam = 
            static_cast<TPhoneCmdParamRingTone*>( aCommandParam );

        // Get volume and ringing tone type
        const TInt volume = ringToneParam->Volume();
        TProfileRingingType ringingType = 
            static_cast<TProfileRingingType>( ringToneParam->RingingType() );

        // Set ringing tone file name to server
        iToneServer.SetRingingToneL( ringToneParam->RingTone() );

        if ( !iAudioVideoRingingTone )
            {
            iAudioVideoRingingTone = CPhoneRingingTone::NewL(
                ringToneParam->RingTone() );
            }
        else if ( iAudioVideoRingingTone->FileName().Compare(
            ringToneParam->RingTone() ) ) 
            {
            iAudioVideoRingingTone->SetFileName(
                ringToneParam->RingTone() );
            }

        // Caller contact text and image and image has
        // higher priority than video ringing tone set for caller.
        if ( iAudioVideoRingingTone->IsVideoRingingTone() &&
             ringToneParam->IsCallerImage() )
            {
            // Play only audio from video ringingtone
            PlayAudioRingTone(
                    volume,
                    ringingType );
            }
        else if ( iAudioVideoRingingTone->IsVideoRingingTone() &&
                !ringToneParam->IsCallerText() )
            {
            // Play video ring tone
            const TBool ringToneIsPersonal = ringToneParam->Type() ==
                EPhoneRingTonePersonal;
            PlayVideoRingingTone( 
                *iAudioVideoRingingTone, 
                volume, 
                ringingType,
                ringToneIsPersonal );
            }
        else
            {
            // Play text to speech, if available
            if ( ringToneParam->TextToSay().Length() )
                {
                PlayTtsTone(
                    ringToneParam->TextToSay(),
                    volume,
                    ringingType );
                }
            // Play audio ring tone
            PlayAudioRingTone( 
                volume, 
                ringingType );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayAudioRingTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayAudioRingTone( 
    TInt aVolume, 
    TProfileRingingType aRingingType )
    {
    iToneServer.PlayAudioRingTone( aVolume, aRingingType );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::BeepOnce
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::BeepOnce( TInt aVolume )
    {
    iToneServer.BeepOnce( aVolume );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlaySilentTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlaySilentTone()
    {
    iToneServer.PlaySilentTone();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayUnsecureVoIPTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayUnsecureVoIPTone()
    {
    iToneServer.PlayUnsecureVoIPTone();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::MuteRingingToneOnAnswer
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::MuteRingingToneOnAnswer()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::MuteRingingToneOnAnswer()" );

    if ( iVideoPlayer &&
            ( iTonePlayingStatus == EVideoTonePlaying ||
              iTonePlayingStatus == EPersonalVideoTonePlaying ) )
        {
        // Mute the video audio
        iVideoPlayer->MuteVideoRingTone();
        iTonePlayingStatus = ESilentVideoTonePlaying;
        iVolume = 0;
        }
    else if ( iTonePlayingStatus != ESilentVideoTonePlaying )
        {
        iToneServer.MuteRingingTone();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::MuteRingingTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::MuteRingingTone()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::MuteRingingTone()" );

    if ( iVideoPlayer &&
            ( iTonePlayingStatus == EVideoTonePlaying ||
              iTonePlayingStatus == EPersonalVideoTonePlaying ) )
        {
        // Mute the video audio
        iVideoPlayer->MuteVideoRingTone();
        iTonePlayingStatus = ESilentVideoTonePlaying;
        iVolume = 0;
        }
    else if ( iTonePlayingStatus != ESilentVideoTonePlaying )
        {
        StopPlaying();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::StopPlaying
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::StopPlaying()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::StopPlaying()" );

    // Stop all playback

    iToneServer.StopPlaying();

    if ( iVideoPlayer )
        {
        iVideoPlayer->StopVideoRingTone();
        }

    iTimer->Cancel();

    iTonePlayingStatus = EIdle;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayTtsTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayTtsTone( 
    const TDesC& aTextToSay, 
    TInt aVolume,
    TProfileRingingType aRingingType )
    {
    iToneServer.PlayTtsTone( aTextToSay, aVolume, aRingingType );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleTimeOutL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleTimeOutL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleTimeOutL()" );

    // Guarding timer for startup has expired
    if ( iTonePlayingStatus == EVideoTonePlaying ||
         iTonePlayingStatus == ESilentVideoTonePlaying || 
         iTonePlayingStatus == EPersonalVideoTonePlaying )
        {
        iVideoPlayer->CancelVideoRingTone(); // close video ringtone display
        PlayDefaultTone( iVolume, iRingingType );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayDefaultTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayDefaultTone( 
    TInt aVolume, 
    TProfileRingingType aRingingType )
    {
    iToneServer.PlayDefaultTone( aVolume, aRingingType );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayBackupTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayBackupTone( 
    TInt aVolume, 
    TProfileRingingType aRingingType )
    {
    iToneServer.PlayBackupTone( aVolume, aRingingType );
    }


// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::ConvertVideoRingingType
// -----------------------------------------------------------------------------
//
MPhoneVideoPlayer::TPlayMode CPhoneRingingTonePlayerAO::ConvertVideoRingingType(
    TProfileRingingType aRingingType )
    {
    MPhoneVideoPlayer::TPlayMode playMode;

    switch ( aRingingType )
        {
        case EProfileRingingTypeAscending:
            playMode = MPhoneVideoPlayer::EPlayAscending;
            break;
        case EProfileRingingTypeRingingOnce:
            playMode = MPhoneVideoPlayer::EPlayOnce;
            break;
        case EProfileRingingTypeRinging:
        default:
            playMode = MPhoneVideoPlayer::EPlayInLoop;
            break;
        }

    return playMode;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::RunL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::RunL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::RunL()" );

    switch ( iState )
        {
        case EPlayingDefaultVideo:
            {
            PlayAudioRingTone( iVolume, iRingingType );
            }
            break;

        case EIdleState:
        default:
            break;
        } // switch iState

    iState = EIdleState;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::DoCancel
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::DoCancel()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::DoCancel()" );
    // Request is completed immediately before SetActive.
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::SetVideoPlayer
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::SetVideoPlayer( 
    MPhoneVideoPlayer* aVideoPlayer )
    {
    iVideoPlayer = aVideoPlayer;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayVideoRingingTone
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayVideoRingingTone( 
    const CPhoneRingingTone& aRingingTone, 
    TInt aVolume, 
    TProfileRingingType aRingingType,
    TBool aPersonalTone )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayVideoRingingTone()" );

    __ASSERT_DEBUG( iVideoPlayer, Panic( EPhoneViewGeneralError ) );

    iVolume = aVolume;
    iRingingType = aRingingType;
    TBool startTimer( ETrue );

     // Extended security check
    if ( ExtendedSecurity() )
        {
        if ( !aRingingTone.IsFileInRom() &&
             !aRingingTone.IsFileDrmProtected() )
            {
            __PHONELOG( EBasic, 
                        EPhoneUIView, 
                        "CPhoneRingingTonePlayerAO::HandleVideoPlayerError - PermissionDenied" );
            PlayDefaultTone( iVolume, iRingingType );
            return;
            }
        __PHONELOG( EBasic, 
                    EPhoneUIView, 
                    "CPhoneRingingTonePlayerAO::HandleVideoPlayerError - ExtSecChk ok" );
        }
 
    __PHONELOG( EBasic, EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayVideoRingingTone - play" );
    switch( aRingingType )
        {
        case EProfileRingingTypeSilent:
            iVideoPlayer->PlayVideoRingTone( 
                aRingingTone.FileName(),
                ConvertVideoRingingType( aRingingType ),
                0,
                iArbitraryVideoScaling,
                this );
            iToneServer.PlaySilentTone(); // To start vibra, video may fail.
            iVolume = 0; // for repeat
            startTimer = EFalse; // no need for fallback
            break;

        case EProfileRingingTypeBeepOnce:
            iVideoPlayer->PlayVideoRingTone( 
                aRingingTone.FileName(),
                ConvertVideoRingingType( aRingingType ),
                0,
                iArbitraryVideoScaling,
                this );
            BeepOnce( iVolume );
            iVolume = 0; // for repeat
            startTimer = EFalse; // no need for fallback
            break;
        default:
            iVideoPlayer->PlayVideoRingTone( 
                aRingingTone.FileName(),
                ConvertVideoRingingType( aRingingType ),
                iVolume,
                iArbitraryVideoScaling,
                this );
            break;
        }

    if ( aPersonalTone )
        {
        iTonePlayingStatus = EPersonalVideoTonePlaying;
        }
    else
        {
        iTonePlayingStatus = EVideoTonePlaying;
        }

    if ( startTimer && !iTimer->IsActive() )
        {
        // Start timer to guard video opening
        iTimer->After( KPhoneMaxRingingWaiting, this );
        }

    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleVideoPlayerError
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleVideoPlayerError( 
    TPhoneVideoPlayerErrorEvent /*aEvent*/,
    TInt aError )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleVideoPlayerError()" );

    if ( aError )
        {
        __PHONELOG1( EBasic, 
                     EPhoneUIView, 
                     "CPhoneRingingTonePlayerAO::HandleVideoPlayerError - error (%d)",
                     aError );
        }

    // cancel guarding timer
    iTimer->Cancel();

    // to remove video window
    iVideoPlayer->CancelVideoRingTone();

    if ( iTonePlayingStatus == EPersonalVideoTonePlaying )
        {
        // Play default tone (active profile tone).
        if ( iAudioVideoRingingTone &&
             !iAudioVideoRingingTone->IsVideoRingingTone() )
            {
            iToneServer.PlayAudioRingTone( iVolume, iRingingType );
            }
        else // audio/video tone is video
            {
            PlayDefaultVideoAsync();
            }
        }
    else
        {
        // Play backup tone
        PlayDefaultTone( iVolume, iRingingType );
        }

    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleVideoPlayerInitComplete
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleVideoPlayerInitComplete()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleVideoPlayerInitComplete()" );

    // cancel guarding timer
    iTimer->Cancel();

    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleVideoPlayerPlayingComplete
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleVideoPlayerPlayingComplete()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleVideoPlayerPlayingComplete()" );
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::PlayDefaultVideoAsync
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::PlayDefaultVideoAsync()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::PlayDefaultVideoAsync()" );

    Cancel();

    iState = EPlayingDefaultVideo; 

    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::ExtendedSecurity
// -----------------------------------------------------------------------------
//
TBool CPhoneRingingTonePlayerAO::ExtendedSecurity() const
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::ExtendedSecurity()" );
    return iExtSecNeeded;
    }


// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::CheckAndHandleToneSizeLimit
// -----------------------------------------------------------------------------
//
TBool CPhoneRingingTonePlayerAO::CheckAndHandleToneSizeLimit(
     CPhoneRingingTone* aRingingTone )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::CheckAndHandleToneSizeLimit()" );

    TBool bValidSize = ETrue;

    // If user has somehow managed to get a too large file as ringing tone,
    // play default tone instead.
    if ( iToneFileSizeLimitKB )
        {
        if ( CheckToneFileSize( aRingingTone->FileName(), iToneFileSizeLimitKB) != KErrNone )
            {
            bValidSize = EFalse;
            }
        }
    __PHONELOG1( 
        EBasic,
        EPhoneUIView, 
        "CPhoneRingingTonePlayerAO::CheckAndHandleToneSizeLimit - bValidSize(%d)",
        bValidSize);

    return bValidSize;
    }

// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::CheckToneFileSize
// -----------------------------------------------------------------------------
//
TInt CPhoneRingingTonePlayerAO::CheckToneFileSize( const TDesC& aFile, TInt aSizeLimitKB )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::CheckToneFileSize()" );

    // Get file size
    TInt size = 0;
    TInt error = KErrNone;
    RFs& fs = CCoeEnv::Static()->FsSession();
    TEntry entry;
    if (KErrNone == fs.Entry( aFile, entry ))
        {
        size = entry.iSize;
        }

    // Check
    aSizeLimitKB *= Kkilo;
    if ( aSizeLimitKB  &&  size > aSizeLimitKB )
        {
        error = KErrTooBig;
        }

    __PHONELOG1( 
        EBasic,
        EPhoneUIView, 
        "CPhoneRingingTonePlayerAO::CheckToneFileSize - size (%d)",
        size );
    return error;
    }


// -----------------------------------------------------------------------------
// CPhoneRingingTonePlayerAO::GetMaxToneFileSizeL
// -----------------------------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::GetMaxToneFileSizeL( TInt& aMaxSizeKB ) const
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::GetMaxToneFileSizeL()" );

    const TInt error = CPhoneCenRepProxy::Instance()->GetInt( 
            KCRUidProfileEngine,
            KProEngRingingToneMaxSize,
            aMaxSizeKB );

    if ( error != KErrNone )
        {
        aMaxSizeKB = 0;
        }
    if ( aMaxSizeKB < 0 )
        {
        aMaxSizeKB = 0;
        }

    __PHONELOG2( 
        EBasic,
        EPhoneUIView, 
        "CPhoneRingingTonePlayerAO::GetMaxToneFileSizeL - error (%d), aMaxSizeKB(%d)",
        error,
        aMaxSizeKB );
    }

// -----------------------------------------------------------
// CPhoneRingingTonePlayerAO::HandleCenRepChangeL
// -----------------------------------------------------------
//
void CPhoneRingingTonePlayerAO::HandleCenRepChangeL( 
    const TUid& aUid,
    const TUint /*aId*/ )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneRingingTonePlayerAO::HandleCenRepChangeL()");

    if ( aUid == KCRUidDRMHelperServer )
        {
        TBuf<256> tempBuf16( KNullDesC );

        User::LeaveIfError( CPhoneCenRepProxy::Instance()->GetString(
            KCRUidDRMHelperServer,
            KDRMHelperServerNotificationPassive,
            tempBuf16 ) );

        const TBool idleIsTopApp = CPhonePubSubProxy::Instance()->Value(
            KPSUidAiInformation, KActiveIdleState ) == EPSAiForeground;

        // Quick validity check and
        // The first Tuint8 is the times the content has been informed.
        // Note should not shown until Idle.
        if ( idleIsTopApp && tempBuf16.Length() >= KPhoneMinDRMTextLength )
            { 
            TBuf8<256> tempBuf8;
            TPtrC8 ptr((TUint8*)tempBuf16.Ptr(), tempBuf16.Size());
            HBufC8* buf8 = HBufC8::NewLC( 256 );
            buf8->Des().Copy(ptr);
            // Extract URI etc. 
            // buf8 format:
            //<TUint8 aTimes><TUint8 aExpirationMark>
            //<TUint8 aPermissionType>
            //<TUint8 aAutomatedContentType><TDesC8 aContentID>
            TInt8 count = (TInt8)(*(buf8->Ptr())); 
            TChar mark = (TChar)(*(buf8->Ptr()+1));
            TInt8 permtype = (TChar)(*(buf8->Ptr()+2));
            TInt8 automcontenttype = (TChar)(*(buf8->Ptr()+3)); 
            TBuf8<256> curi = buf8->Right( buf8->Length()-4);

            if ( curi.Ptr()[curi.Size()-1] == 0 )
                {
                // The last character in the descriptor is 0, which means
                // that the original 16-bit desc was padded so
                // remove the last char...
                curi.SetLength(curi.Size()-1);
                }

            // Create DRM helper. 
            CDRMHelper* drmHelper = CDRMHelper::NewLC();
            // Show notification.
            drmHelper->SetAutomatedType( (CDRMHelper::TDRMHelperAutomatedType)automcontenttype );
            TInt error = drmHelper->CheckRightsAmountL( curi );

            __PHONELOG1( EBasic, EPhoneUIView, 
                "CPhoneRingingTonePlayerAO::HandleCenRepChangeL > CheckRightsAmountL, error: %d"
                , error );

            CleanupStack::PopAndDestroy( drmHelper ); 
            CleanupStack::PopAndDestroy( buf8 ); 
            } 
        }
    }

//  End of File
