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
* Description:  CBubbleMediaReader observer interface.
*
*/


#ifndef MBUBBLEMEDIAREADEROBSERVER_H
#define MBUBBLEMEDIAREADEROBSERVER_H

/**
 *  CBubbleMediaReader observer interface
 *
 *  @lib BubbleManager
 *  @since S60 v3.2
 */
class MBubbleMediaReaderObserver
    {

public:     
    /**
     * Image or animation frame has been read and is available 
     * in the framebuffer. Client should update the screen. 
     *
     * @since S60 v3.2
     */
    virtual void FrameBufferDataChanged() = 0;
    
    /**
     * Reports an error in reading procedure.  
     *
     * @since S60 v3.2
     * @param aError System wide error code.
     */
    virtual void ReaderError( TInt aError ) = 0;
    };

#endif // MBUBBLEMEDIAREADEROBSERVER_H
