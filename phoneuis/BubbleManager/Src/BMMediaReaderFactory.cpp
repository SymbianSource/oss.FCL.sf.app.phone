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
* Description:  Creator of media readers.
*
*/


#include "BMMediaReaderFactory.h"
#include "BMImageReader.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CreateReaderL
// ---------------------------------------------------------------------------
//
MBubbleMediaReader* BubbleMediaReaderFactory::CreateReaderL( 
    const TDesC& aFileName )
    {
    // To be done: Reader selection to be based on MIME type
    // or file extension. Now there is just one default reader
    // available. 
    return CBubbleImageReader::NewL( aFileName );  
    }


    
    

