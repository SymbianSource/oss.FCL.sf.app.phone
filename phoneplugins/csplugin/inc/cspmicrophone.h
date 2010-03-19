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
* Description:  Mic stream to DevSound.
*
*/


#ifndef CSPMICROPHONE_H
#define CSPMICROPHONE_H

#include "cspdevsound.h" 

/**
 *  Provides mic functionality.
 *
 */
NONSHARABLE_CLASS( CSPMicrophone ): public CSPDevSound
    {

public:

    static CSPMicrophone* NewL( MCSPDevSoundObserver& aObserver );

    virtual ~CSPMicrophone();

    /**
     * Determines mic mute state.
     *
    *
     * @return ETrue - mic is muted, EFalse - mic is not muted.
     */
    TBool IsMuted();
    
    /**
     * Set mic muted.
     *
    *
     */
    void SetMuted();
    
    /**
     * Set mic unmuted.
     *
    *
     */
    void SetUnmuted();
   
// from base class MDevSoundObserver

    /**
     * From MDevSoundObserver
     * Notification from Devsound that stream (mic) is
     * activated successfully.
     *
    *
     */
    void BufferToBeEmptied( CMMFBuffer* aBuffer ); 
    
    /**
     * From MDevSoundObserver
     * Notification from devsound that downstream(mic) activation
     * feiled.
     *
    *
     */
    void RecordError( TInt aError );
    
private:
    
// from base class MCSPDevSound
    
    /**
     * From MCSPDevSound.
     * Tries to activate the mic stream.
     * 
    *
     */
    void DoActivateL();

protected:

    CSPMicrophone( MCSPDevSoundObserver& aObserver );

    void ConstructL();

   };

#endif // CSPMICROPHONE_H
