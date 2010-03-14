/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Defines interface for the observer of thumbnail loader.
*
*/


#ifndef MPHCNTTHUMBNAILLOADEROBSERVER_H
#define MPHCNTTHUMBNAILLOADEROBSERVER_H

// INCLUDES
#include    <e32base.h>
#include    "cphcntthumbnailloader.h"

// FORWARD DECLARATIONS
class CFbsBitmap;

// CLASS DECLARATION

/**
*  It defines interface for the observer of thumbnail loader.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
class MPhCntThumbnailLoaderObserver
    {
    public: // New functions

        /**
        * It is called when thumbnail loading has been completed.
        *
        * Ownership of the bitmap is transferred to the observer.
        *
        * @param aId id, never KNullId.
        * @param aResult error code.
        * @param aBitmap bitmap.
        */
        virtual void MpctloThumbnailLoaded(
            CPhCntThumbnailLoader::TThumbnailId aId,
            TInt aResult, 
            CFbsBitmap* aBitmap ) = 0;
       
    };

#endif      // MPHCNTTHUMBNAILLOADEROBSERVER_H
            
// End of File
