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
* Description: 
*     ???
*
*/


#ifndef MMBIMAGEREADEROBSERVER_H
#define MMBIMAGEREADEROBSERVER_H

//  INCLUDES
#include <e32def.h>
#include "BMImageReader.h"

// FORWARD DECLARATIONS
class CBubbleImageReader;

// CLASS DECLARATION

/**
 * Observer interface for CBMThumbnailReader.
 */
class MBubbleImageReaderObserver
  {
    public:
        /**
         * Called by CBubbleImageReader when image read is complete.
         *
         * @param aReader   the completed reader.
         * @param aBitmap   the read image.
         */
        virtual void ImageReadComplete
            ( CBubbleImageReader& aReader, CFbsBitmap* aBitmap ) = 0;

        /**
         * Called by CBubbleImageReader if image reading fails.
         *
         * @param aReader   the failed reader.
         * @param aError    error code of the failure.
         */
        virtual void ImageReadFailed( CBubbleImageReader& aReader, TInt aError ) = 0;

        /**
         * Called by CBubbleImageReader when image open is complete.
         * After this event CBubbleImageReader functions FrameCount() and 
         * FrameInfo() can be called.
         * Default implementation is empty as most clients are interested of 
         * ImageReadComplete/Failed events only.
         *
         * @param aReader   the completed reader.
         */
        virtual void ImageOpenComplete( CBubbleImageReader& aReader ) = 0;
    };

#endif // MMBIMAGEREADEROBSERVER_H

// End of File
