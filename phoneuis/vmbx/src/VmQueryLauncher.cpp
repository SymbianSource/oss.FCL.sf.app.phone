/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Query launcher for Voice mailbox application.
*
*/


// INCLUDE FILES
#include <bldvariant.hrh>
#include <e32std.h>
#include <featmgr.h>
#include "VmQueryLauncher.h"
#include "VmContainer.h"
#include "vmnumber.h"
#include "VMBLogger.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can't leave
// -----------------------------------------------------------------------------
// CVmQueryLauncher::CVmQueryLauncher
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVmQueryLauncher::CVmQueryLauncher( CVmContainer& aContainer, 
                                    MVmQueryLauncher& aObserver,
                                                    TInt aPriority )
                        : CActive( aPriority ), iContainer( aContainer ),
                          iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CVmQueryLauncher::~CVmQueryLauncher
// Destructor
// -----------------------------------------------------------------------------
//
CVmQueryLauncher::~CVmQueryLauncher()
    {
    }

// ----------------------------------------------------------------------------
// CVmQueryLauncher::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVmQueryLauncher* CVmQueryLauncher::NewL( CVmContainer& aContainer,
                                 MVmQueryLauncher& aObserver, TInt aPriority)
    {
    CVmQueryLauncher* self = new( ELeave ) CVmQueryLauncher( aContainer,
                                                    aObserver, aPriority );
    CActiveScheduler::Add( self );
    self->SetActive();
    TRequestStatus* status = &(self->iStatus);

    // The idea is to activate this object as soon as the application has
    // been fully constructed.
    User::RequestComplete( status, KErrNone );

    return self;
    }

// ----------------------------------------------------------------------------
// CVmQueryLauncher::RunL
// Handles an active object's request completion event
// ----------------------------------------------------------------------------
//
void CVmQueryLauncher::RunL()
    {
    VMBLOGSTRING( "VMBX: CVmQueryLauncher::RunL: =>" );
    if ( iStatus == KErrNone )
        {
        TInt type( EVmbx );

        type = iContainer.AskTypeL();

        if ( type == EVmbx )         //if CS vmbx is selected
            {
            iContainer.QueryNumberL( EVmbxNotDefinedQuery );
            }
        else if ( type == EVmbxVideo )
            {
            iContainer.QueryVideoNumberL( EVmbxNotDefinedQuery );
            }
        }
    iObserver.NumberQueryComplete();
    VMBLOGSTRING( "VMBX: CVmQueryLauncher::RunL: <=" );
    }

// ----------------------------------------------------------------------------
// CVmQueryLauncher::DoCancel
// Implements cancellation of an outstanding request
// ----------------------------------------------------------------------------
//
void CVmQueryLauncher::DoCancel()
    {
    VMBLOGSTRING( "VMBX: CVmQueryLauncher::DoCancel: =>" );
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrCancel );
    VMBLOGSTRING( "VMBX: CVmQueryLauncher::DoCancel: <=" );
    }

//  End of File
