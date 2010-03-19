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
* Description:  Starts and stops audio streams.
*
*/


#ifndef CSPAUDIOSTREAMS_H
#define CSPAUDIOSTREAMS_H


#include <e32base.h>
#include <TelephonyAudioRouting.h>
#include "MTelephonyAudioRoutingObserver.h"
#include "mcspdevsoundobserver.h"
#include "mcsptimerobserver.h"

class MCSPDevSoundFactory;
class CSPTimer;
class CSPMicrophone;
class CSPSpeaker;
class MCSPAudioStreamObserver;
class CSPTimer;


/**
 *  Starts and stops audio streams.
 *  If the activation fails then there is a retry timer which
 *  will try the activation later.
 */
NONSHARABLE_CLASS( CSPAudioStreams ) : 
    public CBase, 
    public MCSPDevSoundObserver,
    public MCSPTimerObserver,
    public MTelephonyAudioRoutingObserver
    {

public:

    /**
    * Two-phased constructing.
    */
    static CSPAudioStreams* NewL( );

    /**
    * Two-phased constructing.
    */    
    static CSPAudioStreams* NewLC( );

    /**
    * C++ default destructor.
    */
    virtual ~CSPAudioStreams();

    /**
    * Starts audio streams ie inits mic and speaker.
    */
    void StartStreams();
    
    /**     
    * Stops audio streams ie shuts down mic and speaker.
    */
    void StopStreams();
    
    /**
    * Sets volume for ear. 
    */
    void VolumeChangedEar( TInt aVolume );
    
    /**
    * Sets volume for loudspeaker. 
    */
    void VolumeChangedLoudspeaker( TInt aVolume );
    
    /**
    * Stores volume levels.
    */
    void StoreVolumes(TInt aVolumeEar, TInt aVolumeLoudspeaker);

    /**
    * Set unmuted.
    */    
    void SetUnmuted();

    /**
    * Set muted.
    */
    void SetMuted();

    /**
    * Gives access to CSPMicrophone instance. Ownership not given.
    * @return MCSPMic instance.
    */
    CSPMicrophone& Mic() const;
    
    /**
    * Gives access to CSPSpeaker instance. Ownership not given.
    * @return MCSPSpeaker instance.
    */
    CSPSpeaker& Speaker() const;
    
    /**
    * Set observer for audio stream events.
    * @param aObserver
    */
    void SetAudioStreamObserver( MCSPAudioStreamObserver& aObserver );
    
// from base class MCSPDevSoundObserver

    /**
    * From MCSPDevSoundObserver
    * Notification that Mic has been activated successfully
    */
    void MicActivatedSuccessfully();
    
    /**
    * From MCSPDevSoundObserver
    * Notification that speaker has been activated successfully
    */   
    void SpeakerActivatedSuccessfully();
    
    /**
    * From MCSPDevSoundObserver
    * Notification that Mic initialisation failed.
    */
    void MicActivationFailed();
    
    /**
    * From MCSPDevSoundObserver
    * Notification that Speaker activation failed.
    */
    void SpeakerActivationFailed();
    
// from base class MCSPTimerObserver

    /**
    * From MCSPTimerObserver.
    * Notify from CSPTimer that timeout passed.
    */
    void TimerEvent();
    
protected:  // Functions from base classes  

// from base class MTelephonyAudioRoutingObserver
        
    /**
    * @see MTelephonyAudioRoutingObserver.
    */
    void AvailableOutputsChanged( CTelephonyAudioRouting& aTelephonyAudioRouting );
            
    /**
    * @see MTelephonyAudioRoutingObserver.
    */
    void OutputChanged( CTelephonyAudioRouting& aTelephonyAudioRouting);
    
    /**
    * @see MTelephonyAudioRoutingObserver.
    */
    void SetOutputComplete( CTelephonyAudioRouting& aTelephonyAudioRouting, TInt aError);
  
private:
    
    /** 
    * C++ default constructor. 
    */  
    CSPAudioStreams( );

    void ConstructL( );
    
    static TInt TimerCallBack( TAny* aThisPtr );
    
    void AudioStreamsStarted();
    
    void StartTimer();
    
    void StartMicAndSpeaker();
    
    TBool IsMicAndSpeakerStarted();
    
    void ApplyVolume(TInt aVolumeEar, TInt aVolumeLoudspeaker);

private: // data

    /**
    * Mic.
    * own.
    */     
    CSPMicrophone* iMic;
    
    /**
    * Speaker.
    * Own.
    */
    CSPSpeaker* iSpeaker;
     
    /**
    * Timer for retrying the activation of streams (mic & speaker) if
    * there was a failure in activation.
    * Own.
    */
    CSPTimer* iTimer;
    
    /**
    * Timeout value used with timer. Timeout value doubles every retry to
    * activate the streams.
    */
    TInt iTimeout;
    
    /**
    * Audio output for checking if loadspeaker or ear volume should be used.
    * Own.
    */
    CTelephonyAudioRouting* iAudioRouting;
    
    /**
    *  Observer for stream events.
    * Not own.
    */
    MCSPAudioStreamObserver* iStreamObserver;
    
    /**
    *  Ear volume. 
    */    
    TInt iVolumeEar;
    
    /**
    *  Loudspeaker volume. 
    */
    TInt iVolumeLoudspeaker;
         
    };

#endif // CSPAUDIOSTREAMS_H
