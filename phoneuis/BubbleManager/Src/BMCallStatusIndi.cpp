/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call status indicator
*
*/

#include "BMCallStatusIndi.h"
#include "BMBubbleImageManager.h"
#include "BMBubbleHeader.h"

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleCallStatusIndi::CBubbleCallStatusIndi( 
    CBubbleImageManager& aImageManager ) :
    iImageManager( aImageManager )
    {
    }

// ---------------------------------------------------------------------------
// ReadBubbleHeader
// ---------------------------------------------------------------------------
//
void CBubbleCallStatusIndi::ReadBubbleHeader( const CBubbleHeader& aHeader )
    {
    iCallState = aHeader.CallState();
    iCallFlags = aHeader.CallFlags();
    }

// ---------------------------------------------------------------------------
// MakeVisible
// ---------------------------------------------------------------------------
//
void CBubbleCallStatusIndi::MakeVisible(TBool aVisible)
    {
    CTelBubbleImage::MakeVisible( aVisible );
    
    if ( aVisible )
        {
        SetImageAndMask();
        }
    else
        {
        SetPicture( NULL, NULL );
        }
    };

// ---------------------------------------------------------------------------
// SetImageAndMask
// ---------------------------------------------------------------------------
//
void CBubbleCallStatusIndi::SetImageAndMask()
    {
    SetPicture( NULL, NULL );
    }

// End of file
