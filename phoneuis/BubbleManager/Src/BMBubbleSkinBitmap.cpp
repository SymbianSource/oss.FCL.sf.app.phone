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
* Description:  Bubble skinned bitmap.
*
*/


// INCLUDE FILES
#include    "BMBubbleSkinBitmap.h"
#include    <fbs.h>

// CONSTANTS

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CBubbleSkinBitmap::CBubbleSkinBitmap()
// Default constructor
// 
// ---------------------------------------------------------------------------
//
CBubbleSkinBitmap::CBubbleSkinBitmap() /* : iItemId( KAknsIIDNone ) */
    {
    }

// ---------------------------------------------------------------------------
// CBubbleSkinBitmap::~CBubbleSkinBitmap()
// Destructor
// 
// ---------------------------------------------------------------------------
//
CBubbleSkinBitmap::~CBubbleSkinBitmap()
    {
    delete iSkinnedBitmap;
    delete iSkinnedMask;
    }

/*
// ---------------------------------------------------------------------------
// CBubbleSkinBitmap::SetSkinnedBitmap
// ---------------------------------------------------------------------------
//
void CBubbleSkinBitmap::SetBitmapSkinItemID( const TAknsItemID& aItemID )
    {
    iItemId = aItemID;
    }

// ---------------------------------------------------------------------------
// CBubbleSkinBitmap::SetSkinnedBitmap
// ---------------------------------------------------------------------------
//
TAknsItemID CBubbleSkinBitmap::BitmapSkinItemID() const
    {
    return iItemId;
    }
*/

// ---------------------------------------------------------------------------
// CBubbleSkinBitmap::SetSkinnedBitmap
// ---------------------------------------------------------------------------
//
void CBubbleSkinBitmap::SetSkinnedBitmap( CFbsBitmap* aSkinnedBitmap )
    {
    iSkinnedBitmap = aSkinnedBitmap;
    }

// ---------------------------------------------------------------------------
// CBubbleSkinBitmap::SkinnedBitmap
// ---------------------------------------------------------------------------
//
CFbsBitmap* CBubbleSkinBitmap::SkinnedBitmap() const
    {
    return iSkinnedBitmap;
    }

// ---------------------------------------------------------------------------
// CBubbleSkinBitmap::SetSkinnedMask
// ---------------------------------------------------------------------------
//
void CBubbleSkinBitmap::SetSkinnedMask( CFbsBitmap* aSkinnedMask )
    {
    iSkinnedMask = aSkinnedMask;
    }

// ---------------------------------------------------------------------------
// CBubbleSkinBitmap::SkinnedBitmapMask
// ---------------------------------------------------------------------------
//
CFbsBitmap* CBubbleSkinBitmap::SkinnedMask() const
    {
    return iSkinnedMask;
    }

// ---------------------------------------------------------------------------
// CBubbleSkinBitmap::SetBitmapIndex
// ---------------------------------------------------------------------------
//
void CBubbleSkinBitmap::SetBitmapIndex( const TInt& aBitmapIndex )
    {
    iBitmapIndex = aBitmapIndex;
    }

// ---------------------------------------------------------------------------
// CBubbleSkinBitmap::BitmapIndex
// ---------------------------------------------------------------------------
//
TInt CBubbleSkinBitmap::BitmapIndex() const
    {
    return iBitmapIndex;
    }

// End of File
