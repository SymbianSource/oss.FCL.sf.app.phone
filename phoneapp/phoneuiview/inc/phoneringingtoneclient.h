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
* Description:  Client interface for phone ringing tone server.
*
*/


#ifndef RPHONERINGINGTONEPLAYER_H
#define RPHONERINGINGTONEPLAYER_H

// INCLUDES
#include <Profile.hrh>

// FORWARD DECLARATIONS
class TPhoneCommandParam;

// CLASS DECLARATION

class RPhoneRingingTonePlayer : public RSessionBase
    {
public:
    /**
    * C++ default constructor.
    */
    RPhoneRingingTonePlayer();
    
    /**
    * Destructor.
    */
    ~RPhoneRingingTonePlayer();

    // Tone settings
    
    /**
    * Sets extended security required flag.
    * @param aNeeded Extended security required.
    */
    void SetExtendedSecurityNeeded( TBool aNeeded );
    
    /**
    * Sets extended security required flag.
    * @param aLimit Extended security required.
    */
    void SetToneFileSizeLimit( TInt aLimit );
    
    /**
    * Sets ringing tone.
    * @param aFilename Ringing tone.
    */
    void SetRingingToneL( const TDesC& aFilename );
    
    /**
    * Sets default ringing tone.
    * @param aFilename Ringing tone.
    */
    void SetDefaultRingingToneL( const TDesC& aFilename );

    // Playback control
    
    /**
    * Play audio ring tone 
    * @param aVolume Volume used to play the ringing tone.
    * @param aRingingType Ringing type.
    */
    void PlayAudioRingTone( TInt aVolume, TProfileRingingType aRingingType );

    /**
    * Play TTS tone simultaneously with Personal or Default tones.
    * This method does not command phone to play TTS immediately. Instead 
    * it registers a request to play TTS, when Personal or default tones 
    * are played.
    * @param aTextToSay Text, that should be pronounced by the Text-To-Speech 
    * engine. This is normal descriptor, no UTF-8 and no "(tts)" prefix
    * @param aVolume Volume for the TTS-tone, Does not effect the volume 
    * of the normal tone, that will be played simulteneously with the TTS
    * @param aRingingType ringing type.
    */
    void PlayTtsTone( const TDesC& aTextToSay, TInt aVolume, TProfileRingingType aRingingType );

    /**
    * Play default tone.
    * @param aVolume volume used for the playing.
    * @param aRingingType ringing type.
    */
    void PlayDefaultTone( TInt aVolume, TProfileRingingType aRingingType );

    /**
    * Play backup tone.
    * @param aVolume volume used for the playing.
    * @param aRingingType ringing type.
    */
    void PlayBackupTone( TInt aVolume, TProfileRingingType aRingingType );

    /**
    * Play silent tone. No_sound.wav will be played. If vibration alert
    * is ON, it will vibrate.
    */
    void PlaySilentTone();

    /**
    * Playing the Beep once.
    * @param aVolume volume used to play the ringing tone.
    */
    void BeepOnce( TInt aVolume );

    /**
    * Play unsecure VoIP tone.
    * Play tone if secure call fails and call is changed to unsecure call
    */
    void PlayUnsecureVoIPTone();

    /**
    * Continues video playback with muted audio.
    * Ringing tone is stopped.
    */
    void MuteRingingTone();

    /**
    * Stops playing the ringing tone.
    * Method does not do anything if ringing tone is not playing.
    */
    void StopPlaying();


public:
    /**
    * Establishes a connection with Server.
    * @return Result code indicating the success of operation.
    */
    TInt Connect();
    };

#endif // CPHONERINGINGTONEPLAYERAO_H

// End of file
