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
* Description:  Interface class for phone ringing tone server.
*
*/


#ifndef __MPHONERINGINGTONESRV_H
#define __MPHONERINGINGTONESRV_H

// CLASS DECLARATION
class MPhoneRingingToneServer
    {
public:
    /**
    * Increments sessions.
    */
    virtual void IncrementSessions() = 0;

    /**
    * Decrements sessions.
    */
    virtual void DecrementSessions() = 0;

    // Ringing tone settings
    
    /**
    * Sets extended security required flag.
    * @param aNeeded Extended security required.
    */
    virtual void SetExtendedSecurityNeeded( TBool aNeeded ) = 0;
    
    /**
    * Sets extended security required flag.
    * @param aLimit Extended security required.
    */
    virtual void SetToneFileSizeLimit( TInt aLimit ) = 0;
    
    /**
    * Sets ringing tone.
    */
    virtual void SetRingingToneL( const RMessage2& aMessage ) = 0;
    
    /**
    * Sets default ringing tone.
    * @param aMessage Message from Client.
    */
    virtual void SetDefaultRingingToneL( const RMessage2& aMessage ) = 0;

    // Playback control
    
    /**
    * Play audio ring tone. 
    * @return True if the client message should not be completed.
    * @param aMessage Message from Client.
    */
    virtual TBool PlayAudioRingTone( const RMessage2& aMessage ) = 0;

    /**
    * Play TTS tone simultaneously with Personal or Default tones.
    * This method does not command phone to play TTS immediately. Instead 
    * it registers a request to play TTS, when Personal or default tones 
    * are played.
    * @param aMessage Message from Client.
    */
    virtual void PlayTtsTone( const RMessage2& aMessage ) = 0;

    /**
    * Play default tone.
    * @param aMessage Message from Client.
    */
    virtual void PlayDefaultTone( const RMessage2& aMessage ) = 0;

    /**
    * Play backup tone.
    * @param aMessage Message from Client.
    */
    virtual void PlayBackupTone( const RMessage2& aMessage ) = 0;

    /**
    * Play silent tone. No_sound.wav will be played. If vibration alert
    * is ON, it will vibrate.
    */
    virtual void PlaySilentTone() = 0;

    /**
    * Playing the Beep once.
    * @param aMessage Message from Client.
    */
    virtual void BeepOnce( const RMessage2& aMessage ) = 0;

    /**
    * Play unsecure VoIP tone.
    * Play tone if secure call fails and call is changed to unsecure call.
    */
    virtual void PlayUnsecureVoIPTone() = 0;

    /**
    * Mutes ringing tone.
    */
    virtual void MuteRingingTone() = 0;

    /**
    * Stops playing the ringing tone.
    * Method does not do anything if ringing tone is not playing.
    */
    virtual void StopPlaying() = 0;

    };

#endif // __MPHONERINGINGTONESRV_H
