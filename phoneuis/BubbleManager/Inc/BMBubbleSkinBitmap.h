/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class handles skinned bitmaps
*
*/



#ifndef BUBBLESKINBITMAP_H
#define BUBBLESKINBITMAP_H

// INCLUDES
#include    <e32base.h>
#include    <AknsItemID.h>
#include    <AknsConstants.h>

// FORWARD DECLARATIONS
class CEikImage;   
class CFbsBitmap;


// CLASS DECLARATION

/**
*  CBubbleSkinBitmap class
*  This class stores bitmap information.
*
*  @since Series60_2.8
*/
class CBubbleSkinBitmap : public CBase
    {
    public: // Constructors and destructor

        /**
        * Constructor.
        */
        CBubbleSkinBitmap();

        /**
        * Destructor.
        */
        ~CBubbleSkinBitmap();

    public: // New functions

        /**
        * Set skinned bitmap.
        * @param aSkinnedBitmap Pointer to the skinned bitmap.
        * @return None.
        */
        void SetSkinnedBitmap( CFbsBitmap* aSkinnedBitmap );

        /**
        * Get skinned bitmap.
        * @return the pointer to the skinned bitmap.
        */
        CFbsBitmap* SkinnedBitmap() const;
        
        /**
        * Set skinned bitmap mask.
        * @param aSkinnedBitmapMask Pointer to the skinned bitmap mask.
        * @return None.
        */
        void SetSkinnedMask( CFbsBitmap* aSkinnedMask );

        /**
        * Get skinned bitmap mask.
        * @return the pointer to the skinned bitmap mask.
        */
        CFbsBitmap* SkinnedMask() const;

        /**
        * Set bitmap skin item ID.
        * @param aItemID Bitmap skin item ID.
        * @return None.
        */
        // void SetBitmapSkinItemID( const TAknsItemID& aItemID );

        /**
        * Get bitmap skin item ID.
        * @return bitmap skin item ID.
        */
        // TAknsItemID BitmapSkinItemID() const;

        /**
        * Set bitmap index in the mbm file.
        * @param aBitmapIndex Bitmap index.
        * @return None.
        */
        void SetBitmapIndex( const TInt& aBitmapIndex );

        /**
        * Get bitmap index in the mbm file.        
        * @return bitmap index.
        */
        TInt BitmapIndex() const;

    private: // Data     

        // Reference bitmap skin item ID.
        // TAknsItemID iItemId;

        // Reference bitmap index in mbm file.
        TInt iBitmapIndex;

        // Pointer to the skinned bitmap.
        CFbsBitmap* iSkinnedBitmap;

        // Pointer to the skinned bitmap mask.
        CFbsBitmap* iSkinnedMask;               
    };

#endif // BUBBLESKINBITMAP_H

// End of File
