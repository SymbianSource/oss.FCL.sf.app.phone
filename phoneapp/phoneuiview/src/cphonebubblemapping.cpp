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
* Description: Implementation of CPhoneBubbleMapping class.
*
*/


// INCLUDE FILES
#include    "cphonebubblemapping.h"
#include    "pevirtualengine.h"
#include    "phoneui.pan"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPhoneBubbleMapping::CPhoneBubbleMapping
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneBubbleMapping::CPhoneBubbleMapping()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleMapping::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneBubbleMapping::ConstructL( TInt aMaximumAmount )
    {
    iMappingReserve = aMaximumAmount;
    iMapping = new ( ELeave ) CMappingArray( aMaximumAmount );
    iMapping->SetReserveL( aMaximumAmount );
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleMapping::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneBubbleMapping* CPhoneBubbleMapping::NewL(
    TInt aMaximumAmount )
    {
    CPhoneBubbleMapping* self = 
        new ( ELeave ) CPhoneBubbleMapping;
    
    CleanupStack::PushL( self );
    self->ConstructL( aMaximumAmount );
    CleanupStack::Pop( self );

    return self;
    }

CPhoneBubbleMapping::~CPhoneBubbleMapping()
    {
    delete iMapping;
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::FindBubbleByCall
// ---------------------------------------------------------
//
TBool CPhoneBubbleMapping::FindBubbleByCall( 
    TInt aCallId,
    CBubbleManager::TBubbleId& aId ) const
    {
    const TInt count = iMapping->Count();
    TInt index;
    TBool result = EFalse;

    for ( index = 0; index < count; index++ )
        {
        TMappingItem& item = iMapping->At( index );

        if ( aCallId == item.iCallId )
            {    
            result = ETrue;
            aId = item.iBubbleId;
            break;
            }

        }

    return result;
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::FindCallByBubble
// ---------------------------------------------------------
//       
TBool CPhoneBubbleMapping::FindCallByBubble(
    CBubbleManager::TBubbleId aId,
    TInt& aCallId ) const
    {
    const TInt count = iMapping->Count();
    TInt index;
    TBool result = EFalse;

    for ( index = 0; index < count; index++ )
        {
        TMappingItem& item = iMapping->At( index );

        if ( aId == item.iBubbleId )
            {    
            result = ETrue;
            aCallId = item.iCallId;
            break;
            }
        }

    return result;
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::FindThumbnailByCall
// ---------------------------------------------------------
//       
const CFbsBitmap* CPhoneBubbleMapping::FindThumbnailByCall( 
    TInt aCallId ) const
    {
    TInt index;
    const CFbsBitmap* thumbnail = NULL;

    if ( FindIndexByCall( aCallId, index ) )
        {
        thumbnail = ThumbnailAt( index );
        }

    return thumbnail;
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::AddToMappingL
// ---------------------------------------------------------
//       
void CPhoneBubbleMapping::AddToMappingL( 
    CBubbleManager::TBubbleId aId, 
    TInt aCallId,
    CBubbleManager::TPhoneCallState aCallState,
    CFbsBitmap* aThumbnail )
    {
    CBubbleManager::TBubbleId bubbleId;

    if ( !FindBubbleByCall( aCallId, bubbleId ) )
        {
        TMappingItem item;
        item.iBubbleId = aId;
        item.iCallId = aCallId;
        item.iThumbnail = aThumbnail;
        item.iCallState = aCallState;

        if ( iMapping->Count() < iMappingReserve )
            {
            iMapping->AppendL( item ); // Doesn't leave.
            return;
            }
        }
    Panic( EPhoneViewInvariant );
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::RemoveFromMapping
// ---------------------------------------------------------
//       
void CPhoneBubbleMapping::RemoveFromMapping( 
    TInt aCallId )
    {
    const TInt count = iMapping->Count();
    TInt index;

    for ( index = 0; index < count; index++ )
        {
        TMappingItem& item = iMapping->At( index );

        if ( aCallId == item.iCallId )
            {
            iMapping->Delete( index );
            iMapping->Compress();
            return;
            }
        }
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::ItemCount
// ---------------------------------------------------------
//       
TInt CPhoneBubbleMapping::ItemCount() const
    {
    return iMapping->Count();
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::BubbleAt
// ---------------------------------------------------------
//       
CBubbleManager::TBubbleId CPhoneBubbleMapping::BubbleAt( 
    TInt aIndex ) const
    {
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < iMapping->Count(), 
        Panic( EPhoneViewIndexOutOfBounds ) );
    return iMapping->At( aIndex ).iBubbleId;
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::CallAt
// ---------------------------------------------------------
//              
TInt CPhoneBubbleMapping::CallIdAt( 
    TInt aIndex ) const
    {
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < iMapping->Count(), 
        Panic( EPhoneViewIndexOutOfBounds ) );
    return iMapping->At( aIndex ).iCallId;
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::ThumbnailAt
// ---------------------------------------------------------
//              
const CFbsBitmap* CPhoneBubbleMapping::ThumbnailAt( 
    TInt aIndex )  const
    {
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < iMapping->Count(), 
        Panic( EPhoneViewIndexOutOfBounds ) );
    return iMapping->At( aIndex ).iThumbnail;
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::SetThumbnailAt
// ---------------------------------------------------------
//              
void CPhoneBubbleMapping::SetThumbnailAt( 
    TInt aIndex, 
    CFbsBitmap* aThumbnail )  
    {
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < iMapping->Count(), 
        Panic( EPhoneViewIndexOutOfBounds ) );
        
    TMappingItem& item = iMapping->At( aIndex );
    item.iThumbnail = aThumbnail;
    }
    
// ---------------------------------------------------------
// CPhoneBubbleMapping::SetThumbnailByCall
// ---------------------------------------------------------
//              
void CPhoneBubbleMapping::SetThumbnailByCall( 
    TInt aCallId,
    CFbsBitmap* aThumbnail ) 
    {
    TInt index;

    // if a matching bubble is found
    if ( FindIndexByCall( aCallId, index ) )
        {
        SetThumbnailAt( index, aThumbnail );
        }
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::CallStateAt
// ---------------------------------------------------------
//              
CBubbleManager::TPhoneCallState CPhoneBubbleMapping::CallStateAt( 
    TInt aIndex )  const
    {
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < iMapping->Count(), 
        Panic( EPhoneViewIndexOutOfBounds ) );
    return iMapping->At( aIndex ).iCallState;
    }
    
// ---------------------------------------------------------
// CPhoneBubbleMapping::SetCallStateAt
// ---------------------------------------------------------
//              
void CPhoneBubbleMapping::SetCallStateAt( 
    TInt aIndex, 
    CBubbleManager::TPhoneCallState aCallState )  
    {
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < iMapping->Count(), 
        Panic( EPhoneViewIndexOutOfBounds ) );
    TMappingItem& item = iMapping->At( aIndex );
    item.iCallState = aCallState;
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::SetCallStateByCall
// ---------------------------------------------------------
//              
void CPhoneBubbleMapping::SetCallStateByCall( 
    TInt aCallId,
    CBubbleManager::TPhoneCallState aCallState ) 
    {
    TInt index;

    // if a matching bubble is found
    if ( FindIndexByCall( aCallId, index ) )
        {
        SetCallStateAt( index, aCallState );
        }
    }
    
// ---------------------------------------------------------
// CPhoneBubbleMapping::FindCallStateByCall
// ---------------------------------------------------------
//       
CBubbleManager::TPhoneCallState CPhoneBubbleMapping::FindCallStateByCall( 
    TInt aCallId ) const
    {
    TInt index;
    CBubbleManager::TPhoneCallState callState = CBubbleManager::ENone;

    if ( FindIndexByCall( aCallId, index ) )
        {
        callState = CallStateAt( index );
        }

    return callState;
    }
    
// ---------------------------------------------------------
// CPhoneBubbleMapping::FindCallIdByCallState
// ---------------------------------------------------------
//
TInt CPhoneBubbleMapping::FindCallIdByCallState (
    CBubbleManager::TPhoneCallState aCallState ) 
    {
    const TInt count = iMapping->Count();
    TInt index;

    for ( index = 0; index < count; index++ )
        {
        if ( CallStateAt( index ) == aCallState )
            {
            return CallIdAt( index );
            }
        }
    return KErrNotFound; // Not found CallId by this callState
    }

// ---------------------------------------------------------
// CPhoneBubbleMapping::FindIndexByCall
// ---------------------------------------------------------
//              
TBool CPhoneBubbleMapping::FindIndexByCall( 
    TInt aCallId,
    TInt& aIndex ) const
    {
    TBool result = EFalse;

    for ( aIndex = 0; aIndex < iMapping->Count(); aIndex ++ )
        {
        TMappingItem& item = iMapping->At( aIndex );

        if ( aCallId == item.iCallId )
            {
            // Horray, we found it
            result = ETrue;
            break;
            }
        }    

    return result;
    }

//  End of File  
