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
* Description:  Call Header
*
*/


// INCLUDE FILES
#include    "BMCallHeader.h"


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CBubbleCallHeader::CBubbleCallHeader
// Default constructor
// 
// ---------------------------------------------------------------------------
//
CBubbleCallHeader::CBubbleCallHeader()
    {
    }


// ---------------------------------------------------------------------------
// CBubbleCallHeader::NewL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
CBubbleCallHeader* CBubbleCallHeader::NewL( 
                                 const CBubbleManager::TBubbleId& aBubbleId )
    {
    CBubbleCallHeader* self = new( ELeave )CBubbleCallHeader( );    
    CleanupStack::PushL( self );
    self->ConstructL( aBubbleId );
    CleanupStack::Pop( );    // self
    return self;
    }


// ---------------------------------------------------------------------------
// CBubbleCallHeader::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CBubbleCallHeader::ConstructL( 
    const CBubbleManager::TBubbleId& aBubbleId )
    {
    CBubbleHeader::ConstructL( aBubbleId ); // call to parent's ConstructL
    Reset( );
    }

// ---------------------------------------------------------------------------
// CBubbleCallHeader::~CBubbleCallHeader
// Destructor
// 
// ---------------------------------------------------------------------------
//
CBubbleCallHeader::~CBubbleCallHeader()
    {
    }

// ---------------------------------------------------------------------------
// CBubbleCallHeader::IsUsed
// 
// 
// ---------------------------------------------------------------------------
//
TBool CBubbleCallHeader::IsUsed( ) const
    {
    return iIsUsed;
    }

// ---------------------------------------------------------------------------
// CBubbleCallHeader::SetIsUsed
// 
// 
// ---------------------------------------------------------------------------
//
void CBubbleCallHeader::SetIsUsed( const TBool& aIsUsed )
    {
    iIsUsed = aIsUsed;
    if ( !iIsUsed )
        {
        Reset( );
        }
    }

// ---------------------------------------------------------------------------
// CBubbleCallHeader::IsConference
// 
// 
// ---------------------------------------------------------------------------
//
TBool CBubbleCallHeader::IsConference() const
    {
    return EFalse; // This is for normal call header.
    }

// ---------------------------------------------------------------------------
// CBubbleCallHeader::Reset
//
// 
// ---------------------------------------------------------------------------
//
void CBubbleCallHeader::Reset()
    {
    iIsUsed = EFalse;
    ResetParent( ); // call to parent's reset
    }

// End of File
