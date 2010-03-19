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
* Description:  Observer interface for getting information on audio streams.
*
*/


#ifndef MCSPAUDIOSTREAMOBSERVER_H
#define MCSPAUDIOSTREAMOBSERVER_H

/**
 *  Observer interface for getting information of audio streams.
 *
 */
NONSHARABLE_CLASS( MCSPAudioStreamObserver )
    {

public:

    /**
     * Indication that audio streams has been started. 
     *
     */
    virtual void AudioStreamsStarted() = 0;
    
    /**
     * Indication that audio streams will be closed.
     *
     * Client can do operations affected to streams from this method,
     * but after this method the streams will be closed.
     *
     */
    virtual void AudioStreamsGoingToStop() = 0;

    };


#endif // MCSPAUDIOSTREAMOBSERVER_H
