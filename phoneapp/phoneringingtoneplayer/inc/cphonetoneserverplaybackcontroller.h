/*
* Copyright (c) 2010-2010 Nokia Corporation and/or its subsidiary(-ies). 
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

#ifndef CPHONETONESERVERPLAYBACKCONTROLLER_H
#define CPHONETONESERVERPLAYBACKCONTROLLER_H

#include <e32base.h>
#include <mdaaudiosampleplayer.h>
#include <mdaaudiotoneplayer.h>


class MPhoneToneServer;


NONSHARABLE_CLASS( CPhoneToneServerPlaybackController ) : public CTimer,
    private MMdaAudioPlayerCallback,
    private MMdaAudioToneObserver
    {

public:

    static CPhoneToneServerPlaybackController* NewL();

    ~CPhoneToneServerPlaybackController();

    /**
    * Plays tone set by control panel.
    *
    * @param aName file path of ringingtone
    * @param aVolume volume of ringingtone
    * @param aType type of ringtone e.g. ringing or silent    
    */     
    void PlayNormalToneL( const TFileName& aName, 
                          TInt aVolume, 
                          TProfileRingingType aType );
    
    /**
    * Plays silent tone.
    */     
    void PlaySilentTone();
        
    /**
    * Stops playing.
    */     
    void StopTone();

private:    
    
    /**
    * Plays profile based tone.
    *    
    */     
    void PlayProfileTone();
    
    /**
    * Plays default tone
    *    
    */      
    void PlayDefaultTone();
    
    /**
    * Converts volume level  
    *
    * @param aMaxVolume player's maximum volume
    * @param aVolume requested volume level.
    */     
    TInt ConvertVolume( TInt aMaxVolume, TInt aVolume );
    
    /**
    * Stops and deletes last used player.
    *    
    */     
    void StopAndDeleteProfilePlayer();
    

    /**
    * Checks that file uses rng file type
    *
    * @param aFile ringingtone file name
    * @return TBool true when file uses rng file type.
    */     
    TBool IsRngL( const TDesC& aFile );   
    
private:

    CPhoneToneServerPlaybackController();

    void ConstructL();
    
private:

    /**
     * @see MMdaAudioPlayerCallback
     */
    void MapcInitComplete(
        TInt aError, 
        const TTimeIntervalMicroSeconds& aDuration );

    /**
     * @see MMdaAudioPlayerCallback
     */
    void MapcPlayComplete( TInt aError );

    /**
     * @see MMdaAudioToneObserver
     */
    void MatoPrepareComplete(TInt aError );

    /**
     * @see MMdaAudioToneObserver
     */
    void MatoPlayComplete(TInt aError);


private: 

    /**
     * @see CActive
     */
    virtual void RunL();
    
    
private:   

    // Owned
    CMdaAudioPlayerUtility* iDefaultPlayer;
        
    // Owned
    CMdaAudioPlayerUtility* iProfileSamplePlayer;
        
    // Owned
    CMdaAudioToneUtility* iProfileTonePlayer;
    
    // Owned
    CMdaAudioToneUtility* iSilentPlayer;
    
    TProfileRingingType iRingingType;
        
    TBool iDefaultPlayerInitialized;
    
    TInt iVolume;
  
    };

#endif // CPHONETONESERVERPLAYBACKCONTROLLER_H

// End of file
