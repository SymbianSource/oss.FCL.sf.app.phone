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
* Description:  Finds voip contacts from phonebook
*
*/


#include <MVPbkContactOperationBase.h>

#include "cphcntvoipcontactmatchstrategy.h"
#include "mphcntvoipcontactfinder.h"
#include "PhCntPanic.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntVoipContactMatchStrategy::CPhCntVoipContactMatchStrategy(
    MPhCntVoipContactFinder& aVoipContactFinder,
    MVPbkContactFindObserver& aObserver ) :
    iVoipContactFinder( aVoipContactFinder ),
    iObserver( aObserver )
    {
    }


// ---------------------------------------------------------------------------
// Secondphase constructor
// ---------------------------------------------------------------------------
//
void CPhCntVoipContactMatchStrategy::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntVoipContactMatchStrategy* CPhCntVoipContactMatchStrategy::NewL(
    MPhCntVoipContactFinder& aVoipContactFinder,
    MVPbkContactFindObserver& aObserver )
    {
    CPhCntVoipContactMatchStrategy* self = 
        CPhCntVoipContactMatchStrategy::NewLC( aVoipContactFinder, aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntVoipContactMatchStrategy* CPhCntVoipContactMatchStrategy::NewLC(
    MPhCntVoipContactFinder& aVoipContactFinder,
    MVPbkContactFindObserver& aObserver )
    {
    CPhCntVoipContactMatchStrategy* self = 
        new( ELeave ) CPhCntVoipContactMatchStrategy( 
            aVoipContactFinder, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntVoipContactMatchStrategy::~CPhCntVoipContactMatchStrategy()
    {
    delete iFindOperation;
    }

// ---------------------------------------------------------------------------
// From class MPhCntContactMatchStrategy
// Starts the find
// ---------------------------------------------------------------------------
//
 void CPhCntVoipContactMatchStrategy::FindMatchesL( 
    const TDesC& aMatchString )
    {
    __ASSERT_ALWAYS( !iFindOperation, 
        PhCntPanic( EPhCntOperationAlreadyOngoing ) );
    iFindOperation = 
        iVoipContactFinder.FindVoipContactsL( aMatchString, *this );
    }
 
// ---------------------------------------------------------------------------
// From class MVPbkContactFindObserver
// Notification that find is done succesfully
// ---------------------------------------------------------------------------
//   
 void CPhCntVoipContactMatchStrategy::FindCompleteL( 
    MVPbkContactLinkArray* aResults )
    {
    delete iFindOperation;
    iFindOperation = NULL;
    iObserver.FindCompleteL( aResults );
    }

// ---------------------------------------------------------------------------
// From class MVPbkContactFindObserver
// Notification that find failed
// ---------------------------------------------------------------------------
//     
void CPhCntVoipContactMatchStrategy::FindFailed( 
    TInt aError )
    {
    delete iFindOperation;
    iFindOperation = NULL;
    iObserver.FindFailed( aError );
    }

