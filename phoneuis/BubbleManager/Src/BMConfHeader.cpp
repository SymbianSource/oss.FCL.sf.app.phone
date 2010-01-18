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
* Description:  Conf Header
*
*/


// INCLUDE FILES
#include    "BMConfHeader.h"
#include    "BMCallHeader.h"
#include    "BMPanic.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CBubbleConfHeader::CBubbleConfHeader
// Default constructor
// 
// ---------------------------------------------------------------------------
//
CBubbleConfHeader::CBubbleConfHeader()
    {
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::NewL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
CBubbleConfHeader* CBubbleConfHeader::NewL( 
                                 const CBubbleManager::TBubbleId& aBubbleId )
    {
    CBubbleConfHeader* self = new( ELeave )CBubbleConfHeader( );    
    CleanupStack::PushL( self );
    self->ConstructL( aBubbleId );
    CleanupStack::Pop( );    // self
    return self;
    }


// ---------------------------------------------------------------------------
// CBubbleConfHeader::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfHeader::ConstructL( 
    const CBubbleManager::TBubbleId& aBubbleId )
    {
    CBubbleHeader::ConstructL( aBubbleId ); // call to parent's ConstructL
    
    iCalls = new ( ELeave ) 
        CArrayPtrFlat<CBubbleCallHeader>( KBubbleMaxCallsInConf );    
    iCalls->SetReserveL( KBubbleMaxCallsInConf );
    
    Reset();
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::~CBubbleConfHeader
// Destructor
// 
// ---------------------------------------------------------------------------
//
CBubbleConfHeader::~CBubbleConfHeader()
    {
    if ( iCalls )
        {
        iCalls->Reset();
        delete iCalls;
        }
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::IsConference
// 
// 
// ---------------------------------------------------------------------------
//
TBool CBubbleConfHeader::IsConference( ) const
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::Reset
// Puts everything in initial state
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfHeader::Reset()
    {
    iIsUsed = EFalse;
    iIsExpanded = EFalse;
    iSelection = KBubbleNoHighlightRow;
    
    TInt callCount = iCalls->Count();
    for ( TUint8 i = 0 ; i < callCount; i++ )
        {
        iCalls->At( i )->SetIsInConference( EFalse );
        }
    
    iCalls->Delete( 0 , callCount );
    ResetParent( ); // call to parent's Reset
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::AddRow
// Adds new call to conference
// If max calls are already set to conference function, panics
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfHeader::AddRow( CBubbleCallHeader& aCall )
    {
    __ASSERT_ALWAYS( iCalls->Count() < KBubbleMaxCallsInConf, 
                     Panic( EBMPanicTooManyCallsInConference ) );
    
    iCalls->InsertL( iCalls->Count( ) , &aCall ); //space pre-reserved
    aCall.SetIsInConference( ETrue );
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::RemoveRow
// Removes specified row from the conference call
// If BubbleId doesn't exist in conference call function panics
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfHeader::RemoveRow( const CBubbleManager::TBubbleId& aCallId )
    {
    // Search the element.
    TUint8 index = 0;
    __ASSERT_ALWAYS( iCalls->Count() > 0, 
                     Panic( EBMPanicConferenceCallEmpty ) );

    if ( iCalls->Count() == 0 )
        {
        // Own panic function may not Panic the thread.
        return;
        }

    while ( iCalls->At( index )->BubbleId() != aCallId )
        {
        index++;
        __ASSERT_ALWAYS( index != iCalls->Count(), 
                         Panic( EBMPanicBubbleIdDoesNotExist ) );
        if ( index == iCalls->Count() )
            {
            // Own panic implementation does not panic the thread in every case
            return;
            }
        }
    
    CBubbleManager::TBubbleId oldhighlightid = HighlightId();
    
    iCalls->At( index )->SetIsInConference( EFalse );
    
    iCalls->Delete( index );
    
    // move highlight to new position
    if ( oldhighlightid != aCallId )
        {
        SetHighlightId( oldhighlightid );
        }
    else if ( iSelection >= iCalls->Count( ) )
        {
        iSelection = static_cast<TUint8>( iCalls->Count() );
        }
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::GetRows
// 
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfHeader::GetRows( 
                           CArrayPtrFlat<CBubbleCallHeader>& aCalls ) const
    {
    aCalls.Delete( 0 , aCalls.Count() );
    TInt callCount = iCalls->Count();
    
    for ( TUint8 outIndex = 0; outIndex < callCount ; outIndex++ )
        {
        aCalls.InsertL( outIndex , iCalls->At( outIndex ) ); // can't leave.
        }
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::RowCount
// 
// 
// ---------------------------------------------------------------------------
//
TUint8 CBubbleConfHeader::RowCount( ) const
    {
    return static_cast<TUint8>( iCalls->Count() );
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::IsUsed
// 
// 
// ---------------------------------------------------------------------------
//
TBool CBubbleConfHeader::IsUsed( ) const
    {
    return iIsUsed;
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::SetIsUsed
// If set not used, reset the header.
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfHeader::SetIsUsed( const TBool& aIsUsed )
    {
    iIsUsed = aIsUsed;
    if ( !iIsUsed )
        {
        Reset( );
        }       
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::SetIsExpanded
//
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfHeader::SetIsExpanded( TBool aIsExpanded )
    {
    iIsExpanded = aIsExpanded;
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::IsExpanded
// 
// 
// ---------------------------------------------------------------------------
//
TBool CBubbleConfHeader::IsExpanded( ) const
    {
    return iIsExpanded;
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::SetHighlight
// 
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfHeader::SetHighlight( const CBubbleManager::TRowNumber& aRow )
    {
    __ASSERT_ALWAYS( aRow <= iCalls->Count(),
                     Panic( EBMPanicInvalidConfRowNumber ) );
    
    iSelection = aRow;
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::SetHighlightId
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfHeader::SetHighlightId( 
                                const CBubbleManager::TBubbleId& aBubbleId )
    {
    if ( aBubbleId == KBubbleInvalidId )
        {
        iSelection = KBubbleNoHighlightRow;
        return;
        }
    
    for ( TInt i = 0 ; i < iCalls->Count() ; i++ )
        {
        if ( iCalls->At(i)->BubbleId() == aBubbleId )
            {
            iSelection = static_cast<CBubbleManager::TRowNumber>( i+1 );
            return;
            }
        }
    __ASSERT_DEBUG( EFalse , Panic( EBMPanicBubbleIdDoesNotExist ) );
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::Highlight
// 
// 
// ---------------------------------------------------------------------------
//
CBubbleManager::TRowNumber CBubbleConfHeader::Highlight() const
    {
    return iSelection;
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::HighlightId
// 
// 
// ---------------------------------------------------------------------------
//
CBubbleManager::TBubbleId CBubbleConfHeader::HighlightId() const
    {
    if ( iSelection == KBubbleNoHighlightRow || iSelection > iCalls->Count() )
        {
        return KBubbleInvalidId;
        }  
    return iCalls->At( iSelection - 1 )->BubbleId();
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::MoveHighlightOneUp
// 
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfHeader::MoveHighlightOneUp()
    {
    if( iSelection == KBubbleNoHighlightRow || iSelection == 1 )
        {
        return;
        }
    
    iSelection--;
    }

// ---------------------------------------------------------------------------
// CBubbleConfHeader::MoveHighlightOneDown
// 
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfHeader::MoveHighlightOneDown()
    {
    if ( iSelection == KBubbleNoHighlightRow || 
        iSelection >= iCalls->Count() )
        {
        return;
        }
    iSelection++;
    }

// End of File
