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
* Description:  Provides speaker functionality
*
*/


#ifndef CSPSPEAKER_H
#define CSPSPEAKER_H

#include "cspdevsound.h"

class MCSPDevSoundObserver;

/**
 *  Provides speaker functionality.
 *
 */
NONSHARABLE_CLASS( CSPSpeaker ): public CSPDevSound
    {

public:

    static CSPSpeaker* NewL( MCSPDevSoundObserver& aObserver );

    virtual ~CSPSpeaker();

    /** 
     * Sets volume.
     * @param aVolume Volume.     
     */
    void SetVolume( TInt aVolume );
    
    /**
     * Gets volume.
     * @return Current volume.
     */
    TInt Volume();

// from base class MDevSoundObserver

    /**
     * From MDevSoundObserver.
     * Indication from devsound that stream (speaker) has been activated
     * successfully.
     */
    void BufferToBeFilled( CMMFBuffer* /*aBuffer*/ );

    /**
     * From MDevSoundObserver.
     * Indication from devsound that activation of stream( speaker)
     * failed.
     */
    void PlayError( TInt aErrorCode );

private:
        
    /**
     * From CSPDevSound.
     * Tries to activate the mic stream.
     */
    void DoActivateL();

protected:

    CSPSpeaker( MCSPDevSoundObserver& aObserver );

    void ConstructL();

    };

#endif // CSPSPEAKER_H
