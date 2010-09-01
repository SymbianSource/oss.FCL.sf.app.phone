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
* Description:  Base class for contactlink array fetchers.
*
*/


#include <MVPbkContactLinkArray.h>

#include "cphcntcontactlinkarrayfetch.h"
#include <talogger.h>
// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhCntContactLinkArrayFetch::CPhCntContactLinkArrayFetch()
    {

    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhCntContactLinkArrayFetch::~CPhCntContactLinkArrayFetch()
    {
    delete iFetchedContactLinks;
    }
    
// ---------------------------------------------------------------------------
// Secondphase constructor.
// ---------------------------------------------------------------------------
//
void CPhCntContactLinkArrayFetch::BaseConstructL() 
    {
    CPhCntAsyncToSync::BaseConstructL();
    }

// ---------------------------------------------------------------------------
// From class MVPbkContactFindObserver
// Notifies the base class that response from asynchronous request
// has been received succesfully.
// ---------------------------------------------------------------------------
//
void CPhCntContactLinkArrayFetch::FindCompleteL( MVPbkContactLinkArray* aResults )
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntContactLinkArrayFetch::FindCompleteL" );
    delete iFetchedContactLinks;
    iFetchedContactLinks = aResults;
    // Indicate base class that response from fetch has been received.
    ResponseReceived( KErrNone );
    }
    
// ---------------------------------------------------------------------------
// From class MVPbkContactFindObserver
// Notifies the base class that response from asynchronous request
// has been received succesfully.
// ---------------------------------------------------------------------------
//
void CPhCntContactLinkArrayFetch::FindFailed( TInt aError )
    {
    TEFLOGSTRING( KTAOBJECT, "CNT CPhCntContactLinkArrayFetch::FindFailed" );
    delete iFetchedContactLinks;
    iFetchedContactLinks = NULL;
    // Indicate base class that response from fetch has been received.
    ResponseReceived( aError );
    }
