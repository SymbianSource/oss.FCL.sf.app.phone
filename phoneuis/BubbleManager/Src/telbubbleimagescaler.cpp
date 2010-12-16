/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Ctelbubbleimagescaler implementation
*
*/

#include "telbubbleimagescaler.h"
#include "BMBubbleDebug.h"
#include <bitmaptransforms.h>

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CTelBubbleImageScaler::CTelBubbleImageScaler(
        MTelBubbleImageScalerObserver& aObserver) :
    CActive(CActive::EPriorityStandard), iObserver(aObserver), iState(EScalingIdle)    
    {
    }

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CTelBubbleImageScaler* CTelBubbleImageScaler::NewLC(
        MTelBubbleImageScalerObserver& aObserver)
    {
    CTelBubbleImageScaler* self = new (ELeave) CTelBubbleImageScaler(
            aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CTelBubbleImageScaler* CTelBubbleImageScaler::NewL(
        MTelBubbleImageScalerObserver& aObserver)
    {
    CTelBubbleImageScaler* self = CTelBubbleImageScaler::NewLC(aObserver);
    CleanupStack::Pop( self ); 
    return self;
    }

// ---------------------------------------------------------------------------
// CTelBubbleImageScaler::ConstructL
// ---------------------------------------------------------------------------
//
void CTelBubbleImageScaler::ConstructL()
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CTelBubbleImageScaler::~CTelBubbleImageScaler()
    {
    if ( iScaler )
        {
        iScaler->Cancel(); // Cancel any request, if outstanding
        }
    
    delete iScaler;
    iState = EScalingIdle;
    }

// ---------------------------------------------------------------------------
// CTelBubbleImageScaler::StartScaleL
// ---------------------------------------------------------------------------
//
void CTelBubbleImageScaler::StartScaleL(CFbsBitmap* aSrc, CFbsBitmap* aTarget)
    {
    BM_TRACE_( "CTelBubbleImageScaler::StartScaleL()" );
    if ( !iScaler )
        {
        iScaler = CBitmapScaler::NewL();
        }

    iScaledBitmap = aTarget;

    iScaler->SetQualityAlgorithm( CBitmapScaler::EMaximumQuality );
    iScaler->Scale( &iStatus, *aSrc, *aTarget, EFalse );
    iState = EScalingStarted;
    SetActive();
    }

// ---------------------------------------------------------------------------
// CTelBubbleImageScaler::DoCancel
// ---------------------------------------------------------------------------
//
void CTelBubbleImageScaler::DoCancel()
    {
    BM_TRACE_( "CTelBubbleImageScaler::DoCancel()" );
    if ( iScaler )
        {
        iScaler->Cancel();
        }
    
    iObserver.ImageScalingComplete( KErrCancel, iScaledBitmap );
    iState = EScalingDone;
    iScaledBitmap = NULL;
    }

// ---------------------------------------------------------------------------
// CTelBubbleImageScaler::RunL
// ---------------------------------------------------------------------------
//
void CTelBubbleImageScaler::RunL()
    {
    BM_TRACE_( "CTelBubbleImageScaler::RunL()" );
    iState = EScalingDone;
    TInt status = iStatus.Int();
    iObserver.ImageScalingComplete( status, iScaledBitmap );
    iScaledBitmap = NULL;
    }

// ---------------------------------------------------------------------------
// CTelBubbleImageScaler::RunError
// ---------------------------------------------------------------------------
//
TInt CTelBubbleImageScaler::RunError( TInt aError )
    {
    BM_TRACE_( "CTelBubbleImageScaler::RunError()" );
    iState = EScalingDone;
    iObserver.ImageScalingComplete( aError, iScaledBitmap );
    iScaledBitmap = NULL;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CTelBubbleImageScaler::SetState
// ---------------------------------------------------------------------------
//
void CTelBubbleImageScaler::SetState(TState aState)
    {
    BM_TRACE_1( "CTelBubbleImageScaler::SetState(): %d", aState );
    iState = aState;
    }

// ---------------------------------------------------------------------------
// CTelBubbleImageScaler::GetState
// ---------------------------------------------------------------------------
//
TInt CTelBubbleImageScaler::GetState()
    {
    BM_TRACE_1( "CTelBubbleImageScaler::GetState(): %d", iState );
    return iState;
    }
