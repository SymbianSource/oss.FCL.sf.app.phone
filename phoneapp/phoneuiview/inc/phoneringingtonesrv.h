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
* Description:  Phone ringing tone server definitions.
*
*/


#ifndef __PHONERINGINGTONESRV_H
#define __PHONERINGINGTONESRV_H

// INCLUDES
#include <f32file.h>
#include <Profile.hrh>
#include "mphoneringingtonesrv.h"

// CONSTANTS
_LIT( KRingingToneServerName, "PhoneRingingToneSrv" );
const TInt KRingingToneSrvVersionMajor = 1;
const TInt KRingingToneSrvVersionMinor = 0;
const TInt KRingingToneSrvVersionBuild = 0;

// FORWARD DECLARATIONS
class CPhoneRingingToneSrvPlayerAO;

// CLASS DECLARATION
class CPhoneRingingToneServer : public CServer2, 
                                public MPhoneRingingToneServer
    {
public:
    
    /**
    * Two-phased constructor.
    * @return new instance.
    */
    static CPhoneRingingToneServer* NewLC();

    /**
    * Destructor.
    */
    ~CPhoneRingingToneServer();

    /**
    * Start session.
    */
    TInt StartSession();

    /**
    * Close session.
    */
    void CloseSession();

    /**
    * Thread entry function.
    */
    static TInt ThreadEntryFunc( TAny* aPtr );

    /**
    * From CServer, create a new session.
    * @param aVersion It is the version of the client api.
    * @param aMessage Connect message from Client.
    * @return Returns a new session.
    */          
    CSession2* NewSessionL( const TVersion& aVersion,  const RMessage2& aMessage ) const;

private:
    /**
    * C++ default constructor.
    */
    CPhoneRingingToneServer();

    /**
    * Symbian 2nd phase constructor.
    */
    void ConstructL();

public:
    // Ringing tone settings
    
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
    */
    void SetRingingToneL( const RMessage2& aMessage );
    
    /**
    * Sets default ringing tone.
    * @param aMessage Message from Client.
    */
    void SetDefaultRingingToneL( const RMessage2& aMessage );

    // Playback control
    
    /**
    * Play audio ring tone. 
    * @return True if the client message should not be completed.
    * @param aMessage Message from Client.
    */
    TBool PlayAudioRingTone( const RMessage2& aMessage );

    /**
    * Play TTS tone simultaneously with Personal or Default tones.
    * This method does not command phone to play TTS immediately. Instead 
    * it registers a request to play TTS, when Personal or default tones 
    * are played.
    * @param aMessage Message from Client.
    */
    void PlayTtsTone( const RMessage2& aMessage );

    /**
    * Play default tone.
    * @param aMessage Message from Client.
    */
    void PlayDefaultTone( const RMessage2& aMessage );

    /**
    * Play backup tone.
    * @param aMessage Message from Client.
    */
    void PlayBackupTone( const RMessage2& aMessage );

    /**
    * Play silent tone. No_sound.wav will be played. If vibration alert
    * is ON, it will vibrate.
    */
    void PlaySilentTone();

    /**
    * Playing the Beep once.
    * @param aMessage Message from Client.
    */
    void BeepOnce( const RMessage2& aMessage );

    /**
    * Play unsecure VoIP tone.
    * Play tone if secure call fails and call is changed to unsecure call.
    */
    void PlayUnsecureVoIPTone();

    /**
    * Mutes ringing tone.
    */
    void MuteRingingTone();

    /**
    * Stops playing the ringing tone.
    * Method does not do anything if ringing tone is not playing.
    */
    void StopPlaying();

private:

    /**
    * Runs server.
    */
    static void RunServerL();

private:
    // File server connection
    RFs iFs;

    // Indicates is session already in use
    TBool iSessionInUse;

    // Player active object
    CPhoneRingingToneSrvPlayerAO* iPlayer;
    };

#endif
