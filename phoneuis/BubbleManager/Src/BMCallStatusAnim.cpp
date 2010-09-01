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
* Description:  Call status animation
*
*/

#include "BMCallStatusAnim.h"
#include "BMBubbleImageManager.h"
#include "BMBubbleHeader.h"

#include <eikimage.h>

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CBubbleCallStatusAnim::CBubbleCallStatusAnim( 
    CBubbleImageManager& aImageManager ) :
    iImageManager( aImageManager )
    {
    }


// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CBubbleCallStatusAnim::ConstructL()
    {
    CTelBubbleAnim::ConstructL( 500 );
    }

// ---------------------------------------------------------------------------
// ReadBubbleHeader
// ---------------------------------------------------------------------------
//
void CBubbleCallStatusAnim::ReadBubbleHeader( const CBubbleHeader& aHeader )
    {
    iCallFlags = aHeader.CallFlags();
    }

// ---------------------------------------------------------------------------
// SetCallFlags
// ---------------------------------------------------------------------------
//
void CBubbleCallStatusAnim::SetContainerWindowL( const CCoeControl& aControl )
    {
    CTelBubbleAnim::SetContainerWindowL( aControl );
    }

// ---------------------------------------------------------------------------
// MakeVisible
// ---------------------------------------------------------------------------
//
void CBubbleCallStatusAnim::MakeVisible(TBool aVisible)
    {
    CTelBubbleAnim::MakeVisible( aVisible );
    
    if ( aVisible )
        {
        TRAP_IGNORE( SetAnimationContentL() );
        }
    else
        {
        StopAnimation();
        Reset();
        }
    }

// ---------------------------------------------------------------------------
// SetAnimationContentL
// ---------------------------------------------------------------------------
//
void CBubbleCallStatusAnim::SetAnimationContentL()
    {
    }

// End of file
