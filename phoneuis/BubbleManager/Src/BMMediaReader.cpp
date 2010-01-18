/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base class for media readers.
*
*/


#include "BMMediaReader.h"
#include "BMMediaReaderObserver.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CBubbleMediaReader::CBubbleMediaReader()
    {
    
    }    


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleMediaReader::~CBubbleMediaReader()
    {
    }

// ---------------------------------------------------------------------------
// HandleReadingComplete
// ---------------------------------------------------------------------------
//
void CBubbleMediaReader::HandleReadingComplete()
    {
    if ( iObserver )
        {
        iObserver->FrameBufferDataChanged();            
        }
    }

// ---------------------------------------------------------------------------
// HandleReadingError
// ---------------------------------------------------------------------------
//
void CBubbleMediaReader::HandleReadingError( TInt aError )
    {
    if ( iObserver )
        {
        iObserver->ReaderError( aError );            
        }
    }

// ---------------------------------------------------------------------------
// FrameBuffer
// ---------------------------------------------------------------------------
//    
const CFbsBitmap* CBubbleMediaReader::FrameBuffer() const 
    { 
    return iFrameBuffer;
    }

// ---------------------------------------------------------------------------
// Mask
// ---------------------------------------------------------------------------
//    
const CFbsBitmap* CBubbleMediaReader::Mask() const
    { 
    return iMask;
    }
    

