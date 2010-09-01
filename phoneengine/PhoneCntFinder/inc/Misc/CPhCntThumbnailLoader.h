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
* Description:  Abstract interface for loading thumbail images.
*
*/


#ifndef CPHCNTTHUMBNAILLOADER_H
#define CPHCNTTHUMBNAILLOADER_H

// INCLUDES
#include    <e32base.h>
#include    <cntdef.h>

class CPhCntContactId;

// CONSTANTS
// MACROS
#define KPhCntThumbnailNullId 0

// CLASS DECLARATION

/**
*  It is abstract interface for loading thumbail images.
*
*  Note that it is considered that loading of thumbnail images is not critical.
*  Thus, if something fails during loading procedure, then we ignore the whole 
*  thumbnail.
*
*  However, observer will be informed of both success and failure.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCntThumbnailLoader )
    : public CBase
    {
    public: // New functions

        /**
        * It defines type for thumbnail ids.
        */
        typedef const TAny* TThumbnailId;

        /**
        * Loads thumbnail asynchronously.
        *
        * You can call this to load several images. Images are loaded in
        * FIFO order.
        *
        * @param aId id. if id is already in use, it's canceled. It will
        *            be zeroed if loading failed to the beginning.
        * @param aContactId contact id. Copies the contact id.
        * @return error code.
        */
        virtual TInt Load( TThumbnailId& aId, const CPhCntContactId& aContactId ) = 0;
        
        /**
        * Cancels loading of thumbnail with given id.
        *
        * @param aId id to cancel.
        */
        virtual void Cancel( TThumbnailId aId ) = 0;  
        
    };

#endif      // CPHCNTTHUMBNAILLOADER_H
            
// End of File
