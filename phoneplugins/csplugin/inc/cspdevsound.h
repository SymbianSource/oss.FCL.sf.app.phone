/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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

#ifndef CSPDEVSOUND_H
#define CSPDEVSOUND_H

#include <sounddevice.h>

class MCSPDevSoundObserver;

/**
 *  Wrapper for CMMFDevSound
 *
 */
NONSHARABLE_CLASS( CSPDevSound ): 
    public CBase, 
    public MDevSoundObserver
    {
public:

    /**
    * Destructor.
    */
    virtual ~CSPDevSound();
    
    /**
     * Activates the dev sound stream. MCSPDevSoundObserver methods are called 
     * when activation goes ok or fails. If the stream is already active or 
     * activating then nothing is done.
     */
    virtual void Activate();
    
    /**
     * Deactivates the devsound stream. If stream is not active or
     * activation is not ongoing then nothing is done. 
     */     
    virtual void Deactivate();
    
    /**
     * Indicates that activation is ongoing,
     * but the result of the activation is still unknown.
     * @return ETrue - activation ongoing, EFalse - activation not ongoing.
     */
    TBool IsActivationOngoing() const;
    
    /**
     * Determines wether the devsound stream is active or not.
     * @return ETrue - Stream is active, EFalse - Stream not active.
     */
    TBool IsActive() const;
    
    CMMFDevSound& DevSound();

// from base class MDevSoundObserver

    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void InitializeComplete( TInt aError );  

    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void BufferToBeFilled( CMMFBuffer* aBuffer ); 

    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void PlayError( TInt aError ); 
        
    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void ToneFinished( TInt aError );         
        
    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void BufferToBeEmptied( CMMFBuffer* aBuffer ); 
        
    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void RecordError( TInt aError );
        
    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void ConvertError( TInt aError ); 
        
    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void DeviceMessage( TUid aMessageType, const TDesC8& aMsg );  
    
protected:

    CSPDevSound( MCSPDevSoundObserver& aObserver );

    void ConstructL( 
        TMMFState aMode, TUint aAudioPreference, TUint aAudioPriority );
        
private:

    /**
     * Derived class has to activate the stream here.
     */
    virtual void DoActivateL() = 0;
    
protected: // Data

    /**
     * Indication if device is activated. Derived class has to update this.
     */
    TBool iActive;
    
    /**
     * Indication if activation is ongoing.
     * Derived class has to update this.
     */
    TBool iActivationOngoing;
    
    /**
     * Devsound instance
     * Own.  
     */
    CMMFDevSound* iDevSound;
      
    /**
     * Observer for successfull activation.
     * Not own.
     */
    MCSPDevSoundObserver& iObserver;
    
    /**
     * Audio priority settings for Devsound. 
     */
    TMMFPrioritySettings iPrioritySettings;
    

    };

#endif // CSPDEVSOUND_H
